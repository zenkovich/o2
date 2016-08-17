#include "CppSyntaxParser.h"

#include "SyntaxTree/SyntaxTree.h"
#include "Utils/FileSystem/FileSystem.h"
#include "Utils/Debug.h"
#include "Utils/Timer.h"

namespace CodeTool
{

	CppSyntaxParser::CppSyntaxParser()
	{
		InitializeParsers();
	}

	CppSyntaxParser::~CppSyntaxParser()
	{
		for (auto x : mParsers)
			delete x;
	}

	void CppSyntaxParser::InitializeParsers()
	{
		mParsers.Add(new ExpressionParser("namespace", &CppSyntaxParser::ParseNamespace, false, true));
		mParsers.Add(new ExpressionParser("namespace", &CppSyntaxParser::ParseNamespace, true, true));
		mParsers.Add(new ExpressionParser("//", &CppSyntaxParser::ParseComment, true, true));
		mParsers.Add(new ExpressionParser("/*", &CppSyntaxParser::ParseMultilineComment, true, true));
		mParsers.Add(new ExpressionParser("#pragma", &CppSyntaxParser::ParsePragma, false, true));
		mParsers.Add(new ExpressionParser("#include", &CppSyntaxParser::ParseInclude, false, true));
		mParsers.Add(new ExpressionParser("#define", &CppSyntaxParser::ParseDefine, true, true));
		mParsers.Add(new ExpressionParser("#if", &CppSyntaxParser::ParseIfMacros, true, true));
		mParsers.Add(new ExpressionParser("metaclass", &CppSyntaxParser::ParseMetaClass, true, true));
		mParsers.Add(new ExpressionParser("class", &CppSyntaxParser::ParseClass, true, true));
		mParsers.Add(new ExpressionParser("struct", &CppSyntaxParser::ParseStruct, true, true));
		mParsers.Add(new ExpressionParser("template", &CppSyntaxParser::ParseTemplate, true, true));
		mParsers.Add(new ExpressionParser("typedef", &CppSyntaxParser::ParseTypedef, true, true));
		mParsers.Add(new ExpressionParser("enum", &CppSyntaxParser::ParseEnum, true, true));
		mParsers.Add(new ExpressionParser("using", &CppSyntaxParser::ParseUsing, true, true));
		mParsers.Add(new ExpressionParser("public:", &CppSyntaxParser::ParsePublicSection, true, false));
		mParsers.Add(new ExpressionParser("private:", &CppSyntaxParser::ParsePrivateSection, true, false));
		mParsers.Add(new ExpressionParser("protected:", &CppSyntaxParser::ParseProtectedSection, true, false));
		mParsers.Add(new ExpressionParser("friend", &CppSyntaxParser::ParseFriend, true, false));
		mParsers.Add(new ExpressionParser("CLASS_META", &CppSyntaxParser::ParseClassMeta, false, true));
	}

	void CppSyntaxParser::Parse(SyntaxTree& syntaxTree, const String& sourcesPath)
	{
		o2Debug.Log("Begin parsing from %s", sourcesPath);
		Timer timer;

		mSyntaxTree = &syntaxTree;
		mSourcesPath = sourcesPath;

		FolderInfo srcFolderInfo = o2FileSystem.GetFolderInfo(sourcesPath);
		ParseSourcesInFolder(srcFolderInfo);

		o2Debug.Log("Parsing ended for %f seconds", timer.GetTime());
	}

	void CppSyntaxParser::ParseSourcesInFolder(FolderInfo& folderInfo)
	{
		for (auto& fileInfo : folderInfo.mFiles)
		{
			if (!fileInfo.mPath.EndsWith(".h"))
				continue;

			SyntaxFile* syntaxFile = mnew SyntaxFile();
			mSyntaxTree->mFiles.Add(syntaxFile);

			ParseFile(*syntaxFile, fileInfo);
		}

		for (auto& nestedFolderInfo : folderInfo.mFolders)
			ParseSourcesInFolder(nestedFolderInfo);
	}

	void CppSyntaxParser::ParseFile(SyntaxFile& file, const FileInfo& fileInfo)
	{
		file.mPath = fileInfo.mPath;
		file.mData = o2FileSystem.ReadFile(fileInfo.mPath);
		file.mLastEditedDate = fileInfo.mEditDate;

		ParseSyntaxSection(*file.mGlobalNamespace, file.mData, file, SyntaxProtectionSection::Public);
	}

	void CppSyntaxParser::ParseSyntaxSection(SyntaxSection& section, const String& source, SyntaxFile& file,
											 SyntaxProtectionSection protectionSection)
	{
		section.mLength = source.Length();
		section.mData = source;
		section.mFile = &file;

		String skipSymbols = " \r\n\t";

		int caret = 0;
		int line = 0;
		while (caret < section.mLength)
		{
			if (skipSymbols.Contains(source[caret]))
			{
				caret++;
				continue;
			}

			bool parsedByKeywork = false;
			for (auto parser : mParsers)
			{
				const char* keyWord = parser->keyWord;

				int i = 0;
				bool success = true;
				while (keyWord[i] != '\0')
				{
					if (source[i + caret] != keyWord[i])
					{
						success = false;
						break;
					}
					i++;
				}

				if (!success)
					continue;

				ParserDelegate pd = parser->parser;
				(this->*pd)(section, caret, line, protectionSection);
				parsedByKeywork = true;

				break;
			}

			if (parsedByKeywork)
				continue;

			int blockbegin = caret;
			String block = ReadBlock(section.mData, caret, line);
			block.Trim(" \r\t\n");

			if (!block.IsEmpty())
				TryParseBlock(section, block, blockbegin, caret, line, protectionSection);
			else
				caret++;
		}
	}

	void CppSyntaxParser::TryParseBlock(SyntaxSection& section, const String& block, int blockBegin, int& caret, int& line,
										SyntaxProtectionSection& protectionSection)
	{
		if (IsFunction(block))
		{
			auto func = ParseFunction(block, protectionSection, blockBegin, caret);
			func->mFile = section.mFile;
			section.mFunctions.Add(func);
		}
		else
		{
			auto var = ParseVariable(block, protectionSection, blockBegin, caret);
			var->mFile = section.mFile;
			section.mVariables.Add(var);
		}
	}

	bool CppSyntaxParser::IsFunction(const String& data)
	{
		int locCaret = 0;
		int locLine = 0;
		bool isFunction = false;

		String firstWord = ReadWord(data, locCaret, locLine, " \n\r(){}[]");

		if (firstWord.IsEmpty())
			return false;

		if (firstWord == "virtual")
			firstWord = ReadWord(data, locCaret, locLine, " \n\r(){}[]");

		if (firstWord == "static")
			firstWord = ReadWord(data, locCaret, locLine, " \n\r(){}[]");

		if (firstWord == "typename")
			firstWord = ReadWord(data, locCaret, locLine, " \n\r(){}[]");

		if (firstWord == "inline")
			firstWord = ReadWord(data, locCaret, locLine, " \n\r(){}[]");

		if (GetNextSymbol(data, locCaret, " \n\r\t") == '(')
		{
			String braces = ReadBraces(data, locCaret, locLine);
			braces.Trim(" \n\t\r()");

			int tmpCaret = 0;
			String word = ReadWord(braces, tmpCaret, locLine);

			isFunction = GetNextSymbol(braces, tmpCaret, " \n\r\t") != ':';

			if (!isFunction && braces.StartsWith("std"))
				isFunction = true;
		}
		else
		{
			if (firstWord == "const")
				ReadWord(data, locCaret, locLine, " \n\r(){}[]");

			String thirdWord = ReadWord(data, locCaret, locLine, " \n\r(){}[]");

			if (thirdWord == "operator")
				thirdWord = ReadWord(data, locCaret, locLine, " \n\r(){}");

			if (GetNextSymbol(data, locCaret, " \n\r\t") == '(')
				isFunction = true;
		}

		return isFunction;
	}

	SyntaxVariable* CppSyntaxParser::ParseVariable(const String& data, SyntaxProtectionSection& protectionSection,
												   int begin, int end)
	{
		SyntaxVariable* res = mnew SyntaxVariable();
		res->mBegin = begin;
		res->mLength = end - begin;
		res->mData = data;

		int caret = 0;
		int line = 0;
		String typeWord = ReadWord(data, caret, line, " \n\r(){}[]");
		String typeDefinition = typeWord;

		if (typeWord == "static")
		{
			typeWord = ReadWord(data, caret, line, " \n\r(){}[]");
			typeDefinition += " " + typeWord;
			res->mIsStatic = true;
		}

		if (typeWord == "const")
		{
			typeWord = ReadWord(data, caret, line, " \n\r(){}[]");
			res->mType.mIsContant = true;

			typeDefinition += " " + typeWord;
		}

		if (typeWord.Last() == '&')
			res->mType.mIsReference = true;

		if (typeWord.Last() == '*')
			res->mType.mIsPointer = true;

		res->mType.mName = typeWord;

		res->mClassSection = protectionSection;

		if (GetNextSymbol(data, caret, " \n\r\t") == '(')
		{
			String braces = ReadBraces(data, caret, line); braces.Trim(" \r\t\t()");
			String nextBraces = ReadBraces(data, caret, line); nextBraces.Trim(" \r\t\t()");

			int tmpCaret = 0;
			String bracesFirst = ReadWord(braces, tmpCaret, line);
			tmpCaret += 3;
			res->mName = braces.SubStr(tmpCaret);

			res->mType.mName += " (" + bracesFirst + "*)(" + nextBraces + ")";
		}
		else
			res->mName = ReadWord(data, caret, line, " \n\r(){}[]");

		return res;
	}

	SyntaxFunction* CppSyntaxParser::ParseFunction(const String& data, SyntaxProtectionSection& protectionSection,
												   int begin, int end)
	{
		SyntaxFunction* res = mnew SyntaxFunction();
		res->mBegin = begin;
		res->mLength = end - begin;
		res->mData = data;

// 		if (isNextLexTemplate)
// 		{
// 			res.isTemplate = true;
// 			res.templates = templatesBuffer;
// 			isNextLexTemplate = false;
// 		}

		res->mClassSection = protectionSection;

		int caret = 0;
		int line = 0;
		String typeWord = ReadWord(data, caret, line, " \n\r(){}[]");

		if (typeWord == "virtual")
		{
			res->mIsVirtual = true;
			typeWord = ReadWord(data, caret, line, " \n\r(){}[]");
		}

		if (typeWord == "static")
		{
			res->mIsStatic = true;
			typeWord = ReadWord(data, caret, line, " \n\r(){}[]");
		}

		if (typeWord == "inline")
			typeWord = ReadWord(data, caret, line, " \n\r(){}[]");

		if (typeWord == "typename")
			typeWord = ReadWord(data, caret, line, " \n\r(){}[]");

		if (typeWord == "explicit")
			typeWord = ReadWord(data, caret, line, " \n\r(){}[]");

		if (typeWord == "operator")
		{
			String nextWord = ReadWord(data, caret, line, " \n\r(){}[]");
			res->mName = typeWord + nextWord;
			res->mReturnType.mName = "void";
		}
		else
		{
			if (GetNextSymbol(data, caret, " \n\r\t") == '(')
			{
				res->mName = typeWord;
				res->mReturnType.mName = "void";
			}
			else
			{
				String typeDefinition = typeWord;

				if (typeWord == "const")
				{
					typeWord = ReadWord(data, caret, line, " \n\r(){}[]");
					res->mReturnType.mIsContant = true;

					typeDefinition += " " + typeWord;
				}

				if (typeWord.Last() == '&')
					res->mReturnType.mIsReference = true;

				if (typeWord.Last() == '*')
					res->mReturnType.mIsPointer = true;

				res->mReturnType.mName = typeWord;

				res->mName = ReadWord(data, caret, line, " \n\r(){}[]");

				if (res->mName == "operator")
					res->mName += " " + ReadWord(data, caret, line, " \n\r(){}");
			}
		}

		String paramsStr = ReadBraces(data, caret, line); paramsStr.Trim(" \n\r\t");
		String afterParamWord = ReadWord(data, caret, line);

		if (afterParamWord == "const")
			res->mIsContstant = true;

		if (!paramsStr.IsEmpty())
		{
			auto paramsArr = Split(paramsStr, ',');
			for (auto& prm : paramsArr)
			{
				String trimmedParam = prm.Trimed(" \r\n\t");
				SyntaxProtectionSection tempProtectSection = SyntaxProtectionSection::Public;
				res->mParameters.Add(ParseVariable(trimmedParam, tempProtectSection, begin, end));
			}
		}

		return res;
	}

	void CppSyntaxParser::ParseNamespace(SyntaxSection& section, int& caret, int& line, 
										 SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("namespace");

		String namespaceName = ReadWord(section.mData, caret, line);
		int namespaceBegin = section.mData.Find("{", caret) + 1;
		String block = ReadBlock(section.mData, caret, line); block.Trim(" \r\t\n");

		SyntaxNamespace* newNamespace = mnew SyntaxNamespace();
		newNamespace->mBegin = begin;
		newNamespace->mLength = caret - begin;
		newNamespace->mData = block.SubStr(1, block.Length() - 1); newNamespace->mData.Trim(" \r\t\n");
		newNamespace->mName = namespaceName;
		newNamespace->mFile = section.mFile;
		newNamespace->mParentSection = &section;
		section.mSections.Add(newNamespace);

		ParseSyntaxSection(*newNamespace, newNamespace->mData, *section.mFile, SyntaxProtectionSection::Public);
	}

	void CppSyntaxParser::ParseComment(SyntaxSection& section, int& caret, int& line, 
									   SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("//");

		SyntaxComment* comment = mnew SyntaxComment();
		comment->mData = ReadWord(section.mData, caret, line, "\n", ""); comment->mData.Trim(" \r");
		comment->mBegin = begin;
		comment->mLength = caret - begin;
		comment->mFile = section.mFile;

		section.mComments.Add(comment);
	}

	void CppSyntaxParser::ParseMultilineComment(SyntaxSection& section, int& caret, int& line, 
												SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("/*");
		int end = section.mData.Find("*/", caret);
		caret = end;

		SyntaxComment* comment = mnew SyntaxComment();
		comment->mData = section.mData.SubStr(begin + 2, end - 2); comment->mData.Trim(" \r\t\n");
		comment->mBegin = begin;
		comment->mLength = caret - begin;
		comment->mFile = section.mFile;

		section.mComments.Add(comment);
	}

	void CppSyntaxParser::ParsePragma(SyntaxSection& section, int& caret, int& line, 
									  SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("#pragma");
		ReadWord(section.mData, caret, line);
	}

	void CppSyntaxParser::ParseInclude(SyntaxSection& section, int& caret, int& line, 
									   SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("#include");
		ReadWord(section.mData, caret, line, "\n");
	}

	void CppSyntaxParser::ParseDefine(SyntaxSection& section, int& caret, int& line, 
									  SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("#define");

		ReadWord(section.mData, caret, line);
		ReadWord(section.mData, caret, line, "\n");
	}

	void CppSyntaxParser::ParseIfMacros(SyntaxSection& section, int& caret, int& line, 
										SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("#if");

		String skipSymbols = " \r\n\t";
		String endWord = "#endif";
		int endWordLength = endWord.Length();

		int dataLen = section.mData.Length();
		for (; caret < dataLen; caret++)
		{
			if (skipSymbols.Contains(section.mData[caret]))
				continue;

			String sub = section.mData.SubStr(caret, Math::Min(endWordLength - 1, caret));
			if (sub == endWord)
			{
				caret = Math::Min(dataLen, caret + endWordLength);
				return;
			}
		}
	}

	void CppSyntaxParser::ParseMetaClass(SyntaxSection& section, int& caret, int& line, 
										 SyntaxProtectionSection& protectionSection)
	{
		ParseClassOrStruct(section, caret, line, protectionSection, true, true, "");
	}

	void CppSyntaxParser::ParseClass(SyntaxSection& section, int& caret, int& line, 
									 SyntaxProtectionSection& protectionSection)
	{
		ParseClassOrStruct(section, caret, line, protectionSection, true, false, "");
	}

	void CppSyntaxParser::ParseStruct(SyntaxSection& section, int& caret, int& line, 
									  SyntaxProtectionSection& protectionSection)
	{
		ParseClassOrStruct(section, caret, line, protectionSection, false, false, "");
	}

	void CppSyntaxParser::ParseClassOrStruct(SyntaxSection& section, int& caret, int& line, 
											 SyntaxProtectionSection& protectionSection,
											 bool isClass, bool isMeta, const String& templates)
	{
		int begin = caret;

		if (isMeta)
			caret += strlen("meta");

		if (isClass) caret += strlen("class");
		else         caret += strlen("struct");

		String className = ReadWord(section.mData, caret, line, " \n\t\r:;/");
		String afterName = ReadWord(section.mData, caret, line, ";{/"); afterName.Trim(" :\r\n\t");

		String shortClassName = className;
		className = section.mName + "::" + className;

		SyntaxClass* newClass = mnew SyntaxClass();

		newClass->mBegin         = begin;
		newClass->mLength        = caret - begin;
		newClass->mData          = section.mData.SubStr(begin, caret);
		newClass->mName          = shortClassName;
		newClass->mFullName      = className;
		newClass->mFile          = section.mFile;
		newClass->mParentSection = &section;
		newClass->mClassSection  = protectionSection;

// 		if (isNextLexTemplate)
// 			newClass.templates = templatesBuffer;

		//isNextLexTemplate = false;

		if (!afterName.IsEmpty())
		{
			auto baseClasses = Split(afterName, ',');

			for (auto& baseClass : baseClasses)
			{
				baseClass.Trim();

				int spacePos = baseClass.Find(' ');
				if (spacePos < 0)
					newClass->mBaseClasses.Add(SyntaxClassInheritance(baseClass, SyntaxProtectionSection::Private));
				else
				{
					String sectionTypeName = baseClass.SubStr(0, spacePos);
					String baseClassName = baseClass.SubStr(spacePos + 1);

					if (baseClassName.StartsWith("virtual"))
						baseClassName.Erase(0, strlen("virtual") + 1);

					SyntaxProtectionSection sectionType = SyntaxProtectionSection::Private;

					if (sectionTypeName == "public")
						sectionType = SyntaxProtectionSection::Public;
					else if (sectionTypeName == "protected")
						sectionType = SyntaxProtectionSection::Protected;

					newClass->mBaseClasses.Add(SyntaxClassInheritance(baseClassName, sectionType));
				}
			}
		}

		int dataLength = section.mData.Length();

		if (caret < dataLength && section.mData[caret] == '/')
		{
			String comment = ReadWord(section.mData, caret, line, "\n");
			ReadWord(section.mData, caret, line, ";{/");

			//newClass.comment = new LexComment(){ comment = comment };
		}

		if (caret < dataLength && section.mData[caret] == '{')
		{
			int sectionBegin = caret;
			newClass->mData = ReadBlock(section.mData, caret, line); newClass->mData.Trim("{} \n\r\t");
			//newClass->haveBody = true;

			section.mSections.Add(newClass);

			ParseSyntaxSection(*newClass, newClass->mData, *newClass->mFile, protectionSection);
		}
	}

	void CppSyntaxParser::ParseTemplate(SyntaxSection& section, int& caret, int& line, 
										SyntaxProtectionSection& protectionSection)
	{
		caret += strlen("template");

		int dataLen = section.mData.Length();

		for (; caret < dataLen; caret++)
			if (section.mData[caret] == '<')
				break;

		caret++;
		int braces = 1;
		int begin = caret;

		for (; caret < dataLen; caret++)
		{
			if (section.mData[caret] == '<')
				braces++;

			if (section.mData[caret] == '>')
			{
				braces--;

				if (braces == 0)
					break;
			}
		}

		String tempInside = section.mData.SubStr(begin, caret);

		int tmpCaret = caret + 1;
		String block = ReadBlock(section.mData, tmpCaret, line); block.Trim(" \n\r\t");

		if (block.StartsWith("class"))
		{
			caret = section.mData.Find("class", caret + 1);
			ParseClassOrStruct(section, caret, line, protectionSection, true, false, tempInside);
		}
		else if (block.StartsWith("struct"))
		{
			caret = section.mData.Find("struct", caret + 1);
			ParseClassOrStruct(section, caret, line, protectionSection, false, false, tempInside);
		}
		else if (block.StartsWith("metaclass"))
		{
			caret = section.mData.Find("metaclass", caret + 1);
			ParseClassOrStruct(section, caret, line, protectionSection, true, true, tempInside);
		}
		else if (IsFunction(block))
		{
			auto func = ParseFunction(block, protectionSection, 0, block.Length());
			func->mTemplates = tempInside;
			func->mFile = section.mFile;
			section.mFunctions.Add(func);
		}
	}

	void CppSyntaxParser::ParseTypedef(SyntaxSection& section, int& caret, int& line, 
									   SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("typedef");
		String value = ReadWord(section.mData, caret, line, " \n\r\t"); value.Trim(" \r\t\n");
		String name = ReadWord(section.mData, caret, line, " \n\r\t"); name.Trim(" \r\t\n;");
	}

	void CppSyntaxParser::ParseEnum(SyntaxSection& section, int& caret, int& line, 
									SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("enum");

		String name = ReadWord(section.mData, caret, line);

		if (name == "class")
			name = ReadWord(section.mData, caret, line);

		String block = ReadBlock(section.mData, caret, line); block.Trim(" {}\r\t\n");
		RemoveComments(block);
		auto content = Split(block, ',');

		SyntaxEnum* newEnum = mnew SyntaxEnum();
		newEnum->mBegin  = begin;
		newEnum->mLength = caret - begin;
		newEnum->mData   = section.mData.SubStr(begin, caret);
		newEnum->mName   = name;
		newEnum->mFile   = section.mFile;

		for(auto& x : content)
		{
			x.Trim(" \n\t\r");

			String name, value;
			int valuePos = x.Find('=');

			if (valuePos >= 0)
			{
				name = x.SubStr(0, valuePos); name.Trim(" \n\t\r");
				value = x.SubStr(valuePos + 1); value.Trim(" \n\t\r");
			}
			else name = x;

			newEnum->mEntries.Add(name, value);
		}

		section.mEnums.Add(newEnum);
	}

	void CppSyntaxParser::ParseUsing(SyntaxSection& section, int& caret, int& line, 
									 SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("using");

		ReadWord(section.mData, caret, line);
		ReadWord(section.mData, caret, line);
	}

	void CppSyntaxParser::ParsePublicSection(SyntaxSection& section, int& caret, int& line, 
											 SyntaxProtectionSection& protectionSection)
	{
		caret += strlen("public:");
		protectionSection = SyntaxProtectionSection::Public;
	}

	void CppSyntaxParser::ParsePrivateSection(SyntaxSection& section, int& caret, int& line, 
											  SyntaxProtectionSection& protectionSection)
	{
		caret += strlen("private:");
		protectionSection = SyntaxProtectionSection::Public;
	}

	void CppSyntaxParser::ParseProtectedSection(SyntaxSection& section, int& caret, int& line,
												SyntaxProtectionSection& protectionSection)
	{
		caret += strlen("protected:");
		protectionSection = SyntaxProtectionSection::Public;
	}

	void CppSyntaxParser::ParseFriend(SyntaxSection& section, int& caret, int& line, 
									  SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("friend");
		/*string name = */ ReadWord(section.mData, caret, line, " \n\r\t"); // .Trim(' ', '\r', '\t', '\n');
		/*string value = */ReadWord(section.mData, caret, line, " \n\r\t"); // .Trim(' ', '\r', '\t', '\n');
	}

	void CppSyntaxParser::ParseClassMeta(SyntaxSection& section, int& caret, int& line, 
										 SyntaxProtectionSection& protectionSection)
	{
		int begin = caret;
		caret += strlen("CLASS_META");

		String braces = ReadBraces(section.mData, caret, line); braces.Trim(" \n\r\t");
		String body = ReadBlock(section.mData, caret, line); body.Trim(" \n\r\t{}");

		SyntaxClassMeta* classMeta = mnew SyntaxClassMeta();
		classMeta->mBegin = caret;
		classMeta->mLength = caret - begin;
		classMeta->mFile = section.mFile;
		classMeta->mData = body;
		classMeta->mClassName = braces;

		section.mClassMetas.Add(classMeta);
	}

	String CppSyntaxParser::ReadWord(const String& data, int& caret, int& line,
									 const char* breakSymbols /*= " \n\r(){}.,;+-* /=@!|&*:~\\"*/,
									 const char* skipSymbols /*= " \n\r"*/)
	{
		String res = "";
		int braces = 0, sqBraces = 0, trBraces = 0, fgBraces = 0;
		int dataLen = data.Length();

		for (; caret < dataLen; caret++)
		{
			if (data[caret] == '\n')
				line++;

			int i = 0;
			bool stop = true;
			char s = data[caret];

			while (skipSymbols[i] != '\0')
			{
				if (skipSymbols[i] == s)
				{
					stop = false;
					break;
				}

				i++;
			}

			if (stop)
				break;
		}

		for (; caret < dataLen; caret++)
		{
			if (data[caret] == '\n')
				line++;

			int i = 0;
			bool stop = false;
			char s = data[caret];

			while (breakSymbols[i] != '\0')
			{
				if (breakSymbols[i] == s)
				{
					stop = true;
					break;
				}

				i++;
			}

			if (stop)
				break;

			switch (s)
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

			res += s;
		}

		return res;
	}

	String CppSyntaxParser::ReadBlock(const String& data, int& caret, int& line)
	{
		int begin = caret;
		int braces = 0, fgBraces = 0, sqBraces = 0, trBraces = 0;
		bool isInString = false;
		int dataLen = data.Length();

		for (; caret < dataLen; caret++)
		{
			if (data[caret] == '\n')
				line++;

			if (data[caret] == '{')
				break;

			if (data[caret] == ';')
				return data.SubStr(begin, caret);
		}

		caret++;
		fgBraces++;

		bool complete = false;
		for (; caret < dataLen && !complete; caret++)
		{
			if (data[caret] == '\n')
				line++;

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

		return data.SubStr(begin, Math::Min(caret, dataLen - 1));
	}

	String CppSyntaxParser::ReadBraces(const String& data, int& caret, int& line)
	{
		int begin = caret;
		int braces = 0, fgBraces = 0, sqBraces = 0, trBraces = 0;
		bool isInString = false;
		int dataLen = data.Length();

		for (; caret < dataLen; caret++)
		{
			if (data[caret] == '\n')
				line++;

			if (data[caret] == '(')
				break;

			if (data[caret] == ';')
				return data.SubStr(begin, caret);
		}

		caret++;
		braces++;

		bool complete = false;
		for (; caret < dataLen && !complete; caret++)
		{
			if (data[caret] == '\n')
				line++;

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

		String res = data.SubStr(begin, Math::Min(caret, dataLen));
		int resLen = res.Length();

		if (res[resLen - 1] == ')')
			res.Erase(resLen - 1, resLen);

		if (res[0] == '(')
			res.Erase(0, 1);

		return res;
	}

	char CppSyntaxParser::GetNextSymbol(const String& data, int begin, const char* skipSymbols /*= " \n\r\t()[]{}"*/)
	{
		int dataLen = data.Length();
		for (; begin < dataLen; begin++)
		{
			int i = 0;
			bool stop = true;
			char s = data[begin];

			while (skipSymbols[i] != '\0')
			{
				if (skipSymbols[i] == s)
				{
					stop = false;
					break;
				}

				i++;
			}

			if (stop)
				return s;
		}

		return '\0';
	}

	Vector<String> CppSyntaxParser::Split(const String& data, char splitSymbol)
	{
		Vector<String> res;
		int braces = 0, sqBraces = 0, trBraces = 0, fgBraces = 0;
		int dataLen = data.Length();

		int lastSplit = 0;
		for (int i = 0; i < dataLen; i++)
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
				res.Add(data.SubStr(lastSplit, i));
				lastSplit = i + 1;
			}
		}

		res.Add(data.SubStr(lastSplit));

		return res;
	}

	void CppSyntaxParser::RemoveComments(String& input)
	{
		int len = input.Length();
		int offs = 0;
		for (int i = 0; i < len; i++)
		{
			if (input[i] == '/' && i < len - 1 && input[i + 1] == '/')
			{
				for (; i < len; i++, offs++)
				{
					if (input[i] == '\n')
						break;
				}
			}

			if (input[i] == '/' && i < len - 1 && input[i + 1] == '*')
			{
				for (; i < len; i++, offs++)
				{
					if (input[i - 1] == '*' && input[i] == '/')
						break;
				}
			}

			input[i - offs] = input[i];
		}
	}

	CppSyntaxParser::ExpressionParser::ExpressionParser(const char* keyWord, ParserDelegate parser,
														bool isPossibleInClass /*= true*/,
														bool isPossibleInNamespace /*= true*/):
		keyWord(keyWord), parser(parser), isPossibleInClass(isPossibleInClass), isPossibleInNamespace(isPossibleInNamespace)
	{}
}