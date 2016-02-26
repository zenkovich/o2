#include "EditorUIStyle.h"

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

namespace UIStyle
{
	void RebuildDockableWndStyle()
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

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout.offsetBottom,
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout.offsetBottom,
															  -5.0f, -15.0f, 0.2f));

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

	void RebuildPlayStopButtonStyle()
	{
		UIToggle* sample = mnew UIToggle();
		auto playRootIconLayer = sample->AddLayer("playRootIcon", nullptr);
		auto stopRootIconLayer = sample->AddLayer("stopRootIcon", nullptr);

		auto playIconLayer = playRootIconLayer->AddChildLayer("regular", mnew Sprite("ui/UI_play_btn_regular.png"),
															  Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto playSelectIconLayer = playRootIconLayer->AddChildLayer("select", mnew Sprite("ui/UI_play_btn_select.png"),
																	Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto playPressedIconLayer = playRootIconLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_play_btn_select copy.png"),
																	 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto stopIconLayer = stopRootIconLayer->AddChildLayer("regular", mnew Sprite("ui/UI_stop_btn_regular.png"),
															  Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto stopSelectIconLayer = stopRootIconLayer->AddChildLayer("select", mnew Sprite("ui/UI_stop_btn_select.png"),
																	Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto stopPressedIconLayer = stopRootIconLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_stop_btn_pressed.png"),
																	 Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		Animation playBtnSelectAnim = Animation::EaseInOut(sample, &playSelectIconLayer->transparency, 0.0f, 1.0f, 0.1f);
		*playBtnSelectAnim.AddAnimationValue(&stopSelectIconLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("select", playBtnSelectAnim)->offStateAnimationSpeed = 0.25f;

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

	void RebuildPauseButtonStyle()
	{
		UIToggle* sample = mnew UIToggle();
		auto pauseIconRootLayer = sample->AddLayer("regular", nullptr);
		auto pauseIconLayer = pauseIconRootLayer->AddChildLayer("icon", mnew Sprite("ui/UI_pause_btn_regular.png"),
																Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
																	   Vec2F(-10, 10), Vec2F(10, -10)));

		auto pauseSelectIconLayer = pauseIconRootLayer->AddChildLayer("select", mnew Sprite("ui/UI_pause_btn_select.png"),
																	  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
																			 Vec2F(-10, 10), Vec2F(10, -10)));

		auto pausePressedIconLayer = pauseIconRootLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_pause_btn_pressed.png"),
																	   Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
																			  Vec2F(-10, 10), Vec2F(10, -10)));

		sample->AddState("select", Animation::EaseInOut(sample, &pauseSelectIconLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pausePressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &pauseIconRootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "pause");
	}

	void RebuildStepButtonStyle()
	{
		UIButton* sample = mnew UIButton();
		sample->name = "step button";
		auto stepIconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI_step_btn_regular.png"),
											  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
													 Vec2F(-10, 10), Vec2F(10, -10)));

		auto stepSelectIconLayer = sample->AddLayer("select", mnew Sprite("ui/UI_step_btn_select.png"),
													Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
														   Vec2F(-10, 10), Vec2F(10, -10)));

		auto stepPressedIconLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_step_btn_pressed.png"),
													 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f),
															Vec2F(-10, 10), Vec2F(10, -10)));

		sample->AddState("select", Animation::EaseInOut(sample, &stepSelectIconLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &stepPressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "step");
	}

	void RebuildRoundDropDown()
	{
		UIDropDown* sample = mnew UIDropDown();
		sample->layout.minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_panel_subpanel_bk.png"),
										  Layout::BothStretch(-7, -5, -5, -5));

		auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI_panel_subpanel_select.png"),
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

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("opened", Animation::EaseInOut(sample, &arrowLayer->drawable->scale, Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "round");
	}

	void RebuildArrowToggle()
	{
		UIToggle* sample = mnew UIToggle();
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("select", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_select_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_select_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "arrow");
	}

	void RebuildBrushToggle()
	{
		UIToggle* sample = mnew UIToggle();
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("select", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_brush_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_brush_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "brush");
	}

	void RebuildMoveToggle()
	{
		UIToggle* sample = mnew UIToggle();
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("select", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_move_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_move_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "move");
	}

	void RebuildRotateToggle()
	{
		UIToggle* sample = mnew UIToggle();
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("select", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_rotate_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_rotate_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "rotate");
	}

	void RebuildScaleToggle()
	{
		UIToggle* sample = mnew UIToggle();
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("select", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_scale_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_scale_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "scale");
	}

	void RebuildFrameToggle()
	{
		UIToggle* sample = mnew UIToggle();
		auto rootLayer = sample->AddLayer("root", nullptr);
		auto selectLayer = rootLayer->AddChildLayer("select", nullptr);
		auto iconLayer = selectLayer->AddChildLayer("regular", mnew Sprite("ui/UI_frame_tool.png"),
													Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		auto pressedIconLayer = selectLayer->AddChildLayer("pressed", mnew Sprite("ui/UI_frame_tool_pressed.png"),
														   Layout::Based(BaseCorner::Center, Vec2F(20, 20)));

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f))
			->offStateAnimationSpeed = 0.25f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedIconLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));

		sample->AddState("value", Animation::EaseInOut(sample, &rootLayer->transparency, 0.3f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "frame");
	}

	void RebuildSearchButton()
	{
		UIButton* sample = mnew UIButton();
		sample->layout.minSize = Vec2F(5, 5);

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI_search_regular.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI_search_select.png"),
											Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI_search_pressed.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "search");
	}

	void RebuildListTreeToggle()
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


		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		Animation valueBtnAnim = Animation::EaseInOut(sample, &treeLayer->transparency, 1.0f, 0.0f, 0.1f);
		*valueBtnAnim.AddAnimationValue(&listLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("value", valueBtnAnim);

		o2UI.AddWidgetStyle(sample, "list-tree");
	}

	void RebuildActorsTreeNodeEnableToggle()
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

	void RebuildActorsTreeLockToggle()
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

	void RebuildActorsTreeLinkBtn()
	{
		UIButton* sample = mnew UIButton();

		auto halfHideLayer = sample->AddLayer("halfHide", nullptr);
		auto selectLayer = halfHideLayer->AddChildLayer("select", nullptr);
		auto pressedLayer = selectLayer->AddChildLayer("pressed", nullptr);
		auto back = pressedLayer->AddChildLayer("back", mnew Sprite("ui/UI_link_icon.png"),
												Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 0)));


		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.1f));
		sample->AddState("halfHide", Animation::EaseInOut(sample, &halfHideLayer->transparency, 1.0f, 0.5f, 0.1f));
		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 1.0f, 0.5f, 0.1f));
		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.8f, 1.0f, 0.1f));

		o2UI.AddWidgetStyle(sample, "actorNodeLink");
	}

	void RebuildActorsTree()
	{
		UIActorsTree* sample = mnew UIActorsTree();
		sample->layout.minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(0, 0, 0, 0));
		sample->SetEnableScrollsHiding(true);

		*sample->GetHoverDrawable() = Sprite("ui/UI_ListBox_selection_hover.png");
		*sample->GetSelectionDrawable() = Sprite("ui/UI_ListBox_selection_regular.png");
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

		UITreeNode* itemSample = sample->GetNodeSample();

		itemSample->SetChildrenOffset(10);

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
		itemSample->AddChild(actorNodeEditBox);

		Animation itemEditStateAnim = Animation::EaseInOut(itemSample, &nameLayer->transparency, 1.0f, 0.0f, 0.15f);
		*itemEditStateAnim.AddAnimationValue(&actorNodeEditBox->visible) = AnimatedValue<bool>::Linear(false, true, 0.15f);
		itemSample->AddState("edit", itemEditStateAnim);

		UIButton* itemSampleExpandBtn = mnew UIButton();
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

		sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetHoverDrawable()->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void RebuildPanelDownButton()
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

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "panel down");
	}

	void RebuildTrashDownPanelButton()
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


		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "down panel trash");
	}

	void RebuildMessagesDownPanelToggle()
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

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		Animation valueBtnAnim = Animation::EaseInOut(sample, &offLayer->transparency, 1.0f, 0.0f, 0.1f);
		*valueBtnAnim.AddAnimationValue(&onLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("value", valueBtnAnim);

		o2UI.AddWidgetStyle(sample, "log messages");
	}

	void RebuildWarningsDownPanelToggle()
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

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		Animation valueBtnAnim = Animation::EaseInOut(sample, &offLayer->transparency, 1.0f, 0.0f, 0.1f);
		*valueBtnAnim.AddAnimationValue(&onLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("value", valueBtnAnim);

		o2UI.AddWidgetStyle(sample, "log warnings");
	}

	void RebuildErrorsDownPanelToggle()
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

		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		Animation valueBtnAnim = Animation::EaseInOut(sample, &offLayer->transparency, 1.0f, 0.0f, 0.1f);
		*valueBtnAnim.AddAnimationValue(&onLayer->transparency) =
			AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("value", valueBtnAnim);

		o2UI.AddWidgetStyle(sample, "log errors");
	}

	void RebuildFilterMenuButton()
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


		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "menu filter");
	}

	void RebuildTreeMenuButton()
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


		sample->AddState("select", Animation::EaseInOut(sample, &selectLayer->transparency, 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", Animation::EaseInOut(sample, &pressedLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", Animation::EaseInOut(sample, &focusLayer->transparency, 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "menu tree");
	}

	void RebuildFoldersTree()
	{
		UITree* sample = mnew UITree();
		sample->layout.minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
		sample->SetEnableScrollsHiding(true);

		*sample->GetHoverDrawable() = Sprite("ui/UI_ListBox_selection_hover.png");
		*sample->GetSelectionDrawable() = Sprite("ui/UI_ListBox_selection_regular.png");
		sample->SetSelectionDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

		UITreeNode* itemSample = sample->GetNodeSample();

		itemSample->SetChildrenOffset(10);

		Text* captionLayerText = mnew Text("stdFont.ttf");
		captionLayerText->horAlign = HorAlign::Left;
		captionLayerText->verAlign = VerAlign::Middle;
		captionLayerText->dotsEngings = true;
		itemSample->AddLayer("name", captionLayerText, Layout::BothStretch(30, 0, 0, 0));

		itemSample->AddLayer("folderIcon", mnew Sprite("ui/UI2_folder_icon.png"),
							 Layout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(10, 0)));

		auto actorNodeEditBox = o2UI.CreateWidget<UIEditBox>("backless");
		actorNodeEditBox->name = "nameEditBox";
		actorNodeEditBox->layout = UIWidgetLayout::BothStretch(30, 0, 0, 0);
		itemSample->AddChild(actorNodeEditBox);

		Animation itemEditStateAnim = Animation::EaseInOut(itemSample, &itemSample->layer["name"]->transparency, 1.0f, 0.0f, 0.15f);
		*itemEditStateAnim.AddAnimationValue(&actorNodeEditBox->visible) = AnimatedValue<bool>::Linear(false, true, 0.15f);
		itemSample->AddState("edit", itemEditStateAnim);

		UIButton* itemSampleExpandBtn = mnew UIButton();
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

		sample->AddState("hover", Animation::EaseInOut(sample, &sample->GetHoverDrawable()->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5;

		o2UI.AddWidgetStyle(sample, "folders");
	}

	void RebuildRegularAssetIcon()
	{
		UIAssetIcon* sample = mnew UIAssetIcon();

		sample->AddLayer("icon", mnew Sprite("ui/UI2_big_file_icon.png"),
						 Layout::Based(BaseCorner::Center, Vec2F(40, 40), Vec2F(0, 10)));

		Text* captionText = mnew Text("stdFont.ttf");
		captionText->text = "file name";
		captionText->horAlign = HorAlign::Middle;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		captionText->height = 7;
		auto nameLayer = sample->AddLayer("assetName", captionText, Layout::HorStretch(VerAlign::Middle, -4, -4, 15, -10));

		UIEditBox* nameEditBox = mnew UIEditBox();
		nameEditBox->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));
		nameEditBox->SetViewLayout(Layout::BothStretch(0, 0, 2, 0));
		nameEditBox->SetCaretBlinkingDelay(0.85f);
		nameEditBox->SetMultiLine(false);
		nameEditBox->layout.minSize = Vec2F(30, 40);

		Text* textDrawable = nameEditBox->GetTextDrawable();
		textDrawable->verAlign = VerAlign::Middle;
		textDrawable->horAlign = HorAlign::Middle;
		textDrawable->SetFontAsset("stdFont.ttf");
		textDrawable->height = 7;

		Sprite* caretDrawable = nameEditBox->GetCaretDrawable();
		*caretDrawable = Sprite();
		caretDrawable->size = Vec2F(1, textDrawable->GetFont()->GetHeightPx(textDrawable->GetHeight())*1.7f);
		caretDrawable->pivot = Vec2F(0, 0.26f);
		caretDrawable->color = Color4::Black();

		nameEditBox->name = "nameEditBox";
		nameEditBox->layout = UIWidgetLayout::HorStretch(VerAlign::Middle, -4, -4, 15, -10);
		sample->AddChild(nameEditBox);

		Animation itemEditStateAnim = Animation::EaseInOut(sample, &nameLayer->transparency, 1.0f, 0.0f, 0.15f);
		*itemEditStateAnim.AddAnimationValue(&nameEditBox->visible) = AnimatedValue<bool>::Linear(false, true, 0.15f);
		sample->AddState("edit", itemEditStateAnim);

		sample->AddState("halfHide", Animation::EaseInOut(sample, &sample->transparency, 1.0f, 0.5f, 0.1f));

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void RebuildFolderAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		auto iconLayer = sample->layer["icon"];
		((Sprite*)iconLayer->drawable)->LoadFromImage("ui/UI2_big_folder_icon.png");

		o2UI.AddWidgetStyle(sample, "folder");
	}

	void RebuildPrefabAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		auto iconLayer = sample->layer["icon"];
		((Sprite*)iconLayer->drawable)->LoadFromImage("ui/UI2_actor_icon.png");

		o2UI.AddWidgetStyle(sample, "prefab");
	}

	void RebuildPrefabPreviewAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		auto iconLayer = sample->layer["icon"];
		((Sprite*)iconLayer->drawable)->LoadFromImage("ui/UI2_image_asset_back.png");

		sample->AddLayer("preview", mnew Sprite(),
						 Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 20)));

		o2UI.AddWidgetStyle(sample, "prefab preview");
	}

	void RebuildImagePreviewAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		sample->RemoveLayer("icon");
		sample->AddLayer("preview", mnew Sprite(),
						 Layout::Based(BaseCorner::Center, Vec2F(20, 20), Vec2F(0, 15)));

		o2UI.AddWidgetStyle(sample, "image preview");
	}

	void RebuildTextAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		auto iconLayer = sample->layer["icon"];
		((Sprite*)iconLayer->drawable)->LoadFromImage("ui/UI2_big_text_file_icon.png");

		o2UI.AddWidgetStyle(sample, "text");
	}

	void RebuildAnimationAssetIcon()
	{
		UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>();

		auto iconLayer = sample->layer["icon"];
		((Sprite*)iconLayer->drawable)->LoadFromImage("ui/UI2_anim_file_icon.png");

		o2UI.AddWidgetStyle(sample, "animation");
	}

	void RebuildAssetsGridScroll()
	{
		UIAssetsIconsScrollArea* sample = mnew UIAssetsIconsScrollArea();
		sample->layout.minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
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
		verScrollBar->layout.offsetMax = Vec2F(0, -5);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("enableHorBar", Animation::EaseInOut(sample, &sample->GetVerticalScrollbar()->layout.offsetBottom,
															  5.0f, 15.0f, 0.2f));

		sample->AddState("enableVerBar", Animation::EaseInOut(sample, &sample->GetHorizontalScrollbar()->layout.offsetRight,
															  -5.0f, -15.0f, 0.2f));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void RebuildEditorUIStyle()
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
		RebuildDockableWndStyle();
		RebuildMenuPanelStyle();
		RebuildPlayStopButtonStyle();
		RebuildPauseButtonStyle();
		RebuildStepButtonStyle();
		RebuildBacklessDropdown();
		RebuildRoundDropDown();
		RebuildArrowToggle();
		RebuildBrushToggle();
		RebuildMoveToggle();
		RebuildRotateToggle();
		RebuildScaleToggle();
		RebuildFrameToggle();
		RebuildSearchButton();
		RebuildListTreeToggle();
		RebuildBacklessEditbox();
		RebuildActorsTreeNodeEnableToggle();
		RebuildActorsTreeLockToggle();
		RebuildActorsTreeLinkBtn();
		RebuildActorsTree();
		RebuildPanelDownButton();
		RebuildTrashDownPanelButton();
		RebuildMessagesDownPanelToggle();
		RebuildWarningsDownPanelToggle();
		RebuildErrorsDownPanelToggle();
		RebuildLongListStyle();
		RebuildFilterMenuButton();
		RebuildTreeMenuButton();
		RebuildFoldersTree();
		RebuildBacklessScrollarea();
		RebuildRegularAssetIcon();
		RebuildFolderAssetIcon();
		RebuildPrefabAssetIcon();
		RebuildPrefabPreviewAssetIcon();
		RebuildImagePreviewAssetIcon();
		RebuildTextAssetIcon();
		RebuildAnimationAssetIcon();
		RebuildAssetsGridScroll();

		o2UI.SaveStyle("ui_style.xml");
	}
}