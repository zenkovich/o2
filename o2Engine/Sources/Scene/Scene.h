#pragma once

#include "Assets/ActorAsset.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Serializable.h"
#include "Utils/Singleton.h"
#include "Utils/String.h"
#include "Utils/UID.h"

// Scene graph access macros
#define o2Scene Scene::Instance()

namespace o2
{
	class Actor;
	typedef Vector<Actor*> ActorsVec;

	class SceneLayer;
	typedef Vector<SceneLayer*> SceneLayersVec;
	
	class Tag;

	// -------------------------------------------------------
	// Actors scene. Contains and manages actors, tags, layers
	// -------------------------------------------------------
	class Scene: public Singleton<Scene>
	{
	public:
		typedef Vector<ActorAssetRef> ActorsAssetsVec;
		typedef Dictionary<ActorAssetRef, ActorsVec> ActorsCacheDict;

	public:
		Function<void(Actor*)> onActorCreated;       // Actor creation event
		Function<void(Actor*)> onActorDestroying;    // Actor destroying event
		Function<void(Actor*)> onActorEnableChanged; // Actor enable changing

#if IS_EDITOR
		Function<void(ActorsVec)> onChanged;                     // Actors some change event
		Function<void(Actor*)>    onActorLockChanged;			 // Actor locking change
		Function<void(Actor*)>    onActorNameChanged;			 // Actor name changing event
		Function<void(Actor*)>    onActorChildsHierarchyChanged; // Actor childs hierarchy change event
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
		Actor* GetActorByID(UInt64 id) const;

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

		// Returns actor's index in hierarchy
		int GetActorHierarchyIdx(Actor* actor) const;

		// Reparent actors to new parent at next of prevActor;
		void ReparentActors(const ActorsVec& actors, Actor* newParent, Actor* prevActor);

		// Updates root actors
		void Update(float dt);

#if IS_EDITOR	  
		// It is called when actor was changed
		void OnActorChanged(Actor* actor);   

		// Checks is any actors was changed and calls OnChanged() if changed
		void CheckChangedActors();

		// Returns current changed actors
		const ActorsVec& GetChangedActors() const;

		// Returns cache of linked to prototypes actors
		ActorsCacheDict& GetPrototypesLinksCache();

		// It is called when created actor with prototype, updates cache
		void OnActorWithPrototypeCreated(Actor* actor);

		// It is called when some actor created and linked to prototype, updates linked actors cache
		void OnActorLinkedToPrototype(ActorAssetRef& assetRef, Actor* actor);

		// It is called when actor destroying or prototype link broken, updates cache
		void OnActorPrototypeBreaked(Actor* actor);
#endif       

	protected:
		ActorsVec       mRootActors;          // Scene root actors		
		ActorsVec       mAllActors;           // All scene actors
		SceneLayersVec  mLayers;              // Scene layers
		TagsVec         mTags;                // Scene tags
		SceneLayer*     mDefaultLayer;        // Default scene layer
		ActorsAssetsVec mCache;               // Cached actors assets
				  						      
#if IS_EDITOR	  						      
		ActorsVec       mChangedActors;       // Changed actors array
		ActorsCacheDict mPrototypeLinksCache; // Cache of linked to prototypes actors
#endif

	protected:
		// Default constructor
		Scene();

		// Destructor
		~Scene();

		// Draws scene drawable objects
		void Draw();

		friend class Actor;
		friend class Application;
		friend class DrawableComponent;
	};

	// -------------------------
	// Layer data node converter
	// -------------------------
	class LayerDataNodeConverter: public IDataNodeTypeConverter
	{
	public:
		// Converts layer pointer to data 
		void ToData(void* object, DataNode& data);

		// Gets layer pointer from data
		void FromData(void* object, const DataNode& data);

		// Checks that type is layer's type
		bool IsConvertsType(const Type* type) const;
	};

	template<typename _type>
	Vector<_type>* Scene::FindAllActorsComponents()
	{
		Vector<_type>* res;
		for (auto actor : mRootActors)
			res.Add(actor->GetComponentsInChildren<_type>());

		return res;
	}

	template<typename _type>
	_type* Scene::FindActorComponent()
	{
		for (auto actor : mRootActors)
		{
			_type> res = actor->GetComponentInChildren<_type*();
			if (res)
				return res;
		}

		return nullptr;
	}
};
