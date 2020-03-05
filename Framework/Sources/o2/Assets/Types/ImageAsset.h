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
		const char* GetFileExtensions() const override;

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
			UID          atlasId;     // Atlas owner id @SERIALIZABLE
			PlatformMeta ios;         // IOS specified meta @SERIALIZABLE
			PlatformMeta android;     // Android specified meta @SERIALIZABLE
			PlatformMeta macOS;       // MacOS specified meta @SERIALIZABLE
			PlatformMeta windows;     // Windows specified meta @SERIALIZABLE
			BorderI      sliceBorder; // Default slice border @SERIALIZABLE
			SpriteMode   defaultMode; // Default sprite mode @SERIALIZABLE

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
		// Default constructor
		ImageAsset();

		// Copy-constructor
		ImageAsset(const ImageAsset& asset);

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
	PUBLIC_FIELD(bitmap);
	PUBLIC_FIELD(sliceBorder);
	PUBLIC_FIELD(defaultMode);
	PUBLIC_FIELD(atlas);
	PUBLIC_FIELD(atlasPage);
	PUBLIC_FIELD(atlasRect);
	PUBLIC_FIELD(size);
	PUBLIC_FIELD(width);
	PUBLIC_FIELD(height);
	PUBLIC_FIELD(meta);
	PROTECTED_FIELD(mBitmap);
	PROTECTED_FIELD(mAtlasPage).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAtlasRect).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::ImageAsset)
{

	PUBLIC_FUNCTION(Bitmap*, GetBitmap);
	PUBLIC_FUNCTION(void, SetBitmap, Bitmap*);
	PUBLIC_FUNCTION(UID, GetAtlas);
	PUBLIC_FUNCTION(void, SetAtlas, const UID&);
	PUBLIC_FUNCTION(void, SetSliceBorder, const BorderI&);
	PUBLIC_FUNCTION(BorderI, GetSliceBorder);
	PUBLIC_FUNCTION(void, SetDefaultMode, SpriteMode);
	PUBLIC_FUNCTION(SpriteMode, GetDefaultMode);
	PUBLIC_FUNCTION(UInt, GetAtlasPage);
	PUBLIC_FUNCTION(RectI, GetAtlasRect);
	PUBLIC_FUNCTION(Vec2F, GetSize);
	PUBLIC_FUNCTION(float, GetWidth);
	PUBLIC_FUNCTION(float, GetHeight);
	PUBLIC_FUNCTION(TextureRef, GetAtlasTextureRef);
	PUBLIC_FUNCTION(Meta*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, SaveData, const String&);
	PROTECTED_FUNCTION(void, LoadBitmap);
}
END_META;

CLASS_BASES_META(o2::ImageAsset::PlatformMeta)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ImageAsset::PlatformMeta)
{
	PUBLIC_FIELD(maxSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(scale).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(format).SERIALIZABLE_ATTRIBUTE();
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
	PUBLIC_FIELD(atlasId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(ios).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(android).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(macOS).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(windows).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(sliceBorder).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(defaultMode).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::ImageAsset::Meta)
{

	PUBLIC_FUNCTION(bool, IsEqual, AssetMeta*);
}
END_META;
