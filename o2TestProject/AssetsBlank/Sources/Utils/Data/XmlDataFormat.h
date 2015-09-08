#pragma once

#include "dependencies/pugixml/pugixml.hpp"
#include "Utils/Data/DataDoc.h"
#include "Utils/String.h"

namespace o2
{
	namespace XmlDataFormat
	{
		bool LoadDataDoc(const WString& data, DataNode& node);
		void LoadDataNode(const pugi::xml_node& xmlNode, DataNode& dataNode);

		String SaveDataDoc(const DataNode& node);
		void SaveDataNode(pugi::xml_node& xmlNode, const DataNode& dataNode);
	}
}