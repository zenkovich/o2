#pragma once

#include "o2/Utils/Types/Containers/Vector.h"
#include "o2Editor/Core/Actions/IAction.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
}

namespace Editor
{
	// -----------------------------
	// Scene objects creation action
	// -----------------------------
	class CreateAction: public IAction
	{
	public:
		DataDocument     objectsData;
		Vector<SceneUID> objectsIds;
		SceneUID         insertParentId;
		SceneUID         insertPrevObjectId;

	public:
		// Default constructor
		CreateAction();

		// Constructor wit created actors and their places in their parents
		CreateAction(const Vector<SceneEditableObject*>& objects, SceneEditableObject* parent, SceneEditableObject* prevObject);

		// Returns name of action
		String GetName() const;

		// Creates all objects again
		void Redo();

		// Removes created objects
		void Undo();

		SERIALIZABLE(CreateAction);
	};

}

CLASS_BASES_META(Editor::CreateAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::CreateAction)
{
	FIELD().NAME(objectsData).PUBLIC();
	FIELD().NAME(objectsIds).PUBLIC();
	FIELD().NAME(insertParentId).PUBLIC();
	FIELD().NAME(insertPrevObjectId).PUBLIC();
}
END_META;
CLASS_METHODS_META(Editor::CreateAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
