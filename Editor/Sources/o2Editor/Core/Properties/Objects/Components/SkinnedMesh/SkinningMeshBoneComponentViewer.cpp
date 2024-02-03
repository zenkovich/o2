#include "o2Editor/stdafx.h"
#include "SkinningMeshBoneComponentViewer.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
    SkinningMeshBoneComponentViewer::SkinningMeshBoneComponentViewer() = default;
    
    SkinningMeshBoneComponentViewer::~SkinningMeshBoneComponentViewer()
    {
        o2EditorSceneScreen.RemoveTool(&mWeightsTool);
    }
    
    SkinningMeshBoneComponentViewer& SkinningMeshBoneComponentViewer::operator=(const SkinningMeshBoneComponentViewer& other)
    {
        TObjectPropertiesViewer<SkinningMeshBoneComponent>::operator=(other);
        return *this;
    }
    
    void SkinningMeshBoneComponentViewer::RebuildProperties(const Vector<Pair<Ref<IObject>, Ref<IObject>>>& targetObjects)
    {
        o2EditorProperties.BuildObjectProperties(mSpoiler, &TypeOf(SkinningMeshBoneComponent), mPropertiesContext,
                                                 "", mOnChildFieldChangeCompleted, onChanged);
    }
    
    void SkinningMeshBoneComponentViewer::OnRefreshed(const Vector<Pair<Ref<IObject>, Ref<IObject>>>& targetObjects)
    {
        auto prevTargetObjects = mTypeTargetObjects;
    
        TObjectPropertiesViewer<SkinningMeshBoneComponent>::OnRefreshed(targetObjects);
    
        if (!mTypeTargetObjects.IsEmpty() && prevTargetObjects != mTypeTargetObjects)
        {
            mWeightsTool.boneComponent = mTypeTargetObjects[0].first;
    
            mFrameTetxureLayer.mesh = mTypeTargetObjects[0].first->FindSkinningMesh();
        }
    }
    
    void SkinningMeshBoneComponentViewer::OnEnabled()
    {
        o2EditorSceneScreen.AddTool(&mWeightsTool);
        o2EditorSceneScreen.SelectTool<MeshWeightsTool>();
    
        o2EditorSceneScreen.AddEditorLayer(mmake<WeakRef<EditorLayer>>(&mFrameTetxureLayer));
    }
    
    void SkinningMeshBoneComponentViewer::OnDisabled()
    {
        o2EditorSceneScreen.RemoveTool(&mWeightsTool);
        o2EditorSceneScreen.RemoveEditorLayer(mmake<WeakRef<EditorLayer>>(&mFrameTetxureLayer));
    }
}

DECLARE_TEMPLATE_CLASS(Editor::TObjectPropertiesViewer<SkinningMeshBoneComponent>);
// --- META ---

DECLARE_CLASS(Editor::SkinningMeshBoneComponentViewer, Editor__SkinningMeshBoneComponentViewer);
// --- END META ---