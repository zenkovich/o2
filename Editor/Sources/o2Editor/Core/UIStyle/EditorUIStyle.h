#pragma once

#include "o2Editor/Core/UIStyle/BasicUIStyle.h"

using namespace o2;

namespace Editor
{
	class EditorUIStyleBuilder: public BasicUIStyleBuilder
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
		template<typename _property_type>
		void BuildPropertyWithCaption(const String& propertyStyle, const String& propertyWithCaptionStyle);

	};

	template<typename _property_type>
	void EditorUIStyleBuilder::BuildPropertyWithCaption(const String& propertyStyle, const String& propertyWithCaptionStyle)
	{
		_property_type* property = o2UI.CreateWidget<_property_type>(propertyStyle);

		Label* label = o2UI.CreateWidget<Label>();
		label->name = "propertyName";
		label->horAlign = HorAlign::Left;
		label->layout->widthWeight = 0.6f;
		label->horOverflow = Label::HorOverflow::Dots;

		property->AddChild(label, 0);
		property->borderLeft = 10;

		o2UI.AddWidgetStyle(property, propertyWithCaptionStyle);
	}
}

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

	PUBLIC_FUNCTION(void, RebuildDockableWndStyle);
	PUBLIC_FUNCTION(void, RebuildPlayStopButtonStyle);
	PUBLIC_FUNCTION(void, RebuildPauseButtonStyle);
	PUBLIC_FUNCTION(void, RebuildStepButtonStyle);
	PUBLIC_FUNCTION(void, RebuildRoundDropDown);
	PUBLIC_FUNCTION(void, RebuildMenuPanelDropDown);
	PUBLIC_FUNCTION(void, RebuildSearchButton);
	PUBLIC_FUNCTION(void, RebuildListTreeToggle);
	PUBLIC_FUNCTION(void, RebuildRevertBtn);
	PUBLIC_FUNCTION(void, RebuildActorsTreeNodeEnableToggle);
	PUBLIC_FUNCTION(void, RebuildActorsTreeLockToggle);
	PUBLIC_FUNCTION(void, RebuildActorsTreeLinkBtn);
	PUBLIC_FUNCTION(void, RebuildActorsTree);
	PUBLIC_FUNCTION(void, RebuildPanelDownButton);
	PUBLIC_FUNCTION(void, RebuildTrashDownPanelButton);
	PUBLIC_FUNCTION(void, RebuildMessagesDownPanelToggle);
	PUBLIC_FUNCTION(void, RebuildWarningsDownPanelToggle);
	PUBLIC_FUNCTION(void, RebuildErrorsDownPanelToggle);
	PUBLIC_FUNCTION(void, RebuildFilterMenuButton);
	PUBLIC_FUNCTION(void, RebuildTreeMenuButton);
	PUBLIC_FUNCTION(void, RebuildFoldersTree);
	PUBLIC_FUNCTION(void, RebuildRegularAssetIcon);
	PUBLIC_FUNCTION(void, RebuildAssetsGridScroll);
	PUBLIC_FUNCTION(void, RebuildLinkBtn);
	PUBLIC_FUNCTION(void, RebuildSinglelineEditboxProperty);
	PUBLIC_FUNCTION(void, RebuildEditorDropdown);
	PUBLIC_FUNCTION(void, RebuildRedEditBoxStyle);
	PUBLIC_FUNCTION(void, RebuildGreenEditBoxStyle);
	PUBLIC_FUNCTION(void, RebuildPreviewMenuToggle);
	PUBLIC_FUNCTION(void, RebuildRecordMenuToggle);
	PUBLIC_FUNCTION(void, RebuildRewindLeftMenuButton);
	PUBLIC_FUNCTION(void, RebuildMoveLeftMenuButton);
	PUBLIC_FUNCTION(void, RebuildPlayStopMenuToggle);
	PUBLIC_FUNCTION(void, RebuildRewindRightMenuButton);
	PUBLIC_FUNCTION(void, RebuildMoveRightMenuButton);
	PUBLIC_FUNCTION(void, RebuildLoopMenuToggle);
	PUBLIC_FUNCTION(void, RebuildAddKeyMenuButton);
	PUBLIC_FUNCTION(void, RebuildCurvesMenuToggle);
	PUBLIC_FUNCTION(void, RebuildPropertiesMenuButton);
	PUBLIC_FUNCTION(void, RebuildAddKeyButton);
	PUBLIC_FUNCTION(void, RebuildAddKeyDotButton);
	PUBLIC_FUNCTION(void, RebuildAddSmallBtn);
	PUBLIC_FUNCTION(void, RebuildRemoveSmallBtn);
	PUBLIC_FUNCTION(void, RebuildCreateAssetInstanceBtn);
	PUBLIC_FUNCTION(void, RebuildRemoveAssetInstanceBtn);
	PUBLIC_FUNCTION(void, RebuildSaveAssetInstanceBtn);
	PUBLIC_FUNCTION(void, RebuildFloatProperty);
	PUBLIC_FUNCTION(void, RebuildRedFloatProperty);
	PUBLIC_FUNCTION(void, RebuildGreenFloatProperty);
	PUBLIC_FUNCTION(void, RebuildIntegerProperty);
	PUBLIC_FUNCTION(void, RebuildActorProperty);
	PUBLIC_FUNCTION(void, RebuildBoolProperty);
	PUBLIC_FUNCTION(void, RebuildBorderFProperty);
	PUBLIC_FUNCTION(void, RebuildBorderIProperty);
	PUBLIC_FUNCTION(void, RebuildColorPropety);
	PUBLIC_FUNCTION(void, RebuildComponentProperty);
	PUBLIC_FUNCTION(void, RebuildCurveProperty);
	PUBLIC_FUNCTION(void, RebuildEnumProperty);
	PUBLIC_FUNCTION(void, RebuildLayerProperty);
	PUBLIC_FUNCTION(void, RebuildRectFProperty);
	PUBLIC_FUNCTION(void, RebuildRectIProperty);
	PUBLIC_FUNCTION(void, RebuildStringProperty);
	PUBLIC_FUNCTION(void, RebuildWStringProperty);
	PUBLIC_FUNCTION(void, RebuildTagProperty);
	PUBLIC_FUNCTION(void, RebuildVector2FProperty);
	PUBLIC_FUNCTION(void, RebuildVector2IProperty);
	PUBLIC_FUNCTION(void, RebuildColoredVector2Property);
	PUBLIC_FUNCTION(void, RebuildAssetPropety);
	PUBLIC_FUNCTION(void, RebuildSceneLayersListProperty);
	PUBLIC_FUNCTION(void, RebuildPropertiesWithCaptins);
	PUBLIC_FUNCTION(void, RebuildActorHeadEnableToggle);
	PUBLIC_FUNCTION(void, RebuildActorHeadEnableProperty);
	PUBLIC_FUNCTION(void, RebuildActorHeadName);
	PUBLIC_FUNCTION(void, RebuildActorHeadNameProperty);
	PUBLIC_FUNCTION(void, RebuildActorHeadLockToggle);
	PUBLIC_FUNCTION(void, RebuildActorHeadLockProperty);
	PUBLIC_FUNCTION(void, RebuildActorHeadActorAssetProperty);
	PUBLIC_FUNCTION(void, RebuildActorHeadTags);
	PUBLIC_FUNCTION(void, RebuildActorHeadTagsProperty);
	PUBLIC_FUNCTION(void, RebuildActorHeadLayer);
	PUBLIC_FUNCTION(void, RebuildActorHeadLayerProperty);
	PUBLIC_FUNCTION(void, RebuildAcceptPrototypeBtn);
	PUBLIC_FUNCTION(void, RebuildRevertPrototypeBtn);
	PUBLIC_FUNCTION(void, RebuildBreakPrototypeBtn);
	PUBLIC_FUNCTION(void, RebuildComponentOptionsBtn);
	PUBLIC_FUNCTION(void, RebuildComponentSaveBtn);
	PUBLIC_FUNCTION(void, RebuildHorWideScrollbar);
	PUBLIC_FUNCTION(void, RebuildVerWideScrollbar);
	PUBLIC_FUNCTION(void, RebuildHorWideProgressbar);
	PUBLIC_FUNCTION(void, RebuildVerWideProgressbar);
	PUBLIC_FUNCTION(void, RebuildAnimationTree);
	PUBLIC_FUNCTION(void, RebuildAnimationPropertiesTree);
	PUBLIC_FUNCTION(void, RebuildSpoilerWithHead);
	PUBLIC_FUNCTION(void, RebuildAddComponentButton);
	PUBLIC_FUNCTION(void, RebuildAddComponentsTree);
	PUBLIC_FUNCTION(void, RebuildAnimationStateViewerEditButton);
	PUBLIC_FUNCTION(void, RebuildAnimationStateViewerPlayToggle);
	PUBLIC_FUNCTION(void, RebuildAnimationStateViewerLoopToggle);
	PUBLIC_FUNCTION(void, RebuildAnimationStateViewerProgressBar);
	PUBLIC_FUNCTION(void, RebuildAnimationSpoiler);
	PUBLIC_FUNCTION(void, RebuildEditorUIManager, const String&, bool, bool);
}
END_META;
