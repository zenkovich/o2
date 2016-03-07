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
		const Color4        mHandleRegularColor = Color4(220, 220, 220, 255); // Regular handle color
		const Color4        mHandleSelectColor = Color4(255, 255, 255, 255);  // Hovered handle color
		const Color4        mHandlePressedColor = Color4(150, 150, 150, 255); // Pressed handle color
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
		bool                mIsDragging = false;		// Is frame dragging
		bool                mChangedFromThis = false;	// Is actors changed from this, needs to break circular updating
		ActorsTransformsVec mBeforeTransforms;   		// Array of actors transformations before changing

	public:
		// Draws tool
		void DrawScene();

		// Calls when tool was enabled
		void OnEnabled();

		// Calls when tool was disabled
		void OnDisabled();

		// Calls when scene actors was changed
		void OnSceneChanged(Vector<Actor*> changedActors);

		// Calls when actors selection was changed
		void OnActorsSelectionChanged(Vector<Actor*> actors);

		// Calls when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// Calls when key stay down during frame
		void OnKeyStayDown(const Input::Key& key);

		// Calls when key was pressed
		void OnKeyReleased(const Input::Key& key);

		// Transforms top selected actors
		void TransformActors(const Basis& transform);

		// Transforms top selected actors
		void TransformActorsWithAction(const Basis& transform);

		// Updates selection frame and handles
		void UpdateSelectionFrame();

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
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

		// Right botom rotation handle moved
		void OnRightBottomRotateHandle(const Vec2F& position);

		// Sets all handles enable
		void SetHandlesEnable(bool enable);

		// Updates handles position and angle
		void UdateHandlesTransform();

		// Calls when some handle was pressed, stores before transformations
		void HandlePressed();

		// Calls when handle was released, completes transformation action
		void HandleReleased();
	};
}