#include "Scene.h"

#include "Scene/Actor.h"
#include "Scene/DrawableComponent.h"

namespace o2
{
	DECLARE_SINGLETON(Scene);

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		Clear();
	}

	void Scene::Update(float dt)
	{
		for (auto actor : mActors)
			actor->Update(dt);

		for (auto actor : mActors)
			actor->UpdateChilds(dt);
	}

	void Scene::Draw()
	{
		for (auto comp : mDrawableComponents)
			comp->Draw();
	}
	
	Ptr<Actor> Scene::FindActor(const String& path)
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		for (auto actor : mActors)
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
		auto allActors = mActors;
		for (auto actor : allActors)
			actor.Release();
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
		data = mActors;
		data.SaveToFile(path);
	}

	void Scene::RegDrawableComponent(Ptr<DrawableComponent> component)
	{
		const int binSearchRangeSizeStop = 5;
		int rangeMin = 0, rangeMax = mDrawableComponents.Count();
		float targetDepth = component->mDrawingDepth;
		int position = 0;
		bool skipLinearSearch = false;

		while (rangeMax - rangeMin > binSearchRangeSizeStop)
		{
			int center = (rangeMin + rangeMax) >> 1;

			float centerValue = mDrawableComponents[center];

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
				if (mDrawableComponents[position]->mDrawingDepth > targetDepth)
					break;
		}

		mDrawableComponents.Insert(component, position);
	}

	void Scene::UnregDrawableComponent(Ptr<DrawableComponent> component)
	{
		mDrawableComponents.Remove(component);
	}

	void Scene::ComponentDepthChanged(Ptr<DrawableComponent> component)
	{
		UnregDrawableComponent(component);
		RegDrawableComponent(component);
	}

}