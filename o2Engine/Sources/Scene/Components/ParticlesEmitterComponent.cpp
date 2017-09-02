#include "ParticlesEmitterComponent.h"

#include "Scene/Actor.h"
#include "Utils/EditorPropertyAttribute.h"

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
		return "Particles";
	}

	void ParticlesEmitterComponent::OnTransformChanged()
	{
		mEmitter.basis = mOwner->transform->GetWorldBasis();
	}

}

CLASS_META(o2::ParticlesEmitterComponent)
{
	BASE_CLASS(o2::DrawableComponent);

	PROTECTED_FIELD(mEmitter).EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(String, GetName);
	PROTECTED_FUNCTION(void, OnTransformChanged);
}
END_META;
