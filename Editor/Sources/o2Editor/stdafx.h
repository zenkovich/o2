#pragma once
#include "o2/stdafx.h"
#include "o2/Animation/AnimationClip.h"
#include "o2/Animation/AnimationMask.h"
#include "o2/Animation/AnimationState.h"
#include "o2/Animation/IAnimation.h"
#include "o2/Application/Application.h"
#include "o2/Application/Input.h"
#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetInfo.h"
#include "o2/Assets/Assets.h"
#include "o2/Assets/AssetsTree.h"
#include "o2/Assets/Types/AtlasAsset.h"
#include "o2/Assets/Types/BinaryAsset.h"
#include "o2/Assets/Types/DataAsset.h"
#include "o2/Assets/Types/FolderAsset.h"
#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Assets/Types/VectorFontAsset.h"
#include "o2/Config/ProjectConfig.h"
#include "o2/Events/ApplicationEventsListener.h"
#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Events/EventSystem.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Events/ShortcutKeysListener.h"
#include "o2/Render/BitmapFont.h"
#include "o2/Render/Camera.h"
#include "o2/Render/Font.h"
#include "o2/Render/FontRef.h"
#include "o2/Render/IDrawable.h"
#include "o2/Render/Mesh.h"
#include "o2/Render/Particle.h"
#include "o2/Render/ParticlesEffects.h"
#include "o2/Render/ParticlesEmitter.h"
#include "o2/Render/ParticlesEmitterShapes.h"
#include "o2/Render/RectDrawable.h"
#include "o2/Render/Texture.h"
#include "o2/Render/TextureRef.h"
#include "o2/Render/VectorFont.h"
#include "o2/Render/VectorFontEffects.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/ActorTransform.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Components/AnimationComponent.h"
#include "o2/Scene/ISceneDrawable.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/SceneLayer.h"
#include "o2/Scene/Tags.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/ContextMenu.h"
#include "o2/Scene/UI/Widgets/CustomDropDown.h"
#include "o2/Scene/UI/Widgets/CustomList.h"
#include "o2/Scene/UI/Widgets/DropDown.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/GridLayout.h"
#include "o2/Scene/UI/Widgets/HorizontalLayout.h"
#include "o2/Scene/UI/Widgets/HorizontalProgress.h"
#include "o2/Scene/UI/Widgets/HorizontalScrollBar.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/List.h"
#include "o2/Scene/UI/Widgets/LongList.h"
#include "o2/Scene/UI/Widgets/MenuPanel.h"
#include "o2/Scene/UI/Widgets/ScrollArea.h"
#include "o2/Scene/UI/Widgets/Spoiler.h"
#include "o2/Scene/UI/Widgets/Toggle.h"
#include "o2/Scene/UI/Widgets/Tree.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Scene/UI/Widgets/VerticalProgress.h"
#include "o2/Scene/UI/Widgets/VerticalScrollBar.h"
#include "o2/Scene/UI/Widgets/Window.h"

#include "o2/Utils/Ref.h"
#include "o2/Utils/WeakRef.h"
#include "o2/Utils/TypeTraits.h"

using namespace o2;

typedef Ref<AnimationClip> AnimationClipRef;
typedef Ref<AnimationMask> AnimationMaskRef;
typedef Ref<AnimationState> AnimationStateRef;
typedef Ref<IAnimation> IAnimationRef;
typedef Ref<Application> ApplicationRef;
typedef Ref<Input> InputRef;
typedef Ref<ActorAsset> ActorAssetRef;
typedef Ref<AnimationAsset> AnimationAssetRef;
typedef Ref<Asset> AssetRef;
typedef Ref<AssetInfo> AssetInfoRef;
typedef Ref<Assets> AssetsRef;
typedef Ref<AssetsTree> AssetsTreeRef;
typedef Ref<AtlasAsset> AtlasAssetRef;
typedef Ref<BinaryAsset> BinaryAssetRef;
typedef Ref<DataAsset> DataAssetRef;
typedef Ref<FolderAsset> FolderAssetRef;
typedef Ref<ImageAsset> ImageAssetRef;
typedef Ref<VectorFontAsset> VectorFontAssetRef;
typedef Ref<ProjectConfig> ProjectConfigRef;
typedef Ref<ApplicationEventsListener> ApplicationEventsListenerRef;
typedef Ref<CursorAreaEventsListener> CursorAreaEventsListenerRef;
typedef Ref<DrawableCursorEventsListener> DrawableCursorEventsListenerRef;
typedef Ref<EventSystem> EventSystemRef;
typedef Ref<KeyboardEventsListener> KeyboardEventsListenerRef;
typedef Ref<ShortcutKeysListener> ShortcutKeysListenerRef;
typedef Ref<BitmapFont> BitmapFontRef;
typedef Ref<Camera> CameraRef;
typedef Ref<Font> FontRef;
typedef Ref<IDrawable> IDrawableRef;
typedef Ref<Mesh> MeshRef;
typedef Ref<Particle> ParticleRef;
typedef Ref<ParticlesEffects> ParticlesEffectsRef;
typedef Ref<ParticlesEmitter> ParticlesEmitterRef;
typedef Ref<ParticlesEmitterShapes> ParticlesEmitterShapesRef;
typedef Ref<RectDrawable> RectDrawableRef;
typedef Ref<Texture> TextureRef;
typedef Ref<VectorFont> VectorFontRef;
typedef Ref<VectorFontEffects> VectorFontEffectsRef;
typedef Ref<Actor> ActorRef;
typedef Ref<ActorTransform> ActorTransformRef;
typedef Ref<Component> ComponentRef;
typedef Ref<AnimationComponent> AnimationComponentRef;
typedef Ref<ISceneDrawable> ISceneDrawableRef;
typedef Ref<DrawableComponent> DrawableComponentRef;
typedef Ref<Scene> SceneRef;
typedef Ref<SceneLayer> SceneLayerRef;
typedef Ref<UIManager> UIManagerRef;
typedef Ref<Widget> WidgetRef;
typedef Ref<WidgetLayer> WidgetLayerRef;
typedef Ref<WidgetLayout> WidgetLayoutRef;
typedef Ref<WidgetState> WidgetStateRef;
typedef Ref<Button> ButtonRef;
typedef Ref<ContextMenu> ContextMenuRef;
typedef Ref<CustomDropDown> CustomDropDownRef;
typedef Ref<CustomList> CustomListRef;
typedef Ref<DropDown> DropDownRef;
typedef Ref<EditBox> EditBoxRef;
typedef Ref<GridLayout> GridLayoutRef;
typedef Ref<HorizontalLayout> HorizontalLayoutRef;
typedef Ref<HorizontalProgress> HorizontalProgressRef;
typedef Ref<HorizontalScrollBar> HorizontalScrollBarRef;
typedef Ref<Image> ImageRef;
typedef Ref<Label> LabelRef;
typedef Ref<List> ListRef;
typedef Ref<LongList> LongListRef;
typedef Ref<MenuPanel> MenuPanelRef;
typedef Ref<ScrollArea> ScrollAreaRef;
typedef Ref<Spoiler> SpoilerRef;
typedef Ref<Toggle> ToggleRef;
typedef Ref<Tree> TreeRef;
typedef Ref<VerticalLayout> VerticalLayoutRef;
typedef Ref<VerticalProgress> VerticalProgressRef;
typedef Ref<VerticalScrollBar> VerticalScrollBarRef;
typedef Ref<Window> WindowRef;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
   return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename TargetType, typename BaseType>
Ref<TargetType> DynamicCast(const Ref<BaseType>& src)
{
   return Ref<TargetType>(dynamic_cast<TargetType*>(src.get()));
}

// Classes with "parent" variables using WeakRef<>
class AnimationState
{
   WeakRef<AnimationClip> m_clip;
};

class WidgetState
{
   WeakRef<Widget> m_parent;
};

class DrawableComponent
{
   WeakRef<Scene> m_scene;
};
