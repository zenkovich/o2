```cpp
#include "o2Editor/stdafx.h"
#include "BasicUIStyle.h"

#include "o2/Animation/Tracks/AnimationFloatTrack.h"
#include "o2/Animation/Tracks/AnimationVec2FTrack.h"
#include "o2/Animation/Tracks/AnimationColor4Track.h"
#include "o2/Render/Sprite.h"
#include "o2/Render/Text.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Scene/UI/Widgets/CustomDropDown.h"
#include "o2/Scene/UI/Widgets/CustomList.h"
#include "o2/Scene/UI/Widgets/DropDown.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/HorizontalProgress.h"
#include "o2/Scene/UI/Widgets/HorizontalScrollBar.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/List.h"
#include "o2/Scene/UI/Widgets/LongList.h"
#include "o2/Scene/UI/Widgets/MenuPanel.h"
#include "o2/Scene/UI/Widgets/PopupWidget.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Scene/UI/Widgets/Toggle.h"
#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Scene/UI/Widgets/VerticalProgress.h"
#include "o2/Scene/UI/Widgets/VerticalScrollBar.h"
#include "o2/Scene/UI/Widgets/Window.h"

namespace o2
{
	void BasicUIStyleBuilder::RebuildButtonStyle()
	{
		Ref<Button> sample = mmake<Button>();
		sample->layout->minSize = Ref<Vec2F>(mmake<Vec2F>(20, 20));

		auto regularLayer = sample->AddLayer("regular", mmake<Sprite>("ui/UI4_button_regular.png"),
											 Ref<Layout>(mmake<Layout>(Layout::BothStretch(-9, -9, -10, -10))));

		auto hoverLayer   = sample->AddLayer("hover", mmake<Sprite>("ui/UI4_button_select.png"),
											 Ref<Layout>(mmake<Layout>(Layout::BothStretch(-9, -9, -10, -10))));

		auto pressedLayer = sample->AddLayer("pressed", mmake<Sprite>("ui/UI4_button_pressed.png"),
											 Ref<Layout>(mmake<Layout>(Layout::BothStretch(-9, -9, -10, -10))));

		auto focusLayer   = sample->AddLayer("focus", mmake<Sprite>("ui/UI4_button_focus.png"),
											 Ref<Layout>(mmake<Layout>(Layout::BothStretch(-9, -9, -10, -10))));

		Ref<Text> captionText        = mmake<Text>("stdFont.ttf");
		captionText->text        = "Button";
		captionText->horAlign    = HorAlign::Middle;
		captionText->verAlign    = VerAlign::Middle;
		captionText->dotsEngings = true;
		captionText->wordWrap    = true;
		captionText->color       = Ref<Color4>(mmake<Color4>(96, 125, 139));
		sample->AddLayer("caption", captionText);

		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", AnimationClip::EaseInOut("layer/focus/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCloseButtonStyle()
	{
		Ref<Button> sample = mmake<Button>();
		sample->layout->minSize = Ref<Vec2F>(mmake<Vec2F>(5, 5));

		auto regularLayer = sample->AddLayer("regular", mmake<Sprite>("ui/UI4_Close_button_regular.png"),
											 Ref<Layout>(mmake<Layout>(Layout::Based(BaseCorner::Center, Ref<Vec2F>(mmake<Vec2F>(20, 20))))));

		auto hoverLayer = sample->AddLayer("hover", mmake<Sprite>("ui/UI4_Close_button_select.png"),
										   Ref<Layout>(mmake<Layout>(Layout::Based(BaseCorner::Center, Ref<Vec2F>(mmake<Vec2F>(20, 20))))));

		auto pressedLayer = sample->AddLayer("pressed", mmake<Sprite>("ui/UI4_Close_button_pressed.png"),
											 Ref<Layout>(mmake<Layout>(Layout::Based(BaseCorner::Center, Ref<Vec2F>(mmake<Vec2F>(20, 20))))));


		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "close");
	}

	void BasicUIStyleBuilder::RebuildArrowButtonStyle()
	{
		Ref<Button> sample = mmake<Button>();
		sample->layout->minSize = Ref<Vec2F>(mmake<Vec2F>(5, 5));

		auto regularLayer = sample->AddLayer("regular", mmake<Sprite>("ui/UI4_Options_button_regular.png"),
											 Ref<Layout>(mmake<Layout>(Layout::Based(BaseCorner::Center, Ref<Vec2F>(mmake<Vec2F>(20, 20))))));

		auto hoverLayer = sample->AddLayer("hover", mmake<Sprite>("ui/UI4_Options_button_select.png"),
										   Ref<Layout>(mmake<Layout>(Layout::Based(BaseCorner::Center, Ref<Vec2F>(mmake<Vec2F>(20, 20))))));

		auto pressedLayer = sample->AddLayer("pressed", mmake<Sprite>("ui/UI4_Options_button_pressed.png"),
											 Ref<Layout>(mmake<Layout>(Layout::Based(BaseCorner::Center, Ref<Vec2F>(mmake<Vec2F>(20, 20))))));


		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "arrow");
	}
}
```#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
Ref<T> DynamicCast(const Ref<T>& ptr) {
    return std::dynamic_pointer_cast<T>(ptr);
}

class BasicUIStyleBuilder {
public:
    void RebuildButtonStyle() {
        Ref<Button> sample = mmake<Button>();
        sample->layout->minSize = Vec2F(100, 50);
        sample->AddLayer("background", mmake<Sprite>("ui/UI4_Button.png"));
        sample->AddChild("label", mmake<Label>())
            ->SetText("Button")
            ->SetHorizontalAlignment(HAlign::Center)
            ->SetVerticalAlignment(VAlign::Middle)
            ->SetFontSize(24)
            ->SetFontColor(Color::White);
        sample->AddState("hover", AnimationClip::Linear("background/transparency", 0.0f, 1.0f, 0.1f));
        sample->AddState("pressed", AnimationClip::Linear("background/transparency", 0.0f, 1.0f, 0.05f));
        sample->AddState("visible", AnimationClip::Linear("transparency", 0.0f, 1.0f, 0.2f));
        sample->offStateAnimationSpeed = 0.5f;

        o2UI.AddWidgetStyle(sample, "button");
    }

    void RebuildHorSeparatorStyle() {
        Ref<Widget> sample = mmake<Widget>();
        sample->AddLayer("line", mmake<Sprite>("ui/UI4_Separator.png"),
            Layout::HorStretch(VerAlign::Middle, 0, 0, 5, 0));
        *sample->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, 5);
        sample->layout->maxHeight = 5;
        sample->layout->minHeight = 5;
        sample->layout->height = 5;

        o2UI.AddWidgetStyle(sample, "hor separator");
    }

    void RebuildHorProgressBarStyle() {
        Ref<HorizontalProgress> sample = mmake<HorizontalProgress>();
        sample->layout->minSize = Vec2F(5, 5);
        auto backLayer = sample->AddLayer("back", nullptr);
        auto spriteBackLayer = backLayer->AddChildLayer("sprite", mmake<Sprite>("ui/UI4_Hor_scrollbar_bk.png"),
            Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-2, -4), Vec2F(2, 5)));

        backLayer->interactableLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -4), Vec2F(0, 4));

        auto barLayer = sample->AddLayer("bar", nullptr);
        auto barRegularSprite = barLayer->AddChildLayer("regular", mmake<Sprite>("ui/UI4_Hor_scrollhandle_regular.png"),
            Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-2, -4), Vec2F(2, 5)));

        auto barSelectSprite = barLayer->AddChildLayer("hover", mmake<Sprite>("ui/UI4_Hor_scrollhandle_select.png"),
            Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-2, -4), Vec2F(2, 5)));

        auto barPressedSprite = barLayer->AddChildLayer("pressed", mmake<Sprite>("ui/UI4_Hor_scrollhandle_pressed.png"),
            Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-2, -4), Vec2F(2, 5)));

        sample->AddState("hover", AnimationClip::EaseInOut("layer/bar/child/hover/transparency", 0.0f, 1.0f, 0.1f))
            ->offStateAnimationSpeed = 1.0f / 4.0f;

        sample->AddState("pressed", AnimationClip::EaseInOut("layer/bar/child/pressed/transparency", 0.0f, 1.0f, 0.05f))
            ->offStateAnimationSpeed = 0.5f;

        sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
            ->offStateAnimationSpeed = 0.5f;

        sample->SetOrientation(HorizontalProgress::Orientation::Right);

        o2UI.AddWidgetStyle(sample, "standard");
    }

    void RebuildVerProgressBarStyle() {
        Ref<VerticalProgress> sample = mmake<VerticalProgress>();
        sample->layout->minSize = Vec2F(5, 5);
        auto backLayer = sample->AddLayer("back", nullptr);
        auto spriteBackLayer = backLayer->AddChildLayer("sprite", mmake<Sprite>("ui/UI4_Ver_scrollbar_bk.png"),
            Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, -2), Vec2F(5, 2)));
        backLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));

        auto barLayer = sample->AddLayer("bar", nullptr);
        auto barRegularSprite = barLayer->AddChildLayer("regular", mmake<Sprite>("ui/UI4_Ver_scrollhandle_regular.png"),
            Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, -2), Vec2F(5, 2)));

        auto barSelectSprite = barLayer->AddChildLayer("hover", mmake<Sprite>("ui/UI4_Ver_scrollhandle_select.png"),
            Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, -2), Vec2F(5, 2)));

        auto barPressedSprite = barLayer->AddChildLayer("pressed", mmake<Sprite>("ui/UI4_Ver_scrollhandle_pressed.png"),
            Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, -2), Vec2F(5, 2)));

        sample->AddState("hover", AnimationClip::EaseInOut("layer/bar/child/hover/transparency", 0.0f, 1.0f, 0.1f))
            ->offStateAnimationSpeed = 1.0f / 4.0f;

        sample->AddState("pressed", AnimationClip::EaseInOut("layer/bar/child/pressed/transparency", 0.0f, 1.0f, 0.05f))
            ->offStateAnimationSpeed = 0.5f;

        sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
            ->offStateAnimationSpeed = 0.5f;

        o2UI.AddWidgetStyle(sample, "standard");
    }

    void RebuildHorScrollBarStyle() {
        Ref<HorizontalScrollBar> sample = mmake<HorizontalScrollBar>();
        sample->layout->minSize = Vec2F(5, 5);
        sample->SetScrollSense(0.25f);
        sample->SetMinimalScrollHandleSize(10);

        auto backLayer = sample->AddLayer("back", nullptr);
        backLayer->interactableLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -4), Vec2F(0, 4));
        auto spriteBackLayer = backLayer->AddChildLayer("sprite", mmake<Sprite>("ui/UI4_Hor_scrollbar_bk.png"),
            Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-2, -4), Vec2F(2, 5)));

        auto handleLayer = sample->AddLayer("handle", nullptr);
        handleLayer->interactableLayout = Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(0, -4), Vec2F(0, 4));
        auto barRegularSprite = handleLayer->AddChildLayer("regular", mmake<Sprite>("ui/UI4_Hor_scrollhandle_regular.png"),
            Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-2, -4), Vec2F(2, 5)));

        auto barSelectSprite = handleLayer->AddChildLayer("hover", mmake<Sprite>("ui/UI4_Hor#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
struct DynamicCast
{
    template<typename U>
    static Ref<T> cast(Ref<U> ptr)
    {
        return std::dynamic_pointer_cast<T>(ptr);
    }
};

void BasicUIStyleBuilder::RebuildHorScrollBarStyle()
{
    HorizontalScrollBar* sample = mmake<HorizontalScrollBar>();
    sample->layout->minSize = Vec2F(5, 5);
    sample->SetScrollSense(0.25f);
    sample->SetMinimalScrollHandleSize(10);

    auto backLayer = sample->AddLayer("back", nullptr);
    backLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));
    auto spriteBackLayer = backLayer->AddChildLayer("sprite", mmake<Sprite>("ui/UI4_Hor_scrollbar_bk.png"),
                                                    Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
                                                           Vec2F(-2, -4), Vec2F(2, 5)));

    auto handleLayer = sample->AddLayer("handle", nullptr);
    handleLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));
    auto barRegularSprite = handleLayer->AddChildLayer("regular", mmake<Sprite>("ui/UI4_Hor_scrollhandle_regular.png"),
                                                       Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
                                                              Vec2F(-2, -4), Vec2F(2, 5)));

    auto barSelectSprite = handleLayer->AddChildLayer("hover", mmake<Sprite>("ui/UI4_Hor_scrollhandle_select.png"),
                                                      Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
                                                             Vec2F(-2, -4), Vec2F(2, 5)));

    auto barPressedSprite = handleLayer->AddChildLayer("pressed", mmake<Sprite>("ui/UI4_Hor_scrollhandle_pressed.png"),
                                                       Layout(Vec2F(0.0f, 0.5f), Vec2F(1.0f, 0.5f),
                                                              Vec2F(-2, -4), Vec2F(2, 5)));

    sample->AddState("hover", AnimationClip::EaseInOut("layer/handle/child/hover/transparency", 0.0f, 1.0f, 0.1f))->offStateAnimationSpeed = 1.0f / 4.0f;

    sample->AddState("pressed", AnimationClip::EaseInOut("layer/handle/child/pressed/transparency", 0.0f, 1.0f, 0.05f))->offStateAnimationSpeed = 0.5f;

    sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))->offStateAnimationSpeed = 0.5f;

    o2UI.AddWidgetStyle(sample, "standard");
}

void BasicUIStyleBuilder::RebuildVerScrollBarStyle()
{
    VerticalScrollBar* sample = mmake<VerticalScrollBar>();
    sample->layout->minSize = Vec2F(5, 5);
    sample->SetScrollSense(0.25f);
    sample->SetMinimalScrollHandleSize(10);

    auto backLayer = sample->AddLayer("back", nullptr);
    backLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));
    auto spriteBackLayer = backLayer->AddChildLayer("sprite", mmake<Sprite>("ui/UI4_Ver_scrollbar_bk.png"),
                                                    Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
                                                           Vec2F(-4, -2), Vec2F(5, 2)));

    auto handleLayer = sample->AddLayer("handle", nullptr);
    handleLayer->interactableLayout = Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f), Vec2F(-4, 0), Vec2F(4, 0));
    auto barRegularSprite = handleLayer->AddChildLayer("regular", mmake<Sprite>("ui/UI4_Ver_scrollhandle_regular.png"),
                                                       Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
                                                              Vec2F(-4, -2), Vec2F(5, 2)));

    auto barSelectSprite = handleLayer->AddChildLayer("hover", mmake<Sprite>("ui/UI4_Ver_scrollhandle_select.png"),
                                                      Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
                                                             Vec2F(-4, -2), Vec2F(5, 2)));

    auto barPressedSprite = handleLayer->AddChildLayer("pressed", mmake<Sprite>("ui/UI4_Ver_scrollhandle_pressed.png"),
                                                       Layout(Vec2F(0.5f, 0.0f), Vec2F(0.5f, 1.0f),
                                                              Vec2F(-4, -2), Vec2F(5, 2)));

    sample->AddState("hover", AnimationClip::EaseInOut("layer/handle/child/hover/transparency", 0.0f, 1.0f, 0.1f))->offStateAnimationSpeed = 1.0f / 4.0f;

    sample->AddState("pressed", AnimationClip::EaseInOut("layer/handle/child/pressed/transparency", 0.0f, 1.0f, 0.05f))->offStateAnimationSpeed = 0.5f;

    sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))->offStateAnimationSpeed = 0.5f;

    o2UI.AddWidgetStyle(sample, "standard");
}

void BasicUIStyleBuilder::RebuildCheckboxStyle()
{
    Toggle* sample = mmake<Toggle>();
    sample->layout->minSize = Vec2F(20, 20);
    auto backLayer = sample->AddLayer("back", mmake<Sprite>("ui/UI4_Check_bk.png"),
                                      Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

    auto hoverLayer = sample->AddLayer("backSelect", mmake<Sprite>("ui/UI4_Check_bk_select.png"),
                                       Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

    auto pressedLayer = sample->AddLayer("backPressed", mmake<Sprite>("ui/UI4_Check_bk_pressed.png"),
                                         Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

    auto focusLayer = sample->AddLayer("backFocus", mmake<Sprite>("ui/UI4_Check_bk_focus.png"),
                                       Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(0, -10), Vec2F(20, 10)));

    auto checkLayer = sample->AddLayer("check", mmake<Sprite>("ui/UI4_Ckeck.png"),
                                       Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(1, -11), Vec2F(21, 10)));

    auto unknownLayer = sample->AddLayer("unknown", mmake<Sprite>("ui/UI4_Check_unknown.png"),
                                         Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(1, -11), Vec2F(21, 10)));

    Text* captionText = mmake<Text>("stdFont.ttf");
    captionText->text = "Checkbox";
    captionText->horAlign = HorAlign::Left;
    captionText0);
		horScrollBar->layout->offsetMin = Vec2F(2, 0);
		horScrollBar->layout->offsetMax = Vec2F(-2, 15);
		sample->SetHorizontalScrollBar(horScrollBar);

		VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
		verScrollBar->layout->anchorMin = Vec2F(1, 0);
		verScrollBar->layout->anchorMax = Vec2F(1, 1);
		verScrollBar->layout->offsetMin = Vec2F(-15, 2);
		verScrollBar->layout->offsetMax = Vec2F(0, -2);
		sample->SetVerticalScrollBar(verScrollBar);

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "popup");
	}

	void BasicUIStyleBuilder::RebuildButtonStyle()
	{
		Button* sample = mnew Button();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(3, 3, 3, 3));
		sample->SetViewLayout(Layout::BothStretch(0, 0, 0, 0));

		sample->AddLayer("back", mnew Sprite("ui/UI4_button.png"), Layout::BothStretch(-3, -3, -3, -3));
		sample->AddLayer("hover", mnew Sprite("ui/UI4_button_focus.png"), Layout::BothStretch(-3, -3, -3, -3))
			->SetTransparency(0.0f);

		auto hoverAnim = mmake<AnimationClip>();
		*hoverAnim->AddTrack<float>("layer/hover/transparency") =
			AnimationTrack<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("hover", hoverAnim);

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "button");
	}

	void BasicUIStyleBuilder::RebuildDropdownButtonStyle()
	{
		DropdownButton* sample = mnew DropdownButton();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(3, 3, 3, 3));
		sample->SetViewLayout(Layout::BothStretch(0, 0, 0, 0));

		sample->AddLayer("back", mnew Sprite("ui/UI4_button.png"), Layout::BothStretch(-3, -3, -3, -3));
		sample->AddLayer("hover", mnew Sprite("ui/UI4_button_focus.png"), Layout::BothStretch(-3, -3, -3, -3))
			->SetTransparency(0.0f);

		auto hoverAnim = mmake<AnimationClip>();
		*hoverAnim->AddTrack<float>("layer/hover/transparency") =
			AnimationTrack<float>::EaseInOut(0.0f, 1.0f, 0.1f);

		sample->AddState("hover", hoverAnim);

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "dropdown button");
	}

	void BasicUIStyleBuilder::RebuildInputTextWithoutBackground()
	{
		InputText* sample = mnew InputText();
		sample->layout->minSize = Vec2F(100, 30);
		sample->SetClippingLayout(Layout::BothStretch(1, 1, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(10, 10, 3, 3));

		sample->AddTexture("caret", mnew Sprite("ui/UI4_Button_Focus_Image.png"));

		sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f));

		sample->AddState("focused", AnimationClip::EaseInOut("overlay/caret/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "input text without background");
	}

	void BasicUIStyleBuilder::RebuildInputFieldStyle()
	{
		InputField* sample = mnew InputField();
		sample->layout->minSize = Vec2F(100, 30);
		sample->SetClippingLayout(Layout::BothStretch(1, 1, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(10, 10, 3, 3));

		sample->AddTexture("caret", mnew Sprite("ui/UI4_Button_Focus_Image.png"));

		sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));
		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f));

		sample->AddState("focused", AnimationClip::EaseInOut("overlay/caret/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "input field");
	}

	void BasicUIStyleBuilder::RebuildProgressBarStyle()
	{
		ProgressBar* sample = mnew ProgressBar();
		sample->layout->minSize = Vec2F(100, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 1, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(3, 3, 3, 3));

		auto backSprite = mnew Sprite("ui/UI4_Editbox_regular.png");
		backSprite->SetTransparency(0.7f);
		sample->AddLayer("back", backSprite, Layout::BothStretch(-9, -9, -9, -9));

		auto barSprite = mnew Sprite("ui/UI4_Progress_bar.png");
		barSprite->SetPivot(Vec2F(0, 0.5f));
		barSprite->SetSizeByStripSize(true);
		sample->AddLayer("bar", barSprite, Layout::BothStretch(-6, -9, -6, -9))
			->horizontalResizePolicy = ResizePolicy::Stretch;

		auto hoverAnim = mmake<AnimationClip>();
		*hoverAnim->AddTrack<Vec2F>("layer/bar/sizeOffset") =
			AnimationTrack<Vec2F>::EaseInOut(Vec2F(-6, -9), Vec2F(0, -9), 0.1f);

		sample->AddState("hover", hoverAnim);

		auto valueAnim = mmake<AnimationClip>();
		*valueAnim->AddTrack<Vec2F>("layer/bar/sizeOffset") =
			AnimationTrack<Vec2F>::EaseInOut(Vec2F(-6, -9), Vec2F(0, -9), 0.2f);

		auto valueState = sample->AddState("value", valueAnim);

		auto reduceValueAnim = mmake<AnimationClip>();
		*reduceValueAnim->AddTrack<Vec2F>("layer/bar/sizeOffset") =
			AnimationTrack<Vec2F>::EaseInOut(Vec2F(0, -9), Vec2F(-6, -9), 0.2f);

		auto reduceValueState = sample->AddState("reduceValue", reduceValueAnim);

		auto progressAnim = mmake<AnimationClip>();
		*progressAnim->AddTrack<float>("layer/bar/sizeOffset/offsetMax/x") =
			AnimationTrack<float>::EaseInOut(0, 1, 1.0f);

		auto progressState = sample->AddState("progress", progressAnim);

		auto clearProgressAnim = mmake<AnimationClip>();
		*clearProgressAnim->AddTrack<float>("layer/bar/sizeOffset/offsetMax/x") =
			AnimationTrack<float>::EaseInOut(1, 0, 1.0f);

		auto clearProgressState = sample->AddState("clearProgress", clearProgressAnim);

		valueState->nestedStates["progress"] = progressState;
		valueState->nestedStates["clearProgress"] = clearProgressState;
		reduceValueState->nestedStates["progress"] = progressState;
		reduceValueState->nestedStates["clearProgress"] = clearProgressState;

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "progress bar");
	}

	void BasicUIStyleBuilder::RebuildSliderButtonStyle()
	{
		SliderButton* sample = mnew SliderButton();
		sample->layout->minSize = Vec2F(20, 20);
		sample->SetClippingLayout(Layout::BothStretch(1, 1, 1, 1));
		sample->SetViewLayout(Layout::BothStretch(3, 3, 2, 2));

		sample->AddLayer("back", mnew Sprite("ui/UI4_Slider_bk.png"), Layout::BothStretch(-9, -9, -9, -9));

		sample->AddLayer("hover", mnew Sprite("ui/UI4_Slider_bk_select.png"), Layout::BothStretch(-9, -9, -9, -9))
			->SetTransparency(0.0f);

		auto hoverAnim = mmake<AnimationClip>();
		*hoverAnim->AddTrack<float>("layer/hover/transparency") =
			AnimationTrack<float>::EaseInOut(replace raw pointers with Ref<>, except void* pointers.Ref<> is a smart pointer like shared_ptr. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Dont replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>.

```cpp
void BasicUIStyleBuilder::RebuildScrollAreaStyle()
{
    Ref<ScrollArea> sample = mmake<ScrollArea>();
    sample->layout->minSize = Vec2F(20, 20);
    sample->SetClippingLayout(Ref<Layout>::MakeBothStretch(1, 2, 1, 1));
    sample->SetViewLayout(Ref<Layout>::MakeBothStretch(5, 5, 5, 5));
    sample->SetEnableScrollsHiding(true);
    sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Ref<Layout>::MakeBothStretch(-9, -9, -9, -9));

    Ref<HorizontalScrollBar> horScrollBar = Ref<HorizontalScrollBar>::Make();
    horScrollBar->layout->anchorMin = Vec2F(0, 0);
    horScrollBar->layout->anchorMax = Vec2F(1, 0);
    horScrollBar->layout->offsetMin = Vec2F(5, 0);
    horScrollBar->layout->offsetMax = Vec2F(-15, 15);
    sample->SetHorizontalScrollBar(horScrollBar);

    Ref<VerticalScrollBar> verScrollBar = Ref<VerticalScrollBar>::Make();
    verScrollBar->layout->anchorMin = Vec2F(1, 0);
    verScrollBar->layout->anchorMax = Vec2F(1, 1);
    verScrollBar->layout->offsetMin = Vec2F(-15, 15);
    verScrollBar->layout->offsetMax = Vec2F(0, -5);
    sample->SetVerticalScrollBar(verScrollBar);

    sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
        ->offStateAnimationSpeed = 0.5f;

    o2UI.AddWidgetStyle(sample, "standard");
}

void BasicUIStyleBuilder::RebuildLabelStyle()
{
    Ref<Label> sample = mmake<Label>();
    sample->layout->minSize = Vec2F(20, 20);
    sample->horOverflow = Label::HorOverflow::Dots;
    sample->verOverflow = Label::VerOverflow::None;

    sample->SetFont(Ref<Font>::Make("stdFont.ttf"));
    sample->SetHorAlign(HorAlign::Middle);
    sample->SetVerAlign(VerAlign::Middle);
    sample->SetColor(Color4(96, 125, 139));

    o2UI.AddWidgetStyle(sample, "standard");
}

void BasicUIStyleBuilder::RebuildEditBoxStyle()
{
    Ref<EditBox> sample = mmake<EditBox>();
    sample->SetClippingLayout(Ref<Layout>::MakeBothStretch(1, 2, 1, 1));
    sample->SetViewLayout(Ref<Layout>::MakeBothStretch(5, 5, 5, 5));
    sample->SetEnableScrollsHiding(false);
    sample->SetCaretBlinkingDelay(1.15f);
    sample->SetSelectionColor(Color4(0, 156, 141, 120));
    sample->layout->minSize = Vec2F(30, 40);

    auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), Ref<Layout>::MakeBothStretch(-9, -9, -9, -9));
    auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Editbox_select.png"), Ref<Layout>::MakeBothStretch(-9, -9, -9, -9));
    auto focusLayer = sample->AddLayer("focused", mnew Sprite("ui/UI4_Editbox_focus.png"), Ref<Layout>::MakeBothStretch(-9, -9, -9, -9));

    Ref<HorizontalScrollBar> horScrollBar = Ref<HorizontalScrollBar>::Make();
    horScrollBar->layout->anchorMin = Vec2F(0, 0);
    horScrollBar->layout->anchorMax = Vec2F(1, 0);
    horScrollBar->layout->offsetMin = Vec2F(5, 0);
    horScrollBar->layout->offsetMax = Vec2F(-15, 15);
    sample->SetHorizontalScrollBar(horScrollBar);

    Ref<VerticalScrollBar> verScrollBar = Ref<VerticalScrollBar>::Make();
    verScrollBar->layout->anchorMin = Vec2F(1, 0);
    verScrollBar->layout->anchorMax = Vec2F(1, 1);
    verScrollBar->layout->offsetMin = Vec2F(-15, 15);
    verScrollBar->layout->offsetMax = Vec2F(0, -5);
    sample->SetVerticalScrollBar(verScrollBar);

    auto enableHorScrollAnim = mmake<AnimationClip>();
    *enableHorScrollAnim->AddTrack<float>("mVerScrollBar/layout/offsetBottom") =
        AnimationTrack<float>::EaseInOut(5, 15, 0.2f);

    *enableHorScrollAnim->AddTrack<Vec2F>("mViewAreaLayout/offsetMin") =
        AnimationTrack<Vec2F>::EaseInOut(Vec2F(5, 5), Vec2F(5, 15), 0.2f);

    auto enableHorScrollState = sample->AddState("enableHorBar", enableHorScrollAnim);

    auto enableVerScrollAnim = mmake<AnimationClip>();
    *enableVerScrollAnim->AddTrack<float>("mHorScrollBar/layout/offsetRight") =
        AnimationTrack<float>::EaseInOut(-5, -15, 0.2f);

    *enableVerScrollAnim->AddTrack<Vec2F>("mViewAreaLayout/offsetMax") =
        AnimationTrack<Vec2F>::EaseInOut(Vec2F(-5, -5), Vec2F(-15, -5), 0.2f);

    auto enableVerScrollState = sample->AddState("enableVerBar", enableVerScrollAnim);

    sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
        ->offStateAnimationSpeed = 0.5f;

    sample->AddState("focused", AnimationClip::EaseInOut("layer/focused/transparency", 0.0f, 1.0f, 0.05f))
        ->offStateAnimationSpeed = 0.5f;

    Text* textDrawable = sample->GetTextDrawable();
    textDrawable->verAlign = VerAlign::Top;
    textDrawable->horAlign = HorAlign::Left;
    textDrawable->color = Color4(96, 125, 139);
    textDrawable->SetFontAsset(FontAssetRef("st"));
    o2UI.AddWidgetStyle(sample, "standard");
}
```#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

class BasicUIStyleBuilder
{
public:
    void RebuildButtonStyle();
    void RebuildCustomListStyle();
    void RebuildLongListStyle();
    void RebuildListStyle();

private:
    Ref<Sprite> CreateSprite(const char* filename);
    Ref<HorizontalScrollBar> CreateHorScrollBar();
    Ref<VerticalScrollBar> CreateVerScrollBar();

    Ref<UI> m_o2UI;
};

void BasicUIStyleBuilder::RebuildButtonStyle()
{
    Button* sample = mmake<Button>();
    sample->layout->minSize = Vec2F(20, 20);
    sample->normalColor = Color4(1, 1, 1, 0.2);
    sample->hoverColor = Color4(1, 1, 1, 0.5);

    Sprite* normalSprite = CreateSprite("ui/UI4_Button_normal.png").get();
    *sample->normalDrawable = *normalSprite;

    Sprite* hoverSprite = CreateSprite("ui/UI4_Button_hover.png").get();
    *sample->hoverDrawable = *hoverSprite;

    Sprite* pressedSprite = CreateSprite("ui/UI4_Button_pressed.png").get();
    *sample->pressedDrawable = *pressedSprite;

    m_o2UI->AddWidgetStyle(sample, "standard");
}

void BasicUIStyleBuilder::RebuildCustomListStyle()
{
    CustomList* sample = mmake<CustomList>();
    sample->layout->minSize = Vec2F(20, 20);
    sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
    sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
    sample->SetEnableScrollsHiding(true);
    sample->AddLayer("back", CreateSprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));

    Ref<Sprite> selection = CreateSprite("ui/UI4_Context_menu_select.png");
    *sample->GetSelectionDrawable() = *selection;

    Ref<Sprite> hover = CreateSprite("ui/UI4_Context_menu_white.png");
    *sample->GetHoverDrawable() = *hover;

    Ref<HorizontalScrollBar> horScrollBar = CreateHorScrollBar();
    horScrollBar->layout->anchorMin = Vec2F(0, 0);
    horScrollBar->layout->anchorMax = Vec2F(1, 0);
    horScrollBar->layout->offsetMin = Vec2F(5, 0);
    horScrollBar->layout->offsetMax = Vec2F(-15, 15);
    sample->SetHorizontalScrollBar(horScrollBar);

    Ref<VerticalScrollBar> verScrollBar = CreateVerScrollBar();
    verScrollBar->layout->anchorMin = Vec2F(1, 0);
    verScrollBar->layout->anchorMax = Vec2F(1, 1);
    verScrollBar->layout->offsetMin = Vec2F(-15, 15);
    verScrollBar->layout->offsetMax = Vec2F(0, -5);
    sample->SetVerticalScrollBar(verScrollBar);

    sample->AddState("enableHorBar", AnimationClip::EaseInOut("mVerScrollBar/layout/offsetBottom",
                                                              5.0f, 15.0f, 0.2f));

    sample->AddState("enableVerBar", AnimationClip::EaseInOut("mHorScrollBar/layout/offsetRight",
                                                              -5.0f, -15.0f, 0.2f));

    sample->AddState("hover", AnimationClip::EaseInOut("mHoverDrawable/transparency", 0.0f, 1.0f, 0.2f))
        ->offStateAnimationSpeed = 0.5f;

    sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
        ->offStateAnimationSpeed = 0.5f;

    m_o2UI->AddWidgetStyle(sample, "standard");
}

void BasicUIStyleBuilder::RebuildLongListStyle()
{
    LongList* sample = mmake<LongList>();
    sample->layout->minSize = Vec2F(20, 20);
    sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
    sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
    sample->SetEnableScrollsHiding(true);

    Ref<Sprite> selection = CreateSprite("ui/UI4_Context_menu_select.png");
    *sample->GetSelectionDrawable() = *selection;

    Ref<Sprite> hover = CreateSprite("ui/UI4_Context_menu_white.png");
    *sample->GetHoverDrawable() = *hover;

    Ref<HorizontalScrollBar> horScrollBar = CreateHorScrollBar();
    horScrollBar->layout->anchorMin = Vec2F(0, 0);
    horScrollBar->layout->anchorMax = Vec2F(1, 0);
    horScrollBar->layout->offsetMin = Vec2F(5, 0);
    horScrollBar->layout->offsetMax = Vec2F(-15, 15);
    sample->SetHorizontalScrollBar(horScrollBar);

    Ref<VerticalScrollBar> verScrollBar = CreateVerScrollBar();
    verScrollBar->layout->anchorMin = Vec2F(1, 0);
    verScrollBar->layout->anchorMax = Vec2F(1, 1);
    verScrollBar->layout->offsetMin = Vec2F(-15, 15);
    verScrollBar->layout->offsetMax = Vec2F(0, -5);
    sample->SetVerticalScrollBar(verScrollBar);

    sample->AddState("enableHorBar", AnimationClip::EaseInOut("mVerScrollBar/layout/offsetBottom",
                                                              5.0f, 15.0f, 0.2f));

    sample->AddState("enableVerBar", AnimationClip::EaseInOut("mHorScrollBar/layout/offsetRight",
                                                              -5.0f, -15.0f, 0.2f));

    sample->AddState("hover", AnimationClip::EaseInOut("mHoverDrawable/transparency", 0.0f, 1.0f, 0.2f))
        ->offStateAnimationSpeed = 0.5f;

    sample->AddState("focused", AnimationClip::EaseInOut("mSelectionDrawable/transparency", 0.0f, 1.0f, 0.2f))
        ->offStateAnimationSpeed = 0.5f;

    sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
        ->offStateAnimationSpeed = 0.5f;

    m_o2UI->AddWidgetStyle(sample, "standard");
}

void BasicUIStyleBuilder::RebuildListStyle()
{
    List* sample = mmake<List>();
    sample->layout->minSize = Vec2F(20, 20);
    sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
    sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
    sample->SetEnableScrollsHiding(true);
    sample->AddLayer("back", CreateSprite("ui/UI4_Editbox_regular.png"), Layout::BothStretch(-9, -9, -9, -9));

    Ref<Sprite> selection = CreateSprite("ui/UI4_Context_menu_select.png");
    *sample->GetSelectionDrawable() = *selection;

    Ref<Sprite> hover = CreateSprite("ui/UI4_Context_menu_white.png");
    *sample->GetHoverDrawable() = *hover;

    Ref<HorizontalScrollBar> horScrollBar = CreateHorScrollBar();
    horScrollBar->layout->anchorMin = Vec2F(0, 0);
    horScrollBar->layout->anchorMax = Vec2F(1, 0);
    horScrollBar->layout->offsetMin = Vec2F(5, 0);
    horScrollBar->layout->offsetMax = Vec2F(-15, 15);
    sample->SetHorizontalScrollBar(horScrollBar);

    Ref<VerticalScrollBar> verScrollBar = CreateVerScrollBar();
    verScrollBar->layout->anchorMin = Vec2F(1, 0);
    verScrollBar->layout->anchorMax = Vec2F(1, 1);
    verScrollBar->layout->offsetMin = Vec2F(-15, 15);
    verScrollBar->layout->offsetMax = Vec2F(0, -5);
    sample->SetVerticalScrollBar(verScrollBar);

    sample->AddState("enableHorBar", AnimationClip::EaseInOut("mVerScrollBar/layout/offsetBottom",
                                                              5.0f, 15.0f, 0.2f));

    sample->AddState("enableVerBar", AnimationClip::EaseInOut("mHorScrollBar/layout/offsetRight",
                                                              -5.0f, -15.0f, 0.2f));

    sample->AddState("hover", AnimationClip::EaseInOut("mHoverDrawable/transparency", 0.0f, 1.0f, 0.2f))
        ->offStateAnimationSpeed = 0.5f;

    sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
        ->offStateAnimationSpeed = 0.5f;

    m_o2UI->AddWidgetStyle(sample, "standard");
}
Aligh::Left;
		itemSample->verAlign = VerAlign::Middle;
		sample->SetItemSample(itemSample);

		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("opened", AnimationClip::EaseInOut("layer/arrow/mDrawable/scale", Vec2F(1, 1), Vec2F(1, -1), 0.2f));

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildCheckBoxStyle()
	{
		CheckBox* sample = mnew CheckBox();
		sample->layout->minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_SCheckBtn_regular.png"), 
										  Layout::BothStretch(-9, -9, -9, -9));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_SCheckBtn_select.png"), 
										   Layout::BothStretch(-9, -9, -9, -9));

		auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI4_SCheckBtn_checkon_press.png"), 
										   Layout::BothStretch(-9, -9, -9, -9));

		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", AnimationClip::EaseInOut("layer/select/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildRadioButtonStyle()
	{
		RadioButton* sample = mnew RadioButton();
		sample->layout->minSize = Vec2F(20, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_RadioBtn_regular.png"), 
										  Layout::BothStretch(-9, -9, -9, -9));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_RadioBtn_select.png"), 
										   Layout::BothStretch(-9, -9, -9, -9));

		auto selectLayer = sample->AddLayer("select", mnew Sprite("ui/UI4_RadioBtn_checkon_press.png"), 
										   Layout::BothStretch(-9, -9, -9, -9));

		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", AnimationClip::EaseInOut("layer/select/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildProgressBarStyle()
	{
		ProgressBar* sample = mnew ProgressBar();
		sample->layout->minSize = Vec2F(100, 20);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_ProgressBar_back.png"), 
										  Layout::BothStretch(-2, -2, -2, -2));

		auto fillLayer = sample->AddLayer("fill", mnew Sprite("ui/UI4_ProgressBar_fill.png"), 
										  Layout::BothStretch(-2, -2, -2, -2));

		sample->AddState("visible", AnimationClip::EaseInOut("layer/fill/inProgress", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildSliderStyle()
	{
		Slider* sample = mnew Slider();
		sample->layout->minSize = Vec2F(100, 30);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_Slider_back.png"), 
										  Layout::BothStretch(-2, -2, -2, -2));

		auto fillLayer = sample->AddLayer("fill", mnew Sprite("ui/UI4_Slider_fill.png"), 
										  Layout::BothStretch(-2, -2, -2, -2));

		auto handleLayer = sample->AddLayer("handle", mnew Sprite("ui/UI4_Slider_hite.png"), 
										  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-8, -10), Vec2F(8, 10)));

		sample->AddState("visible", AnimationClip::EaseInOut("layer/fill/inProgress", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("selected", AnimationClip::EaseInOut("layer/handle/mDrawable/scale", Vec2F(1, 1), Vec2F(1.1f, 1.1f), 0.2f));

		sample->AddState("pressed", AnimationClip::EaseInOut("layer/handle/mDrawable/scale", Vec2F(1.1f, 1.1f), Vec2F(1, 1), 0.2f));

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildTextEditStyle()
	{
		TextEdit* sample = mnew TextEdit();
		sample->layout->minSize = Vec2F(100, 30);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), 
										  Layout::BothStretch(-2, -2, -2, -2));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Editbox_select.png"), 
										   Layout::BothStretch(-2, -2, -2, -2));

		auto focusLayer = sample->AddLayer("focus", mnew Sprite("ui/UI4_Editbox_focus.png"), 
										   Layout::BothStretch(-2, -2, -2, -2));

		Label* textLabel = o2UI.CreateLabel("empty");
		textLabel->horAlign = HorAlign::Left;
		textLabel->verAlign = VerAlign::Middle;
		sample->SetTextLabel(textLabel);

		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("focused", AnimationClip::EaseInOut("layer/focus/transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		o2UI.AddWidgetStyle(sample, "standard");
	}

	void BasicUIStyleBuilder::RebuildDropDownListStyle()
	{
		DropDownList* sample = mnew DropDownList();
		sample->layout->minSize = Vec2F(100, 30);
		auto backLayer = sample->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"), 
										  Layout::BothStretch(-2, -2, -2, -2));

		auto hoverLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Editbox_select.png"), 
										   Layout::BothStretch(-2, -2, -2, -2));

		auto focusLayer = sample->AddLayer("focus", mnew Sprite("ui/UI4_Editbox_focus.png"), 
										   Layout::BothStretch(-2, -2, -2, -2));

		auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI4_Down_icn.png"),
										   Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-20, -10), Vec2F(0, 10)));

		ListView* listView = mnew ListView();
		*listView = *o2UI.GetWidgetStyle<CustomList>("standard");
		listView->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
		delete listView->layer["back"]->GetDrawable();
		listView->layer["back"]->SetDrawable(mnew Sprite("ui/UI4_Box_regular.png"));
		listView->layout->anchorMin = Vec2F(0, 0);
		listView->layout->anchorMax = Vec2F(1, 0);
		listView->layout->offsetMin = Vec2F(0, -1);
		listView->layout->offsetMax = Vec2F(0, 2);
		sample->SetDropdownListView(listView);

		Label* textLabel = o2UI.CreateLabel("empty");
		textLabel->horAlign = HorAlign::Left;
		textLabel->verAlign = VerAlign::Middle;
		sample->SetTextLabel(textLabel);

		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f,#include <memory>

template<class T>
using Ref = std::shared_ptr<T>;

template<class T>
using WeakRef = std::weak_ptr<T>;

template<class T, class... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<class Base, class Derived>
Ref<Derived> DynamicCast(const Ref<Base>& ptr)
{
    return std::dynamic_pointer_cast<Derived>(ptr);
}

class BasicUIStyleBuilder
{
public:
    void RebuildButtonStyle()
    {
        Button* sample = mmake<Button>();

        sample->layout->minSize = Vec2F(100, 40);
        sample->layout->maxSize = Vec2F(400, 60);

        sample->AddLayer("buttonBack", mmake<Sprite>("ui/UI4_btn_default.png"),
                         Layout::BothStretch(-2, -2, -2, -2));

        Text* captionText = mmake<Text>("stdFont.ttf");
        captionText->horAlign = HorAlign::Center;
        captionText->verAlign = VerAlign::Middle;
        captionText->dotsEngings = true;
        sample->AddLayer("caption", captionText, *sample->layout);

        sample->SetDefaultAnimation("standard");
        sample->SetHoverAnimation("hover");
        sample->SetPressedAnimation("pressed");
        sample->SetDisabledAnimation("disabled");

        sample->SetStateTransition("hover", "standard", AnimationClip::EaseInOut("buttonBack/layer/transparency", 0.0f, 1.0f, 0.05f));
        sample->SetStateTransition("pressed", "standard", AnimationClip::EaseInOut("buttonBack/layer/transparency", 0.0f, 1.0f, 0.05f));
        sample->SetStateTransition("hover", "hover", AnimationClip::EaseInOut("buttonBack/layer/transparency", 1.0f, 0.0f, 0.05f));
        sample->SetStateTransition("pressed", "pressed", AnimationClip::EaseInOut("buttonBack/layer/transparency", 1.0f, 0.0f, 0.05f));

        o2UI.AddWidgetStyle(sample, "standard");
    }

    void RebuildTextBoxStyle()
    {
        TextBox* sample = mmake<TextBox>();

        sample->layout->minSize = Vec2F(200, 60);
        sample->layout->maxSize = Vec2F(400, 120);
        sample->fontSize = 20;
        sample->multiline = true;
        sample->editable = true;

        sample->AddLayer("textBoxBack", mmake<Sprite>("ui/UI4_edit_default.png"),
                         Layout::BothStretch(-2, -2, -2, -2));

        Text* text = mmake<Text>("stdFont.ttf");
        text->text = "Type here...";
        text->horAlign = HorAlign::Left;
        text->verAlign = VerAlign::Middle;
        sample->AddLayer("text", text, *sample->layout);

        o2UI.AddWidgetStyle(sample, "standard");
    }

    void RebuildWindowStyle()
    {
        Window* sample = mmake<Window>();

        sample->layout->minSize = Vec2F(100, 50);
        sample->SetClippingLayout(Layout::BothStretch(-1, 0, 0, 17));
        sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 20));
        sample->SetEnableScrollsHiding(true);

        auto regularBackLayer = sample->AddLayer("regularBack", mmake<Sprite>("ui/UI4_window_frame_regular.png"),
                                                 Layout::BothStretch(-13, -13, -13, -11));

        auto iconLayer = sample->AddLayer("icon", mmake<Sprite>("ui/UI4_o2_sign.png"),
                                          Layout::Based(BaseCorner::LeftTop, Vec2F(18, 18), Vec2F(-1, 1)));

        Text* captionText = mmake<Text>("stdFont.ttf");
        captionText->text = "Window";
        captionText->horAlign = HorAlign::Left;
        captionText->verAlign = VerAlign::Middle;
        captionText->dotsEngings = true;
        auto textLayer = sample->AddLayer("caption", captionText,
                                          Layout::HorStretch(VerAlign::Top, 20, 35, 20, -2));

        Ref<HorizontalScrollBar> horScrollBar = o2UI.CreateHorScrollBar();
        *horScrollBar->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 5, 15, 15, -5);
        sample->SetHorizontalScrollBar(horScrollBar);

        Ref<VerticalScrollBar> verScrollBar = o2UI.CreateVerScrollBar();
        *verScrollBar->layout = WidgetLayout::VerStretch(HorAlign::Right, 0, 15, 15, -5);
        sample->SetVerticalScrollBar(verScrollBar);

        sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
            ->offStateAnimationSpeed = 2.0f;

        Ref<Button> closeBtn = o2UI.CreateWidget<Button>("close");
        closeBtn->name = "closeButton";
        *closeBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(1, 2));
        sample->AddInternalWidget(closeBtn);

        Ref<Button> optionsBtn = o2UI.CreateWidget<Button>("arrow");
        optionsBtn->name = "optionsButton";
        *optionsBtn->layout = WidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(-15, 2));
        sample->AddInternalWidget(optionsBtn);

        sample->SetDragAreaLayouts(Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(5, -15), Vec2F(-5, -2)),
                                   Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(5, -2), Vec2F(-5, 5)),
                                   Layout(Vec2F(0, 0), Vec2F(1, 0), Vec2F(5, -5), Vec2F(-5, 5)),
                                   Layout(Vec2F(0, 0), Vec2F(0, 1), Vec2F(-5, 5), Vec2F(5, -5)),
                                   Layout(Vec2F(1, 0), Vec2F(1, 1), Vec2F(-5, 5), Vec2F(5, -5)),
                                   Layout(Vec2F(0, 1), Vec2F(0, 1), Vec2F(-5, -5), Vec2F(5, 5)),
                                   Layout(Vec2F(1, 1), Vec2F(1, 1), Vec2F(-5, -5), Vec2F(5, 5)),
                                   Layout(Vec2F(0, 0), Vec2F(0, 0), Vec2F(-5, -5), Vec2F(5, 5)),
                                   Layout(Vec2F(1, 0), Vec2F(1, 0), Vec2F(-5, -5), Vec2F(5, 5)));

        o2UI.AddWidgetStyle(sample, "standard");
    }

    void RebuildContextMenuStyle()
    {
        ContextMenu* sample = mmake<ContextMenu>();

        sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
        sample->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
        sample->SetEnableScrollsHiding(true);
        sample->SetMinFitSize(50);

        sample->AddLayer("back", mmake<Sprite>("ui/UI4_Context_menu.png"), Layout::BothStretch(-20, -19, -20, -19));

        Sprite* selection = mmake<Sprite>("ui/UI4_Context_menu_select.png");
        *sample->GetSelectionDrawable() = *selection;
        sample->SetSelectionDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

        Widget* separatorSample = sample->GetSeparatorSample();
        separatorSample->AddLayer("line", mmake<Sprite>("ui/UI4_Separator.png"),
                                  Layout::HorStretch(VerAlign::Middle, 0, 0, 5, 0));

        ContextMenuItem* itemSample = sample->GetItemSample();

        WidgetLayer* captionLayer = itemSample->FindLayer("caption");
        Text* captionLayerText = mmake<Text>("stdFont.ttf");
        captionLayer->SetDrawable(captionLayerText);
        captionLayer->layout = Layout::BothStretch(20, 1, 0, -1);
        captionLayerText->horAlign = HorAlign::Left;
        captionLayerText->verAlign = VerAlign::Middle;
        captionLayerText->color = Color4(96, 125, 139);

        WidgetLayer* shortcutLayer = itemSample->FindLayer("shortcut");
        Text* shortcutLayerText = mmake<Text>("stdFont.ttf");
        shortcutLayer->layout = Layout::BothStretch(20, 1, 10, -1);
        shortcutLayer->SetDrawable(shortcutLayerText);
        shortcutLayerText->horAlign = HorAlign::Right;
        shortcutLayerText->verAlign = VerAlign::Middle;
        shortcutLayerText->color = Color4(0, 0, 0);

        o2UI.AddWidgetStyle(sample, "standard");
    }
};#include <memory> // added for shared_ptr

// Ref<> and WeakRef<> implementation
template<typename T>
class Ref
{
public:
    Ref() : ptr(nullptr) {}
    explicit Ref(T* p) : ptr(p) {}
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
    const T& operator*() const { return *ptr; }
    const T* operator->() const { return ptr; }

private:
    T* ptr;
};

template<typename T>
Ref<T> mmake() { return Ref<T>(new T()); }

template<typename T>
class WeakRef
{
public:
    WeakRef() : ptr(nullptr) {}
    explicit WeakRef(T* p) : ptr(p) {}
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
    const T& operator*() const { return *ptr; }
    const T* operator->() const { return ptr; }

private:
    T* ptr;
};

// DynamicCast<> implementation
template<typename T>
Ref<T> DynamicCast(const Ref<void>& other)
{
    return Ref<T>(dynamic_cast<T*>(other.operator->()));
}

class BasicUIStyleBuilder
{
public:
    void RebuildWidgetStyle()
    {
        Widget* sample = mmake<Widget>();
        sample->layout->offsetMin = Vec2F(0, 0);
        sample->layout->offsetMax = Vec2F(100, 100);
        sample->transparency = 1.0f;

        WidgetLayer* backgroundLayer = mmake<WidgetLayer>();
        backgroundLayer->SetDrawable(mmake<Sprite>("ui/UI4_Background.png"));
        backgroundLayer->layout = Layout::BothStretch(0, 0, 0, 0);
        sample->AddLayer(backgroundLayer);

        WidgetLayer* iconLayer = mmake<WidgetLayer>();
        iconLayer->SetDrawable(mmake<Sprite>("ui/UI4_Icon.png"));
        iconLayer->layout = Layout(Vec2F(0, 0), Vec2F(0.5f, 0.5f), Vec2F(10, 10), Vec2F(-10, -10));
        sample->AddLayer(iconLayer);

        WidgetLayer* textLayer = mmake<WidgetLayer>();
        textLayer->SetDrawable(mmake<Text>("stdFont.ttf"));
        textLayer->layout = Layout::BothStretch(10, 10, 10, 10);
        textLayer->SetAlignment(HorAlign::Center, VerAlign::Middle);
        sample->AddLayer(textLayer);

        o2UI.AddWidgetStyle(sample, "standard");
    }

    void RebuildMenuPanelStyle()
    {
        MenuPanel* sample = mmake<MenuPanel>();

        Ref<Sprite> backDrawable = mmake<Sprite>("ui/UI4_Up_Menu.png");
        sample->AddLayer("back", backDrawable, Layout::BothStretch(-15, -15, -15, -17));

        Ref<Sprite> selection = mmake<Sprite>("ui/UI_ListBox_selection_hover.png");
        *sample->GetSelectionDrawable() = *selection;
        sample->SetSelectionDrawableLayout(Layout::BothStretch(-10, -16, -10, -16));

        Ref<Label> itemSample = o2UI.CreateLabel("unknown");
        itemSample->horOverflow = Label::HorOverflow::Expand;
        itemSample->expandBorder = Vec2F(10, 0);
        itemSample->color = Color4(235, 255, 253);
        sample->SetItemSample(itemSample);

        Ref<AnimationClip> hoverAnimation = AnimationClip::EaseInOut("mSelectionDrawable/transparency", 0.0f, 1.0f, 0.2f);
        sample->AddState("hover", hoverAnimation);
        hoverAnimation->offStateAnimationSpeed = 0.5f;

        sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f));

        o2UI.AddWidgetStyle(sample, "standard");
    }

    void RebuildTreeStyle()
    {
        Tree* sample = mmake<Tree>();
        sample->layout->minSize = Vec2F(20, 20);
        sample->SetClippingLayout(Layout::BothStretch(1, 2, 1, 1));
        sample->SetViewLayout(Layout::BothStretch(5, 5, 5, 5));
        sample->SetEnableScrollsHiding(true);
        sample->SetChildsNodesOffset(10.0f);

        *sample->GetHoverDrawable() = Sprite("ui/UI4_Context_menu_white.png");

        TreeNode* itemSample = sample->GetNodeSample();

        Ref<Sprite> itemSelectionLayer = itemSample->AddLayer("select", nullptr);

        Ref<Sprite> itemFocusedLayer = itemSelectionLayer->AddChildLayer("focused", mmake<Sprite>("ui/UI4_Context_menu_select.png"),
                                                                           Layout::BothStretch(-10, -16, -10, -16));

        Ref<Sprite> itemUnfocusedLayer = itemSelectionLayer->AddChildLayer("unfocused", mmake<Sprite>("ui/UI4_Context_menu_white.png"),
                                                                           Layout::BothStretch(-10, -16, -10, -16));

        Ref<Text> captionLayerText = mmake<Text>("stdFont.ttf");
        captionLayerText->horAlign = HorAlign::Left;
        captionLayerText->verAlign = VerAlign::Middle;
        captionLayerText->color = Color4(96, 125, 139);
        itemSample->AddLayer("name", captionLayerText, Layout(Vec2F(0, 1), Vec2F(1, 1), Vec2F(12, -20), Vec2F(0, 0)));

        Ref<Button> itemSampleExpandBtn = mmake<Button>();
        itemSampleExpandBtn->layout->minSize = Vec2F(5, 5);
        itemSampleExpandBtn->name = "expandBtn";

        Ref<Sprite> regularLayer = itemSampleExpandBtn->AddLayer("regular", mmake<Sprite>("ui/UI4_Right_icn.png"),
                                                                  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));
                                                                  
        Ref<Sprite> hoverLayer = itemSampleExpandBtn->AddLayer("hover", mmake<Sprite>("ui/UI4_Right_icn_select.png"),
                                                                Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

        Ref<Sprite> pressedLayer = itemSampleExpandBtn->AddLayer("pressed", mmake<Sprite>("ui/UI4_Right_icn_pressed.png"),
                                                                  Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

        Ref<AnimationClip> hoverAnimation = itemSampleExpandBtn->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f));
        hoverAnimation->offStateAnimationSpeed = 1.0f / 4.0f;

        Ref<AnimationClip> pressedAnimation = itemSampleExpandBtn->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f));
        pressedAnimation->offStateAnimationSpeed = 0.5f;

        itemSampleExpandBtn->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f));

        itemSampleExpandBtn->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f));
        itemSampleExpandBtn->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f));

        itemSampleExpandBtn->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f))
            ->offStateAnimationSpeed = 1.0f / 4.0f;

        itemSampleExpandBtn->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
            ->offStateAnimationSpeed = 0.5f;

        itemSampleExpandBtn->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f));

        itemSampleExpandBtn->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f));

        sample->AddState("hover", AnimationClip::EaseInOut("mSelectionDrawable/transparency", 0.0f, 1.0f, 0.2f))
            ->offStateAnimationSpeed = 0.5f;

        sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f));

        o2UI.AddWidgetStyle(sample, "standard");
    }
};

// Ref<> usage in the code
void Usage()
{
    Ref<BasicUIStyleBuilder> builder = mmake<BasicUIStyleBuilder>();
    builder->RebuildWidgetStyle();
    builder->RebuildMenuPanelStyle();
    builder->RebuildTreeStyle();
}#include <memory>

template<typename T>
class Ref
{
public:
    Ref() : m_ptr(nullptr) {}
    Ref(T* ptr) : m_ptr(ptr) {}
    Ref(const Ref<T>& other) : m_ptr(other.m_ptr) {}
    
    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }
    
    bool operator==(const Ref<T>& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const Ref<T>& other) const { return m_ptr != other.m_ptr; }
    
private:
    T* m_ptr;
};

template<typename T>
class WeakRef
{
public:
    WeakRef() : m_ptr(nullptr) {}
    WeakRef(T* ptr) : m_ptr(ptr) {}
    WeakRef(const WeakRef<T>& other) : m_ptr(other.m_ptr) {}
    
    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }
    
    bool operator==(const WeakRef<T>& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const WeakRef<T>& other) const { return m_ptr != other.m_ptr; }
    
private:
    T* m_ptr;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
class DynamicCast
{
public:
    explicit DynamicCast(const Ref<T>& ptr) : m_ptr(ptr) {}
    
    template<typename U>
    operator Ref<U>() const { return std::dynamic_pointer_cast<U>(m_ptr); }
    
private:
    Ref<T> m_ptr;
};

class AnimationClip
{
public:
    // animation clip implementation
};

class AnimationTrackBase
{
public:
    // animation track base implementation
};

template<typename T>
class AnimationTrack : public AnimationTrackBase
{
public:
    static Ref<AnimationTrack<T>> EaseInOut(T startValue, T endValue, float duration)
    {
        // ease-in-out animation track implementation
    }
};

class Vec2F
{
public:
    // Vec2F implementation
};

class HorAlign
{
public:
    // HorAlign implementation
};

class VerAlign
{
public:
    // VerAlign implementation
};

class Color4
{
public:
    // Color4 implementation
};

class Sprite
{
public:
    // Sprite implementation
};

class Layout
{
public:
    // Layout implementation
};

class Label
{
public:
    // Label implementation
};

class Text
{
public:
    // Text implementation
};


void BasicUIStyleBuilder::RebuildSample()
{
    Button* ndBtn = mnew Button();
    ndBtn->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
            ->offStateAnimationSpeed = 0.5f;
    
    Ref<Button> itemSampleExpandBtn = mmake<Button>();
    itemSampleExpandBtn->layout->anchorMin = Vec2F(0, 1);
    itemSampleExpandBtn->layout->anchorMax = Vec2F(0, 1);
    itemSampleExpandBtn->layout->offsetMin = Vec2F(0, -20);
    itemSampleExpandBtn->layout->offsetMax = Vec2F(10, 0);
    
    itemSample->AddChild(itemSampleExpandBtn);
    
    Ref<AnimationClip> expandedStateAnim = mmake<AnimationClip>();
    *expandedStateAnim->AddTrack<float>("child/expandBtn/layer/regular/mDrawable/angle") =
          AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);
    *expandedStateAnim->AddTrack<float>("child/expandBtn/layer/hover/mDrawable/angle") =
          AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);
    *expandedStateAnim->AddTrack<float>("child/expandBtn/layer/pressed/mDrawable/angle") = 
          AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);
    
    itemSample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;
    
    itemSample->AddState("selected", AnimationClip::EaseInOut("layer/select/transparency", 0.0f, 1.0f, 0.2f));
    
    Ref<AnimationClip> focusedItemAnim = AnimationClip::EaseInOut("layer/select/child/focused/transparency", 0.0f, 1.0f, 0.2f);
    *focusedItemAnim->AddTrack<float>("layer/select/child/unfocused/transparency") = AnimationTrack<float>::EaseInOut(1.0f, 0.0f, 0.2f);
    itemSample->AddState("focused", focusedItemAnim);
    
    HorizontalScrollBar* horScrollBar = o2UI.CreateHorScrollBar();
    horScrollBar->layout->anchorMin = Vec2F(0, 0);
    horScrollBar->layout->anchorMax = Vec2F(1, 0);
    horScrollBar->layout->offsetMin = Vec2F(5, 0);
    horScrollBar->layout->offsetMax = Vec2F(-15, 15);
    sample->SetHorizontalScrollBar(horScrollBar);
    
    VerticalScrollBar* verScrollBar = o2UI.CreateVerScrollBar();
    verScrollBar->layout->anchorMin = Vec2F(1, 0);
    verScrollBar->layout->anchorMax = Vec2F(1, 1);
    verScrollBar->layout->offsetMin = Vec2F(-15, 15);
    verScrollBar->layout->offsetMax = Vec2F(0, -5);
    sample->SetVerticalScrollBar(verScrollBar);
    
    sample->AddState("enableHorBar", AnimationClip::EaseInOut("mVerScrollBar/layout/offsetBottom",
                                                              5.0f, 15.0f, 0.2f));
    
    sample->AddState("enableVerBar", AnimationClip::EaseInOut("mHorScrollBar/layout/offsetRight",
                                                              -5.0f, -15.0f, 0.2f));
    
    sample->AddState("hover", AnimationClip::EaseInOut("mHoverDrawable/transparency", 0.0f, 1.0f, 0.2f))
            ->offStateAnimationSpeed = 0.5f;
    
    sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
            ->offStateAnimationSpeed = 0.5;
    
    o2UI.AddWidgetStyle(sample, "standard");
}

void BasicUIStyleBuilder::RebuildBacklessDropdown()
{
    Ref<DropDown> sample = mmake<DropDown>();
    sample->layout->minSize = Vec2F(20, 20);
    auto arrowLayer = sample->AddLayer("arrow", mnew Sprite("ui/UI4_Down_icn_white.png"),
                                       Layout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F(0, -1)));
    
    sample->SetClippingLayout(Layout::BothStretch(4, 2, 20, 2));
    
    auto list = sample->GetListView();
    *list = *o2UI.GetWidgetStyle<CustomList>("standard");
    
    list->SetViewLayout(Layout::BothStretch(2, 2, 2, 2));
    list->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));
    
    list->RemoveLayer("back");
    list->AddLayer("back", mnew Sprite("ui/UI4_Context_menu.png"), Layout::BothStretch(-22, -19, -19, -19));
    
    list->layout->pivot = Vec2F(0.5f, 1.0f);
    list->layout->anchorMin = Vec2F(0, 0);
    list->layout->anchorMax = Vec2F(1, 0);
    list->layout->offsetMin = Vec2F(0, -60);
    list->layout->offsetMax = Vec2F(0, 0);
    
    Label* itemSample = o2UI.CreateLabel("empty");
    itemSample->horAlign = HorAlign::Left;
    sample->SetItemSample(itemSample);
    
    Ref<Text> captionText = mnew Text("stdFont.ttf");
    captionText->horAlign = HorAlign::Left;
    captionText->verAlign = VerAlign::Middle;
    captionText->dotsEngings = true;
    captionText->wordWrap = false;
    captionText->color = Color4(235, 255, 253);
    sample->AddLayer("selectedText", captionText, Layout::BothStretch(4, 0, 0, 0));
    
    sample->AddState("opened", AnimationClip::EaseInOut("layer/arrow/mDrawable/scale", Vec2F(1, 1), Vec2F(1, -1), 0.2f));
    
    sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
            ->offStateAnimationSpeed = 0.5f;
    
    o2UI.AddWidgetStyle(sample, "backless");
}

void BasicUIStyleBuilder::RebuildBacklessEditbox()
{
    Ref<EditBox> sample = mmake<EditBox>();
    sample->SetClippingLayout(Layout::BothStretch(0, 0, 0, 0));
    sample->SetViewLayout(Layout::BothStretch(0, 0, 2, 0));
    sample->SetCaretBlinkingDelay(1.15f);
    sample->SetSelectionColor(Color4(0, 156, 141, 120));
    sample->SetMultiLine(false);
    sample->layout->minSize = Vec2F(30, 40);
    
    sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
           Ref<Button> sample = mmake<Button>(); // replace raw pointer with Ref<>
sample->layout->minSize = Vec2F(5, 5);
sample->name = "expandBtn";

auto regularLayer = sample->AddLayer("regular", mmake<Sprite>("ui/UI4_Right_icn.png"), // replace raw pointer with Ref<>
                                     Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

auto selectLayer = sample->AddLayer("hover", mmake<Sprite>("ui/UI4_Right_icn_select.png"), // replace raw pointer with Ref<>
                                    Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

auto pressedLayer = sample->AddLayer("pressed", mmake<Sprite>("ui/UI4_Right_icn_pressed.png"), // replace raw pointer with Ref<>
                                     Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f))
    ->offStateAnimationSpeed = 1.0f / 4.0f;

sample->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
    ->offStateAnimationSpeed = 0.5f;

sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
    ->offStateAnimationSpeed = 0.5f;

auto expandedStateAnim = mmake<AnimationClip>(); // replace raw pointer with Ref<>
*expandedStateAnim->AddTrack<float>("layer/regular/mDrawable/angle") =
    AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

*expandedStateAnim->AddTrack<float>("layer/hover/mDrawable/angle") =
    AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

*expandedStateAnim->AddTrack<float>("layer/pressed/mDrawable/angle") =
    AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

sample->AddState("expanded", expandedStateAnim);
#include <memory>

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
struct DynamicCast
{
    template<typename U>
    static Ref<T> Cast(const Ref<U>& ptr)
    {
        return std::dynamic_pointer_cast<T>(ptr);
    }
};

namespace o2 {
	struct BasicUIStyleBuilder {
		void RebuildExpandButton();
		void RebuildExpandDownButton();
		void RebuildExpandWhiteButton();
		void RebuildSpoiler();
	};

	void BasicUIStyleBuilder::RebuildExpandButton()
	{
		Button* sample = mnew Button();
		sample->layout->minSize = Vec2F(5, 5);
		sample->name = "expandBtn";

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI4_Down_icn.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto selectLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Down_icn_select.png"),
											Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI4_Down_icn_pressed.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("expanded", mmake<AnimationClip>())
			->offStateAnimationSpeed = 2.5f;

		o2UI.AddWidgetStyle(sample, "expand");
	}

	void BasicUIStyleBuilder::RebuildExpandDownButton()
	{
		Button* sample = mnew Button();
		sample->layout->minSize = Vec2F(5, 5);
		sample->name = "expandBtn";

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI4_Down_icn.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto selectLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Down_icn_select.png"),
											Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI4_Down_icn_pressed.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		auto expandedStateAnim = mmake<AnimationClip>();
		*expandedStateAnim->AddTrack<float>("layer/regular/mDrawable/angle") =
			AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim->AddTrack<float>("layer/hover/mDrawable/angle") =
			AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim->AddTrack<float>("layer/pressed/mDrawable/angle") =
			AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		sample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;

		o2UI.AddWidgetStyle(sample, "expand down");
	}

	void BasicUIStyleBuilder::RebuildExpandWhiteButton()
	{
		Button* sample = mnew Button();
		sample->layout->minSize = Vec2F(5, 5);
		sample->name = "expandBtn";

		auto regularLayer = sample->AddLayer("regular", mnew Sprite("ui/UI4_Right_icn_white.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto selectLayer = sample->AddLayer("hover", mnew Sprite("ui/UI4_Right_icn_white_select.png"),
											Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));

		auto pressedLayer = sample->AddLayer("pressed", mnew Sprite("ui/UI4_Right_icn_white_pressed.png"),
											 Layout(Vec2F(0.5f, 0.5f), Vec2F(0.5f, 0.5f), Vec2F(-10, -10), Vec2F(10, 10)));


		sample->AddState("hover", AnimationClip::EaseInOut("layer/hover/transparency", 0.0f, 1.0f, 0.1f))
			->offStateAnimationSpeed = 1.0f / 4.0f;

		sample->AddState("pressed", AnimationClip::EaseInOut("layer/pressed/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		sample->AddState("visible", AnimationClip::EaseInOut("transparency", 0.0f, 1.0f, 0.2f))
			->offStateAnimationSpeed = 0.5f;

		auto expandedStateAnim = mmake<AnimationClip>();
		*expandedStateAnim->AddTrack<float>("layer/regular/mDrawable/angle") =
			AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim->AddTrack<float>("layer/hover/mDrawable/angle") =
			AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		*expandedStateAnim->AddTrack<float>("layer/pressed/mDrawable/angle") =
			AnimationTrack<float>::EaseInOut(Math::Deg2rad(0.0f), Math::Deg2rad(-90.0f), 0.1f);

		sample->AddState("expanded", expandedStateAnim)->offStateAnimationSpeed = 2.5f;

		o2UI.AddWidgetStyle(sample, "expand white");
	}

	void BasicUIStyleBuilder::RebuildSpoiler()
	{
		Spoiler* sample = mnew Spoiler();
		sample->spacing = 5.0f;
		sample->borderLeft = 10;
		sample->expandHeight = false;
		sample->expandWidth = true;
		sample->fitByChildren = true;
		sample->baseCorner = BaseCorner::RightTop;

		Text* captionText        = mnew Text("stdFont.ttf");
		captionText->text        = "Button";
		captionText->horAlign    = HorAlign::Left;
		captionText->verAlign    = VerAlign::Middle;
		captionText->dotsEngings = true;
		captionText->wordWrap = true;
		captionText->color = Color4(96, 125, 139);
		sample->AddLayer("caption", captionText, Layout::HorStretch(VerAlign::Top, 11, 0, 20));

		auto expandBtn = o2UI.CreateWidget<Button>("expand");
		*expandBtn->layout = WidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(-7, 0));
		expandBtn->SetInternalParent(sample, false);

		sample->SetHeadHeight(20);

		o2UI.AddWidgetStyle(sample, "expand with caption");
	}
}
// --- META ---

DECLARE_CLASS(o2::BasicUIStyleBuilder, o2__BasicUIStyleBuilder);
// --- END META ---