#pragma once

#include "o2/Render/ParticlesEmitter.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"

namespace o2
{
    class SkinningMeshComponent;

    class SkinningMeshBoneComponent: public DrawableComponent
    {
        SERIALIZABLE(SkinningMeshBoneComponent);

    public:
        Vector<Pair<int, float>> vertexWeights; // Weights of vertices dependent on this bone. index - weight @SERIALIZABLE

        float length = 100; // Bone length. Used for visualization @SERIALIZABLE

    public:
        // Default constructor
        SkinningMeshBoneComponent();

        //Copy constructor
        SkinningMeshBoneComponent(const SkinningMeshBoneComponent& other);

        //Destructor
        ~SkinningMeshBoneComponent();

        // Copy operator
        SkinningMeshBoneComponent& operator=(const SkinningMeshBoneComponent& other);

        // Draw particle system
        void Draw() override;

        // Updates component
        void OnUpdate(float dt) override;

        // Searches skinning mesh in parent hierarchy
        Ref<SkinningMeshComponent> FindSkinningMesh() const;

        // Returns name of component
        static String GetName();

        // Returns category of component
        static String GetCategory();

        // Returns name of component icon
        static String GetIcon();

    protected:
        // Called when actor's transform was changed
        void OnTransformUpdated() override;

        // Called when actor was included to scene
        void OnAddToScene() override;

        // Called when actor was excluded from scene
        void OnRemoveFromScene() override;
    };
}
// --- META ---

CLASS_BASES_META(o2::SkinningMeshBoneComponent)
{
    BASE_CLASS(o2::DrawableComponent);
}
END_META;
CLASS_FIELDS_META(o2::SkinningMeshBoneComponent)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(vertexWeights);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(100).NAME(length);
}
END_META;
CLASS_METHODS_META(o2::SkinningMeshBoneComponent)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const SkinningMeshBoneComponent&);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(void, OnUpdate, float);
    FUNCTION().PUBLIC().SIGNATURE(Ref<SkinningMeshComponent>, FindSkinningMesh);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetName);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCategory);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetIcon);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
    FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
    FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
}
END_META;
// --- END META ---
