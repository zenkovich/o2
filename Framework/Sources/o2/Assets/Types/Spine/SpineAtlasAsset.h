#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetRef.h"

#include "spine/Atlas.h"

namespace o2
{
    // -----------------------------------
    // EsotericSowftware spine atlas asset 
    // -----------------------------------
    class SpineAtlasAsset: public AssetWithDefaultMeta<SpineAtlasAsset>
    {
    public:
        // Default constructor
        SpineAtlasAsset();

        // Copy-constructor
        SpineAtlasAsset(const SpineAtlasAsset& asset);

        // Destructor
        ~SpineAtlasAsset();

        // Check equals operator
        SpineAtlasAsset& operator=(const SpineAtlasAsset& asset);

		// Returns spine atlas
		spine::Atlas* GetSpineAtlas();

        // Returns extensions string
        static Vector<String> GetFileExtensions();

        // Returns editor sorting weight
        static int GetEditorSorting() { return 96; }

        SERIALIZABLE(SpineAtlasAsset);
        CLONEABLE_REF(SpineAtlasAsset);

    protected:
		spine::Atlas* mAtlas = nullptr; // Spine atlas

    protected:
        // Loads asset data, using DataValue and serialization
        void LoadData(const String& path) override;

        // Saves asset data, using DataValue and serialization
        void SaveData(const String& path) const override;

        friend class Assets;
    };
}
// --- META ---

CLASS_BASES_META(o2::SpineAtlasAsset)
{
    BASE_CLASS(o2::AssetWithDefaultMeta<SpineAtlasAsset>);
}
END_META;
CLASS_FIELDS_META(o2::SpineAtlasAsset)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAtlas);
}
END_META;
CLASS_METHODS_META(o2::SpineAtlasAsset)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const SpineAtlasAsset&);
    FUNCTION().PUBLIC().SIGNATURE(spine::Atlas*, GetSpineAtlas);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Vector<String>, GetFileExtensions);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(int, GetEditorSorting);
    FUNCTION().PROTECTED().SIGNATURE(void, LoadData, const String&);
    FUNCTION().PROTECTED().SIGNATURE(void, SaveData, const String&);
}
END_META;
// --- END META ---
