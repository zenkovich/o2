#include "o2Editor/stdafx.h"
#include "Select.h"

#include "o2/Scene/Actor.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	SelectAction::SelectAction()
	{}

	SelectAction::SelectAction(const Vector<SceneEditableObject*>& selectedObjects, 
							   const Vector<SceneEditableObject*>& prevSelectedObjects)
	{
		selectedObjectsIds = selectedObjects.Convert<SceneUID>([](SceneEditableObject* x) { return x->GetID(); });
		prevSelectedObjectsIds = prevSelectedObjects.Convert<SceneUID>([](SceneEditableObject* x) { return x->GetID(); });
	}

	String SelectAction::GetName() const
	{
		return "Actors selection";
	}

	void SelectAction::Redo()
	{
		auto& selScreen = o2EditorSceneScreen;

		selScreen.mSelectedObjects = selectedObjectsIds.Convert<SceneEditableObject*>(
			[&](SceneUID id) { return o2Scene.GetEditableObjectByID(id); });

		selScreen.UpdateTopSelectedObjects();
		selScreen.OnObjectsSelectedFromThis();
		selScreen.mNeedRedraw = true;
	}

	void SelectAction::Undo()
	{
		auto& selScreen = o2EditorSceneScreen;

		selScreen.mSelectedObjects.Clear();
		
		prevSelectedObjectsIds.ForEach([&](SceneUID id) { 
			if (auto obj = o2Scene.GetEditableObjectByID(id))
				selScreen.mSelectedObjects.Add(obj);
		});

		selScreen.UpdateTopSelectedObjects();
		selScreen.OnObjectsSelectedFromThis();
		selScreen.mNeedRedraw = true;
	}

}
// --- META ---

DECLARE_CLASS(Editor::SelectAction, Editor__SelectAction);
// --- END META ---
