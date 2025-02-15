#include "o2Editor/stdafx.h"
#include "AnimationStateGraphEditor.h"

#include "o2/Application/Application.h"
#include "o2/Render/Render.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/Widgets/HorizontalScrollBar.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Scene/UI/Widgets/VerticalScrollBar.h"
#include "o2/Scene/UI/Widgets/Window.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2/Utils/Math/Interpolation.h"
#include "o2/Utils/System/Clipboard.h"
#include "o2Editor/Core/Dialogs/KeyEditDlg.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/Core/UI/CurveEditor/CurveActions.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "../AnimationWindow/AnimationWindow.h"

namespace Editor
{
    AnimationStateGraphEditor::AnimationStateGraphEditor(RefCounter* refCounter):
        FrameScrollView(refCounter), SelectableDragHandlesGroup(refCounter)
    {
        mReady = false;

        mSelectionSprite = mmake<Sprite>();
        InitializeContextMenu();

        mBackColor = Color4(225, 232, 232, 255);
        mViewCameraMinScale = 1.0f;
        mReady = true;
    }

    AnimationStateGraphEditor::~AnimationStateGraphEditor()
    {}

	void AnimationStateGraphEditor::SetGraph(const Ref<AnimationStateGraphAsset>& graph, 
                                             const Ref<AnimationStateGraphComponent>& component)
	{
		mGraph = graph;
		mComponent = component;

		InitializeStates();

		if (mComponent)
			mComponent.Lock()->Reset();
	}

	void AnimationStateGraphEditor::InitializeStates()
	{
		for (auto& state : mStatesWidgets)
			state->RemoveWidget();

		mStatesWidgets.Clear();
		mStatesWidgetsMap.Clear();

		if (auto lastComponent = mComponent.Lock())
		{
			lastComponent->onStateStarted -= THIS_FUNC(OnStateGraphStateStarted);
			lastComponent->onStateFinished -= THIS_FUNC(OnStateGraphStateFinished);
			lastComponent->onTransitionStarted -= THIS_FUNC(OnStateGraphTransitionStarted);
			lastComponent->onTransitionFinished -= THIS_FUNC(OnStateGraphTransitionFinished);
		}

		if (!mGraph)
			return;

		auto graph = mGraph.Lock();
		for (auto& state : graph->GetStates())
		{
			auto stateWidget = mmake<StateWidget>(Ref(this), state);
			mStatesWidgets.Add(stateWidget);
			mStatesWidgetsMap[state] = stateWidget;
		}

		for (auto& state : mStatesWidgets)
			state->InitializeTransitions();

		if (auto component = mComponent.Lock())
		{
			component->onStateStarted += THIS_FUNC(OnStateGraphStateStarted);
			component->onStateFinished += THIS_FUNC(OnStateGraphStateFinished);
			component->onTransitionStarted += THIS_FUNC(OnStateGraphTransitionStarted);
			component->onTransitionFinished += THIS_FUNC(OnStateGraphTransitionFinished);
		}
	}

	void AnimationStateGraphEditor::Draw()
	{
		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		DrawLayers();

		Widget::OnDrawn();

		DrawInternalChildren();
		DrawTopLayers();

		if (!mReady)
			return;

		RedrawRenderTarget();
		mRenderTargetSprite->Draw();

		mListenersLayer->OnDrawn(mRenderTargetSprite->GetBasis());

		CursorAreaEventsListener::OnDrawn();

		mHorScrollbar->Draw();
		mVerScrollbar->Draw();

        o2Render.EnableScissorTest(layout->GetWorldRect());

        DrawHandles();
        DrawSelection();

		o2Render.DisableScissorTest();

		DrawDebugFrame();
	}

	void AnimationStateGraphEditor::RedrawContent()
	{
		mListenersLayer->OnBeginDraw();
		mListenersLayer->camera = o2Render.GetCamera();

		DrawGrid();

		DrawTransitions();
		DrawInheritedDepthChildren();

		mListenersLayer->OnEndDraw();
	}

	void AnimationStateGraphEditor::DrawHandles()
	{
	}

	void AnimationStateGraphEditor::DrawSelection()
	{
		if (mIsPressed)
		{
			mSelectionSprite->rect = RectF(LocalToScreenPoint(mSelectingPressedPoint), o2Input.cursorPos);
			mSelectionSprite->Draw();
		}
	}

	void AnimationStateGraphEditor::DrawTransitions()
	{
		for (auto& state : mStatesWidgets)
			state->DrawTransitions();
	}

    void AnimationStateGraphEditor::Update(float dt)
    {
        FrameScrollView::Update(dt);

        if (mReady && mResEnabledInHierarchy && !mIsClipped && mNeedAdjustView)
        {
            mNeedAdjustView = false;
            mViewCameraTargetScale = mAvailableArea.Size()/mViewCamera.GetSize();
            mViewCamera.center = mAvailableArea.Center();
            mViewCameraTargetPos = mViewCamera.position;
        }

		if (mComponent)
			mComponent.Lock()->GetActor()->Update(dt);
    }

    void AnimationStateGraphEditor::UpdateSelfTransform()
    {
        FrameScrollView::UpdateSelfTransform();

        UpdateLocalScreenTransforms();
        OnCameraTransformChanged();
    }

    const Ref<ContextMenu>& AnimationStateGraphEditor::GetContextMenu() const
    {
        return mContextMenu;
    }

    Ref<RefCounterable> AnimationStateGraphEditor::CastToRefCounterable(const Ref<AnimationStateGraphEditor>& ref)
    {
        return DynamicCast<FrameScrollView>(ref);
    }

    void AnimationStateGraphEditor::SetSelectionSpriteImage(const AssetRef<ImageAsset>& image)
    {
        mSelectionSprite->LoadFromImage(image);
    }

    void AnimationStateGraphEditor::OnEnabled()
    {
        FrameScrollView::OnEnabled();

        mContextMenu->SetItemsMaxPriority();
    }

    void AnimationStateGraphEditor::OnDisabled()
    {
        FrameScrollView::OnDisabled();

        mContextMenu->SetItemsMinPriority();
    }

    void AnimationStateGraphEditor::OnScrolled(float scroll)
    {
        Vec2F newScale = mViewCameraTargetScale;

        if (o2Input.IsKeyDown(VK_CONTROL))
            newScale.x *= 1.0f - (scroll*mViewCameraScaleSence);
        else if (o2Input.IsKeyDown(VK_SHIFT))
            newScale.y *= 1.0f - (scroll*mViewCameraScaleSence);
        else
            newScale *= 1.0f - (scroll*mViewCameraScaleSence);

        ChangeCameraScaleRelativeToCursor(newScale);
    }

	void AnimationStateGraphEditor::OnCursorPressed(const Input::Cursor& cursor)
	{
		Focus();

		mSelectingPressedPoint = cursor.position;
	}

	void AnimationStateGraphEditor::OnCursorReleased(const Input::Cursor& cursor)
	{

	}

	void AnimationStateGraphEditor::OnCursorStillDown(const Input::Cursor& cursor)
	{

	}

	void AnimationStateGraphEditor::OnCursorRightMouseStayDown(const Input::Cursor& cursor)
	{
		FrameScrollView::OnCursorRightMouseStayDown(cursor);
	}

	void AnimationStateGraphEditor::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		FrameScrollView::OnCursorRightMouseReleased(cursor);
	}

	void AnimationStateGraphEditor::InitializeContextMenu()
    {
        mContextMenu = o2UI.CreateWidget<ContextMenu>();

        onShow = [&]() { mContextMenu->SetItemsMaxPriority(); };
        onHide = [&]() { mContextMenu->SetItemsMinPriority(); };

        AddChild(mContextMenu);
    }

    void AnimationStateGraphEditor::RecalculateViewArea()
    {
        //mAvailableArea = RectF(firstPoint, firstPoint);

//         for (auto& curve : mCurves)
//         {
//             for (auto& p : curve->approximatedPoints)
//             {
//                 mAvailableArea.left = Math::Min(mAvailableArea.left, p.first.x);
//                 mAvailableArea.right = Math::Max(mAvailableArea.right, p.first.x);
//                 mAvailableArea.top = Math::Max(mAvailableArea.top, p.first.y);
//                 mAvailableArea.bottom = Math::Min(mAvailableArea.bottom, p.first.y);
// 
//                 mAvailableArea.left = Math::Min(mAvailableArea.left, p.second.x);
//                 mAvailableArea.right = Math::Max(mAvailableArea.right, p.second.x);
//                 mAvailableArea.top = Math::Max(mAvailableArea.top, p.second.y);
//                 mAvailableArea.bottom = Math::Min(mAvailableArea.bottom, p.second.y);
//             }
//         }

        float bordersCoef = 1.5f;
        Vec2F size = mAvailableArea.Size();
        mAvailableArea.left -= size.x*bordersCoef;
        mAvailableArea.right += size.x*bordersCoef;
        mAvailableArea.top += size.y*bordersCoef;
        mAvailableArea.bottom -= size.y*bordersCoef;

        mHorScrollbar->SetValueRange(mAvailableArea.left, mAvailableArea.right);
        mVerScrollbar->SetValueRange(mAvailableArea.bottom, mAvailableArea.top);
    }

	void AnimationStateGraphEditor::OnStateGraphStateStarted(const Ref<AnimationGraphState>& state)
	{
		Ref<StateWidget> widget;
		if (mStatesWidgetsMap.TryGetValue(state, widget))
			widget->UpdateState(StateWidget::TransitionState::Planned);
	}

	void AnimationStateGraphEditor::OnStateGraphStateFinished(const Ref<AnimationGraphState>& state)
	{
		Ref<StateWidget> widget;
		if (mStatesWidgetsMap.TryGetValue(state, widget))
			widget->UpdateState(StateWidget::TransitionState::Finished);
	}

	void AnimationStateGraphEditor::OnStateGraphTransitionStarted(const Ref<AnimationGraphTransition>& transition)
	{


	}

	void AnimationStateGraphEditor::OnStateGraphTransitionFinished(const Ref<AnimationGraphTransition>& transition)
	{
		Ref<StateWidget> widget;
		if (mStatesWidgetsMap.TryGetValue(transition->GetDestinationState(), widget))
			widget->UpdateState(StateWidget::TransitionState::None);
	}

	AnimationStateGraphEditor::StateWidget::StateWidget(const Ref<AnimationStateGraphEditor>& owner,
														const Ref<AnimationGraphState>& state)
	{
		editor = owner;
		this->state = state;

        widget = o2UI.CreateWidget<VerticalLayout>("ASG state");
		borderLayer = widget->GetLayer("border");

		dragHandle = mmake<DragHandle>();
		dragHandle->isPointInside = [this](const Vec2F& p) { return widget->IsUnderPoint(p); };
		dragHandle->onHoverEnter = [this]() { widget->SetState("hover", true); };
		dragHandle->onHoverExit = [this]() { widget->SetState("hover", false); };
		dragHandle->onPressed = [this]() { widget->SetState("pressed", true); };
		dragHandle->onReleased = [this]() { widget->SetState("pressed", false); };
        dragHandle->messageFallDownListener = owner.Get();
        dragHandle->onChangedPos = [this](const Vec2F& pos)
		{
			widget->layout->position = pos;
			this->state.Lock()->SetPosition(pos); 
		};

		widget->onDraw = [this]() { dragHandle->Draw(); };

		for (auto& animation : state->GetAnimations())
		{
			Ref<StateAnimation> stateAnimation = mmake<StateAnimation>();
			stateAnimation->name = animation->name;
			stateAnimation->animation = animation;
			animations.Add(stateAnimation);
		}

		animationsListProperty = DynamicCast<VectorProperty>(o2EditorProperties.CreateVectorField(&TypeOf(Vector<Ref<StateAnimation>>), "Animations"));
		animationsListProperty->SetHeaderEnabled(false);
		animationsListProperty->SetCaptionIndexesEnabled(false);
		animationsListProperty->SetCountEditBoxEnabled(false);
		animationsListProperty->SetValuePointers<Vector<Ref<StateAnimation>>>({ &animations });
		widget->AddChild(animationsListProperty);

		widget->layout->position = state->GetPosition();
		dragHandle->position = state->GetPosition();

		editor.Lock()->AddChild(widget);
	}

	void AnimationStateGraphEditor::StateWidget::InitializeTransitions()
	{
		for (auto& transition : state.Lock()->GetTransitions())
		{
			Ref<StateTransition> stateTransition = mmake<StateTransition>();
			stateTransition->owner = Ref(this);
			stateTransition->destination = editor.Lock()->mStatesWidgetsMap[transition->GetDestinationState()];
			transitions.Add(stateTransition);
		}
	}

	void AnimationStateGraphEditor::StateWidget::RemoveWidget()
	{
        if (!widget)
			return;

		auto parent = widget->GetParent();
		if (parent)
			parent.Lock()->RemoveChild(widget);

        widget = nullptr;
	}

	void AnimationStateGraphEditor::StateWidget::DrawTransitions()
	{
		for (auto& transition : transitions)
			transition->Draw();
	}

	void AnimationStateGraphEditor::StateWidget::UpdateState(TransitionState state)
	{
		widget->SetState("finished", state == TransitionState::Finished);
		widget->SetState("planned", state == TransitionState::Planned);
	}

	void AnimationStateGraphEditor::StateTransition::Draw()
	{
		auto from = owner.Lock();
		auto to = destination.Lock();
		if (!from || !to)
			return;

		Vec2F fromPoint = from->widget->layout->GetWorldCenter();
		Vec2F toPoint = to->widget->layout->GetWorldCenter();
		Vec2F dir = (toPoint - fromPoint).Normalized();
		Vec2F norm = dir.Perpendicular();
		Vec2F center = (fromPoint + toPoint)/2.0f;

		Color4 color(126, 149, 160);
		float width = 4.0f;
		float arrowSize = 10.0f;

		static Vector<Vec2F> arrowLocal = { Vec2F(-0.5f, 0.5f), Vec2F(0.0f, 1.5f), Vec2F(0.5f, 0.5f), Vec2F(-0.5f, 0.5f), Vec2F(0.0f, 1.5f) };

		Basis arrowBasis(center, norm*arrowSize, dir*arrowSize);
		Vector<Vec2F> arrowWorld = arrowLocal.Convert<Vec2F>([arrowBasis](const Vec2F& p) { return arrowBasis.Transform(p); });

		o2Render.DrawAALine(fromPoint, toPoint, color, width);
		o2Render.DrawAALine(arrowWorld, color, width);
	}

	const Type* GraphAnimationStateViewer::GetViewingObjectType() const
	{
		if (mRealObjectType)
			return mRealObjectType;

		return GetViewingObjectTypeStatic();
	}

	const Type* GraphAnimationStateViewer::GetViewingObjectTypeStatic()
	{
		return &TypeOf(AnimationStateGraphEditor::StateAnimation);
	}

	Ref<Spoiler> GraphAnimationStateViewer::CreateSpoiler(const Ref<Widget>& parent)
	{
		mSpoiler = IObjectPropertiesViewer::CreateSpoiler(parent);

		mPlayPause = o2UI.CreateWidget<Toggle>("animation state play-stop");
		mPlayPause->name = "play-stop";
		*mPlayPause->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(7, 1));
		mPlayPause->onToggle = THIS_FUNC(OnPlayPauseToggled);
		mSpoiler->AddInternalWidget(mPlayPause);

		mEditBtn = o2UI.CreateWidget<Button>("edit animation state");
		mEditBtn->name = "edit";
		*mEditBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(-40, 1));
		mEditBtn->onClick = THIS_FUNC(OnEditPressed);
		mSpoiler->AddInternalWidget(mEditBtn);

		mLooped = o2UI.CreateWidget<Toggle>("animation state loop");
		mLooped->name = "loop";
		*mLooped->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(-20, 1));
		mLooped->onToggle = THIS_FUNC(OnLoopToggled);
		mSpoiler->AddInternalWidget(mLooped);

		mTimeProgress = o2UI.CreateWidget<HorizontalProgress>("animation state bar");
		mTimeProgress->name = "bar";
		*mTimeProgress->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 2, 18);
		mTimeProgress->onChangeByUser = THIS_FUNC(OnTimeProgressChanged);
		mSpoiler->AddInternalWidget(mTimeProgress);

		if (auto textLayer = GetSpoiler()->GetLayer("caption"))
		{
			textLayer->layout.offsetLeft = 27;
			textLayer->layout.offsetBottom = -19;
			textLayer->layout.offsetTop = 1;
		}

		if (auto header = parent->GetChildByType<Widget>("caption/header"))
		{
			auto spacer = mmake<Widget>();
			spacer->layout->maxWidth = 40;
			header->AddChild(spacer, 1);
		}

		return mSpoiler;
	}

	void GraphAnimationStateViewer::OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets)
	{
		if (mSubscribedPlayer)
			mSubscribedPlayer.Lock()->onUpdate -= THIS_FUNC(OnAnimationUpdated);

		mSubscribedPlayer = nullptr;

		if (!targetObjets.IsEmpty())
		{
			auto state = dynamic_cast<AnimationStateGraphEditor::StateAnimation*>(targetObjets.Last().first);
			if (state && state->state)
				mSubscribedPlayer = Ref(&state->state->GetPlayer());

			if (mSubscribedPlayer)
				mSubscribedPlayer.Lock()->onUpdate += THIS_FUNC(OnAnimationUpdated);
		}
	}

	void GraphAnimationStateViewer::OnFree()
	{
		if (mSubscribedPlayer)
			mSubscribedPlayer.Lock()->onUpdate -= THIS_FUNC(OnAnimationUpdated);

		mSubscribedPlayer = nullptr;
	}

	void GraphAnimationStateViewer::OnPlayPauseToggled(bool play)
	{
		if (auto subscribedPlayer = mSubscribedPlayer.Lock())
		{
			if (play && subscribedPlayer->GetRelativeTime() >= 1.0f - FLT_EPSILON)
				subscribedPlayer->SetRelTime(0.0f);

			subscribedPlayer->SetPlaying(play);
		}

		o2Scene.OnObjectChanged(o2EditorSceneScreen.GetSelectedObjects().First());
	}

	void GraphAnimationStateViewer::OnLoopToggled(bool looped)
	{
		for (auto& targets : mTargetObjects)
		{
			if (!targets.first)
				continue;

			auto animationState = dynamic_cast<IAnimationState*>(targets.first);
			if (!animationState)
				continue;

			animationState->SetLooped(looped);
		}

		o2Scene.OnObjectChanged(o2EditorSceneScreen.GetSelectedObjects().First());
	}

	void GraphAnimationStateViewer::OnEditPressed()
	{
		if (mTargetObjects.IsEmpty())
			return;

		auto state = dynamic_cast<AnimationStateGraphEditor::StateAnimation*>(mTargetObjects.Last().first);
		auto animationState = DynamicCast<AnimationState>(state->state);
		if (!animationState)
			return;

		auto animationRef = animationState->GetAnimation();
		if (!animationRef)
		{
			animationRef.CreateInstance();
			animationState->SetAnimation(animationRef);

			GetSpoiler()->Expand();
		}

		if (animationRef)
		{
			o2EditorAnimationWindow.SetAnimation(animationRef->animation);

			if (!o2EditorSceneScreen.GetSelectedObjects().IsEmpty())
				o2EditorAnimationWindow.SetTarget(DynamicCast<Actor>(o2EditorSceneScreen.GetSelectedObjects().Last()));

			o2EditorAnimationWindow.SetAnimationEditable(Ref(mPropertiesContext->FindOnStack<IEditableAnimation>()));
			o2EditorAnimationWindow.GetWindow()->Focus();
		}
	}

	void GraphAnimationStateViewer::OnTimeProgressChanged(float value)
	{
		if (mSubscribedPlayer)
			mSubscribedPlayer.Lock()->SetRelTime(value);
	}

	void GraphAnimationStateViewer::OnAnimationUpdated(float time)
	{
		if (auto subscribedPlayer = mSubscribedPlayer.Lock())
		{
			mTimeProgress->value = subscribedPlayer->GetLoopTime()/ subscribedPlayer->GetDuration();
			mPlayPause->value = subscribedPlayer->IsPlaying();
		}
	}

}

DECLARE_TEMPLATE_CLASS(o2::LinkRef<Editor::AnimationStateGraphEditor>);
// --- META ---

ENUM_META(Editor::AnimationStateGraphEditor::StateWidget::TransitionState)
{
    ENUM_ENTRY(Finished);
    ENUM_ENTRY(None);
    ENUM_ENTRY(Planned);
}
END_ENUM_META;

DECLARE_CLASS(Editor::AnimationStateGraphEditor, Editor__AnimationStateGraphEditor);

DECLARE_CLASS(Editor::GraphAnimationStateViewer, Editor__GraphAnimationStateViewer);

DECLARE_CLASS(Editor::AnimationStateGraphEditor::StateAnimation, Editor__AnimationStateGraphEditor__StateAnimation);
// --- END META ---
