#pragma once

#include "Assets/Asset.h"

namespace o2
{
	class Bitmap;
	class AtlasAsset;

	// -----------
	// Image asset
	// -----------
	class ImageAsset: public Asset
	{
		friend class AtlasAsset;

	public:
		// -----------------------
		// Platform specified meta
		// -----------------------
		struct PlatformMeta: public ISerializable
		{
			Vec2I  mMaxSize; // Maximum image size
			Vec2F  mScale;   // Image scale ((1; 1) - is default)
			String mFormat;  // Image format

			SERIALIZABLE_IMPL(PlatformMeta);
			IOBJECT(PlatformMeta)
			{
				SRLZ_FIELD(mMaxSize);
				SRLZ_FIELD(mScale);
				SRLZ_FIELD(mFormat);
			}
		};

		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			AssetId      mAtlasId; // Atlas owner id
			PlatformMeta mIOS;     // IOS specified meta
			PlatformMeta mAndroid; // Android specified meta
			PlatformMeta mMacOS;   // MacOS specified meta
			PlatformMeta mWindows; // Windows specified meta

		public:
			// Returns asset type id
			Type::Id GetAssetType() const;

			SERIALIZABLE_IMPL(MetaInfo);

			IOBJECT(MetaInfo)
			{
				BASE_CLASS(IMetaInfo);
				SRLZ_FIELD(mAtlasId);
				SRLZ_FIELD(mIOS);
				SRLZ_FIELD(mAndroid);
				SRLZ_FIELD(mMacOS);
				SRLZ_FIELD(mWindows);
			}
		};

	public:
		Property<Ptr<Bitmap>>     Data;      // Bitmap data property
		Property<AssetId>         AtlasId;   // Atlas owner id property
		Property<Ptr<AtlasAsset>> Atlas;     // Atlas owner asset property
		Getter<UInt>              AtlasPage; // Atlas page index getter
		Getter<RectI>             AtlasRect; // Atlas source image rectangle getter
		Getter<Ptr<MetaInfo>>     Meta;      // Meta information getter

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

		// Returns meta information
		Ptr<MetaInfo> GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		SERIALIZABLE_IMPL(ImageAsset);

		IOBJECT(ImageAsset)
		{
			BASE_CLASS(Asset);
			FIELD(mBitmap);
			SRLZ_FIELD(mAtlasPage);
			SRLZ_FIELD(mAtlasRect);
		}

	protected:
		Ptr<Bitmap> mBitmap;    // Image bitmap. Loading only when needs
		AssetId     mAtlasPage; // Owner atlas page index
		RectI       mAtlasRect; // Owner atlas rectangle

	protected:
		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Load bitmap
		void LoadBitmap();

		// Initializes properties
		void InitializeProperties();
	};
}
