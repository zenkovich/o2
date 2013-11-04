#ifndef XML_TOOLS_H
#define XML_TOOLS_H

#include "public.h"

#include "pugixml/pugixml.hpp"

#include "util/math/vector2.h"
#include "util/math/rect.h"

OPEN_O2_NAMESPACE

struct cXmlTools
{
	static bool loadFromFile(const std::string& fileName, pugi::xml_document& xmlDoc);
	static pugi::xml_node getNode(const pugi::xml_node& node, const std::string& path);
	static vec2f node2vecf(const pugi::xml_node& node);
	static vec2i node2veci(const pugi::xml_node& node);
	static fRect node2rectf(const pugi::xml_node& node);
};

CLOSE_O2_NAMESPACE

#endif //XML_TOOLS_H