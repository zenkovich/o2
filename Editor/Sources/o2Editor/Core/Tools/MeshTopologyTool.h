#pragma once
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/Components/MeshComponent.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2/Utils/Editor/FrameHandles.h"
#include "o2Editor/Core/Tools/IEditorTool.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"

namespace Editor
{
    // ---------------------------------------------------------
    // Mesh component topology tool. Adds extra points into mesh-
    // --------------------------------------------------------
    struct MeshTopologyTool : public IEditTool, public SelectableDragHandlesGroup, public CursorAreaEventsListener, public KeyboardEventsListener
    {
        struct SceneLayer : public SceneEditorLayer
        {
            MeshTopologyTool* tool = nullptr;

        public:
            void DrawOverScene() override;
            void Update(float dt) override;

            int GetOrder() const override;

            bool IsEnabled() const override;

            const String& GetName() const override;
            const String& GetIconName() const override;
        };

        Ref<SceneLayer> sceneLayer;        // Scene layer for drawing spline
        bool isEnabled = false; // Is tool enabled now

        Function<void()> onChanged; // Called when frame changes     

    public:
        // Default constructor
        MeshTopologyTool();

        // Destructor
        ~MeshTopologyTool();

        // Empty copy operator
        MeshTopologyTool& operator=(const MeshTopologyTool& other) { return *this; }

        // Setup editing callbacks
        void Setup(const Function<Vector<Vec2F>> & getPoints, const Function<void(int, Vec2F)> & setPoint,
            const Function<Basis()> & getTransform, const Function<void(Vec2F)> & addPoint,
            const Function<void(int)> & removePoint);

        // Resets callbacks
        void Reset();

        // Returns toggle in menu panel icon name
        String GetPanelIcon() const override;

        // Called when tool was enabled
        void OnEnabled() override;

        // Called when tool was disabled
        void OnDisabled() override;

        // Returns true if point is in this object
        bool IsUnderPoint(const Vec2F & point) override;

        // Returns true when input events can be handled by down listeners
        bool IsInputTransparent() const override;

        IOBJECT(MeshTopologyTool);

    private:
        Function<Vector<Vec2F>> mGetPoints;   // Gets editing points list
        Function<void(int, Vec2F)> mSetPoint;    // Sets editing point position by index
        Function<void(Vec2F)> mAddPoint;    // Add new point
        Function<void(int)> mRemovePoint; // Removes point by index

        Function<Basis()> mGetTransform; // Returns local space transformation

        DragHandle mHandleSample; // Point handle sample
        Vector<Ref<DragHandle>> mHandles;      // List of all handles

        Sprite mSelectionSprite;       // Selection sprite
        Vec2F mSelectingPressedPoint; // Point, where cursor was pressed, selection starts here, in local space

        Vector<Ref<DragHandle>> mSelectingHandlesBuf; // Potentially selecting handles while selecting

        FrameHandles mTransformFrame;                      // Keys transformation frame
        bool mTransformFrameVisible = false;       // Is transform frame visible. it visible when 2 or more main handles was selected
        Basis mTransformFrameBasis;                 // Basis of transform frame in screen space
        Vec2F mTransformBasisOffet = Vec2F(10, 10); // Border between side points and frame

    private:
        void InitializeHandles();
        void ClearHandles();

        void OnHandleMoved(int i, const Vec2F& pos);

        Vec2F WorldToLocal(const Vec2F& point) const;
        Vec2F LocalToWorld(const Vec2F& point) const;

        // Draw selection sprite
        void DrawSelection();

        // Draws transformation frame
        void DrawTransformFrame();

        // Updates transformation frame by selected handles
        void UpdateTransformFrame();

        // Returns is transform frame visible. it will be visible when 2 or more main handles was selected
        bool IsTransformFrameVisible() const;

        // Called when transform frame was transformed
        void OnTransformFrameTransformed(const Basis& basis);

        // Called when cursor pressed on this
        void OnCursorPressed(const Input::Cursor& cursor) override;

        // Called when cursor released (only when cursor pressed this at previous time)
        void OnCursorReleased(const Input::Cursor& cursor) override;

        // Called when cursor stay down during frame
        void OnCursorStillDown(const Input::Cursor& cursor) override;

        // Called when cursor double clicked; creates new point
        void OnCursorDblClicked(const Input::Cursor& cursor) override;
    };
}
// --- META ---

CLASS_BASES_META(Editor::MeshTopologyTool)
{
    BASE_CLASS(Editor::IEditTool);
    BASE_CLASS(o2::SelectableDragHandlesGroup);
    BASE_CLASS(o2::CursorAreaEventsListener);
    BASE_CLASS(o2::KeyboardEventsListener);
}
END_META;
CLASS_FIELDS_META(Editor::MeshTopologyTool)
{
    FIELD().PUBLIC().NAME(sceneLayer);
    FIELD().PUBLIC().DEFAULT_VALUE(false).NAME(isEnabled);
    FIELD().PUBLIC().NAME(onChanged);
    FIELD().PRIVATE().NAME(mGetPoints);
    FIELD().PRIVATE().NAME(mSetPoint);
    FIELD().PRIVATE().NAME(mAddPoint);
    FIEreplace raw pointers with Ref<>, except void* pointers.Ref<> is a smart pointer like shared_ptr. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Dont replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

LD().PRIVATE().NAME(mRemovePoint);
FIELD().PRIVATE().NAME(mGetTransform);
FIELD().PRIVATE().NAME(mHandleSample);
FIELD().PRIVATE().NAME(mHandles);
FIELD().PRIVATE().NAME(mSelectionSprite);
FIELD().PRIVATE().NAME(mSelectingPressedPoint);
FIELD().PRIVATE().NAME(mSelectingHandlesBuf);
FIELD().PRIVATE().NAME(mTransformFrame);
FIELD().PRIVATE().DEFAULT_VALUE(false).NAME(mTransformFrameVisible);
FIELD().PRIVATE().NAME(mTransformFrameBasis);
FIELD().PRIVATE().DEFAULT_VALUE(Vec2F(10, 10)).NAME(mTransformBasisOffet);
}
END_META;
CLASS_METHODS_META(Editor::MeshTopologyTool)
{

    typedef const Function<void(int, Vec2F)>& _tmp1;

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, Setup, const Function<Vector<Vec2F>()>&, const Ref<_tmp1>&, const Function<Basis()>&, const Ref<const Function<void(Vec2F)>>&, const Ref<const Function<void(int)>>&);
    FUNCTION().PUBLIC().SIGNATURE(void, Reset);
    FUNCTION().PUBLIC().SIGNATURE(String, GetPanelIcon);
    FUNCTION().PUBLIC().SIGNATURE(void, OnEnabled);
    FUNCTION().PUBLIC().SIGNATURE(void, OnDisabled);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsInputTransparent);
    FUNCTION().PRIVATE().SIGNATURE(void, InitializeHandles);
    FUNCTION().PRIVATE().SIGNATURE(void, ClearHandles);
    FUNCTION().PRIVATE().SIGNATURE(void, OnHandleMoved, int, const Vec2F&);
    FUNCTION().PRIVATE().SIGNATURE(Vec2F, WorldToLocal, const Vec2F&);
    FUNCTION().PRIVATE().SIGNATURE(Vec2F, LocalToWorld, const Vec2F&);
    FUNCTION().PRIVATE().SIGNATURE(void, DrawSelection);
    FUNCTION().PRIVATE().SIGNATURE(void, DrawTransformFrame);
    FUNCTION().PRIVATE().SIGNATURE(void, UpdateTransformFrame);
    FUNCTION().PRIVATE().SIGNATURE(bool, IsTransformFrameVisible);
    FUNCTION().PRIVATE().SIGNATURE(void, OnTransformFrameTransformed, const Basis&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorDblClicked, const Input::Cursor&);
}
END_META;
// --- END META ---