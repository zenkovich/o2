#include "stdafx.h"
#include "AssetsIconsScroll.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/Animation.h"
#include "Application/Application.h"
#include "Assets/AnimationAsset.h"
#include "Assets/Assets.h"
#include "Assets/DataAsset.h"
#include "Assets/FolderAsset.h"
#include "AssetsWindow.h"
#include "Core/Actions/Create.h"
#include "Core/EditorApplication.h"
#include "Core/EditorScope.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "Scene/Components/ImageComponent.h"
#include "TreeWindow/SceneTree.h"
#include "Scene/UI/Widgets/EditBox.h"
#include "Scene/UI/Widgets/GridLayout.h"
#include "Scene/UI/Widgets/Label.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayer.h"
#include "AssetIcon.h"
#include "Utils/FileSystem/FileSystem.h"

namespace Editor
{
	AssetsIconsScrollArea::AssetsIconsScrollArea() :
		ScrollArea()
	{
		PushEditorScopeOnStack scope;

		mGrid = mnew GridLayout();
		*mGrid->layout = WidgetLayout::BothStretch();
		mGrid->baseCorner = BaseCorner::LeftTop;
		mGrid->fitByChildren = true;
		mGrid->cellSize = mAssetIconSize;
		mGrid->border = RectF(5, 5, 5, 5);
		mGrid->spacing = 5;
		mGrid->arrangeAxis = TwoDirection::Horizontal;
		mGrid->arrangeAxisMaxCells = 5;
		AddChild(mGrid);

		mDragIcon = mnew AssetIcon();

		mHightlightSprite = mnew Sprite();
		mSelectionSprite = mnew Sprite();

		mHightlightAnim.SetTarget(mHightlightSprite);
	}

	AssetsIconsScrollArea::AssetsIconsScrollArea(const AssetsIconsScrollArea& other) :
		ScrollArea(other),
		mHightlightSprite(other.mHightlightSprite->CloneAs<Sprite>()), mHightlightLayout(other.mHightlightLayout),
		mHightlightAnim(other.mHightlightAnim), mSelectionSprite(other.mSelectionSprite->CloneAs<Sprite>())
	{
		PushEditorScopeOnStack scope;

		RemoveAllChildren();

		mGrid = mnew GridLayout();
		*mGrid->layout = WidgetLayout::BothStretch();
		mGrid->baseCorner = BaseCorner::LeftTop;
		mGrid->fitByChildren = true;
		mGrid->cellSize = mAssetIconSize;
		mGrid->border = RectF(5, 5, 5, 5);
		mGrid->spacing = 5;
		mGrid->arrangeAxis = TwoDirection::Horizontal;
		mGrid->arrangeAxisMaxCells = 5;
		AddChild(mGrid);

		mDragIcon = o2UI.CreateWidget<AssetIcon>();

		mHightlightAnim.SetTarget(mHightlightSprite);

		RetargetStatesAnimations();
		SetLayoutDirty();
		InitializeContext();
	}

	AssetsIconsScrollArea::~AssetsIconsScrollArea()
	{
		delete mDragIcon;
		delete mHightlightSprite;
		delete mSelectionSprite;

		for (auto& kv : mIconsPool)
			for (auto icon : kv.second)
				delete icon;
	}

	AssetsIconsScrollArea& AssetsIconsScrollArea::operator=(const AssetsIconsScrollArea& other)
	{
		ScrollArea::operator=(other);
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

		if (mHightlightSprite)
			mHightlightSprite->Draw();

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

		if (mHightlightAnim.IsPlaying())
		{
			if (mHightlightIcon)
			{
				mHightlightSprite->SetScale(Vec2F(1.0f, 1.0f));
				mHightlightSprite->SetRect(mHightlightLayout.Calculate(mHightlightIcon->layout->worldRect));
			}

			mHightlightAnim.Update(dt);

			if (!mHightlightAnim.IsPlaying())
				mHightlightIcon = nullptr;
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
		UpdateAssetsGridByCurrentPath();
		ResetScroll();
	}

	String AssetsIconsScrollArea::GetViewingPath() const
	{
		return mCurrentPath;
	}

	void AssetsIconsScrollArea::UpdateAssetsGridByCurrentPath()
	{
		PushEditorScopeOnStack scope;

		Focus();
		DeselectAllAssets();

		auto prevIcons = mGrid->GetChildWidgets().Select<AssetIcon*>([](auto x) { return (AssetIcon*)x; });
		prevIcons.ForEach([&](auto x) { FreeAssetIconToPool(x); });
		mGrid->RemoveAllChildren(false);

		AssetInfosVec folderAssetsInfos;
		if (mCurrentPath != "")
			folderAssetsInfos = FolderAssetRef(mCurrentPath)->GetContainingAssetsInfos();
		else
			folderAssetsInfos = o2Assets.GetAssetsTree().mRootAssets.Select<AssetInfo>([](auto x) { return (AssetInfo)*x; });

		auto getAssetTypeSortWeight = [](const AssetInfo& asset) {

			if (asset.assetType == &TypeOf(FolderAsset))
				return 1;
			else if (asset.assetType == &TypeOf(ImageAsset))
				return 2;
			else if (asset.assetType == &TypeOf(DataAsset))
				return 5;
			else if (asset.assetType == &TypeOf(AnimationAsset))
				return 4;
			else if (asset.assetType == &TypeOf(ActorAsset))
				return 3;

			return 100;
		};

		folderAssetsInfos.Sort([&](const AssetInfo& a, const AssetInfo& b) {
			int wa = getAssetTypeSortWeight(a), wb = getAssetTypeSortWeight(b);
			if (wa == wb)
				return a.path.ToLowerCase() < b.path.ToLowerCase();

			return wa < wb;
		});

		Vector<Widget*> addingIcons;
		for (auto asset : folderAssetsInfos)
		{
			AssetIcon* assetIcon;

			if (asset.assetType == &TypeOf(FolderAsset))
			{
				assetIcon = GetAssetIconFromPool("folder");
				assetIcon->name = "folder";
			}
			else if (asset.assetType == &TypeOf(ImageAsset))
			{
				assetIcon = GetImageAssetIcon(asset);
				assetIcon->name = "image preview";
			}
			else if (asset.assetType == &TypeOf(DataAsset))
			{
				assetIcon = GetAssetIconFromPool("text");
				assetIcon->name = "text";
			}
			else if (asset.assetType == &TypeOf(AnimationAsset))
			{
				assetIcon = GetAssetIconFromPool("animation");
				assetIcon->name = "animation";
			}
			else if (asset.assetType == &TypeOf(ActorAsset))
			{
				assetIcon = GetAssetIconFromPool("prototype");
				assetIcon->name = "prototype";
			}
			else
			{
				assetIcon = GetAssetIconFromPool("standard");
				assetIcon->name = "standard";
			}

			assetIcon->SetAssetInfo(asset);
			assetIcon->SetState("halfHide", mCuttingAssets.ContainsPred([&](auto x) { return x.first == asset.id; }));
			assetIcon->SetSelectionGroup(this);
			assetIcon->mOwner = this;

			addingIcons.Add(assetIcon);
		}

		mGrid->AddChildren(addingIcons.Cast<Actor*>());

		UpdateCuttingAssets();
	}

	bool AssetsIconsScrollArea::IsFocusable() const
	{
		return true;
	}

	void AssetsIconsScrollArea::HightlightAsset(UID id)
	{
		String assetPath = o2Assets.GetAssetPath(id);
		String assetFolder = o2FileSystem.ExtractPathStr(assetPath);

		SetViewingPath(assetFolder);

		AssetIcon* icon = (AssetIcon*)(mGrid->GetChildWidgets().FindMatch([=](Widget* x) {
			return ((AssetIcon*)x)->GetAssetInfo().id == id; }));

		if (!icon)
			return;

		SetScroll(Vec2F(0.0f, -icon->layout->worldTop + layout->worldTop + GetScroll().y));

		mHightlightIcon = icon;
		mHightlightAnim.RewindAndPlay();
	}

	void AssetsIconsScrollArea::SelectAsset(UID id, bool scroll /*= true*/)
	{
		bool selectionChanged = false;

		String assetPath = o2Assets.GetAssetPath(id);
		String assetFolder = o2FileSystem.ExtractPathStr(assetPath);

		SetViewingPath(assetFolder);

		AssetIcon* icon = (AssetIcon*)(mGrid->GetChildWidgets().FindMatch([=](Widget* x) {
			return ((AssetIcon*)x)->GetAssetInfo().id == id; }));

		if (!icon)
			return;

		if (!mSelectedAssetsIcons.Contains(icon))
		{
			icon->SetSelected(true);
			mSelectedAssetsIcons.Add(icon);

			selectionChanged = true;
		}

		if (scroll)
			SetScroll(Vec2F(0.0f, -icon->layout->worldTop + layout->worldTop + GetScroll().y));

		if (selectionChanged)
			OnAssetsSelected();
	}

	void AssetsIconsScrollArea::DeselectAllAssets()
	{
		for (auto icon : mSelectedAssetsIcons)
			icon->SetSelected(false);

		mSelectedAssetsIcons.Clear();

		OnAssetsSelected();
	}

	Vector<AssetInfo> AssetsIconsScrollArea::GetSelectedAssets() const
	{
		return mSelectedAssetsIcons.Select<AssetInfo>([](AssetIcon* x) { return x->GetAssetInfo(); });
	}

	void AssetsIconsScrollArea::OnAssetsSelected()
	{
		auto lastSelectedPreloadedAssets = mSelectedPreloadedAssets;
		for (auto asset : lastSelectedPreloadedAssets)
		{
			if (!mSelectedAssetsIcons.ContainsPred([&](AssetIcon* x) {
				return x->GetAssetInfo().id == (*asset)->GetAssetId(); }))
			{
				mSelectedPreloadedAssets.Remove(asset);
				(*asset)->Save(false);
				delete asset;
			}
		}

		for (auto icon : mSelectedAssetsIcons)
		{
			if (mSelectedPreloadedAssets.ContainsPred([&](const AssetRef* x) { return (*x)->GetAssetId() == icon->GetAssetInfo().id; }))
				continue;

			AssetRef* iconAsset = mnew AssetRef(o2Assets.GetAssetRef(icon->GetAssetInfo().id));
			mSelectedPreloadedAssets.Add(iconAsset);
		}

		onAssetsSelected(mSelectedAssetsIcons.Select<String>([](AssetIcon* x) { return x->GetAssetInfo().path; }));

		if (PropertiesWindow::IsSingletonInitialzed())
		{
			mChangePropertiesTargetsFromThis = true;

			Vector<IObject*> targets;

			if (mSelectedPreloadedAssets.All([](AssetRef* x) { return (*x)->GetType() == TypeOf(ActorAsset); }))
			{
				targets = mSelectedPreloadedAssets.Select<IObject*>([](AssetRef* x) {
					ActorAssetRef asset(*x); return asset->GetActor(); });
			}
			else if (mSelectedPreloadedAssets.All([](AssetRef* x) { return (*x)->GetType() == TypeOf(FolderAsset); }))
				targets.Clear();
			else
				targets = mSelectedPreloadedAssets.Cast<IObject*>();

			if (!targets.IsEmpty())
				o2EditorPropertiesWindow.SetTargets(targets, THIS_FUNC(CheckPreloadedAssetsSaving));

			mChangePropertiesTargetsFromThis = false;
		}
	}

	void AssetsIconsScrollArea::UpdateSelfTransform()
	{
		Vec2F localPressPoint = mPressedPoint - GetChildrenWorldRect().LeftBottom();

		ScrollArea::UpdateSelfTransform();

		mPressedPoint = localPressPoint + GetChildrenWorldRect().LeftBottom();

		if (mSelecting)
			UpdateSelection(*o2Input.GetCursor(0));

		UpdateAssetsGridSize();
	}

	bool AssetsIconsScrollArea::IsScrollable() const
	{
		return ScrollArea::IsScrollable();
	}

	bool AssetsIconsScrollArea::IsInputTransparent() const
	{
		return ScrollArea::IsInputTransparent();
	}

	void AssetsIconsScrollArea::UpdateCuttingAssets()
	{
		for (auto child : mGrid->GetChildWidgets())
		{
			AssetIcon* icon = (AssetIcon*)child;
			icon->SetState("halfHide", o2EditorAssets.mCuttingAssets.ContainsPred([=](auto x) {
				return x.first == icon->GetAssetInfo().id; }));
		}
	}

	void AssetsIconsScrollArea::CopyData(const Actor& otherActor)
	{
		const AssetsIconsScrollArea& other = dynamic_cast<const AssetsIconsScrollArea&>(otherActor);

		delete mDragIcon;
		delete mHightlightSprite;
		delete mSelectionSprite;

		ScrollArea::CopyData(other);

		mDragIcon = other.mDragIcon->CloneAs<AssetIcon>();

		mGrid = FindChildByType<GridLayout>();
		mContextMenu = FindChildByType<ContextMenu>();

		mHightlightLayout = other.mHightlightLayout;
		mHightlightSprite = other.mHightlightSprite->CloneAs<Sprite>();
		mHightlightAnim.SetTarget(mHightlightSprite);

		mSelectionSprite = other.mSelectionSprite->CloneAs<Sprite>();

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void AssetsIconsScrollArea::OnFocused()
	{
		for (auto icon : mGrid->GetChildWidgets())
			icon->SetState("focused", true);

		Widget::OnFocused();
	}

	void AssetsIconsScrollArea::OnUnfocused()
	{
		for (auto icon : mGrid->GetChildWidgets())
			icon->SetState("focused", false);

		Widget::OnUnfocused();
	}

	void AssetsIconsScrollArea::OnCursorPressed(const Input::Cursor& cursor)
	{
		if (GetIconUnderPoint(cursor.position))
			return;

		mPressedPoint = cursor.position;

		BeginSelecting();
		o2UI.FocusWidget(this);
		UpdateSelection(cursor);
	}

	void AssetsIconsScrollArea::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (cursor.delta == Vec2F())
			return;

		float beginDragOrSelectThreshold = 5.0f;

		if (mSelecting)
			UpdateSelection(cursor);
	}

	void AssetsIconsScrollArea::BeginSelecting()
	{
		mSelecting = true;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAllAssets();
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

		for (auto sel : mSelectedAssetsIcons)
			sel->Hide();

		*mDragIcon = *mSelectedAssetsIcons.Last();
		mDragIcon->DragDropArea::SetInteractable(false);
		mDragIcon->CursorAreaEventsListener::SetInteractable(false);
		mDragIcon->SetEnabled(true);
		mDragIcon->ExcludeFromScene();
		mDragOffset = icon->layout->worldCenter - o2Input.GetCursorPos();

		if (mSelectedAssetsIcons.Count() > 1)
			mDragIcon->assetName = (String)mSelectedAssetsIcons.Count() + " items";
	}

	void AssetsIconsScrollArea::EndDragging(bool droppedToThis /*= false*/)
	{
		for (auto sel : mSelectedAssetsIcons)
			sel->Show();

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
		if (iconUnderCursor && iconUnderCursor->GetAssetInfo().assetType == &TypeOf(FolderAsset))
		{
			String destPath = iconUnderCursor->GetAssetInfo().path;
			auto assetsInfos = mSelectedAssetsIcons.Select<AssetInfo>([](auto x) { return x->GetAssetInfo(); });
			o2Assets.MoveAssets(assetsInfos, destPath, true);

			DeselectAllAssets();
		}

		for (auto sel : mSelectedAssetsIcons)
			sel->Show();
	}

	void AssetsIconsScrollArea::OnDroppedFromSceneTree(SceneTree* sceneTree)
	{
		String destPath = mCurrentPath;

		AssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
		if (iconUnderCursor && iconUnderCursor->GetAssetInfo().assetType == &TypeOf(FolderAsset))
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
		auto firstInstObject = mInstantiatedSceneDragObjects[0];
		auto parent = firstInstObject->GetEditableParent();
		auto parentChilds = parent ? parent->GetEditablesChildren() : o2Scene.GetRootEditableObjects();
		int idx = parentChilds.Find(firstInstObject);
		auto prevActor = idx > 0 ? parentChilds[idx - 1] : nullptr;

		auto createAction = mnew CreateAction(mInstantiatedSceneDragObjects, parent, prevActor);

		o2EditorApplication.DoneAction(createAction);
	}

	void AssetsIconsScrollArea::InstantiateDraggingAssets()
	{
		for (auto& sel : mSelectedAssetsIcons)
		{
			auto info = sel->GetAssetInfo();
			Actor* actor = InstantiateAsset(info);
			if (actor)
			{
				actor->name = o2FileSystem.GetPathWithoutDirectories(o2FileSystem.GetFileNameWithoutExtension(info.path));
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

		for (auto icon : mCurrentSelectingIcons)
			icon->SetSelected(false);

		mCurrentSelectingIcons.Clear();

		for (auto child : mGrid->GetChildWidgets())
		{
			if (child->layout->GetWorldRect().IsIntersects(selectionRect))
			{
				AssetIcon* icon = (AssetIcon*)child;
				icon->SetSelected(true);
				mCurrentSelectingIcons.Add(icon);
			}
		}
	}

	void AssetsIconsScrollArea::CompleteSelecting()
	{
		mSelectedAssetsIcons.Add(mCurrentSelectingIcons);
		mCurrentSelectingIcons.Clear();
		mSelecting = false;
	}

	void AssetsIconsScrollArea::OnCursorMoved(const Input::Cursor& cursor)
	{}

	void AssetsIconsScrollArea::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		AssetIcon* iconUnderCursor = GetIconUnderPoint(cursor.position);
		if (iconUnderCursor)
		{
			if (!mSelectedAssetsIcons.Contains(iconUnderCursor))
			{
				DeselectAllAssets();
				iconUnderCursor->SetSelected(true);
				mCurrentSelectingIcons.Add(iconUnderCursor);
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

		mContextMenu->AddItem("Import", [&]() { OnContextImportPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Open", [&]() { OnContextOpenPressed(); });
		mContextMenu->AddItem("Show in folder", [&]() { OnContextShowInExplorerPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("New folder", [&]() { OnContextCreateFolderPressed(); }, ImageAssetRef(), ShortcutKeys('N', true));
		mContextMenu->AddItem("Create/Prefab", [&]() { OnContextCreatePrefabPressed(); });
		mContextMenu->AddItem("Create/Script", [&]() { OnContextCreateScriptPressed(); });
		mContextMenu->AddItem("Create/Animation", [&]() { OnContextCreateAnimationPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Copy", [&]() { OnContextCopyPressed(); }, ImageAssetRef(), ShortcutKeys('C', true));
		mContextMenu->AddItem("Cut", [&]() { OnContextCutPressed(); }, ImageAssetRef(), ShortcutKeys('X', true));
		mContextMenu->AddItem("Paste", [&]() { OnContextPastePressed(); }, ImageAssetRef(), ShortcutKeys('V', true));
		mContextMenu->AddItem("Delete", [&]() { OnContextDeletePressed(); }, ImageAssetRef(), ShortcutKeys(VK_DELETE));

		onFocused = [&]() { mContextMenu->SetItemsMaxPriority(); };
		onUnfocused = [&]() {
			mContextMenu->SetItemsMinPriority(); };

		AddChild(mContextMenu);
	}

	void AssetsIconsScrollArea::PrepareIconsPools()
	{
		int poolSize = 10;
		String iconsStyles[] = { "standard", "folder", "prototype", "prototype preview", "image preview", "text", "animation" };

		for (auto style : iconsStyles)
		{
			mIconsPool.Add(style, Vector<AssetIcon*>());

			for (int i = 0; i < poolSize; i++)
			{
				AssetIcon* sample = o2UI.CreateWidget<AssetIcon>(style);
				mIconsPool[style].Add(sample);
			}
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

	AssetIcon* AssetsIconsScrollArea::GetImageAssetIcon(const AssetInfo& asset)
	{
		AssetIcon* assetIcon = GetAssetIconFromPool("image preview");

		auto previewLayer = assetIcon->layer["preview"];
		Sprite* previewSprite = (Sprite*)previewLayer->GetDrawable();

		ImageAssetRef previewSpriteAsset(asset.path);
		float previewMaxSize = 30;

		if (previewSpriteAsset->width > previewSpriteAsset->height)
		{
			float cf = previewSpriteAsset->height / previewSpriteAsset->width;
			previewLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(previewMaxSize, previewMaxSize*cf),
												 Vec2F(0, 10));
		}
		else
		{
			float cf = previewSpriteAsset->width / previewSpriteAsset->height;
			previewLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(previewMaxSize*cf, previewMaxSize),
												 Vec2F(0, 10));
		}

		previewSprite->image = previewSpriteAsset;
		previewSprite->mode = SpriteMode::Default;

		return assetIcon;
	}

	void AssetsIconsScrollArea::UpdateAssetsGridSize()
	{
		float availableWidth = layout->GetWidth() - mGrid->GetBorderLeft() - mGrid->GetBorderRight();
		int preCalcCellsCount = Math::Max(1, Math::FloorToInt(availableWidth/mAssetIconSize.x));
		availableWidth -= preCalcCellsCount*mGrid->GetSpacing();

		mGrid->arrangeAxisMaxCells = Math::Max(1, Math::FloorToInt(availableWidth/mAssetIconSize.x));
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
					o2Assets.RenameAsset(iconAssetInfo, name);
				else
					o2Assets.RenameAsset(iconAssetInfo, name + "." + o2FileSystem.GetFileExtension(iconAssetInfo.path));
			});
		}
		else
		{
			if (iconAssetInfo.assetType == &TypeOf(FolderAsset))
				o2EditorAssets.OpenFolder(iconAssetInfo.path);
			else
				o2EditorAssets.OpenAndEditAsset(iconAssetInfo.id);
		}
	}

	AssetIcon* AssetsIconsScrollArea::GetIconUnderPoint(const Vec2F& point) const
	{
		for (auto child : mGrid->GetChildWidgets())
		{
			if (child->layout->IsPointInside(point))
			{
				AssetIcon* icon = dynamic_cast<AssetIcon*>(child);
				return icon;
			}
		}

		return nullptr;
	}

	Sprite* AssetsIconsScrollArea::GetHightlightDrawable() const
	{
		return mHightlightSprite;
	}

	void AssetsIconsScrollArea::SetHightlightAnimation(const Animation& animation)
	{
		mHightlightAnim = animation;
		mHightlightAnim.SetTarget(mHightlightSprite);
	}

	void AssetsIconsScrollArea::SetHightlightLayout(const Layout& layout)
	{
		mHightlightLayout = layout;
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
			mSelectedAssetsIcons.Select<String>([](AssetIcon* x) { return x->GetAssetInfo().path; }));
	}

	void AssetsIconsScrollArea::OnContextCutPressed()
	{
		o2EditorAssets.CutAssets(
			mSelectedAssetsIcons.Select<String>([](AssetIcon* x) { return x->GetAssetInfo().path; }));
	}

	void AssetsIconsScrollArea::OnContextPastePressed()
	{
		o2EditorAssets.PasteAssets(mCurrentPath);
	}


	void AssetsIconsScrollArea::OnContextDeletePressed()
	{
		for (auto asset : mSelectedPreloadedAssets)
			delete asset;

		mSelectedPreloadedAssets.Clear();

		o2EditorAssets.DeleteAssets(
			mSelectedAssetsIcons.Select<String>([](AssetIcon* x) { return x->GetAssetInfo().path; }));
	}

	void AssetsIconsScrollArea::OnContextOpenPressed()
	{
		if (mSelectedAssetsIcons.Count() > 0)
			o2EditorAssets.OpenAndEditAsset(mSelectedAssetsIcons.Last()->GetAssetInfo().id);
	}

	void AssetsIconsScrollArea::OnContextShowInExplorerPressed()
	{
		if (mSelectedAssetsIcons.Count() > 0)
			o2EditorAssets.OpenAsset(mSelectedAssetsIcons.Last()->GetAssetInfo().id);
	}

	void AssetsIconsScrollArea::OnContextImportPressed()
	{
		o2EditorAssets.ImportAssets(mCurrentPath);
	}

	void AssetsIconsScrollArea::OnContextCreateFolderPressed()
	{
		AssetIcon* assetIcon = GetAssetIconFromPool("folder");

		assetIcon->SetState("halfHide", false);
		assetIcon->SetSelectionGroup(this);
		assetIcon->mOwner = this;

		mGrid->AddChild(assetIcon, 0);

		StartAssetRenaming(assetIcon, "New folder", [&](const String& name)
		{
			o2EditorAssets.CreateFolderAsset(mCurrentPath, name);
			o2EditorAssets.SelectAsset(mCurrentPath + "/" + name);
		});
	}

	void AssetsIconsScrollArea::OnContextCreatePrefabPressed()
	{
		o2EditorAssets.CreatePrefabAsset(mCurrentPath);
	}

	void AssetsIconsScrollArea::OnContextCreateScriptPressed()
	{
		o2EditorAssets.CreateScriptAsset(mCurrentPath);
	}

	void AssetsIconsScrollArea::OnContextCreateAnimationPressed()
	{
		o2EditorAssets.CreateAnimationAsset(mCurrentPath);
	}

	Actor* AssetsIconsScrollArea::InstantiateAsset(const ImageAssetRef& asset)
	{
		return mnew Actor({ mnew ImageComponent(asset) });
	}

	Actor* AssetsIconsScrollArea::InstantiateAsset(const ActorAssetRef& asset)
	{
		return mnew Actor(*(asset->GetActor()));
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

		if (!mSelectedAssetsIcons.Contains(icon))
		{
			icon->SetSelected(true);
			mSelectedAssetsIcons.Add(icon);

			if (sendOnSelectionChanged)
				OnAssetsSelected();
		}
	}

	void AssetsIconsScrollArea::Deselect(SelectableDragableObject* object)
	{
		AssetIcon* icon = dynamic_cast<AssetIcon*>(object);

		if (mSelectedAssetsIcons.Contains(icon))
		{
			icon->SetSelected(false);
			mSelectedAssetsIcons.Remove(icon);

			OnAssetsSelected();
		}
	}

	void AssetsIconsScrollArea::AddSelectableObject(SelectableDragableObject* object)
	{}

	void AssetsIconsScrollArea::RemoveSelectableObject(SelectableDragableObject* object)
	{}

	void AssetsIconsScrollArea::OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor)
	{
		if (!o2Input.IsKeyDown(VK_CONTROL) && !o2Input.IsKeyDown(VK_SHIFT))
			DeselectAllAssets();

		if (o2Input.IsKeyDown(VK_SHIFT) && !mSelectedAssetsIcons.IsEmpty())
		{
			auto selectIcon = dynamic_cast<AssetIcon*>(object);
			int iconUnderCursorIdx = mGrid->GetChildWidgets().Find(dynamic_cast<Widget*>(selectIcon));
			int lastSelectedIdx = mGrid->GetChildWidgets().Find(dynamic_cast<Widget*>(mSelectedAssetsIcons.Last()));

			int begin = Math::Min(iconUnderCursorIdx, lastSelectedIdx);
			int end = Math::Max(iconUnderCursorIdx, lastSelectedIdx);

			for (int i = begin; i <= end; i++)
				Select(dynamic_cast<AssetIcon*>(mGrid->GetChildWidgets()[i]), i == end);
		}

		Select(object);
	}

	void AssetsIconsScrollArea::OnSelectableObjectBeganDragging(SelectableDragableObject* object)
	{
		if (!o2Input.IsKeyDown(VK_CONTROL) && !mSelectedAssetsIcons.Contains(dynamic_cast<AssetIcon*>(object)))
		{
			for (auto icon : mSelectedAssetsIcons)
				icon->SetSelected(false);

			mSelectedAssetsIcons.Clear();
		}

		if (!object->IsSelected())
			Select(object, false);
	}

	Actor* AssetsIconsScrollArea::InstantiateAsset(const AssetInfo& assetInfo)
	{
		if (assetInfo.assetType == &TypeOf(ImageAsset))
			return InstantiateAsset(ImageAssetRef(assetInfo.id));
		else if (assetInfo.assetType == &TypeOf(ActorAsset))
			return InstantiateAsset(ActorAssetRef(assetInfo.id));

		return nullptr;
	}

	ISelectableDragableObjectsGroup::SelectDragObjectsVec AssetsIconsScrollArea::GetSelectedDragObjects() const
	{
		return mSelectedAssetsIcons.Cast<SelectableDragableObject*>();
	}

	ISelectableDragableObjectsGroup::SelectDragObjectsVec AssetsIconsScrollArea::GetAllObjects() const
	{
		return mGrid->GetChildWidgets().Cast<SelectableDragableObject*>();
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
