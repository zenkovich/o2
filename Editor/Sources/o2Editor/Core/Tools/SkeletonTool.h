#pragma once
#include "IEditorTool.h"

#include "o2/Scene/Components/SkinningMeshBoneComponent.h"
#include "o2/Scene/Components/SkinningMeshComponent.h"
#include "o2/Utils/Editor/FrameHandles.h"
#include "o2Editor/SceneWindow/SceneDragHandle.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"
#include "o2/Utils/SmartPointers/Ref.h"
#include "o2/Utils/SmartPointers/WeakRef.h"
#include "o2/Utils/SmartPointers/mmake.h"

namespace Editor
{
    // -------------------------------------------------
    // Skeleton tool. Used for editing bones of skeleton
    // -------------------------------------------------
    struct SkeletonTool : public IEditTool
    {
        // ---------------------------------------------------------------------
        // Scene layer. Used to draw and update stuff of bones handles and other
        // ---------------------------------------------------------------------
        struct SceneLayer : public SceneEditorLayer
        {
            SkeletonTool* tool = nullptr;

        public:
            void DrawOverScene() override;
            void Update(float dt) override;

            int GetOrder() const override;

            bool IsEnabled() const override;

            const String& GetName() const override;
            const String& GetIconName() const override;
        };

        // ---------------------------------------------------------
        // Bone handle container. Keeps handle and reference to bone
        // ---------------------------------------------------------
        struct BoneHandle
        {
            SceneDragHandle* handle = nullptr;                            // Bone handle in scene
            Ref<SkinningMeshBoneComponent> boneComponent;                 // Bone component

            Basis pressedTransform;                                       // Stored bone transform when pressed

        public:
            // Default constructor, creates handle
            BoneHandle()
                : handle(nullptr), boneComponent(nullptr), pressedTransform(Basis())
            {
            }

            // Destructor, destroys handle
            ~BoneHandle()
            {
                delete handle;
            }

            // Updates handle position by bone transform
            void UpdatePosition()
            {
                if (handle && boneComponent)
                {
                    handle->SetPosition(boneComponent->GetLocalPosition());
                }
            }

        private:
            // Called when handle is moved, changes bone transformation
            void OnHandleChangedPos(const Vec2F& pos);
        };

        // ---------------------------------------------------------------------------------
        // Skeleton edit instance. Contains handles and reference to skinning mesh component
        // ---------------------------------------------------------------------------------
        struct SkeletonInstance
        {
            Ref<SkinningMeshComponent> skinningMeshComponent;         // Reference to component
            Vector<BoneHandle*>        boneHandles;                   // List of bones handles

        public:
            // Updates handles positions by bones transforms
            void UpdateHandlesPositions()
            {
                for (auto& handle : boneHandles)
                {
                    if (handle)
                    {
                        handle->UpdatePosition();
                    }
                }
            }
        };

    public:
        SceneLayer   sceneLayer;                             // Scene layer for drawing spline
        bool         isEnabled = false;                     // Is tool enabled now     

    public:
        // Default constructor
        SkeletonTool()
            : IEditTool(), sceneLayer(), isEnabled(false)
        {
        }

        // Returns toggle in menu panel icon name
        String GetPanelIcon() const override;

        // Called when tool was enabled
        void OnEnabled() override;

        // Called when tool was disabled
        void OnDisabled() override;

        // Adds and enables handles for editing skeleton bones
        void AddSkeletonInstance(const Ref<SkinningMeshComponent>& skinningMeshComponent)
        {
            auto skeletonInstance = mmake<SkeletonInstance>();
            skeletonInstance->skinningMeshComponent = skinningMeshComponent;

            for (auto& bone : skinningMeshComponent->GetBones())
            {
                auto boneHandle = mmake<BoneHandle>();
                boneHandle->boneComponent = bone;
                boneHandle->handle = new SceneDragHandle();
                skeletonInstance->boneHandles.push_back(boneHandle);
            }

            mSkeletons.push_back(skeletonInstance);
        }

        // Removing skeleton from editing
        void RemoveSkeletonInstance(const Ref<SkinningMeshComponent>& skinningMeshComponent)
        {
            for (auto it = mSkeletons.begin(); it != mSkeletons.end(); ++it)
            {
                if ((*it)->skinningMeshComponent == skinningMeshComponent)
                {
                    for (auto& boneHandle : (*it)->boneHandles)
                    {
                        delete boneHandle;
                    }
                    mSkeletons.erase(it);
                    return;
                }
            }
        }

        // Returns whether the given skeleton is already being edited
        bool IsEditingSkeleton(const Ref<SkinningMeshComponent>& skinningMeshComponent) const
        {
            for (auto& skeleton : mSkeletons)
            {
                if (skeleton->skinningMeshComponent == skinningMeshComponent)
                {
                    return true;
                }
            }
            return false;
        }

        // Returns list of editing skeletons
        const Vector<SkeletonInstance*>& GetEditingSkeletons() const
        {
            return mSkeletons;
        }

        IOBJECT(SkeletonTool);

    private:
        Vector<SkeletonInstance*> mSkeletons; // List of editing skeletons
    };
}
// --- META ---

CLASS_BASES_META(Editor::SkeletonTool)
{
    BASE_CLASS(Editor::IEditTool);
}
END_META;
CLASS_FIELDS_META(Editor::SkeletonTool)
{
    FIELD().PUBLIC().NAME(sceneLayer);
    FIELD().PUBLIC().NAME(::isEnabled);
    FIELD().PRIVATE().NAME(mSkeletons);
}
END_META;
CLASS_METHODS_META(Editor::SkeletonTool)
{
    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(String, GetPanelIcon);
    FUNCTION().PUBLIC().SIGNATURE(void, OnEnabled);
    FUNCTION().PUBLIC().SIGNATURE(void, OnDisabled);
    FUNCTION().PUBLIC().SIGNATURE(void, AddSkeletonInstance, const Ref<SkinningMeshComponent>&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveSkeletonInstance, const Ref<SkinningMeshComponent>&);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEditingSkeleton, const Ref<SkinningMeshComponent>&);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<SkeletonInstance*>&, GetEditingSkeletons);
}
END_META;
// --- END META ---