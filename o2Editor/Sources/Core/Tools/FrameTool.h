#pragma once

#include "Core/Tools/SelectionTool.h"
#include "SceneWindow/SceneDragHandle.h"
#include "Utils/Math/Basis.h"

using namespace o2;

namespace o2
{
	class Sprite;
}

namespace Editor
{
	class TransformAction;

	// ----------------------------------
	// Editor frame and pivot moving tool
	// ----------------------------------
	class FrameTool: public SelectionTool
	{
	public:
		typedef Vector<Basis> BasisVec;

	public:
		// Default constructor
		FrameTool();

		FrameTool& operator=(const FrameTool& other) { return *this; }

		// Destructor
		~FrameTool();

		IOBJECT(FrameTool);

	protected:
		struct SnapLine
		{
			Color4 color;
			Vec2F  begin;
			Vec2F  end;

			SnapLine() {}
			SnapLine(const Vec2F& begin, const Vec2F& end, const Color4& color):begin(begin), end(end), color(color) {}

			bool operator==(const SnapLine& other) const { return color == other.color && begin == other.begin && end == other.end; }
		};
		typedef Vector<SnapLine> LinesVec;

	protected:
		const Color4        mFrameColor = Color4(220, 220, 220, 255);        // Objects frame color
		const Color4        mObjectColor = Color4(220, 220, 220, 150);       // Object color
		const Color4        mParentColor = Color4(220, 220, 220, 100);       // Object parent color
		const Color4        mAnchorsFrameColor = Color4(100, 100, 255, 255); // Widgets anchors frame color
		const Color4        mSnapLinesColor = Color4(100, 255, 100, 255);    // Widgets anchors frame color

		const float         mFrameMinimalSize = 0.001f;   // Minimal size of transforming frame

		SceneDragHandle     mLeftTopRotateHandle;		  // Left top rotation handle
		SceneDragHandle     mLeftBottomRotateHandle;	  // Left bottom rotation handle
		SceneDragHandle     mRightTopRotateHandle;		  // Right top rotation handle
		SceneDragHandle     mRightBottomRotateHandle;	  // Right bottom rotation handle
		SceneDragHandle     mLeftTopHandle;				  // Left top corner frame handle
		SceneDragHandle     mLeftHandle;				  // Left corner frame handle
		SceneDragHandle     mLeftBottomHandle;			  // Left bottom corner frame handle
		SceneDragHandle     mTopHandle;					  // Top corner frame handle
		SceneDragHandle     mBottomHandle;				  // Bottom corner frame handle
		SceneDragHandle     mRightTopHandle;			  // Right top corner frame handle
		SceneDragHandle     mRightHandle;				  // Right corner frame handle
		SceneDragHandle     mRightBottomHandle;			  // Right bottom corner frame handle
		SceneDragHandle     mPivotHandle;				  // Frame or object pivot handle
														  
		SceneDragHandle     mAnchorsLeftTopHandle;		  // Anchors Left top corner frame handle
		SceneDragHandle     mAnchorsLeftBottomHandle;	  // Anchors Left bottom corner frame handle
		SceneDragHandle     mAnchorsRightTopHandle;		  // Anchors Right top corner frame handle
		SceneDragHandle     mAnchorsRightBottomHandle;	  // Anchors Right bottom corner frame handle
		SceneDragHandle     mAnchorsCenter;               // Anchors center, enables when all anchors in one point and drags all of them
														  
		Basis               mFrame;						  // Frame basis
														  
		Basis               mAnchorsFrame;                // Anchors frame basis
		bool                mAnchorsFrameEnabled = false; // Is selected some UI widgets and anchors frame enabled

		bool                mPivotHandleEnabled = false;  // Is selected object supports pivot 

		Basis               mBeginDraggingFrame;          // Frame before dragging any handle
		Vec2F               mBeginDraggingOffset;         // Offset at beginning dragging from frame origin to cursor

		bool                mIsDragging = false;		  // Is frame dragging
		bool                mChangedFromThis = false;	  // Is objects changed from this, needs to break circular updating

		BasisVec            mBeforeTransforms;   		  // Array of objects transformations before changing
		TransformAction*    mTransformAction = nullptr;   // Current transform action. Creates when transform started

		LinesVec            mSnapLines;                   // Immediate drawing lines, used for drawing snapping

	protected:
	    // Draws tool
		void DrawScene();

		// Draws snapping lines
		void DrawSnapLines();

		// It is called when tool was enabled
		void OnEnabled();

		// It is called when tool was disabled
		void OnDisabled();

		// It is called when scene objects was changed
		void OnSceneChanged(Vector<SceneEditableObject*> changedObjects);

		// It is called when objects selection was changed
		void OnObjectsSelectionChanged(Vector<SceneEditableObject*> objects);

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// It is called when key stay down during frame
		void OnKeyStayDown(const Input::Key& key);

		// It is called when key was pressed
		void OnKeyReleased(const Input::Key& key);

		// Transforms top selected objects
		void TransformObjects(const Basis& transform);

		// Transforms top selected objects
		void TransformObjectsWithAction(const Basis& transform);

		// Transforms top selected objects anchors
		void TransformAnchorsObjects(const Basis& transform);

		// Updates selection frame and handles
		void UpdateSelectionFrame();

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Left top handle moved
		void OnLeftTopHandle(const Vec2F& position);

		// Left handle moved
		void OnLeftHandle(const Vec2F& position);

		// Left bottom handle moved
		void OnLeftBottomHandle(const Vec2F& position);

		// Top handle moved
		void OnTopHandle(const Vec2F& position);

		// Bottom handle moved
		void OnBottomHandle(const Vec2F& position);

		// Right top handle moved
		void OnRightTopHandle(const Vec2F& position);

		// Right handle moved
		void OnRightHandle(const Vec2F& position);

		// Right bottom handle moved
		void OnRightBottomHandle(const Vec2F& position);

		// Left top anchor handle moved
		void OnAnchorLeftTopHandle(const Vec2F& position);

		// Left bottom anchor handle moved
		void OnAnchorLeftBottomHandle(const Vec2F& position);

		// Right top anchor handle moved
		void OnAnchorRightTopHandle(const Vec2F& position);

		// Right bottom anchor handle moved
		void OnAnchorRightBottomHandle(const Vec2F& position);

		// Center anchor handle moved
		void OnCenterAnchorHandle(const Vec2F& position);

		// Pivot handle moved
		void OnPivotHandle(const Vec2F& position);

		// Left top rotation handle moved
		void OnLeftTopRotateHandle(const Vec2F& position);

		// Left bottom rotation handle moved
		void OnLeftBottomRotateHandle(const Vec2F& position);

		// Right top rotation handle moved
		void OnRightTopRotateHandle(const Vec2F& position);

		// Right bottom rotation handle moved
		void OnRightBottomRotateHandle(const Vec2F& position);

		// Rotate handle moved
		void OnRotateHandle(const Vec2F& position, Vec2F lastHandleCoords);

		// Sets all handles enable
		void SetHandlesEnable(bool enable);

		// Updates handles position and angle
		void UpdateHandlesTransform();

		// It is called when some handle was pressed, stores before transformations
		void HandlePressed();

		// It is called when handle was released, completes transformation action
		void HandleReleased();

		// Return transformed basis when Left top handle moved
		Basis GetLeftTopHandleTransformed(const Vec2F& position);

		// Return transformed basis when Left handle moved
		Basis GetLeftHandleTransformed(const Vec2F& position);

		// Return transformed basis when Left bottom handle moved
		Basis GetLeftBottomHandleTransformed(const Vec2F& position);

		// Return transformed basis when Top handle moved
		Basis GetTopHandleTransformed(const Vec2F& position);

		// Return transformed basis when Bottom handle moved
		Basis GetBottomHandleTransformed(const Vec2F& position);

		// Return transformed basis when Right top handle moved
		Basis GetRightTopHandleTransformed(const Vec2F& position);

		// Return transformed basis when Right handle moved
		Basis GetRightHandleTransformed(const Vec2F& position);

		// Return transformed basis when Right bottom handle moved
		Basis GetRightBottomHandleTransformed(const Vec2F& position);

		// Return transformed anchor basis when Left top handle moved
		Basis GetLeftTopAnchorHandleTransformed(const Vec2F& position);

		// Return transformed anchor basis when Left bottom handle moved
		Basis GetLeftBottomAnchorHandleTransformed(const Vec2F& position);

		// Return transformed anchor basis when Right top handle moved
		Basis GetRightTopAnchorHandleTransformed(const Vec2F& position);

		// Return transformed anchor basis when Right bottom handle moved
		Basis GetRightBottomAnchorHandleTransformed(const Vec2F& position);

		// Return transformed anchor basis when center anchor moved
		Basis GetAnchorsCenterHandleTransformed(const Vec2F& position);

		// Checks handle position snapping to center and corners of frame
		Vec2F CheckFrameSnapping(const Vec2F& point, const Basis& frame);

		// Checks pivot handle position snapping to center and corners
		Vec2F CheckPivotSnapping(const Vec2F& point);

		// Checks anchor handle position snapping to center and corners of parent
		Vec2F CheckAnchorCenterSnapping(const Vec2F& point);

		// Checks anchor handle position snapping to center and corners of parent
		Vec2F CheckAnchorLeftTopSnapping(const Vec2F& point);

		// Checks anchor handle position snapping to center and corners of parent
		Vec2F CheckAnchorLeftBottomSnapping(const Vec2F& point);

		// Checks anchor handle position snapping to center and corners of parent
		Vec2F CheckAnchorRightTopSnapping(const Vec2F& point);

		// Checks anchor handle position snapping to center and corners of parent
		Vec2F CheckAnchorRightBottomSnapping(const Vec2F& point);

		// Checks top handle position snapping to other objects
		Vec2F CheckTopSnapping(const Vec2F& point);

		// Checks bottom handle position snapping to other objects
		Vec2F CheckBottomSnapping(const Vec2F& point);

		// Checks left handle position snapping to other objects
		Vec2F CheckLeftSnapping(const Vec2F& point);

		// Checks right handle position snapping to other objects
		Vec2F CheckRightSnapping(const Vec2F& point);

		// Checks left top handle position snapping to other objects
		Vec2F CheckLeftTopSnapping(const Vec2F& point);

		// Checks left bottom handle position snapping to other objects
		Vec2F CheckLeftBottomSnapping(const Vec2F& point);

		// Checks right top handle position snapping to other objects
		Vec2F CheckRightTopSnapping(const Vec2F& point);

		// Checks right bottom handle position snapping to other objects
		Vec2F CheckRightBottomSnapping(const Vec2F& point);

		// Checks is point in area of top handle
		bool IsPointInTopHandle(const Vec2F& point);

		// Checks is point in area of left handle
		bool IsPointInLeftHandle(const Vec2F& point);

		// Checks is point in area of right handle
		bool IsPointInRightHandle(const Vec2F& point);

		// Checks is point in area of bottom handle
		bool IsPointInBottomHandle(const Vec2F& point);

		// Checks is point in area of center anchors handle
		bool IsPointInAnchorsCenterHandle(const Vec2F& point);

		// Checks is all anchor handles in same point and enables center anchors handle
		void CheckAnchorsCenterEnabled();

		// Calculates snapping offset for point by parallels lines, offset is on normal
		Vec2F CalculateSnapOffset(const Vec2F& point, const Basis& frame, 
								  const Vector<Vec2F>& xLines, const Vec2F& xNormal,
								  const Vector<Vec2F>& yLines, const Vec2F& yNormal,
								  const Vector<SceneEditableObject*>& objects);
	};
}

CLASS_BASES_META(Editor::FrameTool)
{
	BASE_CLASS(Editor::SelectionTool);
}
END_META;
CLASS_FIELDS_META(Editor::FrameTool)
{
	PROTECTED_FIELD(mFrameColor);
	PROTECTED_FIELD(mObjectColor);
	PROTECTED_FIELD(mParentColor);
	PROTECTED_FIELD(mAnchorsFrameColor);
	PROTECTED_FIELD(mSnapLinesColor);
	PROTECTED_FIELD(mFrameMinimalSize);
	PROTECTED_FIELD(mLeftTopRotateHandle);
	PROTECTED_FIELD(mLeftBottomRotateHandle);
	PROTECTED_FIELD(mRightTopRotateHandle);
	PROTECTED_FIELD(mRightBottomRotateHandle);
	PROTECTED_FIELD(mLeftTopHandle);
	PROTECTED_FIELD(mLeftHandle);
	PROTECTED_FIELD(mLeftBottomHandle);
	PROTECTED_FIELD(mTopHandle);
	PROTECTED_FIELD(mBottomHandle);
	PROTECTED_FIELD(mRightTopHandle);
	PROTECTED_FIELD(mRightHandle);
	PROTECTED_FIELD(mRightBottomHandle);
	PROTECTED_FIELD(mPivotHandle);
	PROTECTED_FIELD(mAnchorsLeftTopHandle);
	PROTECTED_FIELD(mAnchorsLeftBottomHandle);
	PROTECTED_FIELD(mAnchorsRightTopHandle);
	PROTECTED_FIELD(mAnchorsRightBottomHandle);
	PROTECTED_FIELD(mAnchorsCenter);
	PROTECTED_FIELD(mFrame);
	PROTECTED_FIELD(mAnchorsFrame);
	PROTECTED_FIELD(mAnchorsFrameEnabled);
	PROTECTED_FIELD(mPivotHandleEnabled);
	PROTECTED_FIELD(mBeginDraggingFrame);
	PROTECTED_FIELD(mBeginDraggingOffset);
	PROTECTED_FIELD(mIsDragging);
	PROTECTED_FIELD(mChangedFromThis);
	PROTECTED_FIELD(mBeforeTransforms);
	PROTECTED_FIELD(mTransformAction);
	PROTECTED_FIELD(mSnapLines);
}
END_META;
CLASS_METHODS_META(Editor::FrameTool)
{

	PROTECTED_FUNCTION(void, DrawScene);
	PROTECTED_FUNCTION(void, DrawSnapLines);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnSceneChanged, Vector<SceneEditableObject*>);
	PROTECTED_FUNCTION(void, OnObjectsSelectionChanged, Vector<SceneEditableObject*>);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyStayDown, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, TransformObjects, const Basis&);
	PROTECTED_FUNCTION(void, TransformObjectsWithAction, const Basis&);
	PROTECTED_FUNCTION(void, TransformAnchorsObjects, const Basis&);
	PROTECTED_FUNCTION(void, UpdateSelectionFrame);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnLeftTopHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnLeftHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnLeftBottomHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnTopHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnBottomHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRightTopHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRightHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRightBottomHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnAnchorLeftTopHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnAnchorLeftBottomHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnAnchorRightTopHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnAnchorRightBottomHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCenterAnchorHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnPivotHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnLeftTopRotateHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnLeftBottomRotateHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRightTopRotateHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRightBottomRotateHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, OnRotateHandle, const Vec2F&, Vec2F);
	PROTECTED_FUNCTION(void, SetHandlesEnable, bool);
	PROTECTED_FUNCTION(void, UpdateHandlesTransform);
	PROTECTED_FUNCTION(void, HandlePressed);
	PROTECTED_FUNCTION(void, HandleReleased);
	PROTECTED_FUNCTION(Basis, GetLeftTopHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetLeftHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetLeftBottomHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetTopHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetBottomHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetRightTopHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetRightHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetRightBottomHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetLeftTopAnchorHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetLeftBottomAnchorHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetRightTopAnchorHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetRightBottomAnchorHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Basis, GetAnchorsCenterHandleTransformed, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckFrameSnapping, const Vec2F&, const Basis&);
	PROTECTED_FUNCTION(Vec2F, CheckPivotSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckAnchorCenterSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckAnchorLeftTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckAnchorLeftBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckAnchorRightTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckAnchorRightBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(bool, IsPointInTopHandle, const Vec2F&);
	PROTECTED_FUNCTION(bool, IsPointInLeftHandle, const Vec2F&);
	PROTECTED_FUNCTION(bool, IsPointInRightHandle, const Vec2F&);
	PROTECTED_FUNCTION(bool, IsPointInBottomHandle, const Vec2F&);
	PROTECTED_FUNCTION(bool, IsPointInAnchorsCenterHandle, const Vec2F&);
	PROTECTED_FUNCTION(void, CheckAnchorsCenterEnabled);
	PROTECTED_FUNCTION(Vec2F, CalculateSnapOffset, const Vec2F&, const Basis&, const Vector<Vec2F>&, const Vec2F&, const Vector<Vec2F>&, const Vec2F&, const Vector<SceneEditableObject*>&);
}
END_META;
