#include "o2Editor/stdafx.h"
#include "GameWindow.h"

#include "o2/Scene/CameraActor.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/Core/EditorApplication.h"
#include "../Core/Properties/Basic/Vector2IntProperty.h"
#include "o2/Scene/UI/Widgets/Button.h"

namespace Editor
{
	GameWindow::GameWindow():
		IEditorWindow()
	{
		InitializeWindow();
	}

	GameWindow::GameWindow(const GameWindow& other):
		IEditorWindow(other)
	{
		InitializeWindow();
	}

	GameWindow::~GameWindow()
	{
	}

	void GameWindow::InitializeWindow()
	{
		mWindow->caption = "Game";
		mWindow->name = "game window";
		mWindow->SetIcon(mnew Sprite("ui/UI4_game_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-1, 2)));
		mWindow->SetViewLayout(Layout::BothStretch(-1, 0, 0, 18));

		mGameView = mnew GameView();
		*mGameView->GetLayout() = WidgetLayout::BothStretch(0, 0, 0, 19);
		mWindow->AddChild(mGameView);

		InitializeDevicesMenu();

		HorizontalLayout* upPanel = mnew HorizontalLayout();
		upPanel->name = "up panel";
		upPanel->baseCorner = BaseCorner::Right;
		upPanel->expandWidth = false;
		upPanel->spacing = 5;
		*upPanel->GetLayout() = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		upPanel->AddLayer("back", mnew Sprite("ui/UI4_small_panel_back.png"), Layout::BothStretch(-5, -4, -4, -5));
		mWindow->AddChild(upPanel);

		mResolutionsButton = o2UI.CreateWidget<Button>("panel down");
		mResolutionsButton->caption = "Resolution";
		mResolutionsButton->GetLayout()->minWidth = 300;
		mResolutionsButton->onClick = [=]() { mDevicesMenu->Show(mResolutionsButton->GetLayout()->GetWorldLeftBottom()); };
		upPanel->AddChild(mResolutionsButton);

		auto resolutionLabel = o2UI.CreateLabel("Resolution:");
		resolutionLabel->horOverflow = Label::HorOverflow::Expand;
		upPanel->AddChild(resolutionLabel);

		OnCurrentWindowSize(true);
	}

	void GameWindow::InitializeDevicesMenu()
	{
		DataDocument doc;
		doc.LoadFromFile(GetEditorAssetsPath() + String("SimulationDevicesList.json"));

		Vector<SimulationDevice> devicesList;
		devicesList = doc["list"];

		for (auto& device: devicesList)
			mDevicesList[device.deviceName] = device;

		mDevicesMenu = o2UI.CreateWidget<ContextMenu>();

		// 		mCurrentWindowSizeItem = mDevicesMenu->AddToggleItem("Current window size", true, THIS_FUNC(OnCurrentWindowSize));
		// 
		// 		mCustomSizeItem = mDevicesMenu->AddToggleItem("Custom resolution", false, THIS_FUNC(OnCustomResolution));
		// 		mCustomSizeProperty = o2UI.CreateWidget<Vec2IProperty>();
		// 		mCustomSizeProperty->SetValue(Vec2I(800, 600));
		// 		*mCustomSizeProperty->GetLayout() = WidgetLayout::Based(BaseCorner::Right, Vec2F(150, 20));
		// 		mCustomSizeProperty->onChangeCompleted = [&](const String&, const Vector<DataDocument>&, const Vector<DataDocument>&) { OnCustomResolution(true); };
		// 		mCustomSizeItem->AddChild(mCustomSizeProperty);
		// 
		// 		mDevicesMenu->AddItem("---");
		// 
		// 		for (auto kv : mDevicesList)
		// 		{
		// 			String caption = kv.first + " (" + (String)kv.second.resolution.x + " x " + (String)kv.second.resolution.y + ")";
		// 			auto item = mDevicesMenu->AddToggleItem(caption, false, [](bool) {});
		// 			item->onChecked = [=](bool checked) { OnDeviceSelected(kv.first, item); };
		// 		}

		mWindow->AddChild(mDevicesMenu);
	}

	void GameWindow::SetResolution(const Vec2I& resolution)
	{
		mGameView->fixedResolution = true;
		mGameView->resolution = resolution;
	}

	void GameWindow::OnCurrentWindowSize(bool enabled)
	{
		SetDeviceMenuCheckedItem(mCurrentWindowSizeItem);
		mResolutionsButton->caption = "Current window size";
		mGameView->fixedResolution = false;
	}

	void GameWindow::OnCustomResolution(bool enabled)
	{
		SetDeviceMenuCheckedItem(mCustomSizeItem);
		mResolutionsButton->caption = "Fixed " + (String)mCustomSizeProperty->GetCommonValue();
		SetResolution(mCustomSizeProperty->GetCommonValue());
	}

	void GameWindow::OnDeviceSelected(const String& name, const ContextMenuItem* item)
	{
		SetDeviceMenuCheckedItem(item);
		mResolutionsButton->caption = item->text;
		SetResolution(mDevicesList[name].resolution);
	}

	void GameWindow::SetDeviceMenuCheckedItem(const ContextMenuItem* item)
	{
		for (auto& child : mDevicesMenu->GetItemsLayout()->GetChildren())
		{
			if (auto childItem = dynamic_cast<ContextMenuItem*>(child))
			{
				if (childItem->IsCheckable())
					childItem->SetChecked(childItem == item);
			}
		}
	}

	GameWindow::GameView::GameView()
	{
		mRenderTarget = mmake<Texture>(Vec2I(256, 256), TextureFormat::R8G8B8A8, Texture::Usage::RenderTarget);
		mRenderTargetSprite = mnew Sprite(mRenderTarget, RectI(0, 0, 256, 256));
	}

	void GameWindow::GameView::Draw()
	{
		Widget::Draw();

		o2Render.BindRenderTexture(mRenderTarget);

		int editorDepth = EditorScope::GetDepth();
		EditorScope::Exit(editorDepth);

		o2Scene.Draw();

		if (!o2Scene.GetCameras().IsEmpty())
		{
			Ref<CameraActor> prevCamera = o2Render.GetCamera();
			Ref<CameraActor> cameraActor = o2Scene.GetCameras()[0];
			cameraActor->#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;
using RenderTexturePtr = Ref<Texture>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename Derived, typename Base>
inline Ref<Derived> DynamicCast(const Ref<Base>& ptr) {
    return std::dynamic_pointer_cast<Derived>(ptr);
}

class GameWindow {
public:
    class GameView {
    public:
        String GetCreateMenuCategory();

        void OnTransformUpdated();

    private:
        Ref<Texture> mRenderTarget;
        Ref<Sprite> mRenderTargetSprite = mmake<Sprite>();
    };

    class SimulationDevice {
    public:
        bool operator==(const SimulationDevice& other) const;

    private:
        String deviceName;
    };

private:
    void Setup();

private:
    Ref<GameView> cameraActor;
    WeakRef<GameView> prevCamera;
};

void GameWindow::Setup() {
    // ...
}

void GameWindow::GameView::OnTransformUpdated() {
    // ...
}

String GameWindow::GameView::GetCreateMenuCategory() {
    return "UI/Editor";
}

bool GameWindow::SimulationDevice::operator==(const SimulationDevice& other) const {
    return deviceName == other.deviceName;
}

// --- META ---

DECLARE_C