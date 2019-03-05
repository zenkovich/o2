#include "stdafx.h"
#include "UIManager.h"

#include "Application/Application.h"
#include "Assets/Assets.h"
#include "Render/Render.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/ContextMenu.h"
#include "Scene/UI/Widgets/CustomDropDown.h"
#include "Scene/UI/Widgets/CustomList.h"
#include "Scene/UI/Widgets/DropDown.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/Widgets/HorizontalLayout.h"
#include "Scene/UI/Widgets/HorizontalProgress.h"
#include "Scene/UI/Widgets/HorizontalScrollBar.h"
#include "Scene/UI/Widgets/Image.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/Widgets/List.h"
#include "Scene/UI/Widgets/ScrollArea.h"
#include "Scene/UI/Widgets/Toggle.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "Scene/UI/Widgets/VerticalProgress.h"
#include "Scene/UI/Widgets/VerticalScrollBar.h"
#include "Scene/UI/Widget.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"
#include "Scene/UI/Widgets/Window.h"
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

	void UIManager::FocusWidget(Widget* widget)
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

	Widget* UIManager::GetFocusedWidget() const
	{
		return mFocusedWidget;
	}

	void UIManager::FocusNextWidget()
	{
		bool fnd = mFocusedWidget == nullptr;
		Widget* nextFocusingWidget = nullptr;
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

	void UIManager::AddWidgetStyle(Widget* widget, const String& style)
	{
		widget->Hide(true);
		widget->SetName(style);
		mStyleSamples.Add(widget);
	}

	Widget* UIManager::CreateWidget(const Type& type, const String& style /*= "standard"*/)
	{
		Widget* sample = GetWidgetStyle(type, style);
		if (!sample)
			sample = GetWidgetStyle(type, "standard");

		Widget* res = nullptr;

		if (sample)
			res = sample->CloneAs<Widget>();
		else
			res = (Widget*)type.CreateSample();

		if (type != TypeOf(UIContextMenu))
			res->SetEnableForcible(true);

		return res;
	}

	Widget* UIManager::GetWidgetStyle(const Type& type, const String& style)
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

	Button* UIManager::CreateButton(const WString& caption, const Function<void()>& onClick /*= Function<void()>()*/,
									  const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<Button>(style);
		res->caption = caption;
		res->name = caption + " button";
		res->onClick = onClick;
		return res;
	}

	Window* UIManager::CreateWindow(const WString& caption, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<Window>(style);
		res->caption = caption;
		res->name = caption + " window";
		return res;
	}

	Label* UIManager::CreateLabel(const WString& text, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<Label>(style);
		res->text = text;
		res->name = text + " label";
		res->horOverflow = Label::HorOverflow::Dots;
		return res;
	}

	HorizontalLayout* UIManager::CreateHorLayout()
	{
		auto res = mnew HorizontalLayout();
		res->name = "horizontal layout";
		*res->layout = WidgetLayout::BothStretch();
		return res;
	}

	VerticalLayout* UIManager::CreateVerLayout()
	{
		auto res = mnew VerticalLayout();
		res->name = "vertical layout";
		*res->layout = WidgetLayout::BothStretch();
		return res;
	}

	HorizontalProgress* UIManager::CreateHorProgress(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<HorizontalProgress>(style);
		res->name = "horizontal progress";
		return res;
	}

	VerticalProgress* UIManager::CreateVerProgress(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<VerticalProgress>(style);
		res->name = "vertical progress";
		return res;
	}

	HorizontalScrollBar* UIManager::CreateHorScrollBar(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<HorizontalScrollBar>(style);
		res->name = "horizontal scroll bar";
		return res;
	}

	VerticalScrollBar* UIManager::CreateVerScrollBar(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<VerticalScrollBar>(style);
		res->name = "vertical scroll bar";
		return res;
	}

	ScrollArea* UIManager::CreateScrollArea(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<ScrollArea>(style);
		res->name = "scroll area";
		return res;
	}

	EditBox* UIManager::CreateEditBox(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<EditBox>(style);
		res->name = "edit box";
		return res;
	}

	CustomList* UIManager::CreateCustomList(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<CustomList>(style);
		res->name = "custom list";
		return res;
	}

	List* UIManager::CreateList(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<List>(style);
		res->name = "list";
		return res;
	}

	CustomDropDown* UIManager::CreateCustomDropdown(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<CustomDropDown>(style);
		res->name = "custom dropdown";
		return res;
	}

	DropDown* UIManager::CreateDropdown(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<DropDown>(style);
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

	Image* UIManager::CreateImage(const String& name)
	{
		auto res = mnew Image();
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

	void UIManager::DrawWidgetAtTop(Widget* widget)
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