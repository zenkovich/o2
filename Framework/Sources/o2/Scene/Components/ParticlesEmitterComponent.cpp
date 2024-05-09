#include "o2/stdafx.h"
#include "ParticlesEmitterComponent.h"

#include "o2/Scene/Actor.h"

namespace o2
{

    ParticlesEmitterComponent::ParticlesEmitterComponent()
    {}

    ParticlesEmitterComponent::ParticlesEmitterComponent(const ParticlesEmitterComponent& other):
        DrawableComponent(other), ParticlesEmitter(other)
    {}

    ParticlesEmitterComponent::~ParticlesEmitterComponent()
    {}

    ParticlesEmitterComponent& ParticlesEmitterComponent::operator=(const ParticlesEmitterComponent& other)
    {
        DrawableComponent::operator=(other);
        ParticlesEmitter::operator=(other);
        return *this;
    }

    void ParticlesEmitterComponent::Draw()
    {
        ParticlesEmitter::Draw();
    }

    void ParticlesEmitterComponent::OnUpdate(float dt)
    {
        ParticlesEmitter::Update(dt);
    }

    bool ParticlesEmitterComponent::IsUnderPoint(const Vec2F& point)
    {
        return ParticlesEmitter::IsUnderPoint(point);
    }

    String ParticlesEmitterComponent::GetName()
    {
        return "Particles emitter";
    }

    String ParticlesEmitterComponent::GetCategory()
    {
        return "Render";
    }

    String ParticlesEmitterComponent::GetIcon()
    {
        return "ui/UI4_emitter_component.png";
    }

	Ref<o2::RefCounterable> ParticlesEmitterComponent::CastToRefCounterable(const Ref<ParticlesEmitterComponent>& ref)
	{
        return DynamicCast<DrawableComponent>(ref);
	}

	void ParticlesEmitterComponent::OnTransformUpdated()
    {
        basis = mOwner.Lock()->transform->GetWorldBasis();
    }

    void ParticlesEmitterComponent::OnSerialize(DataValue& node) const
    {
        DrawableComponent::OnSerialize(node);
        ParticlesEmitter::OnSerialize(node);
    }

    void ParticlesEmitterComponent::OnDeserialized(const DataValue& node)
    {
        DrawableComponent::OnDeserialized(node);
        ParticlesEmitter::OnDeserialized(node);
    }

    void ParticlesEmitterComponent::OnSerializeDelta(DataValue& node, const IObject& origin) const
    {
        DrawableComponent::OnSerializeDelta(node, origin);
        ParticlesEmitter::OnSerializeDelta(node, origin);
    }

    void ParticlesEmitterComponent::OnDeserializedDelta(const DataValue& node, const IObject& origin)
    {
        DrawableComponent::OnDeserializedDelta(node, origin);
        ParticlesEmitter::OnDeserializedDelta(node, origin);
    }
}

DECLARE_TEMPLATE_CLASS(o2::LinkRef<o2::ParticlesEmitterComponent>);
// --- META ---

DECLARE_CLASS(o2::ParticlesEmitterComponent, o2__ParticlesEmitterComponent);
// --- END META ---
