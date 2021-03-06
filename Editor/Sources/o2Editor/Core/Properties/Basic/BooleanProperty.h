#pragma once

#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace o2
{
	class Toggle;
}

namespace Editor
{
	// -----------------------
	// Editor boolean property
	// -----------------------
	class BooleanProperty: public TPropertyField<bool>
	{
	public:
		// Default constructor
		BooleanProperty();

		// Copy constructor
		BooleanProperty(const BooleanProperty& other);

		// Copy operator
		BooleanProperty& operator=(const BooleanProperty& other);

		IOBJECT(BooleanProperty);

	protected:
		Toggle* mToggle = nullptr; // Toggle 

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();
	};
}

CLASS_BASES_META(Editor::BooleanProperty)
{
	BASE_CLASS(Editor::TPropertyField<bool>);
}
END_META;
CLASS_FIELDS_META(Editor::BooleanProperty)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mToggle).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::BooleanProperty)
{

	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, InitializeControls);
}
END_META;
