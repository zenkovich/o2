#include "UIManager.h"

#include "Application/BaseApplication.h"
#include "Assets/Assets.h"
#include "Render/Render.h"
#include "UI/Button.h"
#include "UI/Label.h"
#include "UI/Widget.h"
#include "Utils/Debug.h"
#include "Utils/Log/LogStream.h"

namespace o2
{
	DECLARE_SINGLETON(UIManager);

	UIManager::UIManager()
	{
		mScreenWidget = mnew Widget();
		mLog = mnew LogStream("UI");
		o2Debug.GetLog()->BindStream(mLog);

		o2Application.onResizingEvent += ObjFunctionPtr<UIManager, void>(this, &UIManager::UpdateRootSize);

		InitializeProperties();
	}

	UIManager::~UIManager()
	{
		mScreenWidget.Release();
	}

	Ptr<Widget> UIManager::AddWidget(Ptr<Widget> widget)
	{
		return mScreenWidget->AddChild(widget);
	}

	Ptr<Widget> UIManager::AddWidget()
	{
		auto res = mnew Widget();
		AddWidget(res);
		return res;
	}

	Ptr<Button> UIManager::AddButton(const String& caption, const String& style /*= "standard"*/)
	{
		auto btn = CreateButton(caption, style);
		AddWidget(btn);
		return btn;
	}

	Ptr<Label> UIManager::AddLabel(const String& text, const String& style /*= "standard"*/)
	{
		auto lbl = CreateLabel(text, style);
		AddWidget(lbl);
		return lbl;
	}

	bool UIManager::RemoveWidget(const String& path)
	{
		return mScreenWidget->RemoveChild(path);
	}

	bool UIManager::RemoveWidget(Ptr<Widget> widget, bool release /*= true*/)
	{
		return mScreenWidget->RemoveChild(widget, release);
	}

	Ptr<Widget> UIManager::GetWidget(const String& path)
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

	void UIManager::AddStyle(Ptr<Widget> widget, const String& style)
	{
		widget->SetName(style);
		mStyleSamples.Add(widget);
	}

	Ptr<Widget> UIManager::GetWidgetStyle(const String& style)
	{
		for (auto styleWidget : mStyleSamples)
			if (styleWidget->GetName() == style)
				return styleWidget;

		return nullptr;
	}

	Ptr<Button> UIManager::CreateButton(const String& caption, const String& style /*= "standard"*/)
	{
		auto btn = CreateWidget<Button>(style);
		btn->captionc = caption;
		return btn;
	}

	Ptr<Label> UIManager::CreateLabel(const String& text, const String& style /*= "standard"*/)
	{
		auto lbl = CreateWidget<Label>(style);
		lbl->text = text;
		return lbl;
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