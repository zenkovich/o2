#pragma once
#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2/Utils/Editor/FrameHandles.h"
#include "o2Editor/Core/Tools/IEditorTool.h"
#include "o2/Utils/SmartPointers.h"

using namespace o2;

namespace Editor
{
    // -----------------------------------------------------------------------------------
    // Unified spline editor. Works with spline wrappers, that represents data from spline
    // -----------------------------------------------------------------------------------
    class SplineEditor: public SelectableDragHandlesGroup, public CursorAreaEventsListener, public KeyboardEventsListener
    {
    public:
        // ---------------------------------------------------
        // Spline wrapper. Wraps getters and setters of spline
        // ---------------------------------------------------
        struct ISplineWrapper
        {
            virtual ~ISplineWrapper() {}
            
            virtual Vec2F WorldToLocal(const Vec2F& point) const;
            virtual Vec2F LocalToWorld(const Vec2F& point) const;

            virtual int GetPointsCount() const = 0;

            virtual bool IsClosed() const = 0;

            virtual void AddPoint(int idx, const Vec2F& position, const Vec2F& prevSupport, const Vec2F& nextSupport) = 0;
            virtual void RemovePoint(int idx) = 0;

            virtual Vec2F GetPointPos(int idx) const = 0;
            virtual void SetPointPos(int idx, const Vec2F& pos) = 0;

            virtual Vec2F GetPointPrevSupportPos(int idx) const = 0;
            virtual void SetPointPrevSupportPos(int idx, const Vec2F& pos) = 0;

            virtual Vec2F GetPointNextSupportPos(int idx) const = 0;
            virtual void SetPointNextSupportPos(int idx, const Vec2F& pos) = 0;

            virtual Vector<Vec2F> GetDrawPoints() const;

            virtual const ApproximationVec2F* GetPointApproximation(int idx) const = 0;
            virtual int GetPointApproximationCount(int idx) const = 0;

            virtual void OnChanged() {}
        };

    public:
        Function<void()> onChanged;

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
        struct PointHandles
        {
            DragHandle position;
            DragHandle prevSupport;
            DragHandle nextSupport;

            bool positionDragged = false;
            bool prevSupportDragged = false;
            bool nextSupportDragged = false;

            bool startDragFromZero = false;

            bool isLast = false;
            bool isFirst = false;

            bool IsSupportsVisible() const;
        };

    protected:
        Ref<ISplineWrapper> mSplineWrapper; // Spline getters and setters wrapper

        PointHandles mHandlesSample; // Samples of handles for cloning

        Vector<Ref<PointHandles>> mSplineHandles;    // Spline handles
        Color4 mSplineColor;        // Color of spline line
        Color4 mSplineSupportColor; // Color of support handles lines
        
        Sprite* mSelectionSprite = nullptr; // Selection sprite @SERIALIZABLE
        Vec2F   mSelectingPressedPoint;     // Point, where cursor was pressed, selection starts here, in local space

        Vector<Ref<DragHandle>> mSelectingHandlesBuf; // Potentially selecting handles while selecting

        FrameHandles mTransformFrame;                      // Keys transformation frame
        bool         mTransformFrameVisible = false;       // Is transform frame visible. it visible when 2 or more main handles was selected
        Basis        mTransformFrameBasis;                 // Basis of transform frame in screen space
        Vec2F        mTransformBasisOffet = Vec2F(10, 10); // Border between side points and frame

        SelectableDragHandlesGroup mSupportHandlesGroup; // Support points handles selection group. They are must be selectable separately from main handles

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

        // Called when beginning transforming some handles. Stores selected keys before 
    };
}void OnTransformBegin();

void OnTransformCompleted();

void OnPrevHandleMoved(int i, const Vec2F& pos, const Ref<PointHandles>& handles);

void OnNextHandleMoved(int i, const Vec2F& pos, const Ref<PointHandles>& handles);

void OnMainHandleMoved(int i, const Vec2F& pos, const Ref<PointHandles>& handles);

void OnPrevHandleReleasedNoDrag(int i, const Ref<PointHandles>& handles);

void OnNextHandleReleasedNoDrag(int i, const Ref<PointHandles>& handles);

void OnMainHandleReleasedNoDrag(int i, const Ref<PointHandles>& handles);

void OnCursorDblClicked(const Input::Cursor& cursor) override;

void OnCursorPressed(const Input::Cursor& cursor) override;

void OnCursorReleased(const Input::Cursor& cursor) override;

void OnCursorStillDown(const Input::Cursor& cursor) override;

void OnSelectionChanged() override;

void OnKeyReleased(const Input::Key& key) override;
};
}