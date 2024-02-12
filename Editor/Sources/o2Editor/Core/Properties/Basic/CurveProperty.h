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
		Ref<Widget>       mEditBox;      // Edit box 
		Ref<CurvePreview> mPreviewImage; // Curve preview image
		CursorEventsArea  mClickArea;    // Box click area

	protected:
		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Called when common curve changed in curve edit dialog, tells to drawing content
		void OnValueChanged();

		// Called when box pressed and shows color picking dialog
		void OnClicked();
	};
}
// --- META ---

CLASS_BASES_META(Editor::CurveProperty)
{
    BASE_CLASS(Editor::TPropertyField<Curve>);
}
END_META;
CLASS_FIELDS_META(Editor::CurveProperty)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mEditBox);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPreviewImage);
    FIELD().PROTECTED().NAME(mClickArea);
}
END_META;
CLASS_METHODS_META(Editor::CurveProperty)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const CurveProperty&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
    FUNCTION().PROTECTED().SIGNATURE(void, OnValueChanged);
    FUNCTION().PROTECTED().SIGNATURE(void, OnClicked);
}
END_META;
// --- END META ---
