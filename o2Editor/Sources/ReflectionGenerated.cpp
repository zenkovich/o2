#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\ReflectionGenerated.h"

#include "Utils/Reflection/Reflection.h"

// Includes
#include "C:\work\o2\o2Engine\Sources\Animation\Animatable.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimatedFloat.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimatedValue.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimatedVector.h"
#include "C:\work\o2\o2Engine\Sources\Animation\Animation.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimationMask.h"
#include "C:\work\o2\o2Engine\Sources\Animation\AnimationState.h"
#include "C:\work\o2\o2Engine\Sources\Animation\IAnimation.h"
#include "C:\work\o2\o2Engine\Sources\Assets\ActorAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\AnimationAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\Asset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\AssetInfo.h"
#include "C:\work\o2\o2Engine\Sources\Assets\AtlasAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\BinaryAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\BitmapFontAsset.h"
#include "C:\work\o2\o2Engine\Sources\Assets\DataAsset.h"
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
#include "C:\work\o2\o2Engine\Sources\Scene\Tags.h"
#include "C:\work\o2\o2Engine\Sources\Scene\Components\EditorTestComponent.h"
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
#include "C:\work\o2\o2Engine\Sources\Utils\Serializable.h"
#include "C:\work\o2\o2Engine\Sources\Utils\ShortcutKeys.h"
#include "C:\work\o2\o2Engine\Sources\Utils\TimeStamp.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Math\Curve.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Math\Layout.h"
#include "C:\work\o2\o2Engine\Sources\Utils\Math\Transform.h"
#include "C:\work\o2\o2Engine\Sources\Assets\AssetsTree.h"
#include "C:\work\o2\o2Engine\Sources\Render\VectorFont.h"
#include "C:\work\o2\o2Engine\Sources\Scene\Scene.h"
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
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\IObjectPropertiesViewer.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\PropertiesWindow.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\ActorsViewer\ActorPropertiesViewer.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\ActorsViewer\DefaultActorAnimationViewer.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\ActorsViewer\DefaultActorComponentViewer.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\ActorsViewer\DefaultActorHeaderViewer.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\ActorsViewer\DefaultActorTransformViewer.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\ActorsViewer\IActorAnimationViewer.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\ActorsViewer\IActorComponentViewer.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\ActorsViewer\IActorHeaderViewer.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\ActorsViewer\IActorTransformViewer.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\ActorProperty.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\AssetProperty.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\BooleanProperty.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\ComponentProperty.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\FloatProperty.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\IntegerProperty.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\IPropertyField.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\LayerProperty.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\StringProperty.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\TagProperty.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\PropertiesWindow\Properties\WStringProperty.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\SceneWindow\SceneEditScreen.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\SceneWindow\SceneEditWidget.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\SceneWindow\SceneWindow.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\TreeWindow\ActorsTree.h"
#include "C:\work\o2\o2Editor\Platforms\Windows\..\..\Sources\TreeWindow\TreeWindow.h"

// Types declarations
o2::Type o2::Animatable::type;
o2::Type o2::AnimatedValue<float>::type;
o2::Type o2::IAnimatedValue::type;
o2::Type o2::AnimatedValue<Vec2F>::type;
o2::Type o2::Animation::type;
o2::Type o2::AnimationMask::type;
o2::Type o2::AnimationState::type;
o2::Type o2::IAnimation::type;
o2::Type o2::ActorAsset::type;
o2::Type o2::AnimationAsset::type;
o2::Type o2::Asset::type;
o2::Type o2::AssetInfo::type;
o2::Type o2::AtlasAsset::type;
o2::Type o2::BinaryAsset::type;
o2::Type o2::BitmapFontAsset::type;
o2::Type o2::DataAsset::type;
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
o2::Type o2::Tag::type;
o2::Type o2::TagGroup::type;
o2::Type o2::EditorTestComponent::type;
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
o2::Type o2::ActorAsset::MetaInfo::type;
o2::Type o2::AnimationAsset::MetaInfo::type;
o2::Type o2::Asset::IMetaInfo::type;
o2::Type o2::AssetTree::AssetNode::type;
o2::Type o2::AtlasAsset::PlatformMeta::type;
o2::Type o2::AtlasAsset::MetaInfo::type;
o2::Type o2::AtlasAsset::Page::type;
o2::Type o2::BinaryAsset::MetaInfo::type;
o2::Type o2::BitmapFontAsset::MetaInfo::type;
o2::Type o2::DataAsset::MetaInfo::type;
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
o2::Type Editor::UIAssetsIconsScrollArea::type;
o2::Type Editor::AssetsWindow::type;
o2::Type Editor::UIAssetsFoldersTree::type;
o2::Type Editor::UIAssetIcon::type;
o2::Type Editor::EditorConfig::type;
o2::Type Editor::EditorActorsTransformAction::type;
o2::Type Editor::CreateActorsAction::type;
o2::Type Editor::DeleteActorsAction::type;
o2::Type Editor::EnableAction::type;
o2::Type Editor::IAction::type;
o2::Type Editor::LockAction::type;
o2::Type Editor::ReparentActorsAction::type;
o2::Type Editor::SelectionAction::type;
o2::Type Editor::FrameTool::type;
o2::Type Editor::IEditTool::type;
o2::Type Editor::MoveTool::type;
o2::Type Editor::RotateTool::type;
o2::Type Editor::ScaleTool::type;
o2::Type Editor::SelectionTool::type;
o2::Type Editor::IEditorWindow::type;
o2::Type Editor::UIDockableWindow::type;
o2::Type Editor::UIDockWindowPlace::type;
o2::Type Editor::WindowsLayout::type;
o2::Type Editor::LogWindow::type;
o2::Type Editor::IObjectPropertiesViewer::type;
o2::Type Editor::PropertiesWindow::type;
o2::Type Editor::ActorPropertiesViewer::type;
o2::Type Editor::DefaultActorAnimationViewer::type;
o2::Type Editor::DefaultActorComponentViewer::type;
o2::Type Editor::DefaultActorHeaderViewer::type;
o2::Type Editor::DefaultActorTransformViewer::type;
o2::Type Editor::IActorAnimationViewer::type;
o2::Type Editor::IActorComponentViewer::type;
o2::Type Editor::IActorHeaderViewer::type;
o2::Type Editor::IActorTransformViewer::type;
o2::Type Editor::ActorProperty::type;
o2::Type Editor::IAssetProperty::type;
o2::Type Editor::BooleanProperty::type;
o2::Type Editor::ComponentProperty::type;
o2::Type Editor::FloatProperty::type;
o2::Type Editor::IntegerProperty::type;
o2::Type Editor::IPropertyField::type;
o2::Type Editor::LayerProperty::type;
o2::Type Editor::StringProperty::type;
o2::Type Editor::TagsProperty::type;
o2::Type Editor::WStringProperty::type;
o2::Type Editor::SceneEditScreen::type;
o2::Type Editor::SceneEditWidget::type;
o2::Type Editor::SceneWindow::type;
o2::Type Editor::UIActorsTree::type;
o2::Type Editor::TreeWindow::type;
o2::Type Editor::EditorConfig::GlobalConfig::type;
o2::Type Editor::EditorConfig::ProjectConfig::type;
o2::Type Editor::DeleteActorsAction::ActorInfo::type;
o2::Type Editor::WindowsLayout::WindowDockPlace::type;
o2::Type o2::AnimatedValue<RectF>::type;
o2::Type o2::AnimatedValue<RectF>::Key::type;
o2::Type o2::AnimatedValue<Color4>::type;
o2::Type o2::AnimatedValue<Color4>::Key::type;
o2::Type o2::AnimatedValue<bool>::type;
o2::Type o2::AnimatedValue<bool>::Key::type;
o2::Type Editor::AssetProperty<ActorAsset>::type;
o2::Type Editor::AssetProperty<AnimationAsset>::type;
o2::Type Editor::AssetProperty<AtlasAsset>::type;
o2::Type Editor::AssetProperty<BinaryAsset>::type;
o2::Type Editor::AssetProperty<BitmapFontAsset>::type;
o2::Type Editor::AssetProperty<DataAsset>::type;
o2::Type Editor::AssetProperty<FolderAsset>::type;
o2::Type Editor::AssetProperty<ImageAsset>::type;
o2::Type Editor::AssetProperty<VectorFontAsset>::type;

// Types initializations
void o2::Animatable::InitializeType(o2::Animatable* sample)
{
	TypeInitializer::RegField(&type, "mStates", (size_t)(char*)(&sample->mStates) - (size_t)(char*)sample, sample->mStates, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValues", (size_t)(char*)(&sample->mValues) - (size_t)(char*)sample, sample->mValues, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBlend", (size_t)(char*)(&sample->mBlend) - (size_t)(char*)sample, sample->mBlend, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Animatable, void, float>(&type, "Update", &o2::Animatable::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, AnimationState*>(&type, "AddState", &o2::Animatable::AddState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const String&, const Animation&, const AnimationMask&, float>(&type, "AddState", &o2::Animatable::AddState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	TypeInitializer::RegFuncParam<const AnimationMask&>(funcInfo, "mask");
	TypeInitializer::RegFuncParam<float>(funcInfo, "weight");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const String&>(&type, "AddState", &o2::Animatable::AddState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void, AnimationState*>(&type, "RemoveState", &o2::Animatable::RemoveState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void, const String&>(&type, "RemoveState", &o2::Animatable::RemoveState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void>(&type, "RemoveAllStates", &o2::Animatable::RemoveAllStates, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const String&>(&type, "GetState", &o2::Animatable::GetState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, const AnimationStatesVec&>(&type, "GetStates", &o2::Animatable::GetStates, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const Animation&, const String&>(&type, "Play", &o2::Animatable::Play, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const Animation&>(&type, "Play", &o2::Animatable::Play, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const String&>(&type, "Play", &o2::Animatable::Play, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const Animation&, const String&, float>(&type, "BlendTo", &o2::Animatable::BlendTo, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<float>(funcInfo, "duration");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const Animation&, float>(&type, "BlendTo", &o2::Animatable::BlendTo, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	TypeInitializer::RegFuncParam<float>(funcInfo, "duration");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, const String&, float>(&type, "BlendTo", &o2::Animatable::BlendTo, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<float>(funcInfo, "duration");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, AnimationState*, AnimationState*, float>(&type, "BlendTo", &o2::Animatable::BlendTo, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<float>(funcInfo, "duration");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void, const String&>(&type, "Stop", &o2::Animatable::Stop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "animationName");
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void>(&type, "StopAll", &o2::Animatable::StopAll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Animatable, void, IAnimatedValue*, const String&>(&type, "UnregAnimatedValue", &o2::Animatable::UnregAnimatedValue, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<IAnimatedValue*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
}

void o2::AnimatedValue<float>::InitializeType(o2::AnimatedValue<float>* sample)
{
	TypeInitializer::RegField(&type, "curve", (size_t)(char*)(&sample->curve) - (size_t)(char*)sample, sample->curve, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)sample, sample->target, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)sample, sample->targetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)sample, sample->targetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)sample, sample->mTargetDelegate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)sample, sample->mTargetProperty, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, float*>(&type, "SetTarget", &o2::AnimatedValue<float>::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, float*, const Function<void()>&>(&type, "SetTarget", &o2::AnimatedValue<float>::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::AnimatedValue<float>::SetTargetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, Setter<float>*>(&type, "SetTargetProperty", &o2::AnimatedValue<float>::SetTargetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Setter<float>*>(funcInfo, "setter");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, float>(&type, "GetValue", &o2::AnimatedValue<float>::GetValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, float, float>(&type, "GetValue", &o2::AnimatedValue<float>::GetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, Vector<Vec2F>, float>(&type, "AddKeys", &o2::AnimatedValue<float>::AddKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Vec2F>>(funcInfo, "values");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, const Key&>(&type, "AddKey", &o2::AnimatedValue<float>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, const Key&, float>(&type, "AddKey", &o2::AnimatedValue<float>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, float, float, float, float, float, float>(&type, "AddKey", &o2::AnimatedValue<float>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, float, float, float>(&type, "AddKey", &o2::AnimatedValue<float>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, Key, float>(&type, "GetKey", &o2::AnimatedValue<float>::GetKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, bool, float>(&type, "RemoveKey", &o2::AnimatedValue<float>::RemoveKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void>(&type, "RemoveAllKeys", &o2::AnimatedValue<float>::RemoveAllKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, bool, float>(&type, "ContainsKey", &o2::AnimatedValue<float>::ContainsKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, const KeysVec&>(&type, "GetKeys", &o2::AnimatedValue<float>::GetKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, const KeysVec&>(&type, "SetKeys", &o2::AnimatedValue<float>::SetKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, float, float>(&type, "SmoothKey", &o2::AnimatedValue<float>::SmoothKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void>(&type, "Evaluate", &o2::AnimatedValue<float>::Evaluate, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, KeysVec>(&type, "GetKeysNonContant", &o2::AnimatedValue<float>::GetKeysNonContant, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void>(&type, "OnCurveChanged", &o2::AnimatedValue<float>::OnCurveChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, const DataNode&>(&type, "OnDeserialized", &o2::AnimatedValue<float>::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, void*>(&type, "SetTargetVoid", &o2::AnimatedValue<float>::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::AnimatedValue<float>::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, void*>(&type, "SetTargetPropertyVoid", &o2::AnimatedValue<float>::SetTargetPropertyVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::AnimatedValue<float>::RegInAnimatable, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<float>, void>(&type, "InitializeProperties", &o2::AnimatedValue<float>::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::IAnimatedValue::InitializeType(o2::IAnimatedValue* sample)
{
	TypeInitializer::RegField(&type, "onKeysChanged", (size_t)(char*)(&sample->onKeysChanged) - (size_t)(char*)sample, sample->onKeysChanged, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::IAnimatedValue::SetTargetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, void*>(&type, "SetTargetVoid", &o2::IAnimatedValue::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::IAnimatedValue::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, void*>(&type, "SetTargetPropertyVoid", &o2::IAnimatedValue::SetTargetPropertyVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::IAnimatedValue::RegInAnimatable, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimatedValue, void, float, float>(&type, "ForceSetTime", &o2::IAnimatedValue::ForceSetTime, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	TypeInitializer::RegFuncParam<float>(funcInfo, "duration");
}

void o2::AnimatedValue<Vec2F>::InitializeType(o2::AnimatedValue<Vec2F>* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)sample, sample->target, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)sample, sample->targetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)sample, sample->targetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)sample, sample->mKeys, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)sample, sample->mTargetDelegate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)sample, sample->mTargetProperty, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, Vec2F*>(&type, "SetTarget", &o2::AnimatedValue<Vec2F>::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vec2F*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, Vec2F*, const Function<void()>&>(&type, "SetTarget", &o2::AnimatedValue<Vec2F>::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vec2F*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::AnimatedValue<Vec2F>::SetTargetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, Setter<Vec2F>*>(&type, "SetTargetProperty", &o2::AnimatedValue<Vec2F>::SetTargetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Setter<Vec2F>*>(funcInfo, "setter");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, Vec2F>(&type, "GetValue", &o2::AnimatedValue<Vec2F>::GetValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, Vec2F, float>(&type, "GetValue", &o2::AnimatedValue<Vec2F>::GetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, Vector<Key>, float>(&type, "AddKeys", &o2::AnimatedValue<Vec2F>::AddKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Key>>(funcInfo, "keys");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const Key&>(&type, "AddKey", &o2::AnimatedValue<Vec2F>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const Key&, float>(&type, "AddKey", &o2::AnimatedValue<Vec2F>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const Key&, float>(&type, "AddSmoothKey", &o2::AnimatedValue<Vec2F>::AddSmoothKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, float, const Vec2F&, const Vec2F&, const Vec2F&, float, float, float, float>(&type, "AddKey", &o2::AnimatedValue<Vec2F>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "prevSupport");
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "nextSupport");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, float, const Vec2F&, float>(&type, "AddKey", &o2::AnimatedValue<Vec2F>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, Key, float>(&type, "GetKey", &o2::AnimatedValue<Vec2F>::GetKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, bool, float>(&type, "RemoveKey", &o2::AnimatedValue<Vec2F>::RemoveKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void>(&type, "RemoveAllKeys", &o2::AnimatedValue<Vec2F>::RemoveAllKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, bool, float>(&type, "ContainsKey", &o2::AnimatedValue<Vec2F>::ContainsKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, const KeysVec&>(&type, "GetKeys", &o2::AnimatedValue<Vec2F>::GetKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const KeysVec&>(&type, "SetKeys", &o2::AnimatedValue<Vec2F>::SetKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, float, float>(&type, "SmoothKey", &o2::AnimatedValue<Vec2F>::SmoothKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void>(&type, "Evaluate", &o2::AnimatedValue<Vec2F>::Evaluate, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, Vec2F, float>(&type, "Evaluate", &o2::AnimatedValue<Vec2F>::Evaluate, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, KeysVec>(&type, "GetKeysNonContant", &o2::AnimatedValue<Vec2F>::GetKeysNonContant, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void>(&type, "UpdateApproximation", &o2::AnimatedValue<Vec2F>::UpdateApproximation, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, const DataNode&>(&type, "OnDeserialized", &o2::AnimatedValue<Vec2F>::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, void*>(&type, "SetTargetVoid", &o2::AnimatedValue<Vec2F>::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::AnimatedValue<Vec2F>::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, void*>(&type, "SetTargetPropertyVoid", &o2::AnimatedValue<Vec2F>::SetTargetPropertyVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::AnimatedValue<Vec2F>::RegInAnimatable, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Vec2F>, void>(&type, "InitializeProperties", &o2::AnimatedValue<Vec2F>::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::Animation::InitializeType(o2::Animation* sample)
{
	TypeInitializer::RegField(&type, "mAnimatedValues", (size_t)(char*)(&sample->mAnimatedValues) - (size_t)(char*)sample, sample->mAnimatedValues, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAnimationState", (size_t)(char*)(&sample->mAnimationState) - (size_t)(char*)sample, sample->mAnimationState, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Animation, void, IObject*, bool>(&type, "SetTarget", &o2::Animation::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IObject*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "errors");
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void>(&type, "Clear", &o2::Animation::Clear, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Animation, bool, const String&>(&type, "RemoveAnimationValue", &o2::Animation::RemoveAnimationValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void>(&type, "Evaluate", &o2::Animation::Evaluate, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void>(&type, "RecalculateDuration", &o2::Animation::RecalculateDuration, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void, const DataNode&>(&type, "OnDeserialized", &o2::Animation::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void, AnimatedValueDef&>(&type, "OnAnimatedValueAdded", &o2::Animation::OnAnimatedValueAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<AnimatedValueDef&>(funcInfo, "valueDef");
}

void o2::AnimationMask::InitializeType(o2::AnimationMask* sample)
{
	TypeInitializer::RegField(&type, "weights", (size_t)(char*)(&sample->weights) - (size_t)(char*)sample, sample->weights, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimationMask, float, const String&>(&type, "GetNodeWeight", &o2::AnimationMask::GetNodeWeight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "node");
}

void o2::AnimationState::InitializeType(o2::AnimationState* sample)
{
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "animation", (size_t)(char*)(&sample->animation) - (size_t)(char*)sample, sample->animation, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mask", (size_t)(char*)(&sample->mask) - (size_t)(char*)sample, sample->mask, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "weight", (size_t)(char*)(&sample->weight) - (size_t)(char*)sample, sample->weight, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "workWeight", (size_t)(char*)(&sample->workWeight) - (size_t)(char*)sample, sample->workWeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner, o2::ProtectSection::Protected);
}

void o2::IAnimation::InitializeType(o2::IAnimation* sample)
{
	TypeInitializer::RegField(&type, "playing", (size_t)(char*)(&sample->playing) - (size_t)(char*)sample, sample->playing, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "reversed", (size_t)(char*)(&sample->reversed) - (size_t)(char*)sample, sample->reversed, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "speed", (size_t)(char*)(&sample->speed) - (size_t)(char*)sample, sample->speed, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "time", (size_t)(char*)(&sample->time) - (size_t)(char*)sample, sample->time, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "relTime", (size_t)(char*)(&sample->relTime) - (size_t)(char*)sample, sample->relTime, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "beginBound", (size_t)(char*)(&sample->beginBound) - (size_t)(char*)sample, sample->beginBound, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "endBound", (size_t)(char*)(&sample->endBound) - (size_t)(char*)sample, sample->endBound, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "loop", (size_t)(char*)(&sample->loop) - (size_t)(char*)sample, sample->loop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "duration", (size_t)(char*)(&sample->duration) - (size_t)(char*)sample, sample->duration, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onPlayEvent", (size_t)(char*)(&sample->onPlayEvent) - (size_t)(char*)sample, sample->onPlayEvent, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onStopEvent", (size_t)(char*)(&sample->onStopEvent) - (size_t)(char*)sample, sample->onStopEvent, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onPlayedEvent", (size_t)(char*)(&sample->onPlayedEvent) - (size_t)(char*)sample, sample->onPlayedEvent, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onUpdate", (size_t)(char*)(&sample->onUpdate) - (size_t)(char*)sample, sample->onUpdate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTime", (size_t)(char*)(&sample->mTime) - (size_t)(char*)sample, sample->mTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mInDurationTime", (size_t)(char*)(&sample->mInDurationTime) - (size_t)(char*)sample, sample->mInDurationTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDuration", (size_t)(char*)(&sample->mDuration) - (size_t)(char*)sample, sample->mDuration, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBeginTime", (size_t)(char*)(&sample->mBeginTime) - (size_t)(char*)sample, sample->mBeginTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEndTime", (size_t)(char*)(&sample->mEndTime) - (size_t)(char*)sample, sample->mEndTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDirection", (size_t)(char*)(&sample->mDirection) - (size_t)(char*)sample, sample->mDirection, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSpeed", (size_t)(char*)(&sample->mSpeed) - (size_t)(char*)sample, sample->mSpeed, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLoop", (size_t)(char*)(&sample->mLoop) - (size_t)(char*)sample, sample->mLoop, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mPlaying", (size_t)(char*)(&sample->mPlaying) - (size_t)(char*)sample, sample->mPlaying, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTimeEvents", (size_t)(char*)(&sample->mTimeEvents) - (size_t)(char*)sample, sample->mTimeEvents, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "Update", &o2::IAnimation::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "Play", &o2::IAnimation::Play, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float, float>(&type, "PlayInBounds", &o2::IAnimation::PlayInBounds, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "beginTime");
	TypeInitializer::RegFuncParam<float>(funcInfo, "endTime");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float, float>(&type, "PlayBackInBounds", &o2::IAnimation::PlayBackInBounds, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "beginTime");
	TypeInitializer::RegFuncParam<float>(funcInfo, "endTime");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "TogglePlay", &o2::IAnimation::TogglePlay, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "RewindAndPlay", &o2::IAnimation::RewindAndPlay, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "Stop", &o2::IAnimation::Stop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "SetBeginBound", &o2::IAnimation::SetBeginBound, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetBeginBound", &o2::IAnimation::GetBeginBound, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "SetEndBound", &o2::IAnimation::SetEndBound, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetEndBound", &o2::IAnimation::GetEndBound, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float, float>(&type, "SetBounds", &o2::IAnimation::SetBounds, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "beginTime");
	TypeInitializer::RegFuncParam<float>(funcInfo, "endTime");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "ResetBounds", &o2::IAnimation::ResetBounds, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, bool>(&type, "SetPlaying", &o2::IAnimation::SetPlaying, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "playing");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, bool>(&type, "IsPlaying", &o2::IAnimation::IsPlaying, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "SetTime", &o2::IAnimation::SetTime, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetTime", &o2::IAnimation::GetTime, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetDuration", &o2::IAnimation::GetDuration, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "SetRelTime", &o2::IAnimation::SetRelTime, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "relTime");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetRelTime", &o2::IAnimation::GetRelTime, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "GoToBegin", &o2::IAnimation::GoToBegin, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "GoToEnd", &o2::IAnimation::GoToEnd, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "PlayForward", &o2::IAnimation::PlayForward, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "PlayBack", &o2::IAnimation::PlayBack, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, bool>(&type, "SetReverse", &o2::IAnimation::SetReverse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "reverse");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, bool>(&type, "IsReversed", &o2::IAnimation::IsReversed, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "SetSpeed", &o2::IAnimation::SetSpeed, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "speed");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, float>(&type, "GetSpeed", &o2::IAnimation::GetSpeed, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, Loop>(&type, "SetLoop", &o2::IAnimation::SetLoop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Loop>(funcInfo, "loop");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, Loop>(&type, "GetLoop", &o2::IAnimation::GetLoop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float, const Function<void()>>(&type, "AddTimeEvent", &o2::IAnimation::AddTimeEvent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	TypeInitializer::RegFuncParam<const Function<void()>>(funcInfo, "eventFunc");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, float>(&type, "RemoveTimeEvent", &o2::IAnimation::RemoveTimeEvent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void, const Function<void()>>(&type, "RemoveTimeEvent", &o2::IAnimation::RemoveTimeEvent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Function<void()>>(funcInfo, "eventFunc");
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "RemoveAllTimeEvents", &o2::IAnimation::RemoveAllTimeEvents, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "UpdateTime", &o2::IAnimation::UpdateTime, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "Evaluate", &o2::IAnimation::Evaluate, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::IAnimation, void>(&type, "InitializeProperties", &o2::IAnimation::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::ActorAsset::InitializeType(o2::ActorAsset* sample)
{
	TypeInitializer::RegField(&type, "actor", (size_t)(char*)(&sample->actor) - (size_t)(char*)sample, sample->actor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<o2::ActorAsset, MetaInfo*>(&type, "GetMeta", &o2::ActorAsset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorAsset, const char*>(&type, "GetFileExtensions", &o2::ActorAsset::GetFileExtensions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorAsset, void, const String&>(&type, "LoadData", &o2::ActorAsset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::ActorAsset, void, const String&>(&type, "SaveData", &o2::ActorAsset::SaveData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::ActorAsset, void>(&type, "InitializeProperties", &o2::ActorAsset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::AnimationAsset::InitializeType(o2::AnimationAsset* sample)
{
	TypeInitializer::RegField(&type, "animation", (size_t)(char*)(&sample->animation) - (size_t)(char*)sample, sample->animation, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimationAsset, MetaInfo*>(&type, "GetMeta", &o2::AnimationAsset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimationAsset, const char*>(&type, "GetFileExtensions", &o2::AnimationAsset::GetFileExtensions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimationAsset, void, const String&>(&type, "LoadData", &o2::AnimationAsset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimationAsset, void, const String&>(&type, "SaveData", &o2::AnimationAsset::SaveData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimationAsset, void>(&type, "InitializeProperties", &o2::AnimationAsset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::Asset::InitializeType(o2::Asset* sample)
{
	TypeInitializer::RegField(&type, "path", (size_t)(char*)(&sample->path) - (size_t)(char*)sample, sample->path, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fullPath", (size_t)(char*)(&sample->fullPath) - (size_t)(char*)sample, sample->fullPath, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "id", (size_t)(char*)(&sample->id) - (size_t)(char*)sample, sample->id, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mPath", (size_t)(char*)(&sample->mPath) - (size_t)(char*)sample, sample->mPath, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMeta", (size_t)(char*)(&sample->mMeta) - (size_t)(char*)sample, sample->mMeta, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Asset, AssetInfo>(&type, "GetAssetInfo", &o2::Asset::GetAssetInfo, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, String>(&type, "GetPath", &o2::Asset::GetPath, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "SetPath", &o2::Asset::SetPath, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, String>(&type, "GetFullPath", &o2::Asset::GetFullPath, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, String>(&type, "GetDataFullPath", &o2::Asset::GetDataFullPath, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, AssetId>(&type, "GetAssetId", &o2::Asset::GetAssetId, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, IMetaInfo*>(&type, "GetMeta", &o2::Asset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void>(&type, "Load", &o2::Asset::Load, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "Load", &o2::Asset::Load, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, AssetId>(&type, "Load", &o2::Asset::Load, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const AssetInfo&>(&type, "Load", &o2::Asset::Load, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "info");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&, bool>(&type, "Save", &o2::Asset::Save, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "rebuildAssetsImmediately");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, bool>(&type, "Save", &o2::Asset::Save, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "rebuildAssetsImmediately");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const AssetInfo&, bool>(&type, "Save", &o2::Asset::Save, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "info");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "rebuildAssetsImmediately");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, const char*>(&type, "GetFileExtensions", &o2::Asset::GetFileExtensions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, DataNode&>(&type, "OnSerialize", &o2::Asset::OnSerialize, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const DataNode&>(&type, "OnDeserialized", &o2::Asset::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "LoadData", &o2::Asset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "SaveData", &o2::Asset::SaveData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "LoadMeta", &o2::Asset::LoadMeta, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void, const String&>(&type, "SaveMeta", &o2::Asset::SaveMeta, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Asset, String>(&type, "GetMetaFullPath", &o2::Asset::GetMetaFullPath, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, AssetId&>(&type, "IdRef", &o2::Asset::IdRef, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, LogStream*>(&type, "GetAssetsLogStream", &o2::Asset::GetAssetsLogStream, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Asset, void>(&type, "InitializeProperties", &o2::Asset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::AssetInfo::InitializeType(o2::AssetInfo* sample)
{
	TypeInitializer::RegField(&type, "mType", (size_t)(char*)(&sample->mType) - (size_t)(char*)sample, sample->mType, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mPath", (size_t)(char*)(&sample->mPath) - (size_t)(char*)sample, sample->mPath, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::AtlasAsset::InitializeType(o2::AtlasAsset* sample)
{
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imagesInfos", (size_t)(char*)(&sample->imagesInfos) - (size_t)(char*)sample, sample->imagesInfos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "images", (size_t)(char*)(&sample->images) - (size_t)(char*)sample, sample->images, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "pages", (size_t)(char*)(&sample->pages) - (size_t)(char*)sample, sample->pages, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mImagesAssetsInfos", (size_t)(char*)(&sample->mImagesAssetsInfos) - (size_t)(char*)sample, sample->mImagesAssetsInfos, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPages", (size_t)(char*)(&sample->mPages) - (size_t)(char*)sample, sample->mPages, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, AssetInfosVec>(&type, "GetImages", &o2::AtlasAsset::GetImages, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, Vector<ImageAsset*>>(&type, "GetImagesAssets", &o2::AtlasAsset::GetImagesAssets, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, PagesVec>(&type, "GetPages", &o2::AtlasAsset::GetPages, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, bool, ImageAsset*>(&type, "ContainsImage", &o2::AtlasAsset::ContainsImage, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<ImageAsset*>(funcInfo, "image");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, bool, const AssetInfo&>(&type, "ContainsImage", &o2::AtlasAsset::ContainsImage, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "imageAssetInfo");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, bool, AssetId>(&type, "ContainsImage", &o2::AtlasAsset::ContainsImage, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, bool, const String&>(&type, "ContainsImage", &o2::AtlasAsset::ContainsImage, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, MetaInfo*>(&type, "GetMeta", &o2::AtlasAsset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, const char*>(&type, "GetFileExtensions", &o2::AtlasAsset::GetFileExtensions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, void, const String&>(&type, "LoadData", &o2::AtlasAsset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, void, const String&>(&type, "SaveData", &o2::AtlasAsset::SaveData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset, void>(&type, "InitializeProperties", &o2::AtlasAsset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::BinaryAsset::InitializeType(o2::BinaryAsset* sample)
{
	TypeInitializer::RegField(&type, "data", (size_t)(char*)(&sample->data) - (size_t)(char*)sample, sample->data, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "dataSize", (size_t)(char*)(&sample->dataSize) - (size_t)(char*)sample, sample->dataSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mData", (size_t)(char*)(&sample->mData) - (size_t)(char*)sample, sample->mData, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDataSize", (size_t)(char*)(&sample->mDataSize) - (size_t)(char*)sample, sample->mDataSize, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, char*>(&type, "GetData", &o2::BinaryAsset::GetData, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, UInt>(&type, "GetDataSize", &o2::BinaryAsset::GetDataSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, void, char*, UInt>(&type, "SetData", &o2::BinaryAsset::SetData, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<char*>(funcInfo, "data");
	TypeInitializer::RegFuncParam<UInt>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, MetaInfo*>(&type, "GetMeta", &o2::BinaryAsset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, const char*>(&type, "GetFileExtensions", &o2::BinaryAsset::GetFileExtensions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, void, const String&>(&type, "LoadData", &o2::BinaryAsset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, void, const String&>(&type, "SaveData", &o2::BinaryAsset::SaveData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset, void>(&type, "InitializeProperties", &o2::BinaryAsset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::BitmapFontAsset::InitializeType(o2::BitmapFontAsset* sample)
{
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)sample, sample->font, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mFont", (size_t)(char*)(&sample->mFont) - (size_t)(char*)sample, sample->mFont, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, MetaInfo*>(&type, "GetMeta", &o2::BitmapFontAsset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, FontRef>(&type, "GetFont", &o2::BitmapFontAsset::GetFont, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, const char*>(&type, "GetFileExtensions", &o2::BitmapFontAsset::GetFileExtensions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, void, const String&>(&type, "LoadData", &o2::BitmapFontAsset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, void>(&type, "InitializeProperties", &o2::BitmapFontAsset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::DataAsset::InitializeType(o2::DataAsset* sample)
{
	TypeInitializer::RegField(&type, "data", (size_t)(char*)(&sample->data) - (size_t)(char*)sample, sample->data, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<o2::DataAsset, MetaInfo*>(&type, "GetMeta", &o2::DataAsset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::DataAsset, const char*>(&type, "GetFileExtensions", &o2::DataAsset::GetFileExtensions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::DataAsset, void, const String&>(&type, "LoadData", &o2::DataAsset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::DataAsset, void, const String&>(&type, "SaveData", &o2::DataAsset::SaveData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::DataAsset, void>(&type, "InitializeProperties", &o2::DataAsset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::FolderAsset::InitializeType(o2::FolderAsset* sample)
{
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "insideAssets", (size_t)(char*)(&sample->insideAssets) - (size_t)(char*)sample, sample->insideAssets, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mContainingAssetsInfos", (size_t)(char*)(&sample->mContainingAssetsInfos) - (size_t)(char*)sample, sample->mContainingAssetsInfos, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::FolderAsset, AssetInfosVec>(&type, "GetContainingAssetsInfos", &o2::FolderAsset::GetContainingAssetsInfos, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FolderAsset, MetaInfo*>(&type, "GetMeta", &o2::FolderAsset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FolderAsset, void, const String&>(&type, "LoadData", &o2::FolderAsset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::FolderAsset, void, const String&>(&type, "SaveData", &o2::FolderAsset::SaveData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::FolderAsset, void>(&type, "InitializeProperties", &o2::FolderAsset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::ImageAsset::InitializeType(o2::ImageAsset* sample)
{
	TypeInitializer::RegField(&type, "bitmap", (size_t)(char*)(&sample->bitmap) - (size_t)(char*)sample, sample->bitmap, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "atlasId", (size_t)(char*)(&sample->atlasId) - (size_t)(char*)sample, sample->atlasId, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "atlas", (size_t)(char*)(&sample->atlas) - (size_t)(char*)sample, sample->atlas, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "atlasPage", (size_t)(char*)(&sample->atlasPage) - (size_t)(char*)sample, sample->atlasPage, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "atlasRect", (size_t)(char*)(&sample->atlasRect) - (size_t)(char*)sample, sample->atlasRect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "size", (size_t)(char*)(&sample->size) - (size_t)(char*)sample, sample->size, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "width", (size_t)(char*)(&sample->width) - (size_t)(char*)sample, sample->width, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "height", (size_t)(char*)(&sample->height) - (size_t)(char*)sample, sample->height, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mBitmap", (size_t)(char*)(&sample->mBitmap) - (size_t)(char*)sample, sample->mBitmap, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAtlasPage", (size_t)(char*)(&sample->mAtlasPage) - (size_t)(char*)sample, sample->mAtlasPage, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAtlasRect", (size_t)(char*)(&sample->mAtlasRect) - (size_t)(char*)sample, sample->mAtlasRect, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, Bitmap*>(&type, "GetBitmap", &o2::ImageAsset::GetBitmap, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void, Bitmap*>(&type, "SetBitmap", &o2::ImageAsset::SetBitmap, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, AssetId>(&type, "GetAtlasId", &o2::ImageAsset::GetAtlasId, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void, AssetId>(&type, "SetAtlasId", &o2::ImageAsset::SetAtlasId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, AtlasAsset*>(&type, "GetAtlas", &o2::ImageAsset::GetAtlas, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void, AtlasAsset*>(&type, "SetAtlas", &o2::ImageAsset::SetAtlas, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AtlasAsset*>(funcInfo, "atlas");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, UInt>(&type, "GetAtlasPage", &o2::ImageAsset::GetAtlasPage, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, RectI>(&type, "GetAtlasRect", &o2::ImageAsset::GetAtlasRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, Vec2F>(&type, "GetSize", &o2::ImageAsset::GetSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, float>(&type, "GetWidth", &o2::ImageAsset::GetWidth, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, float>(&type, "GetHeight", &o2::ImageAsset::GetHeight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, TextureRef>(&type, "GetAtlasTextureRef", &o2::ImageAsset::GetAtlasTextureRef, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, MetaInfo*>(&type, "GetMeta", &o2::ImageAsset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, const char*>(&type, "GetFileExtensions", &o2::ImageAsset::GetFileExtensions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void, const String&>(&type, "LoadData", &o2::ImageAsset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void, const String&>(&type, "SaveData", &o2::ImageAsset::SaveData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void>(&type, "LoadBitmap", &o2::ImageAsset::LoadBitmap, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset, void>(&type, "InitializeProperties", &o2::ImageAsset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::VectorFontAsset::InitializeType(o2::VectorFontAsset* sample)
{
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)sample, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)sample, sample->font, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mFont", (size_t)(char*)(&sample->mFont) - (size_t)(char*)sample, sample->mFont, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, MetaInfo*>(&type, "GetMeta", &o2::VectorFontAsset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, FontRef>(&type, "GetFont", &o2::VectorFontAsset::GetFont, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, const EffectsVec&>(&type, "GetEffects", &o2::VectorFontAsset::GetEffects, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, void, VectorFont::Effect*>(&type, "AddEffect", &o2::VectorFontAsset::AddEffect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "effect");
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, void, VectorFont::Effect*>(&type, "RemoveEffect", &o2::VectorFontAsset::RemoveEffect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "effect");
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, void>(&type, "RemoveAllEffects", &o2::VectorFontAsset::RemoveAllEffects, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, const char*>(&type, "GetFileExtensions", &o2::VectorFontAsset::GetFileExtensions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, void, const String&>(&type, "LoadData", &o2::VectorFontAsset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset, void>(&type, "InitializeProperties", &o2::VectorFontAsset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::AtlasAssetConverter::InitializeType(o2::AtlasAssetConverter* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, Vector<Type::Id>>(&type, "GetProcessingAssetsTypes", &o2::AtlasAssetConverter::GetProcessingAssetsTypes, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void, const AssetTree::AssetNode&>(&type, "ConvertAsset", &o2::AtlasAssetConverter::ConvertAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void, const AssetTree::AssetNode&>(&type, "RemoveAsset", &o2::AtlasAssetConverter::RemoveAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void, const AssetTree::AssetNode&, const AssetTree::AssetNode&>(&type, "MoveAsset", &o2::AtlasAssetConverter::MoveAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeFrom");
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeTo");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, Vector<AssetId>>(&type, "AssetsPostProcess", &o2::AtlasAssetConverter::AssetsPostProcess, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void>(&type, "Reset", &o2::AtlasAssetConverter::Reset, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void>(&type, "CheckBasicAtlas", &o2::AtlasAssetConverter::CheckBasicAtlas, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, Vector<AssetId>>(&type, "CheckRebuildingAtlases", &o2::AtlasAssetConverter::CheckRebuildingAtlases, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, bool, AssetTree::AssetNode*>(&type, "CheckAtlasRebuilding", &o2::AtlasAssetConverter::CheckAtlasRebuilding, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<AssetTree::AssetNode*>(funcInfo, "atlasInfo");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, bool, ImagesVec&, ImagesVec&>(&type, "IsAtlasNeedRebuild", &o2::AtlasAssetConverter::IsAtlasNeedRebuild, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<ImagesVec&>(funcInfo, "currentImages");
	TypeInitializer::RegFuncParam<ImagesVec&>(funcInfo, "lastImages");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void, AssetTree::AssetNode*, ImagesVec&>(&type, "RebuildAtlas", &o2::AtlasAssetConverter::RebuildAtlas, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<AssetTree::AssetNode*>(funcInfo, "atlasInfo");
	TypeInitializer::RegFuncParam<ImagesVec&>(funcInfo, "images");
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAssetConverter, void, ImagePackDef&>(&type, "SaveImageAsset", &o2::AtlasAssetConverter::SaveImageAsset, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<ImagePackDef&>(funcInfo, "imgDef");
}

void o2::FolderAssetConverter::InitializeType(o2::FolderAssetConverter* sample)
{
	TypeInitializer::RegField(&type, "mRemovedFolders", (size_t)(char*)(&sample->mRemovedFolders) - (size_t)(char*)sample, sample->mRemovedFolders, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, Vector<Type::Id>>(&type, "GetProcessingAssetsTypes", &o2::FolderAssetConverter::GetProcessingAssetsTypes, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, void, const AssetTree::AssetNode&>(&type, "ConvertAsset", &o2::FolderAssetConverter::ConvertAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, void, const AssetTree::AssetNode&>(&type, "RemoveAsset", &o2::FolderAssetConverter::RemoveAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, void, const AssetTree::AssetNode&, const AssetTree::AssetNode&>(&type, "MoveAsset", &o2::FolderAssetConverter::MoveAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeFrom");
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeTo");
	funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, Vector<AssetId>>(&type, "AssetsPostProcess", &o2::FolderAssetConverter::AssetsPostProcess, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FolderAssetConverter, void>(&type, "Reset", &o2::FolderAssetConverter::Reset, o2::ProtectSection::Public);
}

void o2::IAssetConverter::InitializeType(o2::IAssetConverter* sample)
{
	TypeInitializer::RegField(&type, "mAssetsBuilder", (size_t)(char*)(&sample->mAssetsBuilder) - (size_t)(char*)sample, sample->mAssetsBuilder, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, Vector<Type::Id>>(&type, "GetProcessingAssetsTypes", &o2::IAssetConverter::GetProcessingAssetsTypes, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, void, const AssetTree::AssetNode&>(&type, "ConvertAsset", &o2::IAssetConverter::ConvertAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, void, const AssetTree::AssetNode&>(&type, "RemoveAsset", &o2::IAssetConverter::RemoveAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, void, const AssetTree::AssetNode&, const AssetTree::AssetNode&>(&type, "MoveAsset", &o2::IAssetConverter::MoveAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeFrom");
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeTo");
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, Vector<AssetId>>(&type, "AssetsPostProcess", &o2::IAssetConverter::AssetsPostProcess, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, void>(&type, "Reset", &o2::IAssetConverter::Reset, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IAssetConverter, void, AssetsBuilder*>(&type, "SetAssetsBuilder", &o2::IAssetConverter::SetAssetsBuilder, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetsBuilder*>(funcInfo, "builder");
}

void o2::ImageAssetConverter::InitializeType(o2::ImageAssetConverter* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::ImageAssetConverter, Vector<Type::Id>>(&type, "GetProcessingAssetsTypes", &o2::ImageAssetConverter::GetProcessingAssetsTypes, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAssetConverter, void, const AssetTree::AssetNode&>(&type, "ConvertAsset", &o2::ImageAssetConverter::ConvertAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAssetConverter, void, const AssetTree::AssetNode&>(&type, "RemoveAsset", &o2::ImageAssetConverter::RemoveAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ImageAssetConverter, void, const AssetTree::AssetNode&, const AssetTree::AssetNode&>(&type, "MoveAsset", &o2::ImageAssetConverter::MoveAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeFrom");
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeTo");
}

void o2::StdAssetConverter::InitializeType(o2::StdAssetConverter* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::StdAssetConverter, Vector<Type::Id>>(&type, "GetProcessingAssetsTypes", &o2::StdAssetConverter::GetProcessingAssetsTypes, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::StdAssetConverter, void, const AssetTree::AssetNode&>(&type, "ConvertAsset", &o2::StdAssetConverter::ConvertAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::StdAssetConverter, void, const AssetTree::AssetNode&>(&type, "RemoveAsset", &o2::StdAssetConverter::RemoveAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::StdAssetConverter, void, const AssetTree::AssetNode&, const AssetTree::AssetNode&>(&type, "MoveAsset", &o2::StdAssetConverter::MoveAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeFrom");
	TypeInitializer::RegFuncParam<const AssetTree::AssetNode&>(funcInfo, "nodeTo");
}

void o2::ProjectConfig::InitializeType(o2::ProjectConfig* sample)
{
	TypeInitializer::RegField(&type, "projectName", (size_t)(char*)(&sample->projectName) - (size_t)(char*)sample, sample->projectName, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "currentPlatform", (size_t)(char*)(&sample->currentPlatform) - (size_t)(char*)sample, sample->currentPlatform, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "projectPath", (size_t)(char*)(&sample->projectPath) - (size_t)(char*)sample, sample->projectPath, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mProjectName", (size_t)(char*)(&sample->mProjectName) - (size_t)(char*)sample, sample->mProjectName, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mPlatform", (size_t)(char*)(&sample->mPlatform) - (size_t)(char*)sample, sample->mPlatform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mProjectPath", (size_t)(char*)(&sample->mProjectPath) - (size_t)(char*)sample, sample->mProjectPath, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, String>(&type, "GetProjectName", &o2::ProjectConfig::GetProjectName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, const String&>(&type, "SetProjectName", &o2::ProjectConfig::SetProjectName, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, Platform>(&type, "GetPlatform", &o2::ProjectConfig::GetPlatform, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, Platform>(&type, "SetPlatform", &o2::ProjectConfig::SetPlatform, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Platform>(funcInfo, "platform");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, String>(&type, "GetProjectPath", &o2::ProjectConfig::GetProjectPath, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, const String&>(&type, "SetProjectPath", &o2::ProjectConfig::SetProjectPath, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, Type&>(&type, "GetType", &o2::ProjectConfig::GetType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, DataNode>(&type, "Serialize", &o2::ProjectConfig::Serialize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, const DataNode&>(&type, "Deserialize", &o2::ProjectConfig::Deserialize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, const String&>(&type, "InitializeDefault", &o2::ProjectConfig::InitializeDefault, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "configFilePath");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void>(&type, "InitializeProperties", &o2::ProjectConfig::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::Camera::InitializeType(o2::Camera* sample)
{
}

void o2::IRectDrawable::InitializeType(o2::IRectDrawable* sample)
{
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)sample, sample->color, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "transparency", (size_t)(char*)(&sample->transparency) - (size_t)(char*)sample, sample->transparency, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabled", (size_t)(char*)(&sample->enabled) - (size_t)(char*)sample, sample->enabled, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mColor", (size_t)(char*)(&sample->mColor) - (size_t)(char*)sample, sample->mColor, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mEnabled", (size_t)(char*)(&sample->mEnabled) - (size_t)(char*)sample, sample->mEnabled, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void>(&type, "Draw", &o2::IRectDrawable::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void, const Color4&>(&type, "SetColor", &o2::IRectDrawable::SetColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, Color4>(&type, "GetColor", &o2::IRectDrawable::GetColor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void, float>(&type, "SetTransparency", &o2::IRectDrawable::SetTransparency, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "transparency");
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, float>(&type, "GetTransparency", &o2::IRectDrawable::GetTransparency, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void, bool>(&type, "SetEnabled", &o2::IRectDrawable::SetEnabled, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "enabled");
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, bool>(&type, "IsEnabled", &o2::IRectDrawable::IsEnabled, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::IRectDrawable::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void>(&type, "ColorChanged", &o2::IRectDrawable::ColorChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void>(&type, "EnableChanged", &o2::IRectDrawable::EnableChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::IRectDrawable, void>(&type, "InitializeProperties", &o2::IRectDrawable::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::Sprite::InitializeType(o2::Sprite* sample)
{
	TypeInitializer::RegField(&type, "texture", (size_t)(char*)(&sample->texture) - (size_t)(char*)sample, sample->texture, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "textureSrcRect", (size_t)(char*)(&sample->textureSrcRect) - (size_t)(char*)sample, sample->textureSrcRect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAssetId", (size_t)(char*)(&sample->imageAssetId) - (size_t)(char*)sample, sample->imageAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAssetPath", (size_t)(char*)(&sample->imageAssetPath) - (size_t)(char*)sample, sample->imageAssetPath, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAsset", (size_t)(char*)(&sample->imageAsset) - (size_t)(char*)sample, sample->imageAsset, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "bitmap", (size_t)(char*)(&sample->bitmap) - (size_t)(char*)sample, sample->bitmap, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftTopColor", (size_t)(char*)(&sample->leftTopColor) - (size_t)(char*)sample, sample->leftTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightTopColor", (size_t)(char*)(&sample->rightTopColor) - (size_t)(char*)sample, sample->rightTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftBottomColor", (size_t)(char*)(&sample->leftBottomColor) - (size_t)(char*)sample, sample->leftBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightBottomColor", (size_t)(char*)(&sample->rightBottomColor) - (size_t)(char*)sample, sample->rightBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mode", (size_t)(char*)(&sample->mode) - (size_t)(char*)sample, sample->mode, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fill", (size_t)(char*)(&sample->fill) - (size_t)(char*)sample, sample->fill, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "sliceBorder", (size_t)(char*)(&sample->sliceBorder) - (size_t)(char*)sample, sample->sliceBorder, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTextureSrcRect", (size_t)(char*)(&sample->mTextureSrcRect) - (size_t)(char*)sample, sample->mTextureSrcRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCornersColors", (size_t)(char*)(&sample->mCornersColors) - (size_t)(char*)sample, sample->mCornersColors, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mImageAssetId", (size_t)(char*)(&sample->mImageAssetId) - (size_t)(char*)sample, sample->mImageAssetId, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAtlasAssetId", (size_t)(char*)(&sample->mAtlasAssetId) - (size_t)(char*)sample, sample->mAtlasAssetId, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMode", (size_t)(char*)(&sample->mMode) - (size_t)(char*)sample, sample->mMode, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSlices", (size_t)(char*)(&sample->mSlices) - (size_t)(char*)sample, sample->mSlices, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFill", (size_t)(char*)(&sample->mFill) - (size_t)(char*)sample, sample->mFill, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMesh", (size_t)(char*)(&sample->mMesh) - (size_t)(char*)sample, sample->mMesh, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMeshBuildFunc", (size_t)(char*)(&sample->mMeshBuildFunc) - (size_t)(char*)sample, sample->mMeshBuildFunc, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "Draw", &o2::Sprite::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, TextureRef>(&type, "SetTexture", &o2::Sprite::SetTexture, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<TextureRef>(funcInfo, "texture");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, TextureRef>(&type, "GetTexture", &o2::Sprite::GetTexture, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const RectI&>(&type, "SetTextureSrcRect", &o2::Sprite::SetTextureSrcRect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectI&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, RectI>(&type, "GetTextureSrcRect", &o2::Sprite::GetTextureSrcRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, Corner, const Color4&>(&type, "SetCornerColor", &o2::Sprite::SetCornerColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Corner>(funcInfo, "corner");
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, Color4, Corner>(&type, "GetCornerColor", &o2::Sprite::GetCornerColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Corner>(funcInfo, "corner");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const Color4&>(&type, "SetLeftTopColor", &o2::Sprite::SetLeftTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, Color4>(&type, "GetLeftTopCorner", &o2::Sprite::GetLeftTopCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const Color4&>(&type, "SetRightTopColor", &o2::Sprite::SetRightTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, Color4>(&type, "GetRightTopCorner", &o2::Sprite::GetRightTopCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const Color4&>(&type, "SetRightBottomColor", &o2::Sprite::SetRightBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, Color4>(&type, "GetRightBottomCorner", &o2::Sprite::GetRightBottomCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const Color4&>(&type, "SetLeftBottomColor", &o2::Sprite::SetLeftBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, Color4>(&type, "GetLeftBottomCorner", &o2::Sprite::GetLeftBottomCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, float>(&type, "SetFill", &o2::Sprite::SetFill, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "fill");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, float>(&type, "GetFill", &o2::Sprite::GetFill, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, SpriteMode>(&type, "SetMode", &o2::Sprite::SetMode, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SpriteMode>(funcInfo, "mode");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, SpriteMode>(&type, "GetMode", &o2::Sprite::GetMode, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const RectI&>(&type, "SetSliceBorder", &o2::Sprite::SetSliceBorder, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectI&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, RectI>(&type, "GetSliceBorder", &o2::Sprite::GetSliceBorder, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const ImageAsset&>(&type, "LoadFromImage", &o2::Sprite::LoadFromImage, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const ImageAsset&>(funcInfo, "image");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const String&>(&type, "LoadFromImage", &o2::Sprite::LoadFromImage, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "imagePath");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, AssetId>(&type, "LoadFromImage", &o2::Sprite::LoadFromImage, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "imageId");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const Color4&>(&type, "LoadMonoColor", &o2::Sprite::LoadMonoColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, Bitmap*>(&type, "LoadFromBitmap", &o2::Sprite::LoadFromBitmap, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, AssetId>(&type, "GetImageId", &o2::Sprite::GetImageId, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, AssetId>(&type, "GetAtlasAssetId", &o2::Sprite::GetAtlasAssetId, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "NormalizeSize", &o2::Sprite::NormalizeSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "NormalizeAspectByWidth", &o2::Sprite::NormalizeAspectByWidth, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "NormalizeAspectByHeight", &o2::Sprite::NormalizeAspectByHeight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "NormalizeAspect", &o2::Sprite::NormalizeAspect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BasisChanged", &o2::Sprite::BasisChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "ColorChanged", &o2::Sprite::ColorChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "UpdateMesh", &o2::Sprite::UpdateMesh, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildDefaultMesh", &o2::Sprite::BuildDefaultMesh, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildSlicedMesh", &o2::Sprite::BuildSlicedMesh, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFillLeftToRightMesh", &o2::Sprite::BuildFillLeftToRightMesh, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFillRightToLeftMesh", &o2::Sprite::BuildFillRightToLeftMesh, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFillUpToDownMesh", &o2::Sprite::BuildFillUpToDownMesh, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFillDownToUpMesh", &o2::Sprite::BuildFillDownToUpMesh, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFill360CWMesh", &o2::Sprite::BuildFill360CWMesh, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "BuildFill360CCWMesh", &o2::Sprite::BuildFill360CCWMesh, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, DataNode&>(&type, "OnSerialize", &o2::Sprite::OnSerialize, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void, const DataNode&>(&type, "OnDeserialized", &o2::Sprite::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "ReloadImage", &o2::Sprite::ReloadImage, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Sprite, void>(&type, "InitializeProperties", &o2::Sprite::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::Text::InitializeType(o2::Text* sample)
{
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)sample, sample->font, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)sample, sample->text, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "height", (size_t)(char*)(&sample->height) - (size_t)(char*)sample, sample->height, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "verAlign", (size_t)(char*)(&sample->verAlign) - (size_t)(char*)sample, sample->verAlign, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "horAlign", (size_t)(char*)(&sample->horAlign) - (size_t)(char*)sample, sample->horAlign, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "wordWrap", (size_t)(char*)(&sample->wordWrap) - (size_t)(char*)sample, sample->wordWrap, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "dotsEngings", (size_t)(char*)(&sample->dotsEngings) - (size_t)(char*)sample, sample->dotsEngings, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "symbolsDistanceCoef", (size_t)(char*)(&sample->symbolsDistanceCoef) - (size_t)(char*)sample, sample->symbolsDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "linesDistanceCoef", (size_t)(char*)(&sample->linesDistanceCoef) - (size_t)(char*)sample, sample->linesDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mMeshMaxPolyCount", (size_t)(char*)(&sample->mMeshMaxPolyCount) - (size_t)(char*)sample, sample->mMeshMaxPolyCount, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mText", (size_t)(char*)(&sample->mText) - (size_t)(char*)sample, sample->mText, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFontAssetId", (size_t)(char*)(&sample->mFontAssetId) - (size_t)(char*)sample, sample->mFontAssetId, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFont", (size_t)(char*)(&sample->mFont) - (size_t)(char*)sample, sample->mFont, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHeight", (size_t)(char*)(&sample->mHeight) - (size_t)(char*)sample, sample->mHeight, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSymbolsDistCoef", (size_t)(char*)(&sample->mSymbolsDistCoef) - (size_t)(char*)sample, sample->mSymbolsDistCoef, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLinesDistanceCoef", (size_t)(char*)(&sample->mLinesDistanceCoef) - (size_t)(char*)sample, sample->mLinesDistanceCoef, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mVerAlign", (size_t)(char*)(&sample->mVerAlign) - (size_t)(char*)sample, sample->mVerAlign, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHorAlign", (size_t)(char*)(&sample->mHorAlign) - (size_t)(char*)sample, sample->mHorAlign, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWordWrap", (size_t)(char*)(&sample->mWordWrap) - (size_t)(char*)sample, sample->mWordWrap, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDotsEndings", (size_t)(char*)(&sample->mDotsEndings) - (size_t)(char*)sample, sample->mDotsEndings, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMeshes", (size_t)(char*)(&sample->mMeshes) - (size_t)(char*)sample, sample->mMeshes, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastTransform", (size_t)(char*)(&sample->mLastTransform) - (size_t)(char*)sample, sample->mLastTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSymbolsSet", (size_t)(char*)(&sample->mSymbolsSet) - (size_t)(char*)sample, sample->mSymbolsSet, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mUpdatingMesh", (size_t)(char*)(&sample->mUpdatingMesh) - (size_t)(char*)sample, sample->mUpdatingMesh, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Text, void>(&type, "Draw", &o2::Text::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, FontRef>(&type, "SetFont", &o2::Text::SetFont, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<FontRef>(funcInfo, "font");
	funcInfo = TypeInitializer::RegFunction<o2::Text, FontRef>(&type, "GetFont", &o2::Text::GetFont, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, BitmapFontAsset*>(&type, "SetFontAsset", &o2::Text::SetFontAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<BitmapFontAsset*>(funcInfo, "asset");
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, VectorFontAsset*>(&type, "SetFontAsset", &o2::Text::SetFontAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFontAsset*>(funcInfo, "asset");
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, AssetId>(&type, "SetFontAsset", &o2::Text::SetFontAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "assetId");
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, const String&>(&type, "SetFontAsset", &o2::Text::SetFontAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "fileName");
	funcInfo = TypeInitializer::RegFunction<o2::Text, Asset*>(&type, "GetFontAsset", &o2::Text::GetFontAsset, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, AssetId>(&type, "GetFontAssetId", &o2::Text::GetFontAssetId, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, int>(&type, "SetHeight", &o2::Text::SetHeight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "height");
	funcInfo = TypeInitializer::RegFunction<o2::Text, int>(&type, "GetHeight", &o2::Text::GetHeight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, const WString&>(&type, "SetText", &o2::Text::SetText, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::Text, WString>(&type, "GetText", &o2::Text::GetText, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, HorAlign>(&type, "SetHorAlign", &o2::Text::SetHorAlign, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<HorAlign>(funcInfo, "align");
	funcInfo = TypeInitializer::RegFunction<o2::Text, HorAlign>(&type, "GetHorAlign", &o2::Text::GetHorAlign, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, VerAlign>(&type, "SetVerAlign", &o2::Text::SetVerAlign, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VerAlign>(funcInfo, "align");
	funcInfo = TypeInitializer::RegFunction<o2::Text, VerAlign>(&type, "GetVerAlign", &o2::Text::GetVerAlign, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, bool>(&type, "SetWordWrap", &o2::Text::SetWordWrap, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "flag");
	funcInfo = TypeInitializer::RegFunction<o2::Text, bool>(&type, "GetWordWrap", &o2::Text::GetWordWrap, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, bool>(&type, "SetDotsEngings", &o2::Text::SetDotsEngings, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "flag");
	funcInfo = TypeInitializer::RegFunction<o2::Text, bool>(&type, "IsDotsEngings", &o2::Text::IsDotsEngings, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, float>(&type, "SetSymbolsDistanceCoef", &o2::Text::SetSymbolsDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::Text, float>(&type, "GetSymbolsDistanceCoef", &o2::Text::GetSymbolsDistanceCoef, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, float>(&type, "SetLinesDistanceCoef", &o2::Text::SetLinesDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::Text, float>(&type, "GetLinesDistanceCoef", &o2::Text::GetLinesDistanceCoef, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, SymbolsSet&>(&type, "GetSymbolsSet", &o2::Text::GetSymbolsSet, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, Vec2F>(&type, "GetRealSize", &o2::Text::GetRealSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, RectF>(&type, "GetRealRect", &o2::Text::GetRealRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void>(&type, "UpdateMesh", &o2::Text::UpdateMesh, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, const Basis&>(&type, "TransformMesh", &o2::Text::TransformMesh, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "bas");
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, int>(&type, "PrepareMesh", &o2::Text::PrepareMesh, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<int>(funcInfo, "charactersCount");
	funcInfo = TypeInitializer::RegFunction<o2::Text, Basis>(&type, "CalculateTextBasis", &o2::Text::CalculateTextBasis, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void>(&type, "ColorChanged", &o2::Text::ColorChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void>(&type, "BasisChanged", &o2::Text::BasisChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Text, void, const DataNode&>(&type, "OnDeserialized", &o2::Text::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Text, void>(&type, "InitializeProperties", &o2::Text::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::FontStrokeEffect::InitializeType(o2::FontStrokeEffect* sample)
{
	TypeInitializer::RegField(&type, "radius", (size_t)(char*)(&sample->radius) - (size_t)(char*)sample, sample->radius, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "alphaThreshold", (size_t)(char*)(&sample->alphaThreshold) - (size_t)(char*)sample, sample->alphaThreshold, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)sample, sample->color, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontStrokeEffect, void, Bitmap*>(&type, "Process", &o2::FontStrokeEffect::Process, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontStrokeEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontStrokeEffect::GetSizeExtend, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FontStrokeEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontStrokeEffect::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::FontGradientEffect::InitializeType(o2::FontGradientEffect* sample)
{
	TypeInitializer::RegField(&type, "color1", (size_t)(char*)(&sample->color1) - (size_t)(char*)sample, sample->color1, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "color2", (size_t)(char*)(&sample->color2) - (size_t)(char*)sample, sample->color2, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "angle", (size_t)(char*)(&sample->angle) - (size_t)(char*)sample, sample->angle, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "length", (size_t)(char*)(&sample->length) - (size_t)(char*)sample, sample->length, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "origin", (size_t)(char*)(&sample->origin) - (size_t)(char*)sample, sample->origin, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontGradientEffect, void, Bitmap*>(&type, "Process", &o2::FontGradientEffect::Process, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontGradientEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontGradientEffect::GetSizeExtend, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FontGradientEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontGradientEffect::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::FontColorEffect::InitializeType(o2::FontColorEffect* sample)
{
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)sample, sample->color, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontColorEffect, void, Bitmap*>(&type, "Process", &o2::FontColorEffect::Process, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontColorEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontColorEffect::GetSizeExtend, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FontColorEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontColorEffect::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::FontShadowEffect::InitializeType(o2::FontShadowEffect* sample)
{
	TypeInitializer::RegField(&type, "blurRadius", (size_t)(char*)(&sample->blurRadius) - (size_t)(char*)sample, sample->blurRadius, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "offset", (size_t)(char*)(&sample->offset) - (size_t)(char*)sample, sample->offset, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)sample, sample->color, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontShadowEffect, void, Bitmap*>(&type, "Process", &o2::FontShadowEffect::Process, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontShadowEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontShadowEffect::GetSizeExtend, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FontShadowEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontShadowEffect::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::Actor::InitializeType(o2::Actor* sample)
{
	TypeInitializer::RegField(&type, "tags", (size_t)(char*)(&sample->tags) - (size_t)(char*)sample, sample->tags, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "id", (size_t)(char*)(&sample->id) - (size_t)(char*)sample, sample->id, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabled", (size_t)(char*)(&sample->enabled) - (size_t)(char*)sample, sample->enabled, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabledInHierarchy", (size_t)(char*)(&sample->enabledInHierarchy) - (size_t)(char*)sample, sample->enabledInHierarchy, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "locked", (size_t)(char*)(&sample->locked) - (size_t)(char*)sample, sample->locked, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "lockedInHierarchy", (size_t)(char*)(&sample->lockedInHierarchy) - (size_t)(char*)sample, sample->lockedInHierarchy, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "parent", (size_t)(char*)(&sample->parent) - (size_t)(char*)sample, sample->parent, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "layer", (size_t)(char*)(&sample->layer) - (size_t)(char*)sample, sample->layer, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "layerName", (size_t)(char*)(&sample->layerName) - (size_t)(char*)sample, sample->layerName, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "childs", (size_t)(char*)(&sample->childs) - (size_t)(char*)sample, sample->childs, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "child", (size_t)(char*)(&sample->child) - (size_t)(char*)sample, sample->child, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "components", (size_t)(char*)(&sample->components) - (size_t)(char*)sample, sample->components, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "component", (size_t)(char*)(&sample->component) - (size_t)(char*)sample, sample->component, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "transform", (size_t)(char*)(&sample->transform) - (size_t)(char*)sample, sample->transform, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "onEnableChanged", (size_t)(char*)(&sample->onEnableChanged) - (size_t)(char*)sample, sample->onEnableChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)sample, sample->mName, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mParent", (size_t)(char*)(&sample->mParent) - (size_t)(char*)sample, sample->mParent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mChilds", (size_t)(char*)(&sample->mChilds) - (size_t)(char*)sample, sample->mChilds, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCompontents", (size_t)(char*)(&sample->mCompontents) - (size_t)(char*)sample, sample->mCompontents, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLayer", (size_t)(char*)(&sample->mLayer) - (size_t)(char*)sample, sample->mLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEnabled", (size_t)(char*)(&sample->mEnabled) - (size_t)(char*)sample, sample->mEnabled, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResEnabled", (size_t)(char*)(&sample->mResEnabled) - (size_t)(char*)sample, sample->mResEnabled, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLocked", (size_t)(char*)(&sample->mLocked) - (size_t)(char*)sample, sample->mLocked, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResLocked", (size_t)(char*)(&sample->mResLocked) - (size_t)(char*)sample, sample->mResLocked, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsOnScene", (size_t)(char*)(&sample->mIsOnScene) - (size_t)(char*)sample, sample->mIsOnScene, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsAsset", (size_t)(char*)(&sample->mIsAsset) - (size_t)(char*)sample, sample->mIsAsset, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAssetId", (size_t)(char*)(&sample->mAssetId) - (size_t)(char*)sample, sample->mAssetId, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Actor, void, float>(&type, "Update", &o2::Actor::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, float>(&type, "UpdateChilds", &o2::Actor::UpdateChilds, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, const String&>(&type, "SetName", &o2::Actor::SetName, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, String>(&type, "GetName", &o2::Actor::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, UInt64>(&type, "GetID", &o2::Actor::GetID, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, UInt64>(&type, "SetId", &o2::Actor::SetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UInt64>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, AssetId>(&type, "GetAssetId", &o2::Actor::GetAssetId, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool>(&type, "IsAsset", &o2::Actor::IsAsset, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, bool>(&type, "GenNewId", &o2::Actor::GenNewId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "childs");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "ExcludeFromScene", &o2::Actor::ExcludeFromScene, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "IncludeInScene", &o2::Actor::IncludeInScene, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool>(&type, "IsOnScene", &o2::Actor::IsOnScene, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, bool>(&type, "SetEnabled", &o2::Actor::SetEnabled, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "active");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "Enable", &o2::Actor::Enable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "Disable", &o2::Actor::Disable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool>(&type, "IsEnabled", &o2::Actor::IsEnabled, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool>(&type, "IsEnabledInHierarchy", &o2::Actor::IsEnabledInHierarchy, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, bool>(&type, "SetLocked", &o2::Actor::SetLocked, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "locked");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "Lock", &o2::Actor::Lock, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "Unlock", &o2::Actor::Unlock, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool>(&type, "IsLocked", &o2::Actor::IsLocked, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, bool>(&type, "IsLockedInHierarchy", &o2::Actor::IsLockedInHierarchy, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, int>(&type, "SetPositionIndexInParent", &o2::Actor::SetPositionIndexInParent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "index");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Actor*, bool>(&type, "SetParent", &o2::Actor::SetParent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "worldPositionStays");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Actor*>(&type, "GetParent", &o2::Actor::GetParent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Actor*, Actor*>(&type, "AddChild", &o2::Actor::AddChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Actor*, Actor*, int>(&type, "AddChild", &o2::Actor::AddChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<int>(funcInfo, "index");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Actor*, const String&>(&type, "GetChild", &o2::Actor::GetChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, ActorsVec>(&type, "GetChilds", &o2::Actor::GetChilds, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Actor*, bool>(&type, "RemoveChild", &o2::Actor::RemoveChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "RemoveAllChilds", &o2::Actor::RemoveAllChilds, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Component*, Component*>(&type, "AddComponent", &o2::Actor::AddComponent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Component*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Component*, bool>(&type, "RemoveComponent", &o2::Actor::RemoveComponent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Component*>(funcInfo, "component");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "RemoveAllComponents", &o2::Actor::RemoveAllComponents, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Component*, const String&>(&type, "GetComponent", &o2::Actor::GetComponent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "typeName");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Component*, const Type*>(&type, "GetComponent", &o2::Actor::GetComponent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Type*>(funcInfo, "type");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Component*, UInt64>(&type, "GetComponent", &o2::Actor::GetComponent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UInt64>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, ComponentsVec>(&type, "GetComponents", &o2::Actor::GetComponents, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Scene::Layer*>(&type, "SetLayer", &o2::Actor::SetLayer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Scene::Layer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, const String&>(&type, "SetLayerName", &o2::Actor::SetLayerName, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "layerName");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Scene::Layer*>(&type, "GetLayer", &o2::Actor::GetLayer, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, String>(&type, "GetLayerName", &o2::Actor::GetLayerName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "OnTransformChanged", &o2::Actor::OnTransformChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Actor*>(&type, "SetParentProp", &o2::Actor::SetParentProp, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "UpdateEnabled", &o2::Actor::UpdateEnabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "UpdateLocking", &o2::Actor::UpdateLocking, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, DataNode&>(&type, "OnSerialize", &o2::Actor::OnSerialize, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, const DataNode&>(&type, "OnDeserialized", &o2::Actor::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Dictionary<String, Actor*>>(&type, "GetAllChilds", &o2::Actor::GetAllChilds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Dictionary<String, Component*>>(&type, "GetAllComponents", &o2::Actor::GetAllComponents, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "ComponentsExcludeFromScene", &o2::Actor::ComponentsExcludeFromScene, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "ComponentsIncludeToScene", &o2::Actor::ComponentsIncludeToScene, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "InitializeProperties", &o2::Actor::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::ActorTransform::InitializeType(o2::ActorTransform* sample)
{
	TypeInitializer::RegField(&type, "actor", (size_t)(char*)(&sample->actor) - (size_t)(char*)sample, sample->actor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldPosition", (size_t)(char*)(&sample->worldPosition) - (size_t)(char*)sample, sample->worldPosition, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldRect", (size_t)(char*)(&sample->worldRect) - (size_t)(char*)sample, sample->worldRect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldAngle", (size_t)(char*)(&sample->worldAngle) - (size_t)(char*)sample, sample->worldAngle, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldBasis", (size_t)(char*)(&sample->worldBasis) - (size_t)(char*)sample, sample->worldBasis, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldNonSizedBasis", (size_t)(char*)(&sample->worldNonSizedBasis) - (size_t)(char*)sample, sample->worldNonSizedBasis, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldAABB", (size_t)(char*)(&sample->worldAABB) - (size_t)(char*)sample, sample->worldAABB, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mWorldNonSizedTransform", (size_t)(char*)(&sample->mWorldNonSizedTransform) - (size_t)(char*)sample, sample->mWorldNonSizedTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mWorldTransform", (size_t)(char*)(&sample->mWorldTransform) - (size_t)(char*)sample, sample->mWorldTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mParentInvertedTransform", (size_t)(char*)(&sample->mParentInvertedTransform) - (size_t)(char*)sample, sample->mParentInvertedTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mParentTransform", (size_t)(char*)(&sample->mParentTransform) - (size_t)(char*)sample, sample->mParentTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsParentInvTransformActual", (size_t)(char*)(&sample->mIsParentInvTransformActual) - (size_t)(char*)sample, sample->mIsParentInvTransformActual, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Actor*>(&type, "GetOwnerActor", &o2::ActorTransform::GetOwnerActor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldPivot", &o2::ActorTransform::SetWorldPivot, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "pivot");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldPivot", &o2::ActorTransform::GetWorldPivot, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldPosition", &o2::ActorTransform::SetWorldPosition, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldPosition", &o2::ActorTransform::GetWorldPosition, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const RectF&>(&type, "SetWorldRect", &o2::ActorTransform::SetWorldRect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, RectF>(&type, "GetWorldRect", &o2::ActorTransform::GetWorldRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, float>(&type, "SetWorldAngle", &o2::ActorTransform::SetWorldAngle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "rad");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, float>(&type, "GetWorldAngle", &o2::ActorTransform::GetWorldAngle, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Basis&>(&type, "SetWorldBasis", &o2::ActorTransform::SetWorldBasis, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "basis");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Basis>(&type, "GetWorldBasis", &o2::ActorTransform::GetWorldBasis, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Basis&>(&type, "SetWorldNonSizedBasis", &o2::ActorTransform::SetWorldNonSizedBasis, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "basis");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Basis>(&type, "GetWorldNonSizedBasis", &o2::ActorTransform::GetWorldNonSizedBasis, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const RectF&>(&type, "SetWorldAxisAlignedRect", &o2::ActorTransform::SetWorldAxisAlignedRect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, RectF>(&type, "GetWorldAxisAlignedRect", &o2::ActorTransform::GetWorldAxisAlignedRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldLeftTop", &o2::ActorTransform::SetWorldLeftTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldLeftTop", &o2::ActorTransform::GetWorldLeftTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldRightTop", &o2::ActorTransform::SetWorldRightTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldRightTop", &o2::ActorTransform::GetWorldRightTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldLeftBottom", &o2::ActorTransform::SetWorldLeftBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldLeftBottom", &o2::ActorTransform::GetWorldLeftBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldRightBottom", &o2::ActorTransform::SetWorldRightBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldRightBottom", &o2::ActorTransform::GetWorldRightBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetWorldCenter", &o2::ActorTransform::SetWorldCenter, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetWorldCenter", &o2::ActorTransform::GetWorldCenter, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetRight", &o2::ActorTransform::SetRight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetRight", &o2::ActorTransform::GetRight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetLeft", &o2::ActorTransform::SetLeft, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetLeft", &o2::ActorTransform::GetLeft, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetUp", &o2::ActorTransform::SetUp, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetUp", &o2::ActorTransform::GetUp, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "SetDown", &o2::ActorTransform::SetDown, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F>(&type, "GetDown", &o2::ActorTransform::GetDown, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, const Vec2F&>(&type, "LookAt", &o2::ActorTransform::LookAt, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldPoint");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F, const Vec2F&>(&type, "World2LocalPoint", &o2::ActorTransform::World2LocalPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldPoint");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F, const Vec2F&>(&type, "Local2WorldPoint", &o2::ActorTransform::Local2WorldPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "localPoint");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F, const Vec2F&>(&type, "World2LocalDir", &o2::ActorTransform::World2LocalDir, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldDir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, Vec2F, const Vec2F&>(&type, "Local2WorldDir", &o2::ActorTransform::Local2WorldDir, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "localDir");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, bool, const Vec2F&>(&type, "IsPointInside", &o2::ActorTransform::IsPointInside, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void, Actor*>(&type, "SetOwner", &o2::ActorTransform::SetOwner, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void>(&type, "UpdateTransform", &o2::ActorTransform::UpdateTransform, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void>(&type, "CheckParentInvTransform", &o2::ActorTransform::CheckParentInvTransform, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::ActorTransform, void>(&type, "InitializeProperties", &o2::ActorTransform::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::Component::InitializeType(o2::Component* sample)
{
	TypeInitializer::RegField(&type, "actor", (size_t)(char*)(&sample->actor) - (size_t)(char*)sample, sample->actor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabled", (size_t)(char*)(&sample->enabled) - (size_t)(char*)sample, sample->enabled, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabledInHierarchy", (size_t)(char*)(&sample->enabledInHierarchy) - (size_t)(char*)sample, sample->enabledInHierarchy, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEnabled", (size_t)(char*)(&sample->mEnabled) - (size_t)(char*)sample, sample->mEnabled, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResEnabled", (size_t)(char*)(&sample->mResEnabled) - (size_t)(char*)sample, sample->mResEnabled, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Component, UInt64>(&type, "GetID", &o2::Component::GetID, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void, float>(&type, "Update", &o2::Component::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::Component, void, bool>(&type, "SetEnabled", &o2::Component::SetEnabled, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "active");
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "Enable", &o2::Component::Enable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "Disable", &o2::Component::Disable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Component, bool>(&type, "IsEnabled", &o2::Component::IsEnabled, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Component, bool>(&type, "IsEnabledInHierarchy", &o2::Component::IsEnabledInHierarchy, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Component, Actor*>(&type, "GetOwnerActor", &o2::Component::GetOwnerActor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void, Scene::Layer*, Scene::Layer*>(&type, "OnLayerChanged", &o2::Component::OnLayerChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Scene::Layer*>(funcInfo, "oldLayer");
	TypeInitializer::RegFuncParam<Scene::Layer*>(funcInfo, "newLayer");
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "UpdateEnabled", &o2::Component::UpdateEnabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "OnTransformChanged", &o2::Component::OnTransformChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void, Actor*>(&type, "SetOwnerActor", &o2::Component::SetOwnerActor, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "OnExcludeFromScene", &o2::Component::OnExcludeFromScene, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "OnIncludeToScene", &o2::Component::OnIncludeToScene, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Component, void>(&type, "InitializeProperties", &o2::Component::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::DrawableComponent::InitializeType(o2::DrawableComponent* sample)
{
	TypeInitializer::RegField(&type, "drawDepth", (size_t)(char*)(&sample->drawDepth) - (size_t)(char*)sample, sample->drawDepth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mDrawingDepth", (size_t)(char*)(&sample->mDrawingDepth) - (size_t)(char*)sample, sample->mDrawingDepth, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void, float>(&type, "SetDrawingDepth", &o2::DrawableComponent::SetDrawingDepth, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "depth");
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, float>(&type, "GetDrawingDepth", &o2::DrawableComponent::GetDrawingDepth, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void, Scene::Layer*, Scene::Layer*>(&type, "OnLayerChanged", &o2::DrawableComponent::OnLayerChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Scene::Layer*>(funcInfo, "oldLayer");
	TypeInitializer::RegFuncParam<Scene::Layer*>(funcInfo, "newLayer");
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void>(&type, "UpdateEnabled", &o2::DrawableComponent::UpdateEnabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void, Actor*>(&type, "SetOwnerActor", &o2::DrawableComponent::SetOwnerActor, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void>(&type, "OnExcludeFromScene", &o2::DrawableComponent::OnExcludeFromScene, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void>(&type, "OnIncludeToScene", &o2::DrawableComponent::OnIncludeToScene, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::DrawableComponent, void>(&type, "InitializeProperties", &o2::DrawableComponent::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::Tag::InitializeType(o2::Tag* sample)
{
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)sample, sample->mName, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mActors", (size_t)(char*)(&sample->mActors) - (size_t)(char*)sample, sample->mActors, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Tag, const String&>(&type, "GetName", &o2::Tag::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Tag, void, const String&>(&type, "SetName", &o2::Tag::SetName, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::Tag, void, Actor*>(&type, "AddActor", &o2::Tag::AddActor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::Tag, void, Actor*>(&type, "RemoveActor", &o2::Tag::RemoveActor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::Tag, void>(&type, "Clear", &o2::Tag::Clear, o2::ProtectSection::Public);
}

void o2::TagGroup::InitializeType(o2::TagGroup* sample)
{
	TypeInitializer::RegField(&type, "onTagAdded", (size_t)(char*)(&sample->onTagAdded) - (size_t)(char*)sample, sample->onTagAdded, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onTagRemoved", (size_t)(char*)(&sample->onTagRemoved) - (size_t)(char*)sample, sample->onTagRemoved, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTags", (size_t)(char*)(&sample->mTags) - (size_t)(char*)sample, sample->mTags, o2::ProtectSection::Private);
	auto funcInfo = TypeInitializer::RegFunction<o2::TagGroup, void, const String&>(&type, "AddTag", &o2::TagGroup::AddTag, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::TagGroup, void, Tag*>(&type, "AddTag", &o2::TagGroup::AddTag, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Tag*>(funcInfo, "tag");
	funcInfo = TypeInitializer::RegFunction<o2::TagGroup, void, const String&>(&type, "RemoveTag", &o2::TagGroup::RemoveTag, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::TagGroup, void, Tag*>(&type, "RemoveTag", &o2::TagGroup::RemoveTag, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Tag*>(funcInfo, "tag");
	funcInfo = TypeInitializer::RegFunction<o2::TagGroup, void>(&type, "Clear", &o2::TagGroup::Clear, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::TagGroup, bool, const String&>(&type, "IsHaveTag", &o2::TagGroup::IsHaveTag, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::TagGroup, bool, Tag*>(&type, "IsHaveTag", &o2::TagGroup::IsHaveTag, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Tag*>(funcInfo, "tag");
	funcInfo = TypeInitializer::RegFunction<o2::TagGroup, const TagsVec&>(&type, "GetTags", &o2::TagGroup::GetTags, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::TagGroup, StringsVec>(&type, "GetTagsNames", &o2::TagGroup::GetTagsNames, o2::ProtectSection::Public);
}

void o2::EditorTestComponent::InitializeType(o2::EditorTestComponent* sample)
{
	TypeInitializer::RegField(&type, "mInteger", (size_t)(char*)(&sample->mInteger) - (size_t)(char*)sample, sample->mInteger, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFloat", (size_t)(char*)(&sample->mFloat) - (size_t)(char*)sample, sample->mFloat, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mString", (size_t)(char*)(&sample->mString) - (size_t)(char*)sample, sample->mString, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWString", (size_t)(char*)(&sample->mWString) - (size_t)(char*)sample, sample->mWString, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBool", (size_t)(char*)(&sample->mBool) - (size_t)(char*)sample, sample->mBool, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mImageAsset", (size_t)(char*)(&sample->mImageAsset) - (size_t)(char*)sample, sample->mImageAsset, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mActorAsset", (size_t)(char*)(&sample->mActorAsset) - (size_t)(char*)sample, sample->mActorAsset, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDataAsset", (size_t)(char*)(&sample->mDataAsset) - (size_t)(char*)sample, sample->mDataAsset, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mActor", (size_t)(char*)(&sample->mActor) - (size_t)(char*)sample, sample->mActor, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTags", (size_t)(char*)(&sample->mTags) - (size_t)(char*)sample, sample->mTags, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLayer", (size_t)(char*)(&sample->mLayer) - (size_t)(char*)sample, sample->mLayer, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mComponent", (size_t)(char*)(&sample->mComponent) - (size_t)(char*)sample, sample->mComponent, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mImageComponent", (size_t)(char*)(&sample->mImageComponent) - (size_t)(char*)sample, sample->mImageComponent, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mColor", (size_t)(char*)(&sample->mColor) - (size_t)(char*)sample, sample->mColor, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mVec2F", (size_t)(char*)(&sample->mVec2F) - (size_t)(char*)sample, sample->mVec2F, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mVec2I", (size_t)(char*)(&sample->mVec2I) - (size_t)(char*)sample, sample->mVec2I, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mVertex", (size_t)(char*)(&sample->mVertex) - (size_t)(char*)sample, sample->mVertex, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mRectF", (size_t)(char*)(&sample->mRectF) - (size_t)(char*)sample, sample->mRectF, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mRectI", (size_t)(char*)(&sample->mRectI) - (size_t)(char*)sample, sample->mRectI, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::ImageComponent::InitializeType(o2::ImageComponent* sample)
{
	TypeInitializer::RegField(&type, "texture", (size_t)(char*)(&sample->texture) - (size_t)(char*)sample, sample->texture, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "textureSrcRect", (size_t)(char*)(&sample->textureSrcRect) - (size_t)(char*)sample, sample->textureSrcRect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAssetId", (size_t)(char*)(&sample->imageAssetId) - (size_t)(char*)sample, sample->imageAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAssetPath", (size_t)(char*)(&sample->imageAssetPath) - (size_t)(char*)sample, sample->imageAssetPath, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAsset", (size_t)(char*)(&sample->imageAsset) - (size_t)(char*)sample, sample->imageAsset, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "bitmap", (size_t)(char*)(&sample->bitmap) - (size_t)(char*)sample, sample->bitmap, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftTopColor", (size_t)(char*)(&sample->leftTopColor) - (size_t)(char*)sample, sample->leftTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightTopColor", (size_t)(char*)(&sample->rightTopColor) - (size_t)(char*)sample, sample->rightTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftBottomColor", (size_t)(char*)(&sample->leftBottomColor) - (size_t)(char*)sample, sample->leftBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightBottomColor", (size_t)(char*)(&sample->rightBottomColor) - (size_t)(char*)sample, sample->rightBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mode", (size_t)(char*)(&sample->mode) - (size_t)(char*)sample, sample->mode, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fill", (size_t)(char*)(&sample->fill) - (size_t)(char*)sample, sample->fill, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "sliceBorder", (size_t)(char*)(&sample->sliceBorder) - (size_t)(char*)sample, sample->sliceBorder, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mSprite", (size_t)(char*)(&sample->mSprite) - (size_t)(char*)sample, sample->mSprite, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "Draw", &o2::ImageComponent::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, TextureRef>(&type, "SetTexture", &o2::ImageComponent::SetTexture, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<TextureRef>(funcInfo, "texture");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, TextureRef>(&type, "GetTexture", &o2::ImageComponent::GetTexture, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const RectI&>(&type, "SetTextureSrcRect", &o2::ImageComponent::SetTextureSrcRect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectI&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, RectI>(&type, "GetTextureSrcRect", &o2::ImageComponent::GetTextureSrcRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, Corner, const Color4&>(&type, "SetCornerColor", &o2::ImageComponent::SetCornerColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Corner>(funcInfo, "corner");
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, Color4, Corner>(&type, "GetCornerColor", &o2::ImageComponent::GetCornerColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Corner>(funcInfo, "corner");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const Color4&>(&type, "SetLeftTopColor", &o2::ImageComponent::SetLeftTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, Color4>(&type, "GetLeftTopCorner", &o2::ImageComponent::GetLeftTopCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const Color4&>(&type, "SetRightTopColor", &o2::ImageComponent::SetRightTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, Color4>(&type, "GetRightTopCorner", &o2::ImageComponent::GetRightTopCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const Color4&>(&type, "SetRightBottomColor", &o2::ImageComponent::SetRightBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, Color4>(&type, "GetRightBottomCorner", &o2::ImageComponent::GetRightBottomCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const Color4&>(&type, "SetLeftBottomColor", &o2::ImageComponent::SetLeftBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, Color4>(&type, "GetLeftBottomCorner", &o2::ImageComponent::GetLeftBottomCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, float>(&type, "SetFill", &o2::ImageComponent::SetFill, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "fill");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, float>(&type, "GetFill", &o2::ImageComponent::GetFill, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, SpriteMode>(&type, "SetMode", &o2::ImageComponent::SetMode, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SpriteMode>(funcInfo, "mode");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, SpriteMode>(&type, "GetMode", &o2::ImageComponent::GetMode, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const RectI&>(&type, "SetSliceBorder", &o2::ImageComponent::SetSliceBorder, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectI&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, RectI>(&type, "GetSliceBorder", &o2::ImageComponent::GetSliceBorder, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const ImageAsset&>(&type, "LoadFromImage", &o2::ImageComponent::LoadFromImage, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const ImageAsset&>(funcInfo, "image");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const String&>(&type, "LoadFromImage", &o2::ImageComponent::LoadFromImage, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "imagePath");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, AssetId>(&type, "LoadFromImage", &o2::ImageComponent::LoadFromImage, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "imageId");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, const Color4&>(&type, "LoadMonoColor", &o2::ImageComponent::LoadMonoColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, Bitmap*>(&type, "LoadFromBitmap", &o2::ImageComponent::LoadFromBitmap, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, AssetId>(&type, "GetImageId", &o2::ImageComponent::GetImageId, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "NormalizeSize", &o2::ImageComponent::NormalizeSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "NormalizeAspectByWidth", &o2::ImageComponent::NormalizeAspectByWidth, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "NormalizeAspectByHeight", &o2::ImageComponent::NormalizeAspectByHeight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "NormalizeAspect", &o2::ImageComponent::NormalizeAspect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "OnTransformChanged", &o2::ImageComponent::OnTransformChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void, Actor*>(&type, "SetOwnerActor", &o2::ImageComponent::SetOwnerActor, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<o2::ImageComponent, void>(&type, "InitializeProperties", &o2::ImageComponent::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIButton::InitializeType(o2::UIButton* sample)
{
	TypeInitializer::RegField(&type, "caption", (size_t)(char*)(&sample->caption) - (size_t)(char*)sample, sample->caption, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "icon", (size_t)(char*)(&sample->icon) - (size_t)(char*)sample, sample->icon, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "buttonsGroup", (size_t)(char*)(&sample->buttonsGroup) - (size_t)(char*)sample, sample->buttonsGroup, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)sample, sample->onClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)sample, sample->shortcut, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mCaptionText", (size_t)(char*)(&sample->mCaptionText) - (size_t)(char*)sample, sample->mCaptionText, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIconSprite", (size_t)(char*)(&sample->mIconSprite) - (size_t)(char*)sample, sample->mIconSprite, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mButtonGroup", (size_t)(char*)(&sample->mButtonGroup) - (size_t)(char*)sample, sample->mButtonGroup, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIButton, void>(&type, "Draw", &o2::UIButton::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const WString&>(&type, "SetCaption", &o2::UIButton::SetCaption, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, WString>(&type, "GetCaption", &o2::UIButton::GetCaption, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, Sprite*>(&type, "SetIcon", &o2::UIButton::SetIcon, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Sprite*>(funcInfo, "sprite");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, Sprite*>(&type, "GetIcon", &o2::UIButton::GetIcon, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, UIButtonGroup*>(&type, "SetButtonGroup", &o2::UIButton::SetButtonGroup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIButtonGroup*>(funcInfo, "group");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, UIButtonGroup*>(&type, "GetButtonGroup", &o2::UIButton::GetButtonGroup, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, bool>(&type, "IsSelectable", &o2::UIButton::IsSelectable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIButton::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIButton::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIButton::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIButton::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIButton::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIButton::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Key&>(&type, "OnKeyPressed", &o2::UIButton::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, const Input::Key&>(&type, "OnKeyReleased", &o2::UIButton::OnKeyReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIButton::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void>(&type, "OnVisibleChanged", &o2::UIButton::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, void>(&type, "InitializeProperties", &o2::UIButton::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIContextMenuItem::InitializeType(o2::UIContextMenuItem* sample)
{
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)sample, sample->onClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)sample, sample->shortcut, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mSubMenu", (size_t)(char*)(&sample->mSubMenu) - (size_t)(char*)sample, sample->mSubMenu, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIContextMenuItem, UIContextMenu*>(&type, "GetSubMenu", &o2::UIContextMenuItem::GetSubMenu, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenuItem, void, UIWidget*>(&type, "OnChildAdded", &o2::UIContextMenuItem::OnChildAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
}

void o2::UIContextMenu::InitializeType(o2::UIContextMenu* sample)
{
	TypeInitializer::RegField(&type, "mOpenSubMenuDelay", (size_t)(char*)(&sample->mOpenSubMenuDelay) - (size_t)(char*)sample, sample->mOpenSubMenuDelay, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFitSizeMin", (size_t)(char*)(&sample->mFitSizeMin) - (size_t)(char*)sample, sample->mFitSizeMin, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxVisibleItems", (size_t)(char*)(&sample->mMaxVisibleItems) - (size_t)(char*)sample, sample->mMaxVisibleItems, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mParentContextMenu", (size_t)(char*)(&sample->mParentContextMenu) - (size_t)(char*)sample, sample->mParentContextMenu, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mChildContextMenu", (size_t)(char*)(&sample->mChildContextMenu) - (size_t)(char*)sample, sample->mChildContextMenu, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLayout", (size_t)(char*)(&sample->mLayout) - (size_t)(char*)sample, sample->mLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)sample, sample->mItemSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSeparatorSample", (size_t)(char*)(&sample->mSeparatorSample) - (size_t)(char*)sample, sample->mSeparatorSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)sample, sample->mSelectionDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)sample, sample->mSelectionLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurrentSelectionRect", (size_t)(char*)(&sample->mCurrentSelectionRect) - (size_t)(char*)sample, sample->mCurrentSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetSelectionRect", (size_t)(char*)(&sample->mTargetSelectionRect) - (size_t)(char*)sample, sample->mTargetSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)sample, sample->mLastSelectCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedItem", (size_t)(char*)(&sample->mSelectedItem) - (size_t)(char*)sample, sample->mSelectedItem, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectSubContextTime", (size_t)(char*)(&sample->mSelectSubContextTime) - (size_t)(char*)sample, sample->mSelectSubContextTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mShownAtFrame", (size_t)(char*)(&sample->mShownAtFrame) - (size_t)(char*)sample, sample->mShownAtFrame, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, float>(&type, "Update", &o2::UIContextMenu::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "Draw", &o2::UIContextMenu::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, UIContextMenu*, const Vec2F&>(&type, "Show", &o2::UIContextMenu::Show, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIContextMenu*>(funcInfo, "parent");
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Vec2F&>(&type, "Show", &o2::UIContextMenu::Show, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIWidget*, const Item&>(&type, "AddItem", &o2::UIContextMenu::AddItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIWidget*, const WString&, const Function<void()>&, ImageAsset*, const ShortcutKeys&>(&type, "AddItem", &o2::UIContextMenu::AddItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "path");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "clickFunc");
	TypeInitializer::RegFuncParam<ImageAsset*>(funcInfo, "icon");
	TypeInitializer::RegFuncParam<const ShortcutKeys&>(funcInfo, "shortcut");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIWidget*, const Item&, int>(&type, "InsertItem", &o2::UIContextMenu::InsertItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, Vector<Item>>(&type, "AddItems", &o2::UIContextMenu::AddItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Item>>(funcInfo, "items");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, Vector<Item>, int>(&type, "InsertItems", &o2::UIContextMenu::InsertItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Item>>(funcInfo, "items");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, Item, int>(&type, "GetItem", &o2::UIContextMenu::GetItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, Vector<Item>>(&type, "GetItems", &o2::UIContextMenu::GetItems, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, int>(&type, "RemoveItem", &o2::UIContextMenu::RemoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const WString&>(&type, "RemoveItem", &o2::UIContextMenu::RemoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "RemoveAllItems", &o2::UIContextMenu::RemoveAllItems, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIVerticalLayout*>(&type, "GetItemsLayout", &o2::UIContextMenu::GetItemsLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIContextMenuItem*>(&type, "GetItemSample", &o2::UIContextMenu::GetItemSample, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIWidget*>(&type, "GetSeparatorSample", &o2::UIContextMenu::GetSeparatorSample, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, Sprite*>(&type, "GetSelectionDrawable", &o2::UIContextMenu::GetSelectionDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Layout&>(&type, "SetSelectionDrawableLayout", &o2::UIContextMenu::SetSelectionDrawableLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, Layout>(&type, "GetSelectionDrawableLayout", &o2::UIContextMenu::GetSelectionDrawableLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, float>(&type, "SetMinFitSize", &o2::UIContextMenu::SetMinFitSize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, int>(&type, "SetMaxItemsVisible", &o2::UIContextMenu::SetMaxItemsVisible, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, bool>(&type, "IsScrollable", &o2::UIContextMenu::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, bool>(&type, "UpdateLayout", &o2::UIContextMenu::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "FitSize", &o2::UIContextMenu::FitSize, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "FitPosition", &o2::UIContextMenu::FitPosition, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "SpecialDraw", &o2::UIContextMenu::SpecialDraw, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIContextMenuItem*, const Item&>(&type, "CreateItem", &o2::UIContextMenu::CreateItem, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, Item, int>(&type, "GetItemDef", &o2::UIContextMenu::GetItemDef, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<int>(funcInfo, "idx");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "OnVisibleChanged", &o2::UIContextMenu::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, UIContextMenuItem*, const Vec2F&>(&type, "GetItemUnderPoint", &o2::UIContextMenu::GetItemUnderPoint, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Vec2F&>(&type, "UpdateHover", &o2::UIContextMenu::UpdateHover, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIContextMenu::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIContextMenu::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIContextMenu::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIContextMenu::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UIContextMenu::OnCursorMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const Input::Key&>(&type, "OnKeyPressed", &o2::UIContextMenu::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "HideWithParent", &o2::UIContextMenu::HideWithParent, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void>(&type, "HideWithChild", &o2::UIContextMenu::HideWithChild, o2::ProtectSection::Protected);
}

void o2::UICustomDropDown::InitializeType(o2::UICustomDropDown* sample)
{
	TypeInitializer::RegField(&type, "selectedItem", (size_t)(char*)(&sample->selectedItem) - (size_t)(char*)sample, sample->selectedItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "selectedItemPos", (size_t)(char*)(&sample->selectedItemPos) - (size_t)(char*)sample, sample->selectedItemPos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "item", (size_t)(char*)(&sample->item) - (size_t)(char*)sample, sample->item, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "itemsCount", (size_t)(char*)(&sample->itemsCount) - (size_t)(char*)sample, sample->itemsCount, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedPos", (size_t)(char*)(&sample->onSelectedPos) - (size_t)(char*)sample, sample->onSelectedPos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedItem", (size_t)(char*)(&sample->onSelectedItem) - (size_t)(char*)sample, sample->onSelectedItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mList", (size_t)(char*)(&sample->mList) - (size_t)(char*)sample, sample->mList, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mClipLayout", (size_t)(char*)(&sample->mClipLayout) - (size_t)(char*)sample, sample->mClipLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAbsoluteClip", (size_t)(char*)(&sample->mAbsoluteClip) - (size_t)(char*)sample, sample->mAbsoluteClip, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMaxListItems", (size_t)(char*)(&sample->mMaxListItems) - (size_t)(char*)sample, sample->mMaxListItems, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, float>(&type, "Update", &o2::UICustomDropDown::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "Draw", &o2::UICustomDropDown::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "Expand", &o2::UICustomDropDown::Expand, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "Collapse", &o2::UICustomDropDown::Collapse, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, bool>(&type, "IsExpanded", &o2::UICustomDropDown::IsExpanded, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, UIWidget*>(&type, "SetItemSample", &o2::UICustomDropDown::SetItemSample, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "sample");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIWidget*>(&type, "GetItemSample", &o2::UICustomDropDown::GetItemSample, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIVerticalLayout*>(&type, "GetLayout", &o2::UICustomDropDown::GetLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIWidget*>(&type, "AddItem", &o2::UICustomDropDown::AddItem, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIWidget*, int>(&type, "AddItem", &o2::UICustomDropDown::AddItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, UIWidget*>(&type, "RemoveItem", &o2::UICustomDropDown::RemoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, int>(&type, "RemoveItem", &o2::UICustomDropDown::RemoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, int, int>(&type, "MoveItem", &o2::UICustomDropDown::MoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	TypeInitializer::RegFuncParam<int>(funcInfo, "newPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, UIWidget*, int>(&type, "MoveItem", &o2::UICustomDropDown::MoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	TypeInitializer::RegFuncParam<int>(funcInfo, "newPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, int, UIWidget*>(&type, "GetItemPosition", &o2::UICustomDropDown::GetItemPosition, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIWidget*, int>(&type, "GetItem", &o2::UICustomDropDown::GetItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "RemoveAllItems", &o2::UICustomDropDown::RemoveAllItems, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Function<bool(UIWidget*, UIWidget*)>&>(&type, "SortItems", &o2::UICustomDropDown::SortItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Function<bool(UIWidget*, UIWidget*)>&>(funcInfo, "sortFunc");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, int>(&type, "GetItemsCount", &o2::UICustomDropDown::GetItemsCount, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, UIWidget*>(&type, "SelectItem", &o2::UICustomDropDown::SelectItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, int>(&type, "SelectItemAt", &o2::UICustomDropDown::SelectItemAt, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UIWidget*>(&type, "GetSelectedItem", &o2::UICustomDropDown::GetSelectedItem, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, int>(&type, "GetSelectedItemPosition", &o2::UICustomDropDown::GetSelectedItemPosition, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, UICustomList*>(&type, "GetListView", &o2::UICustomDropDown::GetListView, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, int>(&type, "SetMaxListSizeInItems", &o2::UICustomDropDown::SetMaxListSizeInItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "itemsCount");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Layout&>(&type, "SetClippingLayout", &o2::UICustomDropDown::SetClippingLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, Layout>(&type, "GetClippingLayout", &o2::UICustomDropDown::GetClippingLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UICustomDropDown::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UICustomDropDown::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UICustomDropDown::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UICustomDropDown::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UICustomDropDown::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "OnVisibleChanged", &o2::UICustomDropDown::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, bool>(&type, "UpdateLayout", &o2::UICustomDropDown::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "OnItemSelected", &o2::UICustomDropDown::OnItemSelected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "OnSelectionChanged", &o2::UICustomDropDown::OnSelectionChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "InitializeProperties", &o2::UICustomDropDown::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UICustomList::InitializeType(o2::UICustomList* sample)
{
	TypeInitializer::RegField(&type, "selectedItems", (size_t)(char*)(&sample->selectedItems) - (size_t)(char*)sample, sample->selectedItems, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "selectedItem", (size_t)(char*)(&sample->selectedItem) - (size_t)(char*)sample, sample->selectedItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "selectedItemPos", (size_t)(char*)(&sample->selectedItemPos) - (size_t)(char*)sample, sample->selectedItemPos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "item", (size_t)(char*)(&sample->item) - (size_t)(char*)sample, sample->item, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "itemsCount", (size_t)(char*)(&sample->itemsCount) - (size_t)(char*)sample, sample->itemsCount, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedPos", (size_t)(char*)(&sample->onSelectedPos) - (size_t)(char*)sample, sample->onSelectedPos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedItem", (size_t)(char*)(&sample->onSelectedItem) - (size_t)(char*)sample, sample->onSelectedItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mVerLayout", (size_t)(char*)(&sample->mVerLayout) - (size_t)(char*)sample, sample->mVerLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)sample, sample->mItemSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)sample, sample->mSelectionDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverDrawable", (size_t)(char*)(&sample->mHoverDrawable) - (size_t)(char*)sample, sample->mHoverDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)sample, sample->mSelectionLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverLayout", (size_t)(char*)(&sample->mHoverLayout) - (size_t)(char*)sample, sample->mHoverLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMultiSelection", (size_t)(char*)(&sample->mMultiSelection) - (size_t)(char*)sample, sample->mMultiSelection, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectedItems", (size_t)(char*)(&sample->mSelectedItems) - (size_t)(char*)sample, sample->mSelectedItems, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentHoverRect", (size_t)(char*)(&sample->mCurrentHoverRect) - (size_t)(char*)sample, sample->mCurrentHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetHoverRect", (size_t)(char*)(&sample->mTargetHoverRect) - (size_t)(char*)sample, sample->mTargetHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastHoverCheckCursor", (size_t)(char*)(&sample->mLastHoverCheckCursor) - (size_t)(char*)sample, sample->mLastHoverCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)sample, sample->mLastSelectCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionSpritesPool", (size_t)(char*)(&sample->mSelectionSpritesPool) - (size_t)(char*)sample, sample->mSelectionSpritesPool, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, float>(&type, "Update", &o2::UICustomList::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "Draw", &o2::UICustomList::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, UIWidget*>(&type, "SetItemSample", &o2::UICustomList::SetItemSample, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "sample");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*>(&type, "GetItemSample", &o2::UICustomList::GetItemSample, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIVerticalLayout*>(&type, "GetLayout", &o2::UICustomList::GetLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*>(&type, "AddItem", &o2::UICustomList::AddItem, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*, int>(&type, "AddItem", &o2::UICustomList::AddItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, UIWidget*>(&type, "RemoveItem", &o2::UICustomList::RemoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, int>(&type, "RemoveItem", &o2::UICustomList::RemoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, int, int>(&type, "MoveItem", &o2::UICustomList::MoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	TypeInitializer::RegFuncParam<int>(funcInfo, "newPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, UIWidget*, int>(&type, "MoveItem", &o2::UICustomList::MoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	TypeInitializer::RegFuncParam<int>(funcInfo, "newPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, int, UIWidget*>(&type, "GetItemPosition", &o2::UICustomList::GetItemPosition, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*, int>(&type, "GetItem", &o2::UICustomList::GetItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "RemoveAllItems", &o2::UICustomList::RemoveAllItems, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Function<bool(UIWidget*, UIWidget*)>&>(&type, "SortItems", &o2::UICustomList::SortItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Function<bool(UIWidget*, UIWidget*)>&>(funcInfo, "sortFunc");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, int>(&type, "GetItemsCount", &o2::UICustomList::GetItemsCount, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, UIWidget*>(&type, "SelectItem", &o2::UICustomList::SelectItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, int>(&type, "SelectItemAt", &o2::UICustomList::SelectItemAt, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Vector<int>&>(&type, "SetSelectedItems", &o2::UICustomList::SetSelectedItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<int>&>(funcInfo, "items");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "ClearSelection", &o2::UICustomList::ClearSelection, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, Vector<int>>(&type, "GetSelectedItems", &o2::UICustomList::GetSelectedItems, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, int>(&type, "GetSelectedItemPos", &o2::UICustomList::GetSelectedItemPos, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*>(&type, "GetSelectedItem", &o2::UICustomList::GetSelectedItem, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, bool>(&type, "SetMultiselectionAvailable", &o2::UICustomList::SetMultiselectionAvailable, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "available");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, bool>(&type, "IsMultiselectionAvailable", &o2::UICustomList::IsMultiselectionAvailable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, Sprite*>(&type, "GetSelectionDrawable", &o2::UICustomList::GetSelectionDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, Sprite*>(&type, "GetHoverDrawable", &o2::UICustomList::GetHoverDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Layout&>(&type, "SetSelectionDrawableLayout", &o2::UICustomList::SetSelectionDrawableLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, Layout>(&type, "GetSelectionDrawableLayout", &o2::UICustomList::GetSelectionDrawableLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Layout&>(&type, "SetHoverDrawableLayout", &o2::UICustomList::SetHoverDrawableLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, Layout>(&type, "GetHoverDrawableLayout", &o2::UICustomList::GetHoverDrawableLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, bool>(&type, "IsScrollable", &o2::UICustomList::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, float>(&type, "UpdateControls", &o2::UICustomList::UpdateControls, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, bool>(&type, "UpdateLayout", &o2::UICustomList::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UICustomList::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UICustomList::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UICustomList::OnCursorMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UICustomList::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UICustomList::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UICustomList::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, float>(&type, "OnScrolled", &o2::UICustomList::OnScrolled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, UIWidget*, const Vec2F&, int*>(&type, "GetItemUnderPoint", &o2::UICustomList::GetItemUnderPoint, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	TypeInitializer::RegFuncParam<int*>(funcInfo, "idxPtr");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const DataNode&>(&type, "OnDeserialized", &o2::UICustomList::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "UpdateTransparency", &o2::UICustomList::UpdateTransparency, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, const Vec2F&>(&type, "UpdateHover", &o2::UICustomList::UpdateHover, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, Sprite*>(&type, "GetSelectionSprite", &o2::UICustomList::GetSelectionSprite, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "OnSelectionChanged", &o2::UICustomList::OnSelectionChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "OnVisibleChanged", &o2::UICustomList::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void>(&type, "InitializeProperties", &o2::UICustomList::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIDropDown::InitializeType(o2::UIDropDown* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "textItem", (size_t)(char*)(&sample->textItem) - (size_t)(char*)sample, sample->textItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedText", (size_t)(char*)(&sample->onSelectedText) - (size_t)(char*)sample, sample->onSelectedText, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, int, const WString&>(&type, "AddItem", &o2::UIDropDown::AddItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, int, const WString&, int>(&type, "AddItem", &o2::UIDropDown::AddItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, void, const Vector<WString>&>(&type, "AddItems", &o2::UIDropDown::AddItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<WString>&>(funcInfo, "data");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, void, const WString&>(&type, "RemoveItem", &o2::UIDropDown::RemoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, int, const WString&>(&type, "FindItem", &o2::UIDropDown::FindItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, WString, int>(&type, "GetItemText", &o2::UIDropDown::GetItemText, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, Vector<WString>>(&type, "GetAllItemsText", &o2::UIDropDown::GetAllItemsText, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, WString>(&type, "GetSelectedItemText", &o2::UIDropDown::GetSelectedItemText, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, void, const WString&>(&type, "SelectItemText", &o2::UIDropDown::SelectItemText, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, void>(&type, "OnSelectionChanged", &o2::UIDropDown::OnSelectionChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIDropDown, void>(&type, "InitializeProperties", &o2::UIDropDown::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIEditBox::InitializeType(o2::UIEditBox* sample)
{
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)sample, sample->text, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "caret", (size_t)(char*)(&sample->caret) - (size_t)(char*)sample, sample->caret, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "selectionBegin", (size_t)(char*)(&sample->selectionBegin) - (size_t)(char*)sample, sample->selectionBegin, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "selectionEnd", (size_t)(char*)(&sample->selectionEnd) - (size_t)(char*)sample, sample->selectionEnd, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChanged", (size_t)(char*)(&sample->onChanged) - (size_t)(char*)sample, sample->onChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChangeCompleted", (size_t)(char*)(&sample->onChangeCompleted) - (size_t)(char*)sample, sample->onChangeCompleted, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mLastText", (size_t)(char*)(&sample->mLastText) - (size_t)(char*)sample, sample->mLastText, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mText", (size_t)(char*)(&sample->mText) - (size_t)(char*)sample, sample->mText, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAvailableSymbols", (size_t)(char*)(&sample->mAvailableSymbols) - (size_t)(char*)sample, sample->mAvailableSymbols, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTextDrawable", (size_t)(char*)(&sample->mTextDrawable) - (size_t)(char*)sample, sample->mTextDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionMesh", (size_t)(char*)(&sample->mSelectionMesh) - (size_t)(char*)sample, sample->mSelectionMesh, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCaretDrawable", (size_t)(char*)(&sample->mCaretDrawable) - (size_t)(char*)sample, sample->mCaretDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCaretBlinkDelay", (size_t)(char*)(&sample->mCaretBlinkDelay) - (size_t)(char*)sample, sample->mCaretBlinkDelay, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCaretBlinkTime", (size_t)(char*)(&sample->mCaretBlinkTime) - (size_t)(char*)sample, sample->mCaretBlinkTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionBegin", (size_t)(char*)(&sample->mSelectionBegin) - (size_t)(char*)sample, sample->mSelectionBegin, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionEnd", (size_t)(char*)(&sample->mSelectionEnd) - (size_t)(char*)sample, sample->mSelectionEnd, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionColor", (size_t)(char*)(&sample->mSelectionColor) - (size_t)(char*)sample, sample->mSelectionColor, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectingByWords", (size_t)(char*)(&sample->mSelectingByWords) - (size_t)(char*)sample, sample->mSelectingByWords, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelWordBegin", (size_t)(char*)(&sample->mSelWordBegin) - (size_t)(char*)sample, sample->mSelWordBegin, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelWordEnd", (size_t)(char*)(&sample->mSelWordEnd) - (size_t)(char*)sample, sample->mSelWordEnd, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMultiLine", (size_t)(char*)(&sample->mMultiLine) - (size_t)(char*)sample, sample->mMultiLine, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWordWrap", (size_t)(char*)(&sample->mWordWrap) - (size_t)(char*)sample, sample->mWordWrap, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxLineChars", (size_t)(char*)(&sample->mMaxLineChars) - (size_t)(char*)sample, sample->mMaxLineChars, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxLinesCount", (size_t)(char*)(&sample->mMaxLinesCount) - (size_t)(char*)sample, sample->mMaxLinesCount, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDrawDepth", (size_t)(char*)(&sample->mDrawDepth) - (size_t)(char*)sample, sample->mDrawDepth, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mJustSelected", (size_t)(char*)(&sample->mJustSelected) - (size_t)(char*)sample, sample->mJustSelected, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastClickTime", (size_t)(char*)(&sample->mLastClickTime) - (size_t)(char*)sample, sample->mLastClickTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastCursorPos", (size_t)(char*)(&sample->mLastCursorPos) - (size_t)(char*)sample, sample->mLastCursorPos, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "Draw", &o2::UIEditBox::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "Update", &o2::UIEditBox::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const WString&>(&type, "SetText", &o2::UIEditBox::SetText, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, WString>(&type, "GetText", &o2::UIEditBox::GetText, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int>(&type, "SetCaretPosition", &o2::UIEditBox::SetCaretPosition, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "caretPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int>(&type, "GetCaretPosition", &o2::UIEditBox::GetCaretPosition, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int, int>(&type, "Select", &o2::UIEditBox::Select, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "begin");
	TypeInitializer::RegFuncParam<int>(funcInfo, "end");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int>(&type, "SetSelectionBegin", &o2::UIEditBox::SetSelectionBegin, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int>(&type, "GetSelectionBegin", &o2::UIEditBox::GetSelectionBegin, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int>(&type, "SetSelectionEnd", &o2::UIEditBox::SetSelectionEnd, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int>(&type, "GetSelectionEnd", &o2::UIEditBox::GetSelectionEnd, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "Deselect", &o2::UIEditBox::Deselect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "SelectAll", &o2::UIEditBox::SelectAll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, Text*>(&type, "GetTextDrawable", &o2::UIEditBox::GetTextDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, Sprite*>(&type, "GetCaretDrawable", &o2::UIEditBox::GetCaretDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Color4&>(&type, "SetSelectionColor", &o2::UIEditBox::SetSelectionColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, Color4>(&type, "GetSelectionColor", &o2::UIEditBox::GetSelectionColor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "SetFilterInteger", &o2::UIEditBox::SetFilterInteger, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "SetFilterFloat", &o2::UIEditBox::SetFilterFloat, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "SetFilterNames", &o2::UIEditBox::SetFilterNames, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const WString&>(&type, "SetAvailableSymbols", &o2::UIEditBox::SetAvailableSymbols, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "availableSymbols");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, WString>(&type, "GetAvailableSymbols", &o2::UIEditBox::GetAvailableSymbols, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int, int>(&type, "SetMaxSizes", &o2::UIEditBox::SetMaxSizes, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "maxLineCharactersCount");
	TypeInitializer::RegFuncParam<int>(funcInfo, "maxLinesCount");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int>(&type, "SetMaxLineCharactersCount", &o2::UIEditBox::SetMaxLineCharactersCount, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int>(&type, "GetMaxLineCharactersCount", &o2::UIEditBox::GetMaxLineCharactersCount, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int>(&type, "SetMaxLinesCount", &o2::UIEditBox::SetMaxLinesCount, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int>(&type, "GetMaxLinesCount", &o2::UIEditBox::GetMaxLinesCount, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, bool>(&type, "SetMultiLine", &o2::UIEditBox::SetMultiLine, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "multiline");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, bool>(&type, "IsMultiLine", &o2::UIEditBox::IsMultiLine, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, bool>(&type, "SetWordWrap", &o2::UIEditBox::SetWordWrap, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "wordWrap");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, bool>(&type, "IsWordWrap", &o2::UIEditBox::IsWordWrap, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "SetCaretBlinkingDelay", &o2::UIEditBox::SetCaretBlinkingDelay, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "delay");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, float>(&type, "GetCaretBlinkingDelay", &o2::UIEditBox::GetCaretBlinkingDelay, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, bool>(&type, "IsScrollable", &o2::UIEditBox::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, bool>(&type, "IsSelectable", &o2::UIEditBox::IsSelectable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "UpdateControls", &o2::UIEditBox::UpdateControls, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "OnVisibleChanged", &o2::UIEditBox::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "OnSelected", &o2::UIEditBox::OnSelected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "OnDeselected", &o2::UIEditBox::OnDeselected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIEditBox::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIEditBox::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIEditBox::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIEditBox::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIEditBox::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIEditBox::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorRightMousePressed", &o2::UIEditBox::OnCursorRightMousePressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorRightMouseStayDown", &o2::UIEditBox::OnCursorRightMouseStayDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &o2::UIEditBox::OnCursorRightMouseReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "OnScrolled", &o2::UIEditBox::OnScrolled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Key&>(&type, "OnKeyPressed", &o2::UIEditBox::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Key&>(&type, "OnKeyReleased", &o2::UIEditBox::OnKeyReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Key&>(&type, "OnKeyStayDown", &o2::UIEditBox::OnKeyStayDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, WString, const WString&>(&type, "GetFilteredText", &o2::UIEditBox::GetFilteredText, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "UpdateScrollParams", &o2::UIEditBox::UpdateScrollParams, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, bool>(&type, "UpdateLayout", &o2::UIEditBox::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "UpdateTransparency", &o2::UIEditBox::UpdateTransparency, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "CheckCharactersAndLinesBounds", &o2::UIEditBox::CheckCharactersAndLinesBounds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "UpdateSelectionAndCaret", &o2::UIEditBox::UpdateSelectionAndCaret, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, Vec2F, int>(&type, "GetTextCaretPosition", &o2::UIEditBox::GetTextCaretPosition, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, int, const Vec2F&>(&type, "GetTextCaretPosition", &o2::UIEditBox::GetTextCaretPosition, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "UpdateCaretBlinking", &o2::UIEditBox::UpdateCaretBlinking, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const RectF&>(&type, "AddSelectionRect", &o2::UIEditBox::AddSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "CheckScrollingToCaret", &o2::UIEditBox::CheckScrollingToCaret, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, bool, bool>(&type, "JumpSelection", &o2::UIEditBox::JumpSelection, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forward");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "selecting");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, KeyboardKey>(&type, "CheckCharacterTyping", &o2::UIEditBox::CheckCharacterTyping, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<KeyboardKey>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, KeyboardKey>(&type, "CheckCharactersErasing", &o2::UIEditBox::CheckCharactersErasing, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<KeyboardKey>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, KeyboardKey>(&type, "CheckCaretMoving", &o2::UIEditBox::CheckCaretMoving, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<KeyboardKey>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, KeyboardKey>(&type, "CheckClipboard", &o2::UIEditBox::CheckClipboard, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<KeyboardKey>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, int, bool>(&type, "MoveCaret", &o2::UIEditBox::MoveCaret, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<int>(funcInfo, "newPosition");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "selecting");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "InitializeProperties", &o2::UIEditBox::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIGridLayout::InitializeType(o2::UIGridLayout* sample)
{
	TypeInitializer::RegField(&type, "baseCorner", (size_t)(char*)(&sample->baseCorner) - (size_t)(char*)sample, sample->baseCorner, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "cellSize", (size_t)(char*)(&sample->cellSize) - (size_t)(char*)sample, sample->cellSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "arrangeAxisMaxCells", (size_t)(char*)(&sample->arrangeAxisMaxCells) - (size_t)(char*)sample, sample->arrangeAxisMaxCells, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "arrangeAxis", (size_t)(char*)(&sample->arrangeAxis) - (size_t)(char*)sample, sample->arrangeAxis, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "spacing", (size_t)(char*)(&sample->spacing) - (size_t)(char*)sample, sample->spacing, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "border", (size_t)(char*)(&sample->border) - (size_t)(char*)sample, sample->border, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderLeft", (size_t)(char*)(&sample->borderLeft) - (size_t)(char*)sample, sample->borderLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderRight", (size_t)(char*)(&sample->borderRight) - (size_t)(char*)sample, sample->borderRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderTop", (size_t)(char*)(&sample->borderTop) - (size_t)(char*)sample, sample->borderTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderBottom", (size_t)(char*)(&sample->borderBottom) - (size_t)(char*)sample, sample->borderBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fitByChildren", (size_t)(char*)(&sample->fitByChildren) - (size_t)(char*)sample, sample->fitByChildren, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mBaseCorner", (size_t)(char*)(&sample->mBaseCorner) - (size_t)(char*)sample, sample->mBaseCorner, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSpacing", (size_t)(char*)(&sample->mSpacing) - (size_t)(char*)sample, sample->mSpacing, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCellSize", (size_t)(char*)(&sample->mCellSize) - (size_t)(char*)sample, sample->mCellSize, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mArrangeAxis", (size_t)(char*)(&sample->mArrangeAxis) - (size_t)(char*)sample, sample->mArrangeAxis, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mArrangeAxisMaxCells", (size_t)(char*)(&sample->mArrangeAxisMaxCells) - (size_t)(char*)sample, sample->mArrangeAxisMaxCells, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)sample, sample->mBorder, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFitByChildren", (size_t)(char*)(&sample->mFitByChildren) - (size_t)(char*)sample, sample->mFitByChildren, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, BaseCorner>(&type, "SetBaseCorner", &o2::UIGridLayout::SetBaseCorner, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<BaseCorner>(funcInfo, "baseCorner");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, BaseCorner>(&type, "GetBaseCorner", &o2::UIGridLayout::GetBaseCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, float>(&type, "SetSpacing", &o2::UIGridLayout::SetSpacing, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "spacing");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, float>(&type, "GetSpacing", &o2::UIGridLayout::GetSpacing, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, const RectF&>(&type, "SetBorder", &o2::UIGridLayout::SetBorder, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, RectF>(&type, "GetBorder", &o2::UIGridLayout::GetBorder, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, float>(&type, "SetBorderLeft", &o2::UIGridLayout::SetBorderLeft, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, float>(&type, "GetBorderLeft", &o2::UIGridLayout::GetBorderLeft, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, float>(&type, "SetBorderRight", &o2::UIGridLayout::SetBorderRight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, float>(&type, "GetBorderRight", &o2::UIGridLayout::GetBorderRight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, float>(&type, "SetBorderTop", &o2::UIGridLayout::SetBorderTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, float>(&type, "GetBorderTop", &o2::UIGridLayout::GetBorderTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, float>(&type, "SetBorderBottom", &o2::UIGridLayout::SetBorderBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, float>(&type, "GetBorderBottom", &o2::UIGridLayout::GetBorderBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, const Vec2F&>(&type, "SetCellSize", &o2::UIGridLayout::SetCellSize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, Vec2F>(&type, "GetCellSize", &o2::UIGridLayout::GetCellSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, TwoDirection>(&type, "SetArrangeAxis", &o2::UIGridLayout::SetArrangeAxis, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<TwoDirection>(funcInfo, "type");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, TwoDirection>(&type, "GetArrangeAxis", &o2::UIGridLayout::GetArrangeAxis, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, int>(&type, "SetArrangeAxisMaxCells", &o2::UIGridLayout::SetArrangeAxisMaxCells, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, int>(&type, "GetArrangeAxisMaxCells", &o2::UIGridLayout::GetArrangeAxisMaxCells, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, bool>(&type, "SetFitByChildren", &o2::UIGridLayout::SetFitByChildren, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "fit");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, bool>(&type, "IsFittingByChildren", &o2::UIGridLayout::IsFittingByChildren, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, bool>(&type, "UpdateLayout", &o2::UIGridLayout::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, UIWidget*>(&type, "OnChildAdded", &o2::UIGridLayout::OnChildAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, UIWidget*>(&type, "OnChildRemoved", &o2::UIGridLayout::OnChildRemoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "RearrangeChilds", &o2::UIGridLayout::RearrangeChilds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromLeftTop", &o2::UIGridLayout::ArrangeFromLeftTop, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromTop", &o2::UIGridLayout::ArrangeFromTop, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromRightTop", &o2::UIGridLayout::ArrangeFromRightTop, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromLeft", &o2::UIGridLayout::ArrangeFromLeft, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromCenter", &o2::UIGridLayout::ArrangeFromCenter, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromRight", &o2::UIGridLayout::ArrangeFromRight, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromLeftBottom", &o2::UIGridLayout::ArrangeFromLeftBottom, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromBottom", &o2::UIGridLayout::ArrangeFromBottom, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ArrangeFromRightBottom", &o2::UIGridLayout::ArrangeFromRightBottom, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "ExpandSizeByChilds", &o2::UIGridLayout::ExpandSizeByChilds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void>(&type, "InitializeProperties", &o2::UIGridLayout::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIHorizontalLayout::InitializeType(o2::UIHorizontalLayout* sample)
{
	TypeInitializer::RegField(&type, "baseCorner", (size_t)(char*)(&sample->baseCorner) - (size_t)(char*)sample, sample->baseCorner, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "spacing", (size_t)(char*)(&sample->spacing) - (size_t)(char*)sample, sample->spacing, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "border", (size_t)(char*)(&sample->border) - (size_t)(char*)sample, sample->border, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderLeft", (size_t)(char*)(&sample->borderLeft) - (size_t)(char*)sample, sample->borderLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderRight", (size_t)(char*)(&sample->borderRight) - (size_t)(char*)sample, sample->borderRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderTop", (size_t)(char*)(&sample->borderTop) - (size_t)(char*)sample, sample->borderTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderBottom", (size_t)(char*)(&sample->borderBottom) - (size_t)(char*)sample, sample->borderBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "expandWidth", (size_t)(char*)(&sample->expandWidth) - (size_t)(char*)sample, sample->expandWidth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "expandHeight", (size_t)(char*)(&sample->expandHeight) - (size_t)(char*)sample, sample->expandHeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fitByChildren", (size_t)(char*)(&sample->fitByChildren) - (size_t)(char*)sample, sample->fitByChildren, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mBaseCorner", (size_t)(char*)(&sample->mBaseCorner) - (size_t)(char*)sample, sample->mBaseCorner, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSpacing", (size_t)(char*)(&sample->mSpacing) - (size_t)(char*)sample, sample->mSpacing, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)sample, sample->mBorder, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandWidth", (size_t)(char*)(&sample->mExpandWidth) - (size_t)(char*)sample, sample->mExpandWidth, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandHeight", (size_t)(char*)(&sample->mExpandHeight) - (size_t)(char*)sample, sample->mExpandHeight, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFitByChildren", (size_t)(char*)(&sample->mFitByChildren) - (size_t)(char*)sample, sample->mFitByChildren, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, BaseCorner>(&type, "SetBaseCorner", &o2::UIHorizontalLayout::SetBaseCorner, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<BaseCorner>(funcInfo, "baseCorner");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, BaseCorner>(&type, "GetBaseCorner", &o2::UIHorizontalLayout::GetBaseCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, float>(&type, "SetSpacing", &o2::UIHorizontalLayout::SetSpacing, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "spacing");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, float>(&type, "GetSpacing", &o2::UIHorizontalLayout::GetSpacing, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, const RectF&>(&type, "SetBorder", &o2::UIHorizontalLayout::SetBorder, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, RectF>(&type, "GetBorder", &o2::UIHorizontalLayout::GetBorder, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, float>(&type, "SetBorderLeft", &o2::UIHorizontalLayout::SetBorderLeft, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, float>(&type, "GetBorderLeft", &o2::UIHorizontalLayout::GetBorderLeft, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, float>(&type, "SetBorderRight", &o2::UIHorizontalLayout::SetBorderRight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, float>(&type, "GetBorderRight", &o2::UIHorizontalLayout::GetBorderRight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, float>(&type, "SetBorderTop", &o2::UIHorizontalLayout::SetBorderTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, float>(&type, "GetBorderTop", &o2::UIHorizontalLayout::GetBorderTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, float>(&type, "SetBorderBottom", &o2::UIHorizontalLayout::SetBorderBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, float>(&type, "GetBorderBottom", &o2::UIHorizontalLayout::GetBorderBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, bool>(&type, "SetWidthExpand", &o2::UIHorizontalLayout::SetWidthExpand, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expand");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, bool>(&type, "IsWidthExpand", &o2::UIHorizontalLayout::IsWidthExpand, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, bool>(&type, "SetHeightExpand", &o2::UIHorizontalLayout::SetHeightExpand, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expand");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, bool>(&type, "IsHeightExpand", &o2::UIHorizontalLayout::IsHeightExpand, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, bool>(&type, "SetFitByChildren", &o2::UIHorizontalLayout::SetFitByChildren, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "fit");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, bool>(&type, "IsFittingByChildren", &o2::UIHorizontalLayout::IsFittingByChildren, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, bool>(&type, "UpdateLayout", &o2::UIHorizontalLayout::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, UIWidget*>(&type, "OnChildAdded", &o2::UIHorizontalLayout::OnChildAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, UIWidget*>(&type, "OnChildRemoved", &o2::UIHorizontalLayout::OnChildRemoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "RearrangeChilds", &o2::UIHorizontalLayout::RearrangeChilds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "ArrangeFromLeftToRight", &o2::UIHorizontalLayout::ArrangeFromLeftToRight, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "ArrangeFromRightToLeft", &o2::UIHorizontalLayout::ArrangeFromRightToLeft, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "ArrangeFromCenter", &o2::UIHorizontalLayout::ArrangeFromCenter, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "ExpandSizeByChilds", &o2::UIHorizontalLayout::ExpandSizeByChilds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, Vector<float>>(&type, "CalculateExpandedWidths", &o2::UIHorizontalLayout::CalculateExpandedWidths, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, UIWidget*, float>(&type, "AlignWidgetByHeight", &o2::UIHorizontalLayout::AlignWidgetByHeight, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	TypeInitializer::RegFuncParam<float>(funcInfo, "widthAnchor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "UpdateLayoutParametres", &o2::UIHorizontalLayout::UpdateLayoutParametres, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void>(&type, "InitializeProperties", &o2::UIHorizontalLayout::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIHorizontalProgress::InitializeType(o2::UIHorizontalProgress* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)sample, sample->minValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)sample, sample->maxValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)sample, sample->scrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)sample, sample->onChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)sample, sample->mSmoothValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)sample, sample->mMinValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)sample, sample->mMaxValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)sample, sample->mScrollSense, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOrientation", (size_t)(char*)(&sample->mOrientation) - (size_t)(char*)sample, sample->mOrientation, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBarLayer", (size_t)(char*)(&sample->mBarLayer) - (size_t)(char*)sample, sample->mBarLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)sample, sample->mBackLayer, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "Update", &o2::UIHorizontalProgress::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "SetValue", &o2::UIHorizontalProgress::SetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "SetValueForcible", &o2::UIHorizontalProgress::SetValueForcible, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, float>(&type, "GetValue", &o2::UIHorizontalProgress::GetValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "SetMinValue", &o2::UIHorizontalProgress::SetMinValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, float>(&type, "GetMinValue", &o2::UIHorizontalProgress::GetMinValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "SetMaxValue", &o2::UIHorizontalProgress::SetMaxValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, float>(&type, "GetMaxValue", &o2::UIHorizontalProgress::GetMaxValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float, float>(&type, "SetValueRange", &o2::UIHorizontalProgress::SetValueRange, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "SetScrollSense", &o2::UIHorizontalProgress::SetScrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, float>(&type, "GetScrollSense", &o2::UIHorizontalProgress::GetScrollSense, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, Orientation>(&type, "SetOrientation", &o2::UIHorizontalProgress::SetOrientation, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Orientation>(funcInfo, "orientation");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, Orientation>(&type, "GetOrientation", &o2::UIHorizontalProgress::GetOrientation, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIHorizontalProgress::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, bool>(&type, "IsScrollable", &o2::UIHorizontalProgress::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, bool>(&type, "UpdateLayout", &o2::UIHorizontalProgress::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void>(&type, "UpdateProgressLayersLayouts", &o2::UIHorizontalProgress::UpdateProgressLayersLayouts, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIHorizontalProgress::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "GetValueFromCursor", &o2::UIHorizontalProgress::GetValueFromCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIHorizontalProgress::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIHorizontalProgress::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIHorizontalProgress::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIHorizontalProgress::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIHorizontalProgress::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIHorizontalProgress::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, float>(&type, "OnScrolled", &o2::UIHorizontalProgress::OnScrolled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, const DataNode&>(&type, "OnDeserialized", &o2::UIHorizontalProgress::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void>(&type, "OnVisibleChanged", &o2::UIHorizontalProgress::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void>(&type, "InitializeProperties", &o2::UIHorizontalProgress::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIHorizontalScrollBar::InitializeType(o2::UIHorizontalScrollBar* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)sample, sample->minValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)sample, sample->maxValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)sample, sample->scrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSize", (size_t)(char*)(&sample->scrollSize) - (size_t)(char*)sample, sample->scrollSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)sample, sample->onChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSmoothChange", (size_t)(char*)(&sample->onSmoothChange) - (size_t)(char*)sample, sample->onSmoothChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)sample, sample->mSmoothValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)sample, sample->mMinValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)sample, sample->mMaxValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)sample, sample->mScrollSense, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollHandleSize", (size_t)(char*)(&sample->mScrollHandleSize) - (size_t)(char*)sample, sample->mScrollHandleSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollhandleMinPxSize", (size_t)(char*)(&sample->mScrollhandleMinPxSize) - (size_t)(char*)sample, sample->mScrollhandleMinPxSize, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressHandleOffset", (size_t)(char*)(&sample->mPressHandleOffset) - (size_t)(char*)sample, sample->mPressHandleOffset, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandlePressed", (size_t)(char*)(&sample->mHandlePressed) - (size_t)(char*)sample, sample->mHandlePressed, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandleLayer", (size_t)(char*)(&sample->mHandleLayer) - (size_t)(char*)sample, sample->mHandleLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)sample, sample->mBackLayer, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "Update", &o2::UIHorizontalScrollBar::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetValue", &o2::UIHorizontalScrollBar::SetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetValueForcible", &o2::UIHorizontalScrollBar::SetValueForcible, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetValue", &o2::UIHorizontalScrollBar::GetValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetSmoothValue", &o2::UIHorizontalScrollBar::GetSmoothValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetMinValue", &o2::UIHorizontalScrollBar::SetMinValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetMinValue", &o2::UIHorizontalScrollBar::GetMinValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetMaxValue", &o2::UIHorizontalScrollBar::SetMaxValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetMaxValue", &o2::UIHorizontalScrollBar::GetMaxValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float, float>(&type, "SetValueRange", &o2::UIHorizontalScrollBar::SetValueRange, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetScrollSense", &o2::UIHorizontalScrollBar::SetScrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetScrollSense", &o2::UIHorizontalScrollBar::GetScrollSense, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetScrollHandleSize", &o2::UIHorizontalScrollBar::SetScrollHandleSize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float>(&type, "GetScrollhandleSize", &o2::UIHorizontalScrollBar::GetScrollhandleSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "SetMinimalScrollhandleSize", &o2::UIHorizontalScrollBar::SetMinimalScrollhandleSize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "pixelSize");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIHorizontalScrollBar::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, bool>(&type, "IsScrollable", &o2::UIHorizontalScrollBar::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, bool>(&type, "UpdateLayout", &o2::UIHorizontalScrollBar::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void>(&type, "UpdateProgressLayersLayouts", &o2::UIHorizontalScrollBar::UpdateProgressLayersLayouts, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void>(&type, "OnLayoutUpdated", &o2::UIHorizontalScrollBar::OnLayoutUpdated, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIHorizontalScrollBar::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, float, const Input::Cursor&>(&type, "GetValueFromCursor", &o2::UIHorizontalScrollBar::GetValueFromCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIHorizontalScrollBar::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIHorizontalScrollBar::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIHorizontalScrollBar::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIHorizontalScrollBar::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIHorizontalScrollBar::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIHorizontalScrollBar::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, float>(&type, "OnScrolled", &o2::UIHorizontalScrollBar::OnScrolled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, const DataNode&>(&type, "OnDeserialized", &o2::UIHorizontalScrollBar::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void>(&type, "OnVisibleChanged", &o2::UIHorizontalScrollBar::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void>(&type, "InitializeProperties", &o2::UIHorizontalScrollBar::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UILabel::InitializeType(o2::UILabel* sample)
{
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)sample, sample->font, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)sample, sample->text, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "verAlign", (size_t)(char*)(&sample->verAlign) - (size_t)(char*)sample, sample->verAlign, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "horAlign", (size_t)(char*)(&sample->horAlign) - (size_t)(char*)sample, sample->horAlign, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "horOverflow", (size_t)(char*)(&sample->horOverflow) - (size_t)(char*)sample, sample->horOverflow, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "verOverflow", (size_t)(char*)(&sample->verOverflow) - (size_t)(char*)sample, sample->verOverflow, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "expandBorder", (size_t)(char*)(&sample->expandBorder) - (size_t)(char*)sample, sample->expandBorder, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "symbolsDistanceCoef", (size_t)(char*)(&sample->symbolsDistanceCoef) - (size_t)(char*)sample, sample->symbolsDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "linesDistanceCoef", (size_t)(char*)(&sample->linesDistanceCoef) - (size_t)(char*)sample, sample->linesDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTextLayer", (size_t)(char*)(&sample->mTextLayer) - (size_t)(char*)sample, sample->mTextLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHorOverflow", (size_t)(char*)(&sample->mHorOverflow) - (size_t)(char*)sample, sample->mHorOverflow, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVerOverflow", (size_t)(char*)(&sample->mVerOverflow) - (size_t)(char*)sample, sample->mVerOverflow, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandBorder", (size_t)(char*)(&sample->mExpandBorder) - (size_t)(char*)sample, sample->mExpandBorder, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UILabel, void>(&type, "Draw", &o2::UILabel::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, FontRef>(&type, "SetFont", &o2::UILabel::SetFont, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<FontRef>(funcInfo, "font");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, FontRef>(&type, "GetFont", &o2::UILabel::GetFont, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, const WString&>(&type, "SetText", &o2::UILabel::SetText, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, WString>(&type, "GetText", &o2::UILabel::GetText, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, HorAlign>(&type, "SetHorAlign", &o2::UILabel::SetHorAlign, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<HorAlign>(funcInfo, "align");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, HorAlign>(&type, "GetHorAlign", &o2::UILabel::GetHorAlign, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, VerAlign>(&type, "SetVerAlign", &o2::UILabel::SetVerAlign, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VerAlign>(funcInfo, "align");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, VerAlign>(&type, "GetVerAlign", &o2::UILabel::GetVerAlign, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, HorOverflow>(&type, "SetHorOverflow", &o2::UILabel::SetHorOverflow, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<HorOverflow>(funcInfo, "overflow");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, HorOverflow>(&type, "GetHorOverflow", &o2::UILabel::GetHorOverflow, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, VerOverflow>(&type, "SetVerOverflow", &o2::UILabel::SetVerOverflow, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VerOverflow>(funcInfo, "overflow");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, VerOverflow>(&type, "GetVerOverflow", &o2::UILabel::GetVerOverflow, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, float>(&type, "SetSymbolsDistanceCoef", &o2::UILabel::SetSymbolsDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, float>(&type, "GetSymbolsDistanceCoef", &o2::UILabel::GetSymbolsDistanceCoef, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, float>(&type, "SetLinesDistanceCoef", &o2::UILabel::SetLinesDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, float>(&type, "GetLinesDistanceCoef", &o2::UILabel::GetLinesDistanceCoef, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, const Vec2F&>(&type, "SetExpandBorder", &o2::UILabel::SetExpandBorder, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, Vec2F>(&type, "GetExpandBorder", &o2::UILabel::GetExpandBorder, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, bool>(&type, "UpdateLayout", &o2::UILabel::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UILabel::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void>(&type, "InitializeProperties", &o2::UILabel::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIList::InitializeType(o2::UIList* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "values", (size_t)(char*)(&sample->values) - (size_t)(char*)sample, sample->values, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "textItem", (size_t)(char*)(&sample->textItem) - (size_t)(char*)sample, sample->textItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedText", (size_t)(char*)(&sample->onSelectedText) - (size_t)(char*)sample, sample->onSelectedText, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIList, int, const WString&>(&type, "AddItem", &o2::UIList::AddItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, int, const WString&, int>(&type, "AddItem", &o2::UIList::AddItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void, const Vector<WString>&>(&type, "AddItems", &o2::UIList::AddItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<WString>&>(funcInfo, "data");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void, const WString&>(&type, "RemoveItem", &o2::UIList::RemoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, int, const WString&>(&type, "FindItem", &o2::UIList::FindItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, WString, int>(&type, "GetItemText", &o2::UIList::GetItemText, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, Vector<WString>>(&type, "GetAllItemsText", &o2::UIList::GetAllItemsText, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIList, WString>(&type, "GetSelectedItemText", &o2::UIList::GetSelectedItemText, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void, const WString&>(&type, "SelectItemText", &o2::UIList::SelectItemText, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void, const Vector<WString>&>(&type, "SetSelectedItems", &o2::UIList::SetSelectedItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<WString>&>(funcInfo, "items");
	funcInfo = TypeInitializer::RegFunction<o2::UIList, Vector<WString>>(&type, "GetSelectedItemsText", &o2::UIList::GetSelectedItemsText, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void>(&type, "OnSelectionChanged", &o2::UIList::OnSelectionChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIList, void>(&type, "initializeProperties", &o2::UIList::initializeProperties, o2::ProtectSection::Protected);
}

void o2::UILongList::InitializeType(o2::UILongList* sample)
{
	TypeInitializer::RegField(&type, "selectedItemPos", (size_t)(char*)(&sample->selectedItemPos) - (size_t)(char*)sample, sample->selectedItemPos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelected", (size_t)(char*)(&sample->onSelected) - (size_t)(char*)sample, sample->onSelected, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "getItemsCountFunc", (size_t)(char*)(&sample->getItemsCountFunc) - (size_t)(char*)sample, sample->getItemsCountFunc, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "getItemsRangeFunc", (size_t)(char*)(&sample->getItemsRangeFunc) - (size_t)(char*)sample, sample->getItemsRangeFunc, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "setupItemFunc", (size_t)(char*)(&sample->setupItemFunc) - (size_t)(char*)sample, sample->setupItemFunc, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)sample, sample->mItemSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)sample, sample->mSelectionDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverDrawable", (size_t)(char*)(&sample->mHoverDrawable) - (size_t)(char*)sample, sample->mHoverDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)sample, sample->mSelectionLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverLayout", (size_t)(char*)(&sample->mHoverLayout) - (size_t)(char*)sample, sample->mHoverLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMinVisibleItemIdx", (size_t)(char*)(&sample->mMinVisibleItemIdx) - (size_t)(char*)sample, sample->mMinVisibleItemIdx, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMaxVisibleItemIdx", (size_t)(char*)(&sample->mMaxVisibleItemIdx) - (size_t)(char*)sample, sample->mMaxVisibleItemIdx, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedItem", (size_t)(char*)(&sample->mSelectedItem) - (size_t)(char*)sample, sample->mSelectedItem, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentSelectionRect", (size_t)(char*)(&sample->mCurrentSelectionRect) - (size_t)(char*)sample, sample->mCurrentSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetSelectionRect", (size_t)(char*)(&sample->mTargetSelectionRect) - (size_t)(char*)sample, sample->mTargetSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentHoverRect", (size_t)(char*)(&sample->mCurrentHoverRect) - (size_t)(char*)sample, sample->mCurrentHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetHoverRect", (size_t)(char*)(&sample->mTargetHoverRect) - (size_t)(char*)sample, sample->mTargetHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastHoverCheckCursor", (size_t)(char*)(&sample->mLastHoverCheckCursor) - (size_t)(char*)sample, sample->mLastHoverCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)sample, sample->mLastSelectCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mItemsPool", (size_t)(char*)(&sample->mItemsPool) - (size_t)(char*)sample, sample->mItemsPool, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDrawDepth", (size_t)(char*)(&sample->mDrawDepth) - (size_t)(char*)sample, sample->mDrawDepth, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, float>(&type, "Update", &o2::UILongList::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "Draw", &o2::UILongList::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, UIWidget*>(&type, "SetItemSample", &o2::UILongList::SetItemSample, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "sample");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, UIWidget*>(&type, "GetItemSample", &o2::UILongList::GetItemSample, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, int>(&type, "SelectItemAt", &o2::UILongList::SelectItemAt, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, int>(&type, "GetSelectedItemPosition", &o2::UILongList::GetSelectedItemPosition, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, Sprite*>(&type, "GetSelectionDrawable", &o2::UILongList::GetSelectionDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, Sprite*>(&type, "GetHoverDrawable", &o2::UILongList::GetHoverDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Layout&>(&type, "SetSelectionDrawableLayout", &o2::UILongList::SetSelectionDrawableLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, Layout>(&type, "GetSelectionDrawableLayout", &o2::UILongList::GetSelectionDrawableLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Layout&>(&type, "SetHoverDrawableLayout", &o2::UILongList::SetHoverDrawableLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, Layout>(&type, "GetHoverDrawableLayout", &o2::UILongList::GetHoverDrawableLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, bool>(&type, "IsScrollable", &o2::UILongList::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, bool>(&type, "OnItemsUpdated", &o2::UILongList::OnItemsUpdated, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "itemsRearranged");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "CalculateScrollArea", &o2::UILongList::CalculateScrollArea, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, float>(&type, "UpdateControls", &o2::UILongList::UpdateControls, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, bool>(&type, "UpdateLayout", &o2::UILongList::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "UpdateVisibleItems", &o2::UILongList::UpdateVisibleItems, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UILongList::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UILongList::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UILongList::OnCursorMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UILongList::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UILongList::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UILongList::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, float>(&type, "OnScrolled", &o2::UILongList::OnScrolled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, UIWidget*, const Vec2F&, int*>(&type, "GetItemUnderPoint", &o2::UILongList::GetItemUnderPoint, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	TypeInitializer::RegFuncParam<int*>(funcInfo, "idxPtr");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const DataNode&>(&type, "OnDeserialized", &o2::UILongList::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "UpdateTransparency", &o2::UILongList::UpdateTransparency, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Vec2F&>(&type, "UpdateHover", &o2::UILongList::UpdateHover, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, int>(&type, "UpdateSelection", &o2::UILongList::UpdateSelection, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "OnSelectionChanged", &o2::UILongList::OnSelectionChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "OnVisibleChanged", &o2::UILongList::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void>(&type, "InitializeProperties", &o2::UILongList::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIMenuPanel::InitializeType(o2::UIMenuPanel* sample)
{
	TypeInitializer::RegField(&type, "mLayout", (size_t)(char*)(&sample->mLayout) - (size_t)(char*)sample, sample->mLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)sample, sample->mItemSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mClickFunctions", (size_t)(char*)(&sample->mClickFunctions) - (size_t)(char*)sample, sample->mClickFunctions, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)sample, sample->mSelectionDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)sample, sample->mSelectionLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurrentSelectionRect", (size_t)(char*)(&sample->mCurrentSelectionRect) - (size_t)(char*)sample, sample->mCurrentSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetSelectionRect", (size_t)(char*)(&sample->mTargetSelectionRect) - (size_t)(char*)sample, sample->mTargetSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)sample, sample->mLastSelectCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedItem", (size_t)(char*)(&sample->mSelectedItem) - (size_t)(char*)sample, sample->mSelectedItem, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectSubContextTime", (size_t)(char*)(&sample->mSelectSubContextTime) - (size_t)(char*)sample, sample->mSelectSubContextTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOpenedContext", (size_t)(char*)(&sample->mOpenedContext) - (size_t)(char*)sample, sample->mOpenedContext, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, float>(&type, "Update", &o2::UIMenuPanel::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void>(&type, "Draw", &o2::UIMenuPanel::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*, const Item&>(&type, "AddItem", &o2::UIMenuPanel::AddItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*, const WString&, const Function<void()>&, ImageAsset*, const ShortcutKeys&>(&type, "AddItem", &o2::UIMenuPanel::AddItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "path");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "clickFunc");
	TypeInitializer::RegFuncParam<ImageAsset*>(funcInfo, "icon");
	TypeInitializer::RegFuncParam<const ShortcutKeys&>(funcInfo, "shortcut");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*, const Item&, int>(&type, "InsertItem", &o2::UIMenuPanel::InsertItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, Vector<Item>>(&type, "AddItems", &o2::UIMenuPanel::AddItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Item>>(funcInfo, "items");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, Vector<Item>, int>(&type, "InsertItems", &o2::UIMenuPanel::InsertItems, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Item>>(funcInfo, "items");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, Item, int>(&type, "GetItem", &o2::UIMenuPanel::GetItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, Vector<Item>>(&type, "GetItems", &o2::UIMenuPanel::GetItems, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, int>(&type, "RemoveItem", &o2::UIMenuPanel::RemoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const WString&>(&type, "RemoveItem", &o2::UIMenuPanel::RemoveItem, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void>(&type, "RemoveAllItems", &o2::UIMenuPanel::RemoveAllItems, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIHorizontalLayout*>(&type, "GetItemsLayout", &o2::UIMenuPanel::GetItemsLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*>(&type, "GetItemSample", &o2::UIMenuPanel::GetItemSample, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, UIWidget*>(&type, "SetItemSample", &o2::UIMenuPanel::SetItemSample, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "sample");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, Sprite*>(&type, "GetSelectionDrawable", &o2::UIMenuPanel::GetSelectionDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Layout&>(&type, "SetSelectionDrawableLayout", &o2::UIMenuPanel::SetSelectionDrawableLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, Layout>(&type, "GetSelectionDrawableLayout", &o2::UIMenuPanel::GetSelectionDrawableLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*, const Item&>(&type, "CreateItem", &o2::UIMenuPanel::CreateItem, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Item&>(funcInfo, "item");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, Item, int>(&type, "GetItemDef", &o2::UIMenuPanel::GetItemDef, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<int>(funcInfo, "idx");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void>(&type, "OnVisibleChanged", &o2::UIMenuPanel::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, UIWidget*, const Vec2F&, int*>(&type, "GetItemUnderPoint", &o2::UIMenuPanel::GetItemUnderPoint, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	TypeInitializer::RegFuncParam<int*>(funcInfo, "idxPtr");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Vec2F&>(&type, "UpdateHover", &o2::UIMenuPanel::UpdateHover, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIMenuPanel::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIMenuPanel::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIMenuPanel::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIMenuPanel::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UIMenuPanel::OnCursorMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIMenuPanel, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIMenuPanel::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
}

void o2::UIScrollArea::InitializeType(o2::UIScrollArea* sample)
{
	TypeInitializer::RegField(&type, "scroll", (size_t)(char*)(&sample->scroll) - (size_t)(char*)sample, sample->scroll, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "horScroll", (size_t)(char*)(&sample->horScroll) - (size_t)(char*)sample, sample->horScroll, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "verScroll", (size_t)(char*)(&sample->verScroll) - (size_t)(char*)sample, sample->verScroll, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onScrolled", (size_t)(char*)(&sample->onScrolled) - (size_t)(char*)sample, sample->onScrolled, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mHorScrollBar", (size_t)(char*)(&sample->mHorScrollBar) - (size_t)(char*)sample, sample->mHorScrollBar, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVerScrollBar", (size_t)(char*)(&sample->mVerScrollBar) - (size_t)(char*)sample, sample->mVerScrollBar, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOwnHorScrollBar", (size_t)(char*)(&sample->mOwnHorScrollBar) - (size_t)(char*)sample, sample->mOwnHorScrollBar, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOwnVerScrollBar", (size_t)(char*)(&sample->mOwnVerScrollBar) - (size_t)(char*)sample, sample->mOwnVerScrollBar, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewAreaLayout", (size_t)(char*)(&sample->mViewAreaLayout) - (size_t)(char*)sample, sample->mViewAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAbsoluteViewArea", (size_t)(char*)(&sample->mAbsoluteViewArea) - (size_t)(char*)sample, sample->mAbsoluteViewArea, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mClipAreaLayout", (size_t)(char*)(&sample->mClipAreaLayout) - (size_t)(char*)sample, sample->mClipAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAbsoluteClipArea", (size_t)(char*)(&sample->mAbsoluteClipArea) - (size_t)(char*)sample, sample->mAbsoluteClipArea, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mScrollPos", (size_t)(char*)(&sample->mScrollPos) - (size_t)(char*)sample, sample->mScrollPos, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollSpeed", (size_t)(char*)(&sample->mScrollSpeed) - (size_t)(char*)sample, sample->mScrollSpeed, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mScrollSpeedDamp", (size_t)(char*)(&sample->mScrollSpeedDamp) - (size_t)(char*)sample, sample->mScrollSpeedDamp, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mScrollArea", (size_t)(char*)(&sample->mScrollArea) - (size_t)(char*)sample, sample->mScrollArea, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mScrollRange", (size_t)(char*)(&sample->mScrollRange) - (size_t)(char*)sample, sample->mScrollRange, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEnableHorScroll", (size_t)(char*)(&sample->mEnableHorScroll) - (size_t)(char*)sample, sample->mEnableHorScroll, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void>(&type, "Draw", &o2::UIScrollArea::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, float>(&type, "Update", &o2::UIScrollArea::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, const Vec2F&>(&type, "SetScroll", &o2::UIScrollArea::SetScroll, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, Vec2F>(&type, "GetScroll", &o2::UIScrollArea::GetScroll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, RectF>(&type, "GetScrollRange", &o2::UIScrollArea::GetScrollRange, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void>(&type, "ResetSroll", &o2::UIScrollArea::ResetSroll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, float>(&type, "SetHorizontalScroll", &o2::UIScrollArea::SetHorizontalScroll, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, float>(&type, "GetHorizontalScroll", &o2::UIScrollArea::GetHorizontalScroll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, float>(&type, "SetVerticalScroll", &o2::UIScrollArea::SetVerticalScroll, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, float>(&type, "GetVerticalScroll", &o2::UIScrollArea::GetVerticalScroll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, UIHorizontalScrollBar*, bool>(&type, "SetHorizontalScrollBar", &o2::UIScrollArea::SetHorizontalScrollBar, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIHorizontalScrollBar*>(funcInfo, "scrollbar");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "owner");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, UIHorizontalScrollBar*>(&type, "GetHorizontalScrollbar", &o2::UIScrollArea::GetHorizontalScrollbar, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, UIVerticalScrollBar*, bool>(&type, "SetVerticalScrollBar", &o2::UIScrollArea::SetVerticalScrollBar, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIVerticalScrollBar*>(funcInfo, "scrollbar");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "owner");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, UIVerticalScrollBar*>(&type, "GetVerticalScrollbar", &o2::UIScrollArea::GetVerticalScrollbar, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, bool>(&type, "SetEnableScrollsHiding", &o2::UIScrollArea::SetEnableScrollsHiding, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "hideScrolls");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, bool>(&type, "IsScrollsHiding", &o2::UIScrollArea::IsScrollsHiding, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, const Layout&>(&type, "SetClippingLayout", &o2::UIScrollArea::SetClippingLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "clipLayout");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, Layout>(&type, "GetClippingLayout", &o2::UIScrollArea::GetClippingLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, const Layout&>(&type, "SetViewLayout", &o2::UIScrollArea::SetViewLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "viewLayout");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, Layout>(&type, "GetViewLayout", &o2::UIScrollArea::GetViewLayout, o2::ProtectSection::Public);
}

void o2::UIToggle::InitializeType(o2::UIToggle* sample)
{
	TypeInitializer::RegField(&type, "caption", (size_t)(char*)(&sample->caption) - (size_t)(char*)sample, sample->caption, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "toggleGroup", (size_t)(char*)(&sample->toggleGroup) - (size_t)(char*)sample, sample->toggleGroup, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)sample, sample->onClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onToggle", (size_t)(char*)(&sample->onToggle) - (size_t)(char*)sample, sample->onToggle, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onToggleByUser", (size_t)(char*)(&sample->onToggleByUser) - (size_t)(char*)sample, sample->onToggleByUser, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)sample, sample->shortcut, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValueUnknown", (size_t)(char*)(&sample->mValueUnknown) - (size_t)(char*)sample, sample->mValueUnknown, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCaptionText", (size_t)(char*)(&sample->mCaptionText) - (size_t)(char*)sample, sample->mCaptionText, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)sample, sample->mBackLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mToggleGroup", (size_t)(char*)(&sample->mToggleGroup) - (size_t)(char*)sample, sample->mToggleGroup, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, float>(&type, "Update", &o2::UIToggle::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const WString&>(&type, "SetCaption", &o2::UIToggle::SetCaption, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, WString>(&type, "GetCaption", &o2::UIToggle::GetCaption, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, bool>(&type, "SetValue", &o2::UIToggle::SetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void>(&type, "SetValueUnknown", &o2::UIToggle::SetValueUnknown, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, bool>(&type, "IsValueUnknown", &o2::UIToggle::IsValueUnknown, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, bool>(&type, "GetValue", &o2::UIToggle::GetValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, bool>(&type, "IsSelectable", &o2::UIToggle::IsSelectable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, UIToggleGroup*>(&type, "SetToggleGroup", &o2::UIToggle::SetToggleGroup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIToggleGroup*>(funcInfo, "toggleGroup");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, UIToggleGroup*>(&type, "GetToggleGroup", &o2::UIToggle::GetToggleGroup, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIToggle::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIToggle::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIToggle::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIToggle::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIToggle::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Key&>(&type, "OnKeyPressed", &o2::UIToggle::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, const Input::Key&>(&type, "OnKeyReleased", &o2::UIToggle::OnKeyReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIToggle::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void>(&type, "OnVisibleChanged", &o2::UIToggle::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, void>(&type, "InitializeProperties", &o2::UIToggle::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UITreeNode::InitializeType(o2::UITreeNode* sample)
{
	TypeInitializer::RegField(&type, "mNeedRebuild", (size_t)(char*)(&sample->mNeedRebuild) - (size_t)(char*)sample, sample->mNeedRebuild, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandedState", (size_t)(char*)(&sample->mExpandedState) - (size_t)(char*)sample, sample->mExpandedState, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandCoef", (size_t)(char*)(&sample->mExpandCoef) - (size_t)(char*)sample, sample->mExpandCoef, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mObject", (size_t)(char*)(&sample->mObject) - (size_t)(char*)sample, sample->mObject, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTree", (size_t)(char*)(&sample->mTree) - (size_t)(char*)sample, sample->mTree, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mChildsOffset", (size_t)(char*)(&sample->mChildsOffset) - (size_t)(char*)sample, sample->mChildsOffset, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mInsertSizeCoef", (size_t)(char*)(&sample->mInsertSizeCoef) - (size_t)(char*)sample, sample->mInsertSizeCoef, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragSizeCoef", (size_t)(char*)(&sample->mDragSizeCoef) - (size_t)(char*)sample, sample->mDragSizeCoef, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void>(&type, "Draw", &o2::UITreeNode::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, float>(&type, "Update", &o2::UITreeNode::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool, bool>(&type, "SetExpanded", &o2::UITreeNode::SetExpanded, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expanded");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, bool>(&type, "IsExpanded", &o2::UITreeNode::IsExpanded, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool>(&type, "Expand", &o2::UITreeNode::Expand, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool>(&type, "Collapse", &o2::UITreeNode::Collapse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void>(&type, "ExpandAll", &o2::UITreeNode::ExpandAll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void>(&type, "CollapseAll", &o2::UITreeNode::CollapseAll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, UITreeNode*, UnknownType*>(&type, "GetNode", &o2::UITreeNode::GetNode, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool, bool, bool>(&type, "Rebuild", &o2::UITreeNode::Rebuild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChilds");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "deepRebuild");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "immediately");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, float>(&type, "SetChildrenOffset", &o2::UITreeNode::SetChildrenOffset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "offset");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, float>(&type, "GetChildrenOffset", &o2::UITreeNode::GetChildrenOffset, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, UnknownType*>(&type, "GetObject", &o2::UITreeNode::GetObject, o2::ProtectSection::Public);
}

void o2::UITree::InitializeType(o2::UITree* sample)
{
	TypeInitializer::RegField(&type, "onDraggedObjects", (size_t)(char*)(&sample->onDraggedObjects) - (size_t)(char*)sample, sample->onDraggedObjects, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "getParentFunc", (size_t)(char*)(&sample->getParentFunc) - (size_t)(char*)sample, sample->getParentFunc, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "getChildsFunc", (size_t)(char*)(&sample->getChildsFunc) - (size_t)(char*)sample, sample->getChildsFunc, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "setupNodeFunc", (size_t)(char*)(&sample->setupNodeFunc) - (size_t)(char*)sample, sample->setupNodeFunc, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onItemClick", (size_t)(char*)(&sample->onItemClick) - (size_t)(char*)sample, sample->onItemClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onItemDblClick", (size_t)(char*)(&sample->onItemDblClick) - (size_t)(char*)sample, sample->onItemDblClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onItemRBClick", (size_t)(char*)(&sample->onItemRBClick) - (size_t)(char*)sample, sample->onItemRBClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onItemsSelectionChanged", (size_t)(char*)(&sample->onItemsSelectionChanged) - (size_t)(char*)sample, sample->onItemsSelectionChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mNodesPoolResizeCount", (size_t)(char*)(&sample->mNodesPoolResizeCount) - (size_t)(char*)sample, sample->mNodesPoolResizeCount, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionSpritesPoolResizeCount", (size_t)(char*)(&sample->mSelectionSpritesPoolResizeCount) - (size_t)(char*)sample, sample->mSelectionSpritesPoolResizeCount, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedColor", (size_t)(char*)(&sample->mSelectedColor) - (size_t)(char*)sample, sample->mSelectedColor, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mUnselectedColor", (size_t)(char*)(&sample->mUnselectedColor) - (size_t)(char*)sample, sample->mUnselectedColor, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverColor", (size_t)(char*)(&sample->mHoverColor) - (size_t)(char*)sample, sample->mHoverColor, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mNodeExpandTimer", (size_t)(char*)(&sample->mNodeExpandTimer) - (size_t)(char*)sample, sample->mNodeExpandTimer, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mNeedRebuild", (size_t)(char*)(&sample->mNeedRebuild) - (size_t)(char*)sample, sample->mNeedRebuild, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAllNodes", (size_t)(char*)(&sample->mAllNodes) - (size_t)(char*)sample, sample->mAllNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNodeSample", (size_t)(char*)(&sample->mNodeSample) - (size_t)(char*)sample, sample->mNodeSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverDrawable", (size_t)(char*)(&sample->mHoverDrawable) - (size_t)(char*)sample, sample->mHoverDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectedDrawable", (size_t)(char*)(&sample->mSelectedDrawable) - (size_t)(char*)sample, sample->mSelectedDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverLayout", (size_t)(char*)(&sample->mHoverLayout) - (size_t)(char*)sample, sample->mHoverLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurrentHoverRect", (size_t)(char*)(&sample->mCurrentHoverRect) - (size_t)(char*)sample, sample->mCurrentHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetHoverRect", (size_t)(char*)(&sample->mTargetHoverRect) - (size_t)(char*)sample, sample->mTargetHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastHoverCheckCursor", (size_t)(char*)(&sample->mLastHoverCheckCursor) - (size_t)(char*)sample, sample->mLastHoverCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressedPoint", (size_t)(char*)(&sample->mPressedPoint) - (size_t)(char*)sample, sample->mPressedPoint, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHoveredItem", (size_t)(char*)(&sample->mHoveredItem) - (size_t)(char*)sample, sample->mHoveredItem, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedItems", (size_t)(char*)(&sample->mSelectedItems) - (size_t)(char*)sample, sample->mSelectedItems, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mWaitSelectionsUpdate", (size_t)(char*)(&sample->mWaitSelectionsUpdate) - (size_t)(char*)sample, sample->mWaitSelectionsUpdate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNodesPool", (size_t)(char*)(&sample->mNodesPool) - (size_t)(char*)sample, sample->mNodesPool, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionSpritesPool", (size_t)(char*)(&sample->mSelectionSpritesPool) - (size_t)(char*)sample, sample->mSelectionSpritesPool, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandedObjects", (size_t)(char*)(&sample->mExpandedObjects) - (size_t)(char*)sample, sample->mExpandedObjects, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRearrangeType", (size_t)(char*)(&sample->mRearrangeType) - (size_t)(char*)sample, sample->mRearrangeType, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMultiSelectAvailable", (size_t)(char*)(&sample->mMultiSelectAvailable) - (size_t)(char*)sample, sample->mMultiSelectAvailable, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDraggingNodes", (size_t)(char*)(&sample->mDraggingNodes) - (size_t)(char*)sample, sample->mDraggingNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragNode", (size_t)(char*)(&sample->mDragNode) - (size_t)(char*)sample, sample->mDragNode, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragNodeBack", (size_t)(char*)(&sample->mDragNodeBack) - (size_t)(char*)sample, sample->mDragNodeBack, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragOffset", (size_t)(char*)(&sample->mDragOffset) - (size_t)(char*)sample, sample->mDragOffset, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressedNode", (size_t)(char*)(&sample->mPressedNode) - (size_t)(char*)sample, sample->mPressedNode, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mInsertNodeCandidate", (size_t)(char*)(&sample->mInsertNodeCandidate) - (size_t)(char*)sample, sample->mInsertNodeCandidate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mUnderCursorItem", (size_t)(char*)(&sample->mUnderCursorItem) - (size_t)(char*)sample, sample->mUnderCursorItem, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBeforeDragSelected", (size_t)(char*)(&sample->mBeforeDragSelected) - (size_t)(char*)sample, sample->mBeforeDragSelected, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandNodeCandidate", (size_t)(char*)(&sample->mExpandNodeCandidate) - (size_t)(char*)sample, sample->mExpandNodeCandidate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandInsertTime", (size_t)(char*)(&sample->mExpandInsertTime) - (size_t)(char*)sample, sample->mExpandInsertTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressedTime", (size_t)(char*)(&sample->mPressedTime) - (size_t)(char*)sample, sample->mPressedTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDrawDepth", (size_t)(char*)(&sample->mDrawDepth) - (size_t)(char*)sample, sample->mDrawDepth, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNeedUpdateLayout", (size_t)(char*)(&sample->mNeedUpdateLayout) - (size_t)(char*)sample, sample->mNeedUpdateLayout, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "Draw", &o2::UITree::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, float>(&type, "Update", &o2::UITree::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, bool>(&type, "RebuildTree", &o2::UITree::RebuildTree, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "immediately");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*>(&type, "UpdateTreeNode", &o2::UITree::UpdateTreeNode, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*, UnknownType*>(&type, "GetNode", &o2::UITree::GetNode, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "ExpandAll", &o2::UITree::ExpandAll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "CollapseAll", &o2::UITree::CollapseAll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Vector<UnknownType*>>(&type, "GetSelectedObjects", &o2::UITree::GetSelectedObjects, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Vector<UnknownType*>&>(&type, "SetSelectedObjects", &o2::UITree::SetSelectedObjects, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<UnknownType*>&>(funcInfo, "objects");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*>(&type, "SelectObject", &o2::UITree::SelectObject, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*>(&type, "SelectAndExpandObject", &o2::UITree::SelectAndExpandObject, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*>(&type, "DeselectObject", &o2::UITree::DeselectObject, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "DeselectAllObjects", &o2::UITree::DeselectAllObjects, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*, const Vec2F&>(&type, "GetTreeNodeUnderPoint", &o2::UITree::GetTreeNodeUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*>(&type, "GetNodeSample", &o2::UITree::GetNodeSample, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Sprite*>(&type, "GetHoverDrawable", &o2::UITree::GetHoverDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Sprite*>(&type, "GetSelectionDrawable", &o2::UITree::GetSelectionDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Layout&>(&type, "SetSelectionDrawableLayout", &o2::UITree::SetSelectionDrawableLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Layout>(&type, "GetSelectionDrawableLayout", &o2::UITree::GetSelectionDrawableLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*, UnknownType*>(&type, "OnObjectCreated", &o2::UITree::OnObjectCreated, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "parent");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownType*>(&type, "OnObjectRemoved", &o2::UITree::OnObjectRemoved, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const UnknownObjectsVec&>(&type, "OnObjectsChanged", &o2::UITree::OnObjectsChanged, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const UnknownObjectsVec&>(funcInfo, "objects");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool>(&type, "IsScrollable", &o2::UITree::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, int>(&type, "SetNodesPoolResizeCount", &o2::UITree::SetNodesPoolResizeCount, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, int>(&type, "SetSelectionSpritesPoolResizeCount", &o2::UITree::SetSelectionSpritesPoolResizeCount, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, RearrangeType>(&type, "SetRearrangeType", &o2::UITree::SetRearrangeType, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<RearrangeType>(funcInfo, "type");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, RearrangeType>(&type, "GetRearrangeType", &o2::UITree::GetRearrangeType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, bool>(&type, "SetMultipleSelectionAvailable", &o2::UITree::SetMultipleSelectionAvailable, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "available");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool>(&type, "IsMultiSelectionAvailable", &o2::UITree::IsMultiSelectionAvailable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Color4&>(&type, "SetSelectedColor", &o2::UITree::SetSelectedColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Color4>(&type, "GetSelectedColor", &o2::UITree::GetSelectedColor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Color4&>(&type, "SetUnselectedColor", &o2::UITree::SetUnselectedColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Color4>(&type, "GetUnselectedColor", &o2::UITree::GetUnselectedColor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Color4&>(&type, "SetHoverColor", &o2::UITree::SetHoverColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Color4>(&type, "GetHoverColor", &o2::UITree::GetHoverColor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, float>(&type, "SetNodeExpandTimer", &o2::UITree::SetNodeExpandTimer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, float>(&type, "GetNodeExpandTimer", &o2::UITree::GetNodeExpandTimer, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool>(&type, "IsSelectable", &o2::UITree::IsSelectable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, float>(&type, "UpdatePressedNodeExpand", &o2::UITree::UpdatePressedNodeExpand, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, bool>(&type, "UpdateRootNodes", &o2::UITree::UpdateRootNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "updateChilds");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "OnSelected", &o2::UITree::OnSelected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "OnDeselected", &o2::UITree::OnDeselected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, bool>(&type, "UpdateLayout", &o2::UITree::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "UpdateScrollParams", &o2::UITree::UpdateScrollParams, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, float>(&type, "GetCurrentHeight", &o2::UITree::GetCurrentHeight, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UITree::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UITree::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UITree::OnCursorMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UITree::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &o2::UITree::OnCursorRightMouseReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "SelectSingleNode", &o2::UITree::SelectSingleNode, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "SelectMultipleNodes", &o2::UITree::SelectMultipleNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "OnItemsSelected", &o2::UITree::OnItemsSelected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UITree::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UITree::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UITreeNode*>(&type, "UpdateHover", &o2::UITree::UpdateHover, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "itemUnderCursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "BeginDragging", &o2::UITree::BeginDragging, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "UpdateDragging", &o2::UITree::UpdateDragging, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "EndDragging", &o2::UITree::EndDragging, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*>(&type, "CreateTreeNode", &o2::UITree::CreateTreeNode, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UITreeNode*>(&type, "FreeTreeNode", &o2::UITree::FreeTreeNode, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Sprite*>(&type, "CreateSelectionSprite", &o2::UITree::CreateSelectionSprite, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, Sprite*>(&type, "FreeSelectionSprite", &o2::UITree::FreeSelectionSprite, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Sprite*>(funcInfo, "sprite");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "CheckSelectedNodes", &o2::UITree::CheckSelectedNodes, o2::ProtectSection::Protected);
}

void o2::UIVerticalLayout::InitializeType(o2::UIVerticalLayout* sample)
{
	TypeInitializer::RegField(&type, "baseCorner", (size_t)(char*)(&sample->baseCorner) - (size_t)(char*)sample, sample->baseCorner, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "spacing", (size_t)(char*)(&sample->spacing) - (size_t)(char*)sample, sample->spacing, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "border", (size_t)(char*)(&sample->border) - (size_t)(char*)sample, sample->border, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderLeft", (size_t)(char*)(&sample->borderLeft) - (size_t)(char*)sample, sample->borderLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderRight", (size_t)(char*)(&sample->borderRight) - (size_t)(char*)sample, sample->borderRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderTop", (size_t)(char*)(&sample->borderTop) - (size_t)(char*)sample, sample->borderTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderBottom", (size_t)(char*)(&sample->borderBottom) - (size_t)(char*)sample, sample->borderBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "expandWidth", (size_t)(char*)(&sample->expandWidth) - (size_t)(char*)sample, sample->expandWidth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "expandHeight", (size_t)(char*)(&sample->expandHeight) - (size_t)(char*)sample, sample->expandHeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fitByChildren", (size_t)(char*)(&sample->fitByChildren) - (size_t)(char*)sample, sample->fitByChildren, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mBaseCorner", (size_t)(char*)(&sample->mBaseCorner) - (size_t)(char*)sample, sample->mBaseCorner, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSpacing", (size_t)(char*)(&sample->mSpacing) - (size_t)(char*)sample, sample->mSpacing, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)sample, sample->mBorder, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandWidth", (size_t)(char*)(&sample->mExpandWidth) - (size_t)(char*)sample, sample->mExpandWidth, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandHeight", (size_t)(char*)(&sample->mExpandHeight) - (size_t)(char*)sample, sample->mExpandHeight, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFitByChildren", (size_t)(char*)(&sample->mFitByChildren) - (size_t)(char*)sample, sample->mFitByChildren, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, BaseCorner>(&type, "SetBaseCorner", &o2::UIVerticalLayout::SetBaseCorner, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<BaseCorner>(funcInfo, "baseCorner");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, BaseCorner>(&type, "GetBaseCorner", &o2::UIVerticalLayout::GetBaseCorner, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, float>(&type, "SetSpacing", &o2::UIVerticalLayout::SetSpacing, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "spacing");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, float>(&type, "GetSpacing", &o2::UIVerticalLayout::GetSpacing, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, const RectF&>(&type, "SetBorder", &o2::UIVerticalLayout::SetBorder, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "border");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, RectF>(&type, "GetBorder", &o2::UIVerticalLayout::GetBorder, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, float>(&type, "SetBorderLeft", &o2::UIVerticalLayout::SetBorderLeft, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, float>(&type, "GetBorderLeft", &o2::UIVerticalLayout::GetBorderLeft, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, float>(&type, "SetBorderRight", &o2::UIVerticalLayout::SetBorderRight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, float>(&type, "GetBorderRight", &o2::UIVerticalLayout::GetBorderRight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, float>(&type, "SetBorderTop", &o2::UIVerticalLayout::SetBorderTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, float>(&type, "GetBorderTop", &o2::UIVerticalLayout::GetBorderTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, float>(&type, "SetBorderBottom", &o2::UIVerticalLayout::SetBorderBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, float>(&type, "GetBorderBottom", &o2::UIVerticalLayout::GetBorderBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, bool>(&type, "SetWidthExpand", &o2::UIVerticalLayout::SetWidthExpand, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expand");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, bool>(&type, "IsWidthExpand", &o2::UIVerticalLayout::IsWidthExpand, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, bool>(&type, "SetHeightExpand", &o2::UIVerticalLayout::SetHeightExpand, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expand");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, bool>(&type, "IsHeightExpand", &o2::UIVerticalLayout::IsHeightExpand, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, bool>(&type, "SetFitByChildren", &o2::UIVerticalLayout::SetFitByChildren, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "fit");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, bool>(&type, "IsFittingByChildren", &o2::UIVerticalLayout::IsFittingByChildren, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, bool>(&type, "UpdateLayout", &o2::UIVerticalLayout::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, UIWidget*>(&type, "OnChildAdded", &o2::UIVerticalLayout::OnChildAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, UIWidget*>(&type, "OnChildRemoved", &o2::UIVerticalLayout::OnChildRemoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "RearrangeChilds", &o2::UIVerticalLayout::RearrangeChilds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "ArrangeFromTopToBottom", &o2::UIVerticalLayout::ArrangeFromTopToBottom, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "ArrangeFromBottomToTop", &o2::UIVerticalLayout::ArrangeFromBottomToTop, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "ArrangeFromCenter", &o2::UIVerticalLayout::ArrangeFromCenter, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "ExpandSizeByChilds", &o2::UIVerticalLayout::ExpandSizeByChilds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, Vector<float>>(&type, "CalculateExpandedHeights", &o2::UIVerticalLayout::CalculateExpandedHeights, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, UIWidget*, float>(&type, "AlignWidgetByWidth", &o2::UIVerticalLayout::AlignWidgetByWidth, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	TypeInitializer::RegFuncParam<float>(funcInfo, "heightAnchor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "UpdateLayoutParametres", &o2::UIVerticalLayout::UpdateLayoutParametres, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void>(&type, "InitializeProperties", &o2::UIVerticalLayout::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIVerticalProgress::InitializeType(o2::UIVerticalProgress* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)sample, sample->minValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)sample, sample->maxValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)sample, sample->scrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)sample, sample->onChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)sample, sample->mSmoothValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)sample, sample->mMinValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)sample, sample->mMaxValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)sample, sample->mScrollSense, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOrientation", (size_t)(char*)(&sample->mOrientation) - (size_t)(char*)sample, sample->mOrientation, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBarLayer", (size_t)(char*)(&sample->mBarLayer) - (size_t)(char*)sample, sample->mBarLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)sample, sample->mBackLayer, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "Update", &o2::UIVerticalProgress::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "SetValue", &o2::UIVerticalProgress::SetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "SetValueForcible", &o2::UIVerticalProgress::SetValueForcible, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, float>(&type, "GetValue", &o2::UIVerticalProgress::GetValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "SetMinValue", &o2::UIVerticalProgress::SetMinValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, float>(&type, "GetMinValue", &o2::UIVerticalProgress::GetMinValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "SetMaxValue", &o2::UIVerticalProgress::SetMaxValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, float>(&type, "GetMaxValue", &o2::UIVerticalProgress::GetMaxValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float, float>(&type, "SetValueRange", &o2::UIVerticalProgress::SetValueRange, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "SetScrollSense", &o2::UIVerticalProgress::SetScrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, float>(&type, "GetScrollSense", &o2::UIVerticalProgress::GetScrollSense, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, Orientation>(&type, "SetOrientation", &o2::UIVerticalProgress::SetOrientation, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Orientation>(funcInfo, "orientation");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, Orientation>(&type, "GetOrientation", &o2::UIVerticalProgress::GetOrientation, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIVerticalProgress::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, bool>(&type, "IsScrollable", &o2::UIVerticalProgress::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, bool>(&type, "UpdateLayout", &o2::UIVerticalProgress::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void>(&type, "UpdateProgressLayersLayouts", &o2::UIVerticalProgress::UpdateProgressLayersLayouts, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIVerticalProgress::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "GetValueFromCursor", &o2::UIVerticalProgress::GetValueFromCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIVerticalProgress::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIVerticalProgress::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIVerticalProgress::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIVerticalProgress::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIVerticalProgress::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIVerticalProgress::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, float>(&type, "OnScrolled", &o2::UIVerticalProgress::OnScrolled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, const DataNode&>(&type, "OnDeserialized", &o2::UIVerticalProgress::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void>(&type, "OnVisibleChanged", &o2::UIVerticalProgress::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void>(&type, "InitializeProperties", &o2::UIVerticalProgress::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIVerticalScrollBar::InitializeType(o2::UIVerticalScrollBar* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)sample, sample->minValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)sample, sample->maxValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)sample, sample->scrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSize", (size_t)(char*)(&sample->scrollSize) - (size_t)(char*)sample, sample->scrollSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)sample, sample->onChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSmoothChange", (size_t)(char*)(&sample->onSmoothChange) - (size_t)(char*)sample, sample->onSmoothChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)sample, sample->mSmoothValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)sample, sample->mMinValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)sample, sample->mMaxValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)sample, sample->mScrollSense, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollHandleSize", (size_t)(char*)(&sample->mScrollHandleSize) - (size_t)(char*)sample, sample->mScrollHandleSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollhandleMinPxSize", (size_t)(char*)(&sample->mScrollhandleMinPxSize) - (size_t)(char*)sample, sample->mScrollhandleMinPxSize, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressHandleOffset", (size_t)(char*)(&sample->mPressHandleOffset) - (size_t)(char*)sample, sample->mPressHandleOffset, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandlePressed", (size_t)(char*)(&sample->mHandlePressed) - (size_t)(char*)sample, sample->mHandlePressed, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandleLayer", (size_t)(char*)(&sample->mHandleLayer) - (size_t)(char*)sample, sample->mHandleLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)sample, sample->mBackLayer, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "Update", &o2::UIVerticalScrollBar::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetValue", &o2::UIVerticalScrollBar::SetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetValueForcible", &o2::UIVerticalScrollBar::SetValueForcible, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetValue", &o2::UIVerticalScrollBar::GetValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetSmoothValue", &o2::UIVerticalScrollBar::GetSmoothValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetMinValue", &o2::UIVerticalScrollBar::SetMinValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetMinValue", &o2::UIVerticalScrollBar::GetMinValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetMaxValue", &o2::UIVerticalScrollBar::SetMaxValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetMaxValue", &o2::UIVerticalScrollBar::GetMaxValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float, float>(&type, "SetValueRange", &o2::UIVerticalScrollBar::SetValueRange, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "minValue");
	TypeInitializer::RegFuncParam<float>(funcInfo, "maxValue");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetScrollSense", &o2::UIVerticalScrollBar::SetScrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "coef");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetScrollSense", &o2::UIVerticalScrollBar::GetScrollSense, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetScrollHandleSize", &o2::UIVerticalScrollBar::SetScrollHandleSize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float>(&type, "GetScrollhandleSize", &o2::UIVerticalScrollBar::GetScrollhandleSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "SetMinimalScrollhandleSize", &o2::UIVerticalScrollBar::SetMinimalScrollhandleSize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "pixelSize");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIVerticalScrollBar::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, bool>(&type, "IsScrollable", &o2::UIVerticalScrollBar::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, bool>(&type, "UpdateLayout", &o2::UIVerticalScrollBar::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void>(&type, "UpdateProgressLayersLayouts", &o2::UIVerticalScrollBar::UpdateProgressLayersLayouts, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void>(&type, "OnLayoutUpdated", &o2::UIVerticalScrollBar::OnLayoutUpdated, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIVerticalScrollBar::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, float, const Input::Cursor&>(&type, "GetValueFromCursor", &o2::UIVerticalScrollBar::GetValueFromCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIVerticalScrollBar::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UIVerticalScrollBar::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UIVerticalScrollBar::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorStillDown", &o2::UIVerticalScrollBar::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UIVerticalScrollBar::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UIVerticalScrollBar::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, float>(&type, "OnScrolled", &o2::UIVerticalScrollBar::OnScrolled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, const DataNode&>(&type, "OnDeserialized", &o2::UIVerticalScrollBar::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void>(&type, "OnVisibleChanged", &o2::UIVerticalScrollBar::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void>(&type, "InitializeProperties", &o2::UIVerticalScrollBar::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIWidget::InitializeType(o2::UIWidget* sample)
{
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "parent", (size_t)(char*)(&sample->parent) - (size_t)(char*)sample, sample->parent, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "childs", (size_t)(char*)(&sample->childs) - (size_t)(char*)sample, sample->childs, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "layers", (size_t)(char*)(&sample->layers) - (size_t)(char*)sample, sample->layers, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "states", (size_t)(char*)(&sample->states) - (size_t)(char*)sample, sample->states, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "transparency", (size_t)(char*)(&sample->transparency) - (size_t)(char*)sample, sample->transparency, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "resTransparency", (size_t)(char*)(&sample->resTransparency) - (size_t)(char*)sample, sample->resTransparency, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "visible", (size_t)(char*)(&sample->visible) - (size_t)(char*)sample, sample->visible, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "child", (size_t)(char*)(&sample->child) - (size_t)(char*)sample, sample->child, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "layer", (size_t)(char*)(&sample->layer) - (size_t)(char*)sample, sample->layer, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "state", (size_t)(char*)(&sample->state) - (size_t)(char*)sample, sample->state, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "layout", (size_t)(char*)(&sample->layout) - (size_t)(char*)sample, sample->layout, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "onLayoutChanged", (size_t)(char*)(&sample->onLayoutChanged) - (size_t)(char*)sample, sample->onLayoutChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelected", (size_t)(char*)(&sample->onSelected) - (size_t)(char*)sample, sample->onSelected, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onDeselected", (size_t)(char*)(&sample->onDeselected) - (size_t)(char*)sample, sample->onDeselected, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)sample, sample->mName, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLayers", (size_t)(char*)(&sample->mLayers) - (size_t)(char*)sample, sample->mLayers, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mStates", (size_t)(char*)(&sample->mStates) - (size_t)(char*)sample, sample->mStates, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mParent", (size_t)(char*)(&sample->mParent) - (size_t)(char*)sample, sample->mParent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mChilds", (size_t)(char*)(&sample->mChilds) - (size_t)(char*)sample, sample->mChilds, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mChildsAbsRect", (size_t)(char*)(&sample->mChildsAbsRect) - (size_t)(char*)sample, sample->mChildsAbsRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTransparency", (size_t)(char*)(&sample->mTransparency) - (size_t)(char*)sample, sample->mTransparency, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResTransparency", (size_t)(char*)(&sample->mResTransparency) - (size_t)(char*)sample, sample->mResTransparency, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDrawingLayers", (size_t)(char*)(&sample->mDrawingLayers) - (size_t)(char*)sample, sample->mDrawingLayers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedState", (size_t)(char*)(&sample->mSelectedState) - (size_t)(char*)sample, sample->mSelectedState, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsSelected", (size_t)(char*)(&sample->mIsSelected) - (size_t)(char*)sample, sample->mIsSelected, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVisibleState", (size_t)(char*)(&sample->mVisibleState) - (size_t)(char*)sample, sample->mVisibleState, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVisible", (size_t)(char*)(&sample->mVisible) - (size_t)(char*)sample, sample->mVisible, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mResVisible", (size_t)(char*)(&sample->mResVisible) - (size_t)(char*)sample, sample->mResVisible, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFullyDisabled", (size_t)(char*)(&sample->mFullyDisabled) - (size_t)(char*)sample, sample->mFullyDisabled, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsSelectable", (size_t)(char*)(&sample->mIsSelectable) - (size_t)(char*)sample, sample->mIsSelectable, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, float>(&type, "Update", &o2::UIWidget::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "Draw", &o2::UIWidget::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const RectF&, float>(&type, "ForceDraw", &o2::UIWidget::ForceDraw, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "area");
	TypeInitializer::RegFuncParam<float>(funcInfo, "transparency");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const String&>(&type, "SetName", &o2::UIWidget::SetName, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, String>(&type, "GetName", &o2::UIWidget::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidget*>(&type, "GetParent", &o2::UIWidget::GetParent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidget*>(&type, "SetParent", &o2::UIWidget::SetParent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "parent");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidget*, UIWidget*>(&type, "AddChild", &o2::UIWidget::AddChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const WidgetsVec&>(&type, "AddChilds", &o2::UIWidget::AddChilds, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WidgetsVec&>(funcInfo, "widgets");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidget*, UIWidget*, int>(&type, "AddChild", &o2::UIWidget::AddChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	TypeInitializer::RegFuncParam<int>(funcInfo, "index");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const String&>(&type, "RemoveChild", &o2::UIWidget::RemoveChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, UIWidget*, bool>(&type, "RemoveChild", &o2::UIWidget::RemoveChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidget*, const String&>(&type, "GetChild", &o2::UIWidget::GetChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "RemoveAllChilds", &o2::UIWidget::RemoveAllChilds, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, const WidgetsVec&>(&type, "GetChilds", &o2::UIWidget::GetChilds, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetLayer*, UIWidgetLayer*>(&type, "AddLayer", &o2::UIWidget::AddLayer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetLayer*, const String&, IRectDrawable*, const Layout&, float>(&type, "AddLayer", &o2::UIWidget::AddLayer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<IRectDrawable*>(funcInfo, "drawable");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	TypeInitializer::RegFuncParam<float>(funcInfo, "depth");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetLayer*, const String&>(&type, "GetLayer", &o2::UIWidget::GetLayer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, UIWidgetLayer*>(&type, "RemoveLayer", &o2::UIWidget::RemoveLayer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const String&>(&type, "RemoveLayer", &o2::UIWidget::RemoveLayer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "RemoveAllLayers", &o2::UIWidget::RemoveAllLayers, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, const LayersVec&>(&type, "GetLayers", &o2::UIWidget::GetLayers, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetState*, const String&>(&type, "AddState", &o2::UIWidget::AddState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetState*, const String&, const Animation&>(&type, "AddState", &o2::UIWidget::AddState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetState*, UIWidgetState*>(&type, "AddState", &o2::UIWidget::AddState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidgetState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const String&>(&type, "RemoveState", &o2::UIWidget::RemoveState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, UIWidgetState*>(&type, "RemoveState", &o2::UIWidget::RemoveState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidgetState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "RemoveAllStates", &o2::UIWidget::RemoveAllStates, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const String&, bool>(&type, "SetState", &o2::UIWidget::SetState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const String&>(&type, "GetState", &o2::UIWidget::GetState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidgetState*, const String&>(&type, "GetStateObject", &o2::UIWidget::GetStateObject, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, const StatesVec&>(&type, "GetStates", &o2::UIWidget::GetStates, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, float>(&type, "SetTransparency", &o2::UIWidget::SetTransparency, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "transparency");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, float>(&type, "GetTransparency", &o2::UIWidget::GetTransparency, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, float>(&type, "GetResTransparency", &o2::UIWidget::GetResTransparency, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "SetVisible", &o2::UIWidget::SetVisible, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "visible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "SetVisibleForcible", &o2::UIWidget::SetVisibleForcible, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "visible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "Show", &o2::UIWidget::Show, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "Hide", &o2::UIWidget::Hide, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool>(&type, "IsVisible", &o2::UIWidget::IsVisible, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "Select", &o2::UIWidget::Select, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "Deselect", &o2::UIWidget::Deselect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool>(&type, "IsSelected", &o2::UIWidget::IsSelected, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool>(&type, "IsSelectable", &o2::UIWidget::IsSelectable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "SetSelectable", &o2::UIWidget::SetSelectable, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "selectable");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIWidget::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "DrawDebugFrame", &o2::UIWidget::DrawDebugFrame, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "OnSelected", &o2::UIWidget::OnSelected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "OnDeselected", &o2::UIWidget::OnDeselected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "UpdateLayout", &o2::UIWidget::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateTransparency", &o2::UIWidget::UpdateTransparency, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateVisibility", &o2::UIWidget::UpdateVisibility, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidget*>(&type, "OnChildSelected", &o2::UIWidget::OnChildSelected, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "RetargetStatesAnimations", &o2::UIWidget::RetargetStatesAnimations, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "RecalculateAbsRect", &o2::UIWidget::RecalculateAbsRect, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateLayersLayouts", &o2::UIWidget::UpdateLayersLayouts, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateLayersDrawingSequence", &o2::UIWidget::UpdateLayersDrawingSequence, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, WidgetsVec>(&type, "GetChildsNonConst", &o2::UIWidget::GetChildsNonConst, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, LayersVec>(&type, "GetLayersNonConst", &o2::UIWidget::GetLayersNonConst, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, StatesVec>(&type, "GetStatesNonConst", &o2::UIWidget::GetStatesNonConst, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, Dictionary<String, UIWidgetLayer*>>(&type, "GetAllLayers", &o2::UIWidget::GetAllLayers, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, Dictionary<String, UIWidget*>>(&type, "GetAllChilds", &o2::UIWidget::GetAllChilds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIWidget::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidgetState*>(&type, "OnStateAdded", &o2::UIWidget::OnStateAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidget*>(&type, "OnChildAdded", &o2::UIWidget::OnChildAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidget*>(&type, "OnChildRemoved", &o2::UIWidget::OnChildRemoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const DataNode&>(&type, "OnDeserialized", &o2::UIWidget::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "OnVisibleChanged", &o2::UIWidget::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "InitializeProperties", &o2::UIWidget::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIWidgetLayer::InitializeType(o2::UIWidgetLayer* sample)
{
	TypeInitializer::RegField(&type, "layout", (size_t)(char*)(&sample->layout) - (size_t)(char*)sample, sample->layout, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "interactableLayout", (size_t)(char*)(&sample->interactableLayout) - (size_t)(char*)sample, sample->interactableLayout, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "depth", (size_t)(char*)(&sample->depth) - (size_t)(char*)sample, sample->depth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "transparency", (size_t)(char*)(&sample->transparency) - (size_t)(char*)sample, sample->transparency, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "drawable", (size_t)(char*)(&sample->drawable) - (size_t)(char*)sample, sample->drawable, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "child", (size_t)(char*)(&sample->child) - (size_t)(char*)sample, sample->child, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTransparency", (size_t)(char*)(&sample->mTransparency) - (size_t)(char*)sample, sample->mTransparency, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResTransparency", (size_t)(char*)(&sample->mResTransparency) - (size_t)(char*)sample, sample->mResTransparency, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDepth", (size_t)(char*)(&sample->mDepth) - (size_t)(char*)sample, sample->mDepth, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAbsolutePosition", (size_t)(char*)(&sample->mAbsolutePosition) - (size_t)(char*)sample, sample->mAbsolutePosition, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mInteractableArea", (size_t)(char*)(&sample->mInteractableArea) - (size_t)(char*)sample, sample->mInteractableArea, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOwnerWidget", (size_t)(char*)(&sample->mOwnerWidget) - (size_t)(char*)sample, sample->mOwnerWidget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mParent", (size_t)(char*)(&sample->mParent) - (size_t)(char*)sample, sample->mParent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mChilds", (size_t)(char*)(&sample->mChilds) - (size_t)(char*)sample, sample->mChilds, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void>(&type, "Draw", &o2::UIWidgetLayer::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, float>(&type, "Update", &o2::UIWidgetLayer::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, UIWidgetLayer*, UIWidgetLayer*>(&type, "AddChild", &o2::UIWidgetLayer::AddChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, bool, UIWidgetLayer*, bool>(&type, "RemoveChild", &o2::UIWidgetLayer::RemoveChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void>(&type, "RemoveAllChilds", &o2::UIWidgetLayer::RemoveAllChilds, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, UIWidgetLayer*>(&type, "SetParent", &o2::UIWidgetLayer::SetParent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "parent");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, UIWidgetLayer*>(&type, "GetParent", &o2::UIWidgetLayer::GetParent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, ChildsVec&>(&type, "GetChilds", &o2::UIWidgetLayer::GetChilds, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, const ChildsVec&>(&type, "GetChilds", &o2::UIWidgetLayer::GetChilds, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, UIWidgetLayer*, const String&, IRectDrawable*, const Layout&, float>(&type, "AddChildLayer", &o2::UIWidgetLayer::AddChildLayer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	TypeInitializer::RegFuncParam<IRectDrawable*>(funcInfo, "drawable");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	TypeInitializer::RegFuncParam<float>(funcInfo, "depth");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, UIWidgetLayer*, const String&>(&type, "GetChild", &o2::UIWidgetLayer::GetChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, LayersVec>(&type, "GetAllChilds", &o2::UIWidgetLayer::GetAllChilds, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, float>(&type, "SetDepth", &o2::UIWidgetLayer::SetDepth, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "depth");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, float>(&type, "GetDepth", &o2::UIWidgetLayer::GetDepth, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, float>(&type, "SetTransparency", &o2::UIWidgetLayer::SetTransparency, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "transparency");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, float>(&type, "GetTransparency", &o2::UIWidgetLayer::GetTransparency, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIWidgetLayer::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, const DataNode&>(&type, "OnDeserialized", &o2::UIWidgetLayer::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, UIWidget*>(&type, "SetOwnerWidget", &o2::UIWidgetLayer::SetOwnerWidget, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "owner");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void, UIWidgetLayer*>(&type, "OnChildAdded", &o2::UIWidgetLayer::OnChildAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void>(&type, "UpdateLayout", &o2::UIWidgetLayer::UpdateLayout, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void>(&type, "UpdateResTransparency", &o2::UIWidgetLayer::UpdateResTransparency, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, Dictionary<String, UIWidgetLayer*>>(&type, "GetAllChildLayers", &o2::UIWidgetLayer::GetAllChildLayers, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, void>(&type, "InitializeProperties", &o2::UIWidgetLayer::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIWidgetLayout::InitializeType(o2::UIWidgetLayout* sample)
{
	TypeInitializer::RegField(&type, "pivot", (size_t)(char*)(&sample->pivot) - (size_t)(char*)sample, sample->pivot, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "size", (size_t)(char*)(&sample->size) - (size_t)(char*)sample, sample->size, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "width", (size_t)(char*)(&sample->width) - (size_t)(char*)sample, sample->width, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "height", (size_t)(char*)(&sample->height) - (size_t)(char*)sample, sample->height, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absPosition", (size_t)(char*)(&sample->absPosition) - (size_t)(char*)sample, sample->absPosition, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absLeftTop", (size_t)(char*)(&sample->absLeftTop) - (size_t)(char*)sample, sample->absLeftTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absLeftBottom", (size_t)(char*)(&sample->absLeftBottom) - (size_t)(char*)sample, sample->absLeftBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absRightTop", (size_t)(char*)(&sample->absRightTop) - (size_t)(char*)sample, sample->absRightTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absRightBottom", (size_t)(char*)(&sample->absRightBottom) - (size_t)(char*)sample, sample->absRightBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absRect", (size_t)(char*)(&sample->absRect) - (size_t)(char*)sample, sample->absRect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absLeft", (size_t)(char*)(&sample->absLeft) - (size_t)(char*)sample, sample->absLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absRight", (size_t)(char*)(&sample->absRight) - (size_t)(char*)sample, sample->absRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absBottom", (size_t)(char*)(&sample->absBottom) - (size_t)(char*)sample, sample->absBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absTop", (size_t)(char*)(&sample->absTop) - (size_t)(char*)sample, sample->absTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "pivotX", (size_t)(char*)(&sample->pivotX) - (size_t)(char*)sample, sample->pivotX, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "pivotY", (size_t)(char*)(&sample->pivotY) - (size_t)(char*)sample, sample->pivotY, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorMin", (size_t)(char*)(&sample->anchorMin) - (size_t)(char*)sample, sample->anchorMin, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorMax", (size_t)(char*)(&sample->anchorMax) - (size_t)(char*)sample, sample->anchorMax, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetMin", (size_t)(char*)(&sample->offsetMin) - (size_t)(char*)sample, sample->offsetMin, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetMax", (size_t)(char*)(&sample->offsetMax) - (size_t)(char*)sample, sample->offsetMax, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorLeft", (size_t)(char*)(&sample->anchorLeft) - (size_t)(char*)sample, sample->anchorLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorRight", (size_t)(char*)(&sample->anchorRight) - (size_t)(char*)sample, sample->anchorRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorBottom", (size_t)(char*)(&sample->anchorBottom) - (size_t)(char*)sample, sample->anchorBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorTop", (size_t)(char*)(&sample->anchorTop) - (size_t)(char*)sample, sample->anchorTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetLeft", (size_t)(char*)(&sample->offsetLeft) - (size_t)(char*)sample, sample->offsetLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetRight", (size_t)(char*)(&sample->offsetRight) - (size_t)(char*)sample, sample->offsetRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetBottom", (size_t)(char*)(&sample->offsetBottom) - (size_t)(char*)sample, sample->offsetBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetTop", (size_t)(char*)(&sample->offsetTop) - (size_t)(char*)sample, sample->offsetTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minSize", (size_t)(char*)(&sample->minSize) - (size_t)(char*)sample, sample->minSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minWidth", (size_t)(char*)(&sample->minWidth) - (size_t)(char*)sample, sample->minWidth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minHeight", (size_t)(char*)(&sample->minHeight) - (size_t)(char*)sample, sample->minHeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxSize", (size_t)(char*)(&sample->maxSize) - (size_t)(char*)sample, sample->maxSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxWidth", (size_t)(char*)(&sample->maxWidth) - (size_t)(char*)sample, sample->maxWidth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxHeight", (size_t)(char*)(&sample->maxHeight) - (size_t)(char*)sample, sample->maxHeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "weight", (size_t)(char*)(&sample->weight) - (size_t)(char*)sample, sample->weight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "widthWeight", (size_t)(char*)(&sample->widthWeight) - (size_t)(char*)sample, sample->widthWeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "heigthWeight", (size_t)(char*)(&sample->heigthWeight) - (size_t)(char*)sample, sample->heigthWeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPivot", (size_t)(char*)(&sample->mPivot) - (size_t)(char*)sample, sample->mPivot, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAnchorMin", (size_t)(char*)(&sample->mAnchorMin) - (size_t)(char*)sample, sample->mAnchorMin, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAnchorMax", (size_t)(char*)(&sample->mAnchorMax) - (size_t)(char*)sample, sample->mAnchorMax, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOffsetMin", (size_t)(char*)(&sample->mOffsetMin) - (size_t)(char*)sample, sample->mOffsetMin, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOffsetMax", (size_t)(char*)(&sample->mOffsetMax) - (size_t)(char*)sample, sample->mOffsetMax, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMinSize", (size_t)(char*)(&sample->mMinSize) - (size_t)(char*)sample, sample->mMinSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxSize", (size_t)(char*)(&sample->mMaxSize) - (size_t)(char*)sample, sample->mMaxSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAbsoluteRect", (size_t)(char*)(&sample->mAbsoluteRect) - (size_t)(char*)sample, sample->mAbsoluteRect, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLocalRect", (size_t)(char*)(&sample->mLocalRect) - (size_t)(char*)sample, sample->mLocalRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mWeight", (size_t)(char*)(&sample->mWeight) - (size_t)(char*)sample, sample->mWeight, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDrivenByParent", (size_t)(char*)(&sample->mDrivenByParent) - (size_t)(char*)sample, sample->mDrivenByParent, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIWidgetLayout::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetPosition", &o2::UIWidgetLayout::SetPosition, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetPosition", &o2::UIWidgetLayout::GetPosition, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetSize", &o2::UIWidgetLayout::SetSize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetSize", &o2::UIWidgetLayout::GetSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetWidth", &o2::UIWidgetLayout::SetWidth, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "width");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetWidth", &o2::UIWidgetLayout::GetWidth, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetHeight", &o2::UIWidgetLayout::SetHeight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "height");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetHeight", &o2::UIWidgetLayout::GetHeight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const RectF&>(&type, "SetRect", &o2::UIWidgetLayout::SetRect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, RectF>(&type, "GetRect", &o2::UIWidgetLayout::GetRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAbsoluteLeft", &o2::UIWidgetLayout::SetAbsoluteLeft, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAbsoluteLeft", &o2::UIWidgetLayout::GetAbsoluteLeft, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAbsoluteRight", &o2::UIWidgetLayout::SetAbsoluteRight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAbsoluteRight", &o2::UIWidgetLayout::GetAbsoluteRight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAbsoluteBottom", &o2::UIWidgetLayout::SetAbsoluteBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAbsoluteBottom", &o2::UIWidgetLayout::GetAbsoluteBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAbsoluteTop", &o2::UIWidgetLayout::SetAbsoluteTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAbsoluteTop", &o2::UIWidgetLayout::GetAbsoluteTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAbsolutePosition", &o2::UIWidgetLayout::SetAbsolutePosition, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "absPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAbsolutePosition", &o2::UIWidgetLayout::GetAbsolutePosition, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAbsoluteLeftTop", &o2::UIWidgetLayout::SetAbsoluteLeftTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "absPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAbsoluteLeftTop", &o2::UIWidgetLayout::GetAbsoluteLeftTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAbsoluteLeftBottom", &o2::UIWidgetLayout::SetAbsoluteLeftBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "absPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAbsoluteLeftBottom", &o2::UIWidgetLayout::GetAbsoluteLeftBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAbsoluteRightTop", &o2::UIWidgetLayout::SetAbsoluteRightTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "absPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAbsoluteRightTop", &o2::UIWidgetLayout::GetAbsoluteRightTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAbsoluteRightBottom", &o2::UIWidgetLayout::SetAbsoluteRightBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "absPosition");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAbsoluteRightBottom", &o2::UIWidgetLayout::GetAbsoluteRightBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const RectF&>(&type, "SetAbsoluteRect", &o2::UIWidgetLayout::SetAbsoluteRect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, RectF>(&type, "GetAbsoluteRect", &o2::UIWidgetLayout::GetAbsoluteRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetPivot", &o2::UIWidgetLayout::SetPivot, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "pivot");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetPivot", &o2::UIWidgetLayout::GetPivot, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetPivotX", &o2::UIWidgetLayout::SetPivotX, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "x");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetPivotX", &o2::UIWidgetLayout::GetPivotX, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetPivotY", &o2::UIWidgetLayout::SetPivotY, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "y");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetPivotY", &o2::UIWidgetLayout::GetPivotY, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAnchorMin", &o2::UIWidgetLayout::SetAnchorMin, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "min");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAnchorMin", &o2::UIWidgetLayout::GetAnchorMin, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetAnchorMax", &o2::UIWidgetLayout::SetAnchorMax, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "max");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetAnchorMax", &o2::UIWidgetLayout::GetAnchorMax, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAnchorLeft", &o2::UIWidgetLayout::SetAnchorLeft, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAnchorLeft", &o2::UIWidgetLayout::GetAnchorLeft, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAnchorRight", &o2::UIWidgetLayout::SetAnchorRight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAnchorRight", &o2::UIWidgetLayout::GetAnchorRight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAnchorBottom", &o2::UIWidgetLayout::SetAnchorBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAnchorBottom", &o2::UIWidgetLayout::GetAnchorBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetAnchorTop", &o2::UIWidgetLayout::SetAnchorTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetAnchorTop", &o2::UIWidgetLayout::GetAnchorTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetOffsetMin", &o2::UIWidgetLayout::SetOffsetMin, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "min");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetOffsetMin", &o2::UIWidgetLayout::GetOffsetMin, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetOffsetMax", &o2::UIWidgetLayout::SetOffsetMax, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "max");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetOffsetMax", &o2::UIWidgetLayout::GetOffsetMax, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetOffsetLeft", &o2::UIWidgetLayout::SetOffsetLeft, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetOffsetLeft", &o2::UIWidgetLayout::GetOffsetLeft, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetOffsetRight", &o2::UIWidgetLayout::SetOffsetRight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetOffsetRight", &o2::UIWidgetLayout::GetOffsetRight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetOffsetBottom", &o2::UIWidgetLayout::SetOffsetBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetOffsetBottom", &o2::UIWidgetLayout::GetOffsetBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetOffsetTop", &o2::UIWidgetLayout::SetOffsetTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetOffsetTop", &o2::UIWidgetLayout::GetOffsetTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetMinimalSize", &o2::UIWidgetLayout::SetMinimalSize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "minSize");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetMinimalSize", &o2::UIWidgetLayout::GetMinimalSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetMinimalWidth", &o2::UIWidgetLayout::SetMinimalWidth, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetMinimalWidth", &o2::UIWidgetLayout::GetMinimalWidth, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetMinimalHeight", &o2::UIWidgetLayout::SetMinimalHeight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetMinimalHeight", &o2::UIWidgetLayout::GetMinimalHeight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetMaximalSize", &o2::UIWidgetLayout::SetMaximalSize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "maxSize");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetMaximalSize", &o2::UIWidgetLayout::GetMaximalSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetMaximalWidth", &o2::UIWidgetLayout::SetMaximalWidth, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetMaximalWidth", &o2::UIWidgetLayout::GetMaximalWidth, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetMaximalHeight", &o2::UIWidgetLayout::SetMaximalHeight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetMaximalHeight", &o2::UIWidgetLayout::GetMaximalHeight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const Vec2F&>(&type, "SetWeight", &o2::UIWidgetLayout::SetWeight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "weight");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, Vec2F>(&type, "GetWeight", &o2::UIWidgetLayout::GetWeight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetWidthWeight", &o2::UIWidgetLayout::SetWidthWeight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "widthWeigth");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetWidthWeight", &o2::UIWidgetLayout::GetWidthWeight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, float>(&type, "SetHeightWeight", &o2::UIWidgetLayout::SetHeightWeight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "heigthWeigth");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, float>(&type, "GetHeightWeight", &o2::UIWidgetLayout::GetHeightWeight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void, const UIWidgetLayout&>(&type, "CopyFrom", &o2::UIWidgetLayout::CopyFrom, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const UIWidgetLayout&>(funcInfo, "other");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void>(&type, "InitializeProperties", &o2::UIWidgetLayout::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIWidgetState::InitializeType(o2::UIWidgetState* sample)
{
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "animation", (size_t)(char*)(&sample->animation) - (size_t)(char*)sample, sample->animation, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "offStateAnimationSpeed", (size_t)(char*)(&sample->offStateAnimationSpeed) - (size_t)(char*)sample, sample->offStateAnimationSpeed, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "onStateFullyTrue", (size_t)(char*)(&sample->onStateFullyTrue) - (size_t)(char*)sample, sample->onStateFullyTrue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onStateFullyFalse", (size_t)(char*)(&sample->onStateFullyFalse) - (size_t)(char*)sample, sample->onStateFullyFalse, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onStateBecomesTrue", (size_t)(char*)(&sample->onStateBecomesTrue) - (size_t)(char*)sample, sample->onStateBecomesTrue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onStateBecomesFalse", (size_t)(char*)(&sample->onStateBecomesFalse) - (size_t)(char*)sample, sample->onStateBecomesFalse, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mState", (size_t)(char*)(&sample->mState) - (size_t)(char*)sample, sample->mState, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIWidgetState, void, bool>(&type, "SetState", &o2::UIWidgetState::SetState, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetState, void, bool>(&type, "SetStateForcible", &o2::UIWidgetState::SetStateForcible, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetState, bool>(&type, "GetState", &o2::UIWidgetState::GetState, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetState, void, float>(&type, "Update", &o2::UIWidgetState::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
}

void o2::UIWindow::InitializeType(o2::UIWindow* sample)
{
	TypeInitializer::RegField(&type, "caption", (size_t)(char*)(&sample->caption) - (size_t)(char*)sample, sample->caption, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "icon", (size_t)(char*)(&sample->icon) - (size_t)(char*)sample, sample->icon, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mIconDrawable", (size_t)(char*)(&sample->mIconDrawable) - (size_t)(char*)sample, sample->mIconDrawable, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCaptionDrawable", (size_t)(char*)(&sample->mCaptionDrawable) - (size_t)(char*)sample, sample->mCaptionDrawable, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mWindowElements", (size_t)(char*)(&sample->mWindowElements) - (size_t)(char*)sample, sample->mWindowElements, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHeadDragHandle", (size_t)(char*)(&sample->mHeadDragHandle) - (size_t)(char*)sample, sample->mHeadDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHeadDragAreaLayout", (size_t)(char*)(&sample->mHeadDragAreaLayout) - (size_t)(char*)sample, sample->mHeadDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHeadDragAreaRect", (size_t)(char*)(&sample->mHeadDragAreaRect) - (size_t)(char*)sample, sample->mHeadDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTopDragHandle", (size_t)(char*)(&sample->mTopDragHandle) - (size_t)(char*)sample, sample->mTopDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTopDragAreaLayout", (size_t)(char*)(&sample->mTopDragAreaLayout) - (size_t)(char*)sample, sample->mTopDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTopDragAreaRect", (size_t)(char*)(&sample->mTopDragAreaRect) - (size_t)(char*)sample, sample->mTopDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBottomDragHandle", (size_t)(char*)(&sample->mBottomDragHandle) - (size_t)(char*)sample, sample->mBottomDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBottomDragAreaLayout", (size_t)(char*)(&sample->mBottomDragAreaLayout) - (size_t)(char*)sample, sample->mBottomDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBottomDragAreaRect", (size_t)(char*)(&sample->mBottomDragAreaRect) - (size_t)(char*)sample, sample->mBottomDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftDragHandle", (size_t)(char*)(&sample->mLeftDragHandle) - (size_t)(char*)sample, sample->mLeftDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftDragAreaLayout", (size_t)(char*)(&sample->mLeftDragAreaLayout) - (size_t)(char*)sample, sample->mLeftDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLeftDragAreaRect", (size_t)(char*)(&sample->mLeftDragAreaRect) - (size_t)(char*)sample, sample->mLeftDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightDragHandle", (size_t)(char*)(&sample->mRightDragHandle) - (size_t)(char*)sample, sample->mRightDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightDragAreaLayout", (size_t)(char*)(&sample->mRightDragAreaLayout) - (size_t)(char*)sample, sample->mRightDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mRightDragAreaRect", (size_t)(char*)(&sample->mRightDragAreaRect) - (size_t)(char*)sample, sample->mRightDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftTopDragHandle", (size_t)(char*)(&sample->mLeftTopDragHandle) - (size_t)(char*)sample, sample->mLeftTopDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftTopDragAreaLayout", (size_t)(char*)(&sample->mLeftTopDragAreaLayout) - (size_t)(char*)sample, sample->mLeftTopDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLeftTopDragAreaRect", (size_t)(char*)(&sample->mLeftTopDragAreaRect) - (size_t)(char*)sample, sample->mLeftTopDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightTopDragHandle", (size_t)(char*)(&sample->mRightTopDragHandle) - (size_t)(char*)sample, sample->mRightTopDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightTopDragAreaLayout", (size_t)(char*)(&sample->mRightTopDragAreaLayout) - (size_t)(char*)sample, sample->mRightTopDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mRightTopDragAreaRect", (size_t)(char*)(&sample->mRightTopDragAreaRect) - (size_t)(char*)sample, sample->mRightTopDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftBottomDragHandle", (size_t)(char*)(&sample->mLeftBottomDragHandle) - (size_t)(char*)sample, sample->mLeftBottomDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftBottomDragAreaLayout", (size_t)(char*)(&sample->mLeftBottomDragAreaLayout) - (size_t)(char*)sample, sample->mLeftBottomDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLeftBottomDragAreaRect", (size_t)(char*)(&sample->mLeftBottomDragAreaRect) - (size_t)(char*)sample, sample->mLeftBottomDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightBottomDragHandle", (size_t)(char*)(&sample->mRightBottomDragHandle) - (size_t)(char*)sample, sample->mRightBottomDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightBottomDragAreaLayout", (size_t)(char*)(&sample->mRightBottomDragAreaLayout) - (size_t)(char*)sample, sample->mRightBottomDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mRightBottomDragAreaRect", (size_t)(char*)(&sample->mRightBottomDragAreaRect) - (size_t)(char*)sample, sample->mRightBottomDragAreaRect, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, float>(&type, "Update", &o2::UIWindow::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "Draw", &o2::UIWindow::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, UIWidget*, UIWidget*>(&type, "AddWindowElement", &o2::UIWindow::AddWindowElement, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, UIWidget*>(&type, "RemoveWindowElement", &o2::UIWindow::RemoveWindowElement, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "RemoveAllWindowElements", &o2::UIWindow::RemoveAllWindowElements, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, Sprite*>(&type, "SetIcon", &o2::UIWindow::SetIcon, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Sprite*>(funcInfo, "icon");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, Sprite*>(&type, "GetIcon", &o2::UIWindow::GetIcon, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, const WString&>(&type, "SetCaption", &o2::UIWindow::SetCaption, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "caption");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, WString>(&type, "GetCaption", &o2::UIWindow::GetCaption, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&, const Layout&>(&type, "SetDragAreaLayouts", &o2::UIWindow::SetDragAreaLayouts, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "head");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "top");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "bottom");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "left");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "right");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "leftTop");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "rightTop");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "leftBottom");
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "rightBottom");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, bool>(&type, "IsSelectable", &o2::UIWindow::IsSelectable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, bool>(&type, "UpdateLayout", &o2::UIWindow::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "UpdateTransparency", &o2::UIWindow::UpdateTransparency, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIWindow::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "InitializeHandles", &o2::UIWindow::InitializeHandles, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, bool>(&type, "SetHandlesInteractable", &o2::UIWindow::SetHandlesInteractable, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "interactable");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "BindHandlesInteractableToVisibility", &o2::UIWindow::BindHandlesInteractableToVisibility, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "OnSelected", &o2::UIWindow::OnSelected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, UIWidget*>(&type, "OnChildSelected", &o2::UIWindow::OnChildSelected, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UIWindow::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, UIWidgetState*>(&type, "OnStateAdded", &o2::UIWindow::OnStateAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetState*>(funcInfo, "state");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "OnVisibleChanged", &o2::UIWindow::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "InitializeProperties", &o2::UIWindow::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::ISerializable::InitializeType(o2::ISerializable* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::ISerializable, DataNode>(&type, "Serialize", &o2::ISerializable::Serialize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, void, const DataNode&>(&type, "Deserialize", &o2::ISerializable::Deserialize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, void, DataNode&>(&type, "OnSerialize", &o2::ISerializable::OnSerialize, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, void, const DataNode&>(&type, "OnDeserialized", &o2::ISerializable::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
}

void o2::ShortcutKeys::InitializeType(o2::ShortcutKeys* sample)
{
	TypeInitializer::RegField(&type, "control", (size_t)(char*)(&sample->control) - (size_t)(char*)sample, sample->control, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "shift", (size_t)(char*)(&sample->shift) - (size_t)(char*)sample, sample->shift, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "alt", (size_t)(char*)(&sample->alt) - (size_t)(char*)sample, sample->alt, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::ShortcutKeys, bool>(&type, "IsPressed", &o2::ShortcutKeys::IsPressed, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ShortcutKeys, bool>(&type, "IsDown", &o2::ShortcutKeys::IsDown, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ShortcutKeys, String>(&type, "AsString", &o2::ShortcutKeys::AsString, o2::ProtectSection::Public);
}

void o2::TimeStamp::InitializeType(o2::TimeStamp* sample)
{
	TypeInitializer::RegField(&type, "mYear", (size_t)(char*)(&sample->mYear) - (size_t)(char*)sample, sample->mYear, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMonth", (size_t)(char*)(&sample->mMonth) - (size_t)(char*)sample, sample->mMonth, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDay", (size_t)(char*)(&sample->mDay) - (size_t)(char*)sample, sample->mDay, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHour", (size_t)(char*)(&sample->mHour) - (size_t)(char*)sample, sample->mHour, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMinute", (size_t)(char*)(&sample->mMinute) - (size_t)(char*)sample, sample->mMinute, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSecond", (size_t)(char*)(&sample->mSecond) - (size_t)(char*)sample, sample->mSecond, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::Curve::InitializeType(o2::Curve* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "length", (size_t)(char*)(&sample->length) - (size_t)(char*)sample, sample->length, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onKeysChanged", (size_t)(char*)(&sample->onKeysChanged) - (size_t)(char*)sample, sample->onKeysChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)sample, sample->mKeys, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::Curve, float, float>(&type, "Evaluate", &o2::Curve::Evaluate, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, Vector<Vec2F>, float>(&type, "AddKeys", &o2::Curve::AddKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Vec2F>>(funcInfo, "values");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, const Key&>(&type, "AddKey", &o2::Curve::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, float, float, float, float, float, float>(&type, "AddKey", &o2::Curve::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, float, float, float>(&type, "AddKey", &o2::Curve::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, Key, float>(&type, "GetKey", &o2::Curve::GetKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, bool, float>(&type, "RemoveKey", &o2::Curve::RemoveKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void>(&type, "RemoveAllKeys", &o2::Curve::RemoveAllKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, bool, float>(&type, "ContainsKey", &o2::Curve::ContainsKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, const KeysVec&>(&type, "GetKeys", &o2::Curve::GetKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, const KeysVec&>(&type, "SetKeys", &o2::Curve::SetKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, float, float>(&type, "SmoothKey", &o2::Curve::SmoothKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, float>(&type, "Length", &o2::Curve::Length, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, bool>(&type, "IsEmpty", &o2::Curve::IsEmpty, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void>(&type, "UpdateApproximation", &o2::Curve::UpdateApproximation, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, KeysVec>(&type, "GetKeysNonContant", &o2::Curve::GetKeysNonContant, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void, const DataNode&>(&type, "OnDeserialized", &o2::Curve::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Curve, void>(&type, "InitializeProperties", &o2::Curve::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::Layout::InitializeType(o2::Layout* sample)
{
	TypeInitializer::RegField(&type, "anchorMin", (size_t)(char*)(&sample->anchorMin) - (size_t)(char*)sample, sample->anchorMin, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "anchorMax", (size_t)(char*)(&sample->anchorMax) - (size_t)(char*)sample, sample->anchorMax, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "offsetMin", (size_t)(char*)(&sample->offsetMin) - (size_t)(char*)sample, sample->offsetMin, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "offsetMax", (size_t)(char*)(&sample->offsetMax) - (size_t)(char*)sample, sample->offsetMax, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::Layout, RectF, const RectF&>(&type, "Calculate", &o2::Layout::Calculate, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "source");
}

void o2::Transform::InitializeType(o2::Transform* sample)
{
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "size", (size_t)(char*)(&sample->size) - (size_t)(char*)sample, sample->size, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scale", (size_t)(char*)(&sample->scale) - (size_t)(char*)sample, sample->scale, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "pivot", (size_t)(char*)(&sample->pivot) - (size_t)(char*)sample, sample->pivot, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldPivot", (size_t)(char*)(&sample->worldPivot) - (size_t)(char*)sample, sample->worldPivot, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "szPivot", (size_t)(char*)(&sample->szPivot) - (size_t)(char*)sample, sample->szPivot, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rect", (size_t)(char*)(&sample->rect) - (size_t)(char*)sample, sample->rect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "angle", (size_t)(char*)(&sample->angle) - (size_t)(char*)sample, sample->angle, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "shear", (size_t)(char*)(&sample->shear) - (size_t)(char*)sample, sample->shear, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "basis", (size_t)(char*)(&sample->basis) - (size_t)(char*)sample, sample->basis, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "nonSizedBasis", (size_t)(char*)(&sample->nonSizedBasis) - (size_t)(char*)sample, sample->nonSizedBasis, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "AABB", (size_t)(char*)(&sample->AABB) - (size_t)(char*)sample, sample->AABB, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftTop", (size_t)(char*)(&sample->leftTop) - (size_t)(char*)sample, sample->leftTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftBottom", (size_t)(char*)(&sample->leftBottom) - (size_t)(char*)sample, sample->leftBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightTop", (size_t)(char*)(&sample->rightTop) - (size_t)(char*)sample, sample->rightTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightBottom", (size_t)(char*)(&sample->rightBottom) - (size_t)(char*)sample, sample->rightBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "right", (size_t)(char*)(&sample->right) - (size_t)(char*)sample, sample->right, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "left", (size_t)(char*)(&sample->left) - (size_t)(char*)sample, sample->left, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "up", (size_t)(char*)(&sample->up) - (size_t)(char*)sample, sample->up, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "down", (size_t)(char*)(&sample->down) - (size_t)(char*)sample, sample->down, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "lookAtPoint", (size_t)(char*)(&sample->lookAtPoint) - (size_t)(char*)sample, sample->lookAtPoint, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mPosition", (size_t)(char*)(&sample->mPosition) - (size_t)(char*)sample, sample->mPosition, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSize", (size_t)(char*)(&sample->mSize) - (size_t)(char*)sample, sample->mSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScale", (size_t)(char*)(&sample->mScale) - (size_t)(char*)sample, sample->mScale, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mPivot", (size_t)(char*)(&sample->mPivot) - (size_t)(char*)sample, sample->mPivot, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAngle", (size_t)(char*)(&sample->mAngle) - (size_t)(char*)sample, sample->mAngle, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mShear", (size_t)(char*)(&sample->mShear) - (size_t)(char*)sample, sample->mShear, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTransform", (size_t)(char*)(&sample->mTransform) - (size_t)(char*)sample, sample->mTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNonSizedTransform", (size_t)(char*)(&sample->mNonSizedTransform) - (size_t)(char*)sample, sample->mNonSizedTransform, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetPosition", &o2::Transform::SetPosition, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetPosition", &o2::Transform::GetPosition, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetSize", &o2::Transform::SetSize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "size");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetSize", &o2::Transform::GetSize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetPivot", &o2::Transform::SetPivot, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "pivot");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetPivot", &o2::Transform::GetPivot, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetWorldPivot", &o2::Transform::SetWorldPivot, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "pivot");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetWorldPivot", &o2::Transform::GetWorldPivot, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetSizePivot", &o2::Transform::SetSizePivot, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "relPivot");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetSizePivot", &o2::Transform::GetSizePivot, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const RectF&>(&type, "SetRect", &o2::Transform::SetRect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, RectF>(&type, "GetRect", &o2::Transform::GetRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetScale", &o2::Transform::SetScale, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "scale");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetScale", &o2::Transform::GetScale, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, float>(&type, "SetAngle", &o2::Transform::SetAngle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "rad");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, float>(&type, "GetAngle", &o2::Transform::GetAngle, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, float>(&type, "SetShear", &o2::Transform::SetShear, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "shear");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, float>(&type, "GetShear", &o2::Transform::GetShear, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Basis&>(&type, "SetBasis", &o2::Transform::SetBasis, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "basis");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Basis>(&type, "GetBasis", &o2::Transform::GetBasis, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Basis&>(&type, "SetNonSizedBasis", &o2::Transform::SetNonSizedBasis, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "basis");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Basis>(&type, "GetNonSizedBasis", &o2::Transform::GetNonSizedBasis, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const RectF&>(&type, "SetAxisAlignedRect", &o2::Transform::SetAxisAlignedRect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, RectF>(&type, "GetAxisAlignedRect", &o2::Transform::GetAxisAlignedRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetLeftTop", &o2::Transform::SetLeftTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetLeftTop", &o2::Transform::GetLeftTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetRightTop", &o2::Transform::SetRightTop, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetRightTop", &o2::Transform::GetRightTop, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetLeftBottom", &o2::Transform::SetLeftBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetLeftBottom", &o2::Transform::GetLeftBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetRightBottom", &o2::Transform::SetRightBottom, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetRightBottom", &o2::Transform::GetRightBottom, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetCenter", &o2::Transform::SetCenter, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetCenter", &o2::Transform::GetCenter, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetRight", &o2::Transform::SetRight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetRight", &o2::Transform::GetRight, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetLeft", &o2::Transform::SetLeft, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetLeft", &o2::Transform::GetLeft, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetUp", &o2::Transform::SetUp, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetUp", &o2::Transform::GetUp, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "SetDown", &o2::Transform::SetDown, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "dir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F>(&type, "GetDown", &o2::Transform::GetDown, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const Vec2F&>(&type, "LookAt", &o2::Transform::LookAt, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldPoint");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F, const Vec2F&>(&type, "World2LocalPoint", &o2::Transform::World2LocalPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldPoint");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F, const Vec2F&>(&type, "Local2WorldPoint", &o2::Transform::Local2WorldPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "localPoint");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F, const Vec2F&>(&type, "World2LocalDir", &o2::Transform::World2LocalDir, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "worldDir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, Vec2F, const Vec2F&>(&type, "Local2WorldDir", &o2::Transform::Local2WorldDir, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "localDir");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, bool, const Vec2F&>(&type, "IsPointInside", &o2::Transform::IsPointInside, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void>(&type, "BasisChanged", &o2::Transform::BasisChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void, const DataNode&>(&type, "OnDeserialized", &o2::Transform::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void>(&type, "UpdateTransform", &o2::Transform::UpdateTransform, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Transform, void>(&type, "InitializeProperties", &o2::Transform::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::AnimatedValue<Vec2F>::Key::InitializeType(o2::AnimatedValue<Vec2F>::Key* sample)
{
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "prevSupportValue", (size_t)(char*)(&sample->prevSupportValue) - (size_t)(char*)sample, sample->prevSupportValue, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "nextSupportValue", (size_t)(char*)(&sample->nextSupportValue) - (size_t)(char*)sample, sample->nextSupportValue, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)sample, sample->curvePrevCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)sample, sample->curvePrevCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)sample, sample->curveNextCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)sample, sample->curveNextCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mApproxValues", (size_t)(char*)(&sample->mApproxValues) - (size_t)(char*)sample, sample->mApproxValues, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)sample, sample->mCurveApproxValues, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mApproxLengths", (size_t)(char*)(&sample->mApproxLengths) - (size_t)(char*)sample, sample->mApproxLengths, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mApproxTotalLength", (size_t)(char*)(&sample->mApproxTotalLength) - (size_t)(char*)sample, sample->mApproxTotalLength, o2::ProtectSection::Public);
}

void o2::Animation::AnimatedValueDef::InitializeType(o2::Animation::AnimatedValueDef* sample)
{
	TypeInitializer::RegField(&type, "mTargetPath", (size_t)(char*)(&sample->mTargetPath) - (size_t)(char*)sample, sample->mTargetPath, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTargetPtr", (size_t)(char*)(&sample->mTargetPtr) - (size_t)(char*)sample, sample->mTargetPtr, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mAnimatedValue", (size_t)(char*)(&sample->mAnimatedValue) - (size_t)(char*)sample, sample->mAnimatedValue, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::ActorAsset::MetaInfo::InitializeType(o2::ActorAsset::MetaInfo* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::ActorAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::ActorAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::AnimationAsset::MetaInfo::InitializeType(o2::AnimationAsset::MetaInfo* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimationAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::AnimationAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::Asset::IMetaInfo::InitializeType(o2::Asset::IMetaInfo* sample)
{
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId, o2::ProtectSection::Private).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::Asset::IMetaInfo, Type::Id>(&type, "GetAssetType", &o2::Asset::IMetaInfo::GetAssetType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Asset::IMetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::Asset::IMetaInfo::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
	funcInfo = TypeInitializer::RegFunction<o2::Asset::IMetaInfo, AssetId>(&type, "ID", &o2::Asset::IMetaInfo::ID, o2::ProtectSection::Public);
}

void o2::AssetTree::AssetNode::InitializeType(o2::AssetTree::AssetNode* sample)
{
	TypeInitializer::RegField(&type, "mMeta", (size_t)(char*)(&sample->mMeta) - (size_t)(char*)sample, sample->mMeta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTime", (size_t)(char*)(&sample->mTime) - (size_t)(char*)sample, sample->mTime, o2::ProtectSection::Public);
}

void o2::AtlasAsset::PlatformMeta::InitializeType(o2::AtlasAsset::PlatformMeta* sample)
{
	TypeInitializer::RegField(&type, "mMaxSize", (size_t)(char*)(&sample->mMaxSize) - (size_t)(char*)sample, sample->mMaxSize, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFormat", (size_t)(char*)(&sample->mFormat) - (size_t)(char*)sample, sample->mFormat, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::AtlasAsset::MetaInfo::InitializeType(o2::AtlasAsset::MetaInfo* sample)
{
	TypeInitializer::RegField(&type, "mIOS", (size_t)(char*)(&sample->mIOS) - (size_t)(char*)sample, sample->mIOS, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAndroid", (size_t)(char*)(&sample->mAndroid) - (size_t)(char*)sample, sample->mAndroid, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMacOS", (size_t)(char*)(&sample->mMacOS) - (size_t)(char*)sample, sample->mMacOS, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWindows", (size_t)(char*)(&sample->mWindows) - (size_t)(char*)sample, sample->mWindows, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)sample, sample->mBorder, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::AtlasAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::MetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::AtlasAsset::MetaInfo::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
}

void o2::AtlasAsset::Page::InitializeType(o2::AtlasAsset::Page* sample)
{
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId, o2::ProtectSection::Private).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSize", (size_t)(char*)(&sample->mSize) - (size_t)(char*)sample, sample->mSize, o2::ProtectSection::Private).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mImagesRects", (size_t)(char*)(&sample->mImagesRects) - (size_t)(char*)sample, sample->mImagesRects, o2::ProtectSection::Private).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)sample, sample->mOwner, o2::ProtectSection::Private);
	auto funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, UInt>(&type, "ID", &o2::AtlasAsset::Page::ID, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, Vec2I>(&type, "Size", &o2::AtlasAsset::Page::Size, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, TextureRef>(&type, "GetTextureRef", &o2::AtlasAsset::Page::GetTextureRef, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, String>(&type, "GetTextureFileName", &o2::AtlasAsset::Page::GetTextureFileName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, const Dictionary<AssetId, RectI>&>(&type, "ImagesRects", &o2::AtlasAsset::Page::ImagesRects, o2::ProtectSection::Public);
}

void o2::BinaryAsset::MetaInfo::InitializeType(o2::BinaryAsset::MetaInfo* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::BinaryAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::BitmapFontAsset::MetaInfo::InitializeType(o2::BitmapFontAsset::MetaInfo* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::BitmapFontAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::DataAsset::MetaInfo::InitializeType(o2::DataAsset::MetaInfo* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::DataAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::DataAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::FolderAsset::MetaInfo::InitializeType(o2::FolderAsset::MetaInfo* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::FolderAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::FolderAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::ImageAsset::PlatformMeta::InitializeType(o2::ImageAsset::PlatformMeta* sample)
{
	TypeInitializer::RegField(&type, "mMaxSize", (size_t)(char*)(&sample->mMaxSize) - (size_t)(char*)sample, sample->mMaxSize, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScale", (size_t)(char*)(&sample->mScale) - (size_t)(char*)sample, sample->mScale, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFormat", (size_t)(char*)(&sample->mFormat) - (size_t)(char*)sample, sample->mFormat, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::ImageAsset::MetaInfo::InitializeType(o2::ImageAsset::MetaInfo* sample)
{
	TypeInitializer::RegField(&type, "mAtlasId", (size_t)(char*)(&sample->mAtlasId) - (size_t)(char*)sample, sample->mAtlasId, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mIOS", (size_t)(char*)(&sample->mIOS) - (size_t)(char*)sample, sample->mIOS, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAndroid", (size_t)(char*)(&sample->mAndroid) - (size_t)(char*)sample, sample->mAndroid, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMacOS", (size_t)(char*)(&sample->mMacOS) - (size_t)(char*)sample, sample->mMacOS, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWindows", (size_t)(char*)(&sample->mWindows) - (size_t)(char*)sample, sample->mWindows, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSliceBorder", (size_t)(char*)(&sample->mSliceBorder) - (size_t)(char*)sample, sample->mSliceBorder, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDefaultMode", (size_t)(char*)(&sample->mDefaultMode) - (size_t)(char*)sample, sample->mDefaultMode, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::ImageAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::ImageAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset::MetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::ImageAsset::MetaInfo::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
}

void o2::VectorFontAsset::MetaInfo::InitializeType(o2::VectorFontAsset::MetaInfo* sample)
{
	TypeInitializer::RegField(&type, "mEffects", (size_t)(char*)(&sample->mEffects) - (size_t)(char*)sample, sample->mEffects, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::VectorFontAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset::MetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::VectorFontAsset::MetaInfo::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
}

void o2::AtlasAssetConverter::Image::InitializeType(o2::AtlasAssetConverter::Image* sample)
{
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)sample, sample->mId, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTime", (size_t)(char*)(&sample->mTime) - (size_t)(char*)sample, sample->mTime, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::VectorFont::Effect::InitializeType(o2::VectorFont::Effect* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<o2::VectorFont::Effect, void, Bitmap*>(&type, "Process", &o2::VectorFont::Effect::Process, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::VectorFont::Effect, Vec2I>(&type, "GetSizeExtend", &o2::VectorFont::Effect::GetSizeExtend, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFont::Effect, bool, Effect*>(&type, "IsEqual", &o2::VectorFont::Effect::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Effect*>(funcInfo, "other");
}

void o2::Scene::Layer::InitializeType(o2::Scene::Layer* sample)
{
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)sample, sample->name, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "actors", (size_t)(char*)(&sample->actors) - (size_t)(char*)sample, sample->actors, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabledActors", (size_t)(char*)(&sample->enabledActors) - (size_t)(char*)sample, sample->enabledActors, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "drawables", (size_t)(char*)(&sample->drawables) - (size_t)(char*)sample, sample->drawables, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabledDrawables", (size_t)(char*)(&sample->enabledDrawables) - (size_t)(char*)sample, sample->enabledDrawables, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "RegDrawableComponent", &o2::Scene::Layer::RegDrawableComponent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "UnregDrawableComponent", &o2::Scene::Layer::UnregDrawableComponent, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "ComponentDepthChanged", &o2::Scene::Layer::ComponentDepthChanged, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "ComponentEnabled", &o2::Scene::Layer::ComponentEnabled, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "ComponentDisabled", &o2::Scene::Layer::ComponentDisabled, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
}

void o2::UIContextMenu::Item::InitializeType(o2::UIContextMenu::Item* sample)
{
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)sample, sample->text, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)sample, sample->shortcut, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "icon", (size_t)(char*)(&sample->icon) - (size_t)(char*)sample, sample->icon, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "subItems", (size_t)(char*)(&sample->subItems) - (size_t)(char*)sample, sample->subItems, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)sample, sample->onClick, o2::ProtectSection::Public);
}

void o2::UIMenuPanel::Item::InitializeType(o2::UIMenuPanel::Item* sample)
{
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)sample, sample->text, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "subItems", (size_t)(char*)(&sample->subItems) - (size_t)(char*)sample, sample->subItems, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)sample, sample->onClick, o2::ProtectSection::Public);
}

void o2::Curve::Key::InitializeType(o2::Curve::Key* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "leftCoef", (size_t)(char*)(&sample->leftCoef) - (size_t)(char*)sample, sample->leftCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "leftCoefPosition", (size_t)(char*)(&sample->leftCoefPosition) - (size_t)(char*)sample, sample->leftCoefPosition, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "rightCoef", (size_t)(char*)(&sample->rightCoef) - (size_t)(char*)sample, sample->rightCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "rightCoefPosition", (size_t)(char*)(&sample->rightCoefPosition) - (size_t)(char*)sample, sample->rightCoefPosition, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mApproxValues", (size_t)(char*)(&sample->mApproxValues) - (size_t)(char*)sample, sample->mApproxValues, o2::ProtectSection::Public);
}

void Editor::UIAssetsIconsScrollArea::InitializeType(Editor::UIAssetsIconsScrollArea* sample)
{
	TypeInitializer::RegField(&type, "mAssetIconSize", (size_t)(char*)(&sample->mAssetIconSize) - (size_t)(char*)sample, sample->mAssetIconSize, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedColor", (size_t)(char*)(&sample->mSelectedColor) - (size_t)(char*)sample, sample->mSelectedColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mUnselectedColor", (size_t)(char*)(&sample->mUnselectedColor) - (size_t)(char*)sample, sample->mUnselectedColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHoverColor", (size_t)(char*)(&sample->mHoverColor) - (size_t)(char*)sample, sample->mHoverColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentPath", (size_t)(char*)(&sample->mCurrentPath) - (size_t)(char*)sample, sample->mCurrentPath, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGrid", (size_t)(char*)(&sample->mGrid) - (size_t)(char*)sample, sample->mGrid, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelection", (size_t)(char*)(&sample->mSelection) - (size_t)(char*)sample, sample->mSelection, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mContextMenu", (size_t)(char*)(&sample->mContextMenu) - (size_t)(char*)sample, sample->mContextMenu, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedAssetsIcons", (size_t)(char*)(&sample->mSelectedAssetsIcons) - (size_t)(char*)sample, sample->mSelectedAssetsIcons, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIconSelectionSprite", (size_t)(char*)(&sample->mIconSelectionSprite) - (size_t)(char*)sample, sample->mIconSelectionSprite, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionSpriteLayout", (size_t)(char*)(&sample->mSelectionSpriteLayout) - (size_t)(char*)sample, sample->mSelectionSpriteLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionSpritesPool", (size_t)(char*)(&sample->mSelectionSpritesPool) - (size_t)(char*)sample, sample->mSelectionSpritesPool, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHoverIcon", (size_t)(char*)(&sample->mHoverIcon) - (size_t)(char*)sample, sample->mHoverIcon, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIconHoverSprite", (size_t)(char*)(&sample->mIconHoverSprite) - (size_t)(char*)sample, sample->mIconHoverSprite, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetHoverSpriteRect", (size_t)(char*)(&sample->mTargetHoverSpriteRect) - (size_t)(char*)sample, sample->mTargetHoverSpriteRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentHoverSpriteRect", (size_t)(char*)(&sample->mCurrentHoverSpriteRect) - (size_t)(char*)sample, sample->mCurrentHoverSpriteRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIconsPool", (size_t)(char*)(&sample->mIconsPool) - (size_t)(char*)sample, sample->mIconsPool, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelecting", (size_t)(char*)(&sample->mSelecting) - (size_t)(char*)sample, sample->mSelecting, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressedPoint", (size_t)(char*)(&sample->mPressedPoint) - (size_t)(char*)sample, sample->mPressedPoint, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressTime", (size_t)(char*)(&sample->mPressTime) - (size_t)(char*)sample, sample->mPressTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentSelectingIcons", (size_t)(char*)(&sample->mCurrentSelectingIcons) - (size_t)(char*)sample, sample->mCurrentSelectingIcons, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragState", (size_t)(char*)(&sample->mDragState) - (size_t)(char*)sample, sample->mDragState, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragIcon", (size_t)(char*)(&sample->mDragIcon) - (size_t)(char*)sample, sample->mDragIcon, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragOffset", (size_t)(char*)(&sample->mDragOffset) - (size_t)(char*)sample, sample->mDragOffset, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mInstSceneDragActors", (size_t)(char*)(&sample->mInstSceneDragActors) - (size_t)(char*)sample, sample->mInstSceneDragActors, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragAssetPropertyField", (size_t)(char*)(&sample->mDragAssetPropertyField) - (size_t)(char*)sample, sample->mDragAssetPropertyField, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragActorPropertyField", (size_t)(char*)(&sample->mDragActorPropertyField) - (size_t)(char*)sample, sample->mDragActorPropertyField, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragComponentPropertyField", (size_t)(char*)(&sample->mDragComponentPropertyField) - (size_t)(char*)sample, sample->mDragComponentPropertyField, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCuttingAssets", (size_t)(char*)(&sample->mCuttingAssets) - (size_t)(char*)sample, sample->mCuttingAssets, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNeedRebuildAssets", (size_t)(char*)(&sample->mNeedRebuildAssets) - (size_t)(char*)sample, sample->mNeedRebuildAssets, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "Draw", &Editor::UIAssetsIconsScrollArea::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, float>(&type, "Update", &Editor::UIAssetsIconsScrollArea::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, const String&>(&type, "SetViewingPath", &Editor::UIAssetsIconsScrollArea::SetViewingPath, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, String>(&type, "GetViewingPath", &Editor::UIAssetsIconsScrollArea::GetViewingPath, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "UpdateAssetsPath", &Editor::UIAssetsIconsScrollArea::UpdateAssetsPath, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, bool>(&type, "IsSelectable", &Editor::UIAssetsIconsScrollArea::IsSelectable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, AssetId, bool>(&type, "SelectAsset", &Editor::UIAssetsIconsScrollArea::SelectAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "DeselectAllAssets", &Editor::UIAssetsIconsScrollArea::DeselectAllAssets, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, Vector<AssetInfo>>(&type, "GetSelectedAssets", &Editor::UIAssetsIconsScrollArea::GetSelectedAssets, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, UIAssetIcon*, const Vec2F&>(&type, "GetIconUnderPoint", &Editor::UIAssetsIconsScrollArea::GetIconUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, bool>(&type, "UpdateLayout", &Editor::UIAssetsIconsScrollArea::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "UpdateCuttingAssets", &Editor::UIAssetsIconsScrollArea::UpdateCuttingAssets, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnSelected", &Editor::UIAssetsIconsScrollArea::OnSelected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnDeselected", &Editor::UIAssetsIconsScrollArea::OnDeselected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::UIAssetsIconsScrollArea::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorReleased", &Editor::UIAssetsIconsScrollArea::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &Editor::UIAssetsIconsScrollArea::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorStillDown", &Editor::UIAssetsIconsScrollArea::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "BeginSelecting", &Editor::UIAssetsIconsScrollArea::BeginSelecting, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "UpdateSelection", &Editor::UIAssetsIconsScrollArea::UpdateSelection, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "CompleteSelecting", &Editor::UIAssetsIconsScrollArea::CompleteSelecting, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, UIAssetIcon*, const Input::Cursor&>(&type, "BeginDragging", &Editor::UIAssetsIconsScrollArea::BeginDragging, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIAssetIcon*>(funcInfo, "iconUnderCursor");
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "UpdateDragging", &Editor::UIAssetsIconsScrollArea::UpdateDragging, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "CompleteDragging", &Editor::UIAssetsIconsScrollArea::CompleteDragging, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "RegActorsCreationAction", &Editor::UIAssetsIconsScrollArea::RegActorsCreationAction, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorMoved", &Editor::UIAssetsIconsScrollArea::OnCursorMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &Editor::UIAssetsIconsScrollArea::OnCursorRightMouseReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, const Input::Key&>(&type, "OnKeyReleased", &Editor::UIAssetsIconsScrollArea::OnKeyReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "InitializeContext", &Editor::UIAssetsIconsScrollArea::InitializeContext, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "PrepareIconsPools", &Editor::UIAssetsIconsScrollArea::PrepareIconsPools, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, UIAssetIcon*, const String&>(&type, "GetAssetIconFromPool", &Editor::UIAssetsIconsScrollArea::GetAssetIconFromPool, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "style");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, UIAssetIcon*>(&type, "FreeAssetIconToPool", &Editor::UIAssetsIconsScrollArea::FreeAssetIconToPool, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIAssetIcon*>(funcInfo, "icon");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, UIAssetIcon*, const AssetInfo&>(&type, "GetImageAssetIcon", &Editor::UIAssetsIconsScrollArea::GetImageAssetIcon, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "asset");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "UpdateAssetsGridSize", &Editor::UIAssetsIconsScrollArea::UpdateAssetsGridSize, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "InitializeSelectionSprite", &Editor::UIAssetsIconsScrollArea::InitializeSelectionSprite, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, Sprite*>(&type, "GetSelectionSprite", &Editor::UIAssetsIconsScrollArea::GetSelectionSprite, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, Sprite*>(&type, "FreeSelectionSprite", &Editor::UIAssetsIconsScrollArea::FreeSelectionSprite, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Sprite*>(funcInfo, "sprite");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void, UIAssetIcon*>(&type, "OnIconDblClicked", &Editor::UIAssetsIconsScrollArea::OnIconDblClicked, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIAssetIcon*>(funcInfo, "icon");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "UpdateHover", &Editor::UIAssetsIconsScrollArea::UpdateHover, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextCopyPressed", &Editor::UIAssetsIconsScrollArea::OnContextCopyPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextCutPressed", &Editor::UIAssetsIconsScrollArea::OnContextCutPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextPastePressed", &Editor::UIAssetsIconsScrollArea::OnContextPastePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextDeletePressed", &Editor::UIAssetsIconsScrollArea::OnContextDeletePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextOpenPressed", &Editor::UIAssetsIconsScrollArea::OnContextOpenPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextShowInExplorerPressed", &Editor::UIAssetsIconsScrollArea::OnContextShowInExplorerPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextImportPressed", &Editor::UIAssetsIconsScrollArea::OnContextImportPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextCreateFolderPressed", &Editor::UIAssetsIconsScrollArea::OnContextCreateFolderPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextCreatePrefabPressed", &Editor::UIAssetsIconsScrollArea::OnContextCreatePrefabPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextCreateScriptPressed", &Editor::UIAssetsIconsScrollArea::OnContextCreateScriptPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "OnContextCreateAnimationPressed", &Editor::UIAssetsIconsScrollArea::OnContextCreateAnimationPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "InstantiateDraggingAssets", &Editor::UIAssetsIconsScrollArea::InstantiateDraggingAssets, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, void>(&type, "ClearInstantiatedDraggingAssets", &Editor::UIAssetsIconsScrollArea::ClearInstantiatedDraggingAssets, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, Actor*, const AssetInfo&>(&type, "InstantiateAsset", &Editor::UIAssetsIconsScrollArea::InstantiateAsset, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "assetInfo");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, Actor*, const ImageAsset&>(&type, "InstantiateAsset", &Editor::UIAssetsIconsScrollArea::InstantiateAsset, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const ImageAsset&>(funcInfo, "asset");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsIconsScrollArea, Actor*, const ActorAsset&>(&type, "InstantiateAsset", &Editor::UIAssetsIconsScrollArea::InstantiateAsset, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const ActorAsset&>(funcInfo, "asset");
}

void Editor::AssetsWindow::InitializeType(Editor::AssetsWindow* sample)
{
	TypeInitializer::RegField(&type, "mFilterButton", (size_t)(char*)(&sample->mFilterButton) - (size_t)(char*)sample, sample->mFilterButton, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSearchEditBox", (size_t)(char*)(&sample->mSearchEditBox) - (size_t)(char*)sample, sample->mSearchEditBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedAssetPathLabel", (size_t)(char*)(&sample->mSelectedAssetPathLabel) - (size_t)(char*)sample, sample->mSelectedAssetPathLabel, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFoldersTree", (size_t)(char*)(&sample->mFoldersTree) - (size_t)(char*)sample, sample->mFoldersTree, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAssetsGridScroll", (size_t)(char*)(&sample->mAssetsGridScroll) - (size_t)(char*)sample, sample->mAssetsGridScroll, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAssetsTree", (size_t)(char*)(&sample->mAssetsTree) - (size_t)(char*)sample, sample->mAssetsTree, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSeparatorHandle", (size_t)(char*)(&sample->mSeparatorHandle) - (size_t)(char*)sample, sample->mSeparatorHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCuttingAssets", (size_t)(char*)(&sample->mCuttingAssets) - (size_t)(char*)sample, sample->mCuttingAssets, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, AssetId>(&type, "SelectAsset", &Editor::AssetsWindow::SelectAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "SelectAsset", &Editor::AssetsWindow::SelectAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const Vector<AssetId>&>(&type, "SelectAsset", &Editor::AssetsWindow::SelectAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<AssetId>&>(funcInfo, "ids");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const Vector<String>&>(&type, "SelectAssets", &Editor::AssetsWindow::SelectAssets, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<String>&>(funcInfo, "paths");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, AssetId>(&type, "OpenAsset", &Editor::AssetsWindow::OpenAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "OpenAsset", &Editor::AssetsWindow::OpenAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, AssetId>(&type, "OpenAndEditAsset", &Editor::AssetsWindow::OpenAndEditAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "OpenAndEditAsset", &Editor::AssetsWindow::OpenAndEditAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void>(&type, "DeselectAssets", &Editor::AssetsWindow::DeselectAssets, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, Vector<AssetInfo>>(&type, "GetSelectedAssets", &Editor::AssetsWindow::GetSelectedAssets, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, String>(&type, "GetOpenedFolderPath", &Editor::AssetsWindow::GetOpenedFolderPath, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "OpenFolder", &Editor::AssetsWindow::OpenFolder, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, AssetId>(&type, "ShowAssetIcon", &Editor::AssetsWindow::ShowAssetIcon, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "ShowAssetIcon", &Editor::AssetsWindow::ShowAssetIcon, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const Vector<String>&>(&type, "CopyAssets", &Editor::AssetsWindow::CopyAssets, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<String>&>(funcInfo, "assetsPaths");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const Vector<String>&>(&type, "CutAssets", &Editor::AssetsWindow::CutAssets, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<String>&>(funcInfo, "assetsPaths");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "PasteAssets", &Editor::AssetsWindow::PasteAssets, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const Vector<String>&>(&type, "DeleteAssets", &Editor::AssetsWindow::DeleteAssets, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<String>&>(funcInfo, "assetsPaths");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "ImportAssets", &Editor::AssetsWindow::ImportAssets, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "CreateFolderAsset", &Editor::AssetsWindow::CreateFolderAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "CreatePrefabAsset", &Editor::AssetsWindow::CreatePrefabAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "CreateScriptAsset", &Editor::AssetsWindow::CreateScriptAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&>(&type, "CreateAnimationAsset", &Editor::AssetsWindow::CreateAnimationAsset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "targetPath");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void>(&type, "InitializeWindow", &Editor::AssetsWindow::InitializeWindow, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const WString&>(&type, "OnSearchEdited", &Editor::AssetsWindow::OnSearchEdited, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "search");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void>(&type, "OnMenuFilterPressed", &Editor::AssetsWindow::OnMenuFilterPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void>(&type, "OnShowTreePressed", &Editor::AssetsWindow::OnShowTreePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const Vector<AssetId>&>(&type, "OnAssetsRebuilded", &Editor::AssetsWindow::OnAssetsRebuilded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vector<AssetId>&>(funcInfo, "changedAssets");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetsWindow, void, const String&, const String&>(&type, "CopyAssetFolder", &Editor::AssetsWindow::CopyAssetFolder, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "src");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "dst");
}

void Editor::UIAssetsFoldersTree::InitializeType(Editor::UIAssetsFoldersTree* sample)
{
	TypeInitializer::RegField(&type, "mFoldersTree", (size_t)(char*)(&sample->mFoldersTree) - (size_t)(char*)sample, sample->mFoldersTree, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mContextMenu", (size_t)(char*)(&sample->mContextMenu) - (size_t)(char*)sample, sample->mContextMenu, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentPath", (size_t)(char*)(&sample->mCurrentPath) - (size_t)(char*)sample, sample->mCurrentPath, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void, const String&>(&type, "SelectAndExpandFolder", &Editor::UIAssetsFoldersTree::SelectAndExpandFolder, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "RebuildTree", &Editor::UIAssetsFoldersTree::RebuildTree, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "InitializeContext", &Editor::UIAssetsFoldersTree::InitializeContext, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, UnknownType*, UnknownType*>(&type, "GetFoldersTreeNodeParent", &Editor::UIAssetsFoldersTree::GetFoldersTreeNodeParent, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, Vector<UnknownType*>, UnknownType*>(&type, "GetFoldersTreeNodeChilds", &Editor::UIAssetsFoldersTree::GetFoldersTreeNodeChilds, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void, UITreeNode*, UnknownType*>(&type, "SetupFoldersTreeNode", &Editor::UIAssetsFoldersTree::SetupFoldersTreeNode, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void, UITreeNode*>(&type, "OnFoldersTreeNodeDblClick", &Editor::UIAssetsFoldersTree::OnFoldersTreeNodeDblClick, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void, UITreeNode*>(&type, "OnFoldersTreeClick", &Editor::UIAssetsFoldersTree::OnFoldersTreeClick, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void, UITreeNode*>(&type, "OnFoldersTreeRightClick", &Editor::UIAssetsFoldersTree::OnFoldersTreeRightClick, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextCopyPressed", &Editor::UIAssetsFoldersTree::OnContextCopyPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextCutPressed", &Editor::UIAssetsFoldersTree::OnContextCutPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextPastePressed", &Editor::UIAssetsFoldersTree::OnContextPastePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextDeletePressed", &Editor::UIAssetsFoldersTree::OnContextDeletePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextOpenPressed", &Editor::UIAssetsFoldersTree::OnContextOpenPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextShowInExplorerPressed", &Editor::UIAssetsFoldersTree::OnContextShowInExplorerPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextImportPressed", &Editor::UIAssetsFoldersTree::OnContextImportPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextCreateFolderPressed", &Editor::UIAssetsFoldersTree::OnContextCreateFolderPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextCreatePrefabPressed", &Editor::UIAssetsFoldersTree::OnContextCreatePrefabPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextCreateScriptPressed", &Editor::UIAssetsFoldersTree::OnContextCreateScriptPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextCreateAnimationPressed", &Editor::UIAssetsFoldersTree::OnContextCreateAnimationPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextExpandPressed", &Editor::UIAssetsFoldersTree::OnContextExpandPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetsFoldersTree, void>(&type, "OnContextCollapsePressed", &Editor::UIAssetsFoldersTree::OnContextCollapsePressed, o2::ProtectSection::Protected);
}

void Editor::UIAssetIcon::InitializeType(Editor::UIAssetIcon* sample)
{
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAssetInfo", (size_t)(char*)(&sample->mAssetInfo) - (size_t)(char*)sample, sample->mAssetInfo, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::UIAssetIcon, void, const AssetInfo&>(&type, "SetAssetInfo", &Editor::UIAssetIcon::SetAssetInfo, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const AssetInfo&>(funcInfo, "info");
	funcInfo = TypeInitializer::RegFunction<Editor::UIAssetIcon, const AssetInfo&>(&type, "GetAssetInfo", &Editor::UIAssetIcon::GetAssetInfo, o2::ProtectSection::Public);
}

void Editor::EditorConfig::InitializeType(Editor::EditorConfig* sample)
{
	TypeInitializer::RegField(&type, "mConfigPath", (size_t)(char*)(&sample->mConfigPath) - (size_t)(char*)sample, sample->mConfigPath, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGlobalConfigPath", (size_t)(char*)(&sample->mGlobalConfigPath) - (size_t)(char*)sample, sample->mGlobalConfigPath, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mProjectConfig", (size_t)(char*)(&sample->mProjectConfig) - (size_t)(char*)sample, sample->mProjectConfig, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGlobalConfig", (size_t)(char*)(&sample->mGlobalConfig) - (size_t)(char*)sample, sample->mGlobalConfig, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::EditorConfig, void>(&type, "SaveGlobalConfigs", &Editor::EditorConfig::SaveGlobalConfigs, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::EditorConfig, void>(&type, "SaveProjectConfigs", &Editor::EditorConfig::SaveProjectConfigs, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::EditorConfig, void>(&type, "LoadConfigs", &Editor::EditorConfig::LoadConfigs, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::EditorConfig, void>(&type, "LoadProjectConfig", &Editor::EditorConfig::LoadProjectConfig, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::EditorConfig, void>(&type, "LoadGlobalConfig", &Editor::EditorConfig::LoadGlobalConfig, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::EditorConfig, void>(&type, "OnWindowChange", &Editor::EditorConfig::OnWindowChange, o2::ProtectSection::Protected);
}

void Editor::EditorActorsTransformAction::InitializeType(Editor::EditorActorsTransformAction* sample)
{
	TypeInitializer::RegField(&type, "actorsIds", (size_t)(char*)(&sample->actorsIds) - (size_t)(char*)sample, sample->actorsIds, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "beforeTransforms", (size_t)(char*)(&sample->beforeTransforms) - (size_t)(char*)sample, sample->beforeTransforms, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "doneTransforms", (size_t)(char*)(&sample->doneTransforms) - (size_t)(char*)sample, sample->doneTransforms, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<Editor::EditorActorsTransformAction, String>(&type, "GetName", &Editor::EditorActorsTransformAction::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::EditorActorsTransformAction, void>(&type, "Redo", &Editor::EditorActorsTransformAction::Redo, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::EditorActorsTransformAction, void>(&type, "Undo", &Editor::EditorActorsTransformAction::Undo, o2::ProtectSection::Public);
}

void Editor::CreateActorsAction::InitializeType(Editor::CreateActorsAction* sample)
{
	TypeInitializer::RegField(&type, "actors", (size_t)(char*)(&sample->actors) - (size_t)(char*)sample, sample->actors, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "actorsIds", (size_t)(char*)(&sample->actorsIds) - (size_t)(char*)sample, sample->actorsIds, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "insertParentId", (size_t)(char*)(&sample->insertParentId) - (size_t)(char*)sample, sample->insertParentId, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "insertPrevActorId", (size_t)(char*)(&sample->insertPrevActorId) - (size_t)(char*)sample, sample->insertPrevActorId, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<Editor::CreateActorsAction, String>(&type, "GetName", &Editor::CreateActorsAction::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::CreateActorsAction, void>(&type, "Redo", &Editor::CreateActorsAction::Redo, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::CreateActorsAction, void>(&type, "Undo", &Editor::CreateActorsAction::Undo, o2::ProtectSection::Public);
}

void Editor::DeleteActorsAction::InitializeType(Editor::DeleteActorsAction* sample)
{
	TypeInitializer::RegField(&type, "actorsInfos", (size_t)(char*)(&sample->actorsInfos) - (size_t)(char*)sample, sample->actorsInfos, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<Editor::DeleteActorsAction, String>(&type, "GetName", &Editor::DeleteActorsAction::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::DeleteActorsAction, void>(&type, "Redo", &Editor::DeleteActorsAction::Redo, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::DeleteActorsAction, void>(&type, "Undo", &Editor::DeleteActorsAction::Undo, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::DeleteActorsAction, int, Actor*>(&type, "GetActorIdx", &Editor::DeleteActorsAction::GetActorIdx, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
}

void Editor::EnableAction::InitializeType(Editor::EnableAction* sample)
{
	TypeInitializer::RegField(&type, "actorsIds", (size_t)(char*)(&sample->actorsIds) - (size_t)(char*)sample, sample->actorsIds, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enable", (size_t)(char*)(&sample->enable) - (size_t)(char*)sample, sample->enable, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<Editor::EnableAction, String>(&type, "GetName", &Editor::EnableAction::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::EnableAction, void>(&type, "Redo", &Editor::EnableAction::Redo, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::EnableAction, void>(&type, "Undo", &Editor::EnableAction::Undo, o2::ProtectSection::Public);
}

void Editor::IAction::InitializeType(Editor::IAction* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<Editor::IAction, String>(&type, "GetName", &Editor::IAction::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IAction, void>(&type, "Redo", &Editor::IAction::Redo, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IAction, void>(&type, "Undo", &Editor::IAction::Undo, o2::ProtectSection::Public);
}

void Editor::LockAction::InitializeType(Editor::LockAction* sample)
{
	TypeInitializer::RegField(&type, "actorsIds", (size_t)(char*)(&sample->actorsIds) - (size_t)(char*)sample, sample->actorsIds, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "lock", (size_t)(char*)(&sample->lock) - (size_t)(char*)sample, sample->lock, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<Editor::LockAction, String>(&type, "GetName", &Editor::LockAction::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::LockAction, void>(&type, "Redo", &Editor::LockAction::Redo, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::LockAction, void>(&type, "Undo", &Editor::LockAction::Undo, o2::ProtectSection::Public);
}

void Editor::ReparentActorsAction::InitializeType(Editor::ReparentActorsAction* sample)
{
	TypeInitializer::RegField(&type, "actorsInfos", (size_t)(char*)(&sample->actorsInfos) - (size_t)(char*)sample, sample->actorsInfos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "newParentId", (size_t)(char*)(&sample->newParentId) - (size_t)(char*)sample, sample->newParentId, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "newPrevActorId", (size_t)(char*)(&sample->newPrevActorId) - (size_t)(char*)sample, sample->newPrevActorId, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<Editor::ReparentActorsAction, void, Actor*, Actor*>(&type, "ActorsReparented", &Editor::ReparentActorsAction::ActorsReparented, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "newParent");
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "prevActor");
	funcInfo = TypeInitializer::RegFunction<Editor::ReparentActorsAction, String>(&type, "GetName", &Editor::ReparentActorsAction::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ReparentActorsAction, void>(&type, "Redo", &Editor::ReparentActorsAction::Redo, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ReparentActorsAction, void>(&type, "Undo", &Editor::ReparentActorsAction::Undo, o2::ProtectSection::Public);
}

void Editor::SelectionAction::InitializeType(Editor::SelectionAction* sample)
{
	TypeInitializer::RegField(&type, "selectedActorsIds", (size_t)(char*)(&sample->selectedActorsIds) - (size_t)(char*)sample, sample->selectedActorsIds, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "prevSelectedActorsIds", (size_t)(char*)(&sample->prevSelectedActorsIds) - (size_t)(char*)sample, sample->prevSelectedActorsIds, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<Editor::SelectionAction, String>(&type, "GetName", &Editor::SelectionAction::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionAction, void>(&type, "Redo", &Editor::SelectionAction::Redo, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionAction, void>(&type, "Undo", &Editor::SelectionAction::Undo, o2::ProtectSection::Public);
}

void Editor::FrameTool::InitializeType(Editor::FrameTool* sample)
{
	TypeInitializer::RegField(&type, "mHandleRegularColor", (size_t)(char*)(&sample->mHandleRegularColor) - (size_t)(char*)sample, sample->mHandleRegularColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandleSelectColor", (size_t)(char*)(&sample->mHandleSelectColor) - (size_t)(char*)sample, sample->mHandleSelectColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandlePressedColor", (size_t)(char*)(&sample->mHandlePressedColor) - (size_t)(char*)sample, sample->mHandlePressedColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFrameHandlesSize", (size_t)(char*)(&sample->mFrameHandlesSize) - (size_t)(char*)sample, sample->mFrameHandlesSize, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandlesRotateSize", (size_t)(char*)(&sample->mHandlesRotateSize) - (size_t)(char*)sample, sample->mHandlesRotateSize, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftTopRotateHandle", (size_t)(char*)(&sample->mLeftTopRotateHandle) - (size_t)(char*)sample, sample->mLeftTopRotateHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftBottomRotateHandle", (size_t)(char*)(&sample->mLeftBottomRotateHandle) - (size_t)(char*)sample, sample->mLeftBottomRotateHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightTopRotateHandle", (size_t)(char*)(&sample->mRightTopRotateHandle) - (size_t)(char*)sample, sample->mRightTopRotateHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightBottomRotateHandle", (size_t)(char*)(&sample->mRightBottomRotateHandle) - (size_t)(char*)sample, sample->mRightBottomRotateHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftTopHandle", (size_t)(char*)(&sample->mLeftTopHandle) - (size_t)(char*)sample, sample->mLeftTopHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftHandle", (size_t)(char*)(&sample->mLeftHandle) - (size_t)(char*)sample, sample->mLeftHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftBottomHandle", (size_t)(char*)(&sample->mLeftBottomHandle) - (size_t)(char*)sample, sample->mLeftBottomHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTopHandle", (size_t)(char*)(&sample->mTopHandle) - (size_t)(char*)sample, sample->mTopHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBottomHandle", (size_t)(char*)(&sample->mBottomHandle) - (size_t)(char*)sample, sample->mBottomHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightTopHandle", (size_t)(char*)(&sample->mRightTopHandle) - (size_t)(char*)sample, sample->mRightTopHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightHandle", (size_t)(char*)(&sample->mRightHandle) - (size_t)(char*)sample, sample->mRightHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightBottomHandle", (size_t)(char*)(&sample->mRightBottomHandle) - (size_t)(char*)sample, sample->mRightBottomHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPivotHandle", (size_t)(char*)(&sample->mPivotHandle) - (size_t)(char*)sample, sample->mPivotHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFrame", (size_t)(char*)(&sample->mFrame) - (size_t)(char*)sample, sample->mFrame, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsDragging", (size_t)(char*)(&sample->mIsDragging) - (size_t)(char*)sample, sample->mIsDragging, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mChangedFromThis", (size_t)(char*)(&sample->mChangedFromThis) - (size_t)(char*)sample, sample->mChangedFromThis, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBeforeTransforms", (size_t)(char*)(&sample->mBeforeTransforms) - (size_t)(char*)sample, sample->mBeforeTransforms, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void>(&type, "DrawScene", &Editor::FrameTool::DrawScene, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void>(&type, "OnEnabled", &Editor::FrameTool::OnEnabled, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void>(&type, "OnDisabled", &Editor::FrameTool::OnDisabled, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, Vector<Actor*>>(&type, "OnSceneChanged", &Editor::FrameTool::OnSceneChanged, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "changedActors");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &Editor::FrameTool::OnActorsSelectionChanged, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::FrameTool::OnKeyPressed, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Input::Key&>(&type, "OnKeyStayDown", &Editor::FrameTool::OnKeyStayDown, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Input::Key&>(&type, "OnKeyReleased", &Editor::FrameTool::OnKeyReleased, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Basis&>(&type, "TransformActors", &Editor::FrameTool::TransformActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "transform");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Basis&>(&type, "TransformActorsWithAction", &Editor::FrameTool::TransformActorsWithAction, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Basis&>(funcInfo, "transform");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void>(&type, "UpdateSelectionFrame", &Editor::FrameTool::UpdateSelectionFrame, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::FrameTool::OnCursorPressed, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Input::Cursor&>(&type, "OnCursorReleased", &Editor::FrameTool::OnCursorReleased, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &Editor::FrameTool::OnCursorPressBreak, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Input::Cursor&>(&type, "OnCursorStillDown", &Editor::FrameTool::OnCursorStillDown, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnLeftTopHandle", &Editor::FrameTool::OnLeftTopHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnLeftHandle", &Editor::FrameTool::OnLeftHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnLeftBottomHandle", &Editor::FrameTool::OnLeftBottomHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnTopHandle", &Editor::FrameTool::OnTopHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnBottomHandle", &Editor::FrameTool::OnBottomHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnRightTopHandle", &Editor::FrameTool::OnRightTopHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnRightHandle", &Editor::FrameTool::OnRightHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnRightBottomHandle", &Editor::FrameTool::OnRightBottomHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnPivotHandle", &Editor::FrameTool::OnPivotHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnLeftTopRotateHandle", &Editor::FrameTool::OnLeftTopRotateHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnLeftBottomRotateHandle", &Editor::FrameTool::OnLeftBottomRotateHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnRightTopRotateHandle", &Editor::FrameTool::OnRightTopRotateHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, const Vec2F&>(&type, "OnRightBottomRotateHandle", &Editor::FrameTool::OnRightBottomRotateHandle, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void, bool>(&type, "SetHandlesEnable", &Editor::FrameTool::SetHandlesEnable, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "enable");
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void>(&type, "UdateHandlesTransform", &Editor::FrameTool::UdateHandlesTransform, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void>(&type, "HandlePressed", &Editor::FrameTool::HandlePressed, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FrameTool, void>(&type, "HandleReleased", &Editor::FrameTool::HandleReleased, o2::ProtectSection::Public);
}

void Editor::IEditTool::InitializeType(Editor::IEditTool* sample)
{
	TypeInitializer::RegField(&type, "mNeedRedraw", (size_t)(char*)(&sample->mNeedRedraw) - (size_t)(char*)sample, sample->mNeedRedraw, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void>(&type, "DrawScene", &Editor::IEditTool::DrawScene, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void>(&type, "DrawScreen", &Editor::IEditTool::DrawScreen, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, Vector<Actor*>>(&type, "OnSceneChanged", &Editor::IEditTool::OnSceneChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "changedActors");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, float>(&type, "Update", &Editor::IEditTool::Update, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void>(&type, "OnEnabled", &Editor::IEditTool::OnEnabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void>(&type, "OnDisabled", &Editor::IEditTool::OnDisabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &Editor::IEditTool::OnActorsSelectionChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::IEditTool::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorReleased", &Editor::IEditTool::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &Editor::IEditTool::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorStillDown", &Editor::IEditTool::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorMoved", &Editor::IEditTool::OnCursorMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::IEditTool::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::IEditTool::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorRightMousePressed", &Editor::IEditTool::OnCursorRightMousePressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorRightMouseStayDown", &Editor::IEditTool::OnCursorRightMouseStayDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &Editor::IEditTool::OnCursorRightMouseReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorMiddleMousePressed", &Editor::IEditTool::OnCursorMiddleMousePressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorMiddleMouseStayDown", &Editor::IEditTool::OnCursorMiddleMouseStayDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Cursor&>(&type, "OnCursorMiddleMouseReleased", &Editor::IEditTool::OnCursorMiddleMouseReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, float>(&type, "OnScrolled", &Editor::IEditTool::OnScrolled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::IEditTool::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Key&>(&type, "OnKeyReleased", &Editor::IEditTool::OnKeyReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditTool, void, const Input::Key&>(&type, "OnKeyStayDown", &Editor::IEditTool::OnKeyStayDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::MoveTool::InitializeType(Editor::MoveTool* sample)
{
	TypeInitializer::RegField(&type, "snapStep", (size_t)(char*)(&sample->snapStep) - (size_t)(char*)sample, sample->snapStep, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mHorDragHandle", (size_t)(char*)(&sample->mHorDragHandle) - (size_t)(char*)sample, sample->mHorDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVerDragHandle", (size_t)(char*)(&sample->mVerDragHandle) - (size_t)(char*)sample, sample->mVerDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBothDragHandle", (size_t)(char*)(&sample->mBothDragHandle) - (size_t)(char*)sample, sample->mBothDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastSceneHandlesPos", (size_t)(char*)(&sample->mLastSceneHandlesPos) - (size_t)(char*)sample, sample->mLastSceneHandlesPos, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSnapPosition", (size_t)(char*)(&sample->mSnapPosition) - (size_t)(char*)sample, sample->mSnapPosition, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandlesAngle", (size_t)(char*)(&sample->mHandlesAngle) - (size_t)(char*)sample, sample->mHandlesAngle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBeforeTransforms", (size_t)(char*)(&sample->mBeforeTransforms) - (size_t)(char*)sample, sample->mBeforeTransforms, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, float>(&type, "Update", &Editor::MoveTool::Update, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void>(&type, "OnEnabled", &Editor::MoveTool::OnEnabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void>(&type, "OnDisabled", &Editor::MoveTool::OnDisabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, Vector<Actor*>>(&type, "OnSceneChanged", &Editor::MoveTool::OnSceneChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "changedActors");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &Editor::MoveTool::OnActorsSelectionChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, const Vec2F&>(&type, "OnHorDragHandleMoved", &Editor::MoveTool::OnHorDragHandleMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, const Vec2F&>(&type, "OnVerDragHandleMoved", &Editor::MoveTool::OnVerDragHandleMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, const Vec2F&>(&type, "OnBothDragHandleMoved", &Editor::MoveTool::OnBothDragHandleMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void>(&type, "HandlePressed", &Editor::MoveTool::HandlePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void>(&type, "HandleReleased", &Editor::MoveTool::HandleReleased, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, const Vec2F&, bool, bool>(&type, "HandlesMoved", &Editor::MoveTool::HandlesMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "snapHor");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "spanVer");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void>(&type, "UpdateHandlesPosition", &Editor::MoveTool::UpdateHandlesPosition, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::MoveTool::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, const Input::Key&>(&type, "OnKeyStayDown", &Editor::MoveTool::OnKeyStayDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, const Input::Key&>(&type, "OnKeyReleased", &Editor::MoveTool::OnKeyReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, const Vec2F&>(&type, "MoveSelectedActors", &Editor::MoveTool::MoveSelectedActors, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
	funcInfo = TypeInitializer::RegFunction<Editor::MoveTool, void, const Vec2F&>(&type, "MoveSelectedActorsWithAction", &Editor::MoveTool::MoveSelectedActorsWithAction, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
}

void Editor::RotateTool::InitializeType(Editor::RotateTool* sample)
{
	TypeInitializer::RegField(&type, "angleSnapStep", (size_t)(char*)(&sample->angleSnapStep) - (size_t)(char*)sample, sample->angleSnapStep, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mRotateRingInsideRadius", (size_t)(char*)(&sample->mRotateRingInsideRadius) - (size_t)(char*)sample, sample->mRotateRingInsideRadius, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRotateRingOutsideRadius", (size_t)(char*)(&sample->mRotateRingOutsideRadius) - (size_t)(char*)sample, sample->mRotateRingOutsideRadius, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRotateRingSegs", (size_t)(char*)(&sample->mRotateRingSegs) - (size_t)(char*)sample, sample->mRotateRingSegs, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRotateRingsColor", (size_t)(char*)(&sample->mRotateRingsColor) - (size_t)(char*)sample, sample->mRotateRingsColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRotateRingsFillColor", (size_t)(char*)(&sample->mRotateRingsFillColor) - (size_t)(char*)sample, sample->mRotateRingsFillColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRotateRingsFillColor2", (size_t)(char*)(&sample->mRotateRingsFillColor2) - (size_t)(char*)sample, sample->mRotateRingsFillColor2, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRotateMeshClockwiseColor", (size_t)(char*)(&sample->mRotateMeshClockwiseColor) - (size_t)(char*)sample, sample->mRotateMeshClockwiseColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRotateMeshCClockwiseColor", (size_t)(char*)(&sample->mRotateMeshCClockwiseColor) - (size_t)(char*)sample, sample->mRotateMeshCClockwiseColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRotateRingFillMesh", (size_t)(char*)(&sample->mRotateRingFillMesh) - (size_t)(char*)sample, sample->mRotateRingFillMesh, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAngleMesh", (size_t)(char*)(&sample->mAngleMesh) - (size_t)(char*)sample, sample->mAngleMesh, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mScenePivot", (size_t)(char*)(&sample->mScenePivot) - (size_t)(char*)sample, sample->mScenePivot, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPivotDragHandle", (size_t)(char*)(&sample->mPivotDragHandle) - (size_t)(char*)sample, sample->mPivotDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressAngle", (size_t)(char*)(&sample->mPressAngle) - (size_t)(char*)sample, sample->mPressAngle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentRotateAngle", (size_t)(char*)(&sample->mCurrentRotateAngle) - (size_t)(char*)sample, sample->mCurrentRotateAngle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRingPressed", (size_t)(char*)(&sample->mRingPressed) - (size_t)(char*)sample, sample->mRingPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSnapAngleAccumulated", (size_t)(char*)(&sample->mSnapAngleAccumulated) - (size_t)(char*)sample, sample->mSnapAngleAccumulated, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBeforeTransforms", (size_t)(char*)(&sample->mBeforeTransforms) - (size_t)(char*)sample, sample->mBeforeTransforms, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, float>(&type, "Update", &Editor::RotateTool::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void>(&type, "DrawScreen", &Editor::RotateTool::DrawScreen, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void>(&type, "OnEnabled", &Editor::RotateTool::OnEnabled, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void>(&type, "OnDisabled", &Editor::RotateTool::OnDisabled, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, Vector<Actor*>>(&type, "OnSceneChanged", &Editor::RotateTool::OnSceneChanged, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "changedActors");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &Editor::RotateTool::OnActorsSelectionChanged, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void>(&type, "UpdateMeshes", &Editor::RotateTool::UpdateMeshes, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void>(&type, "CalcPivotByActorsCenter", &Editor::RotateTool::CalcPivotByActorsCenter, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, const Vec2F&>(&type, "OnPivotDragHandleMoved", &Editor::RotateTool::OnPivotDragHandleMoved, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, bool, const Vec2F&>(&type, "IsPointInRotateRing", &Editor::RotateTool::IsPointInRotateRing, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::RotateTool::OnCursorPressed, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, const Input::Cursor&>(&type, "OnCursorReleased", &Editor::RotateTool::OnCursorReleased, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &Editor::RotateTool::OnCursorPressBreak, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, const Input::Cursor&>(&type, "OnCursorStillDown", &Editor::RotateTool::OnCursorStillDown, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::RotateTool::OnKeyPressed, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, const Input::Key&>(&type, "OnKeyStayDown", &Editor::RotateTool::OnKeyStayDown, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, float>(&type, "RotateActors", &Editor::RotateTool::RotateActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "angleDelta");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, float>(&type, "RotateActorsSeparated", &Editor::RotateTool::RotateActorsSeparated, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "angleDelta");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, float>(&type, "RotateActorsWithAction", &Editor::RotateTool::RotateActorsWithAction, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "angleDelta");
	funcInfo = TypeInitializer::RegFunction<Editor::RotateTool, void, float>(&type, "RotateActorsSeparatedWithAction", &Editor::RotateTool::RotateActorsSeparatedWithAction, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "angleDelta");
}

void Editor::ScaleTool::InitializeType(Editor::ScaleTool* sample)
{
	TypeInitializer::RegField(&type, "bothScaleSence", (size_t)(char*)(&sample->bothScaleSence) - (size_t)(char*)sample, sample->bothScaleSence, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mHorDragHandle", (size_t)(char*)(&sample->mHorDragHandle) - (size_t)(char*)sample, sample->mHorDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVerDragHandle", (size_t)(char*)(&sample->mVerDragHandle) - (size_t)(char*)sample, sample->mVerDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBothDragHandle", (size_t)(char*)(&sample->mBothDragHandle) - (size_t)(char*)sample, sample->mBothDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandlesAngle", (size_t)(char*)(&sample->mHandlesAngle) - (size_t)(char*)sample, sample->mHandlesAngle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSceneHandlesPos", (size_t)(char*)(&sample->mSceneHandlesPos) - (size_t)(char*)sample, sample->mSceneHandlesPos, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandlesSize", (size_t)(char*)(&sample->mHandlesSize) - (size_t)(char*)sample, sample->mHandlesSize, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastHorHandlePos", (size_t)(char*)(&sample->mLastHorHandlePos) - (size_t)(char*)sample, sample->mLastHorHandlePos, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastVerHandlePos", (size_t)(char*)(&sample->mLastVerHandlePos) - (size_t)(char*)sample, sample->mLastVerHandlePos, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastBothHandlePos", (size_t)(char*)(&sample->mLastBothHandlePos) - (size_t)(char*)sample, sample->mLastBothHandlePos, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBeforeTransforms", (size_t)(char*)(&sample->mBeforeTransforms) - (size_t)(char*)sample, sample->mBeforeTransforms, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, float>(&type, "Update", &Editor::ScaleTool::Update, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void>(&type, "DrawScreen", &Editor::ScaleTool::DrawScreen, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void>(&type, "OnEnabled", &Editor::ScaleTool::OnEnabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void>(&type, "OnDisabled", &Editor::ScaleTool::OnDisabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, Vector<Actor*>>(&type, "OnSceneChanged", &Editor::ScaleTool::OnSceneChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "changedActors");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &Editor::ScaleTool::OnActorsSelectionChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, const Vec2F&>(&type, "OnHorDragHandleMoved", &Editor::ScaleTool::OnHorDragHandleMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, const Vec2F&>(&type, "OnVerDragHandleMoved", &Editor::ScaleTool::OnVerDragHandleMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, const Vec2F&>(&type, "OnBothDragHandleMoved", &Editor::ScaleTool::OnBothDragHandleMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void>(&type, "UpdateHandlesPosition", &Editor::ScaleTool::UpdateHandlesPosition, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, float>(&type, "UpdateHandlesAngleAndPositions", &Editor::ScaleTool::UpdateHandlesAngleAndPositions, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "angle");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void>(&type, "UpdateHandlesPositions", &Editor::ScaleTool::UpdateHandlesPositions, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::ScaleTool::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, const Input::Key&>(&type, "OnKeyStayDown", &Editor::ScaleTool::OnKeyStayDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, const Input::Key&>(&type, "OnKeyReleased", &Editor::ScaleTool::OnKeyReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void, const Vec2F&>(&type, "ScaleSelectedActors", &Editor::ScaleTool::ScaleSelectedActors, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "scale");
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void>(&type, "HandlePressed", &Editor::ScaleTool::HandlePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::ScaleTool, void>(&type, "HandleReleased", &Editor::ScaleTool::HandleReleased, o2::ProtectSection::Protected);
}

void Editor::SelectionTool::InitializeType(Editor::SelectionTool* sample)
{
	TypeInitializer::RegField(&type, "mSelectionSprite", (size_t)(char*)(&sample->mSelectionSprite) - (size_t)(char*)sample, sample->mSelectionSprite, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentSelectingActors", (size_t)(char*)(&sample->mCurrentSelectingActors) - (size_t)(char*)sample, sample->mCurrentSelectingActors, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBeforeSelectingActors", (size_t)(char*)(&sample->mBeforeSelectingActors) - (size_t)(char*)sample, sample->mBeforeSelectingActors, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressPoint", (size_t)(char*)(&sample->mPressPoint) - (size_t)(char*)sample, sample->mPressPoint, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectingActors", (size_t)(char*)(&sample->mSelectingActors) - (size_t)(char*)sample, sample->mSelectingActors, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void>(&type, "DrawScene", &Editor::SelectionTool::DrawScene, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void>(&type, "DrawScreen", &Editor::SelectionTool::DrawScreen, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void, float>(&type, "Update", &Editor::SelectionTool::Update, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void>(&type, "OnEnabled", &Editor::SelectionTool::OnEnabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void>(&type, "OnDisabled", &Editor::SelectionTool::OnDisabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void, Vector<Actor*>>(&type, "OnActorsSelectionChanged", &Editor::SelectionTool::OnActorsSelectionChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::SelectionTool::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void, const Input::Cursor&>(&type, "OnCursorReleased", &Editor::SelectionTool::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &Editor::SelectionTool::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void, const Input::Cursor&>(&type, "OnCursorStillDown", &Editor::SelectionTool::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void, const Input::Cursor&>(&type, "OnCursorMoved", &Editor::SelectionTool::OnCursorMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SelectionTool, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::SelectionTool::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::IEditorWindow::InitializeType(Editor::IEditorWindow* sample)
{
	TypeInitializer::RegField(&type, "visible", (size_t)(char*)(&sample->visible) - (size_t)(char*)sample, sample->visible, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mWindow", (size_t)(char*)(&sample->mWindow) - (size_t)(char*)sample, sample->mWindow, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::IEditorWindow, void, bool>(&type, "SetVisible", &Editor::IEditorWindow::SetVisible, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "visible");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditorWindow, void, float>(&type, "Update", &Editor::IEditorWindow::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::IEditorWindow, void>(&type, "Draw", &Editor::IEditorWindow::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IEditorWindow, bool>(&type, "IsVisible", &Editor::IEditorWindow::IsVisible, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IEditorWindow, void>(&type, "Show", &Editor::IEditorWindow::Show, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IEditorWindow, void>(&type, "Hide", &Editor::IEditorWindow::Hide, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IEditorWindow, void>(&type, "PostInitializeWindow", &Editor::IEditorWindow::PostInitializeWindow, o2::ProtectSection::Protected);
}

void Editor::UIDockableWindow::InitializeType(Editor::UIDockableWindow* sample)
{
	TypeInitializer::RegField(&type, "mDockSizeCoef", (size_t)(char*)(&sample->mDockSizeCoef) - (size_t)(char*)sample, sample->mDockSizeCoef, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDockBorder", (size_t)(char*)(&sample->mDockBorder) - (size_t)(char*)sample, sample->mDockBorder, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDocked", (size_t)(char*)(&sample->mDocked) - (size_t)(char*)sample, sample->mDocked, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDockingFrameSample", (size_t)(char*)(&sample->mDockingFrameSample) - (size_t)(char*)sample, sample->mDockingFrameSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDockingFrameAppearance", (size_t)(char*)(&sample->mDockingFrameAppearance) - (size_t)(char*)sample, sample->mDockingFrameAppearance, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDockingFrameCurrent", (size_t)(char*)(&sample->mDockingFrameCurrent) - (size_t)(char*)sample, sample->mDockingFrameCurrent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDockingFrameTarget", (size_t)(char*)(&sample->mDockingFrameTarget) - (size_t)(char*)sample, sample->mDockingFrameTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNonDockSize", (size_t)(char*)(&sample->mNonDockSize) - (size_t)(char*)sample, sample->mNonDockSize, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragOffset", (size_t)(char*)(&sample->mDragOffset) - (size_t)(char*)sample, sample->mDragOffset, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void, float>(&type, "Update", &Editor::UIDockableWindow::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void>(&type, "Draw", &Editor::UIDockableWindow::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, bool>(&type, "IsDocked", &Editor::UIDockableWindow::IsDocked, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, Sprite*>(&type, "GetDockingFrameSample", &Editor::UIDockableWindow::GetDockingFrameSample, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void>(&type, "InitializeDockFrameAppearanceAnim", &Editor::UIDockableWindow::InitializeDockFrameAppearanceAnim, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void>(&type, "InitializeDragHandles", &Editor::UIDockableWindow::InitializeDragHandles, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void, const Input::Cursor&>(&type, "OnMoved", &Editor::UIDockableWindow::OnMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void, const Input::Cursor&>(&type, "OnMoveCompleted", &Editor::UIDockableWindow::OnMoveCompleted, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void, const Input::Cursor&>(&type, "OnMoveBegin", &Editor::UIDockableWindow::OnMoveBegin, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, bool, UIDockWindowPlace*&, Side&, RectF&>(&type, "TraceDock", &Editor::UIDockableWindow::TraceDock, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIDockWindowPlace*&>(funcInfo, "targetDock");
	TypeInitializer::RegFuncParam<Side&>(funcInfo, "dockPosition");
	TypeInitializer::RegFuncParam<RectF&>(funcInfo, "dockZoneRect");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void, UIDockWindowPlace*, Side>(&type, "PlaceNonLineDock", &Editor::UIDockableWindow::PlaceNonLineDock, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIDockWindowPlace*>(funcInfo, "targetDock");
	TypeInitializer::RegFuncParam<Side>(funcInfo, "dockPosition");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void, UIDockWindowPlace*, Side, RectF>(&type, "PlaceLineDock", &Editor::UIDockableWindow::PlaceLineDock, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIDockWindowPlace*>(funcInfo, "targetDock");
	TypeInitializer::RegFuncParam<Side>(funcInfo, "dockPosition");
	TypeInitializer::RegFuncParam<RectF>(funcInfo, "dockZoneRect");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void>(&type, "Undock", &Editor::UIDockableWindow::Undock, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockableWindow, void, bool>(&type, "SetDocked", &Editor::UIDockableWindow::SetDocked, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "docked");
}

void Editor::UIDockWindowPlace::InitializeType(Editor::UIDockWindowPlace* sample)
{
	TypeInitializer::RegField(&type, "mResizibleDir", (size_t)(char*)(&sample->mResizibleDir) - (size_t)(char*)sample, sample->mResizibleDir, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNeighborMin", (size_t)(char*)(&sample->mNeighborMin) - (size_t)(char*)sample, sample->mNeighborMin, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragHandleMin", (size_t)(char*)(&sample->mDragHandleMin) - (size_t)(char*)sample, sample->mDragHandleMin, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragHandleLayoutMin", (size_t)(char*)(&sample->mDragHandleLayoutMin) - (size_t)(char*)sample, sample->mDragHandleLayoutMin, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragHandleAreaMin", (size_t)(char*)(&sample->mDragHandleAreaMin) - (size_t)(char*)sample, sample->mDragHandleAreaMin, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNeighborMax", (size_t)(char*)(&sample->mNeighborMax) - (size_t)(char*)sample, sample->mNeighborMax, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragHandleMax", (size_t)(char*)(&sample->mDragHandleMax) - (size_t)(char*)sample, sample->mDragHandleMax, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragHandleLayoutMax", (size_t)(char*)(&sample->mDragHandleLayoutMax) - (size_t)(char*)sample, sample->mDragHandleLayoutMax, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragHandleAreaMax", (size_t)(char*)(&sample->mDragHandleAreaMax) - (size_t)(char*)sample, sample->mDragHandleAreaMax, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragHandleDepth", (size_t)(char*)(&sample->mDragHandleDepth) - (size_t)(char*)sample, sample->mDragHandleDepth, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::UIDockWindowPlace, void>(&type, "Draw", &Editor::UIDockWindowPlace::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockWindowPlace, bool, const Vec2F&>(&type, "IsUnderPoint", &Editor::UIDockWindowPlace::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockWindowPlace, void, TwoDirection, float, UIDockWindowPlace*, UIDockWindowPlace*>(&type, "SetResizibleDir", &Editor::UIDockWindowPlace::SetResizibleDir, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<TwoDirection>(funcInfo, "dir");
	TypeInitializer::RegFuncParam<float>(funcInfo, "border");
	TypeInitializer::RegFuncParam<UIDockWindowPlace*>(funcInfo, "neighborMin");
	TypeInitializer::RegFuncParam<UIDockWindowPlace*>(funcInfo, "neighborMax");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockWindowPlace, TwoDirection>(&type, "GetResizibleDir", &Editor::UIDockWindowPlace::GetResizibleDir, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockWindowPlace, void, bool>(&type, "UpdateLayout", &Editor::UIDockWindowPlace::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockWindowPlace, void, const Vec2F&>(&type, "OnDragHandleMinMoved", &Editor::UIDockWindowPlace::OnDragHandleMinMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockWindowPlace, void, const Vec2F&>(&type, "OnDragHandleMaxMoved", &Editor::UIDockWindowPlace::OnDragHandleMaxMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockWindowPlace, void>(&type, "CheckInteractable", &Editor::UIDockWindowPlace::CheckInteractable, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIDockWindowPlace, void>(&type, "InitializeDragHandle", &Editor::UIDockWindowPlace::InitializeDragHandle, o2::ProtectSection::Protected);
}

void Editor::WindowsLayout::InitializeType(Editor::WindowsLayout* sample)
{
	TypeInitializer::RegField(&type, "mainDock", (size_t)(char*)(&sample->mainDock) - (size_t)(char*)sample, sample->mainDock, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "windows", (size_t)(char*)(&sample->windows) - (size_t)(char*)sample, sample->windows, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<Editor::WindowsLayout, void, WindowDockPlace*, UIDockWindowPlace*>(&type, "RestoreDock", &Editor::WindowsLayout::RestoreDock, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<WindowDockPlace*>(funcInfo, "dockDef");
	TypeInitializer::RegFuncParam<UIDockWindowPlace*>(funcInfo, "dockWidget");
}

void Editor::LogWindow::InitializeType(Editor::LogWindow* sample)
{
	TypeInitializer::RegField(&type, "mList", (size_t)(char*)(&sample->mList) - (size_t)(char*)sample, sample->mList, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastMessageView", (size_t)(char*)(&sample->mLastMessageView) - (size_t)(char*)sample, sample->mLastMessageView, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMessagesCountLabel", (size_t)(char*)(&sample->mMessagesCountLabel) - (size_t)(char*)sample, sample->mMessagesCountLabel, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mWarningsCountLabel", (size_t)(char*)(&sample->mWarningsCountLabel) - (size_t)(char*)sample, sample->mWarningsCountLabel, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mErrorsCountLabel", (size_t)(char*)(&sample->mErrorsCountLabel) - (size_t)(char*)sample, sample->mErrorsCountLabel, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAllMessages", (size_t)(char*)(&sample->mAllMessages) - (size_t)(char*)sample, sample->mAllMessages, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVisibleMessages", (size_t)(char*)(&sample->mVisibleMessages) - (size_t)(char*)sample, sample->mVisibleMessages, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRegularMessagesEnabled", (size_t)(char*)(&sample->mRegularMessagesEnabled) - (size_t)(char*)sample, sample->mRegularMessagesEnabled, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mWarningMessagesEnabled", (size_t)(char*)(&sample->mWarningMessagesEnabled) - (size_t)(char*)sample, sample->mWarningMessagesEnabled, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mErrorMessagesEnabled", (size_t)(char*)(&sample->mErrorMessagesEnabled) - (size_t)(char*)sample, sample->mErrorMessagesEnabled, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRegularMessagesCount", (size_t)(char*)(&sample->mRegularMessagesCount) - (size_t)(char*)sample, sample->mRegularMessagesCount, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mWarningMessagesCount", (size_t)(char*)(&sample->mWarningMessagesCount) - (size_t)(char*)sample, sample->mWarningMessagesCount, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mErrorMessagesCount", (size_t)(char*)(&sample->mErrorMessagesCount) - (size_t)(char*)sample, sample->mErrorMessagesCount, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void, float>(&type, "Update", &Editor::LogWindow::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void>(&type, "InitializeWindow", &Editor::LogWindow::InitializeWindow, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void>(&type, "OnClearPressed", &Editor::LogWindow::OnClearPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void, bool>(&type, "OnRegularMessagesToggled", &Editor::LogWindow::OnRegularMessagesToggled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void, bool>(&type, "OnWarningMessagesToggled", &Editor::LogWindow::OnWarningMessagesToggled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void, bool>(&type, "OnErrorMessagesToggled", &Editor::LogWindow::OnErrorMessagesToggled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void>(&type, "UpdateVisibleMessages", &Editor::LogWindow::UpdateVisibleMessages, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, int>(&type, "GetVisibleMessagesCount", &Editor::LogWindow::GetVisibleMessagesCount, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, Vector<UnknownType*>, int, int>(&type, "GetVisibleMessagesRange", &Editor::LogWindow::GetVisibleMessagesRange, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<int>(funcInfo, "min");
	TypeInitializer::RegFuncParam<int>(funcInfo, "max");
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void, UIWidget*, UnknownType*>(&type, "SetupListMessage", &Editor::LogWindow::SetupListMessage, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "item");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void, const WString&>(&type, "OutStrEx", &Editor::LogWindow::OutStrEx, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "str");
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void, const WString&>(&type, "OutErrorEx", &Editor::LogWindow::OutErrorEx, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "str");
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void, const WString&>(&type, "OutWarningEx", &Editor::LogWindow::OutWarningEx, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "str");
	funcInfo = TypeInitializer::RegFunction<Editor::LogWindow, void>(&type, "UpdateLastMessageView", &Editor::LogWindow::UpdateLastMessageView, o2::ProtectSection::Protected);
}

void Editor::IObjectPropertiesViewer::InitializeType(Editor::IObjectPropertiesViewer* sample)
{
	TypeInitializer::RegField(&type, "mContentWidget", (size_t)(char*)(&sample->mContentWidget) - (size_t)(char*)sample, sample->mContentWidget, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::IObjectPropertiesViewer, const Type*>(&type, "GetViewingObjectType", &Editor::IObjectPropertiesViewer::GetViewingObjectType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IObjectPropertiesViewer, void, const Vector<IObject*>>(&type, "SetTargets", &Editor::IObjectPropertiesViewer::SetTargets, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vector<IObject*>>(funcInfo, "targets");
	funcInfo = TypeInitializer::RegFunction<Editor::IObjectPropertiesViewer, void>(&type, "OnEnabled", &Editor::IObjectPropertiesViewer::OnEnabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::IObjectPropertiesViewer, void>(&type, "OnDisabled", &Editor::IObjectPropertiesViewer::OnDisabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::IObjectPropertiesViewer, void, float>(&type, "Update", &Editor::IObjectPropertiesViewer::Update, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::IObjectPropertiesViewer, void>(&type, "Draw", &Editor::IObjectPropertiesViewer::Draw, o2::ProtectSection::Protected);
}

void Editor::PropertiesWindow::InitializeType(Editor::PropertiesWindow* sample)
{
	TypeInitializer::RegField(&type, "mPropertyFieldsPoolStep", (size_t)(char*)(&sample->mPropertyFieldsPoolStep) - (size_t)(char*)sample, sample->mPropertyFieldsPoolStep, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargets", (size_t)(char*)(&sample->mTargets) - (size_t)(char*)sample, sample->mTargets, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentViewer", (size_t)(char*)(&sample->mCurrentViewer) - (size_t)(char*)sample, sample->mCurrentViewer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewers", (size_t)(char*)(&sample->mViewers) - (size_t)(char*)sample, sample->mViewers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAvailablePropertiesFields", (size_t)(char*)(&sample->mAvailablePropertiesFields) - (size_t)(char*)sample, sample->mAvailablePropertiesFields, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFieldPropertiesPool", (size_t)(char*)(&sample->mFieldPropertiesPool) - (size_t)(char*)sample, sample->mFieldPropertiesPool, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLabelsPool", (size_t)(char*)(&sample->mLabelsPool) - (size_t)(char*)sample, sample->mLabelsPool, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHorLayoutsPool", (size_t)(char*)(&sample->mHorLayoutsPool) - (size_t)(char*)sample, sample->mHorLayoutsPool, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, void, IObject*>(&type, "SetTarget", &Editor::PropertiesWindow::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IObject*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, void, const Vector<IObject*>>(&type, "SetTargets", &Editor::PropertiesWindow::SetTargets, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<IObject*>>(funcInfo, "targets");
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, Vector<IObject*>>(&type, "GetTargets", &Editor::PropertiesWindow::GetTargets, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, void, float>(&type, "Update", &Editor::PropertiesWindow::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, void>(&type, "Draw", &Editor::PropertiesWindow::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, void, UIVerticalLayout*, const Type*, const Vector<IObject*>&, Vector<IPropertyField*>&>(&type, "BuildTypeViewer", &Editor::PropertiesWindow::BuildTypeViewer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIVerticalLayout*>(funcInfo, "layout");
	TypeInitializer::RegFuncParam<const Type*>(funcInfo, "type");
	TypeInitializer::RegFuncParam<const Vector<IObject*>&>(funcInfo, "objects");
	TypeInitializer::RegFuncParam<Vector<IPropertyField*>&>(funcInfo, "usedPropertyFields");
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, String, const String&>(&type, "MakeSmartFieldName", &Editor::PropertiesWindow::MakeSmartFieldName, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "fieldName");
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, void>(&type, "InitializeWindow", &Editor::PropertiesWindow::InitializeWindow, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, void>(&type, "InitializeViewers", &Editor::PropertiesWindow::InitializeViewers, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, void>(&type, "InitializePropertiesFields", &Editor::PropertiesWindow::InitializePropertiesFields, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, IPropertyField*, const Type*>(&type, "CreatePropertyField", &Editor::PropertiesWindow::CreatePropertyField, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Type*>(funcInfo, "type");
	funcInfo = TypeInitializer::RegFunction<Editor::PropertiesWindow, IPropertyField*, const Type*>(&type, "GetAvailableField", &Editor::PropertiesWindow::GetAvailableField, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Type*>(funcInfo, "type");
}

void Editor::ActorPropertiesViewer::InitializeType(Editor::ActorPropertiesViewer* sample)
{
	TypeInitializer::RegField(&type, "mTargetActors", (size_t)(char*)(&sample->mTargetActors) - (size_t)(char*)sample, sample->mTargetActors, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHeaderViewer", (size_t)(char*)(&sample->mHeaderViewer) - (size_t)(char*)sample, sample->mHeaderViewer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTransformViewer", (size_t)(char*)(&sample->mTransformViewer) - (size_t)(char*)sample, sample->mTransformViewer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAnimationViewer", (size_t)(char*)(&sample->mAnimationViewer) - (size_t)(char*)sample, sample->mAnimationViewer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mComponentsViewers", (size_t)(char*)(&sample->mComponentsViewers) - (size_t)(char*)sample, sample->mComponentsViewers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDefaultComponentViewer", (size_t)(char*)(&sample->mDefaultComponentViewer) - (size_t)(char*)sample, sample->mDefaultComponentViewer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAvailableComponentsViewers", (size_t)(char*)(&sample->mAvailableComponentsViewers) - (size_t)(char*)sample, sample->mAvailableComponentsViewers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mComponentViewersPool", (size_t)(char*)(&sample->mComponentViewersPool) - (size_t)(char*)sample, sample->mComponentViewersPool, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewersLayout", (size_t)(char*)(&sample->mViewersLayout) - (size_t)(char*)sample, sample->mViewersLayout, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::ActorPropertiesViewer, const Type*>(&type, "GetViewingObjectType", &Editor::ActorPropertiesViewer::GetViewingObjectType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ActorPropertiesViewer, void, IActorHeaderViewer*>(&type, "SetActorHeaderViewer", &Editor::ActorPropertiesViewer::SetActorHeaderViewer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IActorHeaderViewer*>(funcInfo, "viewer");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorPropertiesViewer, void, IActorTransformViewer*>(&type, "SetActorTransformViewer", &Editor::ActorPropertiesViewer::SetActorTransformViewer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IActorTransformViewer*>(funcInfo, "viewer");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorPropertiesViewer, void, IActorAnimationViewer*>(&type, "SetActorAnimationViewer", &Editor::ActorPropertiesViewer::SetActorAnimationViewer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IActorAnimationViewer*>(funcInfo, "viewer");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorPropertiesViewer, void, IActorComponentViewer*>(&type, "AddComponentViewerType", &Editor::ActorPropertiesViewer::AddComponentViewerType, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IActorComponentViewer*>(funcInfo, "viewer");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorPropertiesViewer, void, const Vector<IObject*>>(&type, "SetTargets", &Editor::ActorPropertiesViewer::SetTargets, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vector<IObject*>>(funcInfo, "targets");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorPropertiesViewer, void>(&type, "OnEnabled", &Editor::ActorPropertiesViewer::OnEnabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::ActorPropertiesViewer, void>(&type, "OnDisabled", &Editor::ActorPropertiesViewer::OnDisabled, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::ActorPropertiesViewer, void, float>(&type, "Update", &Editor::ActorPropertiesViewer::Update, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorPropertiesViewer, void>(&type, "Draw", &Editor::ActorPropertiesViewer::Draw, o2::ProtectSection::Protected);
}

void Editor::DefaultActorAnimationViewer::InitializeType(Editor::DefaultActorAnimationViewer* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<Editor::DefaultActorAnimationViewer, void, const Vector<Actor*>&>(&type, "SetTargetActors", &Editor::DefaultActorAnimationViewer::SetTargetActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<Actor*>&>(funcInfo, "actors");
}

void Editor::DefaultActorComponentViewer::InitializeType(Editor::DefaultActorComponentViewer* sample)
{
	TypeInitializer::RegField(&type, "mTargetComponents", (size_t)(char*)(&sample->mTargetComponents) - (size_t)(char*)sample, sample->mTargetComponents, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mUsedPropertyFields", (size_t)(char*)(&sample->mUsedPropertyFields) - (size_t)(char*)sample, sample->mUsedPropertyFields, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::DefaultActorComponentViewer, void, const Vector<Component*>&>(&type, "SetTargetComponents", &Editor::DefaultActorComponentViewer::SetTargetComponents, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<Component*>&>(funcInfo, "components");
	funcInfo = TypeInitializer::RegFunction<Editor::DefaultActorComponentViewer, const Type*>(&type, "GetComponentType", &Editor::DefaultActorComponentViewer::GetComponentType, o2::ProtectSection::Public);
}

void Editor::DefaultActorHeaderViewer::InitializeType(Editor::DefaultActorHeaderViewer* sample)
{
	TypeInitializer::RegField(&type, "mDataView", (size_t)(char*)(&sample->mDataView) - (size_t)(char*)sample, sample->mDataView, o2::ProtectSection::Public);
	auto funcInfo = TypeInitializer::RegFunction<Editor::DefaultActorHeaderViewer, void, const Vector<Actor*>&>(&type, "SetTargetActors", &Editor::DefaultActorHeaderViewer::SetTargetActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<Actor*>&>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::DefaultActorHeaderViewer, UIWidget*>(&type, "GetWidget", &Editor::DefaultActorHeaderViewer::GetWidget, o2::ProtectSection::Public);
}

void Editor::DefaultActorTransformViewer::InitializeType(Editor::DefaultActorTransformViewer* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<Editor::DefaultActorTransformViewer, void, const Vector<Actor*>&>(&type, "SetTargetActors", &Editor::DefaultActorTransformViewer::SetTargetActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<Actor*>&>(funcInfo, "actors");
}

void Editor::IActorAnimationViewer::InitializeType(Editor::IActorAnimationViewer* sample)
{
	TypeInitializer::RegField(&type, "mDataView", (size_t)(char*)(&sample->mDataView) - (size_t)(char*)sample, sample->mDataView, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::IActorAnimationViewer, void, const Vector<Actor*>&>(&type, "SetTargetActors", &Editor::IActorAnimationViewer::SetTargetActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<Actor*>&>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::IActorAnimationViewer, UIWidget*>(&type, "GetWidget", &Editor::IActorAnimationViewer::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IActorAnimationViewer, void>(&type, "Expand", &Editor::IActorAnimationViewer::Expand, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IActorAnimationViewer, void>(&type, "Collapse", &Editor::IActorAnimationViewer::Collapse, o2::ProtectSection::Public);
}

void Editor::IActorComponentViewer::InitializeType(Editor::IActorComponentViewer* sample)
{
	TypeInitializer::RegField(&type, "mDataView", (size_t)(char*)(&sample->mDataView) - (size_t)(char*)sample, sample->mDataView, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::IActorComponentViewer, void, const Vector<Component*>&>(&type, "SetTargetComponents", &Editor::IActorComponentViewer::SetTargetComponents, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<Component*>&>(funcInfo, "components");
	funcInfo = TypeInitializer::RegFunction<Editor::IActorComponentViewer, const Type*>(&type, "GetComponentType", &Editor::IActorComponentViewer::GetComponentType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IActorComponentViewer, UIWidget*>(&type, "GetWidget", &Editor::IActorComponentViewer::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IActorComponentViewer, void>(&type, "Expand", &Editor::IActorComponentViewer::Expand, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IActorComponentViewer, void>(&type, "Collapse", &Editor::IActorComponentViewer::Collapse, o2::ProtectSection::Public);
}

void Editor::IActorHeaderViewer::InitializeType(Editor::IActorHeaderViewer* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<Editor::IActorHeaderViewer, void, const Vector<Actor*>&>(&type, "SetTargetActors", &Editor::IActorHeaderViewer::SetTargetActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<Actor*>&>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::IActorHeaderViewer, UIWidget*>(&type, "GetWidget", &Editor::IActorHeaderViewer::GetWidget, o2::ProtectSection::Public);
}

void Editor::IActorTransformViewer::InitializeType(Editor::IActorTransformViewer* sample)
{
	TypeInitializer::RegField(&type, "mDataView", (size_t)(char*)(&sample->mDataView) - (size_t)(char*)sample, sample->mDataView, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::IActorTransformViewer, void, const Vector<Actor*>&>(&type, "SetTargetActors", &Editor::IActorTransformViewer::SetTargetActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<Actor*>&>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::IActorTransformViewer, UIWidget*>(&type, "GetWidget", &Editor::IActorTransformViewer::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IActorTransformViewer, void>(&type, "Expand", &Editor::IActorTransformViewer::Expand, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IActorTransformViewer, void>(&type, "Collapse", &Editor::IActorTransformViewer::Collapse, o2::ProtectSection::Public);
}

void Editor::ActorProperty::InitializeType(Editor::ActorProperty* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::ActorProperty::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, void>(&type, "Update", &Editor::ActorProperty::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, UIWidget*>(&type, "GetWidget", &Editor::ActorProperty::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, Actor*>(&type, "GetCommonValue", &Editor::ActorProperty::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, bool>(&type, "IsValuesDifferent", &Editor::ActorProperty::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, const Type*>(&type, "GetFieldType", &Editor::ActorProperty::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, void, Actor*>(&type, "SetValue", &Editor::ActorProperty::SetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, void>(&type, "InitializeWidget", &Editor::ActorProperty::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::ActorProperty::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::ActorProperty::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::ActorProperty::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::ActorProperty, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::ActorProperty::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::IAssetProperty::InitializeType(Editor::IAssetProperty* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<Editor::IAssetProperty, void, AssetId>(&type, "SetAssetId", &Editor::IAssetProperty::SetAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
}

void Editor::BooleanProperty::InitializeType(Editor::BooleanProperty* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mToggle", (size_t)(char*)(&sample->mToggle) - (size_t)(char*)sample, sample->mToggle, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::BooleanProperty, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::BooleanProperty::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::BooleanProperty, void>(&type, "Update", &Editor::BooleanProperty::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::BooleanProperty, UIWidget*>(&type, "GetWidget", &Editor::BooleanProperty::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::BooleanProperty, bool>(&type, "GetCommonValue", &Editor::BooleanProperty::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::BooleanProperty, bool>(&type, "IsValuesDifferent", &Editor::BooleanProperty::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::BooleanProperty, const Type*>(&type, "GetFieldType", &Editor::BooleanProperty::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::BooleanProperty, void>(&type, "InitializeWidget", &Editor::BooleanProperty::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::BooleanProperty, void, bool>(&type, "OnEdited", &Editor::BooleanProperty::OnEdited, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
}

void Editor::ComponentProperty::InitializeType(Editor::ComponentProperty* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mComponentType", (size_t)(char*)(&sample->mComponentType) - (size_t)(char*)sample, sample->mComponentType, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::ComponentProperty::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, void>(&type, "Update", &Editor::ComponentProperty::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, UIWidget*>(&type, "GetWidget", &Editor::ComponentProperty::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, Component*>(&type, "GetCommonValue", &Editor::ComponentProperty::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, bool>(&type, "IsValuesDifferent", &Editor::ComponentProperty::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, const Type*>(&type, "GetFieldType", &Editor::ComponentProperty::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, void, const Type*>(&type, "SpecializeType", &Editor::ComponentProperty::SpecializeType, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Type*>(funcInfo, "type");
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, const Type*>(&type, "GetSpecializedType", &Editor::ComponentProperty::GetSpecializedType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, void, Component*>(&type, "SetValue", &Editor::ComponentProperty::SetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Component*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, void>(&type, "InitializeWidget", &Editor::ComponentProperty::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::ComponentProperty::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::ComponentProperty::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::ComponentProperty::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::ComponentProperty, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::ComponentProperty::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::FloatProperty::InitializeType(Editor::FloatProperty* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEditBox", (size_t)(char*)(&sample->mEditBox) - (size_t)(char*)sample, sample->mEditBox, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::FloatProperty, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::FloatProperty::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::FloatProperty, void>(&type, "Update", &Editor::FloatProperty::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FloatProperty, UIWidget*>(&type, "GetWidget", &Editor::FloatProperty::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FloatProperty, float>(&type, "GetCommonValue", &Editor::FloatProperty::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FloatProperty, bool>(&type, "IsValuesDifferent", &Editor::FloatProperty::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FloatProperty, const Type*>(&type, "GetFieldType", &Editor::FloatProperty::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::FloatProperty, void>(&type, "InitializeWidget", &Editor::FloatProperty::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::FloatProperty, void, const WString&>(&type, "OnEdited", &Editor::FloatProperty::OnEdited, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "data");
}

void Editor::IntegerProperty::InitializeType(Editor::IntegerProperty* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEditBox", (size_t)(char*)(&sample->mEditBox) - (size_t)(char*)sample, sample->mEditBox, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::IntegerProperty, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::IntegerProperty::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::IntegerProperty, void>(&type, "Update", &Editor::IntegerProperty::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IntegerProperty, UIWidget*>(&type, "GetWidget", &Editor::IntegerProperty::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IntegerProperty, int>(&type, "GetCommonValue", &Editor::IntegerProperty::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IntegerProperty, bool>(&type, "IsValuesDifferent", &Editor::IntegerProperty::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IntegerProperty, const Type*>(&type, "GetFieldType", &Editor::IntegerProperty::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IntegerProperty, void>(&type, "InitializeWidget", &Editor::IntegerProperty::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::IntegerProperty, void, const WString&>(&type, "OnEdited", &Editor::IntegerProperty::OnEdited, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "data");
}

void Editor::IPropertyField::InitializeType(Editor::IPropertyField* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<Editor::IPropertyField, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::IPropertyField::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::IPropertyField, void>(&type, "Update", &Editor::IPropertyField::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IPropertyField, UIWidget*>(&type, "GetWidget", &Editor::IPropertyField::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IPropertyField, const Type*>(&type, "GetFieldType", &Editor::IPropertyField::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::IPropertyField, void, const Type*>(&type, "SpecializeType", &Editor::IPropertyField::SpecializeType, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Type*>(funcInfo, "type");
}

void Editor::LayerProperty::InitializeType(Editor::LayerProperty* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDropDown", (size_t)(char*)(&sample->mDropDown) - (size_t)(char*)sample, sample->mDropDown, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mUpdatingValue", (size_t)(char*)(&sample->mUpdatingValue) - (size_t)(char*)sample, sample->mUpdatingValue, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::LayerProperty, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::LayerProperty::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::LayerProperty, void>(&type, "Update", &Editor::LayerProperty::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::LayerProperty, UIWidget*>(&type, "GetWidget", &Editor::LayerProperty::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::LayerProperty, Scene::Layer*>(&type, "GetCommonValue", &Editor::LayerProperty::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::LayerProperty, bool>(&type, "IsValuesDifferent", &Editor::LayerProperty::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::LayerProperty, const Type*>(&type, "GetFieldType", &Editor::LayerProperty::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::LayerProperty, void>(&type, "InitializeWidget", &Editor::LayerProperty::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::LayerProperty, void, const WString&>(&type, "SelectLayer", &Editor::LayerProperty::SelectLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "name");
}

void Editor::StringProperty::InitializeType(Editor::StringProperty* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEditBox", (size_t)(char*)(&sample->mEditBox) - (size_t)(char*)sample, sample->mEditBox, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::StringProperty, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::StringProperty::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::StringProperty, void>(&type, "Update", &Editor::StringProperty::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::StringProperty, UIWidget*>(&type, "GetWidget", &Editor::StringProperty::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::StringProperty, String>(&type, "GetCommonValue", &Editor::StringProperty::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::StringProperty, bool>(&type, "IsValuesDifferent", &Editor::StringProperty::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::StringProperty, const Type*>(&type, "GetFieldType", &Editor::StringProperty::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::StringProperty, void>(&type, "InitializeWidget", &Editor::StringProperty::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::StringProperty, void, const WString&>(&type, "OnEdited", &Editor::StringProperty::OnEdited, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "data");
}

void Editor::TagsProperty::InitializeType(Editor::TagsProperty* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEditBox", (size_t)(char*)(&sample->mEditBox) - (size_t)(char*)sample, sample->mEditBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTagsContext", (size_t)(char*)(&sample->mTagsContext) - (size_t)(char*)sample, sample->mTagsContext, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPushingTag", (size_t)(char*)(&sample->mPushingTag) - (size_t)(char*)sample, sample->mPushingTag, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::TagsProperty::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, void>(&type, "Update", &Editor::TagsProperty::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, UIWidget*>(&type, "GetWidget", &Editor::TagsProperty::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, const TagGroup&>(&type, "GetCommonValue", &Editor::TagsProperty::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, bool>(&type, "IsValuesDifferent", &Editor::TagsProperty::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, const Type*>(&type, "GetFieldType", &Editor::TagsProperty::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, void>(&type, "InitializeWidget", &Editor::TagsProperty::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, void, const WString&>(&type, "UpdateContextData", &Editor::TagsProperty::UpdateContextData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "filter");
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, void, const WString&>(&type, "OnEditBoxChanged", &Editor::TagsProperty::OnEditBoxChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, void, const WString&>(&type, "OnEditBoxChangeCompleted", &Editor::TagsProperty::OnEditBoxChangeCompleted, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, void, const WString&>(&type, "SetTags", &Editor::TagsProperty::SetTags, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "text");
	funcInfo = TypeInitializer::RegFunction<Editor::TagsProperty, void, String>(&type, "PushTag", &Editor::TagsProperty::PushTag, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<String>(funcInfo, "name");
}

void Editor::WStringProperty::InitializeType(Editor::WStringProperty* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEditBox", (size_t)(char*)(&sample->mEditBox) - (size_t)(char*)sample, sample->mEditBox, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::WStringProperty, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::WStringProperty::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::WStringProperty, void>(&type, "Update", &Editor::WStringProperty::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::WStringProperty, UIWidget*>(&type, "GetWidget", &Editor::WStringProperty::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::WStringProperty, WString>(&type, "GetCommonValue", &Editor::WStringProperty::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::WStringProperty, bool>(&type, "IsValuesDifferent", &Editor::WStringProperty::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::WStringProperty, const Type*>(&type, "GetFieldType", &Editor::WStringProperty::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::WStringProperty, void>(&type, "InitializeWidget", &Editor::WStringProperty::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::WStringProperty, void, const WString&>(&type, "OnEdited", &Editor::WStringProperty::OnEdited, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "data");
}

void Editor::SceneEditScreen::InitializeType(Editor::SceneEditScreen* sample)
{
	TypeInitializer::RegField(&type, "onSelectionChanged", (size_t)(char*)(&sample->onSelectionChanged) - (size_t)(char*)sample, sample->onSelectionChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mRectangle", (size_t)(char*)(&sample->mRectangle) - (size_t)(char*)sample, sample->mRectangle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewCamera", (size_t)(char*)(&sample->mViewCamera) - (size_t)(char*)sample, sample->mViewCamera, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewCameraTargetScale", (size_t)(char*)(&sample->mViewCameraTargetScale) - (size_t)(char*)sample, sample->mViewCameraTargetScale, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewCameraScaleSence", (size_t)(char*)(&sample->mViewCameraScaleSence) - (size_t)(char*)sample, sample->mViewCameraScaleSence, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewCameraScaleElasticyCoef", (size_t)(char*)(&sample->mViewCameraScaleElasticyCoef) - (size_t)(char*)sample, sample->mViewCameraScaleElasticyCoef, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewCameraTargetPos", (size_t)(char*)(&sample->mViewCameraTargetPos) - (size_t)(char*)sample, sample->mViewCameraTargetPos, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewCameraVelocity", (size_t)(char*)(&sample->mViewCameraVelocity) - (size_t)(char*)sample, sample->mViewCameraVelocity, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewCameraPosElasticyCoef", (size_t)(char*)(&sample->mViewCameraPosElasticyCoef) - (size_t)(char*)sample, sample->mViewCameraPosElasticyCoef, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewCameraVelocityDampingCoef", (size_t)(char*)(&sample->mViewCameraVelocityDampingCoef) - (size_t)(char*)sample, sample->mViewCameraVelocityDampingCoef, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewCameraMinScale", (size_t)(char*)(&sample->mViewCameraMinScale) - (size_t)(char*)sample, sample->mViewCameraMinScale, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewCameraMaxScale", (size_t)(char*)(&sample->mViewCameraMaxScale) - (size_t)(char*)sample, sample->mViewCameraMaxScale, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackColor", (size_t)(char*)(&sample->mBackColor) - (size_t)(char*)sample, sample->mBackColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGridColor", (size_t)(char*)(&sample->mGridColor) - (size_t)(char*)sample, sample->mGridColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedActorColor", (size_t)(char*)(&sample->mSelectedActorColor) - (size_t)(char*)sample, sample->mSelectedActorColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMultiSelectedActorColor", (size_t)(char*)(&sample->mMultiSelectedActorColor) - (size_t)(char*)sample, sample->mMultiSelectedActorColor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mActorMinimalSelectionSize", (size_t)(char*)(&sample->mActorMinimalSelectionSize) - (size_t)(char*)sample, sample->mActorMinimalSelectionSize, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSceneToScreenTransform", (size_t)(char*)(&sample->mSceneToScreenTransform) - (size_t)(char*)sample, sample->mSceneToScreenTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mScreenToSceneTransform", (size_t)(char*)(&sample->mScreenToSceneTransform) - (size_t)(char*)sample, sample->mScreenToSceneTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRenderTarget", (size_t)(char*)(&sample->mRenderTarget) - (size_t)(char*)sample, sample->mRenderTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRenderTargetSprite", (size_t)(char*)(&sample->mRenderTargetSprite) - (size_t)(char*)sample, sample->mRenderTargetSprite, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNeedRedraw", (size_t)(char*)(&sample->mNeedRedraw) - (size_t)(char*)sample, sample->mNeedRedraw, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDrawDepth", (size_t)(char*)(&sample->mDrawDepth) - (size_t)(char*)sample, sample->mDrawDepth, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mActorsTree", (size_t)(char*)(&sample->mActorsTree) - (size_t)(char*)sample, sample->mActorsTree, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedActors", (size_t)(char*)(&sample->mSelectedActors) - (size_t)(char*)sample, sample->mSelectedActors, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTopSelectedActors", (size_t)(char*)(&sample->mTopSelectedActors) - (size_t)(char*)sample, sample->mTopSelectedActors, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedFromThis", (size_t)(char*)(&sample->mSelectedFromThis) - (size_t)(char*)sample, sample->mSelectedFromThis, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTools", (size_t)(char*)(&sample->mTools) - (size_t)(char*)sample, sample->mTools, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEnabledTool", (size_t)(char*)(&sample->mEnabledTool) - (size_t)(char*)sample, sample->mEnabledTool, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragHandles", (size_t)(char*)(&sample->mDragHandles) - (size_t)(char*)sample, sample->mDragHandles, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressedHandles", (size_t)(char*)(&sample->mPressedHandles) - (size_t)(char*)sample, sample->mPressedHandles, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightButtonPressedHandle", (size_t)(char*)(&sample->mRightButtonPressedHandle) - (size_t)(char*)sample, sample->mRightButtonPressedHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMiddleButtonPressedHandle", (size_t)(char*)(&sample->mMiddleButtonPressedHandle) - (size_t)(char*)sample, sample->mMiddleButtonPressedHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mUnderCursorHandles", (size_t)(char*)(&sample->mUnderCursorHandles) - (size_t)(char*)sample, sample->mUnderCursorHandles, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastUnderCursorHandles", (size_t)(char*)(&sample->mLastUnderCursorHandles) - (size_t)(char*)sample, sample->mLastUnderCursorHandles, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "Draw", &Editor::SceneEditScreen::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, float>(&type, "Update", &Editor::SceneEditScreen::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, bool>(&type, "IsScrollable", &Editor::SceneEditScreen::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, Vec2F, const Vec2F&>(&type, "ScreenToScenePoint", &Editor::SceneEditScreen::ScreenToScenePoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, Vec2F, const Vec2F&>(&type, "SceneToScreenPoint", &Editor::SceneEditScreen::SceneToScreenPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, Vec2F, const Vec2F&>(&type, "ScreenToSceneVector", &Editor::SceneEditScreen::ScreenToSceneVector, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, Vec2F, const Vec2F&>(&type, "SceneToScreenVector", &Editor::SceneEditScreen::SceneToScreenVector, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const RectF&>(&type, "SetRect", &Editor::SceneEditScreen::SetRect, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "rect");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, RectF>(&type, "GetRect", &Editor::SceneEditScreen::GetRect, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, Actor*, const Color4&>(&type, "DrawActorSelection", &Editor::SceneEditScreen::DrawActorSelection, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, ActorsVec, bool>(&type, "SelectActors", &Editor::SceneEditScreen::SelectActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<ActorsVec>(funcInfo, "actors");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "additive");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, Actor*, bool>(&type, "SelectActor", &Editor::SceneEditScreen::SelectActor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "additive");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "SelectAllActors", &Editor::SceneEditScreen::SelectAllActors, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "ClearSelection", &Editor::SceneEditScreen::ClearSelection, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, const ActorsVec&>(&type, "GetSelectedActors", &Editor::SceneEditScreen::GetSelectedActors, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, const ActorsVec&>(&type, "GetTopSelectedActors", &Editor::SceneEditScreen::GetTopSelectedActors, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, const Color4&>(&type, "GetSingleActorSelectionColor", &Editor::SceneEditScreen::GetSingleActorSelectionColor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, const Color4&>(&type, "GetManyActorsSelectionColor", &Editor::SceneEditScreen::GetManyActorsSelectionColor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, bool, const Vec2F&>(&type, "IsUnderPoint", &Editor::SceneEditScreen::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, Type*>(&type, "InitializeTools", &Editor::SceneEditScreen::InitializeTools, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Type*>(funcInfo, "toolType");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "UpdateHandles", &Editor::SceneEditScreen::UpdateHandles, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, bool, const Input::Cursor&>(&type, "IsHandleWorking", &Editor::SceneEditScreen::IsHandleWorking, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::SceneEditScreen::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorReleased", &Editor::SceneEditScreen::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &Editor::SceneEditScreen::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorStillDown", &Editor::SceneEditScreen::OnCursorStillDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorMoved", &Editor::SceneEditScreen::OnCursorMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::SceneEditScreen::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::SceneEditScreen::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorRightMousePressed", &Editor::SceneEditScreen::OnCursorRightMousePressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorRightMouseStayDown", &Editor::SceneEditScreen::OnCursorRightMouseStayDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &Editor::SceneEditScreen::OnCursorRightMouseReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorMiddleMousePressed", &Editor::SceneEditScreen::OnCursorMiddleMousePressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorMiddleMouseStayDown", &Editor::SceneEditScreen::OnCursorMiddleMouseStayDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Cursor&>(&type, "OnCursorMiddleMouseReleased", &Editor::SceneEditScreen::OnCursorMiddleMouseReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, float>(&type, "OnScrolled", &Editor::SceneEditScreen::OnScrolled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::SceneEditScreen::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Key&>(&type, "OnKeyReleased", &Editor::SceneEditScreen::OnKeyReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, const Input::Key&>(&type, "OnKeyStayDown", &Editor::SceneEditScreen::OnKeyStayDown, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "OnActorsSelectedFromThis", &Editor::SceneEditScreen::OnActorsSelectedFromThis, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, float>(&type, "UpdateCamera", &Editor::SceneEditScreen::UpdateCamera, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "RedrawScene", &Editor::SceneEditScreen::RedrawScene, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "DrawGrid", &Editor::SceneEditScreen::DrawGrid, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "DrawActors", &Editor::SceneEditScreen::DrawActors, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "DrawSelection", &Editor::SceneEditScreen::DrawSelection, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "UpdateSceneScreenTransforms", &Editor::SceneEditScreen::UpdateSceneScreenTransforms, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "BindActorsTree", &Editor::SceneEditScreen::BindActorsTree, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, Vector<Actor*>>(&type, "OnTreeSelectionChanged", &Editor::SceneEditScreen::OnTreeSelectionChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Vector<Actor*>>(funcInfo, "selectedObjects");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "UpdateTopSelectedActors", &Editor::SceneEditScreen::UpdateTopSelectedActors, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, int, Actor*>(&type, "GetActorIdx", &Editor::SceneEditScreen::GetActorIdx, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, ActorsVec>(&type, "OnSceneChanged", &Editor::SceneEditScreen::OnSceneChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<ActorsVec>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void>(&type, "ClearSelectionWithoutAction", &Editor::SceneEditScreen::ClearSelectionWithoutAction, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, ActorsVec, bool>(&type, "SelectActorsWithoutAction", &Editor::SceneEditScreen::SelectActorsWithoutAction, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<ActorsVec>(funcInfo, "actors");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "additive");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditScreen, void, Actor*, bool>(&type, "SelectActorWithoutAction", &Editor::SceneEditScreen::SelectActorWithoutAction, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "additive");
}

void Editor::SceneEditWidget::InitializeType(Editor::SceneEditWidget* sample)
{
	TypeInitializer::RegField(&type, "mSceneEditScreen", (size_t)(char*)(&sample->mSceneEditScreen) - (size_t)(char*)sample, sample->mSceneEditScreen, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::SceneEditWidget, void>(&type, "Draw", &Editor::SceneEditWidget::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditWidget, void, float>(&type, "Update", &Editor::SceneEditWidget::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::SceneEditWidget, void, bool>(&type, "UpdateLayout", &Editor::SceneEditWidget::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
}

void Editor::SceneWindow::InitializeType(Editor::SceneWindow* sample)
{
	TypeInitializer::RegField(&type, "mEditWidget", (size_t)(char*)(&sample->mEditWidget) - (size_t)(char*)sample, sample->mEditWidget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLayersView", (size_t)(char*)(&sample->mLayersView) - (size_t)(char*)sample, sample->mLayersView, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGizomsView", (size_t)(char*)(&sample->mGizomsView) - (size_t)(char*)sample, sample->mGizomsView, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::SceneWindow, void>(&type, "InitializeWindow", &Editor::SceneWindow::InitializeWindow, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneWindow, void>(&type, "InitializeLayersView", &Editor::SceneWindow::InitializeLayersView, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::SceneWindow, void>(&type, "PostInitializeWindow", &Editor::SceneWindow::PostInitializeWindow, o2::ProtectSection::Protected);
}

void Editor::UIActorsTree::InitializeType(Editor::UIActorsTree* sample)
{
	TypeInitializer::RegField(&type, "onDraggedActors", (size_t)(char*)(&sample->onDraggedActors) - (size_t)(char*)sample, sample->onDraggedActors, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onItemClick", (size_t)(char*)(&sample->onItemClick) - (size_t)(char*)sample, sample->onItemClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onItemDblClick", (size_t)(char*)(&sample->onItemDblClick) - (size_t)(char*)sample, sample->onItemDblClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onItemRBClick", (size_t)(char*)(&sample->onItemRBClick) - (size_t)(char*)sample, sample->onItemRBClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onItemsSelectionChanged", (size_t)(char*)(&sample->onItemsSelectionChanged) - (size_t)(char*)sample, sample->onItemsSelectionChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mEnableActorsTogglesGroup", (size_t)(char*)(&sample->mEnableActorsTogglesGroup) - (size_t)(char*)sample, sample->mEnableActorsTogglesGroup, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLockActorsTogglesGroup", (size_t)(char*)(&sample->mLockActorsTogglesGroup) - (size_t)(char*)sample, sample->mLockActorsTogglesGroup, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAttackedToSceneEvents", (size_t)(char*)(&sample->mAttackedToSceneEvents) - (size_t)(char*)sample, sample->mAttackedToSceneEvents, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragActorPropertyField", (size_t)(char*)(&sample->mDragActorPropertyField) - (size_t)(char*)sample, sample->mDragActorPropertyField, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragComponentPropertyField", (size_t)(char*)(&sample->mDragComponentPropertyField) - (size_t)(char*)sample, sample->mDragComponentPropertyField, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void>(&type, "AttachToSceneEvents", &Editor::UIActorsTree::AttachToSceneEvents, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void>(&type, "Draw", &Editor::UIActorsTree::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, float>(&type, "Update", &Editor::UIActorsTree::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void>(&type, "RebuildTree", &Editor::UIActorsTree::RebuildTree, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, Actor*>(&type, "UpdateTreeNode", &Editor::UIActorsTree::UpdateTreeNode, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, UITreeNode*, Actor*>(&type, "GetNode", &Editor::UIActorsTree::GetNode, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void>(&type, "ExpandAll", &Editor::UIActorsTree::ExpandAll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void>(&type, "CollapseAll", &Editor::UIActorsTree::CollapseAll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, const ActorsVec&>(&type, "ManualBeginDraggingActors", &Editor::UIActorsTree::ManualBeginDraggingActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const ActorsVec&>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, const Input::Cursor&>(&type, "ManualUpdateDraggingActors", &Editor::UIActorsTree::ManualUpdateDraggingActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void>(&type, "CompleteManualDraggingActors", &Editor::UIActorsTree::CompleteManualDraggingActors, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void>(&type, "BreakDragging", &Editor::UIActorsTree::BreakDragging, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, ActorsVec>(&type, "GetSelectedActors", &Editor::UIActorsTree::GetSelectedActors, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, const ActorsVec&>(&type, "SetSelectedActors", &Editor::UIActorsTree::SetSelectedActors, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const ActorsVec&>(funcInfo, "actors");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, Actor*>(&type, "SelectActor", &Editor::UIActorsTree::SelectActor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, Actor*>(&type, "SelectAndExpandActor", &Editor::UIActorsTree::SelectAndExpandActor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, Actor*>(&type, "DeselectActor", &Editor::UIActorsTree::DeselectActor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void>(&type, "DeselectAllActors", &Editor::UIActorsTree::DeselectAllActors, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, UITreeNode*, const Vec2F&>(&type, "GetTreeNodeUnderPoint", &Editor::UIActorsTree::GetTreeNodeUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, UITreeNode*>(&type, "GetNodeSample", &Editor::UIActorsTree::GetNodeSample, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, Sprite*>(&type, "GetHoverDrawable", &Editor::UIActorsTree::GetHoverDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, Sprite*>(&type, "GetSelectionDrawable", &Editor::UIActorsTree::GetSelectionDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, const Layout&>(&type, "SetSelectionDrawableLayout", &Editor::UIActorsTree::SetSelectionDrawableLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, Layout>(&type, "GetSelectionDrawableLayout", &Editor::UIActorsTree::GetSelectionDrawableLayout, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, bool>(&type, "IsScrollable", &Editor::UIActorsTree::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, int>(&type, "SetNodesPoolResizeCount", &Editor::UIActorsTree::SetNodesPoolResizeCount, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, int>(&type, "SetSelectionSpritesPoolResizeCount", &Editor::UIActorsTree::SetSelectionSpritesPoolResizeCount, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<int>(funcInfo, "count");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, const Color4&>(&type, "SetSelectedColor", &Editor::UIActorsTree::SetSelectedColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, Color4>(&type, "GetSelectedColor", &Editor::UIActorsTree::GetSelectedColor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, const Color4&>(&type, "SetUnselectedColor", &Editor::UIActorsTree::SetUnselectedColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, Color4>(&type, "GetUnselectedColor", &Editor::UIActorsTree::GetUnselectedColor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, const Color4&>(&type, "SetHoverColor", &Editor::UIActorsTree::SetHoverColor, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "color");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, Color4>(&type, "GetHoverColor", &Editor::UIActorsTree::GetHoverColor, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, bool>(&type, "IsSelectable", &Editor::UIActorsTree::IsSelectable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void>(&type, "Initialize", &Editor::UIActorsTree::Initialize, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, UnknownType*, UnknownType*>(&type, "GetActorsParent", &Editor::UIActorsTree::GetActorsParent, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "obj");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, Vector<UnknownType*>, UnknownType*>(&type, "GetActorsChildren", &Editor::UIActorsTree::GetActorsChildren, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "parentObj");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, UITreeNode*, UnknownType*>(&type, "SetupTreeNodeActor", &Editor::UIActorsTree::SetupTreeNodeActor, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "actorObj");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, Vector<UnknownType*>, UnknownType*, UnknownType*>(&type, "RearrangeActors", &Editor::UIActorsTree::RearrangeActors, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Vector<UnknownType*>>(funcInfo, "objects");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "parentObj");
	TypeInitializer::RegFuncParam<UnknownType*>(funcInfo, "prevObj");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, UITreeNode*, Actor*>(&type, "OnTreeNodeDblClick", &Editor::UIActorsTree::OnTreeNodeDblClick, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, bool>(&type, "EnableActorsGroupPressed", &Editor::UIActorsTree::EnableActorsGroupPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, bool>(&type, "EnableActorsGroupReleased", &Editor::UIActorsTree::EnableActorsGroupReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, bool>(&type, "LockActorsGroupPressed", &Editor::UIActorsTree::LockActorsGroupPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, bool>(&type, "LockActorsGroupReleased", &Editor::UIActorsTree::LockActorsGroupReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, Actor*>(&type, "OnActorCreated", &Editor::UIActorsTree::OnActorCreated, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, Actor*>(&type, "OnActorDestroyed", &Editor::UIActorsTree::OnActorDestroyed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void, const Input::Cursor&>(&type, "UpdateDragging", &Editor::UIActorsTree::UpdateDragging, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::UIActorsTree, void>(&type, "EndDragging", &Editor::UIActorsTree::EndDragging, o2::ProtectSection::Protected);
}

void Editor::TreeWindow::InitializeType(Editor::TreeWindow* sample)
{
	TypeInitializer::RegField(&type, "mListTreeToggle", (size_t)(char*)(&sample->mListTreeToggle) - (size_t)(char*)sample, sample->mListTreeToggle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSearchEditBox", (size_t)(char*)(&sample->mSearchEditBox) - (size_t)(char*)sample, sample->mSearchEditBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mActorsTree", (size_t)(char*)(&sample->mActorsTree) - (size_t)(char*)sample, sample->mActorsTree, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTreeContextMenu", (size_t)(char*)(&sample->mTreeContextMenu) - (size_t)(char*)sample, sample->mTreeContextMenu, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mInSearch", (size_t)(char*)(&sample->mInSearch) - (size_t)(char*)sample, sample->mInSearch, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSearchActors", (size_t)(char*)(&sample->mSearchActors) - (size_t)(char*)sample, sample->mSearchActors, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, UIActorsTree*>(&type, "GetActorsTree", &Editor::TreeWindow::GetActorsTree, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void, Actor*>(&type, "ExpandActorsTreeNode", &Editor::TreeWindow::ExpandActorsTreeNode, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "targetActor");
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "InitializeWindow", &Editor::TreeWindow::InitializeWindow, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "PostInitializeWindow", &Editor::TreeWindow::PostInitializeWindow, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnSearchPressed", &Editor::TreeWindow::OnSearchPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void, bool>(&type, "OnListTreeToggled", &Editor::TreeWindow::OnListTreeToggled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void, const WString&>(&type, "OnSearchEdited", &Editor::TreeWindow::OnSearchEdited, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const WString&>(funcInfo, "searchStr");
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void, Actor*, const String&>(&type, "SearchActorsRecursive", &Editor::TreeWindow::SearchActorsRecursive, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "searchStr");
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void, UITreeNode*, Actor*>(&type, "OnTreeRBPressed", &Editor::TreeWindow::OnTreeRBPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void, Actor*>(&type, "CreateActor", &Editor::TreeWindow::CreateActor, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "newActor");
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextCreateNewPressed", &Editor::TreeWindow::OnContextCreateNewPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextCreateSprite", &Editor::TreeWindow::OnContextCreateSprite, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextCreateButton", &Editor::TreeWindow::OnContextCreateButton, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextCopyPressed", &Editor::TreeWindow::OnContextCopyPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextCutPressed", &Editor::TreeWindow::OnContextCutPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextPastePressed", &Editor::TreeWindow::OnContextPastePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextDeletePressed", &Editor::TreeWindow::OnContextDeletePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextDuplicatePressed", &Editor::TreeWindow::OnContextDuplicatePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextExpandAllPressed", &Editor::TreeWindow::OnContextExpandAllPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextCollapseAllPressed", &Editor::TreeWindow::OnContextCollapseAllPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextLockPressed", &Editor::TreeWindow::OnContextLockPressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void>(&type, "OnContextEnablePressed", &Editor::TreeWindow::OnContextEnablePressed, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void, Actor*>(&type, "OnActorCreated", &Editor::TreeWindow::OnActorCreated, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
	funcInfo = TypeInitializer::RegFunction<Editor::TreeWindow, void, Actor*>(&type, "OnActorDestroyed", &Editor::TreeWindow::OnActorDestroyed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "actor");
}

void Editor::EditorConfig::GlobalConfig::InitializeType(Editor::EditorConfig::GlobalConfig* sample)
{
	TypeInitializer::RegField(&type, "mLastOpenedProjectpath", (size_t)(char*)(&sample->mLastOpenedProjectpath) - (size_t)(char*)sample, sample->mLastOpenedProjectpath, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDefaultLayout", (size_t)(char*)(&sample->mDefaultLayout) - (size_t)(char*)sample, sample->mDefaultLayout, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAvailableLayouts", (size_t)(char*)(&sample->mAvailableLayouts) - (size_t)(char*)sample, sample->mAvailableLayouts, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void Editor::EditorConfig::ProjectConfig::InitializeType(Editor::EditorConfig::ProjectConfig* sample)
{
	TypeInitializer::RegField(&type, "mWindowSize", (size_t)(char*)(&sample->mWindowSize) - (size_t)(char*)sample, sample->mWindowSize, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWindowPosition", (size_t)(char*)(&sample->mWindowPosition) - (size_t)(char*)sample, sample->mWindowPosition, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaximized", (size_t)(char*)(&sample->mMaximized) - (size_t)(char*)sample, sample->mMaximized, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLayout", (size_t)(char*)(&sample->mLayout) - (size_t)(char*)sample, sample->mLayout, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void Editor::DeleteActorsAction::ActorInfo::InitializeType(Editor::DeleteActorsAction::ActorInfo* sample)
{
	TypeInitializer::RegField(&type, "actor", (size_t)(char*)(&sample->actor) - (size_t)(char*)sample, sample->actor, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "parentId", (size_t)(char*)(&sample->parentId) - (size_t)(char*)sample, sample->parentId, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "prevActorId", (size_t)(char*)(&sample->prevActorId) - (size_t)(char*)sample, sample->prevActorId, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "idx", (size_t)(char*)(&sample->idx) - (size_t)(char*)sample, sample->idx, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void Editor::WindowsLayout::WindowDockPlace::InitializeType(Editor::WindowsLayout::WindowDockPlace* sample)
{
	TypeInitializer::RegField(&type, "anchors", (size_t)(char*)(&sample->anchors) - (size_t)(char*)sample, sample->anchors, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "windows", (size_t)(char*)(&sample->windows) - (size_t)(char*)sample, sample->windows, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "childs", (size_t)(char*)(&sample->childs) - (size_t)(char*)sample, sample->childs, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<Editor::WindowsLayout::WindowDockPlace, void, UIWidget*>(&type, "RetrieveLayout", &Editor::WindowsLayout::WindowDockPlace::RetrieveLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
}

void o2::AnimatedValue<RectF>::InitializeType(o2::AnimatedValue<RectF>* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)sample, sample->target, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)sample, sample->targetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)sample, sample->targetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)sample, sample->mKeys, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)sample, sample->mTargetDelegate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)sample, sample->mTargetProperty, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, RectF*>(&type, "SetTarget", &o2::AnimatedValue<RectF>::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<RectF*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, RectF*, const Function<void()>&>(&type, "SetTarget", &o2::AnimatedValue<RectF>::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<RectF*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::AnimatedValue<RectF>::SetTargetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, Setter<RectF>*>(&type, "SetTargetProperty", &o2::AnimatedValue<RectF>::SetTargetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Setter<RectF>*>(funcInfo, "setter");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, RectF>(&type, "GetValue", &o2::AnimatedValue<RectF>::GetValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, RectF, float>(&type, "GetValue", &o2::AnimatedValue<RectF>::GetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, Vector<Key>, float>(&type, "AddKeys", &o2::AnimatedValue<RectF>::AddKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Key>>(funcInfo, "keys");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const Key&>(&type, "AddKey", &o2::AnimatedValue<RectF>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const Key&, float>(&type, "AddKey", &o2::AnimatedValue<RectF>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const Key&, float>(&type, "AddSmoothKey", &o2::AnimatedValue<RectF>::AddSmoothKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, float, const RectF&, float, float, float, float>(&type, "AddKey", &o2::AnimatedValue<RectF>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, float, const RectF&, float>(&type, "AddKey", &o2::AnimatedValue<RectF>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, Key, float>(&type, "GetKey", &o2::AnimatedValue<RectF>::GetKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, bool, float>(&type, "RemoveKey", &o2::AnimatedValue<RectF>::RemoveKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void>(&type, "RemoveAllKeys", &o2::AnimatedValue<RectF>::RemoveAllKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, bool, float>(&type, "ContainsKey", &o2::AnimatedValue<RectF>::ContainsKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, const KeysVec&>(&type, "GetKeys", &o2::AnimatedValue<RectF>::GetKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const KeysVec&>(&type, "SetKeys", &o2::AnimatedValue<RectF>::SetKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, float, float>(&type, "SmoothKey", &o2::AnimatedValue<RectF>::SmoothKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void>(&type, "Evaluate", &o2::AnimatedValue<RectF>::Evaluate, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, RectF, float>(&type, "Evaluate", &o2::AnimatedValue<RectF>::Evaluate, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, KeysVec>(&type, "GetKeysNonContant", &o2::AnimatedValue<RectF>::GetKeysNonContant, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void>(&type, "UpdateApproximation", &o2::AnimatedValue<RectF>::UpdateApproximation, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, const DataNode&>(&type, "OnDeserialized", &o2::AnimatedValue<RectF>::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, void*>(&type, "SetTargetVoid", &o2::AnimatedValue<RectF>::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::AnimatedValue<RectF>::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, void*>(&type, "SetTargetPropertyVoid", &o2::AnimatedValue<RectF>::SetTargetPropertyVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::AnimatedValue<RectF>::RegInAnimatable, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<RectF>, void>(&type, "InitializeProperties", &o2::AnimatedValue<RectF>::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::AnimatedValue<RectF>::Key::InitializeType(o2::AnimatedValue<RectF>::Key* sample)
{
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)sample, sample->curvePrevCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)sample, sample->curvePrevCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)sample, sample->curveNextCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)sample, sample->curveNextCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)sample, sample->mCurveApproxValues, o2::ProtectSection::Public);
}

void o2::AnimatedValue<Color4>::InitializeType(o2::AnimatedValue<Color4>* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)sample, sample->target, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)sample, sample->targetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)sample, sample->targetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)sample, sample->mKeys, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)sample, sample->mTargetDelegate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)sample, sample->mTargetProperty, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, Color4*>(&type, "SetTarget", &o2::AnimatedValue<Color4>::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Color4*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, Color4*, const Function<void()>&>(&type, "SetTarget", &o2::AnimatedValue<Color4>::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Color4*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::AnimatedValue<Color4>::SetTargetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, Setter<Color4>*>(&type, "SetTargetProperty", &o2::AnimatedValue<Color4>::SetTargetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Setter<Color4>*>(funcInfo, "setter");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, Color4>(&type, "GetValue", &o2::AnimatedValue<Color4>::GetValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, Color4, float>(&type, "GetValue", &o2::AnimatedValue<Color4>::GetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, Vector<Key>, float>(&type, "AddKeys", &o2::AnimatedValue<Color4>::AddKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Key>>(funcInfo, "keys");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const Key&>(&type, "AddKey", &o2::AnimatedValue<Color4>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const Key&, float>(&type, "AddKey", &o2::AnimatedValue<Color4>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const Key&, float>(&type, "AddSmoothKey", &o2::AnimatedValue<Color4>::AddSmoothKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, float, const Color4&, float, float, float, float>(&type, "AddKey", &o2::AnimatedValue<Color4>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, float, const Color4&, float>(&type, "AddKey", &o2::AnimatedValue<Color4>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const Color4&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, Key, float>(&type, "GetKey", &o2::AnimatedValue<Color4>::GetKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, bool, float>(&type, "RemoveKey", &o2::AnimatedValue<Color4>::RemoveKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void>(&type, "RemoveAllKeys", &o2::AnimatedValue<Color4>::RemoveAllKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, bool, float>(&type, "ContainsKey", &o2::AnimatedValue<Color4>::ContainsKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, const KeysVec&>(&type, "GetKeys", &o2::AnimatedValue<Color4>::GetKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const KeysVec&>(&type, "SetKeys", &o2::AnimatedValue<Color4>::SetKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, float, float>(&type, "SmoothKey", &o2::AnimatedValue<Color4>::SmoothKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void>(&type, "Evaluate", &o2::AnimatedValue<Color4>::Evaluate, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, Color4, float>(&type, "Evaluate", &o2::AnimatedValue<Color4>::Evaluate, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, KeysVec>(&type, "GetKeysNonContant", &o2::AnimatedValue<Color4>::GetKeysNonContant, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void>(&type, "UpdateApproximation", &o2::AnimatedValue<Color4>::UpdateApproximation, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, const DataNode&>(&type, "OnDeserialized", &o2::AnimatedValue<Color4>::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, void*>(&type, "SetTargetVoid", &o2::AnimatedValue<Color4>::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::AnimatedValue<Color4>::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, void*>(&type, "SetTargetPropertyVoid", &o2::AnimatedValue<Color4>::SetTargetPropertyVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::AnimatedValue<Color4>::RegInAnimatable, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<Color4>, void>(&type, "InitializeProperties", &o2::AnimatedValue<Color4>::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::AnimatedValue<Color4>::Key::InitializeType(o2::AnimatedValue<Color4>::Key* sample)
{
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)sample, sample->curvePrevCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)sample, sample->curvePrevCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)sample, sample->curveNextCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)sample, sample->curveNextCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)sample, sample->mCurveApproxValues, o2::ProtectSection::Public);
}

void o2::AnimatedValue<bool>::InitializeType(o2::AnimatedValue<bool>* sample)
{
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)sample, sample->target, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)sample, sample->targetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)sample, sample->targetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)sample, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)sample, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)sample, sample->mKeys, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)sample, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)sample, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)sample, sample->mTargetDelegate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)sample, sample->mTargetProperty, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, bool*>(&type, "SetTarget", &o2::AnimatedValue<bool>::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool*>(funcInfo, "value");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, bool*, const Function<void()>&>(&type, "SetTarget", &o2::AnimatedValue<bool>::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool*>(funcInfo, "value");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const Function<void()>&>(&type, "SetTargetDelegate", &o2::AnimatedValue<bool>::SetTargetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, Setter<bool>*>(&type, "SetTargetProperty", &o2::AnimatedValue<bool>::SetTargetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Setter<bool>*>(funcInfo, "setter");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, bool>(&type, "GetValue", &o2::AnimatedValue<bool>::GetValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, bool, float>(&type, "GetValue", &o2::AnimatedValue<bool>::GetValue, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, Vector<Key>, float>(&type, "AddKeys", &o2::AnimatedValue<bool>::AddKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Vector<Key>>(funcInfo, "keys");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const Key&>(&type, "AddKey", &o2::AnimatedValue<bool>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const Key&, float>(&type, "AddKey", &o2::AnimatedValue<bool>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const Key&, float>(&type, "AddSmoothKey", &o2::AnimatedValue<bool>::AddSmoothKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Key&>(funcInfo, "key");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, float, const bool&, float, float, float, float>(&type, "AddKey", &o2::AnimatedValue<bool>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const bool&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "leftCoefPosition");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoef");
	TypeInitializer::RegFuncParam<float>(funcInfo, "rightCoefPosition");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, float, const bool&, float>(&type, "AddKey", &o2::AnimatedValue<bool>::AddKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<const bool&>(funcInfo, "value");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, Key, float>(&type, "GetKey", &o2::AnimatedValue<bool>::GetKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, bool, float>(&type, "RemoveKey", &o2::AnimatedValue<bool>::RemoveKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void>(&type, "RemoveAllKeys", &o2::AnimatedValue<bool>::RemoveAllKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, bool, float>(&type, "ContainsKey", &o2::AnimatedValue<bool>::ContainsKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, const KeysVec&>(&type, "GetKeys", &o2::AnimatedValue<bool>::GetKeys, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const KeysVec&>(&type, "SetKeys", &o2::AnimatedValue<bool>::SetKeys, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const KeysVec&>(funcInfo, "keys");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, float, float>(&type, "SmoothKey", &o2::AnimatedValue<bool>::SmoothKey, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	TypeInitializer::RegFuncParam<float>(funcInfo, "smooth");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void>(&type, "Evaluate", &o2::AnimatedValue<bool>::Evaluate, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, bool, float>(&type, "Evaluate", &o2::AnimatedValue<bool>::Evaluate, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, KeysVec>(&type, "GetKeysNonContant", &o2::AnimatedValue<bool>::GetKeysNonContant, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void>(&type, "UpdateApproximation", &o2::AnimatedValue<bool>::UpdateApproximation, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, const DataNode&>(&type, "OnDeserialized", &o2::AnimatedValue<bool>::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, void*>(&type, "SetTargetVoid", &o2::AnimatedValue<bool>::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, void*, const Function<void()>&>(&type, "SetTargetVoid", &o2::AnimatedValue<bool>::SetTargetVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<const Function<void()>&>(funcInfo, "changeEvent");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, void*>(&type, "SetTargetPropertyVoid", &o2::AnimatedValue<bool>::SetTargetPropertyVoid, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<void*>(funcInfo, "target");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void, AnimationState*, const String&>(&type, "RegInAnimatable", &o2::AnimatedValue<bool>::RegInAnimatable, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<AnimationState*>(funcInfo, "state");
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::AnimatedValue<bool>, void>(&type, "InitializeProperties", &o2::AnimatedValue<bool>::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::AnimatedValue<bool>::Key::InitializeType(o2::AnimatedValue<bool>::Key* sample)
{
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)sample, sample->position, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)sample, sample->value, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)sample, sample->curvePrevCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)sample, sample->curvePrevCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)sample, sample->curveNextCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)sample, sample->curveNextCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)sample, sample->mCurveApproxValues, o2::ProtectSection::Public);
}

void Editor::AssetProperty<ActorAsset>::InitializeType(Editor::AssetProperty<ActorAsset>* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::AssetProperty<ActorAsset>::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, void>(&type, "Update", &Editor::AssetProperty<ActorAsset>::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, UIWidget*>(&type, "GetWidget", &Editor::AssetProperty<ActorAsset>::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, const ActorAsset&>(&type, "GetCommonValue", &Editor::AssetProperty<ActorAsset>::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, bool>(&type, "IsValuesDifferent", &Editor::AssetProperty<ActorAsset>::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, const Type*>(&type, "GetFieldType", &Editor::AssetProperty<ActorAsset>::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, void, AssetId>(&type, "SetAssetId", &Editor::AssetProperty<ActorAsset>::SetAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, void>(&type, "InitializeWidget", &Editor::AssetProperty<ActorAsset>::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::AssetProperty<ActorAsset>::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::AssetProperty<ActorAsset>::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::AssetProperty<ActorAsset>::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ActorAsset>, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::AssetProperty<ActorAsset>::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::AssetProperty<AnimationAsset>::InitializeType(Editor::AssetProperty<AnimationAsset>* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::AssetProperty<AnimationAsset>::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, void>(&type, "Update", &Editor::AssetProperty<AnimationAsset>::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, UIWidget*>(&type, "GetWidget", &Editor::AssetProperty<AnimationAsset>::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, const AnimationAsset&>(&type, "GetCommonValue", &Editor::AssetProperty<AnimationAsset>::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, bool>(&type, "IsValuesDifferent", &Editor::AssetProperty<AnimationAsset>::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, const Type*>(&type, "GetFieldType", &Editor::AssetProperty<AnimationAsset>::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, void, AssetId>(&type, "SetAssetId", &Editor::AssetProperty<AnimationAsset>::SetAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, void>(&type, "InitializeWidget", &Editor::AssetProperty<AnimationAsset>::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::AssetProperty<AnimationAsset>::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::AssetProperty<AnimationAsset>::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::AssetProperty<AnimationAsset>::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AnimationAsset>, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::AssetProperty<AnimationAsset>::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::AssetProperty<AtlasAsset>::InitializeType(Editor::AssetProperty<AtlasAsset>* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::AssetProperty<AtlasAsset>::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, void>(&type, "Update", &Editor::AssetProperty<AtlasAsset>::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, UIWidget*>(&type, "GetWidget", &Editor::AssetProperty<AtlasAsset>::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, const AtlasAsset&>(&type, "GetCommonValue", &Editor::AssetProperty<AtlasAsset>::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, bool>(&type, "IsValuesDifferent", &Editor::AssetProperty<AtlasAsset>::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, const Type*>(&type, "GetFieldType", &Editor::AssetProperty<AtlasAsset>::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, void, AssetId>(&type, "SetAssetId", &Editor::AssetProperty<AtlasAsset>::SetAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, void>(&type, "InitializeWidget", &Editor::AssetProperty<AtlasAsset>::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::AssetProperty<AtlasAsset>::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::AssetProperty<AtlasAsset>::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::AssetProperty<AtlasAsset>::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<AtlasAsset>, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::AssetProperty<AtlasAsset>::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::AssetProperty<BinaryAsset>::InitializeType(Editor::AssetProperty<BinaryAsset>* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::AssetProperty<BinaryAsset>::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, void>(&type, "Update", &Editor::AssetProperty<BinaryAsset>::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, UIWidget*>(&type, "GetWidget", &Editor::AssetProperty<BinaryAsset>::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, const BinaryAsset&>(&type, "GetCommonValue", &Editor::AssetProperty<BinaryAsset>::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, bool>(&type, "IsValuesDifferent", &Editor::AssetProperty<BinaryAsset>::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, const Type*>(&type, "GetFieldType", &Editor::AssetProperty<BinaryAsset>::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, void, AssetId>(&type, "SetAssetId", &Editor::AssetProperty<BinaryAsset>::SetAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, void>(&type, "InitializeWidget", &Editor::AssetProperty<BinaryAsset>::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::AssetProperty<BinaryAsset>::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::AssetProperty<BinaryAsset>::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::AssetProperty<BinaryAsset>::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BinaryAsset>, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::AssetProperty<BinaryAsset>::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::AssetProperty<BitmapFontAsset>::InitializeType(Editor::AssetProperty<BitmapFontAsset>* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::AssetProperty<BitmapFontAsset>::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, void>(&type, "Update", &Editor::AssetProperty<BitmapFontAsset>::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, UIWidget*>(&type, "GetWidget", &Editor::AssetProperty<BitmapFontAsset>::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, const BitmapFontAsset&>(&type, "GetCommonValue", &Editor::AssetProperty<BitmapFontAsset>::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, bool>(&type, "IsValuesDifferent", &Editor::AssetProperty<BitmapFontAsset>::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, const Type*>(&type, "GetFieldType", &Editor::AssetProperty<BitmapFontAsset>::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, void, AssetId>(&type, "SetAssetId", &Editor::AssetProperty<BitmapFontAsset>::SetAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, void>(&type, "InitializeWidget", &Editor::AssetProperty<BitmapFontAsset>::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::AssetProperty<BitmapFontAsset>::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::AssetProperty<BitmapFontAsset>::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::AssetProperty<BitmapFontAsset>::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<BitmapFontAsset>, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::AssetProperty<BitmapFontAsset>::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::AssetProperty<DataAsset>::InitializeType(Editor::AssetProperty<DataAsset>* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::AssetProperty<DataAsset>::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, void>(&type, "Update", &Editor::AssetProperty<DataAsset>::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, UIWidget*>(&type, "GetWidget", &Editor::AssetProperty<DataAsset>::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, const DataAsset&>(&type, "GetCommonValue", &Editor::AssetProperty<DataAsset>::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, bool>(&type, "IsValuesDifferent", &Editor::AssetProperty<DataAsset>::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, const Type*>(&type, "GetFieldType", &Editor::AssetProperty<DataAsset>::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, void, AssetId>(&type, "SetAssetId", &Editor::AssetProperty<DataAsset>::SetAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, void>(&type, "InitializeWidget", &Editor::AssetProperty<DataAsset>::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::AssetProperty<DataAsset>::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::AssetProperty<DataAsset>::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::AssetProperty<DataAsset>::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<DataAsset>, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::AssetProperty<DataAsset>::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::AssetProperty<FolderAsset>::InitializeType(Editor::AssetProperty<FolderAsset>* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::AssetProperty<FolderAsset>::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, void>(&type, "Update", &Editor::AssetProperty<FolderAsset>::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, UIWidget*>(&type, "GetWidget", &Editor::AssetProperty<FolderAsset>::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, const FolderAsset&>(&type, "GetCommonValue", &Editor::AssetProperty<FolderAsset>::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, bool>(&type, "IsValuesDifferent", &Editor::AssetProperty<FolderAsset>::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, const Type*>(&type, "GetFieldType", &Editor::AssetProperty<FolderAsset>::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, void, AssetId>(&type, "SetAssetId", &Editor::AssetProperty<FolderAsset>::SetAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, void>(&type, "InitializeWidget", &Editor::AssetProperty<FolderAsset>::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::AssetProperty<FolderAsset>::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::AssetProperty<FolderAsset>::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::AssetProperty<FolderAsset>::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<FolderAsset>, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::AssetProperty<FolderAsset>::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::AssetProperty<ImageAsset>::InitializeType(Editor::AssetProperty<ImageAsset>* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::AssetProperty<ImageAsset>::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, void>(&type, "Update", &Editor::AssetProperty<ImageAsset>::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, UIWidget*>(&type, "GetWidget", &Editor::AssetProperty<ImageAsset>::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, const ImageAsset&>(&type, "GetCommonValue", &Editor::AssetProperty<ImageAsset>::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, bool>(&type, "IsValuesDifferent", &Editor::AssetProperty<ImageAsset>::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, const Type*>(&type, "GetFieldType", &Editor::AssetProperty<ImageAsset>::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, void, AssetId>(&type, "SetAssetId", &Editor::AssetProperty<ImageAsset>::SetAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, void>(&type, "InitializeWidget", &Editor::AssetProperty<ImageAsset>::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::AssetProperty<ImageAsset>::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::AssetProperty<ImageAsset>::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::AssetProperty<ImageAsset>::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<ImageAsset>, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::AssetProperty<ImageAsset>::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}

void Editor::AssetProperty<VectorFontAsset>::InitializeType(Editor::AssetProperty<VectorFontAsset>* sample)
{
	TypeInitializer::RegField(&type, "mAssignFunc", (size_t)(char*)(&sample->mAssignFunc) - (size_t)(char*)sample, sample->mAssignFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGetFunc", (size_t)(char*)(&sample->mGetFunc) - (size_t)(char*)sample, sample->mGetFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesPointers", (size_t)(char*)(&sample->mValuesPointers) - (size_t)(char*)sample, sample->mValuesPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCommonValue", (size_t)(char*)(&sample->mCommonValue) - (size_t)(char*)sample, sample->mCommonValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mValuesDifferent", (size_t)(char*)(&sample->mValuesDifferent) - (size_t)(char*)sample, sample->mValuesDifferent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBox", (size_t)(char*)(&sample->mBox) - (size_t)(char*)sample, sample->mBox, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNameText", (size_t)(char*)(&sample->mNameText) - (size_t)(char*)sample, sample->mNameText, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, void, const Vector<void*>&, bool>(&type, "Setup", &Editor::AssetProperty<VectorFontAsset>::Setup, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vector<void*>&>(funcInfo, "targets");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "isProperty");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, void>(&type, "Update", &Editor::AssetProperty<VectorFontAsset>::Update, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, UIWidget*>(&type, "GetWidget", &Editor::AssetProperty<VectorFontAsset>::GetWidget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, const VectorFontAsset&>(&type, "GetCommonValue", &Editor::AssetProperty<VectorFontAsset>::GetCommonValue, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, bool>(&type, "IsValuesDifferent", &Editor::AssetProperty<VectorFontAsset>::IsValuesDifferent, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, const Type*>(&type, "GetFieldType", &Editor::AssetProperty<VectorFontAsset>::GetFieldType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, void, AssetId>(&type, "SetAssetId", &Editor::AssetProperty<VectorFontAsset>::SetAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<AssetId>(funcInfo, "id");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, void>(&type, "InitializeWidget", &Editor::AssetProperty<VectorFontAsset>::InitializeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, void, const Input::Cursor&>(&type, "OnCursorEnter", &Editor::AssetProperty<VectorFontAsset>::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, void, const Input::Cursor&>(&type, "OnCursorExit", &Editor::AssetProperty<VectorFontAsset>::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, void, const Input::Cursor&>(&type, "OnCursorPressed", &Editor::AssetProperty<VectorFontAsset>::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<Editor::AssetProperty<VectorFontAsset>, void, const Input::Key&>(&type, "OnKeyPressed", &Editor::AssetProperty<VectorFontAsset>::OnKeyPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Key&>(funcInfo, "key");
}


// Registering all types
void RegReflectionTypes()
{
	// Initialize enums
	o2::Reflection::InitializeEnum<o2::Loop>( { { (int)o2::Loop::None, "None" }, { (int)o2::Loop::Repeat, "Repeat" }, { (int)o2::Loop::PingPong, "PingPong" } });
	o2::Reflection::InitializeEnum<o2::BaseCorner>( { { (int)o2::BaseCorner::Left, "Left" }, { (int)o2::BaseCorner::Right, "Right" }, { (int)o2::BaseCorner::Top, "Top" }, { (int)o2::BaseCorner::Bottom, "Bottom" }, { (int)o2::BaseCorner::Center, "Center" }, { (int)o2::BaseCorner::LeftBottom, "LeftBottom" }, { (int)o2::BaseCorner::LeftTop, "LeftTop" }, { (int)o2::BaseCorner::RightBottom, "RightBottom" }, { (int)o2::BaseCorner::RightTop, "RightTop" } });
	o2::Reflection::InitializeEnum<o2::CursorType>( { { (int)o2::CursorType::AppStarting, "AppStarting" }, { (int)o2::CursorType::Arrow, "Arrow" }, { (int)o2::CursorType::Cross, "Cross" }, { (int)o2::CursorType::Hand, "Hand" }, { (int)o2::CursorType::Help, "Help" }, { (int)o2::CursorType::IBeam, "IBeam" }, { (int)o2::CursorType::Icon, "Icon" }, { (int)o2::CursorType::No, "No" }, { (int)o2::CursorType::SizeAll, "SizeAll" }, { (int)o2::CursorType::SizeNeSw, "SizeNeSw" }, { (int)o2::CursorType::SizeNS, "SizeNS" }, { (int)o2::CursorType::SizeNwSe, "SizeNwSe" }, { (int)o2::CursorType::SizeWE, "SizeWE" }, { (int)o2::CursorType::UpArrow, "UpArrow" }, { (int)o2::CursorType::Wait, "Wait" } });
	o2::Reflection::InitializeEnum<o2::Side>( { { (int)o2::Side::Left, "Left" }, { (int)o2::Side::Bottom, "Bottom" }, { (int)o2::Side::Right, "Right" }, { (int)o2::Side::Top, "Top" }, { (int)o2::Side::None, "None" } });
	o2::Reflection::InitializeEnum<o2::Corner>( { { (int)o2::Corner::LeftTop, "LeftTop" }, { (int)o2::Corner::RightTop, "RightTop" }, { (int)o2::Corner::RightBottom, "RightBottom" }, { (int)o2::Corner::LeftBottom, "LeftBottom" } });
	o2::Reflection::InitializeEnum<o2::TwoDirection>( { { (int)o2::TwoDirection::Horizontal, "Horizontal" }, { (int)o2::TwoDirection::Vertical, "Vertical" } });
	o2::Reflection::InitializeEnum<o2::SpriteMode>( { { (int)o2::SpriteMode::Default, "Default" }, { (int)o2::SpriteMode::Sliced, "Sliced" }, { (int)o2::SpriteMode::FillLeftToRight, "FillLeftToRight" }, { (int)o2::SpriteMode::FillRightToLeft, "FillRightToLeft" }, { (int)o2::SpriteMode::FillUpToDown, "FillUpToDown" }, { (int)o2::SpriteMode::FillDownToUp, "FillDownToUp" }, { (int)o2::SpriteMode::Fill360CW, "Fill360CW" }, { (int)o2::SpriteMode::Fill360CCW, "Fill360CCW" } });
	o2::Reflection::InitializeEnum<o2::VerAlign>( { { (int)o2::VerAlign::Top, "Top" }, { (int)o2::VerAlign::Middle, "Middle" }, { (int)o2::VerAlign::Bottom, "Bottom" }, { (int)o2::VerAlign::Both, "Both" } });
	o2::Reflection::InitializeEnum<o2::HorAlign>( { { (int)o2::HorAlign::Left, "Left" }, { (int)o2::HorAlign::Middle, "Middle" }, { (int)o2::HorAlign::Right, "Right" }, { (int)o2::HorAlign::Both, "Both" } });
	o2::Reflection::InitializeEnum<o2::ProtectSection>( { { (int)o2::ProtectSection::Public, "Public" }, { (int)o2::ProtectSection::Private, "Private" }, { (int)o2::ProtectSection::Protected, "Protected" } });
	o2::Reflection::InitializeEnum<o2::FileType>( { { (int)o2::FileType::File, "File" }, { (int)o2::FileType::Image, "Image" }, { (int)o2::FileType::Config, "Config" }, { (int)o2::FileType::Atlas, "Atlas" } });
	o2::Reflection::InitializeEnum<o2::ProjectConfig::Platform>( { { (int)o2::ProjectConfig::Platform::Windows, "Windows" }, { (int)o2::ProjectConfig::Platform::MacOSX, "MacOSX" }, { (int)o2::ProjectConfig::Platform::iOS, "iOS" }, { (int)o2::ProjectConfig::Platform::Android, "Android" } });
	o2::Reflection::InitializeEnum<o2::Texture::Format>( { { (int)o2::Texture::Format::Default, "Default" }, { (int)o2::Texture::Format::R8G8B8A8, "R8G8B8A8" }, { (int)o2::Texture::Format::R8G8B8, "R8G8B8" } });
	o2::Reflection::InitializeEnum<o2::Texture::Usage>( { { (int)o2::Texture::Usage::Default, "Default" }, { (int)o2::Texture::Usage::RenderTarget, "RenderTarget" } });
	o2::Reflection::InitializeEnum<o2::UIHorizontalProgress::Orientation>( { { (int)o2::UIHorizontalProgress::Orientation::Right, "Right" }, { (int)o2::UIHorizontalProgress::Orientation::Left, "Left" } });
	o2::Reflection::InitializeEnum<o2::UILabel::HorOverflow>( { { (int)o2::UILabel::HorOverflow::Cut, "Cut" }, { (int)o2::UILabel::HorOverflow::Dots, "Dots" }, { (int)o2::UILabel::HorOverflow::Expand, "Expand" }, { (int)o2::UILabel::HorOverflow::Wrap, "Wrap" }, { (int)o2::UILabel::HorOverflow::None, "None" } });
	o2::Reflection::InitializeEnum<o2::UILabel::VerOverflow>( { { (int)o2::UILabel::VerOverflow::Cut, "Cut" }, { (int)o2::UILabel::VerOverflow::None, "None" }, { (int)o2::UILabel::VerOverflow::Expand, "Expand" } });
	o2::Reflection::InitializeEnum<o2::UIToggleGroup::Type>( { { (int)o2::UIToggleGroup::Type::OnlySingleTrue, "OnlySingleTrue" }, { (int)o2::UIToggleGroup::Type::VerOneClick, "VerOneClick" }, { (int)o2::UIToggleGroup::Type::HorOneClick, "HorOneClick" } });
	o2::Reflection::InitializeEnum<o2::UITree::RearrangeType>( { { (int)o2::UITree::RearrangeType::Disabled, "Disabled" }, { (int)o2::UITree::RearrangeType::Enabled, "Enabled" }, { (int)o2::UITree::RearrangeType::OnlyReparent, "OnlyReparent" } });
	o2::Reflection::InitializeEnum<o2::UIVerticalProgress::Orientation>( { { (int)o2::UIVerticalProgress::Orientation::Down, "Down" }, { (int)o2::UIVerticalProgress::Orientation::Up, "Up" } });
	o2::Reflection::InitializeEnum<o2::Bitmap::Format>( { { (int)o2::Bitmap::Format::Default, "Default" }, { (int)o2::Bitmap::Format::R8G8B8A8, "R8G8B8A8" } });
	o2::Reflection::InitializeEnum<o2::Bitmap::ImageType>( { { (int)o2::Bitmap::ImageType::Auto, "Auto" }, { (int)o2::Bitmap::ImageType::Png, "Png" } });
	o2::Reflection::InitializeEnum<o2::DataNode::Format>( { { (int)o2::DataNode::Format::Xml, "Xml" }, { (int)o2::DataNode::Format::JSON, "JSON" }, { (int)o2::DataNode::Format::Binary, "Binary" } });
	o2::Reflection::InitializeEnum<Editor::UIAssetsIconsScrollArea::DragState>( { { (int)Editor::UIAssetsIconsScrollArea::DragState::Off, "Off" }, { (int)Editor::UIAssetsIconsScrollArea::DragState::Regular, "Regular" }, { (int)Editor::UIAssetsIconsScrollArea::DragState::Scene, "Scene" }, { (int)Editor::UIAssetsIconsScrollArea::DragState::Tree, "Tree" }, { (int)Editor::UIAssetsIconsScrollArea::DragState::AssetField, "AssetField" }, { (int)Editor::UIAssetsIconsScrollArea::DragState::ActorField, "ActorField" }, { (int)Editor::UIAssetsIconsScrollArea::DragState::ComponentField, "ComponentField" } });
	o2::Reflection::InitializeEnum<Editor::LogWindow::LogMessage::Type>( { { (int)Editor::LogWindow::LogMessage::Type::Regular, "Regular" }, { (int)Editor::LogWindow::LogMessage::Type::Warning, "Warning" }, { (int)Editor::LogWindow::LogMessage::Type::Error, "Error" } });

	// Initialize types
	o2::Reflection::InitializeType<o2::Animatable>("o2::Animatable");
	o2::Reflection::InitializeType<o2::AnimatedValue<float>>("o2::AnimatedValue<float>");
	o2::Reflection::InitializeType<o2::IAnimatedValue>("o2::IAnimatedValue");
	o2::Reflection::InitializeType<o2::AnimatedValue<Vec2F>>("o2::AnimatedValue<Vec2F>");
	o2::Reflection::InitializeType<o2::Animation>("o2::Animation");
	o2::Reflection::InitializeType<o2::AnimationMask>("o2::AnimationMask");
	o2::Reflection::InitializeType<o2::AnimationState>("o2::AnimationState");
	o2::Reflection::InitializeType<o2::IAnimation>("o2::IAnimation");
	o2::Reflection::InitializeType<o2::ActorAsset>("o2::ActorAsset");
	o2::Reflection::InitializeType<o2::AnimationAsset>("o2::AnimationAsset");
	o2::Reflection::InitializeType<o2::Asset>("o2::Asset");
	o2::Reflection::InitializeType<o2::AssetInfo>("o2::AssetInfo");
	o2::Reflection::InitializeType<o2::AtlasAsset>("o2::AtlasAsset");
	o2::Reflection::InitializeType<o2::BinaryAsset>("o2::BinaryAsset");
	o2::Reflection::InitializeType<o2::BitmapFontAsset>("o2::BitmapFontAsset");
	o2::Reflection::InitializeType<o2::DataAsset>("o2::DataAsset");
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
	o2::Reflection::InitializeType<o2::Tag>("o2::Tag");
	o2::Reflection::InitializeType<o2::TagGroup>("o2::TagGroup");
	o2::Reflection::InitializeType<o2::EditorTestComponent>("o2::EditorTestComponent");
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
	o2::Reflection::InitializeType<o2::ActorAsset::MetaInfo>("o2::ActorAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::AnimationAsset::MetaInfo>("o2::AnimationAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::Asset::IMetaInfo>("o2::Asset::IMetaInfo");
	o2::Reflection::InitializeType<o2::AssetTree::AssetNode>("o2::AssetTree::AssetNode");
	o2::Reflection::InitializeType<o2::AtlasAsset::PlatformMeta>("o2::AtlasAsset::PlatformMeta");
	o2::Reflection::InitializeType<o2::AtlasAsset::MetaInfo>("o2::AtlasAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::AtlasAsset::Page>("o2::AtlasAsset::Page");
	o2::Reflection::InitializeType<o2::BinaryAsset::MetaInfo>("o2::BinaryAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::BitmapFontAsset::MetaInfo>("o2::BitmapFontAsset::MetaInfo");
	o2::Reflection::InitializeType<o2::DataAsset::MetaInfo>("o2::DataAsset::MetaInfo");
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
	o2::Reflection::InitializeType<Editor::UIAssetsIconsScrollArea>("Editor::UIAssetsIconsScrollArea");
	o2::Reflection::InitializeType<Editor::AssetsWindow>("Editor::AssetsWindow");
	o2::Reflection::InitializeType<Editor::UIAssetsFoldersTree>("Editor::UIAssetsFoldersTree");
	o2::Reflection::InitializeType<Editor::UIAssetIcon>("Editor::UIAssetIcon");
	o2::Reflection::InitializeType<Editor::EditorConfig>("Editor::EditorConfig");
	o2::Reflection::InitializeType<Editor::EditorActorsTransformAction>("Editor::EditorActorsTransformAction");
	o2::Reflection::InitializeType<Editor::CreateActorsAction>("Editor::CreateActorsAction");
	o2::Reflection::InitializeType<Editor::DeleteActorsAction>("Editor::DeleteActorsAction");
	o2::Reflection::InitializeType<Editor::EnableAction>("Editor::EnableAction");
	o2::Reflection::InitializeType<Editor::IAction>("Editor::IAction");
	o2::Reflection::InitializeType<Editor::LockAction>("Editor::LockAction");
	o2::Reflection::InitializeType<Editor::ReparentActorsAction>("Editor::ReparentActorsAction");
	o2::Reflection::InitializeType<Editor::SelectionAction>("Editor::SelectionAction");
	o2::Reflection::InitializeType<Editor::FrameTool>("Editor::FrameTool");
	o2::Reflection::InitializeType<Editor::IEditTool>("Editor::IEditTool");
	o2::Reflection::InitializeType<Editor::MoveTool>("Editor::MoveTool");
	o2::Reflection::InitializeType<Editor::RotateTool>("Editor::RotateTool");
	o2::Reflection::InitializeType<Editor::ScaleTool>("Editor::ScaleTool");
	o2::Reflection::InitializeType<Editor::SelectionTool>("Editor::SelectionTool");
	o2::Reflection::InitializeType<Editor::IEditorWindow>("Editor::IEditorWindow");
	o2::Reflection::InitializeType<Editor::UIDockableWindow>("Editor::UIDockableWindow");
	o2::Reflection::InitializeType<Editor::UIDockWindowPlace>("Editor::UIDockWindowPlace");
	o2::Reflection::InitializeType<Editor::WindowsLayout>("Editor::WindowsLayout");
	o2::Reflection::InitializeType<Editor::LogWindow>("Editor::LogWindow");
	o2::Reflection::InitializeType<Editor::IObjectPropertiesViewer>("Editor::IObjectPropertiesViewer");
	o2::Reflection::InitializeType<Editor::PropertiesWindow>("Editor::PropertiesWindow");
	o2::Reflection::InitializeType<Editor::ActorPropertiesViewer>("Editor::ActorPropertiesViewer");
	o2::Reflection::InitializeType<Editor::DefaultActorAnimationViewer>("Editor::DefaultActorAnimationViewer");
	o2::Reflection::InitializeType<Editor::DefaultActorComponentViewer>("Editor::DefaultActorComponentViewer");
	o2::Reflection::InitializeType<Editor::DefaultActorHeaderViewer>("Editor::DefaultActorHeaderViewer");
	o2::Reflection::InitializeType<Editor::DefaultActorTransformViewer>("Editor::DefaultActorTransformViewer");
	o2::Reflection::InitializeType<Editor::IActorAnimationViewer>("Editor::IActorAnimationViewer");
	o2::Reflection::InitializeType<Editor::IActorComponentViewer>("Editor::IActorComponentViewer");
	o2::Reflection::InitializeType<Editor::IActorHeaderViewer>("Editor::IActorHeaderViewer");
	o2::Reflection::InitializeType<Editor::IActorTransformViewer>("Editor::IActorTransformViewer");
	o2::Reflection::InitializeType<Editor::ActorProperty>("Editor::ActorProperty");
	o2::Reflection::InitializeType<Editor::IAssetProperty>("Editor::IAssetProperty");
	o2::Reflection::InitializeType<Editor::BooleanProperty>("Editor::BooleanProperty");
	o2::Reflection::InitializeType<Editor::ComponentProperty>("Editor::ComponentProperty");
	o2::Reflection::InitializeType<Editor::FloatProperty>("Editor::FloatProperty");
	o2::Reflection::InitializeType<Editor::IntegerProperty>("Editor::IntegerProperty");
	o2::Reflection::InitializeType<Editor::IPropertyField>("Editor::IPropertyField");
	o2::Reflection::InitializeType<Editor::LayerProperty>("Editor::LayerProperty");
	o2::Reflection::InitializeType<Editor::StringProperty>("Editor::StringProperty");
	o2::Reflection::InitializeType<Editor::TagsProperty>("Editor::TagsProperty");
	o2::Reflection::InitializeType<Editor::WStringProperty>("Editor::WStringProperty");
	o2::Reflection::InitializeType<Editor::SceneEditScreen>("Editor::SceneEditScreen");
	o2::Reflection::InitializeType<Editor::SceneEditWidget>("Editor::SceneEditWidget");
	o2::Reflection::InitializeType<Editor::SceneWindow>("Editor::SceneWindow");
	o2::Reflection::InitializeType<Editor::UIActorsTree>("Editor::UIActorsTree");
	o2::Reflection::InitializeType<Editor::TreeWindow>("Editor::TreeWindow");
	o2::Reflection::InitializeType<Editor::EditorConfig::GlobalConfig>("Editor::EditorConfig::GlobalConfig");
	o2::Reflection::InitializeType<Editor::EditorConfig::ProjectConfig>("Editor::EditorConfig::ProjectConfig");
	o2::Reflection::InitializeType<Editor::DeleteActorsAction::ActorInfo>("Editor::DeleteActorsAction::ActorInfo");
	o2::Reflection::InitializeType<Editor::WindowsLayout::WindowDockPlace>("Editor::WindowsLayout::WindowDockPlace");
	o2::Reflection::InitializeType<o2::AnimatedValue<RectF>>("o2::AnimatedValue<RectF>");
	o2::Reflection::InitializeType<o2::AnimatedValue<RectF>::Key>("o2::AnimatedValue<RectF>::Key");
	o2::Reflection::InitializeType<o2::AnimatedValue<Color4>>("o2::AnimatedValue<Color4>");
	o2::Reflection::InitializeType<o2::AnimatedValue<Color4>::Key>("o2::AnimatedValue<Color4>::Key");
	o2::Reflection::InitializeType<o2::AnimatedValue<bool>>("o2::AnimatedValue<bool>");
	o2::Reflection::InitializeType<o2::AnimatedValue<bool>::Key>("o2::AnimatedValue<bool>::Key");
	o2::Reflection::InitializeType<Editor::AssetProperty<ActorAsset>>("Editor::AssetProperty<ActorAsset>");
	o2::Reflection::InitializeType<Editor::AssetProperty<AnimationAsset>>("Editor::AssetProperty<AnimationAsset>");
	o2::Reflection::InitializeType<Editor::AssetProperty<AtlasAsset>>("Editor::AssetProperty<AtlasAsset>");
	o2::Reflection::InitializeType<Editor::AssetProperty<BinaryAsset>>("Editor::AssetProperty<BinaryAsset>");
	o2::Reflection::InitializeType<Editor::AssetProperty<BitmapFontAsset>>("Editor::AssetProperty<BitmapFontAsset>");
	o2::Reflection::InitializeType<Editor::AssetProperty<DataAsset>>("Editor::AssetProperty<DataAsset>");
	o2::Reflection::InitializeType<Editor::AssetProperty<FolderAsset>>("Editor::AssetProperty<FolderAsset>");
	o2::Reflection::InitializeType<Editor::AssetProperty<ImageAsset>>("Editor::AssetProperty<ImageAsset>");
	o2::Reflection::InitializeType<Editor::AssetProperty<VectorFontAsset>>("Editor::AssetProperty<VectorFontAsset>");

	// Resolve inheritance
	TypeInitializer::AddBaseType<o2::Animatable, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::IAnimation, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::IAnimatedValue, o2::IAnimation>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<float>, o2::IAnimatedValue>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<Vec2F>, o2::IAnimatedValue>();
	TypeInitializer::AddBaseType<o2::Animation, o2::IAnimation>();
	TypeInitializer::AddBaseType<o2::AnimationMask, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::AnimationState, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::Asset, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::ActorAsset, o2::Asset>();
	TypeInitializer::AddBaseType<o2::AnimationAsset, o2::Asset>();
	TypeInitializer::AddBaseType<o2::AssetInfo, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::AtlasAsset, o2::Asset>();
	TypeInitializer::AddBaseType<o2::BinaryAsset, o2::Asset>();
	TypeInitializer::AddBaseType<o2::BitmapFontAsset, o2::Asset>();
	TypeInitializer::AddBaseType<o2::DataAsset, o2::Asset>();
	TypeInitializer::AddBaseType<o2::FolderAsset, o2::Asset>();
	TypeInitializer::AddBaseType<o2::ImageAsset, o2::Asset>();
	TypeInitializer::AddBaseType<o2::VectorFontAsset, o2::Asset>();
	TypeInitializer::AddBaseType<o2::AtlasAssetConverter, o2::IAssetConverter>();
	TypeInitializer::AddBaseType<o2::FolderAssetConverter, o2::IAssetConverter>();
	TypeInitializer::AddBaseType<o2::ImageAssetConverter, o2::IAssetConverter>();
	TypeInitializer::AddBaseType<o2::StdAssetConverter, o2::IAssetConverter>();
	TypeInitializer::AddBaseType<o2::ProjectConfig, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::Transform, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::Camera, o2::Transform>();
	TypeInitializer::AddBaseType<o2::IRectDrawable, o2::Transform>();
	TypeInitializer::AddBaseType<o2::Sprite, o2::IRectDrawable>();
	TypeInitializer::AddBaseType<o2::Text, o2::IRectDrawable>();
	TypeInitializer::AddBaseType<o2::VectorFont::Effect, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::FontStrokeEffect, o2::VectorFont::Effect>();
	TypeInitializer::AddBaseType<o2::FontGradientEffect, o2::VectorFont::Effect>();
	TypeInitializer::AddBaseType<o2::FontColorEffect, o2::VectorFont::Effect>();
	TypeInitializer::AddBaseType<o2::FontShadowEffect, o2::VectorFont::Effect>();
	TypeInitializer::AddBaseType<o2::Actor, o2::Animatable>();
	TypeInitializer::AddBaseType<o2::ActorTransform, o2::Transform>();
	TypeInitializer::AddBaseType<o2::Component, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::DrawableComponent, o2::Component>();
	TypeInitializer::AddBaseType<o2::Tag, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::TagGroup, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::EditorTestComponent, o2::Component>();
	TypeInitializer::AddBaseType<o2::ImageComponent, o2::DrawableComponent>();
	TypeInitializer::AddBaseType<o2::UIWidget, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::UIButton, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIContextMenuItem, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIScrollArea, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIContextMenu, o2::UIScrollArea>();
	TypeInitializer::AddBaseType<o2::UICustomDropDown, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UICustomList, o2::UIScrollArea>();
	TypeInitializer::AddBaseType<o2::UIDropDown, o2::UICustomDropDown>();
	TypeInitializer::AddBaseType<o2::UIEditBox, o2::UIScrollArea>();
	TypeInitializer::AddBaseType<o2::UIGridLayout, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIHorizontalLayout, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIHorizontalProgress, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIHorizontalScrollBar, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UILabel, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIList, o2::UICustomList>();
	TypeInitializer::AddBaseType<o2::UILongList, o2::UIScrollArea>();
	TypeInitializer::AddBaseType<o2::UIMenuPanel, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIToggle, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UITreeNode, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UITree, o2::UIScrollArea>();
	TypeInitializer::AddBaseType<o2::UIVerticalLayout, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIVerticalProgress, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIVerticalScrollBar, o2::UIWidget>();
	TypeInitializer::AddBaseType<o2::UIWidgetLayer, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::UIWidgetLayout, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::UIWidgetState, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::UIWindow, o2::UIScrollArea>();
	TypeInitializer::AddBaseType<o2::ShortcutKeys, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::TimeStamp, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::Curve, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::Layout, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<Vec2F>::Key, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::Animation::AnimatedValueDef, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::Asset::IMetaInfo, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::ActorAsset::MetaInfo, o2::Asset::IMetaInfo>();
	TypeInitializer::AddBaseType<o2::AnimationAsset::MetaInfo, o2::Asset::IMetaInfo>();
	TypeInitializer::AddBaseType<o2::AssetTree::AssetNode, o2::AssetInfo>();
	TypeInitializer::AddBaseType<o2::AtlasAsset::PlatformMeta, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::AtlasAsset::MetaInfo, o2::Asset::IMetaInfo>();
	TypeInitializer::AddBaseType<o2::AtlasAsset::Page, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::BinaryAsset::MetaInfo, o2::Asset::IMetaInfo>();
	TypeInitializer::AddBaseType<o2::BitmapFontAsset::MetaInfo, o2::Asset::IMetaInfo>();
	TypeInitializer::AddBaseType<o2::DataAsset::MetaInfo, o2::Asset::IMetaInfo>();
	TypeInitializer::AddBaseType<o2::FolderAsset::MetaInfo, o2::Asset::IMetaInfo>();
	TypeInitializer::AddBaseType<o2::ImageAsset::PlatformMeta, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::ImageAsset::MetaInfo, o2::Asset::IMetaInfo>();
	TypeInitializer::AddBaseType<o2::VectorFontAsset::MetaInfo, o2::Asset::IMetaInfo>();
	TypeInitializer::AddBaseType<o2::AtlasAssetConverter::Image, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::Scene::Layer, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::UIContextMenu::Item, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::UIMenuPanel::Item, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::Curve::Key, o2::ISerializable>();
	TypeInitializer::AddBaseType<Editor::UIAssetsIconsScrollArea, o2::UIScrollArea>();
	TypeInitializer::AddBaseType<Editor::AssetsWindow, Editor::IEditorWindow>();
	TypeInitializer::AddBaseType<Editor::UIAssetsFoldersTree, o2::UIWidget>();
	TypeInitializer::AddBaseType<Editor::UIAssetIcon, o2::UIWidget>();
	TypeInitializer::AddBaseType<Editor::EditorConfig, o2::ISerializable>();
	TypeInitializer::AddBaseType<Editor::IAction, o2::ISerializable>();
	TypeInitializer::AddBaseType<Editor::EditorActorsTransformAction, Editor::IAction>();
	TypeInitializer::AddBaseType<Editor::CreateActorsAction, Editor::IAction>();
	TypeInitializer::AddBaseType<Editor::DeleteActorsAction, Editor::IAction>();
	TypeInitializer::AddBaseType<Editor::EnableAction, Editor::IAction>();
	TypeInitializer::AddBaseType<Editor::LockAction, Editor::IAction>();
	TypeInitializer::AddBaseType<Editor::ReparentActorsAction, Editor::IAction>();
	TypeInitializer::AddBaseType<Editor::SelectionAction, Editor::IAction>();
	TypeInitializer::AddBaseType<Editor::SelectionTool, Editor::IEditTool>();
	TypeInitializer::AddBaseType<Editor::FrameTool, Editor::SelectionTool>();
	TypeInitializer::AddBaseType<Editor::MoveTool, Editor::SelectionTool>();
	TypeInitializer::AddBaseType<Editor::RotateTool, Editor::SelectionTool>();
	TypeInitializer::AddBaseType<Editor::ScaleTool, Editor::SelectionTool>();
	TypeInitializer::AddBaseType<Editor::UIDockableWindow, o2::UIWindow>();
	TypeInitializer::AddBaseType<Editor::UIDockWindowPlace, o2::UIWidget>();
	TypeInitializer::AddBaseType<Editor::WindowsLayout, o2::ISerializable>();
	TypeInitializer::AddBaseType<Editor::LogWindow, Editor::IEditorWindow>();
	TypeInitializer::AddBaseType<Editor::PropertiesWindow, Editor::IEditorWindow>();
	TypeInitializer::AddBaseType<Editor::ActorPropertiesViewer, Editor::IObjectPropertiesViewer>();
	TypeInitializer::AddBaseType<Editor::DefaultActorAnimationViewer, Editor::IActorAnimationViewer>();
	TypeInitializer::AddBaseType<Editor::DefaultActorComponentViewer, Editor::IActorComponentViewer>();
	TypeInitializer::AddBaseType<Editor::DefaultActorHeaderViewer, Editor::IActorHeaderViewer>();
	TypeInitializer::AddBaseType<Editor::DefaultActorTransformViewer, Editor::IActorTransformViewer>();
	TypeInitializer::AddBaseType<Editor::ActorProperty, Editor::IPropertyField>();
	TypeInitializer::AddBaseType<Editor::IAssetProperty, Editor::IPropertyField>();
	TypeInitializer::AddBaseType<Editor::BooleanProperty, Editor::IPropertyField>();
	TypeInitializer::AddBaseType<Editor::ComponentProperty, Editor::IPropertyField>();
	TypeInitializer::AddBaseType<Editor::FloatProperty, Editor::IPropertyField>();
	TypeInitializer::AddBaseType<Editor::IntegerProperty, Editor::IPropertyField>();
	TypeInitializer::AddBaseType<Editor::LayerProperty, Editor::IPropertyField>();
	TypeInitializer::AddBaseType<Editor::StringProperty, Editor::IPropertyField>();
	TypeInitializer::AddBaseType<Editor::TagsProperty, Editor::IPropertyField>();
	TypeInitializer::AddBaseType<Editor::WStringProperty, Editor::IPropertyField>();
	TypeInitializer::AddBaseType<Editor::SceneEditWidget, o2::UIWidget>();
	TypeInitializer::AddBaseType<Editor::SceneWindow, Editor::IEditorWindow>();
	TypeInitializer::AddBaseType<Editor::UIActorsTree, o2::UITree>();
	TypeInitializer::AddBaseType<Editor::TreeWindow, Editor::IEditorWindow>();
	TypeInitializer::AddBaseType<Editor::EditorConfig::GlobalConfig, o2::ISerializable>();
	TypeInitializer::AddBaseType<Editor::EditorConfig::ProjectConfig, o2::ISerializable>();
	TypeInitializer::AddBaseType<Editor::DeleteActorsAction::ActorInfo, o2::ISerializable>();
	TypeInitializer::AddBaseType<Editor::WindowsLayout::WindowDockPlace, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<RectF>, o2::IAnimatedValue>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<RectF>::Key, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<Color4>, o2::IAnimatedValue>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<Color4>::Key, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<bool>, o2::IAnimatedValue>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<bool>::Key, o2::ISerializable>();
	TypeInitializer::AddBaseType<Editor::AssetProperty<ActorAsset>, Editor::IAssetProperty>();
	TypeInitializer::AddBaseType<Editor::AssetProperty<AnimationAsset>, Editor::IAssetProperty>();
	TypeInitializer::AddBaseType<Editor::AssetProperty<AtlasAsset>, Editor::IAssetProperty>();
	TypeInitializer::AddBaseType<Editor::AssetProperty<BinaryAsset>, Editor::IAssetProperty>();
	TypeInitializer::AddBaseType<Editor::AssetProperty<BitmapFontAsset>, Editor::IAssetProperty>();
	TypeInitializer::AddBaseType<Editor::AssetProperty<DataAsset>, Editor::IAssetProperty>();
	TypeInitializer::AddBaseType<Editor::AssetProperty<FolderAsset>, Editor::IAssetProperty>();
	TypeInitializer::AddBaseType<Editor::AssetProperty<ImageAsset>, Editor::IAssetProperty>();
	TypeInitializer::AddBaseType<Editor::AssetProperty<VectorFontAsset>, Editor::IAssetProperty>();

}