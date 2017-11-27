#include "stdafx.h"
#include "CreateActors.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	CreateActorsAction::CreateActorsAction()
	{}

	CreateActorsAction::CreateActorsAction(const Vector<Actor*>& actors, Actor* parent, Actor* prevActor)
	{
		actorsIds = actors.Select<UInt64>([](Actor* x) { return x->GetID(); });

		actorsData.SetValueRaw(actors);

		insertParentId = parent ? parent->GetID() : 0;
		insertPrevActorId = prevActor ? prevActor->GetID() : 0;
	}

	String CreateActorsAction::GetName() const
	{
		return "New actors";
	}

	void CreateActorsAction::Redo()
	{
		Actor* parent = o2Scene.GetActorByID(insertParentId);
		Actor* prevActor = o2Scene.GetActorByID(insertPrevActorId);
		Vector<Actor*> actors;

		if (parent)
		{
			int insertIdx = parent->GetChildren().Find(prevActor) + 1;

			actorsData.GetValueRaw(actors);

			for (auto actor : actors)
				parent->AddChild(actor, insertIdx++);
		}
		else
		{
			int insertIdx = o2Scene.GetRootActors().Find(prevActor) + 1;

			actorsData.GetValueRaw(actors);

			for (auto actor : actors)
				actor->SetPositionIndexInParent(insertIdx++);
		}
	}

	void CreateActorsAction::Undo()
	{
		for (auto actorId : actorsIds)
		{
			Actor* actor = o2Scene.GetActorByID(actorId);
			if (actor)
				delete actor;
		}

		o2EditorSceneScreen.ClearSelectionWithoutAction();
	}

}

DECLARE_CLASS(Editor::CreateActorsAction);
