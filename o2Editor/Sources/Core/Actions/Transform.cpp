#include "stdafx.h"
#include "Transform.h"

#include "Scene/Actor.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	TransformAction::TransformAction()
	{}

	TransformAction::TransformAction(const Vector<SceneEditableObject*>& actors)
	{
		objectsIds = actors.Select<UInt64>([](SceneEditableObject* x) { return x->GetID(); });
		GetTransforms(objectsIds, beforeTransforms);
	}

	void TransformAction::Completed()
	{
		GetTransforms(objectsIds, doneTransforms);
	}

	String TransformAction::GetName() const
	{
		return "Actors transformation";
	}

	void TransformAction::Redo()
	{
		SetTransforms(objectsIds, doneTransforms);
	}

	void TransformAction::Undo()
	{
		SetTransforms(objectsIds, beforeTransforms);
	}

	void TransformAction::GetTransforms(const Vector<SceneUID>& objectIds, Vector<Transform>& transforms)
	{
		transforms = objectIds.Select<Transform>([=](SceneUID id)
		{
			SceneEditableObject* object = o2Scene.GetEditableObjectByID(id);
			if (object)
			{
				Transform res;
				res.transform = object->GetTransform();
				res.layout = object->GetLayout();
				return res;
			}

			return Transform();
		});
	}

	void TransformAction::SetTransforms(const Vector<SceneUID>& objectIds, Vector<Transform>& transforms)
	{
		for (int i = 0; i < objectsIds.Count(); i++)
		{
			SceneEditableObject* object = o2Scene.GetEditableObjectByID(objectsIds[i]);
			if (object)
			{
				object->SetTransform(transforms[i].transform);
				object->SetLayout(transforms[i].layout);
			}
		}
	}

	bool TransformAction::Transform::operator==(const Transform& other) const
	{
		return transform == other.transform && layout == other.layout;
	}

}

DECLARE_CLASS(Editor::TransformAction);