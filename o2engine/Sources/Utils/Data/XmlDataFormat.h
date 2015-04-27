#pragma once

#include "dependencies/pugixml/pugixml.hpp"
#include "Utils/String.h"
#include "Utils/Data/DataDoc.h"

namespace o2
{
	namespace XmlDataFormat
	{
		bool LoadDataDoc(const String& data, DataDoc& doc);
		void LoadDataNode(const pugi::xml_node& xmlNode, DataNode& dataNode);

		String SaveDataDoc(const DataDoc& doc);
		void SaveDataNode(pugi::xml_node& xmlNode, const DataNode& dataNode);
	}
}