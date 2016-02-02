#pragma once

#include "Core/Tools/SelectionTool.h"
#include "SceneWindow/SceneDragHandle.h"

// ------------------------
// Scale actors editor tool
// ------------------------
class EditorScaleTool: public EditorSelectionTool
{
public:
	// Default constructor
	EditorScaleTool();

	// Destructor
	~EditorScaleTool();

	IOBJECT(EditorScaleTool);

protected:
	SceneDragHandle mHorDragHandle;
	SceneDragHandle mVerDragHandle;
	SceneDragHandle mBothDragHandle;
	float           mHandlesAngle = 0.0f;
	Vec2F           mSceneHandlesPos;

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

	// Updates handles position
	void UpdateHandlesPosition();

	// Calls when key was pressed
	void OnKeyPressed(const Input::Key& key);

	// Calls when key stay down during frame
	void OnKeyStayDown(const Input::Key& key);

	// Calls when key was pressed
	void OnKeyReleased(const Input::Key& key);

	// Moves selected actors on delta
	void ScaleSelectedActors(const Vec2F& scale);
};
