#include "Xml.h"

#include "Utils/FileSystem/File.h"
#include "Utils/SmartPointers.h"

namespace o2
{
	namespace Xml
	{
		bool LoadFromFile(const String& fileName, pugi::xml_document& xmlDoc)
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

		bool LoadFromString(const String& data, pugi::xml_document& xmlDoc)
		{
			pugi::xml_parse_result res = xmlDoc.load_buffer(data.c_str(), data.length());
			return res.status == pugi::status_ok;
		}

		bool SaveToString(String& data, pugi::xml_document& xmlDoc)
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

		bool SaveToFile(const String& fileName, pugi::xml_document& xmlDoc, bool isConfigFile /*= true*/)
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

		pugi::xml_node GetNode(const pugi::xml_node& node, const String& path)
		{
			int slashPos = path.find('/');
			String currNode = path.substr(0, slashPos);

			if (slashPos == path.npos)
				return node.child(currNode.c_str());
			else
				return GetNode(node.child(currNode.c_str()), path.substr(slashPos + 1));
		}

		void ToXmlNode(int object, pugi::xml_node& node)
		{
			node.append_attribute("v") = object;
		}

		void ToXmlNode(unsigned int object, pugi::xml_node& node)
		{
			node.append_attribute("v") = object;
		}

		void ToXmlNode(float object, pugi::xml_node& node)
		{
			node.append_attribute("v") = object;
		}

		void ToXmlNode(String& object, pugi::xml_node& node)
		{
			node.append_attribute("v") = object.c_str();
		}

		void ToXmlNode(bool object, pugi::xml_node& node)
		{
			node.append_attribute("v") = object;
		}

		void ToXmlNode(Vec2F& object, pugi::xml_node& node)
		{
			node.append_attribute("x") = object.x;
			node.append_attribute("y") = object.y;
		}

		void ToXmlNode(Vec2I& object, pugi::xml_node& node)
		{
			node.append_attribute("x") = object.x;
			node.append_attribute("y") = object.y;
		}

		void ToXmlNode(RectF& object, pugi::xml_node& node)
		{
			node.append_attribute("left") = object.left;
			node.append_attribute("top") = object.top;
			node.append_attribute("right") = object.right;
			node.append_attribute("bottom") = object.bottom;
		}

		void ToXmlNode(RectI& object, pugi::xml_node& node)
		{
			node.append_attribute("left") = object.left;
			node.append_attribute("top") = object.top;
			node.append_attribute("right") = object.right;
			node.append_attribute("bottom") = object.bottom;
		}

		void ToXmlNode(Color4& object, pugi::xml_node& node)
		{
			node.append_attribute("r") = object.r;
			node.append_attribute("g") = object.g;
			node.append_attribute("b") = object.b;
			node.append_attribute("a") = object.a;
		}

		void ToXmlNode(WideTime& object, pugi::xml_node& node)
		{
			node.append_attribute("second") = object.mSecond;
			node.append_attribute("minute") = object.mMinute;
			node.append_attribute("hour")   = object.mHour;
			node.append_attribute("day")    = object.mDay;
			node.append_attribute("month")  = object.mMonth;
			node.append_attribute("year")   = object.mYear;
		}

		void FromXmlNode(int& object, pugi::xml_node& node)
		{
			object = node.attribute("v").as_int();
		}

		void FromXmlNode(unsigned int& object, pugi::xml_node& node)
		{
			object = node.attribute("v").as_uint();
		}

		void FromXmlNode(float& object, pugi::xml_node& node)
		{
			object = node.attribute("v").as_float();
		}

		void FromXmlNode(String& object, pugi::xml_node& node)
		{
			object = node.attribute("v").value();
		}

		void FromXmlNode(bool& object, pugi::xml_node& node)
		{
			object = node.attribute("v").as_bool();
		}

		void FromXmlNode(Vec2F& object, pugi::xml_node& node)
		{
			object.x = node.attribute("x").as_float();
			object.y = node.attribute("y").as_float();
		}

		void FromXmlNode(Vec2I& object, pugi::xml_node& node)
		{
			object.x = node.attribute("x").as_int();
			object.y = node.attribute("y").as_int();
		}

		void FromXmlNode(RectF& object, pugi::xml_node& node)
		{
			object.left = node.attribute("left").as_float();
			object.right = node.attribute("right").as_float();
			object.top = node.attribute("top").as_float();
			object.bottom = node.attribute("bottom").as_float();
		}

		void FromXmlNode(RectI& object, pugi::xml_node& node)
		{
			object.left = node.attribute("left").as_int();
			object.right = node.attribute("right").as_int();
			object.top = node.attribute("top").as_int();
			object.bottom = node.attribute("bottom").as_int();
		}

		void FromXmlNode(Color4& object, pugi::xml_node& node)
		{
			object.r = node.attribute("r").as_int();
			object.g = node.attribute("g").as_int();
			object.b = node.attribute("b").as_int();
			object.a = node.attribute("a").as_int();
		}

		void FromXmlNode(WideTime& object, pugi::xml_node& node)
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