#pragma once

#include "o2/Scene/Actor.h"

namespace o2
{
	class MTESceneActor : public Actor
	{
	public:
		// Default constructor
		MTESceneActor();

		// Copy-constructor
		MTESceneActor(const MTESceneActor& other);

		// Copy-operator
		MTESceneActor& operator=(const MTESceneActor& other);

		// Destructor
		~MTESceneActor();

		SERIALIZABLE(MTESceneActor);

	protected:

	protected:
		// Called when result enable was changed
		void OnEnableInHierarchyChanged() override;

		// Called when actor has added to scene; creates rigid body
		void OnAddToScene() override;

		// Called when actor has removed from scene; destroys rigid body
		void OnRemoveFromScene() override;
	};
}

CLASS_BASES_META(o2::MTESceneActor)
{
	BASE_CLASS(o2::Actor);
}
END_META;
CLASS_FIELDS_META(o2::MTESceneActor)
{
}
END_META;
CLASS_METHODS_META(o2::MTESceneActor)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const MTESceneActor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnableInHierarchyChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
}
END_META;
