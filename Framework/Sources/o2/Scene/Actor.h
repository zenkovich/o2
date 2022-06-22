#pragma once

#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Scene/ActorCreationMode.h"
#include "o2/Scene/ActorRef.h"
#include "o2/Scene/ActorTransform.h"
#include "o2/Scene/Tags.h"
#include "o2/Utils/Editor/Attributes/AnimatableAttribute.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"
#include "o2/Utils/Editor/SceneEditableObject.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/UID.h"
#include "o2/Scripts/ScriptValue.h"

namespace o2
{
	class Component;
	class Scene;
	class SceneLayer;

#if IS_EDITOR
	typedef SceneEditableObject ActorBase;

	struct ActorDifferences;

#define OPTIONAL_OVERRIDE override
#else
	typedef ISerializable ActorBase;

#define OPTIONAL_OVERRIDE override
#endif

	// ---------------------------------------------------------------------------------------------
	// Scene actor. This is a basic scene object. All other scene object types is derived from this.
	// It has an unique id, name, tags, scene layer, transformation, components and children actors.
	// Can be created from prototype of other actor. 
	// When editor pragma is enabled, it is derived from SceneEditableObject and support all editor 
	// features for editing actor
	// ---------------------------------------------------------------------------------------------
	class Actor: virtual public ActorBase
	{
	public:
		enum class SceneStatus { InScene, NotInScene, WaitingAddToScene };

	public:
		PROPERTIES(Actor);
		PROPERTY(ActorAssetRef, prototype, SetPrototype, GetPrototype); // Prototype asset reference property @EDITOR_IGNORE

		GETTER(SceneUID, id, GetID);              // Actor unique id
		PROPERTY(String, name, SetName, GetName); // Actor name property @EDITOR_IGNORE @ANIMATABLE @SCRIPTABLE

		PROPERTY(String, layerName, SetLayer, GetLayerName); // Layer name property @EDITOR_IGNORE @ANIMATABLE @SCRIPTABLE

		PROPERTY(bool, enabled, SetEnabled, IsEnabled);         // Is actor enabled property @EDITOR_IGNORE @ANIMATABLE @SCRIPTABLE
		GETTER(bool, enabledInHierarchy, IsEnabledInHierarchy); // Is actor enabled in hierarchy getter

		GETTER(Vector<Actor*>, children, GetChildren);         // Children array getter
		GETTER(Vector<Component*>, components, GetComponents); // Components array getter

		ACCESSOR(Actor*, child, String, GetChild, GetAllChilds);                 // Children accessor
		ACCESSOR(Component*, component, String, GetComponent, GetAllComponents); // Component accessor by type name

	public:
		TagGroup              tags;      // Tags group @EDITOR_IGNORE
		ActorTransform* const transform; // Transformation of actor @EDITOR_IGNORE @ANIMATABLE

	public:
		// Default constructor @SCRIPTABLE
		Actor(ActorCreateMode mode = ActorCreateMode::Default);

		// Actor constructor from prototype
		Actor(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Constructor with components
		Actor(Vector<Component*> components, ActorCreateMode mode = ActorCreateMode::Default);

		// Copy-constructor
		Actor(const Actor& other, ActorCreateMode mode);

		// Copy-constructor
		Actor(const Actor& other);

		// Destructor
		virtual ~Actor();

		// Assign operator
		Actor& operator=(const Actor& other);

		// Updates actor and components @SCRIPTABLE
		virtual void Update(float dt);

		// Updates actor and components with fixed delta time @SCRIPTABLE
		virtual void FixedUpdate(float dt);

		// Updates childs @SCRIPTABLE
		virtual void UpdateChildren(float dt);

		// Updates childs with fixed delta time @SCRIPTABLE
		virtual void FixedUpdateChildren(float dt);

		// Updates self transform, dependent parents and children transforms @SCRIPTABLE
		virtual void UpdateTransform() OPTIONAL_OVERRIDE;

		// Updates transform @SCRIPTABLE
		virtual void UpdateSelfTransform();

		// Updates children transforms @SCRIPTABLE
		virtual void UpdateChildrenTransforms();

		// Sets actor's name
		void SetName(const String& name) OPTIONAL_OVERRIDE;

		// Returns name
		const String& GetName() const OPTIONAL_OVERRIDE;

		// Returns actor's unique id
		SceneUID GetID() const OPTIONAL_OVERRIDE;

		// Sets id. Be carefully! Ids must be unique! Not recommended to change this
		void SetID(SceneUID id);

		// Generates new random id @SCRIPTABLE
		void GenerateNewID(bool withChildren = true) OPTIONAL_OVERRIDE;

		// Returns asset id @SCRIPTABLE
		UID GetAssetID() const;

		// Is this from asset @SCRIPTABLE
		bool IsAsset() const;

		// Sets prototype and links actor to them
		void SetPrototype(ActorAssetRef asset);

		// Returns prototype from this or this parent
		ActorAssetRef GetPrototype() const;

		// Returns prototype directly from only this
		ActorAssetRef GetPrototypeDirectly() const;

		// Returns prototype link pointer
		ActorRef GetPrototypeLink() const;

		// Includes to scene and now will be update and draw automatically from scene @SCRIPTABLE
		void AddToScene();

		// Excludes from scene and will not be update and draw automatically from scene @SCRIPTABLE
		void RemoveFromScene(bool keepEditorObjects = false);

		// Is actor on scene @SCRIPTABLE
		bool IsOnScene() const OPTIONAL_OVERRIDE;

		// Sets actor enabling
		virtual void SetEnabled(bool enabled) OPTIONAL_OVERRIDE;

		// Enables actor @SCRIPTABLE
		void Enable();

		// Disables actor @SCRIPTABLE
		void Disable();

		// Returns is actor enabled
		bool IsEnabled() const OPTIONAL_OVERRIDE;

		// Returns is really enabled @SCRIPTABLE
		bool IsResEnabled() const;

		// Returns is actor enabled in hierarchy @SCRIPTABLE
		bool IsEnabledInHierarchy() const OPTIONAL_OVERRIDE;

		// Sets parent @SCRIPTABLE
		void SetParent(Actor* actor, bool worldPositionStays = true);

		// Returns parent @SCRIPTABLE
		Actor* GetParent() const;

		// Sets index position in parent or scene @SCRIPTABLE
		virtual void SetIndexInSiblings(int index) OPTIONAL_OVERRIDE;

		// Add child actor @SCRIPTABLE
		virtual Actor* AddChild(Actor* actor);

		// Add children actors @SCRIPTABLE
		void AddChildren(const Vector<Actor*>& actors);

		// Add child actor @SCRIPTABLE
		virtual Actor* AddChild(Actor* actor, int index);

		// Returns child actor by path (ex "root/some node/other node/target node") @SCRIPTABLE
		Actor* GetChild(const String& path) const;

		// Returns child actor by name @SCRIPTABLE
		Actor* FindChild(const String& name) const;

		// Returns child actor by predicate @SCRIPTABLE
		Actor* FindChild(const Function<bool(const Actor* child)>& pred) const;

		// Returns child actor by path (ex "root/some node/other node/target node")
		template<typename _type>
		_type* GetChildByType(const String& path) const;

		// Returns child actor by name
		template<typename _type>
		_type* FindChildByTypeAndName(const String& name) const;

		// Searches child with specified type
		template<typename _type>
		_type* FindChildByType(bool searchInChildren = true);

		// Returns children array @SCRIPTABLE
		const Vector<Actor*>& GetChildren() const;

		// Returns all children actors with their children
		virtual void GetAllChildrenActors(Vector<Actor*>& actors);

		// Removes child and destroys him if needed @SCRIPTABLE
		void RemoveChild(Actor* actor, bool release = true);

		// Removes and destroys all childs @SCRIPTABLE
		void RemoveAllChildren(bool release = true);

		// Searches actor with id in this and this children @SCRIPTABLE
		virtual Actor* FindActorById(SceneUID id);

		// And new component
		template<typename _type>
		_type* AddComponent();

		// Adds new component @SCRIPTABLE
		Component* AddComponent(Component* component);

		// Removes component @SCRIPTABLE
		void RemoveComponent(Component* component, bool release = true);

		// Removes all components @SCRIPTABLE
		void RemoveAllComponents();

		// Returns component with type name @SCRIPTABLE
		Component* GetComponent(const String& typeName);

		// Returns component with type @SCRIPTABLE
		Component* GetComponent(const Type* type);

		// Returns component by id @SCRIPTABLE
		Component* GetComponent(SceneUID id);

		// Returns component with type
		template<typename _type>
		_type* GetComponent() const;

		// Returns component with type in this and children
		template<typename _type>
		_type* GetComponentInChildren() const;

		// Return all components by type
		template<typename _type>
		Vector<_type*> GetComponents() const;

		// Returns all components by type in this and children
		template<typename _type>
		Vector<_type*> GetComponentsInChildren() const;

		// Returns all components @SCRIPTABLE
		const Vector<Component*>& GetComponents() const;

		// Sets layer by name
		void SetLayer(const String& layerName);

		// Returns layer @SCRIPTABLE
		SceneLayer* GetLayer() const;

		// Returns layer name
		const String& GetLayerName() const;

		// Beginning serialization callback
		void SerializeBasicOverride(DataValue& node) const;

		// Completion deserialization callback
		void DeserializeBasicOverride(const DataValue& node);

		// Sets default actors creation mode
		static void SetDefaultCreationMode(ActorCreateMode mode);

		// Return default actors creation mode
		static ActorCreateMode GetDefaultCreationMode();

		// Returns is current mode means mode on scene. For InScene always returns true. For returns true only when default creation mode is onscene.
		static bool IsModeOnScene(ActorCreateMode mode);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(Actor);

	protected:
		struct ICopyVisitor
		{
			int depth = 0;

			virtual ~ICopyVisitor() {}
			virtual void OnCopyActor(const Actor* source, Actor* target) = 0;
			virtual void OnCopyComponent(const Component* source, Component* target) = 0;
			virtual void Finalize() {}
		};

		struct SourceToTargetMapCloneVisitor: public ICopyVisitor
		{
			Map<const Actor*, Actor*>         sourceToTargetActors;
			Map<const Component*, Component*> sourceToTargetComponents;

			void OnCopyActor(const Actor* source, Actor* target) override;
			void OnCopyComponent(const Component* source, Component* target) override;
			void Finalize() override;
		};

		struct InstantiatePrototypeCloneVisitor: public SourceToTargetMapCloneVisitor
		{
			void OnCopyActor(const Actor* source, Actor* target) override;
			void OnCopyComponent(const Component* source, Component* target) override;
		};

	protected:
		static ActorCreateMode mDefaultCreationMode;   // Default mode creation

		SceneUID mId;   // Unique actor id
		String   mName; // Name of actor @SERIALIZABLE

		ActorAssetRef mPrototype;               // Prototype asset
		ActorRef      mPrototypeLink = nullptr; // Prototype link actor. Links to source actor from prototype

		String      mLayerName = String("Default"); // Scene layer name @SERIALIZABLE
		SceneLayer* mLayer = nullptr;       // Scene layer. Empty when actor isn't on scene

		Actor* mParent = nullptr; // Parent actor 
		Vector<Actor*> mChildren;         // Children actors 

		Vector<Component*> mComponents; // Components vector 

		bool mEnabled = true;               // Is actor enabled
		bool mResEnabled = true;            // Is actor really enabled. 
		bool mResEnabledInHierarchy = true; // Is actor enabled in hierarchy

		SceneStatus mSceneStatus = SceneStatus::NotInScene; // Actor on scene status

		bool mIsAsset = false; // Is this actor cached asset
		UID  mAssetId;         // Source asset id

		Vector<ActorRef*> mReferences; // References to this actor

		mutable ICopyVisitor* mCopyVisitor = nullptr; // Copy visitor. It is called when copying actor and calls on actor or component copying

	protected:
		// Base actor constructor with transform
		Actor(ActorTransform* transform, SceneStatus sceneStatus = SceneStatus::WaitingAddToScene,
			  const String& name = "unnamed", bool enabled = true, bool resEnabled = true, bool locked = false,
			  bool resLocked = false, const String& layerName = "", SceneLayer* layer = nullptr,
			  SceneUID id = Math::Random(), UID assetId = UID(0));

		// Default constructor with transform
		Actor(ActorTransform* transform, ActorCreateMode mode = ActorCreateMode::Default);

		// Actor constructor from prototype with transform
		Actor(ActorTransform* transform, const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Constructor with components with transform
		Actor(ActorTransform* transform, Vector<Component*> components, ActorCreateMode mode = ActorCreateMode::Default);

		// Copy-constructor with transform
		Actor(ActorTransform* transform, const Actor& other, ActorCreateMode mode = ActorCreateMode::Default);

		// Copies fields from source to dest
		void CopyFields(Vector<const FieldInfo*>& fields, IObject* source, IObject* dest,
						Vector<Actor**>& actorsPointers,
						Vector<Component**>& componentsPointers,
						Vector<ISerializable*>& serializableObjects);

		// Collects fixing actors and components pointers in new component
		void CollectFixingFields(Component* newComponent, Vector<Component**>& componentsPointers,
								 Vector<Actor**>& actorsPointers);

		// Collects component field, except Component class fields
		void GetComponentFields(Component* component, Vector<const FieldInfo*>& fields);

		// Fixes actors and components pointers by actors map
		void FixComponentFieldsPointers(const Vector<Actor**>& actorsPointers,
										const Vector<Component**>& componentsPointers,
										const Map<const Actor*, Actor*>& actorsMap,
										const Map<const Component*, Component*>& componentsMap);

		// Updates result read enable flag
		virtual void UpdateResEnabled();

		// Updates enabling
		virtual void UpdateResEnabledInHierarchy();

		// Regular serializing without prototype
		virtual void SerializeRaw(DataValue& node) const;

		// Regular deserializing without prototype
		virtual void DeserializeRaw(const DataValue& node);

		// Regular serializing with prototype
		virtual void SerializeWithProto(DataValue& node) const;

		// Regular deserializing with prototype
		virtual void DeserializeWithProto(const DataValue& node);

		// Returns dictionary of all children by names
		Map<String, Actor*> GetAllChilds();

		// Returns dictionary of all components by type names
		Map<String, Component*> GetAllComponents();

		// Sets parent
		void SetParentProp(Actor* actor);

		// Is is called when actor has added to scene
		virtual void OnAddToScene();

		// It is called when actor has removed from scene
		virtual void OnRemoveFromScene();

		// It is called on first update
		virtual void OnStart();

		// Is is called on update with frame dt
		virtual void OnUpdate(float dt);

		// It is called on fixed update with fixed dt
		virtual void OnFixedUpdate(float dt);

		// Is is called when actor enabled in hierarchy
		virtual void OnEnabled();

		// It is called when actor disabled in hierarchy
		virtual void OnDisabled();

		// It is called when result enable was changed
		virtual void OnEnableInHierarchyChanged();

		// It is called when transformation was updated
		virtual void OnTransformUpdated();

		// It is called when transformation was changed 
		virtual void OnTransformChanged();

		// It is called when parent changed
		virtual void OnParentChanged(Actor* oldParent);

		// It is called when child actor was added
		virtual void OnChildAdded(Actor* child);

		// It is called when child actor was removed
		virtual void OnChildRemoved(Actor* child);

		// It is called when layer was changed
		virtual void OnLayerChanged(SceneLayer* oldLayer);

		// It is called when new component has added to actor
		virtual void OnComponentAdded(Component* component);

		// It is called when component going to be removed from actor
		virtual void OnComponentRemoving(Component* component);

#if IS_SCRIPTING_SUPPORTED
	public:
		virtual void ReflectValue(ScriptValue& value);
#endif

#if IS_EDITOR
	public:
		PROPERTY(bool, locked, SetLocked, IsLocked);          // Is actor locked property @EDITOR_IGNORE
		GETTER(bool, lockedInHierarchy, IsLockedInHierarchy); // Is actor locked in hierarchy getter

	public:
		Function<void(bool)>   onEnableChanged;         // Enable changing event
		Function<void()>       onChanged;               // Something in actor change event
		Function<void(Actor*)> onParentChanged;         // Actor change parent event
		Function<void()>       onChildHierarchyChanged; // Actor childs hierarchy change event
		Function<void(bool)>   onLockChanged;           // Locking changing event
		Function<void()>       onNameChanged;           // Name changing event

	public:
		struct MakePrototypeCloneVisitor: SourceToTargetMapCloneVisitor
		{
			void OnCopyActor(const Actor* source, Actor* target) override;
			void OnCopyComponent(const Component* source, Component* target) override;
		};

	public:
		// Sets locking. Locked actor can't be selected in editor scene view. But is can be selected in scene tree
		void SetLocked(bool locked) OPTIONAL_OVERRIDE;

		// Locks actor. Locked actor can't be selected in editor scene view. But is can be selected in scene tree
		void Lock();

		// Unlocks actor. Locked actor can't be selected in editor scene view. But is can be selected in scene tree
		void Unlock();

		// Returns is actor locked. Locked actor can't be selected in editor scene view. But is can be selected in scene tree
		bool IsLocked() const OPTIONAL_OVERRIDE;

		// Returns is actor locked in hierarchy. Locked actor can't be selected in editor scene view. But is can be selected in scene tree
		bool IsLockedInHierarchy() const OPTIONAL_OVERRIDE;

		// Breaks link to prototype
		void BreakPrototypeLink();

		// Applies all changes to prototype and saves it
		void ApplyChangesToPrototype();

		// Reverts all properties to prototype
		void RevertToPrototype();

		// Makes prototype asset from this actor and links this to new asset
		ActorAssetRef MakePrototype();

		// Returns is this linked to specified actor with depth links search
		bool IsLinkedToActor(Actor* actor) const;

		// Searches actor in this, what linked to linkActor
		Actor* FindLinkedActor(Actor* linkActor);

		// Returns object's link to prototype
		const SceneEditableObject* GetEditableLink() const override;

		// Returns list of object's children
		Vector<SceneEditableObject*> GetEditableChildren() const override;

		// Returns object's parent object. Return nullptr when it is a root scene object
		SceneEditableObject* GetEditableParent() const override;

		// Sets parent object. nullptr means make this object as root. idx is place in parent children. idx == -1 means last
		void SetEditableParent(SceneEditableObject* object) override;

		// Adds child. idx is place in parent children. idx == -1 means last
		void AddEditableChild(SceneEditableObject* object, int idx = -1) override;

		// Returns is that type of object can be enabled and disabled
		bool IsSupportsDisabling() const override;

		// Returns is that type of object can be locked
		bool IsSupportsLocking() const override;

		// Returns is that type of object can be transformed
		bool IsSupportsTransforming() const override;

		// Returns transform, override when it's supports
		Basis GetTransform() const override;

		// Sets transform of object, override when it's supports
		void SetTransform(const Basis& transform) override;

		// Returns is object supports pivot 
		bool IsSupportsPivot() const override;

		// Sets transform pivot point
		void SetPivot(const Vec2F& pivot) override;

		// Returns transform pivot
		Vec2F GetPivot() const override;

		// It is called when some changed in actor
		void OnChanged() override;

		// It is called when actor's locking was changed
		void OnLockChanged() override;

		// It is called when actor's name was changed
		void OnNameChanged() override;

		// It is called when child changed
		void OnChildrenChanged() override;

	protected:
		bool mLocked = false;    // Is actor locked @SERIALIZABLE
		bool mResLocked = false; // Is actor locked in hierarchy @SERIALIZABLE

	protected:
		// Applies basic actor fields and transform from source to dest  
		void CopyActorChangedFields(Actor* source, Actor* changed, Actor* dest, Vector<Actor*>& allDestChilds, bool withTransform);

		// Separates children actors to linear array, removes child and parent links
		void SeparateActors(Vector<Actor*>& separatedActors);

		// Processes reverting actor
		void ProcessReverting(Actor* dest, const Actor* source, const Vector<Actor*>& separatedActors,
							  Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers,
							  Map<const Actor*, Actor*>& actorsMap,
							  Map<const Component*, Component*>& componentsMap,
							  Vector<ISerializable*>& serializableObjects);

		// Collects differences between this and prototype
		virtual void GetDifferences(ActorDifferences& differences) const OPTIONAL_OVERRIDE;

		// It is called before making prototype from this object
		void BeginMakePrototype() const override;

		// It is called before instantiate from this object
		void BeginInstantiatePrototype() const override;

		// Not using prototype setter
		void SetProtytypeDummy(ActorAssetRef asset);

		// Updates locking
		void UpdateLocking();

#endif // IS_EDITOR

		friend class ActorAsset;
		friend class ActorRef;
		friend class ActorRefResolver;
		friend class ActorTransform;
		friend class Component;
		friend class ComponentRef;
		friend class DrawableComponent;
		friend class ISceneDrawable;
		friend class Scene;
		friend class SceneLayer;
		friend class Tag;
		friend class Widget;
	};
}

#include "o2/Scene/Component.h"
#include "o2/Scene/ActorRefResolver.h"

namespace o2
{
	template<typename _type>
	_type* Actor::FindChildByType(bool searchInChildren /*= true*/)
	{
		for (auto child : mChildren)
			if (child->GetType() == TypeOf(_type))
				return (_type*)child;

		if (searchInChildren)
		{
			for (auto child : mChildren)
				if (auto res = child->FindChildByType<_type>())
					return res;
		}

		return nullptr;
	}

	template<typename _type>
	_type* Actor::GetChildByType(const String& path) const
	{
		return dynamic_cast<_type*>(GetChild(path));
	}

	template<typename _type>
	_type* Actor::FindChildByTypeAndName(const String& name) const
	{
		return dynamic_cast<_type*>(FindChild(name));
	}

	template<typename _type>
	Vector<_type*> Actor::GetComponentsInChildren() const
	{
		Vector<_type*> res = GetComponents<_type>();

		for (auto child : mChildren)
			res.Add(child->GetComponentsInChildren<_type>());

		return res;
	}

	template<typename _type>
	_type* Actor::GetComponentInChildren() const
	{
		_type* res = GetComponent<_type*>();

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
	Vector<_type*> Actor::GetComponents() const
	{
		Vector<_type*> res;
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

PRE_ENUM_META(o2::Actor::SceneStatus);

CLASS_BASES_META(o2::Actor)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::Actor)
{
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(prototype);
	FIELD().PUBLIC().NAME(id);
	FIELD().PUBLIC().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().SCRIPTABLE_ATTRIBUTE().NAME(name);
	FIELD().PUBLIC().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().SCRIPTABLE_ATTRIBUTE().NAME(layerName);
	FIELD().PUBLIC().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().SCRIPTABLE_ATTRIBUTE().NAME(enabled);
	FIELD().PUBLIC().NAME(enabledInHierarchy);
	FIELD().PUBLIC().NAME(children);
	FIELD().PUBLIC().NAME(components);
	FIELD().PUBLIC().NAME(child);
	FIELD().PUBLIC().NAME(component);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(tags);
	FIELD().PUBLIC().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(transform);
	FIELD().PROTECTED().NAME(mId);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mName);
	FIELD().PROTECTED().NAME(mPrototype);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPrototypeLink);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(String("Default")).NAME(mLayerName);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLayer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mParent);
	FIELD().PROTECTED().NAME(mChildren);
	FIELD().PROTECTED().NAME(mComponents);
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mEnabled);
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mResEnabled);
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mResEnabledInHierarchy);
	FIELD().PROTECTED().DEFAULT_VALUE(SceneStatus::NotInScene).NAME(mSceneStatus);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsAsset);
	FIELD().PROTECTED().NAME(mAssetId);
	FIELD().PROTECTED().NAME(mReferences);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mCopyVisitor);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(locked);
	FIELD().PUBLIC().NAME(lockedInHierarchy);
	FIELD().PUBLIC().NAME(onEnableChanged);
	FIELD().PUBLIC().NAME(onChanged);
	FIELD().PUBLIC().NAME(onParentChanged);
	FIELD().PUBLIC().NAME(onChildHierarchyChanged);
	FIELD().PUBLIC().NAME(onLockChanged);
	FIELD().PUBLIC().NAME(onNameChanged);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mLocked);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mResLocked);
}
END_META;
CLASS_METHODS_META(o2::Actor)
{

	typedef const Map<const Actor*, Actor*>& _tmp1;
	typedef const Map<const Component*, Component*>& _tmp2;
	typedef Map<String, Actor*> _tmp3;
	typedef Map<String, Component*> _tmp4;
	typedef Map<const Actor*, Actor*>& _tmp5;
	typedef Map<const Component*, Component*>& _tmp6;

	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(ActorCreateMode);
	FUNCTION().PUBLIC().CONSTRUCTOR(const ActorAssetRef&, ActorCreateMode);
	FUNCTION().PUBLIC().CONSTRUCTOR(Vector<Component*>, ActorCreateMode);
	FUNCTION().PUBLIC().CONSTRUCTOR(const Actor&, ActorCreateMode);
	FUNCTION().PUBLIC().CONSTRUCTOR(const Actor&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, FixedUpdate, float);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, UpdateChildren, float);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, FixedUpdateChildren, float);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, UpdateTransform);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, UpdateSelfTransform);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, UpdateChildrenTransforms);
	FUNCTION().PUBLIC().SIGNATURE(void, SetName, const String&);
	FUNCTION().PUBLIC().SIGNATURE(const String&, GetName);
	FUNCTION().PUBLIC().SIGNATURE(SceneUID, GetID);
	FUNCTION().PUBLIC().SIGNATURE(void, SetID, SceneUID);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, GenerateNewID, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(UID, GetAssetID);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsAsset);
	FUNCTION().PUBLIC().SIGNATURE(void, SetPrototype, ActorAssetRef);
	FUNCTION().PUBLIC().SIGNATURE(ActorAssetRef, GetPrototype);
	FUNCTION().PUBLIC().SIGNATURE(ActorAssetRef, GetPrototypeDirectly);
	FUNCTION().PUBLIC().SIGNATURE(ActorRef, GetPrototypeLink);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, AddToScene);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveFromScene, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsOnScene);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Enable);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Disable);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabled);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsResEnabled);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsEnabledInHierarchy);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetParent, Actor*, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, GetParent);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetIndexInSiblings, int);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, AddChild, Actor*);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, AddChildren, const Vector<Actor*>&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, AddChild, Actor*, int);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, GetChild, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, FindChild, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, FindChild, const Function<bool(const Actor* child)>&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const Vector<Actor*>&, GetChildren);
	FUNCTION().PUBLIC().SIGNATURE(void, GetAllChildrenActors, Vector<Actor*>&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveChild, Actor*, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveAllChildren, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, FindActorById, SceneUID);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Component*, AddComponent, Component*);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveComponent, Component*, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveAllComponents);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Component*, GetComponent, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Component*, GetComponent, const Type*);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Component*, GetComponent, SceneUID);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const Vector<Component*>&, GetComponents);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLayer, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(SceneLayer*, GetLayer);
	FUNCTION().PUBLIC().SIGNATURE(const String&, GetLayerName);
	FUNCTION().PUBLIC().SIGNATURE(void, SerializeBasicOverride, DataValue&);
	FUNCTION().PUBLIC().SIGNATURE(void, DeserializeBasicOverride, const DataValue&);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(void, SetDefaultCreationMode, ActorCreateMode);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(ActorCreateMode, GetDefaultCreationMode);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, IsModeOnScene, ActorCreateMode);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuGroup);
	FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransform*, SceneStatus, const String&, bool, bool, bool, bool, const String&, SceneLayer*, SceneUID, UID);
	FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransform*, ActorCreateMode);
	FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransform*, const ActorAssetRef&, ActorCreateMode);
	FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransform*, Vector<Component*>, ActorCreateMode);
	FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransform*, const Actor&, ActorCreateMode);
	FUNCTION().PROTECTED().SIGNATURE(void, CopyFields, Vector<const FieldInfo*>&, IObject*, IObject*, Vector<Actor**>&, Vector<Component**>&, Vector<ISerializable*>&);
	FUNCTION().PROTECTED().SIGNATURE(void, CollectFixingFields, Component*, Vector<Component**>&, Vector<Actor**>&);
	FUNCTION().PROTECTED().SIGNATURE(void, GetComponentFields, Component*, Vector<const FieldInfo*>&);
	FUNCTION().PROTECTED().SIGNATURE(void, FixComponentFieldsPointers, const Vector<Actor**>&, const Vector<Component**>&, _tmp1, _tmp2);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateResEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateResEnabledInHierarchy);
	FUNCTION().PROTECTED().SIGNATURE(void, SerializeRaw, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, DeserializeRaw, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, SerializeWithProto, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, DeserializeWithProto, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(_tmp3, GetAllChilds);
	FUNCTION().PROTECTED().SIGNATURE(_tmp4, GetAllComponents);
	FUNCTION().PROTECTED().SIGNATURE(void, SetParentProp, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnStart);
	FUNCTION().PROTECTED().SIGNATURE(void, OnUpdate, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnFixedUpdate, float);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnableInHierarchyChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnParentChanged, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnChildAdded, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnChildRemoved, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnLayerChanged, SceneLayer*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnComponentAdded, Component*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnComponentRemoving, Component*);
	FUNCTION().PUBLIC().SIGNATURE(void, ReflectValue, ScriptValue&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLocked, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, Lock);
	FUNCTION().PUBLIC().SIGNATURE(void, Unlock);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsLocked);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsLockedInHierarchy);
	FUNCTION().PUBLIC().SIGNATURE(void, BreakPrototypeLink);
	FUNCTION().PUBLIC().SIGNATURE(void, ApplyChangesToPrototype);
	FUNCTION().PUBLIC().SIGNATURE(void, RevertToPrototype);
	FUNCTION().PUBLIC().SIGNATURE(ActorAssetRef, MakePrototype);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsLinkedToActor, Actor*);
	FUNCTION().PUBLIC().SIGNATURE(Actor*, FindLinkedActor, Actor*);
	FUNCTION().PUBLIC().SIGNATURE(const SceneEditableObject*, GetEditableLink);
	FUNCTION().PUBLIC().SIGNATURE(Vector<SceneEditableObject*>, GetEditableChildren);
	FUNCTION().PUBLIC().SIGNATURE(SceneEditableObject*, GetEditableParent);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEditableParent, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, AddEditableChild, SceneEditableObject*, int);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsDisabling);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsLocking);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsTransforming);
	FUNCTION().PUBLIC().SIGNATURE(Basis, GetTransform);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTransform, const Basis&);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsPivot);
	FUNCTION().PUBLIC().SIGNATURE(void, SetPivot, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2F, GetPivot);
	FUNCTION().PUBLIC().SIGNATURE(void, OnChanged);
	FUNCTION().PUBLIC().SIGNATURE(void, OnLockChanged);
	FUNCTION().PUBLIC().SIGNATURE(void, OnNameChanged);
	FUNCTION().PUBLIC().SIGNATURE(void, OnChildrenChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, CopyActorChangedFields, Actor*, Actor*, Actor*, Vector<Actor*>&, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, SeparateActors, Vector<Actor*>&);
	FUNCTION().PROTECTED().SIGNATURE(void, ProcessReverting, Actor*, const Actor*, const Vector<Actor*>&, Vector<Actor**>&, Vector<Component**>&, _tmp5, _tmp6, Vector<ISerializable*>&);
	FUNCTION().PROTECTED().SIGNATURE(void, GetDifferences, ActorDifferences&);
	FUNCTION().PROTECTED().SIGNATURE(void, BeginMakePrototype);
	FUNCTION().PROTECTED().SIGNATURE(void, BeginInstantiatePrototype);
	FUNCTION().PROTECTED().SIGNATURE(void, SetProtytypeDummy, ActorAssetRef);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateLocking);
}
END_META;
