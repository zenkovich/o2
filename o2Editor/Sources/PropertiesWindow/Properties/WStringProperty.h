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
	class WStringProperty: public IPropertyField
	{
	public:
		// Default constructor
		WStringProperty(UIWidget* widget = nullptr);

		// Destructor
		~WStringProperty();

		// Sets fields
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns value
		WString GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const WString& value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(WStringProperty);

	protected:
		Function<void(void*, const WString&)> mAssignFunc; // Value assign function
		Function<WString(void*)>              mGetFunc;    // Get value function

		TargetsVec mValuesPointers;           // Fields' pointers
		WString    mCommonValue;              // Common field value (if not different)
		bool       mValuesDifferent = true;   // Are values different

		UIWidget*  mPropertyWidget = nullptr; // Property root widget, contains editbox and revert button
		UIEditBox* mEditBox = nullptr;        // Edit box 
		UIButton*  mRevertBtn = nullptr;      // Revert to prototype button

	protected:
		// Sets common value
		void SetCommonValue(const WString& value);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// Edit box change event
		void OnEdited(const WString& data);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetValueByUser(const WString& value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}
