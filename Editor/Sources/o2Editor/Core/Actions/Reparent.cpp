#include "o2Editor/stdafx.h"
#include "Reparent.h"

#include "o2/Scene/Actor.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/SceneTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

namespace Editor
{
	ReparentAction::ReparentAction()
	{}

	ReparentAction::ReparentAction(const Vector<SceneEditableObject*>& beginObjects)
	{
		for (auto object : beginObjects)
		{
			ObjectInfo* info = mnew ObjectInfo();

			SceneEditableObject* parent = object->GetEditableParent();

			Vector<SceneEditableObject*> parentChildren = parent ? 
				parent->GetEditablesChildren() : 
				o2Scene.GetRootActors().Select<SceneEditableObject*>([](Actor* x) { return dynamic_cast<SceneEditableObject*>(x); });

			int actorIdx = parentChildren.Find(object);

			info->objectId = object->GetID();
			info->objectHierarchyIdx = o2Scene.GetObjectHierarchyIdx(object);
			info->lastParentId = parent ? parent->GetID() : 0;
			info->lastPrevObjectId = actorIdx > 0 ? parentChildren[actorIdx - 1]->GetID() : 0;
			info->transform = object->GetTransform();

			objectsInfos.Add(info);
		}

		objectsInfos.Sort([](ObjectInfo* a, ObjectInfo* b) { return a->objectHierarchyIdx < b->objectHierarchyIdx; });
	}

	ReparentAction::~ReparentAction()
	{
		for (auto info : objectsInfos)
			delete info;
	}

	void ReparentAction::ObjectsReparented(SceneEditableObject* newParent, SceneEditableObject* prevActor)
	{
		newParentId = newParent ? newParent->GetID() : 0;
		newPrevObjectId = prevActor ? prevActor->GetID() : 0;
	}

	String ReparentAction::GetName() const
	{
		return "Actors rearrange";
	}

	void ReparentAction::Redo()
	{
		SceneEditableObject* parent = o2Scene.GetEditableObjectByID(newParentId);
		SceneEditableObject* prevObject = o2Scene.GetEditableObjectByID(newPrevObjectId);

		if (parent)
		{
			int insertIdx = parent->GetEditablesChildren().Find(prevObject) + 1;

			for (auto info : objectsInfos)
			{
				SceneEditableObject* object = o2Scene.GetEditableObjectByID(info->objectId);

				object->SetEditableParent(nullptr);
				parent->AddEditableChild(object, insertIdx++);
				object->SetTransform(info->transform);
			}
		}
		else
		{
			int insertIdx = 0;
			
			if (auto prevActor = dynamic_cast<Actor*>(prevObject))
				insertIdx = o2Scene.GetRootActors().Find(prevActor) + 1;

			for (auto info : objectsInfos)
			{
				SceneEditableObject* object = o2Scene.GetEditableObjectByID(info->objectId);

				object->SetEditableParent(nullptr);
				object->SetIndexInSiblings(insertIdx++);
				object->SetTransform(info->transform);
			}
		}

		o2EditorTree.GetSceneTree()->UpdateNodesView();
	}

	void ReparentAction::Undo()
	{
		for (auto info : objectsInfos)
		{
			SceneEditableObject* object = o2Scene.GetEditableObjectByID(info->objectId);
			SceneEditableObject* parent = o2Scene.GetEditableObjectByID(info->lastParentId);
			SceneEditableObject* prevObject = o2Scene.GetEditableObjectByID(info->lastPrevObjectId);

			object->SetEditableParent(nullptr);

			if (parent)
			{
				int idx = parent->GetEditablesChildren().Find(prevObject) + 1;
				parent->AddEditableChild(object, idx);
				object->SetTransform(info->transform);
			}
			else
			{
				int idx = o2Scene.GetRootEditableObjects().Find(prevObject) + 1;
				object->SetIndexInSiblings(idx);
				object->SetTransform(info->transform);
			}
		}

		o2EditorTree.GetSceneTree()->UpdateNodesView();
	}
}

DECLARE_CLASS(Editor::ReparentAction);
