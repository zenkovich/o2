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

		// Returns edit box
		EditBox* GetEditBox() const;

		IOBJECT(FloatProperty);

	protected:
		Ref<EditBox>     mEditBox;    // Edit box 
		CursorEventsArea mDragHangle; // Value changing drag handle

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Called when key was released
		void OnKeyReleased(const Input::Key& key) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Edit box change event
		void OnEdited(const WString& data);

		// Called when drag handle was moved and changes the property value
		void OnDragHandleMoved(const Input::Cursor& cursor);

		// Called when change value move handle pressed, sets cursor infinite mode and stores value to data
		void OnMoveHandlePressed(const Input::Cursor& cursor);

		// Called when change value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnMoveHandleReleased(const Input::Cursor& cursor);
	};
}
// --- META ---

CLASS_BASES_META(Editor::FloatProperty)
{
    BASE_CLASS(Editor::TPropertyField<float>);
    BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::FloatProperty)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEditBox);
    FIELD().PROTECTED().NAME(mDragHangle);
}
END_META;
CLASS_METHODS_META(Editor::FloatProperty)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const FloatProperty&);
    FUNCTION().PUBLIC().SIGNATURE(EditBox*, GetEditBox);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyReleased, const Input::Key&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEdited, const WString&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragHandleMoved, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMoveHandlePressed, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnMoveHandleReleased, const Input::Cursor&);
}
END_META;
// --- END META ---
