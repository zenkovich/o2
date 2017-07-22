#pragma once

#include "Core/Tools/SelectionTool.h"
#include "Scene/ActorTransform.h"
#include "SceneWindow/SceneDragHandle.h"

namespace Editor
{
	// ------------------------
	// Scale actors editor tool
	// ------------------------
	class ScaleTool: public SelectionTool
	{
	public:
		typedef Vector<ActorTransform> ActorsTransformsVec;

	public:
		float bothScaleSence = 0.01f;

		// Default constructor
		ScaleTool();

		// Destructor
		~ScaleTool();

		IOBJECT(ScaleTool);

	protected:
		SceneDragHandle     mHorDragHandle;					// Horizontal scale drag handle
		SceneDragHandle     mVerDragHandle;					// Vertical scale drag handle
		SceneDragHandle     mBothDragHandle;				// Bot axis scale drag handle
		float               mHandlesAngle = 0.0f;			// Handles angle in radians
		Vec2F               mSceneHandlesPos;				// Scene space handles position
		Vec2F               mHandlesSize = Vec2F(100, 100);	// Handles size in screen space
		Vec2F               mLastHorHandlePos;				// Last horizontal handle position
		Vec2F               mLastVerHandlePos;				// Last vertical handle position
		Vec2F               mLastBothHandlePos;				// Last both axis handle position
		ActorsTransformsVec mBeforeTransforms;				// Array of actors' transformations before changing

	protected:
		// Updates tool
		void Update(float dt);

		// Draws screen
		void DrawScreen();

		// It is called when tool was enabled
		void OnEnabled();

		// It is called when tool was disabled
		void OnDisabled();

		// It is called when scene actors was changed
		void OnSceneChanged(Vector<Actor*> changedActors);

		// It is called when actors selection was changed
		void OnActorsSelectionChanged(Vector<Actor*> actors);

		// It is called when horizontal drag handle was moved
		void OnHorDragHandleMoved(const Vec2F& position);

		// It is called when horizontal drag handle was moved
		void OnVerDragHandleMoved(const Vec2F& position);

		// It is called when horizontal drag handle was moved
		void OnBothDragHandleMoved(const Vec2F& position);

		// Updates handles position
		void UpdateHandlesPosition();

		// Updates handles angle and position
		void UpdateHandlesAngleAndPositions(float angle);

		// Updates handles position
		void UpdateHandlesPositions();

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// It is called when key stay down during frame
		void OnKeyStayDown(const Input::Key& key);

		// It is called when key was pressed
		void OnKeyReleased(const Input::Key& key);

		// Moves selected actors on delta
		void ScaleSelectedActors(const Vec2F& scale);

		// It is called when some handle was pressed, stores before transformations
		void HandlePressed();

		// It is called when handle was released, completes transformation action
		void HandleReleased();
	};

}
