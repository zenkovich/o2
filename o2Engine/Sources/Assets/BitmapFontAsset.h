#pragma once

#include "Assets/Asset.h"
#include "Render/FontRef.h"

namespace o2
{
	class Font;

	// -----------------
	// Bitmap font asset
	// -----------------
	class BitmapFontAsset: public Asset
	{
	public:
		class MetaInfo;

	public:
		Getter<MetaInfo*> meta; // Meta information getter
		Getter<FontRef>   font; // Font getter

		// Destructor
		~BitmapFontAsset();

		// Check equals operator
		BitmapFontAsset& operator=(const BitmapFontAsset& asset);

		// Check equals operator
		bool operator==(const BitmapFontAsset& other) const;

		// Check not equals operator
		bool operator!=(const BitmapFontAsset& other) const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns font pointer
		FontRef GetFont() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		SERIALIZABLE(BitmapFontAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			// Returns asset type id
			const Type* GetAssetType() const;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		FontRef mFont;

	protected:
		// Default constructor
		BitmapFontAsset();

		// Constructor by path - loads asset by path
		BitmapFontAsset(const String& path);

		// Constructor by id - loads asset by id
		BitmapFontAsset(UID id);

		// Copy-constructor
		BitmapFontAsset(const BitmapFontAsset& asset);

		// Loads data
		void LoadData(const String& path);

		// Initializes properties
		void InitializeProperties();

		friend class Assets;
	};

	// ---------------------------
	// Bitmap font Asset reference
	// ---------------------------
	class BitmapFontAssetRef: public AssetRef
	{
	public:
		// Creates BitmapFontAsset and returns reference to it
		static BitmapFontAssetRef CreateAsset();

		// Default constructor, references to null
		BitmapFontAssetRef(): AssetRef() {}

		// Copy-constructor
		BitmapFontAssetRef(const AssetRef& other): AssetRef(other) { CheckType<BitmapFontAsset>(); }

		// Copy-constructor
		BitmapFontAssetRef(const BitmapFontAssetRef& other): AssetRef(other) {}

		// Constructor from asset path
		BitmapFontAssetRef(const String& path): AssetRef(path) {}

		// Constructor from asset id
		BitmapFontAssetRef(UID id): AssetRef(id) {}

		// Destructor
		~BitmapFontAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		BitmapFontAssetRef& operator=(const BitmapFontAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		BitmapFontAsset& operator*() { return *((BitmapFontAsset*)mAssetPtr); }

		// Constant getter operator
		const BitmapFontAsset& operator*() const { return *((BitmapFontAsset*)mAssetPtr); }

		// Asset members and field operator
		BitmapFontAsset* operator->() { return ((BitmapFontAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const BitmapFontAsset* operator->() const { return ((BitmapFontAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const BitmapFontAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const BitmapFontAssetRef& other) const { return AssetRef::operator!=(other); }

		SERIALIZABLE(BitmapFontAssetRef);

	protected:
		// Constructor for Assets manager
		BitmapFontAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}