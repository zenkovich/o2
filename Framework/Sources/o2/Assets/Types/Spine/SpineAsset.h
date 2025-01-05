#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetRef.h"
#include "SpineAtlasAsset.h"

#include "spine/SkeletonData.h"
#include "spine/AnimationStateData.h"

namespace o2
{
    // ----------------------------
    // EsotericSoftware spine asset 
    // ----------------------------
    class SpineAsset : public AssetWithDefaultMeta<SpineAsset>
    {
    public:
		PROPERTIES(SpineAsset);

    public:
        // Default constructor
        SpineAsset();

        // Copy-constructor
        SpineAsset(const SpineAsset& asset);

        // Destructor
        ~SpineAsset();

        // Check equals operator
        SpineAsset& operator=(const SpineAsset& asset);

		// Returns spine skeleton data
		spine::SkeletonData* GetSpineSkeletonData();

		// Returns spine animation state data
		spine::AnimationStateData* GetSpineAnimationStateData();

        // Returns extensions string
        static Vector<String> GetFileExtensions();

        // Returns editor sorting weight
        static int GetEditorSorting() { return 96; }

		SERIALIZABLE(SpineAsset);
		CLONEABLE_REF(SpineAsset);

    protected:
		spine::SkeletonData*       mSkeletonData = nullptr;       // Spine skeleton data
		spine::AnimationStateData* mAnimationStateData = nullptr; // Spine animation state data

    protected:
        // Loads asset data, using DataValue and serialization
        void LoadData(const String& path) override;

        // Saves asset data, using DataValue and serialization
        void SaveData(const String& path) const override;

        friend class Assets;
    };
}
// --- META ---

CLASS_BASES_META(o2::SpineAsset)
{
    BASE_CLASS(o2::AssetWithDefaultMeta<SpineAsset>);
}
END_META;
CLASS_FIELDS_META(o2::SpineAsset)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mSkeletonData);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAnimationStateData);
}
END_META;
CLASS_METHODS_META(o2::SpineAsset)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const SpineAsset&);
    FUNCTION().PUBLIC().SIGNATURE(spine::SkeletonData*, GetSpineSkeletonData);
    FUNCTION().PUBLIC().SIGNATURE(spine::AnimationStateData*, GetSpineAnimationStateData);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Vector<String>, GetFileExtensions);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(int, GetEditorSorting);
    FUNCTION().PROTECTED().SIGNATURE(void, LoadData, const String&);
    FUNCTION().PROTECTED().SIGNATURE(void, SaveData, const String&);
}
END_META;
// --- END META ---
