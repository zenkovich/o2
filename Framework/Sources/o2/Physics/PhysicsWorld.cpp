#include "o2/stdafx.h"
#include "PhysicsWorld.h"

namespace o2
{
	DECLARE_SINGLETON(PhysicsWorld);

	void PhysicsWorld::Update(float dt)
	{
		mWorld.Step(dt, velocityIterations, positionIterations);
	}

}
