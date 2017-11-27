#include "stdafx.h"
#include "ActorsTransform.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	ActorsTransformAction::ActorsTransformAction()
	{}

	ActorsTransformAction::ActorsTransformAction(const Vector<Actor*>& actors,
															 const Vector<ActorTransform>& beforeTransforms):
		beforeTransforms(beforeTransforms)
	{
		actorsIds = actors.Select<UInt64>([](Actor* x) { return x->GetID(); });
		doneTransforms = actors.Select<ActorTransform>([](Actor* x) { return *x->transform; });
	}

	String ActorsTransformAction::GetName() const
	{
		return "Actors transformation";
	}

	void ActorsTransformAction::Redo()
	{
		for (int i = 0; i < actorsIds.Count(); i++)
		{
			Actor* actor = o2Scene.GetActorByID(actorsIds[i]);
			if (actor)
				*actor->transform = doneTransforms[i];
		}
	}

	void ActorsTransformAction::Undo()
	{
		for (int i = 0; i < actorsIds.Count(); i++)
		{
			Actor* actor = o2Scene.GetActorByID(actorsIds[i]);
			if (actor)
				*actor->transform = beforeTransforms[i];
		}
	}

}

DECLARE_CLASS(Editor::ActorsTransformAction);
