#include "stdafx.h"
#include "Create.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	CreateAction::CreateAction()
	{}

	CreateAction::CreateAction(const Vector<SceneEditableObject*>& objects, 
											 SceneEditableObject* parent, SceneEditableObject* prevObject)
	{
		objectsIds = objects.Select<SceneUID>([](SceneEditableObject* x) { return x->GetID(); });

		objectsData.SetValueRaw(objects);

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
			int insertIdx = parent->GetEditablesChildren().Find(prevObject) + 1;

			objectsData.GetValueRaw(objects);

			for (auto object : objects)
				parent->AddEditableChild(object, insertIdx++);
		}
		else
		{
			int insertIdx = o2Scene.GetRootEditableObjects().Find(prevObject) + 1;

			objectsData.GetValueRaw(objects);

			for (auto object : objects)
				object->SetIndexInSiblings(insertIdx++);
		}
	}

	void CreateAction::Undo()
	{
		for (auto objectId : objectsIds)
		{
			SceneEditableObject* object = o2Scene.GetEditableObjectByID(objectId);
			if (object)
				delete object;
		}

		o2EditorSceneScreen.ClearSelectionWithoutAction();
	}

}

DECLARE_CLASS(Editor::CreateAction);
