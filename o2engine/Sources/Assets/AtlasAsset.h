#pragma once

#include "Assets/Asset.h"
#include "Render/TextureRef.h"

namespace o2
{
	class ImageAsset;

	// -----------
	// Atlas asset
	// -----------
	class AtlasAsset: public Asset
	{
	public: 
		class MetaInfo;
		class Page;

		typedef Vector<Page> PagesVec;

	public:
		Getter<Ptr<MetaInfo>>           meta;        // Meta information getter
		Getter<AssetInfosVec>           imagesInfos; // Containing images infos getter
		Getter<Vector<Ptr<ImageAsset>>> images;      // Images assets getter
		Getter<PagesVec>                pages;       // Pages getter

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

		// Returns atlas page's texture file name
		static String GetPageTextureFileName(AssetId atlasId, UInt pageIdx);

		// Returns atlas page's texture file name
		static String GetPageTextureFileName(const String& atlasPath, UInt pageIdx);

		// Returns atlas page's texture reference
		static TextureRef GetPageTextureRef(AssetId atlasId, UInt pageIdx);

		// Returns atlas page's texture reference
		static TextureRef GetPageTextureRef(const String& atlasPath, UInt pageIdx);

		SERIALIZABLE(AtlasAsset);

	public:
		// -----------------------------------
		// Platform specified meta information
		// -----------------------------------
		struct PlatformMeta: public ISerializable
		{
			Vec2I  mMaxSize = Vec2I(2048, 2048); // Maximal atlas size @SERIALIZABLE
			String mFormat;                      // Atlas format @SERIALIZABLE

			bool operator==(const PlatformMeta& other) const;

			SERIALIZABLE(PlatformMeta);
		};

		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:

		public:
			PlatformMeta mIOS;     // IOS specified meta @SERIALIZABLE
			PlatformMeta mAndroid; // Android specified meta @SERIALIZABLE
			PlatformMeta mMacOS;   // MacOS specified meta @SERIALIZABLE
			PlatformMeta mWindows; // Windows specified meta @SERIALIZABLE
			int          mBorder;  // Images pack border @SERIALIZABLE

		public:
			// Returns asset type id
			Type::Id GetAssetType() const;

			// Returns true if other meta is equal to this
			bool IsEqual(Ptr<IMetaInfo> other) const;

			SERIALIZABLE(MetaInfo);
		};

		// ----------
		// Atlas page
		// ----------
		class Page: public ISerializable
		{
			friend class AtlasAssetConverter;

			AssetId                    mId;          // Page number @SERIALIZABLE
			Vec2I                      mSize;        // Size of page @SERIALIZABLE
			Dictionary<AssetId, RectI> mImagesRects; // Images source rectangles @SERIALIZABLE

		public:
			// Returns number
			AssetId ID() const;

			// Returns size
			Vec2I Size() const;

			// Returns texture reference
			TextureRef GetTextureRef() const;

			// Returns texture file name
			String GetTextureFileName() const;

			// Returns images rectangles
			const Dictionary<AssetId, RectI>& ImagesRects() const;

			// Check equal operator
			bool operator==(const Page& other) const;

			SERIALIZABLE(Page);
		};

	protected:
		AssetInfosVec mImagesAssetsInfos; // Loaded image infos  @SERIALIZABLE
		PagesVec      mPages;             // Pages @SERIALIZABLE

	protected:
		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Initializes properties
		void InitializeProperties();
	};
}
