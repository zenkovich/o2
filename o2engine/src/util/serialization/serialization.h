#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "public.h"
#include "pugixml/pugixml.hpp"
#include "util/file_system/file.h"
#include "util/math/color.h"
#include "util/math/rect.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

class cLogStream;

/** Type if serialization process. */
struct cSerializeType
{
	enum v { INPUT = 0, OUTPUT };
};

/** Serializable object interface. */
struct cSerializableObj
{
	virtual bool serialize(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log = NULL) = 0;
};

/** Serialize interfaces. */
struct cSerialization
{
	/** Loading xml data to @doc from file. Puts errors in @log, if @log is null - using global gLog. */
	static bool loadData(pugi::xml_document& doc, const std::string& fileName, 
		                 cFileType::value fileType = cFileType::FT_FILE, cLogStream* log = NULL);

	/** Loading xml data to @doc from file. Puts errors in @log, if @log is null - using global gLog. */
	static bool loadData(pugi::xml_document& doc, cInFile& file, cLogStream* log = NULL);

	/** Saving xml data to @doc from file. Puts errors in @log, if @log is null - using global gLog. */
	static bool saveData(pugi::xml_document& doc, const std::string& fileName, cFileType::value fileType = cFileType::FT_FILE);

	/** Saving xml data to @doc from file. Puts errors in @log, if @log is null - using global gLog. */
	static bool saveData(pugi::xml_document& doc, cOutFile& file);

//NOTE: Do not use that functions !
	static bool serializeIn(pugi::xml_node& xmlNode, const std::string& id, cSerializableObj* obj, cLogStream* log = NULL);	
	static bool serializeIn(pugi::xml_node& xmlNode, const std::string& id, int& obj, cLogStream* log = NULL);	
	static bool serializeIn(pugi::xml_node& xmlNode, const std::string& id, float& obj, cLogStream* log = NULL);	
	static bool serializeIn(pugi::xml_node& xmlNode, const std::string& id, bool& obj, cLogStream* log = NULL);	
	static bool serializeIn(pugi::xml_node& xmlNode, const std::string& id, std::string& obj, cLogStream* log = NULL);	
	static bool serializeIn(pugi::xml_node& xmlNode, const std::string& id, vec2i& obj, cLogStream* log = NULL);	
	static bool serializeIn(pugi::xml_node& xmlNode, const std::string& id, vec2f& obj, cLogStream* log = NULL);	
	static bool serializeIn(pugi::xml_node& xmlNode, const std::string& id, fRect& obj, cLogStream* log = NULL);	
	static bool serializeIn(pugi::xml_node& xmlNode, const std::string& id, iRect& obj, cLogStream* log = NULL);	
	static bool serializeIn(pugi::xml_node& xmlNode, const std::string& id, color4& obj, cLogStream* log = NULL);
	
	static bool serializeOut(pugi::xml_node& xmlNode, const std::string& id, cSerializableObj* obj, cLogStream* log = NULL);	
	static bool serializeOut(pugi::xml_node& xmlNode, const std::string& id, int obj, cLogStream* log = NULL);	
	static bool serializeOut(pugi::xml_node& xmlNode, const std::string& id, float obj, cLogStream* log = NULL);	
	static bool serializeOut(pugi::xml_node& xmlNode, const std::string& id, bool obj, cLogStream* log = NULL);	
	static bool serializeOut(pugi::xml_node& xmlNode, const std::string& id, std::string& obj, cLogStream* log = NULL);	
	static bool serializeOut(pugi::xml_node& xmlNode, const std::string& id, vec2i& obj, cLogStream* log = NULL);	
	static bool serializeOut(pugi::xml_node& xmlNode, const std::string& id, vec2f& obj, cLogStream* log = NULL);	
	static bool serializeOut(pugi::xml_node& xmlNode, const std::string& id, fRect& obj, cLogStream* log = NULL);	
	static bool serializeOut(pugi::xml_node& xmlNode, const std::string& id, iRect& obj, cLogStream* log = NULL);	
	static bool serializeOut(pugi::xml_node& xmlNode, const std::string& id, color4& obj, cLogStream* log = NULL);

	template<typename T> 
	static bool serialize(pugi::xml_node& xmlNode, const std::string& id, T& obj, cSerializeType::v type, 
		                  cLogStream* log = NULL)
	{
		if (type == cSerializeType::INPUT)
			return serializeIn(xmlNode, id, obj, log);

		return serializeOut(xmlNode, id, obj, log);
	}
	
	template<typename T>
	static bool serializeArrIn(pugi::xml_node& xmlNode, const std::string& id, T* arr, int count, cLogStream* log = NULL)
	{
		bool res = true;

		pugi::xml_node node;
		if (getNode(node, xmlNode, id))
		{
			for (int i = 0; i < count; i++)
			{
				char vId[32]; sprintf(vId, "v%i", i);
				std::string sv = vId;
				if (!serializeIn(node, sv, arr[i]))
					res = false;
			}
		}
		else
			res = false;

		return res;
	}
	
	template<typename T>
	static bool serializeArrOut(pugi::xml_node& xmlNode, const std::string& id, T* arr, int count, cLogStream* log = NULL)
	{
		bool res = true;

		pugi::xml_node node = xmlNode.append_child(id.c_str());
		for (int i = 0; i < count; i++)
		{
			char vId[32]; sprintf(vId, "v%i", i);
			if (!serializeOut(node, vId, arr[i]))
				res = false;
		}

		return res;
	}

	template<typename T> 
	static bool serializeArr(pugi::xml_node& xmlNode, const char* id, T* arr, int count, cSerializeType::v type, 
		                     cLogStream* log = NULL)
	{
		std::string sid = id;

		if (type == cSerializeType::INPUT)
			return serializeArrIn(xmlNode, sid, arr, count, log);

		return serializeArrOut(xmlNode, sid, arr, count, log);
	}

protected:
	static bool getNode(pugi::xml_node& node, const pugi::xml_node& parent, const std::string& id);
};

/** Declare serialization method. */
#define SERIALIZE_METHOD_DECL() \
	virtual bool serialize(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log = NULL)

/** Implementation of serialize method. You must define class. */
#define SERIALIZE_METHOD_IMPL(CLASS) \
	bool CLASS::serialize(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log)

/** Declare serialize method for inherited class. You must define parent and target class. */
#define SERIALIZE_INHERITED_METHOD_DECL(BASIC_CLASS, CLASS)                                             \
	virtual bool serializeInh(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log = NULL); \
	virtual bool serialize(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log = NULL)     \
	{                                                                                                   \
	    pugi::xml_node basicClassNode = (type == cSerializeType::INPUT) ? (xmlNode.child(#BASIC_CLASS)):(xmlNode.append_child(#BASIC_CLASS));                             \
		if (!BASIC_CLASS::serialize(basicClassNode, type, log)) return false;                           \
		return CLASS::serializeInh(xmlNode, type, log);                                                 \
	}

/** Implementation of serialize method for inherited class. */
#define SERIALIZE_INHERITED_METHOD_IMPL(CLASS) \
	bool CLASS::serializeInh(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log)       

/** Serialize object with ID. */
#define SERIALIZE_ID(obj, id) cSerialization::serialize(xmlNode, id, obj, type, log)

/** Serialize @obj with name "obj". */
#define SERIALIZE(obj) SERIALIZE_ID(obj, #obj)      

/** Serialize array of object with ID. */
#define SERIALIZE_ARR_ID(obj, count, id) cSerialization::serializeArr(xmlNode, id, obj, count, type, log)

/** Serialize array of objects with name "obj". */
#define SERIALIZE_ARR(obj, count) SERIALIZE_ARR_ID(obj, count, #obj)


CLOSE_O2_NAMESPACE

#endif //SERIALIZATION_H