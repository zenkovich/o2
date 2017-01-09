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
	// ------------------------------
	// Editor float property edit box
	// ------------------------------
	class FloatProperty: public IPropertyField
	{
	public:
		// Default constructor
		FloatProperty(UIWidget* widget = nullptr);

		// Destructor
		~FloatProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		float GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(float value);

		// Sets value as unknown
		void SetUnknownValue(float defaultValue = 0.0f);

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(FloatProperty);

	protected:
		Function<void(void*, const float&)> mAssignFunc; // Value assign function
		Function<float(void*)>              mGetFunc;    // Get value function

		Vector<void*>    mValuesPointers;         // Fields' pointers
		float            mCommonValue = 0.0f;     // Common field value (if not different)
		bool             mValuesDifferent = true; // Are values different
					     
		UIEditBox*       mEditBox;                // Edit box 
		CursorEventsArea mDragHangle;             // Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(float value);

		// Edit box change event
		void OnEdited(const WString& data);

		// Calls when drag handle was moved and changes the property value
		void OnDragHandleMoved(const Input::Cursor& cursor);
	};
}