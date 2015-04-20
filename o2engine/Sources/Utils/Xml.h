#pragma once

#include "PugiXml/pugixml.hpp"
#include "Utils/String.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Color.h"
#include "Utils/Time.h"

namespace o2
{
	namespace Xml
	{
		/** Loads xml file. */
		bool LoadFromFile(const String& fileName, pugi::xml_document& xmlDoc);

		/** Loads xml from buffer. */
		bool LoadFromString(const String& data, pugi::xml_document& xmlDoc);

		/** Loads xml file. */
		bool SaveToFile(const String& fileName, pugi::xml_document& xmlDoc);

		/** Loads xml buffer. */
		bool SaveToString(String& data, pugi::xml_document& xmlDoc);

		/** Returns node by path, Path format "node/node/node". */
		pugi::xml_node GetNode(const pugi::xml_node& node, const String& path);


		/** Saving data from object to xml node. */
		void ToXmlNode(int            object, pugi::xml_node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(unsigned int   object, pugi::xml_node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(float          object, pugi::xml_node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(String&        object, pugi::xml_node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(bool           object, pugi::xml_node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(Vec2F&         object, pugi::xml_node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(Vec2I&         object, pugi::xml_node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(RectF&         object, pugi::xml_node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(RectI&         object, pugi::xml_node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(Color4&        object, pugi::xml_node& node);

		/** Saving data from object to xml node. */
		void ToXmlNode(WideTime&      object, pugi::xml_node& node);


		/** Getting data to object from xml node. */
		void FromXmlNode(int&           object, pugi::xml_node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(unsigned int&  object, pugi::xml_node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(float&         object, pugi::xml_node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(String&        object, pugi::xml_node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(bool&          object, pugi::xml_node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(Vec2F&         object, pugi::xml_node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(Vec2I&         object, pugi::xml_node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(RectF&         object, pugi::xml_node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(RectI&         object, pugi::xml_node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(Color4&        object, pugi::xml_node& node);

		/** Getting data to object from xml node. */
		void FromXmlNode(WideTime&      object, pugi::xml_node& node);

		/** Returns specific data from xml node. */
		template<typename T>
		T FromXmlNode(pugi::xml_node& node)
		{
			T res; FromXmlNode(res, node);
			return res;
		}

		/** Saving data from object to xml node. */
		template<typename T>
		void ToXmlNode(T* array, int count, pugi::xml_node& node)
		{
			node.append_attribute("count") = count;
			for (int i = 0; i < count; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				pugi::xml_node elemNode = node.append_child(elemNodeName);

				ToXmlNode(array[i], elemNode);
			}
		}

		/** Getting data to object from xml node. */
		template<typename T>
		void FromXmlNode(T* array, int count, pugi::xml_node& node)
		{
			int srCount = node.attribute("count").as_int();
			for (int i = 0; i < count && i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				pugi::xml_node elemNode = node.child(elemNodeName);

				if (elemNode)
					ToXmlNode(array[i], elemNode);
			}
		}
	}
}