#include "Actor.h"

#include "Scene/Scene.h"
#include "Utils/Math/Basis.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	Actor::Actor():
		mName("unnamed"), mEnabled(true), mResEnabled(true), mLocked(false), mResLocked(false), Animatable(),
		mParent(nullptr), mLayer(nullptr), mId(Math::Random()), mAssetId(0), mIsAsset(false), mIsOnScene(false)
	{
		tags.onTagAdded = [&](Tag* tag) { tag->mActors.Add(this); };
		tags.onTagRemoved = [&](Tag* tag) { tag->mActors.Remove(this); };

		transform.SetOwner(this);
		InitializeProperties();

		if (Scene::IsSingletonInitialzed())
		{
			o2Scene.mRootActors.Add(this);
			o2Scene.mAllActors.Add(this);
			mLayer = o2Scene.GetDefaultLayer();
			mLayer->actors.Add(this);
			mLayer->enabledActors.Add(this);

			o2Scene.onActorCreated(this);

			mIsOnScene = true;
		}

		ActorDataNodeConverter::ActorCreated(this);
	}

	Actor::Actor(const Actor& other):
		mName(other.mName), mEnabled(other.mEnabled), mResEnabled(other.mEnabled), mLocked(other.mLocked),
		mResLocked(other.mResLocked), Animatable(other), transform(other.transform), mParent(nullptr),
		mLayer(other.mLayer), mId(Math::Random()), mAssetId(other.mAssetId), mIsAsset(false), mIsOnScene(false)
	{
		transform.SetOwner(this);

		for (auto child : other.mChilds)
			AddChild(child->Clone());

		for (auto component : other.mComponents)
			AddComponent(component->Clone());

		UpdateEnabled();
		transform.UpdateTransform();

		InitializeProperties();

		if (Scene::IsSingletonInitialzed())
		{
			o2Scene.mRootActors.Add(this);
			o2Scene.mAllActors.Add(this);

			if (!mLayer)
				mLayer = o2Scene.GetDefaultLayer();

			mLayer->actors.Add(this);
			mLayer->enabledActors.Add(this);

			o2Scene.onActorCreated(this);

			mIsOnScene = true;
		}

		ActorDataNodeConverter::ActorCreated(this);
	}

	Actor::Actor(ComponentsVec components):
		Actor()
	{
		for (auto comp : components)
			AddComponent(comp);
	}

	Actor::Actor(CreateMode mode):
		mName("unnamed"), mEnabled(true), mResEnabled(true), mLocked(false), mResLocked(false), Animatable(),
		mParent(nullptr), mLayer(nullptr), mId(Math::Random()), mAssetId(0), mIsAsset(false), mIsOnScene(false)
	{
		tags.onTagAdded = [&](Tag* tag) { tag->mActors.Add(this); };
		tags.onTagRemoved = [&](Tag* tag) { tag->mActors.Remove(this); };

		transform.SetOwner(this);
		InitializeProperties();
	}

	Actor::~Actor()
	{
		if (mParent)
			mParent->RemoveChild(this, false);
		else
		{
			if (Scene::IsSingletonInitialzed())
				o2Scene.mRootActors.Remove(this);
		}

		if (Scene::IsSingletonInitialzed())
		{
			o2Scene.onActorDestroying(this);
			o2Scene.mAllActors.Remove(this);
		}

		RemoveAllChilds();
		RemoveAllComponents();

		if (mLayer)
		{
			mLayer->actors.Remove(this);
			if (mResEnabled)
				mLayer->enabledActors.Remove(this);
		}
	}

	Actor& Actor::operator=(const Actor& other)
	{
		RemoveAllChilds();
		RemoveAllComponents();

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Dictionary<const Actor*, Actor*> actorsMap;
		Dictionary<const Component*, Component*> componentsMap;

		ProcessCopying(this, &other, actorPointersFields, componentPointersFields, actorsMap, componentsMap);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		UpdateEnabled();
		transform.UpdateTransform();

		ACTOR_CHANGED(this);

		return *this;
	}

	void Actor::ProcessCopying(Actor* dest, const Actor* source, Vector<Actor**>& actorsPointers, 
							   Vector<Component**>& componentsPointers, Dictionary<const Actor*, Actor*>& actorsMap,
							   Dictionary<const Component*, Component*>& componentsMap)
	{
		dest->Animatable::operator=(*source);

		dest->mName = source->mName;
		dest->mEnabled = source->mEnabled;
		dest->transform = source->transform;
		dest->mAssetId = source->mAssetId;

		actorsMap.Add(source, dest);

		for (auto child : source->mChilds)
		{
			Actor* newChild = mnew Actor();
			dest->AddChild(newChild);

			ProcessCopying(newChild, child, actorsPointers, componentsPointers, actorsMap, componentsMap);
		}

		for (auto component : source->mComponents)
		{
			Component* newComponent = dest->AddComponent(component->Clone());

			componentsMap.Add(component, newComponent);

			for (auto field : newComponent->GetType().Fields())
			{
				if (field->GetType().IsBasedOn(TypeOf(Component)))
					componentsPointers.Add(field->GetValuePtrStrong<Component*>(newComponent));

				if (field->GetType() == TypeOf(Actor))
					actorsPointers.Add(field->GetValuePtrStrong<Actor*>(newComponent));
			}
		}

		dest->SetLayer(source->mLayer);
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

	void Actor::Update(float dt)
	{
		Animatable::Update(dt);

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

	void Actor::SetName(const String& name)
	{
		mName = name;

		ACTOR_NAME_CHANGED(this);
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

	AssetId Actor::GetAssetId() const
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
		if (mParent)
			SetParent(nullptr);

		if (!Scene::IsSingletonInitialzed())
			return;

		o2Scene.mRootActors.Remove(this);

		if (mLayer)
		{
			mLayer->actors.Remove(this);

			if (mResEnabled)
				mLayer->enabledActors.Remove(this);
		}

		ComponentsExcludeFromScene();

		mIsOnScene = false;
	}

	void Actor::IncludeInScene()
	{
		if (mLayer)
		{
			mLayer->actors.Add(this);

			if (mResEnabled)
				mLayer->enabledActors.Add(this);
		}

		ComponentsIncludeToScene();

		mIsOnScene = true;
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

		ACTOR_CHANGED(this);
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

		ACTOR_LOCK_CHANGED(this);
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
			mParent->mChilds.Remove(this);
			mParent->mChilds.Insert(this, index);
		}
		else
		{
			o2Scene.mRootActors.Remove(this);
			o2Scene.mRootActors.Insert(this, index);
		}

		ACTOR_HIERARCHY_CHANGED(mParent);
	}

	void Actor::SetParent(Actor* actor, bool worldPositionStays /*= true*/)
	{
		if ((actor && actor->mChilds.Contains(this)) || actor == this)
			return;

		Basis lastParentBasis = transform.GetWorldBasis();
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
			transform.SetWorldBasis(lastParentBasis);
		else 
			transform.UpdateTransform();

		UpdateEnabled();

		ACTOR_PARENT_CHANGED(this, oldParent);
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

		actor->transform.UpdateTransform();
		actor->UpdateEnabled();

		ACTOR_PARENT_CHANGED(actor, oldParent);

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

		actor->transform.UpdateTransform();
		actor->UpdateEnabled();

		ACTOR_PARENT_CHANGED(actor, oldParent);

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
			ACTOR_HIERARCHY_CHANGED(this);
		}
		else
		{
			actor->transform.UpdateTransform();
			actor->UpdateEnabled();

			ACTOR_PARENT_CHANGED(actor, oldParent);
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
		ACTOR_HIERARCHY_CHANGED(this);
	}

	Component* Actor::AddComponent(Component* component)
	{
		if (GetComponent(component->GetType().Name()) != nullptr)
			return nullptr;

		component->SetOwnerActor(this);
		ACTOR_CHANGED(this);
		return component;
	}

	void Actor::RemoveComponent(Component* component, bool release /*= true*/)
	{
		mComponents.Remove(component);
		component->mOwner = nullptr;

		ACTOR_CHANGED(this);

		if (release)
			delete component;
	}

	void Actor::RemoveAllComponents()
	{
		auto components = mComponents;
		for (auto comp : components)
			delete comp;

		mComponents.Clear();
		ACTOR_CHANGED(this);
	}

	Component* Actor::GetComponent(const String& typeName)
	{
		for (auto comp : mComponents)
			if (comp->GetType().Name() == typeName)
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
			lastLayer->actors.Remove(this);

		layer->actors.Add(this);

		if (mResEnabled)
		{
			if (lastLayer)
				lastLayer->enabledActors.Remove(this);

			layer->enabledActors.Add(this);
		}

		for (auto comp : mComponents)
			comp->OnLayerChanged(lastLayer, layer);

		ACTOR_CHANGED(this);
	}

	void Actor::SetLayerName(const String& layerName)
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

	void Actor::OnTransformChanged()
	{
		for (auto comp : mComponents)
			comp->OnTransformChanged();

		for (auto child : mChilds)
			child->transform.UpdateTransform();

		ACTOR_CHANGED(this);
	}

	void Actor::SetParentProp(Actor* actor)
	{
		SetParent(actor, false);
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
				mLayer->enabledActors.Add(this);
			else
				mLayer->enabledActors.Remove(this);

			o2Scene.onActorEnableChanged(this);
			ACTOR_CHANGED(this);
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
			ACTOR_LOCK_CHANGED(this);

		for (auto child : mChilds)
			child->UpdateLocking();
	}

	void Actor::OnSerialize(DataNode& node) const
	{
		if (mLayer)
			*node["mLayerName"] = mLayer->name;

		auto childsNode = node.AddNode("Childs");
		for (auto child : mChilds)
			*childsNode->AddNode("Child") = child->Serialize();

		auto componentsNode = node.AddNode("Components");
		for (auto comp : mComponents)
		{
			auto compNode = componentsNode->AddNode("Component");
			*compNode->AddNode("Data") = comp->Serialize();
			*compNode->AddNode("Type") = comp->GetType().Name();
		}
	}

	void Actor::OnDeserialized(const DataNode& node)
	{
		ActorDataNodeConverter::Instance().LockPointersResolving();

		if (auto childsNode = node.GetNode("Components"))
		{
			for (auto childNode : childsNode->GetChildNodes())
			{
				Component* comp = (Component*)o2Reflection.CreateTypeSample(*childNode->GetNode("Type"));
				comp->Deserialize(*childNode->GetNode("Data"));
				comp->mOwner = this;
				mComponents.Add(comp);
			}
		}

		if (auto childsNode = node.GetNode("Childs"))
		{
			for (auto childNode : childsNode->GetChildNodes())
			{
				Actor* child = mnew Actor();
				child->Deserialize(*childNode);
				o2Scene.mRootActors.Remove(child);
				child->mParent = this;
				mChilds.Add(child);
			}
		}

		String layerName = (String)(*node.GetNode("mLayerName"));
		SetLayerName(layerName);

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
			res.Add(child->GetType().Name(), child);

		return res;
	}

	void Actor::ComponentsExcludeFromScene()
	{
		for (auto comp : mComponents)
			comp->OnExcludeFromScene();

		for (auto child : mChilds)
			child->ComponentsExcludeFromScene();
	}

	void Actor::ComponentsIncludeToScene()
	{
		for (auto comp : mComponents)
			comp->OnIncludeToScene();

		for (auto child : mChilds)
			child->ComponentsIncludeToScene();
	}

	void Actor::InitializeProperties()
	{
		INITIALIZE_GETTER(Actor, id, GetID);
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
		INITIALIZE_PROPERTY(Actor, layerName, SetLayerName, GetLayerName);

		child.SetAllAccessFunc(this, &Actor::GetAllChilds);
		component.SetAllAccessFunc(this, &Actor::GetAllComponents);
	}

	DECLARE_SINGLETON(ActorDataNodeConverter);

	void ActorDataNodeConverter::ToData(void* object, DataNode& data)
	{
		Actor* value = *(Actor**)object;

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
		Actor* actor = *(Actor**)object;

		if (auto assetIdNode = data.GetNode("AssetId"))
		{
			AssetId assetId = *assetIdNode;
			actor = o2Scene.GetAssetActorByID(assetId);
			
			if (!actor)
				mUnresolvedActors.Add(ActorDef(&actor, true, assetId));
		}
		else if (auto sceneIdNode = data.GetNode("ID"))
		{
			actor = o2Scene.GetActorByID(*sceneIdNode);
			
			if (!actor)
				mUnresolvedActors.Add(ActorDef(&actor, false, *sceneIdNode));
		}
		else if (auto dataNode = data.GetNode("Data"))
		{
			if (dataNode->Data() == "null")
				actor = nullptr;
			else
			{
				actor = mnew Actor();
				actor->ExcludeFromScene();
				actor->Deserialize(*dataNode);
			}
		}
	}

	bool ActorDataNodeConverter::CheckType(const Type* type) const
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
			if (def.isAsset)
				*def.target = o2Scene.GetAssetActorByID(def.id);
			else
				*def.target = o2Scene.GetActorByID(def.id);

			if (!*def.target)
				*def.target = mNewActors.FindMatch([&](Actor* x) { return x->GetID() == def.id; });
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

}