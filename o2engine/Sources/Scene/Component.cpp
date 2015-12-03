#include "Component.h"

#include "Scene/Actor.h"

namespace o2
{
	IOBJECT_CPP(Component);


	Component::Component():
		mEnabled(true), mResEnabled(true)
	{
		InitializeProperties();
	}

	Component::Component(const Component& other):
		mEnabled(other.mEnabled), mResEnabled(other.mEnabled)
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

	void Component::Draw()
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

	Ptr<Actor> Component::GetOwnerActor() const
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

	void Component::InitializeProperties()
	{
		INITIALIZE_GETTER(Component, actor, GetOwnerActor);
		INITIALIZE_PROPERTY(Component, enabled, SetEnabled, IsEnabled);
		INITIALIZE_GETTER(Component, enabledInHierarchy, IsEnabledInHierarchy);
	}

}