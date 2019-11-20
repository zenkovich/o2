#include "stdafx.h"
#include "ParticlesEmitterComponent.h"

#include "Scene/Actor.h"

namespace o2
{

	ParticlesEmitterComponent::ParticlesEmitterComponent()
	{}

	ParticlesEmitterComponent::ParticlesEmitterComponent(const ParticlesEmitterComponent& other):
		DrawableComponent(other), mEmitter(other.mEmitter)
	{}

	ParticlesEmitterComponent::~ParticlesEmitterComponent()
	{}

	ParticlesEmitterComponent& ParticlesEmitterComponent::operator=(const ParticlesEmitterComponent& other)
	{
		DrawableComponent::operator=(other);
		mEmitter = other.mEmitter;
		return *this;
	}

	void ParticlesEmitterComponent::Draw()
	{
		mEmitter.Draw();
	}

	void ParticlesEmitterComponent::Update(float dt)
	{
		mEmitter.Update(dt);
	}

	String ParticlesEmitterComponent::GetName() const
	{
		return "Emitter";
	}

	String ParticlesEmitterComponent::GetCategory() const
	{
		return "Render/Particles";
	}

	String ParticlesEmitterComponent::GetIcon() const
	{
		return "ui/UI4_emitter_component.png";
	}

	void ParticlesEmitterComponent::OnTransformUpdated()
	{
		mEmitter.basis = mOwner->transform->GetWorldBasis();
	}

}

DECLARE_CLASS(o2::ParticlesEmitterComponent);
