#include "Actor.h"

#include "Utils/Math/Basis.h"

namespace o2
{
	IOBJECT_CPP(Actor);

	Actor::Actor():
		mName("unnamed"), mEnabled(true), mResEnabled(true), Animatable()
	{
		InitializeProperties();
	}

	Actor::Actor(const Actor& other):
		mName(other.mName), mEnabled(other.mEnabled), mResEnabled(other.mEnabled), Animatable(other)
	{
		for (auto child : other.mChilds)
			AddChild(child->Clone());

		for (auto component : other.mCompontents)
			AddComponent(component->Clone());

		UpdateEnabled();
		transform.UpdateTransform();

		InitializeProperties();
	}

	Actor::~Actor()
	{
		if (mParent)
			mParent->RemoveChild(Ptr<Actor>(this), false);

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

	void Actor::SetEnabled(bool active)
	{
		if (mEnabled == active)
			return;

		mEnabled = active;
		UpdateEnabled();
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

	void Actor::SetParent(Ptr<Actor> actor, bool worldPositionStays /*= true*/)
	{
		if (actor->mChilds.Contains(this))
			return;

		Basis lastParentBasis = transform.GetWorldBasis();

		if (mParent)
			mParent->RemoveChild(Ptr<Actor>(this), false);

		mParent = actor;

		if (mParent)
			mParent->mChilds.Add(this);

		if (worldPositionStays)
			transform.SetWorldBasis(lastParentBasis);
		else
			transform.UpdateTransform();

		UpdateEnabled();
	}

	Ptr<Actor> Actor::GetParent() const
	{
		return mParent;
	}

	Ptr<Actor> Actor::AddChild(Ptr<Actor> actor)
	{
		if (mChilds.Contains(actor))
			return actor;

		if (actor->mParent)
			actor->mParent->RemoveChild(actor, false);

		mChilds.Add(actor);
		actor->mParent = this;

		actor->transform.UpdateTransform();
		actor->UpdateEnabled();

		return actor;
	}

	Ptr<Actor> Actor::GetChild(const String& path) const
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

	void Actor::RemoveChild(Ptr<Actor> actor, bool release /*= true*/)
	{
		if (!mChilds.Contains(actor))
			return;

		actor->mParent = nullptr;
		mChilds.Remove(actor);

		if (release)
			actor.Release();
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
			child.Release();
		}

		mChilds.Clear();
	}

	Ptr<Component> Actor::AddComponent(Ptr<Component> component)
	{
		if (GetComponent(component->GetType().Name()) != nullptr)
			return nullptr;

		mCompontents.Add(component);
		component->mOwner = this;
		return component;
	}

	void Actor::RemoveComponent(Ptr<Component> component, bool release /*= true*/)
	{
		mCompontents.Remove(component);
		component->mOwner = nullptr;

		if (release)
			component.Release();
	}

	void Actor::RemoveAllComponents()
	{
		for (auto comp : mCompontents)
		{
			comp->mOwner = nullptr;
			comp.Release();
		}

		mCompontents.Clear();
	}

	Ptr<Component> Actor::GetComponent(const String& typeName)
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

	void Actor::SetParentProp(Ptr<Actor> actor)
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

	Dictionary<String, Ptr<Actor>> Actor::GetAllChilds()
	{
		Dictionary<String, Ptr<Actor>> res;
		for (auto child : mChilds)
			res.Add(child->GetName(), child);

		return res;
	}

	Dictionary<String, Ptr<Component>> Actor::GetAllComponents()
	{
		Dictionary<String, Ptr<Component>> res;
		for (auto child : mCompontents)
			res.Add(child->GetType().Name(), child);

		return res;
	}

	void Actor::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Actor, name, SetName, GetName);
		INITIALIZE_PROPERTY(Actor, enabled, SetEnabled, IsEnabled);
		INITIALIZE_GETTER(Actor, enabledInHierarchy, IsEnabledInHierarchy);
		INITIALIZE_PROPERTY(Actor, parent, SetParentProp, GetParent);
		INITIALIZE_GETTER(Actor, childs, GetChilds);
		INITIALIZE_ACCESSOR(Actor, child, GetChild);
		INITIALIZE_GETTER(Actor, components, GetComponents);
		INITIALIZE_ACCESSOR(Actor, component, GetComponent);

		child.SetAllAccessFunc(this, &Actor::GetAllChilds);
		component.SetAllAccessFunc(this, &Actor::GetAllComponents);
	}
}