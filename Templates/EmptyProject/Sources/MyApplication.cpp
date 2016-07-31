#include "MyApplication.h"

#include "Render/Render.h"
#include "UI/UIManager.h"
#include "UI/Button.h"

namespace MyApp
{
	MyApplication::MyApplication()
	{}

	MyApplication::~MyApplication()
	{}

	void MyApplication::OnStarted()
	{
		auto button = o2UI.AddButton("Hello world", []() { o2Debug.Log("Click!"); });
		button->layout = UIWidgetLayout::Based(BaseCorner::Center, Vec2F(100, 30));
	}

	void MyApplication::OnClosing()
	{}

	void MyApplication::OnUpdate(float dt)
	{}

	void MyApplication::OnDraw()
	{
		o2Render.Clear();
		o2Render.camera = Camera::Default();
	}

}