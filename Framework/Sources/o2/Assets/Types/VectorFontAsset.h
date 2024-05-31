#pragma once

#include "o2/Assets/Types/FontAsset.h"
#include "o2/Render/VectorFont.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"

namespace o2
{
    // -----------------
    // Vector font asset
    // -----------------
    class VectorFontAsset: public FontAsset
    {
    public:
        class Meta;

    public:
        PROPERTIES(VectorFontAsset);
        GETTER(Ref<Meta>, meta, GetMeta); // Meta information getter

    public:
        // Default constructor
        VectorFontAsset();

        // Copy-constructor
        VectorFontAsset(const VectorFontAsset& asset);

        // Check equals operator
        VectorFontAsset& operator=(const VectorFontAsset& asset);

        // Returns meta information
        Ref<Meta> GetMeta() const;

        // Returns font effects array 
        const Vector<Ref<VectorFont::Effect>>& GetEffects() const;

        // Adds effect to font
        void AddEffect(const Ref<VectorFont::Effect>& effect);

        // Adds effect to font
        template<typename _type, typename ... _args>
        void AddEffect(_args ... args);

        // Removes effect from font
        void RemoveEffect(const Ref<VectorFont::Effect>& effect);

        // Removes all effects
        void RemoveAllEffects();

        // Returns extensions string
        static Vector<String> GetFileExtensions();

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
            // Default constructor
            Meta();

            // Copy-constructor
            Meta(const Meta& other);

            // Returns true if other meta is equal to this
            bool IsEqual(AssetMeta* other) const override;

            SERIALIZABLE(Meta);
            CLONEABLE_REF(Meta);

        protected:
            Vector<Ref<VectorFont::Effect>> mEffects; // Font effects array @SERIALIZABLE @EDITOR_PROPERTY @EXPANDED_BY_DEFAULT @INVOKE_ON_CHANGE(UpdateFontEffects)
            
            WeakRef<VectorFontAsset> mAsset = nullptr; // Asset pointer

        protected:
            // Calls UpdateFontEffects from asset
            void UpdateFontEffects();

            friend class VectorFontAsset;
        };

    protected:
        // Loads data
        void LoadData(const String& path) override;

        // Saves asset data, using DataValue and serialization
        void SaveData(const String& path) const override;

        // Updates font effects in 
        void UpdateFontEffects();

        friend class Assets;
    };

    template<typename _type, typename ... _args>
    void VectorFontAsset::AddEffect(_args ... args)
    {
        AddEffect(mmake<_type>(args ...));
    }
}
// --- META ---

CLASS_BASES_META(o2::VectorFontAsset)
{
    BASE_CLASS(o2::FontAsset);
}
END_META;
CLASS_FIELDS_META(o2::VectorFontAsset)
{
    FIELD().PUBLIC().NAME(meta);
}
END_META;
CLASS_METHODS_META(o2::VectorFontAsset)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const VectorFontAsset&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Meta>, GetMeta);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Ref<VectorFont::Effect>>&, GetEffects);
    FUNCTION().PUBLIC().SIGNATURE(void, AddEffect, const Ref<VectorFont::Effect>&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveEffect, const Ref<VectorFont::Effect>&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllEffects);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Vector<String>, GetFileExtensions);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(int, GetEditorSorting);
    FUNCTION().PROTECTED().SIGNATURE(void, LoadData, const String&);
    FUNCTION().PROTECTED().SIGNATURE(void, SaveData, const String&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateFontEffects);
}
END_META;

CLASS_BASES_META(o2::VectorFontAsset::Meta)
{
    BASE_CLASS(o2::DefaultAssetMeta<VectorFontAsset>);
}
END_META;
CLASS_FIELDS_META(o2::VectorFontAsset::Meta)
{
    FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().EXPANDED_BY_DEFAULT_ATTRIBUTE().INVOKE_ON_CHANGE_ATTRIBUTE(UpdateFontEffects).SERIALIZABLE_ATTRIBUTE().NAME(mEffects);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAsset);
}
END_META;
CLASS_METHODS_META(o2::VectorFontAsset::Meta)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const Meta&);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEqual, AssetMeta*);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateFontEffects);
}
END_META;
// --- END META ---
