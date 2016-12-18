#include "AssetsIconsScroll.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/Animation.h"
#include "Application/Application.h"
#include "Assets/ActorAsset.h"
#include "Assets/AnimationAsset.h"
#include "Assets/Assets.h"
#include "Assets/DataAsset.h"
#include "Assets/FolderAsset.h"
#include "Assets/ImageAsset.h"
#include "AssetsWindow.h"
#include "Core/Actions/CreateActors.h"
#include "Core/EditorApplication.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Scene/Actor.h"
#include "Scene/Components/ImageComponent.h"
#include "UI/EditBox.h"
#include "UI/GridLayout.h"
#include "UI/Label.h"
#include "UI/UIManager.h"
#include "UIAssetIcon.h"
#include "Utils/FileSystem/FileSystem.h"
#include "TreeWindow/ActorsTree.h"

namespace Editor
{

	UIAssetsIconsScrollArea::UIAssetsIconsScrollArea():
		UIScrollArea()
	{
		mGrid = mnew UIGridLayout();
		mGrid->layout = UIWidgetLayout::BothStretch();
		mGrid->baseCorner = BaseCorner::LeftTop;
		mGrid->fitByChildren = true;
		mGrid->cellSize = mAssetIconSize;
		mGrid->border = RectF(5, 5, 5, 5);
		mGrid->spacing = 5;
		mGrid->arrangeAxis = TwoDirection::Horizontal;
		mGrid->arrangeAxisMaxCells = 5;
		onLayoutChanged += Function<void()>(this, &UIAssetsIconsScrollArea::UpdateAssetsGridSize);
		AddChild(mGrid);

		mDragIcon = mnew UIAssetIcon();

		mHightlightSprite = mnew Sprite();
		mSelectionSprite  = mnew Sprite();

		mHightlightAnim.SetTarget(mHightlightSprite);
	}

	UIAssetsIconsScrollArea::UIAssetsIconsScrollArea(const UIAssetsIconsScrollArea& other):
		UIScrollArea(other),
		mHightlightSprite(other.mHightlightSprite->Clone()), mHightlightLayout(other.mHightlightLayout),
		mHightlightAnim(other.mHightlightAnim), mSelectionSprite(other.mSelectionSprite->Clone())
	{
		RemoveAllChilds();

		mGrid = mnew UIGridLayout();
		mGrid->layout = UIWidgetLayout::BothStretch();
		mGrid->baseCorner = BaseCorner::LeftTop;
		mGrid->fitByChildren = true;
		mGrid->cellSize = mAssetIconSize;
		mGrid->border = RectF(5, 5, 5, 5);
		mGrid->spacing = 5;
		mGrid->arrangeAxis = TwoDirection::Horizontal;
		mGrid->arrangeAxisMaxCells = 5;
		onLayoutChanged += Function<void()>(this, &UIAssetsIconsScrollArea::UpdateAssetsGridSize);
		AddChild(mGrid);

		onLayoutChanged += Function<void()>(this, &UIAssetsIconsScrollArea::UpdateAssetsGridSize);

		mDragIcon = o2UI.CreateWidget<UIAssetIcon>();

		mHightlightAnim.SetTarget(mHightlightSprite);

		RetargetStatesAnimations();
		UpdateLayout();
		InitializeContext();
	}

	UIAssetsIconsScrollArea::~UIAssetsIconsScrollArea()
	{
		delete mDragIcon;
		delete mHightlightSprite;
		delete mSelectionSprite;

		for (auto kv : mIconsPool)
			for (auto icon : kv.Value())
				delete icon;
	}

	UIAssetsIconsScrollArea& UIAssetsIconsScrollArea::operator=(const UIAssetsIconsScrollArea& other)
	{
		delete mDragIcon;
		delete mHightlightSprite;
		delete mSelectionSprite;

		UIScrollArea::operator=(other);

		mDragIcon = other.mDragIcon->Clone();

		mGrid = FindChild<UIGridLayout>();
		mContextMenu = FindChild<UIContextMenu>();

		mHightlightLayout = other.mHightlightLayout;
		mHightlightSprite = other.mHightlightSprite->Clone();
		mHightlightAnim.SetTarget(mHightlightSprite);

		mSelectionSprite = other.mSelectionSprite->Clone();

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIAssetsIconsScrollArea::Draw()
	{
		if (mFullyDisabled)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		for (auto child : mChilds)
			child->Draw();

		if (mSelecting)
			mSelectionSprite->Draw();

		o2Render.DisableScissorTest();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (mOwnHorScrollBar)
			mHorScrollBar->Draw();

		if (mOwnVerScrollBar)
			mVerScrollBar->Draw();

		if (mIsDraggingIcons && mInstSceneDragActors.Count() == 0)
			o2UI.DrawWidgetAtTop(mDragIcon);

		if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
			DrawDebugFrame();
	}

	void UIAssetsIconsScrollArea::Update(float dt)
	{
		UIScrollArea::Update(dt);

		if (mHightlightAnim.IsPlaying())
		{
			if (mHightlightIcon)
			{
				mHightlightSprite->SetScale(Vec2F(1.0f, 1.0f));
				mHightlightSprite->SetRect(mHightlightLayout.Calculate(mHightlightIcon->layout.absRect));
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

	void UIAssetsIconsScrollArea::SetViewingPath(const String& path)
	{
		if (path == mCurrentPath)
			return;

		mCurrentPath = path;
		UpdateAssetsPath();
		ResetScroll();
	}

	String UIAssetsIconsScrollArea::GetViewingPath() const
	{
		return mCurrentPath;
	}

	void UIAssetsIconsScrollArea::UpdateAssetsPath()
	{
		o2Debug.LogWarning("Update assets folder: %s", mCurrentPath);

		DeselectAllAssets();

		auto prevIcons = mGrid->GetChilds().Select<UIAssetIcon*>([](auto x) { return (UIAssetIcon*)x; });
		prevIcons.ForEach([&](auto x) { FreeAssetIconToPool(x); });
		mGrid->RemoveAllChilds(false);

		AssetInfosVec folderAssetsInfos;
		if (mCurrentPath != "")
			folderAssetsInfos = FolderAsset(mCurrentPath).GetContainingAssetsInfos();
		else
			folderAssetsInfos = o2Assets.GetAssetsTree().mRootAssets.Select<AssetInfo>([](auto x) { return (AssetInfo)*x; });

		auto getAssetTypeSortWeight = [](const AssetInfo& asset) {

			if (asset.mType == TypeOf(FolderAsset).ID())
				return 1;
			else if (asset.mType == TypeOf(ImageAsset).ID())
				return 2;
			else if (asset.mType == TypeOf(DataAsset).ID())
				return 5;
			else if (asset.mType == TypeOf(AnimationAsset).ID())
				return 4;
			else if (asset.mType == TypeOf(ActorAsset).ID())
				return 3;

			return 100;
		};

		folderAssetsInfos.Sort([&](const AssetInfo& a, const AssetInfo& b) {
			int wa = getAssetTypeSortWeight(a), wb = getAssetTypeSortWeight(b);
			if (wa == wb)
				return a.mPath > b.mPath;

			return wa < wb;
		});

		Vector<UIWidget*> addingIcons;
		for (auto asset : folderAssetsInfos)
		{
			UIAssetIcon* assetIcon;

			if (asset.mType == TypeOf(FolderAsset).ID())
			{
				assetIcon = GetAssetIconFromPool("folder");
				assetIcon->name = "folder";
			}
			else if (asset.mType == TypeOf(ImageAsset).ID())
			{
				assetIcon = GetImageAssetIcon(asset);
				assetIcon->name = "image preview";
			}
			else if (asset.mType == TypeOf(DataAsset).ID())
			{
				assetIcon = GetAssetIconFromPool("text");
				assetIcon->name = "text";
			}
			else if (asset.mType == TypeOf(AnimationAsset).ID())
			{
				assetIcon = GetAssetIconFromPool("animation");
				assetIcon->name = "animation";
			}
			else if (asset.mType == TypeOf(ActorAsset).ID())
			{
				assetIcon = GetAssetIconFromPool("prefab");
				assetIcon->name = "prefab";
			}
			else
			{
				assetIcon = GetAssetIconFromPool("standard");
				assetIcon->name = "standard";
			}

			assetIcon->SetAssetInfo(asset);
			assetIcon->SetState("halfHide", mCuttingAssets.ContainsPred([&](auto x) { return x.mFirst == asset.mId; }));
			assetIcon->SetSelectionGroup(this);
			assetIcon->mOwner = this;

			addingIcons.Add(assetIcon);
		}

		mGrid->AddChilds(addingIcons);

		UpdateCuttingAssets();
	}

	bool UIAssetsIconsScrollArea::IsFocusable() const
	{
		return true;
	}

	void UIAssetsIconsScrollArea::HightlightAsset(UID id)
	{
		String assetPath = o2Assets.GetAssetPath(id);
		String assetFolder = o2FileSystem.ExtractPathStr(assetPath);

		SetViewingPath(assetFolder);

		UIAssetIcon* icon = (UIAssetIcon*)(mGrid->GetChilds().FindMatch([=](UIWidget* x) {
			return ((UIAssetIcon*)x)->GetAssetInfo().mId == id; }));

		if (!icon)
			return;

		SetScroll(Vec2F(0.0f, -icon->layout.absTop + layout.absTop + GetScroll().y));

		mHightlightIcon = icon;
		mHightlightAnim.RewindAndPlay();
	}

	void UIAssetsIconsScrollArea::SelectAsset(UID id, bool scroll /*= true*/)
	{
		bool selectionChanged = false;

		String assetPath = o2Assets.GetAssetPath(id);
		String assetFolder = o2FileSystem.ExtractPathStr(assetPath);

		SetViewingPath(assetFolder);

		UIAssetIcon* icon = (UIAssetIcon*)(mGrid->GetChilds().FindMatch([=](UIWidget* x) {
			return ((UIAssetIcon*)x)->GetAssetInfo().mId == id; }));

		if (!icon)
			return;

		if (!mSelectedAssetsIcons.Contains(icon))
		{
			icon->SetSelected(true);
			mSelectedAssetsIcons.Add(icon);

			selectionChanged = true;
		}

		if (scroll)
			SetScroll(Vec2F(0.0f, -icon->layout.absTop + layout.absTop + GetScroll().y));

		if (selectionChanged)
			onAssetsSelected(mSelectedAssetsIcons.Select<String>([](UIAssetIcon* x) { return x->GetAssetInfo().mPath; }));
	}

	void UIAssetsIconsScrollArea::DeselectAllAssets()
	{
		for (auto icon : mSelectedAssetsIcons)
			icon->SetSelected(false);

		mSelectedAssetsIcons.Clear();

		onAssetsSelected({});
	}

	Vector<AssetInfo> UIAssetsIconsScrollArea::GetSelectedAssets() const
	{
		return mSelectedAssetsIcons.Select<AssetInfo>([](UIAssetIcon* x) { return x->GetAssetInfo(); });
	}

	void UIAssetsIconsScrollArea::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		Vec2F localPressPoint = mPressedPoint - mChildsAbsRect.LeftBottom();

		UIScrollArea::UpdateLayout(forcible);

		mPressedPoint = localPressPoint + mChildsAbsRect.LeftBottom();

		if (mSelecting)
			UpdateSelection(*o2Input.GetCursor(0));
	}

	void UIAssetsIconsScrollArea::UpdateCuttingAssets()
	{
		for (auto child : mGrid->GetChilds())
		{
			UIAssetIcon* icon = (UIAssetIcon*)child;
			icon->SetState("halfHide", o2EditorAssets.mCuttingAssets.ContainsPred([=](auto x) {
				return x.mFirst == icon->GetAssetInfo().mId; }));
		}
	}

	void UIAssetsIconsScrollArea::OnFocused()
	{
		for (auto icon : mGrid->GetChilds())
			icon->SetState("focused", true);

		UIWidget::OnFocused();
	}

	void UIAssetsIconsScrollArea::OnUnfocused()
	{
		for (auto icon : mGrid->GetChilds())
			icon->SetState("focused", false);

		UIWidget::OnFocused();
	}

	void UIAssetsIconsScrollArea::OnCursorPressed(const Input::Cursor& cursor)
	{
		mPressedPoint = cursor.position;
		BeginSelecting();
		o2UI.FocusWidget(this);
		UpdateSelection(cursor);
	}

	void UIAssetsIconsScrollArea::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (cursor.delta == Vec2F())
			return;

		float beginDragOrSelectThreshold = 5.0f;

		if (mSelecting)
			UpdateSelection(cursor);
	}

	void UIAssetsIconsScrollArea::BeginSelecting()
	{
		mSelecting = true;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAllAssets();
	}

	void UIAssetsIconsScrollArea::OnCursorReleased(const Input::Cursor& cursor)
	{
		if (mSelecting)
			CompleteSelecting();
		else
		{
			if (!o2Input.IsKeyDown(VK_CONTROL))
				DeselectAllAssets();

			UIAssetIcon* iconUnderCursor = GetIconUnderPoint(cursor.position);
			if (iconUnderCursor && !mSelectedAssetsIcons.ContainsPred([=](auto x) { return x == iconUnderCursor; }))
			{
				iconUnderCursor->SetState("assetSelection", true);
				iconUnderCursor->SetSelected(true);
				mSelectedAssetsIcons.Add(iconUnderCursor);

				onAssetsSelected(mSelectedAssetsIcons.Select<String>([](UIAssetIcon* x) { return x->GetAssetInfo().mPath; }));
			}
		}
	}

	void UIAssetsIconsScrollArea::BeginDragging(UIAssetIcon* icon)
	{
		mIsDraggingIcons = true;
		mDragEnded = false;

		for (auto sel : mSelectedAssetsIcons)
			sel->Hide();

		*mDragIcon = *mSelectedAssetsIcons.Last();
		mDragIcon->DragDropArea::SetInteractable(false);
		mDragIcon->CursorAreaEventsListener::SetInteractable(false);
		mDragOffset = icon->layout.absRect->Center() - o2Input.GetCursorPos();

		if (mSelectedAssetsIcons.Count() > 1)
			mDragIcon->FindChild<UILabel>()->text = (String)mSelectedAssetsIcons.Count() + " items";
	}

	void UIAssetsIconsScrollArea::EndDragging(bool droppedToThis /*= false*/)
	{
		for (auto sel : mSelectedAssetsIcons)
			sel->Show();

		mIsDraggingIcons = false;
	}

	void UIAssetsIconsScrollArea::UpdateDraggingGraphics()
	{
		mDragIcon->SetParent(nullptr);
		mDragIcon->layout.SetRect(RectF(o2Input.GetCursorPos() - mAssetIconSize*0.5f + mDragOffset,
								  o2Input.GetCursorPos() + mAssetIconSize*0.5f + mDragOffset));
	}

	void UIAssetsIconsScrollArea::OnDragEnter(ISelectableDragableObjectsGroup* group)
	{}

	void UIAssetsIconsScrollArea::OnDraggedAbove(ISelectableDragableObjectsGroup* group)
	{}

	void UIAssetsIconsScrollArea::OnDragExit(ISelectableDragableObjectsGroup* group)
	{}

	void UIAssetsIconsScrollArea::OnDropped(ISelectableDragableObjectsGroup* group)
	{
		if (dynamic_cast<UIAssetsIconsScrollArea*>(group))
			OnDroppedFromThis();
		else if (auto actorsTree = dynamic_cast<UIActorsTree*>(group))
			OnDroppedFromActorsTree(actorsTree);
	}

	void UIAssetsIconsScrollArea::OnDroppedFromThis()
	{
		UIAssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
		if (iconUnderCursor && iconUnderCursor->GetAssetInfo().mType == TypeOf(FolderAsset).ID())
		{
			String destPath = iconUnderCursor->GetAssetInfo().mPath;
			auto assetsInfos = mSelectedAssetsIcons.Select<AssetInfo>([](auto x) { return x->GetAssetInfo(); });
			o2Assets.MoveAssets(assetsInfos, destPath, true);

			DeselectAllAssets();
		}

		for (auto sel : mSelectedAssetsIcons)
			sel->Show();
	}

	void UIAssetsIconsScrollArea::OnDroppedFromActorsTree(UIActorsTree* actorsTree)
	{
		String destPath = mCurrentPath;

		UIAssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
		if (iconUnderCursor && iconUnderCursor->GetAssetInfo().mType == TypeOf(FolderAsset).ID())
			destPath = iconUnderCursor->GetAssetInfo().mPath;

		Vector<String> newAssets;
		for (auto actor : actorsTree->GetSelectedActors())
		{
			ActorAsset newAsset;
			newAsset.actor = *actor;
			String path = destPath.IsEmpty() ? newAsset.actor.name + ".prefab" : destPath + "/" + newAsset.actor.name + ".prefab";
			String uniquePath = o2Assets.MakeUniqueAssetName(path);
			newAsset.Save(uniquePath, false);

			newAssets.Add(uniquePath);
		}

		o2Assets.RebuildAssets();

		o2EditorAssets.OpenFolder(destPath);
		o2EditorAssets.SelectAssets(newAssets);
	}

	void UIAssetsIconsScrollArea::RegActorsCreationAction()
	{
		auto firstInstActor = mInstSceneDragActors[0];
		auto parent = firstInstActor->GetParent();
		auto parentChilds = parent ? parent->GetChilds() : o2Scene.GetRootActors();
		int idx = parentChilds.Find(firstInstActor);
		auto prevActor = idx > 0 ? parentChilds[idx - 1] : nullptr;

		auto createAction = mnew CreateActorsAction(mInstSceneDragActors, parent, prevActor);

		o2EditorApplication.DoneAction(createAction);
	}

	void UIAssetsIconsScrollArea::InstantiateDraggingAssets()
	{
		for (auto& sel : mSelectedAssetsIcons)
		{
			auto info = sel->GetAssetInfo();
			Actor* actor = InstantiateAsset(info);
			if (actor)
			{
				actor->name = o2FileSystem.GetPathWithoutDirectories(o2FileSystem.GetFileNameWithoutExtension(info.mPath));
				mInstSceneDragActors.Add(actor);
			}
		}
	}

	void UIAssetsIconsScrollArea::ClearInstantiatedDraggingAssets()
	{
		for (auto actor : mInstSceneDragActors)
			delete actor;

		mInstSceneDragActors.Clear();
	}

	void UIAssetsIconsScrollArea::OnCursorPressBreak(const Input::Cursor& cursor)
	{}

	void UIAssetsIconsScrollArea::UpdateSelection(const Input::Cursor& cursor)
	{
		RectF selectionRect(cursor.position, mPressedPoint);
		mSelectionSprite->SetRect(selectionRect);

		for (auto icon : mCurrentSelectingIcons)
			icon->SetSelected(false);

		mCurrentSelectingIcons.Clear();

		for (auto child : mGrid->GetChilds())
		{
			if (child->layout.GetAbsoluteRect().IsIntersects(selectionRect))
			{
				UIAssetIcon* icon = (UIAssetIcon*)child;
				icon->SetSelected(true);
				mCurrentSelectingIcons.Add(icon);
			}
		}
	}

	void UIAssetsIconsScrollArea::CompleteSelecting()
	{
		mSelectedAssetsIcons.Add(mCurrentSelectingIcons);
		mCurrentSelectingIcons.Clear();
		mSelecting = false;
	}

	void UIAssetsIconsScrollArea::OnCursorMoved(const Input::Cursor& cursor)
	{}

	void UIAssetsIconsScrollArea::OnCursorRightMouseReleased(const Input::Cursor& cursor)
	{
		UIAssetIcon* iconUnderCursor = GetIconUnderPoint(cursor.position);
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

	void UIAssetsIconsScrollArea::OnKeyReleased(const Input::Key& key)
	{
		if (mIsFocused && key == VK_BACK)
		{
			if (mCurrentPath.CountOf("/") > 0)
				o2EditorAssets.OpenFolder(o2FileSystem.GetParentPath(mCurrentPath));
			else
				o2EditorAssets.OpenFolder("");
		}
	}

	void UIAssetsIconsScrollArea::InitializeContext()
	{
		mContextMenu = o2UI.CreateWidget<UIContextMenu>();

		mContextMenu->AddItem("Import", [&]() { OnContextImportPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Open", [&]() { OnContextOpenPressed(); });
		mContextMenu->AddItem("Show in folder", [&]() { OnContextShowInExplorerPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Create/Folder", [&]() { OnContextCreateFolderPressed(); });
		mContextMenu->AddItem("Create/Prefab", [&]() { OnContextCreatePrefabPressed(); });
		mContextMenu->AddItem("Create/Script", [&]() { OnContextCreateScriptPressed(); });
		mContextMenu->AddItem("Create/Animation", [&]() { OnContextCreateAnimationPressed(); });
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Copy", [&]() { OnContextCopyPressed(); }, nullptr, ShortcutKeys('C', true));
		mContextMenu->AddItem("Cut", [&]() { OnContextCutPressed(); }, nullptr, ShortcutKeys('X', true));
		mContextMenu->AddItem("Paste", [&]() { OnContextPastePressed(); }, nullptr, ShortcutKeys('V', true));
		mContextMenu->AddItem("Delete", [&]() { OnContextDeletePressed(); }, nullptr, ShortcutKeys(VK_DELETE));

		AddChild(mContextMenu);
	}

	void UIAssetsIconsScrollArea::PrepareIconsPools()
	{
		int poolSize = 10;
		String iconsStyles[] ={ "standard", "folder", "prefab", "prefab preview", "image preview", "text", "animation" };

		for (auto style : iconsStyles)
		{
			mIconsPool.Add(style, Vector<UIAssetIcon*>());

			for (int i = 0; i < poolSize; i++)
			{
				UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>(style);
				mIconsPool[style].Add(sample);
			}
		}
	}

	UIAssetIcon* UIAssetsIconsScrollArea::GetAssetIconFromPool(const String& style)
	{
		if (!mIconsPool.ContainsKey(style))
			mIconsPool.Add(style, Vector<UIAssetIcon*>());

		int poolResizeStep = 10;

		if (mIconsPool[style].Count() == 0)
		{
			for (int i = 0; i < poolResizeStep; i++)
			{
				UIAssetIcon* sample = o2UI.CreateWidget<UIAssetIcon>(style);
				mIconsPool[style].Add(sample);
			}
		}

		UIAssetIcon* sample = mIconsPool[style].PopBack();
		return sample;
	}

	void UIAssetsIconsScrollArea::FreeAssetIconToPool(UIAssetIcon* icon)
	{
		if (mIconsPool.ContainsKey(icon->name))
			mIconsPool[icon->name].Add(icon);
		else
			delete icon;
	}

	UIAssetIcon* UIAssetsIconsScrollArea::GetImageAssetIcon(const AssetInfo& asset)
	{
		UIAssetIcon* assetIcon = GetAssetIconFromPool("image preview");

		auto previewLayer = assetIcon->layer["preview"];
		Sprite* previewSprite = (Sprite*)previewLayer->drawable;

		ImageAsset previewSpriteAsset(asset.mPath);
		float previewMaxSize = 30;

		if (previewSpriteAsset.width > previewSpriteAsset.height)
		{
			float cf = previewSpriteAsset.height / previewSpriteAsset.width;
			previewLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(previewMaxSize, previewMaxSize*cf),
												 Vec2F(0, 10));
		}
		else
		{
			float cf = previewSpriteAsset.width / previewSpriteAsset.height;
			previewLayer->layout = Layout::Based(BaseCorner::Center, Vec2F(previewMaxSize*cf, previewMaxSize),
												 Vec2F(0, 10));
		}

		previewSprite->imageAsset = previewSpriteAsset;
		previewSprite->mode = SpriteMode::Default;

		return assetIcon;
	}

	void UIAssetsIconsScrollArea::UpdateAssetsGridSize()
	{
		float availableWidth = layout.GetWidth() - mGrid->GetBorderLeft() - mGrid->GetBorderRight();
		int preCalcCellsCount = Math::Max(1, Math::FloorToInt(availableWidth/mAssetIconSize.x));
		availableWidth -= preCalcCellsCount*mGrid->GetSpacing();

		mGrid->arrangeAxisMaxCells = Math::Max(1, Math::FloorToInt(availableWidth/mAssetIconSize.x));
	}

	void UIAssetsIconsScrollArea::OnIconDblClicked(UIAssetIcon* icon)
	{
		AssetInfo iconAssetInfo = icon->GetAssetInfo();
		auto assetNameLabel = icon->GetChild("nameLabel");
		if (assetNameLabel && assetNameLabel->IsUnderPoint(o2Input.cursorPos))
		{
			icon->SetState("edit", true);

			auto editBox = (UIEditBox*)icon->GetChild("nameEditBox");

			editBox->text = o2FileSystem.GetFileNameWithoutExtension(
				o2FileSystem.GetPathWithoutDirectories(iconAssetInfo.mPath));

			editBox->SelectAll();
			editBox->UIWidget::Focus();
			editBox->ResetScroll();

			editBox->onChangeCompleted = [=](const WString& text) {
				icon->SetState("edit", false);
				String ext = o2FileSystem.GetFileExtension(iconAssetInfo.mPath);
				if (ext.IsEmpty())
					o2Assets.RenameAsset(iconAssetInfo, text, false);
				else
					o2Assets.RenameAsset(iconAssetInfo, text + "." + o2FileSystem.GetFileExtension(iconAssetInfo.mPath), false);
				mNeedRebuildAssets = true;
			};

			return;
		}

		if (iconAssetInfo.mType == TypeOf(FolderAsset).ID())
			o2EditorAssets.OpenFolder(iconAssetInfo.mPath);
		else
			o2EditorAssets.OpenAndEditAsset(iconAssetInfo.mId);
	}

	UIAssetIcon* UIAssetsIconsScrollArea::GetIconUnderPoint(const Vec2F& point) const
	{
		for (auto child : mGrid->GetChilds())
		{
			if (child->layout.IsUnderPoint(point))
			{
				UIAssetIcon* icon = (UIAssetIcon*)child;
				return icon;
			}
		}

		return nullptr;
	}

	Sprite* UIAssetsIconsScrollArea::GetHightlightDrawable() const
	{
		return mHightlightSprite;
	}

	void UIAssetsIconsScrollArea::SetHightlightAnimation(const Animation& animation)
	{
		mHightlightAnim = animation;
		mHightlightAnim.SetTarget(mHightlightSprite);
	}

	void UIAssetsIconsScrollArea::SetHightlightLayout(const Layout& layout)
	{
		mHightlightLayout = layout;
	}

	Sprite* UIAssetsIconsScrollArea::GetSelectingDrawable() const
	{
		return mSelectionSprite;
	}

	bool UIAssetsIconsScrollArea::IsUnderPoint(const Vec2F& point)
	{
		return UIWidget::IsUnderPoint(point);
	}

#undef CopyFile

	void UIAssetsIconsScrollArea::OnContextCopyPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CopyAssets(
			mSelectedAssetsIcons.Select<String>([](UIAssetIcon* x) { return x->GetAssetInfo().mPath; }));
	}

	void UIAssetsIconsScrollArea::OnContextCutPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CutAssets(
			mSelectedAssetsIcons.Select<String>([](UIAssetIcon* x) { return x->GetAssetInfo().mPath; }));
	}

	void UIAssetsIconsScrollArea::OnContextPastePressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.PasteAssets(mCurrentPath);
	}


	void UIAssetsIconsScrollArea::OnContextDeletePressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.DeleteAssets(
			mSelectedAssetsIcons.Select<String>([](UIAssetIcon* x) { return x->GetAssetInfo().mPath; }));
	}

	void UIAssetsIconsScrollArea::OnContextOpenPressed()
	{
		if (!IsFocused())
			return;

		if (mSelectedAssetsIcons.Count() > 0)
			o2EditorAssets.OpenAndEditAsset(mSelectedAssetsIcons.Last()->GetAssetInfo().mId);
	}

	void UIAssetsIconsScrollArea::OnContextShowInExplorerPressed()
	{
		if (!IsFocused())
			return;

		if (mSelectedAssetsIcons.Count() > 0)
			o2EditorAssets.OpenAsset(mSelectedAssetsIcons.Last()->GetAssetInfo().mId);
	}

	void UIAssetsIconsScrollArea::OnContextImportPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.ImportAssets(mCurrentPath);
	}

	void UIAssetsIconsScrollArea::OnContextCreateFolderPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CreateFolderAsset(mCurrentPath);
	}

	void UIAssetsIconsScrollArea::OnContextCreatePrefabPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CreatePrefabAsset(mCurrentPath);
	}

	void UIAssetsIconsScrollArea::OnContextCreateScriptPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CreateScriptAsset(mCurrentPath);
	}

	void UIAssetsIconsScrollArea::OnContextCreateAnimationPressed()
	{
		if (!IsFocused())
			return;

		o2EditorAssets.CreateAnimationAsset(mCurrentPath);
	}

	Actor* UIAssetsIconsScrollArea::InstantiateAsset(const ImageAsset& asset)
	{
		return mnew Actor({ mnew ImageComponent(asset) });
	}

	Actor* UIAssetsIconsScrollArea::InstantiateAsset(const ActorAsset& asset)
	{
		return mnew Actor(asset.actor);
	}

	void UIAssetsIconsScrollArea::Select(SelectableDragableObject* object, bool sendOnSelectionChanged)
	{
		UIAssetIcon* icon = dynamic_cast<UIAssetIcon*>(object);

		if (!mSelectedAssetsIcons.Contains(icon))
		{
			icon->SetSelected(true);
			mSelectedAssetsIcons.Add(icon);

			onAssetsSelected(mSelectedAssetsIcons.Select<String>([](UIAssetIcon* x) { return x->GetAssetInfo().mPath; }));
		}
	}

	void UIAssetsIconsScrollArea::Deselect(SelectableDragableObject* object)
	{
		UIAssetIcon* icon = dynamic_cast<UIAssetIcon*>(object);

		if (mSelectedAssetsIcons.Contains(icon))
		{
			icon->SetSelected(false);
			mSelectedAssetsIcons.Remove(icon);

			onAssetsSelected(mSelectedAssetsIcons.Select<String>([](UIAssetIcon* x) { return x->GetAssetInfo().mPath; }));
		}
	}

	void UIAssetsIconsScrollArea::AddSelectableObject(SelectableDragableObject* object)
	{}

	void UIAssetsIconsScrollArea::RemoveSelectableObject(SelectableDragableObject* object)
	{}

	void UIAssetsIconsScrollArea::OnSelectableObjectCursorReleased(SelectableDragableObject* object, const Input::Cursor& cursor)
	{
		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAllAssets();

		Select(object);
	}

	void UIAssetsIconsScrollArea::OnSelectableObjectBeganDragging(SelectableDragableObject* object)
	{
		if (!o2Input.IsKeyDown(VK_CONTROL) && !mSelectedAssetsIcons.Contains(dynamic_cast<UIAssetIcon*>(object)))
			DeselectAllAssets();

		if (!object->IsSelected())
			Select(object, false);
	}

	Actor* UIAssetsIconsScrollArea::InstantiateAsset(const AssetInfo& assetInfo)
	{
		if (assetInfo.mType == TypeOf(ImageAsset).ID())
			return InstantiateAsset(ImageAsset(assetInfo.mId));
		else if (assetInfo.mType == TypeOf(ActorAsset).ID())
			return InstantiateAsset(ActorAsset(assetInfo.mId));

		return nullptr;
	}

	ISelectableDragableObjectsGroup::SelectDragObjectsVec UIAssetsIconsScrollArea::GetSelectedDragObjects() const
	{
		return mSelectedAssetsIcons.Cast<SelectableDragableObject*>();
	}

	ISelectableDragableObjectsGroup::SelectDragObjectsVec UIAssetsIconsScrollArea::GetAllObjects() const
	{
		return mGrid->GetChilds().Cast<SelectableDragableObject*>();
	}

	void UIAssetsIconsScrollArea::Select(SelectableDragableObject* object)
	{
		Select(object, true);
	}
}

// 
// 		CursorAreaEventsListener* listenerUnderCursor = o2Events.GetCursorListenerUnderCursor(0);
// 
// 		SceneEditScreen* sceneEdit = dynamic_cast<SceneEditScreen*>(listenerUnderCursor);
// 		UIActorsTree* actorsTree = dynamic_cast<UIActorsTree*>(listenerUnderCursor);
// 		IAssetProperty* assetProperty = dynamic_cast<IAssetProperty*>(listenerUnderCursor);
// 		ActorProperty* actorProperty = dynamic_cast<ActorProperty*>(listenerUnderCursor);
// 		ComponentProperty* componentProperty = dynamic_cast<ComponentProperty*>(listenerUnderCursor);
// 
// 		if (!sceneEdit && mDragState == DragState::Scene)
// 		{
// 			ClearInstantiatedDraggingAssets();
// 			mDragState = DragState::Regular;
// 		}
// 
// 		if (!actorsTree && mDragState == DragState::Tree)
// 		{
// 			o2EditorTree.GetActorsTree()->BreakDragging();
// 			ClearInstantiatedDraggingAssets();
// 			mDragState = DragState::Regular;
// 		}
// 
// 		if (!assetProperty && mDragState == DragState::AssetField)
// 		{
// 			mDragState = DragState::Regular;
// 			mDragAssetPropertyField->GetWidget()->SetState("selected", false);
// 			o2Application.SetCursor(CursorType::Arrow);
// 		}
// 
// 		if (!actorProperty && mDragState == DragState::ActorField)
// 		{
// 			mDragState = DragState::Regular;
// 			mDragActorPropertyField->GetWidget()->SetState("selected", false);
// 			o2Application.SetCursor(CursorType::Arrow);
// 		}
// 
// 		if (!componentProperty && mDragState == DragState::ComponentField)
// 		{
// 			mDragState = DragState::Regular;
// 			mDragComponentPropertyField->GetWidget()->SetState("selected", false);
// 			o2Application.SetCursor(CursorType::Arrow);
// 		}
// 
// 		if (componentProperty && mDragState != DragState::ComponentField)
// 		{
// 			if (mSelectedAssetsIcons.Count() == 1 && mSelectedAssetsIcons[0].icon->GetAssetInfo().mType ==
// 				TypeOf(ActorAsset).ID())
// 			{
// 				Actor* assetActor = o2Scene.GetAssetActorByID(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId);
// 
// 				if (assetActor->GetComponent(componentProperty->GetSpecializedType()))
// 				{
// 					mDragState = DragState::ComponentField;
// 					componentProperty->GetWidget()->SetState("selected", true);
// 					mDragComponentPropertyField = componentProperty;
// 					o2Application.SetCursor(CursorType::Hand);
// 				}
// 			}
// 		}
// 
// 		if (actorProperty && mDragState != DragState::ActorField)
// 		{
// 			if (mSelectedAssetsIcons.Count() == 1 && mSelectedAssetsIcons[0].icon->GetAssetInfo().mType ==
// 				TypeOf(ActorAsset).ID())
// 			{
// 				mDragState = DragState::ActorField;
// 				actorProperty->GetWidget()->SetState("selected", true);
// 				mDragActorPropertyField = actorProperty;
// 				o2Application.SetCursor(CursorType::Hand);
// 			}
// 		}
// 
// 		if (assetProperty && mDragState != DragState::AssetField)
// 		{
// 			if (mSelectedAssetsIcons.Count() == 1 && assetProperty->GetFieldType()->ID() ==
// 				mSelectedAssetsIcons[0].icon->GetAssetInfo().mType)
// 			{
// 				mDragState = DragState::AssetField;
// 				assetProperty->GetWidget()->SetState("selected", true);
// 				mDragAssetPropertyField = assetProperty;
// 				o2Application.SetCursor(CursorType::Hand);
// 			}
// 		}
// 
// 		if (sceneEdit && mDragState != DragState::Scene)
// 		{
// 			InstantiateDraggingAssets();
// 
// 			if (!mInstSceneDragActors.IsEmpty())
// 				mDragState = DragState::Scene;
// 		}
// 
// 		if (actorsTree && mDragState != DragState::Tree)
// 		{
// 			InstantiateDraggingAssets();
// 
// 			if (!mInstSceneDragActors.IsEmpty())
// 			{
// 				o2EditorTree.GetActorsTree()->ManualBeginDraggingActors(mInstSceneDragActors);
// 				mDragState = DragState::Tree;
// 			}
// 		}
// 
// 		if (mDragState == DragState::Scene)
// 		{
// 			Vec2F sceneCursorPos = o2EditorSceneScreen.ScreenToScenePoint(cursor.position);
// 			for (auto actor : mInstSceneDragActors)
// 				actor->transform.worldPosition = sceneCursorPos;
// 		}
// 
// 		if (mDragState == DragState::Tree)
// 			o2EditorTree.GetActorsTree()->ManualUpdateDraggingActors(cursor);


// 	void UIAssetsIconsScrollArea::CompleteDragging()
// 	{
// 		for (auto sel : mSelectedAssetsIcons)
// 			sel.icon->Show();
// 
// 		CursorAreaEventsListener* listenerUnderCursor = o2Events.GetCursorListenerUnderCursor(0);
// 
// 		if (mDragState == DragState::Tree)
// 		{
// 			o2EditorTree.GetActorsTree()->CompleteManualDraggingActors();
// 			RegActorsCreationAction();
// 			mInstSceneDragActors.Clear();
// 
// 			o2UI.FocusWidget(o2EditorTree.GetActorsTree());
// 		}
// 
// 		if (mDragState == DragState::AssetField)
// 		{
// 			mDragAssetPropertyField->SetAssetId(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId);
// 			mDragAssetPropertyField->GetWidget()->SetState("selected", false);
// 			o2Application.SetCursor(CursorType::Arrow);
// 		}
// 
// 		if (mDragState == DragState::ActorField)
// 		{
// 			mDragActorPropertyField->SetValue(o2Scene.GetAssetActorByID(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId));
// 			mDragActorPropertyField->GetWidget()->SetState("selected", false);
// 			o2Application.SetCursor(CursorType::Arrow);
// 		}
// 
// 		if (mDragState == DragState::ComponentField)
// 		{
// 			Actor* actorAsset = o2Scene.GetAssetActorByID(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId);
// 			mDragComponentPropertyField->SetValue(actorAsset->GetComponent(mDragComponentPropertyField->GetSpecializedType()));
// 			mDragComponentPropertyField->GetWidget()->SetState("selected", false);
// 			o2Application.SetCursor(CursorType::Arrow);
// 		}
// 
// 		if (mDragState == DragState::Regular)
// 		{
// 			UITree* treeWindow = dynamic_cast<UITree*>(listenerUnderCursor);
// 			if (treeWindow)
// 			{
// 				UITreeNode* nodeUnderCursor = treeWindow->GetTreeNodeUnderPoint(o2Input.GetCursorPos());
// 				if (nodeUnderCursor)
// 				{
// 					AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)nodeUnderCursor->GetObject();
// 
// 					String destPath = assetTreeNode->mPath;
// 					auto assetsInfos = mSelectedAssetsIcons.Select<AssetInfo>([](auto x) { return x.icon->GetAssetInfo(); });
// 					o2Assets.MoveAssets(assetsInfos, destPath, true);
// 
// 					DeselectAllAssets();
// 				}
// 			}
// 
// 			UIAssetsIconsScrollArea* scrollArea = dynamic_cast<UIAssetsIconsScrollArea*>(listenerUnderCursor);
// 			if (scrollArea)
// 			{
// 				UIAssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
// 				if (iconUnderCursor && iconUnderCursor->GetAssetInfo().mType == TypeOf(FolderAsset).ID())
// 				{
// 					String destPath = iconUnderCursor->GetAssetInfo().mPath;
// 					auto assetsInfos = mSelectedAssetsIcons.Select<AssetInfo>([](auto x) { return x.icon->GetAssetInfo(); });
// 					o2Assets.MoveAssets(assetsInfos, destPath, true);
// 
// 					DeselectAllAssets();
// 				}
// 			}
// 		}
// 
// 		if (mDragState == DragState::Scene)
// 		{
// 			RegActorsCreationAction();
// 
// 			o2UI.FocusWidget(o2EditorTree.GetActorsTree());
// 			o2EditorTree.GetActorsTree()->SetSelectedActors(mInstSceneDragActors);
// 
// 			mInstSceneDragActors.Clear();
// 		}
// 
// 		mDragState = DragState::Off;
// 	}


// 		CursorAreaEventsListener* listenerUnderCursor = o2Events.GetCursorListenerUnderCursor(0);
// 
// 		if (mDragState == DragState::Tree)
// 		{
// 			o2EditorTree.GetActorsTree()->CompleteManualDraggingActors();
// 			RegActorsCreationAction();
// 			mInstSceneDragActors.Clear();
// 
// 			o2UI.FocusWidget(o2EditorTree.GetActorsTree());
// 		}
// 
// 		if (mDragState == DragState::AssetField)
// 		{
// 			mDragAssetPropertyField->SetAssetId(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId);
// 			mDragAssetPropertyField->GetWidget()->SetState("selected", false);
// 			o2Application.SetCursor(CursorType::Arrow);
// 		}
// 
// 		if (mDragState == DragState::ActorField)
// 		{
// 			mDragActorPropertyField->SetValue(o2Scene.GetAssetActorByID(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId));
// 			mDragActorPropertyField->GetWidget()->SetState("selected", false);
// 			o2Application.SetCursor(CursorType::Arrow);
// 		}
// 
// 		if (mDragState == DragState::ComponentField)
// 		{
// 			Actor* actorAsset = o2Scene.GetAssetActorByID(mSelectedAssetsIcons[0].icon->GetAssetInfo().mId);
// 			mDragComponentPropertyField->SetValue(actorAsset->GetComponent(mDragComponentPropertyField->GetSpecializedType()));
// 			mDragComponentPropertyField->GetWidget()->SetState("selected", false);
// 			o2Application.SetCursor(CursorType::Arrow);
// 		}
// 
// 		if (mDragState == DragState::Regular)
// 		{
// 			UITree* treeWindow = dynamic_cast<UITree*>(listenerUnderCursor);
// 			if (treeWindow)
// 			{
// 				UITreeNode* nodeUnderCursor = treeWindow->GetTreeNodeUnderPoint(o2Input.GetCursorPos());
// 				if (nodeUnderCursor)
// 				{
// 					AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)nodeUnderCursor->GetObject();
// 
// 					String destPath = assetTreeNode->mPath;
// 					auto assetsInfos = mSelectedAssetsIcons.Select<AssetInfo>([](auto x) { return x.icon->GetAssetInfo(); });
// 					o2Assets.MoveAssets(assetsInfos, destPath, true);
// 
// 					DeselectAllAssets();
// 				}
// 			}
// 
// 			UIAssetsIconsScrollArea* scrollArea = dynamic_cast<UIAssetsIconsScrollArea*>(listenerUnderCursor);
// 			if (scrollArea)
// 			{
// 				UIAssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
// 				if (iconUnderCursor && iconUnderCursor->GetAssetInfo().mType == TypeOf(FolderAsset).ID())
// 				{
// 					String destPath = iconUnderCursor->GetAssetInfo().mPath;
// 					auto assetsInfos = mSelectedAssetsIcons.Select<AssetInfo>([](auto x) { return x.icon->GetAssetInfo(); });
// 					o2Assets.MoveAssets(assetsInfos, destPath, true);
// 
// 					DeselectAllAssets();
// 				}
// 			}
// 		}
// 
// 		if (mDragState == DragState::Scene)
// 		{
// 			RegActorsCreationAction();
// 
// 			o2UI.FocusWidget(o2EditorTree.GetActorsTree());
// 			o2EditorTree.GetActorsTree()->SetSelectedActors(mInstSceneDragActors);
// 
// 			mInstSceneDragActors.Clear();
// 		}
// 
// 		mDragState = DragState::Off;
 
CLASS_META(Editor::UIAssetsIconsScrollArea)
{
	BASE_CLASS(o2::UIScrollArea);
	BASE_CLASS(o2::DragDropArea);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::ISelectableDragableObjectsGroup);

	PUBLIC_FIELD(onAssetsSelected);
	PROTECTED_FIELD(mAssetIconSize);
	PROTECTED_FIELD(mCurrentPath);
	PROTECTED_FIELD(mGrid);
	PROTECTED_FIELD(mContextMenu);
	PROTECTED_FIELD(mSelectedAssetsIcons);
	PROTECTED_FIELD(mHightlightIcon);
	PROTECTED_FIELD(mHightlightAnim).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHightlightSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHightlightLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIconsPool);
	PROTECTED_FIELD(mSelectionSprite).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelecting);
	PROTECTED_FIELD(mPressedPoint);
	PROTECTED_FIELD(mCurrentSelectingIcons);
	PROTECTED_FIELD(mIsDraggingIcons);
	PROTECTED_FIELD(mDragEnded);
	PROTECTED_FIELD(mDragIcon);
	PROTECTED_FIELD(mDragOffset);
	PROTECTED_FIELD(mInstSceneDragActors);
	PROTECTED_FIELD(mCuttingAssets);
	PROTECTED_FIELD(mNeedRebuildAssets);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetViewingPath, const String&);
	PUBLIC_FUNCTION(String, GetViewingPath);
	PUBLIC_FUNCTION(void, UpdateAssetsPath);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(void, HightlightAsset, UID);
	PUBLIC_FUNCTION(void, SelectAsset, UID, bool);
	PUBLIC_FUNCTION(void, DeselectAllAssets);
	PUBLIC_FUNCTION(Vector<AssetInfo>, GetSelectedAssets);
	PUBLIC_FUNCTION(UIAssetIcon*, GetIconUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(Sprite*, GetHightlightDrawable);
	PUBLIC_FUNCTION(void, SetHightlightAnimation, const Animation&);
	PUBLIC_FUNCTION(void, SetHightlightLayout, const Layout&);
	PUBLIC_FUNCTION(Sprite*, GetSelectingDrawable);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, UpdateLayout, bool, bool);
	PROTECTED_FUNCTION(void, UpdateCuttingAssets);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnUnfocused);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, BeginSelecting);
	PROTECTED_FUNCTION(void, UpdateSelection, const Input::Cursor&);
	PROTECTED_FUNCTION(void, CompleteSelecting);
	PROTECTED_FUNCTION(void, RegActorsCreationAction);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorRightMouseReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, InitializeContext);
	PROTECTED_FUNCTION(void, PrepareIconsPools);
	PROTECTED_FUNCTION(UIAssetIcon*, GetAssetIconFromPool, const String&);
	PROTECTED_FUNCTION(void, FreeAssetIconToPool, UIAssetIcon*);
	PROTECTED_FUNCTION(UIAssetIcon*, GetImageAssetIcon, const AssetInfo&);
	PROTECTED_FUNCTION(void, UpdateAssetsGridSize);
	PROTECTED_FUNCTION(void, OnIconDblClicked, UIAssetIcon*);
	PROTECTED_FUNCTION(void, OnContextCopyPressed);
	PROTECTED_FUNCTION(void, OnContextCutPressed);
	PROTECTED_FUNCTION(void, OnContextPastePressed);
	PROTECTED_FUNCTION(void, OnContextDeletePressed);
	PROTECTED_FUNCTION(void, OnContextOpenPressed);
	PROTECTED_FUNCTION(void, OnContextShowInExplorerPressed);
	PROTECTED_FUNCTION(void, OnContextImportPressed);
	PROTECTED_FUNCTION(void, OnContextCreateFolderPressed);
	PROTECTED_FUNCTION(void, OnContextCreatePrefabPressed);
	PROTECTED_FUNCTION(void, OnContextCreateScriptPressed);
	PROTECTED_FUNCTION(void, OnContextCreateAnimationPressed);
	PROTECTED_FUNCTION(void, InstantiateDraggingAssets);
	PROTECTED_FUNCTION(void, ClearInstantiatedDraggingAssets);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const AssetInfo&);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const ImageAsset&);
	PROTECTED_FUNCTION(Actor*, InstantiateAsset, const ActorAsset&);
	PROTECTED_FUNCTION(SelectDragObjectsVec, GetSelectedDragObjects);
	PROTECTED_FUNCTION(SelectDragObjectsVec, GetAllObjects);
	PROTECTED_FUNCTION(void, Select, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, Select, SelectableDragableObject*, bool);
	PROTECTED_FUNCTION(void, Deselect, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, AddSelectableObject, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, RemoveSelectableObject, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, OnSelectableObjectCursorReleased, SelectableDragableObject*, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnSelectableObjectBeganDragging, SelectableDragableObject*);
	PROTECTED_FUNCTION(void, BeginDragging, UIAssetIcon*);
	PROTECTED_FUNCTION(void, EndDragging, bool);
	PROTECTED_FUNCTION(void, UpdateDraggingGraphics);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDraggedAbove, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDroppedFromThis);
	PROTECTED_FUNCTION(void, OnDroppedFromActorsTree, UIActorsTree*);
}
END_META;
 