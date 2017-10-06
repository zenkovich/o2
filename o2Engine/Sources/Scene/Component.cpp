#include "Component.h"

#include "Scene/Actor.h"

namespace o2
{
	Component::Component():
		mId(Math::Random())
	{
		InitializeProperties();
	}

	Component::Component(const Component& other):
		mEnabled(other.mEnabled), mResEnabled(other.mEnabled), mId(Math::Random())
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

	void Component::UpdateEnabled()
	{
		bool lastResEnabled = mResEnabled;

		if (mOwner)
			mResEnabled = mEnabled && mOwner->mResEnabled;
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

	void Component::InitializeProperties()
	{
		INITIALIZE_GETTER(Component, actor, GetOwnerActor);
		INITIALIZE_PROPERTY(Component, enabled, SetEnabled, IsEnabled);
		INITIALIZE_GETTER(Component, enabledInHierarchy, IsEnabledInHierarchy);
	}

	void ComponentDataNodeConverter::ToData(void* object, DataNode& data)
	{
		Component* value = *(Component**)object;

		if (value)
		{
			if (auto ownerActor = value->GetOwnerActor())
			{
				if (ownerActor->IsAsset())
				{
					*data.AddNode("AssetId") = ownerActor->GetAssetID();
					*data.AddNode("ComponentId") = value->GetID();
				}
				else if (ownerActor->IsOnScene())
				{
					*data.AddNode("SceneId") = ownerActor->GetID();
					*data.AddNode("ComponentId") = value->GetID();
				}
				else
				{
					*data.AddNode("Data") = value->Serialize();
					*data.AddNode("Type") = value->GetType().GetName();
				}
			}
			else
			{
				*data.AddNode("Data") = value->Serialize();
				*data.AddNode("Type") = value->GetType().GetName();
			}
		}
	}

	void ComponentDataNodeConverter::FromData(void* object, const DataNode& data)
	{
		Component*& component = *(Component**)object;

		if (auto assetIdNode = data.GetNode("AssetId"))
		{
			UID assetId = *assetIdNode;
			auto actor = o2Scene.GetAssetActorByID(assetId);

			UInt64 componentId = *data.GetNode("ComponentId");
			component = actor->GetComponent(componentId);
		}
		else if (auto sceneIdNode = data.GetNode("SceneId"))
		{
			auto actor = o2Scene.GetActorByID(*sceneIdNode);

			UInt64 componentId = *data.GetNode("ComponentId");
			component = actor->GetComponent(componentId);
		}
		else if (auto dataNode = data.GetNode("Data"))
		{
			String type = *data.GetNode("Type");
			component = (Component*)o2Reflection.CreateTypeSample(type);
			component->Deserialize(*dataNode);
		}
		else component = nullptr;
	}

	bool ComponentDataNodeConverter::IsConvertsType(const Type* type) const
	{
		return type->IsBasedOn(TypeOf(Component));
	}
}

CLASS_META(o2::Component)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(actor);
	PUBLIC_FIELD(enabled);
	PUBLIC_FIELD(enabledInHierarchy);
	PROTECTED_FIELD(mPrototypeLink);
	PROTECTED_FIELD(mId).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mOwner);
	PROTECTED_FIELD(mEnabled).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mResEnabled);

	PUBLIC_FUNCTION(UInt64, GetID);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(void, Enable);
	PUBLIC_FUNCTION(void, Disable);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(bool, IsEnabledInHierarchy);
	PUBLIC_FUNCTION(Component*, GetPrototypeLink);
	PUBLIC_FUNCTION(bool, IsLinkedToComponent, Component*);
	PUBLIC_FUNCTION(Actor*, GetOwnerActor);
	PUBLIC_FUNCTION(String, GetName);
	PROTECTED_FUNCTION(void, OnLayerChanged, SceneLayer*, SceneLayer*);
	PROTECTED_FUNCTION(void, UpdateEnabled);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, SetOwnerActor, Actor*);
	PROTECTED_FUNCTION(void, OnExcludeFromScene);
	PROTECTED_FUNCTION(void, OnIncludeToScene);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
