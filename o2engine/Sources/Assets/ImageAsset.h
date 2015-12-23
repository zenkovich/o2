#pragma once

#include "Assets/Asset.h"
#include "Render/Sprite.h"
#include "Render/TextureRef.h"
#include "Utils/Bitmap.h"

namespace o2
{
	class AtlasAsset;

	// -----------
	// Image asset
	// -----------
	class ImageAsset: public Asset
	{
	public:
		class MetaInfo;

	public:
		Property<Ptr<Bitmap>>     bitmap;    // Bitmap data property
		Property<AssetId>         atlasId;   // Atlas owner id property
		Property<Ptr<AtlasAsset>> atlas;     // Atlas owner asset property
		Getter<UInt>              atlasPage; // Atlas page index getter
		Getter<RectI>             atlasRect; // Atlas source image rectangle getter
		Getter<Ptr<MetaInfo>>     meta;      // Meta information getter

		// Default constructor
		ImageAsset();

		// Constructor by path - loads asset by path
		ImageAsset(const String& path);

		// Constructor by id - loads asset by id
		ImageAsset(AssetId id);

		// Copy-constructor
		ImageAsset(const ImageAsset& asset);

		// Destructor
		~ImageAsset();

		// Check equals operator
		ImageAsset& operator=(ImageAsset& asset);

		// Returns bitmap data
		Ptr<Bitmap> GetBitmap();

		// Sets bitmap data
		void SetBitmap(Ptr<Bitmap> bitmap);

		// Returns atlas id
		AssetId GetAtlasId() const;

		// Sets atlas id
		void SetAtlasId(AssetId id);

		// Returns atlas asset
		Ptr<AtlasAsset> GetAtlas() const;

		// Sets atlas
		void SetAtlas(Ptr<AtlasAsset> atlas);

		// Returns atlas page
		UInt GetAtlasPage() const;

		// Returns atlas source rectangle
		RectI GetAtlasRect() const;

		// Returns atlas texture reference
		TextureRef GetAtlasTextureRef() const;

		// Returns meta information
		Ptr<MetaInfo> GetMeta() const;

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
			AssetId      mAtlasId;     // Atlas owner id @SERIALIZABLE
			PlatformMeta mIOS;         // IOS specified meta @SERIALIZABLE
			PlatformMeta mAndroid;     // Android specified meta @SERIALIZABLE
			PlatformMeta mMacOS;       // MacOS specified meta @SERIALIZABLE
			PlatformMeta mWindows;     // Windows specified meta @SERIALIZABLE
			RectI        mSliceBorder; // Default slice border @SERIALIZABLE
			SpriteMode   mDefaultMode; // Default sprite mode @SERIALIZABLE

		public:
			// Returns asset type id
			Type::Id GetAssetType() const;

			// Returns true if other meta is equal to this
			bool IsEqual(Ptr<IMetaInfo> other) const;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		Ptr<Bitmap> mBitmap;    // Image bitmap. Loading only when needs
		UInt        mAtlasPage; // Owner atlas page index @SERIALIZABLE
		RectI       mAtlasRect; // Owner atlas rectangle @SERIALIZABLE

	protected:
		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Load bitmap
		void LoadBitmap();

		// Initializes properties
		void InitializeProperties();

		friend class AtlasAsset;
	};
}
