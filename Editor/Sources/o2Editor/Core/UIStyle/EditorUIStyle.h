#pragma once

#include "o2Editor/Core/UIStyle/BasicUIStyle.h"
#include "o2Editor/Core/Ref.h"
#include "o2Editor/Core/WeakRef.h"

using namespace o2;

namespace Editor
{
    class EditorUIStyleBuilder : public BasicUIStyleBuilder
    {
    public:
        void RebuildDockableWndStyle();
        void RebuildPlayStopButtonStyle();
        void RebuildPauseButtonStyle();
        void RebuildStepButtonStyle();
        void RebuildRoundDropDown();
        void RebuildMenuPanelDropDown();
        void RebuildSearchButton();
        void RebuildListTreeToggle();
        void RebuildRevertBtn();
        void RebuildActorsTreeNodeEnableToggle();
        void RebuildActorsTreeLockToggle();
        void RebuildActorsTreeLinkBtn();
        void RebuildActorsTree();
        void RebuildDrawOrderTree();
        void RebuildPanelDownButton();
        void RebuildTrashDownPanelButton();
        void RebuildMessagesDownPanelToggle();
        void RebuildWarningsDownPanelToggle();
        void RebuildErrorsDownPanelToggle();
        void RebuildFilterMenuButton();
        void RebuildTreeMenuButton();
        void RebuildFoldersTree();
        void RebuildRegularAssetIcon();
        void RebuildAssetsGridScroll();
        void RebuildLinkBtn();
        void RebuildSinglelineEditboxProperty();
        void RebuildEditorDropdown();
        void RebuildRedEditBoxStyle();
        void RebuildGreenEditBoxStyle();

        void RebuildPreviewMenuToggle();
        void RebuildRecordMenuToggle();
        void RebuildRewindLeftMenuButton();
        void RebuildMoveLeftMenuButton();
        void RebuildPlayStopMenuToggle();
        void RebuildRewindRightMenuButton();
        void RebuildMoveRightMenuButton();
        void RebuildLoopMenuToggle();
        void RebuildAddKeyMenuButton();
        void RebuildCurvesMenuToggle();
        void RebuildPropertiesMenuButton();

        void RebuildAddKeyButton();
        void RebuildAddKeyDotButton();

        void RebuildAddSmallBtn();
        void RebuildRemoveSmallBtn();

        void RebuildCreateAssetInstanceBtn();
        void RebuildRemoveAssetInstanceBtn();
        void RebuildSaveAssetInstanceBtn();

        void RebuildFloatProperty();
        void RebuildRedFloatProperty();
        void RebuildGreenFloatProperty();
        void RebuildIntegerProperty();
        void RebuildActorProperty();
        void RebuildBoolProperty();
        void RebuildBorderFProperty();
        void RebuildBorderIProperty();
        void RebuildColorPropety();
        void RebuildComponentProperty();
        void RebuildCurveProperty();
        void RebuildEnumProperty();
        void RebuildLayerProperty();
        void RebuildRectFProperty();
        void RebuildRectIProperty();
        void RebuildStringProperty();
        void RebuildWStringProperty();
        void RebuildTagProperty();
        void RebuildVector2FProperty();
        void RebuildVector2IProperty();
        void RebuildColoredVector2Property();
        void RebuildAssetPropety();
        void RebuildSceneLayersListProperty();

        void RebuildPropertiesWithCaptins();

        void RebuildActorHeadEnableToggle();
        void RebuildActorHeadEnableProperty();

        void RebuildActorHeadName();
        void RebuildActorHeadNameProperty();
        void RebuildActorHeadDepthProperty();

        void RebuildActorHeadLockToggle();
        void RebuildActorHeadLockProperty();

        void RebuildActorHeadActorAssetProperty();

        void RebuildActorHeadTags();
        void RebuildActorHeadTagsProperty();

        void RebuildActorHeadLayer();
        void RebuildActorHeadLayerProperty();

        void RebuildAcceptPrototypeBtn();
        void RebuildRevertPrototypeBtn();
        void RebuildBreakPrototypeBtn();
        void RebuildComponentOptionsBtn();
        void RebuildComponentSaveBtn();
        void RebuildHorWideScrollbar();
        void RebuildVerWideScrollbar();
        void RebuildHorWideProgressbar();
        void RebuildVerWideProgressbar();

        void RebuildAnimationTree();
        void RebuildAnimationPropertiesTree();

        void RebuildSpoilerWithHead();

        void RebuildAddComponentButton();
        void RebuildAddComponentsTree();

        void RebuildAnimationStateViewerEditButton();
        void RebuildAnimationStateViewerPlayToggle();
        void RebuildAnimationStateViewerLoopToggle();
        void RebuildAnimationStateViewerProgressBar();
        void RebuildAnimationSpoiler();

        void RebuildEditorUIManager(const String& stylesFolder, bool saveStyle = true, bool checkEditedDate = true);

        IOBJECT(EditorUIStyleBuilder);

    private:
        template <typename _property_type>
        void BuildPropertyWithCaption(const String& propertyStyle, const String& propertyWithCaptionStyle);
    };

    template <typename _property_type>
    void EditorUIStyleBuilder::BuildPropertyWithCaption(const String& propertyStyle, const String& propertyWithCaptionStyle)
    {
        Ref<_property_type> property = mmake<_property_type>(propertyStyle);

        Ref<Label> label = mmake<Label>();
        label->name = "propertyName";
        label->horAlign = HorAlign::Left;
        label->layout->widthWeight = 0.6f;
        label->horOverflow = Label::HorOverflow::Dots;

        property->AddChild(label, 0);
        property->borderLeft = 10;

        o2UI.AddWidgetStyle(property, propertyWithCaptionStyle);
    }
}  // namespace Editor

// --- META ---

CLASS_BASES_META(Editor::EditorUIStyleBuilder)
{
    BASE_CLASS(o2::BasicUIStyleBuilder);
}
END_META;
CLASS_FIELDS_META(Editor::EditorUIStyleBuilder)
{
}
END_META;
CLASS_METHODS_META(Editor::EditorUIStyleBuilder)
{
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildDockableWndStyle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildPlayStopButtonStyle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildPauseButtonStyle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildStepButtonStyle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRoundDropDown);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildMenuPanelDropDown);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildSearchButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildListTreeToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRevertBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorsTreeNodeEnableToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorsTreeLockToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorsTreeLinkBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorsTree);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildDrawOrderTree);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildPanelDownButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildTrashDownPanelButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildMessagesDownPanelToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildWarningsDownPanelToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildErrorsDownPanelToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildFilterMenuButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildTreeMenuButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildFoldersTree);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRegularAssetIcon);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAssetsGridScroll);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildLinkBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildSinglelineEditboxProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildEditorDropdown);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRedEditBoxStyle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildGreenEditBoxStyle);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildPreviewMenuToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRecordMenuToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRewindLeftMenuButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildMoveLeftMenuButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildPlayStopMenuToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRewindRightMenuButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildMoveRightMenuButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildLoopMenuToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAddKeyMenuButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildCurvesMenuToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildPropertiesMenuButton);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAddKeyButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAddKeyDotButton);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAddSmallBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRemoveSmallBtn);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildCreateAssetInstanceBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRemoveAssetInstanceBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildSaveAssetInstanceBtn);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildFloatProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRedFloatProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildGreenFloatProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildIntegerProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildBoolProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildBorderFProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildBorderIProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildColorPropety);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildComponentProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildCurveProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildEnumProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildLayerProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRectFProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRectIProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildStringProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildWStringProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildTagProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildVector2FProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildVector2IProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildColoredVector2Property);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAssetPropety);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildSceneLayersListProperty);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildPropertiesWithCaptins);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadEnableToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadEnableProperty);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadName);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadNameProperty);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadDepthProperty);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadLockToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadLockProperty);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadActorAssetProperty);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadTags);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadTagsProperty);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadLayer);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildActorHeadLayerProperty);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAcceptPrototypeBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildRevertPrototypeBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildBreakPrototypeBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildComponentOptionsBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildComponentSaveBtn);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildHorWideScrollbar);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildVerWideScrollbar);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildHorWideProgressbar);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildVerWideProgressbar);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAnimationTree);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAnimationPropertiesTree);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildSpoilerWithHead);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAddComponentButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAddComponentsTree);

    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAnimationStateViewerEditButton);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAnimationStateViewerPlayToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAnimationStateViewerLoopToggle);
    FUNCTION().PUBLIC().SIGNATURE(void, RebuildAnimationStateViewerProgressBar);
   Ref<ntopButtonStyle> m_ntopButtonStyle;
Ref<RebuildPauseButtonStyle> m_RebuildPauseButtonStyle;
Ref<RebuildStepButtonStyle> m_RebuildStepButtonStyle;
Ref<RebuildRoundDropDown> m_RebuildRoundDropDown;
Ref<RebuildMenuPanelDropDown> m_RebuildMenuPanelDropDown;
Ref<RebuildSearchButton> m_RebuildSearchButton;
Ref<RebuildListTreeToggle> m_RebuildListTreeToggle;
Ref<RebuildRevertBtn> m_RebuildRevertBtn;
Ref<RebuildActorsTreeNodeEnableToggle> m_RebuildActorsTreeNodeEnableToggle;
Ref<RebuildActorsTreeLockToggle> m_RebuildActorsTreeLockToggle;
Ref<RebuildActorsTreeLinkBtn> m_RebuildActorsTreeLinkBtn;
Ref<RebuildActorsTree> m_RebuildActorsTree;
Ref<RebuildDrawOrderTree> m_RebuildDrawOrderTree;
Ref<RebuildPanelDownButton> m_RebuildPanelDownButton;
Ref<RebuildTrashDownPanelButton> m_RebuildTrashDownPanelButton;
Ref<RebuildMessagesDownPanelToggle> m_RebuildMessagesDownPanelToggle;
Ref<RebuildWarningsDownPanelToggle> m_RebuildWarningsDownPanelToggle;
Ref<RebuildErrorsDownPanelToggle> m_RebuildErrorsDownPanelToggle;
Ref<RebuildFilterMenuButton> m_RebuildFilterMenuButton;
Ref<RebuildTreeMenuButton> m_RebuildTreeMenuButton;
Ref<RebuildFoldersTree> m_RebuildFoldersTree;
Ref<RebuildRegularAssetIcon> m_RebuildRegularAssetIcon;
Ref<RebuildAssetsGridScroll> m_RebuildAssetsGridScroll;
Ref<RebuildLinkBtn> m_RebuildLinkBtn;
Ref<RebuildSinglelineEditboxProperty> m_RebuildSinglelineEditboxProperty;
Ref<RebuildEditorDropdown> m_RebuildEditorDropdown;
Ref<RebuildRedEditBoxStyle> m_RebuildRedEditBoxStyle;
Ref<RebuildGreenEditBoxStyle> m_RebuildGreenEditBoxStyle;
Ref<RebuildPreviewMenuToggle> m_RebuildPreviewMenuToggle;
Ref<RebuildRecordMenuToggle> m_RebuildRecordMenuToggle;
Ref<RebuildRewindLeftMenuButton> m_RebuildRewindLeftMenuButton;
Ref<RebuildMoveLeftMenuButton> m_RebuildMoveLeftMenuButton;
Ref<RebuildPlayStopMenuToggle> m_RebuildPlayStopMenuToggle;
Ref<RebuildRewindRightMenuButton> m_RebuildRewindRightMenuButton;
Ref<RebuildMoveRightMenuButton> m_RebuildMoveRightMenuButton;
Ref<RebuildLoopMenuToggle> m_RebuildLoopMenuToggle;
Ref<RebuildAddKeyMenuButton> m_RebuildAddKeyMenuButton;
Ref<RebuildCurvesMenuToggle> m_RebuildCurvesMenuToggle;
Ref<RebuildPropertiesMenuButton> m_RebuildPropertiesMenuButton;
Ref<RebuildAddKeyButton> m_RebuildAddKeyButton;
Ref<RebuildAddKeyDotButton> m_RebuildAddKeyDotButton;
Ref<RebuildAddSmallBtn> m_RebuildAddSmallBtn;
Ref<RebuildRemoveSmallBtn> m_RebuildRemoveSmallBtn;
Ref<RebuildCreateAssetInstanceBtn> m_RebuildCreateAssetInstanceBtn;
Ref<RebuildRemoveAssetInstanceBtn> m_RebuildRemoveAssetInstanceBtn;
Ref<RebuildSaveAssetInstanceBtn> m_RebuildSaveAssetInstanceBtn;
Ref<RebuildFloatProperty> m_RebuildFloatProperty;
Ref<RebuildRedFloatProperty> m_RebuildRedFloatProperty;
Ref<RebuildGreenFloatProperty> m_RebuildGreenFloatProperty;
Ref<RebuildIntegerProperty> m_RebuildIntegerProperty;
Ref<RebuildActorProperty> m_RebuildActorProperty;
Ref<RebuildBoolProperty> m_RebuildBoolProperty;
Ref<RebuildBorderFProperty> m_RebuildBorderFProperty;
Ref<RebuildBorderIProperty> m_RebuildBorderIProperty;
Ref<RebuildColorPropety> m_RebuildColorPropety;
Ref<RebuildComponentProperty> m_RebuildComponentProperty;
Ref<RebuildCurveProperty> m_RebuildCurveProperty;
Ref<RebuildEnumProperty> m_RebuildEnumProperty;
Ref<RebuildLayerProperty> m_RebuildLayerProperty;
Ref<RebuildRectFProperty> m_RebuildRectFProperty;
Ref<RebuildRectIProperty> m_RebuildRectIProperty;
Ref<RebuildStringProperty> m_RebuildStringProperty;
Ref<RebuildWStringProperty> m_RebuildWStringProperty;
Ref<RebuildTagProperty> m_RebuildTagProperty;
Ref<RebuildVector2FProperty> m_RebuildVector2FProperty;
Ref<RebuildVector2IProperty> m_RebuildVector2IProperty;
Ref<RebuildColoredVector2Property> m_RebuildColoredVector2Property;
Ref<RebuildAssetPropety> m_RebuildAssetPropety;
Ref<RebuildSceneLayersListProperty> m_RebuildSceneLayersListProperty;
Ref<RebuildPropertiesWithCaptins> m_RebuildPropertiesWithCaptins;
Ref<RebuildActorHeadEnableToggle> m_RebuildActorHeadEnableToggle;
Ref<RebuildActorHeadEnableProperty> m_RebuildActorHeadEnableProperty;
Ref<RebuildActorHeadName> m_RebuildActorHeadName;
Ref<RebuildActorHeadNameProperty> m_RebuildActorHeadNameProperty;
Ref<RebuildActorHeadDepthProperty> m_RebuildActorHeadDepthProperty;
Ref<RebuildActorHeadLockToggle> m_RebuildActorHeadLockToggle;#include <memory>

template<typename T>
class Ref
{
public:
    Ref(T* ptr = nullptr) : m_ptr(ptr) {}

    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    operator bool() const { return m_ptr != nullptr; }
    T* get() const { return m_ptr; }

private:
    T* m_ptr;
};

template<typename T>
class WeakRef
{
public:
    WeakRef(T* ptr = nullptr) : m_ptr(ptr) {}

    T* lock() const { return m_ptr; }
    bool expired() const { return m_ptr == nullptr; }

private:
    T* m_ptr;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return mmake<T>(dynamic_cast<T*>(ptr.get()));
}

// Dummy class for testing
class MyClass {};

void RebuildActorHeadLockProperty(const Ref<MyClass>&);
void RebuildActorHeadActorAssetProperty(const Ref<MyClass>&);
void RebuildActorHeadTags(const Ref<MyClass>&);
void RebuildActorHeadTagsProperty(const Ref<MyClass>&);
void RebuildActorHeadLayer(const Ref<MyClass>&);
void RebuildActorHeadLayerProperty(const Ref<MyClass>&);
void RebuildAcceptPrototypeBtn(const Ref<MyClass>&);
void RebuildRevertPrototypeBtn(const Ref<MyClass>&);
void RebuildBreakPrototypeBtn(const Ref<MyClass>&);
void RebuildComponentOptionsBtn(const Ref<MyClass>&);
void RebuildComponentSaveBtn(const Ref<MyClass>&);
void RebuildHorWideScrollbar(const Ref<MyClass>&);
void RebuildVerWideScrollbar(const Ref<MyClass>&);
void RebuildHorWideProgressbar(const Ref<MyClass>&);
void RebuildVerWideProgressbar(const Ref<MyClass>&);
void RebuildAnimationTree(const Ref<MyClass>&);
void RebuildAnimationPropertiesTree(const Ref<MyClass>&);
void RebuildSpoilerWithHead(const Ref<MyClass>&);
void RebuildAddComponentButton(const Ref<MyClass>&);
void RebuildAddComponentsTree(const Ref<MyClass>&);
void RebuildAnimationStateViewerEditButton(const Ref<MyClass>&);
void RebuildAnimationStateViewerPlayToggle(const Ref<MyClass>&);
void RebuildAnimationStateViewerLoopToggle(const Ref<MyClass>&);
void RebuildAnimationStateViewerProgressBar(const Ref<MyClass>&);
void RebuildAnimationSpoiler(const Ref<MyClass>&);
void RebuildEditorUIManager(const std::string&, bool, bool);

int main() {
    Ref<MyClass> obj = mmake<MyClass>();
    RebuildActorHeadLockProperty(obj);
    RebuildActorHeadActorAssetProperty(obj);
    RebuildActorHeadTags(obj);
    RebuildActorHeadTagsProperty(obj);
    RebuildActorHeadLayer(obj);
    RebuildActorHeadLayerProperty(obj);
    RebuildAcceptPrototypeBtn(obj);
    RebuildRevertPrototypeBtn(obj);
    RebuildBreakPrototypeBtn(obj);
    RebuildComponentOptionsBtn(obj);
    RebuildComponentSaveBtn(obj);
    RebuildHorWideScrollbar(obj);
    RebuildVerWideScrollbar(obj);
    RebuildHorWideProgressbar(obj);
    RebuildVerWideProgressbar(obj);
    RebuildAnimationTree(obj);
    RebuildAnimationPropertiesTree(obj);
    RebuildSpoilerWithHead(obj);
    RebuildAddComponentButton(obj);
    RebuildAddComponentsTree(obj);
    RebuildAnimationStateViewerEditButton(obj);
    RebuildAnimationStateViewerPlayToggle(obj);
    RebuildAnimationStateViewerLoopToggle(obj);
    RebuildAnimationStateViewerProgressBar(obj);
    RebuildAnimationSpoiler(obj);
    RebuildEditorUIManager("string", true, false);

    return 0;
}