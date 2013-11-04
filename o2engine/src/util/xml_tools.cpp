#include "xml_tools.h"

#include "util/file_system/file.h"

OPEN_O2_NAMESPACE

bool cXmlTools::loadFromFile( const std::string& fileName, pugi::xml_document& xmlDoc )
{
	cInFile inFile(fileName, cFileType::FT_CONFIG);

	if (!inFile.isOpened())
		return false;

	uint32 dataSize = inFile.getDataSize();
	char* buffer = mnew char[dataSize];

	inFile.readFullData(buffer);

	pugi::xml_parse_result res = xmlDoc.load_buffer(buffer, dataSize);

	safe_release_arr(buffer);

	return res.status == pugi::status_ok;
}

pugi::xml_node cXmlTools::getNode( const pugi::xml_node& node, const std::string& path )
{
	int slashPos = path.find('/');
	std::string currNode = path.substr(0, slashPos);
	
	if (slashPos == path.npos)
		return node.child(currNode.c_str());
	else
		return getNode(node.child(currNode.c_str()), path.substr(slashPos + 1));
}

vec2i cXmlTools::node2veci( const pugi::xml_node& node )
{
	return vec2i(node.attribute("x").as_int(), node.attribute("y").as_int());
}

vec2f cXmlTools::node2vecf( const pugi::xml_node& node )
{
	return vec2f(node.attribute("x").as_float(), node.attribute("y").as_float());
}

fRect cXmlTools::node2rectf( const pugi::xml_node& node )
{
	return fRect(node.attribute("left").as_float(), node.attribute("top").as_float(),
		         node.attribute("right").as_float(), node.attribute("bottom").as_float());
}

CLOSE_O2_NAMESPACE