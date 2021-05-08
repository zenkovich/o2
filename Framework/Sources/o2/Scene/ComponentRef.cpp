#include "o2/stdafx.h"
#include "ComponentRef.h"

namespace o2
{
	ComponentRef::ComponentRef()
	{}

	ComponentRef::ComponentRef(Component* component):
		mComponent(component)
	{
		if (mComponent)
			mComponent->mReferences.Add(this);
	}

	ComponentRef::ComponentRef(const ComponentRef& other):
		ComponentRef(other.mComponent)
	{
		ActorRefResolver::RequireRemap(*this);
	}

	ComponentRef::~ComponentRef()
	{
		if (mComponent)
			mComponent->mReferences.Remove(this);
	}

	bool ComponentRef::operator!=(const ComponentRef& other) const
	{
		return mComponent != other.mComponent;
	}

	bool ComponentRef::operator==(const ComponentRef& other) const
	{
		return mComponent == other.mComponent;
	}

	ComponentRef& ComponentRef::operator=(const ComponentRef& other)
	{
		CopyWithoutRemap(other);
		ActorRefResolver::RequireRemap(*this);

		return *this;
	}

	ComponentRef::operator bool() const
	{
		return mComponent != nullptr;
	}

	Component& ComponentRef::operator*()
	{
		return *mComponent;
	}

	const Component& ComponentRef::operator*() const
	{
		return *mComponent;
	}

	Component* ComponentRef::operator->()
	{
		return mComponent;
	}

	const Component* ComponentRef::operator->() const
	{
		return mComponent;
	}

	Component* ComponentRef::Get()
	{
		return mComponent;
	}

	const Component* ComponentRef::Get() const
	{
		return mComponent;
	}

	void ComponentRef::Destroy()
	{
		if (mComponent)
			o2Scene.DestroyComponent(mComponent);

		*this = nullptr;
	}

	bool ComponentRef::IsValid() const
	{
		return mComponent != nullptr;
	}

	bool ComponentRef::IsWasDeleted() const
	{
		return mWasDeleted;
	}

	const Type& ComponentRef::GetComponentType() const
	{
		return TypeOf(Component);
	}

	const Type* ComponentRef::GetComponentTypeStatic()
	{
		return &TypeOf(Component);
	}

	bool ComponentRef::EqualsDelta(const ComponentRef& obj, const ComponentRef& origin)
	{
		if (obj.mComponent == origin.mComponent)
			return true;

		if (obj.mComponent && obj.mComponent->mPrototypeLink == origin.mComponent)
			return true;

		return false;
	}

	void ComponentRef::CopyWithoutRemap(const ComponentRef& other)
	{
		if (mComponent)
			mComponent->mReferences.Remove(this);

		mComponent = other.mComponent;
		mWasDeleted = other.mWasDeleted;

		UpdateSpecComponent();

		if (mComponent)
			mComponent->mReferences.Add(this);
	}

	void ComponentRef::OnSerialize(DataValue& node) const
	{
		if (mComponent)
		{
			auto actor = mComponent->mOwner;
			if (actor->mIsAsset)
				node.AddMember("AssetID") = actor->GetAssetID();
			else 
				node.AddMember("ActorID") = actor->GetID();

			node.AddMember("ComponentId") = mComponent->mId;
		}
	}

	void ComponentRef::OnDeserialized(const DataValue& node)
	{
		if (auto assetIdNode = node.FindMember("AssetId"))
			ActorRefResolver::RequireResolve(*this, (UID)*assetIdNode, (SceneUID)node.GetMember("ComponentId"));
		else if (auto sceneIdNode = node.FindMember("ActorID"))
			ActorRefResolver::RequireResolve(*this, (SceneUID)*sceneIdNode, (SceneUID)node.GetMember("ComponentId"));
		else *this = nullptr;
	}

}

DECLARE_CLASS(o2::ComponentRef);
