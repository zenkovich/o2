#pragma once

#include "Assets/ActorAsset.h"
#include "Scene/ActorTransform.h"
#include "Scene/Scene.h"
#include "Scene/Tags.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Singleton.h"
#include "Utils/String.h"
#include "Utils/UID.h"

namespace o2
{
	class Component;
	class Actor;

	enum class ActorCreateMode { InScene, NotInScene };

	// --------------------------------------------------------------
	// Actor reference, automatically invalidates when actor deleting
	// --------------------------------------------------------------
	class ActorRef: public ISerializable
	{
	public:
		// Default constructor, nothing to ref
		ActorRef();

		// Constructor for referencing on actor
		ActorRef(Actor* actor);

		// Creates actor by prototype and returns reference to it
		ActorRef(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::InScene);

		// Creates actor with components and returns reference to it
		ActorRef(Vector<Component*> components, ActorCreateMode mode = ActorCreateMode::InScene);

		// Creates copy actor and returns reference to it
		ActorRef(const Actor& other);

		// Destructor
		~ActorRef();

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Assign operator
		ActorRef& operator=(const ActorRef& other);

		// Getter operator
		Actor& operator*();

		// Constant getter operator
		const Actor& operator*() const;

		// Actor members access operator
		Actor* operator->();

		// Constant actor members access operator
		const Actor* operator->() const;

		// Check equals operator
		bool operator==(const ActorRef& other) const;

		// Check not equals operator
		bool operator!=(const ActorRef& other) const;

		// Returns actor pointer
		Actor* Get();

		// Returns actor pointer
		const Actor* Get() const;

		// Returns is reference is valid
		bool IsValid() const;

		// Returns is actor was deleted
		bool IsWasDeleted() const;

		SERIALIZABLE(ActorRef);

	protected:
		Actor* mActor = nullptr;
		bool   mWasDeleted = false;

		friend class Actor;
	};

	// -----------
	// Scene actor
	// -----------
	class Actor: virtual public ISerializable
	{
	public:
		typedef Vector<Actor*> ActorsVec;
		typedef Vector<Component*> ComponentsVec;
		typedef Vector<String> StringsVec;
		
	public:
		Property<ActorAssetRef> prototype;          // Prototype asset reference property
										            
		ActorTransform* const   transform;          // Transformation 
										            
		Property<Actor*>        parent;             // Parent actor property
										            
		Getter<UInt64>          id;                 // Actor's unique id
		Property<String>        name;               // Actor name property
										            
		TagGroup                tags;               // Tags group
										            
		Property<SceneLayer*>   layer;              // Layer property
		Property<String>        layerName;          // Layer name property

		Property<bool>          enabled;            // Is actor enabled property
		Getter<bool>            enabledInHierarchy; // Is actor enabled in hierarchy getter

		Property<bool>          locked;             // Is actor locked property
		Getter<bool>            lockedInHierarchy;  // Is actor locked in hierarchy getter

		Getter<ActorsVec>       children;           // Children array getter
		Getter<ComponentsVec>   components;         // Components array getter

		Accessor<Actor*, const String&>     child;      // Children accessor
		Accessor<Component*, const String&> component;  // Component accessor by type name


		Function<void(bool)> onEnableChanged; // Enable changing event

	public:
		// Default constructor
		Actor(ActorCreateMode mode = ActorCreateMode::InScene);

		// Actor constructor from prototype
		Actor(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::InScene);

		// Constructor with components
		Actor(ComponentsVec components, ActorCreateMode mode = ActorCreateMode::InScene);

		// Copy-constructor
		Actor(const Actor& other);

		// Destructor
		virtual ~Actor();

		// Assign operator
		Actor& operator=(const Actor& other);

		// Updates actor and components
		virtual void Update(float dt);

		// Updates childs
		virtual void UpdateChilds(float dt);

		// Returns prototype from this or this parent
		ActorAssetRef GetPrototype() const;

		// Returns prototype directly from only this
		ActorAssetRef GetPrototypeDirectly() const;

		// Breaks link to prototype, sets actor as actor without prototype
		void BreakPrototypeLink();

		// Applies all changes to prototype and saves it
		void ApplyChangesToPrototype();

		// Reverts all properties to prototype
		void RevertToPrototype();

		// Makes prototype asset from this actor and links this to new asset
		ActorAssetRef MakePrototype();

		// Returns prototype link pointer
		ActorRef GetPrototypeLink() const;

		// Returns is this linked to specified actor with depth links search
		bool IsLinkedToActor(Actor* actor) const;

		// Returns is this linked to specified actor id with depth links search
		bool IsLinkedToActor(UInt64 id) const;

		// Sets actor's name
		void SetName(const String& name);

		// Returns name
		String GetName() const;

		// Returns actor's unique id
		UInt64 GetID() const;

		// Sets id. Be carefully! Ids must be unique! Don't recommending to change this
		void SetID(UInt64 id);

		// Generates new random id 
		void GenerateNewID(bool childs = true);

		// Returns asset id
		UID GetAssetID() const;

		// Is this from asset
		bool IsAsset() const;

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

		// Searches child with specified type
		template<typename _type, typename _check = std::enable_if<std::is_base_of<Actor, _type>::value>::value>
		_type* GetChildByType();

		// Returns children array
		ActorsVec GetChildren() const;

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
		void SetLayer(SceneLayer* layer);

		// Sets layer by name
		void SetLayer(const String& layerName);

		// Returns layer
		SceneLayer* GetLayer() const;

		// Returns layer name
		String GetLayerName() const;

		// Searches actor in this, what linked to linkActor
		Actor* FindLinkedActor(Actor* linkActor);

		// Searches actor in this, what linked to actor with id
		Actor* FindLinkedActor(UInt64 id);

		// Searches actor with id in this and this children
		Actor* FindActorById(UInt64 id);

		SERIALIZABLE(Actor);

#if IS_EDITOR
		Function<void()>       onChanged;               // Something in actor change event
		Function<void(Actor*)> onParentChanged;         // Actor reparent event
		Function<void()>       onChildHierarchyChanged; // Actor childs hierarchy change event
		Function<void(bool)>   onLockChanged;           // Locking changing event
		Function<void()>       onNameChanged;           // Name changing event

        // It is called when some changed in actor
		void OnChanged();

		// It is called when actor's locking was changed
		void OnLockChanged();

		// It is called when actor's name was changed
		void OnNameChanged();

		// It is called when child changed
		void OnChildsChanged();
#endif

	protected:
		typedef Vector<ActorRef*> ActorRefsVec;

		ActorAssetRef   mPrototype;               // Prototype asset
		ActorRef        mPrototypeLink = nullptr; // Prototype link actor. Links to source actor from prototype

		UInt64          mId;                      // Unique actor id
		String          mName;                    // Name of actor

		SceneLayer*     mLayer = nullptr;         // Scene layer

		Actor*          mParent = nullptr;        // Parent actor
		ActorsVec       mChildren;                // Children actors 

		ComponentsVec   mComponents;              // Components vector 

		bool            mEnabled = true;          // Is actor enabled
		bool            mResEnabled = true;       // Is actor enabled in hierarchy

		bool            mLocked = false;          // Is actor locked
		bool            mResLocked = false;       // Is actor locked in hierarchy

		bool            mIsOnScene = true;        // Is actor on scene

		bool            mIsAsset = false;         // Is this actor cached asset
		UID             mAssetId;                 // Source asset id

		ActorRefsVec    mReferences;              // References to this actor

	protected:
		// Base actor constructor with transform
		Actor(ActorTransform* transform, bool isOnScene = true, const String& name = "unnamed", bool enabled = true,
			  bool resEnabled = true, bool locked = false, bool resLocked = false, 
			  SceneLayer* layer = nullptr, UInt64 id = Math::Random(), UID assetId = 0);

		// Default constructor with transform
		Actor(ActorTransform* transform, ActorCreateMode mode = ActorCreateMode::InScene);

		// Actor constructor from prototype with transform
		Actor(ActorTransform* transform, const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::InScene);

		// Constructor with components with transform
		Actor(ActorTransform* transform, ComponentsVec components, ActorCreateMode mode = ActorCreateMode::InScene);

		// Copy-constructor with transform
		Actor(ActorTransform* transform, const Actor& other);

		// Not using prototype setter
		void SetProtytypeDummy(ActorAssetRef asset);

		// Sets parent
		void SetParentProp(Actor* actor);

		// Sets prototype and links actor to them
		void SetPrototype(ActorAssetRef asset);

		// Updates enabling
		void UpdateEnabled();

		// Updates locking
		void UpdateLocking();

		// Beginning serialization callback
		void OnSerialize(DataNode& node) const;

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Regular serializing without prototype
		void SerializeRaw(DataNode& node) const;

		// Regular deserializing without prototype
		void DeserializeRaw(const DataNode& node);

		// Regular serializing with prototype
		void SerializeWithProto(DataNode& node) const;

		// Regular deserializing with prototype
		void DeserializeWithProto(const DataNode& node);

		// Returns dictionary of all children by names
		Dictionary<String, Actor*> GetAllChilds();

		// Returns dictionary of all components by type names
		Dictionary<String, Component*> GetAllComponents();

		// Applies excluding from scene for all components in hierarchy
		void ExcludeComponentsFromScene();

		// Applies including to scene for all components in hierarchy
		void IncludeComponentsToScene();

		// It is called when transformation was changed and updated
		virtual void OnTransformUpdated();

		// It is called when parent changed
		virtual void OnParentChanged(Actor* oldParent);

		// It is called when child actor was added
		virtual void OnChildAdded(Actor* child);

		// It is called when child actor was removed
		virtual void OnChildRemoved(Actor* child);

		// It is called when layer was changed
		virtual void OnLayerChanged(SceneLayer* oldLayer);

		// Returns all children actors with their children
		void GetAllChildrenActors(Vector<Actor*>& actors);

		// Initializes properties
		void InitializeProperties();

		// -----------------------------
		// Actor prototype applying info
		// -----------------------------
		struct ApplyActorInfo
		{
			Actor*    actor;
			Actor*    matchingChild;
			ActorsVec allChildren;

			Vector<Actor**>     actorPointersFields;
			Vector<Component**> componentPointersFields;

			Dictionary<const Actor*, Actor*>         actorsMap;
			Dictionary<const Component*, Component*> componentsMap;

			bool operator==(const ApplyActorInfo& other) const { return actor == other.actor; }
		};

		// Processes copying actor
		void ProcessCopying(Actor* dest, const Actor* source,
							Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers,
							Dictionary<const Actor*, Actor*>& actorsMap,
							Dictionary<const Component*, Component*>& componentsMap,
							bool isSourcePrototype);

		// Processes making prototype
		void ProcessPrototypeMaking(Actor* dest, Actor* source,
									Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers,
									Dictionary<const Actor*, Actor*>& actorsMap,
									Dictionary<const Component*, Component*>& componentsMap,
									bool isInsidePrototype);

		// Copies fields from source to dest
		void CopyFields(Vector<FieldInfo*>& fields, IObject* source, IObject* dest,
						Vector<Actor**>& actorsPointers,
						Vector<Component**>& componentsPointers,
						Vector<ISerializable*>& serializableObjects);

		// Copies changed field from source to dest
		void CopyChangedFields(Vector<FieldInfo*>& fields,
							   IObject* source, IObject* changed, IObject* dest,
							   Vector<Actor**>& actorsPointers,
							   Vector<Component**>& componentsPointers,
							   Vector<ISerializable*>& serializableObjects);

		// Applies basic actor fields and transform from source to dest  
		void CopyActorChangedFields(Actor* source, Actor* changed, Actor* dest, Vector<Actor*>& allDestChilds, bool withTransform);

		// Collects fixing actors and components pointers in new component
		void CollectFixingFields(Component* newComponent, Vector<Component**>& componentsPointers,
								 Vector<Actor**>& actorsPointers);

		// Collects component field, except Component class fields
		void GetComponentFields(Component* component, Vector<FieldInfo*>& fields);

		// Separates children actors to linear array, removes child and parent links
		void SeparateActors(Vector<Actor*>& separatedActors);

		// Processes reverting actor
		void ProcessReverting(Actor* dest, const Actor* source, const Vector<Actor*>& separatedActors,
							  Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers,
							  Dictionary<const Actor*, Actor*>& actorsMap,
							  Dictionary<const Component*, Component*>& componentsMap,
							  Vector<ISerializable*>& serializableObjects);

	   // Fixes actors and components pointers by actors map
		void FixComponentFieldsPointers(const Vector<Actor**>& actorsPointers,
										const Vector<Component**>& componentsPointers,
										const Dictionary<const Actor*, Actor*>& actorsMap,
										const Dictionary<const Component*, Component*>& componentsMap);

		friend class ActorAsset;
		friend class ActorDataNodeConverter;
		friend class ActorRef;
		friend class ActorTransform;
		friend class Component;
		friend class DrawableComponent;
		friend class SceneDrawable;
		friend class Scene;
		friend class SceneLayer;
		friend class Tag;
	};

	// -------------------------
	// Actor data node converter
	// -------------------------
	class ActorDataNodeConverter: public IDataNodeTypeConverter, public Singleton<ActorDataNodeConverter>
	{
	public:
		// Converts actor pointer to data 
		void ToData(const void* object, DataNode& data);

		// Gets actor pointer from data
		void FromData(void* object, const DataNode& data);

		// Checks that type is based on Actor's type
		bool IsConvertsType(const Type* type) const;

	protected:
		struct ActorDef
		{
			Actor** target;
			bool    isAsset;
			UInt64  actorId;
			UID     assetId;

			ActorDef() {}

			ActorDef(Actor** target, UInt64 id):
				target(target), isAsset(false), actorId(id)
			{}

			ActorDef(Actor** target, UID id):
				target(target), isAsset(true), assetId(id)
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

		// It is called when new actor was created
		static void ActorCreated(Actor* actor);

		friend class Actor;
		friend class Scene;
	};

	template<typename _type, typename _check>
	_type* Actor::GetChildByType()
	{
		for (auto child : mChildren)
			if (child->GetType() == TypeOf(_type))
				return (_type*)child;

		for (auto child : mChildren)
			if (auto res = child->GetChildByType<_type>())
				return res;

		return nullptr;
	}

	template<typename _type>
	Vector<_type>* Actor::GetComponentsInChildren() const
	{
		Vector < _type >> res = GetComponents < _type*();

		for (auto child : mChildren)
			res.Add(child->GetComponentsInChildren<_type>());

		return res;
	}

	template<typename _type>
	_type* Actor::GetComponentInChildren() const
	{
		_type > res = GetComponent < _type*();

		if (res)
			return res;

		for (auto child : mChildren)
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
				return dynamic_cast<_type*>(comp);
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

}
