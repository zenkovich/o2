#pragma once

#include "Utils\Serialization.h"

namespace o2
{
	// -----------------
	// Asset information
	// -----------------
	struct AssetInfo: ISerializable
	{
		typedef Vector<AssetInfo> AssetInfosVec;

	public:
		Type::Id      mType;   // Type of asset
		String        mPath;   // Path of asset
		UInt          mId;     // Id of asset
		AssetInfosVec mChilds; // Children assets

	public:
		// Default constructor
		AssetInfo();

		// Constructor
		AssetInfo(const String& path, UInt id, Type::Id type);

		// Check equal operator
		bool operator==(const AssetInfo& other) const;

		// Returns asset by path. Returns default asset information if no asset in path
		AssetInfo FindAssetInfo(const String& path) const;

		// Returns asset by id. Returns default asset information if no asset with id
		AssetInfo FindAssetInfo(UInt id) const;

		SERIALIZABLE_IMPL(AssetInfo);

		IOBJECT(AssetInfo)
		{
			SRLZ_FIELD(mType);
			SRLZ_FIELD(mPath);
			SRLZ_FIELD(mId);
			SRLZ_FIELD(mChilds);
		}
	};
	typedef Vector<AssetInfo> AssetInfosVec;
}
