#pragma once

#include "Assets/ImageAsset.h"
#include "Core/Actions/IAction.h"
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
	class UIEditBox;
	class UIWindow;
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
		void Draw() override;

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Adds editing curve with color. If color is default it will be randomized
		void AddEditingCurve(const String& id, Curve* curve, const Color4& color = Color4::Green());

		// Removed curve from editing
		void RemoveEditingCurve(Curve* curve);

		// Removed curve from editing
		void RemoveEditingCurve(const String& id);

		// Removes all editing curves
		void RemoveAllEditingCurves();

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
		void UpdateTransform(bool withChildren = true) override;

		SERIALIZABLE(UICurveEditor);

	protected:
		typedef Vector<Vec2F> PointsVec;
		typedef Vector<SelectableDragHandle*> SelectableHandlesVec;
		typedef Vector<IAction*> ActionsVec;

		struct KeyHandles
		{
			UICurveEditor*       curveEditor = nullptr;
			SelectableDragHandle mainHandle;
			SelectableDragHandle leftSupportHandle;
			SelectableDragHandle rightSupportHandle;
				           
			int                  curveKeyIdx;

			KeyHandles() {}
			KeyHandles(const SelectableDragHandle& mainSample, const SelectableDragHandle& supportSample, UICurveEditor* editor);

			void Draw(const RectF& camRect);
			bool IsSomeHandleSelected() const;
		};
		typedef Vector<KeyHandles*> KeyHandlesVec;

		struct CurveInfo
		{
			String         curveId;
			Curve*         curve = nullptr;
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

		class CurveCopyInfo: ISerializable
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
		UIContextMenu*          mContextMenu = nullptr;             // Context menu for editing keys properties, copying, pasting and other
							    								    
		SelectableDragHandle    mMainHandleSample;                  // Main handle sample, uses to copy sprites @SERIALIZABLE
		SelectableDragHandle    mSupportHandleSample;               // Support handle sample, uses to copy sprites @SERIALIZABLE
							    								    
		CurveInfosVec           mCurves;                            // Editing curves infos list 
		RangeInfosVec           mRanges;                            // Curves ranges list
							    								    
		SelectableHandlesVec    mSupportHandles;                    // Support points handles list
		SelectableHandlesVec    mSelectingHandlesBuf;               // Potentially selecting handles while selecting
							    								    
		Sprite*                 mSelectionSprite = nullptr;         // Selection sprite @SERIALIZABLE
		FontRef                 mTextFont;                          // Captions text font @SERIALIZABLE
		Text*                   mTextLeft = nullptr;                // Captions text drawable at left border
		Text*                   mTextRight = nullptr;               // Captions text drawable at right border
		Text*                   mTextTop = nullptr;                 // Captions text drawable at top border
		Text*                   mTextBottom = nullptr;              // Captions text drawable at bottom border
							    								    
		Vec2F                   mSelectingPressedPoint;             // Point, where cursor was pressed, selection starts here, in local space
							    								    
		FrameHandles            mTransformFrame;                    // Keys transformation frame
		bool                    mTransformFrameVisible = false;     // Is transform frame visible. it visible when 2 or more main handles was selected
		Basis                   mTransformFrameBasis;               // Basis of transform frame in local space
								 							   	    
		bool                    mIsViewScrolling = false;           // Is scrolling view at this time
							    
		UIWindow*               mEditValueWindow = nullptr;         // Key position and value editing window
		UIEditBox*              mEditValueWindowValue = nullptr;    // Key value editing box
		UIEditBox*              mEditValueWindowPosition = nullptr; // Key position editing box
							    
		CurveKeysInfosVec       mBeforeTransformKeys;               // Stored selected keys before handles transformed
															       
		ActionsVec              mUndoActions;                       // Actions that can be undo
		ActionsVec              mRedoActions;                       // Actions that can be redo

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when visible was changed. Sets context menu items priority
		void OnVisibleChanged() override;

		// It is called when scrolling
		void OnScrolled(float scroll) override;

		// Searches curve by id
		Curve* FindCurve(const String& id);

		// Initializes context menu items
		void InitializeContextMenu();

		// Initializes text drawables by font and sets aligning
		void InitializeTextDrawables();

		// Initializes edit window controls
		void InitializeEditValueWindow();

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
		void OnHandleCursorReleased(SelectableDragHandle* handle, const Input::Cursor& cursor);

		// It is called when selectable handle was began to drag
		void OnHandleBeganDragging(SelectableDragHandle* handle);

		// It is called when selectable handle moved, moves all selected handles position
		void OnHandleMoved(SelectableDragHandle* handle, const Input::Cursor& cursor);

		// It is called when selectable handle completed changing
		void OnHandleCompletedChange(SelectableDragHandle* handle);

		// Sets all selected keys supports type
		void SetSelectedKeysSupportsType(Curve::Key::Type type);

		// It is called when transform frame was transformed
		void OnTransformFrameTransformed(const Basis& basis);

		// It is called when beginning transforming some handles. Stores selected keys before changing
		void OnTransformBegin();

		// It is called when transform completed. Checks that selected keys changed, creates action for undo/redo
		void OnTransformCompleted();

		// It is called when edit key window position edit box was changed
		void OnEditKeyPositionChanged(const WString& str);

		// It is called when edit key window value edit box was changed
		void OnEditKeyValueChanged(const WString& str);

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

	protected:
		class AddKeysAction: public IAction
		{
		public:
			AddKeysAction();
			AddKeysAction(const CurveKeysInfosVec& infos, UICurveEditor* editor);

			String GetName();
			void Redo();
			void Undo();

			SERIALIZABLE(AddKeysAction);

		protected:
			CurveKeysInfosVec mInfos;
			UICurveEditor*    mEditor;
		};

		class DeleteKeysAction: public IAction
		{
		public:
			DeleteKeysAction();
			DeleteKeysAction(const CurveKeysInfosVec& infos, UICurveEditor* editor);

			String GetName();
			void Redo();
			void Undo();

			SERIALIZABLE(DeleteKeysAction);

		protected:
			CurveKeysInfosVec mInfos;
			UICurveEditor*    mEditor;
		};

		class KeysChangeAction: public IAction
		{
		public:
			struct KeysInfo
			{
				String                  curveId;
				Curve::KeysVec          beforeKeys;
				Curve::KeysVec          afterKeys;
				SelectedHandlesInfosVec selectedHandles;

				bool operator==(const KeysInfo& other) const;
			};
			typedef Vector<KeysInfo> KeysInfosVec;

		public:
			KeysChangeAction();
			KeysChangeAction(const KeysInfosVec& infos, UICurveEditor* editor);

			String GetName();
			void Redo();
			void Undo();

			SERIALIZABLE(KeysChangeAction);

		protected:
			KeysInfosVec   mInfos;
			UICurveEditor* mEditor;
		};
	};
}

CLASS_BASES_META(Editor::UICurveEditor)
{
	BASE_CLASS(Editor::UIFrameScrollView);
	BASE_CLASS(o2::SelectableDragHandlesGroup);
}
END_META;
CLASS_FIELDS_META(Editor::UICurveEditor)
{
	PROTECTED_FIELD(mContextMenu);
	PROTECTED_FIELD(mMainHandleSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSupportHandleSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCurves);
	PROTECTED_FIELD(mRanges);
	PROTECTED_FIELD(mSupportHandles);
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
	PROTECTED_FIELD(mEditValueWindow);
	PROTECTED_FIELD(mEditValueWindowValue);
	PROTECTED_FIELD(mEditValueWindowPosition);
	PROTECTED_FIELD(mBeforeTransformKeys);
	PROTECTED_FIELD(mUndoActions);
	PROTECTED_FIELD(mRedoActions);
}
END_META;
CLASS_METHODS_META(Editor::UICurveEditor)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, AddEditingCurve, const String&, Curve*, const Color4&);
	PUBLIC_FUNCTION(void, RemoveEditingCurve, Curve*);
	PUBLIC_FUNCTION(void, RemoveEditingCurve, const String&);
	PUBLIC_FUNCTION(void, RemoveAllEditingCurves);
	PUBLIC_FUNCTION(void, AddCurvesRange, Curve*, Curve*, const Color4&);
	PUBLIC_FUNCTION(void, RemoveCurvesRange, Curve*, Curve*);
	PUBLIC_FUNCTION(void, AddCurvesRange, const String&, const String&, const Color4&);
	PUBLIC_FUNCTION(void, RemoveCurvesRange, const String&, const String&);
	PUBLIC_FUNCTION(void, SetSelectionSpriteImage, const ImageAssetRef&);
	PUBLIC_FUNCTION(void, SetTextFont, const FontRef&);
	PUBLIC_FUNCTION(void, SetMainHandleImages, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&);
	PUBLIC_FUNCTION(void, SetSupportHandleImages, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&, const ImageAssetRef&);
	PUBLIC_FUNCTION(void, UpdateTransform, bool);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(Curve*, FindCurve, const String&);
	PROTECTED_FUNCTION(void, InitializeContextMenu);
	PROTECTED_FUNCTION(void, InitializeTextDrawables);
	PROTECTED_FUNCTION(void, InitializeEditValueWindow);
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
	PROTECTED_FUNCTION(void, OnHandleCursorReleased, SelectableDragHandle*, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnHandleBeganDragging, SelectableDragHandle*);
	PROTECTED_FUNCTION(void, OnHandleMoved, SelectableDragHandle*, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnHandleCompletedChange, SelectableDragHandle*);
	PROTECTED_FUNCTION(void, SetSelectedKeysSupportsType, Curve::Key::Type);
	PROTECTED_FUNCTION(void, OnTransformFrameTransformed, const Basis&);
	PROTECTED_FUNCTION(void, OnTransformBegin);
	PROTECTED_FUNCTION(void, OnTransformCompleted);
	PROTECTED_FUNCTION(void, OnEditKeyPositionChanged, const WString&);
	PROTECTED_FUNCTION(void, OnEditKeyValueChanged, const WString&);
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

CLASS_BASES_META(Editor::UICurveEditor::CurveCopyInfo)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::UICurveEditor::CurveCopyInfo)
{
	PUBLIC_FIELD(curveId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(keys).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::UICurveEditor::CurveCopyInfo)
{
}
END_META;

CLASS_BASES_META(Editor::UICurveEditor::AddKeysAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::UICurveEditor::AddKeysAction)
{
	PROTECTED_FIELD(mInfos);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::UICurveEditor::AddKeysAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::UICurveEditor::DeleteKeysAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::UICurveEditor::DeleteKeysAction)
{
	PROTECTED_FIELD(mInfos);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::UICurveEditor::DeleteKeysAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::UICurveEditor::KeysChangeAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::UICurveEditor::KeysChangeAction)
{
	PROTECTED_FIELD(mInfos);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::UICurveEditor::KeysChangeAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
