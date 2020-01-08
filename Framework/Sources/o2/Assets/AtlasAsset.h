#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Render/TextureRef.h"

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
		PROPERTIES(AtlasAsset);
		GETTER(MetaInfo*, meta, GetMeta);                  // Meta information getter
		GETTER(AssetInfosVec, imagesInfos, GetImages);     // Containing images infos getter
		GETTER(Vector<AssetRef>, images, GetImagesAssets); // Images assets getter
		GETTER(PagesVec, pages, GetPages);                 // Pages getter

	public:
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
		const char* GetFileExtensions() const override;

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
			PlatformMeta mIOS;     // IOS specified meta @SERIALIZABLE
			PlatformMeta mAndroid; // Android specified meta @SERIALIZABLE
			PlatformMeta mMacOS;   // MacOS specified meta @SERIALIZABLE
			PlatformMeta mWindows; // Windows specified meta @SERIALIZABLE
			int          mBorder;  // Images pack border @SERIALIZABLE

		public:
			// Returns asset type id
			const Type* GetAssetType() const override;

			// Returns true if other meta is equal to this
			bool IsEqual(IMetaInfo* other) const override;

			SERIALIZABLE(MetaInfo);
		};

		// ----------
		// Atlas page
		// ----------
		class Page: public ISerializable
		{
			UInt            mId;          // Page number @SERIALIZABLE
			Vec2I           mSize;        // Size of page @SERIALIZABLE
			Map<UID, RectI> mImagesRects; // Images source rectangles @SERIALIZABLE
			AtlasAsset*     mOwner;       // Owner atlas

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
			const Map<UID, RectI>& ImagesRects() const;

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
		void LoadData(const String& path) override;

		// Saves data
		void SaveData(const String& path) override;

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
		const Type& GetAssetType() const override { return TypeOf(AtlasAsset); }

		SERIALIZABLE(AtlasAssetRef);

	protected:
		// Constructor for Assets manager
		AtlasAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}

CLASS_BASES_META(o2::AtlasAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset)
{
	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(imagesInfos);
	PUBLIC_FIELD(images);
	PUBLIC_FIELD(pages);
	PROTECTED_FIELD(mImagesAssetsInfos);
	PROTECTED_FIELD(mPages);
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset)
{

	PUBLIC_FUNCTION(AssetInfosVec, GetImages);
	PUBLIC_FUNCTION(Vector<AssetRef>, GetImagesAssets);
	PUBLIC_FUNCTION(PagesVec, GetPages);
	PUBLIC_FUNCTION(bool, ContainsImage, const ImageAssetRef&);
	PUBLIC_FUNCTION(bool, ContainsImage, const AssetInfo&);
	PUBLIC_FUNCTION(bool, ContainsImage, UID);
	PUBLIC_FUNCTION(bool, ContainsImage, const String&);
	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PUBLIC_STATIC_FUNCTION(String, GetPageTextureFileName, UID, UInt);
	PUBLIC_STATIC_FUNCTION(String, GetPageTextureFileName, const String&, UInt);
	PUBLIC_STATIC_FUNCTION(TextureRef, GetPageTextureRef, UID, UInt);
	PUBLIC_STATIC_FUNCTION(TextureRef, GetPageTextureRef, const String&, UInt);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;

CLASS_BASES_META(o2::AtlasAssetRef)
{
	BASE_CLASS(o2::AssetRef);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::AtlasAssetRef)
{

	PUBLIC_STATIC_FUNCTION(AtlasAssetRef, CreateAsset);
	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_BASES_META(o2::AtlasAsset::PlatformMeta)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset::PlatformMeta)
{
	PUBLIC_FIELD(mMaxSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mFormat).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset::PlatformMeta)
{
}
END_META;

CLASS_BASES_META(o2::AtlasAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset::MetaInfo)
{
	PUBLIC_FIELD(mIOS).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mAndroid).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mMacOS).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mWindows).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mBorder).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset::MetaInfo)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(bool, IsEqual, IMetaInfo*);
}
END_META;

CLASS_BASES_META(o2::AtlasAsset::Page)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset::Page)
{
	PUBLIC_FIELD(mId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mImagesRects).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mOwner);
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset::Page)
{

	typedef const Map<UID, RectI>& _tmp1;

	PUBLIC_FUNCTION(UInt, ID);
	PUBLIC_FUNCTION(Vec2I, Size);
	PUBLIC_FUNCTION(TextureRef, GetTextureRef);
	PUBLIC_FUNCTION(String, GetTextureFileName);
	PUBLIC_FUNCTION(_tmp1, ImagesRects);
}
END_META;
