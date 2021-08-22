#include "o2/stdafx.h"
#include "UIManager.h"

#include "o2/Application/Application.h"
#include "o2/Assets/Assets.h"
#include "o2/Assets/Types/FolderAsset.h"
#include "o2/Render/Render.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Scene/UI/Widgets/CustomDropDown.h"
#include "o2/Scene/UI/Widgets/CustomList.h"
#include "o2/Scene/UI/Widgets/DropDown.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/Widgets/HorizontalProgress.h"
#include "o2/Scene/UI/Widgets/HorizontalScrollBar.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/List.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"
#include "o2/Scene/UI/Widgets/Toggle.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Scene/UI/Widgets/VerticalProgress.h"
#include "o2/Scene/UI/Widgets/VerticalScrollBar.h"
#include "o2/Scene/UI/Widgets/Window.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Utils/StringUtils.h"
#include "o2/Utils/System/Time/Timer.h"

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

			auto focusedIdx = mLastFocusedWidgets.IndexOf(lastFocusedWidget);
			if (focusedIdx >= 0)
				mLastFocusedWidgets.RemoveAt(focusedIdx);
			else
				mLastUnfocusedWidgets.Add(lastFocusedWidget);

			if (lastFocusedWidget->mFocusedState)
				lastFocusedWidget->mFocusedState->SetState(false);
		}

		mFocusedWidget = widget;

		if (mFocusedWidget)
		{
			mFocusedWidget->mIsFocused = true;

			auto unfocusedIdx = mLastUnfocusedWidgets.IndexOf(mFocusedWidget);
			if (unfocusedIdx >= 0)
				mLastUnfocusedWidgets.RemoveAt(unfocusedIdx);
			else
				mLastFocusedWidgets.Add(mFocusedWidget);

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

	void UIManager::LoadStyle(const String& stylesPath)
	{
		ClearStyle();

		FolderAssetRef folder(stylesPath);
		for (auto& subAsset : folder->GetChildrenAssets())
		{
			if (auto& actorAsset = subAsset.Cast<ActorAsset>())
				mStyleSamples.Add(actorAsset);
		}
	}

	void UIManager::SaveStyle(const String& stylesPath)
	{
		for (auto& asset : mStyleSamples)
		{
			asset->SetEditorAsset(true);
			auto path = stylesPath + "/" + GetSmartName(asset->GetActor()->GetType().GetName()) + " " + 
				asset->GetActor()->GetName() + ".proto";

			if (path.Contains("Context Menu standard"))
				o2Debug.Log("asd");

			asset->Save(path, false);
		}

		o2Assets.RebuildAssets();
	}

	void UIManager::ClearStyle()
	{
		mStyleSamples.Clear();
	}

	void UIManager::AddWidgetStyle(Widget* widget, const String& style)
	{
		widget->Hide(true);
		widget->SetName(style);
		mStyleSamples.Add(mnew ActorAsset(widget));
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

		if (type != TypeOf(ContextMenu))
			res->SetEnabledForcible(true);

		return res;
	}

	Widget* UIManager::GetWidgetStyle(const Type& type, const String& style)
	{
		for (auto styleWidget : mStyleSamples)
		{
			if (type == styleWidget->GetActor()->GetType())
			{
				if (style == styleWidget->GetActor()->GetName())
					return dynamic_cast<Widget*>(styleWidget->GetActor());
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

	Toggle* UIManager::CreateToggle(const WString& caption, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<Toggle>(style);
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

		if (PopupWidget::mVisiblePopup)
			PopupWidget::mVisiblePopup->SpecialDraw();

		if (o2Input.IsKeyPressed(VK_TAB))
			FocusNextWidget();
	}

	void UIManager::Update()
	{
		for (auto widget : mLastFocusedWidgets)
			widget->OnFocused();

		for (auto widget : mLastUnfocusedWidgets)
			widget->OnUnfocused();

		mLastFocusedWidgets.Clear();
		mLastUnfocusedWidgets.Clear();
	}

	void UIManager::DrawWidgetAtTop(Widget* widget)
	{
		mTopWidgets.Add(widget);
	}

	const Vector<ActorAssetRef>& UIManager::GetWidgetStyles() const
	{
		return mStyleSamples;
	}

	void UIManager::TryLoadStyle()
	{
		if (o2Assets.IsAssetExist("ui_style.json"))
			LoadStyle("ui_style.json");
	}
}
