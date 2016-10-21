#pragma once

#include "Utils/ITree.h"
#include "Utils/Serializable.h"
#include "Utils/UID.h"

namespace o2
{
	// -----------------
	// Asset information
	// -----------------
	struct AssetInfo: public virtual ISerializable
	{
	public:
		Type::Id mType; // Type of asset @SERIALIZABLE
		String   mPath; // Path of asset @SERIALIZABLE
		UID      mId;   // Id of asset @SERIALIZABLE

	public:
		// Default constructor
		AssetInfo();

		// Constructor
		AssetInfo(const String& path, UID id, Type::Id type);

		// Check equal operator
		bool operator==(const AssetInfo& other) const;

		SERIALIZABLE(AssetInfo);
	};
	typedef Vector<AssetInfo> AssetInfosVec;
}
