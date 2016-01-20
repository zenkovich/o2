#include "Component.h"

#include "Scene/Actor.h"

namespace o2
{
	Component::Component():
		mEnabled(true), mResEnabled(true), mOwner(nullptr)
	{
		InitializeProperties();
	}

	Component::Component(const Component& other):
		mEnabled(other.mEnabled), mResEnabled(other.mEnabled), mOwner(nullptr)
	{
		InitializeProperties();
	}

	Component::~Component()
	{
		if (mOwner)
			mOwner->RemoveComponent(this, false);
	}

	Component& Component::operator=(const Component& other)
	{
		mEnabled = other.mEnabled;
		UpdateEnabled();
		return *this;
	}

	void Component::Update(float dt)
	{}

	void Component::SetEnabled(bool active)
	{
		if (mEnabled == active)
			return;

		mEnabled = active;
		UpdateEnabled();
	}

	void Component::Enable()
	{
		SetEnabled(true);
	}

	void Component::Disable()
	{
		SetEnabled(false);
	}

	bool Component::IsEnabled() const
	{
		return mEnabled;
	}

	bool Component::IsEnabledInHierarchy() const
	{
		return mResEnabled;
	}

	Actor* Component::GetOwnerActor() const
	{
		return mOwner;
	}

	void Component::UpdateEnabled()
	{
		if (mOwner)
			mResEnabled = mEnabled && mOwner->mResEnabled;
		else
			mResEnabled = mEnabled;
	}

	void Component::SetOwnerActor(Actor* actor)
	{
		if (mOwner)
			mOwner->mCompontents.Remove(this);

		mOwner = actor;

		if (mOwner)
		{
			mOwner->mCompontents.Add(this);
			OnTransformChanged();
		}
	}

	void Component::InitializeProperties()
	{
		INITIALIZE_GETTER(Component, actor, GetOwnerActor);
		INITIALIZE_PROPERTY(Component, enabled, SetEnabled, IsEnabled);
		INITIALIZE_GETTER(Component, enabledInHierarchy, IsEnabledInHierarchy);
	}

}