using System.Collections.Generic;
using System.IO;
using System.Linq;

public class CppReflectionGenerator
{
	LexClass       iobjectClass;
	List<LexClass> reflectableClasses;
	string         directoriesCut;

	string CutPath(string input)
	{
		if (input.StartsWith(directoriesCut))
			return input.Substring(directoriesCut.Length);

		return input;
	}

	public void Generate(string outputPath, string directoriesCut, SrcMap map)
	{
		this.directoriesCut = directoriesCut;
		iobjectClass = map.allClasses.Find(x => x.name == "o2::IObject");
		reflectableClasses = map.allClasses.FindAll(x =>
		{
			return x.IsBasedOn(iobjectClass) &&  !x.isTemplate && !x.isTypedef;

		}).ToList();

		GenerateHeader(outputPath);
		GenerateSource(outputPath);
    }

	void GenerateHeader(string outputPath)
	{
		File.WriteAllText(outputPath + ".h", "#pragma once \n\n// Registers all types\nvoid RegReflectionTypes();");
	}

	void GenerateSource(string outputPath)
	{
		string sourceData = "#include \"" + CutPath(outputPath) + ".h\"\n\n#include \"Utils/Reflection/Reflection.h\"\n\n" +
			"// Includes\n" + GetIncludesData() + "\n" +
			"// Types declarations\n" + GetTypesDeclarationData() + "\n" +
			"// Types initializations\n" + GetTypesInitializationsData() + "\n" +
			"// Registering all types\n" + 
			"void RegReflectionTypes()\n{\n" + 
			"\t// Create types\n" + GetTypesCreationData() + "\n" + 
			"\t// Initialize types\n" + GetTypeInitializationData() + "\n" +
			"\t// Resolve inheritance\n" + GetBaseTypesResolvingData() + "\n}";

		File.WriteAllText(outputPath + ".cpp", sourceData);
	}

	string GetIncludesData()
	{
		List<string> includes = new List<string>();
		foreach (var cls in reflectableClasses)
		{
			if (!includes.Contains(cls.source.location))
				includes.Add(cls.source.location);
        }

		string res = "";
		includes.ForEach(x => res += "#include \"" + CutPath(x) + "\"\n");

		return res;
	}

	string GetTypesDeclarationData()
	{
		string res = "";

		reflectableClasses.ForEach(x => res += "o2::Type* " + x.name + "::type;\n");

		return res;

	}

	string GetTypesInitializationsData()
	{
		string res = "";
		foreach (var cls in reflectableClasses)
		{
			res += "void " + cls.name + "::InitializeType(" + cls.name + "* sample)\n{\n";

			foreach (var fld in cls.variables)
			{
				if (fld.isStatic)
					continue;

				res += "\tFIELD(" + fld.name + ")";

				if (fld.comment != null && fld.comment.comment.Contains("@SERIALIZABLE"))
					res += ".AddAttribute<SerializableAttribute<decltype(" + fld.name + ")>>()";

				res += ";\n";
            }

			res += "}\n\n";
        }
		return res;
	}

	string GetTypesCreationData()
	{
		string res = "";

		reflectableClasses.ForEach(x => res += "\t" + x.name + "::type = mnew Type();\n");

		return res;
	}

	string GetTypeInitializationData()
	{
		string res = "";

		reflectableClasses.ForEach(x => res += "\to2::Reflection::InitializeType<" + x.name + ">(\"" + x.name + "\");\n");

		return res;
	}

	string GetBaseTypesResolvingData()
	{
		string res = "";

		List<LexClass> processed = new List<LexClass>();
		reflectableClasses.ForEach(x => res += GetBaseClassSetting(x, processed));

		return res;
	}

	string GetBaseClassSetting(LexClass lexClass, List<LexClass> processed)
	{
		if (!reflectableClasses.Contains(lexClass))
			return "";

		if (processed.Contains(lexClass))
			return "";

		processed.Add(lexClass);

		string res = "";

		foreach (var cls in lexClass.baseClasses)
		{
			if (cls.lexClass == null || cls.lexClass == iobjectClass || !cls.lexClass.IsBasedOn(iobjectClass))
				continue;

			res += GetBaseClassSetting(cls.lexClass, processed);
			res += "\t" + lexClass.name + "::type->AddBaseType(" + cls.lexClass.name + "::type);\n";
		}

		return res;
	}
}