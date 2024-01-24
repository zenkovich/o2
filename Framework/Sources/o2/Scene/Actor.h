#pragma once

#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Scene/ActorCreationMode.h"
#include "o2/Scene/ActorRef.h"
#include "o2/Scene/ActorTransform.h"
#include "o2/Scene/ComponentRef.h"
#include "o2/Scene/ISceneDrawable.h"
#include "o2/Scene/SceneLayerRef.h"
#include "o2/Scene/Tags.h"
#include "o2/Utils/Editor/Attributes/AnimatableAttribute.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"
#include "o2/Utils/Editor/SceneEditableObject.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/UID.h"

#if IS_SCRIPTING_SUPPORTED
#include "o2/Scripts/ScriptValue.h"
#endif

namespace o2
{
    class Component;
    class DrawableComponent;
    class Scene;

#if IS_EDITOR
    typedef SceneEditableObject ActorBase;

    struct ActorDifferences;

#define OPTIONAL_OVERRIDE override
#else
    typedef ISerializable ActorBase;

#define OPTIONAL_OVERRIDE
#endif

    // ---------------------------------------------------------------------------------------------
    // Scene actor. This is a basic scene object. All other scene object types is derived from this.
    // It has an unique id, name, tags, transformation, components and children actors.
    // Can be created from prototype of other actor. 
    // When editor pragma is enabled, it is derived from SceneEditableObject and support all editor 
    // features for editing actor
    // ---------------------------------------------------------------------------------------------
    class Actor: virtual public ActorBase, public ISceneDrawable
    {
    public:
        enum class State { Default, Initializing, Destroying, Destroyed };

    public:
        PROPERTIES(Actor);
        PROPERTY(Ref<ActorAsset>, prototype, SetPrototype, GetPrototype); // Prototype asset reference property @EDITOR_IGNORE

        GETTER(SceneUID, id, GetID);              // Actor unique id
        PROPERTY(String, name, SetName, GetName); // Actor name property @EDITOR_IGNORE @ANIMATABLE

        PROPERTY(bool, enabled, SetEnabled, IsEnabled);         // Is actor enabled property @EDITOR_IGNORE @ANIMATABLE
        GETTER(bool, enabledInHierarchy, IsEnabledInHierarchy); // Is actor enabled in hierarchy getter

        GETTER(Vector<Ref<Actor>>, children, GetChildren);         // Children array getter
        GETTER(Vector<Ref<Component>>, components, GetComponents); // Components array getter

        ACCESSOR(Ref<Actor>, child, String, GetChild, GetAllChilds);                 // Children accessor
        ACCESSOR(Ref<Component>, component, String, GetComponent, GetAllComponents); // Component accessor by type name

    public:
        TagGroup              tags;      // Tags group @EDITOR_IGNORE
        ActorTransform* const transform; // Transformation of actor @EDITOR_IGNORE @ANIMATABLE

    public:
        // Default constructor @SCRIPTABLE
        Actor(ActorCreateMode mode = ActorCreateMode::Default);

        // Actor constructor from prototype
        Actor(const Ref<ActorAsset>& prototype, ActorCreateMode mode = ActorCreateMode::Default);

        // Constructor with components
        Actor(Vector<Ref<Component>> components, ActorCreateMode mode = ActorCreateMode::Default);

        // Copy-constructor
        Actor(const Actor& other, ActorCreateMode mode);

        // Copy-constructor
        Actor(const Actor& other);

        // Destructor
        virtual ~Actor();

        // Assign operator
        Actor& operator=(const Actor& other);

        // Marks actor to destroy, will be destroyed on next frame @SCRIPTABLE
        void Destroy();

        // Draws actor ant its components
        void Draw() override;

        // Updates actor and components @SCRIPTABLE
        virtual void Update(float dt) OPTIONAL_OVERRIDE;

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

        // Sets actor's name @SCRIPTABLE
        void SetName(const String& name) OPTIONAL_OVERRIDE;

        // Returns name @SCRIPTABLE
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
        void SetPrototype(Ref<ActorAsset> asset);

        // Returns prototype from this or this parent @SCRIPTABLE
        Ref<ActorAsset> GetPrototype() const;

        // Returns prototype directly from only this
        Ref<ActorAsset> GetPrototypeDirectly() const;

        // Returns prototype link pointer
        Ref<Actor> GetPrototypeLink() const;

        // Sets scene layer
        void SetLayer(const Ref<SceneLayer>& layer);

        // Sets scene layer by name @SCRIPTABLE
        void SetLayer(const String& name);

        // Returns scene layer @SCRIPTABLE
        const Ref<SceneLayer>& GetLayer() const;

        // Includes to scene and now will be update and draw automatically from scene @SCRIPTABLE
        void AddToScene();

        // Excludes from scene and will not be update and draw automatically from scene @SCRIPTABLE
        void RemoveFromScene(bool keepEditorObjects = false);

        // Is actor on scene @SCRIPTABLE
        bool IsOnScene() const OPTIONAL_OVERRIDE;

        // Sets actor enabling @SCRIPTABLE
        virtual void SetEnabled(bool enabled) OPTIONAL_OVERRIDE;

        // Enables actor @SCRIPTABLE
        void Enable();

        // Disables actor @SCRIPTABLE
        void Disable();

        // Returns is actor enabled @SCRIPTABLE
        bool IsEnabled() const OPTIONAL_OVERRIDE;

        // Returns is really enabled @SCRIPTABLE
        bool IsResEnabled() const;

        // Returns is actor enabled in hierarchy @SCRIPTABLE
        bool IsEnabledInHierarchy() const OPTIONAL_OVERRIDE;

        // Sets parent @SCRIPTABLE
        void SetParent(const Ref<Actor>& actor, bool worldPositionStays = true, int idx = -1);

        // Returns parent @SCRIPTABLE
        const WeakRef<Actor>& GetParent() const;

        // Sets index position in parent or scene @SCRIPTABLE
        virtual void SetIndexInSiblings(int index) OPTIONAL_OVERRIDE;

        // Add child actor @SCRIPTABLE
        virtual Ref<Actor> AddChild(const Ref<Actor>& actor);

        // Add children actors @SCRIPTABLE
        void AddChildren(const Vector<Ref<Actor>>& actors);

        // Add child actor @SCRIPTABLE
        virtual Ref<Actor> AddChild(const Ref<Actor>& actor, int index);

        // Returns child actor by path (ex "root/some node/other node/target node") @SCRIPTABLE
        Ref<Actor> GetChild(const String& path) const;

        // Returns child actor by name @SCRIPTABLE
        Ref<Actor> FindChild(const String& name) const;

        // Returns child actor by predicate
        Ref<Actor> FindChild(const Function<bool(const Ref<Actor>& child)>& pred) const;

        // Returns child actor by path (ex "root/some node/other node/target node")
        template<typename _type>
        Ref<_type> GetChildByType(const String& path) const;

        // Returns child actor by name
        template<typename _type>
        Ref<_type> FindChildByTypeAndName(const String& name) const;

        // Searches child with specified type
        template<typename _type>
        Ref<_type> FindChildByType(bool searchInChildren = true);

        // Returns children array @SCRIPTABLE
		const Vector<Ref<Actor>>& GetChildren() const;

        // Returns all children actors with their children
        virtual void GetAllChildrenActors(Vector<Ref<Actor>>& actors);

        // Removes child and destroys him if needed @SCRIPTABLE
        void RemoveChild(Actor* actor, bool withEvent = true);

        // Removes and destroys all childs @SCRIPTABLE
        void RemoveAllChildren();

        // Searches actor with id in this and this children @SCRIPTABLE
        virtual Ref<Actor> FindActorById(SceneUID id);

        // And new component
        template<typename _type>
        Ref<_type> AddComponent();

        // Adds new component
        Ref<Component> AddComponent(const Ref<Component>& component);

        // Removes component @SCRIPTABLE
        void RemoveComponent(const Ref<Component>& component);

        // Removes all components @SCRIPTABLE
        void RemoveAllComponents();

        // Returns component with type name @SCRIPTABLE
        Ref<Component> GetComponent(const String& typeName);

        // Returns component with type
        Ref<Component> GetComponent(const Type* type);

        // Returns component by id
        Ref<Component> GetComponent(SceneUID id);

        // Returns component with type
        template<typename _type>
        Ref<_type> GetComponent() const;

#if IS_SCRIPTING_SUPPORTED
        // Returns component with type name @SCRIPTABLE
        Ref<Component> GetComponent(const ScriptValue& typeValue);
#endif

        // Returns component with type in this and children
        template<typename _type>
        Ref<_type> GetComponentInChildren() const;

        // Return all components by type
        template<typename _type>
        Vector<Ref<_type>> GetComponents() const;

        // Returns all components by type in this and children
        template<typename _type>
        Vector<Ref<_type>> GetComponentsInChildren() const;

        // Returns all components @SCRIPTABLE
        const Vector<Ref<Component>>& GetComponents() const;

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
        struct ICopyVisitor: public RefCounterable
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

        Ref<SceneLayer> mSceneLayer; // Scene layer @SERIALIZABLE @EDITOR_IGNORE

        Ref<ActorAsset> mPrototype;     // Prototype asset
        Ref<Actor>      mPrototypeLink; // Prototype link actor. Links to source actor from prototype

        WeakRef<Actor>     mParent;   // Parent actor 
        Vector<Ref<Actor>> mChildren; // Children actors 

        Vector<Ref<Component>> mComponents; // Components vector 

        Vector<Ref<DrawableComponent>> mDrawComponents; // Drawable components vector

        // Actor can be logically disabled, but actually stay enabled and be in transition to really disabled state
        // mResEnabled and mResEnabledInHierarchy are disabled by default to get enabled after initialization
        bool mEnabled = true;                // Is actor logically enabled. 
        bool mResEnabled = false;            // Is actor really enabled. 
        bool mResEnabledInHierarchy = false; // Is actor enabled in hierarchy

        State mState = State::Default; // Actor on scene status

        bool mIsOnScene = false; // Is actor on scene

        bool mIsAsset = false; // Is this actor cached asset
        UID  mAssetId;         // Source asset id

        mutable Ref<ICopyVisitor> mCopyVisitor; // Copy visitor. Called when copying actor and calls on actor or component copying

    protected:
        // Base actor constructor with transform
        Actor(ActorTransform* transform, bool onScene = true, const String& name = "unnamed", bool enabled = true, 
              SceneUID id = Math::Random(), UID assetId = UID(0));

        // Default constructor with transform
        Actor(ActorTransform* transform, ActorCreateMode mode = ActorCreateMode::Default);

        // Actor constructor from prototype with transform
        Actor(ActorTransform* transform, const Ref<ActorAsset>& prototype, ActorCreateMode mode = ActorCreateMode::Default);

        // Constructor with components with transform
        Actor(ActorTransform* transform, Vector<Ref<Component>> components, ActorCreateMode mode = ActorCreateMode::Default);

        // Copy-constructor with transform
        Actor(ActorTransform* transform, const Actor& other, ActorCreateMode mode = ActorCreateMode::Default);
        
        // Checks that copy visitor finished work, calls it finalization
        void CheckCopyVisitorFinalization() const;

        // Updates result read enable flag
        virtual void UpdateResEnabled(bool withChildren = true);

        // Updates enabling
        virtual void UpdateResEnabledInHierarchy(bool withChildren = true);

        // Regular serializing without prototype
        virtual void SerializeRaw(DataValue& node) const;

        // Regular deserializing without prototype
        virtual void DeserializeRaw(const DataValue& node);

        // Regular serializing with prototype
        virtual void SerializeWithProto(DataValue& node) const;

        // Regular deserializing with prototype
        virtual void DeserializeWithProto(const DataValue& node);

        // Returns current scene layer
        Ref<SceneLayer> GetSceneDrawableSceneLayer() const override;

        // Returns parent scene drawable
        Ref<ISceneDrawable> GetParentDrawable() override;

        // Returns the index in the parent's list of children, used to sort the rendering
        int GetIndexInParentDrawable() const override;

        // Returns dictionary of all children by names
        Map<String, Ref<Actor>> GetAllChilds();

        // Returns dictionary of all components by type names
        Map<String, Ref<Component>> GetAllComponents();

        // Calls events before destroying: OnDisable, OnDestroy, OnRemoveFromScene
        void OnBeforeDestroy();

        // Called when actor has added to scene 
        virtual void OnAddToScene();

        // Called when actor has removed from scene
        virtual void OnRemoveFromScene();

        // Called when actor and scene is initialized
        virtual void OnInitialized();

        // Called on first update
        virtual void OnStart();

        // Called when actor will be destroyed
        virtual void OnDestroy();

        // Called on update with frame dt
        virtual void OnUpdate(float dt);

        // Called on fixed update with fixed dt
        virtual void OnFixedUpdate(float dt);

        // Called when actor enabled in hierarchy
        virtual void OnEnabled();

        // Called when actor disabled in hierarchy
        virtual void OnDisabled();

        // Called when transformation was updated
        virtual void OnTransformUpdated();

        // Called when parent changed
        virtual void OnParentChanged(const Ref<Actor>& oldParent);

        // Called when child changed
        virtual void OnChildrenChanged() OPTIONAL_OVERRIDE;

        // Called when child actor was added
        virtual void OnChildAdded(const Ref<Actor>& child);

        // Called when child actor was removed
        virtual void OnChildRemoved(Actor* child);

        // Called when new component has added to actor
        virtual void OnComponentAdded(const Ref<Component>& component);

        // Called when component going to be removed from actor
        virtual void OnComponentRemoving(const Ref<Component>& component);

#if IS_SCRIPTING_SUPPORTED
    public:
        // Returns transform for scripting @SCRIPTABLE
        ActorTransform* GetTransform();
#endif

#if IS_EDITOR
    public:
        PROPERTY(bool, locked, SetLocked, IsLocked);          // Is actor locked property @EDITOR_IGNORE
        GETTER(bool, lockedInHierarchy, IsLockedInHierarchy); // Is actor locked in hierarchy getter

    public:
        Function<void(bool)>       onEnableChanged;         // Enable changing event @EDITOR_IGNORE
        Function<void()>           onChanged;               // Something in actor change event @EDITOR_IGNORE
        Function<void(Ref<Actor>)> onParentChanged;         // Actor change parent event @EDITOR_IGNORE
		Function<void()>           onChildHierarchyChanged; // Actor childs hierarchy change event @EDITOR_IGNORE
		Function<void(bool)>       onLockChanged;           // Locking changing event @EDITOR_IGNORE
		Function<void()>           onNameChanged;           // Name changing event @EDITOR_IGNORE

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
        Ref<ActorAsset> MakePrototype();

        // Returns is this linked to specified actor with depth links search
        bool IsLinkedToActor(const Ref<Actor>& actor) const;

        // Searches actor in this, what linked to linkActor
        Ref<Actor> FindLinkedActor(const Ref<Actor>& linkActor);

        // Returns pointer to owner editable object
        Ref<SceneEditableObject> GetEditableOwner() override;

        // Returns object's link to prototype
        Ref<SceneEditableObject> GetEditableLink() const override;

        // Returns list of object's children
        Vector<Ref<SceneEditableObject>> GetEditableChildren() const override;

        // Returns object's parent object. Return nullptr when it is a root scene object
        Ref<SceneEditableObject> GetEditableParent() const override;

        // Sets parent object. nullptr means make this object as root. idx is place in parent children. idx == -1 means last
        void SetEditableParent(const Ref<SceneEditableObject>& object, int idx = -1) override;

        // Adds child. idx is place in parent children. idx == -1 means last
        void AddEditableChild(const Ref<SceneEditableObject>& object, int idx = -1) override;

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

        // Called when some changed in actor
        void OnChanged() override;

        // Called when actor's locking was changed
        void OnLockChanged() override;

        // Called when actor's name was changed
        void OnNameChanged() override;

        // Called when actor's parent was changed
        void OnEditableParentChanged(const Ref<SceneEditableObject>& oldParent) override;

    protected:
        bool mLocked = false;    // Is actor locked @SERIALIZABLE
        bool mResLocked = false; // Is actor locked in hierarchy @SERIALIZABLE

    protected:
        // Applies basic actor fields and transform from source to dest  
        void CopyActorChangedFields(Actor* source, Actor* changed, Actor* dest, Vector<Actor*>& allDestChilds, bool withTransform);

        // Separates children actors to linear array, removes child and parent links
        void SeparateActors(Vector<Ref<Actor>>& separatedActors);

        // Processes reverting actor
        void ProcessReverting(Actor* dest, const Actor* source, const Vector<Ref<Actor>>& separatedActors,
                              Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers,
                              Map<const Actor*, Actor*>& actorsMap,
                              Map<const Component*, Component*>& componentsMap,
                              Vector<ISerializable*>& serializableObjects);

        // Copies fields from source to dest
        void CopyFields(Vector<const FieldInfo*>& fields, IObject* source, IObject* dest,
                        Vector<Actor**>& actorsPointers,
                        Vector<Component**>& componentsPointers,
                        Vector<ISerializable*>& serializableObjects);

        // Collects fixing actors and components pointers in new component
        void CollectFixingFields(Component* newComponent, Vector<Component**>& componentsPointers,
                                 Vector<Actor**>& actorsPointers);

        // Fixes actors and components pointers by actors map
        void FixComponentFieldsPointers(const Vector<Actor**>& actorsPointers,
                                        const Vector<Component**>& componentsPointers,
                                        const Map<const Actor*, Actor*>& actorsMap,
                                        const Map<const Component*, Component*>& componentsMap);

        // Collects component field, except Component class fields
        void GetComponentFields(const Ref<Component>& component, Vector<const FieldInfo*>& fields);

        // Collects differences between this and prototype
        virtual void GetDifferences(ActorDifferences& differences) const OPTIONAL_OVERRIDE;

        // Called before making prototype from this object
        void BeginMakePrototype() const override;

        // Called before instantiate from this object
        void BeginInstantiatePrototype() const override;

        // Not using prototype setter
        void SetProtytypeDummy(Ref<ActorAsset> asset);

        // Updates locking
        void UpdateLocking();

#endif // IS_EDITOR

        friend class ActorAsset;
        friend class ActorRefResolver;
        friend class ActorTransform;
        friend class BaseActorRef;
        friend class BaseComponentRef;
        friend class Component;
        friend class DrawableComponent;
        friend class ISceneDrawable;
        friend class Scene;
        friend class Tag;
        friend class Widget;
    };
}

#include "o2/Scene/Component.h"
#include "o2/Scene/ActorRefResolver.h"

namespace o2
{
    template<typename _type>
    Ref<_type> Actor::FindChildByType(bool searchInChildren /*= true*/)
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
    Ref<_type> Actor::GetChildByType(const String& path) const
    {
        return DynamicCast<_type>(GetChild(path));
    }

    template<typename _type>
    Ref<_type> Actor::FindChildByTypeAndName(const String& name) const
    {
        return DynamicCast<_type>(FindChild(name));
    }

    template<typename _type>
    Vector<Ref<_type>> Actor::GetComponentsInChildren() const
    {
        auto res = GetComponents<_type>();

        for (auto child : mChildren)
            res.Add(child->GetComponentsInChildren<_type>());

        return res;
    }

    template<typename _type>
    Ref<_type> Actor::GetComponentInChildren() const
    {
        auto res = GetComponent<_type>();

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
    Ref<_type> Actor::GetComponent() const
    {
        for (auto comp : mComponents)
        {
            if (comp->GetType().IsBasedOn(TypeOf(_type)))
                return DynamicCast<_type>(comp);
        }

        return nullptr;
    }

    template<typename _type>
    Vector<Ref<_type>> Actor::GetComponents() const
    {
        Vector<Ref<_type>> res;
        for (auto comp : mComponents)
        {
            if (auto typeComp = DynamicCast<_type>(comp))
                res.Add(typeComp);
        }

        return res;
    }

    template<typename _type>
    Ref<_type> Actor::AddComponent()
    {
        if (GetComponent<_type>() != nullptr)
            return nullptr;

        auto newComponent = mmake<_type>();
        AddComponent(newComponent);

        return newComponent;
    }

}
// --- META ---

PRE_ENUM_META(o2::Actor::State);

CLASS_BASES_META(o2::Actor)
{
    BASE_CLASS(ActorBase);
    BASE_CLASS(o2::ISceneDrawable);
}
END_META;
CLASS_FIELDS_META(o2::Actor)
{
    FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(prototype);
    FIELD().PUBLIC().NAME(id);
    FIELD().PUBLIC().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(name);
    FIELD().PUBLIC().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(enabled);
    FIELD().PUBLIC().NAME(enabledInHierarchy);
    FIELD().PUBLIC().NAME(children);
    FIELD().PUBLIC().NAME(components);
    FIELD().PUBLIC().NAME(child);
    FIELD().PUBLIC().NAME(component);
    FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(tags);
    FIELD().PUBLIC().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(transform);
    FIELD().PROTECTED().NAME(mId);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mName);
    FIELD().PROTECTED().EDITOR_IGNORE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mSceneLayer);
    FIELD().PROTECTED().NAME(mPrototype);
    FIELD().PROTECTED().NAME(mPrototypeLink);
    FIELD().PROTECTED().NAME(mParent);
    FIELD().PROTECTED().NAME(mChildren);
    FIELD().PROTECTED().NAME(mComponents);
    FIELD().PROTECTED().NAME(mDrawComponents);
    FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mEnabled);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mResEnabled);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mResEnabledInHierarchy);
    FIELD().PROTECTED().DEFAULT_VALUE(State::Default).NAME(mState);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsOnScene);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsAsset);
    FIELD().PROTECTED().NAME(mAssetId);
    FIELD().PROTECTED().NAME(mCopyVisitor);
#if  IS_EDITOR
    FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(locked);
    FIELD().PUBLIC().NAME(lockedInHierarchy);
    FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onEnableChanged);
    FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onChanged);
    FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onParentChanged);
    FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onChildHierarchyChanged);
    FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onLockChanged);
    FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onNameChanged);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mLocked);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mResLocked);
#endif
}
END_META;
CLASS_METHODS_META(o2::Actor)
{

    typedef Map<String, Ref<Actor>> _tmp1;
    typedef Map<String, Ref<Component>> _tmp2;
    typedef Map<const Actor*, Actor*>& _tmp3;
    typedef Map<const Component*, Component*>& _tmp4;
    typedef const Map<const Actor*, Actor*>& _tmp5;
    typedef const Map<const Component*, Component*>& _tmp6;

    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(ActorCreateMode);
    FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<ActorAsset>&, ActorCreateMode);
    FUNCTION().PUBLIC().CONSTRUCTOR(Vector<Ref<Component>>, ActorCreateMode);
    FUNCTION().PUBLIC().CONSTRUCTOR(const Actor&, ActorCreateMode);
    FUNCTION().PUBLIC().CONSTRUCTOR(const Actor&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Destroy);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, FixedUpdate, float);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, UpdateChildren, float);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, FixedUpdateChildren, float);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, UpdateTransform);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, UpdateSelfTransform);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, UpdateChildrenTransforms);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetName, const String&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const String&, GetName);
    FUNCTION().PUBLIC().SIGNATURE(SceneUID, GetID);
    FUNCTION().PUBLIC().SIGNATURE(void, SetID, SceneUID);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, GenerateNewID, bool);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(UID, GetAssetID);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsAsset);
    FUNCTION().PUBLIC().SIGNATURE(void, SetPrototype, Ref<ActorAsset>);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Ref<ActorAsset>, GetPrototype);
    FUNCTION().PUBLIC().SIGNATURE(Ref<ActorAsset>, GetPrototypeDirectly);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Actor>, GetPrototypeLink);
    FUNCTION().PUBLIC().SIGNATURE(void, SetLayer, const Ref<SceneLayer>&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetLayer, const String&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const Ref<SceneLayer>&, GetLayer);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, AddToScene);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveFromScene, bool);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsOnScene);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetEnabled, bool);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Enable);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Disable);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsEnabled);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsResEnabled);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsEnabledInHierarchy);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetParent, const Ref<Actor>&, bool, int);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const WeakRef<Actor>&, GetParent);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetIndexInSiblings, int);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Ref<Actor>, AddChild, const Ref<Actor>&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, AddChildren, const Vector<Ref<Actor>>&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Ref<Actor>, AddChild, const Ref<Actor>&, int);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Ref<Actor>, GetChild, const String&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Ref<Actor>, FindChild, const String&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Actor>, FindChild, const Function<bool(const Ref<Actor>& child)>&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const Vector<Ref<Actor>>&, GetChildren);
    FUNCTION().PUBLIC().SIGNATURE(void, GetAllChildrenActors, Vector<Ref<Actor>>&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveChild, Actor*, bool);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveAllChildren);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Ref<Actor>, FindActorById, SceneUID);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Component>, AddComponent, const Ref<Component>&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveComponent, const Ref<Component>&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveAllComponents);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Ref<Component>, GetComponent, const String&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Component>, GetComponent, const Type*);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Component>, GetComponent, SceneUID);
#if  IS_SCRIPTING_SUPPORTED
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Ref<Component>, GetComponent, const ScriptValue&);
#endif
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const Vector<Ref<Component>>&, GetComponents);
    FUNCTION().PUBLIC().SIGNATURE(void, SerializeBasicOverride, DataValue&);
    FUNCTION().PUBLIC().SIGNATURE(void, DeserializeBasicOverride, const DataValue&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(void, SetDefaultCreationMode, ActorCreateMode);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(ActorCreateMode, GetDefaultCreationMode);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, IsModeOnScene, ActorCreateMode);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuGroup);
    FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransform*, bool, const String&, bool, SceneUID, UID);
    FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransform*, ActorCreateMode);
    FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransform*, const Ref<ActorAsset>&, ActorCreateMode);
    FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransform*, Vector<Ref<Component>>, ActorCreateMode);
    FUNCTION().PROTECTED().CONSTRUCTOR(ActorTransform*, const Actor&, ActorCreateMode);
    FUNCTION().PROTECTED().SIGNATURE(void, CheckCopyVisitorFinalization);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateResEnabled, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateResEnabledInHierarchy, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, SerializeRaw, DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, DeserializeRaw, const DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, SerializeWithProto, DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, DeserializeWithProto, const DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(Ref<SceneLayer>, GetSceneDrawableSceneLayer);
    FUNCTION().PROTECTED().SIGNATURE(Ref<ISceneDrawable>, GetParentDrawable);
    FUNCTION().PROTECTED().SIGNATURE(int, GetIndexInParentDrawable);
    FUNCTION().PROTECTED().SIGNATURE(_tmp1, GetAllChilds);
    FUNCTION().PROTECTED().SIGNATURE(_tmp2, GetAllComponents);
    FUNCTION().PROTECTED().SIGNATURE(void, OnBeforeDestroy);
    FUNCTION().PROTECTED().SIGNATURE(void, SetParentProp, const Ref<Actor>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
    FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
    FUNCTION().PROTECTED().SIGNATURE(void, OnInitialized);
    FUNCTION().PROTECTED().SIGNATURE(void, OnStart);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDestroy);
    FUNCTION().PROTECTED().SIGNATURE(void, OnUpdate, float);
    FUNCTION().PROTECTED().SIGNATURE(void, OnFixedUpdate, float);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
    FUNCTION().PROTECTED().SIGNATURE(void, OnParentChanged, const Ref<Actor>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnChildrenChanged);
    FUNCTION().PROTECTED().SIGNATURE(void, OnChildAdded, const Ref<Actor>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnChildRemoved, const Ref<Actor>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnComponentAdded, const Ref<Component>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnComponentRemoving, const Ref<Component>&);
#if  IS_SCRIPTING_SUPPORTED
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(ActorTransform*, GetTransform);
#endif
#if  IS_EDITOR
    FUNCTION().PUBLIC().SIGNATURE(void, SetLocked, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, Lock);
    FUNCTION().PUBLIC().SIGNATURE(void, Unlock);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsLocked);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsLockedInHierarchy);
    FUNCTION().PUBLIC().SIGNATURE(void, BreakPrototypeLink);
    FUNCTION().PUBLIC().SIGNATURE(void, ApplyChangesToPrototype);
    FUNCTION().PUBLIC().SIGNATURE(void, RevertToPrototype);
    FUNCTION().PUBLIC().SIGNATURE(Ref<ActorAsset>, MakePrototype);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsLinkedToActor, const Ref<Actor>&);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Actor>, FindLinkedActor, const Ref<Actor>);
    FUNCTION().PUBLIC().SIGNATURE(Ref<SceneEditableObject>, GetEditableOwner);
    FUNCTION().PUBLIC().SIGNATURE(const Ref<SceneEditableObject>&, GetEditableLink);
    FUNCTION().PUBLIC().SIGNATURE(Vector<Ref<SceneEditableObject>>, GetEditableChildren);
    FUNCTION().PUBLIC().SIGNATURE(Ref<SceneEditableObject>, GetEditableParent);
    FUNCTION().PUBLIC().SIGNATURE(void, SetEditableParent, const Ref<SceneEditableObject>&, int);
    FUNCTION().PUBLIC().SIGNATURE(void, AddEditableChild, const Ref<SceneEditableObject>&, int);
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
    FUNCTION().PUBLIC().SIGNATURE(void, OnEditableParentChanged, const Ref<SceneEditableObject>&);
    FUNCTION().PROTECTED().SIGNATURE(void, CopyActorChangedFields, Actor*, Actor*, Actor*, Vector<Actor*>&, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, SeparateActors, Vector<Ref<Actor>>&);
    FUNCTION().PROTECTED().SIGNATURE(void, ProcessReverting, Actor*, const Actor*, const Vector<Actor*>&, Vector<Actor**>&, Vector<Component**>&, _tmp3, _tmp4, Vector<ISerializable*>&);
    FUNCTION().PROTECTED().SIGNATURE(void, CopyFields, Vector<const FieldInfo*>&, IObject*, IObject*, Vector<Actor**>&, Vector<Component**>&, Vector<ISerializable*>&);
    FUNCTION().PROTECTED().SIGNATURE(void, CollectFixingFields, Component*, Vector<Component**>&, Vector<Actor**>&);
    FUNCTION().PROTECTED().SIGNATURE(void, FixComponentFieldsPointers, const Vector<Actor**>&, const Vector<Component**>&, _tmp5, _tmp6);
    FUNCTION().PROTECTED().SIGNATURE(void, GetComponentFields, const Ref<Component>&, Vector<const FieldInfo*>&);
    FUNCTION().PROTECTED().SIGNATURE(void, GetDifferences, ActorDifferences&);
    FUNCTION().PROTECTED().SIGNATURE(void, BeginMakePrototype);
    FUNCTION().PROTECTED().SIGNATURE(void, BeginInstantiatePrototype);
    FUNCTION().PROTECTED().SIGNATURE(void, SetProtytypeDummy, Ref<ActorAsset>);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateLocking);
#endif
}
END_META;
// --- END META ---
