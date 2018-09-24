#include "TestApplication.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include "Assets\VectorFontAsset.h"
#include "MainTestScreen.h"
#include "Render\Camera.h"
#include "Render\Render.h"
#include "Render\VectorFontEffects.h"
#include "TextTestScreen.h"
#include "UITestScreen.h"
#include "UI\UIManager.h"

#undef DrawText

void TestApplication::OnStarted()
{
	//CheckArialFontEffects();
	Actor::SetDefaultCreationMode(ActorCreateMode::NotInScene);

	o2UI.LoadStyle("ui_style.xml");

	Actor::SetDefaultCreationMode(ActorCreateMode::InScene);

	mTestScreens.Add(mnew TextTestScreen(this));
	mTestScreens.Add(mnew UITestScreen(this));
	mTestScreens.Add(mnew MainTestScreen(this));

	//GoToScreen("MainTestScreen");
	GoToScreen("UITestScreen");
	//GoToScreen("TextTestScreen");
}

void TestApplication::OnUpdate(float dt)
{
	if (o2Input.IsKeyPressed(VK_CONTROL))
		mRulerStartPoint = o2Input.GetCursorPos();

	if (o2Input.IsKeyDown(VK_CONTROL))
	{
		o2Application.windowCaption = String::Format("FPS: %i (%vi)(%vi) dt: %f DC: %i", (int)o2Time.GetFPS(),
													 (Vec2I)o2Input.GetCursorPos(), 
													 (Vec2I)(mRulerStartPoint - o2Input.GetCursorPos()), 
													 o2Time.GetDeltaTime(),
													 o2Render.GetDrawCallsCount());
	}
	else
	{
		o2Application.windowCaption = String::Format("FPS: %i (%vi) dt: %f DC: %i", (int)o2Time.GetFPS(), 
													 (Vec2I)o2Input.GetCursorPos(), o2Time.GetDeltaTime(),
													 o2Render.GetDrawCallsCount());
	}


	if (mCurrentScreen)
		mCurrentScreen->Update(dt);

	if (mNextCurrentScreen)
	{
		if (mCurrentScreen)
			mCurrentScreen->Unload();

		mCurrentScreen = mNextCurrentScreen;
		mCurrentScreen->Load();

		mNextCurrentScreen = nullptr;
	}
}

void TestApplication::OnDraw()
{
	o2Render.Clear();
	o2Render.SetCamera(Camera::Default());

	if (mCurrentScreen)
		mCurrentScreen->Draw();
}

void TestApplication::GoToScreen(const String& id)
{
	mNextCurrentScreen = mTestScreens.FindMatch([&](auto x) { return x->GetId() == id; });
}

void TestApplication::CheckArialFontEffects()
{}

