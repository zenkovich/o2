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
		Property<Bitmap*>       bitmap;      // Bitmap data property
		Property<UID>           atlasId;     // Atlas owner id property
		Property<AtlasAssetRef> atlas;       // Atlas owner asset property
		Property<BorderI>       sliceBorder; // Slice border
		Getter<UInt>            atlasPage;   // Atlas page index getter
		Getter<RectI>           atlasRect;   // Atlas source image rectangle getter
		Getter<Vec2F>           size;        // Image size getter
		Getter<float>           width;       // Image width getter
		Getter<float>           height;      // Image height getter
		Getter<MetaInfo*>       meta;        // Meta information getter

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

		// Initializes properties
		void InitializeProperties();

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

		SERIALIZABLE(ImageAssetRef);

	protected:
		// Constructor for Assets manager
		ImageAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}
