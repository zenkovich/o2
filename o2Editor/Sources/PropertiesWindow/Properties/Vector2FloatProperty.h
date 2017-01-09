#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/CursorEventsArea.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIEditBox;
	class UIHorizontalLayout;
	class UIWidget;
}

namespace Editor
{
	// ----------------------------
	// Editor float vector property
	// ----------------------------
	class Vec2FProperty: public IPropertyField
	{
	public:
		// Default constructor
		Vec2FProperty(UIWidget* widget = nullptr);

		// Destructor
		~Vec2FProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

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
		const Type* GetFieldType() const;

		IOBJECT(Vec2FProperty);

	protected:
		Function<void(void*, const Vec2F&)> mAssignFunc;  // Value assign function
		Function<Vec2F(void*)>              mGetFunc;     // Get value function
														  
		Function<void(void*, float)>        mXAssignFunc; // X Value assign function
		Function<float(void*)>              mXGetFunc;    // Get X value function
														  
		Function<void(void*, float)>        mYAssignFunc; // Y Value assign function
		Function<float(void*)>              mYGetFunc;    // Get Y value function

		Vector<void*> mValuesPointers;          // Fields' pointers
		Vec2F         mCommonValue;             // Common field value (if not different)
		bool          mXValuesDifferent = true; // Are left values different
		bool          mYValuesDifferent = true; // Are bottom values different\

		UIWidget*     mWidget = nullptr;        // Horizontal edit boxes layout
		UIEditBox*    mXEditBox = nullptr;      // X Edit box 
		UIEditBox*    mYEditBox = nullptr;      // Y Edit box 

		CursorEventsArea mXDragHangle;          // X Value changing drag handle
		CursorEventsArea mYDragHangle;          // Y Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(const Vec2F& value);

		// Sets common value X
		void SetCommonValueX(float value);

		// Sets common value Y
		void SetCommonValueY(float value);

		// X Edit box change event
		void OnXEdited(const WString& data);

		// Y Edit box change event
		void OnYEdited(const WString& data);

		// Returns value increasing multiplier by cursor delta
		float GetValueMultiplier(float delta) const;

		// Calls when drag handle was moved and changes the property value
		void OnXDragHandleMoved(const Input::Cursor& cursor);

		// Calls when drag handle was moved and changes the property value
		void OnYDragHandleMoved(const Input::Cursor& cursor);
	};
}
