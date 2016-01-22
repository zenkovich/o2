#include "SceneWindow.h"

#include "SceneWindow/SceneEditWidget.h"
#include "UI/CustomDropDown.h"
#include "UI/UIManager.h"

SceneWindow::SceneWindow():
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
	if (auto iconLayer = mWindow->GetLayer("icon"))
	{
		*((Sprite*)iconLayer->drawable) = Sprite("ui/UI2_scene_icon.png");
		iconLayer->layout = Layout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-2, 2));
	}

	mWindow->SetViewLayout(Layout::BothStretch(0, 0, 0, 18));

	mEditWidget = mnew SceneEditWidget();
	mEditWidget->layout = UIWidgetLayout::BothStretch(0, 1, 1, 20);
	mWindow->AddChild(mEditWidget);

	UIWidget* upPanel = mnew UIWidget();
	upPanel->name = "up panel";
	upPanel->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 20, 0);
	upPanel->AddLayer("back", mnew Sprite("ui/UI2_small_panel_back.png"), Layout::BothStretch(-5, -4, -4, -5));
	mWindow->AddChild(upPanel);

	auto layersDropdown = o2UI.CreateWidget<UICustomDropDown>("panel layers");
	layersDropdown->layout = UIWidgetLayout::VerStretch(HorAlign::Right, -1, 1, 100, 1);
	upPanel->AddChild(layersDropdown);

	auto gizmosDropdown = o2UI.CreateWidget<UICustomDropDown>("panel gizmos");
	gizmosDropdown->layout = UIWidgetLayout::VerStretch(HorAlign::Right, -1, 1, 100, 100);
	upPanel->AddChild(gizmosDropdown);
}
