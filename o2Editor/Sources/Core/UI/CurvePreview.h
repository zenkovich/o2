#pragma once

#include "UI/Widget.h"

using namespace o2;

namespace o2
{
	class Curve;
	class Sprite;
}

namespace Editor
{
	// -------------------
	// Curve preview image
	// -------------------
	class UICurvePreview: public UIWidget
	{
	public:
		// Default constructor. Initializes image
		UICurvePreview();

		// Default copy-constructor
		UICurvePreview(const UICurvePreview& other);

		// Copy operator
		UICurvePreview& operator=(const UICurvePreview& other);

		// Sets viewing curve
		void SetCurve(Curve* curve);

		// Draws widget
		void Draw() override;

		// Sets back color of image
		void SetBackColor(const Color4& color);

		// Returns back color of image
		Color4 GetBackColor() const;

		// Sets curve color on image
		void SetCurveColor(const Color4& color);

		// Returns curve color on image
		Color4 GetCurveColor() const;

		SERIALIZABLE(UICurvePreview);

	protected:
		Curve*  mCurve = nullptr;

		bool    mNeedRedraw = false;
		Sprite* mSprite = nullptr;
		Color4  mBackColor = Color4(120, 120, 120, 255);
		Color4  mCurveColor = Color4(0, 255, 0, 255);

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Updates layers layouts, calls after updating widget layout
		void UpdateLayersLayouts() override;

		// Redraws image texture
		void Redraw();

		// It is called when curve keys changed, sets need redraw to true
		void OnCurveChanged();
	};
}

CLASS_BASES_META(Editor::UICurvePreview)
{
	BASE_CLASS(o2::UIWidget);
}
END_META;
CLASS_FIELDS_META(Editor::UICurvePreview)
{
	PROTECTED_FIELD(mCurve);
	PROTECTED_FIELD(mNeedRedraw);
	PROTECTED_FIELD(mSprite);
	PROTECTED_FIELD(mBackColor);
	PROTECTED_FIELD(mCurveColor);
}
END_META;
CLASS_METHODS_META(Editor::UICurvePreview)
{

	PUBLIC_FUNCTION(void, SetCurve, Curve*);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetBackColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetBackColor);
	PUBLIC_FUNCTION(void, SetCurveColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetCurveColor);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, Redraw);
	PROTECTED_FUNCTION(void, OnCurveChanged);
}
END_META;
