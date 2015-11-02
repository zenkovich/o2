#include "MainTestScreen.h"

#include "TestApplication.h"

MainTestScreen::MainTestScreen(Ptr<TestApplication> application):
	ITestScreen(application)
{
}

MainTestScreen::~MainTestScreen()
{
}

void MainTestScreen::Load()
{
	auto root = o2UI.AddWidget();
	root->layout.size = Vec2F(200, 400);

	auto textTest = o2UI.CreateButton("Text");
	textTest->name = "TextTestBtn";
	textTest->layout.anchorTop = 1.0f;
	textTest->layout.anchorBottom = 1.0f;
	textTest->layout.offsetBottom = -30.0f;
	textTest->onClick += [&]() { mApplication->GoToScreen("TextTestScreen"); };

	auto uiTest = o2UI.CreateButton("UI");
	uiTest->name = "UITestBtn";
	uiTest->layout.anchorTop = 1.0f;
	uiTest->layout.anchorBottom = 1.0f;
	uiTest->layout.offsetTop = -40.0f;
	uiTest->layout.offsetBottom = -70.0f;
	uiTest->onClick += [&]() { mApplication->GoToScreen("UITestScreen"); };

	root->AddChild(textTest.Cast<Widget>());
	root->AddChild(uiTest.Cast<Widget>());
}

void MainTestScreen::Unload()
{
	o2UI.RemoveAllWidgets();
}

void MainTestScreen::Update(float dt)
{
	mApplication->GoToScreen("UITestScreen");
}

void MainTestScreen::Draw()
{
}

String MainTestScreen::GetId() const
{
	return "MainTestScreen";
}
