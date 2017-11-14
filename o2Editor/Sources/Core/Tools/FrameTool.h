#pragma once

#include "Core/Tools/SelectionTool.h"
#include "Scene/ActorTransform.h"
#include "SceneWindow/SceneDragHandle.h"
#include "Utils/Math/Basis.h"

using namespace o2;

namespace o2
{
	class Sprite;
}

namespace Editor
{
	// ----------------------------------
	// Editor frame and pivot moving tool
	// ----------------------------------
	class FrameTool: public SelectionTool
	{
	public:
		typedef Vector<ActorTransform> ActorsTransformsVec;

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
		const Color4        mFrameColor = Color4(220, 220, 220, 255); // Regular handle color
		const float         mFrameHandlesSize = 9.0f;	// Frame corners handles size
		const float         mHandlesRotateSize = 17.0f;	// Rotation frame handles size

		SceneDragHandle     mLeftTopRotateHandle;		// Left top rotation handle
		SceneDragHandle     mLeftBottomRotateHandle;	// Left bottom rotation handle
		SceneDragHandle     mRightTopRotateHandle;		// Right top rotation handle
		SceneDragHandle     mRightBottomRotateHandle;	// Right bottom rotation handle
		SceneDragHandle     mLeftTopHandle;				// Left top corner frame handle
		SceneDragHandle     mLeftHandle;				// Left corner frame handle
		SceneDragHandle     mLeftBottomHandle;			// Left bottom corner frame handle
		SceneDragHandle     mTopHandle;					// Top corner frame handle
		SceneDragHandle     mBottomHandle;				// Bottom corner frame handle
		SceneDragHandle     mRightTopHandle;			// Right top corner frame handle
		SceneDragHandle     mRightHandle;				// Right corner frame handle
		SceneDragHandle     mRightBottomHandle;			// Right bottom corner frame handle
		SceneDragHandle     mPivotHandle;				// Frame or actor pivot handle

		Basis               mFrame;						// Frame basis
		Basis               mBeginDraggingFrame;        // Frame before dragging any handle
		Vec2F               mBeginDraggingOffset;       // Offset at beginning dragging from frame origin to cursor

		bool                mIsDragging = false;		// Is frame dragging
		bool                mChangedFromThis = false;	// Is actors changed from this, needs to break circular updating

		ActorsTransformsVec mBeforeTransforms;   		// Array of actors transformations before changing

		LinesVec            mSnapLines;                 // Immediate drawing lines, used for drawing snapping

	protected:
	    // Draws tool
		void DrawScene();

		// Draws snapping lines
		void DrawSnapLines();

		// It is called when tool was enabled
		void OnEnabled();

		// It is called when tool was disabled
		void OnDisabled();

		// It is called when scene actors was changed
		void OnSceneChanged(Vector<Actor*> changedActors);

		// It is called when actors selection was changed
		void OnActorsSelectionChanged(Vector<Actor*> actors);

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// It is called when key stay down during frame
		void OnKeyStayDown(const Input::Key& key);

		// It is called when key was pressed
		void OnKeyReleased(const Input::Key& key);

		// Transforms top selected actors
		void TransformActors(const Basis& transform);

		// Transforms top selected actors
		void TransformActorsWithAction(const Basis& transform);

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

		// Checks pivot handle position snapping to center and corners
		Vec2F CheckPivotSnapping(const Vec2F& point);

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

		// Calculates snapping offset for point by parallels lines, offset is on normal
		Vec2F CalculateSnapOffset(const Vec2F& point, const Basis& frame, 
								  const Vector<Vec2F>& xLines, const Vec2F& xNormal,
								  const Vector<Vec2F>& yLines, const Vec2F& yNormal);
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
	PROTECTED_FIELD(mFrameHandlesSize);
	PROTECTED_FIELD(mHandlesRotateSize);
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
	PROTECTED_FIELD(mFrame);
	PROTECTED_FIELD(mBeginDraggingFrame);
	PROTECTED_FIELD(mBeginDraggingOffset);
	PROTECTED_FIELD(mIsDragging);
	PROTECTED_FIELD(mChangedFromThis);
	PROTECTED_FIELD(mBeforeTransforms);
	PROTECTED_FIELD(mSnapLines);
}
END_META;
CLASS_METHODS_META(Editor::FrameTool)
{

	PROTECTED_FUNCTION(void, DrawScene);
	PROTECTED_FUNCTION(void, DrawSnapLines);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnSceneChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, OnActorsSelectionChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyStayDown, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, TransformActors, const Basis&);
	PROTECTED_FUNCTION(void, TransformActorsWithAction, const Basis&);
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
	PROTECTED_FUNCTION(Vec2F, CheckPivotSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckLeftBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightTopSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CheckRightBottomSnapping, const Vec2F&);
	PROTECTED_FUNCTION(Vec2F, CalculateSnapOffset, const Vec2F&, const Basis&, const Vector<Vec2F>&, const Vec2F&, const Vector<Vec2F>&, const Vec2F&);
}
END_META;
