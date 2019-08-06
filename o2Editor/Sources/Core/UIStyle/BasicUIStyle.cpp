#include "stdafx.h"
#include "BasicUIStyle.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/ContextMenu.h"
#include "Scene/UI/Widgets/CustomDropDown.h"
#include "Scene/UI/Widgets/CustomList.h"
#include "Scene/UI/Widgets/DropDown.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/Widgets/HorizontalProgress.h"
#include "Scene/UI/Widgets/HorizontalScrollBar.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/List.h"
#include "Scene/UI/Widgets/LongList.h"
#include "Scene/UI/Widgets/MenuPanel.h"
#include "Scene/UI/Widgets/ScrollArea.h"
#include "Scene/UI/Widgets/Spoiler.h"
#include "Scene/UI/Widgets/Toggle.h"
#include "Scene/UI/Widgets/Tree.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/VerticalProgress.h"
#include "Scene/UI/Widgets/VerticalScrollBar.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"
#include "Scene/UI/Widgets/Window.h"

namespace o2
{
	void BasicUIStyleBuilder::RebuildButtonStyle()
	{
		Button* sample = mnew Button();
		sample->layout->minSize = Vec2F(20, 20);

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI4_button_regular.png"), 
											 Layout::BothStretch(-9, -9, -10, -10));

		auto hoverLayer   = sample->AddLayer("hover", mnew Sprite("ui/UI4_button_select.png"),
											 Layout::BothStretch(-9, -9, -10, -10));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI4_button_pressed.png"), 
											 Layout::BothStretch(-9, -9, -10, -10));

		auto focusLayer   = sample->AddLayer("focus", mnew Sprite("ui/UI4_button_focus.png"), 
											 Layout::BothStretch(-9, -9, -10, -10));

		Text* captionText        = mnew Text("stdFont.ttf");
		captionText->text        = "Button";
		captionText->horAlign    = HorAlign::Middle;
		captionText->verAlign    = VerAlign::Middle;
		captionText->dotsEngings = true;
		captionText->wordWrap    = true;
		captionText->color       = Color4(96, 125, 139);
		sample->AddLayer("caption", captionText);

		sample->AddState("hover", Animation::EaseInOut(sample, "layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, "layer/focus/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCloseButtonStyle()
	{
		Button* sample = mnew Button();
		sample->layout->minSize = Vec2F(5, 5);

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI4_Close_button_regular.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Close_button_select.png"),
										   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI4_Close_button_pressed.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));


		sample->AddState("hover", Animation::EaseInOut(sample, "layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "close");
	}

	void BasicUIStyleBuilder::RebuildArrowButtonStyle()
	{
		Button* sample = mnew Button();
		sample->layout->minSize = Vec2F(5, 5);

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI4_Options_button_regular.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Options_button_select.png"),
										   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI4_Options_button_pressed.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));


		sample->AddState("hover", Animation::EaseInOut(sample, "layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "arrow");
	}

	void BasicUIStyleBuilder::RebuildHorProgressBarStyle()
	{
		HorizontalProgress* sample = mnew HorizontalProgress();
		sample->layout->minSize = Vec2F(5, 5);
		auto backLayer = sample->AddLayer("back", nullptr);
		auto spriteBackLayer = backLayer->AddChildLayer("sprite", mnew Sprite("ui/UI4_Hor_scrollbar_bk.png"),
														Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															   Vec2F(-2, -4), Vec2F(2, 5)));

		backLayer->interactableLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -4), Vec2F(0, 4));

		auto barLayer = sample->AddLayer("bar", nullptr);
		auto barRegularSprite = barLayer->AddChildLayer("regular", mnew Sprite("ui/UI4_Hor_scrollhandle_regular.png"),
														Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															   Vec2F(-2, -4), Vec2F(2, 5)));

		auto barSelectSprite = barLayer->AddChildLayer("hover", mnew Sprite("ui/UI4_Hor_scrollhandle_select.png"),
													   Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															  Vec2F(-2, -4), Vec2F(2, 5)));

		auto barPressedSprite = barLayer->AddChildLayer("pressed", mnew Sprite("ui/UI4_Hor_scrollhandle_pressed.png"),
														Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															   Vec2F(-2, -4), Vec2F(2, 5)));

		sample->AddState("hover", Animation::EaseInOut(sample, "layer/bar/child/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/bar/child/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->SetOrientation(HorizontalProgress::Orientation::Right);

		o2UI.AddWidgetStyle(sample, "standard");
	}


	void BasicUIStyleBuilder::RebuildVerProgressBarStyle()
	{
		VerticalProgress* sample = mnew VerticalProgress();
		sample->layout->minSize = Vec2F(5, 5);
		auto backLayer = sample->AddLayer("back", nullptr);
		auto spriteBackLayer = backLayer->AddChildLayer("sprite", mnew Sprite("ui/UI4_Ver_scrollbar_bk.png"),
														Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															   Vec2F(-4, -2), Vec2F(5, 2)));
		backLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));

		auto barLayer = sample->AddLayer("bar", nullptr);
		auto barRegularSprite = barLayer->AddChildLayer("regular", mnew Sprite("ui/UI4_Ver_scrollhandle_regular.png"),
														Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															   Vec2F(-4, -2), Vec2F(5, 2)));

		auto barSelectSprite = barLayer->AddChildLayer("hover", mnew Sprite("ui/UI4_Ver_scrollhandle_select.png"),
													   Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															  Vec2F(-4, -2), Vec2F(5, 2)));

		auto barPressedSprite = barLayer->AddChildLayer("pressed", mnew Sprite("ui/UI4_Ver_scrollhandle_pressed.png"),
														Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															   Vec2F(-4, -2), Vec2F(5, 2)));


		sample->AddState("hover", Animation::EaseInOut(sample, "layer/bar/child/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/bar/child/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildHorScrollBarStyle()
	{
		HorizontalScrollBar* sample = mnew HorizontalScrollBar();
		sample->layout->minSize = Vec2F(5, 5);
		sample->SetScrollSense(0.25f);
		sample->SetMinimalScrollHandleSize(10);

		auto backLayer = sample->AddLayer("back", nullptr);
		backLayer->interactableLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -4), Vec2F(0, 4));
		auto spriteBackLayer = backLayer->AddChildLayer("sprite", mnew Sprite("ui/UI4_Hor_scrollbar_bk.png"),
														Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
															   Vec2F(-2, -4), Vec2F(2, 5)));

		auto handleLayer = sample->AddLayer("handle", nullptr);
		handleLayer->interactableLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -4), Vec2F(0, 4));
		auto barRegularSprite = handleLayer->AddChildLayer("regular", mnew Sprite("ui/UI4_Hor_scrollhandle_regular.png"),
														   Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
																  Vec2F(-2, -4), Vec2F(2, 5)));

		auto barSelectSprite = handleLayer->AddChildLayer("hover", mnew Sprite("ui/UI4_Hor_scrollhandle_select.png"),
														  Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
																 Vec2F(-2, -4), Vec2F(2, 5)));

		auto barPressedSprite = handleLayer->AddChildLayer("pressed", mnew Sprite("ui/UI4_Hor_scrollhandle_pressed.png"),
														   Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
																  Vec2F(-2, -4), Vec2F(2, 5)));
											
		sample->AddState("hover", Animation::EaseInOut(sample, "layer/handle/child/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/handle/child/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildVerScrollBarStyle()
	{
		VerticalScrollBar* sample = mnew VerticalScrollBar();
		sample->layout->minSize = Vec2F(5, 5);
		sample->SetScrollSense(0.25f);
		sample->SetMinimalScrollHandleSize(10);


		auto backLayer = sample->AddLayer("back", nullptr);
		backLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));
		auto spriteBackLayer = backLayer->AddChildLayer("sprite", mnew Sprite("ui/UI4_Ver_scrollbar_bk.png"),
														Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
															   Vec2F(-4, -2), Vec2F(5, 2)));

		auto handleLayer = sample->AddLayer("handle", nullptr);
		handleLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));
		auto barRegularSprite = handleLayer->AddChildLayer("regular", mnew Sprite("ui/UI4_Ver_scrollhandle_regular.png"),
														   Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
																  Vec2F(-4, -2), Vec2F(5, 2)));

		auto barSelectSprite = handleLayer->AddChildLayer("hover", mnew Sprite("ui/UI4_Ver_scrollhandle_select.png"),
														  Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
																 Vec2F(-4, -2), Vec2F(5, 2)));

		auto barPressedSprite = handleLayer->AddChildLayer("pressed", mnew Sprite("ui/UI4_Ver_scrollhandle_pressed.png"),
														   Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
																  Vec2F(-4, -2), Vec2F(5, 2)));


		sample->AddState("hover", Animation::EaseInOut(sample, "layer/handle/child/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/handle/child/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCheckboxStyle()
	{
		Toggle* sample = mnew Toggle();
		sample->layout->minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_Check_bk.png"),
										  Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

		auto hoverLayer = sample->AddLayer("backSelect", mnew Sprite("ui/UI4_Check_bk_select.png"),
											Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

		auto pressedLayer = sample->AddLayer("backPressed", mnew Sprite("ui/UI4_Check_bk_pressed.png"),
											 Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

		auto focusLayer = sample->AddLayer("backFocus", mnew Sprite("ui/UI4_Check_bk_focus.png"),
										   Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

		auto checkLayer = sample->AddLayer("check", mnew Sprite("ui/UI4_Ckeck.png"),
										   Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(1, -11), Vec2F(21, 10)));

		auto unknownLayer = sample->AddLayer("unknown", mnew Sprite("ui/UI4_Check_unknown.png"),
										   Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(1, -11), Vec2F(21, 10)));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "Checkbox";
		captionText->horAlign = HorAlign::Left;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		captionText->color = Color4(96, 125, 139);
		sample->AddLayer("caption", captionText, Layout(Vec2F(0, 0), Vec2F(1, 1), Vec2F(20, 0), Vec2F(0, 0)));

		sample->AddState("hover", Animation::EaseInOut(sample, "layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("value", Animation::EaseInOut(sample, "layer/check/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("unknown", Animation::EaseInOut(sample, "layer/unknown/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, "layer/focus/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCheckboxWithoutCaptionStyle()
	{
		Toggle* sample = mnew Toggle();
		sample->layout->minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_Check_bk.png"),
										  Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		auto hoverLayer = sample->AddLayer("backSelect", mnew Sprite("ui/UI4_Check_bk_select.png"),
											Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		auto pressedLayer = sample->AddLayer("backPressed", mnew Sprite("ui/UI4_Check_bk_pressed.png"),
											 Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		auto focusLayer = sample->AddLayer("backFocus", mnew Sprite("ui/UI4_Check_bk_focus.png"),
										   Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		auto checkLayer = sample->AddLayer("check", mnew Sprite("ui/UI4_Ckeck.png"),
										   Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		auto unknownLayer = sample->AddLayer("unknown", mnew Sprite("ui/UI4_Check_unknown.png"),
											 Layout::Based(BaseCorner::Right, Vec2F(20, 20)));

		sample->AddState("hover", Animation::EaseInOut(sample, "layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("value", Animation::EaseInOut(sample, "layer/check/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("unknown", Animation::EaseInOut(sample, "layer/unknown/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, "layer/focus/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "without caption");
	}

	void BasicUIStyleBuilder::RebuildScrollAreaStraightBarsStyle()
	{
		ScrollArea* sample = mnew ScrollArea();
		sample->layout->minSize = Vec2F(30, 30);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(false);
		sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));

		HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		Animation enableHorScrollAnim;
		enableHorScrollAnim.SetTarget(sample);
		*enableHorScrollAnim.AddAnimationValue<float>("mVerScrollBar/layout/offsetBottom") =
			AnimatedValue<float>::EaseInOut(5, 15, 0.2f);

		*enableHorScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMin") =
			AnimatedValue<Vec2F>::EaseInOut(Vec2F(5, 5), Vec2F(5, 15), 0.2f);

		auto enableHorScrollState = sample->AddState("enableHorBar", enableHorScrollAnim);

		Animation enableVerScrollAnim;
		enableVerScrollAnim.SetTarget(sample);
		*enableVerScrollAnim.AddAnimationValue<float>("mHorScrollBar/layout/offsetRight") =
			AnimatedValue<float>::EaseInOut(-5, -15, 0.2f);

		*enableVerScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMax") =
			AnimatedValue<Vec2F>::EaseInOut(Vec2F(-5, -5), Vec2F(-15, -5), 0.2f);

		auto enableVerScrollState = sample->AddState("enableVerBar", enableVerScrollAnim);

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "straight bars");
	}

	void BasicUIStyleBuilder::RebuildScrollAreaStyle()
	{
		ScrollArea* sample = mnew ScrollArea();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);
		sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));

		HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildLabelStyle()
	{
		Label* sample = mnew Label();
		sample->layout->minSize = Vec2F(20, 20);
		sample->horOverflow = Label::HorOverflow::Dots;
		sample->verOverflow = Label::VerOverflow::None;

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "Checkbox";
		captionText->horAlign = HorAlign::Middle;
		captionText->verAlign = VerAlign::Middle;
		captionText->color = Color4(96, 125, 139);
		sample->AddLayer("text", captionText);

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildEditBoxStyle()
	{
		EditBox* sample = mnew EditBox();
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(false);
		sample->SetCaretBlinkingDelay(1.15f);
		sample->SetSelectionColor(Color4(0, 156, 141, 120));
		sample->layout->minSize = Vec2F(30, 40);

		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI4_Editbox_focus.png"), Layout::BothStretch(-9, -9, -9, -9));

		HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		Animation enableHorScrollAnim;
		enableHorScrollAnim.SetTarget(sample);
		*enableHorScrollAnim.AddAnimationValue<float>("mVerScrollBar/layout/offsetBottom") =
			AnimatedValue<float>::EaseInOut(5, 15, 0.2f);

		*enableHorScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMin") =
			AnimatedValue<Vec2F>::EaseInOut(Vec2F(5, 5), Vec2F(5, 15), 0.2f);

		auto enableHorScrollState = sample->AddState("enableHorBar", enableHorScrollAnim);

		Animation enableVerScrollAnim;
		enableVerScrollAnim.SetTarget(sample);
		*enableVerScrollAnim.AddAnimationValue<float>("mHorScrollBar/layout/offsetRIght") =
			AnimatedValue<float>::EaseInOut(-5, -15, 0.2f);

		*enableVerScrollAnim.AddAnimationValue<Vec2F>("mViewAreaLayout/offsetMax") =
			AnimatedValue<Vec2F>::EaseInOut(Vec2F(-5, -5), Vec2F(-15, -5), 0.2f);

		auto enableVerScrollState = sample->AddState("enableVerBar", enableVerScrollAnim);

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, "layer/focus/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		Text* textDrawable = sample->GetTextDrawable();
		textDrawable->verAlign = VerAlign::Top;
		textDrawable->horAlign = HorAlign::Left;
		textDrawable->color = Color4(96, 125, 139);
		textDrawable->SetFontAsset(FontAssetRef("stdFont.ttf"));

		Sprite* caretDrawable = sample->GetCaretDrawable();
		*caretDrawable = Sprite();
		caretDrawable->size = Vec2F(1, textDrawable->GetFont()->GetHeightPx(textDrawable->GetHeight())*1.7f);
		caretDrawable->pivot = Vec2F(0, 0.16f);
		caretDrawable->color = Color4::Black();

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCustomListStyle()
	{
		CustomList* sample = mnew CustomList();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);
		sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));

		Sprite* selection = mnew Sprite("ui/UI4_Context_menu_select.png");
		*sample->GetSelectionDrawable() = *selection;
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-12, -16, -10, -16));

		Sprite* hover = mnew Sprite("ui/UI4_Context_menu_white.png");
		*sample->GetHoverDrawable() = *hover;
		sample->SetHoverDrawableLayout(Layout::BothStretch(-12, -16, -10, -16));

		HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, "mVerScrollBar/layout/offsetBottom",
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, "mHorScrollBar/layout/offsetRight",
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, "mHoverDrawable/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildLongListStyle()
	{
		LongList* sample = mnew LongList();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);

		Sprite* selection = mnew Sprite("ui/UI4_Context_menu_select.png");
		*sample->GetSelectionDrawable() = *selection;
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-12, -16, -10, -16));

		Sprite* hover = mnew Sprite("ui/UI4_Context_menu_white.png");
		*sample->GetHoverDrawable() = *hover;
		sample->SetHoverDrawableLayout(Layout::BothStretch(-12, -16, -10, -16));

		HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, "mVerScrollBar/layout/offsetBottom",
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, "mHorScrollBar/layout/offsetRight",
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, "mHoverDrawable/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, "mSelectionDrawable/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildListStyle()
	{
		List* sample = mnew List();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);
		sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));

		Sprite* selection = mnew Sprite("ui/UI4_Context_menu_select.png");
		*sample->GetSelectionDrawable() = *selection;
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

		Sprite* hover = mnew Sprite("ui/UI4_Context_menu_white.png");
		*sample->GetHoverDrawable() = *hover;
		sample->SetHoverDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

		Label* itemSample = o2UI.CreateLabel("empty");
		itemSample->layout->size = Vec2F(20, 0);
		itemSample->horAlign = HorAlign::Left;
		itemSample->verAlign = VerAlign::Middle;
		sample->SetItemSample(itemSample);

		HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, "mVerScrollBar/layout/offsetBottom",
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, "mHorScrollBar/layout/offsetRight",
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, "mHoverDrawable/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, "mSelectionDrawable/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCustomDropDownStyle()
	{
		CustomDropDown* sample = mnew CustomDropDown();
		sample->layout->minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI4_Editbox_pressed.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI4_Down_icn.png"),
										   Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-20, -10), Vec2F(0, 10)));

		sample->SetClippingLayout(Layout::BothStretch(4, 2, 20, 2));

		Widget* itemSample = mnew Widget();
		itemSample->layout->size = Vec2F(20, 20);
		sample->SetItemSample(itemSample);

		auto list = sample->GetListView();
		*list = *o2UI.GetWidgetStyle<CustomList>("standard");
		list->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		delete list->layer["back"]->GetDrawable();
		list->layer["back"]->SetDrawable(mnew Sprite("ui/UI4_Box_regular.png"));
		list->layout->pivot = Vec2F(0.5f, 1.0f);
		list->layout->anchorMin = Vec2F(0, 0);
		list->layout->anchorMax = Vec2F(1, 0);
		list->layout->offsetMin = Vec2F(-1, -60);
		list->layout->offsetMax = Vec2F(0, 3);

		sample->AddState("hover", Animation::EaseInOut(sample, "layer/hover/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("opened", Animation::EaseInOut(sample, "layer/arrow/mDrawable/scale", Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildDropDownStyle()
	{
		DropDown* sample = mnew DropDown();
		sample->layout->minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), 
										  Layout::BothStretch(-9, -9, -9, -9));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Editbox_select.png"), 
										   Layout::BothStretch(-9, -9, -9, -9));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI4_Editbox_pressed.png"), 
											 Layout::BothStretch(-9, -9, -9, -9));

		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI4_Down_icn.png"),
										   Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-20, -10), Vec2F(0, 10)));

		sample->SetClippingLayout(Layout::BothStretch(4, 2, 20, 2));

		auto list = sample->GetListView();
		*list = *o2UI.GetWidgetStyle<CustomList>("standard");
		list->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		delete list->layer["back"]->GetDrawable();
		list->layer["back"]->SetDrawable(mnew Sprite("ui/UI4_Box_regular.png"));
		list->layout->pivot = Vec2F(0.5f, 1.0f);
		list->layout->anchorMin = Vec2F(0, 0);
		list->layout->anchorMax = Vec2F(1, 0);
		list->layout->offsetMin = Vec2F(2, -60);
		list->layout->offsetMax = Vec2F(0, 3);

		Label* itemSample = o2UI.CreateLabel("empty");
		itemSample->horAlign = HorAlign::Left;
		sample->SetItemSample(itemSample);

		sample->AddState("hover", Animation::EaseInOut(sample, "layer/hover/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("opened", Animation::EaseInOut(sample, "layer/arrow/mDrawable/scale", Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");

		
	}

	void BasicUIStyleBuilder::RebuildWindowStyle()
	{
		Window* sample = mnew Window();
		sample->layout->minSize = Vec2F(100, 50);
		sample->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 17));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 20));
		sample->SetEnableScrollsHiding(true);

		auto regularBackLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI4_window_frame_regular.png"),
												 Layout::BothStretch(-13, -13, -13, -11));

		auto iconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI4_o2_sign.png"),
										  Layout::Based(BaseCorner::LeftTop, Vec2F(18, 18), Vec2F(-1, 1)));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "Window";
		captionText->horAlign = HorAlign::Left;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		auto textLayer = sample->AddLayer("caption", captionText,
										  Layout::HorStretch(VerAlign::Top, 20, 35, 20, -2));

		HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		*horScrollBar->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 5, 15, 15, -5);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		*verScrollBar->layout = WidgetLayout::VerStretch(HorAlign::Right, 0, 15, 15, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 2.0f;

		Button* closeBtn = o2UI.CreateWidget<Button>("close");
		closeBtn->name = "closeButton";
		*closeBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(4, 22));
		sample->AddInternalWidget(closeBtn);

		Button* optionsBtn = o2UI.CreateWidget<Button>("arrow");
		optionsBtn->name = "optionsButton";
		*optionsBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(-12, 22));
		sample->AddInternalWidget(optionsBtn);

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
		ContextMenu* sample = mnew ContextMenu();

		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		sample->SetEnableScrollsHiding(true);
		sample->SetMinFitSize(50);

		sample->AddLayer("back", mnew Sprite("ui/UI4_Context_menu.png"), Layout::BothStretch(-20, -19, -20, -19));

		Sprite* selection = mnew Sprite("ui/UI4_Context_menu_select.png");
		*sample->GetSelectionDrawable() = *selection;
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

		Widget* separatorSample = sample->GetSeparatorSample();
		separatorSample->AddLayer("line", mnew Sprite("ui/UI4_Separator.png"),
								  Layout::HorStretch(VerAlign::Middle, 0, 0, 5, 0));

		ContextMenuItem* itemSample = sample->GetItemSample();

		WidgetLayer* captionLayer = itemSample->FindLayer("caption");
		Text* captionLayerText = mnew Text("stdFont.ttf");
		captionLayer->SetDrawable(captionLayerText);
		captionLayer->layout = Layout::BothStretch(20, 1, 0, -1);
		captionLayerText->horAlign = HorAlign::Left;
		captionLayerText->verAlign = VerAlign::Middle;
		captionLayerText->color = Color4(96, 125, 139);

		WidgetLayer* shortcutLayer = itemSample->FindLayer("shortcut");
		Text* shortcutLayerText = mnew Text("stdFont.ttf");
		shortcutLayer->layout = Layout::BothStretch(20, 1, 10, -1);
		shortcutLayer->SetDrawable(shortcutLayerText);
		shortcutLayerText->horAlign = HorAlign::Right;
		shortcutLayerText->verAlign = VerAlign::Middle;
		shortcutLayerText->color = Color4(96, 125, 139);
		shortcutLayer->transparency = 0.7f;

		WidgetLayer* subIconLayer = itemSample->FindLayer("subIcon");
		subIconLayer->SetDrawable(mnew Sprite("ui/UI4_Right_icn.png"));
		subIconLayer->layout = Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-15, 10), Vec2F(5, -10));

		itemSample->AddLayer("check", mnew Sprite("ui/UI4_Ckeck.png"), 
							 Layout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(0, 0)));

		WidgetLayer* basicLayer = itemSample->FindLayer("basic");

		itemSample->AddState("enabled", Animation::EaseInOut(itemSample, "layer/basic/transparency", 0.5f, 1.0f, 0.2f));
		itemSample->SetStateForcible("enabled", true);

		HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, "mVerScrollBar/layout/offsetBottom",
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, "mHorScrollBar/layout/offsetRight",
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, "mSelectionDrawable/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f));

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildMenuPanelStyle()
	{
		MenuPanel* sample = mnew MenuPanel();

		sample->AddLayer("back", mnew Sprite("ui/UI4_Up_Menu.png"), Layout::BothStretch(-15, -15, -15, -17));

		Sprite* selection = mnew Sprite("ui/UI_ListBox_selection_hover.png");
		*sample->GetSelectionDrawable() = *selection;
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

		auto itemSample = o2UI.CreateLabel("unknown");
		itemSample->horOverflow = Label::HorOverflow::Expand;
		itemSample->expandBorder = Vec2F(10, 0);
		itemSample->color = Color4(235, 255, 253);
		sample->SetItemSample(itemSample);

		sample->AddState("hover", Animation::EaseInOut(sample, "mSelectionDrawable/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f));

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildTreeStyle()
	{
		Tree* sample = mnew Tree();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);
		sample->SetChildsNodesOffset(10.0f);

		*sample->GetHoverDrawable() = Sprite("ui/UI4_Context_menu_white.png");

		TreeNode* itemSample = sample->GetNodeSample();

		auto itemSelectionLayer = itemSample->AddLayer("select", nullptr);

		auto itemFocusedLayer = itemSelectionLayer->AddChildLayer("focused", mnew Sprite("ui/UI4_Context_menu_select.png"),
																  Layout::BothStretch(-10, -16, -10, -16));

		auto itemUnfocusedLayer = itemSelectionLayer->AddChildLayer("unfocused", mnew Sprite("ui/UI4_Context_menu_white.png"),
																	Layout::BothStretch(-10, -16, -10, -16));

		Text* captionLayerText = mnew Text("stdFont.ttf");
		captionLayerText->horAlign = HorAlign::Left;
		captionLayerText->verAlign = VerAlign::Middle;
		captionLayerText->color = Color4(96, 125, 139);
		itemSample->AddLayer("name", captionLayerText, Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(12, -20), Vec2F(0, 0)));

		Button* itemSampleExpandBtn = mnew Button();
		itemSampleExpandBtn->layout->minSize = Vec2F(5, 5);
		itemSampleExpandBtn->name = "expandBtn";

		auto regularLayer = itemSampleExpandBtn->AddLayer("regular", mnew Sprite("ui/UI4_Right_icn.png"),
														  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto hoverLayer = itemSampleExpandBtn->AddLayer("hover", mnew Sprite("ui/UI4_Right_icn_select.png"),
														 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = itemSampleExpandBtn->AddLayer("pressed", mnew Sprite("ui/UI4_Right_icn_pressed.png"),
														  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


		itemSampleExpandBtn->AddState("hover", Animation::EaseInOut(itemSampleExpandBtn, "layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		itemSampleExpandBtn->AddState("pressed", Animation::EaseInOut(itemSampleExpandBtn, "layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		itemSampleExpandBtn->AddState("visible", Animation::EaseInOut(itemSampleExpandBtn, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		itemSampleExpandBtn->layout->anchorMin = Vec2F(0, 1);
		itemSampleExpandBtn->layout->anchorMax = Vec2F(0, 1);
		itemSampleExpandBtn->layout->offsetMin = Vec2F(0, -20);
		itemSampleExpandBtn->layout->offsetMax = Vec2F(10, 0);

		itemSample->AddChild(itemSampleExpandBtn);

		Animation expandedStateAnim(itemSample);
		*expandedStateAnim.AddAnimationValue<float>("child/expandBtn/layer/regular/mDrawable/angle") =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);
		*expandedStateAnim.AddAnimationValue<float>("child/expandBtn/layer/hover/mDrawable/angle") =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);
		*expandedStateAnim.AddAnimationValue<float>("child/expandBtn/layer/pressed/mDrawable/angle") = 
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		itemSample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;

		itemSample->AddState("selected", Animation::EaseInOut(itemSample, "layer/select/transparency", 0.0f, 1.0f, 0.2f));

		Animation focusedItemAnim = Animation::EaseInOut(itemSample, "layer/select/child/focused/transparency", 0.0f, 1.0f, 0.2f);
		*focusedItemAnim.AddAnimationValue<float>("layer/select/child/unfocused/transparency") = AnimatedValue<float>::EaseInOut(1.0f, 0.0f, 0.2f);
		itemSample->AddState("focused", focusedItemAnim);

		HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, "mVerScrollBar/layout/offsetBottom",
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, "mHorScrollBar/layout/offsetRight",
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, "mHoverDrawable/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildBacklessDropdown()
	{
		DropDown* sample = mnew DropDown();
		sample->layout->minSize = Vec2F(20, 20);
		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI4_Down_icn_white.png"),
										   Layout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, -1)));

		sample->SetClippingLayout(Layout::BothStretch(4, 2, 20, 2));

		auto list = sample->GetListView();
		*list = *o2UI.GetWidgetStyle<CustomList>("standard");

		list->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		list->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));

		list->RemoveLayer("back");
		list->AddLayer("back", mnew Sprite("ui/UI4_Context_menu.png"), Layout::BothStretch(-22, -19, -19, -19));

		list->layout->pivot = Vec2F(0.5f, 1.0f);
		list->layout->anchorMin = Vec2F(0, 0);
		list->layout->anchorMax = Vec2F(1, 0);
		list->layout->offsetMin = Vec2F(0, -60);
		list->layout->offsetMax = Vec2F(0, 0);

		Label* itemSample = o2UI.CreateLabel("empty");
		itemSample->horAlign = HorAlign::Left;
		sample->SetItemSample(itemSample);

		// Selected text
		Text* captionText = mnew Text("stdFont.ttf");
		captionText->horAlign = HorAlign::Left;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		captionText->wordWrap = false;
		captionText->color = Color4(235, 255, 253);
		sample->AddLayer("selectedText", captionText, Layout::BothStretch(4, 0, 0, 0));

		// States
		sample->AddState("opened", Animation::EaseInOut(sample, "layer/arrow/mDrawable/scale", Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "backless");
	}

	void BasicUIStyleBuilder::RebuildBacklessEditbox()
	{
		EditBox* sample = mnew EditBox();
		sample->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));
		sample->SetViewLayout(Layout::BothStretch(0, 0, 2, 0));
		sample->SetCaretBlinkingDelay(1.15f);
		sample->SetSelectionColor(Color4(0, 156, 141, 120));
		sample->SetMultiLine(false);
		sample->layout->minSize = Vec2F(30, 40);

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		Text* textDrawable = sample->GetTextDrawable();
		textDrawable->verAlign = VerAlign::Middle;
		textDrawable->horAlign = HorAlign::Left;
		textDrawable->color = Color4(96, 125, 139);
		textDrawable->SetFontAsset(FontAssetRef("stdFont.ttf"));

		Sprite* caretDrawable = sample->GetCaretDrawable();
		*caretDrawable = Sprite();
		caretDrawable->size = Vec2F(1, textDrawable->GetFont()->GetHeightPx(textDrawable->GetHeight())*1.7f);
		caretDrawable->pivot = Vec2F(0, 0.26f);
		caretDrawable->color = Color4::Black();

		o2UI.AddWidgetStyle(sample, "backless");
	}

	void BasicUIStyleBuilder::RebuildSinglelineEditbox()
	{
		EditBox* sample = mnew EditBox();
		sample->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));
		sample->SetViewLayout(Layout::BothStretch(3, 1, 3, -1));
		sample->SetCaretBlinkingDelay(1.15f);
		sample->SetMultiLine(false);
		sample->SetSelectionColor(Color4(0, 156, 141, 120));
		sample->layout->minSize = Vec2F(10, 10);

		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto focusLayer = sample->AddLayer("focus", mnew Sprite("ui/UI4_Editbox_focus.png"), Layout::BothStretch(-9, -9, -9, -9));

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		Animation focusAnim = Animation::EaseInOut(sample, "layer/focus/transparency", 0.0f, 1.0f, 0.05f);
		*focusAnim.AddAnimationValue<float>("layer/hover/transparency") = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.05f);
		sample->AddState("focused", focusAnim)
			->offStateAnimationSpeed = 0.5f;

		Text* textDrawable = sample->GetTextDrawable();
		textDrawable->verAlign = VerAlign::Middle;
		textDrawable->horAlign = HorAlign::Left;
		textDrawable->color = Color4(96, 125, 139);
		textDrawable->SetFontAsset(FontAssetRef("stdFont.ttf"));

		Sprite* caretDrawable = sample->GetCaretDrawable();
		*caretDrawable = Sprite();
		caretDrawable->size = Vec2F(1, textDrawable->GetFont()->GetHeightPx(textDrawable->GetHeight())*1.7f);
		caretDrawable->pivot = Vec2F(0, 0.26f);
		caretDrawable->color = Color4::Black();

		o2UI.AddWidgetStyle(sample, "singleline");
	}

	void BasicUIStyleBuilder::RebuildBacklessScrollarea()
	{
		ScrollArea* sample = mnew ScrollArea();
		sample->layout->minSize = Vec2F(10, 10);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);

		HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout->anchorMin = Vec2F(0, 0);
		horScrollBar->layout->anchorMax = Vec2F(1, 0);
		horScrollBar->layout->offsetMin = Vec2F(5, 0);
		horScrollBar->layout->offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 15);
		verScrollBar->layout->offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "backless");
	}

	void BasicUIStyleBuilder::RebuildExpandButton()
	{
		Button* sample = mnew Button();
		sample->layout->minSize = Vec2F(5, 5);
		sample->name = "expandBtn";

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI4_Right_icn.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto selectLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Right_icn_select.png"),
											Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI4_Right_icn_pressed.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


		sample->AddState("hover", Animation::EaseInOut(sample, "layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		Animation expandedStateAnim(sample);
		*expandedStateAnim.AddAnimationValue<float>("layer/regular/mDrawable/angle") =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim.AddAnimationValue<float>("layer/hover/mDrawable/angle") =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim.AddAnimationValue<float>("layer/pressed/mDrawable/angle") =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		sample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;

		o2UI.AddWidgetStyle(sample, "expand");
	}

	void BasicUIStyleBuilder::RebuildExpandWhiteButton()
	{
		Button* sample = mnew Button();
		sample->layout->minSize = Vec2F(5, 5);
		sample->name = "expandBtn";

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI4_Right_icn_white.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto selectLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Right_icn_white_select.png"),
											Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI4_Right_icn_white_pressed.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


		sample->AddState("hover", Animation::EaseInOut(sample, "layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, "layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, "transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		Animation expandedStateAnim(sample);
		*expandedStateAnim.AddAnimationValue<float>("layer/regular/mDrawable/angle") =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim.AddAnimationValue<float>("layer/hover/mDrawable/angle") =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim.AddAnimationValue<float>("layer/pressed/mDrawable/angle") =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		sample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;

		o2UI.AddWidgetStyle(sample, "expand white");
	}

	void BasicUIStyleBuilder::RebuildSpoiler()
	{
		Spoiler* sample = mnew Spoiler();
		sample->spacing = 5.0f;
		sample->borderLeft = 10;
		sample->expandHeight = false;
		sample->expandWidth = true;
		sample->fitByChildren = true;
		sample->baseCorner = BaseCorner::RightTop;

		Text* captionText        = mnew Text("stdFont.ttf");
		captionText->text        = "Button";
		captionText->horAlign    = HorAlign::Left;
		captionText->verAlign    = VerAlign::Middle;
		captionText->dotsEngings = true;
		captionText->wordWrap = true;
		captionText->color = Color4(96, 125, 139);
		sample->AddLayer("caption", captionText, Layout::HorStretch(VerAlign::Top, 11, 0, 20));

		auto expandBtn = o2UI.CreateWidget<Button>("expand");
		*expandBtn->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-7, 0));
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
