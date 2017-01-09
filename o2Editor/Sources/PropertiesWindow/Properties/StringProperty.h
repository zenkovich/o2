#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIWidget;
	class UIEditBox;
}

namespace Editor
{
	// -------------------------------
	// Editor string property edit box
	// -------------------------------
	class StringProperty: public IPropertyField
	{
	public:
		// Default constructor
		StringProperty(UIWidget* widget = nullptr);

		// Destructor
		~StringProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		String GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const String& value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(StringProperty);

	protected:
		Function<void(void*, const String&)> mAssignFunc; // Value assign function
		Function<String(void*)>              mGetFunc;    // Get value function

		Vector<void*> mValuesPointers;  // Fields' pointers
		String        mCommonValue;     // Common field value (if not different)
		bool          mValuesDifferent; // Are values different

		UIEditBox*    mEditBox;         // Edit box 

	protected:
		// Sets common value
		void SetCommonValue(const String& value);

		// Edit box change event
		void OnEdited(const WString& data);
	};
}