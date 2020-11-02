#pragma once

#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/String.h"
#include "o2/Utils/Types/UID.h"
#include "o2/Utils/Property.h"

// Scene graph access macros
#define o2Scene Scene::Instance()

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
		PROPERTIES(Scene);

#if IS_EDITOR
		Function<void(SceneEditableObject*)> onAddedToScene;             // Actor added to scene event
		Function<void(SceneEditableObject*)> onRemovedFromScene;         // Actor removed from scene event
		Function<void(SceneEditableObject*)> onEnableChanged;            // Actor enable changing
		Function<void(SceneEditableObject*)> onLockChanged;			     // Actor locking change
		Function<void(SceneEditableObject*)> onNameChanged;			     // Actor name changing event
		Function<void(SceneEditableObject*)> onChildrenHierarchyChanged; // Actor childs hierarchy change event

		Function<void(const Vector<SceneEditableObject*>&)> onObjectsChanged; // Actors some change event

		Function<void()> onLayersListChanged; // It is called when layer added, removed or renamed
#endif

	public:
		// Checks is layer exists
		bool HasLayer(const String& name) const;

		// Returns layer by name
		SceneLayer* GetLayer(const String& name);

		// Returns default layer
		SceneLayer* GetDefaultLayer() const;

		// Adds layer with name
		SceneLayer* AddLayer(const String& name);

		// Removes layer
		void RemoveLayer(SceneLayer* layer, bool removeActors = true);

		// Removes layer by name
		void RemoveLayer(const String& name, bool removeActors = true);

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

		IOBJECT(Scene);

	protected:
		Vector<CameraActor*> mCameras; // List of cameras on scene

		Vector<Actor*> mRootActors; // Scene root actors		
		Vector<Actor*> mAllActors;  // All scene actors

		Vector<Actor*> mAddedActors; // List of added on previous frame actors. Will receive OnAddToScene at current frame
		
		Vector<Actor*>     mStartActors;     // List of starting on current frame actors. Will receive OnStart at current frame
		Vector<Component*> mStartComponents; // List of starting on current frame components. Will receive OnStart at current frame

		Vector<Actor*>     mDestroyActors;     // List of destroying on current frame actors
		Vector<Component*> mDestroyComponents; // List of destroying on current frame components

		Map<String, SceneLayer*> mLayersMap;    // Layers by names map
		Vector<SceneLayer*>      mLayers;       // Scene layers
		SceneLayer*              mDefaultLayer; // Default scene layer

		Vector<Tag*> mTags; // Scene tags

		Vector<ActorAssetRef> mCache; // Cached actors assets

	protected:
		// Default constructor
		Scene();

		// Destructor
		~Scene();

		// Updates root actors and their children
		void UpdateActors(float dt);

		// Updates just added actors and components
		void UpdateAddedEntities();

		// Updates starting actors and components
		void UpdateStartingEntities();

		// Updates destroying actors and components
		void UpdateDestroyingEntities();

		// Draws debug info for actor under cursor
		void DrawCursorDebugInfo();

		// Adds actor to destroy list
		void DestroyActor(Actor* actor);

		// It is called when actor adding to scene; registers in actors list and events list
		void AddActorToScene(Actor* actor);

		// It is called when actor constructing and adding to scene. The OnAddToScene will be called on next frame
		void AddActorToSceneDeferred(Actor* actor);

		// It is called when actor removing from scene; unregisters from actors list and events list
		void RemoveActorFromScene(Actor* actor, bool keepEditorObjects = false);

		// It is called when component added to actor, registers for calling OnAddOnScene
		void OnComponentAdded(Component* component);

		// It is called when component removed, register for calling OnRemovFromScene
		void OnComponentRemoved(Component* component);

		// It is called when scene layer renamed, updates layers map
		void OnLayerRenamed(SceneLayer* layer, const String& oldName);

		// It is called when camera was added to scene
		void OnCameraAddedOnScene(CameraActor* camera);

		// It is called when camera was removed from scene
		void OnCameraRemovedScene(CameraActor* camera);

		friend class Actor;
		friend class Application;
		friend class CameraActor;
		friend class DrawableComponent;
		friend class Ref<Actor>;
		friend class SceneLayer;
		friend class Widget;
		friend class WidgetLayer;

#if IS_EDITOR	  	
	public:
		// Returns root editable objects
		Vector<SceneEditableObject*> GetRootEditableObjects();

		// Registers editable object
		void RegEditableObject(SceneEditableObject* object);

		// Removes object from registered list
		void UnregEditableObject(SceneEditableObject* object);

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

		// It is called when scene started to draw. When scene draw started, drawn scene objects will be collected
		void BeginDrawingScene();

		// It is called when scene finished draw. After scene objects will not be collected 
		void EndDrawingScene();

		// It is called when object was created
		void OnObjectCreated(SceneEditableObject* object);

		// It is called when object is destroying
		void OnObjectDestroyed(SceneEditableObject* object);

		// It is called when object was changed
		void OnObjectChanged(SceneEditableObject* object);

		// It is called when object was drawn 
		void OnObjectDrawn(SceneEditableObject* object);

		// It is called when created actor with prototype, updates cache
		void OnActorWithPrototypeCreated(Actor* actor);

		// It is called when some actor created and linked to prototype, updates linked actors cache
		void OnActorLinkedToPrototype(ActorAssetRef& assetRef, Actor* actor);

		// It is called when actor destroying or prototype link broken, updates cache
		void OnActorPrototypeBroken(Actor* actor);

	protected:
		Map<ActorAssetRef, Vector<Actor*>> mPrototypeLinksCache; // Cache of linked to prototypes actors

		Vector<SceneEditableObject*> mChangedObjects;  // Changed actors array
		Vector<SceneEditableObject*> mEditableObjects; // All scene editable objects

		Vector<SceneEditableObject*> mDrawnObjects;           // List of drawn on last frame editable objects
		bool                         mIsDrawingScene = false; // Sets true when started drawing scene, and false when not

		friend class Scene;
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

CLASS_BASES_META(o2::Scene)
{
	BASE_CLASS(o2::Singleton<Scene>);
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::Scene)
{
	PUBLIC_FIELD(onAddedToScene);
	PUBLIC_FIELD(onRemovedFromScene);
	PUBLIC_FIELD(onEnableChanged);
	PUBLIC_FIELD(onLockChanged);
	PUBLIC_FIELD(onNameChanged);
	PUBLIC_FIELD(onChildrenHierarchyChanged);
	PUBLIC_FIELD(onObjectsChanged);
	PUBLIC_FIELD(onLayersListChanged);
	PROTECTED_FIELD(mCameras);
	PROTECTED_FIELD(mRootActors);
	PROTECTED_FIELD(mAllActors);
	PROTECTED_FIELD(mAddedActors);
	PROTECTED_FIELD(mStartActors);
	PROTECTED_FIELD(mStartComponents);
	PROTECTED_FIELD(mDestroyActors);
	PROTECTED_FIELD(mDestroyComponents);
	PROTECTED_FIELD(mLayersMap);
	PROTECTED_FIELD(mLayers);
	PROTECTED_FIELD(mDefaultLayer);
	PROTECTED_FIELD(mTags);
	PROTECTED_FIELD(mCache);
	PROTECTED_FIELD(mPrototypeLinksCache);
	PROTECTED_FIELD(mChangedObjects);
	PROTECTED_FIELD(mEditableObjects);
	PROTECTED_FIELD(mDrawnObjects);
	PROTECTED_FIELD(mIsDrawingScene).DEFAULT_VALUE(false);
}
END_META;
CLASS_METHODS_META(o2::Scene)
{

	typedef const Map<String, SceneLayer*>& _tmp1;
	typedef Map<ActorAssetRef, Vector<Actor*>>& _tmp2;

	PUBLIC_FUNCTION(bool, HasLayer, const String&);
	PUBLIC_FUNCTION(SceneLayer*, GetLayer, const String&);
	PUBLIC_FUNCTION(SceneLayer*, GetDefaultLayer);
	PUBLIC_FUNCTION(SceneLayer*, AddLayer, const String&);
	PUBLIC_FUNCTION(void, RemoveLayer, SceneLayer*, bool);
	PUBLIC_FUNCTION(void, RemoveLayer, const String&, bool);
	PUBLIC_FUNCTION(void, SetLayerOrder, SceneLayer*, int);
	PUBLIC_FUNCTION(const Vector<SceneLayer*>&, GetLayers);
	PUBLIC_FUNCTION(Vector<String>, GetLayersNames);
	PUBLIC_FUNCTION(_tmp1, GetLayersMap);
	PUBLIC_FUNCTION(Tag*, GetTag, const String&);
	PUBLIC_FUNCTION(Tag*, AddTag, const String&);
	PUBLIC_FUNCTION(void, RemoveTag, Tag*);
	PUBLIC_FUNCTION(void, RemoveTag, const String&);
	PUBLIC_FUNCTION(const Vector<Tag*>&, GetTags);
	PUBLIC_FUNCTION(const Vector<Actor*>&, GetRootActors);
	PUBLIC_FUNCTION(Vector<Actor*>&, GetRootActors);
	PUBLIC_FUNCTION(const Vector<Actor*>&, GetAllActors);
	PUBLIC_FUNCTION(Vector<Actor*>&, GetAllActors);
	PUBLIC_FUNCTION(Actor*, GetActorByID, SceneUID);
	PUBLIC_FUNCTION(Actor*, GetAssetActorByID, const UID&);
	PUBLIC_FUNCTION(Actor*, FindActor, const String&);
	PUBLIC_FUNCTION(void, Clear, bool);
	PUBLIC_FUNCTION(void, ClearCache);
	PUBLIC_FUNCTION(void, Load, const String&, bool);
	PUBLIC_FUNCTION(void, Load, const DataDocument&, bool);
	PUBLIC_FUNCTION(void, Save, const String&);
	PUBLIC_FUNCTION(void, Save, DataDocument&);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, FixedUpdate, float);
	PROTECTED_FUNCTION(void, UpdateActors, float);
	PROTECTED_FUNCTION(void, UpdateAddedEntities);
	PROTECTED_FUNCTION(void, UpdateStartingEntities);
	PROTECTED_FUNCTION(void, UpdateDestroyingEntities);
	PROTECTED_FUNCTION(void, DrawCursorDebugInfo);
	PROTECTED_FUNCTION(void, DestroyActor, Actor*);
	PROTECTED_FUNCTION(void, AddActorToScene, Actor*);
	PROTECTED_FUNCTION(void, AddActorToSceneDeferred, Actor*);
	PROTECTED_FUNCTION(void, RemoveActorFromScene, Actor*, bool);
	PROTECTED_FUNCTION(void, OnComponentAdded, Component*);
	PROTECTED_FUNCTION(void, OnComponentRemoved, Component*);
	PROTECTED_FUNCTION(void, OnLayerRenamed, SceneLayer*, const String&);
	PROTECTED_FUNCTION(void, OnCameraAddedOnScene, CameraActor*);
	PROTECTED_FUNCTION(void, OnCameraRemovedScene, CameraActor*);
	PUBLIC_FUNCTION(Vector<SceneEditableObject*>, GetRootEditableObjects);
	PUBLIC_FUNCTION(void, RegEditableObject, SceneEditableObject*);
	PUBLIC_FUNCTION(void, UnregEditableObject, SceneEditableObject*);
	PUBLIC_FUNCTION(const Vector<SceneEditableObject*>&, GetAllEditableObjects);
	PUBLIC_FUNCTION(const Vector<SceneEditableObject*>&, GetChangedObjects);
	PUBLIC_FUNCTION(const Vector<SceneEditableObject*>&, GetDrawnEditableObjects);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableObjectByID, SceneUID);
	PUBLIC_FUNCTION(int, GetObjectHierarchyIdx, SceneEditableObject*);
	PUBLIC_FUNCTION(void, ReparentEditableObjects, const Vector<SceneEditableObject*>&, SceneEditableObject*, SceneEditableObject*);
	PUBLIC_FUNCTION(void, CheckChangedObjects);
	PUBLIC_FUNCTION(_tmp2, GetPrototypesLinksCache);
	PUBLIC_FUNCTION(void, BeginDrawingScene);
	PUBLIC_FUNCTION(void, EndDrawingScene);
	PUBLIC_FUNCTION(void, OnObjectCreated, SceneEditableObject*);
	PUBLIC_FUNCTION(void, OnObjectDestroyed, SceneEditableObject*);
	PUBLIC_FUNCTION(void, OnObjectChanged, SceneEditableObject*);
	PUBLIC_FUNCTION(void, OnObjectDrawn, SceneEditableObject*);
	PUBLIC_FUNCTION(void, OnActorWithPrototypeCreated, Actor*);
	PUBLIC_FUNCTION(void, OnActorLinkedToPrototype, ActorAssetRef&, Actor*);
	PUBLIC_FUNCTION(void, OnActorPrototypeBroken, Actor*);
}
END_META;
