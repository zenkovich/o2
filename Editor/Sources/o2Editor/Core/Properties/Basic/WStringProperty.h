#pragma once

#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace o2
{
	class EditBox;
}

namespace Editor
{
	// -------------------------------
	// Editor string property edit box
	// -------------------------------
	class WStringProperty: public TPropertyField<WString>
	{
	public:
		// Default constructor
		WStringProperty();

		// Copy constructor
		WStringProperty(const WStringProperty& other);

		// Copy operator
		WStringProperty& operator=(const WStringProperty& other);

		IOBJECT(WStringProperty);

	protected:
		Ref<EditBox> mEditBox; // Edit box 

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Edit box change event
		void OnEdited(const WString& data);
	};
}
// --- META ---

CLASS_BASES_META(Editor::WStringProperty)
{
    BASE_CLASS(Editor::TPropertyField<WString>);
}
END_META;
CLASS_FIELDS_META(Editor::WStringProperty)
{
    FIELD().PROTECTED().NAME(mEditBox);
}
END_META;
CLASS_METHODS_META(Editor::WStringProperty)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const WStringProperty&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEdited, const WString&);
}
END_META;
// --- END META ---
