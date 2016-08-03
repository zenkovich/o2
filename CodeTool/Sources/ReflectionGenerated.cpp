#include "C:\work\o2\CodeTool\Platforms\Windows\..\..\Sources\ReflectionGenerated.h"

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
#include "C:\work\o2\CodeTool\Platforms\Windows\..\..\Sources\CodeToolApp.h"
#include "C:\work\o2\CodeTool\Platforms\Windows\..\..\Sources\SyntaxTree\CppSyntaxParser.h"
#include "C:\work\o2\CodeTool\Platforms\Windows\..\..\Sources\SyntaxTree\SyntaxTree.h"

// Types declarations
o2::Type o2::Animatable::type("o2::Animatable");
o2::Type o2::AnimatedValue<float>::type("o2::AnimatedValue<float>");
o2::Type o2::IAnimatedValue::type("o2::IAnimatedValue");
o2::Type o2::AnimatedValue<Vec2F>::type("o2::AnimatedValue<Vec2F>");
o2::Type o2::Animation::type("o2::Animation");
o2::Type o2::AnimationMask::type("o2::AnimationMask");
o2::Type o2::AnimationState::type("o2::AnimationState");
o2::Type o2::IAnimation::type("o2::IAnimation");
o2::Type o2::ActorAsset::type("o2::ActorAsset");
o2::Type o2::AnimationAsset::type("o2::AnimationAsset");
o2::Type o2::Asset::type("o2::Asset");
o2::Type o2::AssetInfo::type("o2::AssetInfo");
o2::Type o2::AtlasAsset::type("o2::AtlasAsset");
o2::Type o2::BinaryAsset::type("o2::BinaryAsset");
o2::Type o2::BitmapFontAsset::type("o2::BitmapFontAsset");
o2::Type o2::DataAsset::type("o2::DataAsset");
o2::Type o2::FolderAsset::type("o2::FolderAsset");
o2::Type o2::ImageAsset::type("o2::ImageAsset");
o2::Type o2::VectorFontAsset::type("o2::VectorFontAsset");
o2::Type o2::AtlasAssetConverter::type("o2::AtlasAssetConverter");
o2::Type o2::FolderAssetConverter::type("o2::FolderAssetConverter");
o2::Type o2::IAssetConverter::type("o2::IAssetConverter");
o2::Type o2::ImageAssetConverter::type("o2::ImageAssetConverter");
o2::Type o2::StdAssetConverter::type("o2::StdAssetConverter");
o2::Type o2::ProjectConfig::type("o2::ProjectConfig");
o2::Type o2::Camera::type("o2::Camera");
o2::Type o2::IRectDrawable::type("o2::IRectDrawable");
o2::Type o2::Sprite::type("o2::Sprite");
o2::Type o2::Text::type("o2::Text");
o2::Type o2::FontStrokeEffect::type("o2::FontStrokeEffect");
o2::Type o2::FontGradientEffect::type("o2::FontGradientEffect");
o2::Type o2::FontColorEffect::type("o2::FontColorEffect");
o2::Type o2::FontShadowEffect::type("o2::FontShadowEffect");
o2::Type o2::Actor::type("o2::Actor");
o2::Type o2::ActorTransform::type("o2::ActorTransform");
o2::Type o2::Component::type("o2::Component");
o2::Type o2::DrawableComponent::type("o2::DrawableComponent");
o2::Type o2::Tag::type("o2::Tag");
o2::Type o2::TagGroup::type("o2::TagGroup");
o2::Type o2::EditorTestComponent::type("o2::EditorTestComponent");
o2::Type o2::ImageComponent::type("o2::ImageComponent");
o2::Type o2::UIButton::type("o2::UIButton");
o2::Type o2::UIContextMenuItem::type("o2::UIContextMenuItem");
o2::Type o2::UIContextMenu::type("o2::UIContextMenu");
o2::Type o2::UICustomDropDown::type("o2::UICustomDropDown");
o2::Type o2::UICustomList::type("o2::UICustomList");
o2::Type o2::UIDropDown::type("o2::UIDropDown");
o2::Type o2::UIEditBox::type("o2::UIEditBox");
o2::Type o2::UIGridLayout::type("o2::UIGridLayout");
o2::Type o2::UIHorizontalLayout::type("o2::UIHorizontalLayout");
o2::Type o2::UIHorizontalProgress::type("o2::UIHorizontalProgress");
o2::Type o2::UIHorizontalScrollBar::type("o2::UIHorizontalScrollBar");
o2::Type o2::UILabel::type("o2::UILabel");
o2::Type o2::UIList::type("o2::UIList");
o2::Type o2::UILongList::type("o2::UILongList");
o2::Type o2::UIMenuPanel::type("o2::UIMenuPanel");
o2::Type o2::UIScrollArea::type("o2::UIScrollArea");
o2::Type o2::UIToggle::type("o2::UIToggle");
o2::Type o2::UITree::type("o2::UITree");
o2::Type o2::UITreeNode::type("o2::UITreeNode");
o2::Type o2::UIVerticalLayout::type("o2::UIVerticalLayout");
o2::Type o2::UIVerticalProgress::type("o2::UIVerticalProgress");
o2::Type o2::UIVerticalScrollBar::type("o2::UIVerticalScrollBar");
o2::Type o2::UIWidget::type("o2::UIWidget");
o2::Type o2::UIWidgetLayer::type("o2::UIWidgetLayer");
o2::Type o2::UIWidgetLayout::type("o2::UIWidgetLayout");
o2::Type o2::UIWidgetState::type("o2::UIWidgetState");
o2::Type o2::UIWindow::type("o2::UIWindow");
o2::Type o2::ISerializable::type("o2::ISerializable");
o2::Type o2::ShortcutKeys::type("o2::ShortcutKeys");
o2::Type o2::TimeStamp::type("o2::TimeStamp");
o2::Type o2::Curve::type("o2::Curve");
o2::Type o2::Layout::type("o2::Layout");
o2::Type o2::Transform::type("o2::Transform");
o2::Type o2::AnimatedValue<Vec2F>::Key::type("o2::AnimatedValue<Vec2F>::Key");
o2::Type o2::Animation::AnimatedValueDef::type("o2::Animation::AnimatedValueDef");
o2::Type o2::ActorAsset::MetaInfo::type("o2::ActorAsset::MetaInfo");
o2::Type o2::AnimationAsset::MetaInfo::type("o2::AnimationAsset::MetaInfo");
o2::Type o2::Asset::IMetaInfo::type("o2::Asset::IMetaInfo");
o2::Type o2::AssetTree::AssetNode::type("o2::AssetTree::AssetNode");
o2::Type o2::AtlasAsset::PlatformMeta::type("o2::AtlasAsset::PlatformMeta");
o2::Type o2::AtlasAsset::MetaInfo::type("o2::AtlasAsset::MetaInfo");
o2::Type o2::AtlasAsset::Page::type("o2::AtlasAsset::Page");
o2::Type o2::BinaryAsset::MetaInfo::type("o2::BinaryAsset::MetaInfo");
o2::Type o2::BitmapFontAsset::MetaInfo::type("o2::BitmapFontAsset::MetaInfo");
o2::Type o2::DataAsset::MetaInfo::type("o2::DataAsset::MetaInfo");
o2::Type o2::FolderAsset::MetaInfo::type("o2::FolderAsset::MetaInfo");
o2::Type o2::ImageAsset::PlatformMeta::type("o2::ImageAsset::PlatformMeta");
o2::Type o2::ImageAsset::MetaInfo::type("o2::ImageAsset::MetaInfo");
o2::Type o2::VectorFontAsset::MetaInfo::type("o2::VectorFontAsset::MetaInfo");
o2::Type o2::AtlasAssetConverter::Image::type("o2::AtlasAssetConverter::Image");
o2::Type o2::VectorFont::Effect::type("o2::VectorFont::Effect");
o2::Type o2::Scene::Layer::type("o2::Scene::Layer");
o2::Type o2::UIContextMenu::Item::type("o2::UIContextMenu::Item");
o2::Type o2::UIMenuPanel::Item::type("o2::UIMenuPanel::Item");
o2::Type o2::Curve::Key::type("o2::Curve::Key");
o2::Type CodeTool::Tmp::type("CodeTool::Tmp");
o2::Type CodeTool::ICppSyntaxStatementParser::type("CodeTool::ICppSyntaxStatementParser");
o2::Type CodeTool::CppSyntaxNamespaceParser::type("CodeTool::CppSyntaxNamespaceParser");
o2::Type CodeTool::CppSyntaxMultilineCommentParser::type("CodeTool::CppSyntaxMultilineCommentParser");
o2::Type CodeTool::CppSyntaxCommentParser::type("CodeTool::CppSyntaxCommentParser");
o2::Type CodeTool::CppSyntaxPragmaParser::type("CodeTool::CppSyntaxPragmaParser");
o2::Type CodeTool::CppSyntaxIncludeParser::type("CodeTool::CppSyntaxIncludeParser");
o2::Type CodeTool::CppSyntaxDefineParser::type("CodeTool::CppSyntaxDefineParser");
o2::Type CodeTool::CppSyntaxIfMacroParser::type("CodeTool::CppSyntaxIfMacroParser");
o2::Type CodeTool::CppSyntaxClassParser::type("CodeTool::CppSyntaxClassParser");
o2::Type CodeTool::CppSyntaxStructParser::type("CodeTool::CppSyntaxStructParser");
o2::Type CodeTool::CppSyntaxTemplateParser::type("CodeTool::CppSyntaxTemplateParser");
o2::Type CodeTool::CppSyntaxTypedefParser::type("CodeTool::CppSyntaxTypedefParser");
o2::Type CodeTool::CppSyntaxEnumParser::type("CodeTool::CppSyntaxEnumParser");
o2::Type CodeTool::CppSyntaxUsingParser::type("CodeTool::CppSyntaxUsingParser");
o2::Type CodeTool::CppSyntaxPublicSectionParser::type("CodeTool::CppSyntaxPublicSectionParser");
o2::Type CodeTool::CppSyntaxPrivateSectionParser::type("CodeTool::CppSyntaxPrivateSectionParser");
o2::Type CodeTool::CppSyntaxProtectedSectionParser::type("CodeTool::CppSyntaxProtectedSectionParser");
o2::Type CodeTool::CppSyntaxFriendParser::type("CodeTool::CppSyntaxFriendParser");
o2::Type CodeTool::SyntaxTree::type("CodeTool::SyntaxTree");
o2::Type CodeTool::SyntaxFile::type("CodeTool::SyntaxFile");
o2::Type CodeTool::SyntaxEntry::type("CodeTool::SyntaxEntry");
o2::Type CodeTool::SyntaxSection::type("CodeTool::SyntaxSection");
o2::Type CodeTool::SyntaxNamespace::type("CodeTool::SyntaxNamespace");
o2::Type CodeTool::SyntaxClass::type("CodeTool::SyntaxClass");
o2::Type CodeTool::SyntaxType::type("CodeTool::SyntaxType");
o2::Type CodeTool::SyntaxVariable::type("CodeTool::SyntaxVariable");
o2::Type CodeTool::SyntaxFunction::type("CodeTool::SyntaxFunction");
o2::Type CodeTool::SyntaxEnum::type("CodeTool::SyntaxEnum");
o2::Type o2::AnimatedValue<Color4>::type("o2::AnimatedValue<Color4>");
o2::Type o2::AnimatedValue<Color4>::Key::type("o2::AnimatedValue<Color4>::Key");
o2::Type o2::AnimatedValue<RectF>::type("o2::AnimatedValue<RectF>");
o2::Type o2::AnimatedValue<RectF>::Key::type("o2::AnimatedValue<RectF>::Key");
o2::Type o2::AnimatedValue<bool>::type("o2::AnimatedValue<bool>");
o2::Type o2::AnimatedValue<bool>::Key::type("o2::AnimatedValue<bool>::Key");

// Types initializations
void o2::Animatable::InitializeType(o2::Animatable* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mStates", (size_t)(char*)(&sample->mStates) - (size_t)(char*)iobject, sample->mStates, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValues", (size_t)(char*)(&sample->mValues) - (size_t)(char*)iobject, sample->mValues, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBlend", (size_t)(char*)(&sample->mBlend) - (size_t)(char*)iobject, sample->mBlend, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "curve", (size_t)(char*)(&sample->curve) - (size_t)(char*)iobject, sample->curve, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)iobject, sample->target, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)iobject, sample->targetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)iobject, sample->targetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)iobject, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)iobject, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)iobject, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)iobject, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)iobject, sample->mTargetDelegate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)iobject, sample->mTargetProperty, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "onKeysChanged", (size_t)(char*)(&sample->onKeysChanged) - (size_t)(char*)iobject, sample->onKeysChanged, o2::ProtectSection::Public);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)iobject, sample->target, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)iobject, sample->targetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)iobject, sample->targetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)iobject, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)iobject, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)iobject, sample->mKeys, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)iobject, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)iobject, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)iobject, sample->mTargetDelegate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)iobject, sample->mTargetProperty, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mAnimatedValues", (size_t)(char*)(&sample->mAnimatedValues) - (size_t)(char*)iobject, sample->mAnimatedValues, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)iobject, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAnimationState", (size_t)(char*)(&sample->mAnimationState) - (size_t)(char*)iobject, sample->mAnimationState, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Animation, void, IObject*, bool>(&type, "SetTarget", &o2::Animation::SetTarget, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IObject*>(funcInfo, "target");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "errors");
	funcInfo = TypeInitializer::RegFunction<o2::Animation, IObject*>(&type, "GetTarget", &o2::Animation::GetTarget, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Animation, void>(&type, "Clear", &o2::Animation::Clear, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Animation, AnimatedValuesVec&>(&type, "GetAnimationsValues", &o2::Animation::GetAnimationsValues, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Animation, const AnimatedValuesVec&>(&type, "GetAnimationsValues", &o2::Animation::GetAnimationsValues, o2::ProtectSection::Public);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "weights", (size_t)(char*)(&sample->weights) - (size_t)(char*)iobject, sample->weights, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimationMask, float, const String&>(&type, "GetNodeWeight", &o2::AnimationMask::GetNodeWeight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "node");
}

void o2::AnimationState::InitializeType(o2::AnimationState* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)iobject, sample->name, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "animation", (size_t)(char*)(&sample->animation) - (size_t)(char*)iobject, sample->animation, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mask", (size_t)(char*)(&sample->mask) - (size_t)(char*)iobject, sample->mask, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "weight", (size_t)(char*)(&sample->weight) - (size_t)(char*)iobject, sample->weight, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "workWeight", (size_t)(char*)(&sample->workWeight) - (size_t)(char*)iobject, sample->workWeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)iobject, sample->mOwner, o2::ProtectSection::Protected);
}

void o2::IAnimation::InitializeType(o2::IAnimation* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "playing", (size_t)(char*)(&sample->playing) - (size_t)(char*)iobject, sample->playing, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "reversed", (size_t)(char*)(&sample->reversed) - (size_t)(char*)iobject, sample->reversed, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "speed", (size_t)(char*)(&sample->speed) - (size_t)(char*)iobject, sample->speed, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "time", (size_t)(char*)(&sample->time) - (size_t)(char*)iobject, sample->time, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "relTime", (size_t)(char*)(&sample->relTime) - (size_t)(char*)iobject, sample->relTime, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "beginBound", (size_t)(char*)(&sample->beginBound) - (size_t)(char*)iobject, sample->beginBound, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "endBound", (size_t)(char*)(&sample->endBound) - (size_t)(char*)iobject, sample->endBound, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "loop", (size_t)(char*)(&sample->loop) - (size_t)(char*)iobject, sample->loop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "duration", (size_t)(char*)(&sample->duration) - (size_t)(char*)iobject, sample->duration, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onPlayEvent", (size_t)(char*)(&sample->onPlayEvent) - (size_t)(char*)iobject, sample->onPlayEvent, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onStopEvent", (size_t)(char*)(&sample->onStopEvent) - (size_t)(char*)iobject, sample->onStopEvent, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onPlayedEvent", (size_t)(char*)(&sample->onPlayedEvent) - (size_t)(char*)iobject, sample->onPlayedEvent, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onUpdate", (size_t)(char*)(&sample->onUpdate) - (size_t)(char*)iobject, sample->onUpdate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTime", (size_t)(char*)(&sample->mTime) - (size_t)(char*)iobject, sample->mTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mInDurationTime", (size_t)(char*)(&sample->mInDurationTime) - (size_t)(char*)iobject, sample->mInDurationTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDuration", (size_t)(char*)(&sample->mDuration) - (size_t)(char*)iobject, sample->mDuration, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBeginTime", (size_t)(char*)(&sample->mBeginTime) - (size_t)(char*)iobject, sample->mBeginTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEndTime", (size_t)(char*)(&sample->mEndTime) - (size_t)(char*)iobject, sample->mEndTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDirection", (size_t)(char*)(&sample->mDirection) - (size_t)(char*)iobject, sample->mDirection, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSpeed", (size_t)(char*)(&sample->mSpeed) - (size_t)(char*)iobject, sample->mSpeed, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLoop", (size_t)(char*)(&sample->mLoop) - (size_t)(char*)iobject, sample->mLoop, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mPlaying", (size_t)(char*)(&sample->mPlaying) - (size_t)(char*)iobject, sample->mPlaying, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTimeEvents", (size_t)(char*)(&sample->mTimeEvents) - (size_t)(char*)iobject, sample->mTimeEvents, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "actor", (size_t)(char*)(&sample->actor) - (size_t)(char*)iobject, sample->actor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)iobject, sample->meta, o2::ProtectSection::Public);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "animation", (size_t)(char*)(&sample->animation) - (size_t)(char*)iobject, sample->animation, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)iobject, sample->meta, o2::ProtectSection::Public);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "path", (size_t)(char*)(&sample->path) - (size_t)(char*)iobject, sample->path, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fullPath", (size_t)(char*)(&sample->fullPath) - (size_t)(char*)iobject, sample->fullPath, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "id", (size_t)(char*)(&sample->id) - (size_t)(char*)iobject, sample->id, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)iobject, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mPath", (size_t)(char*)(&sample->mPath) - (size_t)(char*)iobject, sample->mPath, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMeta", (size_t)(char*)(&sample->mMeta) - (size_t)(char*)iobject, sample->mMeta, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mType", (size_t)(char*)(&sample->mType) - (size_t)(char*)iobject, sample->mType, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mPath", (size_t)(char*)(&sample->mPath) - (size_t)(char*)iobject, sample->mPath, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)iobject, sample->mId, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::AtlasAsset::InitializeType(o2::AtlasAsset* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)iobject, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imagesInfos", (size_t)(char*)(&sample->imagesInfos) - (size_t)(char*)iobject, sample->imagesInfos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "images", (size_t)(char*)(&sample->images) - (size_t)(char*)iobject, sample->images, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "pages", (size_t)(char*)(&sample->pages) - (size_t)(char*)iobject, sample->pages, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mImagesAssetsInfos", (size_t)(char*)(&sample->mImagesAssetsInfos) - (size_t)(char*)iobject, sample->mImagesAssetsInfos, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPages", (size_t)(char*)(&sample->mPages) - (size_t)(char*)iobject, sample->mPages, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "data", (size_t)(char*)(&sample->data) - (size_t)(char*)iobject, sample->data, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "dataSize", (size_t)(char*)(&sample->dataSize) - (size_t)(char*)iobject, sample->dataSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)iobject, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mData", (size_t)(char*)(&sample->mData) - (size_t)(char*)iobject, sample->mData, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDataSize", (size_t)(char*)(&sample->mDataSize) - (size_t)(char*)iobject, sample->mDataSize, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)iobject, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)iobject, sample->font, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mFont", (size_t)(char*)(&sample->mFont) - (size_t)(char*)iobject, sample->mFont, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, MetaInfo*>(&type, "GetMeta", &o2::BitmapFontAsset::GetMeta, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, FontRef>(&type, "GetFont", &o2::BitmapFontAsset::GetFont, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, const char*>(&type, "GetFileExtensions", &o2::BitmapFontAsset::GetFileExtensions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, void, const String&>(&type, "LoadData", &o2::BitmapFontAsset::LoadData, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset, void>(&type, "InitializeProperties", &o2::BitmapFontAsset::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::DataAsset::InitializeType(o2::DataAsset* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "data", (size_t)(char*)(&sample->data) - (size_t)(char*)iobject, sample->data, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)iobject, sample->meta, o2::ProtectSection::Public);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)iobject, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "insideAssets", (size_t)(char*)(&sample->insideAssets) - (size_t)(char*)iobject, sample->insideAssets, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mContainingAssetsInfos", (size_t)(char*)(&sample->mContainingAssetsInfos) - (size_t)(char*)iobject, sample->mContainingAssetsInfos, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "bitmap", (size_t)(char*)(&sample->bitmap) - (size_t)(char*)iobject, sample->bitmap, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "atlasId", (size_t)(char*)(&sample->atlasId) - (size_t)(char*)iobject, sample->atlasId, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "atlas", (size_t)(char*)(&sample->atlas) - (size_t)(char*)iobject, sample->atlas, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "atlasPage", (size_t)(char*)(&sample->atlasPage) - (size_t)(char*)iobject, sample->atlasPage, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "atlasRect", (size_t)(char*)(&sample->atlasRect) - (size_t)(char*)iobject, sample->atlasRect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "size", (size_t)(char*)(&sample->size) - (size_t)(char*)iobject, sample->size, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "width", (size_t)(char*)(&sample->width) - (size_t)(char*)iobject, sample->width, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "height", (size_t)(char*)(&sample->height) - (size_t)(char*)iobject, sample->height, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)iobject, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mBitmap", (size_t)(char*)(&sample->mBitmap) - (size_t)(char*)iobject, sample->mBitmap, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAtlasPage", (size_t)(char*)(&sample->mAtlasPage) - (size_t)(char*)iobject, sample->mAtlasPage, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAtlasRect", (size_t)(char*)(&sample->mAtlasRect) - (size_t)(char*)iobject, sample->mAtlasRect, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "meta", (size_t)(char*)(&sample->meta) - (size_t)(char*)iobject, sample->meta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)iobject, sample->font, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mFont", (size_t)(char*)(&sample->mFont) - (size_t)(char*)iobject, sample->mFont, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mRemovedFolders", (size_t)(char*)(&sample->mRemovedFolders) - (size_t)(char*)iobject, sample->mRemovedFolders, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mAssetsBuilder", (size_t)(char*)(&sample->mAssetsBuilder) - (size_t)(char*)iobject, sample->mAssetsBuilder, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "projectName", (size_t)(char*)(&sample->projectName) - (size_t)(char*)iobject, sample->projectName, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "currentPlatform", (size_t)(char*)(&sample->currentPlatform) - (size_t)(char*)iobject, sample->currentPlatform, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "projectPath", (size_t)(char*)(&sample->projectPath) - (size_t)(char*)iobject, sample->projectPath, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mProjectName", (size_t)(char*)(&sample->mProjectName) - (size_t)(char*)iobject, sample->mProjectName, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mPlatform", (size_t)(char*)(&sample->mPlatform) - (size_t)(char*)iobject, sample->mPlatform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mProjectPath", (size_t)(char*)(&sample->mProjectPath) - (size_t)(char*)iobject, sample->mProjectPath, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, String>(&type, "GetProjectName", &o2::ProjectConfig::GetProjectName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, const String&>(&type, "SetProjectName", &o2::ProjectConfig::SetProjectName, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "name");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, Platform>(&type, "GetPlatform", &o2::ProjectConfig::GetPlatform, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, Platform>(&type, "SetPlatform", &o2::ProjectConfig::SetPlatform, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Platform>(funcInfo, "platform");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, String>(&type, "GetProjectPath", &o2::ProjectConfig::GetProjectPath, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, const String&>(&type, "SetProjectPath", &o2::ProjectConfig::SetProjectPath, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "path");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void, const String&>(&type, "InitializeDefault", &o2::ProjectConfig::InitializeDefault, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "configFilePath");
	funcInfo = TypeInitializer::RegFunction<o2::ProjectConfig, void>(&type, "InitializeProperties", &o2::ProjectConfig::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::Camera::InitializeType(o2::Camera* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
}

void o2::IRectDrawable::InitializeType(o2::IRectDrawable* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)iobject, sample->color, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "transparency", (size_t)(char*)(&sample->transparency) - (size_t)(char*)iobject, sample->transparency, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabled", (size_t)(char*)(&sample->enabled) - (size_t)(char*)iobject, sample->enabled, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mColor", (size_t)(char*)(&sample->mColor) - (size_t)(char*)iobject, sample->mColor, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mEnabled", (size_t)(char*)(&sample->mEnabled) - (size_t)(char*)iobject, sample->mEnabled, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "texture", (size_t)(char*)(&sample->texture) - (size_t)(char*)iobject, sample->texture, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "textureSrcRect", (size_t)(char*)(&sample->textureSrcRect) - (size_t)(char*)iobject, sample->textureSrcRect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAssetId", (size_t)(char*)(&sample->imageAssetId) - (size_t)(char*)iobject, sample->imageAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAssetPath", (size_t)(char*)(&sample->imageAssetPath) - (size_t)(char*)iobject, sample->imageAssetPath, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAsset", (size_t)(char*)(&sample->imageAsset) - (size_t)(char*)iobject, sample->imageAsset, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "bitmap", (size_t)(char*)(&sample->bitmap) - (size_t)(char*)iobject, sample->bitmap, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftTopColor", (size_t)(char*)(&sample->leftTopColor) - (size_t)(char*)iobject, sample->leftTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightTopColor", (size_t)(char*)(&sample->rightTopColor) - (size_t)(char*)iobject, sample->rightTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftBottomColor", (size_t)(char*)(&sample->leftBottomColor) - (size_t)(char*)iobject, sample->leftBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightBottomColor", (size_t)(char*)(&sample->rightBottomColor) - (size_t)(char*)iobject, sample->rightBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mode", (size_t)(char*)(&sample->mode) - (size_t)(char*)iobject, sample->mode, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fill", (size_t)(char*)(&sample->fill) - (size_t)(char*)iobject, sample->fill, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "sliceBorder", (size_t)(char*)(&sample->sliceBorder) - (size_t)(char*)iobject, sample->sliceBorder, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTextureSrcRect", (size_t)(char*)(&sample->mTextureSrcRect) - (size_t)(char*)iobject, sample->mTextureSrcRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCornersColors", (size_t)(char*)(&sample->mCornersColors) - (size_t)(char*)iobject, sample->mCornersColors, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mImageAssetId", (size_t)(char*)(&sample->mImageAssetId) - (size_t)(char*)iobject, sample->mImageAssetId, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAtlasAssetId", (size_t)(char*)(&sample->mAtlasAssetId) - (size_t)(char*)iobject, sample->mAtlasAssetId, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMode", (size_t)(char*)(&sample->mMode) - (size_t)(char*)iobject, sample->mMode, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSlices", (size_t)(char*)(&sample->mSlices) - (size_t)(char*)iobject, sample->mSlices, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFill", (size_t)(char*)(&sample->mFill) - (size_t)(char*)iobject, sample->mFill, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMesh", (size_t)(char*)(&sample->mMesh) - (size_t)(char*)iobject, sample->mMesh, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMeshBuildFunc", (size_t)(char*)(&sample->mMeshBuildFunc) - (size_t)(char*)iobject, sample->mMeshBuildFunc, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)iobject, sample->font, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)iobject, sample->text, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "height", (size_t)(char*)(&sample->height) - (size_t)(char*)iobject, sample->height, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "verAlign", (size_t)(char*)(&sample->verAlign) - (size_t)(char*)iobject, sample->verAlign, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "horAlign", (size_t)(char*)(&sample->horAlign) - (size_t)(char*)iobject, sample->horAlign, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "wordWrap", (size_t)(char*)(&sample->wordWrap) - (size_t)(char*)iobject, sample->wordWrap, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "dotsEngings", (size_t)(char*)(&sample->dotsEngings) - (size_t)(char*)iobject, sample->dotsEngings, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "symbolsDistanceCoef", (size_t)(char*)(&sample->symbolsDistanceCoef) - (size_t)(char*)iobject, sample->symbolsDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "linesDistanceCoef", (size_t)(char*)(&sample->linesDistanceCoef) - (size_t)(char*)iobject, sample->linesDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mMeshMaxPolyCount", (size_t)(char*)(&sample->mMeshMaxPolyCount) - (size_t)(char*)iobject, sample->mMeshMaxPolyCount, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mText", (size_t)(char*)(&sample->mText) - (size_t)(char*)iobject, sample->mText, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFontAssetId", (size_t)(char*)(&sample->mFontAssetId) - (size_t)(char*)iobject, sample->mFontAssetId, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFont", (size_t)(char*)(&sample->mFont) - (size_t)(char*)iobject, sample->mFont, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHeight", (size_t)(char*)(&sample->mHeight) - (size_t)(char*)iobject, sample->mHeight, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSymbolsDistCoef", (size_t)(char*)(&sample->mSymbolsDistCoef) - (size_t)(char*)iobject, sample->mSymbolsDistCoef, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLinesDistanceCoef", (size_t)(char*)(&sample->mLinesDistanceCoef) - (size_t)(char*)iobject, sample->mLinesDistanceCoef, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mVerAlign", (size_t)(char*)(&sample->mVerAlign) - (size_t)(char*)iobject, sample->mVerAlign, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHorAlign", (size_t)(char*)(&sample->mHorAlign) - (size_t)(char*)iobject, sample->mHorAlign, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWordWrap", (size_t)(char*)(&sample->mWordWrap) - (size_t)(char*)iobject, sample->mWordWrap, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDotsEndings", (size_t)(char*)(&sample->mDotsEndings) - (size_t)(char*)iobject, sample->mDotsEndings, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMeshes", (size_t)(char*)(&sample->mMeshes) - (size_t)(char*)iobject, sample->mMeshes, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastTransform", (size_t)(char*)(&sample->mLastTransform) - (size_t)(char*)iobject, sample->mLastTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSymbolsSet", (size_t)(char*)(&sample->mSymbolsSet) - (size_t)(char*)iobject, sample->mSymbolsSet, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mUpdatingMesh", (size_t)(char*)(&sample->mUpdatingMesh) - (size_t)(char*)iobject, sample->mUpdatingMesh, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "radius", (size_t)(char*)(&sample->radius) - (size_t)(char*)iobject, sample->radius, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "alphaThreshold", (size_t)(char*)(&sample->alphaThreshold) - (size_t)(char*)iobject, sample->alphaThreshold, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)iobject, sample->color, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontStrokeEffect, void, Bitmap*>(&type, "Process", &o2::FontStrokeEffect::Process, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontStrokeEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontStrokeEffect::GetSizeExtend, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FontStrokeEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontStrokeEffect::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::FontGradientEffect::InitializeType(o2::FontGradientEffect* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "color1", (size_t)(char*)(&sample->color1) - (size_t)(char*)iobject, sample->color1, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "color2", (size_t)(char*)(&sample->color2) - (size_t)(char*)iobject, sample->color2, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "angle", (size_t)(char*)(&sample->angle) - (size_t)(char*)iobject, sample->angle, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "length", (size_t)(char*)(&sample->length) - (size_t)(char*)iobject, sample->length, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "origin", (size_t)(char*)(&sample->origin) - (size_t)(char*)iobject, sample->origin, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontGradientEffect, void, Bitmap*>(&type, "Process", &o2::FontGradientEffect::Process, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontGradientEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontGradientEffect::GetSizeExtend, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FontGradientEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontGradientEffect::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::FontColorEffect::InitializeType(o2::FontColorEffect* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)iobject, sample->color, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontColorEffect, void, Bitmap*>(&type, "Process", &o2::FontColorEffect::Process, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontColorEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontColorEffect::GetSizeExtend, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FontColorEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontColorEffect::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::FontShadowEffect::InitializeType(o2::FontShadowEffect* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "blurRadius", (size_t)(char*)(&sample->blurRadius) - (size_t)(char*)iobject, sample->blurRadius, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "offset", (size_t)(char*)(&sample->offset) - (size_t)(char*)iobject, sample->offset, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "color", (size_t)(char*)(&sample->color) - (size_t)(char*)iobject, sample->color, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::FontShadowEffect, void, Bitmap*>(&type, "Process", &o2::FontShadowEffect::Process, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::FontShadowEffect, Vec2I>(&type, "GetSizeExtend", &o2::FontShadowEffect::GetSizeExtend, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::FontShadowEffect, bool, VectorFont::Effect*>(&type, "IsEqual", &o2::FontShadowEffect::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<VectorFont::Effect*>(funcInfo, "other");
}

void o2::Actor::InitializeType(o2::Actor* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "tags", (size_t)(char*)(&sample->tags) - (size_t)(char*)iobject, sample->tags, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "id", (size_t)(char*)(&sample->id) - (size_t)(char*)iobject, sample->id, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)iobject, sample->name, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabled", (size_t)(char*)(&sample->enabled) - (size_t)(char*)iobject, sample->enabled, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabledInHierarchy", (size_t)(char*)(&sample->enabledInHierarchy) - (size_t)(char*)iobject, sample->enabledInHierarchy, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "locked", (size_t)(char*)(&sample->locked) - (size_t)(char*)iobject, sample->locked, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "lockedInHierarchy", (size_t)(char*)(&sample->lockedInHierarchy) - (size_t)(char*)iobject, sample->lockedInHierarchy, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "parent", (size_t)(char*)(&sample->parent) - (size_t)(char*)iobject, sample->parent, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "layer", (size_t)(char*)(&sample->layer) - (size_t)(char*)iobject, sample->layer, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "layerName", (size_t)(char*)(&sample->layerName) - (size_t)(char*)iobject, sample->layerName, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "childs", (size_t)(char*)(&sample->childs) - (size_t)(char*)iobject, sample->childs, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "child", (size_t)(char*)(&sample->child) - (size_t)(char*)iobject, sample->child, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "components", (size_t)(char*)(&sample->components) - (size_t)(char*)iobject, sample->components, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "component", (size_t)(char*)(&sample->component) - (size_t)(char*)iobject, sample->component, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "transform", (size_t)(char*)(&sample->transform) - (size_t)(char*)iobject, sample->transform, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "onEnableChanged", (size_t)(char*)(&sample->onEnableChanged) - (size_t)(char*)iobject, sample->onEnableChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)iobject, sample->mId, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)iobject, sample->mName, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mParent", (size_t)(char*)(&sample->mParent) - (size_t)(char*)iobject, sample->mParent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mChilds", (size_t)(char*)(&sample->mChilds) - (size_t)(char*)iobject, sample->mChilds, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mComponents", (size_t)(char*)(&sample->mComponents) - (size_t)(char*)iobject, sample->mComponents, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLayer", (size_t)(char*)(&sample->mLayer) - (size_t)(char*)iobject, sample->mLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEnabled", (size_t)(char*)(&sample->mEnabled) - (size_t)(char*)iobject, sample->mEnabled, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResEnabled", (size_t)(char*)(&sample->mResEnabled) - (size_t)(char*)iobject, sample->mResEnabled, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLocked", (size_t)(char*)(&sample->mLocked) - (size_t)(char*)iobject, sample->mLocked, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResLocked", (size_t)(char*)(&sample->mResLocked) - (size_t)(char*)iobject, sample->mResLocked, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsOnScene", (size_t)(char*)(&sample->mIsOnScene) - (size_t)(char*)iobject, sample->mIsOnScene, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsAsset", (size_t)(char*)(&sample->mIsAsset) - (size_t)(char*)iobject, sample->mIsAsset, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAssetId", (size_t)(char*)(&sample->mAssetId) - (size_t)(char*)iobject, sample->mAssetId, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, const String&>(&type, "SetLayer", &o2::Actor::SetLayer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const String&>(funcInfo, "layerName");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, Scene::Layer*>(&type, "GetLayer", &o2::Actor::GetLayer, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, String>(&type, "GetLayerName", &o2::Actor::GetLayerName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "OnTransformChanged", &o2::Actor::OnTransformChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Actor*, const Actor*, Vector<Actor**>&, Vector<Component**>&, Dictionary<const Actor*, Actor*>&, Dictionary<const Component*, Component*>&>(&type, "ProcessCopying", &o2::Actor::ProcessCopying, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "dest");
	TypeInitializer::RegFuncParam<const Actor*>(funcInfo, "source");
	TypeInitializer::RegFuncParam<Vector<Actor**>&>(funcInfo, "actorsPointers");
	TypeInitializer::RegFuncParam<Vector<Component**>&>(funcInfo, "componentsPointers");
	TypeInitializer::RegFuncParam<Dictionary<const Actor*, Actor*>&>(funcInfo, "actorsMap");
	TypeInitializer::RegFuncParam<Dictionary<const Component*, Component*>&>(funcInfo, "componentsMap");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, const Vector<Actor**>&, const Vector<Component**>&, const Dictionary<const Actor*, Actor*>&, const Dictionary<const Component*, Component*>&>(&type, "FixComponentFieldsPointers", &o2::Actor::FixComponentFieldsPointers, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vector<Actor**>&>(funcInfo, "actorsPointers");
	TypeInitializer::RegFuncParam<const Vector<Component**>&>(funcInfo, "componentsPointers");
	TypeInitializer::RegFuncParam<const Dictionary<const Actor*, Actor*>&>(funcInfo, "actorsMap");
	TypeInitializer::RegFuncParam<const Dictionary<const Component*, Component*>&>(funcInfo, "componentsMap");
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
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "OnChanged", &o2::Actor::OnChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "OnLockChanged", &o2::Actor::OnLockChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "OnNameChanged", &o2::Actor::OnNameChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "OnChildsChanged", &o2::Actor::OnChildsChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void, Actor*>(&type, "OnParentChanged", &o2::Actor::OnParentChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Actor*>(funcInfo, "oldParent");
	funcInfo = TypeInitializer::RegFunction<o2::Actor, void>(&type, "InitializeProperties", &o2::Actor::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::ActorTransform::InitializeType(o2::ActorTransform* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "actor", (size_t)(char*)(&sample->actor) - (size_t)(char*)iobject, sample->actor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldPosition", (size_t)(char*)(&sample->worldPosition) - (size_t)(char*)iobject, sample->worldPosition, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldRect", (size_t)(char*)(&sample->worldRect) - (size_t)(char*)iobject, sample->worldRect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldAngle", (size_t)(char*)(&sample->worldAngle) - (size_t)(char*)iobject, sample->worldAngle, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldBasis", (size_t)(char*)(&sample->worldBasis) - (size_t)(char*)iobject, sample->worldBasis, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldNonSizedBasis", (size_t)(char*)(&sample->worldNonSizedBasis) - (size_t)(char*)iobject, sample->worldNonSizedBasis, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldAABB", (size_t)(char*)(&sample->worldAABB) - (size_t)(char*)iobject, sample->worldAABB, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mWorldNonSizedTransform", (size_t)(char*)(&sample->mWorldNonSizedTransform) - (size_t)(char*)iobject, sample->mWorldNonSizedTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mWorldTransform", (size_t)(char*)(&sample->mWorldTransform) - (size_t)(char*)iobject, sample->mWorldTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mParentInvertedTransform", (size_t)(char*)(&sample->mParentInvertedTransform) - (size_t)(char*)iobject, sample->mParentInvertedTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mParentTransform", (size_t)(char*)(&sample->mParentTransform) - (size_t)(char*)iobject, sample->mParentTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsParentInvTransformActual", (size_t)(char*)(&sample->mIsParentInvTransformActual) - (size_t)(char*)iobject, sample->mIsParentInvTransformActual, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)iobject, sample->mOwner, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "actor", (size_t)(char*)(&sample->actor) - (size_t)(char*)iobject, sample->actor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabled", (size_t)(char*)(&sample->enabled) - (size_t)(char*)iobject, sample->enabled, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "enabledInHierarchy", (size_t)(char*)(&sample->enabledInHierarchy) - (size_t)(char*)iobject, sample->enabledInHierarchy, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)iobject, sample->mId, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)iobject, sample->mOwner, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEnabled", (size_t)(char*)(&sample->mEnabled) - (size_t)(char*)iobject, sample->mEnabled, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResEnabled", (size_t)(char*)(&sample->mResEnabled) - (size_t)(char*)iobject, sample->mResEnabled, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "drawDepth", (size_t)(char*)(&sample->drawDepth) - (size_t)(char*)iobject, sample->drawDepth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mDrawingDepth", (size_t)(char*)(&sample->mDrawingDepth) - (size_t)(char*)iobject, sample->mDrawingDepth, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)iobject, sample->mName, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mActors", (size_t)(char*)(&sample->mActors) - (size_t)(char*)iobject, sample->mActors, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "onTagAdded", (size_t)(char*)(&sample->onTagAdded) - (size_t)(char*)iobject, sample->onTagAdded, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onTagRemoved", (size_t)(char*)(&sample->onTagRemoved) - (size_t)(char*)iobject, sample->onTagRemoved, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTags", (size_t)(char*)(&sample->mTags) - (size_t)(char*)iobject, sample->mTags, o2::ProtectSection::Private).AddAttribute<SerializableAttribute>();
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mInteger", (size_t)(char*)(&sample->mInteger) - (size_t)(char*)iobject, sample->mInteger, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFloat", (size_t)(char*)(&sample->mFloat) - (size_t)(char*)iobject, sample->mFloat, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mString", (size_t)(char*)(&sample->mString) - (size_t)(char*)iobject, sample->mString, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWString", (size_t)(char*)(&sample->mWString) - (size_t)(char*)iobject, sample->mWString, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBool", (size_t)(char*)(&sample->mBool) - (size_t)(char*)iobject, sample->mBool, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mImageAsset", (size_t)(char*)(&sample->mImageAsset) - (size_t)(char*)iobject, sample->mImageAsset, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mActorAsset", (size_t)(char*)(&sample->mActorAsset) - (size_t)(char*)iobject, sample->mActorAsset, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDataAsset", (size_t)(char*)(&sample->mDataAsset) - (size_t)(char*)iobject, sample->mDataAsset, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mActor", (size_t)(char*)(&sample->mActor) - (size_t)(char*)iobject, sample->mActor, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTags", (size_t)(char*)(&sample->mTags) - (size_t)(char*)iobject, sample->mTags, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLayer", (size_t)(char*)(&sample->mLayer) - (size_t)(char*)iobject, sample->mLayer, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mComponent", (size_t)(char*)(&sample->mComponent) - (size_t)(char*)iobject, sample->mComponent, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mImageComponent", (size_t)(char*)(&sample->mImageComponent) - (size_t)(char*)iobject, sample->mImageComponent, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mColor", (size_t)(char*)(&sample->mColor) - (size_t)(char*)iobject, sample->mColor, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mVec2F", (size_t)(char*)(&sample->mVec2F) - (size_t)(char*)iobject, sample->mVec2F, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mVec2I", (size_t)(char*)(&sample->mVec2I) - (size_t)(char*)iobject, sample->mVec2I, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mVertex", (size_t)(char*)(&sample->mVertex) - (size_t)(char*)iobject, sample->mVertex, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mRectF", (size_t)(char*)(&sample->mRectF) - (size_t)(char*)iobject, sample->mRectF, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mRectI", (size_t)(char*)(&sample->mRectI) - (size_t)(char*)iobject, sample->mRectI, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::ImageComponent::InitializeType(o2::ImageComponent* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "texture", (size_t)(char*)(&sample->texture) - (size_t)(char*)iobject, sample->texture, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "textureSrcRect", (size_t)(char*)(&sample->textureSrcRect) - (size_t)(char*)iobject, sample->textureSrcRect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAssetId", (size_t)(char*)(&sample->imageAssetId) - (size_t)(char*)iobject, sample->imageAssetId, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAssetPath", (size_t)(char*)(&sample->imageAssetPath) - (size_t)(char*)iobject, sample->imageAssetPath, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "imageAsset", (size_t)(char*)(&sample->imageAsset) - (size_t)(char*)iobject, sample->imageAsset, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "bitmap", (size_t)(char*)(&sample->bitmap) - (size_t)(char*)iobject, sample->bitmap, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftTopColor", (size_t)(char*)(&sample->leftTopColor) - (size_t)(char*)iobject, sample->leftTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightTopColor", (size_t)(char*)(&sample->rightTopColor) - (size_t)(char*)iobject, sample->rightTopColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftBottomColor", (size_t)(char*)(&sample->leftBottomColor) - (size_t)(char*)iobject, sample->leftBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightBottomColor", (size_t)(char*)(&sample->rightBottomColor) - (size_t)(char*)iobject, sample->rightBottomColor, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mode", (size_t)(char*)(&sample->mode) - (size_t)(char*)iobject, sample->mode, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fill", (size_t)(char*)(&sample->fill) - (size_t)(char*)iobject, sample->fill, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "sliceBorder", (size_t)(char*)(&sample->sliceBorder) - (size_t)(char*)iobject, sample->sliceBorder, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mSprite", (size_t)(char*)(&sample->mSprite) - (size_t)(char*)iobject, sample->mSprite, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "caption", (size_t)(char*)(&sample->caption) - (size_t)(char*)iobject, sample->caption, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "icon", (size_t)(char*)(&sample->icon) - (size_t)(char*)iobject, sample->icon, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "buttonsGroup", (size_t)(char*)(&sample->buttonsGroup) - (size_t)(char*)iobject, sample->buttonsGroup, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)iobject, sample->onClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)iobject, sample->shortcut, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mCaptionText", (size_t)(char*)(&sample->mCaptionText) - (size_t)(char*)iobject, sample->mCaptionText, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIconSprite", (size_t)(char*)(&sample->mIconSprite) - (size_t)(char*)iobject, sample->mIconSprite, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mButtonGroup", (size_t)(char*)(&sample->mButtonGroup) - (size_t)(char*)iobject, sample->mButtonGroup, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIButton, bool>(&type, "IsFocusable", &o2::UIButton::IsFocusable, o2::ProtectSection::Public);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)iobject, sample->onClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)iobject, sample->shortcut, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mSubMenu", (size_t)(char*)(&sample->mSubMenu) - (size_t)(char*)iobject, sample->mSubMenu, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UIContextMenuItem, UIContextMenu*>(&type, "GetSubMenu", &o2::UIContextMenuItem::GetSubMenu, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenuItem, void, UIWidget*>(&type, "OnChildAdded", &o2::UIContextMenuItem::OnChildAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "child");
}

void o2::UIContextMenu::InitializeType(o2::UIContextMenu* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mOpenSubMenuDelay", (size_t)(char*)(&sample->mOpenSubMenuDelay) - (size_t)(char*)iobject, sample->mOpenSubMenuDelay, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFitSizeMin", (size_t)(char*)(&sample->mFitSizeMin) - (size_t)(char*)iobject, sample->mFitSizeMin, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxVisibleItems", (size_t)(char*)(&sample->mMaxVisibleItems) - (size_t)(char*)iobject, sample->mMaxVisibleItems, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mParentContextMenu", (size_t)(char*)(&sample->mParentContextMenu) - (size_t)(char*)iobject, sample->mParentContextMenu, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mChildContextMenu", (size_t)(char*)(&sample->mChildContextMenu) - (size_t)(char*)iobject, sample->mChildContextMenu, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mItemsLayout", (size_t)(char*)(&sample->mItemsLayout) - (size_t)(char*)iobject, sample->mItemsLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)iobject, sample->mItemSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSeparatorSample", (size_t)(char*)(&sample->mSeparatorSample) - (size_t)(char*)iobject, sample->mSeparatorSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)iobject, sample->mSelectionDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)iobject, sample->mSelectionLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurrentSelectionRect", (size_t)(char*)(&sample->mCurrentSelectionRect) - (size_t)(char*)iobject, sample->mCurrentSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetSelectionRect", (size_t)(char*)(&sample->mTargetSelectionRect) - (size_t)(char*)iobject, sample->mTargetSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)iobject, sample->mLastSelectCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedItem", (size_t)(char*)(&sample->mSelectedItem) - (size_t)(char*)iobject, sample->mSelectedItem, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectSubContextTime", (size_t)(char*)(&sample->mSelectSubContextTime) - (size_t)(char*)iobject, sample->mSelectSubContextTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mShownAtFrame", (size_t)(char*)(&sample->mShownAtFrame) - (size_t)(char*)iobject, sample->mShownAtFrame, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, bool, bool>(&type, "UpdateLayout", &o2::UIContextMenu::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
	funcInfo = TypeInitializer::RegFunction<o2::UIContextMenu, void, const RectF&>(&type, "CheckClipping", &o2::UIContextMenu::CheckClipping, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "clipArea");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "selectedItem", (size_t)(char*)(&sample->selectedItem) - (size_t)(char*)iobject, sample->selectedItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "selectedItemPos", (size_t)(char*)(&sample->selectedItemPos) - (size_t)(char*)iobject, sample->selectedItemPos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "item", (size_t)(char*)(&sample->item) - (size_t)(char*)iobject, sample->item, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "itemsCount", (size_t)(char*)(&sample->itemsCount) - (size_t)(char*)iobject, sample->itemsCount, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedPos", (size_t)(char*)(&sample->onSelectedPos) - (size_t)(char*)iobject, sample->onSelectedPos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedItem", (size_t)(char*)(&sample->onSelectedItem) - (size_t)(char*)iobject, sample->onSelectedItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mItemsList", (size_t)(char*)(&sample->mItemsList) - (size_t)(char*)iobject, sample->mItemsList, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mClipLayout", (size_t)(char*)(&sample->mClipLayout) - (size_t)(char*)iobject, sample->mClipLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAbsoluteClip", (size_t)(char*)(&sample->mAbsoluteClip) - (size_t)(char*)iobject, sample->mAbsoluteClip, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMaxListItems", (size_t)(char*)(&sample->mMaxListItems) - (size_t)(char*)iobject, sample->mMaxListItems, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
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
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void, bool, bool>(&type, "UpdateLayout", &o2::UICustomDropDown::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "OnItemSelected", &o2::UICustomDropDown::OnItemSelected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "OnSelectionChanged", &o2::UICustomDropDown::OnSelectionChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UICustomDropDown, void>(&type, "InitializeProperties", &o2::UICustomDropDown::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UICustomList::InitializeType(o2::UICustomList* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "selectedItems", (size_t)(char*)(&sample->selectedItems) - (size_t)(char*)iobject, sample->selectedItems, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "selectedItem", (size_t)(char*)(&sample->selectedItem) - (size_t)(char*)iobject, sample->selectedItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "selectedItemPos", (size_t)(char*)(&sample->selectedItemPos) - (size_t)(char*)iobject, sample->selectedItemPos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "item", (size_t)(char*)(&sample->item) - (size_t)(char*)iobject, sample->item, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "itemsCount", (size_t)(char*)(&sample->itemsCount) - (size_t)(char*)iobject, sample->itemsCount, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedPos", (size_t)(char*)(&sample->onSelectedPos) - (size_t)(char*)iobject, sample->onSelectedPos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedItem", (size_t)(char*)(&sample->onSelectedItem) - (size_t)(char*)iobject, sample->onSelectedItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mVerLayout", (size_t)(char*)(&sample->mVerLayout) - (size_t)(char*)iobject, sample->mVerLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)iobject, sample->mItemSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)iobject, sample->mSelectionDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverDrawable", (size_t)(char*)(&sample->mHoverDrawable) - (size_t)(char*)iobject, sample->mHoverDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)iobject, sample->mSelectionLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverLayout", (size_t)(char*)(&sample->mHoverLayout) - (size_t)(char*)iobject, sample->mHoverLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMultiSelection", (size_t)(char*)(&sample->mMultiSelection) - (size_t)(char*)iobject, sample->mMultiSelection, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectedItems", (size_t)(char*)(&sample->mSelectedItems) - (size_t)(char*)iobject, sample->mSelectedItems, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentHoverRect", (size_t)(char*)(&sample->mCurrentHoverRect) - (size_t)(char*)iobject, sample->mCurrentHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetHoverRect", (size_t)(char*)(&sample->mTargetHoverRect) - (size_t)(char*)iobject, sample->mTargetHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastHoverCheckCursor", (size_t)(char*)(&sample->mLastHoverCheckCursor) - (size_t)(char*)iobject, sample->mLastHoverCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)iobject, sample->mLastSelectCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionSpritesPool", (size_t)(char*)(&sample->mSelectionSpritesPool) - (size_t)(char*)iobject, sample->mSelectionSpritesPool, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UICustomList, void, bool, bool>(&type, "UpdateLayout", &o2::UICustomList::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "textItem", (size_t)(char*)(&sample->textItem) - (size_t)(char*)iobject, sample->textItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedText", (size_t)(char*)(&sample->onSelectedText) - (size_t)(char*)iobject, sample->onSelectedText, o2::ProtectSection::Public);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)iobject, sample->text, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "caret", (size_t)(char*)(&sample->caret) - (size_t)(char*)iobject, sample->caret, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "selectionBegin", (size_t)(char*)(&sample->selectionBegin) - (size_t)(char*)iobject, sample->selectionBegin, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "selectionEnd", (size_t)(char*)(&sample->selectionEnd) - (size_t)(char*)iobject, sample->selectionEnd, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChanged", (size_t)(char*)(&sample->onChanged) - (size_t)(char*)iobject, sample->onChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChangeCompleted", (size_t)(char*)(&sample->onChangeCompleted) - (size_t)(char*)iobject, sample->onChangeCompleted, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mLastText", (size_t)(char*)(&sample->mLastText) - (size_t)(char*)iobject, sample->mLastText, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mText", (size_t)(char*)(&sample->mText) - (size_t)(char*)iobject, sample->mText, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAvailableSymbols", (size_t)(char*)(&sample->mAvailableSymbols) - (size_t)(char*)iobject, sample->mAvailableSymbols, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTextDrawable", (size_t)(char*)(&sample->mTextDrawable) - (size_t)(char*)iobject, sample->mTextDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionMesh", (size_t)(char*)(&sample->mSelectionMesh) - (size_t)(char*)iobject, sample->mSelectionMesh, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCaretDrawable", (size_t)(char*)(&sample->mCaretDrawable) - (size_t)(char*)iobject, sample->mCaretDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCaretBlinkDelay", (size_t)(char*)(&sample->mCaretBlinkDelay) - (size_t)(char*)iobject, sample->mCaretBlinkDelay, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCaretBlinkTime", (size_t)(char*)(&sample->mCaretBlinkTime) - (size_t)(char*)iobject, sample->mCaretBlinkTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionBegin", (size_t)(char*)(&sample->mSelectionBegin) - (size_t)(char*)iobject, sample->mSelectionBegin, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionEnd", (size_t)(char*)(&sample->mSelectionEnd) - (size_t)(char*)iobject, sample->mSelectionEnd, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionColor", (size_t)(char*)(&sample->mSelectionColor) - (size_t)(char*)iobject, sample->mSelectionColor, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectingByWords", (size_t)(char*)(&sample->mSelectingByWords) - (size_t)(char*)iobject, sample->mSelectingByWords, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelWordBegin", (size_t)(char*)(&sample->mSelWordBegin) - (size_t)(char*)iobject, sample->mSelWordBegin, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelWordEnd", (size_t)(char*)(&sample->mSelWordEnd) - (size_t)(char*)iobject, sample->mSelWordEnd, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMultiLine", (size_t)(char*)(&sample->mMultiLine) - (size_t)(char*)iobject, sample->mMultiLine, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWordWrap", (size_t)(char*)(&sample->mWordWrap) - (size_t)(char*)iobject, sample->mWordWrap, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxLineChars", (size_t)(char*)(&sample->mMaxLineChars) - (size_t)(char*)iobject, sample->mMaxLineChars, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxLinesCount", (size_t)(char*)(&sample->mMaxLinesCount) - (size_t)(char*)iobject, sample->mMaxLinesCount, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mJustSelected", (size_t)(char*)(&sample->mJustSelected) - (size_t)(char*)iobject, sample->mJustSelected, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastClickTime", (size_t)(char*)(&sample->mLastClickTime) - (size_t)(char*)iobject, sample->mLastClickTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastCursorPos", (size_t)(char*)(&sample->mLastCursorPos) - (size_t)(char*)iobject, sample->mLastCursorPos, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, bool>(&type, "IsFocusable", &o2::UIEditBox::IsFocusable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, float>(&type, "UpdateControls", &o2::UIEditBox::UpdateControls, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "OnVisibleChanged", &o2::UIEditBox::OnVisibleChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "OnFocused", &o2::UIEditBox::OnFocused, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void>(&type, "OnUnfocused", &o2::UIEditBox::OnUnfocused, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, const Input::Cursor&>(&type, "OnCursorRightMouseStillDown", &o2::UIEditBox::OnCursorRightMouseStillDown, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIEditBox, void, bool, bool>(&type, "UpdateLayout", &o2::UIEditBox::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "baseCorner", (size_t)(char*)(&sample->baseCorner) - (size_t)(char*)iobject, sample->baseCorner, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "cellSize", (size_t)(char*)(&sample->cellSize) - (size_t)(char*)iobject, sample->cellSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "arrangeAxisMaxCells", (size_t)(char*)(&sample->arrangeAxisMaxCells) - (size_t)(char*)iobject, sample->arrangeAxisMaxCells, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "arrangeAxis", (size_t)(char*)(&sample->arrangeAxis) - (size_t)(char*)iobject, sample->arrangeAxis, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "spacing", (size_t)(char*)(&sample->spacing) - (size_t)(char*)iobject, sample->spacing, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "border", (size_t)(char*)(&sample->border) - (size_t)(char*)iobject, sample->border, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderLeft", (size_t)(char*)(&sample->borderLeft) - (size_t)(char*)iobject, sample->borderLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderRight", (size_t)(char*)(&sample->borderRight) - (size_t)(char*)iobject, sample->borderRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderTop", (size_t)(char*)(&sample->borderTop) - (size_t)(char*)iobject, sample->borderTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderBottom", (size_t)(char*)(&sample->borderBottom) - (size_t)(char*)iobject, sample->borderBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fitByChildren", (size_t)(char*)(&sample->fitByChildren) - (size_t)(char*)iobject, sample->fitByChildren, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mBaseCorner", (size_t)(char*)(&sample->mBaseCorner) - (size_t)(char*)iobject, sample->mBaseCorner, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSpacing", (size_t)(char*)(&sample->mSpacing) - (size_t)(char*)iobject, sample->mSpacing, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCellSize", (size_t)(char*)(&sample->mCellSize) - (size_t)(char*)iobject, sample->mCellSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mArrangeAxis", (size_t)(char*)(&sample->mArrangeAxis) - (size_t)(char*)iobject, sample->mArrangeAxis, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mArrangeAxisMaxCells", (size_t)(char*)(&sample->mArrangeAxisMaxCells) - (size_t)(char*)iobject, sample->mArrangeAxisMaxCells, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)iobject, sample->mBorder, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFitByChildren", (size_t)(char*)(&sample->mFitByChildren) - (size_t)(char*)iobject, sample->mFitByChildren, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
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
	funcInfo = TypeInitializer::RegFunction<o2::UIGridLayout, void, bool, bool>(&type, "UpdateLayout", &o2::UIGridLayout::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "baseCorner", (size_t)(char*)(&sample->baseCorner) - (size_t)(char*)iobject, sample->baseCorner, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "spacing", (size_t)(char*)(&sample->spacing) - (size_t)(char*)iobject, sample->spacing, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "border", (size_t)(char*)(&sample->border) - (size_t)(char*)iobject, sample->border, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderLeft", (size_t)(char*)(&sample->borderLeft) - (size_t)(char*)iobject, sample->borderLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderRight", (size_t)(char*)(&sample->borderRight) - (size_t)(char*)iobject, sample->borderRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderTop", (size_t)(char*)(&sample->borderTop) - (size_t)(char*)iobject, sample->borderTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderBottom", (size_t)(char*)(&sample->borderBottom) - (size_t)(char*)iobject, sample->borderBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "expandWidth", (size_t)(char*)(&sample->expandWidth) - (size_t)(char*)iobject, sample->expandWidth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "expandHeight", (size_t)(char*)(&sample->expandHeight) - (size_t)(char*)iobject, sample->expandHeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fitByChildren", (size_t)(char*)(&sample->fitByChildren) - (size_t)(char*)iobject, sample->fitByChildren, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mBaseCorner", (size_t)(char*)(&sample->mBaseCorner) - (size_t)(char*)iobject, sample->mBaseCorner, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSpacing", (size_t)(char*)(&sample->mSpacing) - (size_t)(char*)iobject, sample->mSpacing, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)iobject, sample->mBorder, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mExpandWidth", (size_t)(char*)(&sample->mExpandWidth) - (size_t)(char*)iobject, sample->mExpandWidth, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mExpandHeight", (size_t)(char*)(&sample->mExpandHeight) - (size_t)(char*)iobject, sample->mExpandHeight, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFitByChildren", (size_t)(char*)(&sample->mFitByChildren) - (size_t)(char*)iobject, sample->mFitByChildren, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
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
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalLayout, void, bool, bool>(&type, "UpdateLayout", &o2::UIHorizontalLayout::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)iobject, sample->minValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)iobject, sample->maxValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)iobject, sample->scrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)iobject, sample->onChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)iobject, sample->mValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)iobject, sample->mSmoothValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)iobject, sample->mMinValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)iobject, sample->mMaxValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)iobject, sample->mScrollSense, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOrientation", (size_t)(char*)(&sample->mOrientation) - (size_t)(char*)iobject, sample->mOrientation, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBarLayer", (size_t)(char*)(&sample->mBarLayer) - (size_t)(char*)iobject, sample->mBarLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)iobject, sample->mBackLayer, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalProgress, void, bool, bool>(&type, "UpdateLayout", &o2::UIHorizontalProgress::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)iobject, sample->minValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)iobject, sample->maxValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)iobject, sample->scrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSize", (size_t)(char*)(&sample->scrollSize) - (size_t)(char*)iobject, sample->scrollSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)iobject, sample->onChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSmoothChange", (size_t)(char*)(&sample->onSmoothChange) - (size_t)(char*)iobject, sample->onSmoothChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)iobject, sample->mValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)iobject, sample->mSmoothValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)iobject, sample->mMinValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)iobject, sample->mMaxValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)iobject, sample->mScrollSense, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollHandleSize", (size_t)(char*)(&sample->mScrollHandleSize) - (size_t)(char*)iobject, sample->mScrollHandleSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollhandleMinPxSize", (size_t)(char*)(&sample->mScrollhandleMinPxSize) - (size_t)(char*)iobject, sample->mScrollhandleMinPxSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mPressHandleOffset", (size_t)(char*)(&sample->mPressHandleOffset) - (size_t)(char*)iobject, sample->mPressHandleOffset, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandlePressed", (size_t)(char*)(&sample->mHandlePressed) - (size_t)(char*)iobject, sample->mHandlePressed, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandleLayer", (size_t)(char*)(&sample->mHandleLayer) - (size_t)(char*)iobject, sample->mHandleLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)iobject, sample->mBackLayer, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIHorizontalScrollBar, void, bool, bool>(&type, "UpdateLayout", &o2::UIHorizontalScrollBar::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "font", (size_t)(char*)(&sample->font) - (size_t)(char*)iobject, sample->font, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)iobject, sample->text, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "verAlign", (size_t)(char*)(&sample->verAlign) - (size_t)(char*)iobject, sample->verAlign, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "horAlign", (size_t)(char*)(&sample->horAlign) - (size_t)(char*)iobject, sample->horAlign, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "horOverflow", (size_t)(char*)(&sample->horOverflow) - (size_t)(char*)iobject, sample->horOverflow, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "verOverflow", (size_t)(char*)(&sample->verOverflow) - (size_t)(char*)iobject, sample->verOverflow, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "expandBorder", (size_t)(char*)(&sample->expandBorder) - (size_t)(char*)iobject, sample->expandBorder, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "symbolsDistanceCoef", (size_t)(char*)(&sample->symbolsDistanceCoef) - (size_t)(char*)iobject, sample->symbolsDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "linesDistanceCoef", (size_t)(char*)(&sample->linesDistanceCoef) - (size_t)(char*)iobject, sample->linesDistanceCoef, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTextLayer", (size_t)(char*)(&sample->mTextLayer) - (size_t)(char*)iobject, sample->mTextLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHorOverflow", (size_t)(char*)(&sample->mHorOverflow) - (size_t)(char*)iobject, sample->mHorOverflow, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mVerOverflow", (size_t)(char*)(&sample->mVerOverflow) - (size_t)(char*)iobject, sample->mVerOverflow, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandBorder", (size_t)(char*)(&sample->mExpandBorder) - (size_t)(char*)iobject, sample->mExpandBorder, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
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
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, bool, bool>(&type, "UpdateLayout", &o2::UILabel::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UILabel::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UILabel, void>(&type, "InitializeProperties", &o2::UILabel::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIList::InitializeType(o2::UIList* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "values", (size_t)(char*)(&sample->values) - (size_t)(char*)iobject, sample->values, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "textItem", (size_t)(char*)(&sample->textItem) - (size_t)(char*)iobject, sample->textItem, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSelectedText", (size_t)(char*)(&sample->onSelectedText) - (size_t)(char*)iobject, sample->onSelectedText, o2::ProtectSection::Public);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "selectedItemPos", (size_t)(char*)(&sample->selectedItemPos) - (size_t)(char*)iobject, sample->selectedItemPos, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onFocused", (size_t)(char*)(&sample->onFocused) - (size_t)(char*)iobject, sample->onFocused, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "getItemsCountFunc", (size_t)(char*)(&sample->getItemsCountFunc) - (size_t)(char*)iobject, sample->getItemsCountFunc, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "getItemsRangeFunc", (size_t)(char*)(&sample->getItemsRangeFunc) - (size_t)(char*)iobject, sample->getItemsRangeFunc, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "setupItemFunc", (size_t)(char*)(&sample->setupItemFunc) - (size_t)(char*)iobject, sample->setupItemFunc, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)iobject, sample->mItemSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)iobject, sample->mSelectionDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverDrawable", (size_t)(char*)(&sample->mHoverDrawable) - (size_t)(char*)iobject, sample->mHoverDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)iobject, sample->mSelectionLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverLayout", (size_t)(char*)(&sample->mHoverLayout) - (size_t)(char*)iobject, sample->mHoverLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMinVisibleItemIdx", (size_t)(char*)(&sample->mMinVisibleItemIdx) - (size_t)(char*)iobject, sample->mMinVisibleItemIdx, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMaxVisibleItemIdx", (size_t)(char*)(&sample->mMaxVisibleItemIdx) - (size_t)(char*)iobject, sample->mMaxVisibleItemIdx, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedItem", (size_t)(char*)(&sample->mSelectedItem) - (size_t)(char*)iobject, sample->mSelectedItem, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentSelectionRect", (size_t)(char*)(&sample->mCurrentSelectionRect) - (size_t)(char*)iobject, sample->mCurrentSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetSelectionRect", (size_t)(char*)(&sample->mTargetSelectionRect) - (size_t)(char*)iobject, sample->mTargetSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCurrentHoverRect", (size_t)(char*)(&sample->mCurrentHoverRect) - (size_t)(char*)iobject, sample->mCurrentHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetHoverRect", (size_t)(char*)(&sample->mTargetHoverRect) - (size_t)(char*)iobject, sample->mTargetHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastHoverCheckCursor", (size_t)(char*)(&sample->mLastHoverCheckCursor) - (size_t)(char*)iobject, sample->mLastHoverCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)iobject, sample->mLastSelectCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mItemsPool", (size_t)(char*)(&sample->mItemsPool) - (size_t)(char*)iobject, sample->mItemsPool, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, bool, bool>(&type, "UpdateLayout", &o2::UILongList::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
	funcInfo = TypeInitializer::RegFunction<o2::UILongList, void, const Vec2F&>(&type, "MoveScrollPosition", &o2::UILongList::MoveScrollPosition, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mLayout", (size_t)(char*)(&sample->mLayout) - (size_t)(char*)iobject, sample->mLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mItemSample", (size_t)(char*)(&sample->mItemSample) - (size_t)(char*)iobject, sample->mItemSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mClickFunctions", (size_t)(char*)(&sample->mClickFunctions) - (size_t)(char*)iobject, sample->mClickFunctions, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectionDrawable", (size_t)(char*)(&sample->mSelectionDrawable) - (size_t)(char*)iobject, sample->mSelectionDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSelectionLayout", (size_t)(char*)(&sample->mSelectionLayout) - (size_t)(char*)iobject, sample->mSelectionLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurrentSelectionRect", (size_t)(char*)(&sample->mCurrentSelectionRect) - (size_t)(char*)iobject, sample->mCurrentSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetSelectionRect", (size_t)(char*)(&sample->mTargetSelectionRect) - (size_t)(char*)iobject, sample->mTargetSelectionRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastSelectCheckCursor", (size_t)(char*)(&sample->mLastSelectCheckCursor) - (size_t)(char*)iobject, sample->mLastSelectCheckCursor, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedItem", (size_t)(char*)(&sample->mSelectedItem) - (size_t)(char*)iobject, sample->mSelectedItem, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectSubContextTime", (size_t)(char*)(&sample->mSelectSubContextTime) - (size_t)(char*)iobject, sample->mSelectSubContextTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOpenedContext", (size_t)(char*)(&sample->mOpenedContext) - (size_t)(char*)iobject, sample->mOpenedContext, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "scroll", (size_t)(char*)(&sample->scroll) - (size_t)(char*)iobject, sample->scroll, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "horScroll", (size_t)(char*)(&sample->horScroll) - (size_t)(char*)iobject, sample->horScroll, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "verScroll", (size_t)(char*)(&sample->verScroll) - (size_t)(char*)iobject, sample->verScroll, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onScrolled", (size_t)(char*)(&sample->onScrolled) - (size_t)(char*)iobject, sample->onScrolled, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mHorScrollBar", (size_t)(char*)(&sample->mHorScrollBar) - (size_t)(char*)iobject, sample->mHorScrollBar, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVerScrollBar", (size_t)(char*)(&sample->mVerScrollBar) - (size_t)(char*)iobject, sample->mVerScrollBar, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOwnHorScrollBar", (size_t)(char*)(&sample->mOwnHorScrollBar) - (size_t)(char*)iobject, sample->mOwnHorScrollBar, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOwnVerScrollBar", (size_t)(char*)(&sample->mOwnVerScrollBar) - (size_t)(char*)iobject, sample->mOwnVerScrollBar, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mViewAreaLayout", (size_t)(char*)(&sample->mViewAreaLayout) - (size_t)(char*)iobject, sample->mViewAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAbsoluteViewArea", (size_t)(char*)(&sample->mAbsoluteViewArea) - (size_t)(char*)iobject, sample->mAbsoluteViewArea, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mClipAreaLayout", (size_t)(char*)(&sample->mClipAreaLayout) - (size_t)(char*)iobject, sample->mClipAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAbsoluteClipArea", (size_t)(char*)(&sample->mAbsoluteClipArea) - (size_t)(char*)iobject, sample->mAbsoluteClipArea, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mScrollPos", (size_t)(char*)(&sample->mScrollPos) - (size_t)(char*)iobject, sample->mScrollPos, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollSpeed", (size_t)(char*)(&sample->mScrollSpeed) - (size_t)(char*)iobject, sample->mScrollSpeed, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mScrollSpeedDamp", (size_t)(char*)(&sample->mScrollSpeedDamp) - (size_t)(char*)iobject, sample->mScrollSpeedDamp, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollArea", (size_t)(char*)(&sample->mScrollArea) - (size_t)(char*)iobject, sample->mScrollArea, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mScrollRange", (size_t)(char*)(&sample->mScrollRange) - (size_t)(char*)iobject, sample->mScrollRange, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEnableHorScroll", (size_t)(char*)(&sample->mEnableHorScroll) - (size_t)(char*)iobject, sample->mEnableHorScroll, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void>(&type, "Draw", &o2::UIScrollArea::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, float>(&type, "Update", &o2::UIScrollArea::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, const Vec2F&>(&type, "SetScroll", &o2::UIScrollArea::SetScroll, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "scroll");
	funcInfo = TypeInitializer::RegFunction<o2::UIScrollArea, void, const Vec2F&>(&type, "SetScrollForcible", &o2::UIScrollArea::SetScrollForcible, o2::ProtectSection::Public);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "caption", (size_t)(char*)(&sample->caption) - (size_t)(char*)iobject, sample->caption, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "toggleGroup", (size_t)(char*)(&sample->toggleGroup) - (size_t)(char*)iobject, sample->toggleGroup, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)iobject, sample->onClick, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onToggle", (size_t)(char*)(&sample->onToggle) - (size_t)(char*)iobject, sample->onToggle, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onToggleByUser", (size_t)(char*)(&sample->onToggleByUser) - (size_t)(char*)iobject, sample->onToggleByUser, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)iobject, sample->shortcut, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)iobject, sample->mValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValueUnknown", (size_t)(char*)(&sample->mValueUnknown) - (size_t)(char*)iobject, sample->mValueUnknown, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCaptionText", (size_t)(char*)(&sample->mCaptionText) - (size_t)(char*)iobject, sample->mCaptionText, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)iobject, sample->mBackLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mToggleGroup", (size_t)(char*)(&sample->mToggleGroup) - (size_t)(char*)iobject, sample->mToggleGroup, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIToggle, bool>(&type, "IsFocusable", &o2::UIToggle::IsFocusable, o2::ProtectSection::Public);
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

void o2::UITree::InitializeType(o2::UITree* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "getObjectParentDelegate", (size_t)(char*)(&sample->getObjectParentDelegate) - (size_t)(char*)iobject, sample->getObjectParentDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "getObjectChildrenDelegate", (size_t)(char*)(&sample->getObjectChildrenDelegate) - (size_t)(char*)iobject, sample->getObjectChildrenDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fillNodeDataByObjectDelegate", (size_t)(char*)(&sample->fillNodeDataByObjectDelegate) - (size_t)(char*)iobject, sample->fillNodeDataByObjectDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "getDebugForObject", (size_t)(char*)(&sample->getDebugForObject) - (size_t)(char*)iobject, sample->getDebugForObject, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onNodeClicked", (size_t)(char*)(&sample->onNodeClicked) - (size_t)(char*)iobject, sample->onNodeClicked, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onNodeDoubleClicked", (size_t)(char*)(&sample->onNodeDoubleClicked) - (size_t)(char*)iobject, sample->onNodeDoubleClicked, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onNodeRightButtonClicked", (size_t)(char*)(&sample->onNodeRightButtonClicked) - (size_t)(char*)iobject, sample->onNodeRightButtonClicked, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onObjectsSelectionChanged", (size_t)(char*)(&sample->onObjectsSelectionChanged) - (size_t)(char*)iobject, sample->onObjectsSelectionChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onDraggedObjects", (size_t)(char*)(&sample->onDraggedObjects) - (size_t)(char*)iobject, sample->onDraggedObjects, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mRearrangeType", (size_t)(char*)(&sample->mRearrangeType) - (size_t)(char*)iobject, sample->mRearrangeType, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMultiSelectAvailable", (size_t)(char*)(&sample->mMultiSelectAvailable) - (size_t)(char*)iobject, sample->mMultiSelectAvailable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mNodeWidgetSample", (size_t)(char*)(&sample->mNodeWidgetSample) - (size_t)(char*)iobject, sample->mNodeWidgetSample, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mChildsOffset", (size_t)(char*)(&sample->mChildsOffset) - (size_t)(char*)iobject, sample->mChildsOffset, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mIsNeedUpdateView", (size_t)(char*)(&sample->mIsNeedUpdateView) - (size_t)(char*)iobject, sample->mIsNeedUpdateView, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsNeedUdateLayout", (size_t)(char*)(&sample->mIsNeedUdateLayout) - (size_t)(char*)iobject, sample->mIsNeedUdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsNeedUpdateVisibleNodes", (size_t)(char*)(&sample->mIsNeedUpdateVisibleNodes) - (size_t)(char*)iobject, sample->mIsNeedUpdateVisibleNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAllNodes", (size_t)(char*)(&sample->mAllNodes) - (size_t)(char*)iobject, sample->mAllNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedObjects", (size_t)(char*)(&sample->mSelectedObjects) - (size_t)(char*)iobject, sample->mSelectedObjects, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedNodes", (size_t)(char*)(&sample->mSelectedNodes) - (size_t)(char*)iobject, sample->mSelectedNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNodeWidgetsBuf", (size_t)(char*)(&sample->mNodeWidgetsBuf) - (size_t)(char*)iobject, sample->mNodeWidgetsBuf, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNodesBuf", (size_t)(char*)(&sample->mNodesBuf) - (size_t)(char*)iobject, sample->mNodesBuf, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVisibleNodes", (size_t)(char*)(&sample->mVisibleNodes) - (size_t)(char*)iobject, sample->mVisibleNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMinVisibleNodeIdx", (size_t)(char*)(&sample->mMinVisibleNodeIdx) - (size_t)(char*)iobject, sample->mMinVisibleNodeIdx, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMaxVisibleNodeIdx", (size_t)(char*)(&sample->mMaxVisibleNodeIdx) - (size_t)(char*)iobject, sample->mMaxVisibleNodeIdx, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastClickPos", (size_t)(char*)(&sample->mLastClickPos) - (size_t)(char*)iobject, sample->mLastClickPos, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHoveredItem", (size_t)(char*)(&sample->mHoveredItem) - (size_t)(char*)iobject, sample->mHoveredItem, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHoverDrawable", (size_t)(char*)(&sample->mHoverDrawable) - (size_t)(char*)iobject, sample->mHoverDrawable, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHoverLayout", (size_t)(char*)(&sample->mHoverLayout) - (size_t)(char*)iobject, sample->mHoverLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurrentHoverRect", (size_t)(char*)(&sample->mCurrentHoverRect) - (size_t)(char*)iobject, sample->mCurrentHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetHoverRect", (size_t)(char*)(&sample->mTargetHoverRect) - (size_t)(char*)iobject, sample->mTargetHoverRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsDraggingNodes", (size_t)(char*)(&sample->mIsDraggingNodes) - (size_t)(char*)iobject, sample->mIsDraggingNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFakeDragNode", (size_t)(char*)(&sample->mFakeDragNode) - (size_t)(char*)iobject, sample->mFakeDragNode, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragOffset", (size_t)(char*)(&sample->mDragOffset) - (size_t)(char*)iobject, sample->mDragOffset, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mInsertNodeCandidate", (size_t)(char*)(&sample->mInsertNodeCandidate) - (size_t)(char*)iobject, sample->mInsertNodeCandidate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBeforeDragSelectedItems", (size_t)(char*)(&sample->mBeforeDragSelectedItems) - (size_t)(char*)iobject, sample->mBeforeDragSelectedItems, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDragEnded", (size_t)(char*)(&sample->mDragEnded) - (size_t)(char*)iobject, sample->mDragEnded, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandedObjects", (size_t)(char*)(&sample->mExpandedObjects) - (size_t)(char*)iobject, sample->mExpandedObjects, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandingNodeState", (size_t)(char*)(&sample->mExpandingNodeState) - (size_t)(char*)iobject, sample->mExpandingNodeState, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandingNodeIdx", (size_t)(char*)(&sample->mExpandingNodeIdx) - (size_t)(char*)iobject, sample->mExpandingNodeIdx, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandingNodeChildsCount", (size_t)(char*)(&sample->mExpandingNodeChildsCount) - (size_t)(char*)iobject, sample->mExpandingNodeChildsCount, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandingNodePosition", (size_t)(char*)(&sample->mExpandingNodePosition) - (size_t)(char*)iobject, sample->mExpandingNodePosition, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandingNodeBottomPosition", (size_t)(char*)(&sample->mExpandingNodeBottomPosition) - (size_t)(char*)iobject, sample->mExpandingNodeBottomPosition, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandingNodeCurrCoef", (size_t)(char*)(&sample->mExpandingNodeCurrCoef) - (size_t)(char*)iobject, sample->mExpandingNodeCurrCoef, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandingNodeCurrHeight", (size_t)(char*)(&sample->mExpandingNodeCurrHeight) - (size_t)(char*)iobject, sample->mExpandingNodeCurrHeight, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandingNodeTargetHeight", (size_t)(char*)(&sample->mExpandingNodeTargetHeight) - (size_t)(char*)iobject, sample->mExpandingNodeTargetHeight, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandNodeTime", (size_t)(char*)(&sample->mExpandNodeTime) - (size_t)(char*)iobject, sample->mExpandNodeTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandingNodeFunc", (size_t)(char*)(&sample->mExpandingNodeFunc) - (size_t)(char*)iobject, sample->mExpandingNodeFunc, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandNodeCandidate", (size_t)(char*)(&sample->mExpandNodeCandidate) - (size_t)(char*)iobject, sample->mExpandNodeCandidate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandInsertTime", (size_t)(char*)(&sample->mExpandInsertTime) - (size_t)(char*)iobject, sample->mExpandInsertTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPressedTime", (size_t)(char*)(&sample->mPressedTime) - (size_t)(char*)iobject, sample->mPressedTime, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNodeExpandTime", (size_t)(char*)(&sample->mNodeExpandTime) - (size_t)(char*)iobject, sample->mNodeExpandTime, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mNodeDragIntoZone", (size_t)(char*)(&sample->mNodeDragIntoZone) - (size_t)(char*)iobject, sample->mNodeDragIntoZone, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHightlightAnim", (size_t)(char*)(&sample->mHightlightAnim) - (size_t)(char*)iobject, sample->mHightlightAnim, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHightlightSprite", (size_t)(char*)(&sample->mHightlightSprite) - (size_t)(char*)iobject, sample->mHightlightSprite, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHightlightLayout", (size_t)(char*)(&sample->mHightlightLayout) - (size_t)(char*)iobject, sample->mHightlightLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHightlighNode", (size_t)(char*)(&sample->mHightlighNode) - (size_t)(char*)iobject, sample->mHightlighNode, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVisibleWidgetsCache", (size_t)(char*)(&sample->mVisibleWidgetsCache) - (size_t)(char*)iobject, sample->mVisibleWidgetsCache, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownPtr, UnknownPtr>(&type, "OnObjectCreated", &o2::UITree::OnObjectCreated, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "parent");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownPtr>(&type, "OnObjectRemoved", &o2::UITree::OnObjectRemoved, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const UnknownPtrsVec&>(&type, "OnObjectsChanged", &o2::UITree::OnObjectsChanged, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const UnknownPtrsVec&>(funcInfo, "objects");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "Draw", &o2::UITree::Draw, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, float>(&type, "Update", &o2::UITree::Update, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, bool>(&type, "UpdateNodesView", &o2::UITree::UpdateNodesView, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "immediately");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*, UnknownPtr>(&type, "GetNode", &o2::UITree::GetNode, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "ExpandAll", &o2::UITree::ExpandAll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "CollapseAll", &o2::UITree::CollapseAll, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Vector<UnknownPtr>>(&type, "GetSelectedObjects", &o2::UITree::GetSelectedObjects, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const UnknownPtrsVec&>(&type, "SetSelectedObjects", &o2::UITree::SetSelectedObjects, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const UnknownPtrsVec&>(funcInfo, "objects");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownPtr>(&type, "SelectObject", &o2::UITree::SelectObject, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownPtr>(&type, "SelectAndHightlightObject", &o2::UITree::SelectAndHightlightObject, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownPtr>(&type, "DeselectObject", &o2::UITree::DeselectObject, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "DeselectAllObjects", &o2::UITree::DeselectAllObjects, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownPtr>(&type, "ScrollTo", &o2::UITree::ScrollTo, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownPtr>(&type, "ScrollToAndHightlight", &o2::UITree::ScrollToAndHightlight, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownPtr>(&type, "ExpandParentObjects", &o2::UITree::ExpandParentObjects, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*, const Vec2F&>(&type, "GetTreeNodeUnderPoint", &o2::UITree::GetTreeNodeUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, RearrangeType>(&type, "SetRearrangeType", &o2::UITree::SetRearrangeType, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<RearrangeType>(funcInfo, "type");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, RearrangeType>(&type, "GetRearrangeType", &o2::UITree::GetRearrangeType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, bool>(&type, "SetMultipleSelectionAvailable", &o2::UITree::SetMultipleSelectionAvailable, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "available");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool>(&type, "IsMultiSelectionAvailable", &o2::UITree::IsMultiSelectionAvailable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*>(&type, "GetNodeSample", &o2::UITree::GetNodeSample, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Sprite*>(&type, "GetHoverDrawable", &o2::UITree::GetHoverDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Layout&>(&type, "SetHoverLayout", &o2::UITree::SetHoverLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Sprite*>(&type, "GetHightlightDrawable", &o2::UITree::GetHightlightDrawable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Animation&>(&type, "SetHightlightAnimation", &o2::UITree::SetHightlightAnimation, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Animation&>(funcInfo, "animation");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Layout&>(&type, "SetHightlightLayout", &o2::UITree::SetHightlightLayout, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Layout&>(funcInfo, "layout");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, float>(&type, "SetNodeExpandTimer", &o2::UITree::SetNodeExpandTimer, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "time");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, float>(&type, "GetNodeExpandTimer", &o2::UITree::GetNodeExpandTimer, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, float>(&type, "SetChildsNodesOffset", &o2::UITree::SetChildsNodesOffset, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<float>(funcInfo, "offset");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, float>(&type, "GetChildsNodesOffset", &o2::UITree::GetChildsNodesOffset, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool>(&type, "IsScrollable", &o2::UITree::IsScrollable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool>(&type, "IsFocusable", &o2::UITree::IsFocusable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UITree::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UnknownPtr, UnknownPtr>(&type, "GetObjectParent", &o2::UITree::GetObjectParent, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Vector<UnknownPtr>, UnknownPtr>(&type, "GetObjectChilds", &o2::UITree::GetObjectChilds, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, String, UnknownPtr>(&type, "GetObjectDebug", &o2::UITree::GetObjectDebug, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UITreeNode*, UnknownPtr>(&type, "FillNodeDataByObject", &o2::UITree::FillNodeDataByObject, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "nodeWidget");
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UITreeNode*>(&type, "OnNodeClick", &o2::UITree::OnNodeClick, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "nodeWidget");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UITreeNode*>(&type, "OnNodeDblClick", &o2::UITree::OnNodeDblClick, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "nodeWidget");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UITreeNode*>(&type, "OnNodeRBClick", &o2::UITree::OnNodeRBClick, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "nodeWidget");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownPtrsVec>(&type, "OnNodesSelectionChanged", &o2::UITree::OnNodesSelectionChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UnknownPtrsVec>(funcInfo, "objects");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UnknownPtrsVec, UnknownPtr, UnknownPtr>(&type, "OnDraggedObjects", &o2::UITree::OnDraggedObjects, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UnknownPtrsVec>(funcInfo, "objects");
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "newParent");
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "prevObject");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, SelectDragObjectsVec>(&type, "GetSelectedDragObjects", &o2::UITree::GetSelectedDragObjects, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, SelectDragObjectsVec>(&type, "GetAllObjects", &o2::UITree::GetAllObjects, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, SelectableDragableObject*>(&type, "Select", &o2::UITree::Select, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<SelectableDragableObject*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, SelectableDragableObject*>(&type, "Deselect", &o2::UITree::Deselect, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<SelectableDragableObject*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, SelectableDragableObject*>(&type, "AddSelectableObject", &o2::UITree::AddSelectableObject, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<SelectableDragableObject*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, SelectableDragableObject*>(&type, "RemoveSelectableObject", &o2::UITree::RemoveSelectableObject, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<SelectableDragableObject*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, SelectableDragableObject*, const Input::Cursor&>(&type, "OnSelectableObjectCursorReleased", &o2::UITree::OnSelectableObjectCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<SelectableDragableObject*>(funcInfo, "object");
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, SelectableDragableObject*>(&type, "OnSelectableObjectBeganDragging", &o2::UITree::OnSelectableObjectBeganDragging, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<SelectableDragableObject*>(funcInfo, "object");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, bool, const Vec2F&>(&type, "CheckMultipleSelection", &o2::UITree::CheckMultipleSelection, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "OnSelectionChanged", &o2::UITree::OnSelectionChanged, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, float>(&type, "UpdatePressedNodeExpand", &o2::UITree::UpdatePressedNodeExpand, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "UpdateNodesStructure", &o2::UITree::UpdateNodesStructure, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, int, Node*, int, NodesVec*>(&type, "InsertNodes", &o2::UITree::InsertNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Node*>(funcInfo, "parentNode");
	TypeInitializer::RegFuncParam<int>(funcInfo, "position");
	TypeInitializer::RegFuncParam<NodesVec*>(funcInfo, "newNodes");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, Node*>(&type, "RemoveNodes", &o2::UITree::RemoveNodes, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Node*>(funcInfo, "parentNode");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, Node*, UnknownPtr, Node*>(&type, "CreateNode", &o2::UITree::CreateNode, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UnknownPtr>(funcInfo, "object");
	TypeInitializer::RegFuncParam<Node*>(funcInfo, "parent");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "OnFocused", &o2::UITree::OnFocused, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "OnUnfocused", &o2::UITree::OnUnfocused, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, bool, bool>(&type, "UpdateLayout", &o2::UITree::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "UpdateVisibleNodes", &o2::UITree::UpdateVisibleNodes, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, Node*, int>(&type, "CreateVisibleNodeWidget", &o2::UITree::CreateVisibleNodeWidget, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Node*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<int>(funcInfo, "i");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, Node*, UITreeNode*, int>(&type, "UpdateNodeView", &o2::UITree::UpdateNodeView, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Node*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "widget");
	TypeInitializer::RegFuncParam<int>(funcInfo, "idx");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, Node*, int>(&type, "UpdateNodeWidgetLayout", &o2::UITree::UpdateNodeWidgetLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Node*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<int>(funcInfo, "idx");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, int, float>(&type, "GetNodeIndex", &o2::UITree::GetNodeIndex, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "position");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, float, int>(&type, "GetNodePosition", &o2::UITree::GetNodePosition, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<int>(funcInfo, "idx");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, Node*>(&type, "ExpandNode", &o2::UITree::ExpandNode, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Node*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, Node*>(&type, "CollapseNode", &o2::UITree::CollapseNode, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<Node*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, ExpandState, Node*, int>(&type, "StartExpandingAnimation", &o2::UITree::StartExpandingAnimation, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<ExpandState>(funcInfo, "direction");
	TypeInitializer::RegFuncParam<Node*>(funcInfo, "node");
	TypeInitializer::RegFuncParam<int>(funcInfo, "childrenCount");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, float>(&type, "UpdateNodeExpanding", &o2::UITree::UpdateNodeExpanding, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "CalculateScrollArea", &o2::UITree::CalculateScrollArea, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Vec2F&>(&type, "MoveScrollPosition", &o2::UITree::MoveScrollPosition, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "delta");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorPressed", &o2::UITree::OnCursorPressed, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorMoved", &o2::UITree::OnCursorMoved, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorReleased", &o2::UITree::OnCursorReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorRightMouseReleased", &o2::UITree::OnCursorRightMouseReleased, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorPressBreak", &o2::UITree::OnCursorPressBreak, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UITree::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UITreeNode*>(&type, "UpdateHover", &o2::UITree::UpdateHover, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "itemUnderCursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, UITreeNode*>(&type, "CreateTreeNodeWidget", &o2::UITree::CreateTreeNodeWidget, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, UITreeNode*>(&type, "BeginDragging", &o2::UITree::BeginDragging, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UITreeNode*>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, bool>(&type, "EndDragging", &o2::UITree::EndDragging, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "updateNodes");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "UpdateDraggingGraphics", &o2::UITree::UpdateDraggingGraphics, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void>(&type, "UpdateDraggingInsertion", &o2::UITree::UpdateDraggingInsertion, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, float>(&type, "UpdateDraggingInsertionAnim", &o2::UITree::UpdateDraggingInsertionAnim, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, ISelectableDragableObjectsGroup*>(&type, "OnDragEnter", &o2::UITree::OnDragEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<ISelectableDragableObjectsGroup*>(funcInfo, "group");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, ISelectableDragableObjectsGroup*>(&type, "OnDraggedAbove", &o2::UITree::OnDraggedAbove, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<ISelectableDragableObjectsGroup*>(funcInfo, "group");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, ISelectableDragableObjectsGroup*>(&type, "OnDragExit", &o2::UITree::OnDragExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<ISelectableDragableObjectsGroup*>(funcInfo, "group");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, ISelectableDragableObjectsGroup*>(&type, "OnDropped", &o2::UITree::OnDropped, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<ISelectableDragableObjectsGroup*>(funcInfo, "group");
	funcInfo = TypeInitializer::RegFunction<o2::UITree, void, const DataNode&>(&type, "OnDeserialized", &o2::UITree::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
}

void o2::UITreeNode::InitializeType(o2::UITreeNode* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mNodeDef", (size_t)(char*)(&sample->mNodeDef) - (size_t)(char*)iobject, sample->mNodeDef, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOwnerTree", (size_t)(char*)(&sample->mOwnerTree) - (size_t)(char*)iobject, sample->mOwnerTree, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSelectedState", (size_t)(char*)(&sample->mSelectedState) - (size_t)(char*)iobject, sample->mSelectedState, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mExpandBtn", (size_t)(char*)(&sample->mExpandBtn) - (size_t)(char*)iobject, sample->mExpandBtn, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool, bool>(&type, "SetExpanded", &o2::UITreeNode::SetExpanded, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "expanded");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, bool>(&type, "IsExpanded", &o2::UITreeNode::IsExpanded, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool>(&type, "Expand", &o2::UITreeNode::Expand, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, bool>(&type, "Collapse", &o2::UITreeNode::Collapse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, UnknownPtr>(&type, "GetObject", &o2::UITreeNode::GetObject, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UITreeNode::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, float>(&type, "UpdateTreeLayout", &o2::UITreeNode::UpdateTreeLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<float>(funcInfo, "dt");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, const Input::Cursor&>(&type, "OnCursorDblClicked", &o2::UITreeNode::OnCursorDblClicked, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, const Input::Cursor&>(&type, "OnCursorEnter", &o2::UITreeNode::OnCursorEnter, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, const Input::Cursor&>(&type, "OnCursorExit", &o2::UITreeNode::OnCursorExit, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, const Input::Cursor&>(&type, "OnDragStart", &o2::UITreeNode::OnDragStart, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, const Input::Cursor&, DragDropArea*>(&type, "OnDragged", &o2::UITreeNode::OnDragged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	TypeInitializer::RegFuncParam<DragDropArea*>(funcInfo, "area");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void, const Input::Cursor&>(&type, "OnDragEnd", &o2::UITreeNode::OnDragEnd, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const Input::Cursor&>(funcInfo, "cursor");
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void>(&type, "OnSelected", &o2::UITreeNode::OnSelected, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UITreeNode, void>(&type, "OnDeselected", &o2::UITreeNode::OnDeselected, o2::ProtectSection::Protected);
}

void o2::UIVerticalLayout::InitializeType(o2::UIVerticalLayout* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "baseCorner", (size_t)(char*)(&sample->baseCorner) - (size_t)(char*)iobject, sample->baseCorner, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "spacing", (size_t)(char*)(&sample->spacing) - (size_t)(char*)iobject, sample->spacing, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "border", (size_t)(char*)(&sample->border) - (size_t)(char*)iobject, sample->border, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderLeft", (size_t)(char*)(&sample->borderLeft) - (size_t)(char*)iobject, sample->borderLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderRight", (size_t)(char*)(&sample->borderRight) - (size_t)(char*)iobject, sample->borderRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderTop", (size_t)(char*)(&sample->borderTop) - (size_t)(char*)iobject, sample->borderTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "borderBottom", (size_t)(char*)(&sample->borderBottom) - (size_t)(char*)iobject, sample->borderBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "expandWidth", (size_t)(char*)(&sample->expandWidth) - (size_t)(char*)iobject, sample->expandWidth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "expandHeight", (size_t)(char*)(&sample->expandHeight) - (size_t)(char*)iobject, sample->expandHeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "fitByChildren", (size_t)(char*)(&sample->fitByChildren) - (size_t)(char*)iobject, sample->fitByChildren, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mBaseCorner", (size_t)(char*)(&sample->mBaseCorner) - (size_t)(char*)iobject, sample->mBaseCorner, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSpacing", (size_t)(char*)(&sample->mSpacing) - (size_t)(char*)iobject, sample->mSpacing, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)iobject, sample->mBorder, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mExpandWidth", (size_t)(char*)(&sample->mExpandWidth) - (size_t)(char*)iobject, sample->mExpandWidth, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mExpandHeight", (size_t)(char*)(&sample->mExpandHeight) - (size_t)(char*)iobject, sample->mExpandHeight, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFitByChildren", (size_t)(char*)(&sample->mFitByChildren) - (size_t)(char*)iobject, sample->mFitByChildren, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
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
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalLayout, void, bool, bool>(&type, "UpdateLayout", &o2::UIVerticalLayout::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)iobject, sample->minValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)iobject, sample->maxValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)iobject, sample->scrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)iobject, sample->onChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)iobject, sample->mValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)iobject, sample->mSmoothValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)iobject, sample->mMinValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)iobject, sample->mMaxValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)iobject, sample->mScrollSense, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOrientation", (size_t)(char*)(&sample->mOrientation) - (size_t)(char*)iobject, sample->mOrientation, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBarLayer", (size_t)(char*)(&sample->mBarLayer) - (size_t)(char*)iobject, sample->mBarLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)iobject, sample->mBackLayer, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalProgress, void, bool, bool>(&type, "UpdateLayout", &o2::UIVerticalProgress::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minValue", (size_t)(char*)(&sample->minValue) - (size_t)(char*)iobject, sample->minValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxValue", (size_t)(char*)(&sample->maxValue) - (size_t)(char*)iobject, sample->maxValue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSense", (size_t)(char*)(&sample->scrollSense) - (size_t)(char*)iobject, sample->scrollSense, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scrollSize", (size_t)(char*)(&sample->scrollSize) - (size_t)(char*)iobject, sample->scrollSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onChange", (size_t)(char*)(&sample->onChange) - (size_t)(char*)iobject, sample->onChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onSmoothChange", (size_t)(char*)(&sample->onSmoothChange) - (size_t)(char*)iobject, sample->onSmoothChange, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)iobject, sample->mValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSmoothValue", (size_t)(char*)(&sample->mSmoothValue) - (size_t)(char*)iobject, sample->mSmoothValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mMinValue", (size_t)(char*)(&sample->mMinValue) - (size_t)(char*)iobject, sample->mMinValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxValue", (size_t)(char*)(&sample->mMaxValue) - (size_t)(char*)iobject, sample->mMaxValue, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollSense", (size_t)(char*)(&sample->mScrollSense) - (size_t)(char*)iobject, sample->mScrollSense, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollHandleSize", (size_t)(char*)(&sample->mScrollHandleSize) - (size_t)(char*)iobject, sample->mScrollHandleSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScrollhandleMinPxSize", (size_t)(char*)(&sample->mScrollhandleMinPxSize) - (size_t)(char*)iobject, sample->mScrollhandleMinPxSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mPressHandleOffset", (size_t)(char*)(&sample->mPressHandleOffset) - (size_t)(char*)iobject, sample->mPressHandleOffset, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandlePressed", (size_t)(char*)(&sample->mHandlePressed) - (size_t)(char*)iobject, sample->mHandlePressed, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHandleLayer", (size_t)(char*)(&sample->mHandleLayer) - (size_t)(char*)iobject, sample->mHandleLayer, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBackLayer", (size_t)(char*)(&sample->mBackLayer) - (size_t)(char*)iobject, sample->mBackLayer, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIVerticalScrollBar, void, bool, bool>(&type, "UpdateLayout", &o2::UIVerticalScrollBar::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)iobject, sample->name, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "parent", (size_t)(char*)(&sample->parent) - (size_t)(char*)iobject, sample->parent, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "childs", (size_t)(char*)(&sample->childs) - (size_t)(char*)iobject, sample->childs, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "layers", (size_t)(char*)(&sample->layers) - (size_t)(char*)iobject, sample->layers, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "states", (size_t)(char*)(&sample->states) - (size_t)(char*)iobject, sample->states, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "transparency", (size_t)(char*)(&sample->transparency) - (size_t)(char*)iobject, sample->transparency, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "resTransparency", (size_t)(char*)(&sample->resTransparency) - (size_t)(char*)iobject, sample->resTransparency, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "visible", (size_t)(char*)(&sample->visible) - (size_t)(char*)iobject, sample->visible, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "child", (size_t)(char*)(&sample->child) - (size_t)(char*)iobject, sample->child, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "layer", (size_t)(char*)(&sample->layer) - (size_t)(char*)iobject, sample->layer, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "state", (size_t)(char*)(&sample->state) - (size_t)(char*)iobject, sample->state, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "layout", (size_t)(char*)(&sample->layout) - (size_t)(char*)iobject, sample->layout, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "onLayoutChanged", (size_t)(char*)(&sample->onLayoutChanged) - (size_t)(char*)iobject, sample->onLayoutChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onFocused", (size_t)(char*)(&sample->onFocused) - (size_t)(char*)iobject, sample->onFocused, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onUnfocused", (size_t)(char*)(&sample->onUnfocused) - (size_t)(char*)iobject, sample->onUnfocused, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)iobject, sample->mName, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLayers", (size_t)(char*)(&sample->mLayers) - (size_t)(char*)iobject, sample->mLayers, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mStates", (size_t)(char*)(&sample->mStates) - (size_t)(char*)iobject, sample->mStates, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mParent", (size_t)(char*)(&sample->mParent) - (size_t)(char*)iobject, sample->mParent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mChilds", (size_t)(char*)(&sample->mChilds) - (size_t)(char*)iobject, sample->mChilds, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mChildsAbsRect", (size_t)(char*)(&sample->mChildsAbsRect) - (size_t)(char*)iobject, sample->mChildsAbsRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTransparency", (size_t)(char*)(&sample->mTransparency) - (size_t)(char*)iobject, sample->mTransparency, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResTransparency", (size_t)(char*)(&sample->mResTransparency) - (size_t)(char*)iobject, sample->mResTransparency, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDrawingLayers", (size_t)(char*)(&sample->mDrawingLayers) - (size_t)(char*)iobject, sample->mDrawingLayers, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFocusedState", (size_t)(char*)(&sample->mFocusedState) - (size_t)(char*)iobject, sample->mFocusedState, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsFocused", (size_t)(char*)(&sample->mIsFocused) - (size_t)(char*)iobject, sample->mIsFocused, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsFocusable", (size_t)(char*)(&sample->mIsFocusable) - (size_t)(char*)iobject, sample->mIsFocusable, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVisibleState", (size_t)(char*)(&sample->mVisibleState) - (size_t)(char*)iobject, sample->mVisibleState, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVisible", (size_t)(char*)(&sample->mVisible) - (size_t)(char*)iobject, sample->mVisible, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResVisible", (size_t)(char*)(&sample->mResVisible) - (size_t)(char*)iobject, sample->mResVisible, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFullyDisabled", (size_t)(char*)(&sample->mFullyDisabled) - (size_t)(char*)iobject, sample->mFullyDisabled, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsClipped", (size_t)(char*)(&sample->mIsClipped) - (size_t)(char*)iobject, sample->mIsClipped, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBounds", (size_t)(char*)(&sample->mBounds) - (size_t)(char*)iobject, sample->mBounds, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBoundsWithChilds", (size_t)(char*)(&sample->mBoundsWithChilds) - (size_t)(char*)iobject, sample->mBoundsWithChilds, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, UIWidget*, UIWidget*, bool>(&type, "AddChild", &o2::UIWidget::AddChild, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<UIWidget*>(funcInfo, "widget");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "updateNow");
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
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool, bool>(&type, "RemoveAllChilds", &o2::UIWidget::RemoveAllChilds, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "release");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "updateLayout");
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
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const String&, bool>(&type, "SetStateForcible", &o2::UIWidget::SetStateForcible, o2::ProtectSection::Public);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "Focus", &o2::UIWidget::Focus, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "Unfocus", &o2::UIWidget::Unfocus, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool>(&type, "IsFocused", &o2::UIWidget::IsFocused, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool>(&type, "IsFocusable", &o2::UIWidget::IsFocusable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "SetFocusable", &o2::UIWidget::SetFocusable, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "focusable");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, const Vec2F&>(&type, "IsUnderPoint", &o2::UIWidget::IsUnderPoint, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const Vec2F&>(funcInfo, "point");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "DrawDebugFrame", &o2::UIWidget::DrawDebugFrame, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "OnFocused", &o2::UIWidget::OnFocused, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "OnUnfocused", &o2::UIWidget::OnUnfocused, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, bool, bool>(&type, "CheckIsLayoutDrivenByParent", &o2::UIWidget::CheckIsLayoutDrivenByParent, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcibleLayout");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool, bool>(&type, "UpdateLayout", &o2::UIWidget::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, bool>(&type, "UpdateChildrenLayouts", &o2::UIWidget::UpdateChildrenLayouts, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateBounds", &o2::UIWidget::UpdateBounds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateBoundsWithChilds", &o2::UIWidget::UpdateBoundsWithChilds, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, const RectF&>(&type, "CheckClipping", &o2::UIWidget::CheckClipping, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "clipArea");
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateTransparency", &o2::UIWidget::UpdateTransparency, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void>(&type, "UpdateVisibility", &o2::UIWidget::UpdateVisibility, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidget, void, UIWidget*>(&type, "OnChildFocused", &o2::UIWidget::OnChildFocused, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "layout", (size_t)(char*)(&sample->layout) - (size_t)(char*)iobject, sample->layout, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "interactableLayout", (size_t)(char*)(&sample->interactableLayout) - (size_t)(char*)iobject, sample->interactableLayout, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)iobject, sample->name, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "depth", (size_t)(char*)(&sample->depth) - (size_t)(char*)iobject, sample->depth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "transparency", (size_t)(char*)(&sample->transparency) - (size_t)(char*)iobject, sample->transparency, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "drawable", (size_t)(char*)(&sample->drawable) - (size_t)(char*)iobject, sample->drawable, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "child", (size_t)(char*)(&sample->child) - (size_t)(char*)iobject, sample->child, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTransparency", (size_t)(char*)(&sample->mTransparency) - (size_t)(char*)iobject, sample->mTransparency, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mResTransparency", (size_t)(char*)(&sample->mResTransparency) - (size_t)(char*)iobject, sample->mResTransparency, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDepth", (size_t)(char*)(&sample->mDepth) - (size_t)(char*)iobject, sample->mDepth, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAbsolutePosition", (size_t)(char*)(&sample->mAbsolutePosition) - (size_t)(char*)iobject, sample->mAbsolutePosition, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mInteractableArea", (size_t)(char*)(&sample->mInteractableArea) - (size_t)(char*)iobject, sample->mInteractableArea, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mOwnerWidget", (size_t)(char*)(&sample->mOwnerWidget) - (size_t)(char*)iobject, sample->mOwnerWidget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mParent", (size_t)(char*)(&sample->mParent) - (size_t)(char*)iobject, sample->mParent, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mChilds", (size_t)(char*)(&sample->mChilds) - (size_t)(char*)iobject, sample->mChilds, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
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
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayer, const RectF&>(&type, "GetRect", &o2::UIWidgetLayer::GetRect, o2::ProtectSection::Public);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "pivot", (size_t)(char*)(&sample->pivot) - (size_t)(char*)iobject, sample->pivot, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)iobject, sample->position, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "size", (size_t)(char*)(&sample->size) - (size_t)(char*)iobject, sample->size, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "width", (size_t)(char*)(&sample->width) - (size_t)(char*)iobject, sample->width, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "height", (size_t)(char*)(&sample->height) - (size_t)(char*)iobject, sample->height, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absPosition", (size_t)(char*)(&sample->absPosition) - (size_t)(char*)iobject, sample->absPosition, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absLeftTop", (size_t)(char*)(&sample->absLeftTop) - (size_t)(char*)iobject, sample->absLeftTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absLeftBottom", (size_t)(char*)(&sample->absLeftBottom) - (size_t)(char*)iobject, sample->absLeftBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absRightTop", (size_t)(char*)(&sample->absRightTop) - (size_t)(char*)iobject, sample->absRightTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absRightBottom", (size_t)(char*)(&sample->absRightBottom) - (size_t)(char*)iobject, sample->absRightBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absRect", (size_t)(char*)(&sample->absRect) - (size_t)(char*)iobject, sample->absRect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absLeft", (size_t)(char*)(&sample->absLeft) - (size_t)(char*)iobject, sample->absLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absRight", (size_t)(char*)(&sample->absRight) - (size_t)(char*)iobject, sample->absRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absBottom", (size_t)(char*)(&sample->absBottom) - (size_t)(char*)iobject, sample->absBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "absTop", (size_t)(char*)(&sample->absTop) - (size_t)(char*)iobject, sample->absTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "pivotX", (size_t)(char*)(&sample->pivotX) - (size_t)(char*)iobject, sample->pivotX, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "pivotY", (size_t)(char*)(&sample->pivotY) - (size_t)(char*)iobject, sample->pivotY, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorMin", (size_t)(char*)(&sample->anchorMin) - (size_t)(char*)iobject, sample->anchorMin, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorMax", (size_t)(char*)(&sample->anchorMax) - (size_t)(char*)iobject, sample->anchorMax, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetMin", (size_t)(char*)(&sample->offsetMin) - (size_t)(char*)iobject, sample->offsetMin, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetMax", (size_t)(char*)(&sample->offsetMax) - (size_t)(char*)iobject, sample->offsetMax, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorLeft", (size_t)(char*)(&sample->anchorLeft) - (size_t)(char*)iobject, sample->anchorLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorRight", (size_t)(char*)(&sample->anchorRight) - (size_t)(char*)iobject, sample->anchorRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorBottom", (size_t)(char*)(&sample->anchorBottom) - (size_t)(char*)iobject, sample->anchorBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "anchorTop", (size_t)(char*)(&sample->anchorTop) - (size_t)(char*)iobject, sample->anchorTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetLeft", (size_t)(char*)(&sample->offsetLeft) - (size_t)(char*)iobject, sample->offsetLeft, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetRight", (size_t)(char*)(&sample->offsetRight) - (size_t)(char*)iobject, sample->offsetRight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetBottom", (size_t)(char*)(&sample->offsetBottom) - (size_t)(char*)iobject, sample->offsetBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "offsetTop", (size_t)(char*)(&sample->offsetTop) - (size_t)(char*)iobject, sample->offsetTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minSize", (size_t)(char*)(&sample->minSize) - (size_t)(char*)iobject, sample->minSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minWidth", (size_t)(char*)(&sample->minWidth) - (size_t)(char*)iobject, sample->minWidth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "minHeight", (size_t)(char*)(&sample->minHeight) - (size_t)(char*)iobject, sample->minHeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxSize", (size_t)(char*)(&sample->maxSize) - (size_t)(char*)iobject, sample->maxSize, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxWidth", (size_t)(char*)(&sample->maxWidth) - (size_t)(char*)iobject, sample->maxWidth, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "maxHeight", (size_t)(char*)(&sample->maxHeight) - (size_t)(char*)iobject, sample->maxHeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "weight", (size_t)(char*)(&sample->weight) - (size_t)(char*)iobject, sample->weight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "widthWeight", (size_t)(char*)(&sample->widthWeight) - (size_t)(char*)iobject, sample->widthWeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "heigthWeight", (size_t)(char*)(&sample->heigthWeight) - (size_t)(char*)iobject, sample->heigthWeight, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)iobject, sample->mOwner, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mPivot", (size_t)(char*)(&sample->mPivot) - (size_t)(char*)iobject, sample->mPivot, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAnchorMin", (size_t)(char*)(&sample->mAnchorMin) - (size_t)(char*)iobject, sample->mAnchorMin, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAnchorMax", (size_t)(char*)(&sample->mAnchorMax) - (size_t)(char*)iobject, sample->mAnchorMax, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOffsetMin", (size_t)(char*)(&sample->mOffsetMin) - (size_t)(char*)iobject, sample->mOffsetMin, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOffsetMax", (size_t)(char*)(&sample->mOffsetMax) - (size_t)(char*)iobject, sample->mOffsetMax, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMinSize", (size_t)(char*)(&sample->mMinSize) - (size_t)(char*)iobject, sample->mMinSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMaxSize", (size_t)(char*)(&sample->mMaxSize) - (size_t)(char*)iobject, sample->mMaxSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAbsoluteRect", (size_t)(char*)(&sample->mAbsoluteRect) - (size_t)(char*)iobject, sample->mAbsoluteRect, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLocalRect", (size_t)(char*)(&sample->mLocalRect) - (size_t)(char*)iobject, sample->mLocalRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mWeight", (size_t)(char*)(&sample->mWeight) - (size_t)(char*)iobject, sample->mWeight, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDrivenByParent", (size_t)(char*)(&sample->mDrivenByParent) - (size_t)(char*)iobject, sample->mDrivenByParent, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCheckMinMaxFunc", (size_t)(char*)(&sample->mCheckMinMaxFunc) - (size_t)(char*)iobject, sample->mCheckMinMaxFunc, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void>(&type, "DisableMinMaxSizes", &o2::UIWidgetLayout::DisableMinMaxSizes, o2::ProtectSection::Public);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void>(&type, "CheckMinMax", &o2::UIWidgetLayout::CheckMinMax, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void>(&type, "DontCheckMinMax", &o2::UIWidgetLayout::DontCheckMinMax, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWidgetLayout, void>(&type, "InitializeProperties", &o2::UIWidgetLayout::InitializeProperties, o2::ProtectSection::Protected);
}

void o2::UIWidgetState::InitializeType(o2::UIWidgetState* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)iobject, sample->name, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "animation", (size_t)(char*)(&sample->animation) - (size_t)(char*)iobject, sample->animation, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "offStateAnimationSpeed", (size_t)(char*)(&sample->offStateAnimationSpeed) - (size_t)(char*)iobject, sample->offStateAnimationSpeed, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "onStateFullyTrue", (size_t)(char*)(&sample->onStateFullyTrue) - (size_t)(char*)iobject, sample->onStateFullyTrue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onStateFullyFalse", (size_t)(char*)(&sample->onStateFullyFalse) - (size_t)(char*)iobject, sample->onStateFullyFalse, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onStateBecomesTrue", (size_t)(char*)(&sample->onStateBecomesTrue) - (size_t)(char*)iobject, sample->onStateBecomesTrue, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onStateBecomesFalse", (size_t)(char*)(&sample->onStateBecomesFalse) - (size_t)(char*)iobject, sample->onStateBecomesFalse, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mState", (size_t)(char*)(&sample->mState) - (size_t)(char*)iobject, sample->mState, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)iobject, sample->mOwner, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "caption", (size_t)(char*)(&sample->caption) - (size_t)(char*)iobject, sample->caption, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "icon", (size_t)(char*)(&sample->icon) - (size_t)(char*)iobject, sample->icon, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mIconDrawable", (size_t)(char*)(&sample->mIconDrawable) - (size_t)(char*)iobject, sample->mIconDrawable, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mCaptionDrawable", (size_t)(char*)(&sample->mCaptionDrawable) - (size_t)(char*)iobject, sample->mCaptionDrawable, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mWindowElements", (size_t)(char*)(&sample->mWindowElements) - (size_t)(char*)iobject, sample->mWindowElements, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHeadDragHandle", (size_t)(char*)(&sample->mHeadDragHandle) - (size_t)(char*)iobject, sample->mHeadDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mHeadDragAreaLayout", (size_t)(char*)(&sample->mHeadDragAreaLayout) - (size_t)(char*)iobject, sample->mHeadDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHeadDragAreaRect", (size_t)(char*)(&sample->mHeadDragAreaRect) - (size_t)(char*)iobject, sample->mHeadDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTopDragHandle", (size_t)(char*)(&sample->mTopDragHandle) - (size_t)(char*)iobject, sample->mTopDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTopDragAreaLayout", (size_t)(char*)(&sample->mTopDragAreaLayout) - (size_t)(char*)iobject, sample->mTopDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTopDragAreaRect", (size_t)(char*)(&sample->mTopDragAreaRect) - (size_t)(char*)iobject, sample->mTopDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBottomDragHandle", (size_t)(char*)(&sample->mBottomDragHandle) - (size_t)(char*)iobject, sample->mBottomDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mBottomDragAreaLayout", (size_t)(char*)(&sample->mBottomDragAreaLayout) - (size_t)(char*)iobject, sample->mBottomDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBottomDragAreaRect", (size_t)(char*)(&sample->mBottomDragAreaRect) - (size_t)(char*)iobject, sample->mBottomDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftDragHandle", (size_t)(char*)(&sample->mLeftDragHandle) - (size_t)(char*)iobject, sample->mLeftDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftDragAreaLayout", (size_t)(char*)(&sample->mLeftDragAreaLayout) - (size_t)(char*)iobject, sample->mLeftDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLeftDragAreaRect", (size_t)(char*)(&sample->mLeftDragAreaRect) - (size_t)(char*)iobject, sample->mLeftDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightDragHandle", (size_t)(char*)(&sample->mRightDragHandle) - (size_t)(char*)iobject, sample->mRightDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightDragAreaLayout", (size_t)(char*)(&sample->mRightDragAreaLayout) - (size_t)(char*)iobject, sample->mRightDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mRightDragAreaRect", (size_t)(char*)(&sample->mRightDragAreaRect) - (size_t)(char*)iobject, sample->mRightDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftTopDragHandle", (size_t)(char*)(&sample->mLeftTopDragHandle) - (size_t)(char*)iobject, sample->mLeftTopDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftTopDragAreaLayout", (size_t)(char*)(&sample->mLeftTopDragAreaLayout) - (size_t)(char*)iobject, sample->mLeftTopDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLeftTopDragAreaRect", (size_t)(char*)(&sample->mLeftTopDragAreaRect) - (size_t)(char*)iobject, sample->mLeftTopDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightTopDragHandle", (size_t)(char*)(&sample->mRightTopDragHandle) - (size_t)(char*)iobject, sample->mRightTopDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightTopDragAreaLayout", (size_t)(char*)(&sample->mRightTopDragAreaLayout) - (size_t)(char*)iobject, sample->mRightTopDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mRightTopDragAreaRect", (size_t)(char*)(&sample->mRightTopDragAreaRect) - (size_t)(char*)iobject, sample->mRightTopDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftBottomDragHandle", (size_t)(char*)(&sample->mLeftBottomDragHandle) - (size_t)(char*)iobject, sample->mLeftBottomDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLeftBottomDragAreaLayout", (size_t)(char*)(&sample->mLeftBottomDragAreaLayout) - (size_t)(char*)iobject, sample->mLeftBottomDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mLeftBottomDragAreaRect", (size_t)(char*)(&sample->mLeftBottomDragAreaRect) - (size_t)(char*)iobject, sample->mLeftBottomDragAreaRect, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightBottomDragHandle", (size_t)(char*)(&sample->mRightBottomDragHandle) - (size_t)(char*)iobject, sample->mRightBottomDragHandle, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mRightBottomDragAreaLayout", (size_t)(char*)(&sample->mRightBottomDragAreaLayout) - (size_t)(char*)iobject, sample->mRightBottomDragAreaLayout, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mRightBottomDragAreaRect", (size_t)(char*)(&sample->mRightBottomDragAreaRect) - (size_t)(char*)iobject, sample->mRightBottomDragAreaRect, o2::ProtectSection::Protected);
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
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, bool>(&type, "IsFocusable", &o2::UIWindow::IsFocusable, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, bool, bool>(&type, "UpdateLayout", &o2::UIWindow::UpdateLayout, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "forcible");
	TypeInitializer::RegFuncParam<bool>(funcInfo, "withChildren");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "UpdateTransparency", &o2::UIWindow::UpdateTransparency, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, UIWidgetLayer*>(&type, "OnLayerAdded", &o2::UIWindow::OnLayerAdded, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<UIWidgetLayer*>(funcInfo, "layer");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "InitializeHandles", &o2::UIWindow::InitializeHandles, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, bool>(&type, "SetHandlesInteractable", &o2::UIWindow::SetHandlesInteractable, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<bool>(funcInfo, "interactable");
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "BindHandlesInteractableToVisibility", &o2::UIWindow::BindHandlesInteractableToVisibility, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void>(&type, "OnFocused", &o2::UIWindow::OnFocused, o2::ProtectSection::Protected);
	funcInfo = TypeInitializer::RegFunction<o2::UIWindow, void, UIWidget*>(&type, "OnChildFocused", &o2::UIWindow::OnChildFocused, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<o2::ISerializable, DataNode>(&type, "Serialize", &o2::ISerializable::Serialize, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, void, const DataNode&>(&type, "Deserialize", &o2::ISerializable::Deserialize, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, void, DataNode&>(&type, "OnSerialize", &o2::ISerializable::OnSerialize, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, void, const DataNode&>(&type, "OnDeserialized", &o2::ISerializable::OnDeserialized, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, DataNode, const void*>(&type, "SerializeBasic", &o2::ISerializable::SerializeBasic, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const void*>(funcInfo, "thisObject");
	funcInfo = TypeInitializer::RegFunction<o2::ISerializable, void, const DataNode&, const void*>(&type, "DeserializeBasic", &o2::ISerializable::DeserializeBasic, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<const DataNode&>(funcInfo, "node");
	TypeInitializer::RegFuncParam<const void*>(funcInfo, "thisObject");
}

void o2::ShortcutKeys::InitializeType(o2::ShortcutKeys* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "control", (size_t)(char*)(&sample->control) - (size_t)(char*)iobject, sample->control, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "shift", (size_t)(char*)(&sample->shift) - (size_t)(char*)iobject, sample->shift, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "alt", (size_t)(char*)(&sample->alt) - (size_t)(char*)iobject, sample->alt, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)iobject, sample->key, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::ShortcutKeys, bool>(&type, "IsPressed", &o2::ShortcutKeys::IsPressed, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ShortcutKeys, bool>(&type, "IsDown", &o2::ShortcutKeys::IsDown, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ShortcutKeys, String>(&type, "AsString", &o2::ShortcutKeys::AsString, o2::ProtectSection::Public);
}

void o2::TimeStamp::InitializeType(o2::TimeStamp* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mYear", (size_t)(char*)(&sample->mYear) - (size_t)(char*)iobject, sample->mYear, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMonth", (size_t)(char*)(&sample->mMonth) - (size_t)(char*)iobject, sample->mMonth, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDay", (size_t)(char*)(&sample->mDay) - (size_t)(char*)iobject, sample->mDay, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mHour", (size_t)(char*)(&sample->mHour) - (size_t)(char*)iobject, sample->mHour, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMinute", (size_t)(char*)(&sample->mMinute) - (size_t)(char*)iobject, sample->mMinute, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSecond", (size_t)(char*)(&sample->mSecond) - (size_t)(char*)iobject, sample->mSecond, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::Curve::InitializeType(o2::Curve* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)iobject, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)iobject, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "length", (size_t)(char*)(&sample->length) - (size_t)(char*)iobject, sample->length, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "onKeysChanged", (size_t)(char*)(&sample->onKeysChanged) - (size_t)(char*)iobject, sample->onKeysChanged, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)iobject, sample->mKeys, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "anchorMin", (size_t)(char*)(&sample->anchorMin) - (size_t)(char*)iobject, sample->anchorMin, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "anchorMax", (size_t)(char*)(&sample->anchorMax) - (size_t)(char*)iobject, sample->anchorMax, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "offsetMin", (size_t)(char*)(&sample->offsetMin) - (size_t)(char*)iobject, sample->offsetMin, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "offsetMax", (size_t)(char*)(&sample->offsetMax) - (size_t)(char*)iobject, sample->offsetMax, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::Layout, RectF, const RectF&>(&type, "Calculate", &o2::Layout::Calculate, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<const RectF&>(funcInfo, "source");
}

void o2::Transform::InitializeType(o2::Transform* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)iobject, sample->position, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "size", (size_t)(char*)(&sample->size) - (size_t)(char*)iobject, sample->size, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "scale", (size_t)(char*)(&sample->scale) - (size_t)(char*)iobject, sample->scale, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "pivot", (size_t)(char*)(&sample->pivot) - (size_t)(char*)iobject, sample->pivot, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "worldPivot", (size_t)(char*)(&sample->worldPivot) - (size_t)(char*)iobject, sample->worldPivot, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "szPivot", (size_t)(char*)(&sample->szPivot) - (size_t)(char*)iobject, sample->szPivot, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rect", (size_t)(char*)(&sample->rect) - (size_t)(char*)iobject, sample->rect, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "angle", (size_t)(char*)(&sample->angle) - (size_t)(char*)iobject, sample->angle, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "shear", (size_t)(char*)(&sample->shear) - (size_t)(char*)iobject, sample->shear, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "basis", (size_t)(char*)(&sample->basis) - (size_t)(char*)iobject, sample->basis, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "nonSizedBasis", (size_t)(char*)(&sample->nonSizedBasis) - (size_t)(char*)iobject, sample->nonSizedBasis, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "AABB", (size_t)(char*)(&sample->AABB) - (size_t)(char*)iobject, sample->AABB, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftTop", (size_t)(char*)(&sample->leftTop) - (size_t)(char*)iobject, sample->leftTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "leftBottom", (size_t)(char*)(&sample->leftBottom) - (size_t)(char*)iobject, sample->leftBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightTop", (size_t)(char*)(&sample->rightTop) - (size_t)(char*)iobject, sample->rightTop, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "rightBottom", (size_t)(char*)(&sample->rightBottom) - (size_t)(char*)iobject, sample->rightBottom, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "right", (size_t)(char*)(&sample->right) - (size_t)(char*)iobject, sample->right, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "left", (size_t)(char*)(&sample->left) - (size_t)(char*)iobject, sample->left, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "up", (size_t)(char*)(&sample->up) - (size_t)(char*)iobject, sample->up, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "down", (size_t)(char*)(&sample->down) - (size_t)(char*)iobject, sample->down, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "lookAtPoint", (size_t)(char*)(&sample->lookAtPoint) - (size_t)(char*)iobject, sample->lookAtPoint, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mPosition", (size_t)(char*)(&sample->mPosition) - (size_t)(char*)iobject, sample->mPosition, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSize", (size_t)(char*)(&sample->mSize) - (size_t)(char*)iobject, sample->mSize, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScale", (size_t)(char*)(&sample->mScale) - (size_t)(char*)iobject, sample->mScale, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mPivot", (size_t)(char*)(&sample->mPivot) - (size_t)(char*)iobject, sample->mPivot, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAngle", (size_t)(char*)(&sample->mAngle) - (size_t)(char*)iobject, sample->mAngle, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mShear", (size_t)(char*)(&sample->mShear) - (size_t)(char*)iobject, sample->mShear, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTransform", (size_t)(char*)(&sample->mTransform) - (size_t)(char*)iobject, sample->mTransform, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mNonSizedTransform", (size_t)(char*)(&sample->mNonSizedTransform) - (size_t)(char*)iobject, sample->mNonSizedTransform, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)iobject, sample->position, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "prevSupportValue", (size_t)(char*)(&sample->prevSupportValue) - (size_t)(char*)iobject, sample->prevSupportValue, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "nextSupportValue", (size_t)(char*)(&sample->nextSupportValue) - (size_t)(char*)iobject, sample->nextSupportValue, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)iobject, sample->curvePrevCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)iobject, sample->curvePrevCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)iobject, sample->curveNextCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)iobject, sample->curveNextCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mApproxValues", (size_t)(char*)(&sample->mApproxValues) - (size_t)(char*)iobject, sample->mApproxValues, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)iobject, sample->mCurveApproxValues, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mApproxLengths", (size_t)(char*)(&sample->mApproxLengths) - (size_t)(char*)iobject, sample->mApproxLengths, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mApproxTotalLength", (size_t)(char*)(&sample->mApproxTotalLength) - (size_t)(char*)iobject, sample->mApproxTotalLength, o2::ProtectSection::Public);
}

void o2::Animation::AnimatedValueDef::InitializeType(o2::Animation::AnimatedValueDef* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mTargetPath", (size_t)(char*)(&sample->mTargetPath) - (size_t)(char*)iobject, sample->mTargetPath, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTargetPtr", (size_t)(char*)(&sample->mTargetPtr) - (size_t)(char*)iobject, sample->mTargetPtr, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mAnimatedValue", (size_t)(char*)(&sample->mAnimatedValue) - (size_t)(char*)iobject, sample->mAnimatedValue, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::ActorAsset::MetaInfo::InitializeType(o2::ActorAsset::MetaInfo* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<o2::ActorAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::ActorAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::AnimationAsset::MetaInfo::InitializeType(o2::AnimationAsset::MetaInfo* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<o2::AnimationAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::AnimationAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::Asset::IMetaInfo::InitializeType(o2::Asset::IMetaInfo* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)iobject, sample->mId, o2::ProtectSection::Private).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::Asset::IMetaInfo, Type::Id>(&type, "GetAssetType", &o2::Asset::IMetaInfo::GetAssetType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Asset::IMetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::Asset::IMetaInfo::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
	funcInfo = TypeInitializer::RegFunction<o2::Asset::IMetaInfo, AssetId>(&type, "ID", &o2::Asset::IMetaInfo::ID, o2::ProtectSection::Public);
}

void o2::AssetTree::AssetNode::InitializeType(o2::AssetTree::AssetNode* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mMeta", (size_t)(char*)(&sample->mMeta) - (size_t)(char*)iobject, sample->mMeta, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mTime", (size_t)(char*)(&sample->mTime) - (size_t)(char*)iobject, sample->mTime, o2::ProtectSection::Public);
}

void o2::AtlasAsset::PlatformMeta::InitializeType(o2::AtlasAsset::PlatformMeta* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mMaxSize", (size_t)(char*)(&sample->mMaxSize) - (size_t)(char*)iobject, sample->mMaxSize, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFormat", (size_t)(char*)(&sample->mFormat) - (size_t)(char*)iobject, sample->mFormat, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::AtlasAsset::MetaInfo::InitializeType(o2::AtlasAsset::MetaInfo* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mIOS", (size_t)(char*)(&sample->mIOS) - (size_t)(char*)iobject, sample->mIOS, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAndroid", (size_t)(char*)(&sample->mAndroid) - (size_t)(char*)iobject, sample->mAndroid, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMacOS", (size_t)(char*)(&sample->mMacOS) - (size_t)(char*)iobject, sample->mMacOS, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWindows", (size_t)(char*)(&sample->mWindows) - (size_t)(char*)iobject, sample->mWindows, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mBorder", (size_t)(char*)(&sample->mBorder) - (size_t)(char*)iobject, sample->mBorder, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::AtlasAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::MetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::AtlasAsset::MetaInfo::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
}

void o2::AtlasAsset::Page::InitializeType(o2::AtlasAsset::Page* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)iobject, sample->mId, o2::ProtectSection::Private).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSize", (size_t)(char*)(&sample->mSize) - (size_t)(char*)iobject, sample->mSize, o2::ProtectSection::Private).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mImagesRects", (size_t)(char*)(&sample->mImagesRects) - (size_t)(char*)iobject, sample->mImagesRects, o2::ProtectSection::Private).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mOwner", (size_t)(char*)(&sample->mOwner) - (size_t)(char*)iobject, sample->mOwner, o2::ProtectSection::Private);
	auto funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, UInt>(&type, "ID", &o2::AtlasAsset::Page::ID, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, Vec2I>(&type, "Size", &o2::AtlasAsset::Page::Size, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, TextureRef>(&type, "GetTextureRef", &o2::AtlasAsset::Page::GetTextureRef, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, String>(&type, "GetTextureFileName", &o2::AtlasAsset::Page::GetTextureFileName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::AtlasAsset::Page, const Dictionary<AssetId, RectI>&>(&type, "ImagesRects", &o2::AtlasAsset::Page::ImagesRects, o2::ProtectSection::Public);
}

void o2::BinaryAsset::MetaInfo::InitializeType(o2::BinaryAsset::MetaInfo* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<o2::BinaryAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::BinaryAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::BitmapFontAsset::MetaInfo::InitializeType(o2::BitmapFontAsset::MetaInfo* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<o2::BitmapFontAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::BitmapFontAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::DataAsset::MetaInfo::InitializeType(o2::DataAsset::MetaInfo* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<o2::DataAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::DataAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::FolderAsset::MetaInfo::InitializeType(o2::FolderAsset::MetaInfo* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<o2::FolderAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::FolderAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
}

void o2::ImageAsset::PlatformMeta::InitializeType(o2::ImageAsset::PlatformMeta* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mMaxSize", (size_t)(char*)(&sample->mMaxSize) - (size_t)(char*)iobject, sample->mMaxSize, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mScale", (size_t)(char*)(&sample->mScale) - (size_t)(char*)iobject, sample->mScale, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mFormat", (size_t)(char*)(&sample->mFormat) - (size_t)(char*)iobject, sample->mFormat, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::ImageAsset::MetaInfo::InitializeType(o2::ImageAsset::MetaInfo* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mAtlasId", (size_t)(char*)(&sample->mAtlasId) - (size_t)(char*)iobject, sample->mAtlasId, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mIOS", (size_t)(char*)(&sample->mIOS) - (size_t)(char*)iobject, sample->mIOS, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mAndroid", (size_t)(char*)(&sample->mAndroid) - (size_t)(char*)iobject, sample->mAndroid, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mMacOS", (size_t)(char*)(&sample->mMacOS) - (size_t)(char*)iobject, sample->mMacOS, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mWindows", (size_t)(char*)(&sample->mWindows) - (size_t)(char*)iobject, sample->mWindows, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mSliceBorder", (size_t)(char*)(&sample->mSliceBorder) - (size_t)(char*)iobject, sample->mSliceBorder, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mDefaultMode", (size_t)(char*)(&sample->mDefaultMode) - (size_t)(char*)iobject, sample->mDefaultMode, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::ImageAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::ImageAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::ImageAsset::MetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::ImageAsset::MetaInfo::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
}

void o2::VectorFontAsset::MetaInfo::InitializeType(o2::VectorFontAsset::MetaInfo* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mEffects", (size_t)(char*)(&sample->mEffects) - (size_t)(char*)iobject, sample->mEffects, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	auto funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset::MetaInfo, Type::Id>(&type, "GetAssetType", &o2::VectorFontAsset::MetaInfo::GetAssetType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFontAsset::MetaInfo, bool, IMetaInfo*>(&type, "IsEqual", &o2::VectorFontAsset::MetaInfo::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<IMetaInfo*>(funcInfo, "other");
}

void o2::AtlasAssetConverter::Image::InitializeType(o2::AtlasAssetConverter::Image* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mId", (size_t)(char*)(&sample->mId) - (size_t)(char*)iobject, sample->mId, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mTime", (size_t)(char*)(&sample->mTime) - (size_t)(char*)iobject, sample->mTime, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
}

void o2::VectorFont::Effect::InitializeType(o2::VectorFont::Effect* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<o2::VectorFont::Effect, void, Bitmap*>(&type, "Process", &o2::VectorFont::Effect::Process, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Bitmap*>(funcInfo, "bitmap");
	funcInfo = TypeInitializer::RegFunction<o2::VectorFont::Effect, Vec2I>(&type, "GetSizeExtend", &o2::VectorFont::Effect::GetSizeExtend, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::VectorFont::Effect, bool, Effect*>(&type, "IsEqual", &o2::VectorFont::Effect::IsEqual, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<Effect*>(funcInfo, "other");
}

void o2::Scene::Layer::InitializeType(o2::Scene::Layer* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "name", (size_t)(char*)(&sample->name) - (size_t)(char*)iobject, sample->name, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mActors", (size_t)(char*)(&sample->mActors) - (size_t)(char*)iobject, sample->mActors, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEnabledActors", (size_t)(char*)(&sample->mEnabledActors) - (size_t)(char*)iobject, sample->mEnabledActors, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mDrawables", (size_t)(char*)(&sample->mDrawables) - (size_t)(char*)iobject, sample->mDrawables, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEnabledDrawables", (size_t)(char*)(&sample->mEnabledDrawables) - (size_t)(char*)iobject, sample->mEnabledDrawables, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, const ActorsVec&>(&type, "GetActors", &o2::Scene::Layer::GetActors, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, const ActorsVec&>(&type, "GetEnabledActors", &o2::Scene::Layer::GetEnabledActors, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, const DrawCompsVec&>(&type, "GetDrawableComponents", &o2::Scene::Layer::GetDrawableComponents, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, const DrawCompsVec&>(&type, "GetEnabledDrawableComponents", &o2::Scene::Layer::GetEnabledDrawableComponents, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "RegDrawableComponent", &o2::Scene::Layer::RegDrawableComponent, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "UnregDrawableComponent", &o2::Scene::Layer::UnregDrawableComponent, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "ComponentDepthChanged", &o2::Scene::Layer::ComponentDepthChanged, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "ComponentEnabled", &o2::Scene::Layer::ComponentEnabled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
	funcInfo = TypeInitializer::RegFunction<o2::Scene::Layer, void, DrawableComponent*>(&type, "ComponentDisabled", &o2::Scene::Layer::ComponentDisabled, o2::ProtectSection::Protected);
	TypeInitializer::RegFuncParam<DrawableComponent*>(funcInfo, "component");
}

void o2::UIContextMenu::Item::InitializeType(o2::UIContextMenu::Item* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)iobject, sample->text, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "shortcut", (size_t)(char*)(&sample->shortcut) - (size_t)(char*)iobject, sample->shortcut, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "icon", (size_t)(char*)(&sample->icon) - (size_t)(char*)iobject, sample->icon, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "subItems", (size_t)(char*)(&sample->subItems) - (size_t)(char*)iobject, sample->subItems, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)iobject, sample->onClick, o2::ProtectSection::Public);
}

void o2::UIMenuPanel::Item::InitializeType(o2::UIMenuPanel::Item* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "text", (size_t)(char*)(&sample->text) - (size_t)(char*)iobject, sample->text, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "subItems", (size_t)(char*)(&sample->subItems) - (size_t)(char*)iobject, sample->subItems, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "onClick", (size_t)(char*)(&sample->onClick) - (size_t)(char*)iobject, sample->onClick, o2::ProtectSection::Public);
}

void o2::Curve::Key::InitializeType(o2::Curve::Key* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)iobject, sample->position, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "leftCoef", (size_t)(char*)(&sample->leftCoef) - (size_t)(char*)iobject, sample->leftCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "leftCoefPosition", (size_t)(char*)(&sample->leftCoefPosition) - (size_t)(char*)iobject, sample->leftCoefPosition, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "rightCoef", (size_t)(char*)(&sample->rightCoef) - (size_t)(char*)iobject, sample->rightCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "rightCoefPosition", (size_t)(char*)(&sample->rightCoefPosition) - (size_t)(char*)iobject, sample->rightCoefPosition, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mApproxValues", (size_t)(char*)(&sample->mApproxValues) - (size_t)(char*)iobject, sample->mApproxValues, o2::ProtectSection::Public);
}

void CodeTool::Tmp::InitializeType(CodeTool::Tmp* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
}

void CodeTool::ICppSyntaxStatementParser::InitializeType(CodeTool::ICppSyntaxStatementParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::ICppSyntaxStatementParser, const char*>(&type, "GetKeyWord", &CodeTool::ICppSyntaxStatementParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::ICppSyntaxStatementParser, bool>(&type, "IsPossibleInNamespace", &CodeTool::ICppSyntaxStatementParser::IsPossibleInNamespace, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::ICppSyntaxStatementParser, bool>(&type, "IsPossibleInClass", &CodeTool::ICppSyntaxStatementParser::IsPossibleInClass, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::ICppSyntaxStatementParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::ICppSyntaxStatementParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxNamespaceParser::InitializeType(CodeTool::CppSyntaxNamespaceParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxNamespaceParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxNamespaceParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxNamespaceParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxNamespaceParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxMultilineCommentParser::InitializeType(CodeTool::CppSyntaxMultilineCommentParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxMultilineCommentParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxMultilineCommentParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxMultilineCommentParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxMultilineCommentParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxCommentParser::InitializeType(CodeTool::CppSyntaxCommentParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxCommentParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxCommentParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxCommentParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxCommentParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxPragmaParser::InitializeType(CodeTool::CppSyntaxPragmaParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxPragmaParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxPragmaParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxPragmaParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxPragmaParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxIncludeParser::InitializeType(CodeTool::CppSyntaxIncludeParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxIncludeParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxIncludeParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxIncludeParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxIncludeParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxDefineParser::InitializeType(CodeTool::CppSyntaxDefineParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxDefineParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxDefineParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxDefineParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxDefineParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxIfMacroParser::InitializeType(CodeTool::CppSyntaxIfMacroParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxIfMacroParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxIfMacroParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxIfMacroParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxIfMacroParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxClassParser::InitializeType(CodeTool::CppSyntaxClassParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxClassParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxClassParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxClassParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxClassParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxStructParser::InitializeType(CodeTool::CppSyntaxStructParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxStructParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxStructParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxStructParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxStructParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxTemplateParser::InitializeType(CodeTool::CppSyntaxTemplateParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxTemplateParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxTemplateParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxTemplateParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxTemplateParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxTypedefParser::InitializeType(CodeTool::CppSyntaxTypedefParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxTypedefParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxTypedefParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxTypedefParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxTypedefParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxEnumParser::InitializeType(CodeTool::CppSyntaxEnumParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxEnumParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxEnumParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxEnumParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxEnumParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxUsingParser::InitializeType(CodeTool::CppSyntaxUsingParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxUsingParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxUsingParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxUsingParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxUsingParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxPublicSectionParser::InitializeType(CodeTool::CppSyntaxPublicSectionParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxPublicSectionParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxPublicSectionParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxPublicSectionParser, bool>(&type, "IsPossibleInNamespace", &CodeTool::CppSyntaxPublicSectionParser::IsPossibleInNamespace, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxPublicSectionParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxPublicSectionParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxPrivateSectionParser::InitializeType(CodeTool::CppSyntaxPrivateSectionParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxPrivateSectionParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxPrivateSectionParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxPrivateSectionParser, bool>(&type, "IsPossibleInNamespace", &CodeTool::CppSyntaxPrivateSectionParser::IsPossibleInNamespace, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxPrivateSectionParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxPrivateSectionParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxProtectedSectionParser::InitializeType(CodeTool::CppSyntaxProtectedSectionParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxProtectedSectionParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxProtectedSectionParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxProtectedSectionParser, bool>(&type, "IsPossibleInNamespace", &CodeTool::CppSyntaxProtectedSectionParser::IsPossibleInNamespace, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxProtectedSectionParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxProtectedSectionParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::CppSyntaxFriendParser::InitializeType(CodeTool::CppSyntaxFriendParser* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxFriendParser, const char*>(&type, "GetKeyWord", &CodeTool::CppSyntaxFriendParser::GetKeyWord, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxFriendParser, bool>(&type, "IsPossibleInNamespace", &CodeTool::CppSyntaxFriendParser::IsPossibleInNamespace, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::CppSyntaxFriendParser, void, SyntaxSection&, int&, SyntaxProtectionSection&>(&type, "Parse", &CodeTool::CppSyntaxFriendParser::Parse, o2::ProtectSection::Public);
	TypeInitializer::RegFuncParam<SyntaxSection&>(funcInfo, "section");
	TypeInitializer::RegFuncParam<int&>(funcInfo, "caret");
	TypeInitializer::RegFuncParam<SyntaxProtectionSection&>(funcInfo, "protectionSection");
}

void CodeTool::SyntaxTree::InitializeType(CodeTool::SyntaxTree* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mFiles", (size_t)(char*)(&sample->mFiles) - (size_t)(char*)iobject, sample->mFiles, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGlobalNamespace", (size_t)(char*)(&sample->mGlobalNamespace) - (size_t)(char*)iobject, sample->mGlobalNamespace, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxTree, const SyntaxFilesVec&>(&type, "GetFiles", &CodeTool::SyntaxTree::GetFiles, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxTree, SyntaxNamespace*>(&type, "GetGlobalNamespace", &CodeTool::SyntaxTree::GetGlobalNamespace, o2::ProtectSection::Public);
}

void CodeTool::SyntaxFile::InitializeType(CodeTool::SyntaxFile* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mPath", (size_t)(char*)(&sample->mPath) - (size_t)(char*)iobject, sample->mPath, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mData", (size_t)(char*)(&sample->mData) - (size_t)(char*)iobject, sample->mData, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLastEditedDate", (size_t)(char*)(&sample->mLastEditedDate) - (size_t)(char*)iobject, sample->mLastEditedDate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mGlobalNamespace", (size_t)(char*)(&sample->mGlobalNamespace) - (size_t)(char*)iobject, sample->mGlobalNamespace, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxFile, const String&>(&type, "GetPath", &CodeTool::SyntaxFile::GetPath, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxFile, const String&>(&type, "GetData", &CodeTool::SyntaxFile::GetData, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxFile, const TimeStamp&>(&type, "GetLastEditedDate", &CodeTool::SyntaxFile::GetLastEditedDate, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxFile, SyntaxNamespace*>(&type, "GetGlobalNamespace", &CodeTool::SyntaxFile::GetGlobalNamespace, o2::ProtectSection::Public);
}

void CodeTool::SyntaxEntry::InitializeType(CodeTool::SyntaxEntry* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mBegin", (size_t)(char*)(&sample->mBegin) - (size_t)(char*)iobject, sample->mBegin, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mLength", (size_t)(char*)(&sample->mLength) - (size_t)(char*)iobject, sample->mLength, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mData", (size_t)(char*)(&sample->mData) - (size_t)(char*)iobject, sample->mData, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFile", (size_t)(char*)(&sample->mFile) - (size_t)(char*)iobject, sample->mFile, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxEntry, int>(&type, "GetBegin", &CodeTool::SyntaxEntry::GetBegin, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxEntry, int>(&type, "GetLength", &CodeTool::SyntaxEntry::GetLength, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxEntry, int>(&type, "GetEnd", &CodeTool::SyntaxEntry::GetEnd, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxEntry, const String&>(&type, "GetData", &CodeTool::SyntaxEntry::GetData, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxEntry, SyntaxFile*>(&type, "GetOwnerFile", &CodeTool::SyntaxEntry::GetOwnerFile, o2::ProtectSection::Public);
}

void CodeTool::SyntaxSection::InitializeType(CodeTool::SyntaxSection* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)iobject, sample->mName, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFullName", (size_t)(char*)(&sample->mFullName) - (size_t)(char*)iobject, sample->mFullName, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mParentSection", (size_t)(char*)(&sample->mParentSection) - (size_t)(char*)iobject, sample->mParentSection, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mFunctions", (size_t)(char*)(&sample->mFunctions) - (size_t)(char*)iobject, sample->mFunctions, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mVariables", (size_t)(char*)(&sample->mVariables) - (size_t)(char*)iobject, sample->mVariables, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mSections", (size_t)(char*)(&sample->mSections) - (size_t)(char*)iobject, sample->mSections, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEnums", (size_t)(char*)(&sample->mEnums) - (size_t)(char*)iobject, sample->mEnums, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxSection, SyntaxSection*>(&type, "GetParentSection", &CodeTool::SyntaxSection::GetParentSection, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxSection, const String&>(&type, "GetName", &CodeTool::SyntaxSection::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxSection, const String&>(&type, "GetFullName", &CodeTool::SyntaxSection::GetFullName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxSection, const SyntaxFunctionsVec&>(&type, "GetFunctions", &CodeTool::SyntaxSection::GetFunctions, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxSection, const SyntaxVariablesVec&>(&type, "GetVariables", &CodeTool::SyntaxSection::GetVariables, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxSection, const SyntaxSectionsVec&>(&type, "GetSections", &CodeTool::SyntaxSection::GetSections, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxSection, const SyntaxEnumsVec&>(&type, "GetEnums", &CodeTool::SyntaxSection::GetEnums, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxSection, bool>(&type, "IsClass", &CodeTool::SyntaxSection::IsClass, o2::ProtectSection::Public);
}

void CodeTool::SyntaxNamespace::InitializeType(CodeTool::SyntaxNamespace* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxNamespace, bool>(&type, "IsClass", &CodeTool::SyntaxNamespace::IsClass, o2::ProtectSection::Public);
}

void CodeTool::SyntaxClass::InitializeType(CodeTool::SyntaxClass* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mBaseClasses", (size_t)(char*)(&sample->mBaseClasses) - (size_t)(char*)iobject, sample->mBaseClasses, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsTemplate", (size_t)(char*)(&sample->mIsTemplate) - (size_t)(char*)iobject, sample->mIsTemplate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTemplateParameters", (size_t)(char*)(&sample->mTemplateParameters) - (size_t)(char*)iobject, sample->mTemplateParameters, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mClassSection", (size_t)(char*)(&sample->mClassSection) - (size_t)(char*)iobject, sample->mClassSection, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxClass, const StringsVec&>(&type, "GetBaseClassesNames", &CodeTool::SyntaxClass::GetBaseClassesNames, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxClass, SyntaxClassesVec>(&type, "GetBaseClasses", &CodeTool::SyntaxClass::GetBaseClasses, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxClass, bool>(&type, "IsTemplate", &CodeTool::SyntaxClass::IsTemplate, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxClass, const String&>(&type, "GetTemplateParameters", &CodeTool::SyntaxClass::GetTemplateParameters, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxClass, SyntaxProtectionSection>(&type, "GetClassSection", &CodeTool::SyntaxClass::GetClassSection, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxClass, bool>(&type, "IsClass", &CodeTool::SyntaxClass::IsClass, o2::ProtectSection::Public);
}

void CodeTool::SyntaxType::InitializeType(CodeTool::SyntaxType* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)iobject, sample->mName, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsContant", (size_t)(char*)(&sample->mIsContant) - (size_t)(char*)iobject, sample->mIsContant, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsReference", (size_t)(char*)(&sample->mIsReference) - (size_t)(char*)iobject, sample->mIsReference, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mIsPointer", (size_t)(char*)(&sample->mIsPointer) - (size_t)(char*)iobject, sample->mIsPointer, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxType, const String&>(&type, "GetName", &CodeTool::SyntaxType::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxType, bool>(&type, "IsContant", &CodeTool::SyntaxType::IsContant, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxType, bool>(&type, "IsReference", &CodeTool::SyntaxType::IsReference, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxType, bool>(&type, "IsPointer", &CodeTool::SyntaxType::IsPointer, o2::ProtectSection::Public);
}

void CodeTool::SyntaxVariable::InitializeType(CodeTool::SyntaxVariable* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mType", (size_t)(char*)(&sample->mType) - (size_t)(char*)iobject, sample->mType, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)iobject, sample->mName, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mAttributes", (size_t)(char*)(&sample->mAttributes) - (size_t)(char*)iobject, sample->mAttributes, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mClassSection", (size_t)(char*)(&sample->mClassSection) - (size_t)(char*)iobject, sample->mClassSection, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxVariable, const SyntaxType&>(&type, "GetSynType", &CodeTool::SyntaxVariable::GetSynType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxVariable, const String&>(&type, "GetName", &CodeTool::SyntaxVariable::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxVariable, const StringsVec&>(&type, "GetAttributes", &CodeTool::SyntaxVariable::GetAttributes, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxVariable, SyntaxProtectionSection>(&type, "GetClassSection", &CodeTool::SyntaxVariable::GetClassSection, o2::ProtectSection::Public);
}

void CodeTool::SyntaxFunction::InitializeType(CodeTool::SyntaxFunction* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mReturnType", (size_t)(char*)(&sample->mReturnType) - (size_t)(char*)iobject, sample->mReturnType, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)iobject, sample->mName, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mParameters", (size_t)(char*)(&sample->mParameters) - (size_t)(char*)iobject, sample->mParameters, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mClassSection", (size_t)(char*)(&sample->mClassSection) - (size_t)(char*)iobject, sample->mClassSection, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxFunction, const SyntaxType&>(&type, "GetReturnType", &CodeTool::SyntaxFunction::GetReturnType, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxFunction, const String&>(&type, "GetName", &CodeTool::SyntaxFunction::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxFunction, const SyntaxVariablesVec&>(&type, "GetParameters", &CodeTool::SyntaxFunction::GetParameters, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxFunction, SyntaxProtectionSection>(&type, "GetClassSection", &CodeTool::SyntaxFunction::GetClassSection, o2::ProtectSection::Public);
}

void CodeTool::SyntaxEnum::InitializeType(CodeTool::SyntaxEnum* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "mName", (size_t)(char*)(&sample->mName) - (size_t)(char*)iobject, sample->mName, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mEntries", (size_t)(char*)(&sample->mEntries) - (size_t)(char*)iobject, sample->mEntries, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mClassSection", (size_t)(char*)(&sample->mClassSection) - (size_t)(char*)iobject, sample->mClassSection, o2::ProtectSection::Protected);
	auto funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxEnum, const String&>(&type, "GetName", &CodeTool::SyntaxEnum::GetName, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxEnum, const StringsVec&>(&type, "GetEntries", &CodeTool::SyntaxEnum::GetEntries, o2::ProtectSection::Public);
	funcInfo = TypeInitializer::RegFunction<CodeTool::SyntaxEnum, SyntaxProtectionSection>(&type, "GetClassSection", &CodeTool::SyntaxEnum::GetClassSection, o2::ProtectSection::Public);
}

void o2::AnimatedValue<Color4>::InitializeType(o2::AnimatedValue<Color4>* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)iobject, sample->target, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)iobject, sample->targetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)iobject, sample->targetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)iobject, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)iobject, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)iobject, sample->mKeys, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)iobject, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)iobject, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)iobject, sample->mTargetDelegate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)iobject, sample->mTargetProperty, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)iobject, sample->position, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)iobject, sample->curvePrevCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)iobject, sample->curvePrevCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)iobject, sample->curveNextCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)iobject, sample->curveNextCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)iobject, sample->mCurveApproxValues, o2::ProtectSection::Public);
}

void o2::AnimatedValue<RectF>::InitializeType(o2::AnimatedValue<RectF>* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)iobject, sample->target, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)iobject, sample->targetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)iobject, sample->targetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)iobject, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)iobject, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)iobject, sample->mKeys, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)iobject, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)iobject, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)iobject, sample->mTargetDelegate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)iobject, sample->mTargetProperty, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)iobject, sample->position, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)iobject, sample->curvePrevCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)iobject, sample->curvePrevCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)iobject, sample->curveNextCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)iobject, sample->curveNextCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)iobject, sample->mCurveApproxValues, o2::ProtectSection::Public);
}

void o2::AnimatedValue<bool>::InitializeType(o2::AnimatedValue<bool>* sample)
{
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "target", (size_t)(char*)(&sample->target) - (size_t)(char*)iobject, sample->target, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetDelegate", (size_t)(char*)(&sample->targetDelegate) - (size_t)(char*)iobject, sample->targetDelegate, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "targetProperty", (size_t)(char*)(&sample->targetProperty) - (size_t)(char*)iobject, sample->targetProperty, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "key", (size_t)(char*)(&sample->key) - (size_t)(char*)iobject, sample->key, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "keys", (size_t)(char*)(&sample->keys) - (size_t)(char*)iobject, sample->keys, o2::ProtectSection::Public);
	TypeInitializer::RegField(&type, "mKeys", (size_t)(char*)(&sample->mKeys) - (size_t)(char*)iobject, sample->mKeys, o2::ProtectSection::Protected).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mValue", (size_t)(char*)(&sample->mValue) - (size_t)(char*)iobject, sample->mValue, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTarget", (size_t)(char*)(&sample->mTarget) - (size_t)(char*)iobject, sample->mTarget, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetDelegate", (size_t)(char*)(&sample->mTargetDelegate) - (size_t)(char*)iobject, sample->mTargetDelegate, o2::ProtectSection::Protected);
	TypeInitializer::RegField(&type, "mTargetProperty", (size_t)(char*)(&sample->mTargetProperty) - (size_t)(char*)iobject, sample->mTargetProperty, o2::ProtectSection::Protected);
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
	auto iobject = dynamic_cast<o2::IObject*>(sample);
	TypeInitializer::RegField(&type, "position", (size_t)(char*)(&sample->position) - (size_t)(char*)iobject, sample->position, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "value", (size_t)(char*)(&sample->value) - (size_t)(char*)iobject, sample->value, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoef", (size_t)(char*)(&sample->curvePrevCoef) - (size_t)(char*)iobject, sample->curvePrevCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curvePrevCoefPos", (size_t)(char*)(&sample->curvePrevCoefPos) - (size_t)(char*)iobject, sample->curvePrevCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoef", (size_t)(char*)(&sample->curveNextCoef) - (size_t)(char*)iobject, sample->curveNextCoef, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "curveNextCoefPos", (size_t)(char*)(&sample->curveNextCoefPos) - (size_t)(char*)iobject, sample->curveNextCoefPos, o2::ProtectSection::Public).AddAttribute<SerializableAttribute>();
	TypeInitializer::RegField(&type, "mCurveApproxValues", (size_t)(char*)(&sample->mCurveApproxValues) - (size_t)(char*)iobject, sample->mCurveApproxValues, o2::ProtectSection::Public);
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
	o2::Reflection::InitializeEnum<o2::Actor::CreateMode>( { { (int)o2::Actor::CreateMode::InScene, "InScene" }, { (int)o2::Actor::CreateMode::NotInScene, "NotInScene" } });
	o2::Reflection::InitializeEnum<o2::UIHorizontalProgress::Orientation>( { { (int)o2::UIHorizontalProgress::Orientation::Right, "Right" }, { (int)o2::UIHorizontalProgress::Orientation::Left, "Left" } });
	o2::Reflection::InitializeEnum<o2::UILabel::HorOverflow>( { { (int)o2::UILabel::HorOverflow::Cut, "Cut" }, { (int)o2::UILabel::HorOverflow::Dots, "Dots" }, { (int)o2::UILabel::HorOverflow::Expand, "Expand" }, { (int)o2::UILabel::HorOverflow::Wrap, "Wrap" }, { (int)o2::UILabel::HorOverflow::None, "None" } });
	o2::Reflection::InitializeEnum<o2::UILabel::VerOverflow>( { { (int)o2::UILabel::VerOverflow::Cut, "Cut" }, { (int)o2::UILabel::VerOverflow::None, "None" }, { (int)o2::UILabel::VerOverflow::Expand, "Expand" } });
	o2::Reflection::InitializeEnum<o2::UIToggleGroup::Type>( { { (int)o2::UIToggleGroup::Type::OnlySingleTrue, "OnlySingleTrue" }, { (int)o2::UIToggleGroup::Type::VerOneClick, "VerOneClick" }, { (int)o2::UIToggleGroup::Type::HorOneClick, "HorOneClick" } });
	o2::Reflection::InitializeEnum<o2::UITree::RearrangeType>( { { (int)o2::UITree::RearrangeType::Disabled, "Disabled" }, { (int)o2::UITree::RearrangeType::Enabled, "Enabled" }, { (int)o2::UITree::RearrangeType::OnlyReparent, "OnlyReparent" } });
	o2::Reflection::InitializeEnum<o2::UITree::ExpandState>( { { (int)o2::UITree::ExpandState::None, "None" }, { (int)o2::UITree::ExpandState::Expanding, "Expanding" }, { (int)o2::UITree::ExpandState::Collaping, "Collaping" } });
	o2::Reflection::InitializeEnum<o2::UIVerticalProgress::Orientation>( { { (int)o2::UIVerticalProgress::Orientation::Down, "Down" }, { (int)o2::UIVerticalProgress::Orientation::Up, "Up" } });
	o2::Reflection::InitializeEnum<o2::Bitmap::Format>( { { (int)o2::Bitmap::Format::Default, "Default" }, { (int)o2::Bitmap::Format::R8G8B8A8, "R8G8B8A8" } });
	o2::Reflection::InitializeEnum<o2::Bitmap::ImageType>( { { (int)o2::Bitmap::ImageType::Auto, "Auto" }, { (int)o2::Bitmap::ImageType::Png, "Png" } });
	o2::Reflection::InitializeEnum<o2::DataNode::Format>( { { (int)o2::DataNode::Format::Xml, "Xml" }, { (int)o2::DataNode::Format::JSON, "JSON" }, { (int)o2::DataNode::Format::Binary, "Binary" } });
	o2::Reflection::InitializeEnum<CodeTool::SyntaxProtectionSection>( { { (int)CodeTool::SyntaxProtectionSection::Public, "Public" }, { (int)CodeTool::SyntaxProtectionSection::Private, "Private" }, { (int)CodeTool::SyntaxProtectionSection::Protected, "Protected" } });

	// Initialize types
	o2::Reflection::InitializeType<o2::Animatable>();
	o2::Reflection::InitializeType<o2::AnimatedValue<float>>();
	o2::Reflection::InitializeType<o2::IAnimatedValue>();
	o2::Reflection::InitializeType<o2::AnimatedValue<Vec2F>>();
	o2::Reflection::InitializeType<o2::Animation>();
	o2::Reflection::InitializeType<o2::AnimationMask>();
	o2::Reflection::InitializeType<o2::AnimationState>();
	o2::Reflection::InitializeType<o2::IAnimation>();
	o2::Reflection::InitializeType<o2::ActorAsset>();
	o2::Reflection::InitializeType<o2::AnimationAsset>();
	o2::Reflection::InitializeType<o2::Asset>();
	o2::Reflection::InitializeType<o2::AssetInfo>();
	o2::Reflection::InitializeType<o2::AtlasAsset>();
	o2::Reflection::InitializeType<o2::BinaryAsset>();
	o2::Reflection::InitializeType<o2::BitmapFontAsset>();
	o2::Reflection::InitializeType<o2::DataAsset>();
	o2::Reflection::InitializeType<o2::FolderAsset>();
	o2::Reflection::InitializeType<o2::ImageAsset>();
	o2::Reflection::InitializeType<o2::VectorFontAsset>();
	o2::Reflection::InitializeType<o2::AtlasAssetConverter>();
	o2::Reflection::InitializeType<o2::FolderAssetConverter>();
	o2::Reflection::InitializeType<o2::IAssetConverter>();
	o2::Reflection::InitializeType<o2::ImageAssetConverter>();
	o2::Reflection::InitializeType<o2::StdAssetConverter>();
	o2::Reflection::InitializeType<o2::ProjectConfig>();
	o2::Reflection::InitializeType<o2::Camera>();
	o2::Reflection::InitializeType<o2::IRectDrawable>();
	o2::Reflection::InitializeType<o2::Sprite>();
	o2::Reflection::InitializeType<o2::Text>();
	o2::Reflection::InitializeType<o2::FontStrokeEffect>();
	o2::Reflection::InitializeType<o2::FontGradientEffect>();
	o2::Reflection::InitializeType<o2::FontColorEffect>();
	o2::Reflection::InitializeType<o2::FontShadowEffect>();
	o2::Reflection::InitializeType<o2::Actor>();
	o2::Reflection::InitializeType<o2::ActorTransform>();
	o2::Reflection::InitializeType<o2::Component>();
	o2::Reflection::InitializeType<o2::DrawableComponent>();
	o2::Reflection::InitializeType<o2::Tag>();
	o2::Reflection::InitializeType<o2::TagGroup>();
	o2::Reflection::InitializeType<o2::EditorTestComponent>();
	o2::Reflection::InitializeType<o2::ImageComponent>();
	o2::Reflection::InitializeType<o2::UIButton>();
	o2::Reflection::InitializeType<o2::UIContextMenuItem>();
	o2::Reflection::InitializeType<o2::UIContextMenu>();
	o2::Reflection::InitializeType<o2::UICustomDropDown>();
	o2::Reflection::InitializeType<o2::UICustomList>();
	o2::Reflection::InitializeType<o2::UIDropDown>();
	o2::Reflection::InitializeType<o2::UIEditBox>();
	o2::Reflection::InitializeType<o2::UIGridLayout>();
	o2::Reflection::InitializeType<o2::UIHorizontalLayout>();
	o2::Reflection::InitializeType<o2::UIHorizontalProgress>();
	o2::Reflection::InitializeType<o2::UIHorizontalScrollBar>();
	o2::Reflection::InitializeType<o2::UILabel>();
	o2::Reflection::InitializeType<o2::UIList>();
	o2::Reflection::InitializeType<o2::UILongList>();
	o2::Reflection::InitializeType<o2::UIMenuPanel>();
	o2::Reflection::InitializeType<o2::UIScrollArea>();
	o2::Reflection::InitializeType<o2::UIToggle>();
	o2::Reflection::InitializeType<o2::UITree>();
	o2::Reflection::InitializeType<o2::UITreeNode>();
	o2::Reflection::InitializeType<o2::UIVerticalLayout>();
	o2::Reflection::InitializeType<o2::UIVerticalProgress>();
	o2::Reflection::InitializeType<o2::UIVerticalScrollBar>();
	o2::Reflection::InitializeType<o2::UIWidget>();
	o2::Reflection::InitializeType<o2::UIWidgetLayer>();
	o2::Reflection::InitializeType<o2::UIWidgetLayout>();
	o2::Reflection::InitializeType<o2::UIWidgetState>();
	o2::Reflection::InitializeType<o2::UIWindow>();
	o2::Reflection::InitializeType<o2::ISerializable>();
	o2::Reflection::InitializeType<o2::ShortcutKeys>();
	o2::Reflection::InitializeType<o2::TimeStamp>();
	o2::Reflection::InitializeType<o2::Curve>();
	o2::Reflection::InitializeType<o2::Layout>();
	o2::Reflection::InitializeType<o2::Transform>();
	o2::Reflection::InitializeType<o2::AnimatedValue<Vec2F>::Key>();
	o2::Reflection::InitializeType<o2::Animation::AnimatedValueDef>();
	o2::Reflection::InitializeType<o2::ActorAsset::MetaInfo>();
	o2::Reflection::InitializeType<o2::AnimationAsset::MetaInfo>();
	o2::Reflection::InitializeType<o2::Asset::IMetaInfo>();
	o2::Reflection::InitializeType<o2::AssetTree::AssetNode>();
	o2::Reflection::InitializeType<o2::AtlasAsset::PlatformMeta>();
	o2::Reflection::InitializeType<o2::AtlasAsset::MetaInfo>();
	o2::Reflection::InitializeType<o2::AtlasAsset::Page>();
	o2::Reflection::InitializeType<o2::BinaryAsset::MetaInfo>();
	o2::Reflection::InitializeType<o2::BitmapFontAsset::MetaInfo>();
	o2::Reflection::InitializeType<o2::DataAsset::MetaInfo>();
	o2::Reflection::InitializeType<o2::FolderAsset::MetaInfo>();
	o2::Reflection::InitializeType<o2::ImageAsset::PlatformMeta>();
	o2::Reflection::InitializeType<o2::ImageAsset::MetaInfo>();
	o2::Reflection::InitializeType<o2::VectorFontAsset::MetaInfo>();
	o2::Reflection::InitializeType<o2::AtlasAssetConverter::Image>();
	o2::Reflection::InitializeType<o2::VectorFont::Effect>();
	o2::Reflection::InitializeType<o2::Scene::Layer>();
	o2::Reflection::InitializeType<o2::UIContextMenu::Item>();
	o2::Reflection::InitializeType<o2::UIMenuPanel::Item>();
	o2::Reflection::InitializeType<o2::Curve::Key>();
	o2::Reflection::InitializeType<CodeTool::Tmp>();
	o2::Reflection::InitializeType<CodeTool::ICppSyntaxStatementParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxNamespaceParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxMultilineCommentParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxCommentParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxPragmaParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxIncludeParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxDefineParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxIfMacroParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxClassParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxStructParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxTemplateParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxTypedefParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxEnumParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxUsingParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxPublicSectionParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxPrivateSectionParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxProtectedSectionParser>();
	o2::Reflection::InitializeType<CodeTool::CppSyntaxFriendParser>();
	o2::Reflection::InitializeType<CodeTool::SyntaxTree>();
	o2::Reflection::InitializeType<CodeTool::SyntaxFile>();
	o2::Reflection::InitializeType<CodeTool::SyntaxEntry>();
	o2::Reflection::InitializeType<CodeTool::SyntaxSection>();
	o2::Reflection::InitializeType<CodeTool::SyntaxNamespace>();
	o2::Reflection::InitializeType<CodeTool::SyntaxClass>();
	o2::Reflection::InitializeType<CodeTool::SyntaxType>();
	o2::Reflection::InitializeType<CodeTool::SyntaxVariable>();
	o2::Reflection::InitializeType<CodeTool::SyntaxFunction>();
	o2::Reflection::InitializeType<CodeTool::SyntaxEnum>();
	o2::Reflection::InitializeType<o2::AnimatedValue<Color4>>();
	o2::Reflection::InitializeType<o2::AnimatedValue<Color4>::Key>();
	o2::Reflection::InitializeType<o2::AnimatedValue<RectF>>();
	o2::Reflection::InitializeType<o2::AnimatedValue<RectF>::Key>();
	o2::Reflection::InitializeType<o2::AnimatedValue<bool>>();
	o2::Reflection::InitializeType<o2::AnimatedValue<bool>::Key>();

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
	TypeInitializer::AddBaseType<o2::UITree, o2::UIScrollArea>();
	TypeInitializer::AddBaseType<o2::UITreeNode, o2::UIWidget>();
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
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxNamespaceParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxMultilineCommentParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxCommentParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxPragmaParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxIncludeParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxDefineParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxIfMacroParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxClassParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxStructParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxTemplateParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxTypedefParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxEnumParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxUsingParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxPublicSectionParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxPrivateSectionParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxProtectedSectionParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::CppSyntaxFriendParser, CodeTool::ICppSyntaxStatementParser>();
	TypeInitializer::AddBaseType<CodeTool::SyntaxTree, o2::ISerializable>();
	TypeInitializer::AddBaseType<CodeTool::SyntaxFile, o2::ISerializable>();
	TypeInitializer::AddBaseType<CodeTool::SyntaxEntry, o2::ISerializable>();
	TypeInitializer::AddBaseType<CodeTool::SyntaxSection, CodeTool::SyntaxEntry>();
	TypeInitializer::AddBaseType<CodeTool::SyntaxNamespace, CodeTool::SyntaxSection>();
	TypeInitializer::AddBaseType<CodeTool::SyntaxClass, CodeTool::SyntaxSection>();
	TypeInitializer::AddBaseType<CodeTool::SyntaxType, CodeTool::SyntaxEntry>();
	TypeInitializer::AddBaseType<CodeTool::SyntaxVariable, CodeTool::SyntaxEntry>();
	TypeInitializer::AddBaseType<CodeTool::SyntaxFunction, CodeTool::SyntaxEntry>();
	TypeInitializer::AddBaseType<CodeTool::SyntaxEnum, CodeTool::SyntaxEntry>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<Color4>, o2::IAnimatedValue>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<Color4>::Key, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<RectF>, o2::IAnimatedValue>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<RectF>::Key, o2::ISerializable>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<bool>, o2::IAnimatedValue>();
	TypeInitializer::AddBaseType<o2::AnimatedValue<bool>::Key, o2::ISerializable>();

}