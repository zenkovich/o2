using System;
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

		Console.Write("CPP Generation: Includes done " + DateTime.Now + "\n");

		return res;
	}

	string GetTypesDeclarationData()
	{
		string res = "";

		reflectableClasses.ForEach(x => res += "o2::Type " + x.name.TrimStart(':') + "::type;\n");

		Console.Write("CPP Generation: Types declaration done " + DateTime.Now + "\n");

		return res;

	}

	string GetTypesInitializationsData()
	{
		string[] nonReflectingFunctions = { "IOBJECT", "SERIALIZABLE" };

		string res = "";
		foreach (var cls in reflectableClasses)
		{
			res += "void " + cls.name + "::InitializeType(" + cls.name + "* sample)\n{\n";

			foreach (var fld in cls.variables)
			{
				if (fld.isStatic)
					continue;
				
				res += "\tTypeInitializer::RegField(&type, \"" + fld.name + "\", (size_t)(char*)(&sample->" + fld.name + 
					") - (size_t)(char*)sample, sample->" + fld.name + ")";

				if (fld.comment != null && fld.comment.comment.Contains("@SERIALIZABLE"))
					res += ".AddAttribute<SerializableAttribute<decltype(" + fld.name + ")>>()";

				res += ";\n";
            }

			string dstr = ("~" + cls.shortName);

			bool isFirstFunc = true;
			foreach (var fnc in cls.functions)
			{
				if (fnc.isStatic || fnc.isTemplate || nonReflectingFunctions.Contains(fnc.name) ||
					cls.shortName.StartsWith(fnc.name) || fnc.name.StartsWith("operator") ||
					cls.shortName.StartsWith(fnc.name) || dstr.StartsWith(fnc.name))
				{
					continue;
				}

				if (isFirstFunc)
				{
					res += "\tauto funcInfo = TypeInitializer::RegFunction<" + cls.name + ", " + fnc.returnType.definition;
					isFirstFunc = false;
				}
				else res += "\tfuncInfo = TypeInitializer::RegFunction<" + cls.name + ", " + fnc.returnType.definition;

				foreach (var param in fnc.parameters)
					res += ", " + param.type.definition;

				res += ">(&type, \"" + fnc.name + "\", &" + cls.name + "::" + fnc.name + ");\n";

				//TypeInitializer::RegFuncParam<float>(funcInfo, "dt", false, false, false);
				foreach (var param in fnc.parameters)
				{
					res += "\tTypeInitializer::RegFuncParam<" + param.type.definition + ">(funcInfo, \"" + param.name +
						"\");\n";
				}

				//res += "\n";
            }

			res += "}\n\n";
		}

		Console.Write("CPP Generation: Types initializations functions done " + DateTime.Now + "\n");

		return res;
	}

	string GetTypeInitializationData()
	{
		string res = "";

		reflectableClasses.ForEach(x => res += "\to2::Reflection::InitializeType<" + x.name + ">(\"" + x.name + "\");\n");

		Console.Write("CPP Generation: Types initializations done " + DateTime.Now + "\n");

		return res;
	}

	string GetBaseTypesResolvingData()
	{
		string res = "";

		List<LexClass> processed = new List<LexClass>();
		reflectableClasses.ForEach(x => res += GetBaseClassSetting(x, processed));

		Console.Write("CPP Generation: Base classes resolving done " + DateTime.Now + "\n");

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
			res += "\tTypeInitializer::AddBaseType(&" + lexClass.name + "::type, &" + cls.lexClass.name + "::type);\n";
		}

		return res;
	}
}