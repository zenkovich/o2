#include "stdafx.h"
#include "BasicUIStyle.h"

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
#include "UI/LongList.h"
#include "UI/MenuPanel.h"
#include "UI/ScrollArea.h"
#include "UI/Spoiler.h"
#include "UI/Toggle.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "UI/VerticalProgress.h"
#include "UI/VerticalScrollBar.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "UI/Window.h"

namespace o2
{
	void BasicUIStyleBuilder::RebuildButtonStyle()
	{
		UIButton* sample = mnew UIButton();
		sample->layout->minSize = Vec2F(20, 20);

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI_button_regular.png"),
											 Layout::BothStretch(-9, -9, -10, -10));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_button_select.png"),
										   Layout::BothStretch(-9, -9, -10, -10));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_button_pressed.png"),
											 Layout::BothStretch(-9, -9, -10, -10));

		auto focusLayer = sample->AddLayer("focus", mnew Sprite("ui/UI_button_focus.png"),
										   Layout::BothStretch(-9, -9, -10, -10));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "Button";
		captionText->horAlign = HorAlign::Middle;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		captionText->wordWrap = true;
		sample->AddLayer("caption", captionText);

		sample->AddState("hover", Animation::EaseInOut(sample, &hoverLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCloseButtonStyle()
	{
		UIButton* sample = mnew UIButton();
		sample->layout->minSize = Vec2F(5, 5);

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI_Close_button_regular.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Close_button_select.png"),
										   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_Close_button_pressed.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));


		sample->AddState("hover", Animation::EaseInOut(sample, &hoverLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "close");
	}

	void BasicUIStyleBuilder::RebuildArrowButtonStyle()
	{
		UIButton* sample = mnew UIButton();
		sample->layout->minSize = Vec2F(5, 5);

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI_Options_button_regular.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Options_button_select.png"),
										   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_Options_button_pressed.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));


		sample->AddState("hover", Animation::EaseInOut(sample, &hoverLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "arrow");
	}

	void BasicUIStyleBuilder::RebuildHorProgressBarStyle()
	{
		UIHorizontalProgress* sample = mnew UIHorizontalProgress();
		sample->layout->minSize = Vec2F(5, 5);
		auto backLayer = sample->AddLayer("back", nullptr);
		auto spriteBackLayer = backLayer->AddChildLayer("sprite", mnew Sprite("ui/UI_Hor_scrollbar_bk.png"),
														Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															   Vec2F(-2, -4), Vec2F(2, 5)));

		backLayer->interactableLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -4), Vec2F(0, 4));

		auto barLayer = sample->AddLayer("bar", nullptr);
		auto barRegularSprite = barLayer->AddChildLayer("regular", mnew Sprite("ui/UI_Hor_scrollhandle_regular.png"),
														Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															   Vec2F(-2, -4), Vec2F(2, 5)));

		auto barSelectSprite = barLayer->AddChildLayer("hover", mnew Sprite("ui/UI_Hor_scrollhandle_select.png"),
													   Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															  Vec2F(-2, -4), Vec2F(2, 5)));

		auto barPressedSprite = barLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_Hor_scrollhandle_pressed.png"),
														Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															   Vec2F(-2, -4), Vec2F(2, 5)));

		sample->AddState("hover", Animation::EaseInOut(sample, &barSelectSprite->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &barPressedSprite->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->SetOrientation(UIHorizontalProgress::Orientation::Right);

		o2UI.AddWidgetStyle(sample, "standard");
	}


	void BasicUIStyleBuilder::RebuildVerProgressBarStyle()
	{
		UIVerticalProgress* sample = mnew UIVerticalProgress();
		sample->layout->minSize = Vec2F(5, 5);
		auto backLayer = sample->AddLayer("back", nullptr);
		auto spriteBackLayer = backLayer->AddChildLayer("sprite", mnew Sprite("ui/UI_Ver_scrollbar_bk.png"),
														Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															   Vec2F(-4, -2), Vec2F(5, 2)));
		backLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));

		auto barLayer = sample->AddLayer("bar", nullptr);
		auto barRegularSprite = barLayer->AddChildLayer("regular", mnew Sprite("ui/UI_Ver_scrollhandle_regular.png"),
														Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															   Vec2F(-4, -2), Vec2F(5, 2)));

		auto barSelectSprite = barLayer->AddChildLayer("hover", mnew Sprite("ui/UI_Ver_scrollhandle_select.png"),
													   Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															  Vec2F(-4, -2), Vec2F(5, 2)));

		auto barPressedSprite = barLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_Ver_scrollhandle_pressed.png"),
														Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															   Vec2F(-4, -2), Vec2F(5, 2)));


		sample->AddState("hover", Animation::EaseInOut(sample, &barSelectSprite->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &barPressedSprite->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildHorScrollBarStyle()
	{
		UIHorizontalScrollBar* sample = mnew UIHorizontalScrollBar();
		sample->layout->minSize = Vec2F(5, 5);
		sample->SetScrollSense(0.25f);
		sample->SetMinimalScrollHandleSize(10);

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

		auto barSelectSprite = handleLayer->AddChildLayer("hover", mnew Sprite("ui/UI_Hor_scrollhandle_select.png"),
														  Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
																 Vec2F(-2, -4), Vec2F(2, 5)));

		auto barPressedSprite = handleLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_Hor_scrollhandle_pressed.png"),
														   Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
																  Vec2F(-2, -4), Vec2F(2, 5)));

		sample->AddState("hover", Animation::EaseInOut(sample, &barSelectSprite->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &barPressedSprite->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildVerScrollBarStyle()
	{
		UIVerticalScrollBar* sample = mnew UIVerticalScrollBar();
		sample->layout->minSize = Vec2F(5, 5);
		sample->SetScrollSense(0.25f);
		sample->SetMinimalScrollHandleSize(10);


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

		auto barSelectSprite = handleLayer->AddChildLayer("hover", mnew Sprite("ui/UI_Ver_scrollhandle_select.png"),
														  Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
																 Vec2F(-4, -2), Vec2F(5, 2)));

		auto barPressedSprite = handleLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_Ver_scrollhandle_pressed.png"),
														   Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
																  Vec2F(-4, -2), Vec2F(5, 2)));

		sample->AddState("hover", Animation::EaseInOut(sample, &barSelectSprite->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &barPressedSprite->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCheckboxStyle()
	{
		UIToggle* sample = mnew UIToggle();
		sample->layout->minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Check_bk.png"),
										  Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

		auto hoverLayer = sample->AddLayer("backSelect", mnew Sprite("ui/UI_Check_bk_select.png"),
										   Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

		auto pressedLayer = sample->AddLayer("backPressed", mnew Sprite("ui/UI_Check_bk_pressed.png"),
											 Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

		auto focusLayer = sample->AddLayer("backFocus", mnew Sprite("ui/UI_Check_bk_focus.png"),
										   Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

		auto checkLayer = sample->AddLayer("check", mnew Sprite("ui/UI_Ckeck.png"),
										   Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(1, -11), Vec2F(21, 10)));

		auto unknownLayer = sample->AddLayer("unknown", mnew Sprite("ui/UI_Check_unknown.png"),
											 Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(1, -11), Vec2F(21, 10)));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "Checkbox";
		captionText->horAlign = HorAlign::Left;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		sample->AddLayer("caption", captionText, Layout(Vec2F(0, 0), Vec2F(1, 1), Vec2F(20, 0), Vec2F(0, 0)));

		sample->AddState("hover", Animation::EaseInOut(sample, &hoverLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("value", Animation::EaseInOut(sample, &checkLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("unknown", Animation::EaseInOut(sample, &unknownLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCheckboxWithoutCaptionStyle()
	{
		UIToggle* sample = mnew UIToggle();
		sample->layout->minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Check_bk.png"),
										  Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		auto hoverLayer = sample->AddLayer("backSelect", mnew Sprite("ui/UI_Check_bk_select.png"),
										   Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		auto pressedLayer = sample->AddLayer("backPressed", mnew Sprite("ui/UI_Check_bk_pressed.png"),
											 Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		auto focusLayer = sample->AddLayer("backFocus", mnew Sprite("ui/UI_Check_bk_focus.png"),
										   Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		auto checkLayer = sample->AddLayer("check", mnew Sprite("ui/UI_Ckeck.png"),
										   Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		auto unknownLayer = sample->AddLayer("unknown", mnew Sprite("ui/UI_Check_unknown.png"),
											 Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		sample->AddState("hover", Animation::EaseInOut(sample, &hoverLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("value", Animation::EaseInOut(sample, &checkLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("unknown", Animation::EaseInOut(sample, &unknownLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "without caption");
	}

	void BasicUIStyleBuilder::RebuildScrollAreaStraightBarsStyle()
	{
		UIScrollArea* sample = mnew UIScrollArea();
		sample->layout->minSize = Vec2F(30, 30);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(false);
		sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		Animation enableHorScrollAnim;
		enableHorScrollAnim.SetTarget(sample);
		*enableHorScrollAnim.AddAnimationValue(&sample->GetVerticalScrollbar()->layout->offsetBottom) =
			AnimatedValue<float>::EaseInOut(5, 15, 0.2f);
		*enableHorScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMin") =
			AnimatedValue<Vec2F>::EaseInOut(Vec2F(5, 5), Vec2F(5, 15), 0.2f);

		auto enableHorScrollState = sample->AddState("enableHorBar", enableHorScrollAnim);

		Animation enableVerScrollAnim;
		enableVerScrollAnim.SetTarget(sample);
		*enableVerScrollAnim.AddAnimationValue(&sample->GetHorizontalScrollbar()->layout->offsetRight) =
			AnimatedValue<float>::EaseInOut(-5, -15, 0.2f);
		*enableVerScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMax") =
			AnimatedValue<Vec2F>::EaseInOut(Vec2F(-5, -5), Vec2F(-15, -5), 0.2f);

		auto enableVerScrollState = sample->AddState("enableVerBar", enableVerScrollAnim);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "straight bars");
	}

	void BasicUIStyleBuilder::RebuildScrollAreaStyle()
	{
		UIScrollArea* sample = mnew UIScrollArea();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);
		sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildLabelStyle()
	{
		UILabel* sample = mnew UILabel();
		sample->layout->minSize = Vec2F(20, 20);
		sample->horOverflow = UILabel::HorOverflow::Dots;
		sample->verOverflow = UILabel::VerOverflow::None;

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "Checkbox";
		captionText->horAlign = HorAlign::Middle;
		captionText->verAlign = VerAlign::Middle;
		sample->AddLayer("text", captionText);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildEditBoxStyle()
	{
		UIEditBox* sample = mnew UIEditBox();
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(false);
		sample->SetCaretBlinkingDelay(0.85f);
		sample->layout->minSize = Vec2F(30, 40);

		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI_Editbox_focus.png"), Layout::BothStretch(-9, -9, -9, -9));

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		Animation enableHorScrollAnim;
		enableHorScrollAnim.SetTarget(sample);
		*enableHorScrollAnim.AddAnimationValue(&sample->GetVerticalScrollbar()->layout->offsetBottom) =
			AnimatedValue<float>::EaseInOut(5, 15, 0.2f);
		*enableHorScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMin") =
			AnimatedValue<Vec2F>::EaseInOut(Vec2F(5, 5), Vec2F(5, 15), 0.2f);

		auto enableHorScrollState = sample->AddState("enableHorBar", enableHorScrollAnim);

		Animation enableVerScrollAnim;
		enableVerScrollAnim.SetTarget(sample);
		*enableVerScrollAnim.AddAnimationValue(&sample->GetHorizontalScrollbar()->layout->offsetRight) =
			AnimatedValue<float>::EaseInOut(-5, -15, 0.2f);
		*enableVerScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMax") =
			AnimatedValue<Vec2F>::EaseInOut(Vec2F(-5, -5), Vec2F(-15, -5), 0.2f);

		auto enableVerScrollState = sample->AddState("enableVerBar", enableVerScrollAnim);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		Text* textDrawable = sample->GetTextDrawable();
		textDrawable->verAlign = VerAlign::Top;
		textDrawable->horAlign = HorAlign::Left;
		textDrawable->SetFontAsset("stdFont.ttf");

		Sprite* caretDrawable = sample->GetCaretDrawable();
		*caretDrawable = Sprite();
		caretDrawable->size = Vec2F(1, textDrawable->GetFont()->GetHeightPx(textDrawable->GetHeight())*1.7f);
		caretDrawable->pivot = Vec2F(0, 0.16f);
		caretDrawable->color = Color4::Black();

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCustomListStyle()
	{
		UICustomList* sample = mnew UICustomList();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);
		sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));

		Sprite* selection = mnew Sprite("ui/UI_Context_menu_select.png");
		*sample->GetSelectionDrawable() = *selection;
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-12, -16, -10, -16));

		Sprite* hover = mnew Sprite("ui/UI_ListBox_selection_hover.png");
		*sample->GetHoverDrawable() = *hover;
		sample->SetHoverDrawableLayout(Layout::BothStretch(-12, -16, -10, -16));

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout->offsetBottom,
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout->offsetRight,
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetHoverDrawable()->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildLongListStyle()
	{
		UILongList* sample = mnew UILongList();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);

		Sprite* selection = mnew Sprite("ui/UI_Context_menu_select.png");
		*sample->GetSelectionDrawable() = *selection;
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-12, -16, -10, -16));

		Sprite* hover = mnew Sprite("ui/UI_ListBox_selection_hover.png");
		*sample->GetHoverDrawable() = *hover;
		sample->SetHoverDrawableLayout(Layout::BothStretch(-12, -16, -10, -16));

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout->offsetBottom,
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout->offsetRight,
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetHoverDrawable()->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &sample->GetSelectionDrawable()->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildListStyle()
	{
		UIList* sample = mnew UIList();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);
		sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));

		Sprite* selection = mnew Sprite("ui/UI_Context_menu_select.png");
		*sample->GetSelectionDrawable() = *selection;
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

		Sprite* hover = mnew Sprite("ui/UI_ListBox_selection_hover.png");
		*sample->GetHoverDrawable() = *hover;
		sample->SetHoverDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

		UILabel* itemSample = o2UI.CreateLabel("empty");
		itemSample->layout->size = Vec2F(20, 0);
		itemSample->horAlign = HorAlign::Left;
		itemSample->verAlign = VerAlign::Middle;
		sample->SetItemSample(itemSample);

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout->offsetBottom,
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout->offsetRight,
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetHoverDrawable()->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &sample->GetSelectionDrawable()->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCustomDropDownStyle()
	{
		UICustomDropDown* sample = mnew UICustomDropDown();
		sample->layout->minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_Editbox_pressed.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI_Down_icn.png"),
										   Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-20, -10), Vec2F(0, 10)));

		sample->SetClippingLayout(Layout::BothStretch(4, 2, 20, 2));

		UIWidget* itemSample = mnew UIWidget();
		itemSample->layout->size = Vec2F(20, 20);
		sample->SetItemSample(itemSample);

		auto list = sample->GetListView();
		*list = *o2UI.GetWidgetStyle<UICustomList>("standard");
		list->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		delete list->layer["back"]->drawable;
		list->layer["back"]->drawable = mnew Sprite("ui/UI_Box_regular.png");
		list->layout->pivot = Vec2F(0.5f, 1.0f);
		list->layout->anchorMin = Vec2F(0, 0);
		list->layout->anchorMax = Vec2F(1, 0);
		list->layout->offsetMin = Vec2F(-1, -60);
		list->layout->offsetMax = Vec2F(0, 3);

		sample->AddState("hover", Animation::EaseInOut(sample, &hoverLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("opened", Animation::EaseInOut(sample, &arrowLayer->drawable->scale, Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildDropDownStyle()
	{
		UIDropDown* sample = mnew UIDropDown();
		sample->layout->minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"),
										  Layout::BothStretch(-9, -9, -9, -9));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"),
										   Layout::BothStretch(-9, -9, -9, -9));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_Editbox_pressed.png"),
											 Layout::BothStretch(-9, -9, -9, -9));

		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI_Down_icn.png"),
										   Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-20, -10), Vec2F(0, 10)));

		sample->SetClippingLayout(Layout::BothStretch(4, 2, 20, 2));

		auto list = sample->GetListView();
		*list = *o2UI.GetWidgetStyle<UICustomList>("standard");
		list->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		delete list->layer["back"]->drawable;
		list->layer["back"]->drawable = mnew Sprite("ui/UI_Box_regular.png");
		list->layout->pivot = Vec2F(0.5f, 1.0f);
		list->layout->anchorMin = Vec2F(0, 0);
		list->layout->anchorMax = Vec2F(1, 0);
		list->layout->offsetMin = Vec2F(2, -60);
		list->layout->offsetMax = Vec2F(0, 3);

		UILabel* itemSample = o2UI.CreateLabel("empty");
		itemSample->horAlign = HorAlign::Left;
		sample->SetItemSample(itemSample);

		sample->AddState("hover", Animation::EaseInOut(sample, &hoverLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("opened", Animation::EaseInOut(sample, &arrowLayer->drawable->scale, Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");


	}

	void BasicUIStyleBuilder::RebuildWindowStyle()
	{
		UIWindow* sample = mnew UIWindow();

		sample->layout->minSize = Vec2F(100, 50);

		auto regularBackLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_window_frame_regular.png"),
												 Layout::BothStretch(-12, -12, -14, -14));

		auto iconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI_o2_sign.png"),
										  Layout(Vec2F(0.0f, 1.0f), Vec2F(0.0f, 1.0f), Vec2F(3, -14), Vec2F(17, 1)));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "Window";
		captionText->horAlign = HorAlign::Left;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		auto textLayer = sample->AddLayer("caption", captionText,
										  Layout(Vec2F(0.0f, 1.0f), Vec2F(1.0f, 1.0f), Vec2F(19, -13), Vec2F(-28, 3)));

		sample->SetClippingLayout(Layout::BothStretch(0, 1, -1, 15));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 20));
		sample->SetEnableScrollsHiding(true);

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		*horScrollBar->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, 5, 15, 15, -5);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		*verScrollBar->layout = UIWidgetLayout::VerStretch(HorAlign::Right, 0, 15, 15, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 2.0f;

		UIButton* closeBtn = o2UI.CreateWidget<UIButton>("close");
		closeBtn->name = "closeButton";
		closeBtn->layout->anchorMin = Vec2F(1, 1);
		closeBtn->layout->anchorMax = Vec2F(1, 1);
		closeBtn->layout->offsetMin = Vec2F(-19, -15);
		closeBtn->layout->offsetMax = Vec2F(1, 3);
		sample->AddWindowElement(closeBtn);

		UIButton* optionsBtn = o2UI.CreateWidget<UIButton>("arrow");
		optionsBtn->name = "optionsButton";
		optionsBtn->layout->anchorMin = Vec2F(1, 1);
		optionsBtn->layout->anchorMax = Vec2F(1, 1);
		optionsBtn->layout->offsetMin = Vec2F(-34, -15);
		optionsBtn->layout->offsetMax = Vec2F(-16, 3);
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

	void BasicUIStyleBuilder::RebuildContextMenuStyle()
	{
		UIContextMenu* sample = mnew UIContextMenu();

		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		sample->SetEnableScrollsHiding(true);
		sample->SetMinFitSize(50);

		sample->AddLayer("back", mnew Sprite("ui/UI_Context_menu.png"), Layout::BothStretch(-20, -19, -20, -19));

		Sprite* selection = mnew Sprite("ui/UI_ListBox_selection_hover.png");
		*sample->GetSelectionDrawable() = *selection;
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

		UIWidget* separatorSample = sample->GetSeparatorSample();
		separatorSample->AddLayer("line", mnew Sprite("ui/UI_Separator.png"),
								  Layout::HorStretch(VerAlign::Middle, 0, 0, 5, 0));

		UIWidget* itemSample = sample->GetItemSample();

		UIWidgetLayer* captionLayer = itemSample->FindLayer("caption");
		Text* captionLayerText = mnew Text("stdFont.ttf");
		captionLayer->drawable = captionLayerText;
		captionLayer->layout = Layout::BothStretch(20, 1, 0, -1);
		captionLayerText->horAlign = HorAlign::Left;
		captionLayerText->verAlign = VerAlign::Middle;

		UIWidgetLayer* shortcutLayer = itemSample->FindLayer("shortcut");
		Text* shortcutLayerText = mnew Text("stdFont.ttf");
		shortcutLayer->layout = Layout::BothStretch(20, 1, 10, -1);
		shortcutLayer->drawable = shortcutLayerText;
		shortcutLayerText->horAlign = HorAlign::Right;
		shortcutLayerText->verAlign = VerAlign::Middle;
		shortcutLayer->transparency = 0.7f;

		UIWidgetLayer* subIconLayer = itemSample->FindLayer("subIcon");
		subIconLayer->drawable = mnew Sprite("ui/UI_Right_icn.png");
		subIconLayer->layout = Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-15, 10), Vec2F(5, -10));

		itemSample->AddLayer("check", mnew Sprite("ui/UI_Ckeck.png"),
							 Layout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(0, 0)));

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout->offsetBottom,
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout->offsetRight,
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetSelectionDrawable()->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f));

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildMenuPanelStyle()
	{
		UIMenuPanel* sample = mnew UIMenuPanel();

		sample->AddLayer("back", mnew Sprite("ui/UI_Up_Menu.png"), Layout::BothStretch(-15, -15, -15, -17));

		Sprite* selection = mnew Sprite("ui/UI_ListBox_selection_hover.png");
		*sample->GetSelectionDrawable() = *selection;
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

		auto itemSample = o2UI.CreateLabel("unknown");
		itemSample->horOverflow = UILabel::HorOverflow::Expand;
		itemSample->expandBorder = Vec2F(10, 0);
		sample->SetItemSample(itemSample);

		sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetSelectionDrawable()->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f));

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildTreeStyle()
	{
		UITree* sample = mnew UITree();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);
		sample->SetChildsNodesOffset(10.0f);

		*sample->GetHoverDrawable() = Sprite("ui/UI_Context_menu_white.png");

		UITreeNode* itemSample = sample->GetNodeSample();

		auto itemSelectionLayer = itemSample->AddLayer("select", nullptr);

		auto itemFocusedLayer = itemSelectionLayer->AddChildLayer("focused", mnew Sprite("ui/UI_Context_menu_select.png"),
																  Layout::BothStretch(-10, -16, -10, -16));

		auto itemUnfocusedLayer = itemSelectionLayer->AddChildLayer("unfocused", mnew Sprite("ui/UI_ListBox_selection_hover.png"),
																	Layout::BothStretch(-10, -16, -10, -16));

		Text* captionLayerText = mnew Text("stdFont.ttf");
		captionLayerText->horAlign = HorAlign::Left;
		captionLayerText->verAlign = VerAlign::Middle;
		itemSample->AddLayer("name", captionLayerText, Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(12, -20), Vec2F(0, 0)));

		UIButton* itemSampleExpandBtn = mnew UIButton();
		itemSampleExpandBtn->layout->minSize = Vec2F(5, 5);
		itemSampleExpandBtn->name = "expandBtn";

		auto regularLayer = itemSampleExpandBtn->AddLayer("regular", mnew Sprite("ui/UI_Right_icn.png"),
														  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto hoverLayer = itemSampleExpandBtn->AddLayer("hover", mnew Sprite("ui/UI_Right_icn_select.png"),
														Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = itemSampleExpandBtn->AddLayer("pressed", mnew Sprite("ui/UI_Right_icn_pressed.png"),
														  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


		itemSampleExpandBtn->AddState("hover", Animation::EaseInOut(itemSampleExpandBtn, &hoverLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		itemSampleExpandBtn->AddState("pressed", Animation::EaseInOut(itemSampleExpandBtn, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		itemSampleExpandBtn->AddState("visible", Animation::EaseInOut(itemSampleExpandBtn, &itemSampleExpandBtn->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		itemSampleExpandBtn->layout->anchorMin = Vec2F(0, 1);
		itemSampleExpandBtn->layout->anchorMax = Vec2F(0, 1);
		itemSampleExpandBtn->layout->offsetMin = Vec2F(0, -20);
		itemSampleExpandBtn->layout->offsetMax = Vec2F(10, 0);

		itemSample->AddChild(itemSampleExpandBtn);

		Animation expandedStateAnim(itemSample);
		*expandedStateAnim.AddAnimationValue(&regularLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);
		*expandedStateAnim.AddAnimationValue(&hoverLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);
		*expandedStateAnim.AddAnimationValue(&pressedLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		itemSample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;

		itemSample->AddState("selected", Animation::EaseInOut(itemSample, &itemSelectionLayer->transparency, 0.0f, 1.0f, 0.2f));

		Animation focusedItemAnim = Animation::EaseInOut(itemSample, &itemFocusedLayer->transparency, 0.0f, 1.0f, 0.2f);
		*focusedItemAnim.AddAnimationValue(&itemUnfocusedLayer->transparency) = AnimatedValue<float>::EaseInOut(1.0f, 0.0f, 0.2f);
		itemSample->AddState("focused", focusedItemAnim);

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout->offsetBottom,
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout->offsetRight,
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetHoverDrawable()->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildBacklessDropdown()
	{
		UIDropDown* sample = mnew UIDropDown();
		sample->layout->minSize = Vec2F(20, 20);
		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI_Down_icn.png"),
										   Layout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, -1)));

		sample->SetClippingLayout(Layout::BothStretch(4, 2, 20, 2));

		auto list = sample->GetListView();
		*list = *o2UI.GetWidgetStyle<UICustomList>("standard");

		list->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		list->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));

		list->RemoveLayer("back");
		list->AddLayer("back", mnew Sprite("ui/UI_Context_menu.png"), Layout::BothStretch(-22, -19, -19, -19));

		list->layout->pivot = Vec2F(0.5f, 1.0f);
		list->layout->anchorMin = Vec2F(0, 0);
		list->layout->anchorMax = Vec2F(1, 0);
		list->layout->offsetMin = Vec2F(0, -60);
		list->layout->offsetMax = Vec2F(0, 0);

		UILabel* itemSample = o2UI.CreateLabel("empty");
		itemSample->horAlign = HorAlign::Left;
		sample->SetItemSample(itemSample);

		sample->AddState("opened", Animation::EaseInOut(sample, &arrowLayer->drawable->scale, Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "backless");
	}

	void BasicUIStyleBuilder::RebuildBacklessEditbox()
	{
		UIEditBox* sample = mnew UIEditBox();
		sample->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));
		sample->SetViewLayout(Layout::BothStretch(0, 0, 2, 0));
		sample->SetCaretBlinkingDelay(0.85f);
		sample->SetMultiLine(false);
		sample->layout->minSize = Vec2F(30, 40);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		Text* textDrawable = sample->GetTextDrawable();
		textDrawable->verAlign = VerAlign::Middle;
		textDrawable->horAlign = HorAlign::Left;
		textDrawable->SetFontAsset("stdFont.ttf");

		Sprite* caretDrawable = sample->GetCaretDrawable();
		*caretDrawable = Sprite();
		caretDrawable->size = Vec2F(1, textDrawable->GetFont()->GetHeightPx(textDrawable->GetHeight())*1.7f);
		caretDrawable->pivot = Vec2F(0, 0.26f);
		caretDrawable->color = Color4::Black();

		o2UI.AddWidgetStyle(sample, "backless");
	}

	void BasicUIStyleBuilder::RebuildSinglelineEditbox()
	{
		UIEditBox* sample = mnew UIEditBox();
		sample->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));
		sample->SetViewLayout(Layout::BothStretch(3, 1, 3, -1));
		sample->SetCaretBlinkingDelay(0.85f);
		sample->SetMultiLine(false);
		sample->layout->minSize = Vec2F(10, 10);

		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto focusLayer = sample->AddLayer("focus", mnew Sprite("ui/UI_Editbox_focus.png"), Layout::BothStretch(-9, -9, -9, -9));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		Animation focusAnim = Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f);
		*focusAnim.AddAnimationValue(&hoverLayer->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.05f);
		sample->AddState("focused", focusAnim)
			->offStateAnimationSpeed = 0.5f;

		Text* textDrawable = sample->GetTextDrawable();
		textDrawable->verAlign = VerAlign::Middle;
		textDrawable->horAlign = HorAlign::Left;
		textDrawable->SetFontAsset("stdFont.ttf");

		Sprite* caretDrawable = sample->GetCaretDrawable();
		*caretDrawable = Sprite();
		caretDrawable->size = Vec2F(1, textDrawable->GetFont()->GetHeightPx(textDrawable->GetHeight())*1.7f);
		caretDrawable->pivot = Vec2F(0, 0.26f);
		caretDrawable->color = Color4::Black();

		o2UI.AddWidgetStyle(sample, "singleline");
	}

	void BasicUIStyleBuilder::RebuildBacklessScrollarea()
	{
		UIScrollArea* sample = mnew UIScrollArea();
		sample->layout->minSize = Vec2F(10, 10);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "backless");
	}

	void BasicUIStyleBuilder::RebuildExpandButton()
	{
		UIButton* sample = mnew UIButton();
		sample->layout->minSize = Vec2F(5, 5);
		sample->name = "expandBtn";

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI_Right_icn.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto selectLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Right_icn_select.png"),
											Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_Right_icn_pressed.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		Animation expandedStateAnim(sample);
		*expandedStateAnim.AddAnimationValue(&regularLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim.AddAnimationValue(&selectLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim.AddAnimationValue(&pressedLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		sample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;

		o2UI.AddWidgetStyle(sample, "expand");
	}

	void BasicUIStyleBuilder::RebuildSpoiler()
	{
		UISpoiler* sample = mnew UISpoiler();
		sample->spacing = 5.0f;
		sample->borderLeft = 10;
		sample->expandHeight = false;
		sample->expandWidth = true;
		sample->fitByChildren = true;
		sample->baseCorner = BaseCorner::RightTop;

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "Button";
		captionText->horAlign = HorAlign::Left;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		captionText->wordWrap = true;
		sample->AddLayer("caption", captionText, Layout::HorStretch(VerAlign::Top, 10, 0, 20));

		auto expandBtn = o2UI.CreateWidget<UIButton>("expand");
		*expandBtn->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-7, 0));
		expandBtn->SetInternalParent(sample, false);

		sample->SetHeadHeight(20);

		o2UI.AddWidgetStyle(sample, "expand with caption");
	}

	void BasicUIStyleBuilder::RebuildBasicUIManager()
	{
		o2UI.ClearStyle();

		for (auto func : GetType().GetFunctions())
		{
			if (func->GetName() == "RebuildBasicUIManager")
				continue;

			func->Invoke<void>(this);
		}

		o2UI.SaveStyle("basic_ui_style.xml");
	}
}

DECLARE_CLASS(o2::BasicUIStyleBuilder);
