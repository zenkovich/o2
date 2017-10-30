#pragma once

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedValue.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Assets/Asset.h"

namespace o2
{
	// ---------------
	// Animation asset
	// ---------------
	class AnimationAsset: public Asset
	{
	public:
		class MetaInfo;

	public:
		Animation         animation; // Asset data
		Getter<MetaInfo*> meta;      // Meta information getter

		// Destructor
		~AnimationAsset();

		// Check equals operator
		AnimationAsset& operator=(const AnimationAsset& asset);

		// Check equals operator
		bool operator==(const AnimationAsset& other) const;

		// Check not equals operator
		bool operator!=(const AnimationAsset& other) const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		SERIALIZABLE(AnimationAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			// Returns asset type id
			const Type* GetAssetType() const;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		// Default constructor
		AnimationAsset();

		// Constructor by path - loads asset by path
		AnimationAsset(const String& path);

		// Constructor by id - loads asset by id
		AnimationAsset(UID id);

		// Copy-constructor
		AnimationAsset(const AnimationAsset& asset);

		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Initializes properties
		void InitializeProperties();

		friend class Assets;
	};

	// -------------------------
	// Animation Asset reference
	// -------------------------
	class AnimationAssetRef: public AssetRef
	{
	public:
		// Creates AnimationAsset and returns reference to it
		static AnimationAssetRef CreateAsset();

		// Default constructor, references to null
		AnimationAssetRef(): AssetRef() {}

		// Copy-constructor
		AnimationAssetRef(const AssetRef& other): AssetRef(other) { CheckType<AnimationAsset>(); }

		// Copy-constructor
		AnimationAssetRef(const AnimationAssetRef& other): AssetRef(other) {}

		// Constructor from asset path
		AnimationAssetRef(const String& path): AssetRef(path) {}

		// Constructor from asset id
		AnimationAssetRef(UID id): AssetRef(id) {}

		// Destructor
		~AnimationAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		AnimationAssetRef& operator=(const AnimationAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		AnimationAsset& operator*() { return *((AnimationAsset*)mAssetPtr); }

		// Constant getter operator
		const AnimationAsset& operator*() const { return *((AnimationAsset*)mAssetPtr); }

		// Asset members and field operator
		AnimationAsset* operator->() { return ((AnimationAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const AnimationAsset* operator->() const { return ((AnimationAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const AnimationAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const AnimationAssetRef& other) const { return AssetRef::operator!=(other); }

		// Returns asset type
		const Type& GetAssetType() const { return TypeOf(AnimationAsset); }

		SERIALIZABLE(AnimationAssetRef);

	protected:
		// Constructor for Assets manager
		AnimationAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}

CLASS_BASES_META(o2::AnimationAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::AnimationAsset)
{
	PUBLIC_FIELD(animation);
	PUBLIC_FIELD(meta);
}
END_META;
CLASS_METHODS_META(o2::AnimationAsset)
{

	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_BASES_META(o2::AnimationAssetRef)
{
	BASE_CLASS(o2::AssetRef);
}
END_META;
CLASS_FIELDS_META(o2::AnimationAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::AnimationAssetRef)
{

	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_BASES_META(o2::AnimationAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);
}
END_META;
CLASS_FIELDS_META(o2::AnimationAsset::MetaInfo)
{
}
END_META;
CLASS_METHODS_META(o2::AnimationAsset::MetaInfo)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
}
END_META;
