#include "EditorUIStyle.h"

#include "Animation/Animate.h"
#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "AssetsWindow/AssetsIconsScroll.h"
#include "AssetsWindow/UIAssetIcon.h"
#include "Core/UIStyle/BasicUIStyle.h"
#include "Core/WindowsSystem/UIDockableWindow.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "TreeWindow/ActorsTree.h"
#include "UI/Button.h"
#include "UI/ContextMenu.h"
#include "UI/CustomDropDown.h"
#include "UI/CustomList.h"
#include "UI/DropDown.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/HorizontalProgress.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/Label.h"
#include "UI/List.h"
#include "UI/MenuPanel.h"
#include "UI/ScrollArea.h"
#include "UI/Toggle.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "UI/VerticalProgress.h"
#include "UI/VerticalScrollBar.h"
#include "UI/Window.h"

using namespace o2;

namespace Editor
{
	void EditorUIStyleBuilder::EditorUIStyleBuilder::RebuildDockableWndStyle()
	{
		UIDockableWindow* sample = mnew UIDockableWindow();

		sample->layout.minSize = Vec2F(100, 50);

		*sample->GetDockingFrameSample() = Sprite("ui/UI_Window_place.png");

		auto regularBackLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_window_frame_regular.png"),
												 Layout::BothStretch(-13, -13, -13, -11));

		auto dockedBackLayer = sample->AddLayer("dockedBack", mnew Sprite("ui/UI_window_frame_docked.png"),
												Layout::BothStretch(-13, -13, -13, -11));

		auto iconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI_o2_sign.png"),
										  Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, -1)));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "Window";
		captionText->horAlign = HorAlign::Left;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		auto textLayer = sample->AddLayer("caption", captionText,
										  Layout::HorStretch(VerAlign::Top, 20, 35, 20, -2));

		sample->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 17));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 20));
		sample->SetEnableScrollsHiding(true);

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout.anchorMin = Vec2F(0, 0);
		horScrollBar->layout.anchorMax = Vec2F(1, 0);
		horScrollBar->layout.offsetMin = Vec2F(5, 0);
		horScrollBar->layout.offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout.anchorMin = Vec2F(1, 0);
		verScrollBar->layout.anchorMax = Vec2F(1, 1);
		verScrollBar->layout.offsetMin = Vec2F(-15, 15);
		verScrollBar->layout.offsetMax = Vec2F(0, -20);
		sample->SetVerticalScrollBar(verScrollBar);

		Animation dockedStateAnim;
		dockedStateAnim.SetTarget(sample);
		*dockedStateAnim.AddAnimationValue<float>(&dockedBackLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);
		*dockedStateAnim.AddAnimationValue<float>(&regularBackLayer->transparency) = AnimatedValue<float>::EaseInOut(1, 0, 0.2f);

		auto dockedState = sample->AddState("docked", dockedStateAnim);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 2.0f;

		UIButton* closeBtn = o2UI.CreateWidget<UIButton>("close");
		closeBtn->name = "closeButton";
		closeBtn->layout = UIWidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(0, 2));
		sample->AddWindowElement(closeBtn);

		UIButton* optionsBtn = o2UI.CreateWidget<UIButton>("arrow");
		optionsBtn->name = "optionsButton";
		optionsBtn->layout = UIWidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(-16, 2));
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

	void EditorUIStyleBuilder::RebuildExpandButton()
	{
		UIButton* sample = mnew UIButton();
		sample->layout.minSize = Vec2F(5, 5);
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

	void EditorUIStyleBuilder::RebuildPlayStopButtonStyle()
	{
		UIToggle* sample = mnew UIToggle();
		auto playRootIconLayer = sample->AddLayer("playRootIcon", nullptr);
		auto stopRootIconLayer = sample->AddLayer("stopRootIcon", nullptr);

		auto playIconLayer = playRootIconLayer->AddChildLayer("regular", mnew Sprite("ui/UI_play_btn_regular.png"),
															  Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto playSelectIconLayer = playRootIconLayer->AddChildLayer("hover", mnew Sprite("ui/UI_play_btn_select.png"),
																	Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto playPressedIconLayer = playRootIconLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_play_btn_select copy.png"),
																	 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto stopIconLayer = stopRootIconLayer->AddChildLayer("regular", mnew Sprite("ui/UI_stop_btn_regular.png"),
															  Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto stopSelectIconLayer = stopRootIconLayer->AddChildLayer("hover", mnew Sprite("ui/UI_stop_btn_select.png"),
																	Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto stopPressedIconLayer = stopRootIconLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_stop_btn_pressed.png"),
																	 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		Animation playBtnSelectAnim = Animation::EaseInOut(sample, &playSelectIconLayer->transparency, 0.0f, 1.0f, 0.1f);
		*playBtnSelectAnim.AddAnimationValue(&stopSelectIconLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("hover", playBtnSelectAnim)->offStateAnimationSpeed = 0.25f;

		Animation playBtnPressAnim = Animation::EaseInOut(sample, &playPressedIconLayer->transparency, 0.0f, 1.0f, 0.1f);
		*playBtnPressAnim.AddAnimationValue(&stopPressedIconLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.05f);

		sample->AddState("pressed", playBtnPressAnim)->offStateAnimationSpeed = 0.5f;

		Animation valueBtnPressAnim = Animation::EaseInOut(sample, &playRootIconLayer->transparency, 1.0f, 0.0f, 0.1f);
		*valueBtnPressAnim.AddAnimationValue(&stopRootIconLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("value", valueBtnPressAnim);

		o2UI.AddWidgetStyle(sample, "play-stop");
	}

	void EditorUIStyleBuilder::RebuildPauseButtonStyle()
	{
		UIToggle* sample = mnew UIToggle();
		auto pauseIconRootLayer = sample->AddLayer("regular", nullptr);
		auto pauseIconLayer = pauseIconRootLayer->AddChildLayer("icon", mnew Sprite("ui/UI_pause_btn_regular.png"),
																Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
																Vec2F(-10, 10), Vec2F(10, -10)));

		auto pauseSelectIconLayer = pauseIconRootLayer->AddChildLayer("hover", mnew Sprite("ui/UI_pause_btn_select.png"),
																	  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
																	  Vec2F(-10, 10), Vec2F(10, -10)));

		auto pausePressedIconLayer = pauseIconRootLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_pause_btn_pressed.png"),
																	   Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
																	   Vec2F(-10, 10), Vec2F(10, -10)));

		sample->AddState("hover", Animation::EaseInOut(sample, &pauseSelectIconLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pausePressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &pauseIconRootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "pause");
	}

	void EditorUIStyleBuilder::RebuildStepButtonStyle()
	{
		UIButton* sample = mnew UIButton();
		sample->name = "step button";
		auto stepIconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI_step_btn_regular.png"),
											  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
											  Vec2F(-10, 10), Vec2F(10, -10)));

		auto stepSelectIconLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_step_btn_select.png"),
													Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
													Vec2F(-10, 10), Vec2F(10, -10)));

		auto stepPressedIconLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_step_btn_pressed.png"),
													 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
													 Vec2F(-10, 10), Vec2F(10, -10)));

		sample->AddState("hover", Animation::EaseInOut(sample, &stepSelectIconLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &stepPressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "step");
	}

	void EditorUIStyleBuilder::RebuildRoundDropDown()
	{
		UIDropDown* sample = mnew UIDropDown();
		sample->layout.minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_panel_subpanel_bk.png"),
										  Layout::BothStretch(-7, -5, -5, -5));

		auto selectLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_panel_subpanel_select.png"),
											Layout::BothStretch(-7, -5, -5, -5));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_panel_subpanel_pressed.png"),
											 Layout::BothStretch(-7, -5, -5, -5));

		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI_Down_icn.png"),
										   Layout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(-5, 0)));

		sample->SetClippingLayout(Layout::BothStretch(10, 3, 20, 1));

		auto list = sample->GetListView();
		*list = *o2UI.GetWidgetStyle<UICustomList>("standard");

		list->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		list->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));

		list->RemoveLayer("back");
		list->AddLayer("back", mnew Sprite("ui/UI_Context_menu.png"), Layout::BothStretch(-21, -19, -19, -19));

		list->layout.pivot = Vec2F(0.5f, 1.0f);
		list->layout.anchorMin = Vec2F(0, 0);
		list->layout.anchorMax = Vec2F(1, 0);
		list->layout.offsetMin = Vec2F(0, -60);
		list->layout.offsetMax = Vec2F(0, 0);

		UILabel* itemSample = o2UI.CreateLabel("empty");
		itemSample->horAlign = HorAlign::Left;
		sample->SetItemSample(itemSample);

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("opened", Animation::EaseInOut(sample, &arrowLayer->drawable->scale, Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "round");
	}

	void EditorUIStyleBuilder::RebuildArrowToggle()
	{
		UIToggle* sample = mnew UIToggle();
		sample->layout.minSize = Vec2F(20, 20);
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("hover", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_select_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_select_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "arrow");
	}

	void EditorUIStyleBuilder::RebuildBrushToggle()
	{
		UIToggle* sample = mnew UIToggle();
		sample->layout.minSize = Vec2F(20, 20);
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("hover", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_brush_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_brush_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "brush");
	}

	void EditorUIStyleBuilder::RebuildMoveToggle()
	{
		UIToggle* sample = mnew UIToggle();
		sample->layout.minSize = Vec2F(20, 20);
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("hover", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_move_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_move_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "move");
	}

	void EditorUIStyleBuilder::RebuildRotateToggle()
	{
		UIToggle* sample = mnew UIToggle();
		sample->layout.minSize = Vec2F(20, 20);
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("hover", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_rotate_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_rotate_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "rotate");
	}

	void EditorUIStyleBuilder::RebuildScaleToggle()
	{
		UIToggle* sample = mnew UIToggle();
		sample->layout.minSize = Vec2F(20, 20);
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("hover", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_scale_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_scale_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "scale");
	}

	void EditorUIStyleBuilder::RebuildFrameToggle()
	{
		UIToggle* sample = mnew UIToggle();
		sample->layout.minSize = Vec2F(20, 20);
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("hover", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_frame_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_frame_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "frame");
	}

	void EditorUIStyleBuilder::RebuildSearchButton()
	{
		UIButton* sample = mnew UIButton();
		sample->layout.minSize = Vec2F(5, 5);

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI_search_regular.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto selectLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_search_select.png"),
											Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_search_pressed.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "search");
	}

	void EditorUIStyleBuilder::RebuildListTreeToggle()
	{
		UIToggle* sample = mnew UIToggle();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_panel_button.png"),
										  Layout::BothStretch(-4, -4, -5, -5));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI_panel_button_select.png"),
											Layout::BothStretch(-4, -4, -5, -5));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI_panel_button_pressed.png"),
											 Layout::BothStretch(-4, -4, -5, -5));

		auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI_panel_button_focus.png"),
										   Layout::BothStretch(-4, -4, -5, -5));

		auto listLayer = sample->AddLayer("listIcon", mnew Sprite("ui/UI_list_icon.png"),
										  Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 0)));

		auto treeLayer = sample->AddLayer("treeIcon", mnew Sprite("ui/UI_tree_icon.png"),
										  Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 0)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		Animation valueBtnAnim = Animation::EaseInOut(sample, &treeLayer->transparency, 1.0f, 0.0f, 0.1f);
		*valueBtnAnim.AddAnimationValue(&listLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("value", valueBtnAnim);

		o2UI.AddWidgetStyle(sample, "list-tree");
	}

	void EditorUIStyleBuilder::RebuildActorsTreeNodeEnableToggle()
	{
		UIToggle* sample = mnew UIToggle();

		auto halfHideLayer = sample->AddLayer("halfHide", nullptr);

		auto back = halfHideLayer->AddChildLayer("back", mnew Sprite("ui/UI_off_dot.png"),
												 Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 0)));

		auto dot = halfHideLayer->AddChildLayer("dot", mnew Sprite("ui/UI_on_dot.png"),
												Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 0)));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		Animation valueAnim = Animation::EaseInOut(sample, &dot->transparency, 0.0f, 1.0f, 0.1f);
		*valueAnim.AddAnimationValue(&back->transparency) = AnimatedValue<float>::EaseInOut(1.0f, 0.0f, 0.1f);
		sample->AddState("value", valueAnim);

		sample->AddState("halfHide", Animation::EaseInOut(sample, &halfHideLayer->transparency, 1.0f, 0.5f, 0.1f));

		o2UI.AddWidgetStyle(sample, "actorNodeEnable");
	}

	void EditorUIStyleBuilder::RebuildActorsTreeLockToggle()
	{
		UIToggle* sample = mnew UIToggle();

		auto halfHideLayer = sample->AddLayer("halfHide", nullptr);

		auto locked = halfHideLayer->AddChildLayer("locked", mnew Sprite("ui/UI_lock_icon.png"),
												   Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 0)));

		auto unlocked = halfHideLayer->AddChildLayer("unlocked", mnew Sprite("ui/UI_unlock_icon.png"),
													 Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 0)));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		Animation valueStateAnim = Animation::EaseInOut(sample, &locked->transparency, 0.0f, 1.0f, 0.1f);
		*valueStateAnim.AddAnimationValue(&unlocked->transparency) = AnimatedValue<float>::EaseInOut(1.0f, 0.0f, 0.1f);
		sample->AddState("locked", valueStateAnim);

		sample->AddState("halfHide", Animation::EaseInOut(sample, &halfHideLayer->transparency, 1.0f, 0.5f, 0.1f));

		o2UI.AddWidgetStyle(sample, "actorNodeLock");
	}

	void EditorUIStyleBuilder::RebuildActorsTreeLinkBtn()
	{
		UIButton* sample = mnew UIButton();

		auto halfHideLayer = sample->AddLayer("halfHide", nullptr);
		auto selectLayer = halfHideLayer->AddChildLayer("hover", nullptr);
		auto pressedLayer = selectLayer->AddChildLayer("pressed", nullptr);
		auto back = pressedLayer->AddChildLayer("back", mnew Sprite("ui/UI_link_icon.png"),
												Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 0)));


		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));
		sample->AddState("halfHide", Animation::EaseInOut(sample, &halfHideLayer->transparency, 1.0f, 0.5f, 0.1f));
		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f));
		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.8f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "actorNodeLink");
	}

	void EditorUIStyleBuilder::RebuildActorsTree()
	{
		// basics
		UIActorsTree* sample = mnew UIActorsTree();
		sample->layout.minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 0, 1));
		sample->SetViewLayout(Layout::BothStretch(0, 0, 1, 0));
		sample->SetEnableScrollsHiding(true);
		sample->SetChildsNodesOffset(10);

		// hover
		*sample->GetHoverDrawable() = Sprite("ui/UI_ListBox_selection_hover.png");
		sample->SetHoverLayout(Layout::BothStretch(-10, -16, -10, -16));

		// hightlight
		*sample->GetHightlightDrawable() = Sprite("ui/UI_Window_place.png");
		sample->GetHightlightDrawable()->pivot = Vec2F(0.5f, 0.5f);
		sample->SetHightlightLayout(Layout::BothStretch());
		sample->SetHightlightAnimation(Animate(*sample->GetHightlightDrawable()).
									   Hide().Scale(1.5f).Then().
									   Wait(0.3f).Then().
									   Show().Scale(1.0f).For(0.2f).Then().
									   Wait(1.0f).Then().
									   Hide().For(0.2f));

		// node sample
		UITreeNode* itemSample = sample->GetNodeSample();

		auto itemSelectionLayer = itemSample->AddLayer("select", nullptr);

		auto itemFocusedLayer = itemSelectionLayer->AddChildLayer("focused", mnew Sprite("ui/UI_ListBox_selection_regular.png"),
																  Layout::BothStretch(-10, -16, -10, -16));

		auto itemUnfocusedLayer = itemSelectionLayer->AddChildLayer("unfocused", mnew Sprite("ui/UI_ListBox_selection_pressed.png"),
																	Layout::BothStretch(-10, -16, -10, -16));

		Text* captionLayerText = mnew Text("stdFont.ttf");
		captionLayerText->horAlign = HorAlign::Left;
		captionLayerText->verAlign = VerAlign::Middle;
		itemSample->AddLayer("name", captionLayerText, Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(12, -20), Vec2F(0, 0)));

		auto nameLayer = itemSample->layer["name"];
		nameLayer->layout = Layout::BothStretch(10, 0, 55, 0);
		((Text*)nameLayer->drawable)->dotsEngings = true;

		auto actorNodeEnableToggle = o2UI.CreateWidget<UIToggle>("actorNodeEnable");
		actorNodeEnableToggle->name = "enableToggle";
		actorNodeEnableToggle->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(-5, 0));
		itemSample->AddChild(actorNodeEnableToggle);

		auto actorNodeLockToggle = o2UI.CreateWidget<UIToggle>("actorNodeLock");
		actorNodeLockToggle->name = "lockToggle";
		actorNodeLockToggle->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(-20, 0));
		itemSample->AddChild(actorNodeLockToggle);

		auto actorNodeLinkButton = o2UI.CreateWidget<UIButton>("actorNodeLink");
		actorNodeLinkButton->name = "linkBtn";
		actorNodeLinkButton->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(-35, 0));
		itemSample->AddChild(actorNodeLinkButton);

		auto actorNodeEditBox = o2UI.CreateWidget<UIEditBox>("backless");
		actorNodeEditBox->name = "nameEditBox";
		actorNodeEditBox->layout = UIWidgetLayout::BothStretch(10, 0, 55, 0);
		actorNodeEditBox->Hide(true);
		itemSample->AddChild(actorNodeEditBox);

		Animation itemEditStateAnim = Animation::EaseInOut(itemSample, &nameLayer->transparency, 1.0f, 0.0f, 0.15f);
		*itemEditStateAnim.AddAnimationValue(&actorNodeEditBox->visible) = AnimatedValue<bool>::Linear(false, true, 0.15f);
		itemSample->AddState("edit", itemEditStateAnim);

		// node sample button
		UIButton* itemSampleExpandBtn = mnew UIButton();
		itemSampleExpandBtn->layout.minSize = Vec2F(5, 5);
		itemSampleExpandBtn->name = "expandBtn";

		auto regularLayer = itemSampleExpandBtn->AddLayer("regular", mnew Sprite("ui/UI_Right_icn.png"),
														  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto selectLayer = itemSampleExpandBtn->AddLayer("hover", mnew Sprite("ui/UI_Right_icn_select.png"),
														 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = itemSampleExpandBtn->AddLayer("pressed", mnew Sprite("ui/UI_Right_icn_pressed.png"),
														  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


		itemSampleExpandBtn->AddState("hover", Animation::EaseInOut(itemSampleExpandBtn, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
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

		Animation expandedStateAnim(itemSample);
		*expandedStateAnim.AddAnimationValue(&regularLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim.AddAnimationValue(&selectLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim.AddAnimationValue(&pressedLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		itemSample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;

		itemSample->AddState("selected", Animation::EaseInOut(itemSample, &itemSelectionLayer->transparency, 0.0f, 1.0f, 0.2f));

		Animation focusedItemAnim = Animation::EaseInOut(itemSample, &itemFocusedLayer->transparency, 0.0f, 1.0f, 0.2f);
		*focusedItemAnim.AddAnimationValue(&itemUnfocusedLayer->transparency) = AnimatedValue<float>::EaseInOut(0.3f, 0.0f, 0.2f);
		itemSample->AddState("focused", focusedItemAnim);

		// scrollbars
		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout.anchorMin = Vec2F(0, 0);
		horScrollBar->layout.anchorMax = Vec2F(1, 0);
		horScrollBar->layout.offsetMin = Vec2F(5, 0);
		horScrollBar->layout.offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout.anchorMin = Vec2F(1, 0);
		verScrollBar->layout.anchorMax = Vec2F(1, 1);
		verScrollBar->layout.offsetMin = Vec2F(-15, 15);
		verScrollBar->layout.offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout.offsetBottom,
						 5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout.offsetRight,
						 -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetHoverDrawable()->transparency, 0.0f, 0.8f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->SetStateForcible("hover", false);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void EditorUIStyleBuilder::RebuildPanelDownButton()
	{
		UIButton* sample = mnew UIButton();

		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_panel_button.png"),
										  Layout::BothStretch(-4, -4, -5, -5));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI_panel_button_select.png"),
											Layout::BothStretch(-4, -4, -5, -5));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI_panel_button_pressed.png"),
											 Layout::BothStretch(-4, -4, -5, -5));

		auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI_panel_button_focus.png"),
										   Layout::BothStretch(-4, -4, -5, -5));

		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI_Down_icn.png"),
										   Layout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, 0)));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "Button";
		captionText->horAlign = HorAlign::Middle;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		sample->AddLayer("caption", captionText, Layout::BothStretch(0, 0, 15, 0));

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "panel down");
	}

	void EditorUIStyleBuilder::RebuildTrashDownPanelButton()
	{
		auto sample = mnew UIButton();

		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_panel_button.png"),
										  Layout::BothStretch(-4, -5, -5, -5));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI_panel_button_select.png"),
											Layout::BothStretch(-4, -5, -5, -5));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI_panel_button_pressed.png"),
											 Layout::BothStretch(-4, -5, -5, -5));

		auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI_panel_button_focus.png"),
										   Layout::BothStretch(-4, -4, -5, -5));

		auto trashIconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI2_small_trash_icon.png"),
											   Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 0)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "down panel trash");
	}

	void EditorUIStyleBuilder::RebuildMessagesDownPanelToggle()
	{
		UIToggle* sample = mnew UIToggle();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_panel_button.png"),
										  Layout::BothStretch(-4, -5, -5, -5));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI_panel_button_select.png"),
											Layout::BothStretch(-4, -5, -5, -5));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI_panel_button_pressed.png"),
											 Layout::BothStretch(-4, -5, -5, -5));

		auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI_panel_button_focus.png"),
										   Layout::BothStretch(-4, -4, -5, -5));

		auto offLayer = sample->AddLayer("off", mnew Sprite("ui/UI2_log_info_inactive_icon.png"),
										 Layout::Based(BaseCorner::Left, Vec2F(30, 30), Vec2F(-4, -2)));

		auto onLayer = sample->AddLayer("on", mnew Sprite("ui/UI2_log_info_icon.png"),
										Layout::Based(BaseCorner::Left, Vec2F(30, 30), Vec2F(-4, -2)));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "999";
		captionText->horAlign = HorAlign::Middle;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		sample->AddLayer("caption", captionText, Layout::BothStretch(13, 0, 0, 0));

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		Animation valueBtnAnim = Animation::EaseInOut(sample, &offLayer->transparency, 1.0f, 0.0f, 0.1f);
		*valueBtnAnim.AddAnimationValue(&onLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("value", valueBtnAnim);

		o2UI.AddWidgetStyle(sample, "log messages");
	}

	void EditorUIStyleBuilder::RebuildWarningsDownPanelToggle()
	{
		UIToggle* sample = mnew UIToggle();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_panel_button.png"),
										  Layout::BothStretch(-4, -5, -5, -5));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI_panel_button_select.png"),
											Layout::BothStretch(-4, -5, -5, -5));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI_panel_button_pressed.png"),
											 Layout::BothStretch(-4, -5, -5, -5));

		auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI_panel_button_focus.png"),
										   Layout::BothStretch(-4, -4, -5, -5));

		auto offLayer = sample->AddLayer("off", mnew Sprite("ui/UI2_log_warnings_inactive_icon.png"),
										 Layout::Based(BaseCorner::Left, Vec2F(30, 30), Vec2F(-4, -2)));

		auto onLayer = sample->AddLayer("on", mnew Sprite("ui/UI2_log_warnings_icon.png"),
										Layout::Based(BaseCorner::Left, Vec2F(30, 30), Vec2F(-4, -2)));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "999";
		captionText->horAlign = HorAlign::Middle;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		sample->AddLayer("caption", captionText, Layout::BothStretch(13, 0, 0, 0));

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		Animation valueBtnAnim = Animation::EaseInOut(sample, &offLayer->transparency, 1.0f, 0.0f, 0.1f);
		*valueBtnAnim.AddAnimationValue(&onLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("value", valueBtnAnim);

		o2UI.AddWidgetStyle(sample, "log warnings");
	}

	void EditorUIStyleBuilder::RebuildErrorsDownPanelToggle()
	{
		UIToggle* sample = mnew UIToggle();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_panel_button.png"),
										  Layout::BothStretch(-4, -5, -5, -5));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI_panel_button_select.png"),
											Layout::BothStretch(-4, -5, -5, -5));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI_panel_button_pressed.png"),
											 Layout::BothStretch(-4, -5, -5, -5));

		auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI_panel_button_focus.png"),
										   Layout::BothStretch(-4, -4, -5, -5));

		auto offLayer = sample->AddLayer("off", mnew Sprite("ui/UI2_log_errors_inactive_icon.png"),
										 Layout::Based(BaseCorner::Left, Vec2F(30, 30), Vec2F(-4, -2)));

		auto onLayer = sample->AddLayer("on", mnew Sprite("ui/UI2_log_errors_icon.png"),
										Layout::Based(BaseCorner::Left, Vec2F(30, 30), Vec2F(-4, -2)));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "999";
		captionText->horAlign = HorAlign::Middle;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		sample->AddLayer("caption", captionText, Layout::BothStretch(13, 0, 0, 0));

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		Animation valueBtnAnim = Animation::EaseInOut(sample, &offLayer->transparency, 1.0f, 0.0f, 0.1f);
		*valueBtnAnim.AddAnimationValue(&onLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("value", valueBtnAnim);

		o2UI.AddWidgetStyle(sample, "log errors");
	}

	void EditorUIStyleBuilder::RebuildFilterMenuButton()
	{
		UIButton* sample = mnew UIButton();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_panel_button.png"),
										  Layout::BothStretch(-4, -4, -5, -5));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI_panel_button_select.png"),
											Layout::BothStretch(-4, -4, -5, -5));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI_panel_button_pressed.png"),
											 Layout::BothStretch(-4, -4, -5, -5));

		auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI_panel_button_focus.png"),
										   Layout::BothStretch(-4, -4, -5, -5));

		auto iconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI2_filter_icon.png"),
										  Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(1, 0)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "menu filter");
	}

	void EditorUIStyleBuilder::RebuildTreeMenuButton()
	{
		UIButton* sample = mnew UIButton();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_panel_button.png"),
										  Layout::BothStretch(-4, -5, -5, -5));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI_panel_button_select.png"),
											Layout::BothStretch(-4, -5, -5, -5));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI_panel_button_pressed.png"),
											 Layout::BothStretch(-4, -5, -5, -5));

		auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI_panel_button_focus.png"),
										   Layout::BothStretch(-4, -4, -5, -5));

		auto iconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI_tree_icon.png"),
										  Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 0)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "menu tree");
	}

	void EditorUIStyleBuilder::RebuildFoldersTree()
	{
		// basics
		UITree* sample = mnew UITree();
		sample->layout.minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 0, 1));
		sample->SetViewLayout(Layout::BothStretch(0, 0, 1, 0));
		sample->SetEnableScrollsHiding(true);
		sample->SetChildsNodesOffset(10);

		// hover
		*sample->GetHoverDrawable() = Sprite("ui/UI_ListBox_selection_hover.png");
		sample->SetHoverLayout(Layout::BothStretch(-10, -16, -10, -16));

		// hightlight
		*sample->GetHightlightDrawable() = Sprite("ui/UI_Window_place.png");
		sample->GetHightlightDrawable()->pivot = Vec2F(0.5f, 0.5f);
		sample->SetHightlightLayout(Layout::BothStretch());
		sample->SetHightlightAnimation(Animate(*sample->GetHightlightDrawable()).
									   Hide().Scale(1.5f).Then().
									   Wait(0.3f).Then().
									   Show().Scale(1.0f).For(0.2f).Then().
									   Wait(1.0f).Then().
									   Hide().For(0.2f));

		// node sample
		UITreeNode* itemSample = sample->GetNodeSample();

		auto itemSelectionLayer = itemSample->AddLayer("select", nullptr);

		auto itemFocusedLayer = itemSelectionLayer->AddChildLayer("focused", mnew Sprite("ui/UI_ListBox_selection_regular.png"),
																  Layout::BothStretch(-10, -16, -10, -16));

		auto itemUnfocusedLayer = itemSelectionLayer->AddChildLayer("unfocused", mnew Sprite("ui/UI_ListBox_selection_pressed.png"),
																	Layout::BothStretch(-10, -16, -10, -16));

		itemSample->AddLayer("icon", mnew Sprite("ui/UI2_folder_icon.png"), Layout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(10, 0)));

		Text* captionLayerText = mnew Text("stdFont.ttf");
		captionLayerText->horAlign = HorAlign::Left;
		captionLayerText->verAlign = VerAlign::Middle;
		itemSample->AddLayer("name", captionLayerText, Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(12, -20), Vec2F(0, 0)));

		auto nameLayer = itemSample->layer["name"];
		nameLayer->layout = Layout::BothStretch(30, 0, 5, 0);
		((Text*)nameLayer->drawable)->dotsEngings = true;

		auto actorNodeEditBox = o2UI.CreateWidget<UIEditBox>("backless");
		actorNodeEditBox->name = "nameEditBox";
		actorNodeEditBox->layout = UIWidgetLayout::BothStretch(30, 0, 5, 0);
		actorNodeEditBox->Hide(true);
		itemSample->AddChild(actorNodeEditBox);

		Animation itemEditStateAnim = Animation::EaseInOut(itemSample, &nameLayer->transparency, 1.0f, 0.0f, 0.15f);
		*itemEditStateAnim.AddAnimationValue(&actorNodeEditBox->visible) = AnimatedValue<bool>::Linear(false, true, 0.15f);
		itemSample->AddState("edit", itemEditStateAnim);

		// node sample button
		UIButton* itemSampleExpandBtn = mnew UIButton();
		itemSampleExpandBtn->layout.minSize = Vec2F(5, 5);
		itemSampleExpandBtn->name = "expandBtn";

		auto regularLayer = itemSampleExpandBtn->AddLayer("regular", mnew Sprite("ui/UI_Right_icn.png"),
														  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto selectLayer = itemSampleExpandBtn->AddLayer("hover", mnew Sprite("ui/UI_Right_icn_select.png"),
														 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = itemSampleExpandBtn->AddLayer("pressed", mnew Sprite("ui/UI_Right_icn_pressed.png"),
														  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


		itemSampleExpandBtn->AddState("hover", Animation::EaseInOut(itemSampleExpandBtn, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
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

		Animation expandedStateAnim(itemSample);
		*expandedStateAnim.AddAnimationValue(&regularLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim.AddAnimationValue(&selectLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim.AddAnimationValue(&pressedLayer->drawable->angle) =
			AnimatedValue<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		itemSample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;

		itemSample->AddState("selected", Animation::EaseInOut(itemSample, &itemSelectionLayer->transparency, 0.0f, 1.0f, 0.2f));

		Animation focusedItemAnim = Animation::EaseInOut(itemSample, &itemFocusedLayer->transparency, 0.0f, 1.0f, 0.2f);
		*focusedItemAnim.AddAnimationValue(&itemUnfocusedLayer->transparency) = AnimatedValue<float>::EaseInOut(0.3f, 0.0f, 0.2f);
		itemSample->AddState("focused", focusedItemAnim);

		// scrollbars
		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout.anchorMin = Vec2F(0, 0);
		horScrollBar->layout.anchorMax = Vec2F(1, 0);
		horScrollBar->layout.offsetMin = Vec2F(5, 0);
		horScrollBar->layout.offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout.anchorMin = Vec2F(1, 0);
		verScrollBar->layout.anchorMax = Vec2F(1, 1);
		verScrollBar->layout.offsetMin = Vec2F(-15, 15);
		verScrollBar->layout.offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout.offsetBottom,
						 5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout.offsetRight,
						 -5.0f, -15.0f, 0.2f));

		sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetHoverDrawable()->transparency, 0.0f, 0.8f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->SetStateForcible("hover", false);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5;

		o2UI.AddWidgetStyle(sample, "folders");
	}

	void EditorUIStyleBuilder::RebuildRegularAssetIcon()
	{
		UIAssetIcon* sample = mnew UIAssetIcon();

		// selection layer
		auto itemSelectionLayer = sample->AddLayer("select", nullptr);

		auto itemFocusedLayer = itemSelectionLayer->AddChildLayer("focused", mnew Sprite("ui/UI_ListBox_selection_regular.png"),
																  Layout::BothStretch(-10, -16, -10, -16));

		auto itemUnfocusedLayer = itemSelectionLayer->AddChildLayer("unfocused", mnew Sprite("ui/UI_ListBox_selection_pressed.png"),
																	Layout::BothStretch(-10, -16, -10, -16));

		// icon
		sample->AddLayer("icon", mnew Sprite("ui/UI2_big_file_icon.png"),
						 Layout::Based(BaseCorner::Center, Vec2F(40, 40), Vec2F(0, 10)));

		// hover
		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_ListBox_selection_hover.png"),
										   Layout::BothStretch(-10, -16, -10, -16));

		// name label
		UILabel* nameLabel = o2UI.CreateLabel("file name");
		nameLabel->name = "nameLabel";
		nameLabel->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 22, 0);
		nameLabel->height = 8;
		nameLabel->linesDistanceCoef = 0.7f;
		nameLabel->horAlign = HorAlign::Middle;
		nameLabel->verAlign = VerAlign::Top;
		nameLabel->horOverflow = UILabel::HorOverflow::Wrap;
		nameLabel->verOverflow = UILabel::VerOverflow::Cut;
		sample->AddChild(nameLabel);

		// edit box
		UIEditBox* nameEditBox = mnew UIEditBox();
		nameEditBox->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));
		nameEditBox->SetViewLayout(Layout::BothStretch(0, 0, 2, 0));
		nameEditBox->SetCaretBlinkingDelay(0.85f);
		nameEditBox->SetMultiLine(false);

		Text* textDrawable = nameEditBox->GetTextDrawable();
		textDrawable->SetFontAsset("stdFont.ttf");
		textDrawable->height = 8;

		Sprite* caretDrawable = nameEditBox->GetCaretDrawable();
		*caretDrawable = Sprite();
		caretDrawable->size = Vec2F(1, textDrawable->GetFont()->GetHeightPx(textDrawable->GetHeight())*1.7f);
		caretDrawable->pivot = Vec2F(0, 0.26f);
		caretDrawable->color = Color4::Black();

		nameEditBox->name = "nameEditBox";
		nameEditBox->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 22, 0);
		sample->AddChild(nameEditBox);

		// edit state
		Animation itemEditStateAnim = Animation::EaseInOut(sample, &nameLabel->visible, true, false, 0.15f);
		*itemEditStateAnim.AddAnimationValue(&nameEditBox->visible) = AnimatedValue<bool>::Linear(false, true, 0.15f);
		sample->AddState("edit", itemEditStateAnim);

		// focused state
		Animation focusedItemAnim = Animation::EaseInOut(sample, &itemFocusedLayer->transparency, 0.0f, 1.0f, 0.2f);
		*focusedItemAnim.AddAnimationValue(&itemUnfocusedLayer->transparency) = AnimatedValue<float>::EaseInOut(0.3f, 0.0f, 0.2f);
		sample->AddState("focused", focusedItemAnim);

		sample->AddState("halfHide", Animation::EaseInOut(sample, &sample->transparency, 1.0f, 0.5f, 0.1f));
		sample->AddState("selected", Animation::EaseInOut(sample, &itemSelectionLayer->transparency, 0.0f, 1.0f, 0.2f));
		sample->AddState("hover", Animation::EaseInOut(sample, &hoverLayer->transparency, 0.0f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void EditorUIStyleBuilder::RebuildFolderAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		auto iconLayer = sample->layer["icon"];
		((Sprite*)iconLayer->drawable)->LoadFromImage("ui/UI2_big_folder_icon.png");

		o2UI.AddWidgetStyle(sample, "folder");
	}

	void EditorUIStyleBuilder::RebuildPrototypeAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		auto iconLayer = sample->layer["icon"];
		((Sprite*)iconLayer->drawable)->LoadFromImage("ui/UI2_actor_icon.png");

		o2UI.AddWidgetStyle(sample, "prefab");
	}

	void EditorUIStyleBuilder::RebuildPrefabPreviewAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		auto iconLayer = sample->layer["icon"];
		((Sprite*)iconLayer->drawable)->LoadFromImage("ui/UI2_image_asset_back.png");

		sample->AddLayer("preview", mnew Sprite(),
						 Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 20)));

		o2UI.AddWidgetStyle(sample, "prefab preview");
	}

	void EditorUIStyleBuilder::RebuildImagePreviewAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		sample->RemoveLayer("icon");
		sample->AddLayer("preview", mnew Sprite(),
						 Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 15)));

		o2UI.AddWidgetStyle(sample, "image preview");
	}

	void EditorUIStyleBuilder::RebuildTextAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		auto iconLayer = sample->layer["icon"];
		((Sprite*)iconLayer->drawable)->LoadFromImage("ui/UI2_big_text_file_icon.png");

		o2UI.AddWidgetStyle(sample, "text");
	}

	void EditorUIStyleBuilder::RebuildAnimationAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		auto iconLayer = sample->layer["icon"];
		((Sprite*)iconLayer->drawable)->LoadFromImage("ui/UI2_anim_file_icon.png");

		o2UI.AddWidgetStyle(sample, "animation");
	}

	void EditorUIStyleBuilder::RebuildAssetsGridScroll()
	{
		UIAssetsIconsScrollArea* sample = mnew UIAssetsIconsScrollArea();
		sample->layout.minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);
		
		// hightlight
		*sample->GetHightlightDrawable() = Sprite("ui/UI_Window_place.png");
		sample->GetHightlightDrawable()->pivot = Vec2F(0.5f, 0.5f);
		sample->SetHightlightLayout(Layout::BothStretch());
		sample->SetHightlightAnimation(Animate(*sample->GetHightlightDrawable()).
									   Hide().Scale(1.5f).Then().
									   Wait(0.3f).Then().
									   Show().Scale(1.0f).For(0.2f).Then().
									   Wait(1.0f).Then().
									   Hide().For(0.2f));

		// selection
		*sample->GetSelectingDrawable() = Sprite("ui/UI_Window_place.png");

		UIHorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
		horScrollBar->layout.anchorMin = Vec2F(0, 0);
		horScrollBar->layout.anchorMax = Vec2F(1, 0);
		horScrollBar->layout.offsetMin = Vec2F(5, 0);
		horScrollBar->layout.offsetMax = Vec2F(-15, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		UIVerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout.anchorMin = Vec2F(1, 0);
		verScrollBar->layout.anchorMax = Vec2F(1, 1);
		verScrollBar->layout.offsetMin = Vec2F(-15, 15);
		verScrollBar->layout.offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void EditorUIStyleBuilder::RebuildLinkBtn()
	{
		UIButton* sample = mnew UIButton();

		auto halfHideLayer = sample->AddLayer("halfHide", nullptr);
		auto selectLayer = halfHideLayer->AddChildLayer("hover", nullptr);
		auto pressedLayer = selectLayer->AddChildLayer("pressed", nullptr);
		auto back = pressedLayer->AddChildLayer("back", mnew Sprite("ui/UI2_link_icon.png"),
												Layout::Based(BaseCorner::Center, Vec2F(15, 15), Vec2F(0, 0)));


		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));
		sample->AddState("halfHide", Animation::EaseInOut(sample, &halfHideLayer->transparency, 1.0f, 0.5f, 0.1f));
		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f));
		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.8f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "asset link");
	}

	void EditorUIStyleBuilder::RebuildSinglelineEditBoxWithArrows()
	{
		UIEditBox* sample = o2UI.CreateWidget<UIEditBox>("singleline");
		sample->SetClippingLayout(Layout::BothStretch(0, 0, 10, 0));
		sample->SetViewLayout(Layout::BothStretch(3, 1, 8, -1));

		sample->AddLayer("arrows", mnew Sprite("ui/UI3_ch_arrows.png"), 
						 Layout::Based(BaseCorner::Right, Vec2F(10, 20), Vec2F(0, 0)));

		o2UI.AddWidgetStyle(sample, "singleline with arrows");
	}

	void EditorUIStyleBuilder::RebuildEditorDropdown()
	{
		UIDropDown* sample = mnew UIDropDown();
		sample->layout.minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto selectLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_Editbox_pressed.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI_Down_icn.png"),
										   Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-20, -10), Vec2F(0, 10)));

		sample->SetClippingLayout(Layout::BothStretch(4, 2, 20, 2));

		auto list = sample->GetListView();
		*list = *o2UI.GetWidgetStyle<UICustomList>("standard");
		list->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		delete list->layer["back"]->drawable;
		list->layer["back"]->drawable = mnew Sprite("ui/UI_Box_regular.png");
		list->layout.pivot = Vec2F(0.5f, 1.0f);
		list->layout.anchorMin = Vec2F(0, 0);
		list->layout.anchorMax = Vec2F(1, 0);
		list->layout.offsetMin = Vec2F(0, -60);
		list->layout.offsetMax = Vec2F(0, 1);

		Text* undefinedText = mnew Text("stdFont.ttf");
		undefinedText->text = "--";
		undefinedText->horAlign = HorAlign::Left;
		undefinedText->verAlign = VerAlign::Middle;
		undefinedText->dotsEngings = true;
		auto undefinedLayer = sample->AddLayer("undefined", undefinedText, Layout::BothStretch(3, 0, 3, 0));

		UILabel* itemSample = o2UI.CreateLabel("empty");
		itemSample->horAlign = HorAlign::Left;
		sample->SetItemSample(itemSample);

		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("opened", Animation::EaseInOut(sample, &arrowLayer->drawable->scale, Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("undefined", Animation::EaseInOut(sample, &undefinedLayer->transparency, 0.0f, 1.0f, 0.05f));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "editor property");
	}

	void EditorUIStyleBuilder::RebuildActorHeadEnableToggle()
	{
		UIToggle* sample = mnew UIToggle();
		sample->layout.minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI2_enable_toggle_big.png"),
										  Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto hoverLayer = sample->AddLayer("backSelect", mnew Sprite("ui/UI2_enable_toggle_big_select.png"),
										   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedLayer = sample->AddLayer("backPressed", mnew Sprite("ui/UI2_enable_toggle_big_pressed.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto focusLayer = sample->AddLayer("backFocus", mnew Sprite("ui/UI2_enable_toggle_big_focused.png"),
										   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto checkLayer = sample->AddLayer("check", mnew Sprite("ui/UI2_enable_dot_big.png"),
										   Layout::Based(BaseCorner::Center, Vec2F(10, 10)));

		auto unknownLayer = sample->AddLayer("unknown", mnew Sprite("ui/UI2_enable_dot_big_unknown.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(10, 10)));

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

		o2UI.AddWidgetStyle(sample, "actorHeadEnable");
	}

	void EditorUIStyleBuilder::RebuildActorHeadName()
	{
		UIEditBox* sample = mnew UIEditBox();
		sample->SetClippingLayout(Layout::BothStretch(5, 0, 5, 0));
		sample->SetViewLayout(Layout::BothStretch(7, 0, 7, 0));
		sample->SetCaretBlinkingDelay(0.85f);
		sample->SetMultiLine(false);
		sample->layout.minSize = Vec2F(50, 17);

		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI2_round_field.png"),
										  Layout::BothStretch(-4, -4, -5, -4));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"),
										   Layout::BothStretch(-4, -4, -5, -4));

		auto focusLayer = sample->AddLayer("focus", mnew Sprite("ui/UI2_round_field_select.png"),
										   Layout::BothStretch(-4, -4, -5, -4));

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

		o2UI.AddWidgetStyle(sample, "actorHeadName");
	}

	void EditorUIStyleBuilder::RebuildActorHeadLockToggle()
	{
		UIToggle* sample = mnew UIToggle();
		sample->layout.minSize = Vec2F(20, 20);

		auto rootLayer = sample->AddLayer("root", nullptr);
		auto valueLayer = rootLayer->AddChildLayer("value", nullptr);

		auto unlockLayer = valueLayer->AddChildLayer("unlock", mnew Sprite("ui/UI2_big_lock_open.png"),
													 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto lockLayer = valueLayer->AddChildLayer("lock", mnew Sprite("ui/UI2_big_lock_close.png"),
												   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto unknownLayer = rootLayer->AddChildLayer("unknown", mnew Sprite("ui/UI2_big_lock_unknown.png"),
													 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("hover", Animation::EaseInOut(sample, &rootLayer->transparency, 1.0f, 0.7f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		auto valueAnim = Animation::EaseInOut(sample, &unlockLayer->transparency, 1.0f, 0.0f, 0.1f);
		*valueAnim.AddAnimationValue(&lockLayer->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("value", valueAnim)->offStateAnimationSpeed = 0.5f;

		auto unknownAnim = Animation::EaseInOut(sample, &valueLayer->transparency, 1.0f, 0.0f, 0.1f);
		*unknownAnim.AddAnimationValue(&unknownLayer->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("unknown", unknownAnim)->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "actorHeadLock");
	}

	void EditorUIStyleBuilder::RebuildActorHeadActorAssetProperty()
	{
		auto widget = mnew UIWidget();
		widget->SetFocusable(true);

		auto backLayer = widget->AddLayer("back", mnew Sprite("ui/UI2_round_field_gray.png"),
										  Layout::BothStretch(-4, -4, -5, -4));

		auto selectLayer = widget->AddLayer("hover", mnew Sprite("ui/UI2_round_field_gray_select.png"),
											Layout::BothStretch(-4, -4, -5, -4));

		auto focusLayer = widget->AddLayer("focus", mnew Sprite("ui/UI2_round_field_focused.png"),
										   Layout::BothStretch(-4, -4, -5, -4));

		widget->AddState("focused", Animation::EaseInOut(widget, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		widget->AddState("hover", Animation::EaseInOut(widget, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		auto nameText = mnew Text("stdFont.ttf");
		nameText->text = "--";
		nameText->horAlign = HorAlign::Left;
		nameText->verAlign = VerAlign::Middle;
		nameText->dotsEngings = true;
		widget->AddLayer("caption", nameText, Layout::BothStretch(5, 1, 5, 1));

		auto linkBtn = o2UI.CreateWidget<UIButton>("asset link");
		linkBtn->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(15, 15), Vec2F());
		widget->AddChild(linkBtn);

		o2UI.AddWidgetStyle(widget, "actorHeadAssetProperty");
	}

	void EditorUIStyleBuilder::RebuildActorHeadTagsProperty()
	{
		UIEditBox* sample = mnew UIEditBox();
		sample->SetClippingLayout(Layout::BothStretch(5, 0, 5, 0));
		sample->SetViewLayout(Layout::BothStretch(7, 0, 7, 0));
		sample->SetCaretBlinkingDelay(0.85f);
		sample->SetMultiLine(false);
		sample->layout.minSize = Vec2F(50, 17);

		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI2_round_field_gray.png"),
										  Layout::BothStretch(-4, -4, -5, -4));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI2_round_field_gray_select.png"),
										   Layout::BothStretch(-4, -4, -5, -4));

		auto focusLayer = sample->AddLayer("focus", mnew Sprite("ui/UI2_round_field_select.png"),
										   Layout::BothStretch(-4, -4, -5, -4));

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

		o2UI.AddWidgetStyle(sample, "actorHeadTags");
	}

	void EditorUIStyleBuilder::RebuildActorHeadLayerProperty()
	{
		UIDropDown* sample = mnew UIDropDown();
		sample->layout.minSize = Vec2F(20, 20);

		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI2_round_field_gray.png"),
										  Layout::BothStretch(-4, -4, -5, -4));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI2_round_field_gray_select.png"),
										   Layout::BothStretch(-4, -4, -5, -4));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI2_round_field_pressed.png"),
										   Layout::BothStretch(-4, -4, -5, -4));

		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI_Down_icn.png"),
										   Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-20, -10), Vec2F(0, 10)));

		sample->SetClippingLayout(Layout::BothStretch(4, 2, 20, 2));

		auto list = sample->GetListView();
		*list = *o2UI.GetWidgetStyle<UICustomList>("standard");
		list->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		delete list->layer["back"]->drawable;
		list->layer["back"]->drawable = mnew Sprite("ui/UI_Box_regular.png");
		list->layout.pivot = Vec2F(0.5f, 1.0f);
		list->layout.anchorMin = Vec2F(0, 0);
		list->layout.anchorMax = Vec2F(1, 0);
		list->layout.offsetMin = Vec2F(-1, -60);
		list->layout.offsetMax = Vec2F(0, 3);

		Text* undefinedText = mnew Text("stdFont.ttf");
		undefinedText->text = "--";
		undefinedText->horAlign = HorAlign::Left;
		undefinedText->verAlign = VerAlign::Middle;
		undefinedText->dotsEngings = true;
		auto undefinedLayer = sample->AddLayer("undefined", undefinedText, Layout::BothStretch(3, 0, 3, 0));

		UILabel* itemSample = o2UI.CreateLabel("empty");
		itemSample->horAlign = HorAlign::Left;
		sample->SetItemSample(itemSample);

		sample->AddState("hover", Animation::EaseInOut(sample, &hoverLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("opened", Animation::EaseInOut(sample, &arrowLayer->drawable->scale, Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("undefined", Animation::EaseInOut(sample, &undefinedLayer->transparency, 0.0f, 1.0f, 0.05f));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "actorHeadLayer");
	}

	void EditorUIStyleBuilder::RebuildAcceptPrototypeBtn()
	{
		UIButton* sample = mnew UIButton();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI2_accept_prefab.png"),
										  Layout::Based(BaseCorner::Center, Vec2F(25, 25)));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI2_accept_prefab_select.png"),
											Layout::Based(BaseCorner::Center, Vec2F(25, 25)));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI2_accept_prefab_pressed.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(25, 25)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "acceptPrototype");
	}

	void EditorUIStyleBuilder::RebuildRevertPrototypeBtn()
	{
		UIButton* sample = mnew UIButton();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI2_revert_prefab.png"),
										  Layout::Based(BaseCorner::Center, Vec2F(25, 25)));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI2_revert_prefab_select.png"),
											Layout::Based(BaseCorner::Center, Vec2F(25, 25)));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI2_revert_prefab_pressed.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(25, 25)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "revertPrototype");
	}

	void EditorUIStyleBuilder::RebuildBreakPrototypeBtn()
	{
		UIButton* sample = mnew UIButton();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI2_break_prefab.png"),
										  Layout::Based(BaseCorner::Center, Vec2F(25, 25)));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI2_break_prefab_select.png"),
											Layout::Based(BaseCorner::Center, Vec2F(25, 25)));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI2_break_prefab_pressed.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(25, 25)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "breakPrototype");
	}

	void EditorUIStyleBuilder::RebuildComponentOptionsBtn()
	{
		UIButton* sample = mnew UIButton();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI2_gray_options.png"),
										  Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI2_gray_options_select.png"),
											Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI2_gray_options_pressed.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "componentOptions");
	}

	void EditorUIStyleBuilder::RebuildComponentSaveBtn()
	{
		UIButton* sample = mnew UIButton();
		auto backLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI2_save_gray.png"),
										  Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto selectLayer = sample->AddLayer("selectBack", mnew Sprite("ui/UI2_save_gray copy.png"),
											Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedLayer = sample->AddLayer("pressedBack", mnew Sprite("ui/UI2_save_gray.png"),
											 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));


		sample->AddState("hover", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "componentSave");
	}

	void EditorUIStyleBuilder::RebuildRedEditBoxStyle()
	{
		UIEditBox* sample = mnew UIEditBox();
		sample->SetClippingLayout(Layout::BothStretch(0, 0, 10, 0));
		sample->SetViewLayout(Layout::BothStretch(3, 1, 8, -1));
		sample->SetCaretBlinkingDelay(0.85f);
		sample->SetMultiLine(false);
		sample->layout.minSize = Vec2F(10, 10);

		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI2_red_edit_box.png"), Layout::BothStretch(-4, -4, -4, -4));
		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto focusLayer = sample->AddLayer("focus", mnew Sprite("ui/UI_Editbox_focus.png"), Layout::BothStretch(-9, -9, -9, -9));
		sample->AddLayer("arrows", mnew Sprite("ui/UI3_ch_arrows.png"),
						 Layout::Based(BaseCorner::Right, Vec2F(10, 20), Vec2F(0, 0)));

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

		o2UI.AddWidgetStyle(sample, "red singleline");
	}

	void EditorUIStyleBuilder::RebuildGreenEditBoxStyle()
	{
		UIEditBox* sample = mnew UIEditBox();
		sample->SetClippingLayout(Layout::BothStretch(0, 0, 10, 0));
		sample->SetViewLayout(Layout::BothStretch(3, 1, 8, -1));
		sample->SetCaretBlinkingDelay(0.85f);
		sample->SetMultiLine(false);
		sample->layout.minSize = Vec2F(10, 10);

		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI2_green_edit_box.png"), Layout::BothStretch(-4, -4, -4, -4));
		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"), Layout::BothStretch(-9, -9, -9, -9));
		auto focusLayer = sample->AddLayer("focus", mnew Sprite("ui/UI_Editbox_focus.png"), Layout::BothStretch(-9, -9, -9, -9));
		sample->AddLayer("arrows", mnew Sprite("ui/UI3_ch_arrows.png"),
						 Layout::Based(BaseCorner::Right, Vec2F(10, 20), Vec2F(0, 0)));

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

		o2UI.AddWidgetStyle(sample, "green singleline");
	}

	void EditorUIStyleBuilder::RebuildActorPropety()
	{
		auto widget = mnew UIWidget();

		widget->SetFocusable(true);

		auto backLayer = widget->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), 
										  Layout::BothStretch(-9, -9, -9, -9));

		auto selectLayer = widget->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"), 
											Layout::BothStretch(-9, -9, -9, -9));

		auto focusLayer = widget->AddLayer("focus", mnew Sprite("ui/UI_Editbox_focus.png"),
										   Layout::BothStretch(-9, -9, -9, -9));

		widget->AddState("focused", Animation::EaseInOut(widget, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		widget->AddState("hover", Animation::EaseInOut(widget, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		auto nameText = mnew Text("stdFont.ttf");
		nameText->text = "--";
		nameText->horAlign = HorAlign::Left;
		nameText->verAlign = VerAlign::Middle;
		nameText->dotsEngings = true;
		widget->AddLayer("caption", nameText, Layout::BothStretch(2, 2, 2, 2));

		widget->SetFocusable(true);

		auto linkBtn = o2UI.CreateWidget<UIButton>("asset link");
		linkBtn->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(15, 15), Vec2F());
		widget->AddChild(linkBtn);

		o2UI.AddWidgetStyle(widget, "actorProperty");
	}

	void EditorUIStyleBuilder::RebuildAssetPropety()
	{
		auto widget = mnew UIWidget();
		widget->SetFocusable(true);

		auto backLayer = widget->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), 
										  Layout::BothStretch(-9, -9, -9, -9));

		auto selectLayer = widget->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"), 
											Layout::BothStretch(-9, -9, -9, -9));

		auto focusLayer = widget->AddLayer("focus", mnew Sprite("ui/UI_Editbox_focus.png"), 
										   Layout::BothStretch(-9, -9, -9, -9));

		widget->AddState("focused", Animation::EaseInOut(widget, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		widget->AddState("hover", Animation::EaseInOut(widget, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		auto nameText = mnew Text("stdFont.ttf");
		nameText->text = "--";
		nameText->horAlign = HorAlign::Left;
		nameText->verAlign = VerAlign::Middle;
		nameText->dotsEngings = true;
		widget->AddLayer("caption", nameText, Layout::BothStretch(2, 2, 2, 2));

		auto linkBtn = o2UI.CreateWidget<UIButton>("asset link");
		linkBtn->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(15, 15), Vec2F());
		widget->AddChild(linkBtn);

		o2UI.AddWidgetStyle(widget, "assetProperty");
	}

	void EditorUIStyleBuilder::RebuildComponentProperty()
	{
		auto widget = mnew UIWidget();
		widget->SetFocusable(true);

		auto backLayer = widget->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"),
										  Layout::BothStretch(-9, -9, -9, -9));

		auto selectLayer = widget->AddLayer("hover", mnew Sprite("ui/UI_Editbox_select.png"),
											Layout::BothStretch(-9, -9, -9, -9));

		auto focusLayer = widget->AddLayer("focus", mnew Sprite("ui/UI_Editbox_focus.png"),
										   Layout::BothStretch(-9, -9, -9, -9));

		widget->AddState("focused", Animation::EaseInOut(widget, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		widget->AddState("hover", Animation::EaseInOut(widget, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		auto nameText = mnew Text("stdFont.ttf");
		nameText->text = "--";
		nameText->horAlign = HorAlign::Left;
		nameText->verAlign = VerAlign::Middle;
		nameText->dotsEngings = true;
		widget->AddLayer("caption", nameText, Layout::BothStretch(2, 2, 2, 2));

		auto linkBtn = o2UI.CreateWidget<UIButton>("asset link");
		linkBtn->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(15, 15), Vec2F());
		widget->AddChild(linkBtn);

		o2UI.AddWidgetStyle(widget, "componentProperty");
	}

	void EditorUIStyleBuilder::RebuildVector2Property()
	{
		auto layout = mnew UIWidget();
		layout->name = "vector2 property";

		auto xLabel = o2UI.CreateLabel("X");
		xLabel->layout = UIWidgetLayout(Vec2F(0, 0), Vec2F(0.0f, 1.0f), Vec2F(0, 0), Vec2F(20, 0));
		xLabel->horOverflow = UILabel::HorOverflow::None;
		layout->AddChild(xLabel);

		auto xEdit = o2UI.CreateWidget<UIEditBox>("singleline with arrows");
		xEdit->name = "x edit";
		xEdit->layout = UIWidgetLayout(Vec2F(0, 0), Vec2F(0.5f, 1.0f), Vec2F(20, 0), Vec2F());
		layout->AddChild(xEdit);

		auto yLabel = o2UI.CreateLabel("Y");
		yLabel->layout.maxWidth = 15;
		yLabel->layout = UIWidgetLayout(Vec2F(0.5f, 0), Vec2F(0.5f, 1.0f), Vec2F(0, 0), Vec2F(20, 0));
		yLabel->horOverflow = UILabel::HorOverflow::None;
		layout->AddChild(yLabel);

		auto yEdit = o2UI.CreateWidget<UIEditBox>("singleline with arrows");
		yEdit->name = "y edit";
		yEdit->layout = UIWidgetLayout(Vec2F(0.5f, 0), Vec2F(1, 1.0f), Vec2F(20, 0), Vec2F());
		layout->AddChild(yEdit);

		o2UI.AddWidgetStyle(layout, "vector2 property");
	}

	void EditorUIStyleBuilder::RebuildColoredVector2Property()
	{
		auto layout = mnew UIWidget();
		layout->name = "vector2 property";

		auto xLabel = o2UI.CreateLabel("X");
		xLabel->layout = UIWidgetLayout(Vec2F(0, 0), Vec2F(0.0f, 1.0f), Vec2F(0, 0), Vec2F(20, 0));
		xLabel->horOverflow = UILabel::HorOverflow::None;
		layout->AddChild(xLabel);

		auto xEdit = o2UI.CreateWidget<UIEditBox>("red singleline");
		xEdit->name = "x edit";
		xEdit->layout = UIWidgetLayout(Vec2F(0, 0), Vec2F(0.5f, 1.0f), Vec2F(20, 0), Vec2F());
		layout->AddChild(xEdit);

		auto yLabel = o2UI.CreateLabel("Y");
		yLabel->layout.maxWidth = 15;
		yLabel->layout = UIWidgetLayout(Vec2F(0.5f, 0), Vec2F(0.5f, 1.0f), Vec2F(0, 0), Vec2F(20, 0));
		yLabel->horOverflow = UILabel::HorOverflow::None;
		layout->AddChild(yLabel);

		auto yEdit = o2UI.CreateWidget<UIEditBox>("green singleline");
		yEdit->name = "y edit";
		yEdit->layout = UIWidgetLayout(Vec2F(0.5f, 0), Vec2F(1, 1.0f), Vec2F(20, 0), Vec2F());
		layout->AddChild(yEdit);

		o2UI.AddWidgetStyle(layout, "colored vector2 property");
	}

	void EditorUIStyleBuilder::RebuildRectProperty()
	{
		auto layout = mnew UIWidget();
		layout->name = "rectangle property";

		// left
		auto leftLabel = o2UI.CreateLabel("L");
		leftLabel->layout = UIWidgetLayout(Vec2F(0, 0), Vec2F(0.0f, 1.0f), Vec2F(0, 0), Vec2F(15, 0));
		leftLabel->horOverflow = UILabel::HorOverflow::None;
		layout->AddChild(leftLabel);

		auto leftEdit = o2UI.CreateWidget<UIEditBox>("singleline with arrows");
		leftEdit->name = "left edit";
		leftEdit->layout = UIWidgetLayout(Vec2F(0, 0), Vec2F(0.25f, 1.0f), Vec2F(15, 0), Vec2F());
		layout->AddChild(leftEdit);

		// bottom
		auto bottomLabel = o2UI.CreateLabel("B");
		bottomLabel->layout = UIWidgetLayout(Vec2F(0.25f, 0), Vec2F(0.25f, 1.0f), Vec2F(0, 0), Vec2F(15, 0));
		bottomLabel->horOverflow = UILabel::HorOverflow::None;
		layout->AddChild(bottomLabel);

		auto bottomEdit = o2UI.CreateWidget<UIEditBox>("singleline with arrows");
		bottomEdit->name = "bottom edit";
		bottomEdit->layout = UIWidgetLayout(Vec2F(0.25f, 0), Vec2F(0.5f, 1.0f), Vec2F(15, 0), Vec2F());
		layout->AddChild(bottomEdit);

		// right
		auto rightLabel = o2UI.CreateLabel("R");
		rightLabel->layout = UIWidgetLayout(Vec2F(0.5f, 0), Vec2F(0.5f, 1.0f), Vec2F(0, 0), Vec2F(15, 0));
		rightLabel->horOverflow = UILabel::HorOverflow::None;
		layout->AddChild(rightLabel);

		auto rightEdit = o2UI.CreateWidget<UIEditBox>("singleline with arrows");
		rightEdit->name = "right edit";
		rightEdit->layout = UIWidgetLayout(Vec2F(0.5f, 0), Vec2F(0.75f, 1.0f), Vec2F(15, 0), Vec2F());
		layout->AddChild(rightEdit);

		// top
		auto topLabel = o2UI.CreateLabel("T");
		topLabel->layout = UIWidgetLayout(Vec2F(0.75f, 0), Vec2F(0.75f, 1.0f), Vec2F(0, 0), Vec2F(15, 0));
		topLabel->horOverflow = UILabel::HorOverflow::None;
		layout->AddChild(topLabel);

		auto topEdit = o2UI.CreateWidget<UIEditBox>("singleline with arrows");
		topEdit->name = "top edit";
		topEdit->layout = UIWidgetLayout(Vec2F(0.75f, 0), Vec2F(1.0f, 1.0f), Vec2F(15, 0), Vec2F());
		layout->AddChild(topEdit);

		o2UI.AddWidgetStyle(layout, "rectangle property");
	}

	void EditorUIStyleBuilder::RebuildEditorUIStyle()
	{
		o2UI.ClearStyle();

		auto funcs = GetType().GetFunctionsWithBaseClasses();
		for (auto func : funcs)
		{
			if (func->GetName() == "RebuildBasicUIStyle" || func->GetName() == "RebuildEditorUIStyle")
				continue;

			func->Invoke<void>(this);
		}

		o2UI.SaveStyle("editor_ui_style.xml");
	}
}
 
CLASS_META(Editor::EditorUIStyleBuilder)
{
	BASE_CLASS(o2::BasicUIStyleBuilder);


	PUBLIC_FUNCTION(void, RebuildExpandButton);
	PUBLIC_FUNCTION(void, RebuildDockableWndStyle);
	PUBLIC_FUNCTION(void, RebuildPlayStopButtonStyle);
	PUBLIC_FUNCTION(void, RebuildPauseButtonStyle);
	PUBLIC_FUNCTION(void, RebuildStepButtonStyle);
	PUBLIC_FUNCTION(void, RebuildRoundDropDown);
	PUBLIC_FUNCTION(void, RebuildArrowToggle);
	PUBLIC_FUNCTION(void, RebuildBrushToggle);
	PUBLIC_FUNCTION(void, RebuildMoveToggle);
	PUBLIC_FUNCTION(void, RebuildRotateToggle);
	PUBLIC_FUNCTION(void, RebuildScaleToggle);
	PUBLIC_FUNCTION(void, RebuildFrameToggle);
	PUBLIC_FUNCTION(void, RebuildSearchButton);
	PUBLIC_FUNCTION(void, RebuildListTreeToggle);
	PUBLIC_FUNCTION(void, RebuildActorsTreeNodeEnableToggle);
	PUBLIC_FUNCTION(void, RebuildActorsTreeLockToggle);
	PUBLIC_FUNCTION(void, RebuildActorsTreeLinkBtn);
	PUBLIC_FUNCTION(void, RebuildActorsTree);
	PUBLIC_FUNCTION(void, RebuildPanelDownButton);
	PUBLIC_FUNCTION(void, RebuildTrashDownPanelButton);
	PUBLIC_FUNCTION(void, RebuildMessagesDownPanelToggle);
	PUBLIC_FUNCTION(void, RebuildWarningsDownPanelToggle);
	PUBLIC_FUNCTION(void, RebuildErrorsDownPanelToggle);
	PUBLIC_FUNCTION(void, RebuildFilterMenuButton);
	PUBLIC_FUNCTION(void, RebuildTreeMenuButton);
	PUBLIC_FUNCTION(void, RebuildFoldersTree);
	PUBLIC_FUNCTION(void, RebuildRegularAssetIcon);
	PUBLIC_FUNCTION(void, RebuildFolderAssetIcon);
	PUBLIC_FUNCTION(void, RebuildPrototypeAssetIcon);
	PUBLIC_FUNCTION(void, RebuildPrefabPreviewAssetIcon);
	PUBLIC_FUNCTION(void, RebuildImagePreviewAssetIcon);
	PUBLIC_FUNCTION(void, RebuildTextAssetIcon);
	PUBLIC_FUNCTION(void, RebuildAnimationAssetIcon);
	PUBLIC_FUNCTION(void, RebuildAssetsGridScroll);
	PUBLIC_FUNCTION(void, RebuildLinkBtn);
	PUBLIC_FUNCTION(void, RebuildSinglelineEditBoxWithArrows);
	PUBLIC_FUNCTION(void, RebuildEditorDropdown);
	PUBLIC_FUNCTION(void, RebuildActorPropety);
	PUBLIC_FUNCTION(void, RebuildAssetPropety);
	PUBLIC_FUNCTION(void, RebuildComponentProperty);
	PUBLIC_FUNCTION(void, RebuildVector2Property);
	PUBLIC_FUNCTION(void, RebuildRedEditBoxStyle);
	PUBLIC_FUNCTION(void, RebuildGreenEditBoxStyle);
	PUBLIC_FUNCTION(void, RebuildColoredVector2Property);
	PUBLIC_FUNCTION(void, RebuildRectProperty);
	PUBLIC_FUNCTION(void, RebuildActorHeadEnableToggle);
	PUBLIC_FUNCTION(void, RebuildActorHeadName);
	PUBLIC_FUNCTION(void, RebuildActorHeadLockToggle);
	PUBLIC_FUNCTION(void, RebuildActorHeadActorAssetProperty);
	PUBLIC_FUNCTION(void, RebuildActorHeadTagsProperty);
	PUBLIC_FUNCTION(void, RebuildActorHeadLayerProperty);
	PUBLIC_FUNCTION(void, RebuildAcceptPrototypeBtn);
	PUBLIC_FUNCTION(void, RebuildRevertPrototypeBtn);
	PUBLIC_FUNCTION(void, RebuildBreakPrototypeBtn);
	PUBLIC_FUNCTION(void, RebuildComponentOptionsBtn);
	PUBLIC_FUNCTION(void, RebuildComponentSaveBtn);
	PUBLIC_FUNCTION(void, RebuildEditorUIStyle);
}
END_META;
 