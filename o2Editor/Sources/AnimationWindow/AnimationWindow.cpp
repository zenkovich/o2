#include "stdafx.h"
#include "AnimationWindow.h"

#include "AnimationWindow/Timeline.h"
#include "Animation/Animation.h"
#include "AnimationWindow/Tree.h"
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
	{

	}

	void AnimationWindow::Update(float dt)
	{

	}

	void AnimationWindow::SetAnimation(Animation* animation)
	{
		mAnimation = animation;

		mTimeline->SetDuration(animation->GetDuration());
		mTree->SetAnimation(animation);
	}

	void AnimationWindow::InitializeWindow()
	{
		mWindow->caption = "Animation";
		mWindow->name = "animation window";
		mWindow->SetIcon(mnew Sprite("ui/UI4_animation_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 1)));
		mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
		mWindow->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 18));

		mUpPanel = mnew Widget();
		*mUpPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20);
		mUpPanel->AddLayer("back", mnew Sprite("ui/UI2_small_panel_back.png"), Layout::BothStretch(-4, -4, -5, -5));
		mWindow->AddChild(mUpPanel);

		mWorkArea = mnew Widget();
		*mWorkArea->layout = WidgetLayout::BothStretch(0, 0, 0, 20);
		mWindow->AddChild(mWorkArea);

		InitializeUpPanel();
		InitializeSeparatorHandle();

		mTimeline = mnew AnimationTimeline();
		*mTimeline->layout = WidgetLayout::BothStretch(mTreeViewWidth, 0.0f, 0.0f, 0.0f);

		mTimeScroll = o2UI.CreateHorScrollBar();
		*mTimeScroll->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 10, 10, 20, 0);

		mTimeline->SetScrollBar(mTimeScroll);
		mTimeline->SetDuration(100.0f);

		mWindow->AddChild(mTimeline);

		mTree = o2UI.CreateWidget<AnimationTree>();
		*mTree->layout = WidgetLayout::BothStretch();
		mWorkArea->AddChild(mTree);
	}

	void AnimationWindow::InitializeUpPanel()
	{
		mControlsPanel = mnew Widget();
		mControlsPanel->name = "controls panel";
		*mControlsPanel->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(mTreeViewWidth, 20.0f));
		mControlsPanel->AddLayer("back", mnew Sprite("ui/UI_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

		mRecordToggle = o2UI.CreateWidget<Toggle>("menu record");
		*mRecordToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(1, 1));
		mControlsPanel->AddChild(mRecordToggle);

		mPlayPauseToggle = o2UI.CreateWidget<Toggle>("menu play-stop");
		*mPlayPauseToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(21, 1));
		mPlayPauseToggle->SetValue(false);
		mControlsPanel->AddChild(mPlayPauseToggle);

		mLoopToggle = o2UI.CreateWidget<Toggle>("menu loop-nonloop");
		*mLoopToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(41, 1));
		mLoopToggle->SetValue(true);
		mControlsPanel->AddChild(mLoopToggle);

		mAddKeyButton = o2UI.CreateWidget<Button>("menu add key");
		*mAddKeyButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(61, 1));
		mControlsPanel->AddChild(mAddKeyButton);

		auto searchPanel = mnew Widget();
		searchPanel->name = "search";
		*searchPanel->layout = WidgetLayout::BothStretch(82, 0, 0, 0);

		Button* searchButton = o2UI.CreateWidget<Button>("search");
		*searchButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
		searchPanel->AddChild(searchButton);

		mFilterButton = o2UI.CreateWidget<Button>("menu filter");
		*mFilterButton->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 1));
		mFilterButton->onClick += THIS_FUNC(OnMenuFilterPressed);
		searchPanel->AddChild(mFilterButton);

		mSearchEditBox = o2UI.CreateWidget<EditBox>("backless");
		*mSearchEditBox->layout = WidgetLayout::BothStretch(19, 2, 21, -2);
		mSearchEditBox->onChanged += THIS_FUNC(OnSearchEdited);
		searchPanel->AddChild(mSearchEditBox);

		mControlsPanel->AddChild(searchPanel);

		mUpPanel->AddChild(mControlsPanel);
	}

	void AnimationWindow::InitializeSeparatorHandle()
	{
		mTreeSeparatorHandle = mnew WidgetDragHandle(mnew Sprite("ui/UI_Ver_separator.png"));
		mTreeSeparatorHandle->GetRegularSprite()->pivot = Vec2F(0.5f, 0.5f);
		mTreeSeparatorHandle->GetRegularSprite()->szPivot = Vec2F(4, mTreeSeparatorHandle->GetRegularSprite()->szPivot.Get().y);

		mTreeSeparatorHandle->onChangedPos = [&](const Vec2F& point) {
			mTreeViewWidth = point.x;
			mControlsPanel->layout->right = mTreeViewWidth;
			mTimeline->layout->left = mTreeViewWidth;
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
