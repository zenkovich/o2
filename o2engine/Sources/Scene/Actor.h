#pragma once

#include "Animation/Animatable.h"
#include "Scene/ActorTransform.h"
#include "Scene/Component.h"
#include "Utils/Containers/Vector.h"
#include "Utils/String.h"

namespace o2
{
	// -----------
	// Scene actor
	// -----------
	class Actor: public Animatable
	{
	public:
		typedef Vector<Ptr<Actor>> ActorsVec;
		typedef Vector<Ptr<Component>> ComponentsVec;

	public:
		Property<String>                        name;               // Actor name property
		Property<bool>                          enabled;            // Is actor enabled property
		Getter<bool>                            enabledInHierarchy; // Is actor enabled in hierarchy getter
		Property<Ptr<Actor>>                    parent;             // Parent actor property
		Getter<ActorsVec>                       childs;             // Children array getter
		Accessor<Ptr<Actor>, const String&>     child;              // Children accessor
		Getter<ComponentsVec>                   components;         // Components array getter
		Accessor<Ptr<Component>, const String&> component;          // Component accessor by type name
		ActorTransform                          transform;          // Transformation

		// Default constructor
		Actor();

		// Copy-constructor
		Actor(const Actor& other);

		// Destructor
		~Actor();

		// Assign operator
		Actor& operator=(const Actor& other);

		// Updates actor and components
		void Update(float dt);

		// Updates childs
		void UpdateChilds(float dt);

		// Sets actor's name
		void SetName(const String& name);

		// Returns name
		String GetName() const;

		// Sets actor enabling
		void SetEnabled(bool active);

		// Enables actor
		void Enable();

		// Disables actor
		void Disable();

		// Returns is actor enabled
		bool IsEnabled() const;

		// Returns is actor enabled in hierarchy
		bool IsEnabledInHierarchy() const;

		// Sets parent
		void SetParent(Ptr<Actor> actor, bool worldPositionStays = true);

		// Returns parent
		Ptr<Actor> GetParent() const;

		// Add child actor
		Ptr<Actor> AddChild(Ptr<Actor> actor);

		// Returns child actor by path (ex "root/some node/other node/target node")
		Ptr<Actor> GetChild(const String& path) const;

		// Returns children array
		ActorsVec GetChilds() const;

		// Removes child and destroys him if needed
		void RemoveChild(Ptr<Actor> actor, bool release = true);

		// Removes and destroys all childs
		void RemoveAllChilds();

		// And new component
		template<typename _type>
		Ptr<_type> AddComponent();

		// Adds new component
		Ptr<Component> AddComponent(Ptr<Component> component);

		// Removes component
		void RemoveComponent(Ptr<Component> component, bool release = true);

		// Removes all components
		void RemoveAllComponents();

		// Returns component with type name
		Ptr<Component> GetComponent(const String& typeName);

		// Returns component with type
		template<typename _type>
		Ptr<_type> GetComponent() const;

		// Returns component with type in this and children
		template<typename _type>
		Ptr<_type> GetComponentInChildren() const;

		// Return all components by type
		template<typename _type>
		Vector<Ptr<_type>> GetComponents() const;

		// Returns all components by type in this and children
		template<typename _type>
		Vector<Ptr<_type>> GetComponentsInChildren() const;

		// Returns all components
		ComponentsVec GetComponents() const;

		SERIALIZABLE_IMPL(Actor);

		IOBJECT(Actor)
		{
			BASE_CLASS(Animatable);

			FIELD(name);
			FIELD(enabled);
			FIELD(enabledInHierarchy);
			FIELD(childs);
			FIELD(child);
			FIELD(components);

			SRLZ_FIELD(transform);
			SRLZ_FIELD(mName);
			SRLZ_FIELD(mChilds);
			SRLZ_FIELD(mCompontents);
			SRLZ_FIELD(mEnabled);

			FIELD(mResEnabled);
		}

	protected:
		String         mName;        // Name of actor

		Ptr<Actor>     mParent;      // Parent actor
		ActorsVec      mChilds;      // Children actors
		ComponentsVec  mCompontents; // Components vector
					   
		bool           mEnabled;     // Is actor enabled
		bool           mResEnabled;  // Is actor enabled in hierarchy

	protected:
		// Calls when transformation was changed
		void OnTransformChanged();

		// Sets parent
		void SetParentProp(Ptr<Actor> actor);

		// Updates enabling
		void UpdateEnabled();

		// Returns dictionary of all children by names
		Dictionary<String, Ptr<Actor>> GetAllChilds();

		// Returns dictionary of all components by type names
		Dictionary<String, Ptr<Component>> GetAllComponents();

		// Initializes properties
		void InitializeProperties();

		friend class ActorTransform;
		friend class Component;
	};

	template<typename _type>
	Vector<Ptr<_type>> Actor::GetComponentsInChildren() const
	{
		Vector<Ptr<_type>> res = GetComponents<_type>();

		for (auto child : mChilds)
			res.Add(child->GetComponentsInChildren<_type>());

		return res;
	}

	template<typename _type>
	Ptr<_type> Actor::GetComponentInChildren() const
	{
		Ptr<_type> res = GetComponent<_type>();

		if (res)
			return res;

		for (auto child : mChilds)
		{
			res = child->GetComponentInChildren<_type>();
			if (res)
				return res;
		}

		return res;
	}

	template<typename _type>
	Ptr<_type> Actor::GetComponent() const
	{
		for (auto comp : mCompontents)
		{
			if (comp->GetTypeId() == _type::type->ID())
				return comp;
		}

		return nullptr;
	}

	template<typename _type>
	Vector<Ptr<_type>> Actor::GetComponents() const
	{
		Vector<Ptr<_type>> res;
		for (auto comp : mCompontents)
		{
			if (comp->GetTypeId() == _type::type->ID())
				res.Add(comp);
		}

		return res;
	}

	template<typename _type>
	Ptr<_type> Actor::AddComponent()
	{
		if (GetComponent<_type>() != nullptr)
			return nullptr;

		Ptr<_type> newComponent = mnew _type();
		AddComponent(newComponent);
		return newComponent;
	}

}