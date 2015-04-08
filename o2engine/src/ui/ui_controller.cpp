#include "ui_controller.h"

#include "app/application.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

UIController::UIController():
	mFocusWidget(NULL), mBasicWidget(Layout::Both(), "UIController"), mChangedFocusWidget(false)
{
}

UIController::~UIController()
{
}

void UIController::Update(float dt)
{
	if (mChangedFocusWidget)
	{
		mFocusWidget->OnFocused();
		/*mBasicWidget.mChildWidgets.erase( FIND(mBasicWidget.mChildWidgets, mFocusWidget) );
		mBasicWidget.mChildWidgets.push_back(mFocusWidget);*/

		mChangedFocusWidget = false;
	}

	mBasicWidget.size = AppRender()->GetResolution();
	mBasicWidget.Update(dt);
	mHintController.update(dt);

	if (mFocusWidget)
		mFocusWidget->ProcessInputMessage(*AppInput());

	mBasicWidget.ProcessInputMessage(*AppInput());
}

void UIController::Draw()
{
	mBasicWidget.Draw();
	mHintController.draw();
}

UIWidget* UIController::AddWidget(UIWidget* widget)
{
	mBasicWidget.AddChild(widget);
	return widget;
}

bool UIController::RemoveWidget(UIWidget* widget)
{
	mBasicWidget.RemoveChild(widget);
	return true;
}

bool UIController::RemoveAllWidgets()
{
	mBasicWidget.RemoveAllChilds();
	return true;
}

UIWidget* UIController::GetWidget(const String& idPath)
{
	return mBasicWidget.GetWidget(idPath);
}

void UIController::FocusOnWidget(UIWidget* widget)
{
	if (mFocusWidget)
		mFocusWidget->OnFocusLost();

	mFocusWidget = widget;

	if (mFocusWidget)
		mChangedFocusWidget = true;
}

void UIController::ShowHint( const String& hintText, const Vec2F& position )
{
	mHintController.showAt(position, hintText);
}

void UIController::HideHint()
{
	mHintController.hide();
}

Vec2F UIController::GetClientRectSize() const
{
	return mBasicWidget.GetSize();
}

CLOSE_O2_NAMESPACE