#pragma once

#include "o2Editor/Core/Actions/IAction.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
}

namespace Editor
{
	// -----------------------------------------------------
	// Scene object property change action.
	// Storing path to value, values before and after change
	// -----------------------------------------------------
	class PropertyChangeAction: public IAction
	{
	public:
		Vector<SceneUID> objectsIds;
		String           propertyPath;
		Vector<DataValue> beforeValues;
		Vector<DataValue> afterValues;

	public:
		// Default constructor
		PropertyChangeAction();

		// Constructor with all data
		PropertyChangeAction(const Vector<SceneEditableObject*>& objects,
								   const String& propertyPath, const Vector<DataValue>& beforeValues,
								   const Vector<DataValue>& afterValues);

		// Returns name of action
		String GetName() const;

		// Sets object's properties value as after change
		void Redo();

		// Sets object's properties value as before change
		void Undo();

		SERIALIZABLE(PropertyChangeAction);

	protected:
		// Sets object's properties values
		void SetProperties(Vector<DataValue>& value);
	};
}

CLASS_BASES_META(Editor::PropertyChangeAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::PropertyChangeAction)
{
	PUBLIC_FIELD(objectsIds);
	PUBLIC_FIELD(propertyPath);
	PUBLIC_FIELD(beforeValues);
	PUBLIC_FIELD(afterValues);
}
END_META;
CLASS_METHODS_META(Editor::PropertyChangeAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
	PROTECTED_FUNCTION(void, SetProperties, Vector<DataValue>&);
}
END_META;
