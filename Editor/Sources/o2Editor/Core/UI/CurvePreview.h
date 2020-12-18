#pragma once

#include "o2/Scene/UI/Widget.h"

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
	class CurvePreview: public Widget
	{
	public:
		// Default constructor. Initializes image
		CurvePreview();

		// Default copy-constructor
		CurvePreview(const CurvePreview& other);

		// Copy operator
		CurvePreview& operator=(const CurvePreview& other);

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

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(CurvePreview);

	protected:
		Curve*  mCurve = nullptr;

		bool    mNeedRedraw = false;
		Sprite* mSprite = nullptr;
		Color4  mBackColor = Color4(225, 232, 232);
		Color4  mCurveColor = Color4(44, 62, 80);

	protected:
		// Updates layers layouts, calls after updating widget layout
		void UpdateLayersLayouts() override;

		// Redraws image texture
		void Redraw();

		// It is called when curve keys changed, sets need redraw to true
		void OnCurveChanged();
	};
}

CLASS_BASES_META(Editor::CurvePreview)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::CurvePreview)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mCurve).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mNeedRedraw).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSprite).PROTECTED();
	FIELD().DEFAULT_VALUE(Color4(225, 232, 232)).NAME(mBackColor).PROTECTED();
	FIELD().DEFAULT_VALUE(Color4(44, 62, 80)).NAME(mCurveColor).PROTECTED();
}
END_META;
CLASS_METHODS_META(Editor::CurvePreview)
{

	PUBLIC_FUNCTION(void, SetCurve, Curve*);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetBackColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetBackColor);
	PUBLIC_FUNCTION(void, SetCurveColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetCurveColor);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, Redraw);
	PROTECTED_FUNCTION(void, OnCurveChanged);
}
END_META;
