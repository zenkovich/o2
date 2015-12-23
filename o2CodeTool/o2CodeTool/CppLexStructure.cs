using System.Collections.Generic;
using System.Xml.Serialization;
using System.Linq;

public enum ProtectLevel { Public, Private, Protected }

public class ILexem
{
	[XmlIgnore]
	public string data;

	[XmlIgnore]
	public int begin;

	[XmlIgnore]
	public int end;

	[XmlIgnore]
	public SourceFile source;
}

public class LexComment : ILexem
{
	public string comment;
}

public class LexCodeBlock : ILexem
{
	public string codeBlock;
}

public class LexVariableType : ILexem
{
	public enum Type { Regular, Reference, Pointer }

	public string definition;
	public string name;
	public bool   isContstant;
	public Type   type = Type.Regular;
}

public class LexVariable : ILexem
{
	public ProtectLevel    protectLevel = ProtectLevel.Public;
	public string          definition;
	public LexVariableType type = new LexVariableType();
	public string          name;
	public LexComment      comment;
	public bool            isStatic;

	[XmlIgnore]
	public LexSection      ownSection = null;
}

public class LexFunction : ILexem
{
	public ProtectLevel      protectLevel = ProtectLevel.Public;
	public string            definition;
	public LexVariableType   returnType = new LexVariableType() { name = "void" };
	public string            name;
	public List<LexVariable> parameters = new List<LexVariable>();
	public bool              isContstant;
	public bool              isVirtual;
	public bool              isStatic;
	public LexComment        comment;

	[XmlIgnore]
	public LexSection        ownSection = null;
}

public class LexTypedef : ILexem
{
	public string name;
	public string value;
}

public class LexEnum : ILexem
{
	public string       name;
	public List<string> content = new List<string>();
}

public class LexUnknown : ILexem
{
	public ProtectLevel protectLevel = ProtectLevel.Public;
	public LexComment   comment;
	public string       value;
}

public class LexUsingNamespace : ILexem
{
	public string       name;
	public LexNamespace nspace;
}

public class LexSection : ILexem
{
	public string                  name;

	[XmlIgnore]
	public LexSection              parentLexSection;

	[XmlIgnore]
	public List<LexComment>        comments = new List<LexComment>();
	public List<LexClass>          classes = new List<LexClass>();
	public List<LexFunction>       functions = new List<LexFunction>();
	public List<LexVariable>       variables = new List<LexVariable>();
	public List<LexUnknown>        unknownBlocks = new List<LexUnknown>();
	public List<LexTypedef>        typedefs = new List<LexTypedef>();
	public List<LexEnum>           enums = new List<LexEnum>();
	public List<LexSection>        childSections = new List<LexSection>();
	public List<LexUsingNamespace> usingNamespaces = new List<LexUsingNamespace>();

	public LexSection FindLexSection(string name)
	{
		int delm = name.IndexOf("::");
		if (delm < 0)
		{
			LexSection fnd = classes.Find(x => x.shortName == name);

			if (fnd != null)
				return fnd;

			fnd = childSections.Find(x => x.GetType() == typeof(LexNamespace) && (x as LexNamespace).name == name);

			if (fnd != null)
				return fnd;

			if (GetType() == typeof(LexClass))
			{
				foreach (var baseCls in (this as LexClass).baseClasses)
				{
					if (baseCls.lexClass == null)
						continue;

					fnd = baseCls.lexClass.FindLexSection(name);
					if (fnd != null)
						return fnd;
				}
			}

			if (parentLexSection != null && parentLexSection.usingNamespaces.Find(x => x.nspace == this) == null)
				return parentLexSection.FindLexSection(name);
		}
		else
		{
			string part = name.Substring(0, delm);

			LexSection childSec = childSections.Find(x =>
			{
				return ((x.GetType() == typeof(LexNamespace) && (x as LexNamespace).name == part));
			});

			if (childSec == null)
				childSec = classes.Find(x => x.shortName == part);

			if (childSec != null)
				return childSec.FindLexSection(name.Substring(delm + 2));
			else
			{
				if (parentLexSection != null && parentLexSection.usingNamespaces.Find(x => x.nspace == this) == null)
					return parentLexSection.FindLexSection(name);
			}
		}

		foreach (var sps in usingNamespaces)
		{
			var res = sps.nspace.FindLexSection(name);
			if (res != null)
				return res;
		}

		return null;
	}

	public LexComment FindComment(int begin, int end)
	{
		LexComment nearest = null;
		int minDist = int.MaxValue;
		foreach (var comm in comments)
		{
			if (comm.end < begin)
			{
				int dist = begin - comm.end;

				if (dist > 0 && dist < minDist && dist < 10)
				{
					nearest = comm;
					minDist = dist;
				}
			}
			else if (comm.begin > end)
			{
				string betweenData = data.Substring(end, comm.begin - end);
				bool isOnOneLine = !betweenData.Contains('\n');
				if (isOnOneLine)
					return comm;
			}
		}
		return nearest;
	}
}

public class LexClass : LexSection
{
	public class BaseClassDef
	{
		public ProtectLevel type;
		public string       className;

		[XmlIgnore]
		public LexClass     lexClass;
	}

	public ProtectLevel       protectLevel;
	public string             shortName;
	public List<BaseClassDef> baseClasses = new List<BaseClassDef>();
	public LexComment         comment;
	public bool               haveBody;
	public bool               isTemplate;
	public bool               isTypedef;
	public string             templates;
	public int                id;

	static int idd = 0;

	public LexClass() { id = idd++; }

	public bool IsBasedOn(LexClass baseClass)
	{
		foreach (var bs in baseClasses)
		{
			if (bs.lexClass == baseClass)
				return true;

			if (bs.lexClass != null && bs.lexClass.IsBasedOn(baseClass))
				return true;
		}

		return false;
	}

	public LexClass Clone()
	{
		LexClass copy = MemberwiseClone() as LexClass;

		copy.classes = new List<LexClass>();
		copy.childSections = new List<LexSection>();
		foreach (var cls in classes)
		{
			var cloned = cls.Clone();
            copy.classes.Add(cloned);
			copy.childSections.Add(cloned);
			cloned.parentLexSection = cls;
		}

		return copy;
	}
}

public class LexNamespace : LexSection
{
}

public class LexInclude : ILexem
{
	public string includeFile;
}

public class LexPragma : ILexem
{
	public string pragmaWord;
}

public class LexDefine : ILexem
{
	public string definition;
	public string content;
}

public class SourceFile
{
	public string location;

	[XmlIgnore]
	public string data;

	public List<LexInclude> includes = new List<LexInclude>();
	public List<LexPragma>  pragmas = new List<LexPragma>();
	public List<LexDefine>  defines = new List<LexDefine>();
	public LexNamespace     globalNamespace = new LexNamespace();
}

public class SrcMap
{
	public List<SourceFile> headers = new List<SourceFile>();
	public List<SourceFile> sources = new List<SourceFile>();
	public List<LexClass>   allClasses = new List<LexClass>();
	public LexNamespace     globalNamespace = new LexNamespace();
}