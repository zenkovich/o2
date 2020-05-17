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

	void ParticlesEmitterComponent::Update(float dt)
	{
		ParticlesEmitter::Update(dt);
	}

	String ParticlesEmitterComponent::GetName() const
	{
		return "Emitter";
	}

	String ParticlesEmitterComponent::GetCategory() const
	{
		return "o2/Render/Particles";
	}

	String ParticlesEmitterComponent::GetIcon() const
	{
		return "ui/UI4_emitter_component.png";
	}

	void ParticlesEmitterComponent::OnTransformUpdated()
	{
		basis = mOwner->transform->GetWorldBasis();
	}

	void ParticlesEmitterComponent::OnDeserialized(const DataValue& node)
	{
		DrawableComponent::OnDeserialized(node);
		ParticlesEmitter::OnDeserialized(node);
	}

}

DECLARE_CLASS(o2::ParticlesEmitterComponent);
