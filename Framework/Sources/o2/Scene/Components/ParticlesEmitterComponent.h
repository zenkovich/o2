#pragma once

#include "o2/Render/ParticlesEmitter.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"

namespace o2
{
    class ParticlesEmitterComponent: public DrawableComponent, public ParticlesEmitter
    {
    public:
        ParticlesEmitterComponent();
        ParticlesEmitterComponent(const ParticlesEmitterComponent& other);
        ~ParticlesEmitterComponent();

        ParticlesEmitterComponent& operator=(const ParticlesEmitterComponent& other);

        // Draw particle system
        void Draw() override;

        // Updates component
        void OnUpdate(float dt) override;

        // Returns true if point is under drawable
        bool IsUnderPoint(const Vec2F& point) override;

        // Returns name of component
        static String GetName();

        // Returns category of component
        static String GetCategory();

        // Returns name of component icon
		static String GetIcon();

		// Dynamic cast to RefCounterable via DrawableComponent
		static Ref<RefCounterable> CastToRefCounterable(const Ref<ParticlesEmitterComponent>& ref);

        SERIALIZABLE(ParticlesEmitterComponent);
        CLONEABLE_REF(ParticlesEmitterComponent);

    protected:
        // Called when actor's transform was changed
        void OnTransformUpdated() override;

        // Beginning serialization callback
        void OnSerialize(DataValue& node) const override;

        // Called when object was deserialized
        void OnDeserialized(const DataValue& node) override;

        // Beginning serialization delta callback
        void OnSerializeDelta(DataValue& node, const IObject& origin) const override;

        // Completion deserialization delta callback
        void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;

        REF_COUNTERABLE_IMPL(DrawableComponent, ParticlesEmitter);
    };
}
// --- META ---

CLASS_BASES_META(o2::ParticlesEmitterComponent)
{
    BASE_CLASS(o2::DrawableComponent);
    BASE_CLASS(o2::ParticlesEmitter);
}
END_META;
CLASS_FIELDS_META(o2::ParticlesEmitterComponent)
{
}
END_META;
CLASS_METHODS_META(o2::ParticlesEmitterComponent)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const ParticlesEmitterComponent&);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(void, OnUpdate, float);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetName);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCategory);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetIcon);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Ref<RefCounterable>, CastToRefCounterable, const Ref<ParticlesEmitterComponent>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSerializeDelta, DataValue&, const IObject&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserializedDelta, const DataValue&, const IObject&);
}
END_META;
// --- END META ---
