#include "o2/stdafx.h"
#include "Scene.h"

#include "o2/Application/Input.h"
#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Render/Render.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/ActorRefResolver.h"
#include "o2/Scene/CameraActor.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Scene/SceneLayer.h"
#include "o2/Scene/Tags.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Render/VectorFontEffects.h"

namespace o2
{
	DECLARE_SINGLETON(Scene);

	Scene::Scene()
	{
		mDefaultLayer = AddLayer("Default");
		auto camera = mnew CameraActor();
		camera->name = "Camera";
	}

	Scene::~Scene()
	{
		Clear();
		ClearCache();

		delete mDefaultLayer;
	}

	const o2::Vector<CameraActor*>& Scene::GetCameras() const
	{
		return mCameras;
	}

	void Scene::Update(float dt)
	{
		UpdateAddedEntities();
		UpdateStartingEntities();
		UpdateDestroyingEntities();
		UpdateActors(dt);
	}

	void Scene::FixedUpdate(float dt)
	{
		for (auto actor : mRootActors)
			actor->FixedUpdate(dt);

		for (auto actor : mRootActors)
			actor->FixedUpdateChildren(dt);
	}

	void Scene::UpdateAddedEntities()
	{
		auto addedActors = mAddedActors;

		mStartActors = mAddedActors;

		mAddedActors.Clear();

		for (auto actor : addedActors)
			AddActorToScene(actor);
	}

	void Scene::UpdateStartingEntities()
	{
		auto startActors = mStartActors;
		auto startComponents = mStartComponents;

		mStartActors.Clear();
		mStartComponents.Clear();

		for (auto actor : startActors)
			actor->OnStart();

		for (auto comp : startComponents)
			comp->OnStart();
	}

	void Scene::UpdateDestroyingEntities()
	{
		auto destroyActors = mDestroyActors;
		auto destroyComponents = mDestroyComponents;

		mDestroyActors.Clear();
		mDestroyComponents.Clear();

		for (auto actor : destroyActors)
			delete actor;

		for (auto comp : destroyComponents)
			delete comp;

		if constexpr (IS_EDITOR)
		{
			auto destroyingObjects = mDestroyingObjects;
			mDestroyingObjects.Clear();

			for (auto object : destroyingObjects)
				delete object;
		}
	}

	void Scene::DestroyActor(Actor* actor)
	{
		mDestroyActors.Add(actor);
	}

	void Scene::DestroyComponent(Component* component)
	{
		mDestroyComponents.Add(component);
	}

	void Scene::AddActorToSceneDeferred(Actor* actor)
	{
		mAddedActors.Add(actor);
	}

	void Scene::OnActorIdChanged(Actor* actor, SceneUID prevId)
	{
		mActorsMap.Remove(prevId);
		mActorsMap[actor->mId] = actor;
	}

	void Scene::DestroyEditableObject(SceneEditableObject* object)
	{
		mDestroyingObjects.Add(object);
	}

	void Scene::UpdateActors(float dt)
	{
		for (auto actor : mRootActors)
			actor->Update(dt);

		for (auto actor : mRootActors)
			actor->UpdateChildren(dt);
	}

#undef DrawText

	void Scene::Draw()
	{
		DrawCameras(true);
	}

	void Scene::DrawCameras(bool callOnDrawnLayer)
	{
		if constexpr (IS_EDITOR)
			BeginDrawingScene();

		if (mCameras.IsEmpty())
		{
			if constexpr (IS_EDITOR)
			{
				o2Render.SetCamera(Camera());
				o2Render.Clear();

				static auto font = mnew VectorFont(o2Assets.GetBuiltAssetsPath() + "debugFont.ttf");
				static auto text = mnew Text(FontRef(font));

				text->SetText("No camera");
				text->Draw();
			}
		}
		else
		{
			for (auto camera : mCameras)
			{
				camera->SetupAndDraw();
				camera->listenersLayer.OnDrawn(Camera().GetBasis());
			}
		}

		DrawCursorDebugInfo();

		if constexpr (IS_EDITOR)
			EndDrawingScene();
	}

	void Scene::DrawCursorDebugInfo()
	{
		if (!o2Input.IsKeyDown(VK_F4))
			return;

		struct helper
		{
			static void Process(String& debugInfo, Actor* actor)
			{
				if (actor->transform->IsPointInside(o2Input.GetCursorPos()))
				{
					auto parent = actor;
					String path;
					while (parent)
					{
						path = parent->GetName() + "/" + path;
						parent = parent->GetParent();
					}

					debugInfo += " -- " + path + " - " + actor->GetType().GetName() + "\n";
					debugInfo += "Rect: " + (String)actor->transform->worldRect + "\n";

					Widget* widget = dynamic_cast<Widget*>(actor);
					if (widget)
					{
						debugInfo += "layout - " +
							(String)widget->layout->anchorMin + " " + (String)widget->layout->offsetMin + " - " +
							(String)widget->layout->anchorMax + " " + (String)widget->layout->offsetMax + "\n";

						debugInfo += (widget->mIsClipped ? (String)"clipped, " : (String)"not clipped, ") +
							(widget->mEnabled ? (String)"visible\n" : ((String)"hidden" + (!widget->mResEnabledInHierarchy ? (String)" fully\n" : (String)"\n")));

						debugInfo += "alpha: " + (String)widget->mTransparency +
							", res alpha: " + (String)widget->mResTransparency + "\n";
					}

					debugInfo += "\n";

					if (widget)
					{
						for (auto child : widget->mInternalWidgets)
							Process(debugInfo, child);
					}
				}

				for (auto child : actor->mChildren)
					Process(debugInfo, child);
			}
		};

		String debugInfo;
		for (auto actor : mAllActors)
			helper::Process(debugInfo, actor);

		o2Debug.DrawText(((Vec2F)o2Render.GetResolution().InvertedX())*0.5f, debugInfo);
	}

	void Scene::AddActorToScene(Actor* actor)
	{
		if (!actor->mParent)
			mRootActors.Add(actor);

		mAllActors.Add(actor);
		mActorsMap[actor->mId] = actor;

		actor->OnAddToScene();

		if constexpr (IS_EDITOR)
		{
			mChangedObjects.Add(actor);
			AddEditableObjectToScene(actor);
			onAddedToScene(actor);
		}
	}

	void Scene::RemoveActorFromScene(Actor* actor, bool keepEditorObjects /*= false*/)
	{
		if (!actor->mParent)
			mRootActors.Remove(actor);

		mAllActors.Remove(actor);
		mActorsMap.Remove(actor->mId);

		mStartActors.Remove(actor);
		mAddedActors.Remove(actor);

		if constexpr (IS_EDITOR)
		{
			if (!keepEditorObjects)
				RemoveEditableObjectFromScene(actor);

			OnObjectRemoveFromScene(actor);
		}
	}

	void Scene::OnComponentAdded(Component* component)
	{
		mStartComponents.Add(component);
	}

	void Scene::OnComponentRemoved(Component* component)
	{
		mStartComponents.Remove(component);
	}

	void Scene::OnLayerRenamed(SceneLayer* layer, const String& oldName)
	{
		mLayersMap.Remove(oldName);
		mLayersMap[layer->GetName()] = layer;

		onLayersListChanged();
	}

	void Scene::OnCameraAddedOnScene(CameraActor* camera)
	{
		mCameras.Add(camera);
	}

	void Scene::OnCameraRemovedScene(CameraActor* camera)
	{
		mCameras.Remove(camera);
	}

	bool Scene::HasLayer(const String& name) const
	{
		return mLayersMap.ContainsKey(name);
	}

	SceneLayer* Scene::GetLayer(const String& name)
	{
		SceneLayer* layer = nullptr;
		if (mLayersMap.TryGetValue(name, layer))
			return layer;

		return AddLayer(name);
	}

	SceneLayer* Scene::GetDefaultLayer() const
	{
		return mDefaultLayer;
	}

	SceneLayer* Scene::AddLayer(const String& name)
	{
		SceneLayer* layer = nullptr;
		if (mLayersMap.TryGetValue(name, layer))
			return layer;

		SceneLayer* newLayer = mnew SceneLayer();
		newLayer->mName = name;
		mLayers.Add(newLayer);
		mLayersMap[name] = newLayer;

		onLayersListChanged();

		return newLayer;
	}

	void Scene::RemoveLayer(SceneLayer* layer, bool removeActors /*= true*/)
	{
		if (layer == mDefaultLayer)
			return;

		if (removeActors)
		{
			auto actors = layer->mActors;
			for (auto actor : actors)
				delete actor;
		}

		mLayers.Remove(layer);
		mLayersMap.Remove(layer->mName);

		onLayersListChanged();

		delete layer;
	}

	void Scene::RemoveLayer(const String& name, bool removeActors /*= true*/)
	{
		RemoveLayer(GetLayer(name), removeActors);
	}

	void Scene::SetLayerOrder(SceneLayer* layer, int idx)
	{
		mLayers.Remove(layer);
		mLayers.Insert(layer, idx);
	}

	Tag* Scene::GetTag(const String& name) const
	{
		return mTags.FindOrDefault([&](auto x) { return x->GetName() == name; });
	}

	Tag* Scene::AddTag(const String& name)
	{
		if (GetTag(name))
		{
			o2Debug.LogError("Can't create new tag with name " + name + ": already exist");
			return nullptr;
		}

		Tag* newTag = mnew Tag();
		newTag->SetName(name);
		mTags.Add(newTag);

		return newTag;
	}

	void Scene::RemoveTag(Tag* tag)
	{
		if (!tag)
			return;

		mTags.Remove(tag);
		delete tag;
	}

	void Scene::RemoveTag(const String& name)
	{
		RemoveTag(GetTag(name));
	}

	const Vector<SceneLayer*>& Scene::GetLayers()const
	{
		return mLayers;
	}

	Vector<String> Scene::GetLayersNames() const
	{
		return mLayers.Convert<String>([](SceneLayer* x) { return x->GetName(); });
	}

	const Map<String, SceneLayer*>& Scene::GetLayersMap() const
	{
		return mLayersMap;
	}

	const Vector<Tag*>& Scene::GetTags() const
	{
		return mTags;
	}

	const Vector<Actor*>& Scene::GetRootActors() const
	{
		return mRootActors;
	}

	Vector<Actor*>& Scene::GetRootActors()
	{
		return mRootActors;
	}

	const Vector<Actor*>& Scene::GetAllActors() const
	{
		return mAllActors;
	}

	Vector<Actor*>& Scene::GetAllActors()
	{
		return mAllActors;
	}

	Actor* Scene::GetActorByID(SceneUID id) const
	{
		return mActorsMap.FindKey(id).second;
	}

	Actor* Scene::GetAssetActorByID(const UID& id)
	{
		auto cached = mCache.FindOrDefault([=](const ActorAssetRef& x) { return x->GetUID() == id; });

		if (!cached)
		{
			cached = ActorAssetRef(id);
			mCache.Add(cached);
		}

		return cached->GetActor();
	}

	Actor* Scene::FindActor(const String& path)
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		for (auto actor : mRootActors)
		{
			if (actor->mName == pathPart)
			{
				if (delPos == -1)
					return actor;
				else
					return actor->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	void Scene::Clear(bool keepDefaultLayer /*= true*/)
	{
		auto allActors = mRootActors;
		for (auto actor : allActors)
			delete actor;

		for (auto layer : mLayers)
			delete layer;

		mAddedActors.Clear();
		mStartActors.Clear();
		mStartComponents.Clear();
		mLayers.Clear();
		mLayersMap.Clear();

		if (keepDefaultLayer)
			mDefaultLayer = AddLayer("Default");

		for (auto tag : mTags)
			delete tag;

		mTags.Clear();
	}

	void Scene::ClearCache()
	{
		mCache.Clear();
	}

	void Scene::Load(const String& path, bool append /*= false*/)
	{
		DataDocument data;
		if (data.LoadFromFile(path))
			Load(data, append);
	}

	void Scene::Load(const DataDocument& doc, bool append /*= false*/)
	{
		ActorRefResolver::Instance().LockResolving();

		if (!append)
			Clear(false);

		auto& layersNode = doc.GetMember("Layers");
		if (!layersNode.IsEmpty())
		{
			for (auto& layerNode : layersNode)
			{
				auto layer = mnew SceneLayer();
				layer->Deserialize(layerNode);
				mLayers.Add(layer);
				mLayersMap[layer->mName] = layer;
			}
		}

		auto& defaultLayerNode = doc.GetMember("DefaultLayer");
		if (!defaultLayerNode.IsEmpty())
			mDefaultLayer = GetLayer(defaultLayerNode);
		else
			mDefaultLayer = AddLayer("Default");

		onLayersListChanged();

		auto& tagsNode = doc.GetMember("Tags");
		if (!tagsNode.IsEmpty())
		{
			for (auto& tagNode : tagsNode)
			{
				auto tag = mnew Tag();
				tag->Deserialize(tagNode);
				mTags.Add(tag);
			}
		}

		auto& actorsNode = doc.GetMember("Actors");
		if (!actorsNode.IsEmpty())
		{
			mRootActors = actorsNode;
			mRootActors.Clear();
		}

		ActorRefResolver::Instance().UnlockResolving();
		ActorRefResolver::Instance().ResolveRefs();

		for (auto actor : mRootActors)
			actor->UpdateTransform();

#if IS_EDITOR
		mChangedObjects.Clear();
#endif
	}

	void Scene::Save(const String& path)
	{
		DataDocument data;
		Save(data);
		data.SaveToFile(path);
	}

	void Scene::Save(DataDocument& doc)
	{
		auto& layersNode = doc.AddMember("Layers");
		for (auto layer : mLayers)
			layer->Serialize(layersNode.AddElement());

		doc.AddMember("DefaultLayer") = mDefaultLayer->mName;

		auto& tagsNode = doc.AddMember("Tags");
		for (auto tag : mTags)
			tag->Serialize(tagsNode.AddElement());

		doc.AddMember("Actors") = mRootActors;
	}

#if IS_EDITOR
	void Scene::DrawWithouLayers()
	{
		DrawCameras(false);
	}

	Vector<SceneEditableObject*> Scene::GetRootEditableObjects()
	{
		return mRootActors.Convert<SceneEditableObject*>([](Actor* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	void Scene::AddEditableObjectToScene(SceneEditableObject* object)
	{
		mEditableObjects.Add(object);
	}

	void Scene::RemoveEditableObjectFromScene(SceneEditableObject* object)
	{
		mChangedObjects.Remove(object);
		mEditableObjects.Remove(object);
	}

	const Vector<SceneEditableObject*>& Scene::GetAllEditableObjects()
	{
		return mEditableObjects;
	}

	SceneEditableObject* Scene::GetEditableObjectByID(SceneUID id) const
	{
		return mEditableObjects.FindOrDefault([=](SceneEditableObject* x) { return x->GetID() == id; });
	}

	int Scene::GetObjectHierarchyIdx(SceneEditableObject* object) const
	{
		if (object->GetEditableParent())
		{
			return object->GetEditableParent()->GetEditableChildren().IndexOf(object) + GetObjectHierarchyIdx(object->GetEditableParent());
		}

		return mRootActors.IndexOf([=](Actor* x) { return dynamic_cast<SceneEditableObject*>(x) == object; });
	}

	void Scene::ReparentEditableObjects(const Vector<SceneEditableObject*>& objects,
										SceneEditableObject* newParent, SceneEditableObject* prevObject)
	{
		struct Object
		{
			SceneEditableObject* object;
			int                  idx;
			Basis                transform;

			bool operator==(const Object& other) const { return object == other.object; }
		};
		Vector<Object> objectsDefs;

		for (auto object : objects)
		{
			Object def;
			def.object = object;
			def.transform = object->GetTransform();
			def.idx = o2Scene.GetObjectHierarchyIdx(object);
			objectsDefs.Add(def);

			object->SetEditableParent(nullptr);
			mRootActors.Remove(dynamic_cast<Actor*>(object));
		}

		objectsDefs.Sort([](auto& a, auto& b) { return a.idx < b.idx; });

		if (newParent)
		{
			int insertIdx = newParent->GetEditableChildren().IndexOf(prevObject) + 1;

			for (auto def : objectsDefs)
			{
				newParent->AddEditableChild(def.object, insertIdx++);
				def.object->SetTransform(def.transform);
			}
		}
		else
		{
			int insertIdx = 0;

			if (prevObject)
			{
				insertIdx = mRootActors.IndexOf([=](Actor* x) { return dynamic_cast<SceneEditableObject*>(x) == prevObject; });

				if (insertIdx < 0)
					insertIdx = mRootActors.Count();
				else
					insertIdx++;
			}

			for (auto def : objectsDefs)
			{
				auto actorEditableObject = dynamic_cast<Actor*>(def.object);
				if (actorEditableObject)
				{
					mRootActors.Insert(actorEditableObject, insertIdx++);
					def.object->SetTransform(def.transform);
				}
			}
		}

		if (newParent)
			OnObjectChanged(newParent);

		if (prevObject)
			OnObjectChanged(prevObject);
	}

	void Scene::CheckChangedObjects()
	{
		if (mChangedObjects.Count() > 0) {
			onObjectsChanged(mChangedObjects);
			mChangedObjects.Clear();

			UpdateAddedEntities();
			UpdateStartingEntities();
			UpdateDestroyingEntities();
		}
	}

	const Vector<SceneEditableObject*>& Scene::GetChangedObjects() const
	{
		return mChangedObjects;
	}

	const Vector<SceneEditableObject*>& Scene::GetDrawnEditableObjects() const
	{
		return mDrawnObjects;
	}

	Map<ActorAssetRef, Vector<Actor*>>& Scene::GetPrototypesLinksCache()
	{
		return mPrototypeLinksCache;
	}

	void Scene::BeginDrawingScene()
	{
		mIsDrawingScene = true;
		mDrawnObjects.Clear();
	}

	void Scene::EndDrawingScene()
	{
		mIsDrawingScene = false;
	}

	void Scene::OnObjectAddToScene(SceneEditableObject* object)
	{
		onAddedToScene(object);
		OnObjectChanged(object);
	}

	void Scene::OnObjectRemoveFromScene(SceneEditableObject* object)
	{
		onRemovedFromScene(object);
		OnObjectChanged(nullptr);
	}

	void Scene::OnObjectChanged(SceneEditableObject* object)
	{
		if (!object || object->changedFrame != o2Time.GetCurrentFrame())
		{
			if (object)
				object->changedFrame = o2Time.GetCurrentFrame();

			mChangedObjects.Add(object);
		}
	}

	void Scene::OnObjectDrawn(SceneEditableObject* object)
	{
		if (mIsDrawingScene)
			mDrawnObjects.Add(object);
	}

	void Scene::OnActorWithPrototypeCreated(Actor* actor)
	{
		auto proto = actor->GetPrototype();
		OnActorLinkedToPrototype(proto, actor);
	}

	void Scene::OnActorLinkedToPrototype(ActorAssetRef& assetRef, Actor* actor)
	{
		if (!mPrototypeLinksCache.ContainsKey(assetRef))
			mPrototypeLinksCache.Add(assetRef, Vector<Actor*>());

		mPrototypeLinksCache[assetRef].Add(actor);
	}

	void Scene::OnActorPrototypeBroken(Actor* actor)
	{
		// !!! TODO: Optimize this
		for (auto& it = mPrototypeLinksCache.Begin(); it != mPrototypeLinksCache.End();)
		{
			it->second.Remove(actor);
			if (it->second.IsEmpty())
			{
				mPrototypeLinksCache.Remove(it->first);
				it = mPrototypeLinksCache.Begin();
			}
			else it++;
		}
	}

#endif
}

DECLARE_CLASS(o2::Scene);
