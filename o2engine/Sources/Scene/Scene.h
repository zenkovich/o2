#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Singleton.h"

#define o2Scene Scene::Instance()

namespace o2
{
	class Scene: public Singleton<Scene>
	{
	public:

	protected:
		Scene();
		~Scene();

		void Update(float dt);
		void Draw();
	};
};
