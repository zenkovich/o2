using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;

namespace o2CodeTool
{
	class Program
	{
		static List<LexClass> ClassesSeletor(LexSection section)
		{
			var res = section.classes.FindAll(
					y => y.baseClasses.FindIndex(
						z => z.className == "ISerializable" || z.className == "IObject") >= 0).ToList();

			section.childSections.ForEach(x => ClassesSeletor(x).ForEach(v => res.Add(v)));

			return res;
		}

		static void PrintClass(LexClass cls, string indent)
		{
			Console.Write(indent + "-----------------\n" + indent + "//" + (cls.comment !=null ? cls.comment.comment : "") + "\n" + indent + "class: " + cls.name + ":");

			cls.baseClasses.ForEach(x => Console.Write(x.type + " " + x.className + ", "));

			Console.Write("\n" + indent + "{\n");

// 			Console.Write(indent + "  Classes: \n");
// 			cls.classes.ForEach(x => PrintClass(x, indent + "    "));

			Console.Write("\n" + indent + "  Variables: \n");
			cls.variables.ForEach(x => Console.Write(indent + "  //" + (x.comment !=null ? x.comment.comment:"") + "\n" + indent + "  " + x.protectLevel + " " + x.type.definition + " " + x.name + "\n"));

			Console.Write("\n" + indent + "  Functions: \n");
			cls.functions.ForEach(x =>
			{
				Console.Write(indent + "  //" + (x.comment !=null ? x.comment.comment : "") + "\n" + indent + "  " + x.protectLevel + " " + x.returnType.definition + " " + x.name + "(");
				x.parameters.ForEach(y => Console.Write(y.type.definition + " " + y.name + ", "));
				Console.Write(")\n");
			});

			Console.Write(indent + "}\n");
        }

		static void Main(string[] args)
		{
			DateTime beginTime = DateTime.Now;
			Console.Write("Starting ... " + beginTime + "\n");

			bool debug = false;

			SrcMap data = new SrcMap();
			CppLexParser parser = new CppLexParser();

			string output = args[0];

			for (int i = 1; i < args.Length; i++)
			{
				foreach (var file in Directory.EnumerateFiles(args[i], "*.*", SearchOption.AllDirectories))
				{
					if (file.EndsWith(".h"))
					{
						SourceFile header = new SourceFile();
						header.location = file;
						header.data = File.ReadAllText(file);
						data.headers.Add(header);
					}
					else if (file.EndsWith(".cpp"))
					{
						SourceFile header = new SourceFile();
						header.location = file;
						header.data = File.ReadAllText(file);
						data.sources.Add(header);
					}
				}
			}

			parser.Parse(data);

			Console.Write("Parsing done\n");

			if (debug)
			{
				foreach (var cls in data.allClasses)
				{
					Console.Write("class: " + cls.name + ": ");
					cls.baseClasses.ForEach(x => Console.Write(x.type + " " + x.className + " (" + (x.lexClass != null).ToString() + ")"));
					if (cls.isTemplate)
						Console.Write(" TEMPLATE!!!");
					Console.Write("\n");
				}

// 				var iobjectClass = data.allClasses.Find(x => x.name == "o2::IObject");
// 				var reflectableClasses = data.allClasses.FindAll(x => x.IsBasedOn(iobjectClass) && !x.isTemplate).ToList();
// 				reflectableClasses.ForEach(x => PrintClass(x, ""));

// 				using (var writer = new System.IO.StreamWriter("map.xml"))
// 				{
// 					var serializer = new XmlSerializer(data.GetType());
// 					serializer.Serialize(writer, data);
// 					writer.Flush();
// 				}
			}

			CppReflectionGenerator reflGenerator = new CppReflectionGenerator();
			reflGenerator.Generate(output, "", data);

			Console.Write("Complete! " + (DateTime.Now - beginTime).ToString());
		}
	}
}
