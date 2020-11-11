#include "o2/stdafx.h"
#include "ComponentRef.h"

namespace o2
{
	Ref<Component>::Ref()
	{}

	Ref<Component>::Ref(Component* actor) :
		mComponent(actor)
	{
		if (mComponent)
			mComponent->mReferences.Add(this);
	}

	Ref<Component>::Ref(const Component& other)
	{
		Component* newComponent = mnew Component(other);
		mComponent = newComponent;
		mComponent->mReferences.Add(this);
	}

	Ref<Component>::~Ref()
	{
		if (mComponent)
			mComponent->mReferences.Remove(this);
	}

	bool Ref<Component>::operator!=(const Ref<Component>& other) const
	{
		return mComponent != other.mComponent;
	}

	bool Ref<Component>::operator==(const Ref<Component>& other) const
	{
		return mComponent == other.mComponent;
	}

	Ref<Component>& Ref<Component>::operator=(const Ref<Component>& other)
	{
		if (mComponent)
			mComponent->mReferences.Remove(this);

		mComponent = other.mComponent;
		mWasDeleted = other.mWasDeleted;

		if (mComponent)
			mComponent->mReferences.Add(this);

		return *this;
	}

	Ref<Component>::operator bool() const
	{
		return mComponent != nullptr;
	}

	Component& Ref<Component>::operator*()
	{
		return *mComponent;
	}

	const Component& Ref<Component>::operator*() const
	{
		return *mComponent;
	}

	Component* Ref<Component>::operator->()
	{
		return mComponent;
	}

	const Component* Ref<Component>::operator->() const
	{
		return mComponent;
	}

	Component* Ref<Component>::Get()
	{
		return mComponent;
	}

	const Component* Ref<Component>::Get() const
	{
		return mComponent;
	}

	void Ref<Component>::Destroy()
	{
		if (mComponent)
			delete mComponent;
	}

	bool Ref<Component>::IsValid() const
	{
		return mComponent != nullptr;
	}

	bool Ref<Component>::IsWasDeleted() const
	{
		return mWasDeleted;
	}
}

DECLARE_CLASS(o2::Ref<o2::Component>);
