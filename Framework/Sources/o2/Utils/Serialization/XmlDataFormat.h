#pragma once

#include "3rdPartyLibs/pugixml/pugixml.hpp"
#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	namespace XmlDataFormat
	{
		bool LoadDataDoc(const WString& data, DataValue& node);
		void LoadDataNode(const pugi::xml_node& xmlNode, DataValue& dataNode);

		String SaveDataDoc(const DataValue& node);
		void SaveDataNode(pugi::xml_node& xmlNode, const DataValue& dataNode);
	}
}
