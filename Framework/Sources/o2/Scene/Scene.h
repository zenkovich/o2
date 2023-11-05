#pragma once

#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/String.h"
#include "o2/Utils/Types/UID.h"
#include "o2/Utils/Property.h"

// Scene graph access macros
#define o2Scene o2::Scene::Instance()

namespace o2
{
    class Actor;
    class CameraActor;
    class Component;
    class SceneLayer;
    class Tag;

#if IS_EDITOR
    class SceneEditableObject;
#endif

    // -------------------------------------------------------
    // Actors scene. Contains and manages actors, tags, layers
    // -------------------------------------------------------
    class Scene : public Singleton<Scene>, public IObject
    {
    public:
        // Returns scene log stream
        const LogStream& GetLogStream() const;

        // Checks is layer exists
        bool HasLayer(const String& name) const;

        // Returns layer by name
        SceneLayer* GetLayer(const String& name);

        // Returns default layer
        SceneLayer* GetDefaultLayer() const;

        // Adds layer with name
        SceneLayer* AddLayer(const String& name);

        // Removes layer
        void RemoveLayer(SceneLayer* layer);

        // Removes layer by name
        void RemoveLayer(const String& name);

        // Sets layer order
        void SetLayerOrder(SceneLayer* layer, int idx);

        // Returns layers array
        const Vector<SceneLayer*>& GetLayers() const;

        // Returns layers names array
        Vector<String> GetLayersNames() const;

        // Returns layers map by name
        const Map<String, SceneLayer*>& GetLayersMap() const;

        // Returns tag with name
        Tag* GetTag(const String& name) const;

        // Adds tag with name
        Tag* AddTag(const String& name);

        // Removes tag
        void RemoveTag(Tag* tag);

        // Removes tag with name
        void RemoveTag(const String& name);

        // Returns tags array
        const Vector<Tag*>& GetTags() const;

        // Returns root actors
        const Vector<Actor*>& GetRootActors() const;

        // Returns root actors
        Vector<Actor*>& GetRootActors();

        // Returns all actors
        const Vector<Actor*>& GetAllActors() const;

        // Returns all actors
        Vector<Actor*>& GetAllActors();

        // Returns actor by id
        Actor* GetActorByID(SceneUID id) const;

        // Returns asset actor by asset id. Tries to find in cache
        Actor* GetAssetActorByID(const UID& id);

        // Returns actor by path (ex "some node/other/target")
        Actor* FindActor(const String& path);

        // Returns actor with type in scene
        template<typename _type>
        _type* FindActorByType();

        // Returns cameras on scene
        const Vector<CameraActor*>& GetCameras() const;

        // Returns component with type in scene
        template<typename _type>
        _type* FindActorComponent();

        // Returns all components with type in scene
        template<typename _type>
        Vector<_type>* FindAllActorsComponents();

        // Removes all actors
        void Clear(bool keepDefaultLayer = true);

        // Clears assets cache
        void ClearCache();

        // Loads scene from file. If append is true, old actors will not be destroyed
        void Load(const String& path, bool append = false);

        // Loads scene from document. If append is true, old actors will not be destroyed
        void Load(const DataDocument& doc, bool append = false);

        // Saves scene into file
        void Save(const String& path);

        // Saves scene into document
        void Save(DataDocument& doc);

        // Draws scene drawable objects
        void Draw();

        // Updates root actors
        void Update(float dt);

        // Updates root actors with fixed delta time
        void FixedUpdate(float dt);

        // Updates just added actors and components
        void UpdateAddedEntities();

        // Updates actors transforms
        void UpdateTransforms();

        // Updates destroying actors and components
        void UpdateDestroyingEntities();

        // Adds actor to destroy list, will be removed at next frame
        void DestroyActor(Actor* actor);

        // Adds component to destroy list, will be removed at next frame
        void DestroyComponent(Component* component);

        IOBJECT(Scene);

    protected:
        LogStream* mLog = nullptr; // Scene log

        Vector<CameraActor*> mCameras; // List of cameras on scene

        Vector<Actor*> mRootActors; // Scene root actors        
        Vector<Actor*> mAllActors;  // All scene actors

        Map<SceneUID, Actor*> mActorsMap; // Actors map by uniquie ID

        Vector<Actor*> mAddedActors; // List of added on previous frame actors. Will receive OnAddToScene at current frame

        Vector<Actor*>     mStartActors;     // List of starting on current frame actors. Will receive OnStart at current frame
        Vector<Component*> mStartComponents; // List of starting on current frame components. Will receive OnStart at current frame

        Vector<Actor*>     mDestroyActors;     // List of destroying on current frame actors
        Vector<Component*> mDestroyComponents; // List of destroying on current frame components

        Map<String, SceneLayer*> mLayersMap;    // Layers by names map
        Vector<SceneLayer*>      mLayers;       // Scene layers
        SceneLayer* mDefaultLayer; // Default scene layer

        Vector<Tag*> mTags; // Scene tags

        Vector<ActorAssetRef> mCache; // Cached actors assets

    protected:
        // Called when actor added to scene, adds to scene defered
        static void OnActorCreated(Actor* actor);

        // Called when actor destroyed, removes from scene
        static void OnActorDestroy(Actor* actor);

        // Called when new actor was parented to another actor, removes it from added list
        static void OnNewActorParented(Actor* actor);

        // Called when actor unique id was changed; updates actors map
        static void OnActorIdChanged(Actor* actor, SceneUID prevId);

        // Called when actor adding to scene; registers in actors list and events list
        static void OnAddActorToScene(Actor* actor);

        // Called when actor removing from scene; unregisters from actors list and events list
        static void OnRemoveActorFromScene(Actor* actor, bool keepEditorObjects = false);

    protected:
        // Default constructor
        Scene();

        // Destructor
        ~Scene();

        // Draws cameras
        void DrawCameras();

        // Updates root actors and their children
        void UpdateActors(float dt);

        // Updates starting actors and components
        void UpdateStartingEntities();

        // Draws debug info for actor under cursor
        void DrawCursorDebugInfo();

        // Called when actor adding to scene; registers in actors list and events list
        void AddActorToScene(Actor* actor);

        // Called when actor removing from scene; unregisters from actors list and events list
        void RemoveActorFromScene(Actor* actor, bool keepEditorObjects = false);

        // Called when component added to actor, registers for calling OnAddOnScene
        void OnComponentAdded(Component* component);

        // Called when component removed, register for calling OnRemovFromScene
        void OnComponentRemoved(Component* component);

        // Called when scene layer renamed, updates layers map
        void OnLayerRenamed(SceneLayer* layer, const String& oldName);

        // Called when camera was added to scene
        void OnCameraAddedOnScene(CameraActor* camera);

        // Called when camera was removed from scene
        void OnCameraRemovedScene(CameraActor* camera);

        friend class Actor;
        friend class ActorRef;
        friend class Application;
        friend class CameraActor;
        friend class Component;
        friend class ComponentRef;
        friend class DrawableComponent;
        friend class SceneLayer;
        friend class Widget;
        friend class WidgetLayer;

#if IS_EDITOR          
    public:
        Function<void(SceneEditableObject*)> onAddedToScene;             // Actor added to scene event
        Function<void(SceneEditableObject*)> onRemovedFromScene;         // Actor removed from scene event
        Function<void(SceneEditableObject*)> onEnableChanged;            // Actor enable changing
        Function<void(SceneEditableObject*)> onLockChanged;                 // Actor locking change
        Function<void(SceneEditableObject*)> onNameChanged;                 // Actor name changing event
        Function<void(SceneEditableObject*)> onChildrenHierarchyChanged; // Actor childs hierarchy change event

        Function<void(const Vector<SceneEditableObject*>&)> onObjectsChanged; // Actors some change event

        Function<void()> onLayersListChanged; // Called when layer added, removed or renamed

    public:
        // Links actor recursively to prototypes and their parent prototypes
        static void LinkActorToPrototypesHierarchy(Actor* actor, ActorAssetRef proto);

        // Returns root editable objects
        Vector<SceneEditableObject*> GetRootEditableObjects();

        // Adds editable object to scene
        void AddEditableObjectToScene(SceneEditableObject* object);

        // Removes object from scene
        void RemoveEditableObjectFromScene(SceneEditableObject* object);

        // Removes object at end of frame
        void DestroyEditableObject(SceneEditableObject* object);

        // Returns all editable objects
        const Vector<SceneEditableObject*>& GetAllEditableObjects();

        // Returns current changed actors
        const Vector<SceneEditableObject*>& GetChangedObjects() const;

        // Returns drawn on last frame editable objects
        const Vector<SceneEditableObject*>& GetDrawnEditableObjects() const;

        // Returns actor by id
        SceneEditableObject* GetEditableObjectByID(SceneUID id) const;

        // Returns object's index in hierarchy
        int GetObjectHierarchyIdx(SceneEditableObject* object) const;

        // Reparent scene editableobjects to new parent at next of prevActor;
        void ReparentEditableObjects(const Vector<SceneEditableObject*>& objects,
                                     SceneEditableObject* newParent, SceneEditableObject* prevObject);

        // Checks is any actors was changed and calls OnChanged() if changed
        void CheckChangedObjects();

        // Returns cache of linked to prototypes actors
        Map<ActorAssetRef, Vector<Actor*>>& GetPrototypesLinksCache();

        // Called when scene started to draw. When scene draw started, drawn scene objects will be collected
        void BeginDrawingScene();

        // Called when scene finished draw. After scene objects will not be collected 
        void EndDrawingScene();

        // Called when object was created
        void OnObjectAddToScene(SceneEditableObject* object);

        // Called when object is destroying
        void OnObjectRemoveFromScene(SceneEditableObject* object);

        // Called when object was changed
        void OnObjectChanged(SceneEditableObject* object);

        // Called when object was drawn 
        void OnObjectDrawn(SceneEditableObject* object);

        // Called when created actor with prototype, updates cache
        void OnActorWithPrototypeCreated(Actor* actor);

        // Called when some actor created and linked to prototype, updates linked actors cache
        void OnActorLinkedToPrototype(ActorAssetRef& assetRef, Actor* actor);

        // Called when actor destroying or prototype link broken, updates cache
        void OnActorPrototypeBroken(Actor* actor);

    protected:
        Map<ActorAssetRef, Vector<Actor*>> mPrototypeLinksCache; // Cache of linked to prototypes actors

        Vector<SceneEditableObject*> mChangedObjects;    // Changed actors array
        Vector<SceneEditableObject*> mDestroyingObjects; // Destroying scene editable objects

        Vector<SceneEditableObject*> mEditableObjects;             // All scene editable objects
        mutable Map<SceneUID, SceneEditableObject*> mEditableObjectsByUID; // All scene editable objects by UID

        Vector<SceneEditableObject*> mDrawnObjects;           // List of drawn on last frame editable objects
        bool                         mIsDrawingScene = false; // Sets true when started drawing scene, and false when not

        friend class SceneEditableObject;
#endif
    };
}

#include "o2/Scene/Actor.h"

namespace o2
{
    template<typename _type>
    Vector<_type>* Scene::FindAllActorsComponents()
    {
        Vector<_type>* res = mnew Vector<_type>();
        for (auto actor : mRootActors)
            res->Add(actor->GetComponentsInChildren<_type>());

        return res;
    }

    template<typename _type>
    _type* Scene::FindActorByType()
    {
        for (auto actor : mRootActors)
        {
            if (auto res = dynamic_cast<_type*>(actor))
                return res;

            if (auto res = actor->FindChildByType<_type>())
                return res;
        }

        return nullptr;
    }

    template<typename _type>
    _type* Scene::FindActorComponent()
    {
        for (auto actor : mRootActors)
        {
            _type* res = actor->GetComponentInChildren<_type>();
            if (res)
                return res;
        }

        return nullptr;
    }
};
// --- META ---

CLASS_BASES_META(o2::Scene)
{
    BASE_CLASS(o2::Singleton<Scene>);
    BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::Scene)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLog);
    FIELD().PROTECTED().NAME(mCameras);
    FIELD().PROTECTED().NAME(mRootActors);
    FIELD().PROTECTED().NAME(mAllActors);
    FIELD().PROTECTED().NAME(mActorsMap);
    FIELD().PROTECTED().NAME(mAddedActors);
    FIELD().PROTECTED().NAME(mStartActors);
    FIELD().PROTECTED().NAME(mStartComponents);
    FIELD().PROTECTED().NAME(mDestroyActors);
    FIELD().PROTECTED().NAME(mDestroyComponents);
    FIELD().PROTECTED().NAME(mLayersMap);
    FIELD().PROTECTED().NAME(mLayers);
    FIELD().PROTECTED().NAME(mDefaultLayer);
    FIELD().PROTECTED().NAME(mTags);
    FIELD().PROTECTED().NAME(mCache);
#if  IS_EDITOR          
    FIELD().PUBLIC().NAME(onAddedToScene);
    FIELD().PUBLIC().NAME(onRemovedFromScene);
    FIELD().PUBLIC().NAME(onEnableChanged);
    FIELD().PUBLIC().NAME(onLockChanged);
    FIELD().PUBLIC().NAME(onNameChanged);
    FIELD().PUBLIC().NAME(onChildrenHierarchyChanged);
    FIELD().PUBLIC().NAME(onObjectsChanged);
    FIELD().PUBLIC().NAME(onLayersListChanged);
    FIELD().PROTECTED().NAME(mPrototypeLinksCache);
    FIELD().PROTECTED().NAME(mChangedObjects);
    FIELD().PROTECTED().NAME(mDestroyingObjects);
    FIELD().PROTECTED().NAME(mEditableObjects);
    FIELD().PROTECTED().NAME(mEditableObjectsByUID);
    FIELD().PROTECTED().NAME(mDrawnObjects);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsDrawingScene);
#endif
}
END_META;
CLASS_METHODS_META(o2::Scene)
{

    typedef const Map<String, SceneLayer*>& _tmp1;
    typedef Map<ActorAssetRef, Vector<Actor*>>& _tmp2;

    FUNCTION().PUBLIC().SIGNATURE(const LogStream&, GetLogStream);
    FUNCTION().PUBLIC().SIGNATURE(bool, HasLayer, const String&);
    FUNCTION().PUBLIC().SIGNATURE(SceneLayer*, GetLayer, const String&);
    FUNCTION().PUBLIC().SIGNATURE(SceneLayer*, GetDefaultLayer);
    FUNCTION().PUBLIC().SIGNATURE(SceneLayer*, AddLayer, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveLayer, SceneLayer*);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveLayer, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetLayerOrder, SceneLayer*, int);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<SceneLayer*>&, GetLayers);
    FUNCTION().PUBLIC().SIGNATURE(Vector<String>, GetLayersNames);
    FUNCTION().PUBLIC().SIGNATURE(_tmp1, GetLayersMap);
    FUNCTION().PUBLIC().SIGNATURE(Tag*, GetTag, const String&);
    FUNCTION().PUBLIC().SIGNATURE(Tag*, AddTag, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveTag, Tag*);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveTag, const String&);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Tag*>&, GetTags);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Actor*>&, GetRootActors);
    FUNCTION().PUBLIC().SIGNATURE(Vector<Actor*>&, GetRootActors);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Actor*>&, GetAllActors);
    FUNCTION().PUBLIC().SIGNATURE(Vector<Actor*>&, GetAllActors);
    FUNCTION().PUBLIC().SIGNATURE(Actor*, GetActorByID, SceneUID);
    FUNCTION().PUBLIC().SIGNATURE(Actor*, GetAssetActorByID, const UID&);
    FUNCTION().PUBLIC().SIGNATURE(Actor*, FindActor, const String&);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<CameraActor*>&, GetCameras);
    FUNCTION().PUBLIC().SIGNATURE(void, Clear, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, ClearCache);
    FUNCTION().PUBLIC().SIGNATURE(void, Load, const String&, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, Load, const DataDocument&, bool);
    FUNCTION().PUBLIC().SIGNATURE(void, Save, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, Save, DataDocument&);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, FixedUpdate, float);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateAddedEntities);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateTransforms);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateDestroyingEntities);
    FUNCTION().PUBLIC().SIGNATURE(void, DestroyActor, Actor*);
    FUNCTION().PUBLIC().SIGNATURE(void, DestroyComponent, Component*);
    FUNCTION().PROTECTED().SIGNATURE_STATIC(void, OnActorCreated, Actor*);
    FUNCTION().PROTECTED().SIGNATURE_STATIC(void, OnActorDestroy, Actor*);
    FUNCTION().PROTECTED().SIGNATURE_STATIC(void, OnNewActorParented, Actor*);
    FUNCTION().PROTECTED().SIGNATURE_STATIC(void, OnActorIdChanged, Actor*, SceneUID);
    FUNCTION().PROTECTED().SIGNATURE_STATIC(void, OnAddActorToScene, Actor*);
    FUNCTION().PROTECTED().SIGNATURE_STATIC(void, OnRemoveActorFromScene, Actor*, bool);
    FUNCTION().PROTECTED().CONSTRUCTOR();
    FUNCTION().PROTECTED().SIGNATURE(void, DrawCameras);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateActors, float);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateStartingEntities);
    FUNCTION().PROTECTED().SIGNATURE(void, DrawCursorDebugInfo);
    FUNCTION().PROTECTED().SIGNATURE(void, AddActorToScene, Actor*);
    FUNCTION().PROTECTED().SIGNATURE(void, RemoveActorFromScene, Actor*, bool);
    FUNCTION().PROTECTED().SIGNATURE(void, OnComponentAdded, Component*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnComponentRemoved, Component*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnLayerRenamed, SceneLayer*, const String&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCameraAddedOnScene, CameraActor*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCameraRemovedScene, CameraActor*);
#if  IS_EDITOR          
    FUNCTION().PUBLIC().SIGNATURE_STATIC(void, LinkActorToPrototypesHierarchy, Actor*, ActorAssetRef);
    FUNCTION().PUBLIC().SIGNATURE(Vector<SceneEditableObject*>, GetRootEditableObjects);
    FUNCTION().PUBLIC().SIGNATURE(void, AddEditableObjectToScene, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveEditableObjectFromScene, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(void, DestroyEditableObject, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<SceneEditableObject*>&, GetAllEditableObjects);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<SceneEditableObject*>&, GetChangedObjects);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<SceneEditableObject*>&, GetDrawnEditableObjects);
    FUNCTION().PUBLIC().SIGNATURE(SceneEditableObject*, GetEditableObjectByID, SceneUID);
    FUNCTION().PUBLIC().SIGNATURE(int, GetObjectHierarchyIdx, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(void, ReparentEditableObjects, const Vector<SceneEditableObject*>&, SceneEditableObject*, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(void, CheckChangedObjects);
    FUNCTION().PUBLIC().SIGNATURE(_tmp2, GetPrototypesLinksCache);
    FUNCTION().PUBLIC().SIGNATURE(void, BeginDrawingScene);
    FUNCTION().PUBLIC().SIGNATURE(void, EndDrawingScene);
    FUNCTION().PUBLIC().SIGNATURE(void, OnObjectAddToScene, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(void, OnObjectRemoveFromScene, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(void, OnObjectChanged, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(void, OnObjectDrawn, SceneEditableObject*);
    FUNCTION().PUBLIC().SIGNATURE(void, OnActorWithPrototypeCreated, Actor*);
    FUNCTION().PUBLIC().SIGNATURE(void, OnActorLinkedToPrototype, ActorAssetRef&, Actor*);
    FUNCTION().PUBLIC().SIGNATURE(void, OnActorPrototypeBroken, Actor*);
#endif
}
END_META;
// --- END META ---
