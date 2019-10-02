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
		void RebuildSinglelineEditboxProperty();
		void RebuildEditorDropdown();
		void RebuildRedEditBoxStyle();
		void RebuildGreenEditBoxStyle();

		void RebuildRecordMenuToggle();
		void RebuildPlayStopMenuToggle();
		void RebuildLoopMenuToggle();
		void RebuildAddKeyMenuButton();
		void RebuildAddKeyButton();

		void RebuildAddSmallBtn();
		void RebuildRemoveSmallBtn();

		void RebuildFloatProperty();
		void RebuildRedFloatProperty();
		void RebuildGreenFloatProperty();
		void RebuildIntegerProperty();
		void RebuildActorPropety();
		void RebuildAssetsPropeties();
		void RebuildBoolPropety();
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

		void RebuildEditorUIManager(bool saveStyle = true, bool checkEditedDate = true);

		IOBJECT(EditorUIStyleBuilder);

	private: 
		template<typename _property_type>
		void BuildPropertyWithCaption(const String& propertyStyle, const String& propertyWithCaptionStyle);

		template<typename _type>
		void RebuildAssetPropety();
	};

	template<typename _property_type>
	void EditorUIStyleBuilder::BuildPropertyWithCaption(const String& propertyStyle, const String& propertyWithCaptionStyle)
	{
		_property_type* property = o2UI.CreateWidget<_property_type>(propertyStyle);

		Label* label = o2UI.CreateWidget<Label>();
		label->name = "propertyName";
		label->horAlign = HorAlign::Left;
		label->layout->widthWeight = 1.0f;
		label->horOverflow = Label::HorOverflow::Dots;

		property->AddChild(label, 0);
		property->borderLeft = 10;

		o2UI.AddWidgetStyle(property, propertyWithCaptionStyle);
	}

	template<typename _type>
	void EditorUIStyleBuilder::RebuildAssetPropety()
	{
		auto sample = mnew AssetProperty<_type>();
		sample->layout->minHeight = 20;
		sample->expandHeight = true;
		sample->expandWidth = true;
		sample->fitByChildren = false;

		auto layout = mnew Widget();
		layout->name = "layout";
		sample->AddChild(layout);

		auto box = mnew Widget();
		box->name = "box";
		box->SetFocusable(true);
		*box->layout = WidgetLayout::BothStretch(0, 0, 20, 0);

		auto backLayer = box->AddLayer("back", mnew Sprite("ui/UI4_Editbox_regular.png"),
									   Layout::BothStretch(-9, -9, -9, -9));

		auto selectLayer = box->AddLayer("hover", mnew Sprite("ui/UI4_Editbox_select.png"),
										 Layout::BothStretch(-9, -9, -9, -9));

		auto focusLayer = box->AddLayer("focus", mnew Sprite("ui/UI4_Editbox_focus.png"),
										Layout::BothStretch(-9, -9, -9, -9));

		box->AddState("focused", Animation::EaseInOut(box, "layer/focus/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		box->AddState("hover", Animation::EaseInOut(box, "layer/hover/transparency", 0.0f, 1.0f, 0.05f))
			->offStateAnimationSpeed = 0.5f;

		auto nameText = mnew Text("stdFont.ttf");
		nameText->text = "--";
		nameText->horAlign = HorAlign::Left;
		nameText->verAlign = VerAlign::Middle;
		nameText->dotsEngings = true;
		nameText->color = Color4(96, 125, 139);;
		box->AddLayer("caption", nameText, Layout::BothStretch(2, 2, 2, 2));

		box->SetFocusable(true);

		auto linkBtn = o2UI.CreateWidget<Button>("asset link");
		*linkBtn->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(15, 15), Vec2F());
		box->AddChild(linkBtn);

		Button* revertBtn = o2UI.CreateWidget<Button>("revert");
		revertBtn->name = "revertBtn";
		*revertBtn->layout = WidgetLayout::Based(BaseCorner::Right, Vec2F(20, 20), Vec2F());

		layout->AddChild(box);
		layout->AddChild(revertBtn);

		Animation revertStateAnim = Animation::EaseInOut(sample, "child/layout/child/box/layout/offsetRight", 0.0f, -20.0f, 0.15f);
		*revertStateAnim.AddAnimationValue<bool>("child/layout/child/revertBtn/enabled") = AnimatedValue<bool>::EaseInOut(false, true, 0.15f);
		sample->AddState("revert", revertStateAnim);

		o2UI.AddWidgetStyle(sample, "standard");
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
	PUBLIC_FUNCTION(void, RebuildSinglelineEditboxProperty);
	PUBLIC_FUNCTION(void, RebuildEditorDropdown);
	PUBLIC_FUNCTION(void, RebuildRedEditBoxStyle);
	PUBLIC_FUNCTION(void, RebuildGreenEditBoxStyle);
	PUBLIC_FUNCTION(void, RebuildRecordMenuToggle);
	PUBLIC_FUNCTION(void, RebuildPlayStopMenuToggle);
	PUBLIC_FUNCTION(void, RebuildLoopMenuToggle);
	PUBLIC_FUNCTION(void, RebuildAddKeyMenuButton);
	PUBLIC_FUNCTION(void, RebuildAddKeyButton);
	PUBLIC_FUNCTION(void, RebuildAddSmallBtn);
	PUBLIC_FUNCTION(void, RebuildRemoveSmallBtn);
	PUBLIC_FUNCTION(void, RebuildFloatProperty);
	PUBLIC_FUNCTION(void, RebuildRedFloatProperty);
	PUBLIC_FUNCTION(void, RebuildGreenFloatProperty);
	PUBLIC_FUNCTION(void, RebuildIntegerProperty);
	PUBLIC_FUNCTION(void, RebuildActorPropety);
	PUBLIC_FUNCTION(void, RebuildAssetsPropeties);
	PUBLIC_FUNCTION(void, RebuildBoolPropety);
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
	PUBLIC_FUNCTION(void, RebuildEditorUIManager, bool, bool);
}
END_META;
