#pragma once

#include "Assets/ImageAsset.h"
#include "Core/Actions/ActionsList.h"
#include "Core/Actions/IAction.h"
#include "Core/UI/FrameScrollView.h"
#include "Render/Camera.h"
#include "Render/FontRef.h"
#include "Render/Mesh.h"
#include "Utils/Editor/DragHandle.h"
#include "Utils/Editor/FrameHandles.h"
#include "Utils/Math/Curve.h"

using namespace o2;

namespace o2
{
	class ContextMenu;
	class EditBox;
	class Window;
}

namespace Editor
{
	// ---------------------
	// Curves editing widget
	// ---------------------
	class CurveEditor: public FrameScrollView, public SelectableDragHandlesGroup
	{
	public:
		ActionsList* actionsListDelegate = nullptr; // Actions fall down list. When it is null, editor uses local actions list

	public:
		// Default constructor
		CurveEditor();

		// Copy-constructor
		CurveEditor(const CurveEditor& other);

		// Destructor
		~CurveEditor();

		// Copy-operator
		CurveEditor& operator=(const CurveEditor& other);

		// Draws widget, updates render target 
		void Draw() override;

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Returns list of curves
		Dictionary<String, Curve*> GetCurves() const;

		// Adds editing curve with color. If color is default it will be randomized
		void AddCurve(const String& id, Curve* curve, const Color4& color = Color4::Green());

		// Removed curve from editing
		void RemoveCurve(Curve* curve);

		// Removed curve from editing
		void RemoveCurve(const String& id);

		// Removes all editing curves
		void RemoveAllCurves();

		// Adds curves range with color. It can't be edited, just a solid color between curves
		void AddCurvesRange(Curve* curveA, Curve* curveB, const Color4& color = Color4::Green());

		// Removes curve range
		void RemoveCurvesRange(Curve* curveA, Curve* curveB);

		// Adds curves range with color. It can't be edited, just a solid color between curves
		void AddCurvesRange(const String& idA, const String& idB, const Color4& color = Color4::Green());

		// Removes curve range
		void RemoveCurvesRange(const String& idA, const String& idB);

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
		void UpdateSelfTransform() override;

		SERIALIZABLE(CurveEditor);

	protected:
		typedef Vector<Vec2F> PointsVec;
		typedef Vector<DragHandle*> SelectableHandlesVec;
		typedef Vector<IAction*> ActionsVec;

		struct KeyHandles
		{
			CurveEditor* curveEditor = nullptr;
			DragHandle   mainHandle;
			DragHandle   leftSupportHandle;
			DragHandle   rightSupportHandle;
				           
			int          curveKeyIdx;
			UInt64       curveKeyUid;

		public:
			KeyHandles() {}
			KeyHandles(const DragHandle& mainSample, const DragHandle& supportSample, CurveEditor* editor);

			void Draw(const RectF& camRect);
			bool IsSomeHandleSelected() const;
		};
		typedef Vector<KeyHandles*> KeyHandlesVec;

		struct CurveInfo
		{
			CurveEditor*   curveEditor = nullptr;

			String curveId;
			Curve* curve = nullptr;

			KeyHandlesVec handles;

			PointsVec approximatedPoints;

			Color4 color;
			Vec2F viewScale;

			bool disableChangesHandling = false;

		public:
			CurveInfo();
			~CurveInfo();

			void UpdateHandles();
			void UpdateApproximatedPoints();
			void OnCurveChanged();

			void BeginCurveManualChange();
			void CompleteCurveManualChange();
		};
		typedef Vector<CurveInfo*> CurveInfosVec;

		class CurveCopyInfo: public ISerializable
		{
		public:
			String             curveId; // @SERIALIZABLE
			Vector<Curve::Key> keys;    // @SERIALIZABLE

			SERIALIZABLE(CurveCopyInfo);
		};
		typedef Vector<CurveCopyInfo*> CurveCopyInfosVec;

		struct RangeInfo
		{
			CurveInfo* curveA = nullptr;
			CurveInfo* curveB = nullptr;
			Color4     color;
			Mesh*      mesh;

			RangeInfo();
			~RangeInfo();

			void UpdateMesh();
		};
		typedef Vector<RangeInfo*> RangeInfosVec;

		struct SelectedHandlesInfo
		{
			int  index;
			bool mainHandle;
			bool leftSupportHandle;
			bool rightSupportHandle;

			bool operator==(const SelectedHandlesInfo& other) const;
		};
		typedef Vector<SelectedHandlesInfo> SelectedHandlesInfosVec;

		struct CurveKeysInfo
		{
			String                  curveId;
			Curve::KeysVec          keys;
			SelectedHandlesInfosVec selectedHandles;

			bool operator==(const CurveKeysInfo& other) const;
		};
		typedef Vector<CurveKeysInfo> CurveKeysInfosVec;

	protected:
		ContextMenu* mContextMenu = nullptr; // Context menu for editing keys properties, copying, pasting and other
							    								    
		DragHandle mMainHandleSample;    // Main handle sample, uses to copy sprites @SERIALIZABLE
		DragHandle mSupportHandleSample; // Support handle sample, uses to copy sprites @SERIALIZABLE
							    								    
		CurveInfosVec mCurves; // Editing curves infos list 
		RangeInfosVec mRanges; // Curves ranges list
							    								    
		SelectableHandlesVec       mSupportHandles;      // Support points handles list
		SelectableDragHandlesGroup mSupportHandlesGroup; // Support points handles selection group. They are must be selectable separately from main handles
		
		SelectableHandlesVec mSelectingHandlesBuf; // Potentially selecting handles while selecting
							    								    
		Sprite* mSelectionSprite = nullptr; // Selection sprite @SERIALIZABLE
		FontRef mTextFont;                  // Captions text font @SERIALIZABLE
		Text*   mTextLeft = nullptr;        // Captions text drawable at left border
		Text*   mTextRight = nullptr;       // Captions text drawable at right border
		Text*   mTextTop = nullptr;         // Captions text drawable at top border
		Text*   mTextBottom = nullptr;      // Captions text drawable at bottom border
							    								    
		Vec2F mSelectingPressedPoint; // Point, where cursor was pressed, selection starts here, in local space
							    								    
		FrameHandles mTransformFrame;                // Keys transformation frame
		bool         mTransformFrameVisible = false; // Is transform frame visible. it visible when 2 or more main handles was selected
		Basis        mTransformFrameBasis;           // Basis of transform frame in local space
								 							   	    
		bool mIsViewScrolling = false; // Is scrolling view at this time
							    
		CurveKeysInfosVec mBeforeTransformKeys; // Stored selected keys before handles transformed

		ActionsList mActionsList; // Local actions list. It uses when actionFallDown is null 

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when visible was changed. Sets context menu items priority
		void OnResEnableInHierarchyChanged() override;

		// It is called when scrolling
		void OnScrolled(float scroll) override;

		// Searches curve by id
		Curve* FindCurve(const String& id);

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

		// Removes curve key handles
		void RemoveCurveKeyHandles(CurveInfo* info, int keyId);

		// It is called when one of main curve key handles was moved. Updates graphics and handles
		void OnCurveKeyMainHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// It is called when one of left support curve key handles was moved. Updates graphics and handles
		void OnCurveKeyLeftSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// It is called when one of right support curve key handles was moved. Updates graphics and handles
		void OnCurveKeyRightSupportHandleDragged(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Checks left support handle constrains and returns filtered position
		Vec2F CheckLeftSupportHandlePosition(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Checks right support handle constrains and returns filtered position
		Vec2F CheckRightSupportHandlePosition(CurveInfo* info, KeyHandles* handles, const Vec2F& position);

		// Smooths key support points and updates handles
		void SmoothKey(CurveInfo* info, int idx);

		// It is called when cursor double clicked, adds new point in curve
		void OnCursorDblClicked(const Input::Cursor& cursor) override;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when right mouse button stay down on this, overriding from scroll view to call context menu
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// It is called when right mouse button was released (only when right mouse button pressed this at previous time), overriding from scroll view to call context menu
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

		// Checks supports handles visibility
		void CheckHandlesVisible();

		// Updates transformation frame by selected handles
		void UpdateTransformFrame();

		// Returns is transform frame visible. it will be visible when 2 or more main handles was selected
		bool IsTransformFrameVisible() const;

		// It is called when selectable draggable handle was released
		void OnHandleCursorReleased(DragHandle* handle, const Input::Cursor& cursor);

		// It is called when selectable handle was began to drag
		void OnHandleBeganDragging(DragHandle* handle);

		// It is called when selectable handle moved, moves all selected handles position
		void OnHandleMoved(DragHandle* handle, const Vec2F& cursorPos);

		// It is called when selectable handle completed changing
		void OnHandleCompletedChange(DragHandle* handle);

		// Sets all selected keys supports type
		void SetSelectedKeysSupportsType(Curve::Key::Type type);

		// It is called when transform frame was transformed
		void OnTransformFrameTransformed(const Basis& basis);

		// It is called when beginning transforming some handles. Stores selected keys before changing
		void OnTransformBegin();

		// It is called when transform completed. Checks that selected keys changed, creates action for undo/redo
		void OnTransformCompleted();

		// Stores undo action, clears redo actions
		void DoneAction(IAction* action);

	
		// On context menu edit pressed. Shows key edit window
		void OnEditPressed();

		// On context menu auto smooth pressed. Enables auto smoothing for selected keys
		void OnAutoSmoothChecked(bool checked);

		// On context menu flat pressed. Enables flat support handles for selected keys
		void OnFlatChecked(bool checked);

		// On context menu free pressed. Free handles transform for selected keys
		void OnFreeChecked(bool checked);

		// On context menu broken pressed. Breaks connection between left and right supports
		void OnBrokenChecked(bool checked);

		// On context menu discrete pressed. Sets discrete keys transition for selected
		void OnDiscreteChecked(bool checked);

		// On context menu copy pressed. Stores keys into buffer
		void OnCopyPressed();

		// On context menu cut pressed. Stores keys into buffer and removes them
		void OnCutPressed();

		// On context menu paste pressed. Inserts keys from buffer at cursor position
		void OnPastePressed();

		// On context menu delete pressed. Deletes selected keys
		void OnDeletePressed();

		// On context menu insert pressed. Inserts keys from buffer and moves keys to right
		void OnInsertPressed();

		// On context menu undo pressed. Reverts previous action and puts into actions stack
		void OnUndoPressed();

		// On context menu redo pressed. Restores action from stack
		void OnRedoPressed();

		friend class AddKeysAction;
		friend class DeleteKeysAction;
		friend class KeysChangeAction;
	};
}

CLASS_BASES_META(Editor::CurveEditor)
{
	BASE_CLASS(Editor::FrameScrollView);
	BASE_CLASS(o2::SelectableDragHandlesGroup);
}
END_META;
CLASS_FIELDS_META(Editor::CurveEditor)
{
	PUBLIC_FIELD(actionsListDelegate);
	PROTECTED_FIELD(mContextMenu);
	PROTECTED_FIELD(mMainHandleSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSupportHandleSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCurves);
	PROTECTED_FIELD(mRanges);
	PROTECTED_FIELD(mSupportHandles);
	PROTECTED_FIELD(mSupportHandlesGroup);
	PROTECTED_FIELD(mSelectingHandlesBuf);
	PROTECTED_FIELD(mSelectionSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTextFont).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTextLeft);
	PROTECTED_FIELD(mTextRight);
	PROTECTED_FIELD(mTextTop);
	PROTECTED_FIELD(mTextBottom);
	PROTECTED_FIELD(mSelectingPressedPoint);
	PROTECTED_FIELD(mTransformFrame);
	PROTECTED_FIELD(mTransformFrameVisible);
	PROTECTED_FIELD(mTransformFrameBasis);
	PROTECTED_FIELD(mIsViewScrolling);
	PROTECTED_FIELD(mBeforeTransformKeys);
	PROTECTED_FIELD(mActionsList);
}
END_META;
CLASS_METHODS_META(Editor::CurveEditor)
{

	typedef Dictionary<String, Curve*> _tmp1;

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(_tmp1, GetCurves);
	PUBLIC_FUNCTION(void, AddCurve, const String&, Curve*, const Color4&);
	PUBLIC_FUNCTION(void, RemoveCurve, Curve*);
	PUBLIC_FUNCTION(void, RemoveCurve, const String&);
	PUBLIC_FUNCTION(void, RemoveAllCurves);
	PUBLIC_FUNCTION(void, AddCurvesRange, Curve*, Curve*, const Color4&);
	PUBLIC_FUNCTION(void, RemoveCurvesRange, Curve*, Curve*);
	PUBLIC_FUNCTION(void, AddCurvesRange, const String&, const String&, const Color4&);
	PUBLIC_FUNCTION(void, RemoveCurvesRange, const String&, const String&);
	PUBLIC_FUNCTION(void, SetSelectionSpriteImage, const ImageAssetRef&);
	PUBLIC_FUNCTION(void, SetTextFont, const FontRef&);
	PUBLIC_FUNCTION(void, SetMainHandleImages, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&);
	PUBLIC_FUNCTION(void, SetSupportHandleImages, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnResEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(Curve*, FindCurve, const String&);
	PROTECTED_FUNCTION(void, InitializeContextMenu);
	PROTECTED_FUNCTION(void, InitializeTextDrawables);
	PROTECTED_FUNCTION(void, RecalculateViewArea);
	PROTECTED_FUNCTION(void, RedrawContent);
	PROTECTED_FUNCTION(void, DrawGrid);
	PROTECTED_FUNCTION(void, DrawCurves);
	PROTECTED_FUNCTION(void, DrawHandles);
	PROTECTED_FUNCTION(void, DrawSelection);
	PROTECTED_FUNCTION(void, DrawTransformFrame);
	PROTECTED_FUNCTION(void, AddCurveKeyHandles, CurveInfo*, int);
	PROTECTED_FUNCTION(void, RemoveCurveKeyHandles, CurveInfo*, int);
	PROTECTED_FUNCTION(void, OnCurveKeyMainHandleDragged, CurveInfo*, KeyHandles*, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCurveKeyLeftSupportHandleDragged, CurveInfo*, KeyHandles*, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCurveKeyRightSupportHandleDragged, CurveInfo*, KeyHandles*, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftSupportHandlePosition, CurveInfo*, KeyHandles*, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightSupportHandlePosition, CurveInfo*, KeyHandles*, const Vec2F&);
	PROTECTED_FUNCTION(void, SmoothKey, CurveInfo*, int);
	PROTECTED_FUNCTION(void, OnCursorDblClicked, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseStayDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, CheckHandlesVisible);
	PROTECTED_FUNCTION(void, UpdateTransformFrame);
	PROTECTED_FUNCTION(bool, IsTransformFrameVisible);
	PROTECTED_FUNCTION(void, OnHandleCursorReleased, DragHandle*, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnHandleBeganDragging, DragHandle*);
	PROTECTED_FUNCTION(void, OnHandleMoved, DragHandle*, const Vec2F&);
	PROTECTED_FUNCTION(void, OnHandleCompletedChange, DragHandle*);
	PROTECTED_FUNCTION(void, SetSelectedKeysSupportsType, Curve::Key::Type);
	PROTECTED_FUNCTION(void, OnTransformFrameTransformed, const Basis&);
	PROTECTED_FUNCTION(void, OnTransformBegin);
	PROTECTED_FUNCTION(void, OnTransformCompleted);
	PROTECTED_FUNCTION(void, DoneAction, IAction*);
	PROTECTED_FUNCTION(void, OnEditPressed);
	PROTECTED_FUNCTION(void, OnAutoSmoothChecked, bool);
	PROTECTED_FUNCTION(void, OnFlatChecked, bool);
	PROTECTED_FUNCTION(void, OnFreeChecked, bool);
	PROTECTED_FUNCTION(void, OnBrokenChecked, bool);
	PROTECTED_FUNCTION(void, OnDiscreteChecked, bool);
	PROTECTED_FUNCTION(void, OnCopyPressed);
	PROTECTED_FUNCTION(void, OnCutPressed);
	PROTECTED_FUNCTION(void, OnPastePressed);
	PROTECTED_FUNCTION(void, OnDeletePressed);
	PROTECTED_FUNCTION(void, OnInsertPressed);
	PROTECTED_FUNCTION(void, OnUndoPressed);
	PROTECTED_FUNCTION(void, OnRedoPressed);
}
END_META;

CLASS_BASES_META(Editor::CurveEditor::CurveCopyInfo)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::CurveEditor::CurveCopyInfo)
{
	PUBLIC_FIELD(curveId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(keys).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::CurveEditor::CurveCopyInfo)
{
}
END_META;
