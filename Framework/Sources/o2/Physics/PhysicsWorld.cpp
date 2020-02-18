#include "o2/stdafx.h"
#include "PhysicsWorld.h"

namespace o2
{
	DECLARE_SINGLETON(PhysicsWorld);

	PhysicsWorld::PhysicsWorld():
		mWorld(gravity)
	{}

	void PhysicsWorld::Update(float dt)
	{
		mWorld.SetGravity(gravity);
		mWorld.Step(dt, velocityIterations, positionIterations);
	}

}
