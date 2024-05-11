#pragma once
#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2/Utils/Editor/FrameHandles.h"
#include "o2Editor/Core/Tools/IEditorTool.h"

using namespace o2;

namespace Editor
{
	// -----------------------------------------------------------------------------------
	// Unified spline editor. Works with spline wrappers, that represents data from spline
	// -----------------------------------------------------------------------------------
	class SplineEditor : public SelectableDragHandlesGroup, public CursorAreaEventsListener, public KeyboardEventsListener
	{
	public:
		// ---------------------------------------------------
		// Spline wrapper. Wraps getters and setters of spline
		// ---------------------------------------------------
		struct ISplineWrapper : public RefCounterable
		{
			// Virtual destructor
			virtual ~ISplineWrapper() {}

			// Converts world point to local space
			virtual Vec2F WorldToLocal(const Vec2F& point) const;

			// Converts local point to world space
			virtual Vec2F LocalToWorld(const Vec2F& point) const;

			// Returns count of points in spline
			virtual int GetPointsCount() const = 0;

			// Returns true if spline is closed
			virtual bool IsClosed() const = 0;

			// Adds new point to spline
			virtual void AddPoint(int idx, const Vec2F& position, const Vec2F& prevSupport, const Vec2F& nextSupport) = 0;

			// Removes point from spline
			virtual void RemovePoint(int idx) = 0;

			// Returns position of point
			virtual Vec2F GetPointPos(int idx) const = 0;

			// Sets position of point
			virtual void SetPointPos(int idx, const Vec2F& pos) = 0;

			// Returns previous support position of point
			virtual Vec2F GetPointPrevSupportPos(int idx) const = 0;

			// Sets previous support position of point
			virtual void SetPointPrevSupportPos(int idx, const Vec2F& pos) = 0;

			// Returns next support position of point
			virtual Vec2F GetPointNextSupportPos(int idx) const = 0;

			// Sets next support position of point
			virtual void SetPointNextSupportPos(int idx, const Vec2F& pos) = 0;

			// Returns list of points for drawing
			virtual Vector<Vec2F> GetDrawPoints() const;

			// Returns pointer to approximation array
			virtual const ApproximationVec2F* GetPointApproximation(int idx) const = 0;

			// Returns count of approximation points
			virtual int GetPointApproximationCount(int idx) const = 0;

			// Called when spline was changed
			virtual void OnChanged() {}
		};

	public:
		Function<void()> onChanged; // Called when spline was changed

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
		void SetSpline(const Ref<ISplineWrapper>& wrapper);

		// resets callbacks and spline to null
		void Reset();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns true when input events can be handled by down listeners
		bool IsInputTransparent() const override;

		// Called when spline was updated and need to refresh handles
		void OnSplineChanged();

	protected:
		// -----------------------
		// Handles of spline point
		// -----------------------
		struct PointHandles : public RefCounterable
		{
			DragHandle position;    // Position handle
			DragHandle prevSupport; // Previous support handle
			DragHandle nextSupport; // Next support handle

			bool positionDragged = false;    // Is position handle dragged
			bool prevSupportDragged = false; // Is previous support handle dragged 
			bool nextSupportDragged = false; // Is next support handle dragged

			bool startDragFromZero = false; // Is dragging started from zero position

			bool isLast = false;  // Is last point in spline
			bool isFirst = false; // Is first point in spline

		public:
			// Returns is support handles visible
			bool IsSupportsVisible() const;
		};

	protected:
		Ref<ISplineWrapper> mSplineWrapper; // Spline getters and setters wrapper

		PointHandles              mHandlesSample; // Samples of handles for cloning
		Vector<Ref<PointHandles>> mSplineHandles; // Spline handles

		Color4 mSplineColor;        // Color of spline line
		Color4 mSplineSupportColor; // Color of support handles lines

		Ref<Sprite> mSelectionSprite;       // Selection sprite @SERIALIZABLE
		Vec2F       mSelectingPressedPoint; // Point, where cursor was pressed, selection starts here, in local space

		Vector<Ref<DragHandle>> mSelectingHandlesBuf; // Potentially selecting handles while selecting

		FrameHandles mTransformFrame;                      // Keys transformation frame
		bool         mTransformFrameVisible = false;       // Is transform frame visible. it visible when 2 or more main handles was selected
		Basis        mTransformFrameBasis;                 // Basis of transform frame in screen space
		Vec2F        mTransformBasisOffet = Vec2F(10, 10); // Border between side points and frame

		Ref<SelectableDragHandlesGroup> mSupportHandlesGroup = mmake<SelectableDragHandlesGroup>(); // Support points handles selection group. They are must be selectable separately from main handles

	protected:
		// Removes all handles
		void ClearHandles();

		// Initializes all spline handles
		void InitializeHandles();

		// Checks that support handles are in same position for dragging symmteric
		void CheckDragFromZero(int i, const Ref<PointHandles>& handles);

		// Draws handles
		void DrawHandles();

		// Draw selection sprite
		void DrawSelection();

		// Draws transformation frame
		void DrawTransformFrame();

		// Draws all main handles
		void DrawMainHandles();

		// Draws only support handles
		void DrawSupportHandles();

		// Updates transformation frame by selected handles
		void UpdateTransformFrame();

		// Returns is transform frame visible. it will be visible when 2 or more main handles was selected
		bool IsTransformFrameVisible() const;

		// Called when transform frame was transformed
		void OnTransformFrameTransformed(const Basis& basis);

		// Called when beginning transforming some handles. Stores selected keys before changing
		void OnTransformBegin();

		// Called when transform completed. Checks that selected keys changed, creates action for undo/redo
		void OnTransformCompleted();

		// Called when previous support handle moved. Updates position of support point, checks symmetric
		void OnPrevHandleMoved(int i, const Vec2F& pos, const Ref<PointHandles>& handles);

		// Called when next support handle moved. Updates position of support point, checks symmetric
		void OnNextHandleMoved(int i, const Vec2F& pos, const Ref<PointHandles>& handles);

		// Called when main handle moved. Updates support handles positions also
		void OnMainHandleMoved(int i, const Vec2F& pos, const Ref<PointHandles>& handles);

		// Called when previous support handle was released without dragging, just clicking; removes support handle
		void OnPrevHandleReleasedNoDrag(int i, const Ref<PointHandles>& handles);

		// Called when next support handle was released without dragging, just clicking; removes support handle
		void OnNextHandleReleasedNoDrag(int i, const Ref<PointHandles>& handles);

		// Called when position handle was released without dragging, just clicking; removes both support handles
		void OnMainHandleReleasedNoDrag(int i, const Ref<PointHandles>& handles);

		// Called when cursor double clicked; creates new point
		void OnCursorDblClicked(const Input::Cursor& cursor) override;

		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// Called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// Called when selection is changed - some handle was added or removed from selection; checks visible handles
		void OnSelectionChanged() override;

		// Called when key was released
		void OnKeyReleased(const Input::Key& key) override;

        REF_COUNTERABLE_IMPL(SelectableDragHandlesGroup);
	};
}
