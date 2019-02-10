#pragma once

#include "Core/Properties/IPropertyField.h"

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
		const Type* GetFieldType() const;

		// Specializes field type
		void SpecializeType(const Type* type) override;

		IOBJECT(EnumProperty);

	protected:				       						      
		const EnumType* mEnumType;               // Type of enumeration															      
		const Dictionary<int, String>* mEntries; // Enum entries

		DropDown* mDropDown = nullptr;       // Layer name dropdown
		bool        mUpdatingValue = false;    // Is dropdown value updating and we don't we don't check selection

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

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
	PROTECTED_FIELD(mEnumType);
	PROTECTED_FIELD(mEntries);
	PROTECTED_FIELD(mDropDown);
	PROTECTED_FIELD(mUpdatingValue);
}
END_META;
CLASS_METHODS_META(Editor::EnumProperty)
{

	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnSelectedItem, const WString&);
}
END_META;
