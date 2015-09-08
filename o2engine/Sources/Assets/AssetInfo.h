#pragma once

#include "Utils\Serialization.h"
#include "Utils\Tree.h"

namespace o2
{
	// -----------------
	// Asset information
	// -----------------
	struct AssetInfo: public ISerializable
	{
	public:
		Type::Id mType; // Type of asset
		String   mPath; // Path of asset
		UInt     mId;   // Id of asset

	public:
		// Default constructor
		AssetInfo();

		// Constructor
		AssetInfo(const String& path, UInt id, Type::Id type);

		// Check equal operator
		bool operator==(const AssetInfo& other) const;

		SERIALIZABLE_IMPL(AssetInfo);

		IOBJECT(AssetInfo)
		{
			SRLZ_FIELD(mType);
			SRLZ_FIELD(mPath);
			SRLZ_FIELD(mId);
		}
	};
	typedef Vector<AssetInfo> AssetInfosVec;
}
