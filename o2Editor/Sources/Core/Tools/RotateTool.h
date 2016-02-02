#pragma once

#include "Core/Tools/SelectionTool.h"
#include "SceneWindow/SceneDragHandle.h"

using namespace o2;

namespace o2
{
	class Mesh;
}

// ------------------
// Rotate actors tool
// ------------------
class EditorRotateTool: public EditorSelectionTool
{
public:
	float angleSnapStep = 15.0f;

	// Default constructor
	EditorRotateTool();

	// Destructor
	~EditorRotateTool();

	IOBJECT(EditorRotateTool);

protected:
	const float     mRotateRingInsideRadius = 60;
	const float     mRotateRingOutsideRadius = 100;
	const int       mRotateRingSegs = 100;
	const Color4    mRotateRingsColor = Color4(220, 220, 220, 255);
	const Color4    mRotateRingsFillColor = Color4(220, 220, 220, 50);
	const Color4    mRotateRingsFillColor2 = Color4(220, 220, 220, 100);
	const Color4    mRotateMeshClockwiseColor = Color4(211, 87, 40, 100);
	const Color4    mRotateMeshCClockwiseColor = Color4(87, 211, 40, 100);

	Mesh*           mRotateRingFillMesh;
	Mesh*           mAngleMesh;
	Vec2F           mScenePivot;

	SceneDragHandle mPivotDragHandle;
	float           mPressAngle;
	float           mCurrentRotateAngle;
	bool            mRingPressed = false;
	float           mSnapAngleAccumulated = 0.0f;

public:
	// Updates tool
	void Update(float dt);

	// Draws tool
	void DrawScreen();

	// Calls when tool was enabled
	void OnEnabled();

	// Calls when tool was disabled
	void OnDisabled();

	// Calls when scene actors was changed
	void OnSceneChanged(Vector<Actor*> changedActors);

	// Calls when actors selection was changed
	void OnActorsSelectionChanged(Vector<Actor*> actors);

	// Updates ring and angle meshes
	void UpdateMeshes();

	// Calculates rotate pivot in actors center
	void CalcPivotByActorsCenter();

	// Calls when pivot handle moved
	void OnPivotDragHandleMoved(const Vec2F& position);

	// Returns is point inside rotate ring
	bool IsPointInRotateRing(const Vec2F& point) const;

	// Calls when cursor pressed on this
	void OnCursorPressed(const Input::Cursor& cursor);

	// Calls when cursor released (only when cursor pressed this at previous time)
	void OnCursorReleased(const Input::Cursor& cursor);

	// Calls when cursor pressing was broken (when scrolled scroll area or some other)
	void OnCursorPressBreak(const Input::Cursor& cursor);

	// Calls when cursor stay down during frame
	void OnCursorStillDown(const Input::Cursor& cursor);

	// Calls when key was pressed
	void OnKeyPressed(const Input::Key& key);

	// Calls when key stay down during frame
	void OnKeyStayDown(const Input::Key& key);

	// Rotates actors on angle
	void RotateActors(float angleDelta);

	// Rotates actors on angle separated
	void RotateActorsSeparated(float angleDelta);
};