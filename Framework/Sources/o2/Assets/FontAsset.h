#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Render/FontRef.h"

namespace o2
{
	class Font;

	// ----------
	// Font asset
	// ----------
	class FontAsset : public Asset
	{
	public:
		class MetaInfo;

	public:
		PROPERTIES(FontAsset);
		GETTER(MetaInfo*, meta, GetMeta); // Meta information getter
		GETTER(FontRef, font, GetFont);   // Font getter

	public:
        // Destructor
		~FontAsset();

		// Check equals operator
		FontAsset& operator=(const FontAsset& asset);

		// Check equals operator
		bool operator==(const FontAsset& other) const;

		// Check not equals operator
		bool operator!=(const FontAsset& other) const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns font pointer
		virtual FontRef GetFont() const;

		// Returns extensions string
		const char* GetFileExtensions() const override;

		SERIALIZABLE(FontAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo : public IMetaInfo
		{
		public:
			// Returns asset type id
			const Type* GetAssetType() const override;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		FontRef mFont;

	protected:
		// Default constructor
		FontAsset();

		// Constructor by path - loads asset by path
		FontAsset(const String& path);

		// Constructor by id - loads asset by id
		FontAsset(UID id);

		// Copy-constructor
		FontAsset(const FontAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		friend class Assets;
	};

	// --------------------
	// Font Asset reference
	// --------------------
	class FontAssetRef : public AssetRef
	{
	public:
		// Creates FontAsset and returns reference to it
		static FontAssetRef CreateAsset();

		// Default constructor, references to null
		FontAssetRef() : AssetRef() {}

		// Copy-constructor
		FontAssetRef(const AssetRef& other) : AssetRef(other) { CheckType<FontAsset>(); }

		// Copy-constructor
		FontAssetRef(const FontAssetRef& other) : AssetRef(other) {}

		// Constructor from asset path
		FontAssetRef(const String& path) : AssetRef(path) {}

		// Constructor from asset id
		FontAssetRef(UID id) : AssetRef(id) {}

		// Destructor
		~FontAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		FontAssetRef& operator=(const FontAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		FontAsset& operator*() { return *((FontAsset*)mAssetPtr); }

		// Constant getter operator
		const FontAsset& operator*() const { return *((FontAsset*)mAssetPtr); }

		// Asset members and field operator
		FontAsset* operator->() { return ((FontAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const FontAsset* operator->() const { return ((FontAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const FontAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const FontAssetRef& other) const { return AssetRef::operator!=(other); }

		// Returns asset type
		const Type& GetAssetType() const override { return TypeOf(FontAsset); }

		SERIALIZABLE(FontAssetRef);

	protected:
		// Constructor for Assets manager
		FontAssetRef(Asset* assetPtr, int* refCounter) : AssetRef(assetPtr, refCounter) {}
	};
}

CLASS_BASES_META(o2::FontAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::FontAsset)
{
	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(font);
	PROTECTED_FIELD(mFont);
}
END_META;
CLASS_METHODS_META(o2::FontAsset)
{

	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(FontRef, GetFont);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
}
END_META;

CLASS_BASES_META(o2::FontAssetRef)
{
	BASE_CLASS(o2::AssetRef);
}
END_META;
CLASS_FIELDS_META(o2::FontAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::FontAssetRef)
{

	PUBLIC_STATIC_FUNCTION(FontAssetRef, CreateAsset);
	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_BASES_META(o2::FontAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);
}
END_META;
CLASS_FIELDS_META(o2::FontAsset::MetaInfo)
{
}
END_META;
CLASS_METHODS_META(o2::FontAsset::MetaInfo)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
}
END_META;
