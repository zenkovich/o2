#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Scene/Scene.h"

namespace o2
{
	class Text;
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
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

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
		void SpecializeType(const Type* type);

		IOBJECT(EnumProperty);

	protected:
		Function<void(void*, int)>     mAssignFunc;            // Value assign function
		Function<int(void*)>           mGetFunc;               // Get value function
								       
		const EnumType*                mEnumType;              // Type of enumeration
		Vector<void*>                  mValuesPointers;        // Fields' pointers
		int                            mCommonValue;           // Common field value (if not different)
		bool                           mValuesDifferent;       // Are values different

		const Dictionary<int, String>* mEntries;               // Enum entries

		UIDropDown*                    mDropDown;              // Layer name dropdown
		bool                           mUpdatingValue = false; // Is dropdown value updating and we don't we don't check selection

	protected:
		// Sets common value
		void SetCommonValue(int value);

		// Selects item
		void OnSelectedItem(const WString& name);
	};
}

