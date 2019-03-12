#include "stdafx.h"
#include "AnimationWindow.h"

#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/UIManager.h"

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

	mRecordButton = o2UI.CreateWidget<Button>("menu filter");
	*mRecordButton->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 1));
	mRecordButton->onClick += THIS_FUNC(OnMenuRecordPressed);
	upPanel->AddChild(mRecordButton);

	Button* searchButton = o2UI.CreateWidget<Button>("search");
	*searchButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(-1, 1));
	upPanel->AddChild(searchButton);

	mFilterButton = o2UI.CreateWidget<Button>("menu filter");
	*mFilterButton->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 1));
	mFilterButton->onClick += THIS_FUNC(OnMenuFilterPressed);
	upPanel->AddChild(mFilterButton);

	mSearchEditBox = o2UI.CreateWidget<EditBox>("backless");
	*mSearchEditBox->layout = WidgetLayout::BothStretch(19, 2, 21, -2);
	mSearchEditBox->onChanged += THIS_FUNC(OnSearchEdited);
	upPanel->AddChild(mSearchEditBox);

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
