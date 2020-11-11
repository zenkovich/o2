#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace o2
{
	class EditBox;
}

namespace Editor
{
	// ------------------------------
	// Editor float property edit box
	// ------------------------------
	class FloatProperty: public TPropertyField<float>, public KeyboardEventsListener
	{
	public:
		// Default constructor
		FloatProperty();

		// Copy constructor
		FloatProperty(const FloatProperty& other);

		// Copy operator
		FloatProperty& operator=(const FloatProperty& other);

		IOBJECT(FloatProperty);

	protected:
		EditBox*         mEditBox = nullptr; // Edit box 
		CursorEventsArea mDragHangle;        // Value changing drag handle

	protected:
		// Updates value view
		void UpdateValueView() override;

		// It is called when key was released
		void OnKeyReleased(const Input::Key& key) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Edit box change event
		void OnEdited(const WString& data);

		// It is called when drag handle was moved and changes the property value
		void OnDragHandleMoved(const Input::Cursor& cursor);

		// It is called when change value move handle pressed, sets cursor infinite mode and stores value to data
		void OnMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnMoveHandleReleased(const Input::Cursor& cursor);
	};
}

CLASS_BASES_META(Editor::FloatProperty)
{
	BASE_CLASS(Editor::TPropertyField<float>);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::FloatProperty)
{
	PROTECTED_FIELD(mEditBox).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mDragHangle);
}
END_META;
CLASS_METHODS_META(Editor::FloatProperty)
{

	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnEdited, const WString&);
	PROTECTED_FUNCTION(void, OnDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnMoveHandleReleased, const Input::Cursor&);
}
END_META;
