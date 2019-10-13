#include "stdafx.h"
#include "ToolsPanel.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Core/Tools/FrameTool.h"
#include "Core/Tools/MoveTool.h"
#include "Core/Tools/RotateTool.h"
#include "Core/Tools/ScaleTool.h"
#include "Core/Tools/SelectionTool.h"
#include "Core/UIRoot.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "Dialogs/EditNameDlg.h"
#include "SceneWindow/SceneEditScreen.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/DropDown.h"
#include "Scene/UI/Widgets/HorizontalLayout.h"
#include "Scene/UI/Widgets/Toggle.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widget.h"


namespace Editor
{
	Widget* ToolsPanel::GetPanelWidget() const
	{
		return mPanelRoot;
	}

	Widget* ToolsPanel::GetPlayPanel() const
	{
		return mPlayPanel;
	}

	HorizontalLayout* ToolsPanel::GetToolsPanel() const
	{
		return mEditToolsPanel;
	}

	ToolsPanel::ToolsPanel()
	{
		mPanelRoot = mnew Widget();
		mPanelRoot->name = "tools panel";
		mPanelRoot->AddLayer("back", mnew Sprite("ui/UI4_ToolsPanel_bk.png"), Layout::BothStretch(-2, 0, -2, -8));

		mPanelRoot->layout->anchorMin = Vec2F(0, 1);
		mPanelRoot->layout->anchorMax = Vec2F(1, 1);
		mPanelRoot->layout->offsetMin = Vec2F(0, -45);
		mPanelRoot->layout->offsetMax = Vec2F(0, -20);

		InitializePlayPanel();
		InitializeLayoutSchemesPanel();
		InitializeToolsPanel();

		EditorUIRoot.AddWidget(mPanelRoot);
	}

	ToolsPanel::~ToolsPanel()
	{}

	void ToolsPanel::InitializePlayPanel()
	{
		mPlayPanel = mnew Widget();
		mPlayPanel->name = "play panel";
		mPlayPanel->AddLayer("back", mnew Sprite("ui/UI4_play_panel_bk.png"), Layout::BothStretch(-7, -5, -5, -5));
		*mPlayPanel->layout = WidgetLayout::VerStretch(HorAlign::Left, 3, 2, 200, 10);
		mPanelRoot->AddChild(mPlayPanel);

		mPlayToggle = o2UI.CreateWidget<Toggle>("play-stop");
		*mPlayToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(5, 1));
		mPlayPanel->AddChild(mPlayToggle);

		mPauseToggle = o2UI.CreateWidget<Toggle>("pause");
		*mPauseToggle->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(22, 1));
		mPauseToggle->shortcut = ShortcutKeys(VK_F11);
		mPlayPanel->AddChild(mPauseToggle);

		mStepButton = o2UI.CreateWidget<Button>("step");
		*mStepButton->layout = WidgetLayout::Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(39, 1));
		mStepButton->shortcut = ShortcutKeys(VK_F10);
		mPlayPanel->AddChild(mStepButton);

		mDevicesList = o2UI.CreateDropdown("backless");
		mDevicesList->name = "devices list";
		*mDevicesList->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(115, 20), Vec2F(-3, 1));
		mPlayPanel->AddChild(mDevicesList);

		mDevicesList->AddItems({ "iPhone", "Editor", "Simulator" });
		mDevicesList->selectedItemPos = 0;

		Animation playPanelPlayStateAnim;
		playPanelPlayStateAnim.SetTarget(mPlayPanel);

		*playPanelPlayStateAnim.AddAnimationValue<float>("layout/offsetRight") =
			AnimatedValue<float>::EaseInOut(149.0f, 183.0f, 0.3f);

		auto visiblePauseBtnAnim = playPanelPlayStateAnim.AddAnimationValue<bool>("child/pause/enabled");
		visiblePauseBtnAnim->AddKey(0.0f, false);
		visiblePauseBtnAnim->AddKey(0.1f, false);
		visiblePauseBtnAnim->AddKey(0.11f, true);
		visiblePauseBtnAnim->AddKey(0.3f, true);

		auto visibleStepBtnAnim = playPanelPlayStateAnim.AddAnimationValue<bool>("child/step/enabled");
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
		*mLayoutSchemesList->layout = WidgetLayout::VerStretch(HorAlign::Right, 3, 2, 150, 10);
		mPanelRoot->AddChild(mLayoutSchemesList);
		UpdateWndLayoutSchemas();
		mLayoutSchemesList->onSelectedText = MakeFunction(this, &ToolsPanel::OnSchemeSelected);
	}

	void ToolsPanel::InitializeToolsPanel()
	{
		mEditToolsPanel = mnew HorizontalLayout();
		mEditToolsPanel->name = "edit tools";
		mEditToolsPanel->AddLayer("back", mnew Sprite("ui/UI4_panel_subpanel_bk.png"), Layout::BothStretch(-7, -5, -10, -5));
		*mEditToolsPanel->layout = WidgetLayout::VerStretch(HorAlign::Middle, 3, 2, 200, 10);
		mEditToolsPanel->expandHeight = true;
		mEditToolsPanel->expandWidth = false;
		mEditToolsPanel->fitByChildren = true;
		mEditToolsPanel->baseCorner = BaseCorner::Center;
		mPanelRoot->AddChild(mEditToolsPanel);

		mArrowToolToggle = o2UI.CreateWidget<Toggle>("arrow");
		mBrushToolToggle = o2UI.CreateWidget<Toggle>("brush");
		mMoveToolToggle = o2UI.CreateWidget<Toggle>("move");
		mRotateToolToggle = o2UI.CreateWidget<Toggle>("rotate");
		mScaleToolToggle = o2UI.CreateWidget<Toggle>("scale");
		mFrameToolToggle = o2UI.CreateWidget<Toggle>("frame");

		mEditToolsPanel->AddChild(mArrowToolToggle);
		mEditToolsPanel->AddChild(mBrushToolToggle);
		mEditToolsPanel->AddChild(mMoveToolToggle);
		mEditToolsPanel->AddChild(mRotateToolToggle);
		mEditToolsPanel->AddChild(mScaleToolToggle);
		mEditToolsPanel->AddChild(mFrameToolToggle);

		mArrowToolToggle->toggleGroup = mnew ToggleGroup(ToggleGroup::Type::OnlySingleTrue);
		mBrushToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;
		mMoveToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;
		mRotateToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;
		mScaleToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;
		mFrameToolToggle->toggleGroup = mArrowToolToggle->toggleGroup;

		mArrowToolToggle->shortcut = ShortcutKeys('Q');
		mBrushToolToggle->shortcut = ShortcutKeys('W');
		mMoveToolToggle->shortcut = ShortcutKeys('E');
		mRotateToolToggle->shortcut = ShortcutKeys('R');
		mScaleToolToggle->shortcut = ShortcutKeys('T');
		mFrameToolToggle->shortcut = ShortcutKeys('Y');

		mArrowToolToggle->onToggle = [](bool value) { if (value) o2EditorSceneScreen.SelectTool<SelectionTool>(); };
		mBrushToolToggle->onToggle = [](bool value) { if (value) o2EditorSceneScreen.SelectTool<SelectionTool>(); };
		mMoveToolToggle->onToggle = [](bool value) { if (value) o2EditorSceneScreen.SelectTool<MoveTool>(); };
		mRotateToolToggle->onToggle = [](bool value) { if (value) o2EditorSceneScreen.SelectTool<RotateTool>(); };
		mScaleToolToggle->onToggle = [](bool value) { if (value) o2EditorSceneScreen.SelectTool<ScaleTool>(); };
		mFrameToolToggle->onToggle = [](bool value) { if (value) o2EditorSceneScreen.SelectTool<FrameTool>(); };

		mFrameToolToggle->SetValue(true);
		o2EditorSceneScreen.SelectTool<FrameTool>();
	}

	void ToolsPanel::UpdateWndLayoutSchemas()
	{
		mLayoutSchemesList->RemoveAllItems();

		auto currentLayout = o2EditorWindows.GetWindowsLayout();
		String currentLayoutName = mDefaultSchemeName;
		for (auto availableLayoutKV : o2EditorWindows.mAvailableLayouts)
		{
			mLayoutSchemesList->AddItem(availableLayoutKV.Key());

			if (currentLayout == availableLayoutKV.Value())
				currentLayoutName = availableLayoutKV.Key();
		}

		mLayoutSchemesList->AddItem(mDefaultSchemeName);
		mLayoutSchemesList->AddItem(mSaveAsSchemeName);

		mLayoutSchemesList->SelectItemText(currentLayoutName);
	}

	void ToolsPanel::OnSchemeSelected(const WString& name)
	{
		if (name == mSaveAsSchemeName)
		{
			NameEditDlg::Show("New layout", [&](const String& name) {
				o2EditorWindows.mAvailableLayouts.Add(name, o2EditorWindows.GetWindowsLayout());
				UpdateWndLayoutSchemas();
			});
		}

		if (name == mDefaultSchemeName)
		{
			o2EditorWindows.SetDefaultWindowsLayout();
		}

		if (o2EditorWindows.mAvailableLayouts.ContainsKey(name))
		{
			o2EditorWindows.SetWindowsLayout(name);
		}
	}

}
