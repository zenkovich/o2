#pragma once

#include "Events/KeyboardEventsListener.h"
#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Events/CursorEventsArea.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIEditBox;
	class UIEditBox;
	class UIHorizontalLayout;
	class UIWidget;
}

namespace Editor
{
	// ----------------------------
	// Editor float vector property
	// ----------------------------
	class Vec2FProperty: public IPropertyField, public KeyboardEventsListener
	{
	public:
		// Default constructor
		Vec2FProperty(UIWidget* widget = nullptr);

		// Destructor
		~Vec2FProperty();

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Sets value
		void SetValue(const Vec2F& value);

		// Sets value X
		void SetValueX(float value);

		// Sets value Y
		void SetValueY(float value);

		// Sets value as unknown
		void SetUnknownValue(const Vec2F& defaultValue = Vec2F());

		// Sets value X as unknown
		void SetXUnknownValue(float defaultValue = 0.0f);

		// Sets value Y as unknown
		void SetYUnknownValue(float defaultValue = 0.0f);

		// Returns value
		Vec2F GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(Vec2FProperty);

	protected:
		TargetsVec mValuesProxies;            // Fields' pointers
		Vec2F      mCommonValue;              // Common field value (if not different)
		bool       mXValuesDifferent = true;  // Are left values different
		bool       mYValuesDifferent = true;  // Are bottom values different\
					 								
		UIWidget*  mPropertyWidget = nullptr; // Property root widget, contains editbox and revert button
		UIButton*  mRevertBtn = nullptr;      // Revert to prototype button
		UIEditBox* mXEditBox = nullptr;       // X Edit box 
		UIEditBox* mYEditBox = nullptr;       // Y Edit box 
												 
		CursorEventsArea mXDragHangle; // X Value changing drag handle
		CursorEventsArea mYDragHangle; // Y Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(const Vec2F& value);

		// Sets common value X
		void SetCommonValueX(float value);

		// Sets common value Y
		void SetCommonValueY(float value);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// X Edit box change event
		void OnXEdited(const WString& data);

		// Y Edit box change event
		void OnYEdited(const WString& data);

		// Returns value increasing multiplier by cursor delta
		float GetValueMultiplier(float delta) const;

		// It is called when drag handle was moved and changes the property value
		void OnXDragHandleMoved(const Input::Cursor& cursor);

		// It is called when drag handle was moved and changes the property value
		void OnYDragHandleMoved(const Input::Cursor& cursor);

		// It is called when key was released
		void OnKeyReleased(const Input::Key& key);

		// It is called when change value move handle pressed, sets cursor infinite mode and stores value to data
		void OnXMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnXMoveHandleReleased(const Input::Cursor& cursor);

		// It is called when change value move handle pressed, sets cursor infinite mode and stores value to data
		void OnYMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnYMoveHandleReleased(const Input::Cursor& cursor);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetXValueByUser(float value);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetYValueByUser(float value);

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();
	};
}

CLASS_BASES_META(Editor::Vec2FProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::Vec2FProperty)
{
	PROTECTED_FIELD(mValuesProxies);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mXValuesDifferent);
	PROTECTED_FIELD(mYValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mXEditBox);
	PROTECTED_FIELD(mYEditBox);
	PROTECTED_FIELD(mXDragHangle);
	PROTECTED_FIELD(mYDragHangle);
}
END_META;
CLASS_METHODS_META(Editor::Vec2FProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SetValue, const Vec2F&);
	PUBLIC_FUNCTION(void, SetValueX, float);
	PUBLIC_FUNCTION(void, SetValueY, float);
	PUBLIC_FUNCTION(void, SetUnknownValue, const Vec2F&);
	PUBLIC_FUNCTION(void, SetXUnknownValue, float);
	PUBLIC_FUNCTION(void, SetYUnknownValue, float);
	PUBLIC_FUNCTION(Vec2F, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const Vec2F&);
	PROTECTED_FUNCTION(void, SetCommonValueX, float);
	PROTECTED_FUNCTION(void, SetCommonValueY, float);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnXEdited, const WString&);
	PROTECTED_FUNCTION(void, OnYEdited, const WString&);
	PROTECTED_FUNCTION(float, GetValueMultiplier, float);
	PROTECTED_FUNCTION(void, OnXDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnYDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnXMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnXMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnYMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnYMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, SetXValueByUser, float);
	PROTECTED_FUNCTION(void, SetYValueByUser, float);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
}
END_META;
