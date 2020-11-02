#include "o2/stdafx.h"
#include "ActorRef.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"

namespace o2
{
	Ref<Actor>::Ref()
	{}

	Ref<Actor>::Ref(Actor* actor) :
		mActor(actor)
	{
		if (mActor)
			mActor->mReferences.Add(this);
	}

	Ref<Actor>::Ref(const ActorAssetRef& prototype, ActorCreateMode mode /*= Actor::CreateMode::InScene*/)
	{
		Actor* newActor = mnew Actor(prototype, mode);
		mActor = newActor;
		mActor->mReferences.Add(this);
	}

	Ref<Actor>::Ref(Vector<Component*> components, ActorCreateMode mode /*= ActorCreateMode::InScene*/)
	{
		Actor* newActor = mnew Actor(components, mode);
		mActor = newActor;
		mActor->mReferences.Add(this);
	}

	Ref<Actor>::Ref(const Actor& other)
	{
		Actor* newActor = mnew Actor(other);
		mActor = newActor;
		mActor->mReferences.Add(this);
	}

	Ref<Actor>::~Ref()
	{
		if (mActor)
			mActor->mReferences.Remove(this);
	}

	bool Ref<Actor>::operator!=(const Ref<Actor>& other) const
	{
		return mActor != other.mActor;
	}

	bool Ref<Actor>::operator==(const Ref<Actor>& other) const
	{
		return mActor == other.mActor;
	}

	Ref<Actor>& Ref<Actor>::operator=(const Ref<Actor>& other)
	{
		if (mActor)
			mActor->mReferences.Remove(this);

		mActor = other.mActor;
		mWasDeleted = other.mWasDeleted;

		if (mActor)
			mActor->mReferences.Add(this);

		return *this;
	}

	Ref<Actor>::operator bool() const
	{
		return mActor != nullptr;
	}

	Actor& Ref<Actor>::operator*()
	{
		return *mActor;
	}

	const Actor& Ref<Actor>::operator*() const
	{
		return *mActor;
	}

	Actor* Ref<Actor>::operator->()
	{
		return mActor;
	}

	const Actor* Ref<Actor>::operator->() const
	{
		return mActor;
	}

	Actor* Ref<Actor>::Get()
	{
		return mActor;
	}

	const Actor* Ref<Actor>::Get() const
	{
		return mActor;
	}

	void Ref<Actor>::Destroy()
	{
		if (mActor)
			o2Scene.DestroyActor(mActor);
	}

	bool Ref<Actor>::IsValid() const
	{
		return mActor != nullptr;
	}

	bool Ref<Actor>::IsWasDeleted() const
	{
		return mWasDeleted;
	}
}

DECLARE_CLASS(o2::Ref<o2::Actor>);
