#pragma once

#include "o2/Assets/FontAsset.h"
#include "o2/Render/VectorFont.h"

namespace o2
{
	class Font;

	// -----------------
	// Vector font asset
	// -----------------
	class VectorFontAsset: public FontAsset
	{
	public:
		class MetaInfo;

	public:
		PROPERTIES(VectorFontAsset);
		GETTER(MetaInfo*, meta, GetMeta); // Meta information getter

	public:
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

		// Returns font effects array 
		const Vector<VectorFont::Effect*>& GetEffects() const;

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
		const char* GetFileExtensions() const override;

		SERIALIZABLE(VectorFontAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public AssetMeta
		{
		public:
			// Destructor
			~MetaInfo();

			// Returns asset type id
			const Type* GetAssetType() const override;

			// Returns true if other meta is equal to this
			bool IsEqual(AssetMeta* other) const override;

			SERIALIZABLE(MetaInfo);

		protected:
			Vector<VectorFont::Effect*> mEffects; // Font effects array @SERIALIZABLE

			friend class VectorFontAsset;
		};

	protected:
		// Default constructor
		VectorFontAsset();

		// Constructor by path - loads asset by path
		VectorFontAsset(const String& path);

		// Constructor by id - loads asset by id
		VectorFontAsset(const UID& id);

		// Copy-constructor
		VectorFontAsset(const VectorFontAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		friend class Assets;
	};

	// ---------------------------
	// Vector font Asset reference
	// ---------------------------
	class VectorFontAssetRef: public FontAssetRef
	{
	public:
		// Creates VectorFontAsset and returns reference to it
		static VectorFontAssetRef CreateAsset();

		// Default constructor, references to null
		VectorFontAssetRef(): FontAssetRef() {}

		// Copy-constructor
		VectorFontAssetRef(const IAssetRef& other): FontAssetRef(other) { CheckType<VectorFontAsset>(); }

		// Copy-constructor
		VectorFontAssetRef(const VectorFontAssetRef& other): FontAssetRef(other) {}

		// Constructor from asset path
		VectorFontAssetRef(const String& path): FontAssetRef(path) {}

		// Constructor from asset id
		VectorFontAssetRef(const UID& id): FontAssetRef(id) {}

		// Destructor
		~VectorFontAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		VectorFontAssetRef& operator=(const VectorFontAssetRef& other) { IAssetRef::operator=(other); return *this; }

		// Getter operator
		VectorFontAsset& operator*() { return *((VectorFontAsset*)mAssetPtr); }

		// Constant getter operator
		const VectorFontAsset& operator*() const { return *((VectorFontAsset*)mAssetPtr); }

		// Asset members and field operator
		VectorFontAsset* operator->() { return ((VectorFontAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const VectorFontAsset* operator->() const { return ((VectorFontAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const VectorFontAssetRef& other) const { return IAssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const VectorFontAssetRef& other) const { return IAssetRef::operator!=(other); }

		// Returns asset type
		const Type& GetAssetType() const override { return TypeOf(VectorFontAsset); }

		SERIALIZABLE(VectorFontAssetRef);

	protected:
		// Constructor for Assets manager
		VectorFontAssetRef(Asset* assetPtr, int* refCounter): FontAssetRef(assetPtr, refCounter) {}
	};

	template<typename _type, typename ... _args>
	void VectorFontAsset::AddEffect(_args ... args)
	{
		AddEffect(mnew _type(args ...));
	}
}

CLASS_BASES_META(o2::VectorFontAsset)
{
	BASE_CLASS(o2::FontAsset);
}
END_META;
CLASS_FIELDS_META(o2::VectorFontAsset)
{
	PUBLIC_FIELD(meta);
}
END_META;
CLASS_METHODS_META(o2::VectorFontAsset)
{

	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(const Vector<VectorFont::Effect*>&, GetEffects);
	PUBLIC_FUNCTION(void, AddEffect, VectorFont::Effect*);
	PUBLIC_FUNCTION(void, RemoveEffect, VectorFont::Effect*);
	PUBLIC_FUNCTION(void, RemoveAllEffects);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
}
END_META;

CLASS_BASES_META(o2::VectorFontAssetRef)
{
	BASE_CLASS(o2::FontAssetRef);
}
END_META;
CLASS_FIELDS_META(o2::VectorFontAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::VectorFontAssetRef)
{

	PUBLIC_STATIC_FUNCTION(VectorFontAssetRef, CreateAsset);
	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_BASES_META(o2::VectorFontAsset::MetaInfo)
{
	BASE_CLASS(o2::AssetMeta);
}
END_META;
CLASS_FIELDS_META(o2::VectorFontAsset::MetaInfo)
{
	PROTECTED_FIELD(mEffects).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::VectorFontAsset::MetaInfo)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
	PUBLIC_FUNCTION(bool, IsEqual, AssetMeta*);
}
END_META;
