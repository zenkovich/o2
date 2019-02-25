#pragma once

#include "Assets/ActorAsset.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Serialization/Serializable.h"
#include "Utils/Singleton.h"
#include "Utils/Types/String.h"
#include "Utils/Types/UID.h"

// Scene graph access macros
#define o2Scene Scene::Instance()

namespace o2
{
	class Actor;
	typedef Vector<Actor*> ActorsVec;

	class SceneLayer;
	typedef Vector<SceneLayer*> SceneLayersVec;

	class Tag;
	typedef Vector<Tag*> TagsVec;

#if IS_EDITOR
	class SceneEditableObject;
	typedef Vector<SceneEditableObject*> SceneEditableObjectsVec;
#endif

	// -------------------------------------------------------
	// Actors scene. Contains and manages actors, tags, layers
	// -------------------------------------------------------
	class Scene : public Singleton<Scene>
	{
	public:
		typedef Vector<ActorAssetRef> ActorsAssetsVec;
		typedef Vector<SceneEditableObject*> SceneEditableObjectsVec;
		typedef Dictionary<ActorAssetRef, ActorsVec> ActorsCacheDict;

	public:
#if IS_EDITOR
		Function<void(SceneEditableObject*)>           onCreated;                  // Actor creation event
		Function<void(SceneEditableObject*)>           onDestroying;               // Actor destroying event
		Function<void(SceneEditableObject*)>           onEnableChanged;            // Actor enable changing
		Function<void(SceneEditableObject*)>           onLockChanged;			   // Actor locking change
		Function<void(SceneEditableObject*)>           onNameChanged;			   // Actor name changing event
		Function<void(SceneEditableObject*)>           onChildrenHierarchyChanged; // Actor childs hierarchy change event
		Function<void(const SceneEditableObjectsVec&)> onObjectsChanged;           // Actors some change event
#endif

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

		// Returns layers array
		SceneLayersVec& GetLayers();

		// Returns tag with name
		Tag* GetTag(const String& name) const;

		// Adds tag with name
		Tag* AddTag(const String& name);

		// Removes tag
		void RemoveTag(Tag* tag);

		// Removes tag with name
		void RemoveTag(const String& name);

		// Returns tags array
		const TagsVec& GetTags() const;

		// Returns root actors
		const ActorsVec& GetRootActors() const;

		// Returns root actors
		ActorsVec& GetRootActors();

		// Returns all actors
		const ActorsVec& GetAllActors() const;

		// Returns all actors
		ActorsVec& GetAllActors();

		// Returns actor by id
		Actor* GetActorByID(SceneUID id) const;

		// Returns asset actor by asset id. Tries to find in cache
		Actor* GetAssetActorByID(UID id);

		// Returns actor by path (ex "some node/other/target")
		Actor* FindActor(const String& path);

		// Returns component with type in scene
		template<typename _type>
		_type* FindActorComponent();

		// Returns all components with type in scene
		template<typename _type>
		Vector<_type>* FindAllActorsComponents();

		// Removes all actors
		void Clear();

		// Clears assets cache
		void ClearCache();

		// Loads scene from file. If append is true, old actors will not be destroyed
		void Load(const String& path, bool append = false);

		// Saves scene into file
		void Save(const String& path);

		// Updates root actors
		void Update(float dt); 

	protected:
		ActorsVec       mRootActors;   // Scene root actors		
		ActorsVec       mAllActors;    // All scene actors
		SceneLayersVec  mLayers;       // Scene layers
		TagsVec         mTags;         // Scene tags
		SceneLayer*     mDefaultLayer; // Default scene layer
		ActorsAssetsVec mCache;        // Cached actors assets

	protected:
		// Default constructor
		Scene();

		// Destructor
		~Scene();

		// Draws scene drawable objects
		void Draw();

		// Draws debug info for actor under cursor
		void DrawCursorDebugInfo();

		// It is called when actor created, adds in root and all actors list, registers for editor tools
		static void OnActorCreated(Actor* actor);

		// It is called when actor is destroying - removes from root and all actors lists, unregisters in editor tools
		static void OnActorDestroying(Actor* actor);

		friend class Actor;
		friend class Application;
		friend class DrawableComponent;
		friend class Widget;
		friend class WidgetLayer;

#if IS_EDITOR	  	
	public:
		// Returns root editable objects
		SceneEditableObjectsVec GetRootEditableObjects();

		// Registers editable object
		static void RegEditableObject(SceneEditableObject* object);

		// Removes object from registered list
		static void UnregEditableObject(SceneEditableObject* object);

		// Returns all editable objects
		const SceneEditableObjectsVec& GetAllEditableObjects();

		// Returns current changed actors
		const SceneEditableObjectsVec& GetChangedObjects() const;

		// Returns drawn on last frame editable objects
		const SceneEditableObjectsVec& GetDrawnEditableObjects() const;

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
		ActorsCacheDict& GetPrototypesLinksCache();

		// It is called when scene started to draw. When scene draw started, drawn scene objects will be collected
		void BeginDrawingScene();

		// It is called when scene finished draw. After scene objects will not be collected 
		void EndDrawingScene();

		// It is called when object was created
		static void OnObjectCreated(SceneEditableObject* object);

		// It is called when object is destroying
		static void OnObjectDestroyed(SceneEditableObject* object);

		// It is called when object was changed
		static void OnObjectChanged(SceneEditableObject* object);

		// It is called when object was drawn 
		static void OnObjectDrawn(SceneEditableObject* object);

		// It is called when created actor with prototype, updates cache
		static void OnActorWithPrototypeCreated(Actor* actor);

		// It is called when some actor created and linked to prototype, updates linked actors cache
		static void OnActorLinkedToPrototype(ActorAssetRef& assetRef, Actor* actor);

		// It is called when actor destroying or prototype link broken, updates cache
		static void OnActorPrototypeBroken(Actor* actor);

	protected:
		ActorsCacheDict mPrototypeLinksCache; // Cache of linked to prototypes actors

		SceneEditableObjectsVec mChangedObjects;  // Changed actors array
		SceneEditableObjectsVec mEditableObjects; // All scene editable objects

		SceneEditableObjectsVec mDrawnObjects;           // List of drawn on last frame editable objects
		bool                    mIsDrawingScene = false; // Sets true when started drawing scene, and false when not

		friend class SceneEditableObject;
#endif
	};
}

#include "Scene/Actor.h"

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
