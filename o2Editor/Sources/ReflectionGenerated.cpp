#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\ReflectionGenerated.h"

#include "Utils/Reflection/Reflection.h"

// Includes
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\AssetsWindow\AssetsIconsScroll.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\AssetsWindow\AssetsWindow.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\AssetsWindow\FoldersTree.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\AssetsWindow\UIAssetIcon.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\EditorConfig.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Actions\ActorsTransform.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Actions\CreateActors.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Actions\DeleteActors.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Actions\EnableAction.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Actions\IAction.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Actions\LockAction.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Actions\ReparentActors.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Actions\Selection.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Tools\FrameTool.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Tools\IEditorTool.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Tools\MoveTool.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Tools\RotateTool.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Tools\ScaleTool.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\Tools\SelectionTool.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\WindowsSystem\IEditorWindow.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\WindowsSystem\UIDockableWindow.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\WindowsSystem\UIDockWindowPlace.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\Core\WindowsSystem\WindowsLayout.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\LogWindow\LogWindow.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\SceneWindow\SceneEditScreen.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\SceneWindow\SceneEditWidget.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\SceneWindow\SceneWindow.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\TreeWindow\TreeWindow.h"
#include "C:\work\o2\o2Engine\Sources\Animation\Animatable.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimatedFloat.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimatedValue.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimatedVector.h"
#include "C:\work\o2\o2Engine\Sources\Animation\Animation.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimationMask.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimationState.h"
#include "C:\work\o2\o2Engine\Sources\Animation\IAnimation.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Asset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\AssetInfo.h"
#include "C:\work\o2\o2Engine\Sources\Assets\AtlasAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\BinaryAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\BitmapFontAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\FolderAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\ImageAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\VectorFontAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Builder\AtlasAssetConverter.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Builder\FolderAssetConverter.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Builder\IAssetConverter.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Builder\ImageAssetConverter.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Builder\StdAssetConverter.h"
#include "C:\work\o2\o2Engine\Sources\Config\ProjectConfig.h"
#include "C:\work\o2\o2Engine\Sources\Render\Camera.h"
#include "C:\work\o2\o2Engine\Sources\Render\RectDrawable.h"
#include "C:\work\o2\o2Engine\Sources\Render\Sprite.h"
#include "C:\work\o2\o2Engine\Sources\Render\Text.h"
#include "C:\work\o2\o2Engine\Sources\Render\VectorFontEffects.h"
#include "C:\work\o2\o2Engine\Sources\Scene\Actor.h"
#include "C:\work\o2\o2Engine\Sources\Scene\ActorTransform.h"
#include "C:\work\o2\o2Engine\Sources\Scene\Component.h"
#include "C:\work\o2\o2Engine\Sources\Scene\DrawableComponent.h"
#include "C:\work\o2\o2Engine\Sources\Scene\Components\ImageComponent.h"
#include "C:\work\o2\o2Engine\Sources\UI\Button.h"
#include "C:\work\o2\o2Engine\Sources\UI\ContextMenu.h"
#include "C:\work\o2\o2Engine\Sources\UI\CustomDropDown.h"
#include "C:\work\o2\o2Engine\Sources\UI\CustomList.h"
#include "C:\work\o2\o2Engine\Sources\UI\DropDown.h"
#include "C:\work\o2\o2Engine\Sources\UI\EditBox.h"
#include "C:\work\o2\o2Engine\Sources\UI\GridLayout.h"
#include "C:\work\o2\o2Engine\Sources\UI\HorizontalLayout.h"
#include "C:\work\o2\o2Engine\Sources\UI\HorizontalProgress.h"
#include "C:\work\o2\o2Engine\Sources\UI\HorizontalScrollBar.h"
#include "C:\work\o2\o2Engine\Sources\UI\Label.h"
#include "C:\work\o2\o2Engine\Sources\UI\List.h"
#include "C:\work\o2\o2Engine\Sources\UI\LongList.h"
#include "C:\work\o2\o2Engine\Sources\UI\MenuPanel.h"
#include "C:\work\o2\o2Engine\Sources\UI\ScrollArea.h"
#include "C:\work\o2\o2Engine\Sources\UI\Toggle.h"
#include "C:\work\o2\o2Engine\Sources\UI\Tree.h"
#include "C:\work\o2\o2Engine\Sources\UI\VerticalLayout.h"
#include "C:\work\o2\o2Engine\Sources\UI\VerticalProgress.h"
#include "C:\work\o2\o2Engine\Sources\UI\VerticalScrollBar.h"
#include "C:\work\o2\o2Engine\Sources\UI\Widget.h"
#include "C:\work\o2\o2Engine\Sources\UI\WidgetLayer.h"
#include "C:\work\o2\o2Engine\Sources\UI\WidgetLayout.h"
#include "C:\work\o2\o2Engine\Sources\UI\WidgetState.h"
#include "C:\work\o2\o2Engine\Sources\UI\Window.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Serialization.h"
#include "C:\work\o2\o2Engine\Sources\Utils\ShortcutKeys.h"
#include "C:\work\o2\o2Engine\Sources\Utils\TimeStamp.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Math\Curve.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Math\Layout.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Math\Transform.h"
#include "C:\work\o2\o2Engine\Sources\Assets\AssetsTree.h"
#include "C:\work\o2\o2Engine\Sources\Render\VectorFont.h"
#include "C:\work\o2\o2Engine\Sources\Scene\Scene.h"

// Types declarations
o2::Type UIAssetsIconsScrollArea::type;
o2::Type AssetsWindow::type;
o2::Type UIAssetsFoldersTree::type;
o2::Type UIAssetIcon::type;
o2::Type EditorConfig::type;
o2::Type EditorActorsTransformAction::type;
o2::Type EditorCreateActorsAction::type;
o2::Type EditorDeleteActorsAction::type;
o2::Type EditorEnableAction::type;
o2::Type IEditorAction::type;
o2::Type EditorLockAction::type;
o2::Type EditorReparentActorsAction::type;
o2::Type EditorSelectionAction::type;
o2::Type EditorFrameTool::type;
o2::Type IEditorTool::type;
o2::Type EditorMoveTool::type;
o2::Type EditorRotateTool::type;
o2::Type EditorScaleTool::type;
o2::Type EditorSelectionTool::type;
o2::Type IEditorWindow::type;
o2::Type UIDockableWindow::type;
o2::Type UIDockWindowPlace::type;
o2::Type WindowsLayout::type;
o2::Type LogWindow::type;
o2::Type SceneEditScreen::type;
o2::Type SceneEditWidget::type;
o2::Type SceneWindow::type;
o2::Type TreeWindow::type;
o2::Type o2::Animatable::type;
o2::Type o2::AnimatedValue<float>::type;
o2::Type o2::IAnimatedValue::type;
o2::Type o2::AnimatedValue<Vec2F>::type;
o2::Type o2::Animation::type;
o2::Type o2::AnimationMask::type;
o2::Type o2::AnimationState::type;
o2::Type o2::IAnimation::type;
o2::Type o2::Asset::type;
o2::Type o2::AssetInfo::type;
o2::Type o2::AtlasAsset::type;
o2::Type o2::BinaryAsset::type;
o2::Type o2::BitmapFontAsset::type;
o2::Type o2::FolderAsset::type;
o2::Type o2::ImageAsset::type;
o2::Type o2::VectorFontAsset::type;
o2::Type o2::AtlasAssetConverter::type;
o2::Type o2::FolderAssetConverter::type;
o2::Type o2::IAssetConverter::type;
o2::Type o2::ImageAssetConverter::type;
o2::Type o2::StdAssetConverter::type;
o2::Type o2::ProjectConfig::type;
o2::Type o2::Camera::type;
o2::Type o2::IRectDrawable::type;
o2::Type o2::Sprite::type;
o2::Type o2::Text::type;
o2::Type o2::FontStrokeEffect::type;
o2::Type o2::FontGradientEffect::type;
o2::Type o2::FontColorEffect::type;
o2::Type o2::FontShadowEffect::type;
o2::Type o2::Actor::type;
o2::Type o2::ActorTransform::type;
o2::Type o2::Component::type;
o2::Type o2::DrawableComponent::type;
o2::Type o2::ImageComponent::type;
o2::Type o2::UIButton::type;
o2::Type o2::UIContextMenuItem::type;
o2::Type o2::UIContextMenu::type;
o2::Type o2::UICustomDropDown::type;
o2::Type o2::UICustomList::type;
o2::Type o2::UIDropDown::type;
o2::Type o2::UIEditBox::type;
o2::Type o2::UIGridLayout::type;
o2::Type o2::UIHorizontalLayout::type;
o2::Type o2::UIHorizontalProgress::type;
o2::Type o2::UIHorizontalScrollBar::type;
o2::Type o2::UILabel::type;
o2::Type o2::UIList::type;
o2::Type o2::UILongList::type;
o2::Type o2::UIMenuPanel::type;
o2::Type o2::UIScrollArea::type;
o2::Type o2::UIToggle::type;
o2::Type o2::UITreeNode::type;
o2::Type o2::UITree::type;
o2::Type o2::UIVerticalLayout::type;
o2::Type o2::UIVerticalProgress::type;
o2::Type o2::UIVerticalScrollBar::type;
o2::Type o2::UIWidget::type;
o2::Type o2::UIWidgetLayer::type;
o2::Type o2::UIWidgetLayout::type;
o2::Type o2::UIWidgetState::type;
o2::Type o2::UIWindow::type;
o2::Type o2::ISerializable::type;
o2::Type o2::ShortcutKeys::type;
o2::Type o2::TimeStamp::type;
o2::Type o2::Curve::type;
o2::Type o2::Layout::type;
o2::Type o2::Transform::type;
o2::Type o2::AnimatedValue<Vec2F>::Key::type;
o2::Type o2::Animation::AnimatedValueDef::type;
o2::Type o2::Asset::IMetaInfo::type;
o2::Type o2::AssetTree::AssetNode::type;
o2::Type o2::AtlasAsset::PlatformMeta::type;
o2::Type o2::AtlasAsset::MetaInfo::type;
o2::Type o2::AtlasAsset::Page::type;
o2::Type o2::BinaryAsset::MetaInfo::type;
o2::Type o2::BitmapFontAsset::MetaInfo::type;
o2::Type o2::FolderAsset::MetaInfo::type;
o2::Type o2::ImageAsset::PlatformMeta::type;
o2::Type o2::ImageAsset::MetaInfo::type;
o2::Type o2::VectorFontAsset::MetaInfo::type;
o2::Type o2::AtlasAssetConverter::Image::type;
o2::Type o2::VectorFont::Effect::type;
o2::Type o2::Scene::Layer::type;
o2::Type o2::UIContextMenu::Item::type;
o2::Type o2::UIMenuPanel::Item::type;
o2::Type o2::Curve::Key::type;
o2::Type EditorConfig::GlobalConfig::type;
o2::Type EditorConfig::ProjectConfig::type;
o2::Type EditorDeleteActorsAction::ActorInfo::type;
o2::Type WindowsLayout::WindowDockPlace::type;
o2::Type o2::AnimatedValue<RectF>::type;
o2::Type o2::AnimatedValue<RectF>::Key::type;
o2::Type o2::AnimatedValue<Color4>::type;
o2::Type o2::AnimatedValue<Color4>::Key::type;
o2::Type o2::AnimatedValue<bool>::type;
o2::Type o2::AnimatedValue<bool>::Key::type;

// Types initializations
void ::UIAssetsIconsScrollArea::InitializeType(::UIAssetsIconsScrollArea* sample)
{
	TypeInitializer::RegField(&type, "mAssetIconSize", (size_t)(char*)(&sample->mAssetIconSize) - (size_t)(char*)sample, sample->mAssetIconSize);
	TypeInitializer::RegField(&type, "mSelectedColor", (size_t)(char*)(&sample->mSelectedColor) - (size_t)(char*)sample, sample->mSelectedColor);
	TypeInitializer::RegField(&type, "mUnselectedColor", (size_t)(char*)(&sample->mUnselectedColor) - (size_t)(char*)sample, sample->mUnselectedColor);
	TypeInitializer::RegField(&type, "mHoverColor", (size_t)(char*)(&sample->mHoverColor) - (size_t)(char*)sample, sample->mHoverColor);
	TypeInitializer::RegField(&type, "mCurrentPath", (size_t)(char*)(&sample->mCurrentPath) - (size_t)(char*)sample, sample->mCurrentPath);
	TypeInitializer::RegField(&type, "mGrid", (size_t)(char*)(&sample->mGrid) - (size_t)(char*)sample, sample->mGrid);
	TypeInitializer::RegField(&type, "mSelection", (size_t)(char*)(&sample->mSelection) - (size_t)(char*)sample, sample->mSelection);
	TypeInitializer::RegField(&type, "mContextMenu", (size_t)(char*)(&sample->mContextMenu) - (size_t)(char*)sample, sample->mContextMenu);
	TypeInitializer::RegField(&type, "mSelectedAssetsIcons", (size_t)(char*)(&sample->mSelectedAssetsIcons) - (size_t)(char*)sample, sample->mSelectedAssetsIcons);
	TypeInitializer::RegField(&type, "mIconSelectionSprite", (size_t)(char*)(&sample->mIconSelectionSprite) - (size_t)(char*)sample, sample->mIconSelectionSprite);
	TypeInitializer::RegField(&type, "mSelectionSpriteLayout", (size_t)(char*)(&sample->mSelectionSpriteLayout) - (size_t)(char*)sample, sample->mSelectionSpriteLayout);
	TypeInitializer::RegField(&type, "mSelectionSpritesPool", (size_t)(char*)(&sample->mSelectionSpritesPool) - (size_t)(char*)sample, sample->mSelectionSpritesPool);
	TypeInitializer::RegField(&type, "mHoverIcon", (size_t)(char*)(&sample->mHoverIcon) - (size_t)(char*)sample, sample->mHoverIcon);
	TypeInitializer::RegField(&type, "mIconHoverSprite", (size_t)(char*)(&sample->mIconHoverSprite) - (size_t)(char*)sample, sample->mIconHoverSprite);
	TypeInitializer::RegField(&type, "mTargetHoverSpriteRect", (size_t)(char*)(&sample->mTargetHoverSpriteRect) - (size_t)(char*)sample, sample->mTargetHoverSpriteRect);
	TypeInitializer::RegField(&type, "mCurrentHoverSpriteRect", (size_t)(char*)(&sample->mCurrentHoverSpriteRect) - (size_t)(char*)sample, sample->mCurrentHoverSpriteRect);
	TypeInitializer::RegField(&type, "mIconsPool", (size_t)(char*)(&sample->mIconsPool) - (size_t)(char*)sample, sample->mIconsPool);
	TypeInitializer::RegField(&type, "mSelecting", (size_t)(char*)(&sample->mSelecting) - (size_t)(char*)sample, sample->mSelecting);
	TypeInitializer::RegField(&type, "mPressedPoint", (size_t)(char*)(&sample->mPressedPoint) - (size_t)(char*)sample, sample->mPressedPoint);
	TypeInitializer::RegField(&type, "mPressTime", (size_t)(char*)(&sample->mPressTime) - (size_t)(char*)sample, sample->mPressTime);
	TypeInitializer::RegField(&type, "mCurrentSelectingIcons", (size_t)(char*)(&sample->mCurrentSelectingIcons) - (size_t)(char*)sample, sample->mCurrentSelectingIcons);
	TypeInitializer::RegField(&type, "mCuttingAssets", (size_t)(char*)(&sample->mCuttingAssets) - (size_t)(char*)sample, sample->mCuttingAssets);
	TypeInitializer::RegField(&type, "mNeedRebuildAssets", (size_t)(char*)(&sample->mNeedRebuildAssets) - (size_t)(char*)sample, sample->mNeedRebuildAssets);
	auto funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "Draw", &::UIAssetsIconsScrollArea::Draw);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, float>(&type, "Update", &::UIAssetsIconsScrollArea::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, const String&>(&type, "SetViewingPath", &::UIAssetsIconsScrollArea::SetViewingPath);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, String>(&type, "GetViewingPath", &::UIAssetsIconsScrollArea::GetViewingPath);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "UpdateAssetsPath", &::UIAssetsIconsScrollArea::UpdateAssetsPath);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, bool>(&type, "IsSelectable", &::UIAssetsIconsScrollArea::IsSelectable);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, AssetId>(&type, "SelectAsset", &::UIAssetsIconsScrollArea::SelectAsset);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "DeselectAllAssets", &::UIAssetsIconsScrollArea::DeselectAllAssets);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, Vector<AssetInfo>>(&type, "GetSelectedAssets", &::UIAssetsIconsScrollArea::GetSelectedAssets);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, bool>(&type, "UpdateLayout", &::UIAssetsIconsScrollArea::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "UpdateCuttingAssets", &::UIAssetsIconsScrollArea::UpdateCuttingAssets);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnSelected", &::UIAssetsIconsScrollArea::OnSelected);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnDeselected", &::UIAssetsIconsScrollArea::OnDeselected);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorPressed", &::UIAssetsIconsScrollArea::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorReleased", &::UIAssetsIconsScrollArea::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &::UIAssetsIconsScrollArea::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorStillDown", &::UIAssetsIconsScrollArea::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "UpdateSelection", &::UIAssetsIconsScrollArea::UpdateSelection);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorMoved", &::UIAssetsIconsScrollArea::OnCursorMoved);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &::UIAssetsIconsScrollArea::OnCursorRightMouseReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, const Input::Key&>(&type, "OnKeyReleased", &::UIAssetsIconsScrollArea::OnKeyReleased);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "InitializeContext", &::UIAssetsIconsScrollArea::InitializeContext);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "PrepareIconsPools", &::UIAssetsIconsScrollArea::PrepareIconsPools);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, UIAssetIcon*, const String&>(&type, "GetAssetIconFromPool", &::UIAssetsIconsScrollArea::GetAssetIconFromPool);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "style");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, UIAssetIcon*>(&type, "FreeAssetIconToPool", &::UIAssetsIconsScrollArea::FreeAssetIconToPool);
	TypeInitializer::RegFuncParam<UIAssetIcon*>(funcInfo, "icon");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, UIAssetIcon*, const AssetInfo&>(&type, "GetImageAssetIcon", &::UIAssetsIconsScrollArea::GetImageAssetIcon);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "asset");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "UpdateAssetsGridSize", &::UIAssetsIconsScrollArea::UpdateAssetsGridSize);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "InitializeSelectionSprite", &::UIAssetsIconsScrollArea::InitializeSelectionSprite);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, Sprite*>(&type, "GetSelectionSprite", &::UIAssetsIconsScrollArea::GetSelectionSprite);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, Sprite*>(&type, "FreeSelectionSprite", &::UIAssetsIconsScrollArea::FreeSelectionSprite);
	TypeInitializer::RegFuncParam<Sprite*>(funcInfo, "sprite");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void, UIAssetIcon*>(&type, "OnIconDblClicked", &::UIAssetsIconsScrollArea::OnIconDblClicked);
	TypeInitializer::RegFuncParam<UIAssetIcon*>(funcInfo, "icon");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, UIAssetIcon*, const Vec2F&>(&type, "GetIconUnderPoint", &::UIAssetsIconsScrollArea::GetIconUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "UpdateHover", &::UIAssetsIconsScrollArea::UpdateHover);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextCopyPressed", &::UIAssetsIconsScrollArea::OnContextCopyPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextCutPressed", &::UIAssetsIconsScrollArea::OnContextCutPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextPastePressed", &::UIAssetsIconsScrollArea::OnContextPastePressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextDeletePressed", &::UIAssetsIconsScrollArea::OnContextDeletePressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextOpenPressed", &::UIAssetsIconsScrollArea::OnContextOpenPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextShowInExplorerPressed", &::UIAssetsIconsScrollArea::OnContextShowInExplorerPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextImportPressed", &::UIAssetsIconsScrollArea::OnContextImportPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextCreateFolderPressed", &::UIAssetsIconsScrollArea::OnContextCreateFolderPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextCreatePrefabPressed", &::UIAssetsIconsScrollArea::OnContextCreatePrefabPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextCreateScriptPressed", &::UIAssetsIconsScrollArea::OnContextCreateScriptPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsIconsScrollArea, void>(&type, "OnContextCreateAnimationPressed", &::UIAssetsIconsScrollArea::OnContextCreateAnimationPressed);
}

void ::AssetsWindow::InitializeType(::AssetsWindow* sample)
{
	TypeInitializer::RegField(&type, "mFilterButton", (size_t)(char*)(&sample->mFilterButton) - (size_t)(char*)sample, sample->mFilterButton);
	TypeInitializer::RegField(&type, "mSearchEditBox", (size_t)(char*)(&sample->mSearchEditBox) - (size_t)(char*)sample, sample->mSearchEditBox);
	TypeInitializer::RegField(&type, "mSelectedAssetPathLabel", (size_t)(char*)(&sample->mSelectedAssetPathLabel) - (size_t)(char*)sample, sample->mSelectedAssetPathLabel);
	TypeInitializer::RegField(&type, "mFoldersTree", (size_t)(char*)(&sample->mFoldersTree) - (size_t)(char*)sample, sample->mFoldersTree);
	TypeInitializer::RegField(&type, "mAssetsGridScroll", (size_t)(char*)(&sample->mAssetsGridScroll) - (size_t)(char*)sample, sample->mAssetsGridScroll);
	TypeInitializer::RegField(&type, "mAssetsTree", (size_t)(char*)(&sample->mAssetsTree) - (size_t)(char*)sample, sample->mAssetsTree);
	TypeInitializer::RegField(&type, "mSeparatorHandle", (size_t)(char*)(&sample->mSeparatorHandle) - (size_t)(char*)sample, sample->mSeparatorHandle);
	TypeInitializer::RegField(&type, "mCuttingAssets", (size_t)(char*)(&sample->mCuttingAssets) - (size_t)(char*)sample, sample->mCuttingAssets);
	auto funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, AssetId>(&type, "SelectAsset", &::AssetsWindow::SelectAsset);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "SelectAsset", &::AssetsWindow::SelectAsset);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const Vector<AssetId>&>(&type, "SelectAsset", &::AssetsWindow::SelectAsset);
	TypeInitializer::RegFuncParam<const Vector<AssetId>&>(funcInfo, "ids");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const Vector<String>&>(&type, "SelectAssets", &::AssetsWindow::SelectAssets);
	TypeInitializer::RegFuncParam<const Vector<String>&>(funcInfo, "paths");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, AssetId>(&type, "OpenAsset", &::AssetsWindow::OpenAsset);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "OpenAsset", &::AssetsWindow::OpenAsset);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, AssetId>(&type, "OpenAndEditAsset", &::AssetsWindow::OpenAndEditAsset);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "OpenAndEditAsset", &::AssetsWindow::OpenAndEditAsset);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void>(&type, "DeselectAssets", &::AssetsWindow::DeselectAssets);
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, Vector<AssetInfo>>(&type, "GetSelectedAssets", &::AssetsWindow::GetSelectedAssets);
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, String>(&type, "GetOpenedFolderPath", &::AssetsWindow::GetOpenedFolderPath);
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "OpenFolder", &::AssetsWindow::OpenFolder);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, AssetId>(&type, "ShowAssetIcon", &::AssetsWindow::ShowAssetIcon);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "ShowAssetIcon", &::AssetsWindow::ShowAssetIcon);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const Vector<String>&>(&type, "CopyAssets", &::AssetsWindow::CopyAssets);
	TypeInitializer::RegFuncParam<const Vector<String>&>(funcInfo, "assetsPaths");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const Vector<String>&>(&type, "CutAssets", &::AssetsWindow::CutAssets);
	TypeInitializer::RegFuncParam<const Vector<String>&>(funcInfo, "assetsPaths");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "PasteAssets", &::AssetsWindow::PasteAssets);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const Vector<String>&>(&type, "DeleteAssets", &::AssetsWindow::DeleteAssets);
	TypeInitializer::RegFuncParam<const Vector<String>&>(funcInfo, "assetsPaths");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "ImportAssets", &::AssetsWindow::ImportAssets);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "CreateFolderAsset", &::AssetsWindow::CreateFolderAsset);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "CreatePrefabAsset", &::AssetsWindow::CreatePrefabAsset);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "CreateScriptAsset", &::AssetsWindow::CreateScriptAsset);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&>(&type, "CreateAnimationAsset", &::AssetsWindow::CreateAnimationAsset);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void>(&type, "InitializeWindow", &::AssetsWindow::InitializeWindow);
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const WString&>(&type, "OnSearchEdited", &::AssetsWindow::OnSearchEdited);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "search");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void>(&type, "OnMenuFilterPressed", &::AssetsWindow::OnMenuFilterPressed);
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void>(&type, "OnShowTreePressed", &::AssetsWindow::OnShowTreePressed);
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const Vector<AssetId>&>(&type, "OnAssetsRebuilded", &::AssetsWindow::OnAssetsRebuilded);
	TypeInitializer::RegFuncParam<const Vector<AssetId>&>(funcInfo, "changedAssets");
	funcInfo = TypeInitializer::RegFunction<::AssetsWindow, void, const String&, const String&>(&type, "CopyAssetFolder", &::AssetsWindow::CopyAssetFolder);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "src");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "dst");
}

void ::UIAssetsFoldersTree::InitializeType(::UIAssetsFoldersTree* sample)
{
	TypeInitializer::RegField(&type, "mFoldersTree", (size_t)(char*)(&sample->mFoldersTree) - (size_t)(char*)sample, sample->mFoldersTree);
	TypeInitializer::RegField(&type, "mContextMenu", (size_t)(char*)(&sample->mContextMenu) - (size_t)(char*)sample, sample->mContextMenu);
	TypeInitializer::RegField(&type, "mCurrentPath", (size_t)(char*)(&sample->mCurrentPath) - (size_t)(char*)sample, sample->mCurrentPath);
	auto funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void, const String&>(&type, "SelectAndExpandFolder", &::UIAssetsFoldersTree::SelectAndExpandFolder);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "RebuildTree", &::UIAssetsFoldersTree::RebuildTree);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "InitializeContext", &::UIAssetsFoldersTree::InitializeContext);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, UnknownType*, UnknownType*>(&type, "GetFoldersTreeNodeParent", &::UIAssetsFoldersTree::GetFoldersTreeNodeParent);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, Vector<UnknownType*>, UnknownType*>(&type, "GetFoldersTreeNodeChilds", &::UIAssetsFoldersTree::GetFoldersTreeNodeChilds);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void, UITreeNode*, UnknownType*>(&type, "SetupFoldersTreeNode", &::UIAssetsFoldersTree::SetupFoldersTreeNode);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void, UITreeNode*>(&type, "OnFoldersTreeNodeDblClick", &::UIAssetsFoldersTree::OnFoldersTreeNodeDblClick);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void, UITreeNode*>(&type, "OnFoldersTreeClick", &::UIAssetsFoldersTree::OnFoldersTreeClick);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void, UITreeNode*>(&type, "OnFoldersTreeRightClick", &::UIAssetsFoldersTree::OnFoldersTreeRightClick);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextCopyPressed", &::UIAssetsFoldersTree::OnContextCopyPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextCutPressed", &::UIAssetsFoldersTree::OnContextCutPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextPastePressed", &::UIAssetsFoldersTree::OnContextPastePressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextDeletePressed", &::UIAssetsFoldersTree::OnContextDeletePressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextOpenPressed", &::UIAssetsFoldersTree::OnContextOpenPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextShowInExplorerPressed", &::UIAssetsFoldersTree::OnContextShowInExplorerPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextImportPressed", &::UIAssetsFoldersTree::OnContextImportPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextCreateFolderPressed", &::UIAssetsFoldersTree::OnContextCreateFolderPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextCreatePrefabPressed", &::UIAssetsFoldersTree::OnContextCreatePrefabPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextCreateScriptPressed", &::UIAssetsFoldersTree::OnContextCreateScriptPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextCreateAnimationPressed", &::UIAssetsFoldersTree::OnContextCreateAnimationPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextExpandPressed", &::UIAssetsFoldersTree::OnContextExpandPressed);
	funcInfo = TypeInitializer::RegFunction<::UIAssetsFoldersTree, void>(&type, "OnContextCollapsePressed", &::UIAssetsFoldersTree::OnContextCollapsePressed);
}

void ::UIAssetIcon::InitializeType(::UIAssetIcon* sample)
{
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText);
	TypeInitializer::RegField(&type, "mAssetInfo", (size_t)(char*)(&sample->mAssetInfo) - (size_t)(char*)sample, sample->mAssetInfo);
	auto funcInfo = TypeInitializer::RegFunction<::UIAssetIcon, void, const AssetInfo&>(&type, "SetAssetInfo", &::UIAssetIcon::SetAssetInfo);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "info");
	funcInfo = TypeInitializer::RegFunction<::UIAssetIcon, const AssetInfo&>(&type, "GetAssetInfo", &::UIAssetIcon::GetAssetInfo);
}

void ::EditorConfig::InitializeType(::EditorConfig* sample)
{
	TypeInitializer::RegField(&type, "mConfigPath", (size_t)(char*)(&sample->mConfigPath) - (size_t)(char*)sample, sample->mConfigPath);
	TypeInitializer::RegField(&type, "mGlobalConfigPath", (size_t)(char*)(&sample->mGlobalConfigPath) - (size_t)(char*)sample, sample->mGlobalConfigPath);
	TypeInitializer::RegField(&type, "mProjectConfig", (size_t)(char*)(&sample->mProjectConfig) - (size_t)(char*)sample, sample->mProjectConfig);
	TypeInitializer::RegField(&type, "mGlobalConfig", (size_t)(char*)(&sample->mGlobalConfig) - (size_t)(char*)sample, sample->mGlobalConfig);
	auto funcInfo = TypeInitializer::RegFunction<::EditorConfig, void>(&type, "SaveGlobalConfigs", &::EditorConfig::SaveGlobalConfigs);
	funcInfo = TypeInitializer::RegFunction<::EditorConfig, void>(&type, "SaveProjectConfigs", &::EditorConfig::SaveProjectConfigs);
	funcInfo = TypeInitializer::RegFunction<::EditorConfig, void>(&type, "LoadConfigs", &::EditorConfig::LoadConfigs);
	funcInfo = TypeInitializer::RegFunction<::EditorConfig, void>(&type, "LoadProjectConfig", &::EditorConfig::LoadProjectConfig);
	funcInfo = TypeInitializer::RegFunction<::EditorConfig, void>(&type, "LoadGlobalConfig", &::EditorConfig::LoadGlobalConfig);
	funcInfo = TypeInitializer::RegFunction<::EditorConfig, void>(&type, "OnWindowChange", &::EditorConfig::OnWindowChange);
}

void ::EditorActorsTransformAction::InitializeType(::EditorActorsTransformAction* sample)
{
	TypeInitializer::RegField(&type, "actorsIds", (size_t)(char*)(&sample->actorsIds) - (size_t)(char*)sample, sample->actorsIds);
	TypeInitializer::RegField(&type, "beforeTransforms", (size_t)(char*)(&sample->beforeTransforms) - (size_t)(char*)sample, sample->beforeTransforms);
	TypeInitializer::RegField(&type, "doneTransforms", (size_t)(char*)(&sample->doneTransforms) - (size_t)(char*)sample, sample->doneTransforms);
	auto funcInfo = TypeInitializer::RegFunction<::EditorActorsTransformAction, String>(&type, "GetName", &::EditorActorsTransformAction::GetName);
	funcInfo = TypeInitializer::RegFunction<::EditorActorsTransformAction, void>(&type, "Redo", &::EditorActorsTransformAction::Redo);
	funcInfo = TypeInitializer::RegFunction<::EditorActorsTransformAction, void>(&type, "Undo", &::EditorActorsTransformAction::Undo);
}

void ::EditorCreateActorsAction::InitializeType(::EditorCreateActorsAction* sample)
{
	TypeInitializer::RegField(&type, "actors", (size_t)(char*)(&sample->actors) - (size_t)(char*)sample, sample->actors);
	TypeInitializer::RegField(&type, "actorsIds", (size_t)(char*)(&sample->actorsIds) - (size_t)(char*)sample, sample->actorsIds);
	TypeInitializer::RegField(&type, "insertParentId", (size_t)(char*)(&sample->insertParentId) - (size_t)(char*)sample, sample->insertParentId);
	TypeInitializer::RegField(&type, "insertPrevActorId", (size_t)(char*)(&sample->insertPrevActorId) - (size_t)(char*)sample, sample->insertPrevActorId);
	auto funcInfo = TypeInitializer::RegFunction<::EditorCreateActorsAction, String>(&type, "GetName", &::EditorCreateActorsAction::GetName);
	funcInfo = TypeInitializer::RegFunction<::EditorCreateActorsAction, void>(&type, "Redo", &::EditorCreateActorsAction::Redo);
	funcInfo = TypeInitializer::RegFunction<::EditorCreateActorsAction, void>(&type, "Undo", &::EditorCreateActorsAction::Undo);
}

void ::EditorDeleteActorsAction::InitializeType(::EditorDeleteActorsAction* sample)
{
	TypeInitializer::RegField(&type, "actorsInfos", (size_t)(char*)(&sample->actorsInfos) - (size_t)(char*)sample, sample->actorsInfos);
	auto funcInfo = TypeInitializer::RegFunction<::EditorDeleteActorsAction, String>(&type, "GetName", &::EditorDeleteActorsAction::GetName);
	funcInfo = TypeInitializer::RegFunction<::EditorDeleteActorsAction, void>(&type, "Redo", &::EditorDeleteActorsAction::Redo);
	funcInfo = TypeInitializer::RegFunction<::EditorDeleteActorsAction, void>(&type, "Undo", &::EditorDeleteActorsAction::Undo);
	funcInfo = TypeInitializer::RegFunction<::EditorDeleteActorsAction, int, Actor*>(&type, "GetActorIdx", &::EditorDeleteActorsAction::GetActorIdx);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
}

void ::EditorEnableAction::InitializeType(::EditorEnableAction* sample)
{
	TypeInitializer::RegField(&type, "actorsIds", (size_t)(char*)(&sample->actorsIds) - (size_t)(char*)sample, sample->actorsIds);
	TypeInitializer::RegField(&type, "enable", (size_t)(char*)(&sample->enable) - (size_t)(char*)sample, sample->enable);
	auto funcInfo = TypeInitializer::RegFunction<::EditorEnableAction, String>(&type, "GetName", &::EditorEnableAction::GetName);
	funcInfo = TypeInitializer::RegFunction<::EditorEnableAction, void>(&type, "Redo", &::EditorEnableAction::Redo);
	funcInfo = TypeInitializer::RegFunction<::EditorEnableAction, void>(&type, "Undo", &::EditorEnableAction::Undo);
}

void ::IEditorAction::InitializeType(::IEditorAction* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<::IEditorAction, String>(&type, "GetName", &::IEditorAction::GetName);
	funcInfo = TypeInitializer::RegFunction<::IEditorAction, void>(&type, "Redo", &::IEditorAction::Redo);
	funcInfo = TypeInitializer::RegFunction<::IEditorAction, void>(&type, "Undo", &::IEditorAction::Undo);
}

void ::EditorLockAction::InitializeType(::EditorLockAction* sample)
{
	TypeInitializer::RegField(&type, "actorsIds", (size_t)(char*)(&sample->actorsIds) - (size_t)(char*)sample, sample->actorsIds);
	TypeInitializer::RegField(&type, "lock", (size_t)(char*)(&sample->lock) - (size_t)(char*)sample, sample->lock);
	auto funcInfo = TypeInitializer::RegFunction<::EditorLockAction, String>(&type, "GetName", &::EditorLockAction::GetName);
	funcInfo = TypeInitializer::RegFunction<::EditorLockAction, void>(&type, "Redo", &::EditorLockAction::Redo);
	funcInfo = TypeInitializer::RegFunction<::EditorLockAction, void>(&type, "Undo", &::EditorLockAction::Undo);
}

void ::EditorReparentActorsAction::InitializeType(::EditorReparentActorsAction* sample)
{
	TypeInitializer::RegField(&type, "actorsInfos", (size_t)(char*)(&sample->actorsInfos) - (size_t)(char*)sample, sample->actorsInfos);
	TypeInitializer::RegField(&type, "newParentId", (size_t)(char*)(&sample->newParentId) - (size_t)(char*)sample, sample->newParentId);
	TypeInitializer::RegField(&type, "newPrevActorId", (size_t)(char*)(&sample->newPrevActorId) - (size_t)(char*)sample, sample->newPrevActorId);
	auto funcInfo = TypeInitializer::RegFunction<::EditorReparentActorsAction, void, Actor*, Actor*>(&type, "ActorsReparented", &::EditorReparentActorsAction::ActorsReparented);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "newParent");
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "prevActor");
	funcInfo = TypeInitializer::RegFunction<::EditorReparentActorsAction, String>(&type, "GetName", &::EditorReparentActorsAction::GetName);
	funcInfo = TypeInitializer::RegFunction<::EditorReparentActorsAction, void>(&type, "Redo", &::EditorReparentActorsAction::Redo);
	funcInfo = TypeInitializer::RegFunction<::EditorReparentActorsAction, void>(&type, "Undo", &::EditorReparentActorsAction::Undo);
}

void ::EditorSelectionAction::InitializeType(::EditorSelectionAction* sample)
{
	TypeInitializer::RegField(&type, "selectedActorsIds", (size_t)(char*)(&sample->selectedActorsIds) - (size_t)(char*)sample, sample->selectedActorsIds);
	TypeInitializer::RegField(&type, "prevSelectedActorsIds", (size_t)(char*)(&sample->prevSelectedActorsIds) - (size_t)(char*)sample, sample->prevSelectedActorsIds);
	auto funcInfo = TypeInitializer::RegFunction<::EditorSelectionAction, String>(&type, "GetName", &::EditorSelectionAction::GetName);
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionAction, void>(&type, "Redo", &::EditorSelectionAction::Redo);
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionAction, void>(&type, "Undo", &::EditorSelectionAction::Undo);
}

void ::EditorFrameTool::InitializeType(::EditorFrameTool* sample)
{
	TypeInitializer::RegField(&type, "mHandleRegularColor", (size_t)(char*)(&sample->mHandleRegularColor) - (size_t)(char*)sample, sample->mHandleRegularColor);
	TypeInitializer::RegField(&type, "mHandleSelectColor", (size_t)(char*)(&sample->mHandleSelectColor) - (size_t)(char*)sample, sample->mHandleSelectColor);
	TypeInitializer::RegField(&type, "mHandlePressedColor", (size_t)(char*)(&sample->mHandlePressedColor) - (size_t)(char*)sample, sample->mHandlePressedColor);
	TypeInitializer::RegField(&type, "mFrameHandlesSize", (size_t)(char*)(&sample->mFrameHandlesSize) - (size_t)(char*)sample, sample->mFrameHandlesSize);
	TypeInitializer::RegField(&type, "mHandlesRotateSize", (size_t)(char*)(&sample->mHandlesRotateSize) - (size_t)(char*)sample, sample->mHandlesRotateSize);
	TypeInitializer::RegField(&type, "mLeftTopRotateHandle", (size_t)(char*)(&sample->mLeftTopRotateHandle) - (size_t)(char*)sample, sample->mLeftTopRotateHandle);
	TypeInitializer::RegField(&type, "mLeftBottomRotateHandle", (size_t)(char*)(&sample->mLeftBottomRotateHandle) - (size_t)(char*)sample, sample->mLeftBottomRotateHandle);
	TypeInitializer::RegField(&type, "mRightTopRotateHandle", (size_t)(char*)(&sample->mRightTopRotateHandle) - (size_t)(char*)sample, sample->mRightTopRotateHandle);
	TypeInitializer::RegField(&type, "mRightBottomRotateHandle", (size_t)(char*)(&sample->mRightBottomRotateHandle) - (size_t)(char*)sample, sample->mRightBottomRotateHandle);
	TypeInitializer::RegField(&type, "mLeftTopHandle", (size_t)(char*)(&sample->mLeftTopHandle) - (size_t)(char*)sample, sample->mLeftTopHandle);
	TypeInitializer::RegField(&type, "mLeftHandle", (size_t)(char*)(&sample->mLeftHandle) - (size_t)(char*)sample, sample->mLeftHandle);
	TypeInitializer::RegField(&type, "mLeftBottomHandle", (size_t)(char*)(&sample->mLeftBottomHandle) - (size_t)(char*)sample, sample->mLeftBottomHandle);
	TypeInitializer::RegField(&type, "mTopHandle", (size_t)(char*)(&sample->mTopHandle) - (size_t)(char*)sample, sample->mTopHandle);
	TypeInitializer::RegField(&type, "mBottomHandle", (size_t)(char*)(&sample->mBottomHandle) - (size_t)(char*)sample, sample->mBottomHandle);
	TypeInitializer::RegField(&type, "mRightTopHandle", (size_t)(char*)(&sample->mRightTopHandle) - (size_t)(char*)sample, sample->mRightTopHandle);
	TypeInitializer::RegField(&type, "mRightHandle", (size_t)(char*)(&sample->mRightHandle) - (size_t)(char*)sample, sample->mRightHandle);
	TypeInitializer::RegField(&type, "mRightBottomHandle", (size_t)(char*)(&sample->mRightBottomHandle) - (size_t)(char*)sample, sample->mRightBottomHandle);
	TypeInitializer::RegField(&type, "mPivotHandle", (size_t)(char*)(&sample->mPivotHandle) - (size_t)(char*)sample, sample->mPivotHandle);
	TypeInitializer::RegField(&type, "mFrame", (size_t)(char*)(&sample->mFrame) - (size_t)(char*)sample, sample->mFrame);
	TypeInitializer::RegField(&type, "mIsDragging", (size_t)(char*)(&sample->mIsDragging) - (size_t)(char*)sample, sample->mIsDragging);
	TypeInitializer::RegField(&type, "mChangedFromThis", (size_t)(char*)(&sample->mChangedFromThis) - (size_t)(char*)sample, sample->mChangedFromThis);
	TypeInitializer::RegField(&type, "mBeforeTransforms", (size_t)(char*)(&sample->mBeforeTransforms) - (size_t)(char*)sample, sample->mBeforeTransforms);
	auto funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void>(&type, "DrawScene", &::EditorFrameTool::DrawScene);
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void>(&type, "OnEnabled", &::EditorFrameTool::OnEnabled);
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void>(&type, "OnDisabled", &::EditorFrameTool::OnDisabled);
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, Vector<Actor*>>(&type, "OnSceneChanged", &::EditorFrameTool::OnSceneChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "changedActors");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &::EditorFrameTool::OnActorsSelectionChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Input::Key&>(&type, "OnKeyPressed", &::EditorFrameTool::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Input::Key&>(&type, "OnKeyStayDown", &::EditorFrameTool::OnKeyStayDown);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Input::Key&>(&type, "OnKeyReleased", &::EditorFrameTool::OnKeyReleased);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Basis&>(&type, "TransformActors", &::EditorFrameTool::TransformActors);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "transform");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Basis&>(&type, "TransformActorsWithAction", &::EditorFrameTool::TransformActorsWithAction);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "transform");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void>(&type, "UpdateSelectionFrame", &::EditorFrameTool::UpdateSelectionFrame);
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Input::Cursor&>(&type, "OnCursorPressed", &::EditorFrameTool::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Input::Cursor&>(&type, "OnCursorReleased", &::EditorFrameTool::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &::EditorFrameTool::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Input::Cursor&>(&type, "OnCursorStillDown", &::EditorFrameTool::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnLeftTopHandle", &::EditorFrameTool::OnLeftTopHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnLeftHandle", &::EditorFrameTool::OnLeftHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnLeftBottomHandle", &::EditorFrameTool::OnLeftBottomHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnTopHandle", &::EditorFrameTool::OnTopHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnBottomHandle", &::EditorFrameTool::OnBottomHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnRightTopHandle", &::EditorFrameTool::OnRightTopHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnRightHandle", &::EditorFrameTool::OnRightHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnRightBottomHandle", &::EditorFrameTool::OnRightBottomHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnPivotHandle", &::EditorFrameTool::OnPivotHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnLeftTopRotateHandle", &::EditorFrameTool::OnLeftTopRotateHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnLeftBottomRotateHandle", &::EditorFrameTool::OnLeftBottomRotateHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnRightTopRotateHandle", &::EditorFrameTool::OnRightTopRotateHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, const Vec2F&>(&type, "OnRightBottomRotateHandle", &::EditorFrameTool::OnRightBottomRotateHandle);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void, bool>(&type, "SetHandlesEnable", &::EditorFrameTool::SetHandlesEnable);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "enable");
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void>(&type, "UdateHandlesTransform", &::EditorFrameTool::UdateHandlesTransform);
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void>(&type, "HandlePressed", &::EditorFrameTool::HandlePressed);
	funcInfo = TypeInitializer::RegFunction<::EditorFrameTool, void>(&type, "HandleReleased", &::EditorFrameTool::HandleReleased);
}

void ::IEditorTool::InitializeType(::IEditorTool* sample)
{
	TypeInitializer::RegField(&type, "mNeedRedraw", (size_t)(char*)(&sample->mNeedRedraw) - (size_t)(char*)sample, sample->mNeedRedraw);
	auto funcInfo = TypeInitializer::RegFunction<::IEditorTool, void>(&type, "DrawScene", &::IEditorTool::DrawScene);
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void>(&type, "DrawScreen", &::IEditorTool::DrawScreen);
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, Vector<Actor*>>(&type, "OnSceneChanged", &::IEditorTool::OnSceneChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "changedActors");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, float>(&type, "Update", &::IEditorTool::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void>(&type, "OnEnabled", &::IEditorTool::OnEnabled);
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void>(&type, "OnDisabled", &::IEditorTool::OnDisabled);
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &::IEditorTool::OnActorsSelectionChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorPressed", &::IEditorTool::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorReleased", &::IEditorTool::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &::IEditorTool::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorStillDown", &::IEditorTool::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorMoved", &::IEditorTool::OnCursorMoved);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorEnter", &::IEditorTool::OnCursorEnter);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorExit", &::IEditorTool::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorRightMousePressed", &::IEditorTool::OnCursorRightMousePressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorRightMouseStayDown", &::IEditorTool::OnCursorRightMouseStayDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &::IEditorTool::OnCursorRightMouseReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorMiddleMousePressed", &::IEditorTool::OnCursorMiddleMousePressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorMiddleMouseStayDown", &::IEditorTool::OnCursorMiddleMouseStayDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Cursor&>(&type, "OnCursorMiddleMouseReleased", &::IEditorTool::OnCursorMiddleMouseReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, float>(&type, "OnScrolled", &::IEditorTool::OnScrolled);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Key&>(&type, "OnKeyPressed", &::IEditorTool::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Key&>(&type, "OnKeyReleased", &::IEditorTool::OnKeyReleased);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::IEditorTool, void, const Input::Key&>(&type, "OnKeyStayDown", &::IEditorTool::OnKeyStayDown);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void ::EditorMoveTool::InitializeType(::EditorMoveTool* sample)
{
	TypeInitializer::RegField(&type, "snapStep", (size_t)(char*)(&sample->snapStep) - (size_t)(char*)sample, sample->snapStep);
	TypeInitializer::RegField(&type, "mHorDragHandle", (size_t)(char*)(&sample->mHorDragHandle) - (size_t)(char*)sample, sample->mHorDragHandle);
	TypeInitializer::RegField(&type, "mVerDragHandle", (size_t)(char*)(&sample->mVerDragHandle) - (size_t)(char*)sample, sample->mVerDragHandle);
	TypeInitializer::RegField(&type, "mBothDragHandle", (size_t)(char*)(&sample->mBothDragHandle) - (size_t)(char*)sample, sample->mBothDragHandle);
	TypeInitializer::RegField(&type, "mLastSceneHandlesPos", (size_t)(char*)(&sample->mLastSceneHandlesPos) - (size_t)(char*)sample, sample->mLastSceneHandlesPos);
	TypeInitializer::RegField(&type, "mSnapPosition", (size_t)(char*)(&sample->mSnapPosition) - (size_t)(char*)sample, sample->mSnapPosition);
	TypeInitializer::RegField(&type, "mHandlesAngle", (size_t)(char*)(&sample->mHandlesAngle) - (size_t)(char*)sample, sample->mHandlesAngle);
	TypeInitializer::RegField(&type, "mBeforeTransforms", (size_t)(char*)(&sample->mBeforeTransforms) - (size_t)(char*)sample, sample->mBeforeTransforms);
	auto funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, float>(&type, "Update", &::EditorMoveTool::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void>(&type, "OnEnabled", &::EditorMoveTool::OnEnabled);
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void>(&type, "OnDisabled", &::EditorMoveTool::OnDisabled);
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, Vector<Actor*>>(&type, "OnSceneChanged", &::EditorMoveTool::OnSceneChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "changedActors");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &::EditorMoveTool::OnActorsSelectionChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, const Vec2F&>(&type, "OnHorDragHandleMoved", &::EditorMoveTool::OnHorDragHandleMoved);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, const Vec2F&>(&type, "OnVerDragHandleMoved", &::EditorMoveTool::OnVerDragHandleMoved);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, const Vec2F&>(&type, "OnBothDragHandleMoved", &::EditorMoveTool::OnBothDragHandleMoved);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void>(&type, "HandlePressed", &::EditorMoveTool::HandlePressed);
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void>(&type, "HandleReleased", &::EditorMoveTool::HandleReleased);
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, const Vec2F&, bool, bool>(&type, "HandlesMoved", &::EditorMoveTool::HandlesMoved);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "snapHor");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "spanVer");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void>(&type, "UpdateHandlesPosition", &::EditorMoveTool::UpdateHandlesPosition);
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, const Input::Key&>(&type, "OnKeyPressed", &::EditorMoveTool::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, const Input::Key&>(&type, "OnKeyStayDown", &::EditorMoveTool::OnKeyStayDown);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, const Input::Key&>(&type, "OnKeyReleased", &::EditorMoveTool::OnKeyReleased);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, const Vec2F&>(&type, "MoveSelectedActors", &::EditorMoveTool::MoveSelectedActors);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
	funcInfo = TypeInitializer::RegFunction<::EditorMoveTool, void, const Vec2F&>(&type, "MoveSelectedActorsWithAction", &::EditorMoveTool::MoveSelectedActorsWithAction);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
}

void ::EditorRotateTool::InitializeType(::EditorRotateTool* sample)
{
	TypeInitializer::RegField(&type, "angleSnapStep", (size_t)(char*)(&sample->angleSnapStep) - (size_t)(char*)sample, sample->angleSnapStep);
	TypeInitializer::RegField(&type, "mRotateRingInsideRadius", (size_t)(char*)(&sample->mRotateRingInsideRadius) - (size_t)(char*)sample, sample->mRotateRingInsideRadius);
	TypeInitializer::RegField(&type, "mRotateRingOutsideRadius", (size_t)(char*)(&sample->mRotateRingOutsideRadius) - (size_t)(char*)sample, sample->mRotateRingOutsideRadius);
	TypeInitializer::RegField(&type, "mRotateRingSegs", (size_t)(char*)(&sample->mRotateRingSegs) - (size_t)(char*)sample, sample->mRotateRingSegs);
	TypeInitializer::RegField(&type, "mRotateRingsColor", (size_t)(char*)(&sample->mRotateRingsColor) - (size_t)(char*)sample, sample->mRotateRingsColor);
	TypeInitializer::RegField(&type, "mRotateRingsFillColor", (size_t)(char*)(&sample->mRotateRingsFillColor) - (size_t)(char*)sample, sample->mRotateRingsFillColor);
	TypeInitializer::RegField(&type, "mRotateRingsFillColor2", (size_t)(char*)(&sample->mRotateRingsFillColor2) - (size_t)(char*)sample, sample->mRotateRingsFillColor2);
	TypeInitializer::RegField(&type, "mRotateMeshClockwiseColor", (size_t)(char*)(&sample->mRotateMeshClockwiseColor) - (size_t)(char*)sample, sample->mRotateMeshClockwiseColor);
	TypeInitializer::RegField(&type, "mRotateMeshCClockwiseColor", (size_t)(char*)(&sample->mRotateMeshCClockwiseColor) - (size_t)(char*)sample, sample->mRotateMeshCClockwiseColor);
	TypeInitializer::RegField(&type, "mRotateRingFillMesh", (size_t)(char*)(&sample->mRotateRingFillMesh) - (size_t)(char*)sample, sample->mRotateRingFillMesh);
	TypeInitializer::RegField(&type, "mAngleMesh", (size_t)(char*)(&sample->mAngleMesh) - (size_t)(char*)sample, sample->mAngleMesh);
	TypeInitializer::RegField(&type, "mScenePivot", (size_t)(char*)(&sample->mScenePivot) - (size_t)(char*)sample, sample->mScenePivot);
	TypeInitializer::RegField(&type, "mPivotDragHandle", (size_t)(char*)(&sample->mPivotDragHandle) - (size_t)(char*)sample, sample->mPivotDragHandle);
	TypeInitializer::RegField(&type, "mPressAngle", (size_t)(char*)(&sample->mPressAngle) - (size_t)(char*)sample, sample->mPressAngle);
	TypeInitializer::RegField(&type, "mCurrentRotateAngle", (size_t)(char*)(&sample->mCurrentRotateAngle) - (size_t)(char*)sample, sample->mCurrentRotateAngle);
	TypeInitializer::RegField(&type, "mRingPressed", (size_t)(char*)(&sample->mRingPressed) - (size_t)(char*)sample, sample->mRingPressed);
	TypeInitializer::RegField(&type, "mSnapAngleAccumulated", (size_t)(char*)(&sample->mSnapAngleAccumulated) - (size_t)(char*)sample, sample->mSnapAngleAccumulated);
	TypeInitializer::RegField(&type, "mBeforeTransforms", (size_t)(char*)(&sample->mBeforeTransforms) - (size_t)(char*)sample, sample->mBeforeTransforms);
	auto funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, float>(&type, "Update", &::EditorRotateTool::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void>(&type, "DrawScreen", &::EditorRotateTool::DrawScreen);
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void>(&type, "OnEnabled", &::EditorRotateTool::OnEnabled);
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void>(&type, "OnDisabled", &::EditorRotateTool::OnDisabled);
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, Vector<Actor*>>(&type, "OnSceneChanged", &::EditorRotateTool::OnSceneChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "changedActors");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &::EditorRotateTool::OnActorsSelectionChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void>(&type, "UpdateMeshes", &::EditorRotateTool::UpdateMeshes);
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void>(&type, "CalcPivotByActorsCenter", &::EditorRotateTool::CalcPivotByActorsCenter);
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, const Vec2F&>(&type, "OnPivotDragHandleMoved", &::EditorRotateTool::OnPivotDragHandleMoved);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, bool, const Vec2F&>(&type, "IsPointInRotateRing", &::EditorRotateTool::IsPointInRotateRing);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, const Input::Cursor&>(&type, "OnCursorPressed", &::EditorRotateTool::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, const Input::Cursor&>(&type, "OnCursorReleased", &::EditorRotateTool::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &::EditorRotateTool::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, const Input::Cursor&>(&type, "OnCursorStillDown", &::EditorRotateTool::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, const Input::Key&>(&type, "OnKeyPressed", &::EditorRotateTool::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, const Input::Key&>(&type, "OnKeyStayDown", &::EditorRotateTool::OnKeyStayDown);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, float>(&type, "RotateActors", &::EditorRotateTool::RotateActors);
	TypeInitializer::RegFuncParam<float>(funcInfo, "angleDelta");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, float>(&type, "RotateActorsSeparated", &::EditorRotateTool::RotateActorsSeparated);
	TypeInitializer::RegFuncParam<float>(funcInfo, "angleDelta");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, float>(&type, "RotateActorsWithAction", &::EditorRotateTool::RotateActorsWithAction);
	TypeInitializer::RegFuncParam<float>(funcInfo, "angleDelta");
	funcInfo = TypeInitializer::RegFunction<::EditorRotateTool, void, float>(&type, "RotateActorsSeparatedWithAction", &::EditorRotateTool::RotateActorsSeparatedWithAction);
	TypeInitializer::RegFuncParam<float>(funcInfo, "angleDelta");
}

void ::EditorScaleTool::InitializeType(::EditorScaleTool* sample)
{
	TypeInitializer::RegField(&type, "bothScaleSence", (size_t)(char*)(&sample->bothScaleSence) - (size_t)(char*)sample, sample->bothScaleSence);
	TypeInitializer::RegField(&type, "mHorDragHandle", (size_t)(char*)(&sample->mHorDragHandle) - (size_t)(char*)sample, sample->mHorDragHandle);
	TypeInitializer::RegField(&type, "mVerDragHandle", (size_t)(char*)(&sample->mVerDragHandle) - (size_t)(char*)sample, sample->mVerDragHandle);
	TypeInitializer::RegField(&type, "mBothDragHandle", (size_t)(char*)(&sample->mBothDragHandle) - (size_t)(char*)sample, sample->mBothDragHandle);
	TypeInitializer::RegField(&type, "mHandlesAngle", (size_t)(char*)(&sample->mHandlesAngle) - (size_t)(char*)sample, sample->mHandlesAngle);
	TypeInitializer::RegField(&type, "mSceneHandlesPos", (size_t)(char*)(&sample->mSceneHandlesPos) - (size_t)(char*)sample, sample->mSceneHandlesPos);
	TypeInitializer::RegField(&type, "mHandlesSize", (size_t)(char*)(&sample->mHandlesSize) - (size_t)(char*)sample, sample->mHandlesSize);
	TypeInitializer::RegField(&type, "mLastHorHandlePos", (size_t)(char*)(&sample->mLastHorHandlePos) - (size_t)(char*)sample, sample->mLastHorHandlePos);
	TypeInitializer::RegField(&type, "mLastVerHandlePos", (size_t)(char*)(&sample->mLastVerHandlePos) - (size_t)(char*)sample, sample->mLastVerHandlePos);
	TypeInitializer::RegField(&type, "mLastBothHandlePos", (size_t)(char*)(&sample->mLastBothHandlePos) - (size_t)(char*)sample, sample->mLastBothHandlePos);
	TypeInitializer::RegField(&type, "mBeforeTransforms", (size_t)(char*)(&sample->mBeforeTransforms) - (size_t)(char*)sample, sample->mBeforeTransforms);
	auto funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, float>(&type, "Update", &::EditorScaleTool::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void>(&type, "DrawScreen", &::EditorScaleTool::DrawScreen);
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void>(&type, "OnEnabled", &::EditorScaleTool::OnEnabled);
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void>(&type, "OnDisabled", &::EditorScaleTool::OnDisabled);
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, Vector<Actor*>>(&type, "OnSceneChanged", &::EditorScaleTool::OnSceneChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "changedActors");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &::EditorScaleTool::OnActorsSelectionChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, const Vec2F&>(&type, "OnHorDragHandleMoved", &::EditorScaleTool::OnHorDragHandleMoved);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, const Vec2F&>(&type, "OnVerDragHandleMoved", &::EditorScaleTool::OnVerDragHandleMoved);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, const Vec2F&>(&type, "OnBothDragHandleMoved", &::EditorScaleTool::OnBothDragHandleMoved);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void>(&type, "UpdateHandlesPosition", &::EditorScaleTool::UpdateHandlesPosition);
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, float>(&type, "UpdateHandlesAngleAndPositions", &::EditorScaleTool::UpdateHandlesAngleAndPositions);
	TypeInitializer::RegFuncParam<float>(funcInfo, "angle");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void>(&type, "UpdateHandlesPositions", &::EditorScaleTool::UpdateHandlesPositions);
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, const Input::Key&>(&type, "OnKeyPressed", &::EditorScaleTool::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, const Input::Key&>(&type, "OnKeyStayDown", &::EditorScaleTool::OnKeyStayDown);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, const Input::Key&>(&type, "OnKeyReleased", &::EditorScaleTool::OnKeyReleased);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void, const Vec2F&>(&type, "ScaleSelectedActors", &::EditorScaleTool::ScaleSelectedActors);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "scale");
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void>(&type, "HandlePressed", &::EditorScaleTool::HandlePressed);
	funcInfo = TypeInitializer::RegFunction<::EditorScaleTool, void>(&type, "HandleReleased", &::EditorScaleTool::HandleReleased);
}

void ::EditorSelectionTool::InitializeType(::EditorSelectionTool* sample)
{
	TypeInitializer::RegField(&type, "mSelectionSprite", (size_t)(char*)(&sample->mSelectionSprite) - (size_t)(char*)sample, sample->mSelectionSprite);
	TypeInitializer::RegField(&type, "mCurrentSelectingActors", (size_t)(char*)(&sample->mCurrentSelectingActors) - (size_t)(char*)sample, sample->mCurrentSelectingActors);
	TypeInitializer::RegField(&type, "mBeforeSelectingActors", (size_t)(char*)(&sample->mBeforeSelectingActors) - (size_t)(char*)sample, sample->mBeforeSelectingActors);
	TypeInitializer::RegField(&type, "mPressPoint", (size_t)(char*)(&sample->mPressPoint) - (size_t)(char*)sample, sample->mPressPoint);
	TypeInitializer::RegField(&type, "mSelectingActors", (size_t)(char*)(&sample->mSelectingActors) - (size_t)(char*)sample, sample->mSelectingActors);
	auto funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void>(&type, "DrawScene", &::EditorSelectionTool::DrawScene);
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void>(&type, "DrawScreen", &::EditorSelectionTool::DrawScreen);
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void, float>(&type, "Update", &::EditorSelectionTool::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void>(&type, "OnEnabled", &::EditorSelectionTool::OnEnabled);
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void>(&type, "OnDisabled", &::EditorSelectionTool::OnDisabled);
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &::EditorSelectionTool::OnActorsSelectionChanged);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void, const Input::Cursor&>(&type, "OnCursorPressed", &::EditorSelectionTool::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void, const Input::Cursor&>(&type, "OnCursorReleased", &::EditorSelectionTool::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &::EditorSelectionTool::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void, const Input::Cursor&>(&type, "OnCursorStillDown", &::EditorSelectionTool::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void, const Input::Cursor&>(&type, "OnCursorMoved", &::EditorSelectionTool::OnCursorMoved);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::EditorSelectionTool, void, const Input::Key&>(&type, "OnKeyPressed", &::EditorSelectionTool::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void ::IEditorWindow::InitializeType(::IEditorWindow* sample)
{
	TypeInitializer::RegField(&type, "visible", (size_t)(char*)(&sample->visible) - (size_t)(char*)sample, sample->visible);
	TypeInitializer::RegField(&type, "mWindow", (size_t)(char*)(&sample->mWindow) - (size_t)(char*)sample, sample->mWindow);
	auto funcInfo = TypeInitializer::RegFunction<::IEditorWindow, void, bool>(&type, "SetVisible", &::IEditorWindow::SetVisible);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "visible");
	funcInfo = TypeInitializer::RegFunction<::IEditorWindow, void, float>(&type, "Update", &::IEditorWindow::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::IEditorWindow, void>(&type, "Draw", &::IEditorWindow::Draw);
	funcInfo = TypeInitializer::RegFunction<::IEditorWindow, bool>(&type, "IsVisible", &::IEditorWindow::IsVisible);
	funcInfo = TypeInitializer::RegFunction<::IEditorWindow, void>(&type, "Show", &::IEditorWindow::Show);
	funcInfo = TypeInitializer::RegFunction<::IEditorWindow, void>(&type, "Hide", &::IEditorWindow::Hide);
	funcInfo = TypeInitializer::RegFunction<::IEditorWindow, void>(&type, "PostInitializeWindow", &::IEditorWindow::PostInitializeWindow);
}

void ::UIDockableWindow::InitializeType(::UIDockableWindow* sample)
{
	TypeInitializer::RegField(&type, "mDockSizeCoef", (size_t)(char*)(&sample->mDockSizeCoef) - (size_t)(char*)sample, sample->mDockSizeCoef);
	TypeInitializer::RegField(&type, "mDockBorder", (size_t)(char*)(&sample->mDockBorder) - (size_t)(char*)sample, sample->mDockBorder);
	TypeInitializer::RegField(&type, "mDocked", (size_t)(char*)(&sample->mDocked) - (size_t)(char*)sample, sample->mDocked);
	TypeInitializer::RegField(&type, "mDockingFrameSample", (size_t)(char*)(&sample->mDockingFrameSample) - (size_t)(char*)sample, sample->mDockingFrameSample).AddAttribute<SerializableAttribute<decltype(mDockingFrameSample)>>();
	TypeInitializer::RegField(&type, "mDockingFrameAppearance", (size_t)(char*)(&sample->mDockingFrameAppearance) - (size_t)(char*)sample, sample->mDockingFrameAppearance);
	TypeInitializer::RegField(&type, "mDockingFrameCurrent", (size_t)(char*)(&sample->mDockingFrameCurrent) - (size_t)(char*)sample, sample->mDockingFrameCurrent);
	TypeInitializer::RegField(&type, "mDockingFrameTarget", (size_t)(char*)(&sample->mDockingFrameTarget) - (size_t)(char*)sample, sample->mDockingFrameTarget);
	TypeInitializer::RegField(&type, "mNonDockSize", (size_t)(char*)(&sample->mNonDockSize) - (size_t)(char*)sample, sample->mNonDockSize);
	TypeInitializer::RegField(&type, "mDragOffset", (size_t)(char*)(&sample->mDragOffset) - (size_t)(char*)sample, sample->mDragOffset);
	auto funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void, float>(&type, "Update", &::UIDockableWindow::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void>(&type, "Draw", &::UIDockableWindow::Draw);
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, bool>(&type, "IsDocked", &::UIDockableWindow::IsDocked);
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, Sprite*>(&type, "GetDockingFrameSample", &::UIDockableWindow::GetDockingFrameSample);
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void>(&type, "InitializeDockFrameAppearanceAnim", &::UIDockableWindow::InitializeDockFrameAppearanceAnim);
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void>(&type, "InitializeDragHandles", &::UIDockableWindow::InitializeDragHandles);
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void, const Input::Cursor&>(&type, "OnMoved", &::UIDockableWindow::OnMoved);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void, const Input::Cursor&>(&type, "OnMoveCompleted", &::UIDockableWindow::OnMoveCompleted);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "");
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void, const Input::Cursor&>(&type, "OnMoveBegin", &::UIDockableWindow::OnMoveBegin);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "");
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, bool, UIDockWindowPlace*&, Side&, RectF&>(&type, "TraceDock", &::UIDockableWindow::TraceDock);
	TypeInitializer::RegFuncParam<UIDockWindowPlace*&>(funcInfo, "targetDock");
	TypeInitializer::RegFuncParam<Side&>(funcInfo, "dockPosition");
	TypeInitializer::RegFuncParam<RectF&>(funcInfo, "dockZoneRect");
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void, UIDockWindowPlace*, Side>(&type, "PlaceNonLineDock", &::UIDockableWindow::PlaceNonLineDock);
	TypeInitializer::RegFuncParam<UIDockWindowPlace*>(funcInfo, "targetDock");
	TypeInitializer::RegFuncParam<Side>(funcInfo, "dockPosition");
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void, UIDockWindowPlace*, Side, RectF>(&type, "PlaceLineDock", &::UIDockableWindow::PlaceLineDock);
	TypeInitializer::RegFuncParam<UIDockWindowPlace*>(funcInfo, "targetDock");
	TypeInitializer::RegFuncParam<Side>(funcInfo, "dockPosition");
	TypeInitializer::RegFuncParam<RectF>(funcInfo, "dockZoneRect");
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void>(&type, "Undock", &::UIDockableWindow::Undock);
	funcInfo = TypeInitializer::RegFunction<::UIDockableWindow, void, bool>(&type, "SetDocked", &::UIDockableWindow::SetDocked);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "docked");
}

void ::UIDockWindowPlace::InitializeType(::UIDockWindowPlace* sample)
{
	TypeInitializer::RegField(&type, "mResizibleDir", (size_t)(char*)(&sample->mResizibleDir) - (size_t)(char*)sample, sample->mResizibleDir);
	TypeInitializer::RegField(&type, "mNeighborMin", (size_t)(char*)(&sample->mNeighborMin) - (size_t)(char*)sample, sample->mNeighborMin);
	TypeInitializer::RegField(&type, "mDragHandleMin", (size_t)(char*)(&sample->mDragHandleMin) - (size_t)(char*)sample, sample->mDragHandleMin);
	TypeInitializer::RegField(&type, "mDragHandleLayoutMin", (size_t)(char*)(&sample->mDragHandleLayoutMin) - (size_t)(char*)sample, sample->mDragHandleLayoutMin);
	TypeInitializer::RegField(&type, "mDragHandleAreaMin", (size_t)(char*)(&sample->mDragHandleAreaMin) - (size_t)(char*)sample, sample->mDragHandleAreaMin);
	TypeInitializer::RegField(&type, "mNeighborMax", (size_t)(char*)(&sample->mNeighborMax) - (size_t)(char*)sample, sample->mNeighborMax);
	TypeInitializer::RegField(&type, "mDragHandleMax", (size_t)(char*)(&sample->mDragHandleMax) - (size_t)(char*)sample, sample->mDragHandleMax);
	TypeInitializer::RegField(&type, "mDragHandleLayoutMax", (size_t)(char*)(&sample->mDragHandleLayoutMax) - (size_t)(char*)sample, sample->mDragHandleLayoutMax);
	TypeInitializer::RegField(&type, "mDragHandleAreaMax", (size_t)(char*)(&sample->mDragHandleAreaMax) - (size_t)(char*)sample, sample->mDragHandleAreaMax);
	TypeInitializer::RegField(&type, "mDragHandleDepth", (size_t)(char*)(&sample->mDragHandleDepth) - (size_t)(char*)sample, sample->mDragHandleDepth);
	auto funcInfo = TypeInitializer::RegFunction<::UIDockWindowPlace, void>(&type, "Draw", &::UIDockWindowPlace::Draw);
	funcInfo = TypeInitializer::RegFunction<::UIDockWindowPlace, bool, const Vec2F&>(&type, "IsUnderPoint", &::UIDockWindowPlace::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<::UIDockWindowPlace, void, TwoDirection, float, UIDockWindowPlace*, UIDockWindowPlace*>(&type, "SetResizibleDir", &::UIDockWindowPlace::SetResizibleDir);
	TypeInitializer::RegFuncParam<TwoDirection>(funcInfo, "dir");
	TypeInitializer::RegFuncParam<float>(funcInfo, "border");
	TypeInitializer::RegFuncParam<UIDockWindowPlace*>(funcInfo, "neighborMin");
	TypeInitializer::RegFuncParam<UIDockWindowPlace*>(funcInfo, "neighborMax");
	funcInfo = TypeInitializer::RegFunction<::UIDockWindowPlace, TwoDirection>(&type, "GetResizibleDir", &::UIDockWindowPlace::GetResizibleDir);
	funcInfo = TypeInitializer::RegFunction<::UIDockWindowPlace, void, bool>(&type, "UpdateLayout", &::UIDockWindowPlace::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<::UIDockWindowPlace, void, const Vec2F&>(&type, "OnDragHandleMinMoved", &::UIDockWindowPlace::OnDragHandleMinMoved);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
	funcInfo = TypeInitializer::RegFunction<::UIDockWindowPlace, void, const Vec2F&>(&type, "OnDragHandleMaxMoved", &::UIDockWindowPlace::OnDragHandleMaxMoved);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
	funcInfo = TypeInitializer::RegFunction<::UIDockWindowPlace, void>(&type, "CheckInteractable", &::UIDockWindowPlace::CheckInteractable);
	funcInfo = TypeInitializer::RegFunction<::UIDockWindowPlace, void>(&type, "InitializeDragHandle", &::UIDockWindowPlace::InitializeDragHandle);
}

void ::WindowsLayout::InitializeType(::WindowsLayout* sample)
{
	TypeInitializer::RegField(&type, "mainDock", (size_t)(char*)(&sample->mainDock) - (size_t)(char*)sample, sample->mainDock).AddAttribute<SerializableAttribute<decltype(mainDock)>>();
	TypeInitializer::RegField(&type, "windows", (size_t)(char*)(&sample->windows) - (size_t)(char*)sample, sample->windows).AddAttribute<SerializableAttribute<decltype(windows)>>();
	auto funcInfo = TypeInitializer::RegFunction<::WindowsLayout, void, WindowDockPlace*, UIDockWindowPlace*>(&type, "RestoreDock", &::WindowsLayout::RestoreDock);
	TypeInitializer::RegFuncParam<WindowDockPlace*>(funcInfo, "dockDef");
	TypeInitializer::RegFuncParam<UIDockWindowPlace*>(funcInfo, "dockWidget");
}

void ::LogWindow::InitializeType(::LogWindow* sample)
{
	TypeInitializer::RegField(&type, "mList", (size_t)(char*)(&sample->mList) - (size_t)(char*)sample, sample->mList);
	TypeInitializer::RegField(&type, "mLastMessageView", (size_t)(char*)(&sample->mLastMessageView) - (size_t)(char*)sample, sample->mLastMessageView);
	TypeInitializer::RegField(&type, "mMessagesCountLabel", (size_t)(char*)(&sample->mMessagesCountLabel) - (size_t)(char*)sample, sample->mMessagesCountLabel);
	TypeInitializer::RegField(&type, "mWarningsCountLabel", (size_t)(char*)(&sample->mWarningsCountLabel) - (size_t)(char*)sample, sample->mWarningsCountLabel);
	TypeInitializer::RegField(&type, "mErrorsCountLabel", (size_t)(char*)(&sample->mErrorsCountLabel) - (size_t)(char*)sample, sample->mErrorsCountLabel);
	TypeInitializer::RegField(&type, "mAllMessages", (size_t)(char*)(&sample->mAllMessages) - (size_t)(char*)sample, sample->mAllMessages);
	TypeInitializer::RegField(&type, "mVisibleMessages", (size_t)(char*)(&sample->mVisibleMessages) - (size_t)(char*)sample, sample->mVisibleMessages);
	TypeInitializer::RegField(&type, "mRegularMessagesEnabled", (size_t)(char*)(&sample->mRegularMessagesEnabled) - (size_t)(char*)sample, sample->mRegularMessagesEnabled);
	TypeInitializer::RegField(&type, "mWarningMessagesEnabled", (size_t)(char*)(&sample->mWarningMessagesEnabled) - (size_t)(char*)sample, sample->mWarningMessagesEnabled);
	TypeInitializer::RegField(&type, "mErrorMessagesEnabled", (size_t)(char*)(&sample->mErrorMessagesEnabled) - (size_t)(char*)sample, sample->mErrorMessagesEnabled);
	TypeInitializer::RegField(&type, "mRegularMessagesCount", (size_t)(char*)(&sample->mRegularMessagesCount) - (size_t)(char*)sample, sample->mRegularMessagesCount);
	TypeInitializer::RegField(&type, "mWarningMessagesCount", (size_t)(char*)(&sample->mWarningMessagesCount) - (size_t)(char*)sample, sample->mWarningMessagesCount);
	TypeInitializer::RegField(&type, "mErrorMessagesCount", (size_t)(char*)(&sample->mErrorMessagesCount) - (size_t)(char*)sample, sample->mErrorMessagesCount);
	auto funcInfo = TypeInitializer::RegFunction<::LogWindow, void, float>(&type, "Update", &::LogWindow::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void>(&type, "InitializeWindow", &::LogWindow::InitializeWindow);
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void>(&type, "OnClearPressed", &::LogWindow::OnClearPressed);
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void, bool>(&type, "OnRegularMessagesToggled", &::LogWindow::OnRegularMessagesToggled);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void, bool>(&type, "OnWarningMessagesToggled", &::LogWindow::OnWarningMessagesToggled);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void, bool>(&type, "OnErrorMessagesToggled", &::LogWindow::OnErrorMessagesToggled);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void>(&type, "UpdateVisibleMessages", &::LogWindow::UpdateVisibleMessages);
	funcInfo = TypeInitializer::RegFunction<::LogWindow, int>(&type, "GetVisibleMessagesCount", &::LogWindow::GetVisibleMessagesCount);
	funcInfo = TypeInitializer::RegFunction<::LogWindow, Vector<UnknownType*>, int, int>(&type, "GetVisibleMessagesRange", &::LogWindow::GetVisibleMessagesRange);
	TypeInitializer::RegFuncParam<int>(funcInfo, "min");
	TypeInitializer::RegFuncParam<int>(funcInfo, "max");
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void, UIWidget*, UnknownType*>(&type, "SetupListMessage", &::LogWindow::SetupListMessage);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void, const WString&>(&type, "OutStrEx", &::LogWindow::OutStrEx);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "str");
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void, const WString&>(&type, "OutErrorEx", &::LogWindow::OutErrorEx);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "str");
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void, const WString&>(&type, "OutWarningEx", &::LogWindow::OutWarningEx);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "str");
	funcInfo = TypeInitializer::RegFunction<::LogWindow, void>(&type, "UpdateLastMessageView", &::LogWindow::UpdateLastMessageView);
}

void ::SceneEditScreen::InitializeType(::SceneEditScreen* sample)
{
	TypeInitializer::RegField(&type, "mRectangle", (size_t)(char*)(&sample->mRectangle) - (size_t)(char*)sample, sample->mRectangle);
	TypeInitializer::RegField(&type, "mViewCamera", (size_t)(char*)(&sample->mViewCamera) - (size_t)(char*)sample, sample->mViewCamera);
	TypeInitializer::RegField(&type, "mViewCameraTargetScale", (size_t)(char*)(&sample->mViewCameraTargetScale) - (size_t)(char*)sample, sample->mViewCameraTargetScale);
	TypeInitializer::RegField(&type, "mViewCameraScaleSence", (size_t)(char*)(&sample->mViewCameraScaleSence) - (size_t)(char*)sample, sample->mViewCameraScaleSence);
	TypeInitializer::RegField(&type, "mViewCameraScaleElasticyCoef", (size_t)(char*)(&sample->mViewCameraScaleElasticyCoef) - (size_t)(char*)sample, sample->mViewCameraScaleElasticyCoef);
	TypeInitializer::RegField(&type, "mViewCameraTargetPos", (size_t)(char*)(&sample->mViewCameraTargetPos) - (size_t)(char*)sample, sample->mViewCameraTargetPos);
	TypeInitializer::RegField(&type, "mViewCameraVelocity", (size_t)(char*)(&sample->mViewCameraVelocity) - (size_t)(char*)sample, sample->mViewCameraVelocity);
	TypeInitializer::RegField(&type, "mViewCameraPosElasticyCoef", (size_t)(char*)(&sample->mViewCameraPosElasticyCoef) - (size_t)(char*)sample, sample->mViewCameraPosElasticyCoef);
	TypeInitializer::RegField(&type, "mViewCameraVelocityDampingCoef", (size_t)(char*)(&sample->mViewCameraVelocityDampingCoef) - (size_t)(char*)sample, sample->mViewCameraVelocityDampingCoef);
	TypeInitializer::RegField(&type, "mViewCameraMinScale", (size_t)(char*)(&sample->mViewCameraMinScale) - (size_t)(char*)sample, sample->mViewCameraMinScale);
	TypeInitializer::RegField(&type, "mViewCameraMaxScale", (size_t)(char*)(&sample->mViewCameraMaxScale) - (size_t)(char*)sample, sample->mViewCameraMaxScale);
	TypeInitializer::RegField(&type, "mBackColor", (size_t)(char*)(&sample->mBackColor) - (size_t)(char*)sample, sample->mBackColor);
	TypeInitializer::RegField(&type, "mGridColor", (size_t)(char*)(&sample->mGridColor) - (size_t)(char*)sample, sample->mGridColor);
	TypeInitializer::RegField(&type, "mSelectedActorColor", (size_t)(char*)(&sample->mSelectedActorColor) - (size_t)(char*)sample, sample->mSelectedActorColor);
	TypeInitializer::RegField(&type, "mMultiSelectedActorColor", (size_t)(char*)(&sample->mMultiSelectedActorColor) - (size_t)(char*)sample, sample->mMultiSelectedActorColor);
	TypeInitializer::RegField(&type, "mActorMinimalSelectionSize", (size_t)(char*)(&sample->mActorMinimalSelectionSize) - (size_t)(char*)sample, sample->mActorMinimalSelectionSize);
	TypeInitializer::RegField(&type, "mSceneToScreenTransform", (size_t)(char*)(&sample->mSceneToScreenTransform) - (size_t)(char*)sample, sample->mSceneToScreenTransform);
	TypeInitializer::RegField(&type, "mScreenToSceneTransform", (size_t)(char*)(&sample->mScreenToSceneTransform) - (size_t)(char*)sample, sample->mScreenToSceneTransform);
	TypeInitializer::RegField(&type, "mRenderTarget", (size_t)(char*)(&sample->mRenderTarget) - (size_t)(char*)sample, sample->mRenderTarget);
	TypeInitializer::RegField(&type, "mRenderTargetSprite", (size_t)(char*)(&sample->mRenderTargetSprite) - (size_t)(char*)sample, sample->mRenderTargetSprite);
	TypeInitializer::RegField(&type, "mNeedRedraw", (size_t)(char*)(&sample->mNeedRedraw) - (size_t)(char*)sample, sample->mNeedRedraw);
	TypeInitializer::RegField(&type, "mDrawDepth", (size_t)(char*)(&sample->mDrawDepth) - (size_t)(char*)sample, sample->mDrawDepth);
	TypeInitializer::RegField(&type, "mActorsTree", (size_t)(char*)(&sample->mActorsTree) - (size_t)(char*)sample, sample->mActorsTree);
	TypeInitializer::RegField(&type, "mSelectedActors", (size_t)(char*)(&sample->mSelectedActors) - (size_t)(char*)sample, sample->mSelectedActors);
	TypeInitializer::RegField(&type, "mTopSelectedActors", (size_t)(char*)(&sample->mTopSelectedActors) - (size_t)(char*)sample, sample->mTopSelectedActors);
	TypeInitializer::RegField(&type, "mSelectedFromThis", (size_t)(char*)(&sample->mSelectedFromThis) - (size_t)(char*)sample, sample->mSelectedFromThis);
	TypeInitializer::RegField(&type, "mTools", (size_t)(char*)(&sample->mTools) - (size_t)(char*)sample, sample->mTools);
	TypeInitializer::RegField(&type, "mEnabledTool", (size_t)(char*)(&sample->mEnabledTool) - (size_t)(char*)sample, sample->mEnabledTool);
	TypeInitializer::RegField(&type, "mDragHandles", (size_t)(char*)(&sample->mDragHandles) - (size_t)(char*)sample, sample->mDragHandles);
	TypeInitializer::RegField(&type, "mPressedHandles", (size_t)(char*)(&sample->mPressedHandles) - (size_t)(char*)sample, sample->mPressedHandles);
	TypeInitializer::RegField(&type, "mRightButtonPressedHandle", (size_t)(char*)(&sample->mRightButtonPressedHandle) - (size_t)(char*)sample, sample->mRightButtonPressedHandle);
	TypeInitializer::RegField(&type, "mMiddleButtonPressedHandle", (size_t)(char*)(&sample->mMiddleButtonPressedHandle) - (size_t)(char*)sample, sample->mMiddleButtonPressedHandle);
	TypeInitializer::RegField(&type, "mUnderCursorHandles", (size_t)(char*)(&sample->mUnderCursorHandles) - (size_t)(char*)sample, sample->mUnderCursorHandles);
	TypeInitializer::RegField(&type, "mLastUnderCursorHandles", (size_t)(char*)(&sample->mLastUnderCursorHandles) - (size_t)(char*)sample, sample->mLastUnderCursorHandles);
	auto funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "Draw", &::SceneEditScreen::Draw);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, float>(&type, "Update", &::SceneEditScreen::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, bool>(&type, "IsScrollable", &::SceneEditScreen::IsScrollable);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, Vec2F, const Vec2F&>(&type, "ScreenToScenePoint", &::SceneEditScreen::ScreenToScenePoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, Vec2F, const Vec2F&>(&type, "SceneToScreenPoint", &::SceneEditScreen::SceneToScreenPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, Vec2F, const Vec2F&>(&type, "ScreenToSceneVector", &::SceneEditScreen::ScreenToSceneVector);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, Vec2F, const Vec2F&>(&type, "SceneToScreenVector", &::SceneEditScreen::SceneToScreenVector);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const RectF&>(&type, "SetRect", &::SceneEditScreen::SetRect);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, RectF>(&type, "GetRect", &::SceneEditScreen::GetRect);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, Actor*, const Color4&>(&type, "DrawActorSelection", &::SceneEditScreen::DrawActorSelection);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, ActorsVec, bool>(&type, "SelectActors", &::SceneEditScreen::SelectActors);
	TypeInitializer::RegFuncParam<ActorsVec>(funcInfo, "actors");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "additive");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, Actor*, bool>(&type, "SelectActor", &::SceneEditScreen::SelectActor);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "additive");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "SelectAllActors", &::SceneEditScreen::SelectAllActors);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "ClearSelection", &::SceneEditScreen::ClearSelection);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, const ActorsVec&>(&type, "GetSelectedActors", &::SceneEditScreen::GetSelectedActors);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, const ActorsVec&>(&type, "GetTopSelectedActors", &::SceneEditScreen::GetTopSelectedActors);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, const Color4&>(&type, "GetSingleActorSelectionColor", &::SceneEditScreen::GetSingleActorSelectionColor);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, const Color4&>(&type, "GetManyActorsSelectionColor", &::SceneEditScreen::GetManyActorsSelectionColor);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, bool, const Vec2F&>(&type, "IsUnderPoint", &::SceneEditScreen::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, Type*>(&type, "InitializeTools", &::SceneEditScreen::InitializeTools);
	TypeInitializer::RegFuncParam<Type*>(funcInfo, "toolType");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "UpdateHandles", &::SceneEditScreen::UpdateHandles);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, bool, const Input::Cursor&>(&type, "IsHandleWorking", &::SceneEditScreen::IsHandleWorking);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorPressed", &::SceneEditScreen::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorReleased", &::SceneEditScreen::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &::SceneEditScreen::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorStillDown", &::SceneEditScreen::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorMoved", &::SceneEditScreen::OnCursorMoved);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorEnter", &::SceneEditScreen::OnCursorEnter);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorExit", &::SceneEditScreen::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorRightMousePressed", &::SceneEditScreen::OnCursorRightMousePressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorRightMouseStayDown", &::SceneEditScreen::OnCursorRightMouseStayDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &::SceneEditScreen::OnCursorRightMouseReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorMiddleMousePressed", &::SceneEditScreen::OnCursorMiddleMousePressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorMiddleMouseStayDown", &::SceneEditScreen::OnCursorMiddleMouseStayDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorMiddleMouseReleased", &::SceneEditScreen::OnCursorMiddleMouseReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, float>(&type, "OnScrolled", &::SceneEditScreen::OnScrolled);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Key&>(&type, "OnKeyPressed", &::SceneEditScreen::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Key&>(&type, "OnKeyReleased", &::SceneEditScreen::OnKeyReleased);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, const Input::Key&>(&type, "OnKeyStayDown", &::SceneEditScreen::OnKeyStayDown);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "OnActorsSelectedFromThis", &::SceneEditScreen::OnActorsSelectedFromThis);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, float>(&type, "UpdateCamera", &::SceneEditScreen::UpdateCamera);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "RedrawScene", &::SceneEditScreen::RedrawScene);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "DrawGrid", &::SceneEditScreen::DrawGrid);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "DrawActors", &::SceneEditScreen::DrawActors);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "DrawSelection", &::SceneEditScreen::DrawSelection);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "UpdateSceneScreenTransforms", &::SceneEditScreen::UpdateSceneScreenTransforms);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "BindActorsTree", &::SceneEditScreen::BindActorsTree);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, Vector<UnknownType*>>(&type, "OnTreeSelectionChanged", &::SceneEditScreen::OnTreeSelectionChanged);
	TypeInitializer::RegFuncParam<Vector<UnknownType*>>(funcInfo, "selectedObjects");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "UpdateTopSelectedActors", &::SceneEditScreen::UpdateTopSelectedActors);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, int, Actor*>(&type, "GetActorIdx", &::SceneEditScreen::GetActorIdx);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, ActorsVec>(&type, "OnSceneChanged", &::SceneEditScreen::OnSceneChanged);
	TypeInitializer::RegFuncParam<ActorsVec>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void>(&type, "ClearSelectionWithoutAction", &::SceneEditScreen::ClearSelectionWithoutAction);
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, ActorsVec, bool>(&type, "SelectActorsWithoutAction", &::SceneEditScreen::SelectActorsWithoutAction);
	TypeInitializer::RegFuncParam<ActorsVec>(funcInfo, "actors");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "additive");
	funcInfo = TypeInitializer::RegFunction<::SceneEditScreen, void, Actor*, bool>(&type, "SelectActorWithoutAction", &::SceneEditScreen::SelectActorWithoutAction);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "additive");
}

void ::SceneEditWidget::InitializeType(::SceneEditWidget* sample)
{
	TypeInitializer::RegField(&type, "mSceneEditScreen", (size_t)(char*)(&sample->mSceneEditScreen) - (size_t)(char*)sample, sample->mSceneEditScreen);
	auto funcInfo = TypeInitializer::RegFunction<::SceneEditWidget, void>(&type, "Draw", &::SceneEditWidget::Draw);
	funcInfo = TypeInitializer::RegFunction<::SceneEditWidget, void, float>(&type, "Update", &::SceneEditWidget::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<::SceneEditWidget, void, bool>(&type, "UpdateLayout", &::SceneEditWidget::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
}

void ::SceneWindow::InitializeType(::SceneWindow* sample)
{
	TypeInitializer::RegField(&type, "mEditWidget", (size_t)(char*)(&sample->mEditWidget) - (size_t)(char*)sample, sample->mEditWidget);
	TypeInitializer::RegField(&type, "mLayersView", (size_t)(char*)(&sample->mLayersView) - (size_t)(char*)sample, sample->mLayersView);
	TypeInitializer::RegField(&type, "mGizomsView", (size_t)(char*)(&sample->mGizomsView) - (size_t)(char*)sample, sample->mGizomsView);
	auto funcInfo = TypeInitializer::RegFunction<::SceneWindow, void>(&type, "InitializeWindow", &::SceneWindow::InitializeWindow);
	funcInfo = TypeInitializer::RegFunction<::SceneWindow, void>(&type, "InitializeLayersView", &::SceneWindow::InitializeLayersView);
	funcInfo = TypeInitializer::RegFunction<::SceneWindow, void>(&type, "PostInitializeWindow", &::SceneWindow::PostInitializeWindow);
}

void ::TreeWindow::InitializeType(::TreeWindow* sample)
{
	TypeInitializer::RegField(&type, "mListTreeToggle", (size_t)(char*)(&sample->mListTreeToggle) - (size_t)(char*)sample, sample->mListTreeToggle);
	TypeInitializer::RegField(&type, "mSearchEditBox", (size_t)(char*)(&sample->mSearchEditBox) - (size_t)(char*)sample, sample->mSearchEditBox);
	TypeInitializer::RegField(&type, "mActorsTree", (size_t)(char*)(&sample->mActorsTree) - (size_t)(char*)sample, sample->mActorsTree);
	TypeInitializer::RegField(&type, "mEnableActorsTogglesGroup", (size_t)(char*)(&sample->mEnableActorsTogglesGroup) - (size_t)(char*)sample, sample->mEnableActorsTogglesGroup);
	TypeInitializer::RegField(&type, "mLockActorsTogglesGroup", (size_t)(char*)(&sample->mLockActorsTogglesGroup) - (size_t)(char*)sample, sample->mLockActorsTogglesGroup);
	TypeInitializer::RegField(&type, "mTreeContextMenu", (size_t)(char*)(&sample->mTreeContextMenu) - (size_t)(char*)sample, sample->mTreeContextMenu);
	TypeInitializer::RegField(&type, "mInSearch", (size_t)(char*)(&sample->mInSearch) - (size_t)(char*)sample, sample->mInSearch);
	TypeInitializer::RegField(&type, "mSearchActors", (size_t)(char*)(&sample->mSearchActors) - (size_t)(char*)sample, sample->mSearchActors);
	auto funcInfo = TypeInitializer::RegFunction<::TreeWindow, UITree*>(&type, "GetActorsTree", &::TreeWindow::GetActorsTree);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, Actor*>(&type, "ExpandActorsTreeNode", &::TreeWindow::ExpandActorsTreeNode);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "targetActor");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "InitializeWindow", &::TreeWindow::InitializeWindow);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "PostInitializeWindow", &::TreeWindow::PostInitializeWindow);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnSearchPressed", &::TreeWindow::OnSearchPressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, bool>(&type, "OnListTreeToggled", &::TreeWindow::OnListTreeToggled);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, const WString&>(&type, "OnSearchEdited", &::TreeWindow::OnSearchEdited);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "searchStr");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, Actor*, const String&>(&type, "SearchActorsRecursive", &::TreeWindow::SearchActorsRecursive);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "searchStr");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, UnknownType*, UnknownType*>(&type, "GetActorsParent", &::TreeWindow::GetActorsParent);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "obj");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, Vector<UnknownType*>, UnknownType*>(&type, "GetActorsChildren", &::TreeWindow::GetActorsChildren);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "parentObj");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, UITreeNode*, UnknownType*>(&type, "SetupTreeNodeActor", &::TreeWindow::SetupTreeNodeActor);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "actorObj");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, Vector<UnknownType*>, UnknownType*, UnknownType*>(&type, "RearrangeActors", &::TreeWindow::RearrangeActors);
	TypeInitializer::RegFuncParam<Vector<UnknownType*>>(funcInfo, "objects");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "parentObj");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "prevObj");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, UITreeNode*>(&type, "OnTreeNodeDblClick", &::TreeWindow::OnTreeNodeDblClick);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, UITreeNode*>(&type, "OnTreeRBPressed", &::TreeWindow::OnTreeRBPressed);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, bool>(&type, "EnableActorsGroupPressed", &::TreeWindow::EnableActorsGroupPressed);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, bool>(&type, "EnableActorsGroupReleased", &::TreeWindow::EnableActorsGroupReleased);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, bool>(&type, "LockActorsGroupPressed", &::TreeWindow::LockActorsGroupPressed);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, bool>(&type, "LockActorsGroupReleased", &::TreeWindow::LockActorsGroupReleased);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, Actor*>(&type, "CreateActor", &::TreeWindow::CreateActor);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "newActor");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextCreateNewPressed", &::TreeWindow::OnContextCreateNewPressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextCreateSprite", &::TreeWindow::OnContextCreateSprite);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextCreateButton", &::TreeWindow::OnContextCreateButton);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextCopyPressed", &::TreeWindow::OnContextCopyPressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextCutPressed", &::TreeWindow::OnContextCutPressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextPastePressed", &::TreeWindow::OnContextPastePressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextDeletePressed", &::TreeWindow::OnContextDeletePressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextDuplicatePressed", &::TreeWindow::OnContextDuplicatePressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextExpandAllPressed", &::TreeWindow::OnContextExpandAllPressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextCollapseAllPressed", &::TreeWindow::OnContextCollapseAllPressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextLockPressed", &::TreeWindow::OnContextLockPressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void>(&type, "OnContextEnablePressed", &::TreeWindow::OnContextEnablePressed);
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, Actor*>(&type, "OnActorCreated", &::TreeWindow::OnActorCreated);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<::TreeWindow, void, Actor*>(&type, "OnActorDestroyed", &::TreeWindow::OnActorDestroyed);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
}

void o2::Animatable::InitializeType(o2::Animatable* sample)
{
	TypeInitializer::RegField(&type, "mStates", (size_t)(char*)(&sample->mStates) - (size_t)(char*)sample, sample->mStates).AddAttribute<SerializableAttribute<decltype(mStates)>>();
	TypeInitializer::RegField(&type, "mValues", (size_t)(char*)(&sample->mValues) - (size_t)(char*)sample, sample->mValues);
	TypeInitializer::RegField(&type, "mBlend", (size_t)(char*)(&sample->mBlend) - (size_t)(char*)sample, sample->mBlend);
	auto funcInfo = TypeInitializer::RegFunction<o2::Animatable, void, float>(&type, "Update", &o2::Animatable::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, AnimationState*>(&type, "AddState", &o2::Animatable::AddState);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const String&, const Animation&, const AnimationMask&, float>(&type, "AddState", &o2::Animatable::AddState);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	TypeInitializer::RegFuncParam<const AnimationMask&>(funcInfo, "mask");
	TypeInitializer::RegFuncParam<float>(funcInfo, "weight");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const String&>(&type, "AddState", &o2::Animatable::AddState);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void, AnimationState*>(&type, "RemoveState", &o2::Animatable::RemoveState);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void, const String&>(&type, "RemoveState", &o2::Animatable::RemoveState);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void>(&type, "RemoveAllStates", &o2::Animatable::RemoveAllStates);
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const String&>(&type, "GetState", &o2::Animatable::GetState);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, const AnimationStatesVec&>(&type, "GetStates", &o2::Animatable::GetStates);
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const Animation&, const String&>(&type, "Play", &o2::Animatable::Play);
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const Animation&>(&type, "Play", &o2::Animatable::Play);
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const String&>(&type, "Play", &o2::Animatable::Play);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const Animation&, const String&, float>(&type, "BlendTo", &o2::Animatable::BlendTo);
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<float>(funcInfo, "duration");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const Animation&, float>(&type, "BlendTo", &o2::Animatable::BlendTo);
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	TypeInitializer::RegFuncParam<float>(funcInfo, "duration");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const String&, float>(&type, "BlendTo", &o2::Animatable::BlendTo);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<float>(funcInfo, "duration");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, AnimationState*, float>(&type, "BlendTo", &o2::Animatable::BlendTo);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<float>(funcInfo, "duration");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void, const String&>(&type, "Stop", &o2::Animatable::Stop);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "animationName");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void>(&type, "StopAll", &o2::Animatable::StopAll);
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void, IAnimatedValue*, const String&>(&type, "UnregAnimatedValue", &o2::Animatable::UnregAnimatedValue);
	TypeInitializer::RegFuncParam<IAnimatedValue*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
}

void o2::AnimatedValue<float>::InitializeType(o2::AnimatedValue<float>* sample)
{
	TypeInitializer::RegField(&type, "curve", (size_t)(char*)(&sample->curve) - (size_t)(char*)sample, sample->curve).AddAttribute<SerializableAttribute<decltype(curve)>>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)sample, sample->target);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)sample, sample->targetDelegate);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)sample, sample->targetProperty);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)sample, sample->mTargetDelegate);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)sample, sample->mTargetProperty);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, float*>(&type, "SetTarget", &o2::AnimatedValue<float>::SetTarget);
	TypeInitializer::RegFuncParam<float*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, float*, const Function<void()>&>(&type, "SetTarget", &o2::AnimatedValue<float>::SetTarget);
	TypeInitializer::RegFuncParam<float*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::AnimatedValue<float>::SetTargetDelegate);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, Setter<float>*>(&type, "SetTargetProperty", &o2::AnimatedValue<float>::SetTargetProperty);
	TypeInitializer::RegFuncParam<Setter<float>*>(funcInfo, "setter");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, float>(&type, "GetValue", &o2::AnimatedValue<float>::GetValue);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, float, float>(&type, "GetValue", &o2::AnimatedValue<float>::GetValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, Vector<Vec2F>, float>(&type, "AddKeys", &o2::AnimatedValue<float>::AddKeys);
	TypeInitializer::RegFuncParam<Vector<Vec2F>>(funcInfo, "values");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, const Key&>(&type, "AddKey", &o2::AnimatedValue<float>::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, const Key&, float>(&type, "AddKey", &o2::AnimatedValue<float>::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, float, float, float, float, float, float>(&type, "AddKey", &o2::AnimatedValue<float>::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, float, float, float>(&type, "AddKey", &o2::AnimatedValue<float>::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, Key, float>(&type, "GetKey", &o2::AnimatedValue<float>::GetKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, bool, float>(&type, "RemoveKey", &o2::AnimatedValue<float>::RemoveKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void>(&type, "RemoveAllKeys", &o2::AnimatedValue<float>::RemoveAllKeys);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, bool, float>(&type, "ContainsKey", &o2::AnimatedValue<float>::ContainsKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, const KeysVec&>(&type, "GetKeys", &o2::AnimatedValue<float>::GetKeys);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, const KeysVec&>(&type, "SetKeys", &o2::AnimatedValue<float>::SetKeys);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, float, float>(&type, "SmoothKey", &o2::AnimatedValue<float>::SmoothKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void>(&type, "Evaluate", &o2::AnimatedValue<float>::Evaluate);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, KeysVec>(&type, "GetKeysNonContant", &o2::AnimatedValue<float>::GetKeysNonContant);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void>(&type, "OnCurveChanged", &o2::AnimatedValue<float>::OnCurveChanged);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, const DataNode&>(&type, "OnDeserialized", &o2::AnimatedValue<float>::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, void*>(&type, "SetTargetVoid", &o2::AnimatedValue<float>::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::AnimatedValue<float>::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, void*>(&type, "SetTargetPropertyVoid", &o2::AnimatedValue<float>::SetTargetPropertyVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::AnimatedValue<float>::RegInAnimatable);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void>(&type, "InitializeProperties", &o2::AnimatedValue<float>::InitializeProperties);
}

void o2::IAnimatedValue::InitializeType(o2::IAnimatedValue* sample)
{
	TypeInitializer::RegField(&type, "onKeysChanged", (size_t)(char*)(&sample->onKeysChanged) - (size_t)(char*)sample, sample->onKeysChanged);
	auto funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::IAnimatedValue::SetTargetDelegate);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, void*>(&type, "SetTargetVoid", &o2::IAnimatedValue::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::IAnimatedValue::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, void*>(&type, "SetTargetPropertyVoid", &o2::IAnimatedValue::SetTargetPropertyVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::IAnimatedValue::RegInAnimatable);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, float, float>(&type, "ForceSetTime", &o2::IAnimatedValue::ForceSetTime);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	TypeInitializer::RegFuncParam<float>(funcInfo, "duration");
}

void o2::AnimatedValue<Vec2F>::InitializeType(o2::AnimatedValue<Vec2F>* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)sample, sample->target);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)sample, sample->targetDelegate);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)sample, sample->targetProperty);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)sample, sample->mKeys).AddAttribute<SerializableAttribute<decltype(mKeys)>>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)sample, sample->mTargetDelegate);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)sample, sample->mTargetProperty);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, Vec2F*>(&type, "SetTarget", &o2::AnimatedValue<Vec2F>::SetTarget);
	TypeInitializer::RegFuncParam<Vec2F*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, Vec2F*, const Function<void()>&>(&type, "SetTarget", &o2::AnimatedValue<Vec2F>::SetTarget);
	TypeInitializer::RegFuncParam<Vec2F*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::AnimatedValue<Vec2F>::SetTargetDelegate);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, Setter<Vec2F>*>(&type, "SetTargetProperty", &o2::AnimatedValue<Vec2F>::SetTargetProperty);
	TypeInitializer::RegFuncParam<Setter<Vec2F>*>(funcInfo, "setter");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, Vec2F>(&type, "GetValue", &o2::AnimatedValue<Vec2F>::GetValue);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, Vec2F, float>(&type, "GetValue", &o2::AnimatedValue<Vec2F>::GetValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, Vector<Key>, float>(&type, "AddKeys", &o2::AnimatedValue<Vec2F>::AddKeys);
	TypeInitializer::RegFuncParam<Vector<Key>>(funcInfo, "keys");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const Key&>(&type, "AddKey", &o2::AnimatedValue<Vec2F>::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const Key&, float>(&type, "AddKey", &o2::AnimatedValue<Vec2F>::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const Key&, float>(&type, "AddSmoothKey", &o2::AnimatedValue<Vec2F>::AddSmoothKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, float, const Vec2F&, const Vec2F&, const Vec2F&, float, float, float, float>(&type, "AddKey", &o2::AnimatedValue<Vec2F>::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "prevSupport");
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "nextSupport");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, float, const Vec2F&, float>(&type, "AddKey", &o2::AnimatedValue<Vec2F>::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, Key, float>(&type, "GetKey", &o2::AnimatedValue<Vec2F>::GetKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, bool, float>(&type, "RemoveKey", &o2::AnimatedValue<Vec2F>::RemoveKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void>(&type, "RemoveAllKeys", &o2::AnimatedValue<Vec2F>::RemoveAllKeys);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, bool, float>(&type, "ContainsKey", &o2::AnimatedValue<Vec2F>::ContainsKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, const KeysVec&>(&type, "GetKeys", &o2::AnimatedValue<Vec2F>::GetKeys);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const KeysVec&>(&type, "SetKeys", &o2::AnimatedValue<Vec2F>::SetKeys);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, float, float>(&type, "SmoothKey", &o2::AnimatedValue<Vec2F>::SmoothKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void>(&type, "Evaluate", &o2::AnimatedValue<Vec2F>::Evaluate);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, Vec2F, float>(&type, "Evaluate", &o2::AnimatedValue<Vec2F>::Evaluate);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, KeysVec>(&type, "GetKeysNonContant", &o2::AnimatedValue<Vec2F>::GetKeysNonContant);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void>(&type, "UpdateApproximation", &o2::AnimatedValue<Vec2F>::UpdateApproximation);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const DataNode&>(&type, "OnDeserialized", &o2::AnimatedValue<Vec2F>::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, void*>(&type, "SetTargetVoid", &o2::AnimatedValue<Vec2F>::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::AnimatedValue<Vec2F>::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, void*>(&type, "SetTargetPropertyVoid", &o2::AnimatedValue<Vec2F>::SetTargetPropertyVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::AnimatedValue<Vec2F>::RegInAnimatable);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void>(&type, "InitializeProperties", &o2::AnimatedValue<Vec2F>::InitializeProperties);
}

void o2::Animation::InitializeType(o2::Animation* sample)
{
	TypeInitializer::RegField(&type, "mAnimatedValues", (size_t)(char*)(&sample->mAnimatedValues) - (size_t)(char*)sample, sample->mAnimatedValues).AddAttribute<SerializableAttribute<decltype(mAnimatedValues)>>();
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget);
	TypeInitializer::RegField(&type, "mAnimationState", (size_t)(char*)(&sample->mAnimationState) - (size_t)(char*)sample, sample->mAnimationState);
	auto funcInfo = TypeInitializer::RegFunction<o2::Animation, void, IObject*, bool>(&type, "SetTarget", &o2::Animation::SetTarget);
	TypeInitializer::RegFuncParam<IObject*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "errors");
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void>(&type, "Clear", &o2::Animation::Clear);
	funcInfo = TypeInitializer::RegFunction<o2::Animation, bool, const String&>(&type, "RemoveAnimationValue", &o2::Animation::RemoveAnimationValue);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void>(&type, "Evaluate", &o2::Animation::Evaluate);
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void>(&type, "RecalculateDuration", &o2::Animation::RecalculateDuration);
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void, const DataNode&>(&type, "OnDeserialized", &o2::Animation::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void, AnimatedValueDef&>(&type, "OnAnimatedValueAdded", &o2::Animation::OnAnimatedValueAdded);
	TypeInitializer::RegFuncParam<AnimatedValueDef&>(funcInfo, "valueDef");
}

void o2::AnimationMask::InitializeType(o2::AnimationMask* sample)
{
	TypeInitializer::RegField(&type, "weights", (size_t)(char*)(&sample->weights) - (size_t)(char*)sample, sample->weights).AddAttribute<SerializableAttribute<decltype(weights)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimationMask, float, const String&>(&type, "GetNodeWeight", &o2::AnimationMask::GetNodeWeight);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "node");
}

void o2::AnimationState::InitializeType(o2::AnimationState* sample)
{
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name).AddAttribute<SerializableAttribute<decltype(name)>>();
	TypeInitializer::RegField(&type, "animation", (size_t)(char*)(&sample->animation) - (size_t)(char*)sample, sample->animation).AddAttribute<SerializableAttribute<decltype(animation)>>();
	TypeInitializer::RegField(&type, "mask", (size_t)(char*)(&sample->mask) - (size_t)(char*)sample, sample->mask).AddAttribute<SerializableAttribute<decltype(mask)>>();
	TypeInitializer::RegField(&type, "weight", (size_t)(char*)(&sample->weight) - (size_t)(char*)sample, sample->weight).AddAttribute<SerializableAttribute<decltype(weight)>>();
	TypeInitializer::RegField(&type, "workWeight", (size_t)(char*)(&sample->workWeight) - (size_t)(char*)sample, sample->workWeight);
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner);
}

void o2::IAnimation::InitializeType(o2::IAnimation* sample)
{
	TypeInitializer::RegField(&type, "playing", (size_t)(char*)(&sample->playing) - (size_t)(char*)sample, sample->playing);
	TypeInitializer::RegField(&type, "reversed", (size_t)(char*)(&sample->reversed) - (size_t)(char*)sample, sample->reversed);
	TypeInitializer::RegField(&type, "speed", (size_t)(char*)(&sample->speed) - (size_t)(char*)sample, sample->speed);
	TypeInitializer::RegField(&type, "time", (size_t)(char*)(&sample->time) - (size_t)(char*)sample, sample->time);
	TypeInitializer::RegField(&type, "relTime", (size_t)(char*)(&sample->relTime) - (size_t)(char*)sample, sample->relTime);
	TypeInitializer::RegField(&type, "beginBound", (size_t)(char*)(&sample->beginBound) - (size_t)(char*)sample, sample->beginBound);
	TypeInitializer::RegField(&type, "endBound", (size_t)(char*)(&sample->endBound) - (size_t)(char*)sample, sample->endBound);
	TypeInitializer::RegField(&type, "loop", (size_t)(char*)(&sample->loop) - (size_t)(char*)sample, sample->loop);
	TypeInitializer::RegField(&type, "duration", (size_t)(char*)(&sample->duration) - (size_t)(char*)sample, sample->duration);
	TypeInitializer::RegField(&type, "onPlayEvent", (size_t)(char*)(&sample->onPlayEvent) - (size_t)(char*)sample, sample->onPlayEvent);
	TypeInitializer::RegField(&type, "onStopEvent", (size_t)(char*)(&sample->onStopEvent) - (size_t)(char*)sample, sample->onStopEvent);
	TypeInitializer::RegField(&type, "onPlayedEvent", (size_t)(char*)(&sample->onPlayedEvent) - (size_t)(char*)sample, sample->onPlayedEvent);
	TypeInitializer::RegField(&type, "onUpdate", (size_t)(char*)(&sample->onUpdate) - (size_t)(char*)sample, sample->onUpdate);
	TypeInitializer::RegField(&type, "mTime", (size_t)(char*)(&sample->mTime) - (size_t)(char*)sample, sample->mTime);
	TypeInitializer::RegField(&type, "mInDurationTime", (size_t)(char*)(&sample->mInDurationTime) - (size_t)(char*)sample, sample->mInDurationTime);
	TypeInitializer::RegField(&type, "mDuration", (size_t)(char*)(&sample->mDuration) - (size_t)(char*)sample, sample->mDuration).AddAttribute<SerializableAttribute<decltype(mDuration)>>();
	TypeInitializer::RegField(&type, "mBeginTime", (size_t)(char*)(&sample->mBeginTime) - (size_t)(char*)sample, sample->mBeginTime);
	TypeInitializer::RegField(&type, "mEndTime", (size_t)(char*)(&sample->mEndTime) - (size_t)(char*)sample, sample->mEndTime);
	TypeInitializer::RegField(&type, "mDirection", (size_t)(char*)(&sample->mDirection) - (size_t)(char*)sample, sample->mDirection);
	TypeInitializer::RegField(&type, "mSpeed", (size_t)(char*)(&sample->mSpeed) - (size_t)(char*)sample, sample->mSpeed);
	TypeInitializer::RegField(&type, "mLoop", (size_t)(char*)(&sample->mLoop) - (size_t)(char*)sample, sample->mLoop).AddAttribute<SerializableAttribute<decltype(mLoop)>>();
	TypeInitializer::RegField(&type, "mPlaying", (size_t)(char*)(&sample->mPlaying) - (size_t)(char*)sample, sample->mPlaying);
	TypeInitializer::RegField(&type, "mTimeEvents", (size_t)(char*)(&sample->mTimeEvents) - (size_t)(char*)sample, sample->mTimeEvents);
	auto funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "Update", &o2::IAnimation::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "Play", &o2::IAnimation::Play);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float, float>(&type, "PlayInBounds", &o2::IAnimation::PlayInBounds);
	TypeInitializer::RegFuncParam<float>(funcInfo, "beginTime");
	TypeInitializer::RegFuncParam<float>(funcInfo, "endTime");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float, float>(&type, "PlayBackInBounds", &o2::IAnimation::PlayBackInBounds);
	TypeInitializer::RegFuncParam<float>(funcInfo, "beginTime");
	TypeInitializer::RegFuncParam<float>(funcInfo, "endTime");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "TogglePlay", &o2::IAnimation::TogglePlay);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "RewindAndPlay", &o2::IAnimation::RewindAndPlay);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "Stop", &o2::IAnimation::Stop);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "SetBeginBound", &o2::IAnimation::SetBeginBound);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetBeginBound", &o2::IAnimation::GetBeginBound);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "SetEndBound", &o2::IAnimation::SetEndBound);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetEndBound", &o2::IAnimation::GetEndBound);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float, float>(&type, "SetBounds", &o2::IAnimation::SetBounds);
	TypeInitializer::RegFuncParam<float>(funcInfo, "beginTime");
	TypeInitializer::RegFuncParam<float>(funcInfo, "endTime");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "ResetBounds", &o2::IAnimation::ResetBounds);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, bool>(&type, "SetPlaying", &o2::IAnimation::SetPlaying);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "playing");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, bool>(&type, "IsPlaying", &o2::IAnimation::IsPlaying);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "SetTime", &o2::IAnimation::SetTime);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetTime", &o2::IAnimation::GetTime);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetDuration", &o2::IAnimation::GetDuration);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "SetRelTime", &o2::IAnimation::SetRelTime);
	TypeInitializer::RegFuncParam<float>(funcInfo, "relTime");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetRelTime", &o2::IAnimation::GetRelTime);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "GoToBegin", &o2::IAnimation::GoToBegin);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "GoToEnd", &o2::IAnimation::GoToEnd);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "PlayForward", &o2::IAnimation::PlayForward);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "PlayBack", &o2::IAnimation::PlayBack);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, bool>(&type, "SetReverse", &o2::IAnimation::SetReverse);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "reverse");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, bool>(&type, "IsReversed", &o2::IAnimation::IsReversed);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "SetSpeed", &o2::IAnimation::SetSpeed);
	TypeInitializer::RegFuncParam<float>(funcInfo, "speed");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetSpeed", &o2::IAnimation::GetSpeed);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, Loop>(&type, "SetLoop", &o2::IAnimation::SetLoop);
	TypeInitializer::RegFuncParam<Loop>(funcInfo, "loop");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, Loop>(&type, "GetLoop", &o2::IAnimation::GetLoop);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float, const Function<void()>>(&type, "AddTimeEvent", &o2::IAnimation::AddTimeEvent);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	TypeInitializer::RegFuncParam<const Function<void()>>(funcInfo, "eventFunc");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "RemoveTimeEvent", &o2::IAnimation::RemoveTimeEvent);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, const Function<void()>>(&type, "RemoveTimeEvent", &o2::IAnimation::RemoveTimeEvent);
	TypeInitializer::RegFuncParam<const Function<void()>>(funcInfo, "eventFunc");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "RemoveAllTimeEvents", &o2::IAnimation::RemoveAllTimeEvents);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "UpdateTime", &o2::IAnimation::UpdateTime);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "Evaluate", &o2::IAnimation::Evaluate);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "InitializeProperties", &o2::IAnimation::InitializeProperties);
}

void o2::Asset::InitializeType(o2::Asset* sample)
{
	TypeInitializer::RegField(&type, "path", (size_t)(char*)(&sample->path) - (size_t)(char*)sample, sample->path);
	TypeInitializer::RegField(&type, "fullPath", (size_t)(char*)(&sample->fullPath) - (size_t)(char*)sample, sample->fullPath);
	TypeInitializer::RegField(&type, "id", (size_t)(char*)(&sample->id) - (size_t)(char*)sample, sample->id);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta);
	TypeInitializer::RegField(&type, "mPath", (size_t)(char*)(&sample->mPath) - (size_t)(char*)sample, sample->mPath);
	TypeInitializer::RegField(&type, "mMeta", (size_t)(char*)(&sample->mMeta) - (size_t)(char*)sample, sample->mMeta);
	auto funcInfo = TypeInitializer::RegFunction<o2::Asset, AssetInfo>(&type, "GetAssetInfo", &o2::Asset::GetAssetInfo);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, String>(&type, "GetPath", &o2::Asset::GetPath);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "SetPath", &o2::Asset::SetPath);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, String>(&type, "GetFullPath", &o2::Asset::GetFullPath);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, String>(&type, "GetDataFullPath", &o2::Asset::GetDataFullPath);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, AssetId>(&type, "GetAssetId", &o2::Asset::GetAssetId);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, IMetaInfo*>(&type, "GetMeta", &o2::Asset::GetMeta);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void>(&type, "Load", &o2::Asset::Load);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "Load", &o2::Asset::Load);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, AssetId>(&type, "Load", &o2::Asset::Load);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const AssetInfo&>(&type, "Load", &o2::Asset::Load);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "info");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&, bool>(&type, "Save", &o2::Asset::Save);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "rebuildAssetsImmediately");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, bool>(&type, "Save", &o2::Asset::Save);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "rebuildAssetsImmediately");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const AssetInfo&, bool>(&type, "Save", &o2::Asset::Save);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "info");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "rebuildAssetsImmediately");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, const char*>(&type, "GetFileExtensions", &o2::Asset::GetFileExtensions);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "LoadData", &o2::Asset::LoadData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "SaveData", &o2::Asset::SaveData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "LoadMeta", &o2::Asset::LoadMeta);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "SaveMeta", &o2::Asset::SaveMeta);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, String>(&type, "GetMetaFullPath", &o2::Asset::GetMetaFullPath);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, AssetId&>(&type, "IdRef", &o2::Asset::IdRef);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, LogStream*>(&type, "GetAssetsLogStream", &o2::Asset::GetAssetsLogStream);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void>(&type, "InitializeProperties", &o2::Asset::InitializeProperties);
}

void o2::AssetInfo::InitializeType(o2::AssetInfo* sample)
{
	TypeInitializer::RegField(&type, "mType", (size_t)(char*)(&sample->mType) - (size_t)(char*)sample, sample->mType).AddAttribute<SerializableAttribute<decltype(mType)>>();
	TypeInitializer::RegField(&type, "mPath", (size_t)(char*)(&sample->mPath) - (size_t)(char*)sample, sample->mPath).AddAttribute<SerializableAttribute<decltype(mPath)>>();
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId).AddAttribute<SerializableAttribute<decltype(mId)>>();
}

void o2::AtlasAsset::InitializeType(o2::AtlasAsset* sample)
{
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta);
	TypeInitializer::RegField(&type, "imagesInfos", (size_t)(char*)(&sample->imagesInfos) - (size_t)(char*)sample, sample->imagesInfos);
	TypeInitializer::RegField(&type, "images", (size_t)(char*)(&sample->images) - (size_t)(char*)sample, sample->images);
	TypeInitializer::RegField(&type, "pages", (size_t)(char*)(&sample->pages) - (size_t)(char*)sample, sample->pages);
	TypeInitializer::RegField(&type, "mImagesAssetsInfos", (size_t)(char*)(&sample->mImagesAssetsInfos) - (size_t)(char*)sample, sample->mImagesAssetsInfos).AddAttribute<SerializableAttribute<decltype(mImagesAssetsInfos)>>();
	TypeInitializer::RegField(&type, "mPages", (size_t)(char*)(&sample->mPages) - (size_t)(char*)sample, sample->mPages).AddAttribute<SerializableAttribute<decltype(mPages)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, AssetInfosVec>(&type, "GetImages", &o2::AtlasAsset::GetImages);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, Vector<ImageAsset*>>(&type, "GetImagesAssets", &o2::AtlasAsset::GetImagesAssets);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, PagesVec>(&type, "GetPages", &o2::AtlasAsset::GetPages);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, bool, ImageAsset*>(&type, "ContainsImage", &o2::AtlasAsset::ContainsImage);
	TypeInitializer::RegFuncParam<ImageAsset*>(funcInfo, "image");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, bool, const AssetInfo&>(&type, "ContainsImage", &o2::AtlasAsset::ContainsImage);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "imageAssetInfo");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, bool, AssetId>(&type, "ContainsImage", &o2::AtlasAsset::ContainsImage);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, bool, const String&>(&type, "ContainsImage", &o2::AtlasAsset::ContainsImage);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, MetaInfo*>(&type, "GetMeta", &o2::AtlasAsset::GetMeta);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, const char*>(&type, "GetFileExtensions", &o2::AtlasAsset::GetFileExtensions);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, void, const String&>(&type, "LoadData", &o2::AtlasAsset::LoadData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, void, const String&>(&type, "SaveData", &o2::AtlasAsset::SaveData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, void>(&type, "InitializeProperties", &o2::AtlasAsset::InitializeProperties);
}

void o2::BinaryAsset::InitializeType(o2::BinaryAsset* sample)
{
	TypeInitializer::RegField(&type, "data", (size_t)(char*)(&sample->data) - (size_t)(char*)sample, sample->data);
	TypeInitializer::RegField(&type, "dataSize", (size_t)(char*)(&sample->dataSize) - (size_t)(char*)sample, sample->dataSize);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta);
	TypeInitializer::RegField(&type, "mData", (size_t)(char*)(&sample->mData) - (size_t)(char*)sample, sample->mData);
	TypeInitializer::RegField(&type, "mDataSize", (size_t)(char*)(&sample->mDataSize) - (size_t)(char*)sample, sample->mDataSize);
	auto funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, char*>(&type, "GetData", &o2::BinaryAsset::GetData);
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, UInt>(&type, "GetDataSize", &o2::BinaryAsset::GetDataSize);
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, void, char*, UInt>(&type, "SetData", &o2::BinaryAsset::SetData);
	TypeInitializer::RegFuncParam<char*>(funcInfo, "data");
	TypeInitializer::RegFuncParam<UInt>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, MetaInfo*>(&type, "GetMeta", &o2::BinaryAsset::GetMeta);
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, const char*>(&type, "GetFileExtensions", &o2::BinaryAsset::GetFileExtensions);
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, void, const String&>(&type, "LoadData", &o2::BinaryAsset::LoadData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, void, const String&>(&type, "SaveData", &o2::BinaryAsset::SaveData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, void>(&type, "InitializeProperties", &o2::BinaryAsset::InitializeProperties);
}

void o2::BitmapFontAsset::InitializeType(o2::BitmapFontAsset* sample)
{
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta);
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)sample, sample->font);
	TypeInitializer::RegField(&type, "mFont", (size_t)(char*)(&sample->mFont) - (size_t)(char*)sample, sample->mFont);
	auto funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, MetaInfo*>(&type, "GetMeta", &o2::BitmapFontAsset::GetMeta);
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, FontRef>(&type, "GetFont", &o2::BitmapFontAsset::GetFont);
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, const char*>(&type, "GetFileExtensions", &o2::BitmapFontAsset::GetFileExtensions);
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, void, const String&>(&type, "LoadData", &o2::BitmapFontAsset::LoadData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, void>(&type, "InitializeProperties", &o2::BitmapFontAsset::InitializeProperties);
}

void o2::FolderAsset::InitializeType(o2::FolderAsset* sample)
{
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta);
	TypeInitializer::RegField(&type, "insideAssets", (size_t)(char*)(&sample->insideAssets) - (size_t)(char*)sample, sample->insideAssets);
	TypeInitializer::RegField(&type, "mContainingAssetsInfos", (size_t)(char*)(&sample->mContainingAssetsInfos) - (size_t)(char*)sample, sample->mContainingAssetsInfos);
	auto funcInfo = TypeInitializer::RegFunction<o2::FolderAsset, AssetInfosVec>(&type, "GetContainingAssetsInfos", &o2::FolderAsset::GetContainingAssetsInfos);
	funcInfo = TypeInitializer::RegFunction<o2::FolderAsset, MetaInfo*>(&type, "GetMeta", &o2::FolderAsset::GetMeta);
	funcInfo = TypeInitializer::RegFunction<o2::FolderAsset, void, const String&>(&type, "LoadData", &o2::FolderAsset::LoadData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::FolderAsset, void, const String&>(&type, "SaveData", &o2::FolderAsset::SaveData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::FolderAsset, void>(&type, "InitializeProperties", &o2::FolderAsset::InitializeProperties);
}

void o2::ImageAsset::InitializeType(o2::ImageAsset* sample)
{
	TypeInitializer::RegField(&type, "bitmap", (size_t)(char*)(&sample->bitmap) - (size_t)(char*)sample, sample->bitmap);
	TypeInitializer::RegField(&type, "atlasId", (size_t)(char*)(&sample->atlasId) - (size_t)(char*)sample, sample->atlasId);
	TypeInitializer::RegField(&type, "atlas", (size_t)(char*)(&sample->atlas) - (size_t)(char*)sample, sample->atlas);
	TypeInitializer::RegField(&type, "atlasPage", (size_t)(char*)(&sample->atlasPage) - (size_t)(char*)sample, sample->atlasPage);
	TypeInitializer::RegField(&type, "atlasRect", (size_t)(char*)(&sample->atlasRect) - (size_t)(char*)sample, sample->atlasRect);
	TypeInitializer::RegField(&type, "size", (size_t)(char*)(&sample->size) - (size_t)(char*)sample, sample->size);
	TypeInitializer::RegField(&type, "width", (size_t)(char*)(&sample->width) - (size_t)(char*)sample, sample->width);
	TypeInitializer::RegField(&type, "height", (size_t)(char*)(&sample->height) - (size_t)(char*)sample, sample->height);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta);
	TypeInitializer::RegField(&type, "mBitmap", (size_t)(char*)(&sample->mBitmap) - (size_t)(char*)sample, sample->mBitmap);
	TypeInitializer::RegField(&type, "mAtlasPage", (size_t)(char*)(&sample->mAtlasPage) - (size_t)(char*)sample, sample->mAtlasPage).AddAttribute<SerializableAttribute<decltype(mAtlasPage)>>();
	TypeInitializer::RegField(&type, "mAtlasRect", (size_t)(char*)(&sample->mAtlasRect) - (size_t)(char*)sample, sample->mAtlasRect).AddAttribute<SerializableAttribute<decltype(mAtlasRect)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, Bitmap*>(&type, "GetBitmap", &o2::ImageAsset::GetBitmap);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void, Bitmap*>(&type, "SetBitmap", &o2::ImageAsset::SetBitmap);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, AssetId>(&type, "GetAtlasId", &o2::ImageAsset::GetAtlasId);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void, AssetId>(&type, "SetAtlasId", &o2::ImageAsset::SetAtlasId);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, AtlasAsset*>(&type, "GetAtlas", &o2::ImageAsset::GetAtlas);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void, AtlasAsset*>(&type, "SetAtlas", &o2::ImageAsset::SetAtlas);
	TypeInitializer::RegFuncParam<AtlasAsset*>(funcInfo, "atlas");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, UInt>(&type, "GetAtlasPage", &o2::ImageAsset::GetAtlasPage);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, RectI>(&type, "GetAtlasRect", &o2::ImageAsset::GetAtlasRect);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, Vec2F>(&type, "GetSize", &o2::ImageAsset::GetSize);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, float>(&type, "GetWidth", &o2::ImageAsset::GetWidth);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, float>(&type, "GetHeight", &o2::ImageAsset::GetHeight);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, TextureRef>(&type, "GetAtlasTextureRef", &o2::ImageAsset::GetAtlasTextureRef);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, MetaInfo*>(&type, "GetMeta", &o2::ImageAsset::GetMeta);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, const char*>(&type, "GetFileExtensions", &o2::ImageAsset::GetFileExtensions);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void, const String&>(&type, "LoadData", &o2::ImageAsset::LoadData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void, const String&>(&type, "SaveData", &o2::ImageAsset::SaveData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void>(&type, "LoadBitmap", &o2::ImageAsset::LoadBitmap);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void>(&type, "InitializeProperties", &o2::ImageAsset::InitializeProperties);
}

void o2::VectorFontAsset::InitializeType(o2::VectorFontAsset* sample)
{
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta);
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)sample, sample->font);
	TypeInitializer::RegField(&type, "mFont", (size_t)(char*)(&sample->mFont) - (size_t)(char*)sample, sample->mFont);
	auto funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, MetaInfo*>(&type, "GetMeta", &o2::VectorFontAsset::GetMeta);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, FontRef>(&type, "GetFont", &o2::VectorFontAsset::GetFont);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, const EffectsVec&>(&type, "GetEffects", &o2::VectorFontAsset::GetEffects);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, void, VectorFont::Effect*>(&type, "AddEffect", &o2::VectorFontAsset::AddEffect);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "effect");
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, void, VectorFont::Effect*>(&type, "RemoveEffect", &o2::VectorFontAsset::RemoveEffect);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "effect");
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, void>(&type, "RemoveAllEffects", &o2::VectorFontAsset::RemoveAllEffects);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, const char*>(&type, "GetFileExtensions", &o2::VectorFontAsset::GetFileExtensions);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, void, const String&>(&type, "LoadData", &o2::VectorFontAsset::LoadData);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, void>(&type, "InitializeProperties", &o2::VectorFontAsset::InitializeProperties);
}

void o2::AtlasAssetConverter::InitializeType(o2::AtlasAssetConverter* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, Vector<Type::Id>>(&type, "GetProcessingAssetsTypes", &o2::AtlasAssetConverter::GetProcessingAssetsTypes);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void, const AssetTree::AssetNode&>(&type, "ConvertAsset", &o2::AtlasAssetConverter::ConvertAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void, const AssetTree::AssetNode&>(&type, "RemoveAsset", &o2::AtlasAssetConverter::RemoveAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void, const AssetTree::AssetNode&, const AssetTree::AssetNode&>(&type, "MoveAsset", &o2::AtlasAssetConverter::MoveAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeFrom");
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeTo");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, Vector<AssetId>>(&type, "AssetsPostProcess", &o2::AtlasAssetConverter::AssetsPostProcess);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void>(&type, "Reset", &o2::AtlasAssetConverter::Reset);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void>(&type, "CheckBasicAtlas", &o2::AtlasAssetConverter::CheckBasicAtlas);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, Vector<AssetId>>(&type, "CheckRebuildingAtlases", &o2::AtlasAssetConverter::CheckRebuildingAtlases);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, bool, AssetTree::AssetNode*>(&type, "CheckAtlasRebuilding", &o2::AtlasAssetConverter::CheckAtlasRebuilding);
	TypeInitializer::RegFuncParam<AssetTree::AssetNode*>(funcInfo, "atlasInfo");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, bool, ImagesVec&, ImagesVec&>(&type, "IsAtlasNeedRebuild", &o2::AtlasAssetConverter::IsAtlasNeedRebuild);
	TypeInitializer::RegFuncParam<ImagesVec&>(funcInfo, "currentImages");
	TypeInitializer::RegFuncParam<ImagesVec&>(funcInfo, "lastImages");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void, AssetTree::AssetNode*, ImagesVec&>(&type, "RebuildAtlas", &o2::AtlasAssetConverter::RebuildAtlas);
	TypeInitializer::RegFuncParam<AssetTree::AssetNode*>(funcInfo, "atlasInfo");
	TypeInitializer::RegFuncParam<ImagesVec&>(funcInfo, "images");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void, ImagePackDef&>(&type, "SaveImageAsset", &o2::AtlasAssetConverter::SaveImageAsset);
	TypeInitializer::RegFuncParam<ImagePackDef&>(funcInfo, "imgDef");
}

void o2::FolderAssetConverter::InitializeType(o2::FolderAssetConverter* sample)
{
	TypeInitializer::RegField(&type, "mRemovedFolders", (size_t)(char*)(&sample->mRemovedFolders) - (size_t)(char*)sample, sample->mRemovedFolders);
	auto funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, Vector<Type::Id>>(&type, "GetProcessingAssetsTypes", &o2::FolderAssetConverter::GetProcessingAssetsTypes);
	funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, void, const AssetTree::AssetNode&>(&type, "ConvertAsset", &o2::FolderAssetConverter::ConvertAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, void, const AssetTree::AssetNode&>(&type, "RemoveAsset", &o2::FolderAssetConverter::RemoveAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, void, const AssetTree::AssetNode&, const AssetTree::AssetNode&>(&type, "MoveAsset", &o2::FolderAssetConverter::MoveAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeFrom");
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeTo");
	funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, Vector<AssetId>>(&type, "AssetsPostProcess", &o2::FolderAssetConverter::AssetsPostProcess);
	funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, void>(&type, "Reset", &o2::FolderAssetConverter::Reset);
}

void o2::IAssetConverter::InitializeType(o2::IAssetConverter* sample)
{
	TypeInitializer::RegField(&type, "mAssetsBuilder", (size_t)(char*)(&sample->mAssetsBuilder) - (size_t)(char*)sample, sample->mAssetsBuilder);
	auto funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, Vector<Type::Id>>(&type, "GetProcessingAssetsTypes", &o2::IAssetConverter::GetProcessingAssetsTypes);
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, void, const AssetTree::AssetNode&>(&type, "ConvertAsset", &o2::IAssetConverter::ConvertAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, void, const AssetTree::AssetNode&>(&type, "RemoveAsset", &o2::IAssetConverter::RemoveAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, void, const AssetTree::AssetNode&, const AssetTree::AssetNode&>(&type, "MoveAsset", &o2::IAssetConverter::MoveAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeFrom");
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeTo");
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, Vector<AssetId>>(&type, "AssetsPostProcess", &o2::IAssetConverter::AssetsPostProcess);
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, void>(&type, "Reset", &o2::IAssetConverter::Reset);
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, void, AssetsBuilder*>(&type, "SetAssetsBuilder", &o2::IAssetConverter::SetAssetsBuilder);
	TypeInitializer::RegFuncParam<AssetsBuilder*>(funcInfo, "builder");
}

void o2::ImageAssetConverter::InitializeType(o2::ImageAssetConverter* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::ImageAssetConverter, Vector<Type::Id>>(&type, "GetProcessingAssetsTypes", &o2::ImageAssetConverter::GetProcessingAssetsTypes);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAssetConverter, void, const AssetTree::AssetNode&>(&type, "ConvertAsset", &o2::ImageAssetConverter::ConvertAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAssetConverter, void, const AssetTree::AssetNode&>(&type, "RemoveAsset", &o2::ImageAssetConverter::RemoveAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAssetConverter, void, const AssetTree::AssetNode&, const AssetTree::AssetNode&>(&type, "MoveAsset", &o2::ImageAssetConverter::MoveAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeFrom");
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeTo");
}

void o2::StdAssetConverter::InitializeType(o2::StdAssetConverter* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::StdAssetConverter, Vector<Type::Id>>(&type, "GetProcessingAssetsTypes", &o2::StdAssetConverter::GetProcessingAssetsTypes);
	funcInfo = TypeInitializer::RegFunction<o2::StdAssetConverter, void, const AssetTree::AssetNode&>(&type, "ConvertAsset", &o2::StdAssetConverter::ConvertAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::StdAssetConverter, void, const AssetTree::AssetNode&>(&type, "RemoveAsset", &o2::StdAssetConverter::RemoveAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::StdAssetConverter, void, const AssetTree::AssetNode&, const AssetTree::AssetNode&>(&type, "MoveAsset", &o2::StdAssetConverter::MoveAsset);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeFrom");
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeTo");
}

void o2::ProjectConfig::InitializeType(o2::ProjectConfig* sample)
{
	TypeInitializer::RegField(&type, "projectName", (size_t)(char*)(&sample->projectName) - (size_t)(char*)sample, sample->projectName);
	TypeInitializer::RegField(&type, "currentPlatform", (size_t)(char*)(&sample->currentPlatform) - (size_t)(char*)sample, sample->currentPlatform);
	TypeInitializer::RegField(&type, "projectPath", (size_t)(char*)(&sample->projectPath) - (size_t)(char*)sample, sample->projectPath);
	TypeInitializer::RegField(&type, "mProjectName", (size_t)(char*)(&sample->mProjectName) - (size_t)(char*)sample, sample->mProjectName).AddAttribute<SerializableAttribute<decltype(mProjectName)>>();
	TypeInitializer::RegField(&type, "mPlatform", (size_t)(char*)(&sample->mPlatform) - (size_t)(char*)sample, sample->mPlatform);
	TypeInitializer::RegField(&type, "mProjectPath", (size_t)(char*)(&sample->mProjectPath) - (size_t)(char*)sample, sample->mProjectPath);
	auto funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, String>(&type, "GetProjectName", &o2::ProjectConfig::GetProjectName);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, const String&>(&type, "SetProjectName", &o2::ProjectConfig::SetProjectName);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, Platform>(&type, "GetPlatform", &o2::ProjectConfig::GetPlatform);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, Platform>(&type, "SetPlatform", &o2::ProjectConfig::SetPlatform);
	TypeInitializer::RegFuncParam<Platform>(funcInfo, "platform");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, String>(&type, "GetProjectPath", &o2::ProjectConfig::GetProjectPath);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, const String&>(&type, "SetProjectPath", &o2::ProjectConfig::SetProjectPath);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, const String&>(&type, "InitializeDefault", &o2::ProjectConfig::InitializeDefault);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "configFilePath");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void>(&type, "InitializeProperties", &o2::ProjectConfig::InitializeProperties);
}

void o2::Camera::InitializeType(o2::Camera* sample)
{
}

void o2::IRectDrawable::InitializeType(o2::IRectDrawable* sample)
{
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)sample, sample->color);
	TypeInitializer::RegField(&type, "transparency", (size_t)(char*)(&sample->transparency) - (size_t)(char*)sample, sample->transparency);
	TypeInitializer::RegField(&type, "enabled", (size_t)(char*)(&sample->enabled) - (size_t)(char*)sample, sample->enabled);
	TypeInitializer::RegField(&type, "mColor", (size_t)(char*)(&sample->mColor) - (size_t)(char*)sample, sample->mColor).AddAttribute<SerializableAttribute<decltype(mColor)>>();
	TypeInitializer::RegField(&type, "mEnabled", (size_t)(char*)(&sample->mEnabled) - (size_t)(char*)sample, sample->mEnabled).AddAttribute<SerializableAttribute<decltype(mEnabled)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void>(&type, "Draw", &o2::IRectDrawable::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void, const Color4&>(&type, "SetColor", &o2::IRectDrawable::SetColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, Color4>(&type, "GetColor", &o2::IRectDrawable::GetColor);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void, float>(&type, "SetTransparency", &o2::IRectDrawable::SetTransparency);
	TypeInitializer::RegFuncParam<float>(funcInfo, "transparency");
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, float>(&type, "GetTransparency", &o2::IRectDrawable::GetTransparency);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void, bool>(&type, "SetEnabled", &o2::IRectDrawable::SetEnabled);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "enabled");
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, bool>(&type, "IsEnabled", &o2::IRectDrawable::IsEnabled);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::IRectDrawable::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void>(&type, "ColorChanged", &o2::IRectDrawable::ColorChanged);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void>(&type, "EnableChanged", &o2::IRectDrawable::EnableChanged);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void>(&type, "InitializeProperties", &o2::IRectDrawable::InitializeProperties);
}

void o2::Sprite::InitializeType(o2::Sprite* sample)
{
	TypeInitializer::RegField(&type, "texture", (size_t)(char*)(&sample->texture) - (size_t)(char*)sample, sample->texture);
	TypeInitializer::RegField(&type, "textureSrcRect", (size_t)(char*)(&sample->textureSrcRect) - (size_t)(char*)sample, sample->textureSrcRect);
	TypeInitializer::RegField(&type, "imageAssetId", (size_t)(char*)(&sample->imageAssetId) - (size_t)(char*)sample, sample->imageAssetId);
	TypeInitializer::RegField(&type, "imageAssetPath", (size_t)(char*)(&sample->imageAssetPath) - (size_t)(char*)sample, sample->imageAssetPath);
	TypeInitializer::RegField(&type, "imageAsset", (size_t)(char*)(&sample->imageAsset) - (size_t)(char*)sample, sample->imageAsset);
	TypeInitializer::RegField(&type, "bitmap", (size_t)(char*)(&sample->bitmap) - (size_t)(char*)sample, sample->bitmap);
	TypeInitializer::RegField(&type, "leftTopColor", (size_t)(char*)(&sample->leftTopColor) - (size_t)(char*)sample, sample->leftTopColor);
	TypeInitializer::RegField(&type, "rightTopColor", (size_t)(char*)(&sample->rightTopColor) - (size_t)(char*)sample, sample->rightTopColor);
	TypeInitializer::RegField(&type, "leftBottomColor", (size_t)(char*)(&sample->leftBottomColor) - (size_t)(char*)sample, sample->leftBottomColor);
	TypeInitializer::RegField(&type, "rightBottomColor", (size_t)(char*)(&sample->rightBottomColor) - (size_t)(char*)sample, sample->rightBottomColor);
	TypeInitializer::RegField(&type, "mode", (size_t)(char*)(&sample->mode) - (size_t)(char*)sample, sample->mode);
	TypeInitializer::RegField(&type, "fill", (size_t)(char*)(&sample->fill) - (size_t)(char*)sample, sample->fill);
	TypeInitializer::RegField(&type, "sliceBorder", (size_t)(char*)(&sample->sliceBorder) - (size_t)(char*)sample, sample->sliceBorder);
	TypeInitializer::RegField(&type, "mTextureSrcRect", (size_t)(char*)(&sample->mTextureSrcRect) - (size_t)(char*)sample, sample->mTextureSrcRect);
	TypeInitializer::RegField(&type, "mCornersColors", (size_t)(char*)(&sample->mCornersColors) - (size_t)(char*)sample, sample->mCornersColors);
	TypeInitializer::RegField(&type, "mImageAssetId", (size_t)(char*)(&sample->mImageAssetId) - (size_t)(char*)sample, sample->mImageAssetId);
	TypeInitializer::RegField(&type, "mAtlasAssetId", (size_t)(char*)(&sample->mAtlasAssetId) - (size_t)(char*)sample, sample->mAtlasAssetId);
	TypeInitializer::RegField(&type, "mMode", (size_t)(char*)(&sample->mMode) - (size_t)(char*)sample, sample->mMode).AddAttribute<SerializableAttribute<decltype(mMode)>>();
	TypeInitializer::RegField(&type, "mSlices", (size_t)(char*)(&sample->mSlices) - (size_t)(char*)sample, sample->mSlices).AddAttribute<SerializableAttribute<decltype(mSlices)>>();
	TypeInitializer::RegField(&type, "mFill", (size_t)(char*)(&sample->mFill) - (size_t)(char*)sample, sample->mFill).AddAttribute<SerializableAttribute<decltype(mFill)>>();
	TypeInitializer::RegField(&type, "mMesh", (size_t)(char*)(&sample->mMesh) - (size_t)(char*)sample, sample->mMesh);
	TypeInitializer::RegField(&type, "mMeshBuildFunc", (size_t)(char*)(&sample->mMeshBuildFunc) - (size_t)(char*)sample, sample->mMeshBuildFunc);
	auto funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "Draw", &o2::Sprite::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, TextureRef>(&type, "SetTexture", &o2::Sprite::SetTexture);
	TypeInitializer::RegFuncParam<TextureRef>(funcInfo, "texture");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, TextureRef>(&type, "GetTexture", &o2::Sprite::GetTexture);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const RectI&>(&type, "SetTextureSrcRect", &o2::Sprite::SetTextureSrcRect);
	TypeInitializer::RegFuncParam<const RectI&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, RectI>(&type, "GetTextureSrcRect", &o2::Sprite::GetTextureSrcRect);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, Corner, const Color4&>(&type, "SetCornerColor", &o2::Sprite::SetCornerColor);
	TypeInitializer::RegFuncParam<Corner>(funcInfo, "corner");
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, Color4, Corner>(&type, "GetCornerColor", &o2::Sprite::GetCornerColor);
	TypeInitializer::RegFuncParam<Corner>(funcInfo, "corner");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const Color4&>(&type, "SetLeftTopColor", &o2::Sprite::SetLeftTopColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, Color4>(&type, "GetLeftTopCorner", &o2::Sprite::GetLeftTopCorner);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const Color4&>(&type, "SetRightTopColor", &o2::Sprite::SetRightTopColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, Color4>(&type, "GetRightTopCorner", &o2::Sprite::GetRightTopCorner);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const Color4&>(&type, "SetRightBottomColor", &o2::Sprite::SetRightBottomColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, Color4>(&type, "GetRightBottomCorner", &o2::Sprite::GetRightBottomCorner);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const Color4&>(&type, "SetLeftBottomColor", &o2::Sprite::SetLeftBottomColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, Color4>(&type, "GetLeftBottomCorner", &o2::Sprite::GetLeftBottomCorner);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, float>(&type, "SetFill", &o2::Sprite::SetFill);
	TypeInitializer::RegFuncParam<float>(funcInfo, "fill");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, float>(&type, "GetFill", &o2::Sprite::GetFill);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, SpriteMode>(&type, "SetMode", &o2::Sprite::SetMode);
	TypeInitializer::RegFuncParam<SpriteMode>(funcInfo, "mode");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, SpriteMode>(&type, "GetMode", &o2::Sprite::GetMode);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const RectI&>(&type, "SetSliceBorder", &o2::Sprite::SetSliceBorder);
	TypeInitializer::RegFuncParam<const RectI&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, RectI>(&type, "GetSliceBorder", &o2::Sprite::GetSliceBorder);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, ImageAsset*>(&type, "LoadFromImage", &o2::Sprite::LoadFromImage);
	TypeInitializer::RegFuncParam<ImageAsset*>(funcInfo, "image");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const String&>(&type, "LoadFromImage", &o2::Sprite::LoadFromImage);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "imagePath");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, AssetId>(&type, "LoadFromImage", &o2::Sprite::LoadFromImage);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "imageId");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const Color4&>(&type, "LoadMonoColor", &o2::Sprite::LoadMonoColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, Bitmap*>(&type, "LoadFromBitmap", &o2::Sprite::LoadFromBitmap);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, AssetId>(&type, "GetImageId", &o2::Sprite::GetImageId);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, AssetId>(&type, "GetAtlasAssetId", &o2::Sprite::GetAtlasAssetId);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "NormalizeSize", &o2::Sprite::NormalizeSize);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "NormalizeAspectByWidth", &o2::Sprite::NormalizeAspectByWidth);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "NormalizeAspectByHeight", &o2::Sprite::NormalizeAspectByHeight);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "NormalizeAspect", &o2::Sprite::NormalizeAspect);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BasisChanged", &o2::Sprite::BasisChanged);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "ColorChanged", &o2::Sprite::ColorChanged);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "UpdateMesh", &o2::Sprite::UpdateMesh);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildDefaultMesh", &o2::Sprite::BuildDefaultMesh);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildSlicedMesh", &o2::Sprite::BuildSlicedMesh);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFillLeftToRightMesh", &o2::Sprite::BuildFillLeftToRightMesh);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFillRightToLeftMesh", &o2::Sprite::BuildFillRightToLeftMesh);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFillUpToDownMesh", &o2::Sprite::BuildFillUpToDownMesh);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFillDownToUpMesh", &o2::Sprite::BuildFillDownToUpMesh);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFill360CWMesh", &o2::Sprite::BuildFill360CWMesh);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFill360CCWMesh", &o2::Sprite::BuildFill360CCWMesh);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, DataNode&>(&type, "OnSerialize", &o2::Sprite::OnSerialize);
	TypeInitializer::RegFuncParam<DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const DataNode&>(&type, "OnDeserialized", &o2::Sprite::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "ReloadImage", &o2::Sprite::ReloadImage);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "InitializeProperties", &o2::Sprite::InitializeProperties);
}

void o2::Text::InitializeType(o2::Text* sample)
{
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)sample, sample->font);
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)sample, sample->text);
	TypeInitializer::RegField(&type, "height", (size_t)(char*)(&sample->height) - (size_t)(char*)sample, sample->height);
	TypeInitializer::RegField(&type, "verAlign", (size_t)(char*)(&sample->verAlign) - (size_t)(char*)sample, sample->verAlign);
	TypeInitializer::RegField(&type, "horAlign", (size_t)(char*)(&sample->horAlign) - (size_t)(char*)sample, sample->horAlign);
	TypeInitializer::RegField(&type, "wordWrap", (size_t)(char*)(&sample->wordWrap) - (size_t)(char*)sample, sample->wordWrap);
	TypeInitializer::RegField(&type, "dotsEngings", (size_t)(char*)(&sample->dotsEngings) - (size_t)(char*)sample, sample->dotsEngings);
	TypeInitializer::RegField(&type, "symbolsDistanceCoef", (size_t)(char*)(&sample->symbolsDistanceCoef) - (size_t)(char*)sample, sample->symbolsDistanceCoef);
	TypeInitializer::RegField(&type, "linesDistanceCoef", (size_t)(char*)(&sample->linesDistanceCoef) - (size_t)(char*)sample, sample->linesDistanceCoef);
	TypeInitializer::RegField(&type, "mMeshMaxPolyCount", (size_t)(char*)(&sample->mMeshMaxPolyCount) - (size_t)(char*)sample, sample->mMeshMaxPolyCount);
	TypeInitializer::RegField(&type, "mText", (size_t)(char*)(&sample->mText) - (size_t)(char*)sample, sample->mText).AddAttribute<SerializableAttribute<decltype(mText)>>();
	TypeInitializer::RegField(&type, "mFontAssetId", (size_t)(char*)(&sample->mFontAssetId) - (size_t)(char*)sample, sample->mFontAssetId).AddAttribute<SerializableAttribute<decltype(mFontAssetId)>>();
	TypeInitializer::RegField(&type, "mFont", (size_t)(char*)(&sample->mFont) - (size_t)(char*)sample, sample->mFont);
	TypeInitializer::RegField(&type, "mHeight", (size_t)(char*)(&sample->mHeight) - (size_t)(char*)sample, sample->mHeight);
	TypeInitializer::RegField(&type, "mSymbolsDistCoef", (size_t)(char*)(&sample->mSymbolsDistCoef) - (size_t)(char*)sample, sample->mSymbolsDistCoef).AddAttribute<SerializableAttribute<decltype(mSymbolsDistCoef)>>();
	TypeInitializer::RegField(&type, "mLinesDistanceCoef", (size_t)(char*)(&sample->mLinesDistanceCoef) - (size_t)(char*)sample, sample->mLinesDistanceCoef).AddAttribute<SerializableAttribute<decltype(mLinesDistanceCoef)>>();
	TypeInitializer::RegField(&type, "mVerAlign", (size_t)(char*)(&sample->mVerAlign) - (size_t)(char*)sample, sample->mVerAlign).AddAttribute<SerializableAttribute<decltype(mVerAlign)>>();
	TypeInitializer::RegField(&type, "mHorAlign", (size_t)(char*)(&sample->mHorAlign) - (size_t)(char*)sample, sample->mHorAlign).AddAttribute<SerializableAttribute<decltype(mHorAlign)>>();
	TypeInitializer::RegField(&type, "mWordWrap", (size_t)(char*)(&sample->mWordWrap) - (size_t)(char*)sample, sample->mWordWrap).AddAttribute<SerializableAttribute<decltype(mWordWrap)>>();
	TypeInitializer::RegField(&type, "mDotsEndings", (size_t)(char*)(&sample->mDotsEndings) - (size_t)(char*)sample, sample->mDotsEndings).AddAttribute<SerializableAttribute<decltype(mDotsEndings)>>();
	TypeInitializer::RegField(&type, "mMeshes", (size_t)(char*)(&sample->mMeshes) - (size_t)(char*)sample, sample->mMeshes);
	TypeInitializer::RegField(&type, "mLastTransform", (size_t)(char*)(&sample->mLastTransform) - (size_t)(char*)sample, sample->mLastTransform);
	TypeInitializer::RegField(&type, "mSymbolsSet", (size_t)(char*)(&sample->mSymbolsSet) - (size_t)(char*)sample, sample->mSymbolsSet);
	TypeInitializer::RegField(&type, "mUpdatingMesh", (size_t)(char*)(&sample->mUpdatingMesh) - (size_t)(char*)sample, sample->mUpdatingMesh);
	auto funcInfo = TypeInitializer::RegFunction<o2::Text, void>(&type, "Draw", &o2::Text::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, FontRef>(&type, "SetFont", &o2::Text::SetFont);
	TypeInitializer::RegFuncParam<FontRef>(funcInfo, "font");
	funcInfo = TypeInitializer::RegFunction<o2::Text, FontRef>(&type, "GetFont", &o2::Text::GetFont);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, BitmapFontAsset*>(&type, "SetFontAsset", &o2::Text::SetFontAsset);
	TypeInitializer::RegFuncParam<BitmapFontAsset*>(funcInfo, "asset");
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, VectorFontAsset*>(&type, "SetFontAsset", &o2::Text::SetFontAsset);
	TypeInitializer::RegFuncParam<VectorFontAsset*>(funcInfo, "asset");
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, AssetId>(&type, "SetFontAsset", &o2::Text::SetFontAsset);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "assetId");
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, const String&>(&type, "SetFontAsset", &o2::Text::SetFontAsset);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "fileName");
	funcInfo = TypeInitializer::RegFunction<o2::Text, Asset*>(&type, "GetFontAsset", &o2::Text::GetFontAsset);
	funcInfo = TypeInitializer::RegFunction<o2::Text, AssetId>(&type, "GetFontAssetId", &o2::Text::GetFontAssetId);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, int>(&type, "SetHeight", &o2::Text::SetHeight);
	TypeInitializer::RegFuncParam<int>(funcInfo, "height");
	funcInfo = TypeInitializer::RegFunction<o2::Text, int>(&type, "GetHeight", &o2::Text::GetHeight);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, const WString&>(&type, "SetText", &o2::Text::SetText);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::Text, WString>(&type, "GetText", &o2::Text::GetText);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, HorAlign>(&type, "SetHorAlign", &o2::Text::SetHorAlign);
	TypeInitializer::RegFuncParam<HorAlign>(funcInfo, "align");
	funcInfo = TypeInitializer::RegFunction<o2::Text, HorAlign>(&type, "GetHorAlign", &o2::Text::GetHorAlign);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, VerAlign>(&type, "SetVerAlign", &o2::Text::SetVerAlign);
	TypeInitializer::RegFuncParam<VerAlign>(funcInfo, "align");
	funcInfo = TypeInitializer::RegFunction<o2::Text, VerAlign>(&type, "GetVerAlign", &o2::Text::GetVerAlign);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, bool>(&type, "SetWordWrap", &o2::Text::SetWordWrap);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "flag");
	funcInfo = TypeInitializer::RegFunction<o2::Text, bool>(&type, "GetWordWrap", &o2::Text::GetWordWrap);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, bool>(&type, "SetDotsEngings", &o2::Text::SetDotsEngings);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "flag");
	funcInfo = TypeInitializer::RegFunction<o2::Text, bool>(&type, "IsDotsEngings", &o2::Text::IsDotsEngings);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, float>(&type, "SetSymbolsDistanceCoef", &o2::Text::SetSymbolsDistanceCoef);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::Text, float>(&type, "GetSymbolsDistanceCoef", &o2::Text::GetSymbolsDistanceCoef);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, float>(&type, "SetLinesDistanceCoef", &o2::Text::SetLinesDistanceCoef);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::Text, float>(&type, "GetLinesDistanceCoef", &o2::Text::GetLinesDistanceCoef);
	funcInfo = TypeInitializer::RegFunction<o2::Text, SymbolsSet&>(&type, "GetSymbolsSet", &o2::Text::GetSymbolsSet);
	funcInfo = TypeInitializer::RegFunction<o2::Text, Vec2F>(&type, "GetRealSize", &o2::Text::GetRealSize);
	funcInfo = TypeInitializer::RegFunction<o2::Text, RectF>(&type, "GetRealRect", &o2::Text::GetRealRect);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void>(&type, "UpdateMesh", &o2::Text::UpdateMesh);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, const Basis&>(&type, "TransformMesh", &o2::Text::TransformMesh);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "bas");
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, int>(&type, "PrepareMesh", &o2::Text::PrepareMesh);
	TypeInitializer::RegFuncParam<int>(funcInfo, "charactersCount");
	funcInfo = TypeInitializer::RegFunction<o2::Text, Basis>(&type, "CalculateTextBasis", &o2::Text::CalculateTextBasis);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void>(&type, "ColorChanged", &o2::Text::ColorChanged);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void>(&type, "BasisChanged", &o2::Text::BasisChanged);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, const DataNode&>(&type, "OnDeserialized", &o2::Text::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Text, void>(&type, "InitializeProperties", &o2::Text::InitializeProperties);
}

void o2::FontStrokeEffect::InitializeType(o2::FontStrokeEffect* sample)
{
	TypeInitializer::RegField(&type, "radius", (size_t)(char*)(&sample->radius) - (size_t)(char*)sample, sample->radius).AddAttribute<SerializableAttribute<decltype(radius)>>();
	TypeInitializer::RegField(&type, "alphaThreshold", (size_t)(char*)(&sample->alphaThreshold) - (size_t)(char*)sample, sample->alphaThreshold).AddAttribute<SerializableAttribute<decltype(alphaThreshold)>>();
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)sample, sample->color).AddAttribute<SerializableAttribute<decltype(color)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontStrokeEffect, void, Bitmap*>(&type, "Process", &o2::FontStrokeEffect::Process);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontStrokeEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontStrokeEffect::GetSizeExtend);
	funcInfo = TypeInitializer::RegFunction<o2::FontStrokeEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontStrokeEffect::IsEqual);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::FontGradientEffect::InitializeType(o2::FontGradientEffect* sample)
{
	TypeInitializer::RegField(&type, "color1", (size_t)(char*)(&sample->color1) - (size_t)(char*)sample, sample->color1).AddAttribute<SerializableAttribute<decltype(color1)>>();
	TypeInitializer::RegField(&type, "color2", (size_t)(char*)(&sample->color2) - (size_t)(char*)sample, sample->color2).AddAttribute<SerializableAttribute<decltype(color2)>>();
	TypeInitializer::RegField(&type, "angle", (size_t)(char*)(&sample->angle) - (size_t)(char*)sample, sample->angle).AddAttribute<SerializableAttribute<decltype(angle)>>();
	TypeInitializer::RegField(&type, "length", (size_t)(char*)(&sample->length) - (size_t)(char*)sample, sample->length).AddAttribute<SerializableAttribute<decltype(length)>>();
	TypeInitializer::RegField(&type, "origin", (size_t)(char*)(&sample->origin) - (size_t)(char*)sample, sample->origin).AddAttribute<SerializableAttribute<decltype(origin)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontGradientEffect, void, Bitmap*>(&type, "Process", &o2::FontGradientEffect::Process);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontGradientEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontGradientEffect::GetSizeExtend);
	funcInfo = TypeInitializer::RegFunction<o2::FontGradientEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontGradientEffect::IsEqual);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::FontColorEffect::InitializeType(o2::FontColorEffect* sample)
{
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)sample, sample->color).AddAttribute<SerializableAttribute<decltype(color)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontColorEffect, void, Bitmap*>(&type, "Process", &o2::FontColorEffect::Process);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontColorEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontColorEffect::GetSizeExtend);
	funcInfo = TypeInitializer::RegFunction<o2::FontColorEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontColorEffect::IsEqual);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::FontShadowEffect::InitializeType(o2::FontShadowEffect* sample)
{
	TypeInitializer::RegField(&type, "blurRadius", (size_t)(char*)(&sample->blurRadius) - (size_t)(char*)sample, sample->blurRadius).AddAttribute<SerializableAttribute<decltype(blurRadius)>>();
	TypeInitializer::RegField(&type, "offset", (size_t)(char*)(&sample->offset) - (size_t)(char*)sample, sample->offset).AddAttribute<SerializableAttribute<decltype(offset)>>();
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)sample, sample->color).AddAttribute<SerializableAttribute<decltype(color)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontShadowEffect, void, Bitmap*>(&type, "Process", &o2::FontShadowEffect::Process);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontShadowEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontShadowEffect::GetSizeExtend);
	funcInfo = TypeInitializer::RegFunction<o2::FontShadowEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontShadowEffect::IsEqual);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::Actor::InitializeType(o2::Actor* sample)
{
	TypeInitializer::RegField(&type, "id", (size_t)(char*)(&sample->id) - (size_t)(char*)sample, sample->id);
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name);
	TypeInitializer::RegField(&type, "enabled", (size_t)(char*)(&sample->enabled) - (size_t)(char*)sample, sample->enabled);
	TypeInitializer::RegField(&type, "enabledInHierarchy", (size_t)(char*)(&sample->enabledInHierarchy) - (size_t)(char*)sample, sample->enabledInHierarchy);
	TypeInitializer::RegField(&type, "locked", (size_t)(char*)(&sample->locked) - (size_t)(char*)sample, sample->locked);
	TypeInitializer::RegField(&type, "lockedInHierarchy", (size_t)(char*)(&sample->lockedInHierarchy) - (size_t)(char*)sample, sample->lockedInHierarchy);
	TypeInitializer::RegField(&type, "parent", (size_t)(char*)(&sample->parent) - (size_t)(char*)sample, sample->parent);
	TypeInitializer::RegField(&type, "layer", (size_t)(char*)(&sample->layer) - (size_t)(char*)sample, sample->layer);
	TypeInitializer::RegField(&type, "layerName", (size_t)(char*)(&sample->layerName) - (size_t)(char*)sample, sample->layerName);
	TypeInitializer::RegField(&type, "childs", (size_t)(char*)(&sample->childs) - (size_t)(char*)sample, sample->childs);
	TypeInitializer::RegField(&type, "child", (size_t)(char*)(&sample->child) - (size_t)(char*)sample, sample->child);
	TypeInitializer::RegField(&type, "components", (size_t)(char*)(&sample->components) - (size_t)(char*)sample, sample->components);
	TypeInitializer::RegField(&type, "component", (size_t)(char*)(&sample->component) - (size_t)(char*)sample, sample->component);
	TypeInitializer::RegField(&type, "tag", (size_t)(char*)(&sample->tag) - (size_t)(char*)sample, sample->tag);
	TypeInitializer::RegField(&type, "transform", (size_t)(char*)(&sample->transform) - (size_t)(char*)sample, sample->transform).AddAttribute<SerializableAttribute<decltype(transform)>>();
	TypeInitializer::RegField(&type, "onEnableChanged", (size_t)(char*)(&sample->onEnableChanged) - (size_t)(char*)sample, sample->onEnableChanged);
	TypeInitializer::RegField(&type, "onLockChanged", (size_t)(char*)(&sample->onLockChanged) - (size_t)(char*)sample, sample->onLockChanged);
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId).AddAttribute<SerializableAttribute<decltype(mId)>>();
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)sample, sample->mName).AddAttribute<SerializableAttribute<decltype(mName)>>();
	TypeInitializer::RegField(&type, "mParent", (size_t)(char*)(&sample->mParent) - (size_t)(char*)sample, sample->mParent);
	TypeInitializer::RegField(&type, "mChilds", (size_t)(char*)(&sample->mChilds) - (size_t)(char*)sample, sample->mChilds).AddAttribute<SerializableAttribute<decltype(mChilds)>>();
	TypeInitializer::RegField(&type, "mCompontents", (size_t)(char*)(&sample->mCompontents) - (size_t)(char*)sample, sample->mCompontents).AddAttribute<SerializableAttribute<decltype(mCompontents)>>();
	TypeInitializer::RegField(&type, "mLayer", (size_t)(char*)(&sample->mLayer) - (size_t)(char*)sample, sample->mLayer);
	TypeInitializer::RegField(&type, "mTags", (size_t)(char*)(&sample->mTags) - (size_t)(char*)sample, sample->mTags);
	TypeInitializer::RegField(&type, "mEnabled", (size_t)(char*)(&sample->mEnabled) - (size_t)(char*)sample, sample->mEnabled).AddAttribute<SerializableAttribute<decltype(mEnabled)>>();
	TypeInitializer::RegField(&type, "mResEnabled", (size_t)(char*)(&sample->mResEnabled) - (size_t)(char*)sample, sample->mResEnabled);
	TypeInitializer::RegField(&type, "mLocked", (size_t)(char*)(&sample->mLocked) - (size_t)(char*)sample, sample->mLocked).AddAttribute<SerializableAttribute<decltype(mLocked)>>();
	TypeInitializer::RegField(&type, "mResLocked", (size_t)(char*)(&sample->mResLocked) - (size_t)(char*)sample, sample->mResLocked);
	auto funcInfo = TypeInitializer::RegFunction<o2::Actor, void, float>(&type, "Update", &o2::Actor::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, float>(&type, "UpdateChilds", &o2::Actor::UpdateChilds);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, const String&>(&type, "SetName", &o2::Actor::SetName);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, String>(&type, "GetName", &o2::Actor::GetName);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, UInt64>(&type, "GetId", &o2::Actor::GetId);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, UInt64>(&type, "SetId", &o2::Actor::SetId);
	TypeInitializer::RegFuncParam<UInt64>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, bool>(&type, "GenNewId", &o2::Actor::GenNewId);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "childs");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "ExcludeFromScene", &o2::Actor::ExcludeFromScene);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "IncludeInScene", &o2::Actor::IncludeInScene);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, bool>(&type, "SetEnabled", &o2::Actor::SetEnabled);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "active");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "Enable", &o2::Actor::Enable);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "Disable", &o2::Actor::Disable);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool>(&type, "IsEnabled", &o2::Actor::IsEnabled);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool>(&type, "IsEnabledInHierarchy", &o2::Actor::IsEnabledInHierarchy);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, bool>(&type, "SetLocked", &o2::Actor::SetLocked);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "locked");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "Lock", &o2::Actor::Lock);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "Unlock", &o2::Actor::Unlock);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool>(&type, "IsLocked", &o2::Actor::IsLocked);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool>(&type, "IsLockedInHierarchy", &o2::Actor::IsLockedInHierarchy);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, int>(&type, "SetPositionIndexInParent", &o2::Actor::SetPositionIndexInParent);
	TypeInitializer::RegFuncParam<int>(funcInfo, "index");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Actor*, bool>(&type, "SetParent", &o2::Actor::SetParent);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "worldPositionStays");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Actor*>(&type, "GetParent", &o2::Actor::GetParent);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Actor*, Actor*>(&type, "AddChild", &o2::Actor::AddChild);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Actor*, Actor*, int>(&type, "AddChild", &o2::Actor::AddChild);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<int>(funcInfo, "index");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Actor*, const String&>(&type, "GetChild", &o2::Actor::GetChild);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, ActorsVec>(&type, "GetChilds", &o2::Actor::GetChilds);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Actor*, bool>(&type, "RemoveChild", &o2::Actor::RemoveChild);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "RemoveAllChilds", &o2::Actor::RemoveAllChilds);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Component*, Component*>(&type, "AddComponent", &o2::Actor::AddComponent);
	TypeInitializer::RegFuncParam<Component*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Component*, bool>(&type, "RemoveComponent", &o2::Actor::RemoveComponent);
	TypeInitializer::RegFuncParam<Component*>(funcInfo, "component");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "RemoveAllComponents", &o2::Actor::RemoveAllComponents);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Component*, const String&>(&type, "GetComponent", &o2::Actor::GetComponent);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "typeName");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, ComponentsVec>(&type, "GetComponents", &o2::Actor::GetComponents);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Scene::Layer*>(&type, "SetLayer", &o2::Actor::SetLayer);
	TypeInitializer::RegFuncParam<Scene::Layer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, const String&>(&type, "SetLayerName", &o2::Actor::SetLayerName);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "layerName");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Scene::Layer*>(&type, "GetLayer", &o2::Actor::GetLayer);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, String>(&type, "GetLayerName", &o2::Actor::GetLayerName);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, const String&>(&type, "AddTag", &o2::Actor::AddTag);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "tag");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, const String&>(&type, "RemoveTag", &o2::Actor::RemoveTag);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "tag");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool, const String&>(&type, "IsHaveTag", &o2::Actor::IsHaveTag);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "tag");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "ClearTags", &o2::Actor::ClearTags);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, const StringsVec&>(&type, "GetTags", &o2::Actor::GetTags);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "OnTransformChanged", &o2::Actor::OnTransformChanged);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Actor*>(&type, "SetParentProp", &o2::Actor::SetParentProp);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "UpdateEnabled", &o2::Actor::UpdateEnabled);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "UpdateLocking", &o2::Actor::UpdateLocking);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, DataNode&>(&type, "OnSerialize", &o2::Actor::OnSerialize);
	TypeInitializer::RegFuncParam<DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, const DataNode&>(&type, "OnDeserialized", &o2::Actor::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Dictionary<String, Actor*>>(&type, "GetAllChilds", &o2::Actor::GetAllChilds);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Dictionary<String, Component*>>(&type, "GetAllComponents", &o2::Actor::GetAllComponents);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "ComponentsExcludeFromScene", &o2::Actor::ComponentsExcludeFromScene);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "ComponentsIncludeToScene", &o2::Actor::ComponentsIncludeToScene);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "InitializeProperties", &o2::Actor::InitializeProperties);
}

void o2::ActorTransform::InitializeType(o2::ActorTransform* sample)
{
	TypeInitializer::RegField(&type, "actor", (size_t)(char*)(&sample->actor) - (size_t)(char*)sample, sample->actor);
	TypeInitializer::RegField(&type, "worldPosition", (size_t)(char*)(&sample->worldPosition) - (size_t)(char*)sample, sample->worldPosition);
	TypeInitializer::RegField(&type, "worldRect", (size_t)(char*)(&sample->worldRect) - (size_t)(char*)sample, sample->worldRect);
	TypeInitializer::RegField(&type, "worldAngle", (size_t)(char*)(&sample->worldAngle) - (size_t)(char*)sample, sample->worldAngle);
	TypeInitializer::RegField(&type, "worldBasis", (size_t)(char*)(&sample->worldBasis) - (size_t)(char*)sample, sample->worldBasis);
	TypeInitializer::RegField(&type, "worldNonSizedBasis", (size_t)(char*)(&sample->worldNonSizedBasis) - (size_t)(char*)sample, sample->worldNonSizedBasis);
	TypeInitializer::RegField(&type, "worldAABB", (size_t)(char*)(&sample->worldAABB) - (size_t)(char*)sample, sample->worldAABB);
	TypeInitializer::RegField(&type, "mWorldNonSizedTransform", (size_t)(char*)(&sample->mWorldNonSizedTransform) - (size_t)(char*)sample, sample->mWorldNonSizedTransform);
	TypeInitializer::RegField(&type, "mWorldTransform", (size_t)(char*)(&sample->mWorldTransform) - (size_t)(char*)sample, sample->mWorldTransform);
	TypeInitializer::RegField(&type, "mParentInvertedTransform", (size_t)(char*)(&sample->mParentInvertedTransform) - (size_t)(char*)sample, sample->mParentInvertedTransform);
	TypeInitializer::RegField(&type, "mParentTransform", (size_t)(char*)(&sample->mParentTransform) - (size_t)(char*)sample, sample->mParentTransform);
	TypeInitializer::RegField(&type, "mIsParentInvTransformActual", (size_t)(char*)(&sample->mIsParentInvTransformActual) - (size_t)(char*)sample, sample->mIsParentInvTransformActual);
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner);
	auto funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Actor*>(&type, "GetOwnerActor", &o2::ActorTransform::GetOwnerActor);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldPivot", &o2::ActorTransform::SetWorldPivot);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "pivot");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldPivot", &o2::ActorTransform::GetWorldPivot);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldPosition", &o2::ActorTransform::SetWorldPosition);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldPosition", &o2::ActorTransform::GetWorldPosition);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const RectF&>(&type, "SetWorldRect", &o2::ActorTransform::SetWorldRect);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, RectF>(&type, "GetWorldRect", &o2::ActorTransform::GetWorldRect);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, float>(&type, "SetWorldAngle", &o2::ActorTransform::SetWorldAngle);
	TypeInitializer::RegFuncParam<float>(funcInfo, "rad");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, float>(&type, "GetWorldAngle", &o2::ActorTransform::GetWorldAngle);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Basis&>(&type, "SetWorldBasis", &o2::ActorTransform::SetWorldBasis);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "basis");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Basis>(&type, "GetWorldBasis", &o2::ActorTransform::GetWorldBasis);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Basis&>(&type, "SetWorldNonSizedBasis", &o2::ActorTransform::SetWorldNonSizedBasis);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "basis");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Basis>(&type, "GetWorldNonSizedBasis", &o2::ActorTransform::GetWorldNonSizedBasis);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const RectF&>(&type, "SetWorldAxisAlignedRect", &o2::ActorTransform::SetWorldAxisAlignedRect);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, RectF>(&type, "GetWorldAxisAlignedRect", &o2::ActorTransform::GetWorldAxisAlignedRect);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldLeftTop", &o2::ActorTransform::SetWorldLeftTop);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldLeftTop", &o2::ActorTransform::GetWorldLeftTop);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldRightTop", &o2::ActorTransform::SetWorldRightTop);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldRightTop", &o2::ActorTransform::GetWorldRightTop);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldLeftBottom", &o2::ActorTransform::SetWorldLeftBottom);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldLeftBottom", &o2::ActorTransform::GetWorldLeftBottom);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldRightBottom", &o2::ActorTransform::SetWorldRightBottom);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldRightBottom", &o2::ActorTransform::GetWorldRightBottom);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldCenter", &o2::ActorTransform::SetWorldCenter);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldCenter", &o2::ActorTransform::GetWorldCenter);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetRight", &o2::ActorTransform::SetRight);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetRight", &o2::ActorTransform::GetRight);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetLeft", &o2::ActorTransform::SetLeft);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetLeft", &o2::ActorTransform::GetLeft);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetUp", &o2::ActorTransform::SetUp);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetUp", &o2::ActorTransform::GetUp);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetDown", &o2::ActorTransform::SetDown);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetDown", &o2::ActorTransform::GetDown);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "LookAt", &o2::ActorTransform::LookAt);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldPoint");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F, const Vec2F&>(&type, "World2LocalPoint", &o2::ActorTransform::World2LocalPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldPoint");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F, const Vec2F&>(&type, "Local2WorldPoint", &o2::ActorTransform::Local2WorldPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "localPoint");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F, const Vec2F&>(&type, "World2LocalDir", &o2::ActorTransform::World2LocalDir);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldDir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F, const Vec2F&>(&type, "Local2WorldDir", &o2::ActorTransform::Local2WorldDir);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "localDir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, bool, const Vec2F&>(&type, "IsPointInside", &o2::ActorTransform::IsPointInside);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, Actor*>(&type, "SetOwner", &o2::ActorTransform::SetOwner);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void>(&type, "UpdateTransform", &o2::ActorTransform::UpdateTransform);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void>(&type, "CheckParentInvTransform", &o2::ActorTransform::CheckParentInvTransform);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void>(&type, "InitializeProperties", &o2::ActorTransform::InitializeProperties);
}

void o2::Component::InitializeType(o2::Component* sample)
{
	TypeInitializer::RegField(&type, "actor", (size_t)(char*)(&sample->actor) - (size_t)(char*)sample, sample->actor);
	TypeInitializer::RegField(&type, "enabled", (size_t)(char*)(&sample->enabled) - (size_t)(char*)sample, sample->enabled);
	TypeInitializer::RegField(&type, "enabledInHierarchy", (size_t)(char*)(&sample->enabledInHierarchy) - (size_t)(char*)sample, sample->enabledInHierarchy);
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner);
	TypeInitializer::RegField(&type, "mEnabled", (size_t)(char*)(&sample->mEnabled) - (size_t)(char*)sample, sample->mEnabled).AddAttribute<SerializableAttribute<decltype(mEnabled)>>();
	TypeInitializer::RegField(&type, "mResEnabled", (size_t)(char*)(&sample->mResEnabled) - (size_t)(char*)sample, sample->mResEnabled);
	auto funcInfo = TypeInitializer::RegFunction<o2::Component, void, float>(&type, "Update", &o2::Component::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::Component, void, bool>(&type, "SetEnabled", &o2::Component::SetEnabled);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "active");
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "Enable", &o2::Component::Enable);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "Disable", &o2::Component::Disable);
	funcInfo = TypeInitializer::RegFunction<o2::Component, bool>(&type, "IsEnabled", &o2::Component::IsEnabled);
	funcInfo = TypeInitializer::RegFunction<o2::Component, bool>(&type, "IsEnabledInHierarchy", &o2::Component::IsEnabledInHierarchy);
	funcInfo = TypeInitializer::RegFunction<o2::Component, Actor*>(&type, "GetOwnerActor", &o2::Component::GetOwnerActor);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void, Scene::Layer*, Scene::Layer*>(&type, "OnLayerChanged", &o2::Component::OnLayerChanged);
	TypeInitializer::RegFuncParam<Scene::Layer*>(funcInfo, "oldLayer");
	TypeInitializer::RegFuncParam<Scene::Layer*>(funcInfo, "newLayer");
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "UpdateEnabled", &o2::Component::UpdateEnabled);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "OnTransformChanged", &o2::Component::OnTransformChanged);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void, Actor*>(&type, "SetOwnerActor", &o2::Component::SetOwnerActor);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "OnExcludeFromScene", &o2::Component::OnExcludeFromScene);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "OnIncludeToScene", &o2::Component::OnIncludeToScene);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "InitializeProperties", &o2::Component::InitializeProperties);
}

void o2::DrawableComponent::InitializeType(o2::DrawableComponent* sample)
{
	TypeInitializer::RegField(&type, "drawDepth", (size_t)(char*)(&sample->drawDepth) - (size_t)(char*)sample, sample->drawDepth);
	TypeInitializer::RegField(&type, "mDrawingDepth", (size_t)(char*)(&sample->mDrawingDepth) - (size_t)(char*)sample, sample->mDrawingDepth);
	auto funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void, float>(&type, "SetDrawingDepth", &o2::DrawableComponent::SetDrawingDepth);
	TypeInitializer::RegFuncParam<float>(funcInfo, "depth");
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, float>(&type, "GetDrawingDepth", &o2::DrawableComponent::GetDrawingDepth);
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void, Scene::Layer*, Scene::Layer*>(&type, "OnLayerChanged", &o2::DrawableComponent::OnLayerChanged);
	TypeInitializer::RegFuncParam<Scene::Layer*>(funcInfo, "oldLayer");
	TypeInitializer::RegFuncParam<Scene::Layer*>(funcInfo, "newLayer");
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void>(&type, "UpdateEnabled", &o2::DrawableComponent::UpdateEnabled);
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void, Actor*>(&type, "SetOwnerActor", &o2::DrawableComponent::SetOwnerActor);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void>(&type, "OnExcludeFromScene", &o2::DrawableComponent::OnExcludeFromScene);
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void>(&type, "OnIncludeToScene", &o2::DrawableComponent::OnIncludeToScene);
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void>(&type, "InitializeProperties", &o2::DrawableComponent::InitializeProperties);
}

void o2::ImageComponent::InitializeType(o2::ImageComponent* sample)
{
	TypeInitializer::RegField(&type, "texture", (size_t)(char*)(&sample->texture) - (size_t)(char*)sample, sample->texture);
	TypeInitializer::RegField(&type, "textureSrcRect", (size_t)(char*)(&sample->textureSrcRect) - (size_t)(char*)sample, sample->textureSrcRect);
	TypeInitializer::RegField(&type, "imageAssetId", (size_t)(char*)(&sample->imageAssetId) - (size_t)(char*)sample, sample->imageAssetId);
	TypeInitializer::RegField(&type, "imageAssetPath", (size_t)(char*)(&sample->imageAssetPath) - (size_t)(char*)sample, sample->imageAssetPath);
	TypeInitializer::RegField(&type, "imageAsset", (size_t)(char*)(&sample->imageAsset) - (size_t)(char*)sample, sample->imageAsset);
	TypeInitializer::RegField(&type, "bitmap", (size_t)(char*)(&sample->bitmap) - (size_t)(char*)sample, sample->bitmap);
	TypeInitializer::RegField(&type, "leftTopColor", (size_t)(char*)(&sample->leftTopColor) - (size_t)(char*)sample, sample->leftTopColor);
	TypeInitializer::RegField(&type, "rightTopColor", (size_t)(char*)(&sample->rightTopColor) - (size_t)(char*)sample, sample->rightTopColor);
	TypeInitializer::RegField(&type, "leftBottomColor", (size_t)(char*)(&sample->leftBottomColor) - (size_t)(char*)sample, sample->leftBottomColor);
	TypeInitializer::RegField(&type, "rightBottomColor", (size_t)(char*)(&sample->rightBottomColor) - (size_t)(char*)sample, sample->rightBottomColor);
	TypeInitializer::RegField(&type, "mode", (size_t)(char*)(&sample->mode) - (size_t)(char*)sample, sample->mode);
	TypeInitializer::RegField(&type, "fill", (size_t)(char*)(&sample->fill) - (size_t)(char*)sample, sample->fill);
	TypeInitializer::RegField(&type, "sliceBorder", (size_t)(char*)(&sample->sliceBorder) - (size_t)(char*)sample, sample->sliceBorder);
	TypeInitializer::RegField(&type, "mSprite", (size_t)(char*)(&sample->mSprite) - (size_t)(char*)sample, sample->mSprite).AddAttribute<SerializableAttribute<decltype(mSprite)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "Draw", &o2::ImageComponent::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, TextureRef>(&type, "SetTexture", &o2::ImageComponent::SetTexture);
	TypeInitializer::RegFuncParam<TextureRef>(funcInfo, "texture");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, TextureRef>(&type, "GetTexture", &o2::ImageComponent::GetTexture);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const RectI&>(&type, "SetTextureSrcRect", &o2::ImageComponent::SetTextureSrcRect);
	TypeInitializer::RegFuncParam<const RectI&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, RectI>(&type, "GetTextureSrcRect", &o2::ImageComponent::GetTextureSrcRect);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, Corner, const Color4&>(&type, "SetCornerColor", &o2::ImageComponent::SetCornerColor);
	TypeInitializer::RegFuncParam<Corner>(funcInfo, "corner");
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, Color4, Corner>(&type, "GetCornerColor", &o2::ImageComponent::GetCornerColor);
	TypeInitializer::RegFuncParam<Corner>(funcInfo, "corner");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const Color4&>(&type, "SetLeftTopColor", &o2::ImageComponent::SetLeftTopColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, Color4>(&type, "GetLeftTopCorner", &o2::ImageComponent::GetLeftTopCorner);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const Color4&>(&type, "SetRightTopColor", &o2::ImageComponent::SetRightTopColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, Color4>(&type, "GetRightTopCorner", &o2::ImageComponent::GetRightTopCorner);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const Color4&>(&type, "SetRightBottomColor", &o2::ImageComponent::SetRightBottomColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, Color4>(&type, "GetRightBottomCorner", &o2::ImageComponent::GetRightBottomCorner);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const Color4&>(&type, "SetLeftBottomColor", &o2::ImageComponent::SetLeftBottomColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, Color4>(&type, "GetLeftBottomCorner", &o2::ImageComponent::GetLeftBottomCorner);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, float>(&type, "SetFill", &o2::ImageComponent::SetFill);
	TypeInitializer::RegFuncParam<float>(funcInfo, "fill");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, float>(&type, "GetFill", &o2::ImageComponent::GetFill);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, SpriteMode>(&type, "SetMode", &o2::ImageComponent::SetMode);
	TypeInitializer::RegFuncParam<SpriteMode>(funcInfo, "mode");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, SpriteMode>(&type, "GetMode", &o2::ImageComponent::GetMode);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const RectI&>(&type, "SetSliceBorder", &o2::ImageComponent::SetSliceBorder);
	TypeInitializer::RegFuncParam<const RectI&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, RectI>(&type, "GetSliceBorder", &o2::ImageComponent::GetSliceBorder);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, ImageAsset*>(&type, "LoadFromImage", &o2::ImageComponent::LoadFromImage);
	TypeInitializer::RegFuncParam<ImageAsset*>(funcInfo, "image");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const String&>(&type, "LoadFromImage", &o2::ImageComponent::LoadFromImage);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "imagePath");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, AssetId>(&type, "LoadFromImage", &o2::ImageComponent::LoadFromImage);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "imageId");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const Color4&>(&type, "LoadMonoColor", &o2::ImageComponent::LoadMonoColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, Bitmap*>(&type, "LoadFromBitmap", &o2::ImageComponent::LoadFromBitmap);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, AssetId>(&type, "GetImageId", &o2::ImageComponent::GetImageId);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "NormalizeSize", &o2::ImageComponent::NormalizeSize);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "NormalizeAspectByWidth", &o2::ImageComponent::NormalizeAspectByWidth);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "NormalizeAspectByHeight", &o2::ImageComponent::NormalizeAspectByHeight);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "NormalizeAspect", &o2::ImageComponent::NormalizeAspect);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "OnTransformChanged", &o2::ImageComponent::OnTransformChanged);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, Actor*>(&type, "SetOwnerActor", &o2::ImageComponent::SetOwnerActor);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "InitializeProperties", &o2::ImageComponent::InitializeProperties);
}

void o2::UIButton::InitializeType(o2::UIButton* sample)
{
	TypeInitializer::RegField(&type, "caption", (size_t)(char*)(&sample->caption) - (size_t)(char*)sample, sample->caption);
	TypeInitializer::RegField(&type, "icon", (size_t)(char*)(&sample->icon) - (size_t)(char*)sample, sample->icon);
	TypeInitializer::RegField(&type, "buttonsGroup", (size_t)(char*)(&sample->buttonsGroup) - (size_t)(char*)sample, sample->buttonsGroup);
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)sample, sample->onClick);
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)sample, sample->shortcut);
	TypeInitializer::RegField(&type, "mCaptionText", (size_t)(char*)(&sample->mCaptionText) - (size_t)(char*)sample, sample->mCaptionText);
	TypeInitializer::RegField(&type, "mIconSprite", (size_t)(char*)(&sample->mIconSprite) - (size_t)(char*)sample, sample->mIconSprite);
	TypeInitializer::RegField(&type, "mButtonGroup", (size_t)(char*)(&sample->mButtonGroup) - (size_t)(char*)sample, sample->mButtonGroup);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIButton, void>(&type, "Draw", &o2::UIButton::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const WString&>(&type, "SetCaption", &o2::UIButton::SetCaption);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, WString>(&type, "GetCaption", &o2::UIButton::GetCaption);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, Sprite*>(&type, "SetIcon", &o2::UIButton::SetIcon);
	TypeInitializer::RegFuncParam<Sprite*>(funcInfo, "sprite");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, Sprite*>(&type, "GetIcon", &o2::UIButton::GetIcon);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, UIButtonGroup*>(&type, "SetButtonGroup", &o2::UIButton::SetButtonGroup);
	TypeInitializer::RegFuncParam<UIButtonGroup*>(funcInfo, "group");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, UIButtonGroup*>(&type, "GetButtonGroup", &o2::UIButton::GetButtonGroup);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, bool>(&type, "IsSelectable", &o2::UIButton::IsSelectable);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIButton::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIButton::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIButton::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIButton::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIButton::OnCursorEnter);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIButton::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Key&>(&type, "OnKeyPressed", &o2::UIButton::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Key&>(&type, "OnKeyReleased", &o2::UIButton::OnKeyReleased);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIButton::OnLayerAdded);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void>(&type, "OnVisibleChanged", &o2::UIButton::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void>(&type, "InitializeProperties", &o2::UIButton::InitializeProperties);
}

void o2::UIContextMenuItem::InitializeType(o2::UIContextMenuItem* sample)
{
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)sample, sample->onClick);
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)sample, sample->shortcut);
	TypeInitializer::RegField(&type, "mSubMenu", (size_t)(char*)(&sample->mSubMenu) - (size_t)(char*)sample, sample->mSubMenu);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIContextMenuItem, UIContextMenu*>(&type, "GetSubMenu", &o2::UIContextMenuItem::GetSubMenu);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenuItem, void, UIWidget*>(&type, "OnChildAdded", &o2::UIContextMenuItem::OnChildAdded);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
}

void o2::UIContextMenu::InitializeType(o2::UIContextMenu* sample)
{
	TypeInitializer::RegField(&type, "mOpenSubMenuDelay", (size_t)(char*)(&sample->mOpenSubMenuDelay) - (size_t)(char*)sample, sample->mOpenSubMenuDelay);
	TypeInitializer::RegField(&type, "mFitSizeMin", (size_t)(char*)(&sample->mFitSizeMin) - (size_t)(char*)sample, sample->mFitSizeMin).AddAttribute<SerializableAttribute<decltype(mFitSizeMin)>>();
	TypeInitializer::RegField(&type, "mParentContextMenu", (size_t)(char*)(&sample->mParentContextMenu) - (size_t)(char*)sample, sample->mParentContextMenu);
	TypeInitializer::RegField(&type, "mChildContextMenu", (size_t)(char*)(&sample->mChildContextMenu) - (size_t)(char*)sample, sample->mChildContextMenu);
	TypeInitializer::RegField(&type, "mLayout", (size_t)(char*)(&sample->mLayout) - (size_t)(char*)sample, sample->mLayout);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)sample, sample->mItemSample).AddAttribute<SerializableAttribute<decltype(mItemSample)>>();
	TypeInitializer::RegField(&type, "mSeparatorSample", (size_t)(char*)(&sample->mSeparatorSample) - (size_t)(char*)sample, sample->mSeparatorSample).AddAttribute<SerializableAttribute<decltype(mSeparatorSample)>>();
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)sample, sample->mSelectionDrawable).AddAttribute<SerializableAttribute<decltype(mSelectionDrawable)>>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)sample, sample->mSelectionLayout).AddAttribute<SerializableAttribute<decltype(mSelectionLayout)>>();
	TypeInitializer::RegField(&type, "mCurrentSelectionRect", (size_t)(char*)(&sample->mCurrentSelectionRect) - (size_t)(char*)sample, sample->mCurrentSelectionRect);
	TypeInitializer::RegField(&type, "mTargetSelectionRect", (size_t)(char*)(&sample->mTargetSelectionRect) - (size_t)(char*)sample, sample->mTargetSelectionRect);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)sample, sample->mLastSelectCheckCursor);
	TypeInitializer::RegField(&type, "mSelectedItem", (size_t)(char*)(&sample->mSelectedItem) - (size_t)(char*)sample, sample->mSelectedItem);
	TypeInitializer::RegField(&type, "mSelectSubContextTime", (size_t)(char*)(&sample->mSelectSubContextTime) - (size_t)(char*)sample, sample->mSelectSubContextTime);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, float>(&type, "Update", &o2::UIContextMenu::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "Draw", &o2::UIContextMenu::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, UIContextMenu*, const Vec2F&>(&type, "Show", &o2::UIContextMenu::Show);
	TypeInitializer::RegFuncParam<UIContextMenu*>(funcInfo, "parent");
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Vec2F&>(&type, "Show", &o2::UIContextMenu::Show);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIWidget*, const Item&>(&type, "AddItem", &o2::UIContextMenu::AddItem);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIWidget*, const WString&, const Function<void()>&, ImageAsset*, const ShortcutKeys&>(&type, "AddItem", &o2::UIContextMenu::AddItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "path");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "clickFunc");
	TypeInitializer::RegFuncParam<ImageAsset*>(funcInfo, "icon");
	TypeInitializer::RegFuncParam<const ShortcutKeys&>(funcInfo, "shortcut");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIWidget*, const Item&, int>(&type, "InsertItem", &o2::UIContextMenu::InsertItem);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, Vector<Item>>(&type, "AddItems", &o2::UIContextMenu::AddItems);
	TypeInitializer::RegFuncParam<Vector<Item>>(funcInfo, "items");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, Vector<Item>, int>(&type, "InsertItems", &o2::UIContextMenu::InsertItems);
	TypeInitializer::RegFuncParam<Vector<Item>>(funcInfo, "items");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, Item, int>(&type, "GetItem", &o2::UIContextMenu::GetItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, Vector<Item>>(&type, "GetItems", &o2::UIContextMenu::GetItems);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, int>(&type, "RemoveItem", &o2::UIContextMenu::RemoveItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const WString&>(&type, "RemoveItem", &o2::UIContextMenu::RemoveItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "RemoveAllItems", &o2::UIContextMenu::RemoveAllItems);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIVerticalLayout*>(&type, "GetItemsLayout", &o2::UIContextMenu::GetItemsLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIContextMenuItem*>(&type, "GetItemSample", &o2::UIContextMenu::GetItemSample);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIWidget*>(&type, "GetSeparatorSample", &o2::UIContextMenu::GetSeparatorSample);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, Sprite*>(&type, "GetSelectionDrawable", &o2::UIContextMenu::GetSelectionDrawable);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Layout&>(&type, "SetSelectionDrawableLayout", &o2::UIContextMenu::SetSelectionDrawableLayout);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, Layout>(&type, "GetSelectionDrawableLayout", &o2::UIContextMenu::GetSelectionDrawableLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, float>(&type, "SetMinFitSize", &o2::UIContextMenu::SetMinFitSize);
	TypeInitializer::RegFuncParam<float>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, bool>(&type, "IsScrollable", &o2::UIContextMenu::IsScrollable);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, bool>(&type, "UpdateLayout", &o2::UIContextMenu::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "FitSize", &o2::UIContextMenu::FitSize);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "FitPosition", &o2::UIContextMenu::FitPosition);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "SpecialDraw", &o2::UIContextMenu::SpecialDraw);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIContextMenuItem*, const Item&>(&type, "CreateItem", &o2::UIContextMenu::CreateItem);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, Item, int>(&type, "GetItemDef", &o2::UIContextMenu::GetItemDef);
	TypeInitializer::RegFuncParam<int>(funcInfo, "idx");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "OnVisibleChanged", &o2::UIContextMenu::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIContextMenuItem*, const Vec2F&>(&type, "GetItemUnderPoint", &o2::UIContextMenu::GetItemUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Vec2F&>(&type, "UpdateHover", &o2::UIContextMenu::UpdateHover);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIContextMenu::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIContextMenu::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIContextMenu::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIContextMenu::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UIContextMenu::OnCursorMoved);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Key&>(&type, "OnKeyPressed", &o2::UIContextMenu::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "HideWithParent", &o2::UIContextMenu::HideWithParent);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "HideWithChild", &o2::UIContextMenu::HideWithChild);
}

void o2::UICustomDropDown::InitializeType(o2::UICustomDropDown* sample)
{
	TypeInitializer::RegField(&type, "selectedItem", (size_t)(char*)(&sample->selectedItem) - (size_t)(char*)sample, sample->selectedItem);
	TypeInitializer::RegField(&type, "selectedItemPos", (size_t)(char*)(&sample->selectedItemPos) - (size_t)(char*)sample, sample->selectedItemPos);
	TypeInitializer::RegField(&type, "item", (size_t)(char*)(&sample->item) - (size_t)(char*)sample, sample->item);
	TypeInitializer::RegField(&type, "itemsCount", (size_t)(char*)(&sample->itemsCount) - (size_t)(char*)sample, sample->itemsCount);
	TypeInitializer::RegField(&type, "onSelectedPos", (size_t)(char*)(&sample->onSelectedPos) - (size_t)(char*)sample, sample->onSelectedPos);
	TypeInitializer::RegField(&type, "onSelectedItem", (size_t)(char*)(&sample->onSelectedItem) - (size_t)(char*)sample, sample->onSelectedItem);
	TypeInitializer::RegField(&type, "mList", (size_t)(char*)(&sample->mList) - (size_t)(char*)sample, sample->mList).AddAttribute<SerializableAttribute<decltype(mList)>>();
	TypeInitializer::RegField(&type, "mClipLayout", (size_t)(char*)(&sample->mClipLayout) - (size_t)(char*)sample, sample->mClipLayout);
	TypeInitializer::RegField(&type, "mAbsoluteClip", (size_t)(char*)(&sample->mAbsoluteClip) - (size_t)(char*)sample, sample->mAbsoluteClip);
	TypeInitializer::RegField(&type, "mMaxListItems", (size_t)(char*)(&sample->mMaxListItems) - (size_t)(char*)sample, sample->mMaxListItems);
	auto funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, float>(&type, "Update", &o2::UICustomDropDown::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "Draw", &o2::UICustomDropDown::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "Expand", &o2::UICustomDropDown::Expand);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "Collapse", &o2::UICustomDropDown::Collapse);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, bool>(&type, "IsExpanded", &o2::UICustomDropDown::IsExpanded);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, UIWidget*>(&type, "SetItemSample", &o2::UICustomDropDown::SetItemSample);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "sample");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIWidget*>(&type, "GetItemSample", &o2::UICustomDropDown::GetItemSample);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIVerticalLayout*>(&type, "GetLayout", &o2::UICustomDropDown::GetLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIWidget*>(&type, "AddItem", &o2::UICustomDropDown::AddItem);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIWidget*, int>(&type, "AddItem", &o2::UICustomDropDown::AddItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, UIWidget*>(&type, "RemoveItem", &o2::UICustomDropDown::RemoveItem);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, int>(&type, "RemoveItem", &o2::UICustomDropDown::RemoveItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, int, int>(&type, "MoveItem", &o2::UICustomDropDown::MoveItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	TypeInitializer::RegFuncParam<int>(funcInfo, "newPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, UIWidget*, int>(&type, "MoveItem", &o2::UICustomDropDown::MoveItem);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	TypeInitializer::RegFuncParam<int>(funcInfo, "newPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, int, UIWidget*>(&type, "GetItemPosition", &o2::UICustomDropDown::GetItemPosition);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIWidget*, int>(&type, "GetItem", &o2::UICustomDropDown::GetItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "RemoveAllItems", &o2::UICustomDropDown::RemoveAllItems);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Function<bool(UIWidget*, UIWidget*)>&>(&type, "SortItems", &o2::UICustomDropDown::SortItems);
	TypeInitializer::RegFuncParam<const Function<bool(UIWidget*, UIWidget*)>&>(funcInfo, "sortFunc");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, int>(&type, "GetItemsCount", &o2::UICustomDropDown::GetItemsCount);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, UIWidget*>(&type, "SelectItem", &o2::UICustomDropDown::SelectItem);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, int>(&type, "SelectItemAt", &o2::UICustomDropDown::SelectItemAt);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIWidget*>(&type, "GetSelectedItem", &o2::UICustomDropDown::GetSelectedItem);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, int>(&type, "GetSelectedItemPosition", &o2::UICustomDropDown::GetSelectedItemPosition);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UICustomList*>(&type, "GetListView", &o2::UICustomDropDown::GetListView);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, int>(&type, "SetMaxListSizeInItems", &o2::UICustomDropDown::SetMaxListSizeInItems);
	TypeInitializer::RegFuncParam<int>(funcInfo, "itemsCount");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Layout&>(&type, "SetClippingLayout", &o2::UICustomDropDown::SetClippingLayout);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, Layout>(&type, "GetClippingLayout", &o2::UICustomDropDown::GetClippingLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UICustomDropDown::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UICustomDropDown::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UICustomDropDown::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UICustomDropDown::OnCursorEnter);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UICustomDropDown::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "OnVisibleChanged", &o2::UICustomDropDown::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, bool>(&type, "UpdateLayout", &o2::UICustomDropDown::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "OnItemSelected", &o2::UICustomDropDown::OnItemSelected);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "OnSelectionChanged", &o2::UICustomDropDown::OnSelectionChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "InitializeProperties", &o2::UICustomDropDown::InitializeProperties);
}

void o2::UICustomList::InitializeType(o2::UICustomList* sample)
{
	TypeInitializer::RegField(&type, "selectedItem", (size_t)(char*)(&sample->selectedItem) - (size_t)(char*)sample, sample->selectedItem);
	TypeInitializer::RegField(&type, "selectedItemPos", (size_t)(char*)(&sample->selectedItemPos) - (size_t)(char*)sample, sample->selectedItemPos);
	TypeInitializer::RegField(&type, "item", (size_t)(char*)(&sample->item) - (size_t)(char*)sample, sample->item);
	TypeInitializer::RegField(&type, "itemsCount", (size_t)(char*)(&sample->itemsCount) - (size_t)(char*)sample, sample->itemsCount);
	TypeInitializer::RegField(&type, "onSelectedPos", (size_t)(char*)(&sample->onSelectedPos) - (size_t)(char*)sample, sample->onSelectedPos);
	TypeInitializer::RegField(&type, "onSelectedItem", (size_t)(char*)(&sample->onSelectedItem) - (size_t)(char*)sample, sample->onSelectedItem);
	TypeInitializer::RegField(&type, "mVerLayout", (size_t)(char*)(&sample->mVerLayout) - (size_t)(char*)sample, sample->mVerLayout);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)sample, sample->mItemSample).AddAttribute<SerializableAttribute<decltype(mItemSample)>>();
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)sample, sample->mSelectionDrawable).AddAttribute<SerializableAttribute<decltype(mSelectionDrawable)>>();
	TypeInitializer::RegField(&type, "mHoverDrawable", (size_t)(char*)(&sample->mHoverDrawable) - (size_t)(char*)sample, sample->mHoverDrawable).AddAttribute<SerializableAttribute<decltype(mHoverDrawable)>>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)sample, sample->mSelectionLayout).AddAttribute<SerializableAttribute<decltype(mSelectionLayout)>>();
	TypeInitializer::RegField(&type, "mHoverLayout", (size_t)(char*)(&sample->mHoverLayout) - (size_t)(char*)sample, sample->mHoverLayout).AddAttribute<SerializableAttribute<decltype(mHoverLayout)>>();
	TypeInitializer::RegField(&type, "mSelectedItem", (size_t)(char*)(&sample->mSelectedItem) - (size_t)(char*)sample, sample->mSelectedItem);
	TypeInitializer::RegField(&type, "mCurrentSelectionRect", (size_t)(char*)(&sample->mCurrentSelectionRect) - (size_t)(char*)sample, sample->mCurrentSelectionRect);
	TypeInitializer::RegField(&type, "mTargetSelectionRect", (size_t)(char*)(&sample->mTargetSelectionRect) - (size_t)(char*)sample, sample->mTargetSelectionRect);
	TypeInitializer::RegField(&type, "mCurrentHoverRect", (size_t)(char*)(&sample->mCurrentHoverRect) - (size_t)(char*)sample, sample->mCurrentHoverRect);
	TypeInitializer::RegField(&type, "mTargetHoverRect", (size_t)(char*)(&sample->mTargetHoverRect) - (size_t)(char*)sample, sample->mTargetHoverRect);
	TypeInitializer::RegField(&type, "mLastHoverCheckCursor", (size_t)(char*)(&sample->mLastHoverCheckCursor) - (size_t)(char*)sample, sample->mLastHoverCheckCursor);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)sample, sample->mLastSelectCheckCursor);
	auto funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, float>(&type, "Update", &o2::UICustomList::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "Draw", &o2::UICustomList::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, UIWidget*>(&type, "SetItemSample", &o2::UICustomList::SetItemSample);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "sample");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*>(&type, "GetItemSample", &o2::UICustomList::GetItemSample);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIVerticalLayout*>(&type, "GetLayout", &o2::UICustomList::GetLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*>(&type, "AddItem", &o2::UICustomList::AddItem);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*, int>(&type, "AddItem", &o2::UICustomList::AddItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, UIWidget*>(&type, "RemoveItem", &o2::UICustomList::RemoveItem);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, int>(&type, "RemoveItem", &o2::UICustomList::RemoveItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, int, int>(&type, "MoveItem", &o2::UICustomList::MoveItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	TypeInitializer::RegFuncParam<int>(funcInfo, "newPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, UIWidget*, int>(&type, "MoveItem", &o2::UICustomList::MoveItem);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	TypeInitializer::RegFuncParam<int>(funcInfo, "newPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, int, UIWidget*>(&type, "GetItemPosition", &o2::UICustomList::GetItemPosition);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*, int>(&type, "GetItem", &o2::UICustomList::GetItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "RemoveAllItems", &o2::UICustomList::RemoveAllItems);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Function<bool(UIWidget*, UIWidget*)>&>(&type, "SortItems", &o2::UICustomList::SortItems);
	TypeInitializer::RegFuncParam<const Function<bool(UIWidget*, UIWidget*)>&>(funcInfo, "sortFunc");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, int>(&type, "GetItemsCount", &o2::UICustomList::GetItemsCount);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, UIWidget*>(&type, "SelectItem", &o2::UICustomList::SelectItem);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, int>(&type, "SelectItemAt", &o2::UICustomList::SelectItemAt);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*>(&type, "GetSelectedItem", &o2::UICustomList::GetSelectedItem);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, int>(&type, "GetSelectedItemPosition", &o2::UICustomList::GetSelectedItemPosition);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, Sprite*>(&type, "GetSelectionDrawable", &o2::UICustomList::GetSelectionDrawable);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, Sprite*>(&type, "GetHoverDrawable", &o2::UICustomList::GetHoverDrawable);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Layout&>(&type, "SetSelectionDrawableLayout", &o2::UICustomList::SetSelectionDrawableLayout);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, Layout>(&type, "GetSelectionDrawableLayout", &o2::UICustomList::GetSelectionDrawableLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Layout&>(&type, "SetHoverDrawableLayout", &o2::UICustomList::SetHoverDrawableLayout);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, Layout>(&type, "GetHoverDrawableLayout", &o2::UICustomList::GetHoverDrawableLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, bool>(&type, "IsScrollable", &o2::UICustomList::IsScrollable);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, float>(&type, "UpdateControls", &o2::UICustomList::UpdateControls);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, bool>(&type, "UpdateLayout", &o2::UICustomList::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UICustomList::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UICustomList::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UICustomList::OnCursorMoved);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UICustomList::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UICustomList::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UICustomList::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, float>(&type, "OnScrolled", &o2::UICustomList::OnScrolled);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*, const Vec2F&, int*>(&type, "GetItemUnderPoint", &o2::UICustomList::GetItemUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	TypeInitializer::RegFuncParam<int*>(funcInfo, "idxPtr");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const DataNode&>(&type, "OnDeserialized", &o2::UICustomList::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "UpdateTransparency", &o2::UICustomList::UpdateTransparency);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Vec2F&>(&type, "UpdateHover", &o2::UICustomList::UpdateHover);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, int, UIWidget*>(&type, "UpdateSelection", &o2::UICustomList::UpdateSelection);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "OnSelectionChanged", &o2::UICustomList::OnSelectionChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "OnVisibleChanged", &o2::UICustomList::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "InitializeProperties", &o2::UICustomList::InitializeProperties);
}

void o2::UIDropDown::InitializeType(o2::UIDropDown* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "textItem", (size_t)(char*)(&sample->textItem) - (size_t)(char*)sample, sample->textItem);
	TypeInitializer::RegField(&type, "onSelectedText", (size_t)(char*)(&sample->onSelectedText) - (size_t)(char*)sample, sample->onSelectedText);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, int, const WString&>(&type, "AddItem", &o2::UIDropDown::AddItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, int, const WString&, int>(&type, "AddItem", &o2::UIDropDown::AddItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, void, const Vector<WString>&>(&type, "AddItems", &o2::UIDropDown::AddItems);
	TypeInitializer::RegFuncParam<const Vector<WString>&>(funcInfo, "data");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, void, const WString&>(&type, "RemoveItem", &o2::UIDropDown::RemoveItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, int, const WString&>(&type, "FindItem", &o2::UIDropDown::FindItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, WString, int>(&type, "GetItemText", &o2::UIDropDown::GetItemText);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, Vector<WString>>(&type, "GetAllItemsText", &o2::UIDropDown::GetAllItemsText);
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, WString>(&type, "GetSelectedItemText", &o2::UIDropDown::GetSelectedItemText);
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, void, const WString&>(&type, "SelectItemText", &o2::UIDropDown::SelectItemText);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, void>(&type, "OnSelectionChanged", &o2::UIDropDown::OnSelectionChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, void>(&type, "InitializeProperties", &o2::UIDropDown::InitializeProperties);
}

void o2::UIEditBox::InitializeType(o2::UIEditBox* sample)
{
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)sample, sample->text);
	TypeInitializer::RegField(&type, "caret", (size_t)(char*)(&sample->caret) - (size_t)(char*)sample, sample->caret);
	TypeInitializer::RegField(&type, "selectionBegin", (size_t)(char*)(&sample->selectionBegin) - (size_t)(char*)sample, sample->selectionBegin);
	TypeInitializer::RegField(&type, "selectionEnd", (size_t)(char*)(&sample->selectionEnd) - (size_t)(char*)sample, sample->selectionEnd);
	TypeInitializer::RegField(&type, "onChanged", (size_t)(char*)(&sample->onChanged) - (size_t)(char*)sample, sample->onChanged);
	TypeInitializer::RegField(&type, "onChangeCompleted", (size_t)(char*)(&sample->onChangeCompleted) - (size_t)(char*)sample, sample->onChangeCompleted);
	TypeInitializer::RegField(&type, "mLastText", (size_t)(char*)(&sample->mLastText) - (size_t)(char*)sample, sample->mLastText);
	TypeInitializer::RegField(&type, "mText", (size_t)(char*)(&sample->mText) - (size_t)(char*)sample, sample->mText).AddAttribute<SerializableAttribute<decltype(mText)>>();
	TypeInitializer::RegField(&type, "mAvailableSymbols", (size_t)(char*)(&sample->mAvailableSymbols) - (size_t)(char*)sample, sample->mAvailableSymbols).AddAttribute<SerializableAttribute<decltype(mAvailableSymbols)>>();
	TypeInitializer::RegField(&type, "mTextDrawable", (size_t)(char*)(&sample->mTextDrawable) - (size_t)(char*)sample, sample->mTextDrawable).AddAttribute<SerializableAttribute<decltype(mTextDrawable)>>();
	TypeInitializer::RegField(&type, "mSelectionMesh", (size_t)(char*)(&sample->mSelectionMesh) - (size_t)(char*)sample, sample->mSelectionMesh);
	TypeInitializer::RegField(&type, "mCaretDrawable", (size_t)(char*)(&sample->mCaretDrawable) - (size_t)(char*)sample, sample->mCaretDrawable).AddAttribute<SerializableAttribute<decltype(mCaretDrawable)>>();
	TypeInitializer::RegField(&type, "mCaretBlinkDelay", (size_t)(char*)(&sample->mCaretBlinkDelay) - (size_t)(char*)sample, sample->mCaretBlinkDelay).AddAttribute<SerializableAttribute<decltype(mCaretBlinkDelay)>>();
	TypeInitializer::RegField(&type, "mCaretBlinkTime", (size_t)(char*)(&sample->mCaretBlinkTime) - (size_t)(char*)sample, sample->mCaretBlinkTime);
	TypeInitializer::RegField(&type, "mSelectionBegin", (size_t)(char*)(&sample->mSelectionBegin) - (size_t)(char*)sample, sample->mSelectionBegin);
	TypeInitializer::RegField(&type, "mSelectionEnd", (size_t)(char*)(&sample->mSelectionEnd) - (size_t)(char*)sample, sample->mSelectionEnd);
	TypeInitializer::RegField(&type, "mSelectionColor", (size_t)(char*)(&sample->mSelectionColor) - (size_t)(char*)sample, sample->mSelectionColor).AddAttribute<SerializableAttribute<decltype(mSelectionColor)>>();
	TypeInitializer::RegField(&type, "mSelectingByWords", (size_t)(char*)(&sample->mSelectingByWords) - (size_t)(char*)sample, sample->mSelectingByWords);
	TypeInitializer::RegField(&type, "mSelWordBegin", (size_t)(char*)(&sample->mSelWordBegin) - (size_t)(char*)sample, sample->mSelWordBegin);
	TypeInitializer::RegField(&type, "mSelWordEnd", (size_t)(char*)(&sample->mSelWordEnd) - (size_t)(char*)sample, sample->mSelWordEnd);
	TypeInitializer::RegField(&type, "mMultiLine", (size_t)(char*)(&sample->mMultiLine) - (size_t)(char*)sample, sample->mMultiLine).AddAttribute<SerializableAttribute<decltype(mMultiLine)>>();
	TypeInitializer::RegField(&type, "mWordWrap", (size_t)(char*)(&sample->mWordWrap) - (size_t)(char*)sample, sample->mWordWrap).AddAttribute<SerializableAttribute<decltype(mWordWrap)>>();
	TypeInitializer::RegField(&type, "mMaxLineChars", (size_t)(char*)(&sample->mMaxLineChars) - (size_t)(char*)sample, sample->mMaxLineChars).AddAttribute<SerializableAttribute<decltype(mMaxLineChars)>>();
	TypeInitializer::RegField(&type, "mMaxLinesCount", (size_t)(char*)(&sample->mMaxLinesCount) - (size_t)(char*)sample, sample->mMaxLinesCount).AddAttribute<SerializableAttribute<decltype(mMaxLinesCount)>>();
	TypeInitializer::RegField(&type, "mDrawDepth", (size_t)(char*)(&sample->mDrawDepth) - (size_t)(char*)sample, sample->mDrawDepth);
	TypeInitializer::RegField(&type, "mLastClickTime", (size_t)(char*)(&sample->mLastClickTime) - (size_t)(char*)sample, sample->mLastClickTime);
	TypeInitializer::RegField(&type, "mLastCursorPos", (size_t)(char*)(&sample->mLastCursorPos) - (size_t)(char*)sample, sample->mLastCursorPos);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "Draw", &o2::UIEditBox::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "Update", &o2::UIEditBox::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const WString&>(&type, "SetText", &o2::UIEditBox::SetText);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, WString>(&type, "GetText", &o2::UIEditBox::GetText);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int>(&type, "SetCaretPosition", &o2::UIEditBox::SetCaretPosition);
	TypeInitializer::RegFuncParam<int>(funcInfo, "caretPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int>(&type, "GetCaretPosition", &o2::UIEditBox::GetCaretPosition);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int, int>(&type, "Select", &o2::UIEditBox::Select);
	TypeInitializer::RegFuncParam<int>(funcInfo, "begin");
	TypeInitializer::RegFuncParam<int>(funcInfo, "end");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int>(&type, "SetSelectionBegin", &o2::UIEditBox::SetSelectionBegin);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int>(&type, "GetSelectionBegin", &o2::UIEditBox::GetSelectionBegin);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int>(&type, "SetSelectionEnd", &o2::UIEditBox::SetSelectionEnd);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int>(&type, "GetSelectionEnd", &o2::UIEditBox::GetSelectionEnd);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "Deselect", &o2::UIEditBox::Deselect);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "SelectAll", &o2::UIEditBox::SelectAll);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, Text*>(&type, "GetTextDrawable", &o2::UIEditBox::GetTextDrawable);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, Sprite*>(&type, "GetCaretDrawable", &o2::UIEditBox::GetCaretDrawable);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Color4&>(&type, "SetSelectionColor", &o2::UIEditBox::SetSelectionColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, Color4>(&type, "GetSelectionColor", &o2::UIEditBox::GetSelectionColor);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "SetFilterInteger", &o2::UIEditBox::SetFilterInteger);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "SetFilterFloat", &o2::UIEditBox::SetFilterFloat);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "SetFilterNames", &o2::UIEditBox::SetFilterNames);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const WString&>(&type, "SetAvailableSymbols", &o2::UIEditBox::SetAvailableSymbols);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "availableSymbols");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, WString>(&type, "GetAvailableSymbols", &o2::UIEditBox::GetAvailableSymbols);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int, int>(&type, "SetMaxSizes", &o2::UIEditBox::SetMaxSizes);
	TypeInitializer::RegFuncParam<int>(funcInfo, "maxLineCharactersCount");
	TypeInitializer::RegFuncParam<int>(funcInfo, "maxLinesCount");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int>(&type, "SetMaxLineCharactersCount", &o2::UIEditBox::SetMaxLineCharactersCount);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int>(&type, "GetMaxLineCharactersCount", &o2::UIEditBox::GetMaxLineCharactersCount);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int>(&type, "SetMaxLinesCount", &o2::UIEditBox::SetMaxLinesCount);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int>(&type, "GetMaxLinesCount", &o2::UIEditBox::GetMaxLinesCount);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, bool>(&type, "SetMultiLine", &o2::UIEditBox::SetMultiLine);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "multiline");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, bool>(&type, "IsMultiLine", &o2::UIEditBox::IsMultiLine);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, bool>(&type, "SetWordWrap", &o2::UIEditBox::SetWordWrap);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "wordWrap");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, bool>(&type, "IsWordWrap", &o2::UIEditBox::IsWordWrap);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "SetCaretBlinkingDelay", &o2::UIEditBox::SetCaretBlinkingDelay);
	TypeInitializer::RegFuncParam<float>(funcInfo, "delay");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, float>(&type, "GetCaretBlinkingDelay", &o2::UIEditBox::GetCaretBlinkingDelay);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, bool>(&type, "IsScrollable", &o2::UIEditBox::IsScrollable);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, bool>(&type, "IsSelectable", &o2::UIEditBox::IsSelectable);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "UpdateControls", &o2::UIEditBox::UpdateControls);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "OnVisibleChanged", &o2::UIEditBox::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "OnDeselected", &o2::UIEditBox::OnDeselected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIEditBox::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIEditBox::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIEditBox::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIEditBox::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIEditBox::OnCursorEnter);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIEditBox::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorRightMousePressed", &o2::UIEditBox::OnCursorRightMousePressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorRightMouseStayDown", &o2::UIEditBox::OnCursorRightMouseStayDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &o2::UIEditBox::OnCursorRightMouseReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "OnScrolled", &o2::UIEditBox::OnScrolled);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Key&>(&type, "OnKeyPressed", &o2::UIEditBox::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Key&>(&type, "OnKeyReleased", &o2::UIEditBox::OnKeyReleased);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Key&>(&type, "OnKeyStayDown", &o2::UIEditBox::OnKeyStayDown);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, WString, const WString&>(&type, "GetFilteredText", &o2::UIEditBox::GetFilteredText);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "UpdateScrollParams", &o2::UIEditBox::UpdateScrollParams);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, bool>(&type, "UpdateLayout", &o2::UIEditBox::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "UpdateTransparency", &o2::UIEditBox::UpdateTransparency);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "CheckCharactersAndLinesBounds", &o2::UIEditBox::CheckCharactersAndLinesBounds);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "UpdateSelectionAndCaret", &o2::UIEditBox::UpdateSelectionAndCaret);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, Vec2F, int>(&type, "GetTextCaretPosition", &o2::UIEditBox::GetTextCaretPosition);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int, const Vec2F&>(&type, "GetTextCaretPosition", &o2::UIEditBox::GetTextCaretPosition);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "UpdateCaretBlinking", &o2::UIEditBox::UpdateCaretBlinking);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const RectF&>(&type, "AddSelectionRect", &o2::UIEditBox::AddSelectionRect);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "CheckScrollingToCaret", &o2::UIEditBox::CheckScrollingToCaret);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, bool, bool>(&type, "JumpSelection", &o2::UIEditBox::JumpSelection);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forward");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "selecting");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, KeyboardKey>(&type, "CheckCharacterTyping", &o2::UIEditBox::CheckCharacterTyping);
	TypeInitializer::RegFuncParam<KeyboardKey>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, KeyboardKey>(&type, "CheckCharactersErasing", &o2::UIEditBox::CheckCharactersErasing);
	TypeInitializer::RegFuncParam<KeyboardKey>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, KeyboardKey>(&type, "CheckCaretMoving", &o2::UIEditBox::CheckCaretMoving);
	TypeInitializer::RegFuncParam<KeyboardKey>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, KeyboardKey>(&type, "CheckClipboard", &o2::UIEditBox::CheckClipboard);
	TypeInitializer::RegFuncParam<KeyboardKey>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int, bool>(&type, "MoveCaret", &o2::UIEditBox::MoveCaret);
	TypeInitializer::RegFuncParam<int>(funcInfo, "newPosition");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "selecting");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "InitializeProperties", &o2::UIEditBox::InitializeProperties);
}

void o2::UIGridLayout::InitializeType(o2::UIGridLayout* sample)
{
	TypeInitializer::RegField(&type, "baseCorner", (size_t)(char*)(&sample->baseCorner) - (size_t)(char*)sample, sample->baseCorner);
	TypeInitializer::RegField(&type, "cellSize", (size_t)(char*)(&sample->cellSize) - (size_t)(char*)sample, sample->cellSize);
	TypeInitializer::RegField(&type, "arrangeAxisMaxCells", (size_t)(char*)(&sample->arrangeAxisMaxCells) - (size_t)(char*)sample, sample->arrangeAxisMaxCells);
	TypeInitializer::RegField(&type, "arrangeAxis", (size_t)(char*)(&sample->arrangeAxis) - (size_t)(char*)sample, sample->arrangeAxis);
	TypeInitializer::RegField(&type, "spacing", (size_t)(char*)(&sample->spacing) - (size_t)(char*)sample, sample->spacing);
	TypeInitializer::RegField(&type, "border", (size_t)(char*)(&sample->border) - (size_t)(char*)sample, sample->border);
	TypeInitializer::RegField(&type, "borderLeft", (size_t)(char*)(&sample->borderLeft) - (size_t)(char*)sample, sample->borderLeft);
	TypeInitializer::RegField(&type, "borderRight", (size_t)(char*)(&sample->borderRight) - (size_t)(char*)sample, sample->borderRight);
	TypeInitializer::RegField(&type, "borderTop", (size_t)(char*)(&sample->borderTop) - (size_t)(char*)sample, sample->borderTop);
	TypeInitializer::RegField(&type, "borderBottom", (size_t)(char*)(&sample->borderBottom) - (size_t)(char*)sample, sample->borderBottom);
	TypeInitializer::RegField(&type, "fitByChildren", (size_t)(char*)(&sample->fitByChildren) - (size_t)(char*)sample, sample->fitByChildren);
	TypeInitializer::RegField(&type, "mBaseCorner", (size_t)(char*)(&sample->mBaseCorner) - (size_t)(char*)sample, sample->mBaseCorner);
	TypeInitializer::RegField(&type, "mSpacing", (size_t)(char*)(&sample->mSpacing) - (size_t)(char*)sample, sample->mSpacing);
	TypeInitializer::RegField(&type, "mCellSize", (size_t)(char*)(&sample->mCellSize) - (size_t)(char*)sample, sample->mCellSize);
	TypeInitializer::RegField(&type, "mArrangeAxis", (size_t)(char*)(&sample->mArrangeAxis) - (size_t)(char*)sample, sample->mArrangeAxis);
	TypeInitializer::RegField(&type, "mArrangeAxisMaxCells", (size_t)(char*)(&sample->mArrangeAxisMaxCells) - (size_t)(char*)sample, sample->mArrangeAxisMaxCells);
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)sample, sample->mBorder);
	TypeInitializer::RegField(&type, "mFitByChildren", (size_t)(char*)(&sample->mFitByChildren) - (size_t)(char*)sample, sample->mFitByChildren);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, BaseCorner>(&type, "SetBaseCorner", &o2::UIGridLayout::SetBaseCorner);
	TypeInitializer::RegFuncParam<BaseCorner>(funcInfo, "baseCorner");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, BaseCorner>(&type, "GetBaseCorner", &o2::UIGridLayout::GetBaseCorner);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, float>(&type, "SetSpacing", &o2::UIGridLayout::SetSpacing);
	TypeInitializer::RegFuncParam<float>(funcInfo, "spacing");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, float>(&type, "GetSpacing", &o2::UIGridLayout::GetSpacing);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, const RectF&>(&type, "SetBorder", &o2::UIGridLayout::SetBorder);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, RectF>(&type, "GetBorder", &o2::UIGridLayout::GetBorder);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, float>(&type, "SetBorderLeft", &o2::UIGridLayout::SetBorderLeft);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, float>(&type, "GetBorderLeft", &o2::UIGridLayout::GetBorderLeft);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, float>(&type, "SetBorderRight", &o2::UIGridLayout::SetBorderRight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, float>(&type, "GetBorderRight", &o2::UIGridLayout::GetBorderRight);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, float>(&type, "SetBorderTop", &o2::UIGridLayout::SetBorderTop);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, float>(&type, "GetBorderTop", &o2::UIGridLayout::GetBorderTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, float>(&type, "SetBorderBottom", &o2::UIGridLayout::SetBorderBottom);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, float>(&type, "GetBorderBottom", &o2::UIGridLayout::GetBorderBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, const Vec2F&>(&type, "SetCellSize", &o2::UIGridLayout::SetCellSize);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, Vec2F>(&type, "GetCellSize", &o2::UIGridLayout::GetCellSize);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, TwoDirection>(&type, "SetArrangeAxis", &o2::UIGridLayout::SetArrangeAxis);
	TypeInitializer::RegFuncParam<TwoDirection>(funcInfo, "type");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, TwoDirection>(&type, "GetArrangeAxis", &o2::UIGridLayout::GetArrangeAxis);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, int>(&type, "SetArrangeAxisMaxCells", &o2::UIGridLayout::SetArrangeAxisMaxCells);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, int>(&type, "GetArrangeAxisMaxCells", &o2::UIGridLayout::GetArrangeAxisMaxCells);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, bool>(&type, "SetFitByChildren", &o2::UIGridLayout::SetFitByChildren);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "fit");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, bool>(&type, "IsFittingByChildren", &o2::UIGridLayout::IsFittingByChildren);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, bool>(&type, "UpdateLayout", &o2::UIGridLayout::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, UIWidget*>(&type, "OnChildAdded", &o2::UIGridLayout::OnChildAdded);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, UIWidget*>(&type, "OnChildRemoved", &o2::UIGridLayout::OnChildRemoved);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "RearrangeChilds", &o2::UIGridLayout::RearrangeChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromLeftTop", &o2::UIGridLayout::ArrangeFromLeftTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromTop", &o2::UIGridLayout::ArrangeFromTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromRightTop", &o2::UIGridLayout::ArrangeFromRightTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromLeft", &o2::UIGridLayout::ArrangeFromLeft);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromCenter", &o2::UIGridLayout::ArrangeFromCenter);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromRight", &o2::UIGridLayout::ArrangeFromRight);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromLeftBottom", &o2::UIGridLayout::ArrangeFromLeftBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromBottom", &o2::UIGridLayout::ArrangeFromBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromRightBottom", &o2::UIGridLayout::ArrangeFromRightBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ExpandSizeByChilds", &o2::UIGridLayout::ExpandSizeByChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "InitializeProperties", &o2::UIGridLayout::InitializeProperties);
}

void o2::UIHorizontalLayout::InitializeType(o2::UIHorizontalLayout* sample)
{
	TypeInitializer::RegField(&type, "baseCorner", (size_t)(char*)(&sample->baseCorner) - (size_t)(char*)sample, sample->baseCorner);
	TypeInitializer::RegField(&type, "spacing", (size_t)(char*)(&sample->spacing) - (size_t)(char*)sample, sample->spacing);
	TypeInitializer::RegField(&type, "border", (size_t)(char*)(&sample->border) - (size_t)(char*)sample, sample->border);
	TypeInitializer::RegField(&type, "borderLeft", (size_t)(char*)(&sample->borderLeft) - (size_t)(char*)sample, sample->borderLeft);
	TypeInitializer::RegField(&type, "borderRight", (size_t)(char*)(&sample->borderRight) - (size_t)(char*)sample, sample->borderRight);
	TypeInitializer::RegField(&type, "borderTop", (size_t)(char*)(&sample->borderTop) - (size_t)(char*)sample, sample->borderTop);
	TypeInitializer::RegField(&type, "borderBottom", (size_t)(char*)(&sample->borderBottom) - (size_t)(char*)sample, sample->borderBottom);
	TypeInitializer::RegField(&type, "expandWidth", (size_t)(char*)(&sample->expandWidth) - (size_t)(char*)sample, sample->expandWidth);
	TypeInitializer::RegField(&type, "expandHeight", (size_t)(char*)(&sample->expandHeight) - (size_t)(char*)sample, sample->expandHeight);
	TypeInitializer::RegField(&type, "fitByChildren", (size_t)(char*)(&sample->fitByChildren) - (size_t)(char*)sample, sample->fitByChildren);
	TypeInitializer::RegField(&type, "mBaseCorner", (size_t)(char*)(&sample->mBaseCorner) - (size_t)(char*)sample, sample->mBaseCorner);
	TypeInitializer::RegField(&type, "mSpacing", (size_t)(char*)(&sample->mSpacing) - (size_t)(char*)sample, sample->mSpacing);
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)sample, sample->mBorder);
	TypeInitializer::RegField(&type, "mExpandWidth", (size_t)(char*)(&sample->mExpandWidth) - (size_t)(char*)sample, sample->mExpandWidth);
	TypeInitializer::RegField(&type, "mExpandHeight", (size_t)(char*)(&sample->mExpandHeight) - (size_t)(char*)sample, sample->mExpandHeight);
	TypeInitializer::RegField(&type, "mFitByChildren", (size_t)(char*)(&sample->mFitByChildren) - (size_t)(char*)sample, sample->mFitByChildren);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, BaseCorner>(&type, "SetBaseCorner", &o2::UIHorizontalLayout::SetBaseCorner);
	TypeInitializer::RegFuncParam<BaseCorner>(funcInfo, "baseCorner");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, BaseCorner>(&type, "GetBaseCorner", &o2::UIHorizontalLayout::GetBaseCorner);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, float>(&type, "SetSpacing", &o2::UIHorizontalLayout::SetSpacing);
	TypeInitializer::RegFuncParam<float>(funcInfo, "spacing");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, float>(&type, "GetSpacing", &o2::UIHorizontalLayout::GetSpacing);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, const RectF&>(&type, "SetBorder", &o2::UIHorizontalLayout::SetBorder);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, RectF>(&type, "GetBorder", &o2::UIHorizontalLayout::GetBorder);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, float>(&type, "SetBorderLeft", &o2::UIHorizontalLayout::SetBorderLeft);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, float>(&type, "GetBorderLeft", &o2::UIHorizontalLayout::GetBorderLeft);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, float>(&type, "SetBorderRight", &o2::UIHorizontalLayout::SetBorderRight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, float>(&type, "GetBorderRight", &o2::UIHorizontalLayout::GetBorderRight);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, float>(&type, "SetBorderTop", &o2::UIHorizontalLayout::SetBorderTop);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, float>(&type, "GetBorderTop", &o2::UIHorizontalLayout::GetBorderTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, float>(&type, "SetBorderBottom", &o2::UIHorizontalLayout::SetBorderBottom);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, float>(&type, "GetBorderBottom", &o2::UIHorizontalLayout::GetBorderBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, bool>(&type, "SetWidthExpand", &o2::UIHorizontalLayout::SetWidthExpand);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expand");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, bool>(&type, "IsWidthExpand", &o2::UIHorizontalLayout::IsWidthExpand);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, bool>(&type, "SetHeightExpand", &o2::UIHorizontalLayout::SetHeightExpand);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expand");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, bool>(&type, "IsHeightExpand", &o2::UIHorizontalLayout::IsHeightExpand);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, bool>(&type, "SetFitByChildren", &o2::UIHorizontalLayout::SetFitByChildren);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "fit");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, bool>(&type, "IsFittingByChildren", &o2::UIHorizontalLayout::IsFittingByChildren);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, bool>(&type, "UpdateLayout", &o2::UIHorizontalLayout::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, UIWidget*>(&type, "OnChildAdded", &o2::UIHorizontalLayout::OnChildAdded);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, UIWidget*>(&type, "OnChildRemoved", &o2::UIHorizontalLayout::OnChildRemoved);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "RearrangeChilds", &o2::UIHorizontalLayout::RearrangeChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "ArrangeFromLeftToRight", &o2::UIHorizontalLayout::ArrangeFromLeftToRight);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "ArrangeFromRightToLeft", &o2::UIHorizontalLayout::ArrangeFromRightToLeft);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "ArrangeFromCenter", &o2::UIHorizontalLayout::ArrangeFromCenter);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "ExpandSizeByChilds", &o2::UIHorizontalLayout::ExpandSizeByChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, Vector<float>>(&type, "CalculateExpandedWidths", &o2::UIHorizontalLayout::CalculateExpandedWidths);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, UIWidget*, float>(&type, "AlignWidgetByHeight", &o2::UIHorizontalLayout::AlignWidgetByHeight);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	TypeInitializer::RegFuncParam<float>(funcInfo, "widthAnchor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "UpdateLayoutParametres", &o2::UIHorizontalLayout::UpdateLayoutParametres);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "InitializeProperties", &o2::UIHorizontalLayout::InitializeProperties);
}

void o2::UIHorizontalProgress::InitializeType(o2::UIHorizontalProgress* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)sample, sample->minValue);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)sample, sample->maxValue);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)sample, sample->scrollSense);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)sample, sample->onChange);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue).AddAttribute<SerializableAttribute<decltype(mValue)>>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)sample, sample->mSmoothValue);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)sample, sample->mMinValue).AddAttribute<SerializableAttribute<decltype(mMinValue)>>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)sample, sample->mMaxValue).AddAttribute<SerializableAttribute<decltype(mMaxValue)>>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)sample, sample->mScrollSense).AddAttribute<SerializableAttribute<decltype(mScrollSense)>>();
	TypeInitializer::RegField(&type, "mOrientation", (size_t)(char*)(&sample->mOrientation) - (size_t)(char*)sample, sample->mOrientation).AddAttribute<SerializableAttribute<decltype(mOrientation)>>();
	TypeInitializer::RegField(&type, "mBarLayer", (size_t)(char*)(&sample->mBarLayer) - (size_t)(char*)sample, sample->mBarLayer);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)sample, sample->mBackLayer);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "Update", &o2::UIHorizontalProgress::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "SetValue", &o2::UIHorizontalProgress::SetValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "SetValueForcible", &o2::UIHorizontalProgress::SetValueForcible);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, float>(&type, "GetValue", &o2::UIHorizontalProgress::GetValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "SetMinValue", &o2::UIHorizontalProgress::SetMinValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, float>(&type, "GetMinValue", &o2::UIHorizontalProgress::GetMinValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "SetMaxValue", &o2::UIHorizontalProgress::SetMaxValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, float>(&type, "GetMaxValue", &o2::UIHorizontalProgress::GetMaxValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float, float>(&type, "SetValueRange", &o2::UIHorizontalProgress::SetValueRange);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "SetScrollSense", &o2::UIHorizontalProgress::SetScrollSense);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, float>(&type, "GetScrollSense", &o2::UIHorizontalProgress::GetScrollSense);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, Orientation>(&type, "SetOrientation", &o2::UIHorizontalProgress::SetOrientation);
	TypeInitializer::RegFuncParam<Orientation>(funcInfo, "orientation");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, Orientation>(&type, "GetOrientation", &o2::UIHorizontalProgress::GetOrientation);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIHorizontalProgress::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, bool>(&type, "IsScrollable", &o2::UIHorizontalProgress::IsScrollable);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, bool>(&type, "UpdateLayout", &o2::UIHorizontalProgress::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void>(&type, "UpdateProgressLayersLayouts", &o2::UIHorizontalProgress::UpdateProgressLayersLayouts);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIHorizontalProgress::OnLayerAdded);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "GetValueFromCursor", &o2::UIHorizontalProgress::GetValueFromCursor);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIHorizontalProgress::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIHorizontalProgress::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIHorizontalProgress::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIHorizontalProgress::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIHorizontalProgress::OnCursorEnter);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIHorizontalProgress::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "OnScrolled", &o2::UIHorizontalProgress::OnScrolled);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const DataNode&>(&type, "OnDeserialized", &o2::UIHorizontalProgress::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void>(&type, "OnVisibleChanged", &o2::UIHorizontalProgress::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void>(&type, "InitializeProperties", &o2::UIHorizontalProgress::InitializeProperties);
}

void o2::UIHorizontalScrollBar::InitializeType(o2::UIHorizontalScrollBar* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)sample, sample->minValue);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)sample, sample->maxValue);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)sample, sample->scrollSense);
	TypeInitializer::RegField(&type, "scrollSize", (size_t)(char*)(&sample->scrollSize) - (size_t)(char*)sample, sample->scrollSize);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)sample, sample->onChange);
	TypeInitializer::RegField(&type, "onSmoothChange", (size_t)(char*)(&sample->onSmoothChange) - (size_t)(char*)sample, sample->onSmoothChange);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue).AddAttribute<SerializableAttribute<decltype(mValue)>>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)sample, sample->mSmoothValue);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)sample, sample->mMinValue).AddAttribute<SerializableAttribute<decltype(mMinValue)>>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)sample, sample->mMaxValue).AddAttribute<SerializableAttribute<decltype(mMaxValue)>>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)sample, sample->mScrollSense).AddAttribute<SerializableAttribute<decltype(mScrollSense)>>();
	TypeInitializer::RegField(&type, "mScrollHandleSize", (size_t)(char*)(&sample->mScrollHandleSize) - (size_t)(char*)sample, sample->mScrollHandleSize).AddAttribute<SerializableAttribute<decltype(mScrollHandleSize)>>();
	TypeInitializer::RegField(&type, "mScrollhandleMinPxSize", (size_t)(char*)(&sample->mScrollhandleMinPxSize) - (size_t)(char*)sample, sample->mScrollhandleMinPxSize);
	TypeInitializer::RegField(&type, "mPressHandleOffset", (size_t)(char*)(&sample->mPressHandleOffset) - (size_t)(char*)sample, sample->mPressHandleOffset);
	TypeInitializer::RegField(&type, "mHandlePressed", (size_t)(char*)(&sample->mHandlePressed) - (size_t)(char*)sample, sample->mHandlePressed);
	TypeInitializer::RegField(&type, "mHandleLayer", (size_t)(char*)(&sample->mHandleLayer) - (size_t)(char*)sample, sample->mHandleLayer);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)sample, sample->mBackLayer);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "Update", &o2::UIHorizontalScrollBar::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetValue", &o2::UIHorizontalScrollBar::SetValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetValueForcible", &o2::UIHorizontalScrollBar::SetValueForcible);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetValue", &o2::UIHorizontalScrollBar::GetValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetSmoothValue", &o2::UIHorizontalScrollBar::GetSmoothValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetMinValue", &o2::UIHorizontalScrollBar::SetMinValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetMinValue", &o2::UIHorizontalScrollBar::GetMinValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetMaxValue", &o2::UIHorizontalScrollBar::SetMaxValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetMaxValue", &o2::UIHorizontalScrollBar::GetMaxValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float, float>(&type, "SetValueRange", &o2::UIHorizontalScrollBar::SetValueRange);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetScrollSense", &o2::UIHorizontalScrollBar::SetScrollSense);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetScrollSense", &o2::UIHorizontalScrollBar::GetScrollSense);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetScrollHandleSize", &o2::UIHorizontalScrollBar::SetScrollHandleSize);
	TypeInitializer::RegFuncParam<float>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetScrollhandleSize", &o2::UIHorizontalScrollBar::GetScrollhandleSize);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetMinimalScrollhandleSize", &o2::UIHorizontalScrollBar::SetMinimalScrollhandleSize);
	TypeInitializer::RegFuncParam<float>(funcInfo, "pixelSize");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIHorizontalScrollBar::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, bool>(&type, "IsScrollable", &o2::UIHorizontalScrollBar::IsScrollable);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, bool>(&type, "UpdateLayout", &o2::UIHorizontalScrollBar::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void>(&type, "UpdateProgressLayersLayouts", &o2::UIHorizontalScrollBar::UpdateProgressLayersLayouts);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void>(&type, "OnLayoutUpdated", &o2::UIHorizontalScrollBar::OnLayoutUpdated);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIHorizontalScrollBar::OnLayerAdded);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float, const Input::Cursor&>(&type, "GetValueFromCursor", &o2::UIHorizontalScrollBar::GetValueFromCursor);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIHorizontalScrollBar::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIHorizontalScrollBar::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIHorizontalScrollBar::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIHorizontalScrollBar::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIHorizontalScrollBar::OnCursorEnter);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIHorizontalScrollBar::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "OnScrolled", &o2::UIHorizontalScrollBar::OnScrolled);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const DataNode&>(&type, "OnDeserialized", &o2::UIHorizontalScrollBar::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void>(&type, "OnVisibleChanged", &o2::UIHorizontalScrollBar::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void>(&type, "InitializeProperties", &o2::UIHorizontalScrollBar::InitializeProperties);
}

void o2::UILabel::InitializeType(o2::UILabel* sample)
{
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)sample, sample->font);
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)sample, sample->text);
	TypeInitializer::RegField(&type, "verAlign", (size_t)(char*)(&sample->verAlign) - (size_t)(char*)sample, sample->verAlign);
	TypeInitializer::RegField(&type, "horAlign", (size_t)(char*)(&sample->horAlign) - (size_t)(char*)sample, sample->horAlign);
	TypeInitializer::RegField(&type, "horOverflow", (size_t)(char*)(&sample->horOverflow) - (size_t)(char*)sample, sample->horOverflow);
	TypeInitializer::RegField(&type, "verOverflow", (size_t)(char*)(&sample->verOverflow) - (size_t)(char*)sample, sample->verOverflow);
	TypeInitializer::RegField(&type, "expandBorder", (size_t)(char*)(&sample->expandBorder) - (size_t)(char*)sample, sample->expandBorder);
	TypeInitializer::RegField(&type, "symbolsDistanceCoef", (size_t)(char*)(&sample->symbolsDistanceCoef) - (size_t)(char*)sample, sample->symbolsDistanceCoef);
	TypeInitializer::RegField(&type, "linesDistanceCoef", (size_t)(char*)(&sample->linesDistanceCoef) - (size_t)(char*)sample, sample->linesDistanceCoef);
	TypeInitializer::RegField(&type, "mTextLayer", (size_t)(char*)(&sample->mTextLayer) - (size_t)(char*)sample, sample->mTextLayer);
	TypeInitializer::RegField(&type, "mHorOverflow", (size_t)(char*)(&sample->mHorOverflow) - (size_t)(char*)sample, sample->mHorOverflow);
	TypeInitializer::RegField(&type, "mVerOverflow", (size_t)(char*)(&sample->mVerOverflow) - (size_t)(char*)sample, sample->mVerOverflow);
	TypeInitializer::RegField(&type, "mExpandBorder", (size_t)(char*)(&sample->mExpandBorder) - (size_t)(char*)sample, sample->mExpandBorder);
	auto funcInfo = TypeInitializer::RegFunction<o2::UILabel, void>(&type, "Draw", &o2::UILabel::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, FontRef>(&type, "SetFont", &o2::UILabel::SetFont);
	TypeInitializer::RegFuncParam<FontRef>(funcInfo, "font");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, FontRef>(&type, "GetFont", &o2::UILabel::GetFont);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, const WString&>(&type, "SetText", &o2::UILabel::SetText);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, WString>(&type, "GetText", &o2::UILabel::GetText);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, HorAlign>(&type, "SetHorAlign", &o2::UILabel::SetHorAlign);
	TypeInitializer::RegFuncParam<HorAlign>(funcInfo, "align");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, HorAlign>(&type, "GetHorAlign", &o2::UILabel::GetHorAlign);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, VerAlign>(&type, "SetVerAlign", &o2::UILabel::SetVerAlign);
	TypeInitializer::RegFuncParam<VerAlign>(funcInfo, "align");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, VerAlign>(&type, "GetVerAlign", &o2::UILabel::GetVerAlign);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, HorOverflow>(&type, "SetHorOverflow", &o2::UILabel::SetHorOverflow);
	TypeInitializer::RegFuncParam<HorOverflow>(funcInfo, "overflow");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, HorOverflow>(&type, "GetHorOverflow", &o2::UILabel::GetHorOverflow);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, VerOverflow>(&type, "SetVerOverflow", &o2::UILabel::SetVerOverflow);
	TypeInitializer::RegFuncParam<VerOverflow>(funcInfo, "overflow");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, VerOverflow>(&type, "GetVerOverflow", &o2::UILabel::GetVerOverflow);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, float>(&type, "SetSymbolsDistanceCoef", &o2::UILabel::SetSymbolsDistanceCoef);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, float>(&type, "GetSymbolsDistanceCoef", &o2::UILabel::GetSymbolsDistanceCoef);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, float>(&type, "SetLinesDistanceCoef", &o2::UILabel::SetLinesDistanceCoef);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, float>(&type, "GetLinesDistanceCoef", &o2::UILabel::GetLinesDistanceCoef);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, const Vec2F&>(&type, "SetExpandBorder", &o2::UILabel::SetExpandBorder);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, Vec2F>(&type, "GetExpandBorder", &o2::UILabel::GetExpandBorder);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, bool>(&type, "UpdateLayout", &o2::UILabel::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UILabel::OnLayerAdded);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void>(&type, "InitializeProperties", &o2::UILabel::InitializeProperties);
}

void o2::UIList::InitializeType(o2::UIList* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "textItem", (size_t)(char*)(&sample->textItem) - (size_t)(char*)sample, sample->textItem);
	TypeInitializer::RegField(&type, "onSelectedText", (size_t)(char*)(&sample->onSelectedText) - (size_t)(char*)sample, sample->onSelectedText);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIList, int, const WString&>(&type, "AddItem", &o2::UIList::AddItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, int, const WString&, int>(&type, "AddItem", &o2::UIList::AddItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void, const Vector<WString>&>(&type, "AddItems", &o2::UIList::AddItems);
	TypeInitializer::RegFuncParam<const Vector<WString>&>(funcInfo, "data");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void, const WString&>(&type, "RemoveItem", &o2::UIList::RemoveItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, int, const WString&>(&type, "FindItem", &o2::UIList::FindItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, WString, int>(&type, "GetItemText", &o2::UIList::GetItemText);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, Vector<WString>>(&type, "GetAllItemsText", &o2::UIList::GetAllItemsText);
	funcInfo = TypeInitializer::RegFunction<o2::UIList, WString>(&type, "GetSelectedItemText", &o2::UIList::GetSelectedItemText);
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void, const WString&>(&type, "SelectItemText", &o2::UIList::SelectItemText);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void>(&type, "OnSelectionChanged", &o2::UIList::OnSelectionChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void>(&type, "initializeProperties", &o2::UIList::initializeProperties);
}

void o2::UILongList::InitializeType(o2::UILongList* sample)
{
	TypeInitializer::RegField(&type, "selectedItemPos", (size_t)(char*)(&sample->selectedItemPos) - (size_t)(char*)sample, sample->selectedItemPos);
	TypeInitializer::RegField(&type, "onSelected", (size_t)(char*)(&sample->onSelected) - (size_t)(char*)sample, sample->onSelected);
	TypeInitializer::RegField(&type, "getItemsCountFunc", (size_t)(char*)(&sample->getItemsCountFunc) - (size_t)(char*)sample, sample->getItemsCountFunc);
	TypeInitializer::RegField(&type, "getItemsRangeFunc", (size_t)(char*)(&sample->getItemsRangeFunc) - (size_t)(char*)sample, sample->getItemsRangeFunc);
	TypeInitializer::RegField(&type, "setupItemFunc", (size_t)(char*)(&sample->setupItemFunc) - (size_t)(char*)sample, sample->setupItemFunc);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)sample, sample->mItemSample).AddAttribute<SerializableAttribute<decltype(mItemSample)>>();
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)sample, sample->mSelectionDrawable).AddAttribute<SerializableAttribute<decltype(mSelectionDrawable)>>();
	TypeInitializer::RegField(&type, "mHoverDrawable", (size_t)(char*)(&sample->mHoverDrawable) - (size_t)(char*)sample, sample->mHoverDrawable).AddAttribute<SerializableAttribute<decltype(mHoverDrawable)>>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)sample, sample->mSelectionLayout).AddAttribute<SerializableAttribute<decltype(mSelectionLayout)>>();
	TypeInitializer::RegField(&type, "mHoverLayout", (size_t)(char*)(&sample->mHoverLayout) - (size_t)(char*)sample, sample->mHoverLayout).AddAttribute<SerializableAttribute<decltype(mHoverLayout)>>();
	TypeInitializer::RegField(&type, "mMinVisibleItemIdx", (size_t)(char*)(&sample->mMinVisibleItemIdx) - (size_t)(char*)sample, sample->mMinVisibleItemIdx);
	TypeInitializer::RegField(&type, "mMaxVisibleItemIdx", (size_t)(char*)(&sample->mMaxVisibleItemIdx) - (size_t)(char*)sample, sample->mMaxVisibleItemIdx);
	TypeInitializer::RegField(&type, "mSelectedItem", (size_t)(char*)(&sample->mSelectedItem) - (size_t)(char*)sample, sample->mSelectedItem);
	TypeInitializer::RegField(&type, "mCurrentSelectionRect", (size_t)(char*)(&sample->mCurrentSelectionRect) - (size_t)(char*)sample, sample->mCurrentSelectionRect);
	TypeInitializer::RegField(&type, "mTargetSelectionRect", (size_t)(char*)(&sample->mTargetSelectionRect) - (size_t)(char*)sample, sample->mTargetSelectionRect);
	TypeInitializer::RegField(&type, "mCurrentHoverRect", (size_t)(char*)(&sample->mCurrentHoverRect) - (size_t)(char*)sample, sample->mCurrentHoverRect);
	TypeInitializer::RegField(&type, "mTargetHoverRect", (size_t)(char*)(&sample->mTargetHoverRect) - (size_t)(char*)sample, sample->mTargetHoverRect);
	TypeInitializer::RegField(&type, "mLastHoverCheckCursor", (size_t)(char*)(&sample->mLastHoverCheckCursor) - (size_t)(char*)sample, sample->mLastHoverCheckCursor);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)sample, sample->mLastSelectCheckCursor);
	TypeInitializer::RegField(&type, "mItemsPool", (size_t)(char*)(&sample->mItemsPool) - (size_t)(char*)sample, sample->mItemsPool);
	TypeInitializer::RegField(&type, "mDrawDepth", (size_t)(char*)(&sample->mDrawDepth) - (size_t)(char*)sample, sample->mDrawDepth);
	auto funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, float>(&type, "Update", &o2::UILongList::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "Draw", &o2::UILongList::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, UIWidget*>(&type, "SetItemSample", &o2::UILongList::SetItemSample);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "sample");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, UIWidget*>(&type, "GetItemSample", &o2::UILongList::GetItemSample);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, int>(&type, "SelectItemAt", &o2::UILongList::SelectItemAt);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, int>(&type, "GetSelectedItemPosition", &o2::UILongList::GetSelectedItemPosition);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, Sprite*>(&type, "GetSelectionDrawable", &o2::UILongList::GetSelectionDrawable);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, Sprite*>(&type, "GetHoverDrawable", &o2::UILongList::GetHoverDrawable);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Layout&>(&type, "SetSelectionDrawableLayout", &o2::UILongList::SetSelectionDrawableLayout);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, Layout>(&type, "GetSelectionDrawableLayout", &o2::UILongList::GetSelectionDrawableLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Layout&>(&type, "SetHoverDrawableLayout", &o2::UILongList::SetHoverDrawableLayout);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, Layout>(&type, "GetHoverDrawableLayout", &o2::UILongList::GetHoverDrawableLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, bool>(&type, "IsScrollable", &o2::UILongList::IsScrollable);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, bool>(&type, "OnItemsUpdated", &o2::UILongList::OnItemsUpdated);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "itemsRearranged");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "CalculateScrollArea", &o2::UILongList::CalculateScrollArea);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, float>(&type, "UpdateControls", &o2::UILongList::UpdateControls);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, bool>(&type, "UpdateLayout", &o2::UILongList::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "UpdateVisibleItems", &o2::UILongList::UpdateVisibleItems);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UILongList::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UILongList::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UILongList::OnCursorMoved);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UILongList::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UILongList::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UILongList::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, float>(&type, "OnScrolled", &o2::UILongList::OnScrolled);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, UIWidget*, const Vec2F&, int*>(&type, "GetItemUnderPoint", &o2::UILongList::GetItemUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	TypeInitializer::RegFuncParam<int*>(funcInfo, "idxPtr");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const DataNode&>(&type, "OnDeserialized", &o2::UILongList::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "UpdateTransparency", &o2::UILongList::UpdateTransparency);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Vec2F&>(&type, "UpdateHover", &o2::UILongList::UpdateHover);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, int>(&type, "UpdateSelection", &o2::UILongList::UpdateSelection);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "OnSelectionChanged", &o2::UILongList::OnSelectionChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "OnVisibleChanged", &o2::UILongList::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "InitializeProperties", &o2::UILongList::InitializeProperties);
}

void o2::UIMenuPanel::InitializeType(o2::UIMenuPanel* sample)
{
	TypeInitializer::RegField(&type, "mLayout", (size_t)(char*)(&sample->mLayout) - (size_t)(char*)sample, sample->mLayout);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)sample, sample->mItemSample).AddAttribute<SerializableAttribute<decltype(mItemSample)>>();
	TypeInitializer::RegField(&type, "mClickFunctions", (size_t)(char*)(&sample->mClickFunctions) - (size_t)(char*)sample, sample->mClickFunctions);
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)sample, sample->mSelectionDrawable).AddAttribute<SerializableAttribute<decltype(mSelectionDrawable)>>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)sample, sample->mSelectionLayout).AddAttribute<SerializableAttribute<decltype(mSelectionLayout)>>();
	TypeInitializer::RegField(&type, "mCurrentSelectionRect", (size_t)(char*)(&sample->mCurrentSelectionRect) - (size_t)(char*)sample, sample->mCurrentSelectionRect);
	TypeInitializer::RegField(&type, "mTargetSelectionRect", (size_t)(char*)(&sample->mTargetSelectionRect) - (size_t)(char*)sample, sample->mTargetSelectionRect);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)sample, sample->mLastSelectCheckCursor);
	TypeInitializer::RegField(&type, "mSelectedItem", (size_t)(char*)(&sample->mSelectedItem) - (size_t)(char*)sample, sample->mSelectedItem);
	TypeInitializer::RegField(&type, "mSelectSubContextTime", (size_t)(char*)(&sample->mSelectSubContextTime) - (size_t)(char*)sample, sample->mSelectSubContextTime);
	TypeInitializer::RegField(&type, "mOpenedContext", (size_t)(char*)(&sample->mOpenedContext) - (size_t)(char*)sample, sample->mOpenedContext);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, float>(&type, "Update", &o2::UIMenuPanel::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void>(&type, "Draw", &o2::UIMenuPanel::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*, const Item&>(&type, "AddItem", &o2::UIMenuPanel::AddItem);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*, const WString&, const Function<void()>&, ImageAsset*, const ShortcutKeys&>(&type, "AddItem", &o2::UIMenuPanel::AddItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "path");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "clickFunc");
	TypeInitializer::RegFuncParam<ImageAsset*>(funcInfo, "icon");
	TypeInitializer::RegFuncParam<const ShortcutKeys&>(funcInfo, "shortcut");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*, const Item&, int>(&type, "InsertItem", &o2::UIMenuPanel::InsertItem);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, Vector<Item>>(&type, "AddItems", &o2::UIMenuPanel::AddItems);
	TypeInitializer::RegFuncParam<Vector<Item>>(funcInfo, "items");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, Vector<Item>, int>(&type, "InsertItems", &o2::UIMenuPanel::InsertItems);
	TypeInitializer::RegFuncParam<Vector<Item>>(funcInfo, "items");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, Item, int>(&type, "GetItem", &o2::UIMenuPanel::GetItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, Vector<Item>>(&type, "GetItems", &o2::UIMenuPanel::GetItems);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, int>(&type, "RemoveItem", &o2::UIMenuPanel::RemoveItem);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const WString&>(&type, "RemoveItem", &o2::UIMenuPanel::RemoveItem);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void>(&type, "RemoveAllItems", &o2::UIMenuPanel::RemoveAllItems);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIHorizontalLayout*>(&type, "GetItemsLayout", &o2::UIMenuPanel::GetItemsLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*>(&type, "GetItemSample", &o2::UIMenuPanel::GetItemSample);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, UIWidget*>(&type, "SetItemSample", &o2::UIMenuPanel::SetItemSample);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "sample");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, Sprite*>(&type, "GetSelectionDrawable", &o2::UIMenuPanel::GetSelectionDrawable);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Layout&>(&type, "SetSelectionDrawableLayout", &o2::UIMenuPanel::SetSelectionDrawableLayout);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, Layout>(&type, "GetSelectionDrawableLayout", &o2::UIMenuPanel::GetSelectionDrawableLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*, const Item&>(&type, "CreateItem", &o2::UIMenuPanel::CreateItem);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, Item, int>(&type, "GetItemDef", &o2::UIMenuPanel::GetItemDef);
	TypeInitializer::RegFuncParam<int>(funcInfo, "idx");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void>(&type, "OnVisibleChanged", &o2::UIMenuPanel::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*, const Vec2F&, int*>(&type, "GetItemUnderPoint", &o2::UIMenuPanel::GetItemUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	TypeInitializer::RegFuncParam<int*>(funcInfo, "idxPtr");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Vec2F&>(&type, "UpdateHover", &o2::UIMenuPanel::UpdateHover);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIMenuPanel::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIMenuPanel::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIMenuPanel::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIMenuPanel::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UIMenuPanel::OnCursorMoved);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIMenuPanel::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
}

void o2::UIScrollArea::InitializeType(o2::UIScrollArea* sample)
{
	TypeInitializer::RegField(&type, "scroll", (size_t)(char*)(&sample->scroll) - (size_t)(char*)sample, sample->scroll);
	TypeInitializer::RegField(&type, "horScroll", (size_t)(char*)(&sample->horScroll) - (size_t)(char*)sample, sample->horScroll);
	TypeInitializer::RegField(&type, "verScroll", (size_t)(char*)(&sample->verScroll) - (size_t)(char*)sample, sample->verScroll);
	TypeInitializer::RegField(&type, "onScrolled", (size_t)(char*)(&sample->onScrolled) - (size_t)(char*)sample, sample->onScrolled);
	TypeInitializer::RegField(&type, "mHorScrollBar", (size_t)(char*)(&sample->mHorScrollBar) - (size_t)(char*)sample, sample->mHorScrollBar);
	TypeInitializer::RegField(&type, "mVerScrollBar", (size_t)(char*)(&sample->mVerScrollBar) - (size_t)(char*)sample, sample->mVerScrollBar);
	TypeInitializer::RegField(&type, "mOwnHorScrollBar", (size_t)(char*)(&sample->mOwnHorScrollBar) - (size_t)(char*)sample, sample->mOwnHorScrollBar);
	TypeInitializer::RegField(&type, "mOwnVerScrollBar", (size_t)(char*)(&sample->mOwnVerScrollBar) - (size_t)(char*)sample, sample->mOwnVerScrollBar);
	TypeInitializer::RegField(&type, "mViewAreaLayout", (size_t)(char*)(&sample->mViewAreaLayout) - (size_t)(char*)sample, sample->mViewAreaLayout).AddAttribute<SerializableAttribute<decltype(mViewAreaLayout)>>();
	TypeInitializer::RegField(&type, "mAbsoluteViewArea", (size_t)(char*)(&sample->mAbsoluteViewArea) - (size_t)(char*)sample, sample->mAbsoluteViewArea);
	TypeInitializer::RegField(&type, "mClipAreaLayout", (size_t)(char*)(&sample->mClipAreaLayout) - (size_t)(char*)sample, sample->mClipAreaLayout).AddAttribute<SerializableAttribute<decltype(mClipAreaLayout)>>();
	TypeInitializer::RegField(&type, "mAbsoluteClipArea", (size_t)(char*)(&sample->mAbsoluteClipArea) - (size_t)(char*)sample, sample->mAbsoluteClipArea);
	TypeInitializer::RegField(&type, "mScrollPos", (size_t)(char*)(&sample->mScrollPos) - (size_t)(char*)sample, sample->mScrollPos).AddAttribute<SerializableAttribute<decltype(mScrollPos)>>();
	TypeInitializer::RegField(&type, "mScrollSpeed", (size_t)(char*)(&sample->mScrollSpeed) - (size_t)(char*)sample, sample->mScrollSpeed);
	TypeInitializer::RegField(&type, "mScrollSpeedDamp", (size_t)(char*)(&sample->mScrollSpeedDamp) - (size_t)(char*)sample, sample->mScrollSpeedDamp);
	TypeInitializer::RegField(&type, "mScrollArea", (size_t)(char*)(&sample->mScrollArea) - (size_t)(char*)sample, sample->mScrollArea);
	TypeInitializer::RegField(&type, "mScrollRange", (size_t)(char*)(&sample->mScrollRange) - (size_t)(char*)sample, sample->mScrollRange);
	TypeInitializer::RegField(&type, "mEnableHorScroll", (size_t)(char*)(&sample->mEnableHorScroll) - (size_t)(char*)sample, sample->mEnableHorScroll);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void>(&type, "Draw", &o2::UIScrollArea::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, float>(&type, "Update", &o2::UIScrollArea::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, const Vec2F&>(&type, "SetScroll", &o2::UIScrollArea::SetScroll);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, Vec2F>(&type, "GetScroll", &o2::UIScrollArea::GetScroll);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, RectF>(&type, "GetScrollRange", &o2::UIScrollArea::GetScrollRange);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void>(&type, "ResetSroll", &o2::UIScrollArea::ResetSroll);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, float>(&type, "SetHorizontalScroll", &o2::UIScrollArea::SetHorizontalScroll);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, float>(&type, "GetHorizontalScroll", &o2::UIScrollArea::GetHorizontalScroll);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, float>(&type, "SetVerticalScroll", &o2::UIScrollArea::SetVerticalScroll);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, float>(&type, "GetVerticalScroll", &o2::UIScrollArea::GetVerticalScroll);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, UIHorizontalScrollBar*, bool>(&type, "SetHorizontalScrollBar", &o2::UIScrollArea::SetHorizontalScrollBar);
	TypeInitializer::RegFuncParam<UIHorizontalScrollBar*>(funcInfo, "scrollbar");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "owner");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, UIHorizontalScrollBar*>(&type, "GetHorizontalScrollbar", &o2::UIScrollArea::GetHorizontalScrollbar);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, UIVerticalScrollBar*, bool>(&type, "SetVerticalScrollBar", &o2::UIScrollArea::SetVerticalScrollBar);
	TypeInitializer::RegFuncParam<UIVerticalScrollBar*>(funcInfo, "scrollbar");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "owner");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, UIVerticalScrollBar*>(&type, "GetVerticalScrollbar", &o2::UIScrollArea::GetVerticalScrollbar);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, bool>(&type, "SetEnableScrollsHiding", &o2::UIScrollArea::SetEnableScrollsHiding);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "hideScrolls");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, bool>(&type, "IsScrollsHiding", &o2::UIScrollArea::IsScrollsHiding);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, const Layout&>(&type, "SetClippingLayout", &o2::UIScrollArea::SetClippingLayout);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "clipLayout");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, Layout>(&type, "GetClippingLayout", &o2::UIScrollArea::GetClippingLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, const Layout&>(&type, "SetViewLayout", &o2::UIScrollArea::SetViewLayout);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "viewLayout");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, Layout>(&type, "GetViewLayout", &o2::UIScrollArea::GetViewLayout);
}

void o2::UIToggle::InitializeType(o2::UIToggle* sample)
{
	TypeInitializer::RegField(&type, "caption", (size_t)(char*)(&sample->caption) - (size_t)(char*)sample, sample->caption);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "toggleGroup", (size_t)(char*)(&sample->toggleGroup) - (size_t)(char*)sample, sample->toggleGroup);
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)sample, sample->onClick);
	TypeInitializer::RegField(&type, "onToggle", (size_t)(char*)(&sample->onToggle) - (size_t)(char*)sample, sample->onToggle);
	TypeInitializer::RegField(&type, "onToggleByUser", (size_t)(char*)(&sample->onToggleByUser) - (size_t)(char*)sample, sample->onToggleByUser);
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)sample, sample->shortcut);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue);
	TypeInitializer::RegField(&type, "mCaptionText", (size_t)(char*)(&sample->mCaptionText) - (size_t)(char*)sample, sample->mCaptionText);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)sample, sample->mBackLayer);
	TypeInitializer::RegField(&type, "mToggleGroup", (size_t)(char*)(&sample->mToggleGroup) - (size_t)(char*)sample, sample->mToggleGroup);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, float>(&type, "Update", &o2::UIToggle::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const WString&>(&type, "SetCaption", &o2::UIToggle::SetCaption);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, WString>(&type, "GetCaption", &o2::UIToggle::GetCaption);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, bool>(&type, "SetValue", &o2::UIToggle::SetValue);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, bool>(&type, "GetValue", &o2::UIToggle::GetValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, bool>(&type, "IsSelectable", &o2::UIToggle::IsSelectable);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, UIToggleGroup*>(&type, "SetToggleGroup", &o2::UIToggle::SetToggleGroup);
	TypeInitializer::RegFuncParam<UIToggleGroup*>(funcInfo, "toggleGroup");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, UIToggleGroup*>(&type, "GetToggleGroup", &o2::UIToggle::GetToggleGroup);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIToggle::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIToggle::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIToggle::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIToggle::OnCursorEnter);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIToggle::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Key&>(&type, "OnKeyPressed", &o2::UIToggle::OnKeyPressed);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Key&>(&type, "OnKeyReleased", &o2::UIToggle::OnKeyReleased);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIToggle::OnLayerAdded);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void>(&type, "OnVisibleChanged", &o2::UIToggle::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void>(&type, "InitializeProperties", &o2::UIToggle::InitializeProperties);
}

void o2::UITreeNode::InitializeType(o2::UITreeNode* sample)
{
	TypeInitializer::RegField(&type, "mExpandedState", (size_t)(char*)(&sample->mExpandedState) - (size_t)(char*)sample, sample->mExpandedState);
	TypeInitializer::RegField(&type, "mExpandCoef", (size_t)(char*)(&sample->mExpandCoef) - (size_t)(char*)sample, sample->mExpandCoef);
	TypeInitializer::RegField(&type, "mObject", (size_t)(char*)(&sample->mObject) - (size_t)(char*)sample, sample->mObject);
	TypeInitializer::RegField(&type, "mTree", (size_t)(char*)(&sample->mTree) - (size_t)(char*)sample, sample->mTree);
	TypeInitializer::RegField(&type, "mChildsOffset", (size_t)(char*)(&sample->mChildsOffset) - (size_t)(char*)sample, sample->mChildsOffset).AddAttribute<SerializableAttribute<decltype(mChildsOffset)>>();
	TypeInitializer::RegField(&type, "mInsertSizeCoef", (size_t)(char*)(&sample->mInsertSizeCoef) - (size_t)(char*)sample, sample->mInsertSizeCoef);
	TypeInitializer::RegField(&type, "mDragSizeCoef", (size_t)(char*)(&sample->mDragSizeCoef) - (size_t)(char*)sample, sample->mDragSizeCoef);
	auto funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void>(&type, "Draw", &o2::UITreeNode::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool, bool>(&type, "SetExpanded", &o2::UITreeNode::SetExpanded);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expanded");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, bool>(&type, "IsExpanded", &o2::UITreeNode::IsExpanded);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool>(&type, "Expand", &o2::UITreeNode::Expand);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool>(&type, "Collapse", &o2::UITreeNode::Collapse);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void>(&type, "ExpandAll", &o2::UITreeNode::ExpandAll);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void>(&type, "CollapseAll", &o2::UITreeNode::CollapseAll);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, UITreeNode*, UnknownType*>(&type, "GetNode", &o2::UITreeNode::GetNode);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool>(&type, "Rebuild", &o2::UITreeNode::Rebuild);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChilds");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, float>(&type, "SetChildrenOffset", &o2::UITreeNode::SetChildrenOffset);
	TypeInitializer::RegFuncParam<float>(funcInfo, "offset");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, float>(&type, "GetChildrenOffset", &o2::UITreeNode::GetChildrenOffset);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, UnknownType*>(&type, "GetObject", &o2::UITreeNode::GetObject);
}

void o2::UITree::InitializeType(o2::UITree* sample)
{
	TypeInitializer::RegField(&type, "onDraggedObjects", (size_t)(char*)(&sample->onDraggedObjects) - (size_t)(char*)sample, sample->onDraggedObjects);
	TypeInitializer::RegField(&type, "getParentFunc", (size_t)(char*)(&sample->getParentFunc) - (size_t)(char*)sample, sample->getParentFunc);
	TypeInitializer::RegField(&type, "getChildsFunc", (size_t)(char*)(&sample->getChildsFunc) - (size_t)(char*)sample, sample->getChildsFunc);
	TypeInitializer::RegField(&type, "setupNodeFunc", (size_t)(char*)(&sample->setupNodeFunc) - (size_t)(char*)sample, sample->setupNodeFunc);
	TypeInitializer::RegField(&type, "onItemClick", (size_t)(char*)(&sample->onItemClick) - (size_t)(char*)sample, sample->onItemClick);
	TypeInitializer::RegField(&type, "onItemDblClick", (size_t)(char*)(&sample->onItemDblClick) - (size_t)(char*)sample, sample->onItemDblClick);
	TypeInitializer::RegField(&type, "onItemRBClick", (size_t)(char*)(&sample->onItemRBClick) - (size_t)(char*)sample, sample->onItemRBClick);
	TypeInitializer::RegField(&type, "onItemsSelectionChanged", (size_t)(char*)(&sample->onItemsSelectionChanged) - (size_t)(char*)sample, sample->onItemsSelectionChanged);
	TypeInitializer::RegField(&type, "mNodesPoolResizeCount", (size_t)(char*)(&sample->mNodesPoolResizeCount) - (size_t)(char*)sample, sample->mNodesPoolResizeCount);
	TypeInitializer::RegField(&type, "mSelectionSpritesPoolResizeCount", (size_t)(char*)(&sample->mSelectionSpritesPoolResizeCount) - (size_t)(char*)sample, sample->mSelectionSpritesPoolResizeCount);
	TypeInitializer::RegField(&type, "mSelectedColor", (size_t)(char*)(&sample->mSelectedColor) - (size_t)(char*)sample, sample->mSelectedColor).AddAttribute<SerializableAttribute<decltype(mSelectedColor)>>();
	TypeInitializer::RegField(&type, "mUnselectedColor", (size_t)(char*)(&sample->mUnselectedColor) - (size_t)(char*)sample, sample->mUnselectedColor).AddAttribute<SerializableAttribute<decltype(mUnselectedColor)>>();
	TypeInitializer::RegField(&type, "mHoverColor", (size_t)(char*)(&sample->mHoverColor) - (size_t)(char*)sample, sample->mHoverColor).AddAttribute<SerializableAttribute<decltype(mHoverColor)>>();
	TypeInitializer::RegField(&type, "mAllNodes", (size_t)(char*)(&sample->mAllNodes) - (size_t)(char*)sample, sample->mAllNodes);
	TypeInitializer::RegField(&type, "mNodeSample", (size_t)(char*)(&sample->mNodeSample) - (size_t)(char*)sample, sample->mNodeSample).AddAttribute<SerializableAttribute<decltype(mNodeSample)>>();
	TypeInitializer::RegField(&type, "mHoverDrawable", (size_t)(char*)(&sample->mHoverDrawable) - (size_t)(char*)sample, sample->mHoverDrawable).AddAttribute<SerializableAttribute<decltype(mHoverDrawable)>>();
	TypeInitializer::RegField(&type, "mSelectedDrawable", (size_t)(char*)(&sample->mSelectedDrawable) - (size_t)(char*)sample, sample->mSelectedDrawable).AddAttribute<SerializableAttribute<decltype(mSelectedDrawable)>>();
	TypeInitializer::RegField(&type, "mHoverLayout", (size_t)(char*)(&sample->mHoverLayout) - (size_t)(char*)sample, sample->mHoverLayout).AddAttribute<SerializableAttribute<decltype(mHoverLayout)>>();
	TypeInitializer::RegField(&type, "mCurrentHoverRect", (size_t)(char*)(&sample->mCurrentHoverRect) - (size_t)(char*)sample, sample->mCurrentHoverRect);
	TypeInitializer::RegField(&type, "mTargetHoverRect", (size_t)(char*)(&sample->mTargetHoverRect) - (size_t)(char*)sample, sample->mTargetHoverRect);
	TypeInitializer::RegField(&type, "mLastHoverCheckCursor", (size_t)(char*)(&sample->mLastHoverCheckCursor) - (size_t)(char*)sample, sample->mLastHoverCheckCursor);
	TypeInitializer::RegField(&type, "mPressedPoint", (size_t)(char*)(&sample->mPressedPoint) - (size_t)(char*)sample, sample->mPressedPoint);
	TypeInitializer::RegField(&type, "mHoveredItem", (size_t)(char*)(&sample->mHoveredItem) - (size_t)(char*)sample, sample->mHoveredItem);
	TypeInitializer::RegField(&type, "mSelectedItems", (size_t)(char*)(&sample->mSelectedItems) - (size_t)(char*)sample, sample->mSelectedItems);
	TypeInitializer::RegField(&type, "mWaitSelectionsUpdate", (size_t)(char*)(&sample->mWaitSelectionsUpdate) - (size_t)(char*)sample, sample->mWaitSelectionsUpdate);
	TypeInitializer::RegField(&type, "mNodesPool", (size_t)(char*)(&sample->mNodesPool) - (size_t)(char*)sample, sample->mNodesPool);
	TypeInitializer::RegField(&type, "mSelectionSpritesPool", (size_t)(char*)(&sample->mSelectionSpritesPool) - (size_t)(char*)sample, sample->mSelectionSpritesPool);
	TypeInitializer::RegField(&type, "mExpandedObjects", (size_t)(char*)(&sample->mExpandedObjects) - (size_t)(char*)sample, sample->mExpandedObjects);
	TypeInitializer::RegField(&type, "mRearrangeType", (size_t)(char*)(&sample->mRearrangeType) - (size_t)(char*)sample, sample->mRearrangeType);
	TypeInitializer::RegField(&type, "mMultiSelectAvailable", (size_t)(char*)(&sample->mMultiSelectAvailable) - (size_t)(char*)sample, sample->mMultiSelectAvailable);
	TypeInitializer::RegField(&type, "mDraggingNodes", (size_t)(char*)(&sample->mDraggingNodes) - (size_t)(char*)sample, sample->mDraggingNodes);
	TypeInitializer::RegField(&type, "mDragNode", (size_t)(char*)(&sample->mDragNode) - (size_t)(char*)sample, sample->mDragNode);
	TypeInitializer::RegField(&type, "mDragNodeBack", (size_t)(char*)(&sample->mDragNodeBack) - (size_t)(char*)sample, sample->mDragNodeBack);
	TypeInitializer::RegField(&type, "mDragOffset", (size_t)(char*)(&sample->mDragOffset) - (size_t)(char*)sample, sample->mDragOffset);
	TypeInitializer::RegField(&type, "mPressedNode", (size_t)(char*)(&sample->mPressedNode) - (size_t)(char*)sample, sample->mPressedNode);
	TypeInitializer::RegField(&type, "mInsertNodeCandidate", (size_t)(char*)(&sample->mInsertNodeCandidate) - (size_t)(char*)sample, sample->mInsertNodeCandidate);
	TypeInitializer::RegField(&type, "mUnderCursorItem", (size_t)(char*)(&sample->mUnderCursorItem) - (size_t)(char*)sample, sample->mUnderCursorItem);
	TypeInitializer::RegField(&type, "mExpandNodeCandidate", (size_t)(char*)(&sample->mExpandNodeCandidate) - (size_t)(char*)sample, sample->mExpandNodeCandidate);
	TypeInitializer::RegField(&type, "mExpandInsertTime", (size_t)(char*)(&sample->mExpandInsertTime) - (size_t)(char*)sample, sample->mExpandInsertTime);
	TypeInitializer::RegField(&type, "mPressedTime", (size_t)(char*)(&sample->mPressedTime) - (size_t)(char*)sample, sample->mPressedTime);
	TypeInitializer::RegField(&type, "mDrawDepth", (size_t)(char*)(&sample->mDrawDepth) - (size_t)(char*)sample, sample->mDrawDepth);
	TypeInitializer::RegField(&type, "mNeedUpdateLayout", (size_t)(char*)(&sample->mNeedUpdateLayout) - (size_t)(char*)sample, sample->mNeedUpdateLayout);
	auto funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "Draw", &o2::UITree::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, float>(&type, "Update", &o2::UITree::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "RebuildTree", &o2::UITree::RebuildTree);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*>(&type, "UpdateTreeNode", &o2::UITree::UpdateTreeNode);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*, UnknownType*>(&type, "GetNode", &o2::UITree::GetNode);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "ExpandAll", &o2::UITree::ExpandAll);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "CollapseAll", &o2::UITree::CollapseAll);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Vector<UnknownType*>>(&type, "GetSelectedObjects", &o2::UITree::GetSelectedObjects);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Vector<UnknownType*>&>(&type, "SetSelectedObjects", &o2::UITree::SetSelectedObjects);
	TypeInitializer::RegFuncParam<const Vector<UnknownType*>&>(funcInfo, "objects");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*>(&type, "SelectObject", &o2::UITree::SelectObject);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*>(&type, "SelectAndExpandObject", &o2::UITree::SelectAndExpandObject);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*>(&type, "DeselectObject", &o2::UITree::DeselectObject);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "DeselectAllObjects", &o2::UITree::DeselectAllObjects);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*>(&type, "GetNodeSample", &o2::UITree::GetNodeSample);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Sprite*>(&type, "GetHoverDrawable", &o2::UITree::GetHoverDrawable);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Sprite*>(&type, "GetSelectionDrawable", &o2::UITree::GetSelectionDrawable);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Layout&>(&type, "SetSelectionDrawableLayout", &o2::UITree::SetSelectionDrawableLayout);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Layout>(&type, "GetSelectionDrawableLayout", &o2::UITree::GetSelectionDrawableLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*, UnknownType*>(&type, "OnObjectCreated", &o2::UITree::OnObjectCreated);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "parent");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*>(&type, "OnObjectRemoved", &o2::UITree::OnObjectRemoved);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool>(&type, "IsScrollable", &o2::UITree::IsScrollable);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, int>(&type, "SetNodesPoolResizeCount", &o2::UITree::SetNodesPoolResizeCount);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, int>(&type, "SetSelectionSpritesPoolResizeCount", &o2::UITree::SetSelectionSpritesPoolResizeCount);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, RearrangeType>(&type, "SetRearrangeType", &o2::UITree::SetRearrangeType);
	TypeInitializer::RegFuncParam<RearrangeType>(funcInfo, "type");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, RearrangeType>(&type, "GetRearrangeType", &o2::UITree::GetRearrangeType);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, bool>(&type, "SetMultipleSelectionAvailable", &o2::UITree::SetMultipleSelectionAvailable);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "available");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool>(&type, "IsMultiSelectionAvailable", &o2::UITree::IsMultiSelectionAvailable);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Color4&>(&type, "SetSelectedColor", &o2::UITree::SetSelectedColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Color4>(&type, "GetSelectedColor", &o2::UITree::GetSelectedColor);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Color4&>(&type, "SetUnselectedColor", &o2::UITree::SetUnselectedColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Color4>(&type, "GetUnselectedColor", &o2::UITree::GetUnselectedColor);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Color4&>(&type, "SetHoverColor", &o2::UITree::SetHoverColor);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Color4>(&type, "GetHoverColor", &o2::UITree::GetHoverColor);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool>(&type, "IsSelectable", &o2::UITree::IsSelectable);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "OnSelected", &o2::UITree::OnSelected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "OnDeselected", &o2::UITree::OnDeselected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, bool>(&type, "UpdateLayout", &o2::UITree::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "UpdateScrollParams", &o2::UITree::UpdateScrollParams);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, float>(&type, "GetCurrentHeight", &o2::UITree::GetCurrentHeight);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UITree::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UITree::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UITree::OnCursorMoved);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UITree::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &o2::UITree::OnCursorRightMouseReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "SelectSingleNode", &o2::UITree::SelectSingleNode);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "SelectMultipleNodes", &o2::UITree::SelectMultipleNodes);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "OnItemsSelected", &o2::UITree::OnItemsSelected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UITree::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UITree::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*, const Vec2F&>(&type, "GetItemUnderPoint", &o2::UITree::GetItemUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "UpdateDragging", &o2::UITree::UpdateDragging);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UITreeNode*>(&type, "UpdateHover", &o2::UITree::UpdateHover);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "itemUnderCursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "BeginDragging", &o2::UITree::BeginDragging);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "EndDragging", &o2::UITree::EndDragging);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*>(&type, "CreateTreeNode", &o2::UITree::CreateTreeNode);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UITreeNode*>(&type, "FreeTreeNode", &o2::UITree::FreeTreeNode);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Sprite*>(&type, "CreateSelectionSprite", &o2::UITree::CreateSelectionSprite);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, Sprite*>(&type, "FreeSelectionSprite", &o2::UITree::FreeSelectionSprite);
	TypeInitializer::RegFuncParam<Sprite*>(funcInfo, "sprite");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "CheckSelectedNodes", &o2::UITree::CheckSelectedNodes);
}

void o2::UIVerticalLayout::InitializeType(o2::UIVerticalLayout* sample)
{
	TypeInitializer::RegField(&type, "baseCorner", (size_t)(char*)(&sample->baseCorner) - (size_t)(char*)sample, sample->baseCorner);
	TypeInitializer::RegField(&type, "spacing", (size_t)(char*)(&sample->spacing) - (size_t)(char*)sample, sample->spacing);
	TypeInitializer::RegField(&type, "border", (size_t)(char*)(&sample->border) - (size_t)(char*)sample, sample->border);
	TypeInitializer::RegField(&type, "borderLeft", (size_t)(char*)(&sample->borderLeft) - (size_t)(char*)sample, sample->borderLeft);
	TypeInitializer::RegField(&type, "borderRight", (size_t)(char*)(&sample->borderRight) - (size_t)(char*)sample, sample->borderRight);
	TypeInitializer::RegField(&type, "borderTop", (size_t)(char*)(&sample->borderTop) - (size_t)(char*)sample, sample->borderTop);
	TypeInitializer::RegField(&type, "borderBottom", (size_t)(char*)(&sample->borderBottom) - (size_t)(char*)sample, sample->borderBottom);
	TypeInitializer::RegField(&type, "expandWidth", (size_t)(char*)(&sample->expandWidth) - (size_t)(char*)sample, sample->expandWidth);
	TypeInitializer::RegField(&type, "expandHeight", (size_t)(char*)(&sample->expandHeight) - (size_t)(char*)sample, sample->expandHeight);
	TypeInitializer::RegField(&type, "fitByChildren", (size_t)(char*)(&sample->fitByChildren) - (size_t)(char*)sample, sample->fitByChildren);
	TypeInitializer::RegField(&type, "mBaseCorner", (size_t)(char*)(&sample->mBaseCorner) - (size_t)(char*)sample, sample->mBaseCorner);
	TypeInitializer::RegField(&type, "mSpacing", (size_t)(char*)(&sample->mSpacing) - (size_t)(char*)sample, sample->mSpacing);
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)sample, sample->mBorder);
	TypeInitializer::RegField(&type, "mExpandWidth", (size_t)(char*)(&sample->mExpandWidth) - (size_t)(char*)sample, sample->mExpandWidth);
	TypeInitializer::RegField(&type, "mExpandHeight", (size_t)(char*)(&sample->mExpandHeight) - (size_t)(char*)sample, sample->mExpandHeight);
	TypeInitializer::RegField(&type, "mFitByChildren", (size_t)(char*)(&sample->mFitByChildren) - (size_t)(char*)sample, sample->mFitByChildren);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, BaseCorner>(&type, "SetBaseCorner", &o2::UIVerticalLayout::SetBaseCorner);
	TypeInitializer::RegFuncParam<BaseCorner>(funcInfo, "baseCorner");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, BaseCorner>(&type, "GetBaseCorner", &o2::UIVerticalLayout::GetBaseCorner);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, float>(&type, "SetSpacing", &o2::UIVerticalLayout::SetSpacing);
	TypeInitializer::RegFuncParam<float>(funcInfo, "spacing");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, float>(&type, "GetSpacing", &o2::UIVerticalLayout::GetSpacing);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, const RectF&>(&type, "SetBorder", &o2::UIVerticalLayout::SetBorder);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, RectF>(&type, "GetBorder", &o2::UIVerticalLayout::GetBorder);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, float>(&type, "SetBorderLeft", &o2::UIVerticalLayout::SetBorderLeft);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, float>(&type, "GetBorderLeft", &o2::UIVerticalLayout::GetBorderLeft);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, float>(&type, "SetBorderRight", &o2::UIVerticalLayout::SetBorderRight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, float>(&type, "GetBorderRight", &o2::UIVerticalLayout::GetBorderRight);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, float>(&type, "SetBorderTop", &o2::UIVerticalLayout::SetBorderTop);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, float>(&type, "GetBorderTop", &o2::UIVerticalLayout::GetBorderTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, float>(&type, "SetBorderBottom", &o2::UIVerticalLayout::SetBorderBottom);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, float>(&type, "GetBorderBottom", &o2::UIVerticalLayout::GetBorderBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, bool>(&type, "SetWidthExpand", &o2::UIVerticalLayout::SetWidthExpand);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expand");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, bool>(&type, "IsWidthExpand", &o2::UIVerticalLayout::IsWidthExpand);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, bool>(&type, "SetHeightExpand", &o2::UIVerticalLayout::SetHeightExpand);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expand");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, bool>(&type, "IsHeightExpand", &o2::UIVerticalLayout::IsHeightExpand);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, bool>(&type, "SetFitByChildren", &o2::UIVerticalLayout::SetFitByChildren);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "fit");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, bool>(&type, "IsFittingByChildren", &o2::UIVerticalLayout::IsFittingByChildren);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, bool>(&type, "UpdateLayout", &o2::UIVerticalLayout::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, UIWidget*>(&type, "OnChildAdded", &o2::UIVerticalLayout::OnChildAdded);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, UIWidget*>(&type, "OnChildRemoved", &o2::UIVerticalLayout::OnChildRemoved);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "RearrangeChilds", &o2::UIVerticalLayout::RearrangeChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "ArrangeFromTopToBottom", &o2::UIVerticalLayout::ArrangeFromTopToBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "ArrangeFromBottomToTop", &o2::UIVerticalLayout::ArrangeFromBottomToTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "ArrangeFromCenter", &o2::UIVerticalLayout::ArrangeFromCenter);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "ExpandSizeByChilds", &o2::UIVerticalLayout::ExpandSizeByChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, Vector<float>>(&type, "CalculateExpandedHeights", &o2::UIVerticalLayout::CalculateExpandedHeights);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, UIWidget*, float>(&type, "AlignWidgetByWidth", &o2::UIVerticalLayout::AlignWidgetByWidth);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	TypeInitializer::RegFuncParam<float>(funcInfo, "heightAnchor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "UpdateLayoutParametres", &o2::UIVerticalLayout::UpdateLayoutParametres);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "InitializeProperties", &o2::UIVerticalLayout::InitializeProperties);
}

void o2::UIVerticalProgress::InitializeType(o2::UIVerticalProgress* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)sample, sample->minValue);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)sample, sample->maxValue);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)sample, sample->scrollSense);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)sample, sample->onChange);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue).AddAttribute<SerializableAttribute<decltype(mValue)>>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)sample, sample->mSmoothValue);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)sample, sample->mMinValue).AddAttribute<SerializableAttribute<decltype(mMinValue)>>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)sample, sample->mMaxValue).AddAttribute<SerializableAttribute<decltype(mMaxValue)>>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)sample, sample->mScrollSense).AddAttribute<SerializableAttribute<decltype(mScrollSense)>>();
	TypeInitializer::RegField(&type, "mOrientation", (size_t)(char*)(&sample->mOrientation) - (size_t)(char*)sample, sample->mOrientation).AddAttribute<SerializableAttribute<decltype(mOrientation)>>();
	TypeInitializer::RegField(&type, "mBarLayer", (size_t)(char*)(&sample->mBarLayer) - (size_t)(char*)sample, sample->mBarLayer);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)sample, sample->mBackLayer);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "Update", &o2::UIVerticalProgress::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "SetValue", &o2::UIVerticalProgress::SetValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "SetValueForcible", &o2::UIVerticalProgress::SetValueForcible);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, float>(&type, "GetValue", &o2::UIVerticalProgress::GetValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "SetMinValue", &o2::UIVerticalProgress::SetMinValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, float>(&type, "GetMinValue", &o2::UIVerticalProgress::GetMinValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "SetMaxValue", &o2::UIVerticalProgress::SetMaxValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, float>(&type, "GetMaxValue", &o2::UIVerticalProgress::GetMaxValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float, float>(&type, "SetValueRange", &o2::UIVerticalProgress::SetValueRange);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "SetScrollSense", &o2::UIVerticalProgress::SetScrollSense);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, float>(&type, "GetScrollSense", &o2::UIVerticalProgress::GetScrollSense);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, Orientation>(&type, "SetOrientation", &o2::UIVerticalProgress::SetOrientation);
	TypeInitializer::RegFuncParam<Orientation>(funcInfo, "orientation");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, Orientation>(&type, "GetOrientation", &o2::UIVerticalProgress::GetOrientation);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIVerticalProgress::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, bool>(&type, "IsScrollable", &o2::UIVerticalProgress::IsScrollable);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, bool>(&type, "UpdateLayout", &o2::UIVerticalProgress::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void>(&type, "UpdateProgressLayersLayouts", &o2::UIVerticalProgress::UpdateProgressLayersLayouts);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIVerticalProgress::OnLayerAdded);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "GetValueFromCursor", &o2::UIVerticalProgress::GetValueFromCursor);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIVerticalProgress::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIVerticalProgress::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIVerticalProgress::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIVerticalProgress::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIVerticalProgress::OnCursorEnter);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIVerticalProgress::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "OnScrolled", &o2::UIVerticalProgress::OnScrolled);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const DataNode&>(&type, "OnDeserialized", &o2::UIVerticalProgress::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void>(&type, "OnVisibleChanged", &o2::UIVerticalProgress::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void>(&type, "InitializeProperties", &o2::UIVerticalProgress::InitializeProperties);
}

void o2::UIVerticalScrollBar::InitializeType(o2::UIVerticalScrollBar* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)sample, sample->minValue);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)sample, sample->maxValue);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)sample, sample->scrollSense);
	TypeInitializer::RegField(&type, "scrollSize", (size_t)(char*)(&sample->scrollSize) - (size_t)(char*)sample, sample->scrollSize);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)sample, sample->onChange);
	TypeInitializer::RegField(&type, "onSmoothChange", (size_t)(char*)(&sample->onSmoothChange) - (size_t)(char*)sample, sample->onSmoothChange);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue).AddAttribute<SerializableAttribute<decltype(mValue)>>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)sample, sample->mSmoothValue);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)sample, sample->mMinValue).AddAttribute<SerializableAttribute<decltype(mMinValue)>>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)sample, sample->mMaxValue).AddAttribute<SerializableAttribute<decltype(mMaxValue)>>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)sample, sample->mScrollSense).AddAttribute<SerializableAttribute<decltype(mScrollSense)>>();
	TypeInitializer::RegField(&type, "mScrollHandleSize", (size_t)(char*)(&sample->mScrollHandleSize) - (size_t)(char*)sample, sample->mScrollHandleSize).AddAttribute<SerializableAttribute<decltype(mScrollHandleSize)>>();
	TypeInitializer::RegField(&type, "mScrollhandleMinPxSize", (size_t)(char*)(&sample->mScrollhandleMinPxSize) - (size_t)(char*)sample, sample->mScrollhandleMinPxSize);
	TypeInitializer::RegField(&type, "mPressHandleOffset", (size_t)(char*)(&sample->mPressHandleOffset) - (size_t)(char*)sample, sample->mPressHandleOffset);
	TypeInitializer::RegField(&type, "mHandlePressed", (size_t)(char*)(&sample->mHandlePressed) - (size_t)(char*)sample, sample->mHandlePressed);
	TypeInitializer::RegField(&type, "mHandleLayer", (size_t)(char*)(&sample->mHandleLayer) - (size_t)(char*)sample, sample->mHandleLayer);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)sample, sample->mBackLayer);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "Update", &o2::UIVerticalScrollBar::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetValue", &o2::UIVerticalScrollBar::SetValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetValueForcible", &o2::UIVerticalScrollBar::SetValueForcible);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetValue", &o2::UIVerticalScrollBar::GetValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetSmoothValue", &o2::UIVerticalScrollBar::GetSmoothValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetMinValue", &o2::UIVerticalScrollBar::SetMinValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetMinValue", &o2::UIVerticalScrollBar::GetMinValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetMaxValue", &o2::UIVerticalScrollBar::SetMaxValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetMaxValue", &o2::UIVerticalScrollBar::GetMaxValue);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float, float>(&type, "SetValueRange", &o2::UIVerticalScrollBar::SetValueRange);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetScrollSense", &o2::UIVerticalScrollBar::SetScrollSense);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetScrollSense", &o2::UIVerticalScrollBar::GetScrollSense);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetScrollHandleSize", &o2::UIVerticalScrollBar::SetScrollHandleSize);
	TypeInitializer::RegFuncParam<float>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetScrollhandleSize", &o2::UIVerticalScrollBar::GetScrollhandleSize);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetMinimalScrollhandleSize", &o2::UIVerticalScrollBar::SetMinimalScrollhandleSize);
	TypeInitializer::RegFuncParam<float>(funcInfo, "pixelSize");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIVerticalScrollBar::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, bool>(&type, "IsScrollable", &o2::UIVerticalScrollBar::IsScrollable);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, bool>(&type, "UpdateLayout", &o2::UIVerticalScrollBar::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void>(&type, "UpdateProgressLayersLayouts", &o2::UIVerticalScrollBar::UpdateProgressLayersLayouts);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void>(&type, "OnLayoutUpdated", &o2::UIVerticalScrollBar::OnLayoutUpdated);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIVerticalScrollBar::OnLayerAdded);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float, const Input::Cursor&>(&type, "GetValueFromCursor", &o2::UIVerticalScrollBar::GetValueFromCursor);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIVerticalScrollBar::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIVerticalScrollBar::OnCursorReleased);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIVerticalScrollBar::OnCursorPressBreak);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIVerticalScrollBar::OnCursorStillDown);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIVerticalScrollBar::OnCursorEnter);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIVerticalScrollBar::OnCursorExit);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "OnScrolled", &o2::UIVerticalScrollBar::OnScrolled);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const DataNode&>(&type, "OnDeserialized", &o2::UIVerticalScrollBar::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void>(&type, "OnVisibleChanged", &o2::UIVerticalScrollBar::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void>(&type, "InitializeProperties", &o2::UIVerticalScrollBar::InitializeProperties);
}

void o2::UIWidget::InitializeType(o2::UIWidget* sample)
{
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name);
	TypeInitializer::RegField(&type, "parent", (size_t)(char*)(&sample->parent) - (size_t)(char*)sample, sample->parent);
	TypeInitializer::RegField(&type, "childs", (size_t)(char*)(&sample->childs) - (size_t)(char*)sample, sample->childs);
	TypeInitializer::RegField(&type, "layers", (size_t)(char*)(&sample->layers) - (size_t)(char*)sample, sample->layers);
	TypeInitializer::RegField(&type, "states", (size_t)(char*)(&sample->states) - (size_t)(char*)sample, sample->states);
	TypeInitializer::RegField(&type, "transparency", (size_t)(char*)(&sample->transparency) - (size_t)(char*)sample, sample->transparency);
	TypeInitializer::RegField(&type, "resTransparency", (size_t)(char*)(&sample->resTransparency) - (size_t)(char*)sample, sample->resTransparency);
	TypeInitializer::RegField(&type, "visible", (size_t)(char*)(&sample->visible) - (size_t)(char*)sample, sample->visible);
	TypeInitializer::RegField(&type, "child", (size_t)(char*)(&sample->child) - (size_t)(char*)sample, sample->child);
	TypeInitializer::RegField(&type, "layer", (size_t)(char*)(&sample->layer) - (size_t)(char*)sample, sample->layer);
	TypeInitializer::RegField(&type, "state", (size_t)(char*)(&sample->state) - (size_t)(char*)sample, sample->state);
	TypeInitializer::RegField(&type, "layout", (size_t)(char*)(&sample->layout) - (size_t)(char*)sample, sample->layout).AddAttribute<SerializableAttribute<decltype(layout)>>();
	TypeInitializer::RegField(&type, "onLayoutChanged", (size_t)(char*)(&sample->onLayoutChanged) - (size_t)(char*)sample, sample->onLayoutChanged);
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)sample, sample->mName).AddAttribute<SerializableAttribute<decltype(mName)>>();
	TypeInitializer::RegField(&type, "mLayers", (size_t)(char*)(&sample->mLayers) - (size_t)(char*)sample, sample->mLayers).AddAttribute<SerializableAttribute<decltype(mLayers)>>();
	TypeInitializer::RegField(&type, "mStates", (size_t)(char*)(&sample->mStates) - (size_t)(char*)sample, sample->mStates).AddAttribute<SerializableAttribute<decltype(mStates)>>();
	TypeInitializer::RegField(&type, "mParent", (size_t)(char*)(&sample->mParent) - (size_t)(char*)sample, sample->mParent);
	TypeInitializer::RegField(&type, "mChilds", (size_t)(char*)(&sample->mChilds) - (size_t)(char*)sample, sample->mChilds).AddAttribute<SerializableAttribute<decltype(mChilds)>>();
	TypeInitializer::RegField(&type, "mChildsAbsRect", (size_t)(char*)(&sample->mChildsAbsRect) - (size_t)(char*)sample, sample->mChildsAbsRect);
	TypeInitializer::RegField(&type, "mTransparency", (size_t)(char*)(&sample->mTransparency) - (size_t)(char*)sample, sample->mTransparency).AddAttribute<SerializableAttribute<decltype(mTransparency)>>();
	TypeInitializer::RegField(&type, "mResTransparency", (size_t)(char*)(&sample->mResTransparency) - (size_t)(char*)sample, sample->mResTransparency);
	TypeInitializer::RegField(&type, "mDrawingLayers", (size_t)(char*)(&sample->mDrawingLayers) - (size_t)(char*)sample, sample->mDrawingLayers);
	TypeInitializer::RegField(&type, "mSelectedState", (size_t)(char*)(&sample->mSelectedState) - (size_t)(char*)sample, sample->mSelectedState);
	TypeInitializer::RegField(&type, "mIsSelected", (size_t)(char*)(&sample->mIsSelected) - (size_t)(char*)sample, sample->mIsSelected);
	TypeInitializer::RegField(&type, "mVisibleState", (size_t)(char*)(&sample->mVisibleState) - (size_t)(char*)sample, sample->mVisibleState);
	TypeInitializer::RegField(&type, "mVisible", (size_t)(char*)(&sample->mVisible) - (size_t)(char*)sample, sample->mVisible);
	TypeInitializer::RegField(&type, "mResVisible", (size_t)(char*)(&sample->mResVisible) - (size_t)(char*)sample, sample->mResVisible);
	TypeInitializer::RegField(&type, "mFullyDisabled", (size_t)(char*)(&sample->mFullyDisabled) - (size_t)(char*)sample, sample->mFullyDisabled);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, float>(&type, "Update", &o2::UIWidget::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "Draw", &o2::UIWidget::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const String&>(&type, "SetName", &o2::UIWidget::SetName);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, String>(&type, "GetName", &o2::UIWidget::GetName);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidget*>(&type, "GetParent", &o2::UIWidget::GetParent);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidget*>(&type, "SetParent", &o2::UIWidget::SetParent);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "parent");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidget*, UIWidget*>(&type, "AddChild", &o2::UIWidget::AddChild);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const WidgetsVec&>(&type, "AddChilds", &o2::UIWidget::AddChilds);
	TypeInitializer::RegFuncParam<const WidgetsVec&>(funcInfo, "widgets");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidget*, UIWidget*, int>(&type, "AddChild", &o2::UIWidget::AddChild);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	TypeInitializer::RegFuncParam<int>(funcInfo, "index");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const String&>(&type, "RemoveChild", &o2::UIWidget::RemoveChild);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, UIWidget*, bool>(&type, "RemoveChild", &o2::UIWidget::RemoveChild);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidget*, const String&>(&type, "GetChild", &o2::UIWidget::GetChild);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "RemoveAllChilds", &o2::UIWidget::RemoveAllChilds);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, const WidgetsVec&>(&type, "GetChilds", &o2::UIWidget::GetChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetLayer*, UIWidgetLayer*>(&type, "AddLayer", &o2::UIWidget::AddLayer);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetLayer*, const String&, IRectDrawable*, const Layout&, float>(&type, "AddLayer", &o2::UIWidget::AddLayer);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<IRectDrawable*>(funcInfo, "drawable");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	TypeInitializer::RegFuncParam<float>(funcInfo, "depth");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetLayer*, const String&>(&type, "GetLayer", &o2::UIWidget::GetLayer);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, UIWidgetLayer*>(&type, "RemoveLayer", &o2::UIWidget::RemoveLayer);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const String&>(&type, "RemoveLayer", &o2::UIWidget::RemoveLayer);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "RemoveAllLayers", &o2::UIWidget::RemoveAllLayers);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, const LayersVec&>(&type, "GetLayers", &o2::UIWidget::GetLayers);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetState*, const String&>(&type, "AddState", &o2::UIWidget::AddState);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetState*, const String&, const Animation&>(&type, "AddState", &o2::UIWidget::AddState);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetState*, UIWidgetState*>(&type, "AddState", &o2::UIWidget::AddState);
	TypeInitializer::RegFuncParam<UIWidgetState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const String&>(&type, "RemoveState", &o2::UIWidget::RemoveState);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, UIWidgetState*>(&type, "RemoveState", &o2::UIWidget::RemoveState);
	TypeInitializer::RegFuncParam<UIWidgetState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "RemoveAllStates", &o2::UIWidget::RemoveAllStates);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const String&, bool>(&type, "SetState", &o2::UIWidget::SetState);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const String&>(&type, "GetState", &o2::UIWidget::GetState);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetState*, const String&>(&type, "GetStateObject", &o2::UIWidget::GetStateObject);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, const StatesVec&>(&type, "GetStates", &o2::UIWidget::GetStates);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, float>(&type, "SetTransparency", &o2::UIWidget::SetTransparency);
	TypeInitializer::RegFuncParam<float>(funcInfo, "transparency");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, float>(&type, "GetTransparency", &o2::UIWidget::GetTransparency);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, float>(&type, "GetResTransparency", &o2::UIWidget::GetResTransparency);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "SetVisible", &o2::UIWidget::SetVisible);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "visible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "SetVisibleForcible", &o2::UIWidget::SetVisibleForcible);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "visible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "Show", &o2::UIWidget::Show);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "Hide", &o2::UIWidget::Hide);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool>(&type, "IsVisible", &o2::UIWidget::IsVisible);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "Select", &o2::UIWidget::Select);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "Deselect", &o2::UIWidget::Deselect);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool>(&type, "IsSelected", &o2::UIWidget::IsSelected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool>(&type, "IsSelectable", &o2::UIWidget::IsSelectable);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIWidget::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "DrawDebugFrame", &o2::UIWidget::DrawDebugFrame);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "OnSelected", &o2::UIWidget::OnSelected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "OnDeselected", &o2::UIWidget::OnDeselected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "UpdateLayout", &o2::UIWidget::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateTransparency", &o2::UIWidget::UpdateTransparency);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateVisibility", &o2::UIWidget::UpdateVisibility);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidget*>(&type, "OnChildSelected", &o2::UIWidget::OnChildSelected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "RetargetStatesAnimations", &o2::UIWidget::RetargetStatesAnimations);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "RecalculateAbsRect", &o2::UIWidget::RecalculateAbsRect);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateLayersLayouts", &o2::UIWidget::UpdateLayersLayouts);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateLayersDrawingSequence", &o2::UIWidget::UpdateLayersDrawingSequence);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, WidgetsVec>(&type, "GetChildsNonConst", &o2::UIWidget::GetChildsNonConst);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, LayersVec>(&type, "GetLayersNonConst", &o2::UIWidget::GetLayersNonConst);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, StatesVec>(&type, "GetStatesNonConst", &o2::UIWidget::GetStatesNonConst);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, Dictionary<String, UIWidgetLayer*>>(&type, "GetAllLayers", &o2::UIWidget::GetAllLayers);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, Dictionary<String, UIWidget*>>(&type, "GetAllChilds", &o2::UIWidget::GetAllChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIWidget::OnLayerAdded);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidgetState*>(&type, "OnStateAdded", &o2::UIWidget::OnStateAdded);
	TypeInitializer::RegFuncParam<UIWidgetState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidget*>(&type, "OnChildAdded", &o2::UIWidget::OnChildAdded);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidget*>(&type, "OnChildRemoved", &o2::UIWidget::OnChildRemoved);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const DataNode&>(&type, "OnDeserialized", &o2::UIWidget::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const RectF&, float>(&type, "ForceDraw", &o2::UIWidget::ForceDraw);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "area");
	TypeInitializer::RegFuncParam<float>(funcInfo, "transparency");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "OnVisibleChanged", &o2::UIWidget::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "InitializeProperties", &o2::UIWidget::InitializeProperties);
}

void o2::UIWidgetLayer::InitializeType(o2::UIWidgetLayer* sample)
{
	TypeInitializer::RegField(&type, "layout", (size_t)(char*)(&sample->layout) - (size_t)(char*)sample, sample->layout).AddAttribute<SerializableAttribute<decltype(layout)>>();
	TypeInitializer::RegField(&type, "interactableLayout", (size_t)(char*)(&sample->interactableLayout) - (size_t)(char*)sample, sample->interactableLayout);
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name).AddAttribute<SerializableAttribute<decltype(name)>>();
	TypeInitializer::RegField(&type, "depth", (size_t)(char*)(&sample->depth) - (size_t)(char*)sample, sample->depth);
	TypeInitializer::RegField(&type, "transparency", (size_t)(char*)(&sample->transparency) - (size_t)(char*)sample, sample->transparency);
	TypeInitializer::RegField(&type, "drawable", (size_t)(char*)(&sample->drawable) - (size_t)(char*)sample, sample->drawable).AddAttribute<SerializableAttribute<decltype(drawable)>>();
	TypeInitializer::RegField(&type, "child", (size_t)(char*)(&sample->child) - (size_t)(char*)sample, sample->child);
	TypeInitializer::RegField(&type, "mTransparency", (size_t)(char*)(&sample->mTransparency) - (size_t)(char*)sample, sample->mTransparency).AddAttribute<SerializableAttribute<decltype(mTransparency)>>();
	TypeInitializer::RegField(&type, "mResTransparency", (size_t)(char*)(&sample->mResTransparency) - (size_t)(char*)sample, sample->mResTransparency);
	TypeInitializer::RegField(&type, "mDepth", (size_t)(char*)(&sample->mDepth) - (size_t)(char*)sample, sample->mDepth).AddAttribute<SerializableAttribute<decltype(mDepth)>>();
	TypeInitializer::RegField(&type, "mAbsolutePosition", (size_t)(char*)(&sample->mAbsolutePosition) - (size_t)(char*)sample, sample->mAbsolutePosition);
	TypeInitializer::RegField(&type, "mInteractableArea", (size_t)(char*)(&sample->mInteractableArea) - (size_t)(char*)sample, sample->mInteractableArea);
	TypeInitializer::RegField(&type, "mOwnerWidget", (size_t)(char*)(&sample->mOwnerWidget) - (size_t)(char*)sample, sample->mOwnerWidget);
	TypeInitializer::RegField(&type, "mParent", (size_t)(char*)(&sample->mParent) - (size_t)(char*)sample, sample->mParent);
	TypeInitializer::RegField(&type, "mChilds", (size_t)(char*)(&sample->mChilds) - (size_t)(char*)sample, sample->mChilds).AddAttribute<SerializableAttribute<decltype(mChilds)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void>(&type, "Draw", &o2::UIWidgetLayer::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, float>(&type, "Update", &o2::UIWidgetLayer::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, UIWidgetLayer*, UIWidgetLayer*>(&type, "AddChild", &o2::UIWidgetLayer::AddChild);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, bool, UIWidgetLayer*, bool>(&type, "RemoveChild", &o2::UIWidgetLayer::RemoveChild);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void>(&type, "RemoveAllChilds", &o2::UIWidgetLayer::RemoveAllChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, UIWidgetLayer*>(&type, "SetParent", &o2::UIWidgetLayer::SetParent);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "parent");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, UIWidgetLayer*>(&type, "GetParent", &o2::UIWidgetLayer::GetParent);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, ChildsVec&>(&type, "GetChilds", &o2::UIWidgetLayer::GetChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, const ChildsVec&>(&type, "GetChilds", &o2::UIWidgetLayer::GetChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, UIWidgetLayer*, const String&, IRectDrawable*, const Layout&, float>(&type, "AddChildLayer", &o2::UIWidgetLayer::AddChildLayer);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<IRectDrawable*>(funcInfo, "drawable");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	TypeInitializer::RegFuncParam<float>(funcInfo, "depth");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, UIWidgetLayer*, const String&>(&type, "GetChild", &o2::UIWidgetLayer::GetChild);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, LayersVec>(&type, "GetAllChilds", &o2::UIWidgetLayer::GetAllChilds);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, float>(&type, "SetDepth", &o2::UIWidgetLayer::SetDepth);
	TypeInitializer::RegFuncParam<float>(funcInfo, "depth");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, float>(&type, "GetDepth", &o2::UIWidgetLayer::GetDepth);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, float>(&type, "SetTransparency", &o2::UIWidgetLayer::SetTransparency);
	TypeInitializer::RegFuncParam<float>(funcInfo, "transparency");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, float>(&type, "GetTransparency", &o2::UIWidgetLayer::GetTransparency);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIWidgetLayer::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, const DataNode&>(&type, "OnDeserialized", &o2::UIWidgetLayer::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, UIWidget*>(&type, "SetOwnerWidget", &o2::UIWidgetLayer::SetOwnerWidget);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "owner");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, UIWidgetLayer*>(&type, "OnChildAdded", &o2::UIWidgetLayer::OnChildAdded);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void>(&type, "UpdateLayout", &o2::UIWidgetLayer::UpdateLayout);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void>(&type, "UpdateResTransparency", &o2::UIWidgetLayer::UpdateResTransparency);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, Dictionary<String, UIWidgetLayer*>>(&type, "GetAllChildLayers", &o2::UIWidgetLayer::GetAllChildLayers);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void>(&type, "InitializeProperties", &o2::UIWidgetLayer::InitializeProperties);
}

void o2::UIWidgetLayout::InitializeType(o2::UIWidgetLayout* sample)
{
	TypeInitializer::RegField(&type, "pivot", (size_t)(char*)(&sample->pivot) - (size_t)(char*)sample, sample->pivot);
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position);
	TypeInitializer::RegField(&type, "size", (size_t)(char*)(&sample->size) - (size_t)(char*)sample, sample->size);
	TypeInitializer::RegField(&type, "width", (size_t)(char*)(&sample->width) - (size_t)(char*)sample, sample->width);
	TypeInitializer::RegField(&type, "height", (size_t)(char*)(&sample->height) - (size_t)(char*)sample, sample->height);
	TypeInitializer::RegField(&type, "absPosition", (size_t)(char*)(&sample->absPosition) - (size_t)(char*)sample, sample->absPosition);
	TypeInitializer::RegField(&type, "absLeftTop", (size_t)(char*)(&sample->absLeftTop) - (size_t)(char*)sample, sample->absLeftTop);
	TypeInitializer::RegField(&type, "absLeftBottom", (size_t)(char*)(&sample->absLeftBottom) - (size_t)(char*)sample, sample->absLeftBottom);
	TypeInitializer::RegField(&type, "absRightTop", (size_t)(char*)(&sample->absRightTop) - (size_t)(char*)sample, sample->absRightTop);
	TypeInitializer::RegField(&type, "absRightBottom", (size_t)(char*)(&sample->absRightBottom) - (size_t)(char*)sample, sample->absRightBottom);
	TypeInitializer::RegField(&type, "absRect", (size_t)(char*)(&sample->absRect) - (size_t)(char*)sample, sample->absRect);
	TypeInitializer::RegField(&type, "absLeft", (size_t)(char*)(&sample->absLeft) - (size_t)(char*)sample, sample->absLeft);
	TypeInitializer::RegField(&type, "absRight", (size_t)(char*)(&sample->absRight) - (size_t)(char*)sample, sample->absRight);
	TypeInitializer::RegField(&type, "absBottom", (size_t)(char*)(&sample->absBottom) - (size_t)(char*)sample, sample->absBottom);
	TypeInitializer::RegField(&type, "absTop", (size_t)(char*)(&sample->absTop) - (size_t)(char*)sample, sample->absTop);
	TypeInitializer::RegField(&type, "pivotX", (size_t)(char*)(&sample->pivotX) - (size_t)(char*)sample, sample->pivotX);
	TypeInitializer::RegField(&type, "pivotY", (size_t)(char*)(&sample->pivotY) - (size_t)(char*)sample, sample->pivotY);
	TypeInitializer::RegField(&type, "anchorMin", (size_t)(char*)(&sample->anchorMin) - (size_t)(char*)sample, sample->anchorMin);
	TypeInitializer::RegField(&type, "anchorMax", (size_t)(char*)(&sample->anchorMax) - (size_t)(char*)sample, sample->anchorMax);
	TypeInitializer::RegField(&type, "offsetMin", (size_t)(char*)(&sample->offsetMin) - (size_t)(char*)sample, sample->offsetMin);
	TypeInitializer::RegField(&type, "offsetMax", (size_t)(char*)(&sample->offsetMax) - (size_t)(char*)sample, sample->offsetMax);
	TypeInitializer::RegField(&type, "anchorLeft", (size_t)(char*)(&sample->anchorLeft) - (size_t)(char*)sample, sample->anchorLeft);
	TypeInitializer::RegField(&type, "anchorRight", (size_t)(char*)(&sample->anchorRight) - (size_t)(char*)sample, sample->anchorRight);
	TypeInitializer::RegField(&type, "anchorBottom", (size_t)(char*)(&sample->anchorBottom) - (size_t)(char*)sample, sample->anchorBottom);
	TypeInitializer::RegField(&type, "anchorTop", (size_t)(char*)(&sample->anchorTop) - (size_t)(char*)sample, sample->anchorTop);
	TypeInitializer::RegField(&type, "offsetLeft", (size_t)(char*)(&sample->offsetLeft) - (size_t)(char*)sample, sample->offsetLeft);
	TypeInitializer::RegField(&type, "offsetRight", (size_t)(char*)(&sample->offsetRight) - (size_t)(char*)sample, sample->offsetRight);
	TypeInitializer::RegField(&type, "offsetBottom", (size_t)(char*)(&sample->offsetBottom) - (size_t)(char*)sample, sample->offsetBottom);
	TypeInitializer::RegField(&type, "offsetTop", (size_t)(char*)(&sample->offsetTop) - (size_t)(char*)sample, sample->offsetTop);
	TypeInitializer::RegField(&type, "minSize", (size_t)(char*)(&sample->minSize) - (size_t)(char*)sample, sample->minSize);
	TypeInitializer::RegField(&type, "minWidth", (size_t)(char*)(&sample->minWidth) - (size_t)(char*)sample, sample->minWidth);
	TypeInitializer::RegField(&type, "minHeight", (size_t)(char*)(&sample->minHeight) - (size_t)(char*)sample, sample->minHeight);
	TypeInitializer::RegField(&type, "maxSize", (size_t)(char*)(&sample->maxSize) - (size_t)(char*)sample, sample->maxSize);
	TypeInitializer::RegField(&type, "maxWidth", (size_t)(char*)(&sample->maxWidth) - (size_t)(char*)sample, sample->maxWidth);
	TypeInitializer::RegField(&type, "maxHeight", (size_t)(char*)(&sample->maxHeight) - (size_t)(char*)sample, sample->maxHeight);
	TypeInitializer::RegField(&type, "weight", (size_t)(char*)(&sample->weight) - (size_t)(char*)sample, sample->weight);
	TypeInitializer::RegField(&type, "widthWeight", (size_t)(char*)(&sample->widthWeight) - (size_t)(char*)sample, sample->widthWeight);
	TypeInitializer::RegField(&type, "heigthWeight", (size_t)(char*)(&sample->heigthWeight) - (size_t)(char*)sample, sample->heigthWeight);
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner);
	TypeInitializer::RegField(&type, "mPivot", (size_t)(char*)(&sample->mPivot) - (size_t)(char*)sample, sample->mPivot).AddAttribute<SerializableAttribute<decltype(mPivot)>>();
	TypeInitializer::RegField(&type, "mAnchorMin", (size_t)(char*)(&sample->mAnchorMin) - (size_t)(char*)sample, sample->mAnchorMin).AddAttribute<SerializableAttribute<decltype(mAnchorMin)>>();
	TypeInitializer::RegField(&type, "mAnchorMax", (size_t)(char*)(&sample->mAnchorMax) - (size_t)(char*)sample, sample->mAnchorMax).AddAttribute<SerializableAttribute<decltype(mAnchorMax)>>();
	TypeInitializer::RegField(&type, "mOffsetMin", (size_t)(char*)(&sample->mOffsetMin) - (size_t)(char*)sample, sample->mOffsetMin).AddAttribute<SerializableAttribute<decltype(mOffsetMin)>>();
	TypeInitializer::RegField(&type, "mOffsetMax", (size_t)(char*)(&sample->mOffsetMax) - (size_t)(char*)sample, sample->mOffsetMax).AddAttribute<SerializableAttribute<decltype(mOffsetMax)>>();
	TypeInitializer::RegField(&type, "mMinSize", (size_t)(char*)(&sample->mMinSize) - (size_t)(char*)sample, sample->mMinSize).AddAttribute<SerializableAttribute<decltype(mMinSize)>>();
	TypeInitializer::RegField(&type, "mMaxSize", (size_t)(char*)(&sample->mMaxSize) - (size_t)(char*)sample, sample->mMaxSize).AddAttribute<SerializableAttribute<decltype(mMaxSize)>>();
	TypeInitializer::RegField(&type, "mAbsoluteRect", (size_t)(char*)(&sample->mAbsoluteRect) - (size_t)(char*)sample, sample->mAbsoluteRect).AddAttribute<SerializableAttribute<decltype(mAbsoluteRect)>>();
	TypeInitializer::RegField(&type, "mLocalRect", (size_t)(char*)(&sample->mLocalRect) - (size_t)(char*)sample, sample->mLocalRect);
	TypeInitializer::RegField(&type, "mWeight", (size_t)(char*)(&sample->mWeight) - (size_t)(char*)sample, sample->mWeight).AddAttribute<SerializableAttribute<decltype(mWeight)>>();
	TypeInitializer::RegField(&type, "mDrivenByParent", (size_t)(char*)(&sample->mDrivenByParent) - (size_t)(char*)sample, sample->mDrivenByParent).AddAttribute<SerializableAttribute<decltype(mDrivenByParent)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIWidgetLayout::IsUnderPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetPosition", &o2::UIWidgetLayout::SetPosition);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetPosition", &o2::UIWidgetLayout::GetPosition);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetSize", &o2::UIWidgetLayout::SetSize);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetSize", &o2::UIWidgetLayout::GetSize);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetWidth", &o2::UIWidgetLayout::SetWidth);
	TypeInitializer::RegFuncParam<float>(funcInfo, "width");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetWidth", &o2::UIWidgetLayout::GetWidth);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetHeight", &o2::UIWidgetLayout::SetHeight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "height");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetHeight", &o2::UIWidgetLayout::GetHeight);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const RectF&>(&type, "SetRect", &o2::UIWidgetLayout::SetRect);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, RectF>(&type, "GetRect", &o2::UIWidgetLayout::GetRect);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAbsoluteLeft", &o2::UIWidgetLayout::SetAbsoluteLeft);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAbsoluteLeft", &o2::UIWidgetLayout::GetAbsoluteLeft);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAbsoluteRight", &o2::UIWidgetLayout::SetAbsoluteRight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAbsoluteRight", &o2::UIWidgetLayout::GetAbsoluteRight);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAbsoluteBottom", &o2::UIWidgetLayout::SetAbsoluteBottom);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAbsoluteBottom", &o2::UIWidgetLayout::GetAbsoluteBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAbsoluteTop", &o2::UIWidgetLayout::SetAbsoluteTop);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAbsoluteTop", &o2::UIWidgetLayout::GetAbsoluteTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAbsolutePosition", &o2::UIWidgetLayout::SetAbsolutePosition);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "absPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAbsolutePosition", &o2::UIWidgetLayout::GetAbsolutePosition);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAbsoluteLeftTop", &o2::UIWidgetLayout::SetAbsoluteLeftTop);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "absPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAbsoluteLeftTop", &o2::UIWidgetLayout::GetAbsoluteLeftTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAbsoluteLeftBottom", &o2::UIWidgetLayout::SetAbsoluteLeftBottom);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "absPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAbsoluteLeftBottom", &o2::UIWidgetLayout::GetAbsoluteLeftBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAbsoluteRightTop", &o2::UIWidgetLayout::SetAbsoluteRightTop);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "absPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAbsoluteRightTop", &o2::UIWidgetLayout::GetAbsoluteRightTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAbsoluteRightBottom", &o2::UIWidgetLayout::SetAbsoluteRightBottom);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "absPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAbsoluteRightBottom", &o2::UIWidgetLayout::GetAbsoluteRightBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const RectF&>(&type, "SetAbsoluteRect", &o2::UIWidgetLayout::SetAbsoluteRect);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, RectF>(&type, "GetAbsoluteRect", &o2::UIWidgetLayout::GetAbsoluteRect);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetPivot", &o2::UIWidgetLayout::SetPivot);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "pivot");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetPivot", &o2::UIWidgetLayout::GetPivot);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetPivotX", &o2::UIWidgetLayout::SetPivotX);
	TypeInitializer::RegFuncParam<float>(funcInfo, "x");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetPivotX", &o2::UIWidgetLayout::GetPivotX);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetPivotY", &o2::UIWidgetLayout::SetPivotY);
	TypeInitializer::RegFuncParam<float>(funcInfo, "y");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetPivotY", &o2::UIWidgetLayout::GetPivotY);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAnchorMin", &o2::UIWidgetLayout::SetAnchorMin);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "min");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAnchorMin", &o2::UIWidgetLayout::GetAnchorMin);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAnchorMax", &o2::UIWidgetLayout::SetAnchorMax);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "max");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAnchorMax", &o2::UIWidgetLayout::GetAnchorMax);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAnchorLeft", &o2::UIWidgetLayout::SetAnchorLeft);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAnchorLeft", &o2::UIWidgetLayout::GetAnchorLeft);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAnchorRight", &o2::UIWidgetLayout::SetAnchorRight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAnchorRight", &o2::UIWidgetLayout::GetAnchorRight);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAnchorBottom", &o2::UIWidgetLayout::SetAnchorBottom);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAnchorBottom", &o2::UIWidgetLayout::GetAnchorBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAnchorTop", &o2::UIWidgetLayout::SetAnchorTop);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAnchorTop", &o2::UIWidgetLayout::GetAnchorTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetOffsetMin", &o2::UIWidgetLayout::SetOffsetMin);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "min");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetOffsetMin", &o2::UIWidgetLayout::GetOffsetMin);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetOffsetMax", &o2::UIWidgetLayout::SetOffsetMax);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "max");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetOffsetMax", &o2::UIWidgetLayout::GetOffsetMax);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetOffsetLeft", &o2::UIWidgetLayout::SetOffsetLeft);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetOffsetLeft", &o2::UIWidgetLayout::GetOffsetLeft);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetOffsetRight", &o2::UIWidgetLayout::SetOffsetRight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetOffsetRight", &o2::UIWidgetLayout::GetOffsetRight);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetOffsetBottom", &o2::UIWidgetLayout::SetOffsetBottom);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetOffsetBottom", &o2::UIWidgetLayout::GetOffsetBottom);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetOffsetTop", &o2::UIWidgetLayout::SetOffsetTop);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetOffsetTop", &o2::UIWidgetLayout::GetOffsetTop);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetMinimalSize", &o2::UIWidgetLayout::SetMinimalSize);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "minSize");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetMinimalSize", &o2::UIWidgetLayout::GetMinimalSize);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetMinimalWidth", &o2::UIWidgetLayout::SetMinimalWidth);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetMinimalWidth", &o2::UIWidgetLayout::GetMinimalWidth);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetMinimalHeight", &o2::UIWidgetLayout::SetMinimalHeight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetMinimalHeight", &o2::UIWidgetLayout::GetMinimalHeight);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetMaximalSize", &o2::UIWidgetLayout::SetMaximalSize);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "maxSize");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetMaximalSize", &o2::UIWidgetLayout::GetMaximalSize);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetMaximalWidth", &o2::UIWidgetLayout::SetMaximalWidth);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetMaximalWidth", &o2::UIWidgetLayout::GetMaximalWidth);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetMaximalHeight", &o2::UIWidgetLayout::SetMaximalHeight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetMaximalHeight", &o2::UIWidgetLayout::GetMaximalHeight);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetWeight", &o2::UIWidgetLayout::SetWeight);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "weight");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetWeight", &o2::UIWidgetLayout::GetWeight);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetWidthWeight", &o2::UIWidgetLayout::SetWidthWeight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "widthWeigth");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetWidthWeight", &o2::UIWidgetLayout::GetWidthWeight);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetHeightWeight", &o2::UIWidgetLayout::SetHeightWeight);
	TypeInitializer::RegFuncParam<float>(funcInfo, "heigthWeigth");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetHeightWeight", &o2::UIWidgetLayout::GetHeightWeight);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const UIWidgetLayout&>(&type, "CopyFrom", &o2::UIWidgetLayout::CopyFrom);
	TypeInitializer::RegFuncParam<const UIWidgetLayout&>(funcInfo, "other");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void>(&type, "InitializeProperties", &o2::UIWidgetLayout::InitializeProperties);
}

void o2::UIWidgetState::InitializeType(o2::UIWidgetState* sample)
{
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name).AddAttribute<SerializableAttribute<decltype(name)>>();
	TypeInitializer::RegField(&type, "animation", (size_t)(char*)(&sample->animation) - (size_t)(char*)sample, sample->animation).AddAttribute<SerializableAttribute<decltype(animation)>>();
	TypeInitializer::RegField(&type, "offStateAnimationSpeed", (size_t)(char*)(&sample->offStateAnimationSpeed) - (size_t)(char*)sample, sample->offStateAnimationSpeed).AddAttribute<SerializableAttribute<decltype(offStateAnimationSpeed)>>();
	TypeInitializer::RegField(&type, "onStateFullyTrue", (size_t)(char*)(&sample->onStateFullyTrue) - (size_t)(char*)sample, sample->onStateFullyTrue);
	TypeInitializer::RegField(&type, "onStateFullyFalse", (size_t)(char*)(&sample->onStateFullyFalse) - (size_t)(char*)sample, sample->onStateFullyFalse);
	TypeInitializer::RegField(&type, "onStateBecomesTrue", (size_t)(char*)(&sample->onStateBecomesTrue) - (size_t)(char*)sample, sample->onStateBecomesTrue);
	TypeInitializer::RegField(&type, "onStateBecomesFalse", (size_t)(char*)(&sample->onStateBecomesFalse) - (size_t)(char*)sample, sample->onStateBecomesFalse);
	TypeInitializer::RegField(&type, "mState", (size_t)(char*)(&sample->mState) - (size_t)(char*)sample, sample->mState).AddAttribute<SerializableAttribute<decltype(mState)>>();
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIWidgetState, void, bool>(&type, "SetState", &o2::UIWidgetState::SetState);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetState, void, bool>(&type, "SetStateForcible", &o2::UIWidgetState::SetStateForcible);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetState, bool>(&type, "GetState", &o2::UIWidgetState::GetState);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetState, void, float>(&type, "Update", &o2::UIWidgetState::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
}

void o2::UIWindow::InitializeType(o2::UIWindow* sample)
{
	TypeInitializer::RegField(&type, "caption", (size_t)(char*)(&sample->caption) - (size_t)(char*)sample, sample->caption);
	TypeInitializer::RegField(&type, "icon", (size_t)(char*)(&sample->icon) - (size_t)(char*)sample, sample->icon);
	TypeInitializer::RegField(&type, "mIconDrawable", (size_t)(char*)(&sample->mIconDrawable) - (size_t)(char*)sample, sample->mIconDrawable);
	TypeInitializer::RegField(&type, "mCaptionDrawable", (size_t)(char*)(&sample->mCaptionDrawable) - (size_t)(char*)sample, sample->mCaptionDrawable);
	TypeInitializer::RegField(&type, "mWindowElements", (size_t)(char*)(&sample->mWindowElements) - (size_t)(char*)sample, sample->mWindowElements).AddAttribute<SerializableAttribute<decltype(mWindowElements)>>();
	TypeInitializer::RegField(&type, "mHeadDragHandle", (size_t)(char*)(&sample->mHeadDragHandle) - (size_t)(char*)sample, sample->mHeadDragHandle);
	TypeInitializer::RegField(&type, "mHeadDragAreaLayout", (size_t)(char*)(&sample->mHeadDragAreaLayout) - (size_t)(char*)sample, sample->mHeadDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mHeadDragAreaLayout)>>();
	TypeInitializer::RegField(&type, "mHeadDragAreaRect", (size_t)(char*)(&sample->mHeadDragAreaRect) - (size_t)(char*)sample, sample->mHeadDragAreaRect);
	TypeInitializer::RegField(&type, "mTopDragHandle", (size_t)(char*)(&sample->mTopDragHandle) - (size_t)(char*)sample, sample->mTopDragHandle);
	TypeInitializer::RegField(&type, "mTopDragAreaLayout", (size_t)(char*)(&sample->mTopDragAreaLayout) - (size_t)(char*)sample, sample->mTopDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mTopDragAreaLayout)>>();
	TypeInitializer::RegField(&type, "mTopDragAreaRect", (size_t)(char*)(&sample->mTopDragAreaRect) - (size_t)(char*)sample, sample->mTopDragAreaRect);
	TypeInitializer::RegField(&type, "mBottomDragHandle", (size_t)(char*)(&sample->mBottomDragHandle) - (size_t)(char*)sample, sample->mBottomDragHandle);
	TypeInitializer::RegField(&type, "mBottomDragAreaLayout", (size_t)(char*)(&sample->mBottomDragAreaLayout) - (size_t)(char*)sample, sample->mBottomDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mBottomDragAreaLayout)>>();
	TypeInitializer::RegField(&type, "mBottomDragAreaRect", (size_t)(char*)(&sample->mBottomDragAreaRect) - (size_t)(char*)sample, sample->mBottomDragAreaRect);
	TypeInitializer::RegField(&type, "mLeftDragHandle", (size_t)(char*)(&sample->mLeftDragHandle) - (size_t)(char*)sample, sample->mLeftDragHandle);
	TypeInitializer::RegField(&type, "mLeftDragAreaLayout", (size_t)(char*)(&sample->mLeftDragAreaLayout) - (size_t)(char*)sample, sample->mLeftDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mLeftDragAreaLayout)>>();
	TypeInitializer::RegField(&type, "mLeftDragAreaRect", (size_t)(char*)(&sample->mLeftDragAreaRect) - (size_t)(char*)sample, sample->mLeftDragAreaRect);
	TypeInitializer::RegField(&type, "mRightDragHandle", (size_t)(char*)(&sample->mRightDragHandle) - (size_t)(char*)sample, sample->mRightDragHandle);
	TypeInitializer::RegField(&type, "mRightDragAreaLayout", (size_t)(char*)(&sample->mRightDragAreaLayout) - (size_t)(char*)sample, sample->mRightDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mRightDragAreaLayout)>>();
	TypeInitializer::RegField(&type, "mRightDragAreaRect", (size_t)(char*)(&sample->mRightDragAreaRect) - (size_t)(char*)sample, sample->mRightDragAreaRect);
	TypeInitializer::RegField(&type, "mLeftTopDragHandle", (size_t)(char*)(&sample->mLeftTopDragHandle) - (size_t)(char*)sample, sample->mLeftTopDragHandle);
	TypeInitializer::RegField(&type, "mLeftTopDragAreaLayout", (size_t)(char*)(&sample->mLeftTopDragAreaLayout) - (size_t)(char*)sample, sample->mLeftTopDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mLeftTopDragAreaLayout)>>();
	TypeInitializer::RegField(&type, "mLeftTopDragAreaRect", (size_t)(char*)(&sample->mLeftTopDragAreaRect) - (size_t)(char*)sample, sample->mLeftTopDragAreaRect);
	TypeInitializer::RegField(&type, "mRightTopDragHandle", (size_t)(char*)(&sample->mRightTopDragHandle) - (size_t)(char*)sample, sample->mRightTopDragHandle);
	TypeInitializer::RegField(&type, "mRightTopDragAreaLayout", (size_t)(char*)(&sample->mRightTopDragAreaLayout) - (size_t)(char*)sample, sample->mRightTopDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mRightTopDragAreaLayout)>>();
	TypeInitializer::RegField(&type, "mRightTopDragAreaRect", (size_t)(char*)(&sample->mRightTopDragAreaRect) - (size_t)(char*)sample, sample->mRightTopDragAreaRect);
	TypeInitializer::RegField(&type, "mLeftBottomDragHandle", (size_t)(char*)(&sample->mLeftBottomDragHandle) - (size_t)(char*)sample, sample->mLeftBottomDragHandle);
	TypeInitializer::RegField(&type, "mLeftBottomDragAreaLayout", (size_t)(char*)(&sample->mLeftBottomDragAreaLayout) - (size_t)(char*)sample, sample->mLeftBottomDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mLeftBottomDragAreaLayout)>>();
	TypeInitializer::RegField(&type, "mLeftBottomDragAreaRect", (size_t)(char*)(&sample->mLeftBottomDragAreaRect) - (size_t)(char*)sample, sample->mLeftBottomDragAreaRect);
	TypeInitializer::RegField(&type, "mRightBottomDragHandle", (size_t)(char*)(&sample->mRightBottomDragHandle) - (size_t)(char*)sample, sample->mRightBottomDragHandle);
	TypeInitializer::RegField(&type, "mRightBottomDragAreaLayout", (size_t)(char*)(&sample->mRightBottomDragAreaLayout) - (size_t)(char*)sample, sample->mRightBottomDragAreaLayout).AddAttribute<SerializableAttribute<decltype(mRightBottomDragAreaLayout)>>();
	TypeInitializer::RegField(&type, "mRightBottomDragAreaRect", (size_t)(char*)(&sample->mRightBottomDragAreaRect) - (size_t)(char*)sample, sample->mRightBottomDragAreaRect);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, float>(&type, "Update", &o2::UIWindow::Update);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "Draw", &o2::UIWindow::Draw);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, UIWidget*, UIWidget*>(&type, "AddWindowElement", &o2::UIWindow::AddWindowElement);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, UIWidget*>(&type, "RemoveWindowElement", &o2::UIWindow::RemoveWindowElement);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "RemoveAllWindowElements", &o2::UIWindow::RemoveAllWindowElements);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, Sprite*>(&type, "SetIcon", &o2::UIWindow::SetIcon);
	TypeInitializer::RegFuncParam<Sprite*>(funcInfo, "icon");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, Sprite*>(&type, "GetIcon", &o2::UIWindow::GetIcon);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, const WString&>(&type, "SetCaption", &o2::UIWindow::SetCaption);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "caption");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, WString>(&type, "GetCaption", &o2::UIWindow::GetCaption);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&>(&type, "SetDragAreaLayouts", &o2::UIWindow::SetDragAreaLayouts);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "head");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "top");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "bottom");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "left");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "right");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "leftTop");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "rightTop");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "leftBottom");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "rightBottom");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, bool>(&type, "IsSelectable", &o2::UIWindow::IsSelectable);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, bool>(&type, "UpdateLayout", &o2::UIWindow::UpdateLayout);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "UpdateTransparency", &o2::UIWindow::UpdateTransparency);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIWindow::OnLayerAdded);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "InitializeHandles", &o2::UIWindow::InitializeHandles);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, bool>(&type, "SetHandlesInteractable", &o2::UIWindow::SetHandlesInteractable);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "interactable");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "BindHandlesInteractableToVisibility", &o2::UIWindow::BindHandlesInteractableToVisibility);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "OnSelected", &o2::UIWindow::OnSelected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, UIWidget*>(&type, "OnChildSelected", &o2::UIWindow::OnChildSelected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIWindow::OnCursorPressed);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, UIWidgetState*>(&type, "OnStateAdded", &o2::UIWindow::OnStateAdded);
	TypeInitializer::RegFuncParam<UIWidgetState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "OnVisibleChanged", &o2::UIWindow::OnVisibleChanged);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "InitializeProperties", &o2::UIWindow::InitializeProperties);
}

void o2::ISerializable::InitializeType(o2::ISerializable* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::ISerializable, DataNode>(&type, "Serialize", &o2::ISerializable::Serialize);
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, void, const DataNode&>(&type, "Deserialize", &o2::ISerializable::Deserialize);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, void, DataNode&>(&type, "OnSerialize", &o2::ISerializable::OnSerialize);
	TypeInitializer::RegFuncParam<DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, void, const DataNode&>(&type, "OnDeserialized", &o2::ISerializable::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
}

void o2::ShortcutKeys::InitializeType(o2::ShortcutKeys* sample)
{
	TypeInitializer::RegField(&type, "control", (size_t)(char*)(&sample->control) - (size_t)(char*)sample, sample->control).AddAttribute<SerializableAttribute<decltype(control)>>();
	TypeInitializer::RegField(&type, "shift", (size_t)(char*)(&sample->shift) - (size_t)(char*)sample, sample->shift).AddAttribute<SerializableAttribute<decltype(shift)>>();
	TypeInitializer::RegField(&type, "alt", (size_t)(char*)(&sample->alt) - (size_t)(char*)sample, sample->alt).AddAttribute<SerializableAttribute<decltype(alt)>>();
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key).AddAttribute<SerializableAttribute<decltype(key)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::ShortcutKeys, bool>(&type, "IsPressed", &o2::ShortcutKeys::IsPressed);
	funcInfo = TypeInitializer::RegFunction<o2::ShortcutKeys, bool>(&type, "IsDown", &o2::ShortcutKeys::IsDown);
	funcInfo = TypeInitializer::RegFunction<o2::ShortcutKeys, String>(&type, "AsString", &o2::ShortcutKeys::AsString);
}

void o2::TimeStamp::InitializeType(o2::TimeStamp* sample)
{
	TypeInitializer::RegField(&type, "mYear", (size_t)(char*)(&sample->mYear) - (size_t)(char*)sample, sample->mYear).AddAttribute<SerializableAttribute<decltype(mYear)>>();
	TypeInitializer::RegField(&type, "mMonth", (size_t)(char*)(&sample->mMonth) - (size_t)(char*)sample, sample->mMonth).AddAttribute<SerializableAttribute<decltype(mMonth)>>();
	TypeInitializer::RegField(&type, "mDay", (size_t)(char*)(&sample->mDay) - (size_t)(char*)sample, sample->mDay).AddAttribute<SerializableAttribute<decltype(mDay)>>();
	TypeInitializer::RegField(&type, "mHour", (size_t)(char*)(&sample->mHour) - (size_t)(char*)sample, sample->mHour).AddAttribute<SerializableAttribute<decltype(mHour)>>();
	TypeInitializer::RegField(&type, "mMinute", (size_t)(char*)(&sample->mMinute) - (size_t)(char*)sample, sample->mMinute).AddAttribute<SerializableAttribute<decltype(mMinute)>>();
	TypeInitializer::RegField(&type, "mSecond", (size_t)(char*)(&sample->mSecond) - (size_t)(char*)sample, sample->mSecond).AddAttribute<SerializableAttribute<decltype(mSecond)>>();
}

void o2::Curve::InitializeType(o2::Curve* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys);
	TypeInitializer::RegField(&type, "length", (size_t)(char*)(&sample->length) - (size_t)(char*)sample, sample->length);
	TypeInitializer::RegField(&type, "onKeysChanged", (size_t)(char*)(&sample->onKeysChanged) - (size_t)(char*)sample, sample->onKeysChanged);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)sample, sample->mKeys).AddAttribute<SerializableAttribute<decltype(mKeys)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::Curve, float, float>(&type, "Evaluate", &o2::Curve::Evaluate);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, Vector<Vec2F>, float>(&type, "AddKeys", &o2::Curve::AddKeys);
	TypeInitializer::RegFuncParam<Vector<Vec2F>>(funcInfo, "values");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, const Key&>(&type, "AddKey", &o2::Curve::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, float, float, float, float, float, float>(&type, "AddKey", &o2::Curve::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, float, float, float>(&type, "AddKey", &o2::Curve::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, Key, float>(&type, "GetKey", &o2::Curve::GetKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, bool, float>(&type, "RemoveKey", &o2::Curve::RemoveKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void>(&type, "RemoveAllKeys", &o2::Curve::RemoveAllKeys);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, bool, float>(&type, "ContainsKey", &o2::Curve::ContainsKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, const KeysVec&>(&type, "GetKeys", &o2::Curve::GetKeys);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, const KeysVec&>(&type, "SetKeys", &o2::Curve::SetKeys);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, float, float>(&type, "SmoothKey", &o2::Curve::SmoothKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, float>(&type, "Length", &o2::Curve::Length);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, bool>(&type, "IsEmpty", &o2::Curve::IsEmpty);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void>(&type, "UpdateApproximation", &o2::Curve::UpdateApproximation);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, KeysVec>(&type, "GetKeysNonContant", &o2::Curve::GetKeysNonContant);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, const DataNode&>(&type, "OnDeserialized", &o2::Curve::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void>(&type, "InitializeProperties", &o2::Curve::InitializeProperties);
}

void o2::Layout::InitializeType(o2::Layout* sample)
{
	TypeInitializer::RegField(&type, "anchorMin", (size_t)(char*)(&sample->anchorMin) - (size_t)(char*)sample, sample->anchorMin).AddAttribute<SerializableAttribute<decltype(anchorMin)>>();
	TypeInitializer::RegField(&type, "anchorMax", (size_t)(char*)(&sample->anchorMax) - (size_t)(char*)sample, sample->anchorMax).AddAttribute<SerializableAttribute<decltype(anchorMax)>>();
	TypeInitializer::RegField(&type, "offsetMin", (size_t)(char*)(&sample->offsetMin) - (size_t)(char*)sample, sample->offsetMin).AddAttribute<SerializableAttribute<decltype(offsetMin)>>();
	TypeInitializer::RegField(&type, "offsetMax", (size_t)(char*)(&sample->offsetMax) - (size_t)(char*)sample, sample->offsetMax).AddAttribute<SerializableAttribute<decltype(offsetMax)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::Layout, RectF, const RectF&>(&type, "Calculate", &o2::Layout::Calculate);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "source");
}

void o2::Transform::InitializeType(o2::Transform* sample)
{
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position);
	TypeInitializer::RegField(&type, "size", (size_t)(char*)(&sample->size) - (size_t)(char*)sample, sample->size);
	TypeInitializer::RegField(&type, "scale", (size_t)(char*)(&sample->scale) - (size_t)(char*)sample, sample->scale);
	TypeInitializer::RegField(&type, "pivot", (size_t)(char*)(&sample->pivot) - (size_t)(char*)sample, sample->pivot);
	TypeInitializer::RegField(&type, "worldPivot", (size_t)(char*)(&sample->worldPivot) - (size_t)(char*)sample, sample->worldPivot);
	TypeInitializer::RegField(&type, "szPivot", (size_t)(char*)(&sample->szPivot) - (size_t)(char*)sample, sample->szPivot);
	TypeInitializer::RegField(&type, "rect", (size_t)(char*)(&sample->rect) - (size_t)(char*)sample, sample->rect);
	TypeInitializer::RegField(&type, "angle", (size_t)(char*)(&sample->angle) - (size_t)(char*)sample, sample->angle);
	TypeInitializer::RegField(&type, "shear", (size_t)(char*)(&sample->shear) - (size_t)(char*)sample, sample->shear);
	TypeInitializer::RegField(&type, "basis", (size_t)(char*)(&sample->basis) - (size_t)(char*)sample, sample->basis);
	TypeInitializer::RegField(&type, "nonSizedBasis", (size_t)(char*)(&sample->nonSizedBasis) - (size_t)(char*)sample, sample->nonSizedBasis);
	TypeInitializer::RegField(&type, "AABB", (size_t)(char*)(&sample->AABB) - (size_t)(char*)sample, sample->AABB);
	TypeInitializer::RegField(&type, "leftTop", (size_t)(char*)(&sample->leftTop) - (size_t)(char*)sample, sample->leftTop);
	TypeInitializer::RegField(&type, "leftBottom", (size_t)(char*)(&sample->leftBottom) - (size_t)(char*)sample, sample->leftBottom);
	TypeInitializer::RegField(&type, "rightTop", (size_t)(char*)(&sample->rightTop) - (size_t)(char*)sample, sample->rightTop);
	TypeInitializer::RegField(&type, "rightBottom", (size_t)(char*)(&sample->rightBottom) - (size_t)(char*)sample, sample->rightBottom);
	TypeInitializer::RegField(&type, "right", (size_t)(char*)(&sample->right) - (size_t)(char*)sample, sample->right);
	TypeInitializer::RegField(&type, "left", (size_t)(char*)(&sample->left) - (size_t)(char*)sample, sample->left);
	TypeInitializer::RegField(&type, "up", (size_t)(char*)(&sample->up) - (size_t)(char*)sample, sample->up);
	TypeInitializer::RegField(&type, "down", (size_t)(char*)(&sample->down) - (size_t)(char*)sample, sample->down);
	TypeInitializer::RegField(&type, "lookAtPoint", (size_t)(char*)(&sample->lookAtPoint) - (size_t)(char*)sample, sample->lookAtPoint);
	TypeInitializer::RegField(&type, "mPosition", (size_t)(char*)(&sample->mPosition) - (size_t)(char*)sample, sample->mPosition).AddAttribute<SerializableAttribute<decltype(mPosition)>>();
	TypeInitializer::RegField(&type, "mSize", (size_t)(char*)(&sample->mSize) - (size_t)(char*)sample, sample->mSize).AddAttribute<SerializableAttribute<decltype(mSize)>>();
	TypeInitializer::RegField(&type, "mScale", (size_t)(char*)(&sample->mScale) - (size_t)(char*)sample, sample->mScale).AddAttribute<SerializableAttribute<decltype(mScale)>>();
	TypeInitializer::RegField(&type, "mPivot", (size_t)(char*)(&sample->mPivot) - (size_t)(char*)sample, sample->mPivot).AddAttribute<SerializableAttribute<decltype(mPivot)>>();
	TypeInitializer::RegField(&type, "mAngle", (size_t)(char*)(&sample->mAngle) - (size_t)(char*)sample, sample->mAngle).AddAttribute<SerializableAttribute<decltype(mAngle)>>();
	TypeInitializer::RegField(&type, "mShear", (size_t)(char*)(&sample->mShear) - (size_t)(char*)sample, sample->mShear).AddAttribute<SerializableAttribute<decltype(mShear)>>();
	TypeInitializer::RegField(&type, "mTransform", (size_t)(char*)(&sample->mTransform) - (size_t)(char*)sample, sample->mTransform);
	TypeInitializer::RegField(&type, "mNonSizedTransform", (size_t)(char*)(&sample->mNonSizedTransform) - (size_t)(char*)sample, sample->mNonSizedTransform);
	auto funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetPosition", &o2::Transform::SetPosition);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetPosition", &o2::Transform::GetPosition);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetSize", &o2::Transform::SetSize);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetSize", &o2::Transform::GetSize);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetPivot", &o2::Transform::SetPivot);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "pivot");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetPivot", &o2::Transform::GetPivot);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetWorldPivot", &o2::Transform::SetWorldPivot);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "pivot");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetWorldPivot", &o2::Transform::GetWorldPivot);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetSizePivot", &o2::Transform::SetSizePivot);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "relPivot");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetSizePivot", &o2::Transform::GetSizePivot);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const RectF&>(&type, "SetRect", &o2::Transform::SetRect);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, RectF>(&type, "GetRect", &o2::Transform::GetRect);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetScale", &o2::Transform::SetScale);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "scale");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetScale", &o2::Transform::GetScale);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, float>(&type, "SetAngle", &o2::Transform::SetAngle);
	TypeInitializer::RegFuncParam<float>(funcInfo, "rad");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, float>(&type, "GetAngle", &o2::Transform::GetAngle);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, float>(&type, "SetShear", &o2::Transform::SetShear);
	TypeInitializer::RegFuncParam<float>(funcInfo, "shear");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, float>(&type, "GetShear", &o2::Transform::GetShear);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Basis&>(&type, "SetBasis", &o2::Transform::SetBasis);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "basis");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Basis>(&type, "GetBasis", &o2::Transform::GetBasis);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Basis&>(&type, "SetNonSizedBasis", &o2::Transform::SetNonSizedBasis);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "basis");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Basis>(&type, "GetNonSizedBasis", &o2::Transform::GetNonSizedBasis);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const RectF&>(&type, "SetAxisAlignedRect", &o2::Transform::SetAxisAlignedRect);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, RectF>(&type, "GetAxisAlignedRect", &o2::Transform::GetAxisAlignedRect);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetLeftTop", &o2::Transform::SetLeftTop);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetLeftTop", &o2::Transform::GetLeftTop);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetRightTop", &o2::Transform::SetRightTop);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetRightTop", &o2::Transform::GetRightTop);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetLeftBottom", &o2::Transform::SetLeftBottom);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetLeftBottom", &o2::Transform::GetLeftBottom);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetRightBottom", &o2::Transform::SetRightBottom);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetRightBottom", &o2::Transform::GetRightBottom);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetCenter", &o2::Transform::SetCenter);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetCenter", &o2::Transform::GetCenter);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetRight", &o2::Transform::SetRight);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetRight", &o2::Transform::GetRight);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetLeft", &o2::Transform::SetLeft);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetLeft", &o2::Transform::GetLeft);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetUp", &o2::Transform::SetUp);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetUp", &o2::Transform::GetUp);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetDown", &o2::Transform::SetDown);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetDown", &o2::Transform::GetDown);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "LookAt", &o2::Transform::LookAt);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldPoint");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F, const Vec2F&>(&type, "World2LocalPoint", &o2::Transform::World2LocalPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldPoint");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F, const Vec2F&>(&type, "Local2WorldPoint", &o2::Transform::Local2WorldPoint);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "localPoint");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F, const Vec2F&>(&type, "World2LocalDir", &o2::Transform::World2LocalDir);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldDir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F, const Vec2F&>(&type, "Local2WorldDir", &o2::Transform::Local2WorldDir);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "localDir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, bool, const Vec2F&>(&type, "IsPointInside", &o2::Transform::IsPointInside);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void>(&type, "BasisChanged", &o2::Transform::BasisChanged);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const DataNode&>(&type, "OnDeserialized", &o2::Transform::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void>(&type, "UpdateTransform", &o2::Transform::UpdateTransform);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void>(&type, "InitializeProperties", &o2::Transform::InitializeProperties);
}

void o2::AnimatedValue<Vec2F>::Key::InitializeType(o2::AnimatedValue<Vec2F>::Key* sample)
{
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position).AddAttribute<SerializableAttribute<decltype(position)>>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value).AddAttribute<SerializableAttribute<decltype(value)>>();
	TypeInitializer::RegField(&type, "prevSupportValue", (size_t)(char*)(&sample->prevSupportValue) - (size_t)(char*)sample, sample->prevSupportValue).AddAttribute<SerializableAttribute<decltype(prevSupportValue)>>();
	TypeInitializer::RegField(&type, "nextSupportValue", (size_t)(char*)(&sample->nextSupportValue) - (size_t)(char*)sample, sample->nextSupportValue).AddAttribute<SerializableAttribute<decltype(nextSupportValue)>>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)sample, sample->curvePrevCoef).AddAttribute<SerializableAttribute<decltype(curvePrevCoef)>>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)sample, sample->curvePrevCoefPos).AddAttribute<SerializableAttribute<decltype(curvePrevCoefPos)>>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)sample, sample->curveNextCoef).AddAttribute<SerializableAttribute<decltype(curveNextCoef)>>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)sample, sample->curveNextCoefPos).AddAttribute<SerializableAttribute<decltype(curveNextCoefPos)>>();
	TypeInitializer::RegField(&type, "mApproxValues", (size_t)(char*)(&sample->mApproxValues) - (size_t)(char*)sample, sample->mApproxValues);
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)sample, sample->mCurveApproxValues);
	TypeInitializer::RegField(&type, "mApproxLengths", (size_t)(char*)(&sample->mApproxLengths) - (size_t)(char*)sample, sample->mApproxLengths);
	TypeInitializer::RegField(&type, "mApproxTotalLength", (size_t)(char*)(&sample->mApproxTotalLength) - (size_t)(char*)sample, sample->mApproxTotalLength);
}

void o2::Animation::AnimatedValueDef::InitializeType(o2::Animation::AnimatedValueDef* sample)
{
	TypeInitializer::RegField(&type, "mTargetPath", (size_t)(char*)(&sample->mTargetPath) - (size_t)(char*)sample, sample->mTargetPath).AddAttribute<SerializableAttribute<decltype(mTargetPath)>>();
	TypeInitializer::RegField(&type, "mTargetPtr", (size_t)(char*)(&sample->mTargetPtr) - (size_t)(char*)sample, sample->mTargetPtr);
	TypeInitializer::RegField(&type, "mAnimatedValue", (size_t)(char*)(&sample->mAnimatedValue) - (size_t)(char*)sample, sample->mAnimatedValue).AddAttribute<SerializableAttribute<decltype(mAnimatedValue)>>();
}

void o2::Asset::IMetaInfo::InitializeType(o2::Asset::IMetaInfo* sample)
{
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId).AddAttribute<SerializableAttribute<decltype(mId)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::Asset::IMetaInfo, Type::Id>(&type, "GetAssetType", &o2::Asset::IMetaInfo::GetAssetType);
	funcInfo = TypeInitializer::RegFunction<o2::Asset::IMetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::Asset::IMetaInfo::IsEqual);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
	funcInfo = TypeInitializer::RegFunction<o2::Asset::IMetaInfo, AssetId>(&type, "ID", &o2::Asset::IMetaInfo::ID);
}

void o2::AssetTree::AssetNode::InitializeType(o2::AssetTree::AssetNode* sample)
{
	TypeInitializer::RegField(&type, "mMeta", (size_t)(char*)(&sample->mMeta) - (size_t)(char*)sample, sample->mMeta);
	TypeInitializer::RegField(&type, "mTime", (size_t)(char*)(&sample->mTime) - (size_t)(char*)sample, sample->mTime);
}

void o2::AtlasAsset::PlatformMeta::InitializeType(o2::AtlasAsset::PlatformMeta* sample)
{
	TypeInitializer::RegField(&type, "mMaxSize", (size_t)(char*)(&sample->mMaxSize) - (size_t)(char*)sample, sample->mMaxSize).AddAttribute<SerializableAttribute<decltype(mMaxSize)>>();
	TypeInitializer::RegField(&type, "mFormat", (size_t)(char*)(&sample->mFormat) - (size_t)(char*)sample, sample->mFormat).AddAttribute<SerializableAttribute<decltype(mFormat)>>();
}

void o2::AtlasAsset::MetaInfo::InitializeType(o2::AtlasAsset::MetaInfo* sample)
{
	TypeInitializer::RegField(&type, "mIOS", (size_t)(char*)(&sample->mIOS) - (size_t)(char*)sample, sample->mIOS).AddAttribute<SerializableAttribute<decltype(mIOS)>>();
	TypeInitializer::RegField(&type, "mAndroid", (size_t)(char*)(&sample->mAndroid) - (size_t)(char*)sample, sample->mAndroid).AddAttribute<SerializableAttribute<decltype(mAndroid)>>();
	TypeInitializer::RegField(&type, "mMacOS", (size_t)(char*)(&sample->mMacOS) - (size_t)(char*)sample, sample->mMacOS).AddAttribute<SerializableAttribute<decltype(mMacOS)>>();
	TypeInitializer::RegField(&type, "mWindows", (size_t)(char*)(&sample->mWindows) - (size_t)(char*)sample, sample->mWindows).AddAttribute<SerializableAttribute<decltype(mWindows)>>();
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)sample, sample->mBorder).AddAttribute<SerializableAttribute<decltype(mBorder)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::AtlasAsset::MetaInfo::GetAssetType);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::MetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::AtlasAsset::MetaInfo::IsEqual);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
}

void o2::AtlasAsset::Page::InitializeType(o2::AtlasAsset::Page* sample)
{
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId).AddAttribute<SerializableAttribute<decltype(mId)>>();
	TypeInitializer::RegField(&type, "mSize", (size_t)(char*)(&sample->mSize) - (size_t)(char*)sample, sample->mSize).AddAttribute<SerializableAttribute<decltype(mSize)>>();
	TypeInitializer::RegField(&type, "mImagesRects", (size_t)(char*)(&sample->mImagesRects) - (size_t)(char*)sample, sample->mImagesRects).AddAttribute<SerializableAttribute<decltype(mImagesRects)>>();
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner);
	auto funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, UInt>(&type, "ID", &o2::AtlasAsset::Page::ID);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, Vec2I>(&type, "Size", &o2::AtlasAsset::Page::Size);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, TextureRef>(&type, "GetTextureRef", &o2::AtlasAsset::Page::GetTextureRef);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, String>(&type, "GetTextureFileName", &o2::AtlasAsset::Page::GetTextureFileName);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, const Dictionary<AssetId, RectI>&>(&type, "ImagesRects", &o2::AtlasAsset::Page::ImagesRects);
}

void o2::BinaryAsset::MetaInfo::InitializeType(o2::BinaryAsset::MetaInfo* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::BinaryAsset::MetaInfo::GetAssetType);
}

void o2::BitmapFontAsset::MetaInfo::InitializeType(o2::BitmapFontAsset::MetaInfo* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::BitmapFontAsset::MetaInfo::GetAssetType);
}

void o2::FolderAsset::MetaInfo::InitializeType(o2::FolderAsset::MetaInfo* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::FolderAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::FolderAsset::MetaInfo::GetAssetType);
}

void o2::ImageAsset::PlatformMeta::InitializeType(o2::ImageAsset::PlatformMeta* sample)
{
	TypeInitializer::RegField(&type, "mMaxSize", (size_t)(char*)(&sample->mMaxSize) - (size_t)(char*)sample, sample->mMaxSize).AddAttribute<SerializableAttribute<decltype(mMaxSize)>>();
	TypeInitializer::RegField(&type, "mScale", (size_t)(char*)(&sample->mScale) - (size_t)(char*)sample, sample->mScale).AddAttribute<SerializableAttribute<decltype(mScale)>>();
	TypeInitializer::RegField(&type, "mFormat", (size_t)(char*)(&sample->mFormat) - (size_t)(char*)sample, sample->mFormat).AddAttribute<SerializableAttribute<decltype(mFormat)>>();
}

void o2::ImageAsset::MetaInfo::InitializeType(o2::ImageAsset::MetaInfo* sample)
{
	TypeInitializer::RegField(&type, "mAtlasId", (size_t)(char*)(&sample->mAtlasId) - (size_t)(char*)sample, sample->mAtlasId).AddAttribute<SerializableAttribute<decltype(mAtlasId)>>();
	TypeInitializer::RegField(&type, "mIOS", (size_t)(char*)(&sample->mIOS) - (size_t)(char*)sample, sample->mIOS).AddAttribute<SerializableAttribute<decltype(mIOS)>>();
	TypeInitializer::RegField(&type, "mAndroid", (size_t)(char*)(&sample->mAndroid) - (size_t)(char*)sample, sample->mAndroid).AddAttribute<SerializableAttribute<decltype(mAndroid)>>();
	TypeInitializer::RegField(&type, "mMacOS", (size_t)(char*)(&sample->mMacOS) - (size_t)(char*)sample, sample->mMacOS).AddAttribute<SerializableAttribute<decltype(mMacOS)>>();
	TypeInitializer::RegField(&type, "mWindows", (size_t)(char*)(&sample->mWindows) - (size_t)(char*)sample, sample->mWindows).AddAttribute<SerializableAttribute<decltype(mWindows)>>();
	TypeInitializer::RegField(&type, "mSliceBorder", (size_t)(char*)(&sample->mSliceBorder) - (size_t)(char*)sample, sample->mSliceBorder).AddAttribute<SerializableAttribute<decltype(mSliceBorder)>>();
	TypeInitializer::RegField(&type, "mDefaultMode", (size_t)(char*)(&sample->mDefaultMode) - (size_t)(char*)sample, sample->mDefaultMode).AddAttribute<SerializableAttribute<decltype(mDefaultMode)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::ImageAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::ImageAsset::MetaInfo::GetAssetType);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset::MetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::ImageAsset::MetaInfo::IsEqual);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
}

void o2::VectorFontAsset::MetaInfo::InitializeType(o2::VectorFontAsset::MetaInfo* sample)
{
	TypeInitializer::RegField(&type, "mEffects", (size_t)(char*)(&sample->mEffects) - (size_t)(char*)sample, sample->mEffects).AddAttribute<SerializableAttribute<decltype(mEffects)>>();
	auto funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::VectorFontAsset::MetaInfo::GetAssetType);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset::MetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::VectorFontAsset::MetaInfo::IsEqual);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
}

void o2::AtlasAssetConverter::Image::InitializeType(o2::AtlasAssetConverter::Image* sample)
{
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId).AddAttribute<SerializableAttribute<decltype(mId)>>();
	TypeInitializer::RegField(&type, "mTime", (size_t)(char*)(&sample->mTime) - (size_t)(char*)sample, sample->mTime).AddAttribute<SerializableAttribute<decltype(mTime)>>();
}

void o2::VectorFont::Effect::InitializeType(o2::VectorFont::Effect* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::VectorFont::Effect, void, Bitmap*>(&type, "Process", &o2::VectorFont::Effect::Process);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::VectorFont::Effect, Vec2I>(&type, "GetSizeExtend", &o2::VectorFont::Effect::GetSizeExtend);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFont::Effect, bool, Effect*>(&type, "IsEqual", &o2::VectorFont::Effect::IsEqual);
	TypeInitializer::RegFuncParam<Effect*>(funcInfo, "other");
}

void o2::Scene::Layer::InitializeType(o2::Scene::Layer* sample)
{
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name).AddAttribute<SerializableAttribute<decltype(name)>>();
	TypeInitializer::RegField(&type, "actors", (size_t)(char*)(&sample->actors) - (size_t)(char*)sample, sample->actors);
	TypeInitializer::RegField(&type, "enabledActors", (size_t)(char*)(&sample->enabledActors) - (size_t)(char*)sample, sample->enabledActors);
	TypeInitializer::RegField(&type, "drawables", (size_t)(char*)(&sample->drawables) - (size_t)(char*)sample, sample->drawables);
	TypeInitializer::RegField(&type, "enabledDrawables", (size_t)(char*)(&sample->enabledDrawables) - (size_t)(char*)sample, sample->enabledDrawables);
	auto funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "RegDrawableComponent", &o2::Scene::Layer::RegDrawableComponent);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "UnregDrawableComponent", &o2::Scene::Layer::UnregDrawableComponent);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "ComponentDepthChanged", &o2::Scene::Layer::ComponentDepthChanged);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "ComponentEnabled", &o2::Scene::Layer::ComponentEnabled);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "ComponentDisabled", &o2::Scene::Layer::ComponentDisabled);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
}

void o2::UIContextMenu::Item::InitializeType(o2::UIContextMenu::Item* sample)
{
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)sample, sample->text).AddAttribute<SerializableAttribute<decltype(text)>>();
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)sample, sample->shortcut).AddAttribute<SerializableAttribute<decltype(shortcut)>>();
	TypeInitializer::RegField(&type, "icon", (size_t)(char*)(&sample->icon) - (size_t)(char*)sample, sample->icon).AddAttribute<SerializableAttribute<decltype(icon)>>();
	TypeInitializer::RegField(&type, "subItems", (size_t)(char*)(&sample->subItems) - (size_t)(char*)sample, sample->subItems).AddAttribute<SerializableAttribute<decltype(subItems)>>();
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)sample, sample->onClick);
}

void o2::UIMenuPanel::Item::InitializeType(o2::UIMenuPanel::Item* sample)
{
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)sample, sample->text).AddAttribute<SerializableAttribute<decltype(text)>>();
	TypeInitializer::RegField(&type, "subItems", (size_t)(char*)(&sample->subItems) - (size_t)(char*)sample, sample->subItems).AddAttribute<SerializableAttribute<decltype(subItems)>>();
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)sample, sample->onClick);
}

void o2::Curve::Key::InitializeType(o2::Curve::Key* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value).AddAttribute<SerializableAttribute<decltype(value)>>();
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position).AddAttribute<SerializableAttribute<decltype(position)>>();
	TypeInitializer::RegField(&type, "leftCoef", (size_t)(char*)(&sample->leftCoef) - (size_t)(char*)sample, sample->leftCoef).AddAttribute<SerializableAttribute<decltype(leftCoef)>>();
	TypeInitializer::RegField(&type, "leftCoefPosition", (size_t)(char*)(&sample->leftCoefPosition) - (size_t)(char*)sample, sample->leftCoefPosition).AddAttribute<SerializableAttribute<decltype(leftCoefPosition)>>();
	TypeInitializer::RegField(&type, "rightCoef", (size_t)(char*)(&sample->rightCoef) - (size_t)(char*)sample, sample->rightCoef).AddAttribute<SerializableAttribute<decltype(rightCoef)>>();
	TypeInitializer::RegField(&type, "rightCoefPosition", (size_t)(char*)(&sample->rightCoefPosition) - (size_t)(char*)sample, sample->rightCoefPosition).AddAttribute<SerializableAttribute<decltype(rightCoefPosition)>>();
	TypeInitializer::RegField(&type, "mApproxValues", (size_t)(char*)(&sample->mApproxValues) - (size_t)(char*)sample, sample->mApproxValues);
}

void ::EditorConfig::GlobalConfig::InitializeType(::EditorConfig::GlobalConfig* sample)
{
	TypeInitializer::RegField(&type, "mLastOpenedProjectpath", (size_t)(char*)(&sample->mLastOpenedProjectpath) - (size_t)(char*)sample, sample->mLastOpenedProjectpath).AddAttribute<SerializableAttribute<decltype(mLastOpenedProjectpath)>>();
	TypeInitializer::RegField(&type, "mDefaultLayout", (size_t)(char*)(&sample->mDefaultLayout) - (size_t)(char*)sample, sample->mDefaultLayout).AddAttribute<SerializableAttribute<decltype(mDefaultLayout)>>();
	TypeInitializer::RegField(&type, "mAvailableLayouts", (size_t)(char*)(&sample->mAvailableLayouts) - (size_t)(char*)sample, sample->mAvailableLayouts).AddAttribute<SerializableAttribute<decltype(mAvailableLayouts)>>();
}

void ::EditorConfig::ProjectConfig::InitializeType(::EditorConfig::ProjectConfig* sample)
{
	TypeInitializer::RegField(&type, "mWindowSize", (size_t)(char*)(&sample->mWindowSize) - (size_t)(char*)sample, sample->mWindowSize).AddAttribute<SerializableAttribute<decltype(mWindowSize)>>();
	TypeInitializer::RegField(&type, "mWindowPosition", (size_t)(char*)(&sample->mWindowPosition) - (size_t)(char*)sample, sample->mWindowPosition).AddAttribute<SerializableAttribute<decltype(mWindowPosition)>>();
	TypeInitializer::RegField(&type, "mMaximized", (size_t)(char*)(&sample->mMaximized) - (size_t)(char*)sample, sample->mMaximized).AddAttribute<SerializableAttribute<decltype(mMaximized)>>();
	TypeInitializer::RegField(&type, "mLayout", (size_t)(char*)(&sample->mLayout) - (size_t)(char*)sample, sample->mLayout).AddAttribute<SerializableAttribute<decltype(mLayout)>>();
}

void ::EditorDeleteActorsAction::ActorInfo::InitializeType(::EditorDeleteActorsAction::ActorInfo* sample)
{
	TypeInitializer::RegField(&type, "actor", (size_t)(char*)(&sample->actor) - (size_t)(char*)sample, sample->actor).AddAttribute<SerializableAttribute<decltype(actor)>>();
	TypeInitializer::RegField(&type, "parentId", (size_t)(char*)(&sample->parentId) - (size_t)(char*)sample, sample->parentId).AddAttribute<SerializableAttribute<decltype(parentId)>>();
	TypeInitializer::RegField(&type, "prevActorId", (size_t)(char*)(&sample->prevActorId) - (size_t)(char*)sample, sample->prevActorId).AddAttribute<SerializableAttribute<decltype(prevActorId)>>();
	TypeInitializer::RegField(&type, "idx", (size_t)(char*)(&sample->idx) - (size_t)(char*)sample, sample->idx).AddAttribute<SerializableAttribute<decltype(idx)>>();
}

void ::WindowsLayout::WindowDockPlace::InitializeType(::WindowsLayout::WindowDockPlace* sample)
{
	TypeInitializer::RegField(&type, "anchors", (size_t)(char*)(&sample->anchors) - (size_t)(char*)sample, sample->anchors).AddAttribute<SerializableAttribute<decltype(anchors)>>();
	TypeInitializer::RegField(&type, "windows", (size_t)(char*)(&sample->windows) - (size_t)(char*)sample, sample->windows).AddAttribute<SerializableAttribute<decltype(windows)>>();
	TypeInitializer::RegField(&type, "childs", (size_t)(char*)(&sample->childs) - (size_t)(char*)sample, sample->childs).AddAttribute<SerializableAttribute<decltype(childs)>>();
	auto funcInfo = TypeInitializer::RegFunction<::WindowsLayout::WindowDockPlace, void, UIWidget*>(&type, "RetrieveLayout", &::WindowsLayout::WindowDockPlace::RetrieveLayout);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
}

void o2::AnimatedValue<RectF>::InitializeType(o2::AnimatedValue<RectF>* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)sample, sample->target);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)sample, sample->targetDelegate);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)sample, sample->targetProperty);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)sample, sample->mKeys).AddAttribute<SerializableAttribute<decltype(mKeys)>>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)sample, sample->mTargetDelegate);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)sample, sample->mTargetProperty);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, RectF*>(&type, "SetTarget", &o2::AnimatedValue<RectF>::SetTarget);
	TypeInitializer::RegFuncParam<RectF*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, RectF*, const Function<void()>&>(&type, "SetTarget", &o2::AnimatedValue<RectF>::SetTarget);
	TypeInitializer::RegFuncParam<RectF*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::AnimatedValue<RectF>::SetTargetDelegate);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, Setter<RectF>*>(&type, "SetTargetProperty", &o2::AnimatedValue<RectF>::SetTargetProperty);
	TypeInitializer::RegFuncParam<Setter<RectF>*>(funcInfo, "setter");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, RectF>(&type, "GetValue", &o2::AnimatedValue<RectF>::GetValue);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, RectF, float>(&type, "GetValue", &o2::AnimatedValue<RectF>::GetValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, Vector<Key>, float>(&type, "AddKeys", &o2::AnimatedValue<RectF>::AddKeys);
	TypeInitializer::RegFuncParam<Vector<Key>>(funcInfo, "keys");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const Key&>(&type, "AddKey", &o2::AnimatedValue<RectF>::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const Key&, float>(&type, "AddKey", &o2::AnimatedValue<RectF>::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const Key&, float>(&type, "AddSmoothKey", &o2::AnimatedValue<RectF>::AddSmoothKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, float, const RectF&, float, float, float, float>(&type, "AddKey", &o2::AnimatedValue<RectF>::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, float, const RectF&, float>(&type, "AddKey", &o2::AnimatedValue<RectF>::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, Key, float>(&type, "GetKey", &o2::AnimatedValue<RectF>::GetKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, bool, float>(&type, "RemoveKey", &o2::AnimatedValue<RectF>::RemoveKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void>(&type, "RemoveAllKeys", &o2::AnimatedValue<RectF>::RemoveAllKeys);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, bool, float>(&type, "ContainsKey", &o2::AnimatedValue<RectF>::ContainsKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, const KeysVec&>(&type, "GetKeys", &o2::AnimatedValue<RectF>::GetKeys);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const KeysVec&>(&type, "SetKeys", &o2::AnimatedValue<RectF>::SetKeys);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, float, float>(&type, "SmoothKey", &o2::AnimatedValue<RectF>::SmoothKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void>(&type, "Evaluate", &o2::AnimatedValue<RectF>::Evaluate);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, RectF, float>(&type, "Evaluate", &o2::AnimatedValue<RectF>::Evaluate);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, KeysVec>(&type, "GetKeysNonContant", &o2::AnimatedValue<RectF>::GetKeysNonContant);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void>(&type, "UpdateApproximation", &o2::AnimatedValue<RectF>::UpdateApproximation);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const DataNode&>(&type, "OnDeserialized", &o2::AnimatedValue<RectF>::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, void*>(&type, "SetTargetVoid", &o2::AnimatedValue<RectF>::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::AnimatedValue<RectF>::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, void*>(&type, "SetTargetPropertyVoid", &o2::AnimatedValue<RectF>::SetTargetPropertyVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::AnimatedValue<RectF>::RegInAnimatable);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void>(&type, "InitializeProperties", &o2::AnimatedValue<RectF>::InitializeProperties);
}

void o2::AnimatedValue<RectF>::Key::InitializeType(o2::AnimatedValue<RectF>::Key* sample)
{
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position).AddAttribute<SerializableAttribute<decltype(position)>>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value).AddAttribute<SerializableAttribute<decltype(value)>>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)sample, sample->curvePrevCoef).AddAttribute<SerializableAttribute<decltype(curvePrevCoef)>>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)sample, sample->curvePrevCoefPos).AddAttribute<SerializableAttribute<decltype(curvePrevCoefPos)>>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)sample, sample->curveNextCoef).AddAttribute<SerializableAttribute<decltype(curveNextCoef)>>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)sample, sample->curveNextCoefPos).AddAttribute<SerializableAttribute<decltype(curveNextCoefPos)>>();
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)sample, sample->mCurveApproxValues);
}

void o2::AnimatedValue<Color4>::InitializeType(o2::AnimatedValue<Color4>* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)sample, sample->target);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)sample, sample->targetDelegate);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)sample, sample->targetProperty);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)sample, sample->mKeys).AddAttribute<SerializableAttribute<decltype(mKeys)>>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)sample, sample->mTargetDelegate);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)sample, sample->mTargetProperty);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, Color4*>(&type, "SetTarget", &o2::AnimatedValue<Color4>::SetTarget);
	TypeInitializer::RegFuncParam<Color4*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, Color4*, const Function<void()>&>(&type, "SetTarget", &o2::AnimatedValue<Color4>::SetTarget);
	TypeInitializer::RegFuncParam<Color4*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::AnimatedValue<Color4>::SetTargetDelegate);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, Setter<Color4>*>(&type, "SetTargetProperty", &o2::AnimatedValue<Color4>::SetTargetProperty);
	TypeInitializer::RegFuncParam<Setter<Color4>*>(funcInfo, "setter");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, Color4>(&type, "GetValue", &o2::AnimatedValue<Color4>::GetValue);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, Color4, float>(&type, "GetValue", &o2::AnimatedValue<Color4>::GetValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, Vector<Key>, float>(&type, "AddKeys", &o2::AnimatedValue<Color4>::AddKeys);
	TypeInitializer::RegFuncParam<Vector<Key>>(funcInfo, "keys");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const Key&>(&type, "AddKey", &o2::AnimatedValue<Color4>::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const Key&, float>(&type, "AddKey", &o2::AnimatedValue<Color4>::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const Key&, float>(&type, "AddSmoothKey", &o2::AnimatedValue<Color4>::AddSmoothKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, float, const Color4&, float, float, float, float>(&type, "AddKey", &o2::AnimatedValue<Color4>::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, float, const Color4&, float>(&type, "AddKey", &o2::AnimatedValue<Color4>::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, Key, float>(&type, "GetKey", &o2::AnimatedValue<Color4>::GetKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, bool, float>(&type, "RemoveKey", &o2::AnimatedValue<Color4>::RemoveKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void>(&type, "RemoveAllKeys", &o2::AnimatedValue<Color4>::RemoveAllKeys);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, bool, float>(&type, "ContainsKey", &o2::AnimatedValue<Color4>::ContainsKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, const KeysVec&>(&type, "GetKeys", &o2::AnimatedValue<Color4>::GetKeys);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const KeysVec&>(&type, "SetKeys", &o2::AnimatedValue<Color4>::SetKeys);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, float, float>(&type, "SmoothKey", &o2::AnimatedValue<Color4>::SmoothKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void>(&type, "Evaluate", &o2::AnimatedValue<Color4>::Evaluate);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, Color4, float>(&type, "Evaluate", &o2::AnimatedValue<Color4>::Evaluate);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, KeysVec>(&type, "GetKeysNonContant", &o2::AnimatedValue<Color4>::GetKeysNonContant);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void>(&type, "UpdateApproximation", &o2::AnimatedValue<Color4>::UpdateApproximation);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const DataNode&>(&type, "OnDeserialized", &o2::AnimatedValue<Color4>::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, void*>(&type, "SetTargetVoid", &o2::AnimatedValue<Color4>::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::AnimatedValue<Color4>::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, void*>(&type, "SetTargetPropertyVoid", &o2::AnimatedValue<Color4>::SetTargetPropertyVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::AnimatedValue<Color4>::RegInAnimatable);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void>(&type, "InitializeProperties", &o2::AnimatedValue<Color4>::InitializeProperties);
}

void o2::AnimatedValue<Color4>::Key::InitializeType(o2::AnimatedValue<Color4>::Key* sample)
{
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position).AddAttribute<SerializableAttribute<decltype(position)>>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value).AddAttribute<SerializableAttribute<decltype(value)>>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)sample, sample->curvePrevCoef).AddAttribute<SerializableAttribute<decltype(curvePrevCoef)>>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)sample, sample->curvePrevCoefPos).AddAttribute<SerializableAttribute<decltype(curvePrevCoefPos)>>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)sample, sample->curveNextCoef).AddAttribute<SerializableAttribute<decltype(curveNextCoef)>>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)sample, sample->curveNextCoefPos).AddAttribute<SerializableAttribute<decltype(curveNextCoefPos)>>();
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)sample, sample->mCurveApproxValues);
}

void o2::AnimatedValue<bool>::InitializeType(o2::AnimatedValue<bool>* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)sample, sample->target);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)sample, sample->targetDelegate);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)sample, sample->targetProperty);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)sample, sample->mKeys).AddAttribute<SerializableAttribute<decltype(mKeys)>>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)sample, sample->mTargetDelegate);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)sample, sample->mTargetProperty);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, bool*>(&type, "SetTarget", &o2::AnimatedValue<bool>::SetTarget);
	TypeInitializer::RegFuncParam<bool*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, bool*, const Function<void()>&>(&type, "SetTarget", &o2::AnimatedValue<bool>::SetTarget);
	TypeInitializer::RegFuncParam<bool*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::AnimatedValue<bool>::SetTargetDelegate);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, Setter<bool>*>(&type, "SetTargetProperty", &o2::AnimatedValue<bool>::SetTargetProperty);
	TypeInitializer::RegFuncParam<Setter<bool>*>(funcInfo, "setter");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, bool>(&type, "GetValue", &o2::AnimatedValue<bool>::GetValue);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, bool, float>(&type, "GetValue", &o2::AnimatedValue<bool>::GetValue);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, Vector<Key>, float>(&type, "AddKeys", &o2::AnimatedValue<bool>::AddKeys);
	TypeInitializer::RegFuncParam<Vector<Key>>(funcInfo, "keys");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const Key&>(&type, "AddKey", &o2::AnimatedValue<bool>::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const Key&, float>(&type, "AddKey", &o2::AnimatedValue<bool>::AddKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const Key&, float>(&type, "AddSmoothKey", &o2::AnimatedValue<bool>::AddSmoothKey);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, float, const bool&, float, float, float, float>(&type, "AddKey", &o2::AnimatedValue<bool>::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const bool&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, float, const bool&, float>(&type, "AddKey", &o2::AnimatedValue<bool>::AddKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const bool&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, Key, float>(&type, "GetKey", &o2::AnimatedValue<bool>::GetKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, bool, float>(&type, "RemoveKey", &o2::AnimatedValue<bool>::RemoveKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void>(&type, "RemoveAllKeys", &o2::AnimatedValue<bool>::RemoveAllKeys);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, bool, float>(&type, "ContainsKey", &o2::AnimatedValue<bool>::ContainsKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, const KeysVec&>(&type, "GetKeys", &o2::AnimatedValue<bool>::GetKeys);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const KeysVec&>(&type, "SetKeys", &o2::AnimatedValue<bool>::SetKeys);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, float, float>(&type, "SmoothKey", &o2::AnimatedValue<bool>::SmoothKey);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void>(&type, "Evaluate", &o2::AnimatedValue<bool>::Evaluate);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, bool, float>(&type, "Evaluate", &o2::AnimatedValue<bool>::Evaluate);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, KeysVec>(&type, "GetKeysNonContant", &o2::AnimatedValue<bool>::GetKeysNonContant);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void>(&type, "UpdateApproximation", &o2::AnimatedValue<bool>::UpdateApproximation);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const DataNode&>(&type, "OnDeserialized", &o2::AnimatedValue<bool>::OnDeserialized);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, void*>(&type, "SetTargetVoid", &o2::AnimatedValue<bool>::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::AnimatedValue<bool>::SetTargetVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, void*>(&type, "SetTargetPropertyVoid", &o2::AnimatedValue<bool>::SetTargetPropertyVoid);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::AnimatedValue<bool>::RegInAnimatable);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void>(&type, "InitializeProperties", &o2::AnimatedValue<bool>::InitializeProperties);
}

void o2::AnimatedValue<bool>::Key::InitializeType(o2::AnimatedValue<bool>::Key* sample)
{
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position).AddAttribute<SerializableAttribute<decltype(position)>>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value).AddAttribute<SerializableAttribute<decltype(value)>>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)sample, sample->curvePrevCoef).AddAttribute<SerializableAttribute<decltype(curvePrevCoef)>>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)sample, sample->curvePrevCoefPos).AddAttribute<SerializableAttribute<decltype(curvePrevCoefPos)>>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)sample, sample->curveNextCoef).AddAttribute<SerializableAttribute<decltype(curveNextCoef)>>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)sample, sample->curveNextCoefPos).AddAttribute<SerializableAttribute<decltype(curveNextCoefPos)>>();
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)sample, sample->mCurveApproxValues);
}


// Registering all types
void RegReflectionTypes()
{
	// Initialize types
	o2::Reflection::InitializeType<::UIAssetsIconsScrollArea>("::UIAssetsIconsScrollArea");
	o2::Reflection::InitializeType<::AssetsWindow>("::AssetsWindow");
	o2::Reflection::InitializeType<::UIAssetsFoldersTree>("::UIAssetsFoldersTree");
	o2::Reflection::InitializeType<::UIAssetIcon>("::UIAssetIcon");
	o2::Reflection::InitializeType<::EditorConfig>("::EditorConfig");
	o2::Reflection::InitializeType<::EditorActorsTransformAction>("::EditorActorsTransformAction");
	o2::Reflection::InitializeType<::EditorCreateActorsAction>("::EditorCreateActorsAction");
	o2::Reflection::InitializeType<::EditorDeleteActorsAction>("::EditorDeleteActorsAction");
	o2::Reflection::InitializeType<::EditorEnableAction>("::EditorEnableAction");
	o2::Reflection::InitializeType<::IEditorAction>("::IEditorAction");
	o2::Reflection::InitializeType<::EditorLockAction>("::EditorLockAction");
	o2::Reflection::InitializeType<::EditorReparentActorsAction>("::EditorReparentActorsAction");
	o2::Reflection::InitializeType<::EditorSelectionAction>("::EditorSelectionAction");
	o2::Reflection::InitializeType<::EditorFrameTool>("::EditorFrameTool");
	o2::Reflection::InitializeType<::IEditorTool>("::IEditorTool");
	o2::Reflection::InitializeType<::EditorMoveTool>("::EditorMoveTool");
	o2::Reflection::InitializeType<::EditorRotateTool>("::EditorRotateTool");
	o2::Reflection::InitializeType<::EditorScaleTool>("::EditorScaleTool");
	o2::Reflection::InitializeType<::EditorSelectionTool>("::EditorSelectionTool");
	o2::Reflection::InitializeType<::IEditorWindow>("::IEditorWindow");
	o2::Reflection::InitializeType<::UIDockableWindow>("::UIDockableWindow");
	o2::Reflection::InitializeType<::UIDockWindowPlace>("::UIDockWindowPlace");
	o2::Reflection::InitializeType<::WindowsLayout>("::WindowsLayout");
	o2::Reflection::InitializeType<::LogWindow>("::LogWindow");
	o2::Reflection::InitializeType<::SceneEditScreen>("::SceneEditScreen");
	o2::Reflection::InitializeType<::SceneEditWidget>("::SceneEditWidget");
	o2::Reflection::InitializeType<::SceneWindow>("::SceneWindow");
	o2::Reflection::InitializeType<::TreeWindow>("::TreeWindow");
	o2::Reflection::InitializeType<o2::Animatable>("o2::Animatable");
	o2::Reflection::InitializeType<o2::AnimatedValue<float>>("o2::AnimatedValue<float>");
	o2::Reflection::InitializeType<o2::IAnimatedValue>("o2::IAnimatedValue");
	o2::Reflection::InitializeType<o2::AnimatedValue<Vec2F>>("o2::AnimatedValue<Vec2F>");
	o2::Reflection::InitializeType<o2::Animation>("o2::Animation");
	o2::Reflection::InitializeType<o2::AnimationMask>("o2::AnimationMask");
	o2::Reflection::InitializeType<o2::AnimationState>("o2::AnimationState");
	o2::Reflection::InitializeType<o2::IAnimation>("o2::IAnimation");
	o2::Reflection::InitializeType<o2::Asset>("o2::Asset");
	o2::Reflection::InitializeType<o2::AssetInfo>("o2::AssetInfo");
	o2::Reflection::InitializeType<o2::AtlasAsset>("o2::AtlasAsset");
	o2::Reflection::InitializeType<o2::BinaryAsset>("o2::BinaryAsset");
	o2::Reflection::InitializeType<o2::BitmapFontAsset>("o2::BitmapFontAsset");
	o2::Reflection::InitializeType<o2::FolderAsset>("o2::FolderAsset");
	o2::Reflection::InitializeType<o2::ImageAsset>("o2::ImageAsset");
	o2::Reflection::InitializeType<o2::VectorFontAsset>("o2::VectorFontAsset");
	o2::Reflection::InitializeType<o2::AtlasAssetConverter>("o2::AtlasAssetConverter");
	o2::Reflection::InitializeType<o2::FolderAssetConverter>("o2::FolderAssetConverter");
	o2::Reflection::InitializeType<o2::IAssetConverter>("o2::IAssetConverter");
	o2::Reflection::InitializeType<o2::ImageAssetConverter>("o2::ImageAssetConverter");
	o2::Reflection::InitializeType<o2::StdAssetConverter>("o2::StdAssetConverter");
	o2::Reflection::InitializeType<o2::ProjectConfig>("o2::ProjectConfig");
	o2::Reflection::InitializeType<o2::Camera>("o2::Camera");
	o2::Reflection::InitializeType<o2::IRectDrawable>("o2::IRectDrawable");
	o2::Reflection::InitializeType<o2::Sprite>("o2::Sprite");
	o2::Reflection::InitializeType<o2::Text>("o2::Text");
	o2::Reflection::InitializeType<o2::FontStrokeEffect>("o2::FontStrokeEffect");
	o2::Reflection::InitializeType<o2::FontGradientEffect>("o2::FontGradientEffect");
	o2::Reflection::InitializeType<o2::FontColorEffect>("o2::FontColorEffect");
	o2::Reflection::InitializeType<o2::FontShadowEffect>("o2::FontShadowEffect");
	o2::Reflection::InitializeType<o2::Actor>("o2::Actor");
	o2::Reflection::InitializeType<o2::ActorTransform>("o2::ActorTransform");
	o2::Reflection::InitializeType<o2::Component>("o2::Component");
	o2::Reflection::InitializeType<o2::DrawableComponent>("o2::DrawableComponent");
	o2::Reflection::InitializeType<o2::ImageComponent>("o2::ImageComponent");
	o2::Reflection::InitializeType<o2::UIButton>("o2::UIButton");
	o2::Reflection::InitializeType<o2::UIContextMenuItem>("o2::UIContextMenuItem");
	o2::Reflection::InitializeType<o2::UIContextMenu>("o2::UIContextMenu");
	o2::Reflection::InitializeType<o2::UICustomDropDown>("o2::UICustomDropDown");
	o2::Reflection::InitializeType<o2::UICustomList>("o2::UICustomList");
	o2::Reflection::InitializeType<o2::UIDropDown>("o2::UIDropDown");
	o2::Reflection::InitializeType<o2::UIEditBox>("o2::UIEditBox");
	o2::Reflection::InitializeType<o2::UIGridLayout>("o2::UIGridLayout");
	o2::Reflection::InitializeType<o2::UIHorizontalLayout>("o2::UIHorizontalLayout");
	o2::Reflection::InitializeType<o2::UIHorizontalProgress>("o2::UIHorizontalProgress");
	o2::Reflection::InitializeType<o2::UIHorizontalScrollBar>("o2::UIHorizontalScrollBar");
	o2::Reflection::InitializeType<o2::UILabel>("o2::UILabel");
	o2::Reflection::InitializeType<o2::UIList>("o2::UIList");
	o2::Reflection::InitializeType<o2::UILongList>("o2::UILongList");
	o2::Reflection::InitializeType<o2::UIMenuPanel>("o2::UIMenuPanel");
	o2::Reflection::InitializeType<o2::UIScrollArea>("o2::UIScrollArea");
	o2::Reflection::InitializeType<o2::UIToggle>("o2::UIToggle");
	o2::Reflection::InitializeType<o2::UITreeNode>("o2::UITreeNode");
	o2::Reflection::InitializeType<o2::UITree>("o2::UITree");
	o2::Reflection::InitializeType<o2::UIVerticalLayout>("o2::UIVerticalLayout");
	o2::Reflection::InitializeType<o2::UIVerticalProgress>("o2::UIVerticalProgress");
	o2::Reflection::InitializeType<o2::UIVerticalScrollBar>("o2::UIVerticalScrollBar");
	o2::Reflection::InitializeType<o2::UIWidget>("o2::UIWidget");
	o2::Reflection::InitializeType<o2::UIWidgetLayer>("o2::UIWidgetLayer");
	o2::Reflection::InitializeType<o2::UIWidgetLayout>("o2::UIWidgetLayout");
	o2::Reflection::InitializeType<o2::UIWidgetState>("o2::UIWidgetState");
	o2::Reflection::InitializeType<o2::UIWindow>("o2::UIWindow");
	o2::Reflection::InitializeType<o2::ISerializable>("o2::ISerializable");
	o2::Reflection::InitializeType<o2::ShortcutKeys>("o2::ShortcutKeys");
	o2::Reflection::InitializeType<o2::TimeStamp>("o2::TimeStamp");
	o2::Reflection::InitializeType<o2::Curve>("o2::Curve");
	o2::Reflection::InitializeType<o2::Layout>("o2::Layout");
	o2::Reflection::InitializeType<o2::Transform>("o2::Transform");
	o2::Reflection::InitializeType<o2::AnimatedValue<Vec2F>::Key>("o2::AnimatedValue<Vec2F>::Key");
	o2::Reflection::InitializeType<o2::Animation::AnimatedValueDef>("o2::Animation::AnimatedValueDef");
	o2::Reflection::InitializeType<o2::Asset::IMetaInfo>("o2::Asset::IMetaInfo");
	o2::Reflection::InitializeType<o2::AssetTree::AssetNode>("o2::AssetTree::AssetNode");
	o2::Reflection::InitializeType<o2::AtlasAsset::PlatformMeta>("o2::AtlasAsset::PlatformMeta");
	o2::Reflection::InitializeType<o2::AtlasAsset::MetaInfo>("o2::AtlasAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::AtlasAsset::Page>("o2::AtlasAsset::Page");
	o2::Reflection::InitializeType<o2::BinaryAsset::MetaInfo>("o2::BinaryAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::BitmapFontAsset::MetaInfo>("o2::BitmapFontAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::FolderAsset::MetaInfo>("o2::FolderAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::ImageAsset::PlatformMeta>("o2::ImageAsset::PlatformMeta");
	o2::Reflection::InitializeType<o2::ImageAsset::MetaInfo>("o2::ImageAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::VectorFontAsset::MetaInfo>("o2::VectorFontAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::AtlasAssetConverter::Image>("o2::AtlasAssetConverter::Image");
	o2::Reflection::InitializeType<o2::VectorFont::Effect>("o2::VectorFont::Effect");
	o2::Reflection::InitializeType<o2::Scene::Layer>("o2::Scene::Layer");
	o2::Reflection::InitializeType<o2::UIContextMenu::Item>("o2::UIContextMenu::Item");
	o2::Reflection::InitializeType<o2::UIMenuPanel::Item>("o2::UIMenuPanel::Item");
	o2::Reflection::InitializeType<o2::Curve::Key>("o2::Curve::Key");
	o2::Reflection::InitializeType<::EditorConfig::GlobalConfig>("::EditorConfig::GlobalConfig");
	o2::Reflection::InitializeType<::EditorConfig::ProjectConfig>("::EditorConfig::ProjectConfig");
	o2::Reflection::InitializeType<::EditorDeleteActorsAction::ActorInfo>("::EditorDeleteActorsAction::ActorInfo");
	o2::Reflection::InitializeType<::WindowsLayout::WindowDockPlace>("::WindowsLayout::WindowDockPlace");
	o2::Reflection::InitializeType<o2::AnimatedValue<RectF>>("o2::AnimatedValue<RectF>");
	o2::Reflection::InitializeType<o2::AnimatedValue<RectF>::Key>("o2::AnimatedValue<RectF>::Key");
	o2::Reflection::InitializeType<o2::AnimatedValue<Color4>>("o2::AnimatedValue<Color4>");
	o2::Reflection::InitializeType<o2::AnimatedValue<Color4>::Key>("o2::AnimatedValue<Color4>::Key");
	o2::Reflection::InitializeType<o2::AnimatedValue<bool>>("o2::AnimatedValue<bool>");
	o2::Reflection::InitializeType<o2::AnimatedValue<bool>::Key>("o2::AnimatedValue<bool>::Key");

	// Resolve inheritance
	TypeInitializer::AddBaseType(&o2::UIWidget::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::UIScrollArea::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&::UIAssetsIconsScrollArea::type, &o2::UIScrollArea::type);
	TypeInitializer::AddBaseType(&::AssetsWindow::type, &::IEditorWindow::type);
	TypeInitializer::AddBaseType(&::UIAssetsFoldersTree::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&::UIAssetIcon::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&::EditorConfig::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&::IEditorAction::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&::EditorActorsTransformAction::type, &::IEditorAction::type);
	TypeInitializer::AddBaseType(&::EditorCreateActorsAction::type, &::IEditorAction::type);
	TypeInitializer::AddBaseType(&::EditorDeleteActorsAction::type, &::IEditorAction::type);
	TypeInitializer::AddBaseType(&::EditorEnableAction::type, &::IEditorAction::type);
	TypeInitializer::AddBaseType(&::EditorLockAction::type, &::IEditorAction::type);
	TypeInitializer::AddBaseType(&::EditorReparentActorsAction::type, &::IEditorAction::type);
	TypeInitializer::AddBaseType(&::EditorSelectionAction::type, &::IEditorAction::type);
	TypeInitializer::AddBaseType(&::EditorSelectionTool::type, &::IEditorTool::type);
	TypeInitializer::AddBaseType(&::EditorFrameTool::type, &::EditorSelectionTool::type);
	TypeInitializer::AddBaseType(&::EditorMoveTool::type, &::EditorSelectionTool::type);
	TypeInitializer::AddBaseType(&::EditorRotateTool::type, &::EditorSelectionTool::type);
	TypeInitializer::AddBaseType(&::EditorScaleTool::type, &::EditorSelectionTool::type);
	TypeInitializer::AddBaseType(&o2::UIWindow::type, &o2::UIScrollArea::type);
	TypeInitializer::AddBaseType(&::UIDockableWindow::type, &o2::UIWindow::type);
	TypeInitializer::AddBaseType(&::UIDockWindowPlace::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&::WindowsLayout::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&::LogWindow::type, &::IEditorWindow::type);
	TypeInitializer::AddBaseType(&::SceneEditWidget::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&::SceneWindow::type, &::IEditorWindow::type);
	TypeInitializer::AddBaseType(&::TreeWindow::type, &::IEditorWindow::type);
	TypeInitializer::AddBaseType(&o2::Animatable::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::IAnimation::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::IAnimatedValue::type, &o2::IAnimation::type);
	TypeInitializer::AddBaseType(&o2::AnimatedValue<float>::type, &o2::IAnimatedValue::type);
	TypeInitializer::AddBaseType(&o2::AnimatedValue<Vec2F>::type, &o2::IAnimatedValue::type);
	TypeInitializer::AddBaseType(&o2::Animation::type, &o2::IAnimation::type);
	TypeInitializer::AddBaseType(&o2::AnimationMask::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::AnimationState::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::Asset::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::AssetInfo::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::AtlasAsset::type, &o2::Asset::type);
	TypeInitializer::AddBaseType(&o2::BinaryAsset::type, &o2::Asset::type);
	TypeInitializer::AddBaseType(&o2::BitmapFontAsset::type, &o2::Asset::type);
	TypeInitializer::AddBaseType(&o2::FolderAsset::type, &o2::Asset::type);
	TypeInitializer::AddBaseType(&o2::ImageAsset::type, &o2::Asset::type);
	TypeInitializer::AddBaseType(&o2::VectorFontAsset::type, &o2::Asset::type);
	TypeInitializer::AddBaseType(&o2::AtlasAssetConverter::type, &o2::IAssetConverter::type);
	TypeInitializer::AddBaseType(&o2::FolderAssetConverter::type, &o2::IAssetConverter::type);
	TypeInitializer::AddBaseType(&o2::ImageAssetConverter::type, &o2::IAssetConverter::type);
	TypeInitializer::AddBaseType(&o2::StdAssetConverter::type, &o2::IAssetConverter::type);
	TypeInitializer::AddBaseType(&o2::ProjectConfig::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::Transform::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::Camera::type, &o2::Transform::type);
	TypeInitializer::AddBaseType(&o2::IRectDrawable::type, &o2::Transform::type);
	TypeInitializer::AddBaseType(&o2::Sprite::type, &o2::IRectDrawable::type);
	TypeInitializer::AddBaseType(&o2::Text::type, &o2::IRectDrawable::type);
	TypeInitializer::AddBaseType(&o2::VectorFont::Effect::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::FontStrokeEffect::type, &o2::VectorFont::Effect::type);
	TypeInitializer::AddBaseType(&o2::FontGradientEffect::type, &o2::VectorFont::Effect::type);
	TypeInitializer::AddBaseType(&o2::FontColorEffect::type, &o2::VectorFont::Effect::type);
	TypeInitializer::AddBaseType(&o2::FontShadowEffect::type, &o2::VectorFont::Effect::type);
	TypeInitializer::AddBaseType(&o2::Actor::type, &o2::Animatable::type);
	TypeInitializer::AddBaseType(&o2::ActorTransform::type, &o2::Transform::type);
	TypeInitializer::AddBaseType(&o2::Component::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::DrawableComponent::type, &o2::Component::type);
	TypeInitializer::AddBaseType(&o2::ImageComponent::type, &o2::DrawableComponent::type);
	TypeInitializer::AddBaseType(&o2::UIButton::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UIContextMenuItem::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UIContextMenu::type, &o2::UIScrollArea::type);
	TypeInitializer::AddBaseType(&o2::UICustomDropDown::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UICustomList::type, &o2::UIScrollArea::type);
	TypeInitializer::AddBaseType(&o2::UIDropDown::type, &o2::UICustomDropDown::type);
	TypeInitializer::AddBaseType(&o2::UIEditBox::type, &o2::UIScrollArea::type);
	TypeInitializer::AddBaseType(&o2::UIGridLayout::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UIHorizontalLayout::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UIHorizontalProgress::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UIHorizontalScrollBar::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UILabel::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UIList::type, &o2::UICustomList::type);
	TypeInitializer::AddBaseType(&o2::UILongList::type, &o2::UIScrollArea::type);
	TypeInitializer::AddBaseType(&o2::UIMenuPanel::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UIToggle::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UITreeNode::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UITree::type, &o2::UIScrollArea::type);
	TypeInitializer::AddBaseType(&o2::UIVerticalLayout::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UIVerticalProgress::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UIVerticalScrollBar::type, &o2::UIWidget::type);
	TypeInitializer::AddBaseType(&o2::UIWidgetLayer::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::UIWidgetLayout::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::UIWidgetState::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::ShortcutKeys::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::TimeStamp::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::Curve::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::Layout::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::AnimatedValue<Vec2F>::Key::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::Animation::AnimatedValueDef::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::Asset::IMetaInfo::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::AssetTree::AssetNode::type, &o2::AssetInfo::type);
	TypeInitializer::AddBaseType(&o2::AtlasAsset::PlatformMeta::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::AtlasAsset::MetaInfo::type, &o2::Asset::IMetaInfo::type);
	TypeInitializer::AddBaseType(&o2::AtlasAsset::Page::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::BinaryAsset::MetaInfo::type, &o2::Asset::IMetaInfo::type);
	TypeInitializer::AddBaseType(&o2::BitmapFontAsset::MetaInfo::type, &o2::Asset::IMetaInfo::type);
	TypeInitializer::AddBaseType(&o2::FolderAsset::MetaInfo::type, &o2::Asset::IMetaInfo::type);
	TypeInitializer::AddBaseType(&o2::ImageAsset::PlatformMeta::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::ImageAsset::MetaInfo::type, &o2::Asset::IMetaInfo::type);
	TypeInitializer::AddBaseType(&o2::VectorFontAsset::MetaInfo::type, &o2::Asset::IMetaInfo::type);
	TypeInitializer::AddBaseType(&o2::AtlasAssetConverter::Image::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::Scene::Layer::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::UIContextMenu::Item::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::UIMenuPanel::Item::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::Curve::Key::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&::EditorConfig::GlobalConfig::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&::EditorConfig::ProjectConfig::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&::EditorDeleteActorsAction::ActorInfo::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&::WindowsLayout::WindowDockPlace::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::AnimatedValue<RectF>::type, &o2::IAnimatedValue::type);
	TypeInitializer::AddBaseType(&o2::AnimatedValue<RectF>::Key::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::AnimatedValue<Color4>::type, &o2::IAnimatedValue::type);
	TypeInitializer::AddBaseType(&o2::AnimatedValue<Color4>::Key::type, &o2::ISerializable::type);
	TypeInitializer::AddBaseType(&o2::AnimatedValue<bool>::type, &o2::IAnimatedValue::type);
	TypeInitializer::AddBaseType(&o2::AnimatedValue<bool>::Key::type, &o2::ISerializable::type);

}