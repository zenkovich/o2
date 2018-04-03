#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/CursorEventsArea.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIEditBox;
	class UIHorizontalLayout;
	class UIWidget;
}

namespace Editor
{
	// ------------------------------
	// Editor integer border property
	// ------------------------------
	class BorderIProperty: public IPropertyField
	{
	public:
		// Default constructor
		BorderIProperty(UIWidget* widget = nullptr);

		// Destructor
		~BorderIProperty();

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Sets value
		void SetValue(const BorderI& value);

		// Sets value left
		void SetValueLeft(int value);

		// Sets value right
		void SetValueRight(int value);

		// Sets value top
		void SetValueTop(int value);

		// Sets value bottom
		void SetValueBottom(int value);

		// Sets value as unknown
		void SetUnknownValue(const BorderI& defaultValue = BorderI());

		// Sets value left as unknown
		void SetLeftUnknownValue(int defaultValue = 0.0f);

		// Sets value right as unknown
		void SetRightUnknownValue(int defaultValue = 0.0f);

		// Sets value top as unknown
		void SetTopUnknownValue(int defaultValue = 0.0f);

		// Sets value bottom as unknown
		void SetBottomUnknownValue(int defaultValue = 0.0f);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// Returns value
		BorderI GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(BorderIProperty);

	protected:
		TargetsVec mValuesProxies;               // Fields' pointers
		BorderI    mCommonValue;                  // Common field value (if not different)
		bool       mLeftValuesDifferent = true;   // Are left values different
		bool       mBottomValuesDifferent = true; // Are bottom values different
		bool       mRightValuesDifferent = true;  // Are right values different
		bool       mTopValuesDifferent = true;    // Are top values different

		UIWidget*  mPropertyWidget = nullptr; // Horizontal edit boxes layout
		UIButton*  mRevertBtn = nullptr;      // Property revert button to prototype source
		UIEditBox* mLeftEditBox = nullptr;    // Left Edit box 
		UIEditBox* mBottomEditBox = nullptr;  // Bottom Edit box 
		UIEditBox* mRightEditBox = nullptr;   // Right Edit box 
		UIEditBox* mTopEditBox = nullptr;     // Top Edit box 

		CursorEventsArea mLeftDragHangle;   // Left Value changing drag handle
		CursorEventsArea mRightDragHangle;  // Right Value changing drag handle
		CursorEventsArea mTopDragHangle;    // Top Value changing drag handle
		CursorEventsArea mBottomDragHangle; // Bottom Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(const BorderI& value);

		// Sets common value left
		void SetCommonValueLeft(int value);

		// Sets common value right
		void SetCommonValueRight(int value);

		// Sets common value top
		void SetCommonValueTop(int value);

		// Sets common value bottom
		void SetCommonValueBottom(int value);

		// Left Edit box change event
		void OnLeftEdited(const WString& data);

		// Bottom Edit box change event
		void OnBottomEdited(const WString& data);

		// Right Edit box change event
		void OnRightEdited(const WString& data);

		// Top Edit box change event
		void OnTopEdited(const WString& data);

		// It is called when drag handle was moved and changes the property value
		void OnLeftDragHandleMoved(const Input::Cursor& cursor);

		// It is called when drag handle was moved and changes the property value
		void OnRightDragHandleMoved(const Input::Cursor& cursor);

		// It is called when drag handle was moved and changes the property value
		void OnTopDragHandleMoved(const Input::Cursor& cursor);

		// It is called when drag handle was moved and changes the property value
		void OnBottomDragHandleMoved(const Input::Cursor& cursor);

		// It is called when key was released and changes focused value
		void OnKeyReleased(const Input::Key& key);

		// It is called when change left value move handle pressed, sets cursor infinite mode and stores value to data
		void OnLeftMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change left value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnLeftMoveHandleReleased(const Input::Cursor& cursor);

		// It is called when change right value move handle pressed, sets cursor infinite mode and stores value to data
		void OnRightMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change right value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnRightMoveHandleReleased(const Input::Cursor& cursor);

		// It is called when change top value move handle pressed, sets cursor infinite mode and stores value to data
		void OnTopMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change top value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnTopMoveHandleReleased(const Input::Cursor& cursor);

		// It is called when change bottom value move handle pressed, sets cursor infinite mode and stores value to data
		void OnBottomMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change bottom value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnBottomMoveHandleReleased(const Input::Cursor& cursor);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetLeftValueByUser(int value);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetRightValueByUser(int value);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetBottomValueByUser(int value);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetTopValueByUser(int value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}

CLASS_BASES_META(Editor::BorderIProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::BorderIProperty)
{
	PROTECTED_FIELD(mValuesProxies);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mLeftValuesDifferent);
	PROTECTED_FIELD(mBottomValuesDifferent);
	PROTECTED_FIELD(mRightValuesDifferent);
	PROTECTED_FIELD(mTopValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mLeftEditBox);
	PROTECTED_FIELD(mBottomEditBox);
	PROTECTED_FIELD(mRightEditBox);
	PROTECTED_FIELD(mTopEditBox);
	PROTECTED_FIELD(mLeftDragHangle);
	PROTECTED_FIELD(mRightDragHangle);
	PROTECTED_FIELD(mTopDragHangle);
	PROTECTED_FIELD(mBottomDragHangle);
}
END_META;
CLASS_METHODS_META(Editor::BorderIProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SetValue, const BorderI&);
	PUBLIC_FUNCTION(void, SetValueLeft, int);
	PUBLIC_FUNCTION(void, SetValueRight, int);
	PUBLIC_FUNCTION(void, SetValueTop, int);
	PUBLIC_FUNCTION(void, SetValueBottom, int);
	PUBLIC_FUNCTION(void, SetUnknownValue, const BorderI&);
	PUBLIC_FUNCTION(void, SetLeftUnknownValue, int);
	PUBLIC_FUNCTION(void, SetRightUnknownValue, int);
	PUBLIC_FUNCTION(void, SetTopUnknownValue, int);
	PUBLIC_FUNCTION(void, SetBottomUnknownValue, int);
	PUBLIC_FUNCTION(void, CheckRevertableState);
	PUBLIC_FUNCTION(BorderI, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const BorderI&);
	PROTECTED_FUNCTION(void, SetCommonValueLeft, int);
	PROTECTED_FUNCTION(void, SetCommonValueRight, int);
	PROTECTED_FUNCTION(void, SetCommonValueTop, int);
	PROTECTED_FUNCTION(void, SetCommonValueBottom, int);
	PROTECTED_FUNCTION(void, OnLeftEdited, const WString&);
	PROTECTED_FUNCTION(void, OnBottomEdited, const WString&);
	PROTECTED_FUNCTION(void, OnRightEdited, const WString&);
	PROTECTED_FUNCTION(void, OnTopEdited, const WString&);
	PROTECTED_FUNCTION(void, OnLeftDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnRightDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnTopDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnBottomDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnLeftMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnLeftMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnRightMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnRightMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnTopMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnTopMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnBottomMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnBottomMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, SetLeftValueByUser, int);
	PROTECTED_FUNCTION(void, SetRightValueByUser, int);
	PROTECTED_FUNCTION(void, SetBottomValueByUser, int);
	PROTECTED_FUNCTION(void, SetTopValueByUser, int);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
