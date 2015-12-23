#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Singleton.h"
#include "Utils/String.h"

// Scene graph access macros
#define o2Scene Scene::Instance()

namespace o2
{
	class Actor;
	class DrawableComponent;

	// ----------
	// Scene host
	// ----------
	class Scene: public Singleton<Scene>
	{
	public:
		typedef Vector<Ptr<Actor>> ActorsVec;
		typedef Vector<Ptr<DrawableComponent>> DrawCompsVec;

	public:
		// Returns actor by path (ex "some node/other/target")
		Ptr<Actor> FindActor(const String& path);

		// Returns component with type in scene
		template<typename _type>
		Ptr<_type> FindActorComponent();

		// Returns all components with type in scene
		template<typename _type>
		Vector<Ptr<_type>> FindAllActorsComponents();

		// Removes all actors
		void Clear();

		// Loads scene from file. If append is true, old actors will not be destroyed
		void Load(const String& path, bool append = false);

		// Saves scene into file
		void Save(const String& path);

	protected:
		ActorsVec     mActors;             // Scene root actors
		DrawCompsVec  mDrawableComponents; // Drawable components

	protected:
		// Default constructor
		Scene();

		// Destructor
		~Scene();

		// Updates root actors
		void Update(float dt);

		// Draws scene drawable components
		void Draw();

		// Registers drawable component
		void RegDrawableComponent(Ptr<DrawableComponent> component);

		// Unregisters drawable component
		void UnregDrawableComponent(Ptr<DrawableComponent> component);

		// Calls when drawable component depth was changed and sorts all drawable component
		void ComponentDepthChanged(Ptr<DrawableComponent> component);

		friend class Actor;
		friend class Application;
		friend class DrawableComponent;
		friend class ITemplPtr<Scene>;
	};

	template<typename _type>
	Vector<Ptr<_type>> Scene::FindAllActorsComponents()
	{
		Vector<Ptr<_type>> res;
		for (auto actor : mActors)
			res.Add(actor->GetComponentsInChildren<_type>());

		return res;
	}

	template<typename _type>
	Ptr<_type> Scene::FindActorComponent()
	{
		for (auto actor : mActors)
		{
			Ptr<_type> res = actor->GetComponentInChildren<_type>();
			if (res)
				return res;
		}

		return nullptr;
	}
};
