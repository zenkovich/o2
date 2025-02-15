#pragma once

#include "o2/Assets/Types/AnimationStateGraphAsset.h"
#include "o2/Scene/Components/AnimationStateGraphComponent.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2/Utils/Editor/FrameHandles.h"
#include "o2Editor/Core/Actions/ActionsList.h"
#include "o2Editor/Core/Actions/IAction.h"
#include "o2Editor/Core/Properties/Objects/DefaultObjectPropertiesViewer.h"
#include "o2Editor/Core/Properties/Basic/VectorProperty.h"
#include "o2Editor/Core/UI/FrameScrollView.h"

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

        struct StateWidget;

        struct StateTransition : public RefCounterable
        {
            WeakRef<StateWidget> owner;
            WeakRef<StateWidget> destination;

            Ref<DragHandle> dragHandle;

        public:
            void Draw();
        };

		struct StateWidget : public RefCounterable
        {
			enum class TransitionState { None, Finished, Planned };

			WeakRef<AnimationGraphState> state; 

			Vector<Ref<StateAnimation>> animations;
            Vector<Ref<StateTransition>> transitions;

			Ref<VerticalLayout> widget; 
            Ref<VectorProperty> animationsListProperty;

            Ref<WidgetLayer> borderLayer;

			Ref<DragHandle> dragHandle;

			WeakRef<AnimationStateGraphEditor> editor;

        public:
			StateWidget(const Ref<AnimationStateGraphEditor>& owner,
						const Ref<AnimationGraphState>& state);

			void InitializeTransitions();
            void RemoveWidget();
            void DrawTransitions();

            void UpdateState(TransitionState state);
        };

    protected:
        Ref<ContextMenu> mContextMenu; // Context menu for editing keys properties, copying, pasting and other

		WeakRef<AnimationStateGraphAsset>     mGraph;     // Animation state graph asset
		WeakRef<AnimationStateGraphComponent> mComponent; // Animation state graph component

		Vector<Ref<StateWidget>>                            mStatesWidgets;    // States widgets
        Map<WeakRef<AnimationGraphState>, Ref<StateWidget>> mStatesWidgetsMap; // States widgets map by state

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

        // Redraws content into render target
		void RedrawContent() override;

		// Initializes context menu items
		void InitializeContextMenu();

		// Recalculates view area by curves approximated points
		void RecalculateViewArea();

        // Draws handles
        void DrawHandles();

        // Draws selection sprite
        void DrawSelection();

        // Draws states transitions
        void DrawTransitions();

        // Initializes states list
        void InitializeStates();

		// Called when state started, updates states transition animation
		void OnStateGraphStateStarted(const Ref<AnimationGraphState>& state);

		// Called when state finished, updates states transition animation
		void OnStateGraphStateFinished(const Ref<AnimationGraphState>& state);

		// Called when transition started, updates states transition animation
		void OnStateGraphTransitionStarted(const Ref<AnimationGraphTransition>& transition);

		// Called when transition finished, updates states transition animation
		void OnStateGraphTransitionFinished(const Ref<AnimationGraphTransition>& transition);
        
        REF_COUNTERABLE_IMPL(FrameScrollView, SelectableDragHandlesGroup);
	};

	// ------------------------------------
	// AnimationComponent properties viewer
	// ------------------------------------
	class GraphAnimationStateViewer : public DefaultObjectPropertiesViewer
	{
	public:
		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		// Creates spoiler for properties
		Ref<Spoiler> CreateSpoiler(const Ref<Widget>& parent) override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(GraphAnimationStateViewer);

	private:
		Ref<Toggle> mPlayPause;
		Ref<Button> mEditBtn;
		Ref<Toggle> mLooped;

		Ref<HorizontalProgress> mTimeProgress;

		WeakRef<IAnimation> mSubscribedPlayer;

	private:
		// Called when viewer is refreshed
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// ThCalled when the viewer is freed
		void OnFree() override;

		// Called when play pause toggled
		void OnPlayPauseToggled(bool play);

		// Called when loop toggled
		void OnLoopToggled(bool looped);

		// Called when edit button pressed, sets animation editing
		void OnEditPressed();

		// Called when time progress changed by user, sets subscribed player time 
		void OnTimeProgressChanged(float value);

		// Called when animation updates
		void OnAnimationUpdated(float time);
	};
}
// --- META ---

PRE_ENUM_META(Editor::AnimationStateGraphEditor::StateWidget::TransitionState);

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
    FIELD().PROTECTED().NAME(mStatesWidgetsMap);
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
    FUNCTION().PROTECTED().SIGNATURE(void, RedrawContent);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeContextMenu);
    FUNCTION().PROTECTED().SIGNATURE(void, RecalculateViewArea);
    FUNCTION().PROTECTED().SIGNATURE(void, DrawHandles);
    FUNCTION().PROTECTED().SIGNATURE(void, DrawSelection);
    FUNCTION().PROTECTED().SIGNATURE(void, DrawTransitions);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeStates);
    FUNCTION().PROTECTED().SIGNATURE(void, OnStateGraphStateStarted, const Ref<AnimationGraphState>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnStateGraphStateFinished, const Ref<AnimationGraphState>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnStateGraphTransitionStarted, const Ref<AnimationGraphTransition>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnStateGraphTransitionFinished, const Ref<AnimationGraphTransition>&);
}
END_META;

CLASS_BASES_META(Editor::GraphAnimationStateViewer)
{
    BASE_CLASS(Editor::DefaultObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::GraphAnimationStateViewer)
{
    FIELD().PRIVATE().NAME(mPlayPause);
    FIELD().PRIVATE().NAME(mEditBtn);
    FIELD().PRIVATE().NAME(mLooped);
    FIELD().PRIVATE().NAME(mTimeProgress);
    FIELD().PRIVATE().NAME(mSubscribedPlayer);
}
END_META;
CLASS_METHODS_META(Editor::GraphAnimationStateViewer)
{

    typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetViewingObjectType);
    FUNCTION().PUBLIC().SIGNATURE(Ref<Spoiler>, CreateSpoiler, const Ref<Widget>&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetViewingObjectTypeStatic);
    FUNCTION().PRIVATE().SIGNATURE(void, OnRefreshed, _tmp1);
    FUNCTION().PRIVATE().SIGNATURE(void, OnFree);
    FUNCTION().PRIVATE().SIGNATURE(void, OnPlayPauseToggled, bool);
    FUNCTION().PRIVATE().SIGNATURE(void, OnLoopToggled, bool);
    FUNCTION().PRIVATE().SIGNATURE(void, OnEditPressed);
    FUNCTION().PRIVATE().SIGNATURE(void, OnTimeProgressChanged, float);
    FUNCTION().PRIVATE().SIGNATURE(void, OnAnimationUpdated, float);
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
