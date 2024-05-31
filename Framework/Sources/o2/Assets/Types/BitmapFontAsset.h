#pragma once

#include "o2/Assets/Types/FontAsset.h"

namespace o2
{
    // -----------------
    // Bitmap font asset
    // -----------------
    class BitmapFontAsset: public FontAsset
    {
    public:
        class Meta;

    public:
        PROPERTIES(BitmapFontAsset);
        GETTER(Ref<Meta>, meta, GetMeta); // Meta information getter

    public:
        // Default constructor
        BitmapFontAsset();

        // Copy-constructor
        BitmapFontAsset(const BitmapFontAsset& asset);

        // Check equals operator
        BitmapFontAsset& operator=(const BitmapFontAsset& asset);

        // Returns meta information
        Ref<Meta> GetMeta() const;

        // Returns extensions string
        static Vector<String> GetFileExtensions();

        // Returns editor sorting weight
        static int GetEditorSorting() { return 95; }

        ASSET_TYPE(BitmapFontAsset, Meta);

    public:
        // ----------------
        // Meta information
        // ----------------
        class Meta: public DefaultAssetMeta<BitmapFontAsset>
        {
        public:
            SERIALIZABLE(Meta);
            CLONEABLE_REF(Meta);
        };

    protected:
        // Loads data
        void LoadData(const String& path) override;

        friend class Assets;
    };
}
// --- META ---

CLASS_BASES_META(o2::BitmapFontAsset)
{
    BASE_CLASS(o2::FontAsset);
}
END_META;
CLASS_FIELDS_META(o2::BitmapFontAsset)
{
    FIELD().PUBLIC().NAME(meta);
}
END_META;
CLASS_METHODS_META(o2::BitmapFontAsset)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const BitmapFontAsset&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Meta>, GetMeta);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Vector<String>, GetFileExtensions);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(int, GetEditorSorting);
    FUNCTION().PROTECTED().SIGNATURE(void, LoadData, const String&);
}
END_META;

CLASS_BASES_META(o2::BitmapFontAsset::Meta)
{
    BASE_CLASS(o2::DefaultAssetMeta<BitmapFontAsset>);
}
END_META;
CLASS_FIELDS_META(o2::BitmapFontAsset::Meta)
{
}
END_META;
CLASS_METHODS_META(o2::BitmapFontAsset::Meta)
{
}
END_META;
// --- END META ---
