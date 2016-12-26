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
#include "UI/Window.h"
#include "Utils/Debug.h"
#include "Utils/Log/LogStream.h"

#undef CreateWindow

namespace o2
{
	DECLARE_SINGLETON(UIManager);

	UIManager::UIManager():
		mSelectedWidget(nullptr)
	{
		mScreenWidget = mnew UIWidget();
		mLog = mnew LogStream("UI");
		o2Debug.GetLog()->BindStream(mLog);

		o2Application.onResizingEvent += Function<void()>(this, &UIManager::UpdateRootSize);

		InitializeProperties();
	}

	UIManager::~UIManager()
	{
		delete mScreenWidget;
		ClearStyle();
	}

	UIWidget* UIManager::AddWidget(UIWidget* widget)
	{
		return mScreenWidget->AddChild(widget);
	}

	UIWidget* UIManager::AddWidget()
	{
		UIWidget* res = mnew UIWidget();
		AddWidget(res);
		return res;
	}

	UIButton* UIManager::AddButton(const WString& caption, const Function<void()>& onClick /*= Function<void()>()*/,
								   const String& style /*= "standard"*/)
	{
		UIButton* res = CreateButton(caption, onClick, style);
		AddWidget(res);
		return res;
	}

	UIWindow* UIManager::AddWindow(const WString& caption, const String& style /*= "standard"*/)
	{
		UIWindow* res = CreateWindow(caption, style);
		AddWidget(res);
		return res;
	}

	UILabel* UIManager::AddLabel(const WString& text, const String& style /*= "standard"*/)
	{
		UILabel* res = CreateLabel(text, style);
		AddWidget(res);
		return res;
	}

	UIHorizontalLayout* UIManager::AddHorLayout()
	{
		UIHorizontalLayout* res = mnew UIHorizontalLayout();
		AddWidget(res);
		return res;
	}

	UIVerticalLayout* UIManager::AddVerLayout()
	{
		UIVerticalLayout* res = mnew UIVerticalLayout();
		AddWidget(res);
		return res;
	}

	UIHorizontalProgress* UIManager::AddHorProgress(const String& style /*= "standard"*/)
	{
		UIHorizontalProgress* res = CreateHorProgress(style);
		AddWidget(res);
		return res;
	}

	UIVerticalProgress* UIManager::AddVerProgress(const String& style /*= "standard"*/)
	{
		UIVerticalProgress* res = CreateVerProgress(style);
		AddWidget(res);
		return res;
	}

	UIHorizontalScrollBar* UIManager::AddHorScrollBar(const String& style /*= "standard"*/)
	{
		UIHorizontalScrollBar* res = CreateHorScrollBar(style);
		AddWidget(res);
		return res;
	}

	UIVerticalScrollBar* UIManager::AddVerScrollBar(const String& style /*= "standard"*/)
	{
		UIVerticalScrollBar* res = CreateVerScrollBar(style);
		AddWidget(res);
		return res;
	}

	UIScrollArea* UIManager::AddScrollArea(const String& style /*= "standard"*/)
	{
		UIScrollArea* res = CreateScrollArea(style);
		AddWidget(res);
		return res;
	}

	UIEditBox* UIManager::AddEditBox(const String& style /*= "standard"*/)
	{
		UIEditBox* res = CreateEditBox(style);
		AddWidget(res);
		return res;
	}

	UICustomList* UIManager::AddCustomList(const String& style /*= "standard"*/)
	{
		UICustomList* res = CreateCustomList(style);
		AddWidget(res);
		return res;
	}

	UIList* UIManager::AddList(const String& style /*= "standard"*/)
	{
		UIList* res = CreateList(style);
		AddWidget(res);
		return res;
	}

	UICustomDropDown* UIManager::AddCustomDropdown(const String& style /*= "standard"*/)
	{
		UICustomDropDown* res = CreateCustomDropdown(style);
		AddWidget(res);
		return res;
	}

	UIDropDown* UIManager::AddDropdown(const String& style /*= "standard"*/)
	{
		UIDropDown* res = CreateDropdown(style);
		AddWidget(res);
		return res;
	}

	UIToggle* UIManager::AddToggle(const WString& caption, const String& style /*= "standard"*/)
	{
		UIToggle* res = CreateToggle(caption, style);
		AddWidget(res);
		return res;
	}

	UIImage* UIManager::AddImage(const String& name)
	{
		UIImage* res = CreateImage(name);
		AddWidget(res);
		return res;
	}

	bool UIManager::RemoveWidget(const String& path)
	{
		return mScreenWidget->RemoveChild(path);
	}

	bool UIManager::RemoveWidget(UIWidget* widget, bool release /*= true*/)
	{
		return mScreenWidget->RemoveChild(widget, release);
	}

	UIWidget* UIManager::GetWidget(const String& path)
	{
		return mScreenWidget->GetChild(path);
	}

	void UIManager::RemoveAllWidgets()
	{
		mScreenWidget->RemoveAllChilds();
	}

	const UIManager::WidgetsVec& UIManager::GetAllWidgets() const
	{
		return mScreenWidget->GetChilds();
	}

	UIWidget* UIManager::GetScreenWidget() const
	{
		return mScreenWidget;
	}

	void UIManager::FocusWidget(UIWidget* widget)
	{
		if (widget == mSelectedWidget || (widget && !widget->IsFocusable()))
			return;

		if (mSelectedWidget)
		{
			mSelectedWidget->mIsFocused = false;
			mSelectedWidget->OnUnfocused();

			if (mSelectedWidget->mFocusedState)
				mSelectedWidget->mFocusedState->SetState(false);
		}

		mSelectedWidget = widget;

		if (mSelectedWidget)
		{
			mSelectedWidget->mIsFocused = true;

			mSelectedWidget->OnFocused();

			if (mSelectedWidget->mParent)
				mSelectedWidget->mParent->OnChildFocused(mSelectedWidget);

			if (mSelectedWidget->mFocusedState)
				mSelectedWidget->mFocusedState->SetState(true);
		}
	}

	UIWidget* UIManager::GetFocusedWidget() const
	{
		return mSelectedWidget;
	}

	void UIManager::FocusNextWidget()
	{
		bool fnd = mSelectedWidget == nullptr;
		FocusWidget(SearchSelectableWidget(mScreenWidget, fnd));
	}

	void UIManager::LoadStyle(const String& path)
	{
		DataNode styleData;
		styleData.LoadFromFile(o2Assets.GetDataPath() + path);

		for (auto st : mStyleSamples)
			delete st;

		mStyleSamples = styleData;

		for (auto styleSample : mStyleSamples)
			styleSample->Hide(true);
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

	UIButton* UIManager::CreateButton(const WString& caption, const Function<void()>& onClick /*= Function<void()>()*/,
									  const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIButton>(style);
		res->caption = caption;
		res->name = "button";
		res->onClick = onClick;
		return res;
	}

	UIWindow* UIManager::CreateWindow(const WString& caption, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIWindow>(style);
		res->caption = caption;
		res->name = "window";
		return res;
	}

	UILabel* UIManager::CreateLabel(const WString& text, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UILabel>(style);
		res->text = text;
		res->name = "label";
		res->horOverflow = UILabel::HorOverflow::Dots;
		return res;
	}

	UIHorizontalLayout* UIManager::CreateHorLayout()
	{
		auto res = mnew UIHorizontalLayout();
		res->name = "hor layout";
		return res;
	}

	UIVerticalLayout* UIManager::CreateVerLayout()
	{
		auto res = mnew UIVerticalLayout();
		res->name = "ver layout";
		return res;
	}

	UIHorizontalProgress* UIManager::CreateHorProgress(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIHorizontalProgress>(style);
		res->name = "hor progress";
		return res;
	}

	UIVerticalProgress* UIManager::CreateVerProgress(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIVerticalProgress>(style);
		res->name = "ver progress";
		return res;
	}

	UIHorizontalScrollBar* UIManager::CreateHorScrollBar(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIHorizontalScrollBar>(style);
		res->name = "hot scroll bar";
		return res;
	}

	UIVerticalScrollBar* UIManager::CreateVerScrollBar(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIVerticalScrollBar>(style);
		res->name = "ver scroll bar";
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
		res->name = "toggle";
		return res;
	}

	UIImage* UIManager::CreateImage(const String& name)
	{
		auto res = mnew UIImage();
		res->name = name;
		res->SetImageName(name);
		return res;
	}

	void UIManager::Update(float dt)
	{
		mScreenWidget->Update(dt);

		if (o2Input.IsKeyPressed(VK_TAB))
			FocusNextWidget();
	}

	void UIManager::Draw()
	{
		mScreenWidget->Draw();

		for (auto widget : mTopWidgets)
			widget->Draw();

		mTopWidgets.Clear();

		if (UIContextMenu::mVisibleContextMenu)
			UIContextMenu::mVisibleContextMenu->SpecialDraw();
	}

	void UIManager::UpdateRootSize()
	{
		Vec2I resolution = o2Render.GetResolution();
		mScreenWidget->layout = UIWidgetLayout::Based(BaseCorner::Center, resolution);
	}

	void UIManager::DrawWidgetAtTop(UIWidget* widget)
	{
		mTopWidgets.Add(widget);
	}

	void UIManager::TryLoadStyle()
	{
		if (o2Assets.IsAssetExist("ui_style.xml"))
			LoadStyle("ui_style.xml");
	}

	UIWidget* UIManager::SearchSelectableWidget(UIWidget* widget, bool& foundCurrentSelected)
	{
		for (auto child : widget->mChilds)
		{
			if (!foundCurrentSelected)
			{
				if (child == mSelectedWidget)
					foundCurrentSelected = true;
			}
			else
			{
				if (child->IsFocusable())
					return child;
			}
		}

		for (auto child : widget->mChilds)
		{
			auto res = SearchSelectableWidget(child, foundCurrentSelected);
			if (res)
				return res;
		}

		return nullptr;
	}

	void UIManager::InitializeProperties()
	{
		INITIALIZE_ACCESSOR(UIManager, widget, GetWidget);
	}
}