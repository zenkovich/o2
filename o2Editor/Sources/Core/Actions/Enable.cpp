#include "stdafx.h"
#include "Enable.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	EnableAction::EnableAction()
	{}

	EnableAction::EnableAction(const Vector<SceneEditableObject*>& objects, bool enable):
		enable(enable)
	{
		objectsIds = objects.Select<SceneUID>([](SceneEditableObject* x) { return x->GetID(); });
	}

	String EnableAction::GetName() const
	{
		return enable ? "Enable actors" : "Disable actors";
	}

	void EnableAction::Redo()
	{
		for (auto id : objectsIds)
		{
			auto object = o2Scene.GetEditableObjectByID(id);
			if (object)
				object->SetEnabled(enable);
		}
	}

	void EnableAction::Undo()
	{
		for (auto id : objectsIds)
		{
			auto object = o2Scene.GetEditableObjectByID(id);
			if (object)
				object->SetEnabled(!enable);
		}
	}

}

DECLARE_CLASS(Editor::EnableAction);
