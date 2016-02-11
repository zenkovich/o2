#include "LockAction.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

EditorLockAction::EditorLockAction()
{}

EditorLockAction::EditorLockAction(const Vector<Actor*>& actors, bool lock):
	lock(lock)
{
	actorsIds = actors.Select<UInt64>([](Actor* x) { return x->GetId(); });
}

String EditorLockAction::GetName() const
{
	return lock ? "Lock actors" : "Unlock actors";
}

void EditorLockAction::Redo()
{
	for (auto actorId : actorsIds)
	{
		auto actor = o2Scene.GetActorByID(actorId);
		if (actor)
			actor->SetLocked(lock);
	}
}

void EditorLockAction::Undo()
{
	for (auto actorId : actorsIds)
	{
		auto actor = o2Scene.GetActorByID(actorId);
		if (actor)
			actor->SetLocked(!lock);
	}
}
