#include "o2/stdafx.h"
#include "ActorRef.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"

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

	ActorRef::ActorRef(const ActorRef& other):
		ActorRef(other.mActor)
	{
		ActorRefResolver::RequireRemap(*this);
	}

	ActorRef::~ActorRef()
	{
		if (mActor)
			mActor->mReferences.Remove(this);

		if (ActorRefResolver::IsLocked())
			ActorRefResolver::Instance().mRemapActorRefs.Remove(this);
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

		UpdateSpecActor();

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

	void ActorRef::Destroy()
	{
		if (mActor)
			o2Scene.DestroyActor(mActor);
	}

	bool ActorRef::IsValid() const
	{
		return mActor != nullptr;
	}

	bool ActorRef::IsWasDeleted() const
	{
		return mWasDeleted;
	}

	const Type& ActorRef::GetActorType() const
	{
		return TypeOf(Actor);
	}

	const Type* ActorRef::GetActorTypeStatic()
	{
		return &TypeOf(Actor);
	}

	void ActorRef::OnSerialize(DataValue& node) const
	{
		if (mActor)
		{
			if (mActor->mIsAsset)
				node.AddMember("AssetId") = mActor->GetAssetID();
			else 
				node.AddMember("ID") = mActor->GetID();
		}
	}

	void ActorRef::OnDeserialized(const DataValue& node)
	{
		if (auto assetIdNode = node.FindMember("AssetId"))
			ActorRefResolver::RequireResolve(*this, (UID)*assetIdNode);
		else if (auto sceneIdNode = node.FindMember("ID"))
			ActorRefResolver::RequireResolve(*this, (SceneUID)*sceneIdNode);
		else *this = nullptr;
	}

}

DECLARE_CLASS_MANUAL(o2::Ref<o2::Actor>);

DECLARE_CLASS(o2::ActorRef);
