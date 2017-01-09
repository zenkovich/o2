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
	class WStringProperty: public IPropertyField
	{
	public:
		// Default constructor
		WStringProperty(UIWidget* widget = nullptr);

		// Destructor
		~WStringProperty();

		// Sets fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		WString GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const WString& value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(WStringProperty);

	protected:
		Function<void(void*, const WString&)> mAssignFunc; // Value assign function
		Function<WString(void*)>              mGetFunc;    // Get value function

		Vector<void*> mValuesPointers;  // Fields' pointers
		WString       mCommonValue;     // Common field value (if not different)
		bool          mValuesDifferent; // Are values different

		UIEditBox*    mEditBox;         // Edit box 

	protected:
		// Sets common value
		void SetCommonValue(const WString& value);

		// Edit box change event
		void OnEdited(const WString& data);
	};
}