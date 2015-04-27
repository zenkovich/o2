#include "XmlDataFormat.h"

namespace o2
{
	namespace XmlDataFormat
	{
		bool LoadDataDoc(const String& data, DataDoc& doc)
		{
			pugi::xml_document xmlDoc;
			auto res = xmlDoc.load_buffer(data.Data(), data.Length());

			if (res.status != pugi::status_ok)
				return false;

			for (pugi::xml_node_iterator it = xmlDoc.begin(); it != xmlDoc.end(); ++it)
			{
				DataNode* newNode = new DataNode();
				LoadDataNode(*it, *newNode);
				doc.AddNode(newNode);
			}

			return true;
		}

		void XmlDataFormat::LoadDataNode(const pugi::xml_node& xmlNode, DataNode& dataNode)
		{
			dataNode.SetName(xmlNode.name());
			dataNode = xmlNode.child_value();

			for (pugi::xml_attribute_iterator it = xmlNode.attributes_begin(); it != xmlNode.attributes_end(); ++it)
			{
				DataNode* newNode = new DataNode(it->name());
				*newNode = it->value();
				dataNode.AddNode(newNode);
			}

			for (auto node:xmlNode)
			{
				DataNode* newNode = new DataNode();
				LoadDataNode(node, *newNode);
				dataNode.AddNode(newNode);
			}
		}

		String SaveDataDoc(const DataDoc& doc)
		{
			struct xmlWriter :public pugi::xml_writer
			{
				String* str;
				int length = 0;

				void write(const void* data, size_t size)
				{
					str->Reserve(length + size + 5);
					memcpy(str->Data() + length, data, size);
					length += size;
					str->Data()[length] = '\0';
				}
			};

			String res;
			pugi::xml_document xmlDoc;

			for (auto docNode:doc)
			{
				pugi::xml_node newNode = xmlDoc.append_child(docNode->GetName());
				newNode.append_child(pugi::node_pcdata).set_value((char*)*docNode);
				SaveDataNode(newNode, *docNode);
			}

			xmlWriter writer;
			writer.str = &res;
			xmlDoc.save(writer);

			return res;
		}

		void SaveDataNode(pugi::xml_node& xmlNode, const DataNode& dataNode)
		{
			for (auto docNode : dataNode)
			{
				if (docNode->GetChildNodes().Count() == 0)
				{
					pugi::xml_attribute newAttribute = xmlNode.append_attribute((pugi::char_t*)docNode->GetName().Data());
					newAttribute.set_value((char*)*docNode);
				}
				else
				{
					pugi::xml_node newNode = xmlNode.append_child((pugi::char_t*)docNode->GetName().Data());
					newNode.append_child(pugi::node_pcdata).set_value((char*)*docNode);
					SaveDataNode(newNode, *docNode);
				}
			}
		}
	}
}