#include "o2/stdafx.h"
#include "XmlDataFormat.h"

//namespace o2
// {
//     namespace XmlDataFormat
//     {
//         bool LoadDataDoc(const WString& data, DataValue& node)
//         {
//             pugi::xml_document xmlDoc;
//             auto res = xmlDoc.load_buffer(data.Data(), data.Length()*sizeof(wchar_t));
// 
//             if (res.status != pugi::status_ok)
//                 return false;
// 
//             for (pugi::xml_node_iterator it = xmlDoc.begin(); it != xmlDoc.end(); ++it)
//             {
//                 if (it->type() != pugi::node_element)
//                     continue;
// 
//                 LoadDataValue(*it, node);
//             }
// 
//             return true;
//         }
// 
//         void XmlDataFormat::LoadDataValue(const pugi::xml_node& xmlNode, DataValue& parentValue)
//         {
//             DataValue& DataValue = parentValue.AddMember(DataValue(xmlNode.name(), DataValue.GetDocument()));
//             DataValue.Data() = (wchar_t*)xmlNode.child_value();
// 
//             for (pugi::xml_attribute_iterator it = xmlNode.attributes_begin(); it != xmlNode.attributes_end(); ++it)
//             {
//                 DataValue* newNode = mnew DataValue(it->name());
//                 newNode->Data() = (wchar_t*)it->value();
//                 DataValue.AddMember(newNode);
//             }
// 
//             for (auto node:xmlNode)
//             {
//                 if (node.type() != pugi::node_element)
//                     continue;
// 
//                 DataValue* newNode = mnew DataValue();
//                 LoadDataValue(node, *newNode);
//                 DataValue.AddMember(newNode);
//             }
//         }
// 
//         String SaveDataDoc(const DataValue& node)
//         {
//             struct xmlWriter :public pugi::xml_writer
//             {
//                 size_t size = 1024;
//                 size_t length = 0;
//                 char* data = new char[1024];
// 
//                 ~xmlWriter()
//                 {
//                     delete[] data;
//                 }
// 
//                 void write(const void* data, size_t size)
//                 {
//                     if (length + size > this->size)
//                     {
//                         char* newData = new char[this->size*2];
//                         memcpy(newData, this->data, this->size);
//                         delete[] this->data;
//                         this->data = newData;
//                         this->size *= 2;
//                     }
// 
//                     memcpy(this->data + length, data, size);
//                     length += size;
//                 }
//             };
// 
//             String res;
//             pugi::xml_document xmlDoc;
// 
//             for (auto docNode:node)
//             {
//                 pugi::xml_node newNode = xmlDoc.append_child(docNode->GetName().Data());
// 
//                 if (!docNode->Data().IsEmpty())
//                     newNode.append_child(pugi::node_pcdata).set_value((const wchar_t*)docNode->Data());
// 
//                 SaveDataValue(newNode, *docNode);
//             }
// 
//             xmlWriter writer;
//             xmlDoc.save(writer);
// 
//             writer.data[writer.length] = '\0';
// 
//             return String(writer.data);
//         }
// 
//         void SaveDataValue(pugi::xml_node& xmlNode, const DataValue& DataValue)
//         {
//             for (auto docNode : DataValue)
//             {
//                 if (docNode->GetChildNodes().Count() == 0 && false)
//                 {
//                     pugi::xml_attribute newAttribute = xmlNode.append_attribute((pugi::char_t*)docNode->GetName().Data());
//                     newAttribute.set_value((const wchar_t*)docNode->Data());
//                 }
//                 else
//                 {
//                     pugi::xml_node newNode = xmlNode.append_child((pugi::char_t*)docNode->GetName().Data());
// 
//                     if (!docNode->Data().IsEmpty())
//                         newNode.append_child(pugi::node_pcdata).set_value((const wchar_t*)docNode->Data());
// 
//                     SaveDataValue(newNode, *docNode);
//                 }
//             }
//         }
//     }
//}
