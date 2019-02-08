#include "stdafx.h"
#include "UIRoot.h"

#include "Application/Application.h"
#include "Scene/UI/Widget.h"
#include "Scene/UI/WidgetLayout.h"

DECLARE_SINGLETON(Editor::UIRoot);

namespace Editor
{
	UIRoot::UIRoot()
	{
		mRootWidget = mnew UIWidget(ActorCreateMode::NotInScene);
	}

	UIRoot::~UIRoot()
	{
		delete mRootWidget;
	}

	UIWidget* UIRoot::AddWidget(UIWidget* widget)
	{
		mRootWidget->AddChild(widget);
		return widget;
	}

	void UIRoot::RemoveWidget(UIWidget* widget)
	{
		mRootWidget->RemoveChild(widget);
	}

	void UIRoot::RemoveAllWidgets()
	{
		mRootWidget->RemoveAllChildren();
	}

	UIWidget* UIRoot::GetRootWidget()
	{
		return mRootWidget;
	}

	void UIRoot::Draw()
	{
		mRootWidget->Draw();
	}

	void UIRoot::Update(float dt)
	{
		mRootWidget->Update(dt);
		mRootWidget->UpdateChildren(dt);
	}

	void UIRoot::OnApplicationSized()
	{
		*mRootWidget->layout = UIWidgetLayout::Based(BaseCorner::Center, o2Application.GetContentSize());
	}

}