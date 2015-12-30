#include "EditorUIStyle.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Core/UIStyle/BasicUIStyle.h"
#include "Core/WindowsSystem/UIDockableWindow.h"
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
		Ptr<UIDockableWindow> sample = mnew UIDockableWindow();

		sample->layout.minSize = Vec2F(100, 50);

		*sample->GetDockingFrameSample() = Sprite("ui/UI_Window_place.png");

		auto regularBackLayer = sample->AddLayer("regularBack", mnew Sprite("ui/UI_window_frame_regular.png"),
												 Layout::BothStretch(-12, -12, -12, -12));

		auto dockedBackLayer = sample->AddLayer("dockedBack", mnew Sprite("ui/UI_window_frame_docked.png"),
												Layout::BothStretch(-12, -12, -12, -12));

		auto iconLayer = sample->AddLayer("icon", mnew Sprite("ui/UI_o2_sign.png"),
										  Layout(Vec2F(0.0f, 1.0f), Vec2F(0.0f, 1.0f), Vec2F(3, -16), Vec2F(17, -1)));

		Ptr<Text> captionText = mnew Text("arial.ttf");
		captionText->text = "Window";
		captionText->horAlign = HorAlign::Left;
		captionText->verAlign = VerAlign::Middle;
		captionText->dotsEngings = true;
		auto textLayer = sample->AddLayer("caption", captionText,
										  Layout(Vec2F(0.0f, 1.0f), Vec2F(1.0f, 1.0f), Vec2F(19, -15), Vec2F(-28, 1)));

		sample->SetClippingLayout(Layout::BothStretch(0, 1, -1, 15));
		sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 20));
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
			->offStateAnimationSpeed = 0.5f;

		Ptr<UIButton> closeBtn = o2UI.CreateWidget<UIButton>("close");
		closeBtn->name = "closeButton";
		closeBtn->layout.anchorMin = Vec2F(1, 1);
		closeBtn->layout.anchorMax = Vec2F(1, 1);
		closeBtn->layout.offsetMin = Vec2F(-21, -17);
		closeBtn->layout.offsetMax = Vec2F(-1, 1);
		sample->AddWindowElement(closeBtn);

		Ptr<UIButton> optionsBtn = o2UI.CreateWidget<UIButton>("arrow");
		optionsBtn->name = "optionsButton";
		optionsBtn->layout.anchorMin = Vec2F(1, 1);
		optionsBtn->layout.anchorMax = Vec2F(1, 1);
		optionsBtn->layout.offsetMin = Vec2F(-36, -17);
		optionsBtn->layout.offsetMax = Vec2F(-18, 1);
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
		Ptr<UIToggle> sample = mnew UIToggle();
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
		Ptr<UIToggle> sample = mnew UIToggle();
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
		Ptr<UIButton> sample = mnew UIButton();
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

	void RebuildBacklessDropdown()
	{
		Ptr<UIDropDown> sample = mnew UIDropDown();
		sample->layout.minSize = Vec2F(20, 20);
		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI_Down_icn.png"),
										   Layout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, -1)));

		sample->SetClippingLayout(Layout::BothStretch(4, 2, 20, 2));

		auto list = sample->GetListView();
		*list = *o2UI.GetWidgetStyle<UICustomList>("standard");
		list->layer["back"]->drawable.Release();
		list->layer["back"]->drawable = mnew Sprite("ui/UI_Box_regular.png");
		list->layout.pivot = Vec2F(0.5f, 1.0f);
		list->layout.anchorMin = Vec2F(0, 0);
		list->layout.anchorMax = Vec2F(1, 0);
		list->layout.offsetMin = Vec2F(0, -60);
		list->layout.offsetMax = Vec2F(0, 0);

		Ptr<UILabel> itemSample = o2UI.CreateLabel("empty");
		itemSample->horAlign = HorAlign::Left;
		sample->SetItemSample(itemSample);

		sample->AddState("opened", Animation::EaseInOut(sample, &arrowLayer->drawable->scale, Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("visible", Animation::EaseInOut(sample, &sample->transparency, 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "backless");
	}

	void RebuildRoundDropDown()
	{
		Ptr<UIDropDown> sample = mnew UIDropDown();
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
		list->layer["back"]->drawable.Release();
		list->layer["back"]->drawable = mnew Sprite("ui/UI_Context_menu.png");
		list->layer["back"]->layout = Layout::BothStretch(-19, -19, -20, -20);
		list->layout.pivot = Vec2F(0.5f, 1.0f);
		list->layout.anchorMin = Vec2F(0, 0);
		list->layout.anchorMax = Vec2F(1, 0);
		list->layout.offsetMin = Vec2F(5, -60);
		list->layout.offsetMax = Vec2F(-5, 0);

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

		o2UI.AddWidgetStyle(sample, "round");
	}

	void RebuildArrowToggle()
	{
		Ptr<UIToggle> sample = mnew UIToggle();
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
		Ptr<UIToggle> sample = mnew UIToggle();
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
		Ptr<UIToggle> sample = mnew UIToggle();
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
		Ptr<UIToggle> sample = mnew UIToggle();
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
		Ptr<UIToggle> sample = mnew UIToggle();
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
		Ptr<UIToggle> sample = mnew UIToggle();
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

		o2UI.SaveStyle("ui_style.xml");
	}
}
