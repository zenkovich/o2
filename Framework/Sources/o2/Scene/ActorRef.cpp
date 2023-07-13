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

	ActorRef::ActorRef(const ActorRef& other) :
		ActorRef(other.mActor)
	{
		if (other.mRequiredResolveData)
		{
			mRequiredResolveData = other.mRequiredResolveData->Clone();
			mRequiredResolveData->RequireResolve(*this);
		}

		ActorRefResolver::RequireRemap(*this);
	}

	ActorRef::~ActorRef()
	{
		if (mActor)
			mActor->mReferences.Remove(this);

		if (mRequiredResolveData)
			delete mRequiredResolveData;

		ActorRefResolver::OnActorRefDestroyed(this);
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
		CopyWithoutRemap(other);
		ActorRefResolver::RequireRemap(*this);

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

		*this = nullptr;
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

	bool ActorRef::EqualsDelta(const ActorRef& obj, const ActorRef& origin)
	{
		if (obj.mActor == origin.mActor)
			return true;

		if (obj.mActor && obj.mActor->mPrototypeLink == origin.mActor)
			return true;

		return false;
	}

	void ActorRef::CopyWithoutRemap(const ActorRef& other)
	{
		if (mActor)
			mActor->mReferences.Remove(this);

		mActor = other.mActor;
		mWasDeleted = other.mWasDeleted;

		if (mRequiredResolveData)
		{
			delete mRequiredResolveData;
			mRequiredResolveData = nullptr;
		}

		UpdateSpecActor();

		if (mActor)
			mActor->mReferences.Add(this);
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
		{
			auto resolveData = mnew AssetRequireResolveData();
			resolveData->uid = (UID)*assetIdNode;
			resolveData->RequireResolve(*this);
			mRequiredResolveData = resolveData;
		}
		else if (auto sceneIdNode = node.FindMember("ID"))
		{
			auto resolveData = mnew SceneRequireResolveData();
			resolveData->uid = (SceneUID)*sceneIdNode;
			resolveData->RequireResolve(*this);
			mRequiredResolveData = resolveData;
		}
		else
			*this = nullptr;
	}

	void ActorRef::SceneRequireResolveData::RequireResolve(ActorRef& ref)
	{
		ActorRefResolver::RequireResolve(ref, uid);
	}

	ActorRef::IRequiredResolveData* ActorRef::SceneRequireResolveData::Clone() const
	{
		return mnew SceneRequireResolveData(*this);
	}

	void ActorRef::AssetRequireResolveData::RequireResolve(ActorRef& ref)
	{
		ActorRefResolver::RequireResolve(ref, uid);
	}

	ActorRef::IRequiredResolveData* ActorRef::AssetRequireResolveData::Clone() const
	{
		return mnew AssetRequireResolveData(*this);
	}
}

DECLARE_TEMPLATE_CLASS(o2::Ref<o2::Actor>);
// --- META ---

DECLARE_CLASS(o2::ActorRef);
// --- END META ---
