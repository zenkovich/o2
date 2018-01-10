#include "MainTestScreen.h"

#include "TestApplication.h"
#include "UI\Button.h"
#include "UI\UIManager.h"
#include "UI\Widget.h"

MainTestScreen::MainTestScreen(TestApplication* application):
	ITestScreen(application)
{
}

MainTestScreen::~MainTestScreen()
{
}

void MainTestScreen::Load()
{
}

void MainTestScreen::Unload()
{
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
