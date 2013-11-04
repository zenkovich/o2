#include "serialization.h"

#include "util/log/log_stream.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

bool cSerialization::loadData( pugi::xml_document& doc, const std::string& fileName, 
	                           cFileType::value fileType /*= cFileType::FT_FILE*/, cLogStream* log /*= NULL*/ )
{
	cInFile file(fileName, fileType);

	if (!file.isOpened())
	{
		cLogStream* nlog = log;
		if (!nlog)
			nlog = gLog;

		nlog->out("ERROR: Failed to open data file: '%s'", fileName.c_str());
		return false;
	}

	return loadData(doc, file, log);
}

bool cSerialization::loadData( pugi::xml_document& doc, cInFile& file, cLogStream* log /*= NULL*/ )
{
	if (!file.isOpened())
		return false;

	uint64 size = file.getDataSize();
	char* buf = new char[(unsigned int)size];
	file.readFullData((void*)buf);

	pugi::xml_parse_result res = doc.load_buffer(buf, (unsigned int)size);
	if (res.status != pugi::status_ok)
	{
		cLogStream* nlog = log;
		if (!nlog)
			nlog = gLog;

		nlog->out("ERROR: Failed to load config file '%s': %s", file.getFilename().c_str(), res.description());

		return false;
	}

	return true;
}

bool cSerialization::saveData( pugi::xml_document& doc, const std::string& fileName, cFileType::value fileType /*= cFileType::FT_FILE*/ )
{
	cOutFile file(fileName, fileType);
	return saveData(doc, file);
}

bool cSerialization::saveData( pugi::xml_document& doc, cOutFile& file )
{
	struct xmlWriter:public pugi::xml_writer
	{
		cOutFile* file;

		void write(const void* data, size_t size)
		{
			file->writeData(data, size);
		}
	};

	xmlWriter writer;
	writer.file = &file;
	doc.save(writer);

	file.close();

	return true;
}

bool cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, cSerializableObj* obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id))
	{
		return obj->serialize(chNode, cSerializeType::INPUT, log);
	}

	return false;
}

bool cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, int& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id))
	{
		obj = chNode.attribute("v").as_int();

		return true;
	}

	return false;
}

bool cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, float& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id))
	{
		obj = chNode.attribute("v").as_float();

		return true;
	}

	return false;
}

bool cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, std::string& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id))
	{
		obj = chNode.attribute("v").value();

		return true;
	}

	return false;
}

bool cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, vec2i& obj, cLogStream* log /*= NULL*/ )
{	
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id))
	{
		obj.x = chNode.attribute("x").as_int();
		obj.y = chNode.attribute("y").as_int();

		return true;
	}

	return false;
}

bool cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, vec2f& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id))
	{
		obj.x = chNode.attribute("x").as_float();
		obj.y = chNode.attribute("y").as_float();

		return true;
	}

	return false;
}

bool cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, fRect& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id))
	{
		obj.left = chNode.attribute("left").as_float();
		obj.top = chNode.attribute("top").as_float();
		obj.right = chNode.attribute("right").as_float();
		obj.down = chNode.attribute("bottom").as_float();

		return true;
	}

	return false;
}

bool cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, iRect& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id))
	{
		obj.left = chNode.attribute("left").as_int();
		obj.top = chNode.attribute("top").as_int();
		obj.right = chNode.attribute("right").as_int();
		obj.down = chNode.attribute("bottom").as_int();

		return true;
	}

	return false;
}

bool cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, color4& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id))
	{
		float r, g, b, a;
		r = chNode.attribute("r").as_float();
		g = chNode.attribute("g").as_float();
		b = chNode.attribute("b").as_float();
		a = chNode.attribute("a").as_float();
		obj = color4(r, g, b, a);

		return true;
	}

	return false;
}

bool cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, bool& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id))
	{
		std::string value = chNode.attribute("v").value();
		obj = value == "true" || value == "TRUE" || value == "YES" || value == "1";

		return true;
	}

	return false;
}

bool cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, cSerializableObj* obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	return obj->serialize(xmlNode, cSerializeType::INPUT, log);
}

bool cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, int obj, cLogStream* log /*= NULL*/ )
{	
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("v") = obj;

	return true;
}

bool cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, float obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("v") = obj;

	return true;
}

bool cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, std::string& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("v") = obj.c_str();

	return true;
}

bool cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, vec2i& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("x") = obj.x;
	chNode.append_attribute("y") = obj.y;

	return true;
}

bool cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, vec2f& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("x") = obj.x;
	chNode.append_attribute("y") = obj.y;

	return true;
}

bool cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, fRect& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("left") = obj.left;
	chNode.append_attribute("top") = obj.top;
	chNode.append_attribute("right") = obj.right;
	chNode.append_attribute("bottom") = obj.down;

	return true;
}

bool cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, iRect& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("left") = obj.left;
	chNode.append_attribute("top") = obj.top;
	chNode.append_attribute("right") = obj.right;
	chNode.append_attribute("bottom") = obj.down;

	return true;
}

bool cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, color4& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("r") = obj.rf();
	chNode.append_attribute("g") = obj.gf();
	chNode.append_attribute("b") = obj.bf();
	chNode.append_attribute("a") = obj.af();

	return true;
}

bool cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, bool obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("v") = obj ? "true":"false";

	return true;
}

bool cSerialization::getNode( pugi::xml_node& node, const pugi::xml_node& parent, const std::string& id )
{
	node = parent.child(id.c_str());
	return !node.empty();
}

CLOSE_O2_NAMESPACE
