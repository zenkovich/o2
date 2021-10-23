#include "o2Editor/stdafx.h"
#include "UIRoot.h"

#include "o2/Application/Application.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"

DECLARE_SINGLETON(Editor::UIRoot);

namespace Editor
{
	UIRoot::UIRoot()
	{
		mRootWidget = mnew Widget(ActorCreateMode::NotInScene);
	}

	UIRoot::~UIRoot()
	{
		delete mRootWidget;
	}

	Widget* UIRoot::AddWidget(Widget* widget)
	{
		mRootWidget->AddChild(widget);
		return widget;
	}

	void UIRoot::RemoveWidget(Widget* widget)
	{
		mRootWidget->RemoveChild(widget);
	}

	void UIRoot::RemoveAllWidgets()
	{
		mRootWidget->RemoveAllChildren();
	}

	Widget* UIRoot::GetRootWidget()
	{
		return mRootWidget;
	}

	void UIRoot::Draw()
	{
		mListenersLayer.OnBeginDraw();
		
		Camera prevCamera = o2Render.GetCamera();
		
		float scale = o2Application.GetGraphicsScale();
		Camera camera = Camera::Default();
		camera.scale = Vec2F(1.0f/scale, 1.0f/scale);
		o2Render.camera = camera;
		
		mListenersLayer.camera = o2Render.GetCamera();
		
		mRootWidget->Draw();
		
		o2Render.SetCamera(prevCamera);
		
		mListenersLayer.OnEndDraw();
		mListenersLayer.OnDrawn(Camera::Default().GetBasis());
	}

	void UIRoot::Update(float dt)
	{
		mRootWidget->Update(dt);
		mRootWidget->UpdateChildren(dt);
	}

	void UIRoot::OnApplicationSized()
	{
		*mRootWidget->layout = WidgetLayout::Based(BaseCorner::Center, o2Application.GetContentSize()/o2Application.GetGraphicsScale());
	}

}
