#include "ui_hint_controller.h"

#include "ui_label.h"
#include "ui_controller.h"
#include "app/application.h"

OPEN_O2_NAMESPACE

UIHintController::UIHintController():
mWidget(NULL), mLabel(NULL), mNeedShow(false)
{

}

UIHintController::~UIHintController()
{
	SafeRelease(mWidget);
}

void UIHintController::ShowAt(const Vec2F& position, const String& hintString)
{
	if (!mWidget)
		return;

	mNeedShow = true;
	mNextPlacePosition = position;
	mNextPlaceHintText = hintString;

	if (mWidget->IsVisible())
		mWidget->SetVisible(false);
	else
		BeginAppearing();
}

void UIHintController::Hide()
{
	if (!mWidget)
		return;

	mWidget->SetVisible(false);
}

void UIHintController::SetupWidget(UIWidget* widget, UILabel* label)
{
	mWidget = widget;
	mLabel = label;
	mWidget->onVisibleOff.Add(callback<UIHintController>(this, &UIHintController::WidgetDisappeared));
	mWidget->GetState("visible")->SetState(false, true);
}

void UIHintController::WidgetDisappeared()
{
	if (!mNeedShow)
		return;

	BeginAppearing();
}

void UIHintController::BeginAppearing()
{
	if (!mWidget)
		return;

	mNeedShow = false;

	mLabel->SetCText(mNextPlaceHintText);
	Vec2F textRealSize = mLabel->GetTextRealSize();
	mWidget->SetSize(textRealSize + mLabel->GetLayout().mLTAbsolute -  mLabel->GetLayout().mRBAbsolute);

	mNextPlacePosition.x = Clamp(mNextPlacePosition.x, 0.0f, AppUI()->GetClientRectSize().x - mWidget->GetSize().x);
	mNextPlacePosition.y = Clamp(mNextPlacePosition.y, 0.0f, AppUI()->GetClientRectSize().y - mWidget->GetSize().y);

	mWidget->SetPosition(mNextPlacePosition);
	mWidget->SetVisible(true);
}

void UIHintController::Update(float dt)
{
	if (!mWidget)
		return;

	mWidget->Update(dt);
}

void UIHintController::Draw()
{
	if (!mWidget)
		return;

	mWidget->Draw();
}

CLOSE_O2_NAMESPACE