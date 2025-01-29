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

		mHorScrollbar->Draw();
		mVerScrollbar->Draw();

        o2Render.EnableScissorTest(layout->GetWorldRect());

        DrawHandles();
        DrawSelection();

		o2Render.DisableScissorTest();

		DrawDebugFrame();
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

    void AnimationStateGraphEditor::RedrawContent()
    {
        mListenersLayer->OnBeginDraw();
        mListenersLayer->camera = o2Render.GetCamera();

		DrawGrid();

		CursorAreaEventsListener::OnDrawn();
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

	void AnimationStateGraphEditor::InitializeStates()
	{
		for (auto& state : mStatesWidgets)
			state->RemoveWidget();

		mStates.Clear();

		if (!mGraph)
			return;

		auto graph = mGraph.Lock();
		for (auto& state : graph->GetStates())
		{
			Ref<StateWidget> stateWidget = mmake<StateWidget>(Ref(this));
			stateWidget->Initialize(state);
			mStatesWidgets.Add(stateWidget);
		}
	}

	AnimationStateGraphEditor::StateWidget::StateWidget(const Ref<AnimationStateGraphEditor>& owner)
	{
		editor = owner;

        widget = o2UI.CreateWidget<VerticalLayout>("ASG state");

		dragHandle = mmake<DragHandle>();
		dragHandle->isPointInside = [=](const Vec2F& p) { return widget->IsUnderPoint(p); };
		dragHandle->onHoverEnter = [=]() { widget->SetState("hover", true); };
		dragHandle->onHoverExit = [=]() { widget->SetState("hover", false); };
		dragHandle->onPressed = [=]() { widget->SetState("pressed", true); };
		dragHandle->onReleased = [=]() { widget->SetState("pressed", false); };
        dragHandle->messageFallDownListener = owner.Get();
        dragHandle->onChangedPos = [=](const Vec2F& pos) { widget->layout->position = pos; state.Lock()->SetPosition(pos); };
		widget->onDraw = [=]() { dragHandle->Draw(); };

        animationsListProperty = DynamicCast<VectorProperty>(o2EditorProperties.CreateVectorField(&TypeOf(Vector<Ref<StateAnimation>>), "Animations"));
		widget->AddChild(animationsListProperty);
	}

	void AnimationStateGraphEditor::StateWidget::Initialize(const Ref<AnimationGraphState>& state)
	{
        this->state = state;

        animations.Clear();

		for (auto& animation : state->GetAnimations())
		{
			Ref<StateAnimation> stateAnimation = mmake<StateAnimation>();
			stateAnimation->name = animation->name;
			stateAnimation->animation = animation;
			animations.Add(stateAnimation);
		}

        animationsListProperty->SetValuePointers<Vector<Ref<StateAnimation>>>({ &animations });

		widget->layout->position = state->GetPosition();
        dragHandle->position = state->GetPosition();

		editor.Lock()->AddChild(widget);
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
}

DECLARE_TEMPLATE_CLASS(o2::LinkRef<Editor::AnimationStateGraphEditor>);
// --- META ---

DECLARE_CLASS(Editor::AnimationStateGraphEditor, Editor__AnimationStateGraphEditor);

DECLARE_CLASS(Editor::AnimationStateGraphEditor::StateAnimation, Editor__AnimationStateGraphEditor__StateAnimation);
// --- END META ---
