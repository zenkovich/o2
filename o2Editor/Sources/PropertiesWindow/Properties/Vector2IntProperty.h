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
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

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
		const Type* GetFieldType() const;

		IOBJECT(Vec2IProperty);

	protected:
		Function<void(void*, const Vec2I&)> mAssignFunc;  // Value assign function
		Function<Vec2I(void*)>              mGetFunc;     // Get value function

		Function<void(void*, int)>          mXAssignFunc; // X Value assign function
		Function<int(void*)>                mXGetFunc;    // Get X value function
										    
		Function<void(void*, int)>          mYAssignFunc; // Y Value assign function
		Function<int(void*)>                mYGetFunc;    // Get Y value function

		Vector<void*> mValuesPointers;          // Fields' pointers
		Vec2I         mCommonValue;             // Common field value (if not different)
		bool          mXValuesDifferent = true; // Are left values different
		bool          mYValuesDifferent = true; // Are bottom values different\

		UIWidget*     mWidget = nullptr;        // Horizontal edit boxes layout
		UIEditBox*    mXEditBox = nullptr;      // X Edit box 
		UIEditBox*    mYEditBox = nullptr;      // Y Edit box 

		CursorEventsArea mXDragHangle;          // X Value changing drag handle
		CursorEventsArea mYDragHangle;          // Y Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(const Vec2I& value);

		// Sets common value X
		void SetCommonValueX(int value);

		// Sets common value Y
		void SetCommonValueY(int value);

		// X Edit box change event
		void OnXEdited(const WString& data);

		// Y Edit box change event
		void OnYEdited(const WString& data);

		// Calls when drag handle was moved and changes the property value
		void OnXDragHandleMoved(const Input::Cursor& cursor);

		// Calls when drag handle was moved and changes the property value
		void OnYDragHandleMoved(const Input::Cursor& cursor);
	};
}
