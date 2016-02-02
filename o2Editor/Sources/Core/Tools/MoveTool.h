#pragma once

#include "Core/Tools/SelectionTool.h"
#include "SceneWindow/SceneDragHandle.h"

// -----------------------
// Move actors editor tool
// -----------------------
class EditorMoveTool: public EditorSelectionTool
{
public:
	float snapStep = 10.0f;

	// Default constructor
	EditorMoveTool();

	// Destructor
	~EditorMoveTool();

	IOBJECT(EditorMoveTool);

protected:
	SceneDragHandle mHorDragHandle;
	SceneDragHandle mVerDragHandle;
	SceneDragHandle mBothDragHandle;
	Vec2F           mLastSceneHandlesPos;
	Vec2F           mSnapPosition;
	float           mHandlesAngle = 0.0f;

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
};