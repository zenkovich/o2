#pragma once

#include "Assets/Asset.h"
#include "Assets/AtlasAsset.h"
#include "Render/TextureRef.h"
#include "Utils/Bitmap.h"

namespace o2
{
	// -----------
	// Image asset
	// -----------
	class ImageAsset: public Asset
	{
	public:
		class MetaInfo;

	public:
		PROPERTIES(ImageAsset);
		PROPERTY(Bitmap*, bitmap, SetBitmap, GetBitmap);                // Bitmap data property
		PROPERTY(UID, atlasId, SetAtlasId, GetAtlasId);                 // Atlas owner id property
		PROPERTY(AtlasAssetRef, atlas, SetAtlas, GetAtlas);             // Atlas owner asset property
		PROPERTY(BorderI, sliceBorder, SetSliceBorder, GetSliceBorder); // Slice border
		GETTER(UInt, atlasPage, GetAtlasPage);                          // Atlas page index getter
		GETTER(RectI, atlasRect, GetAtlasRect);                         // Atlas source image rectangle getter
		GETTER(Vec2F, size, GetSize);                                   // Image size getter
		GETTER(float, width, GetWidth);                                 // Image width getter
		GETTER(float, height, GetHeight);                               // Image height getter
		GETTER(MetaInfo*, meta, GetMeta);                               // Meta information getter

		// Destructor
		~ImageAsset();

		// Check equals operator
		ImageAsset& operator=(const ImageAsset& asset);

		// Check equals operator
		bool operator==(const ImageAsset& other) const;

		// Check not equals operator
		bool operator!=(const ImageAsset& other) const;

		// Returns bitmap data
		Bitmap* GetBitmap();

		// Sets bitmap data
		void SetBitmap(Bitmap* bitmap);

		// Returns atlas id
		UID GetAtlasId() const;

		// Sets atlas id
		void SetAtlasId(UID id);

		// Returns atlas asset
		AtlasAssetRef GetAtlas() const;

		// Sets atlas
		void SetAtlas(const AtlasAssetRef& atlas);

		// Sets slice border
		void SetSliceBorder(const BorderI& border);

		// Returns slice border
		BorderI GetSliceBorder() const;

		// Returns atlas page
		UInt GetAtlasPage() const;

		// Returns atlas source rectangle
		RectI GetAtlasRect() const;

		// Returns image size
		Vec2F GetSize() const;

		// Returns image width
		float GetWidth() const;

		// Returns image height
		float GetHeight() const;

		// Returns atlas texture reference
		TextureRef GetAtlasTextureRef() const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		SERIALIZABLE(ImageAsset);

	public:
		// -----------------------
		// Platform specified meta
		// -----------------------
		struct PlatformMeta: public ISerializable
		{
			Vec2I  mMaxSize; // Maximum image size @SERIALIZABLE
			Vec2F  mScale;   // Image scale ((1; 1) - is default) @SERIALIZABLE
			String mFormat;  // Image format @SERIALIZABLE

			bool operator==(const PlatformMeta& other) const;

			SERIALIZABLE(PlatformMeta);
		};

		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			UID          mAtlasId;     // Atlas owner id @SERIALIZABLE
			PlatformMeta mIOS;         // IOS specified meta @SERIALIZABLE
			PlatformMeta mAndroid;     // Android specified meta @SERIALIZABLE
			PlatformMeta mMacOS;       // MacOS specified meta @SERIALIZABLE
			PlatformMeta mWindows;     // Windows specified meta @SERIALIZABLE
			BorderI      mSliceBorder; // Default slice border @SERIALIZABLE
			SpriteMode   mDefaultMode; // Default sprite mode @SERIALIZABLE

		public:
			// Returns asset type id
			const Type* GetAssetType() const;

			// Returns true if other meta is equal to this
			bool IsEqual(IMetaInfo* other) const;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		Bitmap* mBitmap;    // Image bitmap. Loading only when needs
		UInt    mAtlasPage; // Owner atlas page index
		RectI   mAtlasRect; // Owner atlas rectange

	protected:
		// Default constructor
		ImageAsset();

		// Constructor by path - loads asset by path
		ImageAsset(const String& path);

		// Constructor by id - loads asset by id
		ImageAsset(UID id);

		// Copy-constructor
		ImageAsset(const ImageAsset& asset);

		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Load bitmap
		void LoadBitmap();

		friend class AtlasAsset;
		friend class Assets;
	};

	// ---------------------
	// Image Asset reference
	// ---------------------
	class ImageAssetRef: public AssetRef
	{
	public:
		// Creates ImageAsset and returns reference to it
		static ImageAssetRef CreateAsset();

		// Default constructor, references to null
		ImageAssetRef(): AssetRef() {}

		// Constructor from asset reference
		ImageAssetRef(const AssetRef& other): AssetRef(other) { CheckType<ImageAsset>(); }

		// Copy-constructor
		ImageAssetRef(const ImageAssetRef& other): AssetRef(other) {}

		// Constructor from asset path
		ImageAssetRef(const String& path): AssetRef(path) {}

		// Constructor from asset id
		ImageAssetRef(UID id): AssetRef(id) {}

		// Destructor
		~ImageAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		ImageAssetRef& operator=(const ImageAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		ImageAsset& operator*() { return *((ImageAsset*)mAssetPtr); }

		// Constant getter operator
		const ImageAsset& operator*() const { return *((ImageAsset*)mAssetPtr); }

		// Asset members and field operator
		ImageAsset* operator->() { return ((ImageAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const ImageAsset* operator->() const { return ((ImageAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const ImageAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const ImageAssetRef& other) const { return AssetRef::operator!=(other); }

		// Returns asset type
		const Type& GetAssetType() const { return TypeOf(ImageAsset); }

		SERIALIZABLE(ImageAssetRef);

	protected:
		// Constructor for Assets manager
		ImageAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}

CLASS_BASES_META(o2::ImageAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::ImageAsset)
{
	PUBLIC_FIELD(bitmap);
	PUBLIC_FIELD(atlasId);
	PUBLIC_FIELD(atlas);
	PUBLIC_FIELD(sliceBorder);
	PUBLIC_FIELD(atlasPage);
	PUBLIC_FIELD(atlasRect);
	PUBLIC_FIELD(size);
	PUBLIC_FIELD(width);
	PUBLIC_FIELD(height);
	PUBLIC_FIELD(meta);
	PROTECTED_FIELD(mBitmap);
	PROTECTED_FIELD(mAtlasPage);
	PROTECTED_FIELD(mAtlasRect);
}
END_META;
CLASS_METHODS_META(o2::ImageAsset)
{

	PUBLIC_FUNCTION(void, PROPERTIES, ImageAsset);
	PUBLIC_FUNCTION(Bitmap*, GetBitmap);
	PUBLIC_FUNCTION(void, SetBitmap, Bitmap*);
	PUBLIC_FUNCTION(UID, GetAtlasId);
	PUBLIC_FUNCTION(void, SetAtlasId, UID);
	PUBLIC_FUNCTION(AtlasAssetRef, GetAtlas);
	PUBLIC_FUNCTION(void, SetAtlas, const AtlasAssetRef&);
	PUBLIC_FUNCTION(void, SetSliceBorder, const BorderI&);
	PUBLIC_FUNCTION(BorderI, GetSliceBorder);
	PUBLIC_FUNCTION(UInt, GetAtlasPage);
	PUBLIC_FUNCTION(RectI, GetAtlasRect);
	PUBLIC_FUNCTION(Vec2F, GetSize);
	PUBLIC_FUNCTION(float, GetWidth);
	PUBLIC_FUNCTION(float, GetHeight);
	PUBLIC_FUNCTION(TextureRef, GetAtlasTextureRef);
	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
	PROTECTED_FUNCTION(void, LoadBitmap);
}
END_META;

CLASS_BASES_META(o2::ImageAssetRef)
{
	BASE_CLASS(o2::AssetRef);
}
END_META;
CLASS_FIELDS_META(o2::ImageAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::ImageAssetRef)
{

	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_BASES_META(o2::ImageAsset::PlatformMeta)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ImageAsset::PlatformMeta)
{
	PUBLIC_FIELD(mMaxSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mScale).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFormat).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::ImageAsset::PlatformMeta)
{
}
END_META;

CLASS_BASES_META(o2::ImageAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);
}
END_META;
CLASS_FIELDS_META(o2::ImageAsset::MetaInfo)
{
	PUBLIC_FIELD(mAtlasId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mIOS).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mAndroid).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mMacOS).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mWindows).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mSliceBorder).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mDefaultMode).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::ImageAsset::MetaInfo)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(bool, IsEqual, IMetaInfo*);
}
END_META;
