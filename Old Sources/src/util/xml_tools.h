#pragma once

#include "public.h"

#include "pugixml/pugixml.hpp"
#include "time_utils.h"

OPEN_O2_NAMESPACE

class Serializable;

/** Some helping functions for working with xml. */
struct XmlTools
{
	/** Loads xml file. */
	static bool LoadFromFile(const String& fileName, pugi::xml_document& xmlDoc, bool isConfigFile = true);

	/** Loads xml from buffer. */
	static bool LoadFromString(const String& data, pugi::xml_document& xmlDoc);

	/** Loads xml file. */
	static bool SaveToFile(const String& fileName, pugi::xml_document& xmlDoc, bool isConfigFile = true);

	/** Loads xml buffer. */
	static bool SaveToString(String& data, pugi::xml_document& xmlDoc);

	/** Returns node by path, Path format "node/node/node". */
	static pugi::xml_node GetNode(const pugi::xml_node& node, const String& path);


	/** Saving data from object to xml node. */
	static void ToXmlNode(int            object, pugi::xml_node& node);

	/** Saving data from object to xml node. */
	static void ToXmlNode(unsigned int   object, pugi::xml_node& node);

	/** Saving data from object to xml node. */
	static void ToXmlNode(float          object, pugi::xml_node& node);

	/** Saving data from object to xml node. */
	static void ToXmlNode(String&        object, pugi::xml_node& node);

	/** Saving data from object to xml node. */
	static void ToXmlNode(bool           object, pugi::xml_node& node);

	/** Saving data from object to xml node. */
	static void ToXmlNode(Vec2F&         object, pugi::xml_node& node);

	/** Saving data from object to xml node. */
	static void ToXmlNode(Vec2I&         object, pugi::xml_node& node);

	/** Saving data from object to xml node. */
	static void ToXmlNode(RectF&         object, pugi::xml_node& node);

	/** Saving data from object to xml node. */
	static void ToXmlNode(RectI&         object, pugi::xml_node& node);

	/** Saving data from object to xml node. */
	static void ToXmlNode(Color4&        object, pugi::xml_node& node);

	/** Saving data from object to xml node. */
	static void ToXmlNode(WideTime&      object, pugi::xml_node& node);


	/** Getting data to object from xml node. */
	static void FromXmlNode(int&           object, pugi::xml_node& node);

	/** Getting data to object from xml node. */
	static void FromXmlNode(unsigned int&  object, pugi::xml_node& node);

	/** Getting data to object from xml node. */
	static void FromXmlNode(float&         object, pugi::xml_node& node);

	/** Getting data to object from xml node. */
	static void FromXmlNode(String&        object, pugi::xml_node& node);

	/** Getting data to object from xml node. */
	static void FromXmlNode(bool&          object, pugi::xml_node& node);

	/** Getting data to object from xml node. */
	static void FromXmlNode(Vec2F&         object, pugi::xml_node& node);

	/** Getting data to object from xml node. */
	static void FromXmlNode(Vec2I&         object, pugi::xml_node& node);

	/** Getting data to object from xml node. */
	static void FromXmlNode(RectF&         object, pugi::xml_node& node);

	/** Getting data to object from xml node. */
	static void FromXmlNode(RectI&         object, pugi::xml_node& node);

	/** Getting data to object from xml node. */
	static void FromXmlNode(Color4&        object, pugi::xml_node& node);

	/** Getting data to object from xml node. */
	static void FromXmlNode(WideTime&      object, pugi::xml_node& node);

	/** Returns specific data from xml node. */
	template<typename T>
	static T Get(pugi::xml_node& node)
	{
		T res; FromXmlNode(res, node);
		return res;
	}

	/** Saving data from object to xml node. */
	template<typename T>
	static void ToXmlNode(T* array, int count, pugi::xml_node& node)
	{
		node.append_attribute("count") = count;
		for (int i = 0; i < count; i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.append_child(elemNodeName);

			ToXmlNode(array[i], elemNode);
		}
	}

	/** Saving data from object to xml node. */
	template<typename T>
	static void ToXmlNode(Vector<T>& array, pugi::xml_node& node)
	{
		node.append_attribute("count") = count;
		for (int i = 0; i < (int)array.size(); i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.append_child(elemNodeName);

			ToXmlNode(&(array[i]), elemNode);
		}
	}

	/** Saving data from object to xml node. */
	template<typename T>
	static void ToXmlNode(Vector<T*>& array, pugi::xml_node& node)
	{
		node.append_attribute("count") = array.size();
		for (int i = 0; i < (int)array.size(); i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.append_child(elemNodeName);

			ToXmlNode(array[i], elemNode);
		}
	}


	/** Getting data to object from xml node. */
	template<typename T>
	static void FromXmlNode(T* array, int count, pugi::xml_node& node)
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


	/** Getting data to object from xml node. */
	template<typename T>
	static void FromXmlNode(Vector<T>& array, pugi::xml_node& node)
	{
		int srCount = node.attribute("count").as_int();
		for (int i = 0; i < srCount; i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.child(elemNodeName);

			if (elemNode)
			{
				T elem;
				ToXmlNode(&elem, elemNode);
				array.push_back(elem);
			}
			else break;
		}
	}


	/** Getting data to object from xml node. */
	template<typename T>
	static void FromXmlNode(Vector<T*>& array, pugi::xml_node& node)
	{
		int srCount = node.attribute("count").as_int();
		for (int i = 0; i < srCount; i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.child(elemNodeName);

			if (elemNode)
			{
				T* elem = mnew T();
				ToXmlNode(elem, elemNode);
				array.push_back(elem);
			}
			else break;
		}
	}
};

CLOSE_O2_NAMESPACE