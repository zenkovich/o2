#pragma once

#include "Events/CursorEventsArea.h"
#include "Events/KeyboardEventsListener.h"
#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIEditBox;
	class UIWidget;
}

namespace Editor
{
	// --------------------------------
	// Editor integer property edit box
	// --------------------------------
	class IntegerProperty: public IPropertyField
	{
	public:
		// Default constructor
		IntegerProperty(UIWidget* widget = nullptr);

		// Destructor
		~IntegerProperty();

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Sets value
		void SetValue(int value);

		// Sets value as unknown
		void SetUnknownValue(int defaultValue = 0);

		// Returns value
		int GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		virtual const Type* GetFieldType() const;

		IOBJECT(IntegerProperty);

	protected:
		TargetsVec       mValuesProxies;            // Fields' pointers
		int              mCommonValue = 0;          // Common field value (if not different)
		bool             mValuesDifferent = true;   // Are values different

		UIWidget*        mPropertyWidget = nullptr; // Property root widget, contains editbox and revert button
		UIEditBox*       mEditBox = nullptr;        // Edit box 
		UIButton*        mRevertBtn = nullptr;      // Revert to prototype button
		CursorEventsArea mDragHangle;               // Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(int value);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// Edit box change event
		void OnEdited(const WString& data);

		// It is called when drag handle was moved and changes the property value
		void OnDragHandleMoved(const Input::Cursor& cursor);

		// It is called when key was released
		void OnKeyReleased(const Input::Key& key);

		// It is called when change value move handle pressed, sets cursor infinite mode and stores value to data
		void OnMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnMoveHandleReleased(const Input::Cursor& cursor);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetValueByUser(int value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}

CLASS_BASES_META(Editor::IntegerProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::IntegerProperty)
{
	PROTECTED_FIELD(mValuesProxies);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mDragHangle);
}
END_META;
CLASS_METHODS_META(Editor::IntegerProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SetValue, int);
	PUBLIC_FUNCTION(void, SetUnknownValue, int);
	PUBLIC_FUNCTION(int, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, int);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnEdited, const WString&);
	PROTECTED_FUNCTION(void, OnDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, SetValueByUser, int);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
