#include "stdafx.h"
#include "Scene.h"

#include "Application/Input.h"
#include "Assets/ActorAsset.h"
#include "Scene/Actor.h"
#include "Scene/ActorDataNodeConverter.h"
#include "Scene/DrawableComponent.h"
#include "Scene/SceneLayer.h"
#include "Scene/Tags.h"
#include "Scene/UI/WidgetLayout.h"

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
						parent = parent->parent;
					}

					debugInfo += " -- " + path + " - " + actor->GetType().GetName() + "\n";
					debugInfo += "Rect: " + (String)actor->transform->worldRect + "\n";

					UIWidget* widget = dynamic_cast<UIWidget*>(actor);
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

	void Scene::OnObjectCreated(SceneEditableObject* object)
	{
		onCreated(object);
		OnObjectChanged(object);
	}

	void Scene::OnObjectDestroyed(SceneEditableObject* object)
	{
		onDestroying(object);
		OnObjectChanged(nullptr);
	}

	void Scene::OnObjectChanged(SceneEditableObject* object)
	{
		if (!mChangedObjects.Contains(object))
			mChangedObjects.Add(object);
	}

	void Scene::OnObjectDrawn(SceneEditableObject* object)
	{
		if (mIsDrawingScene)
			mDrawnObjects.Add(object);
	}

	void Scene::CheckChangedObjects()
	{
		if (mChangedObjects.Count() > 0)
		{
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

	Scene::ActorsCacheDict& Scene::GetPrototypesLinksCache()
	{
		return mPrototypeLinksCache;
	}

	void Scene::OnActorWithPrototypeCreated(Actor* actor)
	{
		auto proto = actor->GetPrototype();
		OnActorLinkedToPrototype(proto, actor);
	}

	void Scene::OnActorLinkedToPrototype(ActorAssetRef& assetRef, Actor* actor)
	{
		if (!mPrototypeLinksCache.ContainsKey(assetRef))
			mPrototypeLinksCache.Add(assetRef, ActorsVec());

		mPrototypeLinksCache[assetRef].Add(actor);
	}

	void Scene::OnActorPrototypeBroken(Actor* actor)
	{
		for (auto kv = mPrototypeLinksCache.Begin(); kv != mPrototypeLinksCache.End();)
		{
			kv.Value().Remove(actor);
			if (kv.Value().IsEmpty())
			{
				mPrototypeLinksCache.Remove(kv.Key());
				kv = mPrototypeLinksCache.Begin();
			}
			else kv++;
		}
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

#endif
}
