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
	public:
		// Default constructor
		AssetMeta();

		// Virtual destructor
		virtual ~AssetMeta();

		// Returns asset id
		const UID& ID() const;

		// Returns asset type id
		virtual const Type* GetAssetType() const;

		// Returns true if other meta is equal to this
		virtual bool IsEqual(AssetMeta* other) const;

		SERIALIZABLE(AssetMeta);

	private:
		UID mId; // Id of asset @SERIALIZABLE

		friend class Asset;
	};
	
	// -----------------------------------------------
	// Asset meta specialization. Use it as base class
	// -----------------------------------------------
	template<typename T>
	class DefaultAssetMeta : public AssetMeta
	{
	public:
		// Returns asset type id
		const Type* GetAssetType() const override { return &TypeOf(T); }

		SERIALIZABLE(DefaultAssetMeta<T>);
	};
}

CLASS_BASES_META(o2::AssetMeta)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AssetMeta)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(mId).PRIVATE();
}
END_META;
CLASS_METHODS_META(o2::AssetMeta)
{

	PUBLIC_FUNCTION(const UID&, ID);
	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(bool, IsEqual, AssetMeta*);
}
END_META;

META_TEMPLATES(typename T)
CLASS_BASES_META(o2::DefaultAssetMeta<T>)
{
	BASE_CLASS(o2::AssetMeta);
}
END_META;
META_TEMPLATES(typename T)
CLASS_FIELDS_META(o2::DefaultAssetMeta<T>)
{
}
END_META;
META_TEMPLATES(typename T)
CLASS_METHODS_META(o2::DefaultAssetMeta<T>)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
}
END_META;
