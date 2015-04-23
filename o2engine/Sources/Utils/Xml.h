#pragma once

#include "Dependencies/PugiXml/pugixml.hpp"
#include "Utils/String.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Color.h"
#include "Utils/Time.h"
#include "Utils/Containers/Array.h"

namespace o2
{
	namespace Xml
	{
		typedef pugi::xml_document Document;
		typedef pugi::xml_node Node;
		typedef pugi::xml_attribute Attribute;

		/** Loads xml file. */
		bool LoadFromFile(const TString& fileName, Document& xmlDoc);

		/** Loads xml from buffer. */
		bool LoadFromString(const TString& data, Document& xmlDoc);

		/** Loads xml file. */
		bool SaveToFile(const TString& fileName, Document& xmlDoc);

		/** Loads xml buffer. */
		bool SaveToString(TString& data, Document& xmlDoc);

		/** Returns node by path, Path format "node/node/node". */
		Node GetNode(const Node& node, const TString& path);


		/** Saving data from object to xml node. */
		void ToXmlNode(int           object, Node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(UInt          object, Node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(float         object, Node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(TString&       object, Node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(bool          object, Node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(Vec2F&        object, Node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(Vec2I&        object, Node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(RectF&        object, Node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(RectI&        object, Node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(Color4&       object, Node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(WideTime&     object, Node& node);

		/** Returns specific data from xml node. */
		template<typename T>
		T FromXmlNode(Node& node)
		{
			T res; FromXmlNode(res, node);
			return res;
		}

		/** Getting data to object from xml node. */
		void FromXmlNode(int&           object, Node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(unsigned int&  object, Node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(float&         object, Node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(TString&        object, Node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(bool&          object, Node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(Vec2F&         object, Node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(Vec2I&         object, Node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(RectF&         object, Node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(RectI&         object, Node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(Color4&        object, Node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(WideTime&      object, Node& node);


		/** Saving data from object to xml node. */
		template<typename T>
		static void ToXmlNode(T* array, int count, Node& node)
		{
			node.append_attribute("count") = count;
			for (int i = 0; i < count; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				Node elemNode = node.append_child(elemNodeName);

				ToXmlNode(array[i], elemNode);
			}
		}

		/** Saving data from object to xml node. */
		template<typename T>
		static void ToXmlNode(Array<T>& array, Node& node)
		{
			node.append_attribute("count") = array.Count();
			for (int i = 0; i < (int)array.Count(); i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				Node elemNode = node.append_child(elemNodeName);

				ToXmlNode(array[i], elemNode);
			}
		}

		/** Saving data from object to xml node. */
		template<typename T>
		static void ToXmlNode(Array<T*>& array, Node& node)
		{
			node.append_attribute("count") = array.size();
			for (int i = 0; i < (int)array.size(); i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				Node elemNode = node.append_child(elemNodeName);

				ToXmlNode(array[i], elemNode);
			}
		}


		/** Getting data to object from xml node. */
		template<typename T>
		static void FromXmlNode(T*& array, int& count, Node& node)
		{
			int srCount = node.attribute("count").as_int();

			array = new T[srCount];
			count = srCount;

			for (int i = 0; i < count && i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				Node elemNode = node.child(elemNodeName);

				if (elemNode)
					FromXmlNode(array[i], elemNode);
			}
		}


		/** Getting data to object from xml node. */
		template<typename T>
		static void FromXmlNode(Array<T>& array, Node& node)
		{
			array.Clear();
			int srCount = node.attribute("count").as_int();
			for (int i = 0; i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				Node elemNode = node.child(elemNodeName);

				if (elemNode)
				{
					T elem = T();
					FromXmlNode(elem, elemNode);
					array.Add(elem);
				}
				else break;
			}
		}


		/** Getting data to object from xml node. */
		template<typename T>
		static void FromXmlNode(Array<T*>& array, Node& node)
		{
			array.Clear();
			int srCount = node.attribute("count").as_int();
			for (int i = 0; i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				Node elemNode = node.child(elemNodeName);

				if (elemNode)
				{
					T* elem = mnew T();
					FromXmlNode(elem, elemNode);
					array.Add(elem);
				}
				else break;
			}
		}
	}
}