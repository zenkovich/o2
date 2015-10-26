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
	mBackground.LoadFromImage("ui/UI_Background.png");
	mBackground.size = (Vec2I)o2Render.resolution;

// 	Ptr<Button> buttonStyleSample = mnew Button();
// 	buttonStyleSample->AddSpriteLayer("regular", "ui/UI_button_regular.png", Layout::Both(-10, -10, -11, -11));
// 	auto selectLayer = buttonStyleSample->AddSpriteLayer("select", "ui/UI_button_select.png", Layout::Both(-10, -10, -11, -11));
// 	auto pressedLayer = buttonStyleSample->AddSpriteLayer("pressed", "ui/UI_button_pressed.png", Layout::Both(-10, -10, -11, -11));
// 	buttonStyleSample->AddTextLayer("caption", "Button", "arial.ttf");
// 
// 	Animation selectStateAnim;
// 	selectStateAnim.SetTarget(buttonStyleSample);
// 	*selectStateAnim.AddAnimationValue<float>(&selectLayer->drawable->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.1f);
// 
// 	auto selectState = buttonStyleSample->AddState("select", selectStateAnim);
// 	selectState->offStateAnimationSpeed = 1.0f/4.0f;
// 
// 	Animation pressedStateAnim;
// 	pressedStateAnim.SetTarget(buttonStyleSample);
// 	*pressedStateAnim.AddAnimationValue<float>(&pressedLayer->drawable->transparency) = AnimatedValue<float>::EaseInOut(0, 1, 0.05f);
// 
// 	auto pressedState = buttonStyleSample->AddState("pressed", pressedStateAnim);
// 	pressedState->offStateAnimationSpeed = 0.5f;
// 
// 	buttonStyleSample->onClick += []() { o2Debug.Log("Clicked!"); };
// 
// 	o2UI.AddStyle(buttonStyleSample.Cast<Widget>(), "standard");
// 	o2UI.SaveStyle("ui_style.xml");
// 
// 	o2UI.LoadStyle("ui_style.xml");

	mFakeWindow.LoadFromImage("ui/UI_window_frame_regular.png");
	mFakeWindow.size = Vec2F(550, 550);

	mHorLayout = mnew HorizontalLayout();
	mHorLayout->layout.size = Vec2F(500, 500);

	auto h1 = mnew HorizontalLayout();
	h1->spacing = 5;
	h1->AddChild(o2UI.CreateButton("Button 1"));
	h1->AddChild(o2UI.CreateLabel("Label text\nMulti line"));
	mHorLayout->AddChild(h1);

	mVerLayout = mnew VerticalLayout();
	mVerLayout->AddChild(o2UI.CreateButton("Skotobaza"));
	mVerLayout->AddChild(o2UI.CreateButton("Bonanza"));
	auto btn = o2UI.CreateButton("Add new button");
	btn->onClick += [&]() { CreateNewBtn(); };
	mVerLayout->AddChild(btn);
	mVerLayout->spacing = 5;
	mHorLayout->AddChild(mVerLayout);

	mHorLayout->border = RectF(5, 5, 5, 5);
	mHorLayout->spacing = 5.0f;
	
	o2UI.AddWidget(mHorLayout);
}

void UITestScreen::Unload()
{
	o2UI.RemoveAllWidgets();
}

void UITestScreen::Update(float dt)
{
	if (o2Input.IsKeyPressed('H'))
		mButton->captionc = "alala\nkkk";

	if (o2Input.IsKeyPressed(VK_ESCAPE))
		mApplication->GoToScreen("MainTestScreen");

	if (o2Input.IsKeyPressed('Q'))
		mVerLayout->expandHeight = !mVerLayout->expandHeight;

	if (o2Input.IsKeyPressed('W'))
		mVerLayout->expandWidth = !mVerLayout->expandWidth;

	if (o2Input.IsKeyPressed(VK_NUMPAD1))
		mVerLayout->baseCorner = VerticalLayout::BaseCorner::LeftBottom;

	if (o2Input.IsKeyPressed(VK_NUMPAD2))
		mVerLayout->baseCorner = VerticalLayout::BaseCorner::Bottom;

	if (o2Input.IsKeyPressed(VK_NUMPAD3))
		mVerLayout->baseCorner = VerticalLayout::BaseCorner::RightBottom;

	if (o2Input.IsKeyPressed(VK_NUMPAD4))
		mVerLayout->baseCorner = VerticalLayout::BaseCorner::Left;

	if (o2Input.IsKeyPressed(VK_NUMPAD5))
		mVerLayout->baseCorner = VerticalLayout::BaseCorner::Center;

	if (o2Input.IsKeyPressed(VK_NUMPAD6))
		mVerLayout->baseCorner = VerticalLayout::BaseCorner::Right;

	if (o2Input.IsKeyPressed(VK_NUMPAD7))
		mVerLayout->baseCorner = VerticalLayout::BaseCorner::LeftTop;

	if (o2Input.IsKeyPressed(VK_NUMPAD8))
		mVerLayout->baseCorner = VerticalLayout::BaseCorner::Top;

	if (o2Input.IsKeyPressed(VK_NUMPAD9))
		mVerLayout->baseCorner = VerticalLayout::BaseCorner::RightTop;
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
	mVerLayout->AddChild(btn);

	//mVerLayout->border = RectF(5, 5, 5, 5);
	mVerLayout->spacing = 5.0f;
}
