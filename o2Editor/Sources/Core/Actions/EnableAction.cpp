#include "EnableAction.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

EditorEnableAction::EditorEnableAction()
{}

EditorEnableAction::EditorEnableAction(const Vector<Actor*>& actors, bool enable):
	enable(enable)
{
	actorsIds = actors.Select<UInt64>([](Actor* x) { return x->GetId(); });
}

String EditorEnableAction::GetName() const
{
	return enable ? "Enable actors" : "Disable actors";
}

void EditorEnableAction::Redo()
{
	for (auto actorId : actorsIds)
	{
		auto actor = o2Scene.GetActorByID(actorId);
		if (actor)
			actor->SetEnabled(enable);
	}
}

void EditorEnableAction::Undo()
{
	for (auto actorId : actorsIds)
	{
		auto actor = o2Scene.GetActorByID(actorId);
		if (actor)
			actor->SetEnabled(!enable);
	}
}
