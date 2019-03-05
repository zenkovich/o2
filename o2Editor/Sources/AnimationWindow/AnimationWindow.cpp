#include "stdafx.h"
#include "AnimationWindow.h"

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
}

}

DECLARE_CLASS(Editor::AnimationWindow);
