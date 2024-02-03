#pragma once

#include "o2/Scene/Components/SkinningMeshBoneComponent.h"
#include "o2/Scene/Components/SkinningMeshComponent.h"
#include "o2Editor/Core/Properties/IObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Objects/Components/SkinnedMesh/SkinningMeshEditorLayer.h"
#include "o2Editor/Core/Tools/CustomFrameTool.h"
#include "o2Editor/Core/Tools/MeshTopologyTool.h"
#include "o2Editor/Core/Tools/SplineTool.h"
#include "o2Editor/Core/UI/SplineEditor/SplineEditor.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"
#include "o2/Core/Ref.h"
#include "o2/Core/WeakRef.h"

using namespace o2;

namespace o2
{
    class Spoiler;
    class Button;
}

namespace Editor
{
    class SceneDragHandle;
    struct SkeletonTool;

    // ------------------------------
    // Skinning Mesh component viewer
    // ------------------------------
    class SkinningMeshComponentViewer : public TObjectPropertiesViewer<Ref<SkinningMeshComponent>>
    {
    public:
        // Default constructor
        SkinningMeshComponentViewer();

        // Destructor
        ~SkinningMeshComponentViewer();

        // Copy operator
        SkinningMeshComponentViewer& operator=(const SkinningMeshComponentViewer& other);

        IOBJECT(SkinningMeshComponentViewer);

    protected:
        SplineTool              mSplineTool;                  // Spline tool
        CustomFrameTool         mFrameTool;                   // Mapping frame tool
        MeshTopologyTool        mTopologyTool;                // Mesh topology tool
        Ref<SkeletonTool>       mSkeletonTool;                // Shared skeleton tool
        SkinningMeshEditorLayer mFrameTetxureLayer;           // Frame texture drawing layer

        Ref<Button> mFitAndCenterButton;  // Fit and centerize button
        Ref<Button> mEditSkeletonButton;  // Enable/disable skeleton editing button
        Ref<Button> mReskinButton;        // Resets mesh skinning and updates bones transforms

        bool mEditingSkeleton = false;  // It editing skeleton enabled

    protected:
        // Called when the viewer is refreshed, builds properties, and places them in mPropertiesContext
        void RebuildProperties(const Vector<Pair<Ref<IObject>, Ref<IObject>>>& targetObjets) override;

        // Called when viewer is refreshed
        void OnRefreshed(const Vector<Pair<Ref<IObject>, Ref<IObject>>>& targetObjets) override;

        // Enable viewer event function
        void OnEnabled() override;

        // Disable viewer event function
        void OnDisabled() override;

        // Called when button pressed
        void FitAndCenterize();

        // Called when edit skeleton pressed
        void OnEditSkeletonPressed();

        // Called when reskin pressed
        void OnReskinPressed();

        // Updates the caption on the skeleton edit button depending on whether it is enabled or not
        void UpdateEditSkeletonButtonCaption();
    };
}

// --- META ---

CLASS_BASES_META(Editor::SkinningMeshComponentViewer)
{
    BASE_CLASS(Editor::TObjectPropertiesViewer<Ref<SkinningMeshComponent>>);
}
END_META;

CLASS_FIELDS_META(Editor::SkinningMeshComponentViewer)
{
    FIELD().PROTECTED().NAME(mSplineTool),
    FIELD().PROTECTED().NAME(mFrameTool),
    FIELD().PROTECTED().NAME(mTopologyTool),
    FIELD().PROTECTED().DEFAULT_VALUE(MakeRef<SkeletonTool>()).NAME(mSkeletonTool),
    FIELD().PROTECTED().NAME(mFrameTetxureLayer),
    FIELD().PROTECTED().DEFAULT_VALUE(MakeRef<Button>()).NAME(mFitAndCenterButton),
    FIELD().PROTECTED().DEFAULT_VALUE(MakeRef<Button>()).NAME(mEditSkeletonButton),
    FIELD().PROTECTED().DEFAULT_VALUE(MakeRef<Button>()).NAME(mReskinButton),
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mEditingSkeleton)
}
END_META;

CLASS_METHODS_META(Editor::SkinningMeshComponentViewer)
{
    typedef const Vector<Pair<Ref<IObject>, Ref<IObject>>>& _tmp1;
    typedef const Vector<Pair<Ref<IObject>, Ref<IObject>>>& _tmp2;

    FUNCTION().PUBLIC().CONSTRUCTOR(),
    FUNCTION().PROTECTED().SIGNATURE(void, RebuildProperties, const Ref<IObject>&),
    FUNCTION().PROTECTED().SIGNATURE(void, OnRefreshed, const _tmp2&),
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled),
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled),
    FUNCTION().PROTECTED().SIGNATURE(void, FitAndCenterize),
    FUNCTION().PROTECTED().SIGNATURE(void, OnEditSkeletonPressed),
    FUNCTION().PROTECTED().SIGNATURE(void, OnReskinPressed),
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateEditSkeletonButtonCaption)
}
END_META;
// --- END META ---