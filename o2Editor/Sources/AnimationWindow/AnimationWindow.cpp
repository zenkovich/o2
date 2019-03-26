#include "stdafx.h"
#include "AnimationWindow.h"

#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/Widgets/Toggle.h"

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

void AnimationWindow::InitializeWindow()
{
	mWindow->caption = "Animation";
	mWindow->name = "animation window";
	mWindow->SetIcon(mnew Sprite("ui/UI4_animation_icon.png"));
	mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 1)));
	mWindow->SetViewLayout(Layout::BothStretch(-2, 0, 0, 18));
	mWindow->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 18));

	InitializeUpPanel();
}

void AnimationWindow::InitializeUpPanel()
{
	Widget* upPanel = mnew Widget();
	upPanel->name = "up panel";
	*upPanel->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(mTreeViewWidth, 20.0f));
	upPanel->AddLayer("back", mnew Sprite("ui/UI_square_field.png"), Layout::BothStretch(-4, -4, -5, -5));

	mRecordToggle = o2UI.CreateWidget<Toggle>("menu record");
	*mRecordToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(1, 1));
	upPanel->AddChild(mRecordToggle);

	mPlayPauseToggle = o2UI.CreateWidget<Toggle>("menu play-stop");
	*mPlayPauseToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(21, 1));
	mPlayPauseToggle->SetValue(false);
	upPanel->AddChild(mPlayPauseToggle);

	mLoopToggle = o2UI.CreateWidget<Toggle>("menu loop-nonloop");
	*mLoopToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(41, 1));
	mLoopToggle->SetValue(true);
	upPanel->AddChild(mLoopToggle);

	mAddKeyButton = o2UI.CreateWidget<Button>("menu add key");
	*mAddKeyButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(61, 1));
	upPanel->AddChild(mAddKeyButton);

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

	upPanel->AddChild(searchPanel);

	mWindow->AddChild(upPanel);
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
