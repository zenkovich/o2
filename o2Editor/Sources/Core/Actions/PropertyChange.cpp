#include "stdafx.h"
#include "PropertyChange.h"

#include "Scene/Actor.h"
#include "Scene/Component.h"
#include "Scene/Scene.h"
#include "Utils/Editor/SceneEditableObject.h"

namespace Editor
{
	PropertyChangeAction::PropertyChangeAction()
	{}

	PropertyChangeAction::PropertyChangeAction(const Vector<SceneEditableObject*>& objects,
											   const String& propertyPath,
											   const Vector<DataNode>& beforeValues,
											   const Vector<DataNode>& afterValues) :
		objectsIds(objects.Select<SceneUID>([](const SceneEditableObject* x) { return x->GetID(); })),
		propertyPath(propertyPath), beforeValues(beforeValues), afterValues(afterValues)
	{}

	String PropertyChangeAction::GetName() const
	{
		return "Property changed";
	}

	void PropertyChangeAction::Redo()
	{
		SetProperties(afterValues);
	}

	void PropertyChangeAction::Undo()
	{
		SetProperties(beforeValues);
	}

	void PropertyChangeAction::SetProperties(Vector<DataNode>& values)
	{
		Vector<SceneEditableObject*> objects = objectsIds.Select<SceneEditableObject*>([](SceneUID id) { 
			return o2Scene.GetEditableObjectByID(id); });

		const Type* componentType = nullptr;
		String finalPropertyPath = propertyPath;
		if (propertyPath.StartsWith("component:"))
		{
			int c = ((String)"component:").Length();

			String typeName = propertyPath.SubStr(c, propertyPath.Find('/', c));

			c += typeName.Length() + 1;
			finalPropertyPath.Erase(0, c);

			componentType = o2Reflection.GetType(typeName);
		}

		int i = 0;
		for (auto object : objects)
		{
			if (!object)
			{
				continue;
				i++;
			}

			FieldInfo* fi = nullptr;
			void* ptr = nullptr;

			if (componentType)
			{
				if (Actor* actor = dynamic_cast<Actor*>(object))
				{
					Component* component = actor->GetComponent(componentType);
					if (component)
						ptr = component->GetType().GetFieldPtr(component, finalPropertyPath, fi);
				}
			}
			else
			{
				ptr = object->GetType().GetFieldPtr(object, finalPropertyPath, fi);
			}

			if (fi && ptr)
				fi->Deserialize(ptr, values[i]);

			object->OnChanged();

			i++;
		}
	}
}

DECLARE_CLASS(Editor::PropertyChangeAction);
