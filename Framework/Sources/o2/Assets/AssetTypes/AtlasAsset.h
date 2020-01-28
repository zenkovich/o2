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
	class AtlasAsset: public TAsset<AtlasAsset>
	{
	public: 
		class Meta;
		class Page;

	public:
		PROPERTIES(AtlasAsset);
		GETTER(Meta*, meta, GetMeta);                      // Meta information getter
		GETTER(Vector<AssetRef>, images, GetImages); // Images assets getter
		GETTER(Vector<Page>, pages, GetPages);             // Pages getter

	public:
		// Check equals operator
		AtlasAsset& operator=(const AtlasAsset& asset);

		// Returns containing images assets
		const Vector<ImageAssetRef>& GetImages() const;

		// Returns pages array
		const Vector<Page>& GetPages() const;

		// Is contains image
		bool ContainsImage(const ImageAssetRef& image);

		// Returns meta information
		Meta* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const override;

		// Returns atlas page's texture file name
		static String GetPageTextureFileName(const UID& atlasId, UInt pageIdx);

		// Returns atlas page's texture file name
		static String GetPageTextureFileName(const String& atlasPath, UInt pageIdx);

		// Returns atlas page's texture reference
		static TextureRef GetPageTextureRef(const UID& atlasId, UInt pageIdx);

		// Returns atlas page's texture reference
		static TextureRef GetPageTextureRef(const String& atlasPath, UInt pageIdx);

		SERIALIZABLE(AtlasAsset);

	public:
		// -----------------------------------
		// Platform specified meta information
		// -----------------------------------
		struct PlatformMeta: public ISerializable
		{
			Vec2I  maxSize = Vec2I(2048, 2048); // Maximal atlas size @SERIALIZABLE
			String format;                      // Atlas format @SERIALIZABLE

			bool operator==(const PlatformMeta& other) const;

			SERIALIZABLE(PlatformMeta);
		};

		// ----------------
		// Meta information
		// ----------------
		class Meta: public TAssetMeta<AtlasAsset>
		{
		public:
			PlatformMeta ios;     // IOS specified meta @SERIALIZABLE
			PlatformMeta android; // Android specified meta @SERIALIZABLE
			PlatformMeta macOS;   // MacOS specified meta @SERIALIZABLE
			PlatformMeta windows; // Windows specified meta @SERIALIZABLE
			int          border;  // Images pack border @SERIALIZABLE

		public:
			// Returns true if other meta is equal to this
			bool IsEqual(AssetMeta* other) const override;

			SERIALIZABLE(Meta);
		};

		// ----------
		// Atlas page
		// ----------
		class Page: public ISerializable
		{
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

		private:
			UInt            mId;          // Page number @SERIALIZABLE
			Vec2I           mSize;        // Size of page @SERIALIZABLE
			Map<UID, RectI> mImagesRects; // Images source rectangles @SERIALIZABLE
			AtlasAsset*     mOwner;       // Owner atlas

			friend class AtlasAssetConverter;
			friend class AtlasAsset;
		};

	protected:
		Vector<ImageAssetRef> mImages; // Loaded image infos @SERIALIZABLE
		Vector<Page>          mPages;  // Pages @SERIALIZABLE

	protected:
		// Default constructor
		AtlasAsset();

		// Copy-constructor
		AtlasAsset(const AtlasAsset& asset);

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node) override;

		friend class Assets;
	};

	typedef Ref<AtlasAsset> AtlasAssetRef;
}

CLASS_BASES_META(o2::AtlasAsset)
{
	BASE_CLASS(o2::TAsset<AtlasAsset>);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset)
{
	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(images);
	PUBLIC_FIELD(pages);
	PROTECTED_FIELD(mImages).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPages).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset)
{

	PUBLIC_FUNCTION(const Vector<ImageAssetRef>&, GetImages);
	PUBLIC_FUNCTION(const Vector<Page>&, GetPages);
	PUBLIC_FUNCTION(bool, ContainsImage, const ImageAssetRef&);
	PUBLIC_FUNCTION(Meta*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PUBLIC_STATIC_FUNCTION(String, GetPageTextureFileName, const UID&, UInt);
	PUBLIC_STATIC_FUNCTION(String, GetPageTextureFileName, const String&, UInt);
	PUBLIC_STATIC_FUNCTION(TextureRef, GetPageTextureRef, const UID&, UInt);
	PUBLIC_STATIC_FUNCTION(TextureRef, GetPageTextureRef, const String&, UInt);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
}
END_META;

CLASS_BASES_META(o2::AtlasAsset::PlatformMeta)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset::PlatformMeta)
{
	PUBLIC_FIELD(maxSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(format).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset::PlatformMeta)
{
}
END_META;

CLASS_BASES_META(o2::AtlasAsset::Meta)
{
	BASE_CLASS(o2::TAssetMeta<AtlasAsset>);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset::Meta)
{
	PUBLIC_FIELD(ios).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(android).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(macOS).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(windows).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(border).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AtlasAsset::Meta)
{

	PUBLIC_FUNCTION(bool, IsEqual, AssetMeta*);
}
END_META;

CLASS_BASES_META(o2::AtlasAsset::Page)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AtlasAsset::Page)
{
	PRIVATE_FIELD(mId).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mSize).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mImagesRects).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mOwner);
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
