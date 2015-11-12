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

	auto editBox = o2UI.AddEditBox();
	editBox->layout.size = Vec2F(200, 100);
	//editBox->text = "Hello edit box!\nI'm text\nMultiline!\n\n\n\nMultilineddddddddddddddddddddddddddddddddddddddddddddddd!\nMultiline!\nMultiline!\nMultiline!\nMultiline!";
	editBox->text = "ABCDEFGHKLMNOPQRS";

	// 	mBar = o2UI.CreateWidget<UIHorizontalProgress>();
	// 	mBar->layout.size = Vec2F(200, 20);
	// 	mBar->SetOrientation(UIHorizontalProgress::Orientation::Left);
	// 	mBar->SetValueRange(50, 150);
	// 	o2UI.AddWidget(mBar);
	// 
	// 	mScroll = o2UI.CreateWidget<UIHorizontalScrollBar>();
	// 	mScroll->layout.size = Vec2F(200, 20);
	// 	mScroll->layout.position = Vec2F(0, 40);
	// 	o2UI.AddWidget(mScroll);
	// 
	// 	auto verBar = o2UI.CreateWidget<UIVerticalProgress>();
	// 	verBar->layout.size = Vec2F(20, 200);
	// 	verBar->layout.position = Vec2F(150, 0);
	// 	verBar->SetOrientation(UIVerticalProgress::Orientation::Down);
	// 	o2UI.AddWidget(verBar);
	// 
	// 	auto verScroll = o2UI.CreateWidget<UIVerticalScrollBar>();
	// 	verScroll->layout.size = Vec2F(20, 200);
	// 	verScroll->layout.position = Vec2F(170, 0);
	// 	o2UI.AddWidget(verScroll);
	// 
	// 	auto btn = o2UI.AddButton("Button");
	// 	btn->layout.size = Vec2F(100, 30);
	// 	btn->layout.position = Vec2F(0, 100);
	// 	//o2UI.AddWidget(btn);
	// 
	// 	auto chkbox = o2UI.CreateWidget<UIToggle>();
	// 	chkbox->layout.size = Vec2F(50, 20);
	// 	chkbox->layout.position = Vec2F(0, -100);
	// 	chkbox->caption = "Bababa";
	// 	o2UI.AddWidget(chkbox);

// 	auto scrollArea = o2UI.CreateWidget<UIScrollArea>();
// 	scrollArea->layout.size = Vec2F(300, 300);
// 	int buttons = 50;
// 	Vec2F buttonSize(50, 500);
// 	float buttonsSpacing = 5;
// 	auto layout = o2UI.CreateHorLayout();
// 	layout->layout.anchorMin = Vec2F(0, 0);
// 	layout->layout.anchorMax = Vec2F(1, 1);
// 	layout->spacing = 5.0f;
// 	layout->expandHeight = true;
// 	layout->expandWidth = false;
// 	layout->baseCorner = BaseCorner::Bottom;
// 	layout->fitByChildren = true;
// 	scrollArea->AddChild(layout);
// 	for (int i = 0; i < buttons; i++)
// 	{
// 		auto label = o2UI.CreateLabel(String::Format("Label %i", i));
// 		label->layout.size = buttonSize;
// 		layout->AddChild(label);
// 
// 		auto btn = o2UI.CreateButton(String::Format("Button %i", i));
// 		btn->layout.size = buttonSize;
// 		layout->AddChild(btn);
// 	}
// // 	auto ch = o2UI.CreateLabel("Label");
// // 	ch->layout.size = Vec2F(500, 500);
// // 	ch->layout.position = Vec2F(50, 20);
// // 	scrollArea->AddChild(ch);
// 	o2UI.AddWidget(scrollArea);

	// 	mHorLayout = mnew UIHorizontalLayout();
	// 	mHorLayout->layout.size = Vec2F(500, 500);
	// 
	// 	auto h1 = mnew UIHorizontalLayout();
	// 	h1->spacing = 5;
	// 	h1->AddChild(o2UI.CreateButton("Button 1"));
	// 	h1->AddChild(o2UI.CreateLabel("Label text\nMulti line"));
	// 	mHorLayout->AddChild(h1);
	// 
	// 	mHorLayout2 = mnew UIHorizontalLayout();
	// 	mHorLayout2->AddChild(o2UI.CreateButton("Skotobaza"));
	// 	mHorLayout2->AddChild(o2UI.CreateButton("Bonanza"));
	// 	auto btn = o2UI.CreateButton("Add new button");
	// 	btn->onClick += [&]() { CreateNewBtn(); };
	// 	mHorLayout2->AddChild(btn);
	// 	mHorLayout2->spacing = 5;
	// 	mHorLayout->AddChild(mHorLayout2);
	// 
	// 	mHorLayout->border = RectF(5, 5, 5, 5);
	// 	mHorLayout->spacing = 5.0f;
	// 	
	// 	o2UI.AddWidget(Ptr<UIWidget>(mHorLayout));
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
	auto label = CheckLabel();
	auto editbox = CheckEditBox();

	if (!btn || !hprogress || !vprogress || !hscroll || !vscroll || !chkbox || !scrollArea || !label || !editbox)
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

bool UITestScreen::CheckScrollArea()
{
	if (o2UI.GetWidgetStyle<UIScrollArea>("standard"))
		return true;

	Ptr<UIScrollArea> sample = mnew UIScrollArea();
	sample->clipArea = Layout::Both(1, 2, 1, 1);
	sample->viewArea = Layout::Both(5, 15, 15, 5);
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

void UITestScreen::Unload()
{
	o2UI.RemoveAllWidgets();
}

void UITestScreen::Update(float dt)
{
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

void UITestScreen::CreateNewBtn()
{
	auto btn = o2UI.CreateButton("Btn");
	btn->layout.size = Vec2F(Math::Random(20, 40), Math::Random(20, 40));
	//btn->layout.size = mButton->layout.size;
	mHorLayout->AddChild(btn);

	//mVerLayout->border = RectF(5, 5, 5, 5);
	mHorLayout2->spacing = 5.0f;
}
