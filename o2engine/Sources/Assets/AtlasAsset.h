#pragma once

#include "Assets/Asset.h"

namespace o2
{
	class ImageAsset;

	// -----------
	// Atlas asset
	// -----------
	class AtlasAsset: public Asset
	{
	public:
		// -----------------------------------
		// Platform specified meta information
		// -----------------------------------
		struct PlatformMeta: public ISerializable
		{
			Vec2I  mMaxSize = Vec2I(2048, 2048); // Maximal atlas size
			String mFormat;                      // Atlas format

			bool operator==(const PlatformMeta& other) const;

			SERIALIZABLE_IMPL(PlatformMeta);
			IOBJECT(PlatformMeta)
			{
				SRLZ_FIELD(mMaxSize);
				SRLZ_FIELD(mFormat);
			}
		};

		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:

		public:
			PlatformMeta mIOS;     // IOS specified meta
			PlatformMeta mAndroid; // Android specified meta
			PlatformMeta mMacOS;   // MacOS specified meta
			PlatformMeta mWindows; // Windows specified meta

		public:
			// Returns asset type id
			Type::Id GetAssetType() const;

			// Returns true if other meta is equal to this
			bool IsEqual(Ptr<IMetaInfo> other) const;

			SERIALIZABLE_IMPL(MetaInfo);
			IOBJECT(MetaInfo)
			{
				BASE_CLASS(IMetaInfo);
				SRLZ_FIELD(mIOS);
				SRLZ_FIELD(mAndroid);
				SRLZ_FIELD(mMacOS);
				SRLZ_FIELD(mWindows);
			}
		};

		// ----------
		// Atlas page
		// ----------
		class Page: public ISerializable
		{
			friend class AtlasAssetConverter;

			AssetId                    mId;          // Page number
			Vec2I                      mSize;        // Size of page
			UInt                       mTextureId;   // Texture id
			Dictionary<AssetId, RectI> mImagesRects; // Images source rectangles

		public:
			// Returns number
			AssetId ID() const;

			// Returns size
			Vec2I Size() const;

			// Returns texture id
			UInt TextureId() const;

			// Returns images rectangles
			const Dictionary<AssetId, RectI>& ImagesRects() const;

			// Check equal operator
			bool operator==(const Page& other) const;

			SERIALIZABLE_IMPL(Page);
			IOBJECT(Page)
			{
				SRLZ_FIELD(mId);
				SRLZ_FIELD(mSize);
				SRLZ_FIELD(mImagesRects);
			}
		};
		typedef Vector<Page> PagesVec;

	public:
		Getter<Ptr<MetaInfo>>           Meta;        // Meta information getter
		Getter<AssetInfosVec>           ImagesInfos; // Containing images infos getter
		Getter<Vector<Ptr<ImageAsset>>> Images;      // Images assets getter
		Getter<PagesVec>                Pages;       // Pages getter

		// Default constructor
		AtlasAsset();

		// Constructor by path - loads asset by path
		AtlasAsset(const String& path);

		// Constructor by id - loads asset by id
		AtlasAsset(AssetId id);

		// Copy-constructor
		AtlasAsset(const AtlasAsset& asset);

		// Destructor
		~AtlasAsset();

		// Check equals operator
		AtlasAsset& operator=(const AtlasAsset& asset);

		// Returns containing images infos array
		AssetInfosVec GetImages() const;

		// Returns containing images assets
		Vector<Ptr<ImageAsset>> GetImagesAssets() const;

		// Returns pages array
		PagesVec GetPages() const;

		// Is contains image
		bool ContainsImage(Ptr<ImageAsset> image);

		// Is contains image
		bool ContainsImage(const AssetInfo& imageAssetInfo);

		// Is contains image
		bool ContainsImage(AssetId id);

		// Is contains image
		bool ContainsImage(const String& path);

		// Returns meta information
		Ptr<MetaInfo> GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		SERIALIZABLE_IMPL(AtlasAsset);

		IOBJECT(AtlasAsset)
		{
			BASE_CLASS(Asset);
			SRLZ_FIELD(mImagesAssetsInfos);
			SRLZ_FIELD(mPages);
		}

	protected:
		AssetInfosVec mImagesAssetsInfos; // Loaded image infos 
		PagesVec      mPages;             // Pages

	protected:
		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Initializes properties
		void InitializeProperties();
	};
}
