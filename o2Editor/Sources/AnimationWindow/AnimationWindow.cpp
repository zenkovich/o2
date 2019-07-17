#include "stdafx.h"
#include "AnimationWindow.h"

#include "Animation/Animation.h"
#include "AnimationWindow/KeyHandlesSheet.h"
#include "AnimationWindow/Timeline.h"
#include "AnimationWindow/Tree.h"
#include "Core/EditorScope.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/Widgets/Toggle.h"
#include "Utils/Editor/DragHandle.h"

DECLARE_SINGLETON(Editor::AnimationWindow);

namespace Editor
{
	AnimationWindow::AnimationWindow()
	{
		InitializeWindow();
	}

	AnimationWindow::~AnimationWindow()
	{}

	void AnimationWindow::Update(float dt)
	{
		if (mAnimation)
		{
			if (mAnimation->IsPlaying() != mPlayPauseToggle->GetValue())
				mPlayPauseToggle->SetValue(mAnimation->IsPlaying());
		}
	}

	void AnimationWindow::SetAnimation(Animation* animation)
	{
		mAnimation = animation;

		if (mAnimation)
		{
			mAnimation->Stop();
			mLoopToggle->SetValue(mAnimation->GetLoop() == Loop::Repeat);
		}

		mPlayPauseToggle->SetValue(false);

		mHandlesSheet->SetAnimation(animation);
		mTimeline->SetAnimation(animation);
		mTree->SetAnimation(animation);
	}

	void AnimationWindow::InitializeWindow()
	{
		PushScopeEnterOnStack scope;

		mWindow->caption = "Animation";
		mWindow->name = "animation window";
		mWindow->SetIcon(mnew Sprite("ui/UI4_animation_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 1)));
		mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 18));

		InitializeUpPanel();
		InitializeTimeline();

		mWorkArea = mnew Widget();
		*mWorkArea->layout = WidgetLayout::BothStretch(0, 0, 0, 18);
		mWindow->AddChild(mWorkArea);

		InitializeHandlesSheet();
		InitializeTree();
		InitializeSeparatorHandle();

		mHandlesSheet->Initialize(mTimeline, mTree);
		mTree->Initialize(mTimeline, mHandlesSheet);
	}

	void AnimationWindow::InitializeHandlesSheet()
	{
		mHandlesSheet = mnew KeyHandlesSheet();
		*mHandlesSheet->layout = WidgetLayout::BothStretch(mTreeViewWidth, 0, 0, 0);
		mWorkArea->AddChild(mHandlesSheet);
	}

	void AnimationWindow::InitializeTree()
	{
		mTree = o2UI.CreateWidget<AnimationTree>();
		*mTree->layout = WidgetLayout::BothStretch();
		mTree->SetTreeWidth(mTreeViewWidth);
		mWorkArea->AddChild(mTree);
	}

	void AnimationWindow::InitializeTimeline()
	{
		mTimeline = mnew AnimationTimeline();
		*mTimeline->layout = WidgetLayout::BothStretch(mTreeViewWidth, 0.0f, 0.0f, 0.0f);

		mTimeScroll = o2UI.CreateHorScrollBar();
		*mTimeScroll->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 10, 10, 20, 0);

		mTimeline->SetScrollBar(mTimeScroll);

		mWindow->AddChild(mTimeline);
	}

	void AnimationWindow::InitializeUpPanel()
	{
		mUpPanel = mnew Widget();
		*mUpPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20);
		mUpPanel->AddLayer("back", mnew Sprite("ui/UI4_small_panel_back.png"), Layout::BothStretch(-4, -4, -5, -5));
		mWindow->AddChild(mUpPanel);

		mControlsPanel = mnew Widget();
		mControlsPanel->name = "controls panel";
		*mControlsPanel->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(mTreeViewWidth, 20.0f));
		mControlsPanel->AddLayer("back", mnew Sprite("ui/UI4_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		mRecordToggle = o2UI.CreateWidget<Toggle>("menu record");
		*mRecordToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(1, 0));
		mControlsPanel->AddChild(mRecordToggle);

		mPlayPauseToggle = o2UI.CreateWidget<Toggle>("menu play-stop");
		*mPlayPauseToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(21, 0));
		mPlayPauseToggle->SetValue(false);
		mPlayPauseToggle->onToggleByUser = THIS_FUNC(OnPlayPauseToggled);
		mControlsPanel->AddChild(mPlayPauseToggle);

		mLoopToggle = o2UI.CreateWidget<Toggle>("menu loop-nonloop");
		*mLoopToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(41, 0));
		mLoopToggle->SetValue(true);
		mLoopToggle->onToggleByUser = THIS_FUNC(OnLoopToggled);
		mControlsPanel->AddChild(mLoopToggle);

		mAddKeyButton = o2UI.CreateWidget<Button>("menu add key");
		*mAddKeyButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(61, 0));
		mControlsPanel->AddChild(mAddKeyButton);

		mUpPanel->AddChild(mControlsPanel);
	}

	void AnimationWindow::InitializeSeparatorHandle()
	{
		mTreeSeparatorHandle = mnew WidgetDragHandle(mnew Sprite("ui/UI4_Ver_separator.png"));
		mTreeSeparatorHandle->GetRegularSprite()->pivot = Vec2F(0.5f, 0.5f);
		mTreeSeparatorHandle->GetRegularSprite()->szPivot = Vec2F(4, mTreeSeparatorHandle->GetRegularSprite()->szPivot.Get().y);

		mTreeSeparatorHandle->onChangedPos = [&](const Vec2F& point) {
			mTreeViewWidth = point.x;
			mControlsPanel->layout->right = mTreeViewWidth;
			mTimeline->layout->left = mTreeViewWidth;
			mHandlesSheet->layout->left = mTreeViewWidth;
			mTree->SetTreeWidth(mTreeViewWidth);
		};

		mTreeSeparatorHandle->checkPositionFunc = [&](const Vec2F& point) {
			return Vec2F(point.x, mWorkArea->layout->GetHeight()*0.5f);
		};

		mTreeSeparatorHandle->onLayoutUpdated = [&]() {
			mTreeSeparatorHandle->SetSpritesSize(Vec2F(5.0f, mWorkArea->layout->GetHeight() + 50.0f));
			mTreeSeparatorHandle->SetPosition(Vec2F(mTreeViewWidth, mWorkArea->layout->GetHeight()*0.5f));
		};

		mTreeSeparatorHandle->cursorType = CursorType::SizeWE;

		mWorkArea->AddChild(mTreeSeparatorHandle);
	}

	void AnimationWindow::OnPlayPauseToggled(bool play)
	{
		if (mAnimation)
		{
			if (mAnimation->GetLoop() != Loop::Repeat && Math::Equals(mAnimation->GetTime(), mAnimation->GetDuration()))
				mAnimation->SetTime(0.0f);

			mAnimation->SetPlaying(play);
		}
	}

	void AnimationWindow::OnLoopToggled(bool loop)
	{
		if (mAnimation)
			mAnimation->SetLoop(loop ? Loop::Repeat : Loop::None);
	}

	void AnimationWindow::OnSearchEdited(const WString& search)
	{

	}

	void AnimationWindow::OnMenuFilterPressed()
	{

	}

	void AnimationWindow::OnMenuRecordPressed()
	{

	}

}

DECLARE_CLASS(Editor::AnimationWindow);
