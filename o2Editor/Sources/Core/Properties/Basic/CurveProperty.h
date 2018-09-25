#pragma once

#include "Core/Properties/IPropertyField.h"
#include "Events/CursorEventsArea.h"
#include "Utils/Math/Curve.h"

using namespace o2;

namespace o2
{
	class UIWidget;
}

namespace Editor
{
	class UICurvePreview;

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
		UIWidget*        mEditBox = nullptr;         // Edit box 
		UICurvePreview*  mPreviewImage = nullptr;    // Curve preview image
		CursorEventsArea mClickArea;                 // Box click area

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

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
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mPreviewImage);
	PROTECTED_FIELD(mClickArea);
}
END_META;
CLASS_METHODS_META(Editor::CurveProperty)
{

	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnValueChanged);
	PROTECTED_FUNCTION(void, OnClicked);
}
END_META;
