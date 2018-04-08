#pragma once

#include "dependencies/pugixml/pugixml.hpp"
#include "Utils/Serialization/DataNode.h"
#include "Utils/Types/String.h"

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
