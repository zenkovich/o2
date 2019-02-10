#pragma once

#include "Core/Properties/IPropertyField.h"

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
		EditBox* mEditBox = nullptr; // Edit box 

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Edit box change event
		void OnEdited(const WString& data);
	};
}

CLASS_BASES_META(Editor::StringProperty)
{
	BASE_CLASS(Editor::TPropertyField<String>);
}
END_META;
CLASS_FIELDS_META(Editor::StringProperty)
{
	PROTECTED_FIELD(mEditBox);
}
END_META;
CLASS_METHODS_META(Editor::StringProperty)
{

	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnEdited, const WString&);
}
END_META;
