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
		void Draw();

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
		// Updates layers layouts, calls after updating widget layout
		void UpdateLayersLayouts();

		// Redraws image texture
		void Redraw();

		// It is called when curve keys changed, sets need redraw to true
		void OnCurveChanged();
	};
}
