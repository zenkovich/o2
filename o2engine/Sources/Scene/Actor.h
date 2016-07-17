#pragma once

#include "Animation/Animatable.h"
#include "Scene/ActorTransform.h"
#include "Scene/Component.h"
#include "Scene/Scene.h"
#include "Scene/Tags.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Singleton.h"
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

		enum class CreateMode { InScene, NotInScene };

	public:
		TagGroup                            tags;                    // Tags group
		Getter<UInt64>                      id;                      // Actor's unique id
		Property<String>                    name;                    // Actor name property
		Property<bool>                      enabled;                 // Is actor enabled property
		Getter<bool>                        enabledInHierarchy;      // Is actor enabled in hierarchy getter
		Property<bool>                      locked;                  // Is actor locked property
		Getter<bool>                        lockedInHierarchy;       // Is actor locked in hierarchy getter
		Property<Actor*>                    parent;                  // Parent actor property
		Property<Scene::Layer*>             layer;                   // Layer property
		Property<String>                    layerName;               // Layer name property
		Getter<ActorsVec>                   childs;                  // Children array getter
		Accessor<Actor*, const String&>     child;                   // Children accessor
		Getter<ComponentsVec>               components;              // Components array getter
		Accessor<Component*, const String&> component;               // Component accessor by type name
		ActorTransform                      transform;               // Transformation @SERIALIZABLE
		Function<void(bool)>                onEnableChanged;         // Enable changing event

#if IS_EDITOR
		Function<void()>                    onChanged;               // Something in actor change event
		Function<void(Actor*)>              onParentChanged;         // Actor reparent event
		Function<void()>                    onChildHierarchyChanged; // Actor childs hierarchy change event
		Function<void(bool)>                onLockChanged;           // Locking changing event
		Function<void()>                    onNameChanged;           // Name changing event
#endif
		
		// Default constructor
		Actor();

		// Create mode
		Actor(CreateMode mode);

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

		// Returns actor's unique id
		UInt64 GetID() const;

		// Sets id. Be carefully! Ids must be unique! Don't recommending to change this
		void SetId(UInt64 id);

		// Returns asset id
		AssetId GetAssetId() const;

		// Is this from asset
		bool IsAsset() const;

		// Generates new random id 
		void GenNewId(bool childs = true);

		// Excludes from scene and will not be update and draw automatically from scene
		void ExcludeFromScene();

		// Includes to scene and now will be update and draw automatically from scene
		void IncludeInScene();

		// Is actor on scene
		bool IsOnScene() const;

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
		Component* GetComponent(const Type* type);

		// Returns component by id
		Component* GetComponent(UInt64 id);

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

		SERIALIZABLE(Actor);

	protected:
		UInt64         mId;          // Unique actor id @SERIALIZABLE
		String         mName;        // Name of actor @SERIALIZABLE

		Actor*         mParent;      // Parent actor
		ActorsVec      mChilds;      // Children actors 
		ComponentsVec  mComponents; // Components vector 
		Scene::Layer*  mLayer;       // Scene layer

		bool           mEnabled;     // Is actor enabled @SERIALIZABLE
		bool           mResEnabled;  // Is actor enabled in hierarchy

		bool           mLocked;      // Is actor locked @SERIALIZABLE
		bool           mResLocked;   // Is actor locked in hierarchy

		bool           mIsOnScene;   // Is actor on scene

		bool           mIsAsset;     // Is this actor cached asset
		AssetId        mAssetId;     // Source asset id

	protected:
		// Calls when transformation was changed
		void OnTransformChanged();

		// Processes copying actor
		void ProcessCopying(Actor* dest, const Actor* source, 
							Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers, 
							Dictionary<const Actor*, Actor*>& actorsMap,
							Dictionary<const Component*, Component*>& componentsMap);

		// Fixes actors and components pointers by actors map
		void FixComponentFieldsPointers(const Vector<Actor**>& actorsPointers,
										const Vector<Component**>& componentsPointers,
										const Dictionary<const Actor*, Actor*>& actorsMap,
										const Dictionary<const Component*, Component*>& componentsMap);

		// Sets parent
		void SetParentProp(Actor* actor);

		// Updates enabling
		void UpdateEnabled();

		// Updates locking
		void UpdateLocking();

		// Beginning serialization callback
		void OnSerialize(DataNode& node) const;

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Returns dictionary of all children by names
		Dictionary<String, Actor*> GetAllChilds();

		// Returns dictionary of all components by type names
		Dictionary<String, Component*> GetAllComponents();

		// Applies excluding from scene for all components in hierarchy
		void ComponentsExcludeFromScene();

		// Applies including to scene for all components in hierarchy
		void ComponentsIncludeToScene();

		// Initializes properties
		void InitializeProperties();

		friend class ActorAsset;
		friend class ActorDataNodeConverter;
		friend class ActorTransform;
		friend class Component;
		friend class DrawableComponent;
		friend class Scene;
		friend class Tag;
	};

	// -------------------------
	// Actor data node converter
	// -------------------------
	class ActorDataNodeConverter: public IDataNodeTypeConverter, public Singleton<ActorDataNodeConverter>
	{
	public:
		// Converts actor pointer to data 
		void ToData(void* object, DataNode& data);

		// Gets actor pointer from data
		void FromData(void* object, const DataNode& data);

		// Checks that type is based on Actor's type
		bool CheckType(const Type* type) const;

	protected:
		struct ActorDef
		{
			Actor** target;
			bool    isAsset;
			UInt64  id;

			ActorDef() {}
			
			ActorDef(Actor** target, bool isAsset, UInt64 id):
				target(target), isAsset(isAsset), id(id)
			{}

			bool operator==(const ActorDef& other) const
			{
				return target == other.target;
			}
		};

		typedef Vector<ActorDef> ActorDefsVec;
		typedef Vector<Actor*> ActorsVec;

	protected:
		ActorDefsVec mUnresolvedActors; 
		ActorsVec    mNewActors;
		int          mLockDepth = 0;

	protected:
		// Locks pointers resolving depth
		void LockPointersResolving();

		// Unlocks pointers resolving depth
		void UnlockPointersResolving();

		// Resolves actors' pointers. Works when lock depth is 0
		void ResolvePointers();

		// Calls when new actor was created
		static void ActorCreated(Actor* actor);

		friend class Actor;
		friend class Scene;
	};

	template<typename _type>
	Vector<_type>* Actor::GetComponentsInChildren() const
	{
		Vector < _type >> res = GetComponents < _type*();

		for (auto child : mChilds)
			res.Add(child->GetComponentsInChildren<_type>());

		return res;
	}

	template<typename _type>
	_type* Actor::GetComponentInChildren() const
	{
		_type > res = GetComponent < _type*();

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
		for (auto comp : mComponents)
		{
			if (comp->GetType().IsBasedOn(TypeOf(_type)))
				return comp;
		}

		return nullptr;
	}

	template<typename _type>
	Vector<_type>* Actor::GetComponents() const
	{
		Vector<_type>* res;
		for (auto comp : mComponents)
		{
			if (comp->GetType().IsBasedOn(TypeOf(_type)))
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

#define ACTOR_CHANGED(ACTOR)            \
    if (ACTOR) (ACTOR)->onChanged();    \
	if (Scene::IsSingletonInitialzed()) \
	    o2Scene.OnActorChanged(ACTOR);

#define ACTOR_LOCK_CHANGED(ACTOR)                  \
    if (ACTOR)                                     \
	{                                              \
		(ACTOR)->onLockChanged((ACTOR)->mLocked); \
		(ACTOR)->onChanged();                      \
    }                                              \
	if (Scene::IsSingletonInitialzed())            \
    {                                              \
	    o2Scene.OnActorChanged(ACTOR);             \
        o2Scene.onActorLockChanged(ACTOR);         \
    }

#define ACTOR_NAME_CHANGED(ACTOR)          \
    if (ACTOR)                             \
	{                                      \
		(ACTOR)->onNameChanged();          \
		(ACTOR)->onChanged();              \
    }                                      \
	if (Scene::IsSingletonInitialzed())    \
    {                                      \
	    o2Scene.OnActorChanged(ACTOR);     \
        o2Scene.onActorNameChanged(ACTOR); \
    }

#define ACTOR_HIERARCHY_CHANGED(ACTOR)                \
    if (ACTOR)                                        \
	{                                                 \
		(ACTOR)->onChildHierarchyChanged();           \
		(ACTOR)->onChanged();                         \
    }                                                 \
	if (Scene::IsSingletonInitialzed())               \
    {                                                 \
	    o2Scene.OnActorChanged(ACTOR);                \
        o2Scene.onActorChildsHierarchyChanged(ACTOR); \
    }

#define ACTOR_PARENT_CHANGED(ACTOR, OLD_PARENT)                      \
    if (ACTOR)                                                       \
	{                                                                \
		(ACTOR)->onParentChanged(OLD_PARENT);                        \
		(ACTOR)->onChanged();                                        \
    }                                                                \
	if (Scene::IsSingletonInitialzed())                              \
	{                                                                \
	    o2Scene.OnActorChanged(ACTOR);                               \
	    o2Scene.OnActorChanged(OLD_PARENT);                          \
	    o2Scene.onActorChildsHierarchyChanged(OLD_PARENT);           \
	    if (ACTOR)                                                   \
        {                                                            \
	        o2Scene.OnActorChanged((ACTOR)->mParent);                \
	        o2Scene.onActorChildsHierarchyChanged((ACTOR)->mParent); \
        }                                                            \
    }

#define COMPONENT_CHANGED(COMPONENT) 		             \
    if ((COMPONENT)->mOwner)                             \
    { 										             \
        (COMPONENT)->mOwner->onChanged();                \
    	if (Scene::IsSingletonInitialzed())              \
    	    o2Scene.OnActorChanged((COMPONENT)->mOwner); \
    }		

#else
#define ACTOR_CHANGED(ACTOR)
#define COMPONENT_CHANGED(COMPONENT)
#define ACTOR_ENABLE_CHANGED(ACTOR)
#define ACTOR_NAME_CHANGED(ACTOR)
#define ACTOR_PARENT_CHANGED(ACTOR, OLD_PARENT)
#define ACTOR_HIERARCHY_CHANGED(ACTOR) 
#endif

	}