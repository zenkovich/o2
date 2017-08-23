#include "SceneWindow.h"

#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/CustomDropDown.h"
#include "UI/UIManager.h"

namespace Editor
{
	SceneWindow::SceneWindow():
		IEditorWindow()
	{
		InitializeWindow();
	}

	SceneWindow::SceneWindow(const SceneWindow& other):
		IEditorWindow(other)
	{
		InitializeWindow();
	}

	SceneWindow::~SceneWindow()
	{}

	void SceneWindow::InitializeWindow()
	{
		mWindow->caption = "Scene";
		mWindow->name = "scene window";
		mWindow->SetIcon(mnew Sprite("ui/UI2_scene_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-2, 2)));
		mWindow->SetViewLayout(Layout::BothStretch(-1, 0, 0, 18));

		mEditWidget = mnew SceneEditScreen();
		mEditWidget->layout = UIWidgetLayout::BothStretch(0, 0, 0, 19);
		mWindow->AddChild(mEditWidget);

		UIWidget* upPanel = mnew UIWidget();
		upPanel->name = "up panel";
		upPanel->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		upPanel->AddLayer("back", mnew Sprite("ui/UI2_small_panel_back.png"), Layout::BothStretch(-5, -4, -4, -5));
		mWindow->AddChild(upPanel);

		auto layersButton = o2UI.CreateWidget<UIButton>("panel down");
		layersButton->caption = "Layers";
		layersButton->layout = UIWidgetLayout::VerStretch(HorAlign::Right, -1, 1, 100, 0);
		upPanel->AddChild(layersButton);

		auto gizmosButton = o2UI.CreateWidget<UIButton>("panel down");
		gizmosButton->caption = "Gizmos";
		gizmosButton->layout = UIWidgetLayout::VerStretch(HorAlign::Right, -1, 1, 100, 100);
		upPanel->AddChild(gizmosButton);
	}

	void SceneWindow::InitializeLayersView()
	{

	}

	void SceneWindow::PostInitializeWindow()
	{
		o2EditorSceneScreen.BindActorsTree();
	}

}

CLASS_META(Editor::SceneWindow)
{
	BASE_CLASS(Editor::IEditorWindow);

	PROTECTED_FIELD(mEditWidget);
	PROTECTED_FIELD(mLayersView);
	PROTECTED_FIELD(mGizomsView);

	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, InitializeLayersView);
	PROTECTED_FUNCTION(void, PostInitializeWindow);
}
END_META;
