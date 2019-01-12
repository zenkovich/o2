#include "stdafx.h"
#include "UIManager.h"

#include "Application/Application.h"
#include "Assets/Assets.h"
#include "Render/Render.h"
#include "UI/Button.h"
#include "UI/ContextMenu.h"
#include "UI/CustomDropDown.h"
#include "UI/CustomList.h"
#include "UI/DropDown.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/HorizontalProgress.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/Image.h"
#include "UI/Label.h"
#include "UI/List.h"
#include "UI/ScrollArea.h"
#include "UI/Toggle.h"
#include "UI/VerticalLayout.h"
#include "UI/VerticalProgress.h"
#include "UI/VerticalScrollBar.h"
#include "UI/Widget.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "UI/Window.h"
#include "Utils/Debug/Debug.h"
#include "Utils/Debug/Log/LogStream.h"
#include "Utils/System/Time/Timer.h"

#undef CreateWindow

namespace o2
{
	DECLARE_SINGLETON(UIManager);

	UIManager::UIManager()
	{
		mLog = mnew LogStream("UI");
		o2Debug.GetLog()->BindStream(mLog);
	}

	UIManager::~UIManager()
	{
		ClearStyle();
	}

	void UIManager::FocusWidget(UIWidget* widget)
	{
		if (widget == mFocusedWidget || (widget && !widget->IsFocusable()))
			return;

		if (mFocusedWidget)
		{
			auto lastFocusedWidget = mFocusedWidget;
			mFocusedWidget = nullptr;

			lastFocusedWidget->mIsFocused = false;
			lastFocusedWidget->OnUnfocused();

			if (lastFocusedWidget->mFocusedState)
				lastFocusedWidget->mFocusedState->SetState(false);
		}

		mFocusedWidget = widget;

		if (mFocusedWidget)
		{
			mFocusedWidget->mIsFocused = true;

			mFocusedWidget->OnFocused();

			if (mFocusedWidget->mParentWidget)
				mFocusedWidget->mParentWidget->OnChildFocused(mFocusedWidget);

			if (mFocusedWidget->mFocusedState)
				mFocusedWidget->mFocusedState->SetState(true);
		}
	}

	UIWidget* UIManager::GetFocusedWidget() const
	{
		return mFocusedWidget;
	}

	void UIManager::FocusNextWidget()
	{
		bool fnd = mFocusedWidget == nullptr;
		UIWidget* nextFocusingWidget = nullptr;
		for (auto widget : mFocusableWidgets)
		{
			if (!fnd)
			{
				if (widget == mFocusedWidget)
					fnd = true;
			}
			else
			{
				if (widget->IsFocusable())
				{
					nextFocusingWidget = widget;
					break;
				}
			}
		}

		FocusWidget(nextFocusingWidget);
	}

	void UIManager::LoadStyle(const String& path)
	{
		Timer t;

		DataNode styleData;
		styleData.LoadFromFile(o2Assets.GetDataPath() + path);

		o2Debug.Log("Loaded file " + path + " for " + String(t.GetDeltaTime()) + "sec");

		for (auto st : mStyleSamples)
			delete st;

		mStyleSamples.Clear();

		mStyleSamples = styleData;

		for (auto styleSample : mStyleSamples)
			styleSample->Hide(true);

		o2Debug.Log("Loaded styles for " + String(t.GetDeltaTime()) + "sec");
	}

	void UIManager::SaveStyle(const String& path)
	{
		DataNode styleData;
		styleData = mStyleSamples;
		styleData.SaveToFile(o2Assets.GetAssetsPath() + path);

		o2Assets.RebuildAssets();
	}

	void UIManager::ClearStyle()
	{
		for (auto sample : mStyleSamples)
			delete sample;

		mStyleSamples.Clear();
	}

	void UIManager::AddWidgetStyle(UIWidget* widget, const String& style)
	{
		widget->Hide(true);
		widget->SetName(style);
		mStyleSamples.Add(widget);
	}

	UIWidget* UIManager::CreateWidget(const Type& type, const String& style /*= "standard"*/)
	{
		UIWidget* sample = GetWidgetStyle(type, style);
		if (!sample)
			sample = GetWidgetStyle(type, "standard");

		UIWidget* res = nullptr;

		if (sample)
			res = sample->CloneAs<UIWidget>();
		else
			res = (UIWidget*)type.CreateSample();

		if (type != TypeOf(UIContextMenu))
			res->SetEnableForcible(true);

		return res;
	}

	UIWidget* UIManager::GetWidgetStyle(const Type& type, const String& style)
	{
		for (auto styleWidget : mStyleSamples)
		{
			if (type == styleWidget->GetType())
			{
				if (style == styleWidget->GetName())
					return styleWidget;
			}
		}

		return nullptr;
	}

	UIButton* UIManager::CreateButton(const WString& caption, const Function<void()>& onClick /*= Function<void()>()*/,
									  const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIButton>(style);
		res->caption = caption;
		res->name = caption + " button";
		res->onClick = onClick;
		return res;
	}

	UIWindow* UIManager::CreateWindow(const WString& caption, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIWindow>(style);
		res->caption = caption;
		res->name = caption + " window";
		return res;
	}

	UILabel* UIManager::CreateLabel(const WString& text, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UILabel>(style);
		res->text = text;
		res->name = text + " label";
		res->horOverflow = UILabel::HorOverflow::Dots;
		return res;
	}

	UIHorizontalLayout* UIManager::CreateHorLayout()
	{
		auto res = mnew UIHorizontalLayout();
		res->name = "horizontal layout";
		*res->layout = UIWidgetLayout::BothStretch();
		return res;
	}

	UIVerticalLayout* UIManager::CreateVerLayout()
	{
		auto res = mnew UIVerticalLayout();
		res->name = "vertical layout";
		*res->layout = UIWidgetLayout::BothStretch();
		return res;
	}

	UIHorizontalProgress* UIManager::CreateHorProgress(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIHorizontalProgress>(style);
		res->name = "horizontal progress";
		return res;
	}

	UIVerticalProgress* UIManager::CreateVerProgress(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIVerticalProgress>(style);
		res->name = "vertical progress";
		return res;
	}

	UIHorizontalScrollBar* UIManager::CreateHorScrollBar(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIHorizontalScrollBar>(style);
		res->name = "horizontal scroll bar";
		return res;
	}

	UIVerticalScrollBar* UIManager::CreateVerScrollBar(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIVerticalScrollBar>(style);
		res->name = "vertical scroll bar";
		return res;
	}

	UIScrollArea* UIManager::CreateScrollArea(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIScrollArea>(style);
		res->name = "scroll area";
		return res;
	}

	UIEditBox* UIManager::CreateEditBox(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIEditBox>(style);
		res->name = "edit box";
		return res;
	}

	UICustomList* UIManager::CreateCustomList(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UICustomList>(style);
		res->name = "custom list";
		return res;
	}

	UIList* UIManager::CreateList(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIList>(style);
		res->name = "list";
		return res;
	}

	UICustomDropDown* UIManager::CreateCustomDropdown(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UICustomDropDown>(style);
		res->name = "custom dropdown";
		return res;
	}

	UIDropDown* UIManager::CreateDropdown(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIDropDown>(style);
		res->name = "dropdown";
		return res;
	}

	UIToggle* UIManager::CreateToggle(const WString& caption, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIToggle>(style);
		res->caption = caption;
		res->name = caption + " toggle";
		return res;
	}

	UIImage* UIManager::CreateImage(const String& name)
	{
		auto res = mnew UIImage();
		res->name = name;
		res->SetImageName(name);
		return res;
	}

	void UIManager::Draw()
	{
		for (auto widget : mTopWidgets)
			widget->Draw();

		mTopWidgets.Clear();

		if (UIContextMenu::mVisibleContextMenu)
			UIContextMenu::mVisibleContextMenu->SpecialDraw();
	}

	void UIManager::DrawWidgetAtTop(UIWidget* widget)
	{
		mTopWidgets.Add(widget);
	}

	const UIManager::WidgetsVec& UIManager::GetWidgetStyles() const
	{
		return mStyleSamples;
	}

	void UIManager::TryLoadStyle()
	{
		if (o2Assets.IsAssetExist("ui_style.xml"))
			LoadStyle("ui_style.xml");
	}
}