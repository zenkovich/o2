#pragma once

#include "Assets/ImageAsset.h"
#include "Core/UI/FrameScrollView.h"
#include "Render/Camera.h"
#include "Render/FontRef.h"
#include "Render/Mesh.h"
#include "Utils/DragHandle.h"
#include "Utils/FrameHandles.h"
#include "Utils/Math/Curve.h"

using namespace o2;

namespace o2
{
	class UIContextMenu;
}

namespace Editor
{
	// ---------------------
	// Curves editing widget
	// ---------------------
	class UICurveEditor: public UIFrameScrollView, public SelectableDragHandlesGroup
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

		// Sets main key handle images
		void SetMainHandleImages(const ImageAssetRef& regular, const ImageAssetRef& hover, const ImageAssetRef& pressed,
								 const ImageAssetRef& selected);

		// Sets support key handle images
		void SetSupportHandleImages(const ImageAssetRef& regular, const ImageAssetRef& hover, const ImageAssetRef& pressed,
									const ImageAssetRef& selected);

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		SERIALIZABLE(UICurveEditor);

	protected:
		typedef Vector<Vec2F> PointsVec;
		typedef Vector<SelectableDragHandle*> SelectableHandlesVec;

		struct KeyHandles
		{
			UICurveEditor*       curveEditor = nullptr;
			SelectableDragHandle mainHandle;
			SelectableDragHandle leftSupportHandle;
			SelectableDragHandle rightSupportHandle;
				           
			int                  curveKeyIdx;

			KeyHandles() {}
			KeyHandles(const SelectableDragHandle& mainSample, const SelectableDragHandle& supportSample, UICurveEditor* editor);

			void Draw();
		};
		typedef Vector<KeyHandles*> KeyHandlesVec;

		struct CurveInfo
		{
			Curve*         curve;
			KeyHandlesVec  handles;
			PointsVec      approximatedPoints;
			Color4         color;
			Vec2F          viewScale;

			CurveInfo();
			~CurveInfo();

			void UpdateHandles();
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
		UIContextMenu*         mContextMenu;                  // Context menu for editing keys properties, copying, pasting and other

		SelectableDragHandle   mMainHandleSample;             // Main handle sample, uses to copy sprites @SERIALIZABLE
		SelectableDragHandle   mSupportHandleSample;          // Support handle sample, uses to copy sprites @SERIALIZABLE
							   
		CurveInfosVec          mCurves;                       // Editing curves infos list 
		RangeInfosVec          mRanges;                       // Curves ranges list

		SelectableHandlesVec   mSupportHandles;               // Support points handles list
		SelectableHandlesVec   mSelectingHandlesBuf;          // Potentially selecting handles while selecting

		Sprite*                mSelectionSprite = nullptr;    // Selection sprite @SERIALIZABLE
		FontRef                mTextFont;                     // Captions text font @SERIALIZABLE
		Text*                  mTextLeft = nullptr;           // Captions text drawable at left border
		Text*                  mTextRight = nullptr;          // Captions text drawable at right border
		Text*                  mTextTop = nullptr;            // Captions text drawable at top border
		Text*                  mTextBottom = nullptr;         // Captions text drawable at bottom border

		Vec2F                  mSelectingPressedPoint;        // Point, where cursor was pressed, selection starts here, in local space

		FrameHandles           mTransformFrame;               // Keys transformation frame
		Basis                  mTransformFrameBasis;          // Basis of transform frame in local space

		bool                   mIsViewScrolling = false;      // Is scrolling view at this time

	protected:
		// Initializes context menu items
		void InitializeContextMenu();

		// Initializes text drawables by font and sets aligning
		void InitializeTextDrawables();

		// Recalculates view area by curves approximated points
		void RecalculateViewArea();

		// Redraws content into render target
		void RedrawContent();

		// Draws grid and captions
		void DrawGrid();

		// Draws curves
		void DrawCurves();

		// Draws handles
		void DrawHandles();

		// Draw selection sprite
		void DrawSelection();

		// Draws transformation frame
		void DrawTransformFrame();

		// Adds curve key main and support handles and initializes them
		void AddCurveKeyHandles(CurveInfo* info, int keyId);

		// Calls when one of main curve key handles was moved. Updates graphics and handles
		void OnCurveKeyMainHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Calls when one of left support curve key handles was moved. Updates graphics and handles
		void OnCurveKeyLeftSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Calls when one of right support curve key handles was moved. Updates graphics and handles
		void OnCurveKeyRightSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Checks left support handle constrains and returns filtered position
		Vec2F CheckLeftSupportHandlePosition(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Checks right support handle constrains and returns filtered position
		Vec2F CheckRightSupportHandlePosition(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Calls when cursor double clicked, adds new point in curve
		void OnCursorDblClicked(const Input::Cursor& cursor);

		// Smoothes key support points and updates handles
		void SmoothKey(CurveInfo* info, int idx);

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when right mouse button stay down on this, overriding from scroll view to call context menu
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor);

		// Calls when right mouse button was released (only when right mouse button pressed this at previous time), overriding from scroll view to call context menu
		void OnCursorRightMouseReleased(const Input::Cursor& cursor);

		// Checks supports handles visibility
		void CheckHandlesVisible();

		// Updates transformation frame by selected handles
		void UpdateTransformFrame();

		// Calls when selectable draggable handle was released
		void OnHandleCursorReleased(SelectableDragHandle* handle, const Input::Cursor& cursor);

		// Calls when selectable handle was began to drag
		void OnHandleBeganDragging(SelectableDragHandle* handle);

		// Calls when selectable handle moved, moves all selected handles position
		void OnHandleMoved(SelectableDragHandle* handle, const Input::Cursor& cursor);

		// Sets all selected keys supports type
		void SetSelectedKeysSupportsType(Curve::Key::Type type);

		// Calls when transform frame was transformed
		void OnTransformFrameTransformed(const Basis& basis);

	// Context menu items functions
		void OnAutoSmoothChecked(bool checked);

		void OnFlatChecked(bool checked);

		void OnFreeChecked(bool checked);

		void OnBrokenChecked(bool checked);

		void OnDiscreteChecked(bool checked);

		void OnCopyPressed();

		void OnCutPressed();

		void OnPastePressed();

		void OnDeletePressed();

		void OnInsertPressed();
	};
}
