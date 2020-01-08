#pragma once

#include "3rdPartyLibs/pugixml/pugixml.hpp"
#include "o2/Utils/Serialization/DataNode.h"
#include "o2/Utils/Types/String.h"

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
