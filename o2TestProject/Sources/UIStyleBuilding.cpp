#include "UIStyleBuilding.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/Button.h"
#include "UI/ContextMenu.h"
#include "UI/CustomDropDown.h"
#include "UI/CustomList.h"
#include "UI/DropDown.h"
#include "UI/EditBox.h"
#include "UI/HorizontalProgress.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/Label.h"
#include "UI/List.h"
#include "UI/ScrollArea.h"
#include "UI/Toggle.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "UI/VerticalProgress.h"
#include "UI/VerticalScrollBar.h"
#include "UI/Window.h"

using namespace o2;

void RebuildButtonStyle()
{
	Ptr<UIButton> sample = mnew UIButton();
	sample->layout.minSize = Vec2F(20, 20);

	auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI_button_regular.png"), Layout::Both(-9, -9, -10, -10));
	auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI_button_select.png"), Layout::Both(-9, -9, -10, -10));
	auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_button_pressed.png"), Layout::Both(-9, -9, -10, -10));
	auto focusLayer = sample->AddLayer("focus", mnew Sprite("ui/UI_button_focus.png"), Layout::Both(-9, -9, -10, -10));

	Ptr<Text> captionText = mnew Text("arial.ttf");
	captionText->text = "Button";
	captionText->horAlign = HorAlign::Middle;
	captionText->verAlign = VerAlign::Middle;
	captionText->dotsEngings = true;
	sample->AddLayer("caption", captionText);

	sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
		->offStateAnimationSpeed = 1.0f / 4.0f;

	sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildCloseButtonStyle()
{
	Ptr<UIButton> sample = mnew UIButton();
	sample->layout.minSize = Vec2F(5, 5);

	auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI_Close_button_regular.png"),
										 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

	auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI_Close_button_select.png"),
										Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

	auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_Close_button_pressed.png"),
										 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


	sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
		->offStateAnimationSpeed = 1.0f / 4.0f;

	sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "close");
}

void RebuildArrowButtonStyle()
{
	Ptr<UIButton> sample = mnew UIButton();
	sample->layout.minSize = Vec2F(5, 5);

	auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI_Options_button_regular.png"),
										 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

	auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI_Options_button_select.png"),
										Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

	auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_Options_button_pressed.png"),
										 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


	sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
		->offStateAnimationSpeed = 1.0f / 4.0f;

	sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "arrow");
}

void RebuildHorProgressBarStyle()
{
	Ptr<UIHorizontalProgress> sample = mnew UIHorizontalProgress();
	sample->layout.minSize = Vec2F(5, 5);
	auto backLayer = sample->AddLayer("back", nullptr);
	auto spriteBackLayer = backLayer->AddChildLayer("sprite", mnew Sprite("ui/UI_Hor_scrollbar_bk.png"),
													Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
														   Vec2F(-2, -4), Vec2F(2, 5)));

	backLayer->interactableLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -4), Vec2F(0, 4));

	auto barLayer = sample->AddLayer("bar", nullptr);
	auto barRegularSprite = barLayer->AddChildLayer("regular", mnew Sprite("ui/UI_Hor_scrollhandle_regular.png"),
													Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
														   Vec2F(-2, -4), Vec2F(2, 5)));

	auto barSelectSprite = barLayer->AddChildLayer("select", mnew Sprite("ui/UI_Hor_scrollhandle_select.png"),
												   Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
														  Vec2F(-2, -4), Vec2F(2, 5)));

	auto barPressedSprite = barLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_Hor_scrollhandle_pressed.png"),
													Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
														   Vec2F(-2, -4), Vec2F(2, 5)));

	sample->AddState("select", Animation::EaseInOut(sample, &barSelectSprite->transparency, 0.0f, 1.0f, 0.1f))
		->offStateAnimationSpeed = 1.0f / 4.0f;

	sample->AddState("pressed", Animation::EaseInOut(sample, &barPressedSprite->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	sample->SetOrientation(UIHorizontalProgress::Orientation::Right);

	o2UI.AddWidgetStyle(sample, "standard");
}


void RebuildVerProgressBarStyle()
{
	Ptr<UIVerticalProgress> sample = mnew UIVerticalProgress();
	sample->layout.minSize = Vec2F(5, 5);
	auto backLayer = sample->AddLayer("back", nullptr);
	auto spriteBackLayer = backLayer->AddChildLayer("sprite", mnew Sprite("ui/UI_Ver_scrollbar_bk.png"),
													Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
														   Vec2F(-4, -2), Vec2F(5, 2)));
	backLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));

	auto barLayer = sample->AddLayer("bar", nullptr);
	auto barRegularSprite = barLayer->AddChildLayer("regular", mnew Sprite("ui/UI_Ver_scrollhandle_regular.png"),
													Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
														   Vec2F(-4, -2), Vec2F(5, 2)));

	auto barSelectSprite = barLayer->AddChildLayer("select", mnew Sprite("ui/UI_Ver_scrollhandle_select.png"),
												   Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
														  Vec2F(-4, -2), Vec2F(5, 2)));

	auto barPressedSprite = barLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_Ver_scrollhandle_pressed.png"),
													Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
														   Vec2F(-4, -2), Vec2F(5, 2)));


	sample->AddState("select", Animation::EaseInOut(sample, &barSelectSprite->transparency, 0.0f, 1.0f, 0.1f))
		->offStateAnimationSpeed = 1.0f / 4.0f;

	sample->AddState("pressed", Animation::EaseInOut(sample, &barPressedSprite->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildHorScrollBarStyle()
{
	Ptr<UIHorizontalScrollBar> sample = mnew UIHorizontalScrollBar();
	sample->layout.minSize = Vec2F(5, 5);
	sample->SetScrollSense(0.25f);
	auto backLayer = sample->AddLayer("back", nullptr);
	backLayer->interactableLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -4), Vec2F(0, 4));
	auto spriteBackLayer = backLayer->AddChildLayer("sprite", mnew Sprite("ui/UI_Hor_scrollbar_bk.png"),
													Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
														   Vec2F(-2, -4), Vec2F(2, 5)));

	auto handleLayer = sample->AddLayer("handle", nullptr);
	handleLayer->interactableLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -4), Vec2F(0, 4));
	auto barRegularSprite = handleLayer->AddChildLayer("regular", mnew Sprite("ui/UI_Hor_scrollhandle_regular.png"),
													   Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															  Vec2F(-2, -4), Vec2F(2, 5)));

	auto barSelectSprite = handleLayer->AddChildLayer("select", mnew Sprite("ui/UI_Hor_scrollhandle_select.png"),
													  Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															 Vec2F(-2, -4), Vec2F(2, 5)));

	auto barPressedSprite = handleLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_Hor_scrollhandle_pressed.png"),
													   Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															  Vec2F(-2, -4), Vec2F(2, 5)));

	sample->AddState("select", Animation::EaseInOut(sample, &barSelectSprite->transparency, 0.0f, 1.0f, 0.1f))
		->offStateAnimationSpeed = 1.0f / 4.0f;

	sample->AddState("pressed", Animation::EaseInOut(sample, &barPressedSprite->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildVerScrollBarStyle()
{
	Ptr<UIVerticalScrollBar> sample = mnew UIVerticalScrollBar();
	sample->layout.minSize = Vec2F(5, 5);
	sample->SetScrollSense(0.25f);
	auto backLayer = sample->AddLayer("back", nullptr);
	backLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));
	auto spriteBackLayer = backLayer->AddChildLayer("sprite", mnew Sprite("ui/UI_Ver_scrollbar_bk.png"),
													Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
														   Vec2F(-4, -2), Vec2F(5, 2)));

	auto handleLayer = sample->AddLayer("handle", nullptr);
	handleLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));
	auto barRegularSprite = handleLayer->AddChildLayer("regular", mnew Sprite("ui/UI_Ver_scrollhandle_regular.png"),
													   Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															  Vec2F(-4, -2), Vec2F(5, 2)));

	auto barSelectSprite = handleLayer->AddChildLayer("select", mnew Sprite("ui/UI_Ver_scrollhandle_select.png"),
													  Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															 Vec2F(-4, -2), Vec2F(5, 2)));

	auto barPressedSprite = handleLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_Ver_scrollhandle_pressed.png"),
													   Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															  Vec2F(-4, -2), Vec2F(5, 2)));

	sample->AddState("select", Animation::EaseInOut(sample, &barSelectSprite->transparency, 0.0f, 1.0f, 0.1f))
		->offStateAnimationSpeed = 1.0f / 4.0f;

	sample->AddState("pressed", Animation::EaseInOut(sample, &barPressedSprite->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildCheckboxStyle()
{
	Ptr<UIToggle> sample = mnew UIToggle();
	sample->layout.minSize = Vec2F(20, 20);
	auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Check_bk.png"),
									  Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

	auto selectLayer = sample->AddLayer("backSelect", mnew Sprite("ui/UI_Check_bk_select.png"),
										Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

	auto pressedLayer = sample->AddLayer("backPressed", mnew Sprite("ui/UI_Check_bk_pressed.png"),
										 Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

	auto focusLayer = sample->AddLayer("backFocus", mnew Sprite("ui/UI_Check_bk_focus.png"),
									   Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

	auto checkLayer = sample->AddLayer("check", mnew Sprite("ui/UI_Ckeck.png"),
									   Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(1, -11), Vec2F(21, 10)));

	Ptr<Text> captionText = mnew Text("arial.ttf");
	captionText->text = "Checkbox";
	captionText->horAlign = HorAlign::Left;
	captionText->verAlign = VerAlign::Middle;
	captionText->dotsEngings = true;
	sample->AddLayer("caption", captionText, Layout(Vec2F(0, 0), Vec2F(1, 1), Vec2F(20, 0), Vec2F(0, 0)));

	sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
		->offStateAnimationSpeed = 1.0f / 4.0f;

	sample->AddState("value", Animation::EaseInOut(sample, &checkLayer->transparency, 0.0f, 1.0f, 0.1f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildScrollAreaStraightBarsStyle()
{
	Ptr<UIScrollArea> sample = mnew UIScrollArea();
	sample->layout.minSize = Vec2F(30, 30);
	sample->SetClippingLayout(Layout::Both(1, 2, 1, 1));
	sample->SetViewLayout(Layout::Both(5, 5, 5, 5));
	sample->SetEnableScrollsHiding(false);
	sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::Both(-9, -9, -9, -9));

	Ptr<UIHorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
	horScrollBar->layout.anchorMin = Vec2F(0, 0);
	horScrollBar->layout.anchorMax = Vec2F(1, 0);
	horScrollBar->layout.offsetMin = Vec2F(5, 0);
	horScrollBar->layout.offsetMax = Vec2F(-15, 15);
	sample->SetHorizontalScrollBar(horScrollBar);

	Ptr<UIVerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
	verScrollBar->layout.anchorMin = Vec2F(1, 0);
	verScrollBar->layout.anchorMax = Vec2F(1, 1);
	verScrollBar->layout.offsetMin = Vec2F(-15, 15);
	verScrollBar->layout.offsetMax = Vec2F(0, -5);
	sample->SetVerticalScrollBar(verScrollBar);

	Animation enableHorScrollAnim;
	enableHorScrollAnim.SetTarget(sample);
	*enableHorScrollAnim.AddAnimationValue<float>(&sample->GetVerticalScrollbar()->layout.offsetBottom) =
		AnimatedValue<float>::EaseInOut(5, 15, 0.2f);
	*enableHorScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMin") =
		AnimatedValue<Vec2F>::EaseInOut(Vec2F(5, 5), Vec2F(5, 15), 0.2f);

	auto enableHorScrollState = sample->AddState("enableHorBar", enableHorScrollAnim);

	Animation enableVerScrollAnim;
	enableVerScrollAnim.SetTarget(sample);
	*enableVerScrollAnim.AddAnimationValue<float>(&sample->GetHorizontalScrollbar()->layout.offsetRight) =
		AnimatedValue<float>::EaseInOut(-5, -15, 0.2f);
	*enableVerScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMax") =
		AnimatedValue<Vec2F>::EaseInOut(Vec2F(-5, -5), Vec2F(-15, -5), 0.2f);

	auto enableVerScrollState = sample->AddState("enableVerBar", enableVerScrollAnim);

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "straightBars");
}

void RebuildScrollAreaStyle()
{
	Ptr<UIScrollArea> sample = mnew UIScrollArea();
	sample->layout.minSize = Vec2F(20, 20);
	sample->SetClippingLayout(Layout::Both(1, 2, 1, 1));
	sample->SetViewLayout(Layout::Both(5, 5, 5, 5));
	sample->SetEnableScrollsHiding(true);
	sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::Both(-9, -9, -9, -9));

	Ptr<UIHorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
	horScrollBar->layout.anchorMin = Vec2F(0, 0);
	horScrollBar->layout.anchorMax = Vec2F(1, 0);
	horScrollBar->layout.offsetMin = Vec2F(5, 0);
	horScrollBar->layout.offsetMax = Vec2F(-15, 15);
	sample->SetHorizontalScrollBar(horScrollBar);

	Ptr<UIVerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
	verScrollBar->layout.anchorMin = Vec2F(1, 0);
	verScrollBar->layout.anchorMax = Vec2F(1, 1);
	verScrollBar->layout.offsetMin = Vec2F(-15, 15);
	verScrollBar->layout.offsetMax = Vec2F(0, -5);
	sample->SetVerticalScrollBar(verScrollBar);

	sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout.offsetBottom,
														  5.0f, 15.0f, 0.2f));

	sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout.offsetBottom,
														  -5.0f, -15.0f, 0.2f));

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildLabelStyle()
{
	Ptr<UILabel> sample = mnew UILabel();
	sample->layout.minSize = Vec2F(20, 20);
	sample->horOverflow = UILabel::HorOverflow::Dots;
	sample->verOverflow = UILabel::VerOverflow::None;

	Ptr<Text> captionText = mnew Text("arial.ttf");
	captionText->text = "Checkbox";
	captionText->horAlign = HorAlign::Middle;
	captionText->verAlign = VerAlign::Middle;
	sample->AddLayer("text", captionText);

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildEditBoxStyle()
{
	Ptr<UIEditBox> sample = mnew UIEditBox();
	sample->SetClippingLayout(Layout::Both(1, 2, 1, 1));
	sample->SetViewLayout(Layout::Both(5, 5, 5, 5));
	sample->SetEnableScrollsHiding(false);
	sample->SetCaretBlinkingDelay(0.85f);
	sample->layout.minSize = Vec2F(30, 40);

	auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::Both(-9, -9, -9, -9));
	auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI_Editbox_select.png"), Layout::Both(-9, -9, -9, -9));
	auto focusLayer = sample->AddLayer("select", mnew Sprite("ui/UI_Editbox_focus.png"), Layout::Both(-9, -9, -9, -9));

	Ptr<UIHorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
	horScrollBar->layout.anchorMin = Vec2F(0, 0);
	horScrollBar->layout.anchorMax = Vec2F(1, 0);
	horScrollBar->layout.offsetMin = Vec2F(5, 0);
	horScrollBar->layout.offsetMax = Vec2F(-15, 15);
	sample->SetHorizontalScrollBar(horScrollBar);

	Ptr<UIVerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
	verScrollBar->layout.anchorMin = Vec2F(1, 0);
	verScrollBar->layout.anchorMax = Vec2F(1, 1);
	verScrollBar->layout.offsetMin = Vec2F(-15, 15);
	verScrollBar->layout.offsetMax = Vec2F(0, -5);
	sample->SetVerticalScrollBar(verScrollBar);

	Animation enableHorScrollAnim;
	enableHorScrollAnim.SetTarget(sample);
	*enableHorScrollAnim.AddAnimationValue<float>(&sample->GetVerticalScrollbar()->layout.offsetBottom) =
		AnimatedValue<float>::EaseInOut(5, 15, 0.2f);
	*enableHorScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMin") =
		AnimatedValue<Vec2F>::EaseInOut(Vec2F(5, 5), Vec2F(5, 15), 0.2f);

	auto enableHorScrollState = sample->AddState("enableHorBar", enableHorScrollAnim);

	Animation enableVerScrollAnim;
	enableVerScrollAnim.SetTarget(sample);
	*enableVerScrollAnim.AddAnimationValue<float>(&sample->GetHorizontalScrollbar()->layout.offsetRight) =
		AnimatedValue<float>::EaseInOut(-5, -15, 0.2f);
	*enableVerScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMax") =
		AnimatedValue<Vec2F>::EaseInOut(Vec2F(-5, -5), Vec2F(-15, -5), 0.2f);

	auto enableVerScrollState = sample->AddState("enableVerBar", enableVerScrollAnim);

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	Ptr<Text> textDrawable = sample->GetTextDrawable();
	textDrawable->verAlign = VerAlign::Top;
	textDrawable->horAlign = HorAlign::Left;
	textDrawable->SetFontAsset("arial.ttf");

	Ptr<Sprite> caretDrawable = sample->GetCaretDrawable();
	*caretDrawable = Sprite();
	caretDrawable->size = Vec2F(1, textDrawable->GetFont()->GetHeight()*1.7f);
	caretDrawable->pivot = Vec2F(0, 0.16f);
	caretDrawable->color = Color4::Black();

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildCustomListStyle()
{
	Ptr<UICustomList> sample = mnew UICustomList();
	sample->layout.minSize = Vec2F(20, 20);
	sample->SetClippingLayout(Layout::Both(1, 2, 1, 1));
	sample->SetViewLayout(Layout::Both(5, 5, 5, 5));
	sample->SetEnableScrollsHiding(true);
	sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::Both(-9, -9, -9, -9));

	Ptr<Sprite> selection = mnew Sprite("ui/UI_ListBox_selection_regular.png");
	*sample->GetSelectionDrawable() = *selection;
	sample->SetSelectionDrawableLayout(Layout::Both(-10, -16, -10, -16));

	Ptr<Sprite> hover = mnew Sprite("ui/UI_ListBox_selection_hover.png");
	*sample->GetHoverDrawable() = *hover;
	sample->SetHoverDrawableLayout(Layout::Both(-10, -16, -10, -16));

	Ptr<UIHorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
	horScrollBar->layout.anchorMin = Vec2F(0, 0);
	horScrollBar->layout.anchorMax = Vec2F(1, 0);
	horScrollBar->layout.offsetMin = Vec2F(5, 0);
	horScrollBar->layout.offsetMax = Vec2F(-15, 15);
	sample->SetHorizontalScrollBar(horScrollBar);

	Ptr<UIVerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
	verScrollBar->layout.anchorMin = Vec2F(1, 0);
	verScrollBar->layout.anchorMax = Vec2F(1, 1);
	verScrollBar->layout.offsetMin = Vec2F(-15, 15);
	verScrollBar->layout.offsetMax = Vec2F(0, -5);
	sample->SetVerticalScrollBar(verScrollBar);

	sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout.offsetBottom,
														  5.0f, 15.0f, 0.2f));

	sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout.offsetBottom,
														  -5.0f, -15.0f, 0.2f));

	sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetHoverDrawable()->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("selected", Animation::EaseInOut(sample, &sample->GetSelectionDrawable()->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildListStyle()
{
	Ptr<UIList> sample = mnew UIList();
	sample->layout.minSize = Vec2F(20, 20);
	sample->SetClippingLayout(Layout::Both(1, 2, 1, 1));
	sample->SetViewLayout(Layout::Both(5, 5, 5, 5));
	sample->SetEnableScrollsHiding(true);
	sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::Both(-9, -9, -9, -9));

	Ptr<Sprite> selection = mnew Sprite("ui/UI_ListBox_selection_regular.png");
	*sample->GetSelectionDrawable() = *selection;
	sample->SetSelectionDrawableLayout(Layout::Both(-10, -16, -10, -16));

	Ptr<Sprite> hover = mnew Sprite("ui/UI_ListBox_selection_hover.png");
	*sample->GetHoverDrawable() = *hover;
	sample->SetHoverDrawableLayout(Layout::Both(-10, -16, -10, -16));

	Ptr<UILabel> itemSample = o2UI.CreateLabel("empty");
	itemSample->layout.size = Vec2F(20, 0);
	itemSample->horAlign = HorAlign::Left;
	itemSample->verAlign = VerAlign::Middle;
	sample->SetItemSample(itemSample);

	Ptr<UIHorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
	horScrollBar->layout.anchorMin = Vec2F(0, 0);
	horScrollBar->layout.anchorMax = Vec2F(1, 0);
	horScrollBar->layout.offsetMin = Vec2F(5, 0);
	horScrollBar->layout.offsetMax = Vec2F(-15, 15);
	sample->SetHorizontalScrollBar(horScrollBar);

	Ptr<UIVerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
	verScrollBar->layout.anchorMin = Vec2F(1, 0);
	verScrollBar->layout.anchorMax = Vec2F(1, 1);
	verScrollBar->layout.offsetMin = Vec2F(-15, 15);
	verScrollBar->layout.offsetMax = Vec2F(0, -5);
	sample->SetVerticalScrollBar(verScrollBar);

	sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout.offsetBottom,
														  5.0f, 15.0f, 0.2f));

	sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout.offsetBottom,
														  -5.0f, -15.0f, 0.2f));

	sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetHoverDrawable()->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("selected", Animation::EaseInOut(sample, &sample->GetSelectionDrawable()->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildCustomDropDownStyle()
{
	Ptr<UICustomDropDown> sample = mnew UICustomDropDown();
	sample->layout.minSize = Vec2F(20, 20);
	auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::Both(-9, -9, -9, -9));
	auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI_Editbox_select.png"), Layout::Both(-9, -9, -9, -9));
	auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_Editbox_pressed.png"), Layout::Both(-9, -9, -9, -9));
	auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI_Down_icn.png"),
									   Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-20, -10), Vec2F(0, 10)));

	sample->SetClippingLayout(Layout::Both(4, 2, 20, 2));

	Ptr<UIWidget> itemSample = mnew UIWidget();
	itemSample->layout.size = Vec2F(20, 20);
	sample->SetItemSample(itemSample);

	auto list = sample->GetListView();
	*list = *o2UI.GetWidgetStyle<UICustomList>("standard");
	list->layer["back"]->drawable.Release();
	list->layer["back"]->drawable = mnew Sprite("ui/UI_Box_regular.png");
	list->layout.pivot = Vec2F(0.5f, 1.0f);
	list->layout.anchorMin = Vec2F(0, 0);
	list->layout.anchorMax = Vec2F(1, 0);
	list->layout.offsetMin = Vec2F(-1, -60);
	list->layout.offsetMax = Vec2F(0, 3);

	sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("opened", Animation::EaseInOut(sample, &arrowLayer->drawable->scale, Vec2F(1, 1), Vec2F(1, -1), 0.2f));

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildDropDownStyle()
{
	Ptr<UIDropDown> sample = mnew UIDropDown();
	sample->layout.minSize = Vec2F(20, 20);
	auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::Both(-9, -9, -9, -9));
	auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI_Editbox_select.png"), Layout::Both(-9, -9, -9, -9));
	auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_Editbox_pressed.png"), Layout::Both(-9, -9, -9, -9));
	auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI_Down_icn.png"),
									   Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-20, -10), Vec2F(0, 10)));

	sample->SetClippingLayout(Layout::Both(4, 2, 20, 2));

	auto list = sample->GetListView();
	*list = *o2UI.GetWidgetStyle<UICustomList>("standard");
	list->layer["back"]->drawable.Release();
	list->layer["back"]->drawable = mnew Sprite("ui/UI_Box_regular.png");
	list->layout.pivot = Vec2F(0.5f, 1.0f);
	list->layout.anchorMin = Vec2F(0, 0);
	list->layout.anchorMax = Vec2F(1, 0);
	list->layout.offsetMin = Vec2F(-1, -60);
	list->layout.offsetMax = Vec2F(0, 3);

	Ptr<UILabel> itemSample = o2UI.CreateLabel("empty");
	itemSample->horAlign = HorAlign::Left;
	sample->SetItemSample(itemSample);

	sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("opened", Animation::EaseInOut(sample, &arrowLayer->drawable->scale, Vec2F(1, 1), Vec2F(1, -1), 0.2f));

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildWindowStyle()
{
	Ptr<UIWindow> sample = mnew UIWindow();

	sample->layout.minSize = Vec2F(100, 50);

	auto regularBackLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_window_frame_regular.png"),
											 Layout::Both(-12, -12, -14, -14));

	auto iconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI_o2_sign.png"),
									  Layout(Vec2F(0.0f, 1.0f), Vec2F(0.0f, 1.0f), Vec2F(3, -14), Vec2F(17, 1)));

	Ptr<Text> captionText = mnew Text("arial.ttf");
	captionText->text = "Window";
	captionText->horAlign = HorAlign::Left;
	captionText->verAlign = VerAlign::Middle;
	captionText->dotsEngings = true;
	auto textLayer = sample->AddLayer("caption", captionText,
									  Layout(Vec2F(0.0f, 1.0f), Vec2F(1.0f, 1.0f), Vec2F(19, -13), Vec2F(-28, 3)));

	sample->SetClippingLayout(Layout::Both(0, 1, -1, 15));
	sample->SetViewLayout(Layout::Both(5, 5, 5, 20));
	sample->SetEnableScrollsHiding(true);

	Ptr<UIHorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
	horScrollBar->layout.anchorMin = Vec2F(0, 0);
	horScrollBar->layout.anchorMax = Vec2F(1, 0);
	horScrollBar->layout.offsetMin = Vec2F(5, 0);
	horScrollBar->layout.offsetMax = Vec2F(-15, 15);
	sample->SetHorizontalScrollBar(horScrollBar);

	Ptr<UIVerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
	verScrollBar->layout.anchorMin = Vec2F(1, 0);
	verScrollBar->layout.anchorMax = Vec2F(1, 1);
	verScrollBar->layout.offsetMin = Vec2F(-15, 15);
	verScrollBar->layout.offsetMax = Vec2F(0, -20);
	sample->SetVerticalScrollBar(verScrollBar);

	sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout.offsetBottom,
														  5.0f, 15.0f, 0.2f));

	sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout.offsetBottom,
														  -5.0f, -15.0f, 0.2f));

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	Ptr<UIButton> closeBtn = o2UI.CreateWidget<UIButton>("close");
	closeBtn->name = "closeButton";
	closeBtn->layout.anchorMin = Vec2F(1, 1);
	closeBtn->layout.anchorMax = Vec2F(1, 1);
	closeBtn->layout.offsetMin = Vec2F(-19, -15);
	closeBtn->layout.offsetMax = Vec2F(1, 3);
	sample->AddWindowElement(closeBtn);

	Ptr<UIButton> optionsBtn = o2UI.CreateWidget<UIButton>("arrow");
	optionsBtn->name = "optionsButton";
	optionsBtn->layout.anchorMin = Vec2F(1, 1);
	optionsBtn->layout.anchorMax = Vec2F(1, 1);
	optionsBtn->layout.offsetMin = Vec2F(-34, -15);
	optionsBtn->layout.offsetMax = Vec2F(-16, 3);
	sample->AddWindowElement(optionsBtn);

	sample->SetDragAreaLayouts(Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(5, -15), Vec2F(-5, -2)),    // head
							   Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(5, -2), Vec2F(-5, 5)),      // top
							   Layout(Vec2F(0, 0), Vec2F(1, 0), Vec2F(5, -5), Vec2F(-5, 5)),      // bottom
							   Layout(Vec2F(0, 0), Vec2F(0, 1), Vec2F(-5, 5), Vec2F(5, -5)),      // left
							   Layout(Vec2F(1, 0), Vec2F(1, 1), Vec2F(-5, 5), Vec2F(5, -5)),      // right
							   Layout(Vec2F(0, 1), Vec2F(0, 1), Vec2F(-5, -5), Vec2F(5, 5)),      // left top
							   Layout(Vec2F(1, 1), Vec2F(1, 1), Vec2F(-5, -5), Vec2F(5, 5)),      // right top
							   Layout(Vec2F(0, 0), Vec2F(0, 0), Vec2F(-5, -5), Vec2F(5, 5)),      // left bottom 
							   Layout(Vec2F(1, 0), Vec2F(1, 0), Vec2F(-5, -5), Vec2F(5, 5)));     // right bottom

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildContextMenuStyle()
{
	Ptr<UIContextMenu> sample = mnew UIContextMenu();

	sample->SetClippingLayout(Layout::Both(1, 2, 1, 1));
	sample->SetViewLayout(Layout::Both(5, 5, 5, 5));
	sample->SetEnableScrollsHiding(true);

	sample->AddLayer("back", mnew Sprite("ui/UI_Context_menu.png"), Layout::Both(-19, -19, -20, -20));

	Ptr<Sprite> selection = mnew Sprite("ui/UI_ListBox_selection_regular.png");
	*sample->GetSelectionDrawable() = *selection;
	sample->SetSelectionDrawableLayout(Layout::Both(-10, -16, -10, -16));

	Ptr<UIWidget> itemSample = sample->GetItemSample();

	Ptr<UIWidgetLayer> captionLayer = itemSample->GetLayer("caption");
	Ptr<Text> captionLayerText = mnew Text("arial.ttf");
	captionLayer->drawable = captionLayerText;
	captionLayerText->horAlign = HorAlign::Left;
	captionLayerText->verAlign = VerAlign::Middle;

	Ptr<UIWidgetLayer> shortcutLayer = itemSample->GetLayer("shortcut");
	Ptr<Text> shortcutLayerText = mnew Text("arial.ttf");
	shortcutLayer->drawable = shortcutLayerText;
	shortcutLayerText->horAlign = HorAlign::Right;
	shortcutLayerText->verAlign = VerAlign::Middle;

	Ptr<UIWidgetLayer> subIconLayer = itemSample->GetLayer("subIcon");
	subIconLayer->drawable = mnew Sprite("ui/UI_Right_icn.png");
	subIconLayer->layout = Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-15, 10), Vec2F(5, -10));

	Ptr<UIHorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
	horScrollBar->layout.anchorMin = Vec2F(0, 0);
	horScrollBar->layout.anchorMax = Vec2F(1, 0);
	horScrollBar->layout.offsetMin = Vec2F(5, 0);
	horScrollBar->layout.offsetMax = Vec2F(-15, 15);
	sample->SetHorizontalScrollBar(horScrollBar);

	Ptr<UIVerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
	verScrollBar->layout.anchorMin = Vec2F(1, 0);
	verScrollBar->layout.anchorMax = Vec2F(1, 1);
	verScrollBar->layout.offsetMin = Vec2F(-15, 15);
	verScrollBar->layout.offsetMax = Vec2F(0, -5);
	sample->SetVerticalScrollBar(verScrollBar);

	sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout.offsetBottom,
														  5.0f, 15.0f, 0.2f));

	sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout.offsetBottom,
														  -5.0f, -15.0f, 0.2f));

	sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetSelectionDrawable()->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f));

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildTreeStyle()
{
	Ptr<UITree> sample = mnew UITree();
	sample->layout.minSize = Vec2F(20, 20);
	sample->SetClippingLayout(Layout::Both(1, 2, 1, 1));
	sample->SetViewLayout(Layout::Both(5, 5, 5, 5));
	sample->SetEnableScrollsHiding(true);

	*sample->GetSelectionDrawable() = Sprite("ui/UI_ListBox_selection_regular.png");
	sample->SetSelectionDrawableLayout(Layout::Both(-10, -16, -10, -16));

	Ptr<UITreeNode> itemSample = sample->GetNodeSample();

	Ptr<Text> captionLayerText = mnew Text("arial.ttf");
	captionLayerText->horAlign = HorAlign::Left;
	captionLayerText->verAlign = VerAlign::Middle;
	itemSample->AddLayer("name", captionLayerText, Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(12, -20), Vec2F(0, 0)));

	itemSample->SetChildrenOffset(15);

	Ptr<UIButton> itemSampleExpandBtn = mnew UIButton();
	itemSampleExpandBtn->layout.minSize = Vec2F(5, 5);
	itemSampleExpandBtn->name = "expandBtn";

	auto regularLayer = itemSampleExpandBtn->AddLayer("regular", mnew Sprite("ui/UI_Right_icn.png"),
										 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

	auto selectLayer = itemSampleExpandBtn->AddLayer("select", mnew Sprite("ui/UI_Right_icn_select.png"),
										Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

	auto pressedLayer = itemSampleExpandBtn->AddLayer("pressed", mnew Sprite("ui/UI_Right_icn_pressed.png"),
										 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


	itemSampleExpandBtn->AddState("select", Animation::EaseInOut(itemSampleExpandBtn, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
		->offStateAnimationSpeed = 1.0f / 4.0f;

	itemSampleExpandBtn->AddState("pressed", Animation::EaseInOut(itemSampleExpandBtn, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
		->offStateAnimationSpeed = 0.5f;

	itemSampleExpandBtn->AddState("visible", Animation::EaseInOut(itemSampleExpandBtn, &itemSampleExpandBtn->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	itemSampleExpandBtn->layout.anchorMin = Vec2F(0, 1);
	itemSampleExpandBtn->layout.anchorMax = Vec2F(0, 1);
	itemSampleExpandBtn->layout.offsetMin = Vec2F(0, -20);
	itemSampleExpandBtn->layout.offsetMax = Vec2F(10, 0);

	itemSample->AddChild(itemSampleExpandBtn);

	Animation expandedStateAnim = Animation::EaseInOut(itemSample, "mExpandCoef", 0.0f, 1.0f, 0.2f);
	*expandedStateAnim.AddAnimationValue(&regularLayer->drawable->angle) =
		AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);
	*expandedStateAnim.AddAnimationValue(&selectLayer->drawable->angle) =
		AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);
	*expandedStateAnim.AddAnimationValue(&pressedLayer->drawable->angle) =
		AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

	itemSample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;

	Ptr<UIHorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
	horScrollBar->layout.anchorMin = Vec2F(0, 0);
	horScrollBar->layout.anchorMax = Vec2F(1, 0);
	horScrollBar->layout.offsetMin = Vec2F(5, 0);
	horScrollBar->layout.offsetMax = Vec2F(-15, 15);
	sample->SetHorizontalScrollBar(horScrollBar);

	Ptr<UIVerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
	verScrollBar->layout.anchorMin = Vec2F(1, 0);
	verScrollBar->layout.anchorMax = Vec2F(1, 1);
	verScrollBar->layout.offsetMin = Vec2F(-15, 15);
	verScrollBar->layout.offsetMax = Vec2F(0, -5);
	sample->SetVerticalScrollBar(verScrollBar);

	sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout.offsetRight,
														  5.0f, 15.0f, 0.2f));

	sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout.offsetRight,
														  -5.0f, -15.0f, 0.2f));

	sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetSelectionDrawable()->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5f;

	sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
		->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");
}

void RebuildUIStyle()
{
	o2UI.ClearStyle();

	RebuildButtonStyle();
	RebuildCloseButtonStyle();
	RebuildArrowButtonStyle();
	RebuildHorProgressBarStyle();
	RebuildVerProgressBarStyle();
	RebuildHorScrollBarStyle();
	RebuildVerScrollBarStyle();
	RebuildCheckboxStyle();
	RebuildScrollAreaStyle();
	RebuildScrollAreaStraightBarsStyle();
	RebuildLabelStyle();
	RebuildEditBoxStyle();
	RebuildCustomListStyle();
	RebuildListStyle();
	RebuildCustomDropDownStyle();
	RebuildDropDownStyle();
	RebuildWindowStyle();
	RebuildContextMenuStyle();
	RebuildTreeStyle();

	o2UI.SaveStyle("ui_style.xml");
}