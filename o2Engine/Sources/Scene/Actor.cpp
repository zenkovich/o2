#include "Actor.h"

#include "Scene/Scene.h"
#include "Utils/Math/Basis.h"
#include "Utils/Reflection/Reflection.h"
#include "Utils/Timer.h"

namespace o2
{
	Actor::Actor(ActorTransform* transform, const String& name, bool enabled, bool resEnabled, 
				 bool locked, bool resLocked, Scene::Layer* layer, UInt64 id, UID assetId, bool isOnScene):
		transform(transform), mName(name), mEnabled(enabled), mResEnabled(resEnabled),
		mLocked(locked), mResLocked(resLocked), mLayer(layer), mId(id), mAssetId(assetId), mIsOnScene(isOnScene)
	{

	}


	Actor::Actor(ActorCreateMode mode /*= CreateMode::InScene*/):
		Actor(mnew ActorTransform())
	{
		mIsOnScene = false;

		tags.onTagAdded = [&](Tag* tag) { tag->mActors.Add(this); };
		tags.onTagRemoved = [&](Tag* tag) { tag->mActors.Remove(this); };

		transform->SetOwner(this);
		InitializeProperties();

		if (Scene::IsSingletonInitialzed())
		{
			if (mode == ActorCreateMode::InScene)
			{
				o2Scene.mRootActors.Add(this);
				o2Scene.mAllActors.Add(this);
				mIsOnScene = true;
			}

			mLayer = o2Scene.GetDefaultLayer();
			mLayer->mActors.Add(this);
			mLayer->mEnabledActors.Add(this);

			o2Scene.onActorCreated(this);
		}

		ActorDataNodeConverter::ActorCreated(this);
	}

	Actor::Actor(const Actor& other):
		Actor(mnew ActorTransform(*other.transform), other.mName, other.mEnabled, other.mEnabled, other.mLocked,
			  other.mLocked, other.mLayer, Math::Random(), other.mAssetId, true)
	{
		transform->SetOwner(this);

		if (other.mIsAsset)
			SetPrototype(ActorAssetRef(other.GetAssetId()));

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Dictionary<const Actor*, Actor*> actorsMap;
		Dictionary<const Component*, Component*> componentsMap;

		ProcessCopying(this, &other, actorPointersFields, componentPointersFields, actorsMap, componentsMap, true);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		UpdateEnabled();
		transform->OnChanged();

		InitializeProperties();

		if (Scene::IsSingletonInitialzed())
		{
			o2Scene.mRootActors.Add(this);
			o2Scene.mAllActors.Add(this);
			o2Scene.onActorCreated(this);
		}

		ActorDataNodeConverter::ActorCreated(this);
	}

	Actor::Actor(ComponentsVec components):
		Actor()
	{
		for (auto comp : components)
			AddComponent(comp);
	}

	Actor::Actor(const ActorAssetRef& prototype, ActorCreateMode mode /*= CreateMode::InScene*/):
		Actor(mnew ActorTransform(*prototype->GetActor()->transform), prototype->GetActor()->mName, 
			  prototype->GetActor()->mEnabled, prototype->GetActor()->mEnabled, prototype->GetActor()->mLocked,
			  prototype->GetActor()->mLocked, prototype->GetActor()->mLayer, Math::Random(), 
			  0, mode == ActorCreateMode::InScene)
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
		transform->OnChanged();

		InitializeProperties();

		if (Scene::IsSingletonInitialzed() && mode == ActorCreateMode::InScene)
		{
			o2Scene.mRootActors.Add(this);
			o2Scene.mAllActors.Add(this);
			o2Scene.onActorCreated(this);
		}

		ActorDataNodeConverter::ActorCreated(this);
	}

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

		RemoveAllChilds();
		RemoveAllComponents();

		if (mLayer)
		{
			mLayer->mActors.Remove(this);
			if (mResEnabled)
				mLayer->mEnabledActors.Remove(this);
		}

		if (Scene::IsSingletonInitialzed())
			o2Scene.mChangedActors.Remove(this);
	}

	Actor& Actor::operator=(const Actor& other)
	{
		RemoveAllChilds();
		RemoveAllComponents();

		SetPrototype(other.mPrototype);

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Dictionary<const Actor*, Actor*> actorsMap;
		Dictionary<const Component*, Component*> componentsMap;

		ProcessCopying(this, &other, actorPointersFields, componentPointersFields, actorsMap, componentsMap, false);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		UpdateEnabled();
		transform->OnChanged();

		OnChanged();

		return *this;
	}


	void Actor::Update(float dt)
	{
		for (auto comp : mComponents)
			comp->Update(dt);
	}

	void Actor::UpdateChilds(float dt)
	{
		for (auto child : mChilds)
			child->Update(dt);

		for (auto child : mChilds)
			child->UpdateChilds(dt);
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

		for (auto child : mChilds)
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
		transform->OnChanged();

		OnChanged();
	}

	ActorAssetRef Actor::MakePrototype()
	{
		ActorAssetRef prototypeAsset = ActorAssetRef::CreateAsset();
		Actor* prototype = prototypeAsset->GetActor();
		prototype->ExcludeFromScene();

		prototype->RemoveAllChilds();
		prototype->RemoveAllComponents();

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Dictionary<const Actor*, Actor*> actorsMap;
		Dictionary<const Component*, Component*> componentsMap;

		ProcessPrototypeMaking(prototype, this, actorPointersFields, componentPointersFields, actorsMap, componentsMap, false);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		SetPrototype(prototypeAsset);

		prototype->UpdateEnabled();
		prototype->transform->OnChanged();
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

	void Actor::SetId(UInt64 id)
	{
		mId = id;
	}

	UID Actor::GetAssetId() const
	{
		return mAssetId;
	}

	bool Actor::IsAsset() const
	{
		if (mParent)
			return mIsAsset || mParent->IsAsset();

		return mIsAsset;
	}

	void Actor::GenNewId(bool childs /*= true*/)
	{
		mId = Math::Random();

		if (childs)
		{
			for (auto child : mChilds)
				child->GenNewId(childs);
		}
	}

	void Actor::ExcludeFromScene()
	{
		if (!mIsOnScene)
			return;

		if (mParent && mParent->mIsOnScene)
			SetParent(nullptr);

		if (!Scene::IsSingletonInitialzed())
			return;

		o2Scene.mRootActors.Remove(this);
		o2Scene.mAllActors.Remove(this);
		ComponentsExcludeFromScene();
		mIsOnScene = false;

		for (auto child : mChilds)
			child->ExcludeFromScene();
	}

	void Actor::IncludeInScene()
	{
		if (mIsOnScene)
			return;

		if (!mParent)
			o2Scene.mRootActors.Add(this);

		o2Scene.mAllActors.Add(this);
		mIsOnScene = true;
		ComponentsIncludeToScene();

		for (auto child : mChilds)
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
			int lastIdx = mParent->mChilds.Find(this);
			mParent->mChilds.Insert(this, index);

			if (index <= lastIdx)
				lastIdx++;

			mParent->mChilds.RemoveAt(lastIdx);
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
		if ((actor && actor->mChilds.Contains(this)) || actor == this || actor == mParent)
			return;

		Basis lastParentBasis = transform->GetWorldBasis();
		auto oldParent = mParent;

		if (mParent)
			mParent->RemoveChild(this, false);
		else
			o2Scene.mRootActors.Remove(this);

		mParent = actor;

		if (mParent)
			mParent->mChilds.Add(this);
		else
			o2Scene.mRootActors.Add(this);

		if (worldPositionStays)
			transform->SetWorldBasis(lastParentBasis);
		else
			transform->OnChanged();

		UpdateEnabled();

		OnParentChanged(oldParent);
	}

	Actor* Actor::GetParent() const
	{
		return mParent;
	}

	Actor* Actor::AddChild(Actor* actor)
	{
		if (mChilds.Contains(actor) || actor == this)
			return actor;

		auto oldParent = actor->mParent;

		if (actor->mParent)
			actor->mParent->RemoveChild(actor, false);
		else
			o2Scene.mRootActors.Remove(actor);

		mChilds.Add(actor);
		actor->mParent = this;

		actor->transform->OnChanged();
		actor->UpdateEnabled();

		actor->OnParentChanged(oldParent);

		return actor;
	}

	Actor* Actor::AddChild(Actor* actor, int index)
	{
		if (mChilds.Contains(actor) || actor == this)
			return actor;

		auto oldParent = actor->mParent;

		if (actor->mParent)
			actor->mParent->RemoveChild(actor, false);
		else
			o2Scene.mRootActors.Remove(actor);

		mChilds.Insert(actor, index);
		actor->mParent = this;

		actor->transform->OnChanged();
		actor->UpdateEnabled();

		actor->OnParentChanged(oldParent);

		return actor;
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

		for (auto child : mChilds)
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

	Actor::ActorsVec Actor::GetChilds() const
	{
		return mChilds;
	}

	void Actor::RemoveChild(Actor* actor, bool release /*= true*/)
	{
		if (!mChilds.Contains(actor))
			return;

		auto oldParent = actor->mParent;

		actor->mParent = nullptr;
		mChilds.Remove(actor);

		if (release)
		{
			delete actor;
			OnChildsChanged();
		}
		else
		{
			actor->transform->OnChanged();
			actor->UpdateEnabled();

			actor->OnParentChanged(oldParent);
		}
	}

	void Actor::RemoveAllChilds()
	{
		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			delete child;
		}

		mChilds.Clear();

		OnChildsChanged();
	}

	Component* Actor::AddComponent(Component* component)
	{
// 		if (GetComponent(component->GetType().Name()) != nullptr)
// 			return nullptr;

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

	void Actor::SetLayer(Scene::Layer* layer)
	{
		Scene::Layer* lastLayer = mLayer;

		if (layer == nullptr)
			layer = o2Scene.GetDefaultLayer();

		mLayer = layer;

		if (lastLayer)
			lastLayer->mActors.Remove(this);

		layer->mActors.Add(this);

		if (mResEnabled)
		{
			if (lastLayer)
				lastLayer->mEnabledActors.Remove(this);

			layer->mEnabledActors.Add(this);
		}

		for (auto comp : mComponents)
			comp->OnLayerChanged(lastLayer, layer);

		OnChanged();
	}

	void Actor::SetLayer(const String& layerName)
	{
		SetLayer(o2Scene.GetLayer(layerName));
	}

	Scene::Layer* Actor::GetLayer() const
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

		for (auto child : mChilds)
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

		for (auto child : mChilds)
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

		for (auto child : mChilds)
			if (auto res = child->FindActorById(id))
				return res;

		return nullptr;
	}

	void Actor::SetProtytypeDummy(ActorAssetRef asset)
	{}

	void Actor::OnTransformChanged()
	{
		for (auto comp : mComponents)
			comp->OnTransformChanged();

		for (auto child : mChilds)
			child->transform->OnChanged();

		OnChanged();
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

		for (auto child : mChilds)
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

		for (auto child : mChilds)
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

		node["Transform"] = transform;

		if (mLayer)
			node["LayerName"] = mLayer->name;

		auto childsNode = node.AddNode("Childs");
		for (auto child : mChilds)
			*childsNode->AddNode("Child") = child->Serialize();

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

		if (auto layerNode = node.GetNode("LayerName"))
			SetLayer(layerNode->Data());
		else
			SetLayer(o2Scene.GetDefaultLayer());

		node.GetNode("Transform")->GetValue(*transform);

		if (auto componentsNode = node.GetNode("Components"))
		{
			for (auto componentNode : componentsNode->GetChildNodes())
			{
				Component* component = (Component*)o2Reflection.CreateTypeSample(*componentNode->GetNode("Type"));
				component->Deserialize(*componentNode->GetNode("Data"));
				component->SetOwnerActor(this);
			}
		}

		if (auto childsNode = node.GetNode("Childs"))
		{
			for (auto childNode : childsNode->GetChildNodes())
			{
				Actor* child = mnew Actor(mIsOnScene ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);
				child->Deserialize(*childNode);
				o2Scene.mRootActors.Remove(child);
				child->mParent = this;
				mChilds.Add(child);
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
		for (auto child : mChilds)
			*childsNode->AddNode("Child") = child->Serialize();

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

		RemoveAllChilds();
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

		// children
		if (auto childsNode = node.GetNode("Childs"))
		{
			for (auto childNode : *childsNode)
			{
				Actor* child = mnew Actor(mIsOnScene ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);
				mChilds.Add(child);
				child->mParent = this;

				child->Deserialize(*childNode);
			}
		}

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

		transform->OnChanged();

		ActorDataNodeConverter::Instance().UnlockPointersResolving();
		ActorDataNodeConverter::Instance().ResolvePointers();
	}

	Dictionary<String, Actor*> Actor::GetAllChilds()
	{
		Dictionary<String, Actor*> res;
		for (auto child : mChilds)
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

	void Actor::ComponentsExcludeFromScene()
	{
		for (auto comp : mComponents)
			comp->OnExcludeFromScene();
	}

	void Actor::ComponentsIncludeToScene()
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

	void Actor::SeparateActors(Vector<Actor*>& separatedActors)
	{
		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			separatedActors.Add(child);

			child->SeparateActors(separatedActors);
		}

		mChilds.Clear();
	}

	void Actor::GetAllChildrenActors(Vector<Actor*>& actors)
	{
		actors.Add(mChilds);

		for (auto child : mChilds)
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
						Component* newProtoComponent = protoChild->AddComponent(component->Clone());
						componentsMap.Add(component, newProtoComponent);

						component->mPrototypeLink = newProtoComponent;

						CollectFixingFields(newProtoComponent, componentPointersFields, actorPointersFields);

						for (auto& info : applyActorsInfos)
						{
							if (!info.matchingChild)
								continue;

							Component* newComponent = info.matchingChild->AddComponent(component->Clone());
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
				Component* newProtoComponent = newProtoChild->AddComponent(component->Clone());
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
					Component* newComponent = newChild->AddComponent(component->Clone());
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
		transform->OnChanged();

		for (auto& info : applyActorsInfos)
			info.actor->transform->UpdateTransform();

		OnChanged();

		mPrototype->Save();
	}

	void Actor::ProcessCopying(Actor* dest, const Actor* source, Vector<Actor**>& actorsPointers,
							   Vector<Component**>& componentsPointers, Dictionary<const Actor*, Actor*>& actorsMap,
							   Dictionary<const Component*, Component*>& componentsMap,
							   bool isSourcePrototype)
	{
		dest->mName = source->mName;
		dest->mEnabled = source->mEnabled;
		*dest->transform = *source->transform;
		dest->mAssetId = source->mAssetId;

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

		for (auto child : source->mChilds)
		{
			Actor* newChild = mnew Actor(dest->mIsOnScene ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);
			dest->AddChild(newChild);

			ProcessCopying(newChild, child, actorsPointers, componentsPointers, actorsMap, componentsMap, isSourcePrototype);
		}

		for (auto component : source->mComponents)
		{
			Component* newComponent = dest->AddComponent(component->Clone());

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

		for (auto child : source->mChilds)
		{
			Actor* newChild = mnew Actor(dest->mIsOnScene ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);
			dest->AddChild(newChild);

			ProcessPrototypeMaking(newChild, child, actorsPointers, componentsPointers, actorsMap, componentsMap,
								   source->mPrototype.IsValid() || isInsidePrototype);
		}

		for (auto component : source->mComponents)
		{
			Component* newComponent = dest->AddComponent(component->Clone());
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

		for (auto child : source->mChilds)
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

			Component* newComponent = dest->AddComponent(component->Clone());
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
					if (*baseType != TypeOf(Component))
						GetFields(baseType, fields);
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
		INITIALIZE_GETTER(Actor, childs, GetChilds);
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
				*data.AddNode("AssetId") = value->GetAssetId();
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

	ActorRef::ActorRef(Vector<Component*> components)
	{
		Actor* newActor = mnew Actor(components);
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

CLASS_META(o2::ActorRef)
{
	BASE_CLASS(o2::ISerializable);

	PROTECTED_FIELD(mActor);
	PROTECTED_FIELD(mWasDeleted);

	PUBLIC_FUNCTION(Actor*, Get);
	PUBLIC_FUNCTION(const Actor*, Get);
	PUBLIC_FUNCTION(bool, IsValid);
	PUBLIC_FUNCTION(bool, IsWasDeleted);
}
END_META;

CLASS_META(o2::Actor)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(tags);
	PUBLIC_FIELD(prototype);
	PUBLIC_FIELD(id);
	PUBLIC_FIELD(name);
	PUBLIC_FIELD(enabled);
	PUBLIC_FIELD(enabledInHierarchy);
	PUBLIC_FIELD(locked);
	PUBLIC_FIELD(lockedInHierarchy);
	PUBLIC_FIELD(parent);
	PUBLIC_FIELD(layer);
	PUBLIC_FIELD(layerName);
	PUBLIC_FIELD(childs);
	PUBLIC_FIELD(child);
	PUBLIC_FIELD(components);
	PUBLIC_FIELD(component);
	PUBLIC_FIELD(transform);
	PUBLIC_FIELD(onEnableChanged);
	PROTECTED_FIELD(mPrototype);
	PROTECTED_FIELD(mPrototypeLink);
	PROTECTED_FIELD(mId);
	PROTECTED_FIELD(mName);
	PROTECTED_FIELD(mParent);
	PROTECTED_FIELD(mChilds);
	PROTECTED_FIELD(mComponents);
	PROTECTED_FIELD(mLayer);
	PROTECTED_FIELD(mEnabled);
	PROTECTED_FIELD(mResEnabled);
	PROTECTED_FIELD(mLocked);
	PROTECTED_FIELD(mResLocked);
	PROTECTED_FIELD(mIsOnScene);
	PROTECTED_FIELD(mIsAsset);
	PROTECTED_FIELD(mAssetId);
	PROTECTED_FIELD(mReferences);

	typedef Dictionary<String, Actor*> _tmp1;
	typedef Dictionary<String, Component*> _tmp2;
	typedef Dictionary<const Actor*, Actor*>& _tmp3;
	typedef Dictionary<const Component*, Component*>& _tmp4;
	typedef Dictionary<const Actor*, Actor*>& _tmp5;
	typedef Dictionary<const Component*, Component*>& _tmp6;
	typedef Dictionary<const Actor*, Actor*>& _tmp7;
	typedef Dictionary<const Component*, Component*>& _tmp8;
	typedef const Dictionary<const Actor*, Actor*>& _tmp9;
	typedef const Dictionary<const Component*, Component*>& _tmp10;

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, UpdateChilds, float);
	PUBLIC_FUNCTION(ActorAssetRef, GetPrototype);
	PUBLIC_FUNCTION(ActorAssetRef, GetPrototypeDirectly);
	PUBLIC_FUNCTION(void, BreakPrototypeLink);
	PUBLIC_FUNCTION(void, ApplyChangesToPrototype);
	PUBLIC_FUNCTION(void, RevertToPrototype);
	PUBLIC_FUNCTION(ActorAssetRef, MakePrototype);
	PUBLIC_FUNCTION(ActorRef, GetPrototypeLink);
	PUBLIC_FUNCTION(bool, IsLinkedToActor, Actor*);
	PUBLIC_FUNCTION(bool, IsLinkedToActor, UInt64);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(UInt64, GetID);
	PUBLIC_FUNCTION(void, SetId, UInt64);
	PUBLIC_FUNCTION(UID, GetAssetId);
	PUBLIC_FUNCTION(bool, IsAsset);
	PUBLIC_FUNCTION(void, GenNewId, bool);
	PUBLIC_FUNCTION(void, ExcludeFromScene);
	PUBLIC_FUNCTION(void, IncludeInScene);
	PUBLIC_FUNCTION(bool, IsOnScene);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(void, Enable);
	PUBLIC_FUNCTION(void, Disable);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(bool, IsEnabledInHierarchy);
	PUBLIC_FUNCTION(void, SetLocked, bool);
	PUBLIC_FUNCTION(void, Lock);
	PUBLIC_FUNCTION(void, Unlock);
	PUBLIC_FUNCTION(bool, IsLocked);
	PUBLIC_FUNCTION(bool, IsLockedInHierarchy);
	PUBLIC_FUNCTION(void, SetPositionIndexInParent, int);
	PUBLIC_FUNCTION(void, SetParent, Actor*, bool);
	PUBLIC_FUNCTION(Actor*, GetParent);
	PUBLIC_FUNCTION(Actor*, AddChild, Actor*);
	PUBLIC_FUNCTION(Actor*, AddChild, Actor*, int);
	PUBLIC_FUNCTION(Actor*, GetChild, const String&);
	PUBLIC_FUNCTION(ActorsVec, GetChilds);
	PUBLIC_FUNCTION(void, RemoveChild, Actor*, bool);
	PUBLIC_FUNCTION(void, RemoveAllChilds);
	PUBLIC_FUNCTION(Component*, AddComponent, Component*);
	PUBLIC_FUNCTION(void, RemoveComponent, Component*, bool);
	PUBLIC_FUNCTION(void, RemoveAllComponents);
	PUBLIC_FUNCTION(Component*, GetComponent, const String&);
	PUBLIC_FUNCTION(Component*, GetComponent, const Type*);
	PUBLIC_FUNCTION(Component*, GetComponent, UInt64);
	PUBLIC_FUNCTION(ComponentsVec, GetComponents);
	PUBLIC_FUNCTION(void, SetLayer, Scene::Layer*);
	PUBLIC_FUNCTION(void, SetLayer, const String&);
	PUBLIC_FUNCTION(Scene::Layer*, GetLayer);
	PUBLIC_FUNCTION(String, GetLayerName);
	PUBLIC_FUNCTION(Actor*, FindLinkedActor, Actor*);
	PUBLIC_FUNCTION(Actor*, FindLinkedActor, UInt64);
	PUBLIC_FUNCTION(Actor*, FindActorById, UInt64);
	PROTECTED_FUNCTION(void, SetProtytypeDummy, ActorAssetRef);
	PROTECTED_FUNCTION(void, OnTransformChanged);
	PROTECTED_FUNCTION(void, SetParentProp, Actor*);
	PROTECTED_FUNCTION(void, SetPrototype, ActorAssetRef);
	PROTECTED_FUNCTION(void, UpdateEnabled);
	PROTECTED_FUNCTION(void, UpdateLocking);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, SerializeRaw, DataNode&);
	PROTECTED_FUNCTION(void, DeserializeRaw, const DataNode&);
	PROTECTED_FUNCTION(void, SerializeWithProto, DataNode&);
	PROTECTED_FUNCTION(void, DeserializeWithProto, const DataNode&);
	PROTECTED_FUNCTION(_tmp1, GetAllChilds);
	PROTECTED_FUNCTION(_tmp2, GetAllComponents);
	PROTECTED_FUNCTION(void, ComponentsExcludeFromScene);
	PROTECTED_FUNCTION(void, ComponentsIncludeToScene);
	PROTECTED_FUNCTION(void, OnParentChanged, Actor*);
	PROTECTED_FUNCTION(void, SeparateActors, Vector<Actor*>&);
	PROTECTED_FUNCTION(void, GetAllChildrenActors, Vector<Actor*>&);
	PROTECTED_FUNCTION(void, InitializeProperties);
	PROTECTED_FUNCTION(void, ProcessCopying, Actor*, const Actor*, Vector<Actor**>&, Vector<Component**>&, _tmp3, _tmp4, bool);
	PROTECTED_FUNCTION(void, ProcessPrototypeMaking, Actor*, Actor*, Vector<Actor**>&, Vector<Component**>&, _tmp5, _tmp6, bool);
	PROTECTED_FUNCTION(void, CopyFields, Vector<FieldInfo*>&, IObject*, IObject*, Vector<Actor**>&, Vector<Component**>&, Vector<ISerializable*>&);
	PROTECTED_FUNCTION(void, CopyChangedFields, Vector<FieldInfo*>&, IObject*, IObject*, IObject*, Vector<Actor**>&, Vector<Component**>&, Vector<ISerializable*>&);
	PROTECTED_FUNCTION(void, CopyActorChangedFields, Actor*, Actor*, Actor*, Vector<Actor*>&, bool);
	PROTECTED_FUNCTION(void, CollectFixingFields, Component*, Vector<Component**>&, Vector<Actor**>&);
	PROTECTED_FUNCTION(void, GetComponentFields, Component*, Vector<FieldInfo*>&);
	PROTECTED_FUNCTION(void, ProcessReverting, Actor*, const Actor*, const Vector<Actor*>&, Vector<Actor**>&, Vector<Component**>&, _tmp7, _tmp8, Vector<ISerializable*>&);
	PROTECTED_FUNCTION(void, FixComponentFieldsPointers, const Vector<Actor**>&, const Vector<Component**>&, _tmp9, _tmp10);
}
END_META;

ENUM_META_(o2::ActorCreateMode, ActorCreateMode)
{
	ENUM_ENTRY(InScene);
	ENUM_ENTRY(NotInScene);
}
END_ENUM_META;
