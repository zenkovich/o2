#include "o2/stdafx.h"
#include "MTEScene.h"

#include "Box2D/Dynamics/b2Body.h"
#include "o2/Physics/PhysicsWorld.h"
#include "o2/Scene/Physics/ICollider.h"

namespace o2
{
	MTESceneActor::MTESceneActor()
	{}

	MTESceneActor::MTESceneActor(const MTESceneActor& other):
		Actor(other)
	{}

	MTESceneActor::~MTESceneActor()
	{
	}

	MTESceneActor& MTESceneActor::operator=(const MTESceneActor& other)
	{

		return *this;
	}


	void MTESceneActor::OnEnableInHierarchyChanged()
	{
		Actor::OnEnableInHierarchyChanged();

	}

	void MTESceneActor::OnAddToScene()
	{
		Actor::OnAddToScene();
	}

	void MTESceneActor::OnRemoveFromScene()
	{
		Actor::OnRemoveFromScene();
	}
}

DECLARE_TEMPLATE_CLASS(o2::Ref<o2::MTESceneActor>);

DECLARE_CLASS(o2::MTESceneActor);
