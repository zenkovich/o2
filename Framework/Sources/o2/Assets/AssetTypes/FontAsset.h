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
		class MetaInfo : public AssetMeta
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
		FontAsset(const UID& id);

		// Copy-constructor
		FontAsset(const FontAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		friend class Assets;
	};

	// --------------------
	// Font Asset reference
	// --------------------
	class FontAssetRef : public IAssetRef
	{
	public:
		// Creates FontAsset and returns reference to it
		static FontAssetRef CreateAsset();

		// Default constructor, references to null
		FontAssetRef() : IAssetRef() {}

		// Copy-constructor
		FontAssetRef(const IAssetRef& other) : IAssetRef(other) { CheckType<FontAsset>(); }

		// Copy-constructor
		FontAssetRef(const FontAssetRef& other) : IAssetRef(other) {}

		// Constructor from asset path
		FontAssetRef(const String& path) : IAssetRef(path) {}

		// Constructor from asset id
		FontAssetRef(const UID& id) : IAssetRef(id) {}

		// Destructor
		~FontAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		FontAssetRef& operator=(const FontAssetRef& other) { IAssetRef::operator=(other); return *this; }

		// Getter operator
		FontAsset& operator*() { return *((FontAsset*)mAssetPtr); }

		// Constant getter operator
		const FontAsset& operator*() const { return *((FontAsset*)mAssetPtr); }

		// Asset members and field operator
		FontAsset* operator->() { return ((FontAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const FontAsset* operator->() const { return ((FontAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const FontAssetRef& other) const { return IAssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const FontAssetRef& other) const { return IAssetRef::operator!=(other); }

		// Returns asset type
		const Type& GetAssetType() const override { return TypeOf(FontAsset); }

		SERIALIZABLE(FontAssetRef);

	protected:
		// Constructor for Assets manager
		FontAssetRef(Asset* assetPtr, int* refCounter) : IAssetRef(assetPtr, refCounter) {}
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
	BASE_CLASS(o2::IAssetRef);
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
	BASE_CLASS(o2::AssetMeta);
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
