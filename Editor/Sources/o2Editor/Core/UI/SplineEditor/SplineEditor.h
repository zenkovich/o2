#pragma once
#include "o2/Events/CursorEventsArea.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2/Utils/Editor/FrameHandles.h"
#include "../../Tools/IEditorTool.h"

using namespace o2;

namespace Editor
{
	// -----------------------------------------------------------------------------------
	// Unified spline editor. Works with spline wrappers, that represents data from spline
	// -----------------------------------------------------------------------------------
	class SplineEditor: public SelectableDragHandlesGroup
	{
	public:
		// ---------------------------------------------------
		// Spline wrapper. Wraps getters and setters of spline
		// ---------------------------------------------------
		struct ISplineWrapper
		{
			virtual Vec2F WorldToLocal(const Vec2F& point) const;
			virtual Vec2F LocalToWorld(const Vec2F& point) const;

			virtual int GetPointsCount() const = 0;

			virtual void AddPoint(int idx, const Vec2F& position, const Vec2F& prevSupport, const Vec2F& nextSupport) = 0;
			virtual void RemovePoint(int idx) = 0;

			virtual Vec2F GetPointPos(int idx) const = 0;
			virtual void SetPointPos(int idx, const Vec2F& pos) = 0;

			virtual Vec2F GetPointPrevSupportPos(int idx) const = 0;
			virtual void SetPointPrevSupportPos(int idx, const Vec2F& pos) = 0;

			virtual Vec2F GetPointNextSupportPos(int idx) const = 0;
			virtual void SetPointNextSupportPos(int idx, const Vec2F& pos) = 0;

			virtual Vector<Vec2F> GetDrawPoints() const;
		};

	public:
		// Default constructor
		SplineEditor();

		// Destructor
		~SplineEditor();

		// Draws spline handles and spline itself
		void Draw();

		// Updates handles
		void Update(float dt);

		// Sets spline wrapper
		void SetSpline(ISplineWrapper* wrapper);

		// Sets background input transparency
		void SetInputTransparent(bool transparent);

		// It is called when spline was updated and need to refresh handles
		void OnSplineChanged();

	protected:
		struct PointHandles
		{
			DragHandle position;
			DragHandle prevSupport;
			DragHandle nextSupport;

			bool dragSymmetric = false;
		};

	protected:

		ISplineWrapper* mSplineWrapper = nullptr; // Spline getters and setters wrapper

		PointHandles mHandlesSample; // Samples of handles for cloning

		Vector<PointHandles*> mSplineHandles;    // Spline handles
		CursorEventsArea      mBackgroundEvents; // Events area for catching events in background

		Color4 mSplineColor;        // Color of spline line
		Color4 mSplineSupportColor; // Color of support handles lines
		
		Sprite* mSelectionSprite = nullptr; // Selection sprite @SERIALIZABLE
		Vec2F   mSelectingPressedPoint;     // Point, where cursor was pressed, selection starts here, in local space

		FrameHandles mTransformFrame;                // Keys transformation frame
		bool         mTransformFrameVisible = false; // Is transform frame visible. it visible when 2 or more main handles was selected
		Basis        mTransformFrameBasis;           // Basis of transform frame in screen space

		SelectableDragHandlesGroup mSupportHandlesGroup; // Support points handles selection group. They are must be selectable separately from main handles

	protected:
		// Removes all handles
		void ClearHandles();

		// Initializes all spline handles
		void InitializeHandles();

		// Checks that support handles are symmetric when starting dragging one of them
		void CheckDragSymmetric(int i, PointHandles* handles);

		// It is called when previous support handle moved. Updates position of support point, checks symmetric
		void OnPrevHandleMoved(int i, const Vec2F& pos, PointHandles* handles);

		// It is called when next support handle moved. Updates position of support point, checks symmetric
		void OnNextHandleMoved(int i, const Vec2F& pos, PointHandles* handles);

		// It is called when main handle moved. Updates support handles positions alsu
		void OnMainHandleMoved(int i, const Vec2F& pos, PointHandles* handles);

		// It is called when background double clicked
		void OnBackgroundDblClicked(const Vec2F& point);

		// Draws all main handles
		void DrawMainHandles();

		// Draws only support handles
		void DrawSupportHandles();
	};
}
