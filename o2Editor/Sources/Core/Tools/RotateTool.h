#pragma once

#include "Core/Tools/SelectionTool.h"
#include "Scene/ActorTransform.h"
#include "SceneWindow/SceneDragHandle.h"

using namespace o2;

namespace o2
{
	class Mesh;
}

namespace Editor
{
	// ------------------
	// Rotate actors tool
	// ------------------
	class RotateTool: public SelectionTool
	{
	public:
		typedef Vector<ActorTransform> ActorsTransformsVec;

	public:
		float angleSnapStep = 15.0f; // Rotation angle step in degree

		// Default constructor
		RotateTool();

		RotateTool& operator=(const RotateTool& other) { return *this; }

		// Destructor
		~RotateTool();

		IOBJECT(RotateTool);

	protected:
		const float         mRotateRingInsideRadius = 60;						   // Rotate ring inside radius in pixels
		const float         mRotateRingOutsideRadius = 100;						   // Rotate ring outside radius in pixels
		const int           mRotateRingSegs = 50;								   // Rotate ring segments
		const Color4        mRotateRingsColor = Color4(220, 220, 220, 255);		   // Rotate ring border color
		const Color4        mRotateRingsFillColor = Color4(220, 220, 220, 50);	   // Rotate ring color 1
		const Color4        mRotateRingsFillColor2 = Color4(220, 220, 220, 100);   // Rotate ring color 2
		const Color4        mRotateMeshClockwiseColor = Color4(211, 87, 40, 100);  // Rotate angle clockwise rotation color
		const Color4        mRotateMeshCClockwiseColor = Color4(87, 211, 40, 100); // Rotate angle counter clockwise rotation color

		Mesh*               mRotateRingFillMesh;		  // Rotate ring mesh
		Mesh*               mAngleMesh;					  // Rotation angle mesh
		Vec2F               mScenePivot;				  // Rotation pivot in scene space

		SceneDragHandle     mPivotDragHandle;			  // Pivot drag handle
		float               mPressAngle;				  // Angle at cursor pressing
		float               mCurrentRotateAngle;		  // Current rotation angle
		bool                mRingPressed = false;		  // Is rotate ring was pressed
		float               mSnapAngleAccumulated = 0.0f; // Snapping angle accumulated
		ActorsTransformsVec mBeforeTransforms;  		  // Array of actors' transformations before changing

	public:
		// Updates tool
		void Update(float dt);

		// Draws tool
		void DrawScreen();

		// It is called when tool was enabled
		void OnEnabled();

		// It is called when tool was disabled
		void OnDisabled();

		// It is called when scene actors was changed
		void OnSceneChanged(Vector<Actor*> changedActors);

		// It is called when actors selection was changed
		void OnActorsSelectionChanged(Vector<Actor*> actors);

		// Updates ring and angle meshes
		void UpdateMeshes();

		// Calculates rotate pivot in actors center
		void CalcPivotByActorsCenter();

		// It is called when pivot handle moved
		void OnPivotDragHandleMoved(const Vec2F& position);

		// Returns is point inside rotate ring
		bool IsPointInRotateRing(const Vec2F& point) const;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// It is called when key stay down during frame
		void OnKeyStayDown(const Input::Key& key);

		// Rotates actors on angle
		void RotateActors(float angleDelta);

		// Rotates actors on angle separated
		void RotateActorsSeparated(float angleDelta);

		// Rotates actors on angle
		void RotateActorsWithAction(float angleDelta);

		// Rotates actors on angle separated
		void RotateActorsSeparatedWithAction(float angleDelta);
	};
}
