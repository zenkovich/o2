#include "o2Editor/stdafx.h"
#include "Delete.h"

#include "o2/Scene/Actor.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/SceneTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"
#include "o2/Scene/Scene.h"

namespace Editor
{

	DeleteAction::DeleteAction()
	{}

	DeleteAction::DeleteAction(const Vector<SceneEditableObject*>& objects)
	{
		for (auto object : objects)
		{
			ObjectInfo info;
			info.objectData.SetValue(object);
			info.idx = o2Scene.GetObjectHierarchyIdx(object);

			if (auto parent = object->GetEditableParent())
			{
				info.parentId = parent->GetID();

				auto parentChilds = parent->GetEditablesChildren();
				info.prevObjectId = 0;

				for (auto child : parentChilds)
				{
					if (child == object)
						break;

					info.prevObjectId = child->GetID();
				}
			}
			else
			{
				info.parentId = 0;

				auto rootObjects = o2Scene.GetRootEditableObjects();
				info.prevObjectId = 0;

				for (auto child : rootObjects)
				{
					if (child == object)
						break;

					info.prevObjectId = child->GetID();
				}
			}

			objectsInfos.Add(info);
		}

		objectsInfos.Sort([](auto& a, auto& b) { return a.idx < b.idx; });
	}

	String DeleteAction::GetName() const
	{
		return "Actors deletion";
	}

	void DeleteAction::Redo()
	{
		for (auto info : objectsInfos)
		{
			auto object = o2Scene.GetEditableObjectByID((SceneUID)info.objectData["Value"]["Id"]);
			if (object)
				delete object;
		}

		o2EditorSceneScreen.ClearSelectionWithoutAction();
		o2EditorTree.GetSceneTree()->UpdateNodesView();
	}

	void DeleteAction::Undo()
	{
		SceneEditableObject* lastRestored = nullptr;
		for (auto info : objectsInfos)
		{
			SceneEditableObject* parent = o2Scene.GetEditableObjectByID(info.parentId);
			if (parent)
			{
				SceneUID prevId = info.prevObjectId;
				int idx = parent->GetEditablesChildren().FindIdx([=](SceneEditableObject* x) { return x->GetID() == prevId; }) + 1;

				SceneEditableObject* newObject;
				info.objectData.GetValue(newObject);
				parent->AddEditableChild(newObject, idx);

				o2EditorSceneScreen.SelectObjectWithoutAction(newObject);
				lastRestored = newObject;
			}
			else
			{
				int idx = o2Scene.GetRootActors().FindIdx([&](Actor* x) { return x->GetID() == info.prevObjectId; }) + 1;

				SceneEditableObject* newObject;
				info.objectData.GetValue(newObject);
				newObject->SetIndexInSiblings(idx);

				o2EditorSceneScreen.SelectObjectWithoutAction(newObject);
				lastRestored = newObject;
			}
		}

		o2EditorTree.HightlightObjectTreeNode(lastRestored);
		o2EditorTree.GetSceneTree()->UpdateNodesView();
	}

	bool DeleteAction::ObjectInfo::operator==(const ObjectInfo& other) const
	{
		return objectData == other.objectData && parentId == other.parentId && prevObjectId == other.prevObjectId;
	}
}

DECLARE_CLASS(Editor::DeleteAction);

DECLARE_CLASS(Editor::DeleteAction::ObjectInfo);
