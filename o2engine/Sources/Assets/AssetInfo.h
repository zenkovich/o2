#pragma once

#include "Utils\Serializable.h"
#include "Utils\Tree.h"

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
		AssetId  mId;   // Id of asset @SERIALIZABLE

	public:
		// Default constructor
		AssetInfo();

		// Constructor
		AssetInfo(const String& path, AssetId id, Type::Id type);

		// Check equal operator
		bool operator==(const AssetInfo& other) const;

		SERIALIZABLE(AssetInfo);
	};
	typedef Vector<AssetInfo> AssetInfosVec;
}
