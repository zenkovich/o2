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

		// It is called when some handle was pressed, stores before transformations
		void HandlePressed();

		// It is called when handle was released, completes transformation action
		void HandleReleased();

		// Handles moved
		void HandlesMoved(const Vec2F& delta, bool snapHor = false, bool spanVer = false);

		// Updates handles position
		void UpdateHandlesPosition();

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// It is called when key stay down during frame
		void OnKeyStayDown(const Input::Key& key);

		// It is called when key was pressed
		void OnKeyReleased(const Input::Key& key);

		// Moves selected actors on delta
		void MoveSelectedActors(const Vec2F& delta);

		// Moves selected actors on delta
		void MoveSelectedActorsWithAction(const Vec2F& delta);
	};
}

CLASS_BASES_META(Editor::MoveTool)
{
	BASE_CLASS(Editor::SelectionTool);
}
END_META;
CLASS_FIELDS_META(Editor::MoveTool)
{
	PUBLIC_FIELD(snapStep);
	PROTECTED_FIELD(mHorDragHandle);
	PROTECTED_FIELD(mVerDragHandle);
	PROTECTED_FIELD(mBothDragHandle);
	PROTECTED_FIELD(mLastSceneHandlesPos);
	PROTECTED_FIELD(mSnapPosition);
	PROTECTED_FIELD(mHandlesAngle);
	PROTECTED_FIELD(mBeforeTransforms);
}
END_META;
CLASS_METHODS_META(Editor::MoveTool)
{

	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, OnSceneChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, OnActorsSelectionChanged, Vector<Actor*>);
	PROTECTED_FUNCTION(void, OnHorDragHandleMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, OnVerDragHandleMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, OnBothDragHandleMoved, const Vec2F&);
	PROTECTED_FUNCTION(void, HandlePressed);
	PROTECTED_FUNCTION(void, HandleReleased);
	PROTECTED_FUNCTION(void, HandlesMoved, const Vec2F&, bool, bool);
	PROTECTED_FUNCTION(void, UpdateHandlesPosition);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyStayDown, const Input::Key&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, MoveSelectedActors, const Vec2F&);
	PROTECTED_FUNCTION(void, MoveSelectedActorsWithAction, const Vec2F&);
}
END_META;
