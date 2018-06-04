#include "stdafx.h"
#include "ActorsPropertyChange.h"

#include "Scene/Actor.h"
#include "Scene/Component.h"
#include "Scene/Scene.h"

namespace Editor
{
	ActorsPropertyChangeAction::ActorsPropertyChangeAction()
	{}

	ActorsPropertyChangeAction::ActorsPropertyChangeAction(const Vector<Actor*>& actors,
														   const String& propertyPath,
														   const Vector<DataNode>& beforeValues,
														   const Vector<DataNode>& afterValues) :
		actorsIds(actors.Select<UInt64>([](const Actor* x) { return x->GetID(); })), propertyPath(propertyPath),
		beforeValues(beforeValues), afterValues(afterValues)
	{}

	String ActorsPropertyChangeAction::GetName() const
	{
		return "Property changed";
	}

	void ActorsPropertyChangeAction::Redo()
	{
		SetProperties(afterValues);
	}

	void ActorsPropertyChangeAction::Undo()
	{
		SetProperties(beforeValues);
	}

	void ActorsPropertyChangeAction::SetProperties(Vector<DataNode>& values)
	{
		Vector<Actor*> actors = actorsIds.Select<Actor*>([](UInt64 id) { return o2Scene.GetActorByID(id); });

		const Type* componentType = nullptr;
		String finalPropertyPath = propertyPath;
		if (propertyPath.StartsWith("component:"))
		{
			int c = ((String)"component:").Length();
			String typeName = propertyPath.SubStr(c, propertyPath.Find('/', c));
			componentType = o2Reflection.GetType(typeName);

			if (componentType)
				finalPropertyPath.Erase(0, c);
		}

		int i = 0;
		for (auto actor : actors)
		{
			if (actor)
			{
				FieldInfo* fi = nullptr;
				void* ptr = nullptr;

				if (componentType)
				{
					Component* component = actor->GetComponent(componentType);
					if (component)
						ptr = component->GetType().GetFieldPtr(component, finalPropertyPath, fi);
				}
				else
				{
					ptr = actor->GetType().GetFieldPtr(actor, finalPropertyPath, fi);
				}

				if (fi && ptr)
					fi->Deserialize(ptr, values[i]);

				actor->OnChanged();
			}

			i++;
		}
	}
}

DECLARE_CLASS(Editor::ActorsPropertyChangeAction);
