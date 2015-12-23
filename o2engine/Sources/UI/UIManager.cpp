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

	UIManager::UIManager()
	{
		mScreenWidget = mnew UIWidget();
		mLog = mnew LogStream("UI");
		o2Debug.GetLog()->BindStream(mLog);

		o2Application.onResizingEvent += Function<void()>(this, &UIManager::UpdateRootSize);

		InitializeProperties();
	}

	UIManager::~UIManager()
	{
		mScreenWidget.Release();
		ClearStyle();
	}

	Ptr<UIWidget> UIManager::AddWidget(Ptr<UIWidget> widget)
	{
		return mScreenWidget->AddChild(widget);
	}

	Ptr<UIWidget> UIManager::AddWidget()
	{
		Ptr<UIWidget> res = mnew UIWidget();
		AddWidget(res);
		return res;
	}

	Ptr<UIButton> UIManager::AddButton(const WString& caption, const Function<void()>& onClick /*= Function<void()>()*/, 
									   const String& style /*= "standard"*/)
	{
		Ptr<UIButton> res = CreateButton(caption, onClick, style);
		AddWidget(res);
		return res;
	}

	Ptr<UIWindow> UIManager::AddWindow(const WString& caption, const String& style /*= "standard"*/)
	{
		Ptr<UIWindow> res = CreateWindow(caption, style);
		AddWidget(res);
		return res;
	}

	Ptr<UILabel> UIManager::AddLabel(const WString& text, const String& style /*= "standard"*/)
	{
		Ptr<UILabel> res = CreateLabel(text, style);
		AddWidget(res);
		return res;
	}

	Ptr<UIHorizontalLayout> UIManager::AddHorLayout()
	{
		Ptr<UIHorizontalLayout> res = mnew UIHorizontalLayout();
		AddWidget(res);
		return res;
	}

	Ptr<UIVerticalLayout> UIManager::AddVerLayout()
	{
		Ptr<UIVerticalLayout> res = mnew UIVerticalLayout();
		AddWidget(res);
		return res;
	}

	Ptr<UIHorizontalProgress> UIManager::AddHorProgress(const String& style /*= "standard"*/)
	{
		Ptr<UIHorizontalProgress> res = CreateHorProgress(style);
		AddWidget(res);
		return res;
	}

	Ptr<UIVerticalProgress> UIManager::AddVerProgress(const String& style /*= "standard"*/)
	{
		Ptr<UIVerticalProgress> res = CreateVerProgress(style);
		AddWidget(res);
		return res;
	}

	Ptr<UIHorizontalScrollBar> UIManager::AddHorScrollBar(const String& style /*= "standard"*/)
	{
		Ptr<UIHorizontalScrollBar> res = CreateHorScrollBar(style);
		AddWidget(res);
		return res;
	}

	Ptr<UIVerticalScrollBar> UIManager::AddVerScrollBar(const String& style /*= "standard"*/)
	{
		Ptr<UIVerticalScrollBar> res = CreateVerScrollBar(style);
		AddWidget(res);
		return res;
	}

	Ptr<UIScrollArea> UIManager::AddScrollArea(const String& style /*= "standard"*/)
	{
		Ptr<UIScrollArea> res = CreateScrollArea(style);
		AddWidget(res);
		return res;
	}

	Ptr<UIEditBox> UIManager::AddEditBox(const String& style /*= "standard"*/)
	{
		Ptr<UIEditBox> res = CreateEditBox(style);
		AddWidget(res);
		return res;
	}

	Ptr<UICustomList> UIManager::AddCustomList(const String& style /*= "standard"*/)
	{
		Ptr<UICustomList> res = CreateCustomList(style);
		AddWidget(res);
		return res;
	}

	Ptr<UIList> UIManager::AddList(const String& style /*= "standard"*/)
	{
		Ptr<UIList> res = CreateList(style);
		AddWidget(res);
		return res;
	}

	Ptr<UICustomDropDown> UIManager::AddCustomDropdown(const String& style /*= "standard"*/)
	{
		Ptr<UICustomDropDown> res = CreateCustomDropdown(style);
		AddWidget(res);
		return res;
	}

	Ptr<UIDropDown> UIManager::AddDropdown(const String& style /*= "standard"*/)
	{
		Ptr<UIDropDown> res = CreateDropdown(style);
		AddWidget(res);
		return res;
	}

	Ptr<UIToggle> UIManager::AddToggle(const WString& caption, const String& style /*= "standard"*/)
	{
		Ptr<UIToggle> res = CreateToggle(caption, style);
		AddWidget(res);
		return res;
	}

	bool UIManager::RemoveWidget(const String& path)
	{
		return mScreenWidget->RemoveChild(path);
	}

	bool UIManager::RemoveWidget(Ptr<UIWidget> widget, bool release /*= true*/)
	{
		return mScreenWidget->RemoveChild(widget, release);
	}

	Ptr<UIWidget> UIManager::GetWidget(const String& path)
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

	Ptr<UIWidget> UIManager::GetScreenWidget() const
	{
		return mScreenWidget;
	}

	void UIManager::SelectWidget(Ptr<UIWidget> widget)
	{
		if (widget == mSelectedWidget || (widget && !widget->IsSelectable()))
			return;

		if (mSelectedWidget)
		{
			mSelectedWidget->mIsSelected = false;
			mSelectedWidget->OnDeselected();

			if (mSelectedWidget->mSelectedState)
				mSelectedWidget->mSelectedState->SetState(false);
		}

		mSelectedWidget = widget;

		if (mSelectedWidget)
		{
			mSelectedWidget->mIsSelected = true;

			mSelectedWidget->OnSelected();

			if (mSelectedWidget->mParent)
				mSelectedWidget->mParent->OnChildSelected(mSelectedWidget);

			if (mSelectedWidget->mSelectedState)
				mSelectedWidget->mSelectedState->SetState(true);
		}
	}

	Ptr<UIWidget> UIManager::GetSelectedWidget() const
	{
		return mSelectedWidget;
	}

	void UIManager::SelectNextWidget()
	{
		bool fnd = mSelectedWidget == nullptr;
		SelectWidget(SearchSelectableWidget(mScreenWidget, fnd));
	}

	void UIManager::LoadStyle(const String& path)
	{
		DataNode styleData;
		styleData.LoadFromFile(o2Assets.GetDataPath() + path);

		for (auto st : mStyleSamples)
			st.Release();

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
			sample.Release();

		mStyleSamples.Clear();
	}

	void UIManager::AddWidgetStyle(Ptr<UIWidget> widget, const String& style)
	{
		widget->Hide(true);
		widget->SetName(style);
		mStyleSamples.Add(widget);
	}

	Ptr<UIButton> UIManager::CreateButton(const WString& caption, const Function<void()>& onClick /*= Function<void()>()*/, 
										  const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIButton>(style);
		res->caption = caption;
		res->name = "button";
		res->onClick = onClick;
		return res;
	}

	Ptr<UIWindow> UIManager::CreateWindow(const WString& caption, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIWindow>(style);
		res->caption = caption;
		res->name = "window";
		return res;
	}

	Ptr<UILabel> UIManager::CreateLabel(const WString& text, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UILabel>(style);
		res->text = text;
		res->name = "label";
		res->horOverflow = UILabel::HorOverflow::Dots;
		return res;
	}

	Ptr<UIHorizontalLayout> UIManager::CreateHorLayout()
	{
		auto res = mnew UIHorizontalLayout();
		res->name = "hor layout";
		return res;
	}

	Ptr<UIVerticalLayout> UIManager::CreateVerLayout()
	{
		auto res = mnew UIVerticalLayout();
		res->name = "ver layout";
		return res;
	}

	Ptr<UIHorizontalProgress> UIManager::CreateHorProgress(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIHorizontalProgress>(style);
		res->name = "hor progress";
		return res;
	}

	Ptr<UIVerticalProgress> UIManager::CreateVerProgress(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIVerticalProgress>(style);
		res->name = "ver progress";
		return res;
	}

	Ptr<UIHorizontalScrollBar> UIManager::CreateHorScrollBar(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIHorizontalScrollBar>(style);
		res->name = "hot scroll bar";
		return res;
	}

	Ptr<UIVerticalScrollBar> UIManager::CreateVerScrollBar(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIVerticalScrollBar>(style);
		res->name = "ver scroll bar";
		return res;
	}

	Ptr<UIScrollArea> UIManager::CreateScrollArea(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIScrollArea>(style);
		res->name = "scroll area";
		return res;
	}

	Ptr<UIEditBox> UIManager::CreateEditBox(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIEditBox>(style);
		res->name = "edit box";
		return res;
	}

	Ptr<UICustomList> UIManager::CreateCustomList(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UICustomList>(style);
		res->name = "custom list";
		return res;
	}

	Ptr<UIList> UIManager::CreateList(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIList>(style);
		res->name = "list";
		return res;
	}

	Ptr<UICustomDropDown> UIManager::CreateCustomDropdown(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UICustomDropDown>(style);
		res->name = "custom dropdown";
		return res;
	}

	Ptr<UIDropDown> UIManager::CreateDropdown(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIDropDown>(style);
		res->name = "dropdown";
		return res;
	}

	Ptr<UIToggle> UIManager::CreateToggle(const WString& caption, const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<UIToggle>(style);
		res->caption = caption;
		res->name = "toggle";
		return res;
	}

	void UIManager::Update(float dt)
	{
		mScreenWidget->Update(dt);

		if (o2Input.IsKeyPressed(VK_TAB))
			SelectNextWidget();
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
		mScreenWidget->layout.size = o2Render.GetResolution();
	}

	void UIManager::RegTopWidget(Ptr<UIWidget> widget)
	{
		mTopWidgets.Add(widget);
	}

	void UIManager::TryLoadStyle()
	{
		if (o2Assets.IsAssetExist("ui_style.xml"))
			LoadStyle("ui_style.xml");
	}

	Ptr<UIWidget> UIManager::SearchSelectableWidget(Ptr<UIWidget> widget, bool& foundCurrentSelected)
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
				if (child->IsSelectable())
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