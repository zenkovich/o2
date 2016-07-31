#include "CodeToolApp.h"

#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Utils/Debug.h"
#include "BasicUIStyle.h"
#include "Utils/Timer.h"
#include "UI/UIManager.h"
#include "UI/Button.h"

namespace CodeTool
{
	CodeToolApplication::CodeToolApplication()
	{}

	CodeToolApplication::~CodeToolApplication()
	{}

	void CodeToolApplication::OnStarted()
	{
		auto button = o2UI.AddButton("Hello world", []() { o2Debug.Log("Click!"); });
		button->layout = UIWidgetLayout::Based(BaseCorner::Center, Vec2F(100, 30));
	}

	void CodeToolApplication::OnClosing()
	{}

	void CodeToolApplication::OnUpdate(float dt)
	{}

	void CodeToolApplication::OnDraw()
	{
		o2Render.Clear();
		o2Render.camera = Camera::Default();
	}

}