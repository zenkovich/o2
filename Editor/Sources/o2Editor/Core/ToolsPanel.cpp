#include "o2Editor/stdafx.h"
#include "ToolsPanel.h"

#include "EditorApplication.h"
#include "o2/Animation/Tracks/AnimationColor4Track.h"
#include "o2/Animation/Tracks/AnimationFloatTrack.h"
#include "o2/Animation/Tracks/AnimationVec2FTrack.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/DropDown.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/Widgets/Toggle.h"
#include "o2/Utils/Tasks/TaskManager.h"
#include "o2Editor/Core/Dialogs/EditNameDlg.h"
#include "o2Editor/Core/Tools/FrameTool.h"
#include "o2Editor/Core/Tools/MoveTool.h"
#include "o2Editor/Core/Tools/RotateTool.h"
#include "o2Editor/Core/Tools/ScaleTool.h"
#include "o2Editor/Core/Tools/SelectionTool.h"
#include "o2Editor/Core/UIRoot.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
    Ref<Widget> ToolsPanel::GetPanelWidget() const
    {
        return mPanelRoot;
    }

    Ref<Widget> ToolsPanel::GetPlayPanel() const
    {
        return mPlayPanel;
    }

    Ref<HorizontalLayout> ToolsPanel::GetToolsPanel() const
    {
        return mToolsPanel;
    }

    void ToolsPanel::AddToolToggle(Ref<Toggle> toggle)
    {
        if (!toggle->GetStateObject("visible")->GetAnimationClip()->GetTrack<float>("layout/minWidth"))
        {
            *toggle->GetStateObject("visible")->GetAnimationClip()->AddTrack<float>("layout/minWidth") =
                AnimationTrack<float>::EaseInOut(0.0f, 20.0f, 0.2f);
        }

        mToolsPanel->AddChild(toggle);
        toggle->SetEnabledForcible(false);
        toggle->SetEnabled(true);
        toggle->SetToggleGroup(&mToolsTogglesGroup);
    }

    void ToolsPanel::RemoveToolToggle(Ref<Toggle> toggle)
    {
        toggle->GetStateObject("visible")->onStateFullyFalse = [=]() {
            o2Tasks.Invoke([=]() { mToolsPanel->RemoveChild(toggle, false); });
        };
        toggle->SetEnabled(false);
    }

    void ToolsPanel::Update(float dt)
    {
        mPlayToggle->value = o2EditorApplication.IsPlaying();
        mPauseToggle->value = o2EditorApplication.isPaused;
    }

    ToolsPanel::ToolsPanel() :
        mToolsTogglesGroup(ToggleGroup::Type::OnlySingleTrue)
    {
        mPanelRoot = mmake<Widget>();
        mPanelRoot->name = "tools panel";
        mPanelRoot->AddLayer("back", mmake<Sprite>("ui/UI4_ToolsPanel_bk.png"), mmake<Layout>(BothStretch(-2, 0, -2, -8)));

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
    {
    }

    void ToolsPanel::InitializePlayPanel()
    {
        mPlayPanel = mmake<Widget>();
        mPlayPanel->name = "play panel";
        mPlayPanel->AddLayer("back", mmake<Sprite>("ui/UI4_play_panel_bk.png"), mmake<Layout>(BothStretch(-7, -5, -5, -5)));
        *mPlayPanel->layout = mmake<WidgetLayout>(VerStretch(HorAlign::Left, 3, 2, 200, 10));
        mPanelRoot->AddChild(mPlayPanel);

        mPlayToggle = o2UI.CreateWidget<Toggle>("play-stop");
        mPlayToggle->onToggle = [&](bool value) { OnPlayStopToggled(value); };
        *mPlayToggle->layout = mmake<WidgetLayout>(Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(5, 1)));
        mPlayPanel->AddChild(mPlayToggle);

        mPauseToggle = o2UI.CreateWidget<Toggle>("pause");
        mPauseToggle->onToggle = [this](bool value) { OnPauseToggled(value); };
        *mPauseToggle->layout = mmake<WidgetLayout>(Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(22, 1)));
        mPauseToggle->shortcut = ShortcutKeys(VK_F11);
        mPlayPanel->AddChild(mPauseToggle);

        mStepButton = o2UI.CreateWidget<Button>("step");
        mStepButton->onClick = [this]() { OnStepPressed(); };
        *mStepButton->layout = mmake<WidgetLayout>(Based(BaseCorner::Left, Vec2F(20, 20), Vec2F(39, 1)));
        mStepButton->shortcut = ShortcutKeys(VK_F10);
        mPlayPanel->AddChild(mStepButton);

        mDevicesList = o2UI.CreateDropdown("backless");
        mDevicesList->name = "devices list";
        *mDevicesList->layout = mmake<WidgetLayout>(Based(BaseCorner::Right, Vec2F(115, 20), Vec2F(-3, 1)));
        mPlayPanel->AddChild(mDevicesList);

        mDevicesList->AddItems({ "iPhone", "Editor", "Simulator" });
        mDevicesList->selectedItemPos = 0;

        auto playPanelPlayStateAnim = mmake<AnimationClip>();

        *playPanelPlayStateAnim->AddTrack<float>("layout/offsetRight") =
            AnimationTrack<float>::EaseInOut(149.0f, 183.0f, 0.3f);

        auto visiblePauseBtnAnim = playPanelPlayStateAnim->AddTrack<bool>("child/pause/enabled");
        visiblePauseBtnAnim->AddKey(0.0f, false);
        visiblePauseBtnAnim->AddKey(0.1f, false);
        visiblePauseBtnAnim->AddKey(0.11f, true);
        visiblePauseBtnAnim->AddKey(0.3f, true);

        auto visibleStepBtnAnim = playPanelPlayStateAnim->AddTrack<bool>("child/step/enabled");
        visibleStepBtnAnim->AddKey(0.0f, false);
        visibleStepBtnAnim->AddKey(0.25f, false);
        visibleStepBtnAnim->AddKey(0.26f, true);
        visibleStepBtnAnim->AddKey(0.3f, true);

        mPlayPanel->AddState("playing", playPanelPlayStateAnim);

        mPlayToggle->onToggle += [=](bool playing) {
            mPlayPanel->SetState("playing");
        };
    }
}
#include <memory>

// Ref<> and WeakRef<> templates
template<typename T>
class Ref {
public:
	Ref(T* ptr) : ptr(ptr) {}
	T* operator->() { return ptr; }
	T& operator*() { return *ptr; }
private:
	T* ptr;
};

template<typename T>
class WeakRef {
public:
	WeakRef(T* ptr) : ptr(ptr) {}
	T* operator->() { return ptr; }
	T& operator*() { return *ptr; }
private:
	T* ptr;
};

// DynamicCast<> template
template<typename Derived, typename Base, typename T>
T DynamicCast(Ref<Base>& base) {
    return static_cast<T>(base.operator->());
};

namespace o2Editor {

	using namespace o2Engine;

	ToolsPanel::ToolsPanel() :
		mLayoutSchemesList(nullptr),
		mToolsPanel(nullptr),
		mPlayStopButton(nullptr),
		mPauseButton(nullptr),
		mStepButton(nullptr)
	{
		mPanelRoot = mnew VerStretchLayout();
		mPanelRoot->baseCorner = BaseCorner::TopLeft;
		mPanelRoot->expand = true;
		mPanelRoot->expandWidth = false;
		mPanelRoot->padding = Layout::UniformPadding();

		mPanelRoot->layout->top = 0;
		mPanelRoot->layout->left = 0;
		mPanelRoot->layout->width = 200;
		mPanelRoot->layout->height = 600;

		InitializeLayoutSchemesPanel();
		InitializeToolsPanel();
		InitializePlaybackControls();

		o2EditorApplication.OnPlayStopToggled += MakeFunction(this, &ToolsPanel::OnPlayStopToggled);
		o2EditorApplication.OnStepPressed += MakeFunction(this, &ToolsPanel::OnStepPressed);
		o2EditorApplication.OnPauseToggled += MakeFunction(this, &ToolsPanel::OnPauseToggled);
	}

	void ToolsPanel::AddToolToggle(const Ref<Button>& button)
	{
		*mToolsPanel->layout = WidgetLayout::VerAlign(HorAlign::Center, 2, 2 + (int)(mButtons.size() * 3), 200, (int)mButtons.size() * 26);

		mToolsPanel->AddChild(button);
		mButtons.push_back(button);
	}

	void ToolsPanel::InitializePlaybackControls()
	{
		mPlaybackToolsPanel = mnew HorizontalLayout();
		mPlaybackToolsPanel->name = "playback";
		*mPlaybackToolsPanel->layout = WidgetLayout::VerStretch(HorAlign::Middle, 3, 2, 200, 10);
		mPlaybackToolsPanel->expandHeight = false;
		mPlaybackToolsPanel->expandWidth = false;
		mPlaybackToolsPanel->baseCorner = BaseCorner::Center;
		mPanelRoot->AddChild(mPlaybackToolsPanel);

		mPlayStopButton = mnew ToggleButton();
		*mPlayStopButton->layout = WidgetLayout::HorStretch(3, 2, 100, 30);
		mPlayStopButton->fitByChildren = true;
		mPlayStopButton->spriteIdle = mPanelIdle;
		mPlayStopButton->spriteHot = mPanelHot;
		mPlayStopButton->spritePressed = mPanelPressed;
		mPlayStopButton->spriteFocused = mPanelFocused;
		mPlayStopButton->layout->contentWidthFrac = 1.0f;
		mPlayStopButton->layout->contentHeightFrac = 1.0f;
		mPlayStopButton->onToggled = [](bool playing) {
			playing ? o2EditorApplication.ResumePlayback() : o2EditorApplication.PausePlayback();
		};
		mPlaybackToolsPanel->AddChild(mPlayStopButton);

		mPauseButton = mnew Button();
		*mPauseButton->layout = WidgetLayout::HorStretch(3, 2, 100, 30);
		
		// Convert raw pointers to Ref<>
		const Ref<Button>& pauseButtonRef = mPauseButton;
		const Ref<Sprite>& panelSpriteIdleRef = mPanelIdle;
		const Ref<Sprite>& panelSpriteHotRef = mPanelHot;
		const Ref<Sprite>& panelSpritePressedRef = mPanelPressed;
		const Ref<Sprite>& panelSpriteFocusedRef = mPanelFocused;
		
		// Use const Ref<>& for function arguments with pointer types
		mPauseButton->fitByChildren = true;
		mPauseButton->spriteIdle = panelSpriteIdleRef;
		mPauseButton->spriteHot = panelSpriteHotRef;
		mPauseButton->spritePressed = panelSpritePressedRef;
		mPauseButton->spriteFocused = panelSpriteFocusedRef;
		mPauseButton->layout->contentWidthFrac = 1.0f;
		mPauseButton->layout->contentHeightFrac = 1.0f;
		mPauseButton->onClick = [=]() {
			o2EditorApplication.PausePlayback();
		};
		mPlaybackToolsPanel->AddChild(mPauseButton);

		mStepButton = mnew Button();
		*mStepButton->layout = WidgetLayout::HorStretch(3, 2, 33, 30);
		mStepButton->fitByChildren = true;
		mStepButton->spriteIdle = mStepSprite;
		mStepButton->spriteHot = mStepSpriteHot;
		mStepButton->spritePressed = mStepSpritePressed;
		mStepButton->spriteFocused = mStepSpriteFocused;
		mStepButton->onClick = MakeFunction(this, &ToolsPanel::OnStepPressed);
		mPlaybackToolsPanel->AddChild(mStepButton);
	}

	void ToolsPanel::InitializeLayoutSchemesPanel()
	{
		mLayoutSchemesList = mmake<Dropdown>("round");
		mLayoutSchemesList->layout = WidgetLayout::VerStretch(HorAlign::Right, 3, 2, 150, 10);
		mPanelRoot->AddChild(mLayoutSchemesList);
		UpdateWndLayoutSchemas();
		mLayoutSchemesList->onSelectedText = MakeFunction(this, &ToolsPanel::OnSchemeSelected);
	}

	void ToolsPanel::InitializeToolsPanel()
	{
		mToolsPanel = mmake<HorizontalLayout>();
		mToolsPanel->name = "edit tools";
		mToolsPanel->AddLayer("back", mmake<Sprite>("ui/UI4_panel_subpanel_bk.png"), Layout::BothStretch(-7, -5, -10, -5));
		mToolsPanel->layout = WidgetLayout::VerStretch(HorAlign::Middle, 3, 2, 200, 10);
		mToolsPanel->expandHeight = true;
		mToolsPanel->expandWidth = false;
		mToolsPanel->fitByChildren = true;
		mToolsPanel->baseCorner = BaseCorner::Center;
		mPanelRoot->AddChild(mToolsPanel);

		for (auto tool : o2EditorSceneScreen.GetTools())
			AddToolToggle(tool->GetPanelToggle());

		o2EditorSceneScreen.SelectTool<FrameTool>();
	}

	void ToolsPanel::UpdateWndLayoutSchemas()
	{
		mLayoutSchemesList->RemoveAllItems();

		auto currentLayout = o2EditorWindows.GetWindowsLayout();
		String currentLayoutName = mDefaultSchemeName;
		for (auto& availableLayoutKV : o2EditorWindows.mAvailableLayouts)
		{
			mLayoutSchemesList->AddItem(availableLayoutKV.first);

			if (currentLayout == availableLayoutKV.second)
				currentLayoutName = availableLayoutKV.first;
		}

		mLayoutSchemesList->AddItem(mDefaultSchemeName);
		mLayoutSchemesList->AddItem(mSaveAsSchemeName);

		mLayoutSchemesList->SelectItemText(currentLayoutName);
	}

	void ToolsPanel::OnSchemeSelected(const WString& name)
	{
		if ((String)name == mSaveAsSchemeName)
		{
			NameEditDlg::Show("New layout", [&](const String& name) {
				o2EditorWindows.mAvailableLayouts.Add(name, o2EditorWindows.GetWindowsLayout());
				UpdateWndLayoutSchemas();
			});
		}

		if ((String)name == mDefaultSchemeName)
		{
			o2EditorWindows.SetDefaultWindowsLayout();
		}

		if (o2EditorWindows.mAvailableLayouts.ContainsKey(name))
		{
			o2EditorWindows.SetWindowsLayout(name);
		}
	}

	void ToolsPanel::OnPlayStopToggled(bool play)
	{
		o2EditorApplication.SetPlaying(play);
	}

	void ToolsPanel::OnPauseToggled(bool pause)
	{
		o2EditorApplication.isPaused = pause;
	}

	void ToolsPanel::OnStepPressed()
	{
		o2EditorApplication.isPaused = true;
		o2EditorApplication.step = true;
	}

}