#include "stdafx.h"
#include "Select.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	SelectAction::SelectAction()
	{}

	SelectAction::SelectAction(const Vector<SceneEditableObject*>& selectedObjects, 
							   const Vector<SceneEditableObject*>& prevSelectedObjects)
	{
		selectedObjectsIds = selectedObjects.Select<SceneUID>([](SceneEditableObject* x) { return x->GetID(); });
		prevSelectedObjectsIds = prevSelectedObjects.Select<SceneUID>([](SceneEditableObject* x) { return x->GetID(); });
	}

	String SelectAction::GetName() const
	{
		return "Actors selection";
	}

	void SelectAction::Redo()
	{
		auto& selScreen = o2EditorSceneScreen;

		selScreen.mSelectedObjects = selectedObjectsIds.Select<SceneEditableObject*>(
			[&](SceneUID id) { return o2Scene.GetEditableObjectByID(id); });

		selScreen.UpdateTopSelectedObjects();
		selScreen.OnObjectsSelectedFromThis();
		selScreen.mNeedRedraw = true;
	}

	void SelectAction::Undo()
	{
		auto& selScreen = o2EditorSceneScreen;

		selScreen.mSelectedObjects = prevSelectedObjectsIds.Select<SceneEditableObject*>(
			[&](SceneUID id) { return o2Scene.GetEditableObjectByID(id); });

		selScreen.UpdateTopSelectedObjects();
		selScreen.OnObjectsSelectedFromThis();
		selScreen.mNeedRedraw = true;
	}

}

DECLARE_CLASS(Editor::SelectAction);
