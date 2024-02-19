#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace o2
{
	class Button;
	class EditBox;
	class Widget;
}

namespace Editor
{
	// -------------------------------
	// Editor string property edit box
	// -------------------------------
	class StringProperty: public TPropertyField<String>
	{
	public:
		// Default constructor
		StringProperty();

		// Copy constructor
		StringProperty(const StringProperty& other);

		// Copy operator
		StringProperty& operator=(const StringProperty& other);

		IOBJECT(StringProperty);

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

CLASS_BASES_META(Editor::StringProperty)
{
    BASE_CLASS(Editor::TPropertyField<String>);
}
END_META;
CLASS_FIELDS_META(Editor::StringProperty)
{
    FIELD().PROTECTED().NAME(mEditBox);
}
END_META;
CLASS_METHODS_META(Editor::StringProperty)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const StringProperty&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEdited, const WString&);
}
END_META;
// --- END META ---
