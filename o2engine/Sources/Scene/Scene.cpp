#include "Scene.h"

#include "Assets/ActorAsset.h"
#include "Scene/Actor.h"
#include "Scene/DrawableComponent.h"

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

		for (auto layer : mLayers)
			delete layer;
	}

	void Scene::Update(float dt)
	{
		for (auto actor : mRootActors)
			actor->Update(dt);

		for (auto actor : mRootActors)
			actor->UpdateChilds(dt);
	}

	void Scene::Draw()
	{
		for (auto layer : mLayers)
		{
			for (auto comp : layer->enabledDrawables)
				comp->Draw();
		}
	}

	Scene::Layer* Scene::GetLayer(const String& name) const
	{
		return mLayers.FindMatch([&](auto x) { return x->name == name; });
	}

	Scene::Layer* Scene::GetDefaultLayer() const
	{
		return mDefaultLayer;
	}

	Scene::Layer* Scene::AddLayer(const String& name)
	{
		if (GetLayer(name))
			return AddLayer(name + "_");

		Layer* newLayer = mnew Layer();
		newLayer->name = name;
		mLayers.Add(newLayer);
		return newLayer;
	}

	void Scene::RemoveLayer(Layer* layer, bool removeActors /*= true*/)
	{
		if (layer == mDefaultLayer)
			return;

		if (removeActors)
		{
			auto actors = layer->actors;
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

	Scene::LayersVec& Scene::GetLayers()
	{
		return mLayers;
	}

	const Scene::ActorsVec& Scene::GetRootActors() const
	{
		return mRootActors;
	}

	Scene::ActorsVec& Scene::GetRootActors()
	{
		return mRootActors;
	}

	const Scene::ActorsVec& Scene::GetAllActors() const
	{
		return mAllActors;
	}

	Scene::ActorsVec& Scene::GetAllActors()
	{
		return mAllActors;
	}

	Actor* Scene::GetActorByID(UInt64 id) const
	{
		return mAllActors.FindMatch([=](Actor* x) { return x->mId == id; });
	}

	Actor* Scene::GetAssetActorByID(AssetId id)
	{
		auto cached = mCache.FindMatch([=](ActorAsset* x) { return x->GetAssetId() == id; });

		if (!cached)
		{
			cached = mnew ActorAsset(id);
			mCache.Add(cached);
		}

		return &cached->actor;
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
	}

	void Scene::ClearCache()
	{
		for (auto asset : mCache)
			delete asset;

		mCache.Clear();
	}

	void Scene::Load(const String& path, bool append /*= false*/)
	{
		if (!append)
			Clear();

		DataNode data;
		data.LoadFromFile(path);
		ActorsVec loadActors = *data["actors"];
	}

	void Scene::Save(const String& path)
	{
		DataNode data;
		data = mRootActors;
		data.SaveToFile(path);
	}

	int Scene::GetActorHierarchyIdx(Actor* actor) const
	{
		if (actor->GetParent())
		{
			return actor->GetParent()->GetChilds().Find(actor) + GetActorHierarchyIdx(actor->GetParent());
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
			def.transform = actor->transform.GetWorldNonSizedBasis();
			def.idx = o2Scene.GetActorHierarchyIdx(def.actor);
			actorsDefs.Add(def);
		}

		for (auto def : actorsDefs)
			def.actor->ExcludeFromScene();

		actorsDefs.Sort([](auto& a, auto& b) { return a.idx < b.idx; });

		if (newParent)
		{
			int insertIdx = newParent->GetChilds().Find(prevActor) + 1;

			for (auto def : actorsDefs)
				newParent->AddChild(def.actor, insertIdx++);
		}
		else
		{
			int insertIdx = o2Scene.GetRootActors().Find(prevActor) + 1;

			for (auto def : actorsDefs)
				def.actor->SetPositionIndexInParent(insertIdx++);
		}

		for (auto def : actorsDefs)
		{
			def.actor->IncludeInScene();
			def.actor->transform.SetWorldNonSizedBasis(def.transform);
		}
	}

	void Scene::Layer::RegDrawableComponent(DrawableComponent* component)
	{
		drawables.Add(component);

		if (component->mResEnabled)
			ComponentEnabled(component);
	}

	void Scene::Layer::UnregDrawableComponent(DrawableComponent* component)
	{
		if (component->mResEnabled)
			ComponentDisabled(component);

		drawables.Remove(component);
	}

	void Scene::Layer::ComponentDepthChanged(DrawableComponent* component)
	{
		ComponentDisabled(component);
		ComponentEnabled(component);
	}

	void Scene::Layer::ComponentEnabled(DrawableComponent* component)
	{
		const int binSearchRangeSizeStop = 5;
		int rangeMin = 0, rangeMax = enabledDrawables.Count();
		float targetDepth = component->mDrawingDepth;
		int position = 0;
		bool skipLinearSearch = false;

		while (rangeMax - rangeMin > binSearchRangeSizeStop)
		{
			int center = (rangeMin + rangeMax) >> 1;

			float centerValue = enabledDrawables[center]->mDrawingDepth;

			if (targetDepth < centerValue)
				rangeMax = center;
			else if (targetDepth > centerValue)
				rangeMin = center;
			else
			{
				position = center;
				skipLinearSearch = true;
				break;
			}
		}

		if (!skipLinearSearch)
		{
			for (position = rangeMin; position < rangeMax; position++)
				if (enabledDrawables[position]->mDrawingDepth > targetDepth)
					break;
		}

		enabledDrawables.Insert(component, position);
	}

	void Scene::Layer::ComponentDisabled(DrawableComponent* component)
	{
		enabledDrawables.Remove(component);
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
#endif
}