#include "o2/stdafx.h"
#include "Scene.h"

#include "o2/Application/Input.h"
#include "o2/Assets/ActorAsset.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/ActorDataNodeConverter.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Scene/SceneLayer.h"
#include "o2/Scene/Tags.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Render/Render.h"

namespace o2
{
	DECLARE_SINGLETON(Scene);

	Scene::Scene()
	{
		mDefaultLayer = AddLayer("Default");
	}

	Scene::~Scene()
	{
		Clear();
		ClearCache();

		delete mDefaultLayer;
	}

	void Scene::Update(float dt)
	{
		for (auto actor : mRootActors)
			actor->Update(dt);

		for (auto actor : mRootActors)
			actor->UpdateChildren(dt);
	}

	void Scene::Draw()
	{
#if IS_EDITOR
		BeginDrawingScene();
#endif

		for (auto layer : mLayers)
		{
			for (auto comp : layer->mEnabledDrawables)
				comp->Draw();
		}

		DrawCursorDebugInfo();

#if IS_EDITOR
		EndDrawingScene();
#endif
	}

#undef DrawText

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

	void Scene::OnActorCreated(Actor* actor, bool isOnScene)
	{
		if (!IsSingletonInitialzed())
			return;

		if (isOnScene)
		{
			Instance().mRootActors.Add(actor);
			Instance().mAllActors.Add(actor);

#if IS_EDITOR
			RegEditableObject(actor);
#endif
		}

#if IS_EDITOR
		OnObjectCreated(actor);
#endif
	}

	void Scene::OnActorDestroying(Actor* actor)
	{
		if (!IsSingletonInitialzed())
			return;

		Instance().mRootActors.Remove(actor);
		Instance().mAllActors.Remove(actor);

#if IS_EDITOR
		OnObjectDestroyed(actor);
		OnActorPrototypeBroken(actor);
#endif
	}

	SceneLayer* Scene::GetLayer(const String& name)
	{
		if (auto layer = mLayers.FindMatch([&](auto x) { return x->name == name; }))
			return layer;

		return AddLayer(name);
	}

	SceneLayer* Scene::GetDefaultLayer() const
	{
		return mDefaultLayer;
	}

	SceneLayer* Scene::AddLayer(const String& name)
	{
		if (auto layer = mLayers.FindMatch([&](auto x) { return x->name == name; }))
			return layer;

		SceneLayer* newLayer = mnew SceneLayer();
		newLayer->name = name;
		mLayers.Add(newLayer);
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

		delete layer;
	}

	void Scene::RemoveLayer(const String& name, bool removeActors /*= true*/)
	{
		RemoveLayer(GetLayer(name), removeActors);
	}

	Tag* Scene::GetTag(const String& name) const
	{
		return mTags.FindMatch([&](auto x) { return x->GetName() == name; });
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

	SceneLayersVec& Scene::GetLayers()
	{
		return mLayers;
	}

	const TagsVec& Scene::GetTags() const
	{
		return mTags;
	}

	const ActorsVec& Scene::GetRootActors() const
	{
		return mRootActors;
	}

	ActorsVec& Scene::GetRootActors()
	{
		return mRootActors;
	}

	const ActorsVec& Scene::GetAllActors() const
	{
		return mAllActors;
	}

	ActorsVec& Scene::GetAllActors()
	{
		return mAllActors;
	}

	Actor* Scene::GetActorByID(SceneUID id) const
	{
		return mAllActors.FindMatch([=](Actor* x) { return x->mId == id; });
	}

	Actor* Scene::GetAssetActorByID(UID id)
	{
		auto cached = mCache.FindMatch([=](const ActorAssetRef& x) { return x->GetAssetId() == id; });

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

	void Scene::Clear()
	{
		auto allActors = mRootActors;
		for (auto actor : allActors)
			delete actor;

		for (auto layer : mLayers)
			delete layer;

		mLayers.Clear();

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
		ActorDataNodeConverter::Instance().LockPointersResolving();

		if (!append)
		{
			Clear();
			delete mDefaultLayer;
			mLayers.Clear();
		}

		DataNode data;
		data.LoadFromFile(path);

		auto layersNode = data.GetNode("Layers");
		for (auto layerNode : *layersNode)
		{
			auto layer = mnew SceneLayer();
			layer->Deserialize(*layerNode);
			mLayers.Add(layer);
		}

		mDefaultLayer = GetLayer(data.GetNode("DefaultLayer")->Data());

		auto tagsNode = data.GetNode("Tags");
		for (auto tagNode : *tagsNode)
		{
			auto tag = mnew Tag();
			tag->Deserialize(*tagNode);
			mTags.Add(tag);
		}

		auto actorsNode = data.GetNode("Actors");
		for (auto actorNode : *actorsNode)
		{
			auto actor = mnew Actor();
			actor->Deserialize(*actorNode);
		}

		ActorDataNodeConverter::Instance().UnlockPointersResolving();
		ActorDataNodeConverter::Instance().ResolvePointers();
	}

	void Scene::Save(const String& path)
	{
		DataNode data;

		auto layersNode = data.AddNode("Layers");
		for (auto layer : mLayers)
			*layersNode->AddNode("Layer") = layer->Serialize();

		*data.AddNode("DefaultLayer") = mDefaultLayer->name;

		auto tagsNode = data.AddNode("Tags");
		for (auto tag : mTags)
			*tagsNode->AddNode("Tag") = tag->Serialize();

		auto actorsNode = data.AddNode("Actors");
		for (auto actor : mRootActors)
			*actorsNode->AddNode("Actor") = actor->Serialize();

		data.SaveToFile(path);
	}

#if IS_EDITOR
	SceneEditableObjectsVec Scene::GetRootEditableObjects()
	{
		return mRootActors.Select<SceneEditableObject*>([](Actor* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	void Scene::RegEditableObject(SceneEditableObject* object)
	{
		if (IsSingletonInitialzed())
			Instance().mEditableObjects.Add(object);
	}

	void Scene::UnregEditableObject(SceneEditableObject* object)
	{
		if (IsSingletonInitialzed())
			Instance().mEditableObjects.Remove(object);
	}

	const SceneEditableObjectsVec& Scene::GetAllEditableObjects()
	{
		return mEditableObjects;
	}

	SceneEditableObject* Scene::GetEditableObjectByID(SceneUID id) const
	{
		return mEditableObjects.FindMatch([=](SceneEditableObject* x) { return x->GetID() == id; });
	}

	int Scene::GetObjectHierarchyIdx(SceneEditableObject* object) const
	{
		if (object->GetEditableParent())
		{
			return object->GetEditableParent()->GetEditablesChildren().Find(object) + GetObjectHierarchyIdx(object->GetEditableParent());
		}

		return mRootActors.FindIdx([=](Actor* x) { return dynamic_cast<SceneEditableObject*>(x) == object; });
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
			int insertIdx = newParent->GetEditablesChildren().Find(prevObject) + 1;

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
				insertIdx = mRootActors.FindIdx([=](Actor* x) { return dynamic_cast<SceneEditableObject*>(x) == prevObject; });

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
		}
	}

	const SceneEditableObjectsVec& Scene::GetChangedObjects() const
	{
		return mChangedObjects;
	}

	const SceneEditableObjectsVec& Scene::GetDrawnEditableObjects() const
	{
		return mDrawnObjects;
	}

	Scene::ActorsCacheMap& Scene::GetPrototypesLinksCache()
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

	void Scene::OnObjectCreated(SceneEditableObject* object)
	{
		if (!IsSingletonInitialzed())
			return;

		Instance().onCreated(object);
		OnObjectChanged(object);
	}

	void Scene::OnObjectDestroyed(SceneEditableObject* object)
	{
		if (!IsSingletonInitialzed())
			return;

		Instance().onDestroying(object);
		UnregEditableObject(object);
		OnObjectChanged(nullptr);
	}

	void Scene::OnObjectChanged(SceneEditableObject* object)
	{
		if (!IsSingletonInitialzed())
			return;

		if (!Instance().mChangedObjects.Contains(object))
			Instance().mChangedObjects.Add(object);
	}

	void Scene::OnObjectDrawn(SceneEditableObject* object)
	{
		if (!IsSingletonInitialzed())
			return;

		if (Instance().mIsDrawingScene)
			Instance().mDrawnObjects.Add(object);
	}

	void Scene::OnActorWithPrototypeCreated(Actor* actor)
	{
		auto proto = actor->GetPrototype();
		OnActorLinkedToPrototype(proto, actor);
	}

	void Scene::OnActorLinkedToPrototype(ActorAssetRef& assetRef, Actor* actor)
	{
		if (!IsSingletonInitialzed())
			return;

		if (!Instance().mPrototypeLinksCache.ContainsKey(assetRef))
			Instance().mPrototypeLinksCache.Add(assetRef, ActorsVec());

		Instance().mPrototypeLinksCache[assetRef].Add(actor);
	}

	void Scene::OnActorPrototypeBroken(Actor* actor)
	{
		if (!IsSingletonInitialzed())
			return;

		for (auto& it = Instance().mPrototypeLinksCache.Begin(); it != Instance().mPrototypeLinksCache.End();)
		{
			it->second.Remove(actor);
			if (it->second.IsEmpty())
			{
				Instance().mPrototypeLinksCache.Remove(it->first);
				it = Instance().mPrototypeLinksCache.Begin();
			}
			else it++;
		}
	}

#endif
}
