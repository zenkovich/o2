#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Scene/Scene.h"

namespace o2
{
	class Text;
	class UIButton;
	class UIDropDown;
}

namespace Editor
{
	// -----------------------------
	// Editor enum property dropdown
	// -----------------------------
	class EnumProperty: public IPropertyField
	{
	public:
		// Default constructor
		EnumProperty(UIWidget* widget = nullptr);

		// Destructor
		~EnumProperty();

		// Sets fields
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns value
		int GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(int value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns editing by this field type
		const Type* GetFieldType() const;

		// Specializes field type
		void SpecializeType(const Type* type) override;

		IOBJECT(EnumProperty);

	protected:
		Function<void(void*, int)> mAssignFunc; // Value assign function
		Function<int(void*)>       mGetFunc;    // Get value function
								       						      
		const EnumType* mEnumType;               // Type of enumeration
		TargetsVec      mValuesPointers;         // Fields' pointers
		int             mCommonValue = 0;        // Common field value (if not different)
		bool            mValuesDifferent = true; // Are values different
															      
		const Dictionary<int, String>* mEntries; // Enum entries

		UIWidget*   mPropertyWidget = nullptr; // Property widget
		UIButton*   mRevertBtn = nullptr;      // Revert to source prototype button
		UIDropDown* mDropDown = nullptr;       // Layer name dropdown
		bool        mUpdatingValue = false;    // Is dropdown value updating and we don't we don't check selection

	protected:
		// Sets common value
		void SetCommonValue(int value);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// Selects item
		void OnSelectedItem(const WString& name);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetValueByUser(int value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}
