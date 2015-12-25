#pragma once

#include "public.h"
#include "render_system/texture.h"
#include "ui_button.h"
#include "ui_rect.h"
#include "util/math/layout.h"
#include "util/singleton.h"

OPEN_O2_NAMESPACE

class UIWidget;
class UISprite;
class UIProgressBar;
class UICheckBox;
class UIScrollBar;
class UIEditBox;
class UILabel;
class UIScrollArea;
class UIHorLayout;
class UIVerLayout;
class UIRect;

/** UI Skin help class. This load a some user interface skin and gives functions for fast and easy widgets creation. */
class UISkinManager: public Singleton<UISkinManager>
{
	friend class UIStdSkinInitializer;

protected:
	UIButton*      mButtonSample;
	UIRect*        mBackgroundSample;
	UIProgressBar* mProgressBarSample;
	UICheckBox*    mCheckBoxSample;
	UIScrollBar*   mHorScrollbarSample;
	UIScrollBar*   mHorThinScrollbarSample;
	UIScrollBar*   mVerThinScrollbarSample;
	UIEditBox*     mSingleLineEditBoxSample;
	UIEditBox*     mMultilineEditBox;
	UILabel*       mLabelSample;
	UIScrollArea*  mScrollAreaSample;
	UIRect*        mRectPadSample;
	int            mWidgetIdx;

public:
	/** ctor. */
	UISkinManager();

	/** Creates widget. */
	UIWidget* Widget(const Vec2F& size, const Vec2F& position = Vec2F(), const String& id = "");

	/** Creates background rectangle widget. */
	UIRect* Background(const Layout& layout = Layout::Both(), const String& id = "");

	/** Creates button. */
	UIButton* Button(const String& caption, const Layout& layout, const String& id = "");

	/** Creates sprite widget from texture. */
	UISprite* Sprite(const TextureRef& texture, const Layout& layout = Layout::Both(), const String& id = "");

	/** Creates progress bar. */
	UIProgressBar* ProgressBar(const Layout& layout = Layout::Both(), const String& id = "",
		                       float value = 0, float minValue = 0, float maxValue = 1);

	/** Creates checkbox. */
	UICheckBox* CheckBox(const String& caption, const Layout& layout, const String& id = "", bool checked = false);

	/** Creates horizontal scroll bar. */
	UIScrollBar* HorScrollBar(const Layout& layout = Layout::Both(), const String& id = "",
		                      float value = 0, float minValue = 0, float maxValue = 1, float barSize = 0.1f);

	/** Creates thin horizontal scroll bar. */
	UIScrollBar* HorThinScrollBar(const Layout& layout = Layout::Both(), const String& id = "",
		                          float value = 0, float minValue = 0, float maxValue = 1, float barSize = 0.1f);

	/** Creates thin horizontal scroll bar. */
	UIScrollBar* VerThinScrollBar(const Layout& layout = Layout::Both(), const String& id = "",
		                          float value = 0, float minValue = 0, float maxValue = 1, float barSize = 0.1f);

	/** Creates edit box. */
	UIEditBox* Editbox(const Layout& layout = Layout::Both(), const String& id = "", const String& text = "",
		               bool multiLine = false);

	/** Creates label. */
	UILabel* Label(const String& text, const Layout& layout = Layout::Both(), const String& id = "");

	/** Creates scroll area. */
	UIScrollArea* ScrollArea(const Layout& layout = Layout::Both(), const String& id = "");

	/** Creates horizontal layout widget. */
	UIHorLayout* HorLayout(const Layout& layout = Layout::Both(), const String& id= "", float widgetsDistance = 10.0f);

	/** Creates vertical layout widget. */
	UIVerLayout* VerLayout(const Layout& layout = Layout::Both(), const String& id= "", float widgetsDistance = 10.0f);

	/** Creates rectangle pad widget. */
	UIRect* RectPad(const Layout& layout = Layout::Both(), const String& id = "");


	/** Sets button sample. */
	void SetButtonSample(UIButton* buttonSample);

	/** Sets background sample. */
	void SetBackgroundSample(UIRect* backgroundSample);

	/** Sets progress bar sample. */
	void SetProgressbarSample(UIProgressBar* progressbarSample);

	/** Sets checkbox sample. */
	void SetCheckBoxSample(UICheckBox* checkbox);

	/** Sets horizontal scroll bar sample. */
	void SetHorScrollbarSample(UIScrollBar* scrollbar);

	/** Sets horizontal thin scroll bar sample. */
	void SetHorThinScrollbarSample(UIScrollBar* scrollbar);

	/** Sets vertical thin scroll bar sample. */
	void SetVerThinScrollbarSample(UIScrollBar* scrollbar);

	/** Sets single line edit box sample. */
	void SetSingleLineEditBoxSample(UIEditBox* editboxSampl);

	/** Sets multi line edit box sample. */
	void SetMultilineEditBoxSample(UIEditBox* editboxSample);

	/** Sets label sample. */
	void SetLabelSample(UILabel* labelSample);

	/** Sets scrollarea sample. */
	void SetScrollAreaSample(UIScrollArea* scrollAreaSample);

	/** Sets rectangle pad sample. */
	void SetRectPadSample(UIRect* padSample);

protected:
	void AddVisibleState(UIWidget* widget, float duration = 0.5f);

	String CheckEmptyId(const String& id, const String& prefix);
};

#define uiSkin() UISkinManager::InstancePtr()

CLOSE_O2_NAMESPACE
