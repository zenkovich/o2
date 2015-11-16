#include "UITestScreen.h"

#include "Render/Render.h"
#include "TestApplication.h"

UITestScreen::UITestScreen(Ptr<TestApplication> application):
	ITestScreen(application)
{
}

UITestScreen::~UITestScreen()
{
}

void UITestScreen::Load()
{
	CheckStyle();

	mBackground.LoadFromImage("ui/UI_Background.png");
	mBackground.size = (Vec2I)o2Render.resolution;

	mFakeWindow.LoadFromImage("ui/UI_window_frame_regular.png");
	mFakeWindow.size = Vec2F(550, 550);

	// 	auto list = o2UI.CreateCustomList();
	// 	auto item = o2UI.CreateLabel("empty");
	// 	item->layout.size = Vec2F(20, 20);
	// 	list->SetItemSample(item);
	// 	for (int i = 0; i < 10; i++)
	// 		list->AddItem().Cast<UILabel>()->text = String::Format("Item #%i", i + 1);
	// 	o2UI.AddWidget(list);
	// 	list->layout.size = Vec2F(150, 150);

	auto dropdown = o2UI.CreateDropdown();
	dropdown->SetMaxListSizeInItems(5);
	for (int i = 0; i < 10; i++)
		dropdown->AddItem(String::Format("Item #%i", i + 1));
	o2UI.AddWidget(dropdown);
	dropdown->layout.size = Vec2F(150, 22);


	//  	auto list = o2UI.CreateWidget<UICustomList>();
	//  	auto item = o2UI.CreateLabel("empty");
	//  	item->layout.size = Vec2F(20, 20);
	//  	list->SetItemSample(item);
	//  	for (int i = 0; i < 10; i++)
	//  		list->AddItem().Cast<UILabel>()->text = String::Format("Item #%i", i + 1);
	// 	o2UI.AddWidget(list);
	// 	list->layout.size = Vec2F(150, 60);

	//  	auto area = o2UI.AddScrollArea();
	//  	area->layout.size = Vec2F(300, 300);
	//  
	//  	auto layout = o2UI.CreateVerLayout();
	//  	layout->spacing = 5;
	//  	layout->expandHeight = false;
	//  	layout->expandWidth = false;
	//  	layout->fitByChildren = true;
	//  	area->AddChild(layout);
	//  
	//  	for (int i = 0; i < 20; i++)
	//  	{
	//  		auto btn = o2UI.CreateButton("Button!");
	//  		btn->layout.size = Vec2F(500, 50);
	//  		layout->AddChild(btn);
	//  	}
}

void UITestScreen::CheckStyle()
{
	o2UI.ClearStyle();

	auto btn = CheckButtonStyle();
	auto hprogress = CheckHorProgressBarStyle();
	auto vprogress = CheckVerProgressBarStyle();
	auto hscroll = CheckHorScrollBarStyle();
	auto vscroll = CheckVerScrollBarStyle();
	auto chkbox = CheckCheckboxStyle();
	auto scrollArea = CheckScrollArea();
	auto scrollArea2 = CheckScrollAreaStraightBars();
	auto label = CheckLabel();
	auto editbox = CheckEditBox();
	auto listview = CheckCustomList();
	auto textlist = CheckList();
	auto cdropdown = CheckCustomDropDown();
	auto dropdown = CheckDropDown();

	if (!btn || !hprogress || !vprogress || !hscroll || !vscroll || !chkbox || !scrollArea || !label || !editbox ||
		!listview || !textlist || !cdropdown || !scrollArea2 || !dropdown)
		o2UI.SaveStyle("ui_style.xml");
}

bool UITestScreen::CheckButtonStyle()
{
	if (o2UI.GetWidgetStyle<UIButton>("standard"))
		return true;

	Ptr<UIButton> buttonStyleSample = mnew UIButton();
	buttonStyleSample->AddLayer("regular", mnew Sprite("ui/UI_button_regular.png"),
								Layout::Both(-10, -10, -11, -11));

	auto selectLayer = buttonStyleSample->AddLayer("select", mnew Sprite("ui/UI_button_select.png"),
												   Layout::Both(-10, -10, -11, -11));

	auto pressedLayer = buttonStyleSample->AddLayer("pressed", mnew Sprite("ui/UI_button_pressed.png"),
													Layout::Both(-10, -10, -11, -11));

	Ptr<Text> captionText = mnew Text("arial.ttf");
	captionText->text = "Button";
	captionText->horAlign = Text::HorAlign::Middle;
	captionText->verAlign = Text::VerAlign::Middle;
	buttonStyleSample->AddLayer("caption", captionText);

	Animation selectStateAnim;
	selectStateAnim.SetTarget(buttonStyleSample);
	*selectStateAnim.AddAnimationValue<float>(&selectLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.1f);

	auto selectState = buttonStyleSample->AddState("select", selectStateAnim);
	selectState->offStateAnimationSpeed = 1.0f/4.0f;

	Animation pressedStateAnim;
	pressedStateAnim.SetTarget(buttonStyleSample);
	*pressedStateAnim.AddAnimationValue<float>(&pressedLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.05f);

	auto pressedState = buttonStyleSample->AddState("pressed", pressedStateAnim);
	pressedState->offStateAnimationSpeed = 0.5f;

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(buttonStyleSample);
	*visibleStateAnim.AddAnimationValue<float>(&buttonStyleSample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = buttonStyleSample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(buttonStyleSample, "standard");

	return false;
}

bool UITestScreen::CheckHorProgressBarStyle()
{
	if (o2UI.GetWidgetStyle<UIHorizontalProgress>("standard"))
		return true;

	Ptr<UIHorizontalProgress> sample = mnew UIHorizontalProgress();
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

	Animation selectStateAnim;
	selectStateAnim.SetTarget(sample);
	*selectStateAnim.AddAnimationValue(&barSelectSprite->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

	Animation pressedStateAnim;
	pressedStateAnim.SetTarget(sample);
	*pressedStateAnim.AddAnimationValue(&barPressedSprite->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.05f);

	auto selectState = sample->AddState("select", selectStateAnim);
	selectState->offStateAnimationSpeed = 1.0f/4.0f;

	auto pressedState = sample->AddState("pressed", pressedStateAnim);
	pressedState->offStateAnimationSpeed = 1.0f/2.0f;

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	sample->SetOrientation(UIHorizontalProgress::Orientation::Right);

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}


bool UITestScreen::CheckVerProgressBarStyle()
{
	if (o2UI.GetWidgetStyle<UIVerticalProgress>("standard"))
		return true;

	Ptr<UIVerticalProgress> sample = mnew UIVerticalProgress();
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

	Animation selectStateAnim;
	selectStateAnim.SetTarget(sample);
	*selectStateAnim.AddAnimationValue(&barSelectSprite->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

	Animation pressedStateAnim;
	pressedStateAnim.SetTarget(sample);
	*pressedStateAnim.AddAnimationValue(&barPressedSprite->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.05f);

	auto selectState = sample->AddState("select", selectStateAnim);
	selectState->offStateAnimationSpeed = 1.0f/4.0f;

	auto pressedState = sample->AddState("pressed", pressedStateAnim);
	pressedState->offStateAnimationSpeed = 1.0f/2.0f;

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

bool UITestScreen::CheckHorScrollBarStyle()
{
	if (o2UI.GetWidgetStyle<UIHorizontalScrollBar>("standard"))
		return true;

	Ptr<UIHorizontalScrollBar> sample = mnew UIHorizontalScrollBar();
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

	Animation selectStateAnim;
	selectStateAnim.SetTarget(sample);
	*selectStateAnim.AddAnimationValue(&barSelectSprite->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

	Animation pressedStateAnim;
	pressedStateAnim.SetTarget(sample);
	*pressedStateAnim.AddAnimationValue(&barPressedSprite->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.05f);

	auto selectState = sample->AddState("select", selectStateAnim);
	selectState->offStateAnimationSpeed = 1.0f/4.0f;

	auto pressedState = sample->AddState("pressed", pressedStateAnim);
	pressedState->offStateAnimationSpeed = 1.0f/2.0f;

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

bool UITestScreen::CheckVerScrollBarStyle()
{
	if (o2UI.GetWidgetStyle<UIVerticalScrollBar>("standard"))
		return true;

	Ptr<UIVerticalScrollBar> sample = mnew UIVerticalScrollBar();
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

	Animation selectStateAnim;
	selectStateAnim.SetTarget(sample);
	*selectStateAnim.AddAnimationValue(&barSelectSprite->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.1f);

	Animation pressedStateAnim;
	pressedStateAnim.SetTarget(sample);
	*pressedStateAnim.AddAnimationValue(&barPressedSprite->transparency) = AnimatedValue<float>::EaseInOut(0.0f, 1.0f, 0.05f);

	auto selectState = sample->AddState("select", selectStateAnim);
	selectState->offStateAnimationSpeed = 1.0f/4.0f;

	auto pressedState = sample->AddState("pressed", pressedStateAnim);
	pressedState->offStateAnimationSpeed = 1.0f/2.0f;

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

bool UITestScreen::CheckCheckboxStyle()
{
	if (o2UI.GetWidgetStyle<UIToggle>("standard"))
		return true;

	Ptr<UIToggle> sample = mnew UIToggle();
	auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Check_bk.png"),
									  Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

	auto selectLayer = sample->AddLayer("backSelect", mnew Sprite("ui/UI_Check_bk_select.png"),
										Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

	auto pressedLayer = sample->AddLayer("backPressed", mnew Sprite("ui/UI_Check_bk_pressed.png"),
										 Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

	auto checkLayer = sample->AddLayer("check", mnew Sprite("ui/UI_Ckeck.png"),
									   Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(1, -11), Vec2F(21, 10)));

	Ptr<Text> captionText = mnew Text("arial.ttf");
	captionText->text = "Checkbox";
	captionText->horAlign = Text::HorAlign::Left;
	captionText->verAlign = Text::VerAlign::Middle;
	sample->AddLayer("caption", captionText, Layout(Vec2F(0, 0), Vec2F(1, 1), Vec2F(20, 0), Vec2F(0, 0)));

	Animation selectStateAnim;
	selectStateAnim.SetTarget(sample);
	*selectStateAnim.AddAnimationValue<float>(&selectLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.1f);

	auto selectState = sample->AddState("select", selectStateAnim);
	selectState->offStateAnimationSpeed = 1.0f/4.0f;

	Animation pressedStateAnim;
	pressedStateAnim.SetTarget(sample);
	*pressedStateAnim.AddAnimationValue<float>(&pressedLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.05f);

	auto pressedState = sample->AddState("pressed", pressedStateAnim);
	pressedState->offStateAnimationSpeed = 0.5f;

	Animation valueStateAnim;
	valueStateAnim.SetTarget(sample);
	*valueStateAnim.AddAnimationValue<float>(&checkLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.1f);
	sample->AddState("value", valueStateAnim);

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

bool UITestScreen::CheckScrollAreaStraightBars()
{
	auto style = o2UI.GetWidgetStyle<UIScrollArea>("straightBars");
	if (style && style->GetName() == (String)"straightBars")
		return true;

	Ptr<UIScrollArea> sample = mnew UIScrollArea();
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

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "straightBars");

	return false;
}

bool UITestScreen::CheckScrollArea()
{
	if (o2UI.GetWidgetStyle<UIScrollArea>("standard"))
		return true;

	Ptr<UIScrollArea> sample = mnew UIScrollArea();
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

	Animation enableHorScrollAnim;
	enableHorScrollAnim.SetTarget(sample);
	*enableHorScrollAnim.AddAnimationValue<float>(&sample->GetVerticalScrollbar()->layout.offsetBottom) =
		AnimatedValue<float>::EaseInOut(5, 15, 0.2f);

	auto enableHorScrollState = sample->AddState("enableHorBar", enableHorScrollAnim);

	Animation enableVerScrollAnim;
	enableVerScrollAnim.SetTarget(sample);
	*enableVerScrollAnim.AddAnimationValue<float>(&sample->GetHorizontalScrollbar()->layout.offsetRight) =
		AnimatedValue<float>::EaseInOut(-5, -15, 0.2f);

	auto enableVerScrollState = sample->AddState("enableVerBar", enableVerScrollAnim);

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

bool UITestScreen::CheckLabel()
{
	if (o2UI.GetWidgetStyle<UILabel>("standard"))
		return true;

	Ptr<UILabel> sample = mnew UILabel();
	Ptr<Text> captionText = mnew Text("arial.ttf");
	captionText->text = "Checkbox";
	captionText->horAlign = Text::HorAlign::Middle;
	captionText->verAlign = Text::VerAlign::Middle;
	sample->AddLayer("text", captionText);

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

bool UITestScreen::CheckEditBox()
{
	if (o2UI.GetWidgetStyle<UIEditBox>("standard"))
		return true;

	Ptr<UIEditBox> sample = mnew UIEditBox();

	sample->SetTextLayout(Layout::Both(5, 15, 15, 5));
	sample->SetClipArea(Layout::Both(1, 2, 1, 1));
	sample->SetCaretBlinkingDelay(0.85f);

	auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI_Editbox_regular.png"), Layout::Both(-9, -9, -9, -9));
	auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI_Editbox_select.png"), Layout::Both(-9, -9, -9, -9));

	Animation selectStateAnim;
	selectStateAnim.SetTarget(sample);
	*selectStateAnim.AddAnimationValue<float>(&selectLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.1f);

	auto selectState = sample->AddState("select", selectStateAnim);
	selectState->offStateAnimationSpeed = 1.0f/4.0f;

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	Ptr<Text> textDrawable = sample->GetTextDrawable();
	textDrawable->verAlign = Text::VerAlign::Top;
	textDrawable->horAlign = Text::HorAlign::Left;
	textDrawable->font = FontRef("arial.ttf");

	Ptr<Sprite> caretDrawable = sample->GetCaretDrawable();
	*caretDrawable = Sprite();
	caretDrawable->size = Vec2F(1, textDrawable->GetFont()->GetHeight()*1.7f);
	caretDrawable->pivot = Vec2F(0, 0.16f);
	caretDrawable->color = Color4::Black();

	Ptr<UIHorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
	horScrollBar->layout.anchorMin = Vec2F(0, 0);
	horScrollBar->layout.anchorMax = Vec2F(1, 0);
	horScrollBar->layout.offsetMin = Vec2F(5, 0);
	horScrollBar->layout.offsetMax = Vec2F(-15, 15);
	sample->SetHorizontalScrollBar(horScrollBar);

	Ptr<UIVerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
	verScrollBar->scrollSense = 5.0f;
	verScrollBar->layout.anchorMin = Vec2F(1, 0);
	verScrollBar->layout.anchorMax = Vec2F(1, 1);
	verScrollBar->layout.offsetMin = Vec2F(-15, 15);
	verScrollBar->layout.offsetMax = Vec2F(0, -5);
	sample->SetVerticalScrollBar(verScrollBar);

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

bool UITestScreen::CheckCustomList()
{
	if (o2UI.GetWidgetStyle<UICustomList>("standard"))
		return true;

	Ptr<UICustomList> sample = mnew UICustomList();
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

	Animation enableHorScrollAnim;
	enableHorScrollAnim.SetTarget(sample);
	*enableHorScrollAnim.AddAnimationValue<float>(&sample->GetVerticalScrollbar()->layout.offsetBottom) =
		AnimatedValue<float>::EaseInOut(5, 15, 0.2f);

	auto enableHorScrollState = sample->AddState("enableHorBar", enableHorScrollAnim);

	Animation enableVerScrollAnim;
	enableVerScrollAnim.SetTarget(sample);
	*enableVerScrollAnim.AddAnimationValue<float>(&sample->GetHorizontalScrollbar()->layout.offsetRight) =
		AnimatedValue<float>::EaseInOut(-5, -15, 0.2f);

	auto enableVerScrollState = sample->AddState("enableVerBar", enableVerScrollAnim);

	Animation hoverStateAnim;
	hoverStateAnim.SetTarget(sample);
	*hoverStateAnim.AddAnimationValue<float>(&sample->GetHoverDrawable()->transparency) =
		AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto hoverState = sample->AddState("hover", hoverStateAnim);
	hoverState->offStateAnimationSpeed = 0.5;

	Animation selectedStateAnim;
	selectedStateAnim.SetTarget(sample);
	*selectedStateAnim.AddAnimationValue<float>(&sample->GetSelectionDrawable()->transparency) =
		AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto selectedState = sample->AddState("selected", selectedStateAnim);
	selectedState->offStateAnimationSpeed = 0.5;

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

bool UITestScreen::CheckList()
{
	if (o2UI.GetWidgetStyle<UIList>("standard"))
		return true;

	Ptr<UIList> sample = mnew UIList();
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
	itemSample->layout.size = Vec2F(20, 20);
	itemSample->horAlign = Text::HorAlign::Left;
	itemSample->verAlign = Text::VerAlign::Middle;
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

	Animation hoverStateAnim;
	hoverStateAnim.SetTarget(sample);
	*hoverStateAnim.AddAnimationValue<float>(&sample->GetHoverDrawable()->transparency) =
		AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto hoverState = sample->AddState("hover", hoverStateAnim);
	hoverState->offStateAnimationSpeed = 0.5;

	Animation selectedStateAnim;
	selectedStateAnim.SetTarget(sample);
	*selectedStateAnim.AddAnimationValue<float>(&sample->GetSelectionDrawable()->transparency) =
		AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto selectedState = sample->AddState("selected", selectedStateAnim);
	selectedState->offStateAnimationSpeed = 0.5;

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

bool UITestScreen::CheckCustomDropDown()
{
	if (o2UI.GetWidgetStyle<UICustomDropDown>("standard"))
		return true;

	Ptr<UICustomDropDown> sample = mnew UICustomDropDown();
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

	Animation selectedStateAnim;
	selectedStateAnim.SetTarget(sample);
	*selectedStateAnim.AddAnimationValue<float>(&selectLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto selectedState = sample->AddState("select", selectedStateAnim);
	selectedState->offStateAnimationSpeed = 0.5;

	Animation pressedStateAnim;
	pressedStateAnim.SetTarget(sample);
	*pressedStateAnim.AddAnimationValue<float>(&pressedLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.05f);

	auto pressedState = sample->AddState("pressed", pressedStateAnim);
	pressedState->offStateAnimationSpeed = 0.5f;

	Animation openAnimStateAnim;
	openAnimStateAnim.SetTarget(sample);
	*openAnimStateAnim.AddAnimationValue<Vec2F>(&arrowLayer->drawable->scale) = 
		AnimatedValue<Vec2F>::EaseInOut(Vec2F(1, 1), Vec2F(1, -1), 0.2f);

	auto openedState = sample->AddState("opened", openAnimStateAnim);
	//openedState->offStateAnimationSpeed = 2.0f;

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

bool UITestScreen::CheckDropDown()
{
	if (o2UI.GetWidgetStyle<UIDropDown>("standard"))
		return true;

	Ptr<UIDropDown> sample = mnew UIDropDown();
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
	itemSample->layout.size = Vec2F(20, 20);
	itemSample->horAlign = Text::HorAlign::Left;
	sample->SetItemSample(itemSample);

	Animation selectedStateAnim;
	selectedStateAnim.SetTarget(sample);
	*selectedStateAnim.AddAnimationValue<float>(&selectLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto selectedState = sample->AddState("select", selectedStateAnim);
	selectedState->offStateAnimationSpeed = 0.5;

	Animation pressedStateAnim;
	pressedStateAnim.SetTarget(sample);
	*pressedStateAnim.AddAnimationValue<float>(&pressedLayer->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.05f);

	auto pressedState = sample->AddState("pressed", pressedStateAnim);
	pressedState->offStateAnimationSpeed = 0.5f;

	Animation openAnimStateAnim;
	openAnimStateAnim.SetTarget(sample);
	*openAnimStateAnim.AddAnimationValue<Vec2F>(&arrowLayer->drawable->scale) =
		AnimatedValue<Vec2F>::EaseInOut(Vec2F(1, 1), Vec2F(1, -1), 0.2f);

	auto openedState = sample->AddState("opened", openAnimStateAnim);
	//openedState->offStateAnimationSpeed = 2.0f;

	Animation visibleStateAnim;
	visibleStateAnim.SetTarget(sample);
	*visibleStateAnim.AddAnimationValue<float>(&sample->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.2f);

	auto visibleState = sample->AddState("visible", visibleStateAnim);
	visibleState->offStateAnimationSpeed = 0.5;

	o2UI.AddWidgetStyle(sample, "standard");

	return false;
}

void UITestScreen::Unload()
{
	o2UI.RemoveAllWidgets();
}

void UITestScreen::Update(float dt)
{
	if (o2Input.IsKeyPressed('D'))
		o2Debug.Log("debug");

	if (o2Input.IsKeyPressed(VK_ESCAPE))
		mApplication->GoToScreen("MainTestScreen");
}

void UITestScreen::Draw()
{
	mBackground.Draw();
	mFakeWindow.Draw();
}

String UITestScreen::GetId() const
{
	return "UITestScreen";
}
