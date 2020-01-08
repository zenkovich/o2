#include "o2Editor/stdafx.h"
#include "Lock.h"

#include "o2/Scene/Actor.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	LockAction::LockAction()
	{}

	LockAction::LockAction(const Vector<SceneEditableObject*>& objects, bool lock):
		lock(lock)
	{
		objectsIds = objects.Select<SceneUID>([](SceneEditableObject* x) { return x->GetID(); });
	}

	String LockAction::GetName() const
	{
		return lock ? "Lock actors" : "Unlock actors";
	}

	void LockAction::Redo()
	{
		for (auto id : objectsIds)
		{
			auto object = o2Scene.GetEditableObjectByID(id);
			if (object)
				object->SetLocked(lock);
		}
	}

	void LockAction::Undo()
	{
		for (auto id : objectsIds)
		{
			auto object = o2Scene.GetEditableObjectByID(id);
			if (object)
				object->SetLocked(!lock);
		}
	}

}

DECLARE_CLASS(Editor::LockAction);
