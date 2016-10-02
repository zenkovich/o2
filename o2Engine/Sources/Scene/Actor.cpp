#include "Actor.h"

#include "Scene/Scene.h"
#include "Utils/Math/Basis.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	Actor::Actor(CreateMode mode /*= CreateMode::InScene*/):
		mName("unnamed"), mEnabled(true), mResEnabled(true), mLocked(false), mResLocked(false), Animatable(),
		mParent(nullptr), mLayer(nullptr), mId(Math::Random()), mAssetId(0), mIsAsset(false), mIsOnScene(false)
	{
		tags.onTagAdded = [&](Tag* tag) { tag->mActors.Add(this); };
		tags.onTagRemoved = [&](Tag* tag) { tag->mActors.Remove(this); };

		transform.SetOwner(this);
		InitializeProperties();

		if (mode == CreateMode::InScene && Scene::IsSingletonInitialzed())
		{
			o2Scene.mRootActors.Add(this);
			o2Scene.mAllActors.Add(this);
			mLayer = o2Scene.GetDefaultLayer();
			mLayer->mActors.Add(this);
			mLayer->mEnabledActors.Add(this);

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

			mLayer->mActors.Add(this);
			mLayer->mEnabledActors.Add(this);

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

	Actor::~Actor()
	{
		if (mParent)
			mParent->RemoveChild(this, false);
		else
		{
			if (Scene::IsSingletonInitialzed())
				o2Scene.mRootActors.Remove(this);
		}

		if (Scene::IsSingletonInitialzed() && mIsOnScene)
		{
			o2Scene.onActorDestroying(this);
			o2Scene.mAllActors.Remove(this);
		}

		RemoveAllChilds();
		RemoveAllComponents();

		if (mLayer)
		{
			mLayer->mActors.Remove(this);
			if (mResEnabled)
				mLayer->mEnabledActors.Remove(this);
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

		OnChanged();

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
			mLayer->mActors.Remove(this);

			if (mResEnabled)
				mLayer->mEnabledActors.Remove(this);
		}

		ComponentsExcludeFromScene();

		mIsOnScene = false;
	}

	void Actor::IncludeInScene()
	{
		if (mLayer)
		{
			mLayer->mActors.Add(this);

			if (mResEnabled)
				mLayer->mEnabledActors.Add(this);
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
			mParent->mChilds.Remove(this);
			mParent->mChilds.Insert(this, index);
			mParent->OnChildsChanged();
		}
		else
		{
			o2Scene.mRootActors.Remove(this);
			o2Scene.mRootActors.Insert(this, index);
		}
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

		actor->transform.UpdateTransform();
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

		actor->transform.UpdateTransform();
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
			actor->transform.UpdateTransform();
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
		if (GetComponent(component->GetType().Name()) != nullptr)
			return nullptr;

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

	void Actor::OnTransformChanged()
	{
		for (auto comp : mComponents)
			comp->OnTransformChanged();

		for (auto child : mChilds)
			child->transform.UpdateTransform();

		OnChanged();
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
		SetLayer(layerName);

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

 
CLASS_META(o2::Actor)
{
	BASE_CLASS(o2::Animatable);

	PUBLIC_FIELD(tags);
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
	PUBLIC_FIELD(transform).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(onEnableChanged);
	PROTECTED_FIELD(mId).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mName).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mParent);
	PROTECTED_FIELD(mChilds);
	PROTECTED_FIELD(mComponents);
	PROTECTED_FIELD(mLayer);
	PROTECTED_FIELD(mEnabled).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mResEnabled);
	PROTECTED_FIELD(mLocked).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mResLocked);
	PROTECTED_FIELD(mIsOnScene);
	PROTECTED_FIELD(mIsAsset);
	PROTECTED_FIELD(mAssetId);

	typedef Dictionary<const Actor*, Actor*>& _tmp1;
	typedef Dictionary<const Component*, Component*>& _tmp2;
	typedef const Dictionary<const Actor*, Actor*>& _tmp3;
	typedef const Dictionary<const Component*, Component*>& _tmp4;
	typedef Dictionary<String, Actor*> _tmp5;
	typedef Dictionary<String, Component*> _tmp6;

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, UpdateChilds, float);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(UInt64, GetID);
	PUBLIC_FUNCTION(void, SetId, UInt64);
	PUBLIC_FUNCTION(AssetId, GetAssetId);
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
	PROTECTED_FUNCTION(void, OnTransformChanged);
	PROTECTED_FUNCTION(void, ProcessCopying, Actor*, const Actor*, Vector<Actor**>&, Vector<Component**>&, _tmp1, _tmp2);
	PROTECTED_FUNCTION(void, FixComponentFieldsPointers, const Vector<Actor**>&, const Vector<Component**>&, _tmp3, _tmp4);
	PROTECTED_FUNCTION(void, SetParentProp, Actor*);
	PROTECTED_FUNCTION(void, UpdateEnabled);
	PROTECTED_FUNCTION(void, UpdateLocking);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(_tmp5, GetAllChilds);
	PROTECTED_FUNCTION(_tmp6, GetAllComponents);
	PROTECTED_FUNCTION(void, ComponentsExcludeFromScene);
	PROTECTED_FUNCTION(void, ComponentsIncludeToScene);
	PROTECTED_FUNCTION(void, OnChanged);
	PROTECTED_FUNCTION(void, OnLockChanged);
	PROTECTED_FUNCTION(void, OnNameChanged);
	PROTECTED_FUNCTION(void, OnChildsChanged);
	PROTECTED_FUNCTION(void, OnParentChanged, Actor*);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

ENUM_META(o2::Actor::CreateMode)
{
	ENUM_ENTRY(InScene);
	ENUM_ENTRY(NotInScene);
}
END_ENUM_META;
 