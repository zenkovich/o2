#pragma once

#include "o2/Utils/Singleton.h"
#include "Box2D/Dynamics/b2World.h"

namespace o2
{
	// -------------------
	// Box2D Physics world
	// -------------------
	class PhysicsWorld : public Singleton<PhysicsWorld>
	{
	public:
		int velocityIterations = 8;
		int positionIterations = 3;

	public:
		// Updates physics world and sync bodies
		void Update(float dt);

	private:
		b2World mWorld;
	};
}
