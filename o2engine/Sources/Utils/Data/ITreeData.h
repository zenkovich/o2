#pragma once

#include "Utils/Containers/Array.h"
#include "Utils/String.h"

namespace o2
{
	class IDataNode
	{
	public:
		virtual ~IDataNode();

		String GetName() const;
	};
}