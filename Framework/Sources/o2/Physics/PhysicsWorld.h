#pragma once

#include "o2/Utils/Singleton.h"
#include "Box2D/Dynamics/b2World.h"

namespace o2
{
	class PhysicsWorld : public Singleton<PhysicsWorld>
	{
	public:

	private:
		b2World mWorld;
	};
}
