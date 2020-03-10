#pragma once

#include "o2/Assets/Types/FontAsset.h"
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
		class Meta;

	public:
		PROPERTIES(VectorFontAsset);
		GETTER(Meta*, meta, GetMeta); // Meta information getter

	public:
		// Check equals operator
		VectorFontAsset& operator=(const VectorFontAsset& asset);

		// Returns meta information
		Meta* GetMeta() const;

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

		// Returns editor sorting weight
		static int GetEditorSorting() { return 92; }

		ASSET_TYPE(VectorFontAsset, Meta);

	public:
		// ----------------
		// Meta information
		// ----------------
		class Meta: public DefaultAssetMeta<VectorFontAsset>
		{
		public:
			// Destructor
			~Meta();

			// Returns true if other meta is equal to this
			bool IsEqual(AssetMeta* other) const override;

			SERIALIZABLE(Meta);

		protected:
			Vector<VectorFont::Effect*> mEffects; // Font effects array @SERIALIZABLE @EDITOR_PROPERTY @EXPANDED_BY_DEFAULT

			friend class VectorFontAsset;
		};

	protected:
		// Default constructor
		VectorFontAsset();

		// Copy-constructor
		VectorFontAsset(const VectorFontAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		// Saves asset data, using DataNode and serialization
		void SaveData(const String& path) const override;

		friend class Assets;
	};

	typedef Ref<VectorFontAsset> VectorFontAssetRef;

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

	PUBLIC_FUNCTION(Meta*, GetMeta);
	PUBLIC_FUNCTION(const Vector<VectorFont::Effect*>&, GetEffects);
	PUBLIC_FUNCTION(void, AddEffect, VectorFont::Effect*);
	PUBLIC_FUNCTION(void, RemoveEffect, VectorFont::Effect*);
	PUBLIC_FUNCTION(void, RemoveAllEffects);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PUBLIC_STATIC_FUNCTION(int, GetEditorSorting);
	PROTECTED_FUNCTION(void, LoadData, const String&);
	PROTECTED_FUNCTION(void, SaveData, const String&);
}
END_META;

CLASS_BASES_META(o2::VectorFontAsset::Meta)
{
	BASE_CLASS(o2::DefaultAssetMeta<VectorFontAsset>);
}
END_META;
CLASS_FIELDS_META(o2::VectorFontAsset::Meta)
{
	PROTECTED_FIELD(mEffects).EDITOR_PROPERTY_ATTRIBUTE().EXPANDED_BY_DEFAULT_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::VectorFontAsset::Meta)
{

	PUBLIC_FUNCTION(bool, IsEqual, AssetMeta*);
}
END_META;
