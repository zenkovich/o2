#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetRef.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Bitmap/Bitmap.h"

namespace o2
{
	// -----------
	// Image asset
	// -----------
	class ImageAsset: public Asset
	{
	public:
		class Meta;

	public:
		PROPERTIES(ImageAsset);
		PROPERTY(Bitmap*, bitmap, SetBitmap, GetBitmap); // Bitmap data property

		PROPERTY(BorderI, sliceBorder, SetSliceBorder, GetSliceBorder);    // Slice border property
		PROPERTY(SpriteMode, defaultMode, SetDefaultMode, GetDefaultMode); // Sprite default mode property

		PROPERTY(UID, atlas, SetAtlas, GetAtlas); // Atlas owner asset property
		GETTER(UInt, atlasPage, GetAtlasPage);    // Atlas page index getter
		GETTER(RectI, atlasRect, GetAtlasRect);   // Atlas source image rectangle getter

		GETTER(Vec2F, size, GetSize);     // Image size getter
		GETTER(float, width, GetWidth);   // Image width getter
		GETTER(float, height, GetHeight); // Image height getter

		GETTER(Meta*, meta, GetMeta); // Meta information getter

	public:
		// Default constructor
		ImageAsset();

		// Copy-constructor
		ImageAsset(const ImageAsset& asset);

		// Destructor
		~ImageAsset();

		// Check equals operator
		ImageAsset& operator=(const ImageAsset& asset);

		// Returns bitmap data
		Bitmap* GetBitmap();

		// Sets bitmap data
		void SetBitmap(Bitmap* bitmap);

		// Returns atlas asset
		UID GetAtlas() const;

		// Sets atlas
		void SetAtlas(const UID& atlas);

		// Sets slice border
		void SetSliceBorder(const BorderI& border);

		// Returns slice border
		BorderI GetSliceBorder() const;

		// Sets default sprite mode
		void SetDefaultMode(SpriteMode mode);

		// Returns default sprite mode
		SpriteMode GetDefaultMode() const;

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
		Meta* GetMeta() const;

		// Returns extensions string
		static const char* GetFileExtensions();

		ASSET_TYPE(ImageAsset, Meta);

	public:
		// -----------------------
		// Platform specified meta
		// -----------------------
		struct PlatformMeta: public ISerializable
		{
			Vec2I  maxSize; // Maximum image size @SERIALIZABLE
			Vec2F  scale;   // Image scale ((1; 1) - is default) @SERIALIZABLE
			String format;  // Image format @SERIALIZABLE

			bool operator==(const PlatformMeta& other) const;

			SERIALIZABLE(PlatformMeta);
		};

		// ----------------
		// Meta information
		// ----------------
		class Meta: public DefaultAssetMeta<ImageAsset>
		{
		public:
			UID          atlasId = UID::empty; // Atlas owner id @SERIALIZABLE
			PlatformMeta ios;                  // IOS specified meta @SERIALIZABLE
			PlatformMeta android;              // Android specified meta @SERIALIZABLE
			PlatformMeta macOS;                // MacOS specified meta @SERIALIZABLE
			PlatformMeta windows;              // Windows specified meta @SERIALIZABLE
			BorderI      sliceBorder;          // Default slice border @SERIALIZABLE @EDITOR_IGNORE
			SpriteMode   defaultMode;          // Default sprite mode @SERIALIZABLE @EDITOR_IGNORE

		public:
			// Returns true if other meta is equal to this
			bool IsEqual(AssetMeta* other) const override;

			SERIALIZABLE(Meta);
		};

	protected:
		Bitmap* mBitmap = nullptr; // Image bitmap. Loading only when needs

		UInt  mAtlasPage; // Owner atlas page index @SERIALIZABLE
		RectI mAtlasRect; // Owner atlas rectangle @SERIALIZABLE

	protected:
		// Saves data
		void SaveData(const String& path) const override;

		// Load bitmap
		void LoadBitmap();

		friend class AtlasAsset;
		friend class Assets;
	};

	typedef Ref<ImageAsset> ImageAssetRef;
}

CLASS_BASES_META(o2::ImageAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::ImageAsset)
{
	FIELD().PUBLIC().NAME(bitmap);
	FIELD().PUBLIC().NAME(sliceBorder);
	FIELD().PUBLIC().NAME(defaultMode);
	FIELD().PUBLIC().NAME(atlas);
	FIELD().PUBLIC().NAME(atlasPage);
	FIELD().PUBLIC().NAME(atlasRect);
	FIELD().PUBLIC().NAME(size);
	FIELD().PUBLIC().NAME(width);
	FIELD().PUBLIC().NAME(height);
	FIELD().PUBLIC().NAME(meta);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mBitmap);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mAtlasPage);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mAtlasRect);
}
END_META;
CLASS_METHODS_META(o2::ImageAsset)
{

	FUNCTION().PUBLIC().SIGNATURE(Bitmap*, GetBitmap);
	FUNCTION().PUBLIC().SIGNATURE(void, SetBitmap, Bitmap*);
	FUNCTION().PUBLIC().SIGNATURE(UID, GetAtlas);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAtlas, const UID&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetSliceBorder, const BorderI&);
	FUNCTION().PUBLIC().SIGNATURE(BorderI, GetSliceBorder);
	FUNCTION().PUBLIC().SIGNATURE(void, SetDefaultMode, SpriteMode);
	FUNCTION().PUBLIC().SIGNATURE(SpriteMode, GetDefaultMode);
	FUNCTION().PUBLIC().SIGNATURE(UInt, GetAtlasPage);
	FUNCTION().PUBLIC().SIGNATURE(RectI, GetAtlasRect);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetSize);
	FUNCTION().PUBLIC().SIGNATURE(float, GetWidth);
	FUNCTION().PUBLIC().SIGNATURE(float, GetHeight);
	FUNCTION().PUBLIC().SIGNATURE(TextureRef, GetAtlasTextureRef);
	FUNCTION().PUBLIC().SIGNATURE(Meta*, GetMeta);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const char*, GetFileExtensions);
	FUNCTION().PROTECTED().SIGNATURE(void, SaveData, const String&);
	FUNCTION().PROTECTED().SIGNATURE(void, LoadBitmap);
}
END_META;

CLASS_BASES_META(o2::ImageAsset::PlatformMeta)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ImageAsset::PlatformMeta)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(maxSize);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(scale);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(format);
}
END_META;
CLASS_METHODS_META(o2::ImageAsset::PlatformMeta)
{
}
END_META;

CLASS_BASES_META(o2::ImageAsset::Meta)
{
	BASE_CLASS(o2::DefaultAssetMeta<ImageAsset>);
}
END_META;
CLASS_FIELDS_META(o2::ImageAsset::Meta)
{
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(UID::empty).NAME(atlasId);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(ios);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(android);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(macOS);
	FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(windows);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(sliceBorder);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(defaultMode);
}
END_META;
CLASS_METHODS_META(o2::ImageAsset::Meta)
{

	FUNCTION().PUBLIC().SIGNATURE(bool, IsEqual, AssetMeta*);
}
END_META;
