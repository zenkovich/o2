#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIEditBox;
	class UIWidget;
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
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns value
		String GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const String& value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(StringProperty);

	protected:
		Function<void(void*, const String&)> mAssignFunc; // Value assign function
		Function<String(void*)>              mGetFunc;    // Get value function

		TargetsVec mValuesPointers;         // Fields' pointers
		String     mCommonValue;            // Common field value (if not different)
		bool       mValuesDifferent = true; // Are values different

		UIWidget*  mPropertyWidget = nullptr; // Property root widget, contains editbox and revert button
		UIEditBox* mEditBox = nullptr;        // Edit box 
		UIButton*  mRevertBtn = nullptr;      // Revert to prototype button

	protected:
		// Sets common value
		void SetCommonValue(const String& value);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// Edit box change event
		void OnEdited(const WString& data);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetValueByUser(const String& value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}
