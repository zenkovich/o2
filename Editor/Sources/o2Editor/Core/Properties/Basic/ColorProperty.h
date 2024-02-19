#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace o2
{
	class Image;
	class Widget;
}

namespace Editor
{
	// ------------------------------
	// Editor color property edit box
	// ------------------------------
	class ColorProperty: public TPropertyField<Color4>
	{
	public:
		// Default constructor
		ColorProperty();

		// Copy constructor
		ColorProperty(const ColorProperty& other);

		// Copy operator
		ColorProperty& operator=(const ColorProperty& other);

		IOBJECT(ColorProperty);

	protected:
		Ref<Widget>      mEditBox;     // Edit box 
		Ref<Image>       mColorSprite; // Color preview sprite
		CursorEventsArea mClickArea;   // Box click area

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Called when box pressed and shows color picking dialog
		void OnClicked();
	};
}
// --- META ---

CLASS_BASES_META(Editor::ColorProperty)
{
    BASE_CLASS(Editor::TPropertyField<Color4>);
}
END_META;
CLASS_FIELDS_META(Editor::ColorProperty)
{
    FIELD().PROTECTED().NAME(mEditBox);
    FIELD().PROTECTED().NAME(mColorSprite);
    FIELD().PROTECTED().NAME(mClickArea);
}
END_META;
CLASS_METHODS_META(Editor::ColorProperty)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const ColorProperty&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
    FUNCTION().PROTECTED().SIGNATURE(void, OnClicked);
}
END_META;
// --- END META ---
