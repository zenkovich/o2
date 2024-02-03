#include "o2Editor/stdafx.h"
#include "AssetsIconsScroll.h"

#include "o2/Animation/AnimationClip.h"
#include "o2/Animation/Tracks/AnimationFloatTrack.h"
#include "o2/Application/Application.h"
#include "o2/Assets/Assets.h"
#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Assets/Types/DataAsset.h"
#include "o2/Assets/Types/FolderAsset.h"
#include "o2/Render/Render.h"
#include "o2/Render/Sprite.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/Components/ImageComponent.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/Scene/UI/Widgets/GridLayout.h"
#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/Utils/StringUtils.h"
#include "o2Editor/AssetsWindow/AssetIcon.h"
#include "o2Editor/AssetsWindow/AssetsWindow.h"
#include "o2Editor/Core/Actions/Create.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/Properties/Properties.h"
#include "o2Editor/PropertiesWindow/PropertiesWindow.h"
#include "o2Editor/TreeWindow/SceneHierarchyTree.h"

namespace Editor
{
    AssetsIconsScrollArea::AssetsIconsScrollArea()
    {
        mDragIcon = mmake<AssetIcon>();

        mHighlightSprite = mmake<Ref<Sprite>>();
        mSelectionSprite = mmake<Ref<Sprite>>();

        mHighlightAnim->SetTarget(mHighlightSprite.get());
    }

    AssetsIconsScrollArea::AssetsIconsScrollArea(const AssetsIconsScrollArea& other) :
        GridLayoutScrollArea(other), mHighlightSprite(other.mHighlightSprite->CloneAsRef<Sprite>()),
        mHighlightLayout(other.mHighlightLayout), mHighlightAnim(other.mHighlightAnim),
        mSelectionSprite(other.mSelectionSprite->CloneAsRef<Sprite>())
    {
        PushEditorScopeOnStack scope;

        RemoveAllChildren();

        mDragIcon = o2UI.CreateWidget<AssetIcon>();

        if (other.mHighlighClip)
            mHighlighClip = other.mHighlighClip->CloneAsRef<AnimationClip>();

        mHighlightAnim->SetTarget(mHighlightSprite.get());
        mHighlightAnim->SetClip(mHighlighClip);

        RetargetStatesAnimations();
        SetLayoutDirty();
        InitializeContext();
    }

    AssetsIconsScrollArea::~AssetsIconsScrollArea()
    {
        delete mDragIcon;
        delete mHighlightSprite;
        delete mSelectionSprite;

        for (auto& kv : mIconsPool)
        {
            for (auto icon : kv.second)
                delete icon;
        }
    }

    AssetsIconsScrollArea& AssetsIconsScrollArea::operator=(const AssetsIconsScrollArea& other)
    {
        delete mDragIcon;
        delete mHighlightSprite;
        delete mSelectionSprite;

        ScrollArea::operator=(other);

        mDragIcon = other.mDragIcon->CloneAs<AssetIcon>();

        mContextMenu = FindChildByType<ContextMenu>();

        mHighlightLayout = other.mHighlightLayout;
        mHighlightSprite = other.mHighlightSprite->CloneAs<Ref<Sprite>>();
        mHighlighClip = other.mHighlighClip->CloneAsRef<AnimationClip>();
        mHighlightAnim->SetTarget(mHighlightSprite.get());
        mHighlightAnim->SetClip(mHighlighClip);

        mSelectionSprite = other.mSelectionSprite->CloneAs<Ref<Sprite>>();

        RetargetStatesAnimations();
        SetLayoutDirty();
        return *this;
    }

    void AssetsIconsScrollArea::Draw()
    {
        if (!mResEnabledInHierarchy)
            return;

        for (auto layer : mDrawingLayers)
            layer->Draw();

        IDrawable::OnDrawn();

        o2Render.EnableScissorTest(mAbsoluteClipArea);

        for (auto child : mChildrenInheritedDepth)
            child->Draw();

        if (mSelecting)
            mSelectionSprite->Draw();

        if (mHighlightSprite)
            mHighlightSprite->Draw();

        o2Render.DisableScissorTest();

        CursorAreaEventsListener::OnDrawn();

        for (auto layer : mTopDrawingLayers)
            layer->Draw();

        if (mOwnHorScrollBar)
            mHorScrollBar->Draw();

        if (mOwnVerScrollBar)
            mVerScrollBar->Draw();

        if (mIsDraggingIcons && mInstantiatedSceneDragObjects.Count() == 0)
            o2UI.DrawWidgetAtTop(mDragIcon);

        if (IsUIDebugEnabled() || o2Input.IsKeyDown(VK_F1))
            DrawDebugFrame();
    }

    void AssetsIconsScrollArea::Update(float dt)
    {
        ScrollArea::Update(dt);

        if (mHighlightAnim->IsPlaying())
        {
            if (mHighlightIcon)
            {
                mHighlightSprite->SetScale(Vec2F(1.0f, 1.0f));
                mHighlightSprite->SetRect(mHighlightLayout.Calculate(mHighlightIcon->layout->worldRect));
            }

            mHighlightAnim->Update(dt);

            if (!mHighlightAnim->IsPlaying())
                mHighlightIcon = nullptr;
        }

        if (mDragEnded)
            EndDragging();
    }

    void AssetsIconsScrollArea::SetViewingPath(const String& path)
    {
        if (path == mCurrentPath)
            return;

        mCurrentPath = path;
        UpdateAssetsByCurReplace raw pointers with Ref<> for all non-void* pointers.

```cpp
        void SanityCheck()
        {
            mAssetInfos = nullptr;
            mHighlightIcon = nullptr;
            mCurrentPath = nullptr;

            mItemSample = nullptr;

            mVisibleAssetIcons.clear();
            mSelectedPreloadedAssets.clear();
            mChangePropertiesTargetsFromThis = false;
        }

        void Initialize()
        {
            mHighlightIcon = mmake<AssetIcon>();

            addChild(mHighlightIcon);

            mHighlightIcon->SetState("highlight");
            mHighlightIcon->SetVisible(false);
            mHighlightAnim = mmake<StringAnimation>(mHighlightIcon);
            mHighlightAnim->SetStateName("highlight");
            mHighlightAnim->SetDirection(AnimDirection::Forward);
            mHighlightAnim->SetMode(AnimMode::Loop);
            mHighlightAnim->SetSpeed(1.0f);
            mHighlightAnim->SetAutoStart(false);
        }

        void Clear()
        {
            mVisibleAssetIcons.clear();
            mSelectedPreloadedAssets.clear();

            selectWidget(nullptr);
        }

        bool HandleInputEvent(const InputEventPtr& event) override
        {
            if (event->IsA<KeyEvent>())
            {
                auto keyEvent = static_cast<KeyEvent*>(event.get());

                if (keyEvent->IsA<KeyDownEvent>())
                {
                    auto downEvent = static_cast<KeyDownEvent*>(keyEvent);
                    if (downEvent->mKey == KEY_ESCAPE)
                    {
                        DeselectAllAssets();
                        return true;
                    }
                }
            }

            return ScrollArea::HandleInputEvent(event);
        }

        void CopyAssetPaths()
        {
            SafeClipboardCopy(mSelectedAssets.Convert<String>([](const Ref<AssetInfo>& x) { return x->path; }));
        }

        void CutAssetPaths()
        {
            CopyAssetPaths();

            o2EditorAssets.mCuttingAssets.Clear();

            for (auto asset : mSelectedAssets)
            {
                if (asset->meta->GetFlags() & AssetMeta::FlagNoDelete)
                    continue;

                o2EditorAssets.mCuttingAssets.Add({ asset->meta->ID(), asset->path });

                DeselectAsset(asset->meta->ID(), false);
            }

            UpdateCuttingAssets();
        }

        void PasteAssetPaths()
        {
            for (auto& asset : o2EditorAssets.mCuttingAssets)
            {
                String assetPath = o2Assets.GetAssetPath(asset.first);
                String assetFolder = o2FileSystem.ExtractPathStr(assetPath);

                if (mCurrentPath != assetFolder)
                    continue;

                PasteAssetFromCut(asset.first, asset.second);
            }
        }

        Ref<AssetMeta>* PasteAssetFromCut(UID uid, const String& oldAssetFolder)
        {
            String assetPath = o2Assets.GetAssetPath(uid);
            String assetFolder = o2FileSystem.ExtractPathStr(assetPath);

            String newPath = mCurrentPath + "/" + o2FileSystem.ExtractNameStr(oldAssetFolder) + o2FileSystem.ExtractExtensionStr(assetPath);
            String newRelativePath = mCurrentPath + "/" + o2FileSystem.ExtractNameStr(oldAssetFolder) + o2FileSystem.ExtractExtensionStr(assetPath);
            if (o2FileSystem.EFileExists(newPath))
                newPath = o2Assets.GetUniquePathInFolder(mCurrentPath, o2FileSystem.ExtractNameStr(oldAssetFolder), o2FileSystem.ExtractExtensionStr(assetPath));

            AssetRef oldAssetRef(uid);
            AssetRef newAssetRef(newPath, oldAssetRef.GetMeta());

            // Copy
            String originFP = o2FileSystem.GetFullPath(oldAssetFolder);
            String destFP = o2FileSystem.GetFullPath(mCurrentPath);
            bool destPathUnique = true;
            ComputeUniquePath(originFP, destFP, newRelativePath, destPathUnique, true);

            bool copySuccess = false;
            if (o2FileSystem.EFileExists(originFP) && destPathUnique)
            {
                String uniquePath;
                if (CopyFile(originFP, destFP, true, true, uniquePath))
                {
                    String newDestFP = destFP + "/" + uniquePath;
                    DeselectAsset(uid, false);
                    o2Assets.CreateNewAsset(newAssetRef, newDestFP);
                    copySuccess = true;
                }
            }

            if (!copySuccess)
            {
                OPP_LOGW("Move asset failed: can not copy file from '" + originFP + "' to '" + destFP + "'");
                return nullptr;
            }

            // Move
            UID moveID = uid;
            String moveFromFolder = oldAssetFolder;
            String moveToFP = destFP;
            String moveToFolder = mCurrentPath;

            OPP_LOGD("Move Asset From UID=" + moveID);
            OPP_LOGD("From: " + moveFromFolder);
            OPP_LOGD("To Path: " + moveToFolder);

            o2Assets.MoveAssetTo(moveID, moveFromFolder, moveToFolder, moveToFP);

            auto newAsset = Ref<AssetMeta>::FromUniquePath(newPath);
            SelectAsset(newAsset);

            return o2Assets.GetAssetRef(newAsset);
        }
#include <memory> // for shared_ptr
#include <type_traits> // for remove_pointer

template<class T>
struct TypeOf { };

template<class T>
using RemoveCVRef = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template<class T>
struct Ref
{
    Ref() : ptr(nullptr) { }
    Ref(const Ref& r) : ptr(r.ptr) { if (ptr) ptr->AddRef(); }
    template<class Y, class = typename std::enable_if<std::is_convertible<RemoveCVRef<Y>*, RemoveCVRef<T>*>::value>::type>
    Ref(const Ref<Y>& r) : ptr(r.ptr) { if (ptr) ptr->AddRef(); }
    explicit Ref(T* v) : ptr(v) { if (ptr) ptr->AddRef(); }
    template<class Y, class D, class = typename std::enable_if<std::is_convertible<RemoveCVRef<Y>*, RemoveCVRef<T>*>::value>::type>
    Ref(Y* p, D d) : ptr(p, d) { }
    template <class Y, class D, class = typename std::enable_if<std::is_convertible<RemoveCVRef<Y>*, RemoveCVRef<T>*>::value>::type>
    Ref(std::unique_ptr<Y, D>& p) : ptr(p.get(), std::default_delete<RemoveCVRef<Y> >()), unique_ptr_extractor(std::move(p)) { }
    ~Ref() { if (ptr) ptr->Release(); }
    Ref& operator=(T* newVal) { if (ptr) ptr->Release(); ptr = newVal; if (ptr) ptr->AddRef(); return *this; }
    template<class Y, class = typename std::enable_if<std::is_convertible<RemoveCVRef<Y>*, RemoveCVRef<T>*>::value>::type>
    Ref& operator=(const Ref<Y>& newVal) { if (ptr) ptr->Release(); ptr = newVal.ptr; if (ptr) ptr->AddRef(); return *this; }
    T* operator->() const { return ptr; }
    operator bool() const { return ptr != nullptr; }

private:
    T* ptr;
};

template<class Interface>
using WeakRef = Ref<Interface>;

template<class T, class... Args>
Ref<T> mmake(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template<class From, class To>
Ref<To> DynamicCast(const Ref<From>& r)
{
    return dynamic_cast<To*>(r.operator->());
}

type_of auto& ThisFunc = [](auto& a) { return a; };



class AssetsIconsScrollArea : public GridLayoutScrollArea, public ISelectionGroup
{
public:
    void SetSelectedAssets(const Vector<Ref<Asset>>& assets)
    {
        mSelectedAssets = assets;
    }

    void UpdateSelection(const Input::Cursor& cursor)
    {
        RectF selectionRect(mPressedPoint, Vec2F(cursor.position));

        mSelectionSprite->SetSizes(selectionRect.Width(), selectionRect.Height());
        mSelectionSprite->SetPosition(selectionRect.LeftBottom());

        for (auto icon : mVisibleAssetIcons)
        {
            auto iconSurface = icon->GetChildrenWorldRect();
            RectF iconRect(iconSurface.LeftBottom() + mScroll, iconSurface.RightTop() + mScroll);
            icon->SetSelected(selectionRect.Intersects(iconRect));
        }
    }

    void DeselectAllAssets()
    {
        for (auto icon : mVisibleAssetIcons)
            icon->SetSelected(false);

        mSelectedAssets.Clear();
    }

    AssetIcon* GetIconUnderPoint(const Vec2F& point)
    {
        for (auto icon : mVisibleAssetIcons)
        {
            auto iconSurface = icon->GetChildrenWorldRect();
            RectF iconRect(iconSurface.LeftBottom() + mScroll, iconSurface.RightTop() + mScroll);

            if (iconRect.Contains(point))
                return icon;
        }

        return nullptr;
    }

    void OnAssetsPropertiesChanged(const Vector<Ref<IObject>>& assets)
    {
        mChangePropertiesTargetsFromThis = true;

        if (assets.IsEmpty())
        {
            if (mSelectedPreloadedAssets.IsEmpty())
                targets.Clear();
            else
                targets = mSelectedPreloadedAssets.DynamicCast<IObject*>();
        }
        else
            targets = assets;

        o2EditorPropertiesWindow.SetTargets(targets, THIS_FUNC(OnAssetsPropertiesChanged));

        mChangePropertiesTargetsFromThis = false;
    }

    void UpdateSelfTransform()
    {
        Vec2F localPressPoint = mPressedPoint - GetChildrenWorldRect().LeftBottom();

        GridLayoutScrollArea::UpdateSelfTransform();

        mPressedPoint = localPressPoint + GetChildrenWorldRect().LeftBottom();

        if (mSelecting)
            UpdateSelection(*o2Input.GetCursor(0));
    }

    bool IsScrollable() const
    {
        return ScrollArea::IsScrollable();
    }

    bool IsInputTransparent() const
    {
        return ScrollArea::IsInputTransparent();
    }

    String GetCreateMenuCategory()
    {
        return "UI/Editor";
    }

    int GetItemsCount() const
    {
        return mAssetInfos.Count();
    }

    Vector<Ref<Asset>*> GetItemsRange(int start, int end) const
    {
        Vector<Ref<Asset>*> result;
        for (int i = start; i < end; i++)
            result.Add(Ref<Asset>*>(&mAssetInfos[i]));

        return result;
    }

    void SetupItemWidget(Widget* widget, Ref<Asset>* item)
    {
        auto asset = *item;
        AssetIcon* assetIcon = dynamic_cast<AssetIcon*>(widget);

        auto iconLayer = assetIcon->layer["icon"];
        auto iconSprite = dynamic_cast<Sprite*>(iconLayer->GetDrawable());

        if (asset->meta->GetAssetType() == &TypeOf(ImageAsset))
        {
            Ref<ImageAsset> previewSpriteAsset(asset->path);
            float previewMaxSize = 30;

            if (previewSpriteAsset->width > previewSpriteAsset->height)
            {
                float cf = previewSpriteAsset->height / previewSpriteAsset->width;
                iconLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(previewMaxSize, previewMaxSize * cf),
                    Vec2F(0, 10));
            }
            else
            {
                float cf = previewSpriteAsset->width / previewSpriteAsset->height;
                iconLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(previewMaxSize * cf, previewMaxSize),
                    Vec2F(0, 10));
            }

            iconSprite->image = previewSpriteAsset;
            iconSprite->mode = SpriteMode::Default;
        }
        else
        {
            iconSprite->imageName = asset->meta->GetAssetType()->InvokeStatic<String>("GetEditorIcon");
            iconSprite->mode = SpriteMode::FixedAspect;
            iconLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(40, 40), Vec2F(0, 10));
        }

        assetIcon->SetAssetInfo(asset);
        assetIcon->SetState("halfHide", mCuttingAssets.Contains([&](auto x) { return x.first == asset->meta->ID(); }));
        assetIcon->SetSelectionGroup(this);
        assetIcon->SetSelected(mSelectedAssets.Contains(asset));
        assetIcon->SetDragOnlySelected(true);
        assetIcon->mOwner = this;
    }

    void UpdateVisibleItems()
    {
        PushEditorScopeOnStack scope;
        GridLayoutScrollArea::UpdateVisibleItems();

        mVisibleAssetIcons = mChildWidgets.DynamicCast<AssetIcon*>();
    }

    void OnFocused()
    {
        for (auto icon : mChildWidgets)
            icon->SetState("focused", true);

        Widget::OnFocused();
    }

    void OnUnfocused()
    {
        for (auto icon : mChildWidgets)
            icon->SetState("focused", false);

        Widget::OnUnfocused();
    }

    void OnCursorPressed(const Input::Cursor& cursor)
    {
        if (auto icon = GetIconUnderPoint(cursor.position))
        {
            if (icon->IsSelected())
                return;
        }

        mPressedPoint = cursor.position;

        BeginSelecting();
    }

    void OnCursorStillDown(const Input::Cursor& cursor)
    {
        if (cursor.delta == Vec2F())
            return;

        if (mSelecting)
            UpdateSelection(cursor);
    }

    void BeginSelecting()
    {
        mSelecting = true;

        if (!o2Input.IsKeyDown(VK_CONTROL))
            DeselectAllAssets();

        o2UI.FocusWidget(Ref(this));

        RectF selectionRect(mPressedPoint, mPressedPoint);
        mSelectionSprite->SetSizes(selectionRect.Width(), selectionRect.Height());
        mSelectionSprite->SetPosition(selectionRect.LeftBottom());
    }

private:
    Vector<Ref<Asset>> mSelectedAssets;
    Vector<AssetIcon*> mVisibleAssetIcons;
    bool mSelecting;
    Vec2F mPressedPoint;
    Ref<Sprite> mSelectionSprite;
};replace raw pointers with Ref<>, except void* pointers.Ref<> is a smart pointer like shared_ptr. Use const Ref<>& for function arguments with pointer types, also in overridden methods. Dont replace void* pointers!! Remove default nullptr value for class members with type Ref<>. If class variable is some kind of "parent", use WeakRef<>. Use mmake<>() to create Ref<>, just like make_shared<>(). Replace dynamic_cast<type*> with DynamicCast<type> to cast Ref<>. 

/ntRect(selectionRect);

    mCurrentSelectingInfos.Clear();
}

void AssetsIconsScrollArea::OnCursorReleased(const Input::Cursor& cursor)
{
    if (mSelecting)
        CompleteSelecting();
    else if (!mIsDraggingIcons)
    {
        if (!o2Input.IsKeyDown(VK_CONTROL))
            DeselectAllAssets();
    }
}

void AssetsIconsScrollArea::BeginDragging(AssetIcon* icon)
{
    mIsDraggingIcons = true;
    mDragEnded = false;

    AssetIcon* dragIcon = nullptr;
    for (const auto& sel : mSelectedAssets)
    {
        if (auto icon = FindVisibleIcon(sel))
        {
            icon->Hide();
            dragIcon = icon;
        }
    }

    if (dragIcon)
    {
        mDragIcon = MMake<AssetIcon>(*dragIcon);
        mDragIcon->DragDropArea::SetInteractable(false);
        mDragIcon->CursorAreaEventsListener::SetInteractable(false);
        mDragIcon->SetEnabled(true);
        mDragIcon->RemoveFromScene();
        mDragOffset = icon->layout->worldCenter - o2Input.GetCursorPos();

        if (mSelectedAssets.Count() > 1)
            mDragIcon->assetName = (String)mSelectedAssets.Count() + " items";
    }
}

void AssetsIconsScrollArea::EndDragging(bool droppedToThis /*= false*/)
{
    for (const auto& sel : mSelectedAssets)
    {
        if (auto icon = FindVisibleIcon(sel))
            icon->Show();
    }

    mIsDraggingIcons = false;
}

void AssetsIconsScrollArea::UpdateDraggingGraphics()
{
    mDragIcon->SetEditableParent(nullptr);
    mDragIcon->layout->SetRect(RectF(o2Input.GetCursorPos() - mAssetIconSize * 0.5f + mDragOffset,
                                     o2Input.GetCursorPos() + mAssetIconSize * 0.5f + mDragOffset));

    mDragIcon->UpdateSelfTransform();
    mDragIcon->UpdateChildrenTransforms();
}

void AssetsIconsScrollArea::OnDragEnter(const Ref<ISelectableDragableObjectsGroup>& group)
{}

void AssetsIconsScrollArea::OnDraggedAbove(const Ref<ISelectableDragableObjectsGroup>& group)
{}

void AssetsIconsScrollArea::OnDragExit(const Ref<ISelectableDragableObjectsGroup>& group)
{}

void AssetsIconsScrollArea::OnDropped(const Ref<ISelectableDragableObjectsGroup>& group)
{
    if (DynamicCast<AssetsIconsScrollArea>(group))
        OnDroppedFromThis();
    else if (auto sceneTree = DynamicCast<SceneHierarchyTree>(group))
        OnDroppedFromSceneTree(sceneTree);
}

void AssetsIconsScrollArea::OnDroppedFromThis()
{
    for (const auto& sel : mSelectedAssets)
    {
        if (auto icon = FindVisibleIcon(sel))
            icon->Show();
    }

    AssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
    if (iconUnderCursor && iconUnderCursor->GetAssetInfo()->meta->GetAssetType() == &TypeOf(FolderAsset))
    {
        String destPath = iconUnderCursor->GetAssetInfo()->path;
        Vector<UID> assetsInfos = mSelectedAssets.Convert<UID>([](const Ref<AssetInfo>& x) { return x->meta->ID(); });
        DeselectAllAssets();

        o2Assets.MoveAssets(assetsInfos, destPath);
        o2Assets.RebuildAssets();
    }
}

void AssetsIconsScrollArea::OnDroppedFromSceneTree(const Ref<SceneHierarchyTree>& sceneTree)
{
    String destPath = mCurrentPath;

    AssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
    if (iconUnderCursor && iconUnderCursor->GetAssetInfo()->meta->GetAssetType() == &TypeOf(FolderAsset))
        destPath = iconUnderCursor->GetAssetInfo()->path;

    Vector<String> newAssets;
    for (const auto& object : sceneTree->GetSelectedObjects())
    {
        if (auto actor = DynamicCast<Actor>(object))
        {
            Ref<ActorAsset> newAsset = actor->MakePrototype();
            String path = destPath.IsEmpty() ? newAsset->GetActor()->name + String(".proto") : destPath + "/" +
                newAsset->GetActor()->name + String(".proto");

            String uniquePath = o2Assets.MakeUniqueAssetName(path);
            newAsset->Save(uniquePath);

            newAssets.Add(uniquePath);
        }
    }

    o2Assets.RebuildAssets();
    o2EditorAssets.OpenFolder(destPath);
    o2EditorAssets.SelectAssets(newAssets);
}

void AssetsIconsScrollArea::RegObjectsCreationAction()
{
    if (mInstantiatedSceneDragObjects.IsEmpty())
        return;

    auto firstInstObject = mInstantiatedSceneDragObjects[0];
    auto parent = firstInstObject->GetEditableParent();
    auto parentChilds = parent ? parent->GetEditableChildren() : o2Scene.GetRootEditableObjects();
    int idx = parentChilds.IndexOf(firstInstObject);
    auto prevActor = idx > 0 ? parentChilds[idx - 1] : nullptr;

    auto createAction = MMake<CreateAction>(mInstantiatedSceneDragObjects, parent, prevActor);

    o2EditorApplication.DoneAction(createAction);
}Ref<AssetIcon> sample = mmake<AssetIcon>(style);
        mIconsPool[style].push_back(sample);

        return sample;
    }

    void AssetsIconsScrollArea::ReturnAssetIconToPool(AssetIcon* icon)
    {
        for (auto& pair : mIconsPool)
        {
            if (pair.second.contains(icon))
            {
                pair.second.remove(icon);
                break;
            }
        }

        delete icon;
    }

    void AssetsIconsScrollArea::ClearPoolIcon()
    {
        for (auto& pair : mIconsPool)
        {
            for (auto icon : pair.second)
                delete icon;

            pair.second.clear();
        }

        mIconsPool.clear();
    }#include <memory>

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
class DynamicCast
{
public:
    DynamicCast(const Ref<T>& ref) : m_ref(ref) {}

    template<typename U>
    operator Ref<U>() const
    {
        return std::dynamic_pointer_cast<U>(m_ref);
    }

private:
    const Ref<T>& m_ref;
};

AssetIcon* AssetsIconsScrollArea::CreateAssetIcon()
{
    if (mIconsPool.ContainsKey(style))
    {
        Ref<AssetIcon> icon = mIconsPool[style].PopBack();
        icon->Init(this, style);
        return icon.get();
    }

    Ref<AssetIcon> icon = mmake<AssetIcon>();
    icon->Init(this, style);
    mAssetIcons.Append(icon);

    return icon.get();
}

void AssetsIconsScrollArea::FreeAssetIconToPool(AssetIcon* icon)
{
    if (mIconsPool.ContainsKey(icon->name))
        mIconsPool[icon->name].Add(Ref<AssetIcon>(icon));
    else
        delete icon;
}

AssetIcon* AssetsIconsScrollArea::FindVisibleIcon(const Ref<AssetInfo>& info)
{
    return mVisibleAssetIcons.FindOrDefault([&](const Ref<AssetIcon>& x) { return x && x->GetAssetInfo() == info; }).get();
}

void AssetsIconsScrollArea::OnAssetDblClick(const Ref<AssetIcon>& icon)
{
    AssetInfo iconAssetInfo = *icon->GetAssetInfo();
    auto assetNameLabel = icon->GetChildWidget("nameLabel");
    if (assetNameLabel && assetNameLabel->IsUnderPoint(o2Input.cursorPos))
    {
        auto name = o2FileSystem.GetFileNameWithoutExtension(o2FileSystem.GetPathWithoutDirectories(iconAssetInfo.path));
        StartAssetRenaming(icon, name, [&](const String& name)
                           {
                               String extension = o2FileSystem.GetFileExtension(iconAssetInfo.path);
                               String newName = extension.IsEmpty() ? name : name + "." + extension;
                               o2Assets.RenameAsset(iconAssetInfo.meta->ID(), newName);
                               o2Assets.RebuildAssets();

                               String parentPath = o2FileSystem.GetParentPath(iconAssetInfo.path);
                               String newPath = parentPath.IsEmpty() ? newName : parentPath + "/" + newName;
                               o2EditorAssets.SelectAsset(newPath);
                           });
    }
    else
    {
        if (iconAssetInfo.meta->GetAssetType() == &TypeOf(FolderAsset))
            o2EditorAssets.OpenFolder(iconAssetInfo.path);
        else
            o2EditorAssets.OpenAndEditAsset(iconAssetInfo.meta->ID());
    }
}

Ref<AssetIcon> AssetsIconsScrollArea::GetIconUnderPoint(const Vec2F& point) const
{
    for (const auto& child : mChildWidgets)
    {
        if (child->layout->IsPointInside(point))
        {
            auto icon = std::dynamic_pointer_cast<AssetIcon>(child);
            return icon;
        }
    }

    return nullptr;
}

Ref<Sprite> AssetsIconsScrollArea::GetHighlightDrawable() const
{
    return mHighlightSprite;
}

void AssetsIconsScrollArea::SetHighlightAnimation(const Ref<AnimationClip>& animation)
{
    mHighlighClip = animation;
    mHighlightAnim->SetClip(mHighlighClip);
    mHighlightAnim->SetTarget(mHighlightSprite);
}

void AssetsIconsScrollArea::SetHighlightLayout(const Layout& layout)
{
    mHighlightLayout = layout;
}

Ref<Sprite> AssetsIconsScrollArea::GetSelectingDrawable() const
{
    return mSelectionSprite;
}

bool AssetsIconsScrollArea::IsUnderPoint(const Vec2F& point)
{
    return Widget::IsUnderPoint(point);
}

#undef CopyFile

void AssetsIconsScrollArea::OnContextCopyPressed()
{
    o2EditorAssets.CopyAssets(
        mSelectedAssets.Convert<String>([](const Ref<AssetInfo>& x) { return x->path; }));
}

void AssetsIconsScrollArea::OnContextCutPressed()
{
    o2EditorAssets.CutAssets(
        mSelectedAssets.Convert<String>([](const Ref<AssetInfo>& x) { return x->path; }));
}

void AssetsIconsScrollArea::OnContextPastePressed()
{
    o2EditorAssets.PasteAssets(mCurrentPath);
    UpdateAssetsByCurrentPath();
}

void AssetsIconsScrollArea::OnContextDeletePressed()
{
    o2EditorPropertiesWindow.ResetTargets();

    mSelectedPreloadedAssets.Clear();

    o2EditorAssets.DeleteAssets(
        mSelectedAssets.Convert<String>([](const Ref<AssetInfo>& x) { return x->path; }));
}

void AssetsIconsScrollArea::OnContextOpenPressed()
{
    if (!mSelectedAssets.empty())
        o2EditorAssets.OpenAndEditAsset(mSelectedAssets.Last()->meta->ID());
}

void AssetsIconsScrollArea::OnContextShowInExplorerPressed()
{
    if (!mSelectedAssets.empty())
        o2EditorAssets.OpenAsset(mSelectedAssets.Last()->meta->ID());
}

void AssetsIconsScrollArea::CreateAsset(const Type* assetType)
{
    String newAssetName = "New " + GetSmartName(assetType->GetName());

    auto objectType = dynamic_cast<const ObjectType*>(assetType);

    mNewAsset = objectType->DynamicCastToIObject();
    mNewAsset->SetPath(mCurrentPath + "/" + newAssetName);
    mAssetInfos.Add(Ref<AssetInfo>(mNewAsset->GetInfo()));

    SortAssetInfos();
    OnItemsUpdated(true);
    ScrollTo(static_cast<void*>(&mNewAsset->GetInfo()));

    Ref<AssetIcon> icon = CreateAssetIcon();
    if (icon)
        icon->SetAssetInfo(mNewAsset->GetInfo());
}#include <memory>
#include <vector>
#include <functional>
#include <type_traits>

template<class T>
class Ref
{
public:
    Ref() = default;
    Ref(T* ptr) : m_ptr(ptr) {}
    Ref(const Ref<T>& other) : m_ptr(other.m_ptr) {}
    template<class U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    Ref(const Ref<U>& other) : m_ptr(other.m_ptr) {}

    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }

    T* get() const { return m_ptr; }

    template<class U>
    Ref<U> DynamicCast() const
    {
        return Ref<U>(dynamic_cast<U*>(m_ptr.get()));
    }

    explicit operator bool() const { return m_ptr != nullptr; }

private:
    std::shared_ptr<T> m_ptr;
};

template<typename T>
Ref<T> MakeRef()
{
    return Ref<T>(new T());
}

template<typename T, typename... Args>
Ref<T> MakeRef(Args&&... args)
{
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template<class T>
class WeakRef
{
public:
    WeakRef() = default;
    WeakRef(const Ref<T>& ref) : m_ptr(ref.get()) {}
    WeakRef(const WeakRef<T>& other) : m_ptr(other.m_ptr) {}

    operator bool() const { return !expired(); }

    bool expired() const { return !m_ptr.lock(); }

    Ref<T> lock() const
    {
        return Ref<T>(m_ptr.lock());
    }

private:
    std::weak_ptr<T> m_ptr;
};

class ImageAsset {};

template<class T>
class Vector
{
public:
    void Add(const T& item)
    {
        m_items.push_back(item);
    }

    bool IsEmpty() const
    {
        return m_items.empty();
    }

    const T& operator[](size_t index) const
    {
        return m_items[index];
    }

private:
    std::vector<T> m_items;
};

class String {};

template<class T>
class DynamicCast
{
public:
    template<class U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    DynamicCast(const Ref<U>& ref) : m_ptr(ref.get())
    {
        static_assert(std::is_pointer_v<T>, "T must be a pointer type");
        static_assert(std::is_same_v<std::remove_cvref_t<decltype(*m_ptr)>, std::remove_cvref_t<U>>, "U must be the same type as T");
    }

    operator T() const { return m_ptr; }

private:
    T m_ptr;
};

class AssetInfo
{
public:
    void* GetInfo() { return nullptr; }
};

class AssetIcon {};
class Actor {};
class ImageComponent
{
public:
    ImageComponent(const Ref<ImageAsset>& asset) : m_asset(asset) {}

    void FitActorByImage() {}

private:
    Ref<ImageAsset> m_asset;
};

class SelectableDragableObject
{
public:
    virtual bool IsSelected() const = 0;
    virtual void SetSelected(bool selected) = 0;
};

class AssetsIconsScrollArea
{
public:
    void RenameAsset(AssetIcon* icon, const String& newAssetName)
    {
        if (!icon)
            return;

        auto assetType = dynamic_cast<Ref<AssetIcon>*>(icon->GetAssetInfo());
        if (!assetType)
            return;

        auto extesions = assetType->InvokeStatic<Vector<String>>("GetFileExtensions");
        auto extension = !extesions.IsEmpty() ? extesions[0] : String("");

        StartAssetRenaming(icon, newAssetName,
            [&, extension](const String& name)
            {
                String path = (!mCurrentPath.IsEmpty() ? mCurrentPath + "/" + name : name);
                if (!extension.IsEmpty())
                    path += "." + extension;

                mNewAsset->Save(path);

                o2Assets.RebuildAssets();
                o2EditorAssets.SelectAsset(path);
            });
    }

    Actor* AssetsIconsScrollArea::InstantiateAsset(const ImageAssetRef& asset)
    {
        Ref<Actor> actor = MakeRef<Actor>();
        Ref<ImageComponent> comp = MakeRef<ImageComponent>(asset);
        actor->AddComponent(comp);
        comp->FitActorByImage();
        return actor.get();
    }

    Actor* AssetsIconsScrollArea::InstantiateAsset(const Ref<ActorAsset>& asset)
    {
        return asset->GetActor()->CloneAs<Actor>().get();
    }

    void AssetsIconsScrollArea::OnAssetsPropertiesChanged()
    {
        for (const Ref<AssetIcon>& asset : mSelectedPreloadedAssets)
            (*asset)->SetDirty();

        CheckPreloadedAssetsSaving();
    }

    void AssetsIconsScrollArea::CheckPreloadedAssetsSaving()
    {
        if (mChangePropertiesTargetsFromThis)
            return;

        for (const Ref<AssetIcon>& asset : mSelectedPreloadedAssets)
        {
            (*asset)->Save();
            delete asset.get();
        }

        mSelectedPreloadedAssets.Clear();
    }

    void AssetsIconsScrollArea::Select(SelectableDragableObject* object, bool sendOnSelectionChanged)
    {
        AssetIcon* icon = dynamic_cast<AssetIcon*>(object);
        Ref<AssetInfo> info = icon->GetAssetInfo();

        if (!mSelectedAssets.Contains(info))
        {
            icon->SetSelected(true);
            mSelectedAssets.Add(info);

            if (sendOnSelectionChanged)
                OnAssetsSelected();
        }
    }

    void AssetsIconsScrollArea::Deselect(SelectableDragableObject* object)
    {
        AssetIcon* icon = dynamic_cast<AssetIcon*>(object);
        Ref<AssetInfo> info = icon->GetAssetInfo();

        if (mSelectedAssets.Contains(info))
        {
            icon->SetSelected(false);
            mSelectedAssets.Remove(info);

            OnAssetsSelected();
        }
    }

    void AssetsIconsScrollArea::AddSelectableObject(SelectableDragableObject* object)
    {}

    void AssetsIconsScrollArea::RemoveSelectableObject(const Ref<SelectableDragableObject>& object)
    {}

    void AssetsIconsScrollArea::OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor)
    {
        // 		if ((mPressedPoint - cursor.position).Length() > 5.0f)
        // 			return;

        if (!o2Input.IsKeyDown(VK_CONTROL) && !o2Input.IsKeyDown(VK_SHIFT))
            DeselectAllAssets();

        if (o2Input.IsKeyDown(VK_SHIFT) && !mSelectedAssets.IsEmpty())
        {
            AssetIcon* selectIcon = dynamic_cast<AssetIcon*>(object);
            Ref<AssetInfo> selectInfo = selectIcon->GetAssetInfo();
            int iconUnderCursorIdx = mAssetInfos.IndexOf(selectInfo);
            int lastSelectedIdx = mAssetInfos.IndexOf(mSelectedAssets.Last());

            int begin = Math::Min(iconUnderCursorIdx, lastSelectedIdx);
            int end = Math::Max(iconUnderCursorIdx, lastSelectedIdx);

            for (int i = begin; i <= end; i++)
            {
                Ref<AssetInfo> info = mAssetInfos[i];
                if (mSelectedAssets.Contains(info))
                    continue;

                if (AssetIcon* icon = FindVisibleIcon(info))
                    Select(icon);
                else
                    mSelectedAssets.Add(info);
            }
        }
        else
        {
            if (object->IsSelected())
                Deselect(object);
            else
                Select(object);
        }
    }

    void AssetsIconsScrollArea::OnSelectableObjectBeganDragging(SelectableDragableObject* object)
    {
        if (mSelecting)
        {
            Select(object, false);
            mSelecting = false;
        }

        if (!o2Input.IsKeyDown(VK_CONTROL) && !mSelectedAssets.Contains(dynamic_cast<AssetIcon*>(object)->GetAssetInfo()))
        {
            for (const Ref<AssetInfo>& info : mSelectedAssets)
            {
                if (AssetIcon* icon = FindVisibleIcon(info))
                    icon->SetSelected(false);
            }

            mSelectedAssets.Clear();
        }

        if (!object->IsSelected())
            Select(object, false);
    }

    Actor* AssetsIconsScrollArea::InstantiateAsset(const AssetInfo& assetInfo)
    {
        if (assetInfo.meta->GetAssetType() == &TypeOf(ImageAsset))
            return InstantiateAsset(MakeRef<ImageAsset>(assetInfo)).get();
        return nullptr;
    }

private:
    Vector<Ref<AssetInfo>> mSelectedAssets;
    Vector<Ref<AssetIcon>> mSelectedPreloadedAssets;

    Ref<Actor> InstantiateAsset(const ImageAsset asset);
    Ref<Actor> InstantiateAsset(const Ref<ActorAsset>& asset);
    void CheckPreloadedAssetsSaving();
    void OnAssetsSelected();
    Ref<AssetIcon> FindVisibleIcon(const Ref<AssetInfo>& info);
};

int main()
{
    return 0;
}#include <memory>
#include <functional>
#include <vector>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

class AssetInfo;

class SelectableDragableObject {};

class AssetIcon {
public:
    void SetState(const std::string& state, bool value) {}

    SelectableDragableObject* GetChild(const std::string& name) { return nullptr; }
};

class EditBox {
public:
    std::wstring text;

    using WString = std::wstring;
    using Function = std::function<void(const std::string&)>;

    Function onChangeCompleted;

    void Focus() {}

    void ResetScroll() {}

    void SelectAll() {}
};

namespace Editor {
    class AssetsIconsScrollArea {
    public:
        Ref<AssetInfo> InstantiateAsset(const Ref<AssetInfo>& assetInfo)
        {
            if (assetInfo && assetInfo->meta->GetAssetType() == TypeOf(ScriptAsset))
                return InstantiateAsset(Ref<ScriptAsset>(assetInfo->meta->ID()));
            else if (assetInfo && assetInfo->meta->GetAssetType() == TypeOf(ActorAsset))
                return InstantiateAsset(Ref<ActorAsset>(assetInfo->meta->ID()));

            return nullptr;
        }

        Vector<SelectableDragableObject*> GetSelectedDragObjects() const
        {
            return mSelectedAssets.Convert<SelectableDragableObject*>([](const Ref<AssetInfo>& x) { return (SelectableDragableObject*)x.Get(); });
        }

        Vector<SelectableDragableObject*> GetAllObjects() const
        {
            return mChildWidgets.Cast<SelectableDragableObject*>();
        }

        void Select(SelectableDragableObject* object)
        {
            Select(object, true);
        }

        void StartAssetRenaming(AssetIcon* icon, const std::string& name,
            const std::function<void(const std::string&)>& onCompleted)
        {
            icon->SetState("edit", true);

            auto editBox = (EditBox*)icon->GetChild("nameEditBox");
            editBox->text = name;

            editBox->SelectAll();
            editBox->Focus();
            editBox->ResetScroll();

            editBox->onChangeCompleted = [=](const EditBox::WString& text) {
                icon->SetState("edit", false);
                onCompleted(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(text));
            };
        }

    private:
        std::vector<Ref<AssetInfo>> mSelectedAssets;
        std::vector<SelectalbeDragableObject*> mChildWidgets;

        template<typename T>
        void InstantiateAsset(const Ref<T>& asset)
        {
            // Implementation for InstantiateAsset
        }
    };

    class AssetInfo {
    public:
        const MetaAsset* meta;
    };

    class MetaAsset {
    public:
        virtual const std::type_info& GetAssetType() const = 0;
        virtual std::string ID() const = 0;
    };

    class ScriptAsset : public MetaAsset {
    public:
        const MetaAsset* GetAssetType() const override { return &TypeOf(ScriptAsset); }
        std::string ID() const override { return {}; }
    };

    class ActorAsset : public MetaAsset {
    public:
        const MetaAsset* GetAssetType() const override { return &TypeOf(ActorAsset); }
        std::string ID() const override { return {}; }
    };

    template<typename T>
    const MetaAsset* TypeOf()
    {
        static_assert(std::is_base_of_v<MetaAsset, T>, "T must be derived from MetaAsset");

        return &T::instance;
    }

    // --- META ---

    class Editor__AssetsIconsScrollArea : public AssetsIconsScrollArea {};
    // --- END META ---

} // namespace Editor