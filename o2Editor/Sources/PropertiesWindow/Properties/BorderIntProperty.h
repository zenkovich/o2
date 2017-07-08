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
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Reverts value to prototype value
		void Revert();

		// Returns root widget
		UIWidget* GetWidget() const;

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
		void CheckRevertableState();

		// Returns value
		BorderI GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(BorderIProperty);

	protected:
		Function<void(void*, const BorderI&)> mAssignFunc;      // Value assign function
		Function<BorderI(void*)>              mGetFunc;         // Get value function

		Function<void(void*, int)>            mLeftAssignFunc;  // Left Value assign function
		Function<int(void*)>                  mLeftGetFunc;     // Get left value function

		Function<void(void*, int)>            mRightAssignFunc; // Right Value assign function
		Function<int(void*)>                  mRightGetFunc;    // Get right value function

		Function<void(void*, int)>            mTopAssignFunc;   // Top Value assign function
		Function<int(void*)>                  mTopGetFunc;      // Get top value function

		Function<void(void*, int)>            mBottomAssignFunc; // Bottom Value assign function
		Function<int(void*)>                  mBottomGetFunc;    // Get bottom value function

		TargetsVec mValuesPointers;               // Fields' pointers
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

		// Calls when drag handle was moved and changes the property value
		void OnLeftDragHandleMoved(const Input::Cursor& cursor);

		// Calls when drag handle was moved and changes the property value
		void OnRightDragHandleMoved(const Input::Cursor& cursor);

		// Calls when drag handle was moved and changes the property value
		void OnTopDragHandleMoved(const Input::Cursor& cursor);

		// Calls when drag handle was moved and changes the property value
		void OnBottomDragHandleMoved(const Input::Cursor& cursor);

		// Calls when key was released and changes focused value
		void OnKeyReleased(const Input::Key& key);

		// Calls when change left value move handle pressed, sets cursor infinite mode and stores value to data
		void OnLeftMoveHandlePressed(const Input::Cursor& cursor);

		// Calls when change left value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnLeftMoveHandleReleased(const Input::Cursor& cursor);

		// Calls when change right value move handle pressed, sets cursor infinite mode and stores value to data
		void OnRightMoveHandlePressed(const Input::Cursor& cursor);

		// Calls when change right value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnRightMoveHandleReleased(const Input::Cursor& cursor);

		// Calls when change top value move handle pressed, sets cursor infinite mode and stores value to data
		void OnTopMoveHandlePressed(const Input::Cursor& cursor);

		// Calls when change top value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnTopMoveHandleReleased(const Input::Cursor& cursor);

		// Calls when change bottom value move handle pressed, sets cursor infinite mode and stores value to data
		void OnBottomMoveHandlePressed(const Input::Cursor& cursor);

		// Calls when change bottom value move handle  released, turns off cursor infinite mode, 
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

		// Checks that left value was changed and calls onChangeCompleted
		void CheckLeftValueChangeCompleted();

		// Checks that right value was changed and calls onChangeCompleted
		void CheckRightValueChangeCompleted();

		// Checks that bottom value was changed and calls onChangeCompleted
		void CheckBottomValueChangeCompleted();

		// Checks that top value was changed and calls onChangeCompleted
		void CheckTopValueChangeCompleted();

		// Stores left values to data
		void StoreLeftValues(Vector<DataNode>& data) const;

		// Stores right values to data
		void StoreRightValues(Vector<DataNode>& data) const;

		// Stores top values to data
		void StoreTopValues(Vector<DataNode>& data) const;

		// Stores bottom values to data
		void StoreBottomValues(Vector<DataNode>& data) const;
	};
}
