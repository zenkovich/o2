#include "LockAction.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	LockAction::LockAction()
	{}

	LockAction::LockAction(const Vector<Actor*>& actors, bool lock):
		lock(lock)
	{
		actorsIds = actors.Select<UInt64>([](Actor* x) { return x->GetID(); });
	}

	String LockAction::GetName() const
	{
		return lock ? "Lock actors" : "Unlock actors";
	}

	void LockAction::Redo()
	{
		for (auto actorId : actorsIds)
		{
			auto actor = o2Scene.GetActorByID(actorId);
			if (actor)
				actor->SetLocked(lock);
		}
	}

	void LockAction::Undo()
	{
		for (auto actorId : actorsIds)
		{
			auto actor = o2Scene.GetActorByID(actorId);
			if (actor)
				actor->SetLocked(!lock);
		}
	}

}