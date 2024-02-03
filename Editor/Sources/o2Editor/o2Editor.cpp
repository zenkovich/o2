extern void __RegisterClass__Editor__CurveKeysChangeAction();
extern void __RegisterClass__Editor__AnimationWindow();
extern void __RegisterClass__Editor__CurvesSheet();
extern void __RegisterClass__Editor__KeyHandlesSheet();
extern void __RegisterClass__Editor__AnimationPropertiesTree();
extern void __RegisterClass__Editor__AnimationPropertiesTreeNode();
extern void __RegisterClass__Editor__AnimationTimeline();
extern void __RegisterClass__Editor__AnimationKeyDragHandle();
extern void __RegisterClass__Editor__ITrackControl();
extern void __RegisterClass__Editor__MapKeyFramesTrackControl();
extern void __RegisterClass__Editor__Vec2KeyFramesTrackControl();
extern void __RegisterClass__Editor__AnimationTree();
extern void __RegisterClass__Editor__AnimationTreeNode();
extern void __RegisterClass__Editor__AssetIcon();
extern void __RegisterClass__Editor__AssetsIconsScrollArea();
extern void __RegisterClass__Editor__AssetsWindow();
extern void __RegisterClass__Editor__AssetsFoldersTree();
extern void __RegisterClass__Editor__FoldersTree();
extern void __RegisterClass__Editor__CreateAction();
extern void __RegisterClass__Editor__DeleteAction();
extern void __RegisterClass__Editor__DeleteAction__ObjectInfo();
extern void __RegisterClass__Editor__EnableAction();
extern void __RegisterClass__Editor__IAction();
extern void __RegisterClass__Editor__LockAction();
extern void __RegisterClass__Editor__PropertyChangeAction();
extern void __RegisterClass__Editor__ReparentAction();
extern void __RegisterClass__Editor__SelectAction();
extern void __RegisterClass__Editor__TransformAction();
extern void __RegisterClass__Editor__EditorConfig();
extern void __RegisterClass__Editor__EditorConfig__GlobalConfig();
extern void __RegisterClass__Editor__EditorConfig__ProjectConfig();
extern void __RegisterClass__Editor__ActorProperty();
extern void __RegisterClass__Editor__AssetProperty();
extern void __RegisterClass__Editor__BooleanProperty();
extern void __RegisterClass__Editor__BorderFProperty();
extern void __RegisterClass__Editor__BorderIProperty();
extern void __RegisterClass__Editor__ColorProperty();
extern void __RegisterClass__Editor__ComponentProperty();
extern void __RegisterClass__Editor__CurveProperty();
extern void __RegisterClass__Editor__EnumProperty();
extern void __RegisterClass__Editor__FloatProperty();
extern void __RegisterClass__Editor__FunctionProperty();
extern void __RegisterClass__Editor__IntegerProperty();
extern void __RegisterClass__Editor__ObjectProperty();
extern void __RegisterClass__Editor__ObjectPtrProperty();
extern void __RegisterClass__Editor__RectFProperty();
extern void __RegisterClass__Editor__RectIProperty();
extern void __RegisterClass__Editor__SceneLayerRefProperty();
extern void __RegisterClass__Editor__SceneLayersListProperty();
extern void __RegisterClass__Editor__ScriptValueProperty();
extern void __RegisterClass__Editor__StringProperty();
extern void __RegisterClass__Editor__TagsProperty();
extern void __RegisterClass__Editor__Vec2FProperty();
extern void __RegisterClass__Editor__Vec2IProperty();
extern void __RegisterClass__Editor__VectorProperty();
extern void __RegisterClass__Editor__WStringProperty();
extern void __RegisterClass__Editor__IObjectPropertiesViewer();
extern void __RegisterClass__Editor__IPropertyField();
extern void __RegisterClass__Editor__ObjectViewer();
extern void __RegisterClass__Editor__AnimationViewer();
extern void __RegisterClass__Editor__ImageAssetViewer();
extern void __RegisterClass__Editor__VectorFontAssetViewer();
extern void __RegisterClass__Editor__CameraActorViewer();
extern void __RegisterClass__Editor__AnimationStateViewer();
extern void __RegisterClass__Editor__MeshComponentViewer();
extern void __RegisterClass__Editor__SkinningMeshBoneComponentViewer();
extern void __RegisterClass__Editor__SkinningMeshComponentViewer();
extern void __RegisterClass__Editor__DefaultObjectPropertiesViewer();
extern void __RegisterClass__Editor__SpriteViewer();
extern void __RegisterClass__Editor__TextViewer();
extern void __RegisterClass__Editor__CustomFrameTool();
extern void __RegisterClass__Editor__FrameTool();
extern void __RegisterClass__Editor__IEditTool();
extern void __RegisterClass__Editor__MeshTopologyTool();
extern void __RegisterClass__Editor__MeshWeightsTool();
extern void __RegisterClass__Editor__MoveTool();
extern void __RegisterClass__Editor__RotateTool();
extern void __RegisterClass__Editor__ScaleTool();
extern void __RegisterClass__Editor__SelectionTool();
extern void __RegisterClass__Editor__SkeletonTool();
extern void __RegisterClass__Editor__SplineTool();
extern void __RegisterClass__o2__BasicUIStyleBuilder();
extern void __RegisterClass__Editor__EditorUIStyleBuilder();
extern void __RegisterClass__Editor__CurveAddKeysAction();
extern void __RegisterClass__Editor__CurveDeleteKeysAction();
Replace raw pointers with Ref<> in the code:

```cpp
Editor__CurveKeysChangeAction();
extern void __RegisterClass__Editor__CurvesEditor();
extern void __RegisterClass__Editor__CurvesEditor__CurveHandle();
extern void __RegisterClass__Editor__CurvesEditor__CurveCopyInfo();
extern void __RegisterClass__Editor__CurvePreview();
extern void __RegisterClass__Editor__FrameScrollView();
extern void __RegisterClass__Editor__ImageSlicesEditorWidget();
extern void __RegisterClass__Editor__ImageSlicesEditorWidget__PreviewImage();
extern void __RegisterClass__Editor__ScrollView();
extern void __RegisterClass__Editor__SpoilerWithHead();
extern void __RegisterClass__Editor__TexturePreview();
extern void __RegisterClass__Editor__DockWindowPlace();
extern void __RegisterClass__Editor__DockableWindow();
extern void __RegisterClass__Editor__IEditorWindow();
extern void __RegisterClass__Editor__WindowsLayout();
extern void __RegisterClass__Editor__WindowsLayout__WindowDockPlace();
extern void __RegisterClass__Editor__GameWindow();
extern void __RegisterClass__Editor__GameWindow__GameView();
extern void __RegisterClass__Editor__GameWindow__SimulationDevice();
extern void __RegisterClass__Editor__LogWindow();
extern void __RegisterClass__Editor__ActorViewer();
extern void __RegisterClass__Editor__AddComponentPanel();
extern void __RegisterClass__Editor__ComponentsTree();
extern void __RegisterClass__Editor__ComponentsTreeNode();
extern void __RegisterClass__Editor__DefaultActorComponentViewer();
extern void __RegisterClass__Editor__DefaultActorHeaderViewer();
extern void __RegisterClass__Editor__DefaultActorPropertiesViewer();
extern void __RegisterClass__Editor__DefaultActorTransformViewer();
extern void __RegisterClass__Editor__IActorComponentViewer();
extern void __RegisterClass__Editor__IActorHeaderViewer();
extern void __RegisterClass__Editor__IActorPropertiesViewer();
extern void __RegisterClass__Editor__IActorTransformViewer();
extern void __RegisterClass__Editor__AssetPropertiesViewer();
extern void __RegisterClass__Editor__DefaultPropertiesViewer();
extern void __RegisterClass__Editor__IPropertiesViewer();
extern void __RegisterClass__Editor__PropertiesWindow();
extern void __RegisterClass__Editor__DefaultWidgetLayerHeaderViewer();
extern void __RegisterClass__Editor__DefaultWidgetLayerLayoutViewer();
extern void __RegisterClass__Editor__DefaultWidgetLayerPropertiesViewer();
extern void __RegisterClass__Editor__IWidgetLayerHeaderViewer();
extern void __RegisterClass__Editor__IWidgetLayerLayoutViewer();
extern void __RegisterClass__Editor__IWidgetLayerPropertiesViewer();
extern void __RegisterClass__Editor__WidgetLayerViewer();
extern void __RegisterClass__Editor__LayersPopup();
extern void __RegisterClass__Editor__LayerPopupItem();
extern void __RegisterClass__Editor__SceneDragHandle();
extern void __RegisterClass__Editor__SceneEditScreen();
extern void __RegisterClass__Editor__SceneWindow();
extern void __RegisterClass__Editor__DrawOrderTree();
extern void __RegisterClass__Editor__DrawOrderTreeNode();
extern void __RegisterClass__Editor__SceneHierarchyTree();
extern void __RegisterClass__Editor__SceneHierarchyTreeNode();
extern void __RegisterClass__Editor__SceneTree();
extern void __RegisterClass__Editor__SceneTreeNode();
extern void __RegisterClass__Editor__TreeWindow();


extern void InitializeTypeso2Editor()
{
    __RegisterClass__Editor__AnimationAddKeysAction();
    __RegisterClass__Editor__AnimationDeleteKeysAction();
    __RegisterClass__Editor__AnimationKeysChangeAction();
    __RegisterClass__Editor__AnimationWindow();
    __RegisterClass__Editor__CurvesSheet();
    __RegisterClass__Editor__KeyHandlesSheet();
    __RegisterClass__Editor__AnimationPropertiesTree();
    __RegisterClass__Editor__AnimationPropertiesTreeNode();
    __RegisterClass__Editor__AnimationTimeline();
    __RegisterClass__Editor__AnimationKeyDragHandle();
    __RegisterClass__Editor__ITrackControl();
    __RegisterClass__Editor__MapKeyFramesTrackControl();
    __RegisterClass__Editor__Vec2KeyFramesTrackControl();
    __RegisterClass__Editor__AnimationTree();
    __RegisterClass__Editor__AnimationTreeNode();
    __RegisterClass__Editor__AssetIcon();
    __RegisterClass__Editor__AssetsIconsScrollArea();
    __RegisterClass__Editor__AssetsWindow();
    __RegisterClass__Editor__AssetsFoldersTree();
    __RegisterClass__Editor__FoldersTree();
    __RegisterClass__Editor__CreateAction();
    __RegisterClass__Editor__DeleteAction();
    __RegisterClass__Editor__DeleteAction__ObjectInfo();
    __RegisterClass__Editor__EnableAction();
    __RegisterClass__Editor__IAction();
    __RegisterClass__Editor__LockAction();
    __RegisterClass__Editor__PropertyChangeAction();
    __RegisterClass__Editor__ReparentAction();
    __RegisterClass__Editor__SelectAction();
    __RegisterClass__Editor__TransformAction();
    __RegisterClass__Editor__EditorConfig();
    __RegisterClass__Editor__EditorConfig__GlobalConfig();
    __RegisterClass__Editor__EditorConfig__ProjectConfig();
    __RegisterClass__Editor__Acto
```Replace raw pointers with Ref<> and const Ref<>& for function arguments:

```cpp
Ref<AssetProperty> property;
Ref<BooleanProperty> booleanProperty;
Ref<BorderFProperty> borderFProperty;
Ref<BorderIProperty> borderIProperty;
Ref<ColorProperty> colorProperty;
Ref<ComponentProperty> componentProperty;
Ref<CurveProperty> curveProperty;
Ref<EnumProperty> enumProperty;
Ref<FloatProperty> floatProperty;
Ref<FunctionProperty> functionProperty;
Ref<IntegerProperty> integerProperty;
Ref<ObjectProperty> objectProperty;
Ref<ObjectPtrProperty> objectPtrProperty;
Ref<RectFProperty> rectFProperty;
Ref<RectIProperty> rectIProperty;
Ref<SceneLayerRefProperty> sceneLayerRefProperty;
Ref<SceneLayersListProperty> sceneLayersListProperty;
Ref<ScriptValueProperty> scriptValueProperty;
Ref<StringProperty> stringProperty;
Ref<TagsProperty> tagsProperty;
Ref<Vec2FProperty> vec2FProperty;
Ref<Vec2IProperty> vec2IProperty;
Ref<VectorProperty> vectorProperty;
Ref<WStringProperty> wstringProperty;
Ref<IObjectPropertiesViewer> objectPropertiesViewer;
Ref<IPropertyField> propertyField;
Ref<ObjectViewer> objectViewer;
Ref<AnimationViewer> animationViewer;
Ref<ImageAssetViewer> imageAssetViewer;
Ref<VectorFontAssetViewer> vectorFontAssetViewer;
Ref<CameraActorViewer> cameraActorViewer;
Ref<AnimationStateViewer> animationStateViewer;
Ref<MeshComponentViewer> meshComponentViewer;
Ref<SkinningMeshBoneComponentViewer> skinningMeshBoneComponentViewer;
Ref<SkinningMeshComponentViewer> skinningMeshComponentViewer;
Ref<DefaultObjectPropertiesViewer> defaultObjectPropertiesViewer;
Ref<SpriteViewer> spriteViewer;
Ref<TextViewer> textViewer;
Ref<CustomFrameTool> customFrameTool;
Ref<FrameTool> frameTool;
Ref<IEditTool> editTool;
Ref<MeshTopologyTool> meshTopologyTool;
Ref<MeshWeightsTool> meshWeightsTool;
Ref<MoveTool> moveTool;
Ref<RotateTool> rotateTool;
Ref<ScaleTool> scaleTool;
Ref<SelectionTool> selectionTool;
Ref<SkeletonTool> skeletonTool;
Ref<SplineTool> splineTool;
Ref<o2::BasicUIStyleBuilder> basicUIStyleBuilder;
Ref<EditorUIStyleBuilder> editorUIStyleBuilder;
Ref<CurveAddKeysAction> curveAddKeysAction;
Ref<CurveDeleteKeysAction> curveDeleteKeysAction;
Ref<CurveKeysChangeAction> curveKeysChangeAction;
Ref<CurvesEditor> curvesEditor;
Ref<CurvesEditor::CurveHandle> curveHandle;
Ref<CurvesEditor::CurveCopyInfo> curveCopyInfo;
Ref<CurvePreview> curvePreview;
Ref<FrameScrollView> frameScrollView;
Ref<ImageSlicesEditorWidget> imageSlicesEditorWidget;
Ref<ImageSlicesEditorWidget::PreviewImage> previewImage;
Ref<ScrollView> scrollView;
Ref<SpoilerWithHead> spoilerWithHead;
Ref<TexturePreview> texturePreview;
Ref<DockWindowPlace> dockWindowPlace;
Ref<DockableWindow> dockableWindow;
Ref<IEditorWindow> editorWindow;
Ref<WindowsLayout> windowsLayout;
Ref<WindowsLayout::WindowDockPlace> windowDockPlace;
Ref<GameWindow> gameWindow;
Ref<GameWindow::GameView> gameView;
Ref<GameWindow::SimulationDevice> simulationDevice;
Ref<LogWindow> logWindow;
Ref<ActorViewer> actorViewer;
Ref<AddComponentPanel> addComponentPanel;
Ref<ComponentsTree> componentsTree;
Ref<ComponentsTreeNode> componentsTreeNode;
Ref<DefaultActorComponentViewer> defaultActorComponentViewer;
Ref<DefaultActorHeaderViewer> defaultActorHeaderViewer;
Ref<DefaultActorPropertiesViewer> defaultActorPropertiesViewer;
Ref<DefaultActorTransformViewer> defaultActorTransformViewer;
Ref<IActorComponentViewer> actorComponentViewer;
Ref<IActorHeaderViewer> actorHeaderViewer;
Ref<IActorPropertiesViewer> actorPropertiesViewer;
Ref<IActorTransformViewer> actorTransformViewer;
Ref<AssetPropertiesViewer> assetPropertiesViewer;
Ref<DefaultPropertiesViewer> defaultPropertiesViewer;
Ref<IPropertiesViewer> propertiesViewer;
Ref<PropertiesWindow> propertiesWindow;
Ref<DefaultWidgetLayerHeaderViewer> defaultWidgetLayerHeaderViewer;
Ref<DefaultWidgetLayerLayoutViewer> defaultWidgetLayerLayoutViewer;
Ref<DefaultWidgetLayerPropertiesViewer> defaultWidgetLayerPropertiesViewer;
Ref<IWidgetLayerHeaderViewer> widgetLayerHeaderViewer;
Ref<IWidgetLayerLayoutViewer> widgetLayerLayoutViewer;
Ref<IWidgetLayerPropertiesViewer> widgetLayerPropertiesViewer;
Ref<WidgetLayerViewer> widgetLayerViewer;
Ref<LayersPopup> layersPopup;
Ref<LayerPopupItem> layerPopupItem;
```

Replace `dynamic_cast<type*>` with `DynamicCast<type>`:

```cpp
DynamicCast<type>(pointer);
```#include <memory>

// Ref<>
template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// WeakRef<>
template <typename T>
using WeakRef = std::weak_ptr<T>;

// DynamicCast<>
template <typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr) {
    return std::dynamic_pointer_cast<T>(ptr);
}

void SceneDragHandle();
void __RegisterClass__Editor__SceneEditScreen();
void __RegisterClass__Editor__SceneWindow();
void __RegisterClass__Editor__DrawOrderTree();
void __RegisterClass__Editor__DrawOrderTreeNode();
void __RegisterClass__Editor__SceneHierarchyTree();
void __RegisterClass__Editor__SceneHierarchyTreeNode();
void __RegisterClass__Editor__SceneTree();
void __RegisterClass__Editor__SceneTreeNode();
void __RegisterClass__Editor__TreeWindow();

int main()
{
    SceneDragHandle();
    __RegisterClass__Editor__SceneEditScreen();
    __RegisterClass__Editor__SceneWindow();
    __RegisterClass__Editor__DrawOrderTree();
    __RegisterClass__Editor__DrawOrderTreeNode();
    __RegisterClass__Editor__SceneHierarchyTree();
    __RegisterClass__Editor__SceneHierarchyTreeNode();
    __RegisterClass__Editor__SceneTree();
    __RegisterClass__Editor__SceneTreeNode();
    __RegisterClass__Editor__TreeWindow();
    return 0;
}