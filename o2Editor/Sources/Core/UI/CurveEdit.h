#pragma once

#include "Assets/ImageAsset.h"
#include "Core/UI/ScrollView.h"
#include "Events/CursorEventsListener.h"
#include "Render/Camera.h"
#include "Render/FontRef.h"
#include "Render/Mesh.h"
#include "Utils/DragHandle.h"
#include "Utils/Math/Curve.h"

using namespace o2;

namespace o2
{
	class UIHorizontalScrollBar;
	class UIVerticalScrollBar;
}

namespace Editor
{
	// ---------------------
	// Curves editing widget
	// ---------------------
	class UICurveEditor: public UIScrollView
	{
	public:
		// Default constructor
		UICurveEditor();

		// Copy-constructor
		UICurveEditor(const UICurveEditor& other);

		// Destructor
		~UICurveEditor();

		// Copy-operator
		UICurveEditor& operator=(const UICurveEditor& other);

		// Draws widget, updates render target 
		void Draw();

		// Updates drawables, states and widget
		void Update(float dt);

		// Adds editing curve with color. If color is default it will be randomized
		void AddEditingCurve(Curve* curve, const Color4& color = Color4::Green());

		// Removed curve from editing
		void RemoveEditingCurve(Curve* curve);

		// Removes all editing curves
		void RemoveAllEditingCurves();

		// Adds curves range with color. It can't be edited, just a solid color between curves
		void AddCurvesRange(Curve* curveA, Curve* curveB, const Color4& color = Color4::Green());

		// Removes curve range
		void RemoveCurvesRange(Curve* curveA, Curve* curveB);

		// Sets selection rectange sprite image
		void SetSelectionSpriteImage(const ImageAssetRef& image);

		// Sets captions text font
		void SetTextFont(const FontRef& font);

		// Sets horizontal scrollbar
		void SetHorScrollbar(UIHorizontalScrollBar* scrollbar);

		// Sets vertical scrollbar
		void SetVerScrollbar(UIVerticalScrollBar* scrollbar);

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		SERIALIZABLE(UICurveEditor);

	protected:
		typedef Vector<DragHandle*> DragHandlesVec;
		typedef Vector<Vec2F> PointsVec;

		struct CurveInfo
		{
			Curve*         curve;
			DragHandlesVec handles;
			PointsVec      approximatedPoints;
			Color4         color;
			Vec2F          viewScale;

			CurveInfo();
			~CurveInfo();

			void UpdateApproximatedPoints();
		};
		typedef Vector<CurveInfo*> CurveInfosVec;

		struct RangeInfo
		{
			CurveInfo* curveA;
			CurveInfo* curveB;
			Color4     color;
			Mesh*      mesh;

			RangeInfo();
			~RangeInfo();

			void UpdateMesh();
		};
		typedef Vector<RangeInfo*> RangeInfosVec;

	protected:
		UIHorizontalScrollBar* mHorScrollbar = nullptr;       // Horizontal view scrollbar @SERIALIZABLE
		UIVerticalScrollBar*   mVerScrollbar = nullptr;       // Vertical view scrollbar @SERIALIZABLE

		CurveInfosVec          mCurves;                       // Editing curves infos list 
		RangeInfosVec          mRanges;                       // Curves ranges list
		DragHandlesVec         mAllHandles;                   // All handles
		DragHandlesVec         mSelectedHandles;              // Current selected handles

		Sprite*                mSelectionSprite = nullptr;    // Selection sprite @SERIALIZABLE
		FontRef                mTextFont;                     // Captions text font @SERIALIZABLE
		Text*                  mText = nullptr;               // Captions text drawable

	protected:
		// Redraws content into render target
		void RedrawContent();
	};
}
