#pragma once

#include "o2/Assets/Types/AnimationStateGraphAsset.h"
#include "o2/Scene/Components/AnimationStateGraphComponent.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2/Utils/Editor/FrameHandles.h"
#include "o2Editor/Core/Actions/ActionsList.h"
#include "o2Editor/Core/Actions/IAction.h"
#include "o2Editor/Core/UI/FrameScrollView.h"
#include "o2Editor/Core/Properties/Basic/VectorProperty.h"

using namespace o2;

namespace o2
{
    class ContextMenu;
    class EditBox;
    class Window;
}

namespace Editor
{
    // ---------------------
    // Curves editing widget
    // ---------------------
    class AnimationStateGraphEditor : public FrameScrollView, public SelectableDragHandlesGroup
    {
    public:
        Ref<ActionsList> actionsListDelegate; // Actions fall down list. When it is null, editor uses local actions list

    public:
        // Default constructor
        explicit AnimationStateGraphEditor(RefCounter* refCounter);

        // Destructor
        ~AnimationStateGraphEditor();

		// Sets graph and component
		void SetGraph(const Ref<AnimationStateGraphAsset>& graph,
					  const Ref<AnimationStateGraphComponent>& component);

        // Draws widget, updates render target 
        void Draw() override;

        // Updates drawables, states and widget
        void Update(float dt) override;

        // Sets selection rectangle sprite image
        void SetSelectionSpriteImage(const AssetRef<ImageAsset>& image);

        // Updates layout
        void UpdateSelfTransform() override;

        // Returns context menu
        const Ref<ContextMenu>& GetContextMenu() const;

        // Dynamic cast to RefCounterable via Component
        static Ref<RefCounterable> CastToRefCounterable(const Ref<AnimationStateGraphEditor>& ref);

        SERIALIZABLE(AnimationStateGraphEditor);
        CLONEABLE_REF(AnimationStateGraphEditor);

    public:
        struct StateAnimation : public RefCounterable, public IObject
        {
            String name;
			Ref<IAnimationState> state; 
			Ref<AnimationGraphState::Animation> animation;

			IOBJECT(StateAnimation);
        };

		struct StateWidget : public RefCounterable
        {
			WeakRef<AnimationGraphState> state; 

			Vector<Ref<StateAnimation>> animations;

			Ref<VerticalLayout> widget; 
            Ref<VectorProperty> animationsListProperty;

			Ref<DragHandle> dragHandle;

			WeakRef<AnimationStateGraphEditor> editor;

        public:
            StateWidget(const Ref<AnimationStateGraphEditor>& owner);
			void Initialize(const Ref<AnimationGraphState>& state);
            void RemoveWidget();
        };

    protected:
        Ref<ContextMenu> mContextMenu; // Context menu for editing keys properties, copying, pasting and other

		WeakRef<AnimationStateGraphAsset>     mGraph;     // Animation state graph asset
		WeakRef<AnimationStateGraphComponent> mComponent; // Animation state graph component

		Vector<Ref<StateWidget>> mStatesWidgets; // States widgets

        Ref<Sprite> mSelectionSprite;       // Selection sprite @SERIALIZABLE
		Vec2F       mSelectingPressedPoint; // Point, where cursor was pressed, selection starts here, in local space

		Ref<CursorAreaEventListenersLayer> mListenersLayer = mmake<CursorAreaEventListenersLayer>(); // Listeners layer

		bool mNeedAdjustView = false; // True when need to adjust view scale. This works in update

        ActionsList mActionsList; // Local actions list. It uses when actionFallDown is null

    protected:
        // Called when visible was changed. Sets context menu items priority
        void OnEnabled() override;

        // Called when visible was changed. Sets context menu items priority
        void OnDisabled() override;

        // Called when scrolling
		void OnScrolled(float scroll) override;

		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// Called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// Called when right mouse button stay down on this, overriding from scroll view to call context menu
		void OnCursorRightMouseStayDown(const Input::Cursor& cursor) override;

		// Called when right mouse button was released (only when right mouse button pressed this at previous time), overriding from scroll view to call context menu
		void OnCursorRightMouseReleased(const Input::Cursor& cursor) override;

        // Initializes context menu items
        void InitializeContextMenu();

        // Recalculates view area by curves approximated points
        void RecalculateViewArea();

        // Redraws content into render target
        void RedrawContent() override;

        // Draws handles
        void DrawHandles();

        // Draw selection sprite
        void DrawSelection();

        // Initializes states list
        void InitializeStates();
        
        REF_COUNTERABLE_IMPL(FrameScrollView, SelectableDragHandlesGroup);
    };
}
// --- META ---

CLASS_BASES_META(Editor::AnimationStateGraphEditor)
{
    BASE_CLASS(Editor::FrameScrollView);
    BASE_CLASS(o2::SelectableDragHandlesGroup);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationStateGraphEditor)
{
    FIELD().PUBLIC().NAME(actionsListDelegate);
    FIELD().PROTECTED().NAME(mContextMenu);
    FIELD().PROTECTED().NAME(mGraph);
    FIELD().PROTECTED().NAME(mComponent);
    FIELD().PROTECTED().NAME(mStatesWidgets);
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mSelectionSprite);
    FIELD().PROTECTED().NAME(mSelectingPressedPoint);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<CursorAreaEventListenersLayer>()).NAME(mListenersLayer);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mNeedAdjustView);
    FIELD().PROTECTED().NAME(mActionsList);
}
END_META;
CLASS_METHODS_META(Editor::AnimationStateGraphEditor)
{

    FUNCTION().PUBLIC().CONSTRUCTOR(RefCounter*);
    FUNCTION().PUBLIC().SIGNATURE(void, SetGraph, const Ref<AnimationStateGraphAsset>&, const Ref<AnimationStateGraphComponent>&);
    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, SetSelectionSpriteImage, const AssetRef<ImageAsset>&);
    FUNCTION().PUBLIC().SIGNATURE(void, UpdateSelfTransform);
    FUNCTION().PUBLIC().SIGNATURE(const Ref<ContextMenu>&, GetContextMenu);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Ref<RefCounterable>, CastToRefCounterable, const Ref<AnimationStateGraphEditor>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
    FUNCTION().PROTECTED().SIGNATURE(void, OnScrolled, float);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseStayDown, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorRightMouseReleased, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeContextMenu);
    FUNCTION().PROTECTED().SIGNATURE(void, RecalculateViewArea);
    FUNCTION().PROTECTED().SIGNATURE(void, RedrawContent);
    FUNCTION().PROTECTED().SIGNATURE(void, DrawHandles);
    FUNCTION().PROTECTED().SIGNATURE(void, DrawSelection);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeStates);
}
END_META;

CLASS_BASES_META(Editor::AnimationStateGraphEditor::StateAnimation)
{
    BASE_CLASS(o2::RefCounterable);
    BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationStateGraphEditor::StateAnimation)
{
    FIELD().PUBLIC().NAME(name);
    FIELD().PUBLIC().NAME(state);
    FIELD().PUBLIC().NAME(animation);
}
END_META;
CLASS_METHODS_META(Editor::AnimationStateGraphEditor::StateAnimation)
{
}
END_META;
// --- END META ---
