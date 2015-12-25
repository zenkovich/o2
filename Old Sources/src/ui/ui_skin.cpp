#include "ui_skin.h"

#include "render_system/render_system.h"
#include "render_system/texture.h"
#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_editbox.h"
#include "ui_hor_layout.h"
#include "ui_label.h"
#include "ui_progressbar.h"
#include "ui_rect.h"
#include "ui_scroll_bar.h"
#include "ui_scrollarea.h"
#include "ui_sprite.h"
#include "ui_transition_state.h"
#include "ui_ver_layout.h"
#include "ui_widget.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(UISkinManager);

UISkinManager::UISkinManager()
{
	mWidgetIdx = 0;
}

UIWidget* UISkinManager::Widget(const Vec2F& size, const Vec2F& position /*= vec2f()*/, const String& id /*= ""*/)
{
	UIWidget* widget = mnew UIWidget(Layout::Fixed(position, size), CheckEmptyId(id, "widget"));
	return widget;
}

UIButton* UISkinManager::Button(const String& caption, const Layout& layout, const String& id /*= ""*/)
{
	UIButton* button = static_cast<UIButton*>(mButtonSample->Clone());
	button->GetLayout(layout);
	button->SetId(CheckEmptyId(id, "button"));
	button->SetCCaption(caption);

	return button;
}

void UISkinManager::SetButtonSample(UIButton* buttonSample)
{
	mButtonSample = buttonSample;
}

UIRect* UISkinManager::Background(const Layout& layout /*= uiBothLayout()*/, const String& id /*= ""*/)
{
	UIRect* background = static_cast<UIRect*>(mBackgroundSample->Clone());
	background->GetLayout(layout);
	background->SetId(CheckEmptyId(id, "background"));

	return background;
}

void UISkinManager::SetBackgroundSample(UIRect* backgroundSample)
{
	mBackgroundSample = backgroundSample;
}

UISprite* UISkinManager::Sprite(const TextureRef& texture, const Layout& layout /*= cLayout::both()*/,
								const String& id /*= ""*/)
{
	UISprite* spriteWidget = mnew UISprite(layout, CheckEmptyId(id, "sprite"));
	spriteWidget->mSprite.SetTexture(texture);
	spriteWidget->mSprite.SetTextureSrcRect(RectF(Vec2F(), texture.getSize()));

	AddVisibleState(spriteWidget);

	return spriteWidget;
}

void UISkinManager::AddVisibleState(UIWidget* widget, float duration /*= 0.5f*/)
{
	UITransitionState* state = mnew UITransitionState("visible");
	state->AddProperty(&widget->transparency, 0.0f, 1.0f, duration);
	widget->AddState(state);
}

void UISkinManager::SetProgressbarSample(UIProgressBar* progressbarSample)
{
	mProgressBarSample = progressbarSample;
}

UIProgressBar* UISkinManager::ProgressBar(const Layout& layout /*= cLayout::both()*/,
										  const String& id /*= ""*/, float value /*= 0*/,
										  float minValue /*= 0*/, float maxValue /*= 1*/)
{
	UIProgressBar* progressbar = static_cast<UIProgressBar*>(mProgressBarSample->Clone());
	progressbar->GetLayout(layout);
	progressbar->SetId(CheckEmptyId(id, "progressBar"));
	progressbar->SetValueRange(minValue, maxValue);
	progressbar->SetValue(value);
	return progressbar;
}

void UISkinManager::SetCheckBoxSample(UICheckBox* checkbox)
{
	mCheckBoxSample = checkbox;
}

UICheckBox* UISkinManager::CheckBox(const String& caption, const Layout& layout, const String& id /*= ""*/,
									bool checked /*= false*/)
{
	UICheckBox* checkbox = static_cast<UICheckBox*>(mCheckBoxSample->Clone());
	checkbox->GetLayout(layout);
	checkbox->SetCCaption(caption);
	checkbox->SetId(CheckEmptyId(id, "checkbox"));
	checkbox->SetChecked(checked);
	return checkbox;
}

UIScrollBar* UISkinManager::HorScrollBar(const Layout& layout /*= cLayout::both()*/, const String& id /*= ""*/,
										 float value /*= 0*/, float minValue /*= 0*/, float maxValue /*= 1*/,
										 float barSize /*= 0.1f*/)
{
	UIScrollBar* scrollbar = static_cast<UIScrollBar*>(mHorScrollbarSample->Clone());
	scrollbar->GetLayout(layout);
	scrollbar->SetId(CheckEmptyId(id, "horScrollbar"));
	scrollbar->SetValueRange(minValue, maxValue);
	scrollbar->SetValue(value);
	scrollbar->SetBarSize(barSize);
	return scrollbar;
}

UIScrollBar* UISkinManager::VerThinScrollBar(const Layout& layout /*= cLayout::both()*/, const String& id /*= ""*/,
											 float value /*= 0*/, float minValue /*= 0*/, float maxValue /*= 1*/,
											 float barSize /*= 0.1f*/)
{
	UIScrollBar* scrollbar = static_cast<UIScrollBar*>(mVerThinScrollbarSample->Clone());
	scrollbar->GetLayout(layout);
	scrollbar->SetId(CheckEmptyId(id, "verThisScrollBar"));
	scrollbar->SetValueRange(minValue, maxValue);
	scrollbar->SetValue(value);
	scrollbar->SetBarSize(barSize);
	return scrollbar;
}

UIScrollBar* UISkinManager::HorThinScrollBar(const Layout& layout /*= cLayout::both()*/, const String& id /*= ""*/,
											 float value /*= 0*/, float minValue /*= 0*/, float maxValue /*= 1*/,
											 float barSize /*= 0.1f*/)
{
	UIScrollBar* scrollbar = static_cast<UIScrollBar*>(mHorThinScrollbarSample->Clone());
	scrollbar->GetLayout(layout);
	scrollbar->SetId(CheckEmptyId(id, "horThisScrollBar"));
	scrollbar->SetValueRange(minValue, maxValue);
	scrollbar->SetValue(value);
	scrollbar->SetBarSize(barSize);
	return scrollbar;
}

void UISkinManager::SetHorScrollbarSample(UIScrollBar* scrollbar)
{
	mHorScrollbarSample = scrollbar;
}

UIEditBox* UISkinManager::Editbox(const Layout& layout /*= cLayout::both()*/, const String& id /*= ""*/,
								  const String& text /*= ""*/, bool multiLine /*= false*/)
{
	UIEditBox* src = multiLine ? mMultilineEditBox:mSingleLineEditBoxSample;
	UIEditBox* res = static_cast<UIEditBox*>(src->Clone());
	res->GetLayout(layout);
	res->SetId(CheckEmptyId(id, "editbox"));
	res->SetCText(text);
	return res;
}

void UISkinManager::SetSingleLineEditBoxSample(UIEditBox* editboxSampl)
{
	mSingleLineEditBoxSample = editboxSampl;
}

UILabel* UISkinManager::Label(const String& text, const Layout& layout /*= cLayout::both()*/, const String& id /*= ""*/)
{
	UILabel* res = static_cast<UILabel*>(mLabelSample->Clone());
	res->GetLayout(layout);
	res->SetId(CheckEmptyId(id, "label"));
	res->SetCText(text);
	return res;
}

void UISkinManager::SetLabelSample(UILabel* labelSample)
{
	mLabelSample = labelSample;
}

void UISkinManager::SetMultilineEditBoxSample(UIEditBox* editboxSample)
{
	mMultilineEditBox = editboxSample;
}

void UISkinManager::SetHorThinScrollbarSample(UIScrollBar* scrollbar)
{
	mHorThinScrollbarSample = scrollbar;
}

void UISkinManager::SetVerThinScrollbarSample(UIScrollBar* scrollbar)
{
	mVerThinScrollbarSample = scrollbar;
}

void UISkinManager::SetScrollAreaSample(UIScrollArea* scrollAreaSample)
{
	mScrollAreaSample = scrollAreaSample;
}

UIScrollArea* UISkinManager::ScrollArea(const Layout& layout /*= cLayout::both()*/, const String& id /*= ""*/)
{
	UIScrollArea* res = static_cast<UIScrollArea*>(mScrollAreaSample->Clone());
	res->GetLayout(layout);
	res->SetId(CheckEmptyId(id, "scrollArea"));
	return res;
}

UIHorLayout* UISkinManager::HorLayout(const Layout& layout /*= cLayout::both()*/, const String& id/*= ""*/,
									  float widgetsDistance /*= 10.0f*/)
{
	return mnew UIHorLayout(layout, widgetsDistance, CheckEmptyId(id, "horlayout"));
}

UIVerLayout* UISkinManager::VerLayout(const Layout& layout /*= cLayout::both()*/, const String& id/*= ""*/, float widgetsDistance /*= 10.0f*/)
{
	return mnew UIVerLayout(layout, widgetsDistance, CheckEmptyId(id, "verlayout"));
}

String UISkinManager::CheckEmptyId(const String& id, const String& prefix)
{
	if (id.empty())
		return prefix + ToString(mWidgetIdx++);

	return id;
}

void UISkinManager::SetRectPadSample(UIRect* padSample)
{
	mRectPadSample = padSample;
}

UIRect* UISkinManager::RectPad(const Layout& layout /*= cLayout::both()*/, const String& id /*= ""*/)
{
	UIRect* res = mnew UIRect(*mRectPadSample);
	res->GetLayout(layout);
	res->SetId(CheckEmptyId(id, "pad"));
	return res;
}

CLOSE_O2_NAMESPACE