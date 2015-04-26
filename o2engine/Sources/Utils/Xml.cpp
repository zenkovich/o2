#include "Xml.h"

#include "Utils/FileSystem/File.h"
#include "Utils/SmartPointers.h"

namespace o2
{
	namespace Xml
	{
		bool LoadFromFile(const String& fileName, Document& xmlDoc)
		{
			InFile inFile(fileName);

			if (!inFile.IsOpened())
				return false;

			UInt dataSize = inFile.GetDataSize();
			AutoArr<char> buffer(new char[dataSize]);

			inFile.ReadFullData(buffer);

			pugi::xml_parse_result res = xmlDoc.load_buffer(buffer, dataSize);

			return res.status == pugi::status_ok;
		}

		bool LoadFromString(const String& data, Document& xmlDoc)
		{
			pugi::xml_parse_result res = xmlDoc.load_buffer(data, data.Length());
			return res.status == pugi::status_ok;
		}

		bool SaveToString(String& data, Document& xmlDoc)
		{
			struct xmlWriter:public pugi::xml_writer
			{
				String* str;

				void write(const void* data, size_t size)
				{
					*str += (char*)data;
				}
			};

			xmlWriter writer;
			writer.str = &data;
			xmlDoc.save(writer);

			return true;
		}

		bool SaveToFile(const String& fileName, Document& xmlDoc)
		{
			struct xmlWriter:public pugi::xml_writer
			{
				OutFile* file;

				void write(const void* data, size_t size)
				{
					file->WriteData(data, size);
				}
			};

			OutFile outFile(fileName);

			if (!outFile.IsOpened())
				return false;

			xmlWriter writer;
			writer.file = &outFile;
			xmlDoc.save(writer);

			outFile.Close();

			return true;
		}

		Node GetNode(const Node& node, const String& path)
		{
			int slashPos = path.Find("/");
			String currNode = path.SubStr(0, slashPos);

			if (slashPos == -1)
				return node.child(currNode);
			else
				return GetNode(node.child(currNode), path.SubStr(slashPos + 1));
		}

		void ToXmlNode(int object, Node& node)
		{
			node.append_attribute("v") = object;
		}

		void ToXmlNode(unsigned int object, Node& node)
		{
			node.append_attribute("v") = object;
		}

		void ToXmlNode(float object, Node& node)
		{
			node.append_attribute("v") = object;
		}

		void ToXmlNode(String& object, Node& node)
		{
			node.append_attribute("v") = object;
		}

		void ToXmlNode(bool object, Node& node)
		{
			node.append_attribute("v") = object;
		}

		void ToXmlNode(Vec2F& object, Node& node)
		{
			node.append_attribute("x") = object.x;
			node.append_attribute("y") = object.y;
		}

		void ToXmlNode(Vec2I& object, Node& node)
		{
			node.append_attribute("x") = object.x;
			node.append_attribute("y") = object.y;
		}

		void ToXmlNode(RectF& object, Node& node)
		{
			node.append_attribute("left") = object.left;
			node.append_attribute("top") = object.top;
			node.append_attribute("right") = object.right;
			node.append_attribute("bottom") = object.bottom;
		}

		void ToXmlNode(RectI& object, Node& node)
		{
			node.append_attribute("left") = object.left;
			node.append_attribute("top") = object.top;
			node.append_attribute("right") = object.right;
			node.append_attribute("bottom") = object.bottom;
		}

		void ToXmlNode(Color4& object, Node& node)
		{
			node.append_attribute("r") = object.r;
			node.append_attribute("g") = object.g;
			node.append_attribute("b") = object.b;
			node.append_attribute("a") = object.a;
		}

		void ToXmlNode(WideTime& object, Node& node)
		{
			node.append_attribute("second") = object.mSecond;
			node.append_attribute("minute") = object.mMinute;
			node.append_attribute("hour")   = object.mHour;
			node.append_attribute("day")    = object.mDay;
			node.append_attribute("month")  = object.mMonth;
			node.append_attribute("year")   = object.mYear;
		}

		void FromXmlNode(int& object, Node& node)
		{
			object = node.attribute("v").as_int();
		}

		void FromXmlNode(unsigned int& object, Node& node)
		{
			object = node.attribute("v").as_uint();
		}

		void FromXmlNode(float& object, Node& node)
		{
			object = node.attribute("v").as_float();
		}

		void FromXmlNode(String& object, Node& node)
		{
			object = node.attribute("v").value();
		}

		void FromXmlNode(bool& object, Node& node)
		{
			object = node.attribute("v").as_bool();
		}

		void FromXmlNode(Vec2F& object, Node& node)
		{
			object.x = node.attribute("x").as_float();
			object.y = node.attribute("y").as_float();
		}

		void FromXmlNode(Vec2I& object, Node& node)
		{
			object.x = node.attribute("x").as_int();
			object.y = node.attribute("y").as_int();
		}

		void FromXmlNode(RectF& object, Node& node)
		{
			object.left = node.attribute("left").as_float();
			object.right = node.attribute("right").as_float();
			object.top = node.attribute("top").as_float();
			object.bottom = node.attribute("bottom").as_float();
		}

		void FromXmlNode(RectI& object, Node& node)
		{
			object.left = node.attribute("left").as_int();
			object.right = node.attribute("right").as_int();
			object.top = node.attribute("top").as_int();
			object.bottom = node.attribute("bottom").as_int();
		}

		void FromXmlNode(Color4& object, Node& node)
		{
			object.r = node.attribute("r").as_int();
			object.g = node.attribute("g").as_int();
			object.b = node.attribute("b").as_int();
			object.a = node.attribute("a").as_int();
		}

		void FromXmlNode(WideTime& object, Node& node)
		{
			object.mSecond = node.attribute("second").as_int();
			object.mMinute = node.attribute("minute").as_int();
			object.mHour   = node.attribute("hour").as_int();
			object.mDay    = node.attribute("day").as_int();
			object.mMonth  = node.attribute("month").as_int();
			object.mYear   = node.attribute("year").as_int();
		}
	}
}