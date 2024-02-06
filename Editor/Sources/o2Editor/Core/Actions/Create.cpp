#include "o2Editor/stdafx.h"
#include "Create.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

namespace Editor
{
	CreateAction::CreateAction()
	{}

	CreateAction::CreateAction(const Vector<SceneEditableObject*>& objects, 
											 SceneEditableObject* parent, SceneEditableObject* prevObject)
	{
		objectsIds = objects.Convert<SceneUID>([](SceneEditableObject* x) { return x->GetID(); });

		objectsData.Set(objects);

		insertParentId = parent ? parent->GetID() : 0;
		insertPrevObjectId = prevObject ? prevObject->GetID() : 0;
	}

	String CreateAction::GetName() const
	{
		return "Create objects";
	}

	void CreateAction::Redo()
	{
		SceneEditableObject* parent = o2Scene.GetEditableObjectByID(insertParentId);
		SceneEditableObject* prevObject = o2Scene.GetEditableObjectByID(insertPrevObjectId);
		Vector<SceneEditableObject*> objects;

		if (parent)
		{
			int insertIdx = parent->GetEditableChildren().IndexOf(prevObject) + 1;

			objectsData.Get(objects);

			for (auto& object : objects)
				parent->AddEditableChild(object, insertIdx++);
		}
		else
		{
			int insertIdx = o2Scene.GetRootEditableObjects().IndexOf(prevObject) + 1;

			objectsData.Get(objects);

			for (auto& object : objects)
				object->SetIndexInSiblings(insertIdx++);
		}

		o2EditorTree.HighlightObjectTreeNode(objects.Last());
		o2EditorSceneScreen.SelectObjectsWithoutAction(objects, false);
	}

	void CreateAction::Undo()
	{
		for (auto& objectId : objectsIds)
		{
			SceneEditableObject* object = o2Scene.GetEditableObjectByID(objectId);
			if (object)
				delete object;
		}

		o2EditorSceneScreen.ClearSelectionWithoutAction();
	}

}
// --- META ---

DECLARE_CLASS(Editor::CreateAction, Editor__CreateAction);
// --- END META ---
