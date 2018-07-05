#include "stdafx.h"
#include "ActorRef.h"

#include "Scene/Actor.h"

namespace o2
{
	ActorRef::ActorRef()
	{}

	ActorRef::ActorRef(Actor* actor) :
		mActor(actor)
	{
		if (mActor)
			mActor->mReferences.Add(this);
	}

	ActorRef::ActorRef(const ActorAssetRef& prototype, ActorCreateMode mode /*= Actor::CreateMode::InScene*/)
	{
		Actor* newActor = mnew Actor(prototype, mode);
		mActor = newActor;
		mActor->mReferences.Add(this);
	}

	ActorRef::ActorRef(Vector<Component*> components, ActorCreateMode mode /*= ActorCreateMode::InScene*/)
	{
		Actor* newActor = mnew Actor(components, mode);
		mActor = newActor;
		mActor->mReferences.Add(this);
	}

	ActorRef::ActorRef(const Actor& other)
	{
		Actor* newActor = mnew Actor(other);
		mActor = newActor;
		mActor->mReferences.Add(this);
	}

	ActorRef::~ActorRef()
	{
		if (mActor)
			mActor->mReferences.Remove(this);
	}

	bool ActorRef::operator!=(const ActorRef& other) const
	{
		return mActor != other.mActor;
	}

	bool ActorRef::operator==(const ActorRef& other) const
	{
		return mActor == other.mActor;
	}

	ActorRef& ActorRef::operator=(const ActorRef& other)
	{
		if (mActor)
			mActor->mReferences.Remove(this);

		mActor = other.mActor;
		mWasDeleted = other.mWasDeleted;

		if (mActor)
			mActor->mReferences.Add(this);

		return *this;
	}

	ActorRef::operator bool() const
	{
		return mActor != nullptr;
	}

	Actor& ActorRef::operator*()
	{
		return *mActor;
	}

	const Actor& ActorRef::operator*() const
	{
		return *mActor;
	}

	Actor* ActorRef::operator->()
	{
		return mActor;
	}

	const Actor* ActorRef::operator->() const
	{
		return mActor;
	}

	Actor* ActorRef::Get()
	{
		return mActor;
	}

	const Actor* ActorRef::Get() const
	{
		return mActor;
	}

	bool ActorRef::IsValid() const
	{
		return mActor != nullptr;
	}

	bool ActorRef::IsWasDeleted() const
	{
		return mWasDeleted;
	}
}

DECLARE_CLASS(o2::ActorRef);
