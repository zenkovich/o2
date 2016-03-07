#include "Actor.h"

#include "Scene/Scene.h"
#include "Utils/Math/Basis.h"

namespace o2
{
	Actor::Actor():
		mName("unnamed"), mEnabled(true), mResEnabled(true), mLocked(false), mResLocked(false), Animatable(),
		mParent(nullptr), mLayer(nullptr), mId(Math::Random()), mAssetId(0), mIsAsset(false), mIsOnScene(false)
	{
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
	}

	Actor::Actor(const Actor& other):
		mName(other.mName), mEnabled(other.mEnabled), mResEnabled(other.mEnabled), mLocked(other.mLocked),
		mResLocked(other.mResLocked), Animatable(other), transform(other.transform), mParent(nullptr),
		mLayer(other.mLayer), mId(Math::Random()), mAssetId(other.mAssetId), mIsAsset(false), mIsOnScene(false)
	{
		transform.SetOwner(this);

		for (auto child : other.mChilds)
			AddChild(child->Clone());

		for (auto component : other.mCompontents)
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

		Animatable::operator=(other);

		mName = other.mName;
		mEnabled = other.mEnabled;
		transform = other.transform;
		mAssetId = other.mAssetId;

		for (auto child : other.mChilds)
			AddChild(child->Clone());

		for (auto component : other.mCompontents)
			AddComponent(component->Clone());

		UpdateEnabled();
		transform.UpdateTransform();

		SetLayer(other.mLayer);

		ACTOR_CHANGED(this);

		return *this;
	}

	void Actor::Update(float dt)
	{
		Animatable::Update(dt);

		for (auto comp : mCompontents)
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

	UInt64 Actor::GetId() const
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
		mCompontents.Remove(component);
		component->mOwner = nullptr;

		ACTOR_CHANGED(this);

		if (release)
			delete component;
	}

	void Actor::RemoveAllComponents()
	{
		auto components = mCompontents;
		for (auto comp : components)
			delete comp;

		mCompontents.Clear();
		ACTOR_CHANGED(this);
	}

	Component* Actor::GetComponent(const String& typeName)
	{
		for (auto comp : mCompontents)
			if (comp->GetType().Name() == typeName)
				return comp;

		return nullptr;
	}

	Component* Actor::GetComponent(const Type* type)
	{
		for (auto comp : mCompontents)
			if (&comp->GetType()== type)
				return comp;

		return nullptr;
	}

	Actor::ComponentsVec Actor::GetComponents() const
	{
		return mCompontents;
	}

	void Actor::SetLayer(Scene::Layer* layer)
	{
		Scene::Layer* lastLayer = mLayer;
		if (layer == nullptr)
			layer = o2Scene.GetDefaultLayer();

		mLayer = layer;

		lastLayer->actors.Remove(this);
		layer->actors.Add(this);

		if (mResEnabled)
		{
			lastLayer->enabledActors.Remove(this);
			layer->enabledActors.Add(this);
		}

		for (auto comp : mCompontents)
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

	void Actor::AddTag(const String& tag)
	{
		if (!mTags.Contains(tag))
			mTags.Add(tag);

		ACTOR_CHANGED(this);
	}

	void Actor::RemoveTag(const String& tag)
	{
		mTags.Remove(tag);
		ACTOR_CHANGED(this);
	}

	bool Actor::IsHaveTag(const String& tag) const
	{
		return mTags.Contains(tag);
	}

	void Actor::ClearTags()
	{
		mTags.Clear();
		ACTOR_CHANGED(this);
	}

	const Actor::StringsVec& Actor::GetTags() const
	{
		return mTags;
	}

	void Actor::OnTransformChanged()
	{
		for (auto comp : mCompontents)
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

			ACTOR_CHANGED(this);
		}

		for (auto comp : mCompontents)
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
			ACTOR_CHANGED(this);

		for (auto child : mChilds)
			child->UpdateLocking();
	}

	void Actor::OnSerialize(DataNode& node)
	{
		*node["mLayerName"] = mLayer->name;
	}

	void Actor::OnDeserialized(const DataNode& node)
	{
		for (auto comp : mCompontents)
			comp->mOwner = this;

		for (auto child : mChilds)
		{
			o2Scene.mRootActors.Remove(child);
			child->mParent = this;
		}

		String layerName = (String)(*node.GetNode("mLayerName"));
		SetLayerName(layerName);
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
		for (auto child : mCompontents)
			res.Add(child->GetType().Name(), child);

		return res;
	}

	void Actor::ComponentsExcludeFromScene()
	{
		for (auto comp : mCompontents)
			comp->OnExcludeFromScene();

		for (auto child : mChilds)
			child->ComponentsExcludeFromScene();
	}

	void Actor::ComponentsIncludeToScene()
	{
		for (auto comp : mCompontents)
			comp->OnIncludeToScene();

		for (auto child : mChilds)
			child->ComponentsIncludeToScene();
	}

	void Actor::InitializeProperties()
	{
		INITIALIZE_GETTER(Actor, id, GetId);
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
		INITIALIZE_ACCESSOR(Actor, tag, IsHaveTag);

		child.SetAllAccessFunc(this, &Actor::GetAllChilds);
		component.SetAllAccessFunc(this, &Actor::GetAllComponents);
	}

}