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
		const Type* GetValueType() const override;

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
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEnumType);
	FIELD().PROTECTED().NAME(mEntries);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mDropDown);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mUpdatingValue);
}
END_META;
CLASS_METHODS_META(Editor::EnumProperty)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const EnumProperty&);
	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetValueType);
	FUNCTION().PUBLIC().SIGNATURE(void, SpecializeType, const Type*);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetValueTypeStatic);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSelectedItem, const WString&);
}
END_META;
