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
#include "o2Editor/TreeWindow/SceneTree.h"

namespace Editor
{
	AssetsIconsScrollArea::AssetsIconsScrollArea()
	{
		mDragIcon = mnew AssetIcon();

		mHighlightSprite = mnew Sprite();
		mSelectionSprite = mnew Sprite();

		mHighlightAnim.SetTarget(mHighlightSprite);
	}

	AssetsIconsScrollArea::AssetsIconsScrollArea(const AssetsIconsScrollArea& other):
		GridLayoutScrollArea(other), mHighlightSprite(other.mHighlightSprite->CloneAs<Sprite>()),
		mHighlightLayout(other.mHighlightLayout), mHighlightAnim(other.mHighlightAnim),
		mSelectionSprite(other.mSelectionSprite->CloneAs<Sprite>())
	{
		PushEditorScopeOnStack scope;

		RemoveAllChildren();

		mDragIcon = o2UI.CreateWidget<AssetIcon>();

		mHighlighClip = other.mHighlighClip;
		mHighlightAnim.SetTarget(mHighlightSprite);
		mHighlightAnim.SetClip(&mHighlighClip);

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
		mHighlightSprite = other.mHighlightSprite->CloneAs<Sprite>();
		mHighlighClip = other.mHighlighClip;
		mHighlightAnim.SetTarget(mHighlightSprite);
		mHighlightAnim.SetClip(&mHighlighClip);

		mSelectionSprite = other.mSelectionSprite->CloneAs<Sprite>();

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

		for (auto child : mDrawingChildren)
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

		if (mHighlightAnim.IsPlaying())
		{
			if (mHighlightIcon)
			{
				mHighlightSprite->SetScale(Vec2F(1.0f, 1.0f));
				mHighlightSprite->SetRect(mHighlightLayout.Calculate(mHighlightIcon->layout->worldRect));
			}

			mHighlightAnim.Update(dt);

			if (!mHighlightAnim.IsPlaying())
				mHighlightIcon = nullptr;
		}

		if (mDragEnded)
			EndDragging();

		if (mNeedRebuildAssets)
		{
			mNeedRebuildAssets = false;
			o2Assets.RebuildAssets();
		}
	}

	void AssetsIconsScrollArea::SetViewingPath(const String& path)
	{
		if (path == mCurrentPath)
			return;

		mCurrentPath = path;
		UpdateAssetsByCurrentPath();
		ResetScroll();
	}

	const String& AssetsIconsScrollArea::GetViewingPath() const
	{
		return mCurrentPath;
	}

	void AssetsIconsScrollArea::UpdateAssetsByCurrentPath()
	{
		PushEditorScopeOnStack scope;

		Focus();
		DeselectAllAssets();

		if (mCurrentPath != "")
		{
			FolderAssetRef ref(mCurrentPath);
			mAssetInfos = ref->GetInfo().GetChildren().Cast<const AssetInfo*>();
		}
		else mAssetInfos = o2Assets.GetAssetsTree().rootAssets.Cast<const AssetInfo*>();

		SortAssetInfos();
		OnItemsUpdated(true);
	}

	void AssetsIconsScrollArea::SortAssetInfos()
	{
		Map<const AssetInfo*, Pair<String, int>> sortingCache;
		for (auto assetInfo : mAssetInfos)
		{
			sortingCache[assetInfo] = { assetInfo->path.ToLowerCase(),
				assetInfo->meta->GetAssetType()->InvokeStatic<int>("GetEditorSorting") };
		}

		mAssetInfos.Sort([&](const AssetInfo* a, const AssetInfo* b) {
			if (a->meta->GetAssetType() == b->meta->GetAssetType())
				return sortingCache[a].first < sortingCache[b].first;

			return sortingCache[a].second > sortingCache[b].second;
		});
	}

	bool AssetsIconsScrollArea::IsFocusable() const
	{
		return true;
	}

	void AssetsIconsScrollArea::UpdateCuttingAssets()
	{
		for (auto child : mChildWidgets)
		{
			AssetIcon* icon = (AssetIcon*)child;
			icon->SetState("halfHide", o2EditorAssets.mCuttingAssets.Contains([=](auto x) {
				return x.first == icon->GetAssetInfo().meta->ID(); }));
		}
	}

	void AssetsIconsScrollArea::HighlightAsset(const UID& id)
	{
		String assetPath = o2Assets.GetAssetPath(id);
		String assetFolder = o2FileSystem.ExtractPathStr(assetPath);

		SetViewingPath(assetFolder);

		auto assetInfo = mAssetInfos.FindOrDefault([&](auto* x) { return x->meta->ID() == id; });
		ScrollTo((void*)assetInfo);

		AssetIcon* icon = (AssetIcon*)(mChildWidgets.FindOrDefault([=](Widget* x) {
			return &((AssetIcon*)x)->GetAssetInfo() == assetInfo; }));

		if (!icon)
			return;

		mHighlightIcon = icon;
		mHighlightAnim.RewindAndPlay();
	}

	void AssetsIconsScrollArea::SelectAsset(const UID& id, bool scroll /*= true*/)
	{
		bool selectionChanged = false;

		String assetPath = o2Assets.GetAssetPath(id);
		String assetFolder = o2FileSystem.ExtractPathStr(assetPath);

		SetViewingPath(assetFolder);

		auto assetInfo = &o2Assets.GetAssetInfo(id);
		if (!assetInfo->IsValid())
			return;

		if (!mSelectedAssets.Contains(assetInfo))
		{
			if (auto assetIcon = FindVisibleIcon(assetInfo))
				assetIcon->SetSelected(true);

			mSelectedAssets.Add(assetInfo);

			selectionChanged = true;
		}

		if (scroll)
		{
			int idx = mAssetInfos.IndexOf(assetInfo);
			float itemHeight = mItemSample->layout->minHeight;
			int itemsInLine = Math::Max(1, Math::FloorToInt(mAbsoluteViewArea.Width()/mItemSample->layout->GetMinWidth()));

			SetScroll(Vec2F(0.0f, idx/itemsInLine*itemHeight));
		}

		if (selectionChanged)
			OnAssetsSelected();
	}

	void AssetsIconsScrollArea::DeselectAllAssets()
	{
		for (auto icon : mVisibleAssetIcons)
			icon->SetSelected(false);

		mSelectedAssets.Clear();

		OnAssetsSelected();
	}

	const Vector<const AssetInfo*>& AssetsIconsScrollArea::GetSelectedAssets() const
	{
		return mSelectedAssets;
	}

	void AssetsIconsScrollArea::OnAssetsSelected()
	{
		auto lastSelectedPreloadedAssets = mSelectedPreloadedAssets;
		for (auto asset : lastSelectedPreloadedAssets)
		{
			if (!mSelectedAssets.Contains([&](const AssetInfo* x) {
				return x->meta->ID() == (*asset)->GetUID(); }))
			{
				mSelectedPreloadedAssets.Remove(asset);
				(*asset)->Save(false);
				delete asset;
			}
		}

		for (auto icon : mSelectedAssets)
		{
			if (mSelectedPreloadedAssets.Contains([&](const AssetRef* x) { return (*x)->GetUID() == icon->meta->ID(); }))
				continue;

			AssetRef* iconAsset = o2Assets.GetAssetRef(icon->meta->ID()).CloneAs<AssetRef>();
			mSelectedPreloadedAssets.Add(iconAsset);
		}

		onAssetsSelected(mSelectedAssets.Convert<String>([](const AssetInfo* x) { return x->path; }));

		if (PropertiesWindow::IsSingletonInitialzed())
		{
			mChangePropertiesTargetsFromThis = true;

			Vector<IObject*> targets;

			if (mSelectedPreloadedAssets.All([](AssetRef* x) { return (*x)->GetType() == TypeOf(ActorAsset); }))
			{
				targets = mSelectedPreloadedAssets.Convert<IObject*>([](AssetRef* x) {
					ActorAssetRef asset(*x); return asset->GetActor(); });
			}
			else if (mSelectedPreloadedAssets.All([](AssetRef* x) { return (*x)->GetType() == TypeOf(FolderAsset); }))
				targets.Clear();
			else
				targets = mSelectedPreloadedAssets.DynamicCast<IObject*>();

			if (!targets.IsEmpty())
				o2EditorPropertiesWindow.SetTargets(targets, THIS_FUNC(OnAssetsPropertiesChanged));

			mChangePropertiesTargetsFromThis = false;
		}
	}

	void AssetsIconsScrollArea::UpdateSelfTransform()
	{
		Vec2F localPressPoint = mPressedPoint - GetChildrenWorldRect().LeftBottom();

		GridLayoutScrollArea::UpdateSelfTransform();

		mPressedPoint = localPressPoint + GetChildrenWorldRect().LeftBottom();

		if (mSelecting)
			UpdateSelection(*o2Input.GetCursor(0));
	}

	bool AssetsIconsScrollArea::IsScrollable() const
	{
		return ScrollArea::IsScrollable();
	}

	bool AssetsIconsScrollArea::IsInputTransparent() const
	{
		return ScrollArea::IsInputTransparent();
	}

	String AssetsIconsScrollArea::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	int AssetsIconsScrollArea::GetItemsCount() const
	{
		return mAssetInfos.Count();
	}

	Vector<void*> AssetsIconsScrollArea::GetItemsRange(int start, int end) const
	{
		return mAssetInfos.Take(start, end).Cast<void*>();
	}

	void AssetsIconsScrollArea::SetupItemWidget(Widget* widget, void* item)
	{
		AssetInfo* asset = (AssetInfo*)item;
		AssetIcon* assetIcon = dynamic_cast<AssetIcon*>(widget);

		auto iconLayer = assetIcon->layer["icon"];
		auto iconSprite = dynamic_cast<Sprite*>(iconLayer->GetDrawable());

		if (asset->meta->GetAssetType() == &TypeOf(ImageAsset))
		{
			ImageAssetRef previewSpriteAsset(asset->path);
			float previewMaxSize = 30;

			if (previewSpriteAsset->width > previewSpriteAsset->height)
			{
				float cf = previewSpriteAsset->height / previewSpriteAsset->width;
				iconLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(previewMaxSize, previewMaxSize*cf),
												  Vec2F(0, 10));
			}
			else
			{
				float cf = previewSpriteAsset->width / previewSpriteAsset->height;
				iconLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(previewMaxSize*cf, previewMaxSize),
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

	void AssetsIconsScrollArea::UpdateVisibleItems()
	{
		PushEditorScopeOnStack scope;
		GridLayoutScrollArea::UpdateVisibleItems();

		mVisibleAssetIcons = mChildWidgets.DynamicCast<AssetIcon*>();
	}

	void AssetsIconsScrollArea::OnFocused()
	{
		for (auto icon : mChildWidgets)
			icon->SetState("focused", true);

		Widget::OnFocused();
	}

	void AssetsIconsScrollArea::OnUnfocused()
	{
		for (auto icon : mChildWidgets)
			icon->SetState("focused", false);

		Widget::OnUnfocused();
	}

	void AssetsIconsScrollArea::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (auto icon = GetIconUnderPoint(cursor.position))
		{
			if (icon->IsSelected())
				return;
		}

		mPressedPoint = cursor.position;

		BeginSelecting();
	}

	void AssetsIconsScrollArea::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (cursor.delta == Vec2F())
			return;

		if (mSelecting)
			UpdateSelection(cursor);
	}

	void AssetsIconsScrollArea::BeginSelecting()
	{
		mSelecting = true;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAllAssets();

		o2UI.FocusWidget(this);

		RectF selectionRect(mPressedPoint, mPressedPoint);
		mSelectionSprite->SetRect(selectionRect);

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
		for (auto sel : mSelectedAssets)
		{
			if (auto icon = FindVisibleIcon(sel))
			{
				icon->Hide();
				dragIcon = icon;
			}
		}

		if (dragIcon)
		{
			*mDragIcon = *dragIcon;
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
		for (auto sel : mSelectedAssets)
		{
			if (auto icon = FindVisibleIcon(sel))
				icon->Show();
		}

		mIsDraggingIcons = false;
	}

	void AssetsIconsScrollArea::UpdateDraggingGraphics()
	{
		mDragIcon->SetEditableParent(nullptr);
		mDragIcon->layout->SetRect(RectF(o2Input.GetCursorPos() - mAssetIconSize*0.5f + mDragOffset,
								   o2Input.GetCursorPos() + mAssetIconSize*0.5f + mDragOffset));

		mDragIcon->UpdateSelfTransform();
		mDragIcon->UpdateChildrenTransforms();
	}

	void AssetsIconsScrollArea::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{}

	void AssetsIconsScrollArea::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{}

	void AssetsIconsScrollArea::OnDragExit(ISelectableDragableObjectsGroup* group)
	{}

	void AssetsIconsScrollArea::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		if (dynamic_cast<AssetsIconsScrollArea*>(group))
			OnDroppedFromThis();
		else if (auto sceneTree = dynamic_cast<SceneTree*>(group))
			OnDroppedFromSceneTree(sceneTree);
	}

	void AssetsIconsScrollArea::OnDroppedFromThis()
	{
		AssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
		if (iconUnderCursor && iconUnderCursor->GetAssetInfo().meta->GetAssetType() == &TypeOf(FolderAsset))
		{
			String destPath = iconUnderCursor->GetAssetInfo().path;
			auto assetsInfos = mSelectedAssets.Convert<UID>([](const AssetInfo* x) { return x->meta->ID(); });
			o2Assets.MoveAssets(assetsInfos, destPath, true);

			DeselectAllAssets();
		}

		for (auto sel : mSelectedAssets)
		{
			if (auto icon = FindVisibleIcon(sel))
				icon->Show();
		}
	}

	void AssetsIconsScrollArea::OnDroppedFromSceneTree(SceneTree* sceneTree)
	{
		String destPath = mCurrentPath;

		AssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
		if (iconUnderCursor && iconUnderCursor->GetAssetInfo().meta->GetAssetType() == &TypeOf(FolderAsset))
			destPath = iconUnderCursor->GetAssetInfo().path;

		Vector<String> newAssets;
		for (auto object : sceneTree->GetSelectedObjects())
		{
			if (Actor* actor = dynamic_cast<Actor*>(object))
			{
				ActorAssetRef newAsset = actor->MakePrototype();
				String path = destPath.IsEmpty() ? newAsset->GetActor()->name + String(".proto") : destPath + "/" +
					newAsset->GetActor()->name + String(".proto");

				String uniquePath = o2Assets.MakeUniqueAssetName(path);
				newAsset->Save(uniquePath, false);

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

		auto createAction = mnew CreateAction(mInstantiatedSceneDragObjects, parent, prevActor);

		o2EditorApplication.DoneAction(createAction);
	}

	void AssetsIconsScrollArea::InstantiateDraggingAssets()
	{
		ForcePopEditorScopeOnStack scope;

		for (auto sel : mSelectedAssets)
		{
			Actor* actor = InstantiateAsset(*sel);
			if (actor)
			{
				actor->name = o2FileSystem.GetPathWithoutDirectories(o2FileSystem.GetFileNameWithoutExtension(sel->path));
				mInstantiatedSceneDragObjects.Add(actor);
			}
		}
	}

	void AssetsIconsScrollArea::ClearInstantiatedDraggingAssets()
	{
		for (auto actor : mInstantiatedSceneDragObjects)
			delete actor;

		mInstantiatedSceneDragObjects.Clear();
	}

	void AssetsIconsScrollArea::OnCursorPressBreak(const Input::Cursor& cursor)
	{}

	void AssetsIconsScrollArea::UpdateSelection(const Input::Cursor& cursor)
	{
		RectF selectionRect(cursor.position, mPressedPoint);
		mSelectionSprite->SetRect(selectionRect);

		for (auto info : mCurrentSelectingInfos)
		{
			if (auto icon = FindVisibleIcon(info))
				icon->SetSelected(false);
		}

		mCurrentSelectingInfos.Clear();

		for (auto child : mChildWidgets)
		{
			if (child->layout->GetWorldRect().IsIntersects(selectionRect))
			{
				AssetIcon* icon = dynamic_cast<AssetIcon*>(child);
				icon->SetSelected(true);
				mCurrentSelectingInfos.Add(&icon->GetAssetInfo());
			}
		}
	}

	void AssetsIconsScrollArea::CompleteSelecting()
	{
		mSelectedAssets.Add(mCurrentSelectingInfos);
		mCurrentSelectingInfos.Clear();
		mSelecting = false;

		OnAssetsSelected();
	}

	void AssetsIconsScrollArea::OnCursorMoved(const Input::Cursor& cursor)
	{}

	void AssetsIconsScrollArea::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		AssetIcon* iconUnderCursor = GetIconUnderPoint(cursor.position);
		if (iconUnderCursor)
		{
			if (!mSelectedAssets.Contains(&iconUnderCursor->GetAssetInfo()))
			{
				DeselectAllAssets();
				iconUnderCursor->SetSelected(true);
				mCurrentSelectingInfos.Add(&iconUnderCursor->GetAssetInfo());
			}
		}

		o2UI.FocusWidget(this);
		mContextMenu->Show();
	}

	void AssetsIconsScrollArea::OnScrolled(float scroll)
	{
		ScrollArea::OnScrolled(scroll);
	}

	void AssetsIconsScrollArea::OnKeyReleased(const Input::Key& key)
	{
		if (mIsFocused && key == VK_BACK)
		{
			if (mCurrentPath.CountOf("/") > 0)
				o2EditorAssets.OpenFolder(o2FileSystem.GetParentPath(mCurrentPath));
			else
				o2EditorAssets.OpenFolder("");
		}
	}

	void AssetsIconsScrollArea::InitializeContext()
	{
		mContextMenu = o2UI.CreateWidget<ContextMenu>();

		mContextMenu->AddItem("Open", [&]() { OnContextOpenPressed(); });
		mContextMenu->AddItem("Show in folder", [&]() { OnContextShowInExplorerPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("New folder", [&]() { OnContextCreateAssetPressed(&TypeOf(FolderAsset)); });

		InitializeCreateContext();

		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Copy", [&]() { OnContextCopyPressed(); }, ImageAssetRef(), ShortcutKeys('C', true));
		mContextMenu->AddItem("Cut", [&]() { OnContextCutPressed(); }, ImageAssetRef(), ShortcutKeys('X', true));
		mContextMenu->AddItem("Paste", [&]() { OnContextPastePressed(); }, ImageAssetRef(), ShortcutKeys('V', true));
		mContextMenu->AddItem("Delete", [&]() { OnContextDeletePressed(); }, ImageAssetRef(), ShortcutKeys(VK_DELETE));

		onFocused = [&]() { mContextMenu->SetItemsMaxPriority(); };
		onUnfocused = [&]() {
			mContextMenu->SetItemsMinPriority(); };

		AddInternalWidget(mContextMenu);
	}

	void AssetsIconsScrollArea::InitializeCreateContext()
	{
		auto types = TypeOf(Asset).GetDerivedTypes();

		for (auto type : types)
		{
			if (!type->InvokeStatic<bool>("IsAvailableToCreateFromEditor"))
				continue;

			if (type == &TypeOf(FolderAsset))
				continue;

			mContextMenu->AddItem("Create/" + GetSmartName(type->GetName()),
								  [=]() { OnContextCreateAssetPressed(type); });
		}
	}

	AssetIcon* AssetsIconsScrollArea::GetAssetIconFromPool(const String& style)
	{
		if (!mIconsPool.ContainsKey(style))
			mIconsPool.Add(style, Vector<AssetIcon*>());

		int poolResizeStep = 10;

		if (mIconsPool[style].Count() == 0)
		{
			for (int i = 0; i < poolResizeStep; i++)
			{
				AssetIcon* sample = o2UI.CreateWidget<AssetIcon>(style);
				mIconsPool[style].Add(sample);
			}
		}

		AssetIcon* sample = mIconsPool[style].PopBack();
		return sample;
	}

	void AssetsIconsScrollArea::FreeAssetIconToPool(AssetIcon* icon)
	{
		if (mIconsPool.ContainsKey(icon->name))
			mIconsPool[icon->name].Add(icon);
		else
			delete icon;
	}

	AssetIcon* AssetsIconsScrollArea::FindVisibleIcon(const AssetInfo* info)
	{
		return mVisibleAssetIcons.FindOrDefault([=](AssetIcon* x) { return &x->GetAssetInfo() == info; });
	}

	void AssetsIconsScrollArea::OnAssetDblClick(AssetIcon* icon)
	{
		AssetInfo iconAssetInfo = icon->GetAssetInfo();
		auto assetNameLabel = icon->GetChildWidget("nameLabel");
		if (assetNameLabel && assetNameLabel->IsUnderPoint(o2Input.cursorPos))
		{
			auto name = o2FileSystem.GetFileNameWithoutExtension(o2FileSystem.GetPathWithoutDirectories(iconAssetInfo.path));
			StartAssetRenaming(icon, name, [=](const String& name)
			{
				String ext = o2FileSystem.GetFileExtension(iconAssetInfo.path);
				if (ext.IsEmpty())
					o2Assets.RenameAsset(iconAssetInfo.meta->ID(), name);
				else
					o2Assets.RenameAsset(iconAssetInfo.meta->ID(), name + "." + o2FileSystem.GetFileExtension(iconAssetInfo.path));
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

	AssetIcon* AssetsIconsScrollArea::GetIconUnderPoint(const Vec2F& point) const
	{
		for (auto child : mChildWidgets)
		{
			if (child->layout->IsPointInside(point))
			{
				AssetIcon* icon = dynamic_cast<AssetIcon*>(child);
				return icon;
			}
		}

		return nullptr;
	}

	Sprite* AssetsIconsScrollArea::GetHighlightDrawable() const
	{
		return mHighlightSprite;
	}

	void AssetsIconsScrollArea::SetHighlightAnimation(const AnimationClip& animation)
	{
		mHighlighClip = animation;
		mHighlightAnim.SetClip(animation.CloneAs<AnimationClip>(), true);
		mHighlightAnim.SetTarget(mHighlightSprite);
	}

	void AssetsIconsScrollArea::SetHighlightLayout(const Layout& layout)
	{
		mHighlightLayout = layout;
	}

	Sprite* AssetsIconsScrollArea::GetSelectingDrawable() const
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
			mSelectedAssets.Convert<String>([](const AssetInfo* x) { return x->path; }));
	}

	void AssetsIconsScrollArea::OnContextCutPressed()
	{
		o2EditorAssets.CutAssets(
			mSelectedAssets.Convert<String>([](const AssetInfo* x) { return x->path; }));
	}

	void AssetsIconsScrollArea::OnContextPastePressed()
	{
		o2EditorAssets.PasteAssets(mCurrentPath);
	}


	void AssetsIconsScrollArea::OnContextDeletePressed()
	{
		o2EditorPropertiesWindow.ResetTargets();

		for (auto asset : mSelectedPreloadedAssets)
			delete asset;

		mSelectedPreloadedAssets.Clear();

		o2EditorAssets.DeleteAssets(
			mSelectedAssets.Convert<String>([](const AssetInfo* x) { return x->path; }));
	}

	void AssetsIconsScrollArea::OnContextOpenPressed()
	{
		if (mSelectedAssets.Count() > 0)
			o2EditorAssets.OpenAndEditAsset(mSelectedAssets.Last()->meta->ID());
	}

	void AssetsIconsScrollArea::OnContextShowInExplorerPressed()
	{
		if (mSelectedAssets.Count() > 0)
			o2EditorAssets.OpenAsset(mSelectedAssets.Last()->meta->ID());
	}

	void AssetsIconsScrollArea::OnContextCreateAssetPressed(const Type* assetType)
	{
		String newAssetName = "New " + GetSmartName(assetType->GetName());

		auto objectType = dynamic_cast<const ObjectType*>(assetType);

		mNewAsset = dynamic_cast<Asset*>(objectType->DynamicCastToIObject(objectType->CreateSample()));
		mNewAsset->SetPath(mCurrentPath + "/" + newAssetName);
		mAssetInfos.Add(&mNewAsset->GetInfo());

		SortAssetInfos();
		OnItemsUpdated(true);
		ScrollTo((void*)&mNewAsset->GetInfo());

		AssetIcon* icon = (AssetIcon*)(mChildWidgets.FindOrDefault([=](Widget* x) {
			return &((AssetIcon*)x)->GetAssetInfo() == &mNewAsset->GetInfo(); }));

		if (!icon)
			return;

		StartAssetRenaming(icon, newAssetName,
						   [&](const String& name)
		{
			String path = !mCurrentPath.IsEmpty() ? mCurrentPath + "/" + name : name;
			mNewAsset->Save(path);
			o2EditorAssets.SelectAsset(path);
		});
	}

	void AssetsIconsScrollArea::OnContextCreateFolderPressed()
	{
		// 		AssetIcon* assetIcon = GetAssetIconFromPool("folder");
		// 
		// 		assetIcon->SetState("halfHide", false);
		// 		assetIcon->SetSelectionGroup(this);
		// 		assetIcon->mOwner = this;
		// 
		// 		mGrid->AddChild(assetIcon, 0);
		// 
		// 		StartAssetRenaming(assetIcon, "New folder", [&](const String& name)
		// 		{
		// 			FolderAssetRef folderAsset = FolderAssetRef::CreateAsset();
		// 			folderAsset->Save(o2Assets.MakeUniqueAssetName(mCurrentPath + "/" + name));
		// 
		// 			o2EditorAssets.SelectAsset(mCurrentPath + "/" + name);
		// 		});
	}

	Actor* AssetsIconsScrollArea::InstantiateAsset(const ImageAssetRef& asset)
	{
		auto actor = mnew Actor();
		auto comp = mnew ImageComponent(asset);
		actor->AddComponent(comp);
		comp->FitActorByImage();
		return actor;
	}

	Actor* AssetsIconsScrollArea::InstantiateAsset(const ActorAssetRef& asset)
	{
		return asset->GetActor()->CloneAs<Actor>();
	}

	void AssetsIconsScrollArea::OnAssetsPropertiesChanged()
	{
		for (auto asset : mSelectedPreloadedAssets)
			(*asset)->SetDirty();

		CheckPreloadedAssetsSaving();
	}

	void AssetsIconsScrollArea::CheckPreloadedAssetsSaving()
	{
		if (mChangePropertiesTargetsFromThis)
			return;

		for (auto asset : mSelectedPreloadedAssets)
		{
			(*asset)->Save(false);
			delete asset;
		}

		mSelectedPreloadedAssets.Clear();
	}

	void AssetsIconsScrollArea::Select(SelectableDragableObject* object, bool sendOnSelectionChanged)
	{
		AssetIcon* icon = dynamic_cast<AssetIcon*>(object);
		auto info = &icon->GetAssetInfo();

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
		auto info = &icon->GetAssetInfo();

		if (mSelectedAssets.Contains(info))
		{
			icon->SetSelected(false);
			mSelectedAssets.Remove(info);

			OnAssetsSelected();
		}
	}

	void AssetsIconsScrollArea::AddSelectableObject(SelectableDragableObject* object)
	{}

	void AssetsIconsScrollArea::RemoveSelectableObject(SelectableDragableObject* object)
	{}

	void AssetsIconsScrollArea::OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor)
	{
// 		if ((mPressedPoint - cursor.position).Length() > 5.0f)
// 			return;

		if (!o2Input.IsKeyDown(VK_CONTROL) && !o2Input.IsKeyDown(VK_SHIFT))
			DeselectAllAssets();

		if (o2Input.IsKeyDown(VK_SHIFT) && !mSelectedAssets.IsEmpty())
		{
			auto selectIcon = dynamic_cast<AssetIcon*>(object);
			auto selectInfo = &selectIcon->GetAssetInfo();
			int iconUnderCursorIdx = mAssetInfos.IndexOf(selectInfo);
			int lastSelectedIdx = mAssetInfos.IndexOf(mSelectedAssets.Last());

			int begin = Math::Min(iconUnderCursorIdx, lastSelectedIdx);
			int end = Math::Max(iconUnderCursorIdx, lastSelectedIdx);

			for (int i = begin; i <= end; i++)
			{
				auto info = mAssetInfos[i];
				if (mSelectedAssets.Contains(info))
					continue;

				if (auto icon = FindVisibleIcon(info))
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

		if (!o2Input.IsKeyDown(VK_CONTROL) && !mSelectedAssets.Contains(&dynamic_cast<AssetIcon*>(object)->GetAssetInfo()))
		{
			for (auto info : mSelectedAssets)
			{
				if (auto icon = FindVisibleIcon(info))
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
			return InstantiateAsset(ImageAssetRef(assetInfo.meta->ID()));
		else if (assetInfo.meta->GetAssetType() == &TypeOf(ActorAsset))
			return InstantiateAsset(ActorAssetRef(assetInfo.meta->ID()));

		return nullptr;
	}

	Vector<SelectableDragableObject*> AssetsIconsScrollArea::GetSelectedDragObjects() const
	{
		return mSelectedAssets.Cast<SelectableDragableObject*>();
	}

	Vector<SelectableDragableObject*> AssetsIconsScrollArea::GetAllObjects() const
	{
		return mChildWidgets.Cast<SelectableDragableObject*>();
	}

	void AssetsIconsScrollArea::Select(SelectableDragableObject* object)
	{
		Select(object, true);
	}

	void AssetsIconsScrollArea::StartAssetRenaming(AssetIcon* icon, const String& name,
												   const Function<void(const String&)>& onCompleted)
	{
		icon->SetState("edit", true);

		auto editBox = (EditBox*)icon->GetChild("nameEditBox");
		editBox->text = name;

		editBox->SelectAll();
		editBox->Focus();
		editBox->ResetScroll();

		editBox->onChangeCompleted = [=](const WString& text) {
			icon->SetState("edit", false);
			onCompleted(text);
		};
	}
}

DECLARE_CLASS(Editor::AssetsIconsScrollArea);
