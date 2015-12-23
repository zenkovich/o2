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

		typedef Vector<Ptr<VectorFont::Effect>> EffectsVec;

	public:
		Getter<Ptr<MetaInfo>> meta; // Meta information getter
		Getter<FontRef>       font; // Font getter

        // Default constructor
		VectorFontAsset();

		// Constructor by path - loads asset by path
		VectorFontAsset(const String& path);

		// Constructor by id - loads asset by id
		VectorFontAsset(AssetId id);

		// Copy-constructor
		VectorFontAsset(const VectorFontAsset& asset);

		// Destructor
		~VectorFontAsset();

		// Check equals operator
		VectorFontAsset& operator=(const VectorFontAsset& asset);

		// Returns meta information
		Ptr<MetaInfo> GetMeta() const;

		// Returns font pointer
		FontRef GetFont() const;

		// Returns font effects array 
		const EffectsVec& GetEffects() const;

		// Adds effect to font
		void AddEffect(Ptr<VectorFont::Effect> effect);

		// Adds effect to font
		template<typename _type, typename ... _args>
		void AddEffect(_args ... args);

		// Removes effect from font
		void RemoveEffect(Ptr<VectorFont::Effect> effect);

		// Removes all effects
		void RemoveAllEffects();

		// Returns extensions string
		const char* GetFileExtensions() const;

		IOBJECT(VectorFontAsset);

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
			Type::Id GetAssetType() const;

			// Returns true if other meta is equal to this
			bool IsEqual(Ptr<IMetaInfo> other) const;

			SERIALIZABLE(MetaInfo);

		protected:
			EffectsVec mEffects; // Font effects array @SERIALIZABLE

			friend class VectorFontAsset;
		};

	protected:
		FontRef mFont;

	protected:
		// Loads data
		void LoadData(const String& path);

		// Initializes properties
		void InitializeProperties();
	};

	template<typename _type, typename ... _args>
	void VectorFontAsset::AddEffect(_args ... args)
	{
		AddEffect(mnew _type(args ...));
	}
}