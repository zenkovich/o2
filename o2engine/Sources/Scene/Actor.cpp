#include "Actor.h"

#include "Scene/Scene.h"
#include "Utils/Math/Basis.h"

namespace o2
{
	Actor::Actor():
		mName("unnamed"), mEnabled(true), mResEnabled(true), mLocked(false), mResLocked(false), Animatable(), 
		mParent(nullptr)
	{
		transform.SetOwner(this);
		InitializeProperties();

		if (Scene::IsSingletonInitialzed())
			o2Scene.mActors.Add(this);
	}

	Actor::Actor(const Actor& other):
		mName(other.mName), mEnabled(other.mEnabled), mResEnabled(other.mEnabled), mLocked(other.mLocked), 
		mResLocked(other.mResLocked), Animatable(other), 
		transform(other.transform), mParent(nullptr)
	{
		transform.SetOwner(this);

		for (auto child : other.mChilds)
			AddChild(child->Clone());

		for (auto component : other.mCompontents)
			AddComponent(component->Clone());

		UpdateEnabled();
		transform.UpdateTransform();

		InitializeProperties();

		o2Scene.mActors.Add(this);
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
			o2Scene.mActors.Remove(this);

		RemoveAllChilds();
		RemoveAllComponents();
	}

	Actor& Actor::operator=(const Actor& other)
	{
		RemoveAllChilds();
		RemoveAllComponents();

		Animatable::operator=(other);

		mName = other.mName;
		mEnabled = other.mEnabled;
		transform = other.transform;

		for (auto child : other.mChilds)
			AddChild(child->Clone());

		for (auto component : other.mCompontents)
			AddComponent(component->Clone());

		UpdateEnabled();
		transform.UpdateTransform();

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
	}

	String Actor::GetName() const
	{
		return mName;
	}

	void Actor::ExcludeFromScene()
	{
		if (mParent)
			SetParent(nullptr);

		o2Scene.mActors.Remove(this);
	}

	void Actor::SetEnabled(bool active)
	{
		if (mEnabled == active)
			return;

		mEnabled = active;
		UpdateEnabled();

		onEnableChanged(mEnabled);
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

		onLockChanged(mLocked);
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
			o2Scene.mActors.Remove(this);
			o2Scene.mActors.Insert(this, index);
		}
	}

	void Actor::SetParent(Actor* actor, bool worldPositionStays /*= true*/)
	{
		if ((actor && actor->mChilds.Contains(this)) || actor == this)
			return;

		Basis lastParentBasis = transform.GetWorldBasis();

		if (mParent)
			mParent->RemoveChild(this, false);
		else
			o2Scene.mActors.Remove(this);

		mParent = actor;

		if (mParent)
			mParent->mChilds.Add(this);
		else
			o2Scene.mActors.Add(this);

		if (worldPositionStays)
			transform.SetWorldBasis(lastParentBasis);
		else
			transform.UpdateTransform();

		UpdateEnabled();
	}

	Actor* Actor::GetParent() const
	{
		return mParent;
	}

	Actor* Actor::AddChild(Actor* actor)
	{
		if (mChilds.Contains(actor) || actor == this)
			return actor;

		if (actor->mParent)
			actor->mParent->RemoveChild(actor, false);
		else
			o2Scene.mActors.Remove(actor);

		mChilds.Add(actor);
		actor->mParent = this;

		actor->transform.UpdateTransform();
		actor->UpdateEnabled();

		return actor;
	}

	Actor* Actor::AddChild(Actor* actor, int index)
	{
		if (mChilds.Contains(actor) || actor == this)
			return actor;

		if (actor->mParent)
			actor->mParent->RemoveChild(actor, false);
		else
			o2Scene.mActors.Remove(actor);

		mChilds.Insert(actor, index);
		actor->mParent = this;

		actor->transform.UpdateTransform();
		actor->UpdateEnabled();

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

		actor->mParent = nullptr;
		mChilds.Remove(actor);

		if (release)
			delete actor;
		else
		{
			actor->transform.UpdateTransform();
			actor->UpdateEnabled();
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
	}

	Component* Actor::AddComponent(Component* component)
	{
		if (GetComponent(component->GetType().Name()) != nullptr)
			return nullptr;

		component->SetOwnerActor(this);
		return component;
	}

	void Actor::RemoveComponent(Component* component, bool release /*= true*/)
	{
		mCompontents.Remove(component);
		component->mOwner = nullptr;

		if (release)
			delete component;
	}

	void Actor::RemoveAllComponents()
	{
		for (auto comp : mCompontents)
		{
			comp->mOwner = nullptr;
			delete comp;
		}

		mCompontents.Clear();
	}

	Component* Actor::GetComponent(const String& typeName)
	{
		for (auto comp : mCompontents)
			if (comp->GetType().Name() == typeName)
				return comp;

		return nullptr;
	}

	Actor::ComponentsVec Actor::GetComponents() const
	{
		return mCompontents;
	}

	void Actor::OnTransformChanged()
	{
		for (auto comp : mCompontents)
			comp->OnTransformChanged();

		for (auto child : mChilds)
			child->transform.UpdateTransform();
	}

	void Actor::SetParentProp(Actor* actor)
	{
		SetParent(actor, false);
	}

	void Actor::UpdateEnabled()
	{
		if (mParent)
			mResEnabled = mEnabled && mParent->mResEnabled;
		else
			mResEnabled = mEnabled;

		for (auto comp : mCompontents)
			comp->UpdateEnabled();

		for (auto child : mChilds)
			child->UpdateEnabled();
	}

	void Actor::UpdateLocking()
	{
		if (mParent)
			mResLocked = mLocked || mParent->mResLocked;
		else
			mResLocked = mLocked;

		for (auto child : mChilds)
			child->UpdateLocking();
	}

	void Actor::OnDeserialized(const DataNode& node)
	{
		for (auto child : mChilds)
		{
			o2Scene.mActors.Remove(child);
			child->mParent = this;
		}
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

	void Actor::InitializeProperties()
	{
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

		child.SetAllAccessFunc(this, &Actor::GetAllChilds);
		component.SetAllAccessFunc(this, &Actor::GetAllComponents);
	}
}