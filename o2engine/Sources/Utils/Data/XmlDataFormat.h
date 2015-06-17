#pragma once

#include "dependencies/pugixml/pugixml.hpp"
#include "Utils/Data/DataDoc.h"
#include "Utils/String.h"

namespace o2
{
	namespace XmlDataFormat
	{
		bool LoadDataDoc(const WString& data, DataDoc& doc);
		void LoadDataNode(const pugi::xml_node& xmlNode, DataNode& dataNode);

		String SaveDataDoc(const DataDoc& doc);
		void SaveDataNode(pugi::xml_node& xmlNode, const DataNode& dataNode);
	}
}