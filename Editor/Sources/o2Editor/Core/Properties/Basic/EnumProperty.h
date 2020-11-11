#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"

namespace o2
{
	class DropDown;
}

namespace Editor
{
	// -----------------------------
	// Editor enum property dropdown
	// -----------------------------
	class EnumProperty: public TPropertyField<int>
	{
	public:
		// Default constructor
		EnumProperty();

		// Copy constructor
		EnumProperty(const EnumProperty& other);

		// Copy operator
		EnumProperty& operator=(const EnumProperty& other);

		// Returns editing by this field type
		const Type* GetValueType() const;

		// Specializes field type
		void SpecializeType(const Type* type);

		// Returns editing by this field type by static function, can't be changed during runtime
		static const Type* GetValueTypeStatic();

		IOBJECT(EnumProperty);

	protected:				       						      
		const EnumType*         mEnumType = nullptr; // Type of enumeration															      
		const Map<int, String>* mEntries;            // Enum entries

		DropDown* mDropDown = nullptr;       // Layer name dropdown
		bool      mUpdatingValue = false;    // Is dropdown value updating and we don't we don't check selection

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Selects item
		void OnSelectedItem(const WString& name);
	};
}

CLASS_BASES_META(Editor::EnumProperty)
{
	BASE_CLASS(Editor::TPropertyField<int>);
}
END_META;
CLASS_FIELDS_META(Editor::EnumProperty)
{
	PROTECTED_FIELD(mEnumType).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mEntries);
	PROTECTED_FIELD(mDropDown).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mUpdatingValue).DEFAULT_VALUE(false);
}
END_META;
CLASS_METHODS_META(Editor::EnumProperty)
{

	PUBLIC_FUNCTION(const Type*, GetValueType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PUBLIC_STATIC_FUNCTION(const Type*, GetValueTypeStatic);
	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnSelectedItem, const WString&);
}
END_META;
