#include "UIRoot.h"

#include "Application/Application.h"
#include "UI/Widget.h"
#include "UI/WidgetLayout.h"

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

	void UIRoot::AddWidget(UIWidget* widget)
	{
		mRootWidget->AddChild(widget);
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