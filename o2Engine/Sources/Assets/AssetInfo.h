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
		void OnSerialize(DataNode& node) const;

		// Completion deserialization callback, reads asset type name
		void OnDeserialized(const DataNode& node);
	};
	typedef Vector<AssetInfo> AssetInfosVec;
}
