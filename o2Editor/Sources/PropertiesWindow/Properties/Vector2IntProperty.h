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
	// Editor integer vector property
	// ------------------------------
	class Vec2IProperty: public IPropertyField
	{
	public:
		// Default constructor
		Vec2IProperty(UIWidget* widget = nullptr);

		// Destructor
		~Vec2IProperty();

		// Sets fields
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Sets value
		void SetValue(const Vec2I& value);

		// Sets value X
		void SetValueX(int value);

		// Sets value Y
		void SetValueY(int value);

		// Sets value as unknown
		void SetUnknownValue(const Vec2I& defaultValue = Vec2I());

		// Sets value X as unknown
		void SetXUnknownValue(int defaultValue = 0);

		// Sets value Y as unknown
		void SetYUnknownValue(int defaultValue = 0);

		// Returns value
		Vec2I GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(Vec2IProperty);

	protected:
		Function<void(void*, const Vec2I&)> mAssignFunc;  // Value assign function
		Function<Vec2I(void*)>              mGetFunc;     // Get value function

		Function<void(void*, int)>          mXAssignFunc; // X Value assign function
		Function<int(void*)>                mXGetFunc;    // Get X value function
										    
		Function<void(void*, int)>          mYAssignFunc; // Y Value assign function
		Function<int(void*)>                mYGetFunc;    // Get Y value function

		TargetsVec mValuesPointers;           // Fields' pointers
		Vec2I      mCommonValue;              // Common field value (if not different)
		bool       mXValuesDifferent = true;  // Are left values different
		bool       mYValuesDifferent = true;  // Are bottom values different\
				 								
		UIWidget*  mPropertyWidget = nullptr; // Property root widget, contains editboxes and revert button
		UIButton*  mRevertBtn = nullptr;      // Revert to prototype button
		UIEditBox* mXEditBox = nullptr;       // X Edit box 
		UIEditBox* mYEditBox = nullptr;       // Y Edit box 

		CursorEventsArea mXDragHangle; // X Value changing drag handle
		CursorEventsArea mYDragHangle; // Y Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(const Vec2I& value);

		// Sets common value X
		void SetCommonValueX(int value);

		// Sets common value Y
		void SetCommonValueY(int value);

		// Checks value for reverting to prototype
		void CheckRevertableState();

		// X Edit box change event
		void OnXEdited(const WString& data);

		// Y Edit box change event
		void OnYEdited(const WString& data);

		// It is called when key was released
		void OnKeyReleased(const Input::Key& key);

		// It is called when drag handle was moved and changes the property value
		void OnXDragHandleMoved(const Input::Cursor& cursor);

		// It is called when drag handle was moved and changes the property value
		void OnYDragHandleMoved(const Input::Cursor& cursor);

		// It is called when change value move handle pressed, sets cursor infinite mode and stores value to data
		void OnXMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change value move handle pressed, sets cursor infinite mode and stores value to data
		void OnYMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnXMoveHandleReleased(const Input::Cursor& cursor);

		// It is called when change value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnYMoveHandleReleased(const Input::Cursor& cursor);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetXValueByUser(int value);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetYValueByUser(int value);

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();
	};
}
