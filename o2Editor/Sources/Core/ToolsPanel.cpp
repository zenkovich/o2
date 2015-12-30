#include "ToolsPanel.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "UI/Button.h"
#include "UI/DropDown.h"
#include "UI/HorizontalLayout.h"
#include "UI/Toggle.h"
#include "UI/UIManager.h"
#include "UI/Widget.h"


ToolsPanel::ToolsPanel()
{
	mPanelRoot = mnew UIWidget();
	mPanelRoot->AddLayer("back", mnew Sprite("ui/UI_ToolsPanel_bk.png"), Layout::BothStretch(-2, -3, -2, -2));

	mPanelRoot->layout.anchorMin = Vec2F(0, 1);
	mPanelRoot->layout.anchorMax = Vec2F(1, 1);
	mPanelRoot->layout.offsetMin = Vec2F(0, -45);
	mPanelRoot->layout.offsetMax = Vec2F(0, -20);

	InitializePlayPanel();
	InitializeLayoutSchemesPanel();
	InitializeToolsPanel();

	o2UI.AddWidget(mPanelRoot);
}

ToolsPanel::~ToolsPanel()
{}

void ToolsPanel::InitializePlayPanel()
{
	mPlayPanel = mnew UIWidget();
	mPlayPanel->name = "play panel";
	mPlayPanel->AddLayer("back", mnew Sprite("ui/UI_play_panel_bk.png"), Layout::BothStretch(-7, -5, -5, -5));
	mPlayPanel->layout = UIWidgetLayout::VerStretch(HorAlign::Left, 3, 2, 200, 10);
	mPanelRoot->AddChild(mPlayPanel);

	mPlayToggle = o2UI.CreateWidget<UIToggle>("play-stop");
	mPlayToggle->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(5, 1));
	mPlayPanel->AddChild(mPlayToggle);

	mPauseToggle = o2UI.CreateWidget<UIToggle>("pause");
	mPauseToggle->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(22, 1));
	mPlayPanel->AddChild(mPauseToggle);

	mStepButton = o2UI.CreateWidget<UIButton>("step");
	mStepButton->layout = UIWidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(39, 1));
	mPlayPanel->AddChild(mStepButton);

	mDevicesList = o2UI.CreateDropdown("backless");
	mDevicesList->name = "devices list";
	mDevicesList->layout = UIWidgetLayout::Based(BaseCorner::Right, Vec2F(115, 20), Vec2F(-3, 1));
	mPlayPanel->AddChild(mDevicesList);

	mDevicesList->AddItems({ "iPhone", "Editor", "Simulator" });
	mDevicesList->selectedItemPos = 0;

	Animation playPanelPlayStateAnim;
	playPanelPlayStateAnim.SetTarget(mPlayPanel);

	*playPanelPlayStateAnim.AddAnimationValue(&mPlayPanel->layout.offsetRight) =
		AnimatedValue<float>::EaseInOut(149.0f, 183.0f, 0.3f);

	auto visiblePauseBtnAnim = playPanelPlayStateAnim.AddAnimationValue(&mPauseToggle->visible);
	visiblePauseBtnAnim->AddKey(0.0f, false);
	visiblePauseBtnAnim->AddKey(0.1f, false);
	visiblePauseBtnAnim->AddKey(0.11f, true);
	visiblePauseBtnAnim->AddKey(0.3f, true);

	auto visibleStepBtnAnim = playPanelPlayStateAnim.AddAnimationValue(&mStepButton->visible);
	visibleStepBtnAnim->AddKey(0.0f, false);
	visibleStepBtnAnim->AddKey(0.25f, false);
	visibleStepBtnAnim->AddKey(0.26f, true);
	visibleStepBtnAnim->AddKey(0.3f, true);

	mPlayPanel->AddState("playing", playPanelPlayStateAnim);

	mPlayToggle->onToggle += [=](bool playing) {
		mPlayPanel->SetState("playing", playing);
	};
}

void ToolsPanel::InitializeLayoutSchemesPanel()
{
	mLayoutSchemesList = o2UI.CreateDropdown("round");
	mLayoutSchemesList->layout = UIWidgetLayout::VerStretch(HorAlign::Right, 3, 2, 150, 10);

	mLayoutSchemesList->AddItems({ "Standard", "Wide", "User defined" });
	mLayoutSchemesList->selectedItemPos = 0;

	mPanelRoot->AddChild(mLayoutSchemesList);
}

void ToolsPanel::InitializeToolsPanel()
{
	mEditToolsPanel = mnew UIHorizontalLayout();
	mEditToolsPanel->name = "edit tools";
	mEditToolsPanel->AddLayer("back", mnew Sprite("ui/UI_panel_subpanel_bk.png"), Layout::BothStretch(-7, -5, -10, -5));
	mEditToolsPanel->layout = UIWidgetLayout::VerStretch(HorAlign::Middle, 3, 2, 200, 10);
	mEditToolsPanel->expandHeight = false;
	mEditToolsPanel->expandWidth = false;
	mEditToolsPanel->fitByChildren = true;
	mEditToolsPanel->baseCorner = BaseCorner::Center;
	mPanelRoot->AddChild(mEditToolsPanel);

	mArrowToolToggle = o2UI.CreateWidget<UIToggle>("arrow");
	mBrushToolToggle = o2UI.CreateWidget<UIToggle>("brush");
	mMoveToolToggle = o2UI.CreateWidget<UIToggle>("move");
	mRotateToolToggle = o2UI.CreateWidget<UIToggle>("rotate");
	mScaleToolToggle = o2UI.CreateWidget<UIToggle>("scale");
	mFrameToolToggle = o2UI.CreateWidget<UIToggle>("frame");

	mEditToolsPanel->AddChild(mArrowToolToggle);
	mEditToolsPanel->AddChild(mBrushToolToggle);
	mEditToolsPanel->AddChild(mMoveToolToggle);
	mEditToolsPanel->AddChild(mRotateToolToggle);
	mEditToolsPanel->AddChild(mScaleToolToggle);
	mEditToolsPanel->AddChild(mFrameToolToggle);

	mArrowToolToggle->toggleGroup = mnew UIToggleGroup();
	mBrushToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;
	mMoveToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;
	mRotateToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;
	mScaleToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;
	mBrushToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;
	mFrameToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;

	mArrowToolToggle->SetValue(true);
}
