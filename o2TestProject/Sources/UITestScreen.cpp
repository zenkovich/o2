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

	mHorLayout2 = mnew HorizontalLayout();
	mHorLayout2->AddChild(o2UI.CreateButton("Skotobaza"));
	mHorLayout2->AddChild(o2UI.CreateButton("Bonanza"));
	auto btn = o2UI.CreateButton("Add new button");
	btn->onClick += [&]() { CreateNewBtn(); };
	mHorLayout2->AddChild(btn);
	mHorLayout2->spacing = 5;
	mHorLayout->AddChild(mHorLayout2);

	mHorLayout->border = RectF(5, 5, 5, 5);
	mHorLayout->spacing = 5.0f;
	
	o2UI.AddWidget(Ptr<Widget>(mHorLayout));
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
		mHorLayout2->expandHeight = !mHorLayout2->expandHeight;

	if (o2Input.IsKeyPressed('W'))
		mHorLayout2->expandWidth = !mHorLayout2->expandWidth;

	if (o2Input.IsKeyPressed(VK_NUMPAD1))
		mHorLayout2->baseCorner = HorizontalLayout::BaseCorner::LeftBottom;

	if (o2Input.IsKeyPressed(VK_NUMPAD2))
		mHorLayout2->baseCorner = HorizontalLayout::BaseCorner::Bottom;

	if (o2Input.IsKeyPressed(VK_NUMPAD3))
		mHorLayout2->baseCorner = HorizontalLayout::BaseCorner::RightBottom;

	if (o2Input.IsKeyPressed(VK_NUMPAD4))
		mHorLayout2->baseCorner = HorizontalLayout::BaseCorner::Left;

	if (o2Input.IsKeyPressed(VK_NUMPAD5))
		mHorLayout2->baseCorner = HorizontalLayout::BaseCorner::Center;

	if (o2Input.IsKeyPressed(VK_NUMPAD6))
		mHorLayout2->baseCorner = HorizontalLayout::BaseCorner::Right;

	if (o2Input.IsKeyPressed(VK_NUMPAD7))
		mHorLayout2->baseCorner = HorizontalLayout::BaseCorner::LeftTop;

	if (o2Input.IsKeyPressed(VK_NUMPAD8))
		mHorLayout2->baseCorner = HorizontalLayout::BaseCorner::Top;

	if (o2Input.IsKeyPressed(VK_NUMPAD9))
		mHorLayout2->baseCorner = HorizontalLayout::BaseCorner::RightTop;
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
