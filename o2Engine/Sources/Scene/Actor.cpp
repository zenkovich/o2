#include "stdafx.h"
#include "Actor.h"

#include "Scene/Component.h"
#include "Scene/SceneLayer.h"

namespace o2
{

	ActorCreateMode Actor::mDefaultCreationMode = ActorCreateMode::InScene;

	Actor::Actor(ActorTransform* transform, bool isOnScene, const String& name, bool enabled, bool resEnabled,
				 bool locked, bool resLocked, SceneLayer* layer, UInt64 id, UID assetId):
		transform(transform), mName(name), mEnabled(enabled), mResEnabled(resEnabled),
		mLocked(locked), mResLocked(resLocked), mLayer(layer), mId(id), mAssetId(assetId), mIsOnScene(isOnScene)
	{}

	Actor::Actor(ActorTransform* transform, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(transform, mode == ActorCreateMode::InScene || mode == ActorCreateMode::Default && mDefaultCreationMode == ActorCreateMode::InScene)
	{
		mIsOnScene = false;

		tags.onTagAdded = [&](Tag* tag) { tag->mActors.Add(this); };
		tags.onTagRemoved = [&](Tag* tag) { tag->mActors.Remove(this); };

		transform->SetOwner(this);
		InitializeProperties();

		if (Scene::IsSingletonInitialzed())
		{
			mLayer = o2Scene.GetDefaultLayer();

			if (mode == ActorCreateMode::InScene ||
				(mode == ActorCreateMode::Default && mDefaultCreationMode == ActorCreateMode::InScene))
			{
				o2Scene.mRootActors.Add(this);
				o2Scene.mAllActors.Add(this);
				mLayer->mEnabledActors.Add(this);

				mIsOnScene = true;
			}

			mLayer->mActors.Add(this);

			o2Scene.onActorCreated(this);
		}

		ActorDataNodeConverter::ActorCreated(this);
	}

	Actor::Actor(ActorTransform* transform, const ActorAssetRef& prototype, ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(transform, mode == ActorCreateMode::InScene, prototype->GetActor()->mName,
			  prototype->GetActor()->mEnabled, prototype->GetActor()->mEnabled, prototype->GetActor()->mLocked,
			  prototype->GetActor()->mLocked, prototype->GetActor()->mLayer, Math::Random(), 0)
	{
		transform->SetOwner(this);

		SetPrototype(prototype);

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Dictionary<const Actor*, Actor*> actorsMap;
		Dictionary<const Component*, Component*> componentsMap;

		ProcessCopying(this, prototype->GetActor(), actorPointersFields, componentPointersFields, actorsMap, componentsMap, true);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		UpdateEnabled();
		transform->SetDirty();

		InitializeProperties();

		if (Scene::IsSingletonInitialzed() && (mode == ActorCreateMode::InScene ||
			(mode == ActorCreateMode::Default && mDefaultCreationMode == ActorCreateMode::InScene)))
		{
			o2Scene.mRootActors.Add(this);
			o2Scene.mAllActors.Add(this);
			o2Scene.onActorCreated(this);
		}

		ActorDataNodeConverter::ActorCreated(this);
	}

	Actor::Actor(ActorTransform* transform, ComponentsVec components, ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(transform, mode)
	{
		for (auto comp : components)
			AddComponent(comp);
	}

	Actor::Actor(ActorTransform* transform, const Actor& other):
		Actor(transform, mDefaultCreationMode == ActorCreateMode::InScene, other.mName, other.mEnabled, 
			  other.mEnabled, other.mLocked, other.mLocked, other.mLayer, Math::Random(), other.mAssetId)
	{
		transform->SetOwner(this);

		if (other.mIsAsset)
			SetPrototype(ActorAssetRef(other.GetAssetID()));

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Dictionary<const Actor*, Actor*> actorsMap;
		Dictionary<const Component*, Component*> componentsMap;

		ProcessCopying(this, &other, actorPointersFields, componentPointersFields, actorsMap, componentsMap, true);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		UpdateEnabled();
		transform->SetDirty();

		InitializeProperties();

		if (Scene::IsSingletonInitialzed() && mDefaultCreationMode == ActorCreateMode::InScene)
		{
			o2Scene.mRootActors.Add(this);
			o2Scene.mAllActors.Add(this);
			o2Scene.onActorCreated(this);
		}

		ActorDataNodeConverter::ActorCreated(this);
	}

	Actor::Actor(ActorCreateMode mode /*= CreateMode::Default*/):
		Actor(mnew ActorTransform(), mode)
	{}

	Actor::Actor(ComponentsVec components, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(mnew ActorTransform(), components, mode)
	{}

	Actor::Actor(const ActorAssetRef& prototype, ActorCreateMode mode /*= CreateMode::Default*/) :
		Actor(mnew ActorTransform(*prototype->GetActor()->transform), prototype, mode)
	{}

	Actor::Actor(const Actor& other) :
		Actor(mnew ActorTransform(*other.transform), other)
	{}

	Actor::~Actor()
	{
		for (auto ref : mReferences)
		{
			ref->mActor = nullptr;
			ref->mWasDeleted = true;
		}

		if (mParent)
			mParent->RemoveChild(this, false);
		else
		{
			if (Scene::IsSingletonInitialzed())
				o2Scene.mRootActors.Remove(this);
		}

		if (Scene::IsSingletonInitialzed())
		{
			if (mIsOnScene)
			{
				o2Scene.onActorDestroying(this);
				o2Scene.mAllActors.Remove(this);
			}

			o2Scene.OnActorPrototypeBreaked(this);
		}

		RemoveAllChildren();
		RemoveAllComponents();

		if (mLayer)
		{
			mLayer->mActors.Remove(this);

			if (mResEnabled && mIsOnScene)
				mLayer->mEnabledActors.Remove(this);
		}

		if (Scene::IsSingletonInitialzed())
			o2Scene.mChangedActors.Remove(this);
	}

	Actor& Actor::operator=(const Actor& other)
	{
		RemoveAllChildren();
		RemoveAllComponents();

		SetPrototype(other.mPrototype);

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Dictionary<const Actor*, Actor*> actorsMap;
		Dictionary<const Component*, Component*> componentsMap;

		ProcessCopying(this, &other, actorPointersFields, componentPointersFields, actorsMap, componentsMap, false);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		UpdateEnabled();
		transform->SetDirty();

		OnChanged();

		return *this;
	}

	void Actor::Update(float dt)
	{
		if (transform->mData->updateFrame == 0)
		{
			for (auto child : mChildren)
				child->transform->SetDirty(true);

			UpdateTransform(false);
		}

		for (auto comp : mComponents)
			comp->Update(dt);
	}

	void Actor::UpdateChildren(float dt)
	{
		for (auto child : mChildren)
			child->Update(dt);

		for (auto child : mChildren)
			child->UpdateChildren(dt);
	}

	void Actor::UpdateTransform(bool withChildren /*= true*/)
	{
		transform->Update();

		if (withChildren)
			UpdateChildrenTransforms();
	}

	void Actor::UpdateChildrenTransforms()
	{
		for (auto child : mChildren)
			child->UpdateTransform(false);

		for (auto child : mChildren)
			child->UpdateChildrenTransforms();
	}

	ActorAssetRef Actor::GetPrototype() const
	{
		if (mPrototype)
			return mPrototype;

		if (mPrototypeLink && mParent)
			return mParent->GetPrototype();

		return ActorAssetRef();
	}

	ActorAssetRef Actor::GetPrototypeDirectly() const
	{
		return mPrototype;
	}

	void Actor::BreakPrototypeLink()
	{
		if (!mPrototype && !mPrototypeLink)
			return;

		SetPrototype(ActorAssetRef());

		// remove from cache

		for (auto child : mChildren)
			child->BreakPrototypeLink();

		for (auto component : mComponents)
			component->mPrototypeLink = nullptr;

		OnChanged();
	}

	void Actor::RevertToPrototype()
	{
		if (!mPrototypeLink)
			return;

		Vector<Actor*> separatedActors;
		SeparateActors(separatedActors);

		for (auto it = separatedActors.Begin(); it != separatedActors.End();)
		{
			if ((*it)->mPrototypeLink)
				++it;
			else
			{
				delete *it;
				it = separatedActors.Remove(it);
			}
		}

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Dictionary<const Actor*, Actor*> actorsMap;
		Dictionary<const Component*, Component*> componentsMap;
		Vector<ISerializable*> serializableObjects;

		ProcessReverting(this, mPrototypeLink.Get(), separatedActors, actorPointersFields, componentPointersFields, actorsMap,
						 componentsMap, serializableObjects);

		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		for (auto serializable : serializableObjects)
			serializable->OnDeserialized(DataNode());

		UpdateEnabled();
		transform->SetDirty();

		OnChanged();
	}

	ActorAssetRef Actor::MakePrototype()
	{
		ActorAssetRef prototypeAsset = ActorAssetRef::CreateAsset();
		Actor* prototype = prototypeAsset->GetActor();
		prototype->ExcludeFromScene();

		prototype->RemoveAllChildren();
		prototype->RemoveAllComponents();

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Dictionary<const Actor*, Actor*> actorsMap;
		Dictionary<const Component*, Component*> componentsMap;

		ProcessPrototypeMaking(prototype, this, actorPointersFields, componentPointersFields, actorsMap, componentsMap, false);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		SetPrototype(prototypeAsset);

		prototype->UpdateEnabled();
		prototype->transform->SetDirty();
		prototype->OnChanged();

		return prototypeAsset;
	}

	ActorRef Actor::GetPrototypeLink() const
	{
		return mPrototypeLink;
	}

	bool Actor::IsLinkedToActor(Actor* actor) const
	{
		if (mPrototypeLink)
		{
			auto t = mPrototypeLink.Get();
			while (t)
			{
				if (t == actor)
					return true;

				t = t->mPrototypeLink.Get();
			}
		}

		return false;
	}

	bool Actor::IsLinkedToActor(UInt64 id) const
	{
		if (mPrototypeLink)
		{
			auto t = mPrototypeLink.Get();
			while (t)
			{
				if (t->mId == id)
					return true;

				t = t->mPrototypeLink.Get();
			}
		}

		return false;
	}

	void Actor::SetName(const String& name)
	{
		mName = name;
		OnNameChanged();
	}

	String Actor::GetName() const
	{
		return mName;
	}

	UInt64 Actor::GetID() const
	{
		return mId;
	}

	void Actor::SetID(UInt64 id)
	{
		mId = id;
	}

	UID Actor::GetAssetID() const
	{
		return mAssetId;
	}

	bool Actor::IsAsset() const
	{
		if (mParent)
			return mIsAsset || mParent->IsAsset();

		return mIsAsset;
	}

	void Actor::GenerateNewID(bool childs /*= true*/)
	{
		mId = Math::Random();

		if (childs)
		{
			for (auto child : mChildren)
				child->GenerateNewID(childs);
		}
	}

	void Actor::ExcludeFromScene()
	{
		if (!mIsOnScene)
			return;

		if (!Scene::IsSingletonInitialzed())
			return;

		if (mLayer && mResEnabled)
			mLayer->mEnabledActors.Remove(this);

		o2Scene.mRootActors.Remove(this);
		o2Scene.mAllActors.Remove(this);

		OnExcludeFromScene();

		ExcludeComponentsFromScene();

		mIsOnScene = false;

		for (auto child : mChildren)
			child->ExcludeFromScene();
	}

	void Actor::IncludeInScene()
	{
		if (mIsOnScene)
			return;

		if (!mParent)
			o2Scene.mRootActors.Add(this);

		if (mLayer && mResEnabled)
			mLayer->mEnabledActors.Add(this);

		o2Scene.mAllActors.Add(this);

		mIsOnScene = true;

		OnIncludeToScene();

		IncludeComponentsToScene();

		for (auto child : mChildren)
			child->IncludeInScene();
	}

	bool Actor::IsOnScene() const
	{
		return mIsOnScene;
	}

	void Actor::SetEnabled(bool active)
	{
		if (mEnabled == active)
			return;

		mEnabled = active;
		UpdateEnabled();

		onEnableChanged(mEnabled);
		o2Scene.onActorEnableChanged(this);

		OnChanged();
	}

	void Actor::Enable()
	{
		SetEnabled(true);
	}

	void Actor::Disable()
	{
		SetEnabled(false);
	}

	bool Actor::IsEnabled() const
	{
		return mEnabled;
	}

	bool Actor::IsEnabledInHierarchy() const
	{
		return mResEnabled;
	}

	void Actor::SetLocked(bool locked)
	{
		mLocked = locked;
		UpdateLocking();

		OnLockChanged();
	}

	void Actor::Lock()
	{
		SetLocked(true);
	}

	void Actor::Unlock()
	{
		SetLocked(false);
	}

	bool Actor::IsLocked() const
	{
		return mLocked;
	}

	bool Actor::IsLockedInHierarchy() const
	{
		return mResLocked;
	}

	void Actor::SetPositionIndexInParent(int index)
	{
		if (mParent)
		{
			int lastIdx = mParent->mChildren.Find(this);
			mParent->mChildren.Insert(this, index);

			if (index <= lastIdx)
				lastIdx++;

			mParent->mChildren.RemoveAt(lastIdx);
			mParent->OnChildsChanged();
		}
		else
		{
			if (mIsOnScene)
			{
				int lastIdx = o2Scene.mRootActors.Find(this);
				o2Scene.mRootActors.Insert(this, index);

				if (index <= lastIdx)
					lastIdx++;

				o2Scene.mRootActors.RemoveAt(lastIdx);
			}
		}
	}

	void Actor::SetParent(Actor* actor, bool worldPositionStays /*= true*/)
	{
		if ((actor && actor->mChildren.Contains(this)) || actor == this || actor == mParent)
			return;

		Basis lastParentBasis = transform->GetWorldBasis();
		auto oldParent = mParent;

		if (mParent)
			mParent->RemoveChild(this, false);
		else
			o2Scene.mRootActors.Remove(this);

		mParent = actor;

		if (mParent)
			mParent->mChildren.Add(this);
		else
			o2Scene.mRootActors.Add(this);

		if (worldPositionStays)
			transform->SetWorldBasis(lastParentBasis);
		else
			transform->SetDirty();

		UpdateEnabled();

		if (actor && actor->mIsOnScene && !mIsOnScene)
			ExcludeFromScene();

		if (actor && !actor->mIsOnScene && mIsOnScene)
			IncludeInScene();

		OnParentChanged(oldParent);
	}

	Actor* Actor::GetParent() const
	{
		return mParent;
	}

	Actor* Actor::AddChild(Actor* actor)
	{
		if (mChildren.Contains(actor) || actor == this)
			return actor;

		auto oldParent = actor->mParent;

		if (actor->mParent)
			actor->mParent->RemoveChild(actor, false);
		else
		{
			if (Scene::IsSingletonInitialzed())
				o2Scene.mRootActors.Remove(actor);
		}

		mChildren.Add(actor);
		actor->mParent = this;

		actor->transform->SetDirty();
		actor->UpdateEnabled();

		OnChildAdded(actor);
		OnChildsChanged();
		actor->OnParentChanged(oldParent);

		if (actor->mIsOnScene && !mIsOnScene)
			ExcludeFromScene();

		if (!actor->mIsOnScene && mIsOnScene)
			IncludeInScene();

		return actor;
	}

	Actor* Actor::AddChild(Actor* actor, int index)
	{
		if (mChildren.Contains(actor) || actor == this)
			return actor;

		auto oldParent = actor->mParent;

		if (actor->mParent)
			actor->mParent->RemoveChild(actor, false);
		else
			o2Scene.mRootActors.Remove(actor);

		mChildren.Insert(actor, index);
		actor->mParent = this;

		actor->transform->SetDirty();
		actor->UpdateEnabled();

		OnChildAdded(actor);
		OnChildsChanged();
		actor->OnParentChanged(oldParent);

		if (actor->mIsOnScene && !mIsOnScene)
			ExcludeFromScene();

		if (!actor->mIsOnScene && mIsOnScene)
			IncludeInScene();

		return actor;
	}

	Vector<Actor*> Actor::AddChildren(Vector<Actor*> actors)
	{
		for (auto actor : actors)
			AddChild(actor);

		return actors;
	}

	Actor* Actor::GetChild(const String& path) const
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		if (pathPart == "..")
		{
			if (mParent)
			{
				if (delPos == -1)
					return mParent;
				else
					return mParent->GetChild(path.SubStr(delPos + 1));
			}

			return nullptr;
		}

		for (auto child : mChildren)
		{
			if (child->mName == pathPart)
			{
				if (delPos == -1)
					return child;
				else
					return child->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	Actor::ActorsVec Actor::GetChildren() const
	{
		return mChildren;
	}

	void Actor::RemoveChild(Actor* actor, bool release /*= true*/)
	{
		if (!mChildren.Contains(actor))
			return;

		auto oldParent = actor->mParent;

		actor->mParent = nullptr;
		mChildren.Remove(actor);

		if (release)
		{
			actor->OnParentChanged(oldParent);
			OnChildRemoved(actor);
			OnChildsChanged();

			delete actor;
		}
		else
		{
			actor->OnParentChanged(oldParent);
			actor->transform->SetDirty();
			actor->UpdateEnabled();
		}
	}

	void Actor::RemoveAllChildren(bool release /*= true*/)
	{
		for (auto child : mChildren)
		{
			child->mParent = nullptr;
			OnChildRemoved(child);
			child->OnParentChanged(this);

			if (release)
				delete child;
		}

		mChildren.Clear();

		OnChildsChanged();
	}

	Component* Actor::AddComponent(Component* component)
	{
		component->SetOwnerActor(this);
		OnChanged();
		return component;
	}

	void Actor::RemoveComponent(Component* component, bool release /*= true*/)
	{
		mComponents.Remove(component);
		component->mOwner = nullptr;

		OnChanged();

		if (release)
			delete component;
	}

	void Actor::RemoveAllComponents()
	{
		auto components = mComponents;
		for (auto comp : components)
			delete comp;

		mComponents.Clear();

		OnChanged();
	}

	Component* Actor::GetComponent(const String& typeName)
	{
		for (auto comp : mComponents)
			if (comp->GetType().GetName() == typeName)
				return comp;

		return nullptr;
	}

	Component* Actor::GetComponent(const Type* type)
	{
		for (auto comp : mComponents)
			if (comp->GetType().IsBasedOn(*type))
				return comp;

		return nullptr;
	}

	Component* Actor::GetComponent(UInt64 id)
	{
		for (auto comp : mComponents)
			if (comp->GetID() == id)
				return comp;

		return nullptr;
	}

	Actor::ComponentsVec Actor::GetComponents() const
	{
		return mComponents;
	}

	void Actor::SetLayer(SceneLayer* layer)
	{
		if (layer == mLayer)
			return;

		SceneLayer* lastLayer = mLayer;

		if (layer == nullptr)
			layer = o2Scene.GetDefaultLayer();

		mLayer = layer;

		if (lastLayer)
		{
			lastLayer->mActors.Remove(this);

			if (mResEnabled && mIsOnScene)
				lastLayer->mEnabledActors.Remove(this);
		}

		layer->mActors.Add(this);

		if (mResEnabled && mIsOnScene)
			layer->mEnabledActors.Add(this);

		OnLayerChanged(lastLayer);

		for (auto comp : mComponents)
			comp->OnLayerChanged(lastLayer, layer);

		OnChanged();
	}

	void Actor::SetLayer(const String& layerName)
	{
		SetLayer(o2Scene.GetLayer(layerName));
	}

	SceneLayer* Actor::GetLayer() const
	{
		return mLayer;
	}

	String Actor::GetLayerName() const
	{
		return mLayer->name;
	}

	Actor* Actor::FindLinkedActor(Actor* linkActor)
	{
		if (GetPrototypeLink() == linkActor)
			return this;

		for (auto child : mChildren)
		{
			if (auto res = child->FindLinkedActor(linkActor))
				return res;
		}

		return nullptr;
	}

	Actor* Actor::FindLinkedActor(UInt64 id)
	{
		if (IsLinkedToActor(id))
			return this;

		for (auto child : mChildren)
		{
			if (auto res = child->FindLinkedActor(id))
				return res;
		}

		return nullptr;
	}

	Actor* Actor::FindActorById(UInt64 id)
	{
		if (mId == id)
			return this;

		for (auto child : mChildren)
			if (auto res = child->FindActorById(id))
				return res;

		return nullptr;
	}

	void Actor::SetDefaultCreationMode(ActorCreateMode mode)
	{
		if (mode == ActorCreateMode::Default)
			mode = ActorCreateMode::InScene;

		mDefaultCreationMode = mode;
	}

	ActorCreateMode Actor::GetDefaultCreationMode()
	{
		return mDefaultCreationMode;
	}

	void Actor::SetProtytypeDummy(ActorAssetRef asset)
	{}

	void Actor::OnTransformUpdated()
	{
		for (auto comp : mComponents)
			comp->OnTransformUpdated();
	}

	void Actor::SetParentProp(Actor* actor)
	{
		SetParent(actor, false);
	}

	void Actor::SetPrototype(ActorAssetRef asset)
	{
		o2Scene.OnActorPrototypeBreaked(this);

		auto linkAsset = asset;
		while (linkAsset)
		{
			o2Scene.OnActorLinkedToPrototype(linkAsset, this);
			linkAsset = linkAsset->GetActor()->GetPrototype();
		}

		mPrototype = asset;
		if (asset)
			mPrototypeLink = asset->GetActor();
		else
			mPrototypeLink = nullptr;
	}

	void Actor::UpdateEnabled()
	{
		bool lastResEnabled = mResEnabled;

		if (mParent)
			mResEnabled = mEnabled && mParent->mResEnabled;
		else
			mResEnabled = mEnabled;

		if (lastResEnabled != mResEnabled)
		{
			if (mResEnabled)
				mLayer->mEnabledActors.Add(this);
			else
				mLayer->mEnabledActors.Remove(this);

			o2Scene.onActorEnableChanged(this);
			OnChanged();
		}

		for (auto comp : mComponents)
			comp->UpdateEnabled();

		for (auto child : mChildren)
			child->UpdateEnabled();
	}

	void Actor::UpdateLocking()
	{
		bool lastResLocked = mResLocked;

		if (mParent)
			mResLocked = mLocked || mParent->mResLocked;
		else
			mResLocked = mLocked;

		if (lastResLocked != mResLocked)
			OnLockChanged();

		for (auto child : mChildren)
			child->UpdateLocking();
	}

	void Actor::OnSerialize(DataNode& node) const
	{
		if (mPrototypeLink)
			SerializeWithProto(node);
		else
			SerializeRaw(node);
	}

	void Actor::OnDeserialized(const DataNode& node)
	{
		if (node.GetNode("PrototypeLink") || node.GetNode("Prototype"))
			DeserializeWithProto(node);
		else
			DeserializeRaw(node);
	}

	void Actor::SerializeRaw(DataNode& node) const
	{
		node["Id"] = mId;
		node["Name"] = mName;
		node["Enabled"] = mEnabled;
		node["Locked"] = mLocked;

		node["Transform"].SetValue(*transform);

		if (mLayer)
			node["LayerName"] = mLayer->name;

		auto childsNode = node.AddNode("Childs");
		for (auto child : mChildren)
		{
			auto childNode = childsNode->AddNode("Child");
			*childNode->AddNode("Type") = child->GetType().GetName();
			*childNode->AddNode("Data") = child->Serialize();
		}

		auto componentsNode = node.AddNode("Components");
		for (auto component : mComponents)
		{
			auto componentNode = componentsNode->AddNode("Component");
			*componentNode->AddNode("Data") = component->Serialize();
			*componentNode->AddNode("Type") = component->GetType().GetName();
		}
	}

	void Actor::DeserializeRaw(const DataNode& node)
	{
		ActorDataNodeConverter::Instance().LockPointersResolving();
		if (ActorDataNodeConverter::Instance().mLockDepth == 0)
			ActorDataNodeConverter::Instance().ActorCreated(this);

		mId = *node.GetNode("Id");
		mName = *node.GetNode("Name");
		mLocked = *node.GetNode("Locked");
		mEnabled = *node.GetNode("Enabled");

		if (GetType().GetName() == "o2::UIMenuPanel")
			o2Debug.Log("asd");

		if (auto layerNode = node.GetNode("LayerName"))
			SetLayer(layerNode->Data());
		else
			SetLayer(o2Scene.GetDefaultLayer());

		node.GetNode("Transform")->GetValue(*transform);

		RemoveAllComponents();

		if (auto componentsNode = node.GetNode("Components"))
		{
			for (auto componentNode : componentsNode->GetChildNodes())
			{
				Component* component = (Component*)o2Reflection.CreateTypeSample(*componentNode->GetNode("Type"));
				component->Deserialize(*componentNode->GetNode("Data"));
				component->SetOwnerActor(this);
			}
		}

		RemoveAllChildren();

		if (auto childsNode = node.GetNode("Childs"))
		{
			for (auto childNode : childsNode->GetChildNodes())
			{
				DataNode* typeNode = childNode->GetNode("Type");
				DataNode* dataNode = childNode->GetNode("Data");
				if (typeNode && dataNode)
				{
					const ObjectType* type = dynamic_cast<const ObjectType*>(o2Reflection.GetType(typeNode->Data()));
					if (type)
					{
						Actor* child = dynamic_cast<Actor*>(type->DynamicCastToIObject(type->CreateSample()));
						child->Deserialize(*dataNode);
						o2Scene.mRootActors.Remove(child);
						AddChild(child);
					}
				}
			}
		}

		ActorDataNodeConverter::Instance().UnlockPointersResolving();
		ActorDataNodeConverter::Instance().ResolvePointers();
	}

	void Actor::SerializeWithProto(DataNode& node) const
	{
		const Actor* proto = mPrototypeLink.Get();

		// Prototype data
		if (mPrototype)
			node["Prototype"] = mPrototype;

		if (mPrototypeLink)
			node["PrototypeLink"] = proto->GetID();

		// Basic data
		node["Id"] = mId;

		if (mName != proto->mName)
			node["Name"] = mName;

		if (mEnabled != proto->mEnabled)
			node["Enabled"] = mEnabled;

		if (mLocked != proto->mLocked)
			node["Locked"] = mLocked;

		if (mLayer != proto->mLayer)
			node["LayerName"] = mLayer->name;

		// Transform data
		auto transformNode = node.AddNode("Transform");

		if (transform->mData->position != proto->transform->mData->position)
			(*transformNode)["Position"] = transform->mData->position;

		if (transform->mData->size != proto->transform->mData->size)
			(*transformNode)["Size"] = transform->mData->size;

		if (transform->mData->scale != proto->transform->mData->scale)
			(*transformNode)["Scale"] = transform->mData->scale;

		if (transform->mData->pivot != proto->transform->mData->pivot)
			(*transformNode)["Pivot"] = transform->mData->pivot;

		if (!Math::Equals(transform->mData->angle, proto->transform->mData->angle))
			(*transformNode)["Angle"] = transform->mData->angle;

		if (!Math::Equals(transform->mData->shear, proto->transform->mData->shear))
			(*transformNode)["Shear"] = transform->mData->shear;

		// Children data
		auto childsNode = node.AddNode("Childs");
		for (auto child : mChildren)
		{
			auto childNode = childsNode->AddNode("Child");
			*childNode->AddNode("Type") = child->GetType().GetName();
			*childNode->AddNode("Data") = child->Serialize();
		}

		// Components data
		auto componentsNode = node.AddNode("Components");
		for (auto component : mComponents)
		{
			auto compNode = componentsNode->AddNode("Component");
			*compNode->AddNode("Type") = component->GetType().GetName();

			auto& dataNode = *compNode->AddNode("Data");
			if (auto componentProtoLink = component->mPrototypeLink)
			{
				dataNode["PrototypeLink"] = componentProtoLink->mId;

				dataNode.SetValueDelta(*component, *component->mPrototypeLink);
			}
			else dataNode = component->Serialize();
		}
	}

	void Actor::DeserializeWithProto(const DataNode& node)
	{
		ActorDataNodeConverter::Instance().LockPointersResolving();
		ActorDataNodeConverter::Instance().ActorCreated(this);

		RemoveAllChildren();
		RemoveAllComponents();

		if (auto prototypeNode = node.GetNode("Prototype"))
		{
			SetPrototype(*prototypeNode);
		}

		if (auto prototypeLinkNode = node.GetNode("PrototypeLink"))
		{
			UInt64 id = *prototypeLinkNode;
			Actor* parent = mParent;
			while (parent && parent->mPrototypeLink)
			{
				bool found = false;

				Actor* protoLink = parent->mPrototypeLink.Get();
				while (protoLink)
				{
					if (auto fnd = protoLink->FindActorById(id))
					{
						mPrototypeLink = fnd;
						found = true;
						break;
					}

					protoLink = protoLink->mPrototypeLink.Get();
				}

				if (found)
					break;

				parent = parent->mParent;
			}
		}

		mId = *node.GetNode("Id");

		if (!mPrototypeLink)
			return;

		const Actor* proto = mPrototypeLink.Get();

		if (auto subNode = node.GetNode("Name"))
			mName = *subNode;
		else
			mName = proto->mName;

		if (auto subNode = node.GetNode("Enabled"))
			mEnabled = *subNode;
		else
			mEnabled = proto->mEnabled;

		if (auto subNode = node.GetNode("Locked"))
			mLocked = *subNode;
		else
			mLocked = proto->mLocked;

		if (auto subNode = node.GetNode("LayerName"))
			SetLayer(o2Scene.GetLayer(*subNode));
		else
			SetLayer(proto->mLayer);

		// Transform data
		if (auto transformNode = node.GetNode("Transform"))
		{
			if (auto subNode = transformNode->GetNode("Position"))
				transform->mData->position = *subNode;
			else
				transform->mData->position = proto->transform->mData->position;

			if (auto subNode = transformNode->GetNode("Size"))
				transform->mData->size = *subNode;
			else
				transform->mData->size = proto->transform->mData->size;

			if (auto subNode = transformNode->GetNode("Scale"))
				transform->mData->scale = *subNode;
			else
				transform->mData->scale = proto->transform->mData->scale;

			if (auto subNode = transformNode->GetNode("Pivot"))
				transform->mData->pivot = *subNode;
			else
				transform->mData->pivot = proto->transform->mData->pivot;

			if (auto subNode = transformNode->GetNode("Angle"))
				transform->mData->angle = *subNode;
			else
				transform->mData->angle = proto->transform->mData->angle;

			if (auto subNode = transformNode->GetNode("Shear"))
				transform->mData->shear = *subNode;
			else
				transform->mData->shear = proto->transform->mData->shear;
		}

		RemoveAllChildren();

		// children
		if (auto childsNode = node.GetNode("Childs"))
		{
			for (auto childNode : *childsNode)
			{
				DataNode* typeNode = childNode->GetNode("Type");
				DataNode* dataNode = childNode->GetNode("Data");
				if (typeNode && dataNode)
				{
					const ObjectType* type = dynamic_cast<const ObjectType*>(o2Reflection.GetType(typeNode->Data()));
					if (type)
					{
						Actor* child = dynamic_cast<Actor*>(type->DynamicCastToIObject(type->CreateSample()));
						child->Deserialize(*dataNode);
						o2Scene.mRootActors.Remove(child);
						AddChild(child);
					}
				}
			}
		}

		RemoveAllComponents();

		// components
		if (auto componentsNode = node.GetNode("Components"))
		{
			for (auto componentNode : *componentsNode)
			{
				String type = (*componentNode)["Type"];
				Component* newComponent = (Component*)o2Reflection.CreateTypeSample(type);

				mComponents.Add(newComponent);
				newComponent->mOwner = this;

				if (newComponent)
				{
					auto componentDataNode = (*componentNode)["Data"];

					if (auto prototypeLinkNode = componentDataNode.GetNode("PrototypeLink"))
					{
						UInt64 id = *prototypeLinkNode;
						if (mPrototypeLink)
						{
							Actor* x = mPrototypeLink.Get();
							while (x)
							{
								bool found = false;

								for (auto protoLinkComponent : x->mComponents)
								{
									if (protoLinkComponent->mId == id)
									{
										newComponent->mPrototypeLink = protoLinkComponent;
										found = true;
										break;
									}
								}

								if (found)
									break;

								x = x->mPrototypeLink.Get();
							}
						}

						if (!newComponent->mPrototypeLink)
							newComponent->Deserialize(componentDataNode);
						else
							componentDataNode.GetValueDelta(*newComponent, *newComponent->mPrototypeLink);
					}
				}
				else o2Debug.LogError("Can't create component with type:" + type);
			}
		}

		transform->SetDirty();

		ActorDataNodeConverter::Instance().UnlockPointersResolving();
		ActorDataNodeConverter::Instance().ResolvePointers();
	}

	Dictionary<String, Actor*> Actor::GetAllChilds()
	{
		Dictionary<String, Actor*> res;
		for (auto child : mChildren)
			res.Add(child->GetName(), child);

		return res;
	}

	Dictionary<String, Component*> Actor::GetAllComponents()
	{
		Dictionary<String, Component*> res;
		for (auto child : mComponents)
			res.Add(child->GetType().GetName(), child);

		return res;
	}

	void Actor::OnExcludeFromScene()
	{}

	void Actor::OnIncludeToScene()
	{}

	void Actor::ExcludeComponentsFromScene()
	{
		for (auto comp : mComponents)
			comp->OnExcludeFromScene();
	}

	void Actor::IncludeComponentsToScene()
	{
		for (auto comp : mComponents)
			comp->OnIncludeToScene();
	}

#ifdef IS_EDITOR

	void Actor::OnChanged()
	{
		onChanged();

		if (Scene::IsSingletonInitialzed() && mIsOnScene)
			o2Scene.OnActorChanged(this);
	}

	void Actor::OnLockChanged()
	{
		onLockChanged(mLocked);
		onChanged();

		if (Scene::IsSingletonInitialzed() && mIsOnScene)
		{
			o2Scene.OnActorChanged(this);
			o2Scene.onActorLockChanged(this);
		}
	}

	void Actor::OnNameChanged()
	{
		onNameChanged();
		onChanged();

		if (Scene::IsSingletonInitialzed() && mIsOnScene)
		{
			o2Scene.OnActorChanged(this);
			o2Scene.onActorNameChanged(this);
		}
	}

	void Actor::OnChildsChanged()
	{
		onChildHierarchyChanged();
		onChanged();

		if (Scene::IsSingletonInitialzed() && mIsOnScene)
		{
			o2Scene.OnActorChanged(this);
			o2Scene.onActorChildsHierarchyChanged(this);
		}
	}

	void Actor::OnParentChanged(Actor* oldParent)
	{
		onParentChanged(oldParent);
		onChanged();

		if (Scene::IsSingletonInitialzed() && mIsOnScene)
		{
			o2Scene.OnActorChanged(this);
			o2Scene.OnActorChanged(oldParent);
			o2Scene.OnActorChanged(mParent);

			o2Scene.onActorChildsHierarchyChanged(oldParent);
			o2Scene.onActorChildsHierarchyChanged(mParent);
		}
	}

	void Actor::OnChildAdded(Actor* child)
	{}

	void Actor::OnChildRemoved(Actor* child)
	{}

	void Actor::OnLayerChanged(SceneLayer* oldLayer)
	{}

	void Actor::SeparateActors(Vector<Actor*>& separatedActors)
	{
		for (auto child : mChildren)
		{
			child->mParent = nullptr;
			separatedActors.Add(child);

			child->SeparateActors(separatedActors);
		}

		mChildren.Clear();
	}

	void Actor::GetAllChildrenActors(Vector<Actor*>& actors)
	{
		actors.Add(mChildren);

		for (auto child : mChildren)
			child->GetAllChildrenActors(actors);
	}

	void Actor::ApplyChangesToPrototype()
	{
		if (!mPrototype)
			return;

		Actor* prototypeActor = mPrototype->GetActor();

		// collecting data
		Vector<Actor*> linkedProtoActors = o2Scene.mPrototypeLinksCache[mPrototype];
		linkedProtoActors.Remove(this);

		Vector<ApplyActorInfo> applyActorsInfos(linkedProtoActors.Count());
		for (auto linkedActor : linkedProtoActors)
		{
			applyActorsInfos.Add(ApplyActorInfo());
			applyActorsInfos.Last().actor = linkedActor;
			applyActorsInfos.Last().allChildren.Add(linkedActor);
			linkedActor->GetAllChildrenActors(applyActorsInfos.Last().allChildren);
		}

		Vector<Actor*> allProtoChildren, allThisChildren;
		allProtoChildren.Add(prototypeActor);
		prototypeActor->GetAllChildrenActors(allProtoChildren);

		allThisChildren.Add(this);
		GetAllChildrenActors(allThisChildren);

		Vector<ISerializable*> serializableObjects;

		// check removed actors
		for (int i = allProtoChildren.Count() - 1; i >= 0; i--)
		{
			Actor* itActor = allProtoChildren[i];
			bool removed = allThisChildren.FindMatch([&](Actor* x) { return x->IsLinkedToActor(itActor); }) == nullptr;
			if (removed)
			{
				for (auto& info : applyActorsInfos)
				{
					for (int j = info.allChildren.Count() - 1; j >= 0; j--)
					{
						Actor* childItActor = info.allChildren[j];

						if (childItActor->IsLinkedToActor(itActor))
						{
							delete childItActor;
							info.allChildren.RemoveAt(j);
						}
					}
				}

				delete itActor;
				allProtoChildren.RemoveAt(i);
			}
		}

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Dictionary<const Actor*, Actor*> actorsMap;
		Dictionary<const Component*, Component*> componentsMap;

		// check new and modified actors
		for (auto child : allThisChildren)
		{
			Actor* protoChild = child->mPrototypeLink.Get();

			while (protoChild && !allProtoChildren.Contains(protoChild) && protoChild->mPrototypeLink)
				protoChild = protoChild->mPrototypeLink.Get();

			if (!allProtoChildren.Contains(protoChild))
				protoChild = nullptr;

			// Modified actor
			if (protoChild)
			{
				actorsMap.Add(child, protoChild);

				for (auto& info : applyActorsInfos)
				{
					info.matchingChild = info.allChildren.FindMatch([&](Actor* x) { return x->IsLinkedToActor(protoChild); });

					if (info.matchingChild)
					{
						info.actorsMap.Add(child, info.matchingChild);
						CopyActorChangedFields(protoChild, child, info.matchingChild, info.allChildren, info.actor != info.matchingChild);
					}
				}

				if (child->mParent && child->mParent->mPrototypeLink)
				{
					Actor* newParent = allProtoChildren.FindMatch([&](Actor* x) { return child->mParent->IsLinkedToActor(x); });
					protoChild->SetParent(newParent);
				}

				protoChild->mName = child->mName;
				protoChild->mEnabled = child->mEnabled;
				*protoChild->transform = *child->transform;
				protoChild->mAssetId = child->mAssetId;
				protoChild->SetLayer(child->mLayer);

				// check new/removed components 
				// removing
				auto childProtoLinkComponents = protoChild->mComponents;
				for (auto protoComponent : childProtoLinkComponents)
				{
					bool removed = child->mComponents.FindMatch([&](Component* x) {
						return x->IsLinkedToComponent(protoComponent); }) == nullptr;

					if (removed)
					{
						for (auto& info : applyActorsInfos)
						{
							if (!info.matchingChild)
								continue;

							Component* matchingComponent = info.matchingChild->mComponents.FindMatch([&](Component* x) {
								return x->IsLinkedToComponent(protoComponent); });

							if (matchingComponent)
								delete matchingComponent;
						}

						delete protoComponent;
					}
				}

				// new or changed
				for (auto component : child->mComponents)
				{
					Component* protoComponent = component->GetPrototypeLink();
					if (protoComponent)
					{
						// check differences
						Vector<FieldInfo*> fields;
						GetComponentFields(protoComponent, fields);

						for (auto& info : applyActorsInfos)
						{
							if (!info.matchingChild)
								continue;

							Component* matchingComponent = info.matchingChild->mComponents.FindMatch([&](Component* x) {
								return x->IsLinkedToComponent(protoComponent); });

							if (!matchingComponent)
								continue;

							CopyChangedFields(fields, protoComponent, component, matchingComponent,
											  info.actorPointersFields, info.componentPointersFields,
											  serializableObjects);
						}

						CopyFields(fields, component, protoComponent, actorPointersFields, componentPointersFields,
								   serializableObjects);
					}
					else
					{
						// new component
						Component* newProtoComponent = protoChild->AddComponent(component->CloneAs<Component>());
						componentsMap.Add(component, newProtoComponent);

						component->mPrototypeLink = newProtoComponent;

						CollectFixingFields(newProtoComponent, componentPointersFields, actorPointersFields);

						for (auto& info : applyActorsInfos)
						{
							if (!info.matchingChild)
								continue;

							Component* newComponent = info.matchingChild->AddComponent(component->CloneAs<Component>());
							newComponent->mPrototypeLink = newProtoComponent;
							info.componentsMap.Add(component, newComponent);

							CollectFixingFields(newComponent, info.componentPointersFields, info.actorPointersFields);
						}
					}
				}

				continue;
			}

			// new child
			Actor* newProtoChild = mnew Actor(ActorCreateMode::NotInScene);
			allProtoChildren.Add(newProtoChild);
			actorsMap.Add(child, newProtoChild);

			Actor* childParentProtoLink = child->mParent->mPrototypeLink.Get();
			if (!allProtoChildren.Contains(childParentProtoLink))
				childParentProtoLink = allProtoChildren.FindMatch([&](Actor* x) { return x->IsLinkedToActor(protoChild); });

			newProtoChild->SetParent(childParentProtoLink);

			newProtoChild->mName     = child->mName;
			newProtoChild->mEnabled  = child->mEnabled;
			*newProtoChild->transform = *child->transform;
			newProtoChild->mAssetId  = child->mAssetId;
			newProtoChild->SetLayer(child->mLayer);

			if (child->mPrototype)
				newProtoChild->SetPrototype(child->mPrototype);

			bool isChildProtoLinked = child->mPrototypeLink != nullptr;

			if (isChildProtoLinked)
				newProtoChild->mPrototypeLink = child->mPrototypeLink;

			child->mPrototypeLink = newProtoChild;

			// copy components
			for (auto component : child->mComponents)
			{
				Component* newProtoComponent = newProtoChild->AddComponent(component->CloneAs<Component>());
				componentsMap.Add(component, newProtoComponent);

				if (isChildProtoLinked)
					newProtoComponent->mPrototypeLink = component->mPrototypeLink;
				else
					component->mPrototypeLink = newProtoComponent;

				CollectFixingFields(newProtoComponent, componentPointersFields, actorPointersFields);
			}

			for (auto& info : applyActorsInfos)
			{
				Actor* newChild = mnew Actor(info.actor->mIsOnScene ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);
				info.allChildren.Add(newChild);
				info.actorsMap.Add(child, newChild);

				Actor* newChildParent = info.allChildren.FindMatch([&](Actor* x) { return x->IsLinkedToActor(childParentProtoLink); });

				newChild->SetParent(newChildParent);

				newChild->mName     = child->mName;
				newChild->mEnabled  = child->mEnabled;
				*newChild->transform = *child->transform;
				newChild->mAssetId  = child->mAssetId;
				newChild->SetLayer(child->mLayer);

				if (child->mPrototype)
					newChild->SetPrototype(child->mPrototype);

				newChild->mPrototypeLink = newProtoChild;

				for (auto component : child->mComponents)
				{
					Component* newComponent = newChild->AddComponent(CloneAs<Component>());
					info.componentsMap.Add(component, newComponent);
					newComponent->mPrototypeLink = component->mPrototypeLink;
					CollectFixingFields(newComponent, info.componentPointersFields, info.actorPointersFields);
				}

				if (newChild->mIsOnScene)
					newChild->SetLayer(child->mLayer);
			}
		}

		// fix actors and components fields
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		for (auto& info : applyActorsInfos)
		{
			FixComponentFieldsPointers(info.actorPointersFields, info.componentPointersFields, info.actorsMap,
									   info.componentsMap);

			info.actor->OnChanged();
		}

		for (auto serializable : serializableObjects)
			serializable->OnDeserialized(DataNode());

		// update transformation
		transform->SetDirty();

		for (auto& info : applyActorsInfos)
			info.actor->transform->UpdateTransform();

		OnChanged();

		mPrototype->Save();
	}

	void Actor::CopyData(const Actor& otherActor)
	{
		mName = otherActor.mName;
		mEnabled = otherActor.mEnabled;
		transform->CopyFrom(*otherActor.transform);
		mAssetId = otherActor.mAssetId;
	}

	void Actor::ProcessCopying(Actor* dest, const Actor* source, Vector<Actor**>& actorsPointers,
							   Vector<Component**>& componentsPointers, Dictionary<const Actor*, Actor*>& actorsMap,
							   Dictionary<const Component*, Component*>& componentsMap,
							   bool isSourcePrototype)
	{
		dest->CopyData(*source);

		if (!dest->mPrototype && source->mPrototype)
		{
			dest->mPrototype = source->mPrototype;

			ActorAssetRef proto = source->mPrototype;
			while (proto)
			{
				o2Scene.OnActorLinkedToPrototype(proto, dest);
				proto = proto->GetActor()->GetPrototype();
			}
		}

		if (dest->mParent && dest->mParent->mPrototypeLink)
		{
			if (isSourcePrototype)
				dest->mPrototypeLink = const_cast<Actor*>(source);
			else
				dest->mPrototypeLink = source->mPrototypeLink;
		}

		actorsMap.Add(source, dest);

		for (auto child : source->mChildren)
		{
			const ObjectType* type = dynamic_cast<const ObjectType*>(&child->GetType());

			Actor* newChild = dynamic_cast<Actor*>(type->DynamicCastToIObject(child->GetType().CreateSample()));
			if (!dest->mIsOnScene)
				newChild->ExcludeFromScene();

			dest->AddChild(newChild);

			ProcessCopying(newChild, child, actorsPointers, componentsPointers, actorsMap, componentsMap, isSourcePrototype);
		}

		for (auto component : source->mComponents)
		{
			Component* newComponent = dest->AddComponent(CloneAs<Component>());

			componentsMap.Add(component, newComponent);

			if (dest->mPrototypeLink)
			{
				if (isSourcePrototype)
					newComponent->mPrototypeLink = component;
				else
					newComponent->mPrototypeLink = component->mPrototypeLink;
			}

			CollectFixingFields(newComponent, componentsPointers, actorsPointers);
		}

		dest->SetLayer(source->mLayer);
	}

	void Actor::ProcessPrototypeMaking(Actor* dest, Actor* source, Vector<Actor**>& actorsPointers,
									   Vector<Component**>& componentsPointers,
									   Dictionary<const Actor*, Actor*>& actorsMap,
									   Dictionary<const Component*, Component*>& componentsMap,
									   bool isInsidePrototype)
	{
		dest->mName = source->mName;
		dest->mEnabled = source->mEnabled;
		*dest->transform = *source->transform;
		dest->mAssetId = source->mAssetId;

		if (!isInsidePrototype && !source->mPrototype && source->mPrototypeLink)
			source->mPrototypeLink = nullptr;

		dest->SetPrototype(source->mPrototype);

		dest->mPrototypeLink = source->mPrototypeLink;
		source->mPrototypeLink = dest;

		actorsMap.Add(source, dest);

		for (auto child : source->mChildren)
		{
			Actor* newChild = mnew Actor(dest->mIsOnScene ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);
			dest->AddChild(newChild);

			ProcessPrototypeMaking(newChild, child, actorsPointers, componentsPointers, actorsMap, componentsMap,
								   source->mPrototype.IsValid() || isInsidePrototype);
		}

		for (auto component : source->mComponents)
		{
			Component* newComponent = dest->AddComponent(CloneAs<Component>());
			componentsMap.Add(component, newComponent);
			newComponent->mPrototypeLink = component->mPrototypeLink;
			component->mPrototypeLink = newComponent;
			CollectFixingFields(newComponent, componentsPointers, actorsPointers);
		}

		dest->SetLayer(source->mLayer);
	}

	void Actor::ProcessReverting(Actor* dest, const Actor* source, const Vector<Actor*>& separatedActors,
								 Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers,
								 Dictionary<const Actor*, Actor*>& actorsMap,
								 Dictionary<const Component*, Component*>& componentsMap,
								 Vector<ISerializable*>& serializableObjects)
	{
		dest->mName = source->mName;
		dest->mEnabled = source->mEnabled;
		*dest->transform = *source->transform;
		dest->mAssetId = source->mAssetId;

		actorsMap.Add(source, dest);

		for (auto child : source->mChildren)
		{
			Actor* newChild = nullptr;

			newChild = separatedActors.FindMatch([&](Actor* x) { return x->GetPrototypeLink() == child; });

			if (!newChild)
				newChild = mnew Actor(dest->mIsOnScene ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);

			dest->AddChild(newChild);

			ProcessReverting(newChild, child, separatedActors, actorsPointers, componentsPointers, actorsMap,
							 componentsMap, serializableObjects);
		}

		for (auto it = dest->mComponents.Begin(); it != dest->mComponents.End();)
		{
			if (!(*it)->mPrototypeLink)
			{
				(*it)->mOwner = nullptr;
				delete *it;
				it = dest->mComponents.Remove(it);
			}
			else ++it;
		}

		for (auto component : source->mComponents)
		{
			Component* matchingComponent = dest->mComponents.FindMatch([&](Component* x) { return x->GetPrototypeLink() == component; });
			if (matchingComponent)
			{
				Vector<FieldInfo*> fields;
				GetComponentFields(matchingComponent, fields);

				CopyFields(fields, component, matchingComponent, actorsPointers, componentsPointers,
						   serializableObjects);

				continue;
			}

			Component* newComponent = dest->AddComponent(CloneAs<Component>());
			componentsMap.Add(component, newComponent);

			CollectFixingFields(newComponent, componentsPointers, actorsPointers);
		}

		dest->SetLayer(source->mLayer);
	}

	void Actor::CopyFields(Vector<FieldInfo*>& fields, IObject* source, IObject* dest, Vector<Actor**>& actorsPointers,
						   Vector<Component**>& componentsPointers, Vector<ISerializable*>& serializableObjects)
	{
		for (auto field : fields)
		{
			if (!field->HasAttribute<SerializableAttribute>())
				continue;

			if (*field->GetType() == TypeOf(Actor*))
			{
				Actor* sourceValue = field->GetValue<Actor*>(source);
				Actor** destValuePtr = (Actor**)(field->GetValuePtrStrong(dest));

				*destValuePtr = sourceValue;
				actorsPointers.Add(destValuePtr);
			}
			else if (*field->GetType() == TypeOf(Component*))
			{
				Component* sourceValue = field->GetValue<Component*>(source);
				Component** destValuePtr = (Component**)(field->GetValuePtrStrong(dest));

				*destValuePtr = sourceValue;
				componentsPointers.Add(destValuePtr);
			}
			else if (field->GetType()->IsBasedOn(TypeOf(AssetRef)))
			{
				field->CopyValue(dest, source);
			}
			else if (field->GetType()->IsBasedOn(TypeOf(IObject)))
			{
				if (field->GetType()->IsBasedOn(TypeOf(ISerializable)))
					serializableObjects.Add((ISerializable*)field->GetValuePtr(dest));

				CopyFields(field->GetType()->GetFieldsWithBaseClasses(),
					(IObject*)field->GetValuePtr(source),
						   (IObject*)field->GetValuePtr(dest),
						   actorsPointers, componentsPointers, serializableObjects);
			}
			else field->CopyValue(dest, source);
		}
	}

	void Actor::CopyChangedFields(Vector<FieldInfo*>& fields, IObject* source, IObject* changed, IObject* dest, Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers, Vector<ISerializable*>& serializableObjects)
	{
		for (auto field : fields)
		{
			if (!field->HasAttribute<SerializableAttribute>())
				continue;

			if (*field->GetType() == TypeOf(Actor*))
			{
				Actor* changedValue = field->GetValue<Actor*>(changed);
				Actor* sourceValue = field->GetValue<Actor*>(source);
				Actor** destValuePtr = (Actor**)(field->GetValuePtrStrong(dest));

				bool valueChanged = false;
				if (changedValue != nullptr)
					valueChanged = changedValue->GetPrototypeLink() != sourceValue;
				else
					valueChanged = sourceValue != nullptr;

				bool destValueSameToSource = false;
				if (*destValuePtr != nullptr)
					destValueSameToSource = (*destValuePtr)->GetPrototypeLink() == sourceValue;
				else
					destValueSameToSource = sourceValue == nullptr;

				if (valueChanged && destValueSameToSource)
				{
					*destValuePtr = changedValue;
					actorsPointers.Add(destValuePtr);
				}
			}
			else if (*field->GetType() == TypeOf(Component*))
			{
				Component* changedValue = field->GetValue<Component*>(changed);
				Component* sourceValue = field->GetValue<Component*>(source);
				Component** destValuePtr = (Component**)(field->GetValuePtrStrong(dest));

				bool valueChanged = false;
				if (changedValue != nullptr)
					valueChanged = changedValue->GetPrototypeLink() != sourceValue;
				else
					valueChanged = sourceValue != nullptr;

				bool destValueSameToSource = false;
				if (*destValuePtr != nullptr)
					destValueSameToSource = (*destValuePtr)->GetPrototypeLink() == sourceValue;
				else
					destValueSameToSource = sourceValue == nullptr;

				if (valueChanged && destValueSameToSource)
				{
					*destValuePtr = changedValue;
					componentsPointers.Add(destValuePtr);
				}
			}
			else if (field->GetType()->IsBasedOn(TypeOf(AssetRef)))
			{
				if (!field->IsValueEquals(changed, source) && field->IsValueEquals(source, dest))
					field->CopyValue(dest, changed);
			}
			else if (field->GetType()->IsBasedOn(TypeOf(IObject)))
			{
				if (field->GetType()->IsBasedOn(TypeOf(ISerializable)))
					serializableObjects.Add((ISerializable*)field->GetValuePtr(dest));

				CopyChangedFields(field->GetType()->GetFieldsWithBaseClasses(),
					(IObject*)field->GetValuePtr(source),
								  (IObject*)field->GetValuePtr(changed),
								  (IObject*)field->GetValuePtr(dest),
								  actorsPointers, componentsPointers, serializableObjects);
			}
			else
			{
				if (!field->IsValueEquals(changed, source) && field->IsValueEquals(source, dest))
					field->CopyValue(dest, changed);
			}
		}
	}

	void Actor::CopyActorChangedFields(Actor* source, Actor* changed, Actor* dest, Vector<Actor*>& allDestChilds,
									   bool withTransform)
	{
		if (changed->mParent && changed->mParent->mPrototypeLink)
		{
			if (!changed->mParent->IsLinkedToActor(source->mParent) && dest->mParent &&
				dest->mParent->IsLinkedToActor(source->mParent))
			{
				Actor* newParent = allDestChilds.FindMatch([&](Actor* x) { return x->IsLinkedToActor(changed->mParent->mPrototypeLink.Get()); });
				dest->SetParent(newParent);
			}
		}

		if (source->mName != changed->mName && dest->mName == source->mName)
			dest->mName = changed->mName;

		if (source->mEnabled != changed->mEnabled && dest->mEnabled == source->mEnabled)
			dest->mEnabled = changed->mEnabled;

		if (source->mLocked != changed->mLocked && dest->mLocked == source->mLocked)
			dest->mLocked = changed->mLocked;

		if (source->mLayer != changed->mLayer && dest->mLayer == source->mLayer)
			dest->SetLayer(changed->mLayer);

		// transform
		if (withTransform)
		{
			if (source->transform->mData->position != changed->transform->mData->position &&
				dest->transform->mData->position == source->transform->mData->position)
			{
				dest->transform->mData->position = changed->transform->mData->position;
			}

			if (source->transform->mData->scale != changed->transform->mData->scale &&
				dest->transform->mData->scale == source->transform->mData->scale)
			{
				dest->transform->mData->scale = changed->transform->mData->scale;
			}

			if (source->transform->mData->size != changed->transform->mData->size &&
				dest->transform->mData->size == source->transform->mData->size)
			{
				dest->transform->mData->size = changed->transform->mData->size;
			}

			if (source->transform->mData->pivot != changed->transform->mData->pivot &&
				dest->transform->mData->pivot == source->transform->mData->pivot)
			{
				dest->transform->mData->pivot = changed->transform->mData->pivot;
			}

			if (!Math::Equals(source->transform->mData->angle, changed->transform->mData->angle) &&
				Math::Equals(dest->transform->mData->angle, source->transform->mData->angle))
			{
				dest->transform->mData->angle = changed->transform->mData->angle;
			}

			if (!Math::Equals(source->transform->mData->shear, changed->transform->mData->shear) &&
				Math::Equals(dest->transform->mData->shear, source->transform->mData->shear))
			{
				dest->transform->mData->shear = changed->transform->mData->shear;
			}
		}
	}

	void Actor::CollectFixingFields(Component* newComponent, Vector<Component **> &componentsPointers,
									Vector<Actor **> &actorsPointers)
	{
		Vector<FieldInfo*> fields;
		GetComponentFields(newComponent, fields);

		for (auto field : fields)
		{
			if (field->GetType()->GetUsage() == Type::Usage::Pointer)
			{
				const PointerType* fieldType = (const PointerType*)field->GetType();

				if (fieldType->GetUnpointedType()->IsBasedOn(TypeOf(Component)))
					componentsPointers.Add((Component**)(field->GetValuePtrStrong(newComponent)));

				if (*fieldType == TypeOf(Actor*))
					actorsPointers.Add((Actor**)(field->GetValuePtrStrong(newComponent)));
			}
		}
	}

	void Actor::GetComponentFields(Component* component, Vector<FieldInfo*>& fields)
	{
		struct helper
		{
			static void GetFields(const Type* type, Vector<FieldInfo*>& fields)
			{
				fields.Add(type->GetFields());

				for (auto baseType : type->GetBaseTypes())
				{
					if (*baseType.type != TypeOf(Component))
						GetFields(baseType.type, fields);
				}
			}
		};

		helper::GetFields(&component->GetType(), fields);
	}

	void Actor::FixComponentFieldsPointers(const Vector<Actor**>& actorsPointers,
										   const Vector<Component**>& componentsPointers,
										   const Dictionary<const Actor*, Actor*>& actorsMap,
										   const Dictionary<const Component*, Component*>& componentsMap)
	{
		for (auto actorPtr : actorsPointers)
		{
			Actor* newActorPtr;
			if (actorsMap.TryGetValue(*actorPtr, newActorPtr))
				*actorPtr = newActorPtr;
		}

		for (auto componentPtr : componentsPointers)
		{
			Component* newComponentPtr;
			if (componentsMap.TryGetValue(*componentPtr, newComponentPtr))
				*componentPtr = newComponentPtr;
		}
	}

#else //IS_EDITOR

	void Actor::OnChanged() {}

	void Actor::OnLockChanged() {}

	void Actor::OnNameChanged() {}

	void Actor::OnChildsChanged() {}

	void Actor::OnParentChanged(Actor* oldParent) {}

#endif //IS_EDITOR

	void Actor::InitializeProperties()
	{
		INITIALIZE_GETTER(Actor, id, GetID);
		INITIALIZE_PROPERTY(Actor, prototype, GetPrototype, SetProtytypeDummy);
		INITIALIZE_PROPERTY(Actor, name, SetName, GetName);
		INITIALIZE_PROPERTY(Actor, enabled, SetEnabled, IsEnabled);
		INITIALIZE_GETTER(Actor, enabledInHierarchy, IsEnabledInHierarchy);
		INITIALIZE_PROPERTY(Actor, locked, SetLocked, IsLocked);
		INITIALIZE_GETTER(Actor, lockedInHierarchy, IsLockedInHierarchy);
		INITIALIZE_PROPERTY(Actor, parent, SetParentProp, GetParent);
		INITIALIZE_GETTER(Actor, children, GetChildren);
		INITIALIZE_ACCESSOR(Actor, child, GetChild);
		INITIALIZE_GETTER(Actor, components, GetComponents);
		INITIALIZE_ACCESSOR(Actor, component, GetComponent);
		INITIALIZE_PROPERTY(Actor, layer, SetLayer, GetLayer);
		INITIALIZE_PROPERTY(Actor, layerName, SetLayer, GetLayerName);

		child.SetAllAccessFunc(this, &Actor::GetAllChilds);
		component.SetAllAccessFunc(this, &Actor::GetAllComponents);
	}

	DECLARE_SINGLETON(ActorDataNodeConverter);

	void ActorDataNodeConverter::ToData(const void* object, DataNode& data)
	{
		const Actor* value = *(const Actor**)object;

		if (value)
		{
			if (value->mIsAsset)
				*data.AddNode("AssetId") = value->GetAssetID();
			else if (value->IsOnScene() || value->IsAsset())
				*data.AddNode("ID") = value->GetID();
			else
				*data.AddNode("Data") = value ? value->Serialize() : (String)"null";
		}
	}

	void ActorDataNodeConverter::FromData(void* object, const DataNode& data)
	{
		Actor*& actor = *(Actor**)object;

		if (auto assetIdNode = data.GetNode("AssetId"))
		{
			UID assetId = *assetIdNode;
			actor = o2Scene.GetAssetActorByID(assetId);

			if (!actor)
				mUnresolvedActors.Add(ActorDef(&actor, assetId));
		}
		else if (auto sceneIdNode = data.GetNode("ID"))
		{
			if (mLockDepth == 0)
				actor = o2Scene.GetActorByID(*sceneIdNode);

			if (!actor)
				mUnresolvedActors.Add(ActorDef(&actor, (UInt64)*sceneIdNode));
		}
		else if (auto dataNode = data.GetNode("Data"))
		{
			if (dataNode->Data() == "null")
				actor = nullptr;
			else
			{
				actor = mnew Actor(ActorCreateMode::NotInScene);
				actor->Deserialize(*dataNode);
			}
		}
		else actor = nullptr;
	}

	bool ActorDataNodeConverter::IsConvertsType(const Type* type) const
	{
		return type->IsBasedOn(*TypeOf(Actor).GetPointerType());
	}

	void ActorDataNodeConverter::LockPointersResolving()
	{
		mLockDepth++;
	}

	void ActorDataNodeConverter::UnlockPointersResolving()
	{
		mLockDepth--;

		if (mLockDepth < 0)
		{
			o2Debug.LogWarning("ActorDataNodeConverter lock depth is less than zero. Something going wrong");
			mLockDepth = 0;
		}
	}

	void ActorDataNodeConverter::ResolvePointers()
	{
		if (mLockDepth > 0)
			return;

		for (auto def : mUnresolvedActors)
		{
			*def.target = mNewActors.FindMatch([&](Actor* x) { return x->GetID() == def.actorId; });

			if (!*def.target)
			{
				if (def.isAsset)
					*def.target = o2Scene.GetAssetActorByID(def.assetId);
				else
					*def.target = o2Scene.GetActorByID(def.actorId);
			}
		}

		mNewActors.Clear();
		mUnresolvedActors.Clear();
	}

	void ActorDataNodeConverter::ActorCreated(Actor* actor)
	{
		if (!IsSingletonInitialzed())
			return;

		if (mInstance->mLockDepth < 1)
			return;

		mInstance->mNewActors.Add(actor);
	}

	ActorRef::ActorRef()
	{}

	ActorRef::ActorRef(Actor* actor):
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

DECLARE_CLASS(o2::Actor);

ENUM_META_(o2::ActorCreateMode, ActorCreateMode)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(InScene);
	ENUM_ENTRY(NotInScene);
}
END_ENUM_META;
