#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Utils/Math/Curve.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace o2
{
	class Widget;
}

namespace Editor
{
	class CurvePreview;

	// ------------------------------
	// Editor curve property edit box
	// ------------------------------
	class CurveProperty: public TPropertyField<Curve>
	{
	public:
		// Default constructor
		CurveProperty();

		// Copy constructor
		CurveProperty(const CurveProperty& other);

		// Copy operator
		CurveProperty& operator=(const CurveProperty& other);

		IOBJECT(CurveProperty);

	protected:
		Widget*          mEditBox = nullptr;         // Edit box 
		CurvePreview*    mPreviewImage = nullptr;    // Curve preview image
		CursorEventsArea mClickArea;                 // Box click area

	protected:
		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// It is called when common curve changed in curve edit dialog, tells to drawing content
		void OnValueChanged();

		// It is called when box pressed and shows color picking dialog
		void OnClicked();
	};
}

CLASS_BASES_META(Editor::CurveProperty)
{
	BASE_CLASS(Editor::TPropertyField<Curve>);
}
END_META;
CLASS_FIELDS_META(Editor::CurveProperty)
{
	PROTECTED_FIELD(mEditBox).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mPreviewImage).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mClickArea);
}
END_META;
CLASS_METHODS_META(Editor::CurveProperty)
{

	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnValueChanged);
	PROTECTED_FUNCTION(void, OnClicked);
}
END_META;
