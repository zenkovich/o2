#pragma once

#include "Assets/Asset.h"
#include "Render/TextureRef.h"

namespace o2
{
	class ImageAsset;
	class ImageAssetRef;

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
		Getter<MetaInfo*>        meta;        // Meta information getter
		Getter<AssetInfosVec>    imagesInfos; // Containing images infos getter
		Getter<Vector<AssetRef>> images;      // Images assets getter
		Getter<PagesVec>         pages;       // Pages getter

		// Destructor
		~AtlasAsset();

		// Check equals operator
		AtlasAsset& operator=(const AtlasAsset& asset);

		// Check equals operator
		bool operator==(const AtlasAsset& other) const;

		// Check not equals operator
		bool operator!=(const AtlasAsset& other) const;

		// Returns containing images infos array
		AssetInfosVec GetImages() const;

		// Returns containing images assets
		Vector<AssetRef> GetImagesAssets() const;

		// Returns pages array
		PagesVec GetPages() const;

		// Is contains image
		bool ContainsImage(const ImageAssetRef& image);

		// Is contains image
		bool ContainsImage(const AssetInfo& imageAssetInfo);

		// Is contains image
		bool ContainsImage(UID id);

		// Is contains image
		bool ContainsImage(const String& path);

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		// Returns atlas page's texture file name
		static String GetPageTextureFileName(UID atlasId, UInt pageIdx);

		// Returns atlas page's texture file name
		static String GetPageTextureFileName(const String& atlasPath, UInt pageIdx);

		// Returns atlas page's texture reference
		static TextureRef GetPageTextureRef(UID atlasId, UInt pageIdx);

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
			const Type* GetAssetType() const;

			// Returns true if other meta is equal to this
			bool IsEqual(IMetaInfo* other) const;

			SERIALIZABLE(MetaInfo);
		};

		// ----------
		// Atlas page
		// ----------
		class Page: public ISerializable
		{
			UInt                   mId;          // Page number @SERIALIZABLE
			Vec2I                  mSize;        // Size of page @SERIALIZABLE
			Dictionary<UID, RectI> mImagesRects; // Images source rectangles @SERIALIZABLE
			AtlasAsset*            mOwner;       // Owner atlas

		public:
			// Returns number
			UInt ID() const;

			// Returns size
			Vec2I Size() const;

			// Returns texture reference
			TextureRef GetTextureRef() const;

			// Returns texture file name
			String GetTextureFileName() const;

			// Returns images rectangles
			const Dictionary<UID, RectI>& ImagesRects() const;

			// Check equal operator
			bool operator==(const Page& other) const;

			SERIALIZABLE(Page);

			friend class AtlasAssetConverter;
			friend class AtlasAsset;
		};

	protected:
		AssetInfosVec mImagesAssetsInfos; // Loaded image infos
		PagesVec      mPages;             // Pages

	protected:
		// Default constructor
		AtlasAsset();

		// Constructor by path - loads asset by path
		AtlasAsset(const String& path);

		// Constructor by id - loads asset by id
		AtlasAsset(UID id);

		// Copy-constructor
		AtlasAsset(const AtlasAsset& asset);

		// Loads data
		void LoadData(const String& path);

		// Saves data
		void SaveData(const String& path);

		// Initializes properties
		void InitializeProperties();

		friend class Assets;
	};

	// ---------------------
	// Atlas Asset reference
	// ---------------------
	class AtlasAssetRef: public AssetRef
	{
	public:
		// Creates AtlasAsset and returns reference to it
		static AtlasAssetRef CreateAsset();

		// Default constructor, references to null
		AtlasAssetRef(): AssetRef() {}

		// Copy-constructor
		AtlasAssetRef(const AssetRef& other): AssetRef(other) { CheckType<AtlasAsset>(); }

		// Copy-constructor
		AtlasAssetRef(const AtlasAssetRef& other): AssetRef(other) {}

		// Constructor from asset path
		AtlasAssetRef(const String& path): AssetRef(path) {}

		// Constructor from asset id
		AtlasAssetRef(UID id): AssetRef(id) {}

		// Destructor
		~AtlasAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		AtlasAssetRef& operator=(const AtlasAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		AtlasAsset& operator*() { return *((AtlasAsset*)mAssetPtr); }

		// Constant getter operator
		const AtlasAsset& operator*() const { return *((AtlasAsset*)mAssetPtr); }

		// Asset members and field operator
		AtlasAsset* operator->() { return ((AtlasAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const AtlasAsset* operator->() const { return ((AtlasAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const AtlasAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const AtlasAssetRef& other) const { return AssetRef::operator!=(other); }

		// Returns asset type
		const Type& GetAssetType() const { return TypeOf(AtlasAsset); }

		SERIALIZABLE(AtlasAssetRef);

	protected:
		// Constructor for Assets manager
		AtlasAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}
