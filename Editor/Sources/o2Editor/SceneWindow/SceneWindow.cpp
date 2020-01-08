#include "o2Editor/stdafx.h"
#include "SceneWindow.h"

#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/CustomDropDown.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"

namespace Editor
{
	SceneWindow::SceneWindow() :
		IEditorWindow()
	{
		InitializeWindow();
	}

	SceneWindow::SceneWindow(const SceneWindow& other) :
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
		mWindow->SetIcon(mnew Sprite("ui/UI4_scene_icon.png"));
		mWindow->SetIconLayout(Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-2, 2)));
		mWindow->SetViewLayout(Layout::BothStretch(-1, 0, 0, 18));

		mEditWidget = mnew SceneEditScreen();
		*mEditWidget->layout = WidgetLayout::BothStretch(0, 0, 0, 19);
		mWindow->AddChild(mEditWidget);

		Widget* upPanel = mnew Widget();
		upPanel->name = "up panel";
		*upPanel->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
		upPanel->AddLayer("back", mnew Sprite("ui/UI4_small_panel_back.png"), Layout::BothStretch(-5, -4, -4, -5));
		mWindow->AddChild(upPanel);

		auto layersButton = o2UI.CreateWidget<Button>("panel down");
		layersButton->caption = "Layers";
		*layersButton->layout = WidgetLayout::VerStretch(HorAlign::Right, 0, 0, 100, 0);
		upPanel->AddChild(layersButton);

		auto gizmosButton = o2UI.CreateWidget<Button>("panel down");
		gizmosButton->caption = "Gizmos";
		*gizmosButton->layout = WidgetLayout::VerStretch(HorAlign::Right, 0, 0, 100, 100);
		upPanel->AddChild(gizmosButton);
	}

	void SceneWindow::InitializeLayersView()
	{}

	void SceneWindow::PostInitializeWindow()
	{
		o2EditorSceneScreen.BindSceneTree();
	}

}

DECLARE_CLASS(Editor::SceneWindow);
