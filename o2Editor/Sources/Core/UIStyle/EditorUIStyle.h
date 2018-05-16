#pragma once

#include "Core/UIStyle/BasicUIStyle.h"

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
		void RebuildArrowToggle();
		void RebuildBrushToggle();
		void RebuildMoveToggle();
		void RebuildRotateToggle();
		void RebuildScaleToggle();
		void RebuildFrameToggle();
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
		void RebuildFolderAssetIcon();
		void RebuildPrototypeAssetIcon();
		void RebuildPrefabPreviewAssetIcon();
		void RebuildImagePreviewAssetIcon();
		void RebuildTextAssetIcon();
		void RebuildAnimationAssetIcon();
		void RebuildAssetsGridScroll();
		void RebuildLinkBtn();
		void RebuildSinglelineEditBoxWithArrows();
		void RebuildSinglelineEditboxProperty();
		void RebuildEditorDropdown();
		void RebuildRedEditBoxStyle();
		void RebuildGreenEditBoxStyle();

		void RebuildActorPropety();
		void RebuildColorPropety();
		void RebuildAssetPropety();
		void RebuildComponentProperty();
		void RebuildVector2Property();
		void RebuildColoredVector2Property();
		void RebuildRectProperty();
		void RebuildNewRectProperty();

		void RebuildActorPropetyWithCaption();
		void RebuildColorPropetyWithCaption();
		void RebuildAssetPropetyWithCaption();
		void RebuildComponentPropertyWithCaption();
		void RebuildVector2PropertyWithCaption();
		void RebuildColoredVector2PropertyWithCaption();
		void RebuildRectPropertyWithCaption();
		void RebuildNewRectPropertyWithCaption();

		void RebuildActorHeadEnableToggle();
		void RebuildActorHeadName();
		void RebuildActorHeadLockToggle();
		void RebuildActorHeadActorAssetProperty();
		void RebuildActorHeadTagsProperty();
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
		void RebuildBooleanProperty();

		void RebuildEditorUIManager();

		IOBJECT(EditorUIStyleBuilder);

	private: 
		template<typename _property_type>
		void BuildPropertyWithCaption(const String& propertyStyle, const String& propertyWithCaptionStyle);
	};

	template<typename _property_type>
	void EditorUIStyleBuilder::BuildPropertyWithCaption(const String& propertyStyle, const String& propertyWithCaptionStyle)
	{
		_property_type* property = o2UI.CreateWidget<_property_type>(propertyStyle);

		UILabel* label = o2UI.CreateWidget<UILabel>();
		label->name = "propertyName";
		label->horAlign = HorAlign::Left;
		label->layout->widthWeight = 3.0f;
		label->horOverflow = UILabel::HorOverflow::Dots;
		label->text = name;

		property->AddChild(label, 0);

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
	PUBLIC_FUNCTION(void, RebuildArrowToggle);
	PUBLIC_FUNCTION(void, RebuildBrushToggle);
	PUBLIC_FUNCTION(void, RebuildMoveToggle);
	PUBLIC_FUNCTION(void, RebuildRotateToggle);
	PUBLIC_FUNCTION(void, RebuildScaleToggle);
	PUBLIC_FUNCTION(void, RebuildFrameToggle);
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
	PUBLIC_FUNCTION(void, RebuildFolderAssetIcon);
	PUBLIC_FUNCTION(void, RebuildPrototypeAssetIcon);
	PUBLIC_FUNCTION(void, RebuildPrefabPreviewAssetIcon);
	PUBLIC_FUNCTION(void, RebuildImagePreviewAssetIcon);
	PUBLIC_FUNCTION(void, RebuildTextAssetIcon);
	PUBLIC_FUNCTION(void, RebuildAnimationAssetIcon);
	PUBLIC_FUNCTION(void, RebuildAssetsGridScroll);
	PUBLIC_FUNCTION(void, RebuildLinkBtn);
	PUBLIC_FUNCTION(void, RebuildSinglelineEditBoxWithArrows);
	PUBLIC_FUNCTION(void, RebuildSinglelineEditboxProperty);
	PUBLIC_FUNCTION(void, RebuildEditorDropdown);
	PUBLIC_FUNCTION(void, RebuildActorPropety);
	PUBLIC_FUNCTION(void, RebuildColorPropety);
	PUBLIC_FUNCTION(void, RebuildAssetPropety);
	PUBLIC_FUNCTION(void, RebuildComponentProperty);
	PUBLIC_FUNCTION(void, RebuildVector2Property);
	PUBLIC_FUNCTION(void, RebuildRedEditBoxStyle);
	PUBLIC_FUNCTION(void, RebuildGreenEditBoxStyle);
	PUBLIC_FUNCTION(void, RebuildColoredVector2Property);
	PUBLIC_FUNCTION(void, RebuildRectProperty);
	PUBLIC_FUNCTION(void, RebuildNewRectProperty);
	PUBLIC_FUNCTION(void, RebuildActorHeadEnableToggle);
	PUBLIC_FUNCTION(void, RebuildActorHeadName);
	PUBLIC_FUNCTION(void, RebuildActorHeadLockToggle);
	PUBLIC_FUNCTION(void, RebuildActorHeadActorAssetProperty);
	PUBLIC_FUNCTION(void, RebuildActorHeadTagsProperty);
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
	PUBLIC_FUNCTION(void, RebuildBooleanProperty);
	PUBLIC_FUNCTION(void, RebuildEditorUIManager);
}
END_META;
