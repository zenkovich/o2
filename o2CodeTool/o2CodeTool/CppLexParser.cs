using System;
using System.Collections.Generic;
using System.Linq;

class CppLexParser
{
	public SourceFile source;
	bool isNextLexTemplate = false;
	string templatesBuffer = "";

	delegate void ParserFunc(LexSection section, ref int caret, ref ProtectLevel protectionLevel);

	Dictionary<string, ParserFunc> globalKeyParsers = new Dictionary<string, ParserFunc>();
	Dictionary<string, ParserFunc> classBodyKeyParsers = new Dictionary<string, ParserFunc>();


	public CppLexParser()
	{
		globalKeyParsers.Add("/*", ParseMultiLineComment);
		globalKeyParsers.Add("#pragma", ParsePragma);
		globalKeyParsers.Add("#include", ParseInclude);
		globalKeyParsers.Add("#define", ParseDefine);
		globalKeyParsers.Add("#if", ParseIfMacro);
		globalKeyParsers.Add("//", ParseSingleLineComment);
		globalKeyParsers.Add("class", ParseClass);
		globalKeyParsers.Add("struct", ParseStruct);
		globalKeyParsers.Add("namespace", ParseNamespace);
		globalKeyParsers.Add("template", ParseTemplate);
		globalKeyParsers.Add("typedef", ParseTypedef);
		globalKeyParsers.Add("enum", ParseEnum);
		globalKeyParsers.Add("using", ParseUsing);

		classBodyKeyParsers.Add("/*", ParseMultiLineComment);
		classBodyKeyParsers.Add("//", ParseSingleLineComment);
		classBodyKeyParsers.Add("class", ParseClass);
		classBodyKeyParsers.Add("struct", ParseStruct);
		classBodyKeyParsers.Add("public:", ParseClassPublic);
		classBodyKeyParsers.Add("private:", ParseClassPrivate);
		classBodyKeyParsers.Add("protected:", ParseClassProtected);
		classBodyKeyParsers.Add("template", ParseTemplate);
		classBodyKeyParsers.Add("typedef", ParseTypedef);
		classBodyKeyParsers.Add("friend", ParseFriend);
		classBodyKeyParsers.Add("enum", ParseEnum);
		classBodyKeyParsers.Add("#if", ParseIfMacro);
	}

	public void Parse(SourceFile source)
	{
		ProcessSlashLine(source);

		this.source = source;
		source.globalNamespace = new LexNamespace()
		{
			begin = 0,
			end = source.data.Length,
			data = source.data,
			name = ""
		};

		ParseLexSection(source.globalNamespace, ProtectLevel.Public);
		isNextLexTemplate = false;
	}

	void CopyNamespace(LexNamespace source, LexNamespace target)
	{
		target.name = source.name;

		target.classes.AddRange(source.classes);
		target.childSections.AddRange(source.classes);
		target.functions.AddRange(source.functions);
		target.variables.AddRange(source.variables);
		target.unknownBlocks.AddRange(source.unknownBlocks);
		target.typedefs.AddRange(source.typedefs);
		target.enums.AddRange(source.enums);

		source.usingNamespaces.ForEach(x =>
		{
			if (target.usingNamespaces.Find(y => y.name == x.name) == null)
				target.usingNamespaces.Add(x);
		});

		target.classes.ForEach(x => x.parentLexSection = target);

		foreach (var chSource in source.childSections)
		{
			if (chSource.GetType() != typeof(LexNamespace))
				continue;

			var chTarget = target.childSections.Find(x =>
			{
				return x.GetType() == typeof(LexNamespace) && (x as LexNamespace).name == (chSource as LexNamespace).name;
			});

			if (chTarget == null)
			{
				chTarget = new LexNamespace();
				(chTarget as LexNamespace).name = (chSource as LexNamespace).name;
				target.childSections.Add(chTarget);
			}

			chTarget.parentLexSection = target;

			CopyNamespace(chSource as LexNamespace, chTarget as LexNamespace);
		}
	}

	public void Parse(SrcMap srcMap)
	{
		srcMap.headers.ForEach(x => Parse(x));

		srcMap.globalNamespace = new LexNamespace();

		foreach (var header in srcMap.headers)
			CopyNamespace(header.globalNamespace, srcMap.globalNamespace);

		BindUsingNamespaces(srcMap.globalNamespace);

		ProcessTypedefs(srcMap.globalNamespace);

		srcMap.allClasses = GetAllClasses(srcMap.globalNamespace);

		foreach (var cls in srcMap.allClasses)
		{
			if (cls.parentLexSection != null && cls.parentLexSection.GetType() == typeof(LexClass) && (cls.parentLexSection as LexClass).isTemplate)
				cls.isTemplate = true;
		}

		Console.Write("Starting specializing templates... " + DateTime.Now + "\n");

		SpecializeTemplates(srcMap);

		foreach (var cls in srcMap.allClasses)
		{
			foreach (var baseCls in cls.baseClasses)
			{
				baseCls.lexClass = cls.parentLexSection.FindLexSection(baseCls.className) as LexClass;
			}
		}

		BindComments(srcMap.globalNamespace);
	}

	void ProcessTypedefs(LexSection section)
	{
		foreach (var tpd in section.typedefs)
		{
			LexClass defClass = section.FindLexSection(tpd.value) as LexClass;
			if (defClass == null)
				continue;

			LexClass newClass = defClass.Clone();
			string oldName = newClass.shortName;
			newClass.shortName = tpd.name;
			RenameClass(newClass, newClass.name, section.name + "::" + newClass.shortName);
			section.classes.Add(newClass);
			section.childSections.Add(newClass);
		}

		section.childSections.ForEach(x => ProcessTypedefs(x));
	}

	void RenameClass(LexClass cls, string oldName, string newName)
	{
		cls.isTypedef = true;

		int pos = cls.name.IndexOf(oldName);
		if (pos >= 0)
			cls.name = cls.name.Remove(pos, oldName.Length).Insert(pos, newName);

		cls.classes.ForEach(x => RenameClass(x, oldName, newName));
	}

	void SpecializeTemplates(SrcMap srcMap)
	{
		List<LexClass> newClasses = new List<LexClass>();
		foreach (var cls in srcMap.allClasses)
		{
			if (!cls.isTemplate)
				continue;

			if (cls.shortName[cls.shortName.Length - 1] == '>')
				continue;

			List<string> specializations = new List<string>();

			foreach (var src in srcMap.headers)
				SearchSpecializations(specializations, src, cls);

			/*foreach (var src in srcMap.sources)
				SearchSpecializations(specializations, src, cls);*/

			//Console.Write("Found " + specializations.Count() + " specializations for " + cls.name + "\n");

			foreach (var spec in specializations)
			{
				LexClass newClass = cls.Clone();
				newClass.shortName += "<" + spec + ">";
				newClass.isTemplate = false;
				SpecializeClassName(newClass, spec, cls.shortName, newClasses, srcMap.allClasses);
				cls.parentLexSection.classes.Add(newClass);
				cls.parentLexSection.childSections.Add(newClass);
			}
		}

		newClasses.ForEach(x => srcMap.allClasses.Add(x));
	}

	void SpecializeClassName(LexClass cls, string specialize, string tempClassName, List<LexClass> newClasses, List<LexClass> allClasses)
	{
		cls.isTemplate = false;
		int pos = cls.name.IndexOf(tempClassName);
		if (pos >= 0)
		{
			pos += tempClassName.Length;
			cls.name = cls.name.Insert(pos, "<" + specialize + ">");
		}

		string[] templates = new string[0];
		if (cls.templates != null)
			templates = cls.templates.Split(',');

		for (int i = 0; i < templates.Count(); i++)
		{
			int x = templates[i].IndexOf("typename");
			if (x >= 0)
				templates[i] = templates[i].Remove(x, "typename".Length).Trim(' ');
		}

		for (int i = 0; i < cls.functions.Count(); i++)
		{
			var func = cls.functions[i];

			if (func.returnType.definition != null)
			{
				foreach (var templ in templates)
					func.returnType.definition = func.returnType.definition.Replace(templ, specialize);
			}

			foreach (var param in func.parameters)
			{
				foreach (var templ in templates)
					param.type.definition = param.type.definition.Replace(templ, specialize);
			}
		}

		var fnd = allClasses.Find(x => x.name == cls.name);
		if (fnd != null)
			fnd.isTemplate = false;
		else
			newClasses.Add(cls);

		cls.classes.ForEach(x => SpecializeClassName(x, specialize, tempClassName, newClasses, allClasses));
	}

	void SearchSpecializations(List<string> specializations, SourceFile src, LexClass cls)
	{
		int caret = 0;
		while (caret < src.data.Length)
		{
			caret = src.data.IndexOf(cls.shortName, caret);

			if (caret < 0)
				break;

			caret += cls.shortName.Length;

			string spec = ReadWord(src.data, ref caret).Trim(' ', '\n', '\t', '\r');
			if (spec.Length > 0 && spec[0] == '<' && spec[spec.Length - 1] == '>')
			{
				string sspec = spec.Substring(1, spec.Length - 2).Trim(' ', '\n', '\t', '\r');
				if (!specializations.Contains(sspec) && !cls.templates.Contains(sspec))
					specializations.Add(sspec);
			}
		}
	}

	void BindUsingNamespaces(LexSection section)
	{
		section.usingNamespaces.ForEach(x => x.nspace = section.FindLexSection(x.name) as LexNamespace);
		section.childSections.ForEach(x => BindUsingNamespaces(x));
	}

	void BindComments(LexSection section)
	{
		section.functions.ForEach(x => x.comment = section.FindComment(x.begin, x.end));
		section.variables.ForEach(x => x.comment = section.FindComment(x.begin, x.end));
		section.unknownBlocks.ForEach(x => x.comment = section.FindComment(x.begin, x.end));
		section.childSections.ForEach(x => BindComments(x));
	}

	List<LexClass> GetAllClasses(LexSection section)
	{
		List<LexClass> res = new List<LexClass>(section.classes.FindAll(x => x.haveBody));
		section.childSections.ForEach(x => res.AddRange(GetAllClasses(x)));

		return res;
	}

	void ParseLexSection(LexSection section, ProtectLevel protectionLevel)
	{
		section.source = source;

		string skipSymbols = " \r\n\t";
		int dataLen = section.data.Length;
		int caret = 0;

		for (caret = 0; caret < dataLen; caret++)
		{
			if (skipSymbols.Contains(section.data[caret]))
				continue;

			Dictionary<string, ParserFunc> keyParsers = globalKeyParsers;

			if (section.GetType() == typeof(LexClass))
				keyParsers = classBodyKeyParsers;

			bool passedKeyWord = false;
			foreach (var keyWordParser in keyParsers)
			{
				string sub = section.data.Substring(caret, Math.Min(keyWordParser.Key.Length, dataLen - caret));

				if (sub == keyWordParser.Key)
				{
					keyWordParser.Value(section, ref caret, ref protectionLevel);
					passedKeyWord = true;
					break;
				}
			}

			if (passedKeyWord)
				continue;

			int blockbegin = caret;
			string block = ReadBlock(section.data, ref caret);
			block = block.Trim(' ', '\r', '\t', '\n');

			if (block.Length > 0)
				TryParseBlock(section, block, blockbegin, ref caret, ref protectionLevel);
		}

		section.functions.ForEach(x => x.comment = section.FindComment(x.begin, x.end));
		section.variables.ForEach(x => x.comment = section.FindComment(x.begin, x.end));
		section.unknownBlocks.ForEach(x => x.comment = section.FindComment(x.begin, x.end));
	}

	void TryParseBlock(LexSection section, string block, int blockBegin, ref int caret, ref ProtectLevel protectionLevel)
	{
		int locCaret = 0;
		bool isFunction = false;

		string firstWord = ReadWord(block, ref locCaret, " \n\r(){}[]");

		if (firstWord.Length == 0)
		{
			LexUnknown unkn = new LexUnknown()
			{
				begin = blockBegin,
				end = caret,
				data = block,
				protectLevel = protectionLevel,
				value = block
			};

			section.unknownBlocks.Add(unkn);
			return;
		}

		if (firstWord == "virtual")
			firstWord = ReadWord(block, ref locCaret, " \n\r(){}[]");

		if (firstWord == "static")
			firstWord = ReadWord(block, ref locCaret, " \n\r(){}[]");

		if (firstWord == "typename")
			firstWord = ReadWord(block, ref locCaret, " \n\r(){}[]");

		if (firstWord == "inline")
			firstWord = ReadWord(block, ref locCaret, " \n\r(){}[]");

		if (GetNextSymbol(block, locCaret, " \n\r\t") == '(')
		{
			string braces = ReadBraces(block, ref locCaret).Trim(' ', '\n', '\t', '\r', '(', ')');
			int tmpCaret = 0;
			string word = ReadWord(braces, ref tmpCaret);

			isFunction = GetNextSymbol(braces, tmpCaret, " \n\r\t") != ':';

			if (!isFunction && braces.StartsWith("std"))
				isFunction = true;
		}
		else
		{
			if (firstWord == "const")
				ReadWord(block, ref locCaret, " \n\r(){}[]");

			string thirdWord = ReadWord(block, ref locCaret, " \n\r(){}[]");

			if (thirdWord == "operator")
				thirdWord = ReadWord(block, ref locCaret, " \n\r(){}");

			if (GetNextSymbol(block, locCaret, " \n\r\t") == '(')
				isFunction = true;
		}

		if (isFunction)
			section.functions.Add(ParseFunction(block, protectionLevel, blockBegin, caret));
		else
			section.variables.Add(ParseVariable(block, protectionLevel, blockBegin, caret));
	}

	LexVariable ParseVariable(string data, ProtectLevel protectLevel, int begin, int end)
	{
		LexVariable res = new LexVariable()
		{
			begin = begin,
			end = end,
			data = data
		};

		res.type = new LexVariableType();

		res.definition = data;

		int caret = 0;
		string typeWord = ReadWord(data, ref caret, " \n\r(){}[]");
		string typeDefinition = typeWord;

		if (typeWord == "static")
		{
			typeWord = ReadWord(data, ref caret, " \n\r(){}[]");
			typeDefinition += " " + typeWord;
			res.isStatic = true;
		}

		if (typeWord == "const")
		{
			typeWord = ReadWord(data, ref caret, " \n\r(){}[]");
			res.type.isContstant = true;

			typeDefinition += " " + typeWord;
		}

		if (typeWord.Last() == '&')
			res.type.type = LexVariableType.Type.Reference;

		if (typeWord.Last() == '*')
			res.type.type = LexVariableType.Type.Pointer;

		res.type.name = typeWord;
		res.type.definition = typeDefinition;

		res.protectLevel = protectLevel;

		if (GetNextSymbol(data, caret, " \n\r\t") == '(')
		{
			string braces = ReadBraces(data, ref caret).Trim(' ', '\r', '\t', '\t', '(', ')');
			string nexBraces = ReadBraces(data, ref caret).Trim(' ', '\r', '\t', '\t', '(', ')');

			int tmpCaret = 0;
			string bracesFirst = ReadWord(braces, ref tmpCaret);
			tmpCaret += 3;
			res.name = braces.Substring(tmpCaret);

			res.type.name = res.type.name + " (" + bracesFirst + "*)(" + nexBraces + ")";
			res.type.definition = res.type.name;
		}
		else
		{
			res.name = ReadWord(data, ref caret, " \n\r(){}[]");
		}

		return res;
	}

	LexFunction ParseFunction(string data, ProtectLevel protectLevel, int begin, int end)
	{
		LexFunction res = new LexFunction()
		{
			begin = begin,
			end = end,
			data = data
		};

		if (isNextLexTemplate)
		{
			res.isTemplate = true;
			res.templates = templatesBuffer;
			isNextLexTemplate = false;
		}

		res.returnType = new LexVariableType();

		res.definition = data;
		res.protectLevel = protectLevel;

		int caret = 0;
		string typeWord = ReadWord(data, ref caret, " \n\r(){}[]");

		if (typeWord == "virtual")
		{
			res.isVirtual = true;
			typeWord = ReadWord(data, ref caret, " \n\r(){}[]");
		}

		if (typeWord == "static")
		{
			res.isStatic = true;
			typeWord = ReadWord(data, ref caret, " \n\r(){}[]");
		}

		if (typeWord == "inline")
			typeWord = ReadWord(data, ref caret, " \n\r(){}[]");

		if (typeWord == "typename")
			typeWord = ReadWord(data, ref caret, " \n\r(){}[]");

		if (typeWord == "explicit")
			typeWord = ReadWord(data, ref caret, " \n\r(){}[]");

		if (typeWord == "operator")
		{
			string nextWord = ReadWord(data, ref caret, " \n\r(){}[]");
			res.name = typeWord + nextWord;
			res.returnType.name = "void";
		}
		else
		{
			if (GetNextSymbol(data, caret, " \n\r\t") == '(')
			{
				res.name = typeWord;
				res.returnType.name = "void";
			}
			else
			{
				string typeDefinition = typeWord;

				if (typeWord == "const")
				{
					typeWord = ReadWord(data, ref caret, " \n\r(){}[]");
					res.returnType.isContstant = true;

					typeDefinition += " " + typeWord;
				}

				if (typeWord.Last() == '&')
					res.returnType.type = LexVariableType.Type.Reference;

				if (typeWord.Last() == '*')
					res.returnType.type = LexVariableType.Type.Pointer;

				res.returnType.name = typeWord;
				res.returnType.definition = typeDefinition;

				res.name = ReadWord(data, ref caret, " \n\r(){}[]");

				if (res.name == "operator")
					res.name += " " + ReadWord(data, ref caret, " \n\r(){}");
			}
		}

		string paramsStr = ReadBraces(data, ref caret).Trim(' ', '\n', '\r', '\t');
		string afterParamWord = ReadWord(data, ref caret);

		if (afterParamWord == "const")
			res.isContstant = true;

		if (paramsStr.Length > 0)
		{
			var paramsArr = Split(paramsStr, ',');
			foreach (var prm in paramsArr)
			{
				string trimmedParam = prm.Trim(' ', '\r', '\n', '\t');
				res.parameters.Add(ParseVariable(trimmedParam, ProtectLevel.Public, begin, end));
			}
		}

		return res;
	}

	void ParseUsing(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "using".Length;

		ReadWord(section.data, ref caret);

		string name = ReadWord(section.data, ref caret);
		section.usingNamespaces.Add(new LexUsingNamespace()
		{
			begin = begin,
			end = caret,
			data = section.data.Substring(begin, caret - begin),
			name = name
		});
	}

	void ParseEnum(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "enum".Length;

		string name = ReadWord(section.data, ref caret);

		if (name == "class")
			name = ReadWord(section.data, ref caret);

		string block = ReadBlock(section.data, ref caret).Trim(' ', '{', '}', '\r', '\t', '\n');
		string blockWithoutComments = RemoveComments(block);
		var content = Split(blockWithoutComments, ',');

		LexEnum newEnum = new LexEnum()
		{
			begin = begin,
			end = caret,
			data = section.data.Substring(begin, caret - begin),
			name = name
		};

		foreach (var x in content)
		{
			string xc = x.Trim(' ', '\n', '\t', '\r');

			if (xc.IndexOf('=') >= 0)
				xc = xc.Substring(0, xc.IndexOf('=')).Trim(' ', '\n', '\t', '\r');

			newEnum.content.Add(xc);
		}

		section.enums.Add(newEnum);
	}

	void ParseNamespace(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "namespace".Length;

		string namespaceName = ReadWord(section.data, ref caret);
		int namespaceBegin = section.data.IndexOf('{', caret) + 1;
		string block = ReadBlock(section.data, ref caret).Trim(' ', '\r', '\t', '\n');
		string body = block.Substring(1, block.Length - 2).Trim(' ', '\r', '\t', '\n');

		LexNamespace newNamespace = new LexNamespace()
		{
			begin = begin,
			end = caret,
			data = body,
			name = namespaceName,
			source = source,
			parentLexSection = section
		};
		section.childSections.Add(newNamespace);

		ParseLexSection(newNamespace, ProtectLevel.Public);
	}

	void ParseClass(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		ParseClassOrStruct(section, ref caret, ref protectionLevel, false);
	}

	void ParseStruct(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		ParseClassOrStruct(section, ref caret, ref protectionLevel, true);
	}

	void ParseClassOrStruct(LexSection section, ref int caret, ref ProtectLevel protectionLevel, bool isstruct)
	{
		int begin = caret;

		if (isstruct) caret += "struct".Length;
		else caret += "class".Length;

		string className = ReadWord(section.data, ref caret, " \n\t\r:;/");
		string afterName = ReadWord(section.data, ref caret, ";{/").Trim(' ', ':', '\r', '\n', '\t');

		string shortClassName = className;

		if (section.GetType() == typeof(LexClass))
			className = (section as LexClass).name + "::" + className;
		else
			className = (section as LexNamespace).name + "::" + className;

		LexClass newClass = new LexClass()
		{
			begin = begin,
			end = caret,
			data = section.data.Substring(begin, caret - begin),
			name = className,
			shortName = shortClassName,
			source = source,
			parentLexSection = section,
			protectLevel = protectionLevel,
			haveBody = false,
			isTemplate = isNextLexTemplate
		};

		if (isNextLexTemplate)
			newClass.templates = templatesBuffer;

		isNextLexTemplate = false;

		if (afterName.Length > 0)
		{
			var baseClasses = Split(afterName, ',');

			foreach (var baseClass in baseClasses)
			{
				string trimmedBaseClass = baseClass.Trim();

				int spacePos = trimmedBaseClass.IndexOf(' ');
				if (spacePos < 0)
				{
					newClass.baseClasses.Add(new LexClass.BaseClassDef()
					{
						type = ProtectLevel.Private,
						className = trimmedBaseClass
					});
				}
				else
				{
					string sectionTypeName = trimmedBaseClass.Substring(0, spacePos);
					string baseClassName = trimmedBaseClass.Substring(spacePos + 1);

					if (baseClassName.StartsWith("virtual"))
						baseClassName = baseClassName.Substring("virtual".Length + 1);

					ProtectLevel sectionType = ProtectLevel.Private;

					if (sectionTypeName == "public")
						sectionType = ProtectLevel.Public;
					else if (sectionTypeName == "protected")
						sectionType = ProtectLevel.Protected;

					newClass.baseClasses.Add(new LexClass.BaseClassDef()
					{
						type = sectionType,
						className = baseClassName
					});
				}
			}
		}

		if (caret < section.data.Length && section.data[caret] == '/')
		{
			string comment = ReadWord(section.data, ref caret, "\n");
			ReadWord(section.data, ref caret, ";{/");

			newClass.comment = new LexComment() { comment = comment };
		}

		if (caret < section.data.Length && section.data[caret] == '{')
		{
			int sectionBegin = caret;
			newClass.data = ReadBlock(section.data, ref caret).Trim('{', '}', ' ', '\n', '\r', '\t');
			newClass.haveBody = true;

			section.classes.Add(newClass);
			section.childSections.Add(newClass);

			ParseLexSection(newClass, isstruct ? ProtectLevel.Public : ProtectLevel.Private);
		}
	}

	void ParseClassPublic(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		caret += "public:".Length;
		protectionLevel = ProtectLevel.Public;
	}

	void ParseClassPrivate(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		caret += "private:".Length;
		protectionLevel = ProtectLevel.Private;
	}

	void ParseClassProtected(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		caret += "protected:".Length;
		protectionLevel = ProtectLevel.Protected;
	}

	void ParseTemplate(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		caret += "template".Length;

		for (; caret < section.data.Length; caret++)
			if (section.data[caret] == '<')
				break;

		caret++;
		int braces = 1;
		int begin = caret;

		for (; caret < section.data.Length; caret++)
		{
			if (section.data[caret] == '<')
				braces++;

			if (section.data[caret] == '>')
			{
				braces--;

				if (braces == 0)
					break;
			}
		}

		string tempInside = section.data.Substring(begin, caret - begin);

		isNextLexTemplate = true;
		templatesBuffer = tempInside;
	}

	void ParseSingleLineComment(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "//".Length;
		string comment = ReadWord(section.data, ref caret, "\n", "").Trim(' ', '\r');
		section.comments.Add(new LexComment()
		{
			begin = begin,
			end = caret,
			data = section.data.Substring(begin, caret - begin),
			comment = comment,
			source = source
		});
	}

	void ParseMultiLineComment(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "/*".Length;
		int end = section.data.IndexOf("*/", caret);
		caret = end;
		section.comments.Add(new LexComment()
		{
			begin = begin,
			end = end,
			data = section.data.Substring(begin, caret - begin),
			comment = section.data.Substring(begin + 2, end - begin - 4).Trim(' ', '\r', '\t', '\n'),
			source = source
		});
	}

	void ParseTypedef(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "typedef".Length;
		string value = ReadWord(section.data, ref caret, " \n\r\t").Trim(' ', '\r', '\t', '\n');
		string name = ReadWord(section.data, ref caret, " \n\r\t").Trim(' ', '\r', '\t', '\n', ';');
		section.typedefs.Add(new LexTypedef()
		{
			begin = begin,
			end = caret,
			data = section.data.Substring(begin, caret - begin),
			name = name,
			value = value,
			source = source
		});
	}


	void ParseFriend(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "friend".Length;
		string name = ReadWord(section.data, ref caret, " \n\r\t").Trim(' ', '\r', '\t', '\n');
		string value = ReadWord(section.data, ref caret, " \n\r\t").Trim(' ', '\r', '\t', '\n');
	}


	void ParsePragma(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "#pragma".Length;
		string word = ReadWord(section.data, ref caret);
		source.pragmas.Add(new LexPragma()
		{
			begin = begin,
			end = caret,
			data = section.data.Substring(begin, caret - begin),
			pragmaWord = word,
			source = source
		});
	}

	void ParseDefine(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "#define".Length;

		string definition = ReadWord(section.data, ref caret);
		string next = ReadWord(section.data, ref caret, "\n").Trim(' ', '\r', '\t');

		source.defines.Add(new LexDefine()
		{
			begin = begin,
			end = caret,
			data = section.data.Substring(begin, caret - begin),
			definition = definition,
			content = next,
			source = source
		});
	}

	void ParseIfMacro(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "#if".Length;

		string skipSymbols = " \r\n\t";
		string endWord = "#endif";

		int dataLen = section.data.Length;
		for (; caret < dataLen; caret++)
		{
			if (skipSymbols.Contains(section.data[caret]))
				continue;

			string sub = section.data.Substring(caret, Math.Min(endWord.Length, dataLen - caret));
			if (sub == endWord)
			{
				caret = Math.Min(dataLen, caret + endWord.Length);
				return;
			}
		}
	}

	void ParseInclude(LexSection section, ref int caret, ref ProtectLevel protectionLevel)
	{
		int begin = caret;
		caret += "#include".Length;
		string word = ReadWord(section.data, ref caret, "\n").Trim(' ', '\r', '"');
		source.includes.Add(new LexInclude()
		{
			begin = begin,
			end = caret,
			data = section.data.Substring(begin, caret - begin),
			includeFile = word,
			source = source
		});
	}

	string ReadWord(string data, ref int caret,
					string breakSymbols = " \n\r(){}.,;+-*/=@!|&*:~\\",
					string skipSymbols = " \n\r")
	{
		string res = "";
		int braces = 0, sqBraces = 0, trBraces = 0, fgBraces = 0;

		for (; caret < data.Length; caret++)
		{
			if (!skipSymbols.Contains(data[caret]))
				break;
		}

		for (; caret < data.Length; caret++)
		{

			if (breakSymbols.Contains(data[caret]) && fgBraces == 0 && braces == 0 && sqBraces == 0 && trBraces == 0)
				break;

			switch (data[caret])
			{
				case '{': fgBraces++; break;
				case '}': fgBraces--; break;

				case ')': braces--; break;
				case '(': braces++; break;

				case '[': sqBraces++; break;
				case ']': sqBraces--; break;

				case '<': trBraces++; break;
				case '>': trBraces--; break;
			}

			res += data[caret];
		}

		return res;
	}

	string ReadBlock(string data, ref int caret)
	{
		int begin = caret;
		int braces = 0, fgBraces = 0, sqBraces = 0, trBraces = 0;
		bool isInString = false;

		for (; caret < data.Length; caret++)
		{
			if (data[caret] == '{')
				break;

			if (data[caret] == ';')
				return data.Substring(begin, caret - begin);
		}

		caret++;
		fgBraces++;

		bool complete = false;
		for (; caret < data.Length && !complete; caret++)
		{
			if (isInString)
			{
				if (data[caret] == '"' && data[caret - 1] != '\\')
					isInString = false;

				continue;
			}

			switch (data[caret])
			{
				case '{': fgBraces++; break;
				case '}':
				fgBraces--;
				if (fgBraces == 0 && braces == 0 && sqBraces == 0)
					complete = true;
				break;

				case ')': braces--; break;
				case '(': braces++; break;

				case '[': sqBraces++; break;
				case ']': sqBraces--; break;

				case '<': trBraces++; break;
				case '>': trBraces--; break;

				case '"':
				isInString = true;
				break;
			}
		}

		caret--;

		return data.Substring(begin, Math.Min(caret, data.Length - 1) - begin + 1);
	}
	string ReadBraces(string data, ref int caret)
	{
		int begin = caret;
		int braces = 0, fgBraces = 0, sqBraces = 0, trBraces = 0;
		bool isInString = false;

		for (; caret < data.Length; caret++)
		{
			if (data[caret] == '(')
				break;

			if (data[caret] == ';')
				return data.Substring(begin, caret - begin);
		}

		caret++;
		braces++;

		bool complete = false;
		for (; caret < data.Length && !complete; caret++)
		{
			if (isInString)
			{
				if (data[caret] == '"' && data[caret - 1] != '\\')
					isInString = false;

				continue;
			}

			switch (data[caret])
			{
				case '}': fgBraces--; break;
				case '{': fgBraces++; break;

				case '[': sqBraces++; break;
				case ']': sqBraces--; break;

				case '<': trBraces++; break;
				case '>': trBraces--; break;

				case '(': braces++; break;
				case ')':
				braces--;
				if (fgBraces == 0 && braces == 0 && sqBraces == 0 && trBraces == 0)
					complete = true;
				break;

				case '"':
				isInString = true;
				break;
			}
		}

		string res = data.Substring(begin, Math.Min(caret, data.Length) - begin);
		if (res.Last() == ')')
			res = res.Remove(res.Length - 1, 1);

		if (res.First() == '(')
			res = res.Remove(0, 1);

		return res;
	}

	char GetNextSymbol(string data, int begin, string skipSymbols = " \n\r\t()[]{}")
	{
		for (; begin < data.Length; begin++)
			if (!skipSymbols.Contains(data[begin]))
				return data[begin];

		return '\0';
	}

	List<string> Split(string data, char splitSymbol)
	{
		List<string> res = new List<string>();
		int braces = 0, sqBraces = 0, trBraces = 0, fgBraces = 0;

		int lastSplit = 0;
		for (int i = 0; i < data.Length; i++)
		{
			switch (data[i])
			{
				case '{': fgBraces++; break;
				case '}': fgBraces--; break;

				case '(': braces++; break;
				case ')': braces--; break;

				case '<': trBraces++; break;
				case '>': trBraces--; break;

				case '[': sqBraces++; break;
				case ']': sqBraces--; break;
			}

			if (braces == 0 && sqBraces == 0 && trBraces == 0 && fgBraces == 0 && data[i] == splitSymbol)
			{
				res.Add(data.Substring(lastSplit, i - lastSplit));
				lastSplit = i + 1;
			}
		}
		res.Add(data.Substring(lastSplit, data.Length - lastSplit));

		return res;
	}

	string RemoveComments(string input)
	{
		string res = "";
		int len = input.Length;

		for (int i = 0; i < len; i++)
		{
			if (i > 0)
			{
				if (input[i - 1] == '/' && input[i] == '/')
				{
					res = res.Remove(res.Length - 1, 1);
					for (; i < len; i++)
					{
						if (input[i] == '\n')
							break;
					}
				}

				if (input[i - 1] == '/' && input[i] == '*')
				{
					res = res.Remove(res.Length - 1, 1);
					for (; i < len; i++)
					{
						if (input[i - 1] == '*' && input[i] == '/')
							break;
					}
				}
			}

			if (i < len)
				res += input[i];
		}

		return res;
	}

	void ProcessSlashLine(SourceFile source)
	{
		for (int i = 0; i < source.data.Length; i++)
		{
			if (source.data[i] == '\n')
			{
				for (int j = i - 1; j >= 0; j--)
				{
					if (source.data[j] == ' ')
						continue;

					if (source.data[j] == '\r')
						continue;

					if (source.data[j] == '\t')
						continue;

					if (source.data[j] == '\\')
					{
						source.data = source.data.Remove(j, i - j + 1);
						i = j - 1;
					}

					break;
				}
			}
		}
	}
}