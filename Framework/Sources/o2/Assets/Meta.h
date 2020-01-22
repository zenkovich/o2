#pragma once

#include "o2/Utils/Types/UID.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	// --------------------------------------
	// Basic asset meta information interface
	// --------------------------------------
	class AssetMeta : public ISerializable
	{
		friend class Asset;

		UID mId; // Id of asset @SERIALIZABLE

	public:
		// Default constructor
		AssetMeta();

		// Virtual destructor
		virtual ~AssetMeta();

		// Returns asset type id
		virtual const Type* GetAssetType() const;

		// Returns true if other meta is equal to this
		virtual bool IsEqual(AssetMeta* other) const;

		// Returns asset id
		const UID& ID() const;

		SERIALIZABLE(AssetMeta);
	};
}
