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
	// --------------------------------
	// Editor integer property edit box
	// --------------------------------
	class IntegerProperty: public TPropertyField<int>, public KeyboardEventsListener
	{
	public:
		// Default constructor
		IntegerProperty();

		// Copy constructor
		IntegerProperty(const IntegerProperty& other);

		// Copy operator
		IntegerProperty& operator=(const IntegerProperty& other);

		IOBJECT(IntegerProperty);

	protected:
		EditBox*         mEditBox = nullptr;        // Edit box 
		CursorEventsArea mDragHangle;               // Value changing drag handle

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

CLASS_BASES_META(Editor::IntegerProperty)
{
	BASE_CLASS(Editor::TPropertyField<int>);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::IntegerProperty)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEditBox);
	FIELD().PROTECTED().NAME(mDragHangle);
}
END_META;
CLASS_METHODS_META(Editor::IntegerProperty)
{

	FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
	FUNCTION().PROTECTED().SIGNATURE(void, OnKeyReleased, const Input::Key&);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEdited, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragHandleMoved, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnMoveHandlePressed, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnMoveHandleReleased, const Input::Cursor&);
}
END_META;
