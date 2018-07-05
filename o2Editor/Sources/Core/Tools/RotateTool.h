#pragma once

#include "Core/Tools/SelectionTool.h"
#include "SceneWindow/SceneDragHandle.h"
#include "Utils/Editor/SceneEditableObject.h"
#include "Utils/Math/Basis.h"

using namespace o2;

namespace o2
{
	class Mesh;
	class SceneEditableObject;
}

namespace Editor
{
	class TransformAction;

	// -------------------
	// Rotate objects tool
	// -------------------
	class RotateTool: public SelectionTool
	{
	public:
		typedef Vector<Basis> BasisVec;

	public:
		float angleSnapStep = 15.0f; // Rotation angle step in degree

		// Default constructor
		RotateTool();

		RotateTool& operator=(const RotateTool& other) { return *this; }

		// Destructor
		~RotateTool();

		IOBJECT(RotateTool);

	protected:
		const float      mRotateRingInsideRadius = 60;						   // Rotate ring inside radius in pixels
		const float      mRotateRingOutsideRadius = 100;						   // Rotate ring outside radius in pixels
		const int        mRotateRingSegs = 50;								   // Rotate ring segments
		const Color4     mRotateRingsColor = Color4(220, 220, 220, 255);		   // Rotate ring border color
		const Color4     mRotateRingsFillColor = Color4(220, 220, 220, 50);	   // Rotate ring color 1
		const Color4     mRotateRingsFillColor2 = Color4(220, 220, 220, 100);   // Rotate ring color 2
		const Color4     mRotateMeshClockwiseColor = Color4(211, 87, 40, 100);  // Rotate angle clockwise rotation color
		const Color4     mRotateMeshCClockwiseColor = Color4(87, 211, 40, 100); // Rotate angle counter clockwise rotation color
						 
		Mesh*            mRotateRingFillMesh = nullptr; // Rotate ring mesh
		Mesh*            mAngleMesh = nullptr;          // Rotation angle mesh
		Vec2F            mScenePivot;				    // Rotation pivot in scene space
						 							   
		SceneDragHandle  mPivotDragHandle;			    // Pivot drag handle
		float            mPressAngle;				    // Angle at cursor pressing
		float            mCurrentRotateAngle;		    // Current rotation angle
		bool             mRingPressed = false;		    // Is rotate ring was pressed
		float            mSnapAngleAccumulated = 0.0f;  // Snapping angle accumulated
						 
		BasisVec         mBeforeTransforms;  		    // Array of objects' transformations before changing
		TransformAction* mTransformAction = nullptr;    // Current transform action. Creates when transform started

	public:
		// Updates tool
		void Update(float dt);

		// Draws tool
		void DrawScreen();

		// It is called when tool was enabled
		void OnEnabled();

		// It is called when tool was disabled
		void OnDisabled();

		// It is called when scene objects was changed
		void OnSceneChanged(Vector<SceneEditableObject*> changedObjects);

		// It is called when objects selection was changed
		void OnObjectsSelectionChanged(Vector<SceneEditableObject*> objects);

		// Updates ring and angle meshes
		void UpdateMeshes();

		// Calculates rotate pivot in objects center
		void CalcPivotByObjectsCenter();

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

		// Rotates objects on angle
		void RotateObjects(float angleDelta);

		// Rotates objects on angle separated
		void RotateObjectsSeparated(float angleDelta);

		// Rotates objects on angle
		void RotateObjectsWithAction(float angleDelta);

		// Rotates objects on angle separated
		void RotateObjectsSeparatedWithAction(float angleDelta);
	};
}

CLASS_BASES_META(Editor::RotateTool)
{
	BASE_CLASS(Editor::SelectionTool);
}
END_META;
CLASS_FIELDS_META(Editor::RotateTool)
{
	PUBLIC_FIELD(angleSnapStep);
	PROTECTED_FIELD(mRotateRingInsideRadius);
	PROTECTED_FIELD(mRotateRingOutsideRadius);
	PROTECTED_FIELD(mRotateRingSegs);
	PROTECTED_FIELD(mRotateRingsColor);
	PROTECTED_FIELD(mRotateRingsFillColor);
	PROTECTED_FIELD(mRotateRingsFillColor2);
	PROTECTED_FIELD(mRotateMeshClockwiseColor);
	PROTECTED_FIELD(mRotateMeshCClockwiseColor);
	PROTECTED_FIELD(mRotateRingFillMesh);
	PROTECTED_FIELD(mAngleMesh);
	PROTECTED_FIELD(mScenePivot);
	PROTECTED_FIELD(mPivotDragHandle);
	PROTECTED_FIELD(mPressAngle);
	PROTECTED_FIELD(mCurrentRotateAngle);
	PROTECTED_FIELD(mRingPressed);
	PROTECTED_FIELD(mSnapAngleAccumulated);
	PROTECTED_FIELD(mBeforeTransforms);
	PROTECTED_FIELD(mTransformAction);
}
END_META;
CLASS_METHODS_META(Editor::RotateTool)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, DrawScreen);
	PUBLIC_FUNCTION(void, OnEnabled);
	PUBLIC_FUNCTION(void, OnDisabled);
	PUBLIC_FUNCTION(void, OnSceneChanged, Vector<SceneEditableObject*>);
	PUBLIC_FUNCTION(void, OnObjectsSelectionChanged, Vector<SceneEditableObject*>);
	PUBLIC_FUNCTION(void, UpdateMeshes);
	PUBLIC_FUNCTION(void, CalcPivotByObjectsCenter);
	PUBLIC_FUNCTION(void, OnPivotDragHandleMoved, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsPointInRotateRing, const Vec2F&);
	PUBLIC_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PUBLIC_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PUBLIC_FUNCTION(void, OnKeyStayDown, const Input::Key&);
	PUBLIC_FUNCTION(void, RotateObjects, float);
	PUBLIC_FUNCTION(void, RotateObjectsSeparated, float);
	PUBLIC_FUNCTION(void, RotateObjectsWithAction, float);
	PUBLIC_FUNCTION(void, RotateObjectsSeparatedWithAction, float);
}
END_META;
