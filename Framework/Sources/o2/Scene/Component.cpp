#include "o2/stdafx.h"
#include "Component.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"

namespace o2
{
	Component::Component() :
		mId(Math::Random())
	{}

	Component::Component(const Component& other) :
		mEnabled(other.mEnabled), mResEnabled(other.mEnabled), mId(Math::Random()),
		actor(this), enabled(this), enabledInHierarchy(this)
	{}

	Component::~Component()
	{
		if (mOwner)
			mOwner->RemoveComponent(this, false);
	}

	Component& Component::operator=(const Component& other)
	{
		mEnabled = other.mEnabled;
		UpdateEnabled();

		if (mOwner)
			mOwner->OnChanged();

		return *this;
	}

	UInt64 Component::GetID() const
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
		return mResEnabled;
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

	String Component::GetName() const
	{
		return String();
	}

	String Component::GetCategory() const
	{
		return "";
	}

	String Component::GetIcon() const
	{
		return "ui/UI4_component_icon.png";
	}

	void Component::UpdateEnabled()
	{
		bool lastResEnabled = mResEnabled;

		if (mOwner)
			mResEnabled = mEnabled && mOwner->mResEnabledInHierarchy;
		else
			mResEnabled = mEnabled;

		if (lastResEnabled != mResEnabled && mOwner)
			mOwner->OnChanged();
	}

	void Component::SetOwnerActor(Actor* actor)
	{
		if (mOwner)
		{
			mOwner->OnChanged();
			mOwner->mComponents.Remove(this);
		}

		mOwner = actor;

		if (mOwner)
		{
			mOwner->mComponents.Add(this);
			OnTransformUpdated();
			mOwner->OnChanged();
		}
	}

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
