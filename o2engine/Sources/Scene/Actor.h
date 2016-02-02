#pragma once

#include "Animation/Animatable.h"
#include "Scene/ActorTransform.h"
#include "Scene/Component.h"
#include "Scene/Scene.h"
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
		typedef Vector<Actor*> ActorsVec;
		typedef Vector<Component*> ComponentsVec;
		typedef Vector<String> StringsVec;

	public:
		Property<String>                    name;               // Actor name property
		Property<bool>                      enabled;            // Is actor enabled property
		Getter<bool>                        enabledInHierarchy; // Is actor enabled in hierarchy getter
		Property<bool>                      locked;             // Is actor locked property
		Getter<bool>                        lockedInHierarchy;  // Is actor locked in hierarchy getter
		Property<Actor*>                    parent;             // Parent actor property
		Property<Scene::Layer*>             layer;              // Layer property
		Property<String>                    layerName;          // Layer name property
		Getter<ActorsVec>                   childs;             // Children array getter
		Accessor<Actor*, const String&>     child;              // Children accessor
		Getter<ComponentsVec>               components;         // Components array getter
		Accessor<Component*, const String&> component;          // Component accessor by type name
		Accessor<bool, const String&>       tag;                // Tag existing accessor
		ActorTransform                      transform;          // Transformation @SERIALIZABLE
		Function<void(bool)>                onEnableChanged;    // Enable changing event
		Function<void(bool)>                onLockChanged;      // Locking changing event

#if IS_EDITOR
		Function<void()>                    onChanged;          // Calls when something in actor was changed
#endif

		// Default constructor
		Actor();

		// Constructor with components
		Actor(ComponentsVec components);

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

		// Excludes from scene and will not be update and draw automatically from scene
		void ExcludeFromScene();

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

		// Sets locking
		void SetLocked(bool locked);

		// Locks actor
		void Lock();

		// Unlocks actor
		void Unlock();

		// Returns is actor locked
		bool IsLocked() const;

		// Returns is actor locked in hierarchy
		bool IsLockedInHierarchy() const;

		// Sets index position in parent or scene
		void SetPositionIndexInParent(int index);

		// Sets parent
		void SetParent(Actor* actor, bool worldPositionStays = true);

		// Returns parent
		Actor* GetParent() const;

		// Add child actor
		Actor* AddChild(Actor* actor);

		// Add child actor
		Actor* AddChild(Actor* actor, int index);

		// Returns child actor by path (ex "root/some node/other node/target node")
		Actor* GetChild(const String& path) const;

		// Returns children array
		ActorsVec GetChilds() const;

		// Removes child and destroys him if needed
		void RemoveChild(Actor* actor, bool release = true);

		// Removes and destroys all childs
		void RemoveAllChilds();

		// And new component
		template<typename _type>
		_type* AddComponent();

		// Adds new component
		Component* AddComponent(Component* component);

		// Removes component
		void RemoveComponent(Component* component, bool release = true);

		// Removes all components
		void RemoveAllComponents();

		// Returns component with type name
		Component* GetComponent(const String& typeName);

		// Returns component with type
		template<typename _type>
		_type* GetComponent() const;

		// Returns component with type in this and children
		template<typename _type>
		_type* GetComponentInChildren() const;

		// Return all components by type
		template<typename _type>
		Vector<_type>* GetComponents() const;

		// Returns all components by type in this and children
		template<typename _type>
		Vector<_type>* GetComponentsInChildren() const;

		// Returns all components
		ComponentsVec GetComponents() const;

		// Sets layer
		void SetLayer(Scene::Layer* layer);

		// Sets layer by name
		void SetLayerName(const String& layerName);

		// Returns layer
		Scene::Layer* GetLayer() const;

		// Returns layer name
		String GetLayerName() const;

		// Adds tag
		void AddTag(const String& tag);

		// Removes tag
		void RemoveTag(const String& tag);

		// Returns is actor have tag
		bool IsHaveTag(const String& tag) const;

		// Removes all tags
		void ClearTags();

		// Returns tags array
		const StringsVec& GetTags() const;

		SERIALIZABLE(Actor);

	protected:
		String         mName;        // Name of actor @SERIALIZABLE

		Actor*         mParent;      // Parent actor
		ActorsVec      mChilds;      // Children actors @SERIALIZABLE
		ComponentsVec  mCompontents; // Components vector @SERIALIZABLE
		Scene::Layer*  mLayer;       // Scene layer
		StringsVec     mTags;        // Tags
					   
		bool           mEnabled;     // Is actor enabled @SERIALIZABLE
		bool           mResEnabled;  // Is actor enabled in hierarchy

		bool           mLocked;      // Is actor locked @SERIALIZABLE
		bool           mResLocked;   // Is actor locked in hierarchy

	protected:
		// Calls when transformation was changed
		void OnTransformChanged();

		// Sets parent
		void SetParentProp(Actor* actor);

		// Updates enabling
		void UpdateEnabled();

		// Updates locking
		void UpdateLocking();

		// Beginning serialization callback
		void OnSerialize(DataNode& node);

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Returns dictionary of all children by names
		Dictionary<String, Actor*> GetAllChilds();

		// Returns dictionary of all components by type names
		Dictionary<String, Component*> GetAllComponents();

#if IS_EDITOR
		// Calls when something in actor was changed
		void OnChanged();
#endif

		// Initializes properties
		void InitializeProperties();

		friend class ActorTransform;
		friend class Component;
		friend class Scene;
		friend class DrawableComponent;
	};

	template<typename _type>
	Vector<_type>* Actor::GetComponentsInChildren() const
	{
		Vector<_type>> res = GetComponents<_type*();

		for (auto child : mChilds)
			res.Add(child->GetComponentsInChildren<_type>());

		return res;
	}

	template<typename _type>
	_type* Actor::GetComponentInChildren() const
	{
		_type> res = GetComponent<_type*();

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
	_type* Actor::GetComponent() const
	{
		for (auto comp : mCompontents)
		{
			if (comp->GetType() == *_type::type)
				return comp;
		}

		return nullptr;
	}

	template<typename _type>
	Vector<_type>* Actor::GetComponents() const
	{
		Vector<_type>* res;
		for (auto comp : mCompontents)
		{
			if (comp->GetType() == *_type::type)
				res.Add(comp);
		}

		return res;
	}

	template<typename _type>
	_type* Actor::AddComponent()
	{
		if (GetComponent<_type>() != nullptr)
			return nullptr;

		_type* newComponent = mnew _type();
		AddComponent(newComponent);
		return newComponent;
	}


#if IS_EDITOR
#define ACTOR_CHANGED(ACTOR) (ACTOR)->OnChanged()
#define COMPONENT_CHANGED(COMPONENT) if ((COMPONENT)->mOwner) (COMPONENT)->mOwner->OnChanged();
#else
#define ACTOR_CHANGED(ACTOR)
#define COMPONENT_CHANGED(COMPONENT)
#endif

}