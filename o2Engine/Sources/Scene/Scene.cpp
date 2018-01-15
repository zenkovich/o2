#include "stdafx.h"
#include "Scene.h"

#include "Application/Input.h"
#include "Assets/ActorAsset.h"
#include "Scene/Actor.h"
#include "Scene/DrawableComponent.h"
#include "Scene/SceneLayer.h"
#include "Scene/Tags.h"
#include "UI/WidgetLayout.h"

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
		for (auto layer : mLayers)
		{
			for (auto comp : layer->mEnabledDrawables)
				comp->Draw();
		}

		DrawCursorDebugInfo();
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
							(String)widget->layout->anchorMin + " "  +(String)widget->layout->offsetMin + " - " +
							(String)widget->layout->anchorMax + " "  +(String)widget->layout->offsetMax + "\n";

						debugInfo += (widget->mIsClipped ? (String)"clipped, " : (String)"not clipped, ") +
							(widget->mVisible ? (String)"visible\n" : ((String)"hidden" + (widget->mFullyDisabled ? (String)" fully\n" : (String)"\n")));

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
			o2Debug.LogError("Can't create new tag with name %s: already exist", name);
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

	Actor* Scene::GetActorByID(UInt64 id) const
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

	int Scene::GetActorHierarchyIdx(Actor* actor) const
	{
		if (actor->GetParent())
		{
			return actor->GetParent()->GetChildren().Find(actor) + GetActorHierarchyIdx(actor->GetParent());
		}

		return mRootActors.Find(actor);
	}

	void Scene::ReparentActors(const ActorsVec& actors, Actor* newParent, Actor* prevActor)
	{
		struct ActorDef
		{
			Actor* actor;
			int    idx;
			Basis  transform;

			bool operator==(const ActorDef& other) const { return actor == other.actor; }
		};
		Vector<ActorDef> actorsDefs;

		for (auto actor : actors)
		{
			ActorDef def;
			def.actor = actor;
			def.transform = actor->transform->GetWorldNonSizedBasis();
			def.idx = o2Scene.GetActorHierarchyIdx(def.actor);
			actorsDefs.Add(def);

			actor->SetParent(nullptr);
			mRootActors.Remove(actor);
		}

		actorsDefs.Sort([](auto& a, auto& b) { return a.idx < b.idx; });

		if (newParent)
		{
			int insertIdx = newParent->GetChildren().Find(prevActor) + 1;

			for (auto def : actorsDefs)
			{
				newParent->AddChild(def.actor, insertIdx++);
				def.actor->transform->SetWorldNonSizedBasis(def.transform);
			}
		}
		else
		{
			int insertIdx = 0;
			
			if (prevActor)
			{
				insertIdx = mRootActors.Find(prevActor);

				if (insertIdx < 0)
					insertIdx = mRootActors.Count();
				else
					insertIdx++;
			}

			for (auto def : actorsDefs)
			{
				mRootActors.Insert(def.actor, insertIdx++);
				def.actor->transform->SetWorldNonSizedBasis(def.transform);
			}
		}

		if (newParent)
			OnActorChanged(newParent);

		if (prevActor)
			OnActorChanged(prevActor);
	}

#if IS_EDITOR
	void Scene::OnActorChanged(Actor* actor)
	{
		if (!mChangedActors.Contains(actor))
			mChangedActors.Add(actor);
	}

	void Scene::CheckChangedActors()
	{
		if (mChangedActors.Count() > 0)
		{
			onChanged(mChangedActors);
			mChangedActors.Clear();
		}
	}

	const ActorsVec& Scene::GetChangedActors() const
	{
		return mChangedActors;
	}

	Scene::ActorsCacheDict& Scene::GetPrototypesLinksCache()
	{
		return mPrototypeLinksCache;
	}

	void Scene::OnActorWithPrototypeCreated(Actor* actor)
	{
		OnActorLinkedToPrototype(actor->GetPrototype(), actor);
	}

	void Scene::OnActorLinkedToPrototype(ActorAssetRef& assetRef, Actor* actor)
	{
		if (!mPrototypeLinksCache.ContainsKey(assetRef))
			mPrototypeLinksCache.Add(assetRef, ActorsVec());

		mPrototypeLinksCache[assetRef].Add(actor);
	}

	void Scene::OnActorPrototypeBreaked(Actor* actor)
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

#endif

	void LayerDataNodeConverter::ToData(void* object, DataNode& data)
	{
		if (object)
		{
			SceneLayer* value = (SceneLayer*)object;
			data = value->name;
		}
	}

	void LayerDataNodeConverter::FromData(void* object, const DataNode& data)
	{
		SceneLayer*& value = *(SceneLayer**)object;
		value = o2Scene.GetLayer(data);
	}

	bool LayerDataNodeConverter::IsConvertsType(const Type* type) const
	{
		return type->IsBasedOn(*TypeOf(SceneLayer).GetPointerType());
	}

}
