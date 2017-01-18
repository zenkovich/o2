#pragma once

#include "Assets/Asset.h"
#include "Render/FontRef.h"
#include "Render/VectorFont.h"

namespace o2
{
	class Font;

	// -----------------
	// Vector font asset
	// -----------------
	class VectorFontAsset: public Asset
	{
	public:
		class MetaInfo;

		typedef Vector<VectorFont::Effect*> EffectsVec;

	public:
		Getter<MetaInfo*> meta; // Meta information getter
		Getter<FontRef>   font; // Font getter

		// Destructor
		~VectorFontAsset();

		// Check equals operator
		VectorFontAsset& operator=(const VectorFontAsset& asset);

		// Check equals operator
		bool operator==(const VectorFontAsset& other) const;

		// Check not equals operator
		bool operator!=(const VectorFontAsset& other) const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns font pointer
		FontRef GetFont() const;

		// Returns font effects array 
		const EffectsVec& GetEffects() const;

		// Adds effect to font
		void AddEffect(VectorFont::Effect* effect);

		// Adds effect to font
		template<typename _type, typename ... _args>
		void AddEffect(_args ... args);

		// Removes effect from font
		void RemoveEffect(VectorFont::Effect* effect);

		// Removes all effects
		void RemoveAllEffects();

		// Returns extensions string
		const char* GetFileExtensions() const;

		SERIALIZABLE(VectorFontAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			// Destructor
			~MetaInfo();

			// Returns asset type id
			const Type* GetAssetType() const;

			// Returns true if other meta is equal to this
			bool IsEqual(IMetaInfo* other) const;

			SERIALIZABLE(MetaInfo);

		protected:
			EffectsVec mEffects; // Font effects array @SERIALIZABLE

			friend class VectorFontAsset;
		};

	protected:
		FontRef mFont;

	protected:
		// Default constructor
		VectorFontAsset();

		// Constructor by path - loads asset by path
		VectorFontAsset(const String& path);

		// Constructor by id - loads asset by id
		VectorFontAsset(UID id);

		// Copy-constructor
		VectorFontAsset(const VectorFontAsset& asset);

		// Loads data
		void LoadData(const String& path);

		// Initializes properties
		void InitializeProperties();

		friend class Assets;
	};

	// ---------------------------
	// Vector font Asset reference
	// ---------------------------
	class VectorFontAssetRef: public AssetRef
	{
	public:
		// Creates VectorFontAsset and returns reference to it
		static VectorFontAssetRef CreateAsset();

		// Default constructor, references to null
		VectorFontAssetRef(): AssetRef() {}

		// Copy-constructor
		VectorFontAssetRef(const AssetRef& other): AssetRef(other) { CheckType<VectorFontAsset>(); }

		// Copy-constructor
		VectorFontAssetRef(const VectorFontAssetRef& other): AssetRef(other) {}

		// Constructor from asset path
		VectorFontAssetRef(const String& path): AssetRef(path) {}

		// Constructor from asset id
		VectorFontAssetRef(UID id): AssetRef(id) {}

		// Destructor
		~VectorFontAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		VectorFontAssetRef& operator=(const VectorFontAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		VectorFontAsset& operator*() { return *((VectorFontAsset*)mAssetPtr); }

		// Constant getter operator
		const VectorFontAsset& operator*() const { return *((VectorFontAsset*)mAssetPtr); }

		// Asset members and field operator
		VectorFontAsset* operator->() { return ((VectorFontAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const VectorFontAsset* operator->() const { return ((VectorFontAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const VectorFontAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const VectorFontAssetRef& other) const { return AssetRef::operator!=(other); }

		SERIALIZABLE(VectorFontAssetRef);

	protected:
		// Constructor for Assets manager
		VectorFontAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};

	template<typename _type, typename ... _args>
	void VectorFontAsset::AddEffect(_args ... args)
	{
		AddEffect(mnew _type(args ...));
	}
}