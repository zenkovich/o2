#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/CursorEventsArea.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIWidget;
	class UIEditBox;
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
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Sets value
		void SetValue(int value);

		// Sets value as unknown
		void SetUnknownValue(int defaultValue = 0);

		// Returns value
		int GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(IntegerProperty);

	protected:
		Function<void(void*, const int&)> mAssignFunc; // Value assign function
		Function<int(void*)>              mGetFunc;    // Get value function

		Vector<void*>    mValuesPointers;         // Fields' pointers
		int              mCommonValue = 0;        // Common field value (if not different)
		bool             mValuesDifferent = true; // Are values different
					     
		UIEditBox*       mEditBox;                // Edit box 
		CursorEventsArea mDragHangle;             // Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(int value);

		// Edit box change event
		void OnEdited(const WString& data);

		// Calls when drag handle was moved and changes the property value
		void OnDragHandleMoved(const Input::Cursor& cursor);
	};
}