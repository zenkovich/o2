#pragma once

#include "Core/Tools/SelectionTool.h"
#include "Scene/ActorTransform.h"
#include "SceneWindow/SceneDragHandle.h"

namespace Editor
{
	// -----------------------
	// Move actors editor tool
	// -----------------------
	class MoveTool: public SelectionTool
	{
	public:
		typedef Vector<ActorTransform> ActorsTransformsVec;

	public:
		float snapStep = 10.0f; // Moving snap step

		// Default constructor
		MoveTool();

		// Destructor
		~MoveTool();

		IOBJECT(MoveTool);

	protected:
		SceneDragHandle     mHorDragHandle;       // Horizontal arrow handle
		SceneDragHandle     mVerDragHandle;       // Vertical arrow handle
		SceneDragHandle     mBothDragHandle;      // Both arrow handle
		Vec2F               mLastSceneHandlesPos; // Last scene handles position 
		Vec2F               mSnapPosition;        // Snapping handles position
		float               mHandlesAngle = 0.0f; // Handles angle, in radians
		ActorsTransformsVec mBeforeTransforms;    // Before transformation transforms

	protected:
		// Updates tool
		void Update(float dt);

		// Calls when tool was enabled
		void OnEnabled();

		// Calls when tool was disabled
		void OnDisabled();

		// Calls when scene actors was changed
		void OnSceneChanged(Vector<Actor*> changedActors);

		// Calls when actors selection was changed
		void OnActorsSelectionChanged(Vector<Actor*> actors);

		// Calls when horizontal drag handle was moved
		void OnHorDragHandleMoved(const Vec2F& position);

		// Calls when horizontal drag handle was moved
		void OnVerDragHandleMoved(const Vec2F& position);

		// Calls when horizontal drag handle was moved
		void OnBothDragHandleMoved(const Vec2F& position);

		// Calls when some handle was pressed, stores before transformations
		void HandlePressed();

		// Calls when handle was released, completes transformation action
		void HandleReleased();

		// Handles moved
		void HandlesMoved(const Vec2F& delta, bool snapHor = false, bool spanVer = false);

		// Updates handles position
		void UpdateHandlesPosition();

		// Calls when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// Calls when key stay down during frame
		void OnKeyStayDown(const Input::Key& key);

		// Calls when key was pressed
		void OnKeyReleased(const Input::Key& key);

		// Moves selected actors on delta
		void MoveSelectedActors(const Vec2F& delta);

		// Moves selected actors on delta
		void MoveSelectedActorsWithAction(const Vec2F& delta);
	};
}
