#include "o2/stdafx.h"
#include "Component.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/ActorRefResolver.h"
#include "o2/Scene/Scene.h"

namespace o2
{
	Component::Component() :
		mId(Math::Random())
	{
		ActorRefResolver::ComponentCreated(this);
	}

	Component::Component(const Component& other) :
		mEnabled(other.mEnabled), mEnabledInHierarchy(other.mEnabled), mId(Math::Random()),
		actor(this), enabled(this), enabledInHierarchy(this)
	{
		ActorRefResolver::ComponentCreated(this);
	}

	Component::~Component()
	{
		if (mOwner)
			mOwner->RemoveComponent(this, false);

		for (auto ref : mReferences)
		{
			ref->mComponent = nullptr;
			ref->mWasDeleted = true;
		}
	}

	Component& Component::operator=(const Component& other)
	{
		mEnabled = other.mEnabled;
		UpdateEnabled();

		if (mOwner)
			mOwner->OnChanged();

		return *this;
	}

	SceneUID Component::GetID() const
	{
		return mId;
	}

	void Component::Update(float dt)
	{}

	void Component::SetEnabled(bool active)
	{
		if (mEnabled == active)
			return;

		mEnabled = active;
		UpdateEnabled();

		if (mOwner)
			mOwner->OnChanged();
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
		return mEnabledInHierarchy;
	}

	Component* Component::GetPrototypeLink() const
	{
		return mPrototypeLink;
	}

	bool Component::IsLinkedToComponent(Component* component) const
	{
		if (mPrototypeLink)
		{
			auto t = mPrototypeLink;
			while (t)
			{
				if (t == component)
					return true;

				t = t->mPrototypeLink;
			}
		}

		return false;
	}

	Actor* Component::GetOwnerActor() const
	{
		return mOwner;
	}

	String Component::GetName()
	{
		return String();
	}

	String Component::GetCategory()
	{
		return "";
	}

	String Component::GetIcon()
	{
		return "ui/UI4_component_icon.png";
	}

	bool Component::IsAvailableFromCreateMenu()
	{
		return true;
	}

	void Component::UpdateEnabled()
	{
		bool lastEnabledInHierarchy = mEnabledInHierarchy;

		if (mOwner)
			mEnabledInHierarchy = mEnabled && mOwner->mResEnabledInHierarchy;
		else
			mEnabledInHierarchy = mEnabled;

		if (lastEnabledInHierarchy != mEnabledInHierarchy)
		{
			if (mEnabledInHierarchy)
				OnEnabled();
			else
				OnDisabled();

			if (mOwner)
				mOwner->OnChanged();
		}
	}

	void Component::OnSerialize(DataValue& node) const
	{
		node.AddMember("mId") = mId;
	}

	void Component::OnDeserialized(const DataValue& node)
	{
		auto prevId = mId;
		mId = node.GetMember("mId");
		ActorRefResolver::OnComponentIdChanged(this, prevId);
	}

	void Component::OnSerializeDelta(DataValue& node, const IObject& origin) const
	{
		OnSerialize(node);
	}

	void Component::OnDeserializedDelta(const DataValue& node, const IObject& origin)
	{
		OnDeserialized(node);
	}

	void Component::SetOwnerActor(Actor* actor)
	{
		if (mOwner == actor)
			return;

		if (mOwner)
			mOwner->RemoveComponent(this, false);

		mOwner = actor;

		if (mOwner)
			OnTransformUpdated();

		OnParentChanged(nullptr);
	}

	void Component::OnAddToScene()
	{
		o2Scene.OnComponentAdded(this);
	}

	void Component::OnRemoveFromScene()
	{
		o2Scene.OnComponentRemoved(this);
	}

	void Component::FixedUpdate(float dt)
	{}

// 	void ComponentDataValueConverter::ToData(void* object, DataValue& data)
// 	{
// 		Component* value = *(Component**)object;
// 
// 		if (value)
// 		{
// 			if (auto ownerActor = value->GetOwnerActor())
// 			{
// 				if (ownerActor->IsAsset())
// 				{
// 					*data.AddMember("AssetId") = ownerActor->GetAssetID();
// 					*data.AddMember("ComponentId") = value->GetID();
// 				}
// 				else if (ownerActor->IsOnScene())
// 				{
// 					*data.AddMember("SceneId") = ownerActor->GetID();
// 					*data.AddMember("ComponentId") = value->GetID();
// 				}
// 				else
// 				{
// 					*data.AddMember("Data") = value->Serialize();
// 					*data.AddMember("Type") = value->GetType().GetName();
// 				}
// 			}
// 			else
// 			{
// 				*data.AddMember("Data") = value->Serialize();
// 				*data.AddMember("Type") = value->GetType().GetName();
// 			}
// 		}
// 	}
// 
// 	void ComponentDataValueConverter::FromData(void* object, const DataValue& data)
// 	{
// 		Component*& component = *(Component**)object;
// 
// 		if (auto assetIdNode = data.GetNode("AssetId"))
// 		{
// 			UID assetId = *assetIdNode;
// 			auto actor = o2Scene.GetAssetActorByID(assetId);
// 
// 			UInt64 componentId = *data.GetNode("ComponentId");
// 			component = actor->GetComponent(componentId);
// 		}
// 		else if (auto sceneIdNode = data.GetNode("SceneId"))
// 		{
// 			auto actor = o2Scene.GetActorByID(*sceneIdNode);
// 
// 			UInt64 componentId = *data.GetNode("ComponentId");
// 			component = actor->GetComponent(componentId);
// 		}
// 		else if (auto DataValue = data.GetNode("Data"))
// 		{
// 			String type = *data.GetNode("Type");
// 			component = (Component*)o2Reflection.CreateTypeSample(type);
// 			component->Deserialize(*DataValue);
// 		}
// 		else component = nullptr;
// 	}
// 
// 	bool ComponentDataValueConverter::IsConvertsType(const Type* type) const
// 	{
// 		return type->IsBasedOn(TypeOf(Component));
// 	}
}

DECLARE_CLASS(o2::Component);
