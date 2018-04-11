#pragma once

#include "Assets/ActorAsset.h"
#include "Scene/ActorTransform.h"
#include "Scene/Scene.h"
#include "Scene/Tags.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Singleton.h"
#include "Utils/Types/String.h"
#include "Utils/Types/UID.h"

namespace o2
{
	class Component;
	class Actor;

	enum class ActorCreateMode { InScene, NotInScene, Default };

	// --------------------------------------------------------------
	// Actor reference, automatically invalidates when actor deleting
	// --------------------------------------------------------------
	class ActorRef: public ISerializable
	{
	public:
		// Default constructor, no reference
		ActorRef();

		// Constructor with referencing on actor
		ActorRef(Actor* actor);

		// Creates actor by prototype and returns reference on it
		ActorRef(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Creates actor with components and returns reference on it
		ActorRef(Vector<Component*> components, ActorCreateMode mode = ActorCreateMode::Default);

		// Creates a copy of actor and returns reference on it
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

	// ---------------------------------------------------------------------------------------------
	// Scene actor. This is a basic scene object. All other scene object types is derived from this.
	// It has an unique id, name, tags, scene layer, transformation, components and children actors.
	// Can be created from prototype of other actor. 
	// ---------------------------------------------------------------------------------------------
	class Actor: virtual public ISerializable
	{
	public:
		typedef Vector<Actor*> ActorsVec;
		typedef Vector<Component*> ComponentsVec;

	public:
		PROPERTIES(Actor);
		PROPERTY(ActorAssetRef, prototype, SetPrototype, GetPrototype); // Prototype asset reference property

		PROPERTY(Actor*, parent, SetParent, GetParent); // Parent actor property

		GETTER(UInt64, id, GetID);                // Actor unique id
		PROPERTY(String, name, SetName, GetName); // Actor name property


		PROPERTY(SceneLayer*, layer, SetLayer, GetLayer);    // Layer property
		PROPERTY(String, layerName, SetLayer, GetLayerName); // Layer name property

		PROPERTY(bool, enabled, SetEnabled, IsEnabled);         // Is actor enabled property
		GETTER(bool, enabledInHierarchy, IsEnabledInHierarchy); // Is actor enabled in hierarchy getter

		PROPERTY(bool, locked, SetLocked, IsLocked);           // Is actor locked property
		GETTER(bool, lockedInHierarchy, IsLockedInHierarchy);  // Is actor locked in hierarchy getter

		GETTER(ActorsVec, children, GetChildren);         // Children array getter
		GETTER(ComponentsVec, components, GetComponents); // Components array getter

		ACCESSOR(Actor*, child, String, GetChild, GetAllChilds);                 // Children accessor
		ACCESSOR(Component*, component, String, GetComponent, GetAllComponents); // Component accessor by type name

		TagGroup                tags;      // Tags group
		ActorTransform* const   transform; // Transformation of actor

		Function<void(bool)> onEnableChanged; // Enable changing event

	public:
		// Default constructor
		Actor(ActorCreateMode mode = ActorCreateMode::Default);

		// Actor constructor from prototype
		Actor(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Constructor with components
		Actor(ComponentsVec components, ActorCreateMode mode = ActorCreateMode::Default);

		// Copy-constructor
		Actor(const Actor& other);

		// Destructor
		virtual ~Actor();

		// Assign operator
		Actor& operator=(const Actor& other);

		// Updates actor and components
		virtual void Update(float dt);

		// Updates childs
		virtual void UpdateChildren(float dt);

		// Updates transform
		virtual void UpdateTransform(bool withChildren = true);

		// Updates children transforms
		virtual void UpdateChildrenTransforms();

		// Returns prototype from this or this parent
		ActorAssetRef GetPrototype() const;

		// Returns prototype directly from only this
		ActorAssetRef GetPrototypeDirectly() const;

		// Breaks link to prototype
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
		virtual void SetEnabled(bool active);

		// Enables actor
		void Enable();

		// Disables actor
		void Disable();

		// Returns is actor enabled
		bool IsEnabled() const;

		// Returns is really enabled
		bool IsResEnabled() const;

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
		virtual void SetPositionIndexInParent(int index);

		// Sets parent
		void SetParent(Actor* actor, bool worldPositionStays = true);

		// Returns parent
		Actor* GetParent() const;

		// Add child actor
		Actor* AddChild(Actor* actor);

		// Add children actors
		Vector<Actor*> AddChildren(Vector<Actor*> actors);

		// Add child actor
		Actor* AddChild(Actor* actor, int index);

		// Returns child actor by path (ex "root/some node/other node/target node")
		Actor* GetChild(const String& path) const;

		// Returns child actor by name
		Actor* FindChild(const String& name) const;

		// Searches child with specified type
		template<typename _type>
		_type* GetChildByType();

		// Returns children array
		ActorsVec GetChildren() const;

		// Removes child and destroys him if needed
		void RemoveChild(Actor* actor, bool release = true);

		// Removes and destroys all childs
		void RemoveAllChildren(bool release = true);

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

		// Sets default actores creation mode
		static void SetDefaultCreationMode(ActorCreateMode mode);

		// Return default actors creation mode
		static ActorCreateMode GetDefaultCreationMode();

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

		// Returns the path of create menu category
		const String& GetCreateCategory() const;
#endif

		SERIALIZABLE(Actor);

	protected:
		typedef Vector<ActorRef*> ActorRefsVec;

		static ActorCreateMode mDefaultCreationMode;   // Default mode creation

		ActorAssetRef   mPrototype;                    // Prototype asset
		ActorRef        mPrototypeLink = nullptr;      // Prototype link actor. Links to source actor from prototype
												       
		UInt64          mId;                           // Unique actor id
		String          mName;                         // Name of actor
												       
		SceneLayer*     mLayer = nullptr;              // Scene layer @EXCLUDE_POINTER_SEARCH
												       
		Actor*          mParent = nullptr;             // Parent actor
		ActorsVec       mChildren;                     // Children actors 
												       
		ComponentsVec   mComponents;                   // Components vector 

		bool            mEnabled = true;               // Is actor enabled
		bool            mResEnabled = true;            // Is actor really enabled. 
		bool            mResEnabledInHierarchy = true; // Is actor enabled in hierarchy

		bool            mLocked = false;               // Is actor locked
		bool            mResLocked = false;            // Is actor locked in hierarchy

		bool            mIsOnScene = true;             // Is actor on scene

		bool            mIsAsset = false;              // Is this actor cached asset
		UID             mAssetId;                      // Source asset id

		ActorRefsVec    mReferences;                   // References to this actor

	protected:
		// Base actor constructor with transform
		Actor(ActorTransform* transform, bool isOnScene = true, const String& name = "unnamed", bool enabled = true,
			  bool resEnabled = true, bool locked = false, bool resLocked = false,
			  SceneLayer* layer = nullptr, UInt64 id = Math::Random(), UID assetId = 0);

		// Default constructor with transform
		Actor(ActorTransform* transform, ActorCreateMode mode = ActorCreateMode::Default);

		// Actor constructor from prototype with transform
		Actor(ActorTransform* transform, const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Constructor with components with transform
		Actor(ActorTransform* transform, ComponentsVec components, ActorCreateMode mode = ActorCreateMode::Default);

		// Copy-constructor with transform
		Actor(ActorTransform* transform, const Actor& other);

		// Not using prototype setter
		void SetProtytypeDummy(ActorAssetRef asset);

		// Sets parent
		void SetParentProp(Actor* actor);

		// Sets prototype and links actor to them
		void SetPrototype(ActorAssetRef asset);

		// Updates result read enable flag
		virtual void UpdateResEnabled();

		// Updates enabling
		virtual void UpdateResEnabledInHierarchy();

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

		// It is called when actor excluding from scene
		virtual void OnExcludeFromScene();

		// It is called when actor including from scene
		virtual void OnIncludeToScene();

		// Applies excluding from scene for all components in hierarchy
		void ExcludeComponentsFromScene();

		// Applies including to scene for all components in hierarchy
		void IncludeComponentsToScene();

		// It is called when result enable was changed
		virtual void OnResEnableInHierarchyChanged();

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

		// Copies data of actor from other to this
		virtual void CopyData(const Actor& otherActor);

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
		friend class UIWidget;
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

	template<typename _type>
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

CLASS_BASES_META(o2::ActorRef)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ActorRef)
{
	PROTECTED_FIELD(mActor);
	PROTECTED_FIELD(mWasDeleted);
}
END_META;
CLASS_METHODS_META(o2::ActorRef)
{

	PUBLIC_FUNCTION(Actor*, Get);
	PUBLIC_FUNCTION(const Actor*, Get);
	PUBLIC_FUNCTION(bool, IsValid);
	PUBLIC_FUNCTION(bool, IsWasDeleted);
}
END_META;

CLASS_BASES_META(o2::Actor)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Actor)
{
	PUBLIC_FIELD(prototype);
	PUBLIC_FIELD(parent);
	PUBLIC_FIELD(id);
	PUBLIC_FIELD(name);
	PUBLIC_FIELD(layer);
	PUBLIC_FIELD(layerName);
	PUBLIC_FIELD(enabled);
	PUBLIC_FIELD(enabledInHierarchy);
	PUBLIC_FIELD(locked);
	PUBLIC_FIELD(lockedInHierarchy);
	PUBLIC_FIELD(children);
	PUBLIC_FIELD(components);
	PUBLIC_FIELD(child);
	PUBLIC_FIELD(component);
	PUBLIC_FIELD(tags);
	PUBLIC_FIELD(transform);
	PUBLIC_FIELD(onEnableChanged);
	PROTECTED_FIELD(mPrototype);
	PROTECTED_FIELD(mPrototypeLink);
	PROTECTED_FIELD(mId);
	PROTECTED_FIELD(mName);
	PROTECTED_FIELD(mLayer).EXCLUDE_POINTER_SEARCH_ATTRIBUTE();
	PROTECTED_FIELD(mParent);
	PROTECTED_FIELD(mChildren);
	PROTECTED_FIELD(mComponents);
	PROTECTED_FIELD(mEnabled);
	PROTECTED_FIELD(mResEnabled);
	PROTECTED_FIELD(mResEnabledInHierarchy);
	PROTECTED_FIELD(mLocked);
	PROTECTED_FIELD(mResLocked);
	PROTECTED_FIELD(mIsOnScene);
	PROTECTED_FIELD(mIsAsset);
	PROTECTED_FIELD(mAssetId);
	PROTECTED_FIELD(mReferences);
}
END_META;
CLASS_METHODS_META(o2::Actor)
{

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
	PUBLIC_FUNCTION(void, UpdateChildren, float);
	PUBLIC_FUNCTION(void, UpdateTransform, bool);
	PUBLIC_FUNCTION(void, UpdateChildrenTransforms);
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
	PUBLIC_FUNCTION(void, SetID, UInt64);
	PUBLIC_FUNCTION(void, GenerateNewID, bool);
	PUBLIC_FUNCTION(UID, GetAssetID);
	PUBLIC_FUNCTION(bool, IsAsset);
	PUBLIC_FUNCTION(void, ExcludeFromScene);
	PUBLIC_FUNCTION(void, IncludeInScene);
	PUBLIC_FUNCTION(bool, IsOnScene);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(void, Enable);
	PUBLIC_FUNCTION(void, Disable);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(bool, IsResEnabled);
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
	PUBLIC_FUNCTION(Vector<Actor*>, AddChildren, Vector<Actor*>);
	PUBLIC_FUNCTION(Actor*, AddChild, Actor*, int);
	PUBLIC_FUNCTION(Actor*, GetChild, const String&);
	PUBLIC_FUNCTION(Actor*, FindChild, const String&);
	PUBLIC_FUNCTION(ActorsVec, GetChildren);
	PUBLIC_FUNCTION(void, RemoveChild, Actor*, bool);
	PUBLIC_FUNCTION(void, RemoveAllChildren, bool);
	PUBLIC_FUNCTION(Component*, AddComponent, Component*);
	PUBLIC_FUNCTION(void, RemoveComponent, Component*, bool);
	PUBLIC_FUNCTION(void, RemoveAllComponents);
	PUBLIC_FUNCTION(Component*, GetComponent, const String&);
	PUBLIC_FUNCTION(Component*, GetComponent, const Type*);
	PUBLIC_FUNCTION(Component*, GetComponent, UInt64);
	PUBLIC_FUNCTION(ComponentsVec, GetComponents);
	PUBLIC_FUNCTION(void, SetLayer, SceneLayer*);
	PUBLIC_FUNCTION(void, SetLayer, const String&);
	PUBLIC_FUNCTION(SceneLayer*, GetLayer);
	PUBLIC_FUNCTION(String, GetLayerName);
	PUBLIC_FUNCTION(Actor*, FindLinkedActor, Actor*);
	PUBLIC_FUNCTION(Actor*, FindLinkedActor, UInt64);
	PUBLIC_FUNCTION(Actor*, FindActorById, UInt64);
	PROTECTED_FUNCTION(void, SetProtytypeDummy, ActorAssetRef);
	PROTECTED_FUNCTION(void, SetParentProp, Actor*);
	PROTECTED_FUNCTION(void, SetPrototype, ActorAssetRef);
	PROTECTED_FUNCTION(void, UpdateResEnabled);
	PROTECTED_FUNCTION(void, UpdateResEnabledInHierarchy);
	PROTECTED_FUNCTION(void, UpdateLocking);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, SerializeRaw, DataNode&);
	PROTECTED_FUNCTION(void, DeserializeRaw, const DataNode&);
	PROTECTED_FUNCTION(void, SerializeWithProto, DataNode&);
	PROTECTED_FUNCTION(void, DeserializeWithProto, const DataNode&);
	PROTECTED_FUNCTION(_tmp1, GetAllChilds);
	PROTECTED_FUNCTION(_tmp2, GetAllComponents);
	PROTECTED_FUNCTION(void, OnExcludeFromScene);
	PROTECTED_FUNCTION(void, OnIncludeToScene);
	PROTECTED_FUNCTION(void, ExcludeComponentsFromScene);
	PROTECTED_FUNCTION(void, IncludeComponentsToScene);
	PROTECTED_FUNCTION(void, OnResEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, OnParentChanged, Actor*);
	PROTECTED_FUNCTION(void, OnChildAdded, Actor*);
	PROTECTED_FUNCTION(void, OnChildRemoved, Actor*);
	PROTECTED_FUNCTION(void, OnLayerChanged, SceneLayer*);
	PROTECTED_FUNCTION(void, GetAllChildrenActors, Vector<Actor*>&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, ProcessCopying, Actor*, const Actor*, Vector<Actor**>&, Vector<Component**>&, _tmp3, _tmp4, bool);
	PROTECTED_FUNCTION(void, ProcessPrototypeMaking, Actor*, Actor*, Vector<Actor**>&, Vector<Component**>&, _tmp5, _tmp6, bool);
	PROTECTED_FUNCTION(void, CopyFields, Vector<FieldInfo*>&, IObject*, IObject*, Vector<Actor**>&, Vector<Component**>&, Vector<ISerializable*>&);
	PROTECTED_FUNCTION(void, CopyChangedFields, Vector<FieldInfo*>&, IObject*, IObject*, IObject*, Vector<Actor**>&, Vector<Component**>&, Vector<ISerializable*>&);
	PROTECTED_FUNCTION(void, CopyActorChangedFields, Actor*, Actor*, Actor*, Vector<Actor*>&, bool);
	PROTECTED_FUNCTION(void, CollectFixingFields, Component*, Vector<Component**>&, Vector<Actor**>&);
	PROTECTED_FUNCTION(void, GetComponentFields, Component*, Vector<FieldInfo*>&);
	PROTECTED_FUNCTION(void, SeparateActors, Vector<Actor*>&);
	PROTECTED_FUNCTION(void, ProcessReverting, Actor*, const Actor*, const Vector<Actor*>&, Vector<Actor**>&, Vector<Component**>&, _tmp7, _tmp8, Vector<ISerializable*>&);
	PROTECTED_FUNCTION(void, FixComponentFieldsPointers, const Vector<Actor**>&, const Vector<Component**>&, _tmp9, _tmp10);
}
END_META;
