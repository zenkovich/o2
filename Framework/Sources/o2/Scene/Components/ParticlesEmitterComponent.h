#pragma once

#include "o2/Render/Particles/ParticlesEmitter.h"
#include "o2/Scene/Component.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"

namespace o2
{
	// ---------------------------
	// Particles emitter component
	// ---------------------------
    class ParticlesEmitterComponent: public Component, public ParticlesEmitter
    {
    public:
		// Default constructor
        ParticlesEmitterComponent();

		// Copy constructor
        ParticlesEmitterComponent(const ParticlesEmitterComponent& other);

		// Destructor
        ~ParticlesEmitterComponent();

		// Assign operator
        ParticlesEmitterComponent& operator=(const ParticlesEmitterComponent& other);

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

		// Dynamic cast to RefCounterable via Component
		static Ref<RefCounterable> CastToRefCounterable(const Ref<ParticlesEmitterComponent>& ref);

        SERIALIZABLE(ParticlesEmitterComponent);
		CLONEABLE_REF(ParticlesEmitterComponent);
		REF_COUNTERABLE_IMPL(Component, ParticlesEmitter);

	protected:
		// Draw particle system
		void OnDraw() override;

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
    };
}
// --- META ---

CLASS_BASES_META(o2::ParticlesEmitterComponent)
{
    BASE_CLASS(o2::Component);
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
    FUNCTION().PUBLIC().SIGNATURE(void, OnUpdate, float);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetName);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCategory);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetIcon);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Ref<RefCounterable>, CastToRefCounterable, const Ref<ParticlesEmitterComponent>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDraw);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSerializeDelta, DataValue&, const IObject&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserializedDelta, const DataValue&, const IObject&);
}
END_META;
// --- END META ---
