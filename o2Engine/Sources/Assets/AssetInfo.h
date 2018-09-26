#pragma once

#include "Utils/Basic/ITree.h"
#include "Utils/Serialization/Serializable.h"
#include "Utils/Types/UID.h"

namespace o2
{
	// -----------------
	// Asset information
	// -----------------
	struct AssetInfo: public ISerializable
	{
	public:
		const Type* assetType; // Type of asset
		String      path;      // Path of asset @SERIALIZABLE
		UID         id;        // Id of asset @SERIALIZABLE

	public:
		// Default constructor
		AssetInfo();

		// Constructor
		AssetInfo(const String& path, UID id, const Type* type);

		// Check equal operator
		bool operator==(const AssetInfo& other) const;

		SERIALIZABLE(AssetInfo);

	protected:
		// Beginning serialization callback, writes asset type name
		void OnSerialize(DataNode& node) const override;

		// Completion deserialization callback, reads asset type name
		void OnDeserialized(const DataNode& node) override;
	};
	typedef Vector<AssetInfo> AssetInfosVec;
}

CLASS_BASES_META(o2::AssetInfo)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetInfo)
{
	PUBLIC_FIELD(assetType);
	PUBLIC_FIELD(path).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(id).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AssetInfo)
{

	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
}
END_META;
