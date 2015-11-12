#include "UIManager.h"

#include "Application/BaseApplication.h"
#include "Assets/Assets.h"
#include "Render/Render.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/HorizontalProgress.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/Label.h"
#include "UI/ScrollArea.h"
#include "UI/VerticalLayout.h"
#include "UI/VerticalProgress.h"
#include "UI/VerticalScrollBar.h"
#include "UI/Widget.h"
#include "Utils/Debug.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	DECLARE_SINGLETON(UIManager);

	UIManager::UIManager()
	{
		mScreenWidget = mnew UIWidget();
		mLog = mnew LogStream("UI");
		o2Debug.GetLog()->BindStream(mLog);

		o2Application.onResizingEvent += ObjFunctionPtr<UIManager, void>(this, &UIManager::UpdateRootSize);

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

	Ptr<UIButton> UIManager::AddButton(const WString& caption, const String& style /*= "standard"*/)
	{
		Ptr<UIButton> res = CreateButton(caption, style);
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

	void UIManager::LoadStyle(const String& path)
	{
		DataNode styleData;
		styleData.LoadFromFile(o2Assets.GetDataPath() + path);

		for (auto st : mStyleSamples)
			st.Release();

		mStyleSamples = styleData;
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
		widget->SetName(style);
		mStyleSamples.Add(widget);
	}

	Ptr<UIButton> UIManager::CreateButton(const WString& caption, const String& style /*= "standard"*/)
	{
		auto btn = CreateWidget<UIButton>(style);
		btn->caption = caption;
		return btn;
	}

	Ptr<UILabel> UIManager::CreateLabel(const WString& text, const String& style /*= "standard"*/)
	{
		auto lbl = CreateWidget<UILabel>(style);
		lbl->text = text;
		return lbl;
	}

	Ptr<UIHorizontalLayout> UIManager::CreateHorLayout()
	{
		return mnew UIHorizontalLayout();
	}

	Ptr<UIVerticalLayout> UIManager::CreateVerLayout()
	{
		return mnew UIVerticalLayout();
	}

	Ptr<UIHorizontalProgress> UIManager::CreateHorProgress(const String& style /*= "standard"*/)
	{
		return CreateWidget<UIHorizontalProgress>(style);
	}

	Ptr<UIVerticalProgress> UIManager::CreateVerProgress(const String& style /*= "standard"*/)
	{
		return CreateWidget<UIVerticalProgress>(style);
	}

	Ptr<UIHorizontalScrollBar> UIManager::CreateHorScrollBar(const String& style /*= "standard"*/)
	{
		return CreateWidget<UIHorizontalScrollBar>(style);
	}

	Ptr<UIVerticalScrollBar> UIManager::CreateVerScrollBar(const String& style /*= "standard"*/)
	{
		return CreateWidget<UIVerticalScrollBar>(style);
	}

	Ptr<UIScrollArea> UIManager::CreateScrollArea(const String& style /*= "standard"*/)
	{
		return CreateWidget<UIScrollArea>(style);
	}

	Ptr<UIEditBox> UIManager::CreateEditBox(const String& style /*= "standard"*/)
	{
		return CreateWidget<UIEditBox>(style);
	}

	void UIManager::Update(float dt)
	{
		mScreenWidget->Update(dt);
	}

	void UIManager::Draw()
	{
		mScreenWidget->Draw();
	}

	void UIManager::UpdateRootSize()
	{
		mScreenWidget->layout.size = o2Render.GetResolution();
	}

	void UIManager::InitializeProperties()
	{
		INITIALIZE_ACCESSOR(UIManager, widget, GetWidget);
	}
}