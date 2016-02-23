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
#include "AssetsWindow/AssetsWindow.h"
#include "Events/CursorEventsListener.h"
#include "Events/EventSystem.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "SceneWindow/SceneEditScreen.h"
#include "SceneWindow/SceneEditWidget.h"
#include "UI/ContextMenu.h"
#include "UI/EditBox.h"
#include "UI/GridLayout.h"
#include "UI/Tree.h"
#include "UI/UIManager.h"
#include "UIAssetIcon.h"
#include "Utils/Clipboard.h"
#include "Utils/FileSystem/FileSystem.h"

UIAssetsIconsScrollArea::UIAssetsIconsScrollArea():
	UIScrollArea(), DrawableCursorEventsListener(this), mSelection(nullptr), mPressTime(10), mIconHoverSprite(nullptr),
	mContextMenu(nullptr)
{
	mGrid = mnew UIGridLayout();
	mGrid->layout = UIWidgetLayout::BothStretch();
	mGrid->baseCorner = BaseCorner::LeftTop;
	mGrid->fitByChildren = true;
	mGrid->cellSize = mAssetIconSize;
	mGrid->border = RectF(5, 5, 5, 5);
	mGrid->arrangeAxis = TwoDirection::Horizontal;
	mGrid->arrangeAxisMaxCells = 5;
	onLayoutChanged += Function<void()>(this, &UIAssetsIconsScrollArea::UpdateAssetsGridSize);
	AddChild(mGrid);

	mSelection = mnew Sprite("ui/UI_Window_place.png");
	InitializeSelectionSprite();

	mDragIcon = o2UI.CreateWidget<UIAssetIcon>();
}

UIAssetsIconsScrollArea::UIAssetsIconsScrollArea(const UIAssetsIconsScrollArea& other):
	UIScrollArea(other), mSelection(other.mSelection->Clone()), DrawableCursorEventsListener(this), mPressTime(10), 
	mIconHoverSprite(nullptr), mContextMenu(nullptr)
{
	mGrid = FindChild<UIGridLayout>();
	onLayoutChanged += Function<void()>(this, &UIAssetsIconsScrollArea::UpdateAssetsGridSize);
	InitializeSelectionSprite();

	mDragIcon = o2UI.CreateWidget<UIAssetIcon>();

	RetargetStatesAnimations();
	UpdateLayout();
	InitializeContext();
}

UIAssetsIconsScrollArea::~UIAssetsIconsScrollArea()
{
	delete mSelection;
	delete mDragIcon;

	for (auto kv : mIconsPool)
		for (auto icon : kv.Value())
			delete icon;
}

UIAssetsIconsScrollArea& UIAssetsIconsScrollArea::operator=(const UIAssetsIconsScrollArea& other)
{
	delete mSelection;

	UIScrollArea::operator=(other);

	mGrid = FindChild<UIGridLayout>();
	mSelection = other.mSelection->Clone();

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

	o2Render.EnableScissorTest(mAbsoluteClipArea);

	mIconHoverSprite->Draw();

	for (auto& sel : mSelectedAssetsIcons)
		sel.selectionSprite->Draw();

	for (auto& sel : mCurrentSelectingIcons)
		sel.selectionSprite->Draw();

	for (auto child : mChilds)
		child->Draw();

	if (mSelecting)
		mSelection->Draw();

	o2Render.DisableScissorTest();

	for (auto layer : mTopDrawingLayers)
		layer->Draw();

	if (mOwnHorScrollBar)
		mHorScrollBar->Draw();

	if (mOwnVerScrollBar)
		mVerScrollBar->Draw();

	if (mDragging)
		o2UI.DrawWidgetAtTop(mDragIcon);

	if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
		DrawDebugFrame();
}

void UIAssetsIconsScrollArea::Update(float dt)
{
	UIScrollArea::Update(dt);

	mPressTime += dt;

	UpdateHover();

	float hoverRectTransitionCoef = 20.0f;
	if (mCurrentHoverSpriteRect != mTargetHoverSpriteRect)
	{
		mCurrentHoverSpriteRect = Math::Lerp(mCurrentHoverSpriteRect, mTargetHoverSpriteRect, dt*hoverRectTransitionCoef);
		mIconHoverSprite->SetRect(mCurrentHoverSpriteRect);
	}

	if (mNeedRebuildAssets)
	{
		mNeedRebuildAssets = false;
		o2Assets.RebuildAssets();
	}
}

void UIAssetsIconsScrollArea::SetViewingPath(const String& path)
{
	mCurrentPath = path;
	UpdateAssetsPath();
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

	folderAssetsInfos.Reverse();

	Vector<UIWidget*> addingIcons;
	for (auto asset : folderAssetsInfos)
	{
		UIAssetIcon* assetIcon;

		if (asset.mType == FolderAsset::type.ID())
		{
			assetIcon = GetAssetIconFromPool("folder");
			assetIcon->name = "folder";
		}
		else if (asset.mType == ImageAsset::type.ID())
		{
			assetIcon = GetImageAssetIcon(asset);
			assetIcon->name = "image preview";
		}
		else if (asset.mType == DataAsset::type.ID())
		{
			assetIcon = GetAssetIconFromPool("text");
			assetIcon->name = "text";
		}
		else if (asset.mType == AnimationAsset::type.ID())
		{
			assetIcon = GetAssetIconFromPool("animation");
			assetIcon->name = "animation";
		}
		else if (asset.mType == ActorAsset::type.ID())
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

		addingIcons.Add(assetIcon);
	}

	mGrid->AddChilds(addingIcons);

	UpdateCuttingAssets();
}

bool UIAssetsIconsScrollArea::IsSelectable() const
{
	return true;
}

void UIAssetsIconsScrollArea::SelectAsset(AssetId id)
{
	UIAssetIcon* icon = (UIAssetIcon*)(mGrid->GetChilds().FindMatch([=](UIWidget* x) {
		return ((UIAssetIcon*)x)->GetAssetInfo().mId == id; }));

	if (!icon)
		return;

	if (mSelectedAssetsIcons.ContainsPred([=](auto x) { return x.icon == icon; }))
		return;

	icon->SetState("assetSelection", true);
	auto selSprite = GetSelectionSprite();
	selSprite->SetRect(mSelectionSpriteLayout.Calculate(icon->layout.GetAbsoluteRect()));
	mSelectedAssetsIcons.Add(IconSelection(icon, selSprite));
}

void UIAssetsIconsScrollArea::DeselectAllAssets()
{
	for (auto icon : mSelectedAssetsIcons)
		FreeSelectionSprite(icon.selectionSprite);

	mSelectedAssetsIcons.Clear();
}

Vector<AssetInfo> UIAssetsIconsScrollArea::GetSelectedAssets() const
{
	return mSelectedAssetsIcons.Select<AssetInfo>([](const IconSelection& x) { return x.icon->GetAssetInfo(); });
}

void UIAssetsIconsScrollArea::UpdateLayout(bool forcible /*= false*/)
{
	Vec2F localPressPoint = mPressedPoint - mChildsAbsRect.LeftBottom();

	UIScrollArea::UpdateLayout(forcible);

	mPressedPoint = localPressPoint + mChildsAbsRect.LeftBottom();
	
	if (mSelecting)
		UpdateSelection(*o2Input.GetCursor(0));

	for (auto& sel : mSelectedAssetsIcons)
		sel.selectionSprite->SetRect(mSelectionSpriteLayout.Calculate(sel.icon->layout.GetAbsoluteRect()));

	for (auto& sel : mCurrentSelectingIcons)
		sel.selectionSprite->SetRect(mSelectionSpriteLayout.Calculate(sel.icon->layout.GetAbsoluteRect()));

	UpdateHover();
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

void UIAssetsIconsScrollArea::OnSelected()
{
	for (auto& sel : mSelectedAssetsIcons)
		sel.selectionSprite->SetColor(mSelectedColor);
}

void UIAssetsIconsScrollArea::OnDeselected()
{
	for (auto& sel : mSelectedAssetsIcons)
		sel.selectionSprite->SetColor(mUnselectedColor);
}

void UIAssetsIconsScrollArea::OnCursorPressed(const Input::Cursor& cursor)
{
	mPressedPoint = cursor.mPosition;
	o2UI.SelectWidget(this);
}

void UIAssetsIconsScrollArea::OnCursorStillDown(const Input::Cursor& cursor)
{
	if (cursor.mDelta == Vec2F())
		return;

	float beginDragOrSelectThreshold = 5.0f;

	if (!mSelecting && !mDragging && (cursor.mPosition - mPressedPoint).Length() > beginDragOrSelectThreshold)
	{
		UIAssetIcon* iconUnderCursor = GetIconUnderPoint(mPressedPoint);
		if (iconUnderCursor && mSelectedAssetsIcons.ContainsPred([=](auto x) { return x.icon == iconUnderCursor; }))
			BeginDragging(iconUnderCursor, cursor);
		else
			BeginSelecting();
	}

	if (mSelecting)
		UpdateSelection(cursor);

	if (mDragging)
		UpdateDragging(cursor);
}

void UIAssetsIconsScrollArea::BeginSelecting()
{
	mSelecting = true;

	if (!o2Input.IsKeyDown(VK_CONTROL))
		DeselectAllAssets();
}

void UIAssetsIconsScrollArea::OnCursorReleased(const Input::Cursor& cursor)
{
	if (mPressTime < 0.3f && !mSelecting)
	{
		UIAssetIcon* iconUnderCursor = GetIconUnderPoint(cursor.mPosition);
		if (iconUnderCursor)
			OnIconDblClicked(iconUnderCursor);
	}
	mPressTime = 0.0f;

	if (mSelecting) CompleteSelecting();
	else if (mDragging) CompleteDragging();
	else
	{
		if (!o2Input.IsKeyDown(VK_CONTROL))
			DeselectAllAssets();

		UIAssetIcon* iconUnderCursor = GetIconUnderPoint(cursor.mPosition);
		if (iconUnderCursor && !mSelectedAssetsIcons.ContainsPred([=](auto x) { return x.icon == iconUnderCursor; }))
		{
			iconUnderCursor->SetState("assetSelection", true);
			auto selSprite = GetSelectionSprite();
			selSprite->SetRect(mSelectionSpriteLayout.Calculate(iconUnderCursor->layout.GetAbsoluteRect()));
			mSelectedAssetsIcons.Add(IconSelection(iconUnderCursor, selSprite));
		}
	}
}


void UIAssetsIconsScrollArea::BeginDragging(UIAssetIcon* iconUnderCursor, const Input::Cursor &cursor)
{
	mDragging = true;
	for (auto sel : mSelectedAssetsIcons)
		sel.icon->Hide();

	*mDragIcon = *mSelectedAssetsIcons.Last().icon;
	mDragOffset = iconUnderCursor->layout.absRect->Center() - cursor.mPosition;

	if (mSelectedAssetsIcons.Count() > 1)
		((Text*)mDragIcon->layer["assetName"]->drawable)->text = (String)mSelectedAssetsIcons.Count() + " items";
}

void UIAssetsIconsScrollArea::UpdateDragging(const Input::Cursor& cursor)
{
	mDragIcon->SetParent(nullptr);
	mDragIcon->layout.SetRect(RectF(cursor.mPosition - mAssetIconSize*0.5f + mDragOffset,
									cursor.mPosition + mAssetIconSize*0.5f + mDragOffset));
}

void UIAssetsIconsScrollArea::CompleteDragging()
{
	mDragging = false;
	for (auto sel : mSelectedAssetsIcons)
		sel.icon->Show();

	CursorEventsListener* listenerUnderCursor = o2Events.GetCursorListenerUnderCursor(0);

	UITree* treeWindow = dynamic_cast<UITree*>(listenerUnderCursor);
	if (treeWindow)
	{
		if (treeWindow->name == "actors")
		{
			o2Debug.Log("Put actors tree");
		}
		else if (treeWindow->name == "folders")
		{
			UITreeNode* nodeUnderCursor = treeWindow->GetTreeNodeUnderPoint(o2Input.GetCursorPos());
			if (nodeUnderCursor)
			{
				AssetTree::AssetNode* assetTreeNode = (AssetTree::AssetNode*)(void*)nodeUnderCursor->GetObject();

				String destPath = assetTreeNode->mPath;
				auto assetsInfos = mSelectedAssetsIcons.Select<AssetInfo>([](auto x) { return x.icon->GetAssetInfo(); });
				o2Assets.MoveAssets(assetsInfos, destPath, true);

				DeselectAllAssets();
			}
		}
	}

	UIAssetsIconsScrollArea* scrollArea = dynamic_cast<UIAssetsIconsScrollArea*>(listenerUnderCursor);
	if (scrollArea)
	{
		UIAssetIcon* iconUnderCursor = GetIconUnderPoint(o2Input.GetCursorPos());
		if (iconUnderCursor && iconUnderCursor->GetAssetInfo().mType == FolderAsset::type.ID())
		{
			String destPath = iconUnderCursor->GetAssetInfo().mPath;
			auto assetsInfos = mSelectedAssetsIcons.Select<AssetInfo>([](auto x) { return x.icon->GetAssetInfo(); });
			o2Assets.MoveAssets(assetsInfos, destPath, true);

			DeselectAllAssets();
		}
	}

	SceneEditScreen* sceneEdit = dynamic_cast<SceneEditScreen*>(listenerUnderCursor);
	if (sceneEdit)
	{
		o2Debug.Log("Put scene");
	}
}

void UIAssetsIconsScrollArea::OnCursorPressBreak(const Input::Cursor& cursor)
{}

void UIAssetsIconsScrollArea::UpdateSelection(const Input::Cursor& cursor)
{
	RectF selectionRect(cursor.mPosition, mPressedPoint);
	mSelection->SetRect(selectionRect);

	for (auto icon : mCurrentSelectingIcons)
		FreeSelectionSprite(icon.selectionSprite);

	mCurrentSelectingIcons.Clear();

	for (auto child : mGrid->GetChilds())
	{
		if (child->layout.GetAbsoluteRect().IsIntersects(selectionRect))
		{
			UIAssetIcon* icon = (UIAssetIcon*)child;
			icon->SetState("assetSelection", true);
			auto selSprite = GetSelectionSprite();
			selSprite->SetRect(mSelectionSpriteLayout.Calculate(icon->layout.GetAbsoluteRect()));
			mCurrentSelectingIcons.Add(IconSelection(icon, selSprite));
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
	UIAssetIcon* iconUnderCursor = GetIconUnderPoint(cursor.mPosition);
	if (iconUnderCursor)
	{
		if (!mSelectedAssetsIcons.ContainsPred([=](const IconSelection& x) { return x.icon == iconUnderCursor; }))
		{
			DeselectAllAssets();
			iconUnderCursor->SetState("assetSelection", true);
			auto selSprite = GetSelectionSprite();
			selSprite->SetRect(mSelectionSpriteLayout.Calculate(iconUnderCursor->layout.GetAbsoluteRect()));
			mCurrentSelectingIcons.Add(IconSelection(iconUnderCursor, selSprite));
		}
	}

	o2UI.SelectWidget(this);
	mContextMenu->Show();
}

void UIAssetsIconsScrollArea::OnKeyReleased(const Input::Key& key)
{
	if (mIsSelected && key == VK_BACK)
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
	String iconsStyles[] = { "standard", "folder", "prefab", "prefab preview", "image preview", "text", "animation" };

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

	previewSprite->imageAsset = &previewSpriteAsset;
	previewSprite->mode = SpriteMode::Default;

	return assetIcon;
}

void UIAssetsIconsScrollArea::UpdateAssetsGridSize()
{
	mGrid->arrangeAxisMaxCells = Math::Max(1, Math::FloorToInt(layout.GetWidth() / mAssetIconSize.x));
}

void UIAssetsIconsScrollArea::InitializeSelectionSprite()
{
	mIconSelectionSprite = mnew Sprite("ui/UI_Context_menu_white.png");
	mSelectionSpriteLayout = Layout::BothStretch(-10, -7, -10, -15);

	mIconHoverSprite = mnew Sprite("ui/UI_ListBox_selection_hover.png");
	mIconHoverSprite->SetColor(mHoverColor);
	AddState("hover", Animation::EaseInOut<float>(this, &mIconHoverSprite->transparency, 0.0f, 1.0f, 0.1f));

	int initialPoolSize = 40;
	for (int i = 0; i < initialPoolSize; i++)
		mSelectionSpritesPool.Add(mIconSelectionSprite->Clone());
}

Sprite* UIAssetsIconsScrollArea::GetSelectionSprite()
{
	if (mSelectionSpritesPool.Count() == 0)
	{
		int poolResizeCount = 20;
		for (int i = 0; i < poolResizeCount; i++)
			mSelectionSpritesPool.Add(mIconSelectionSprite->Clone());
	}

	Sprite* res = mSelectionSpritesPool.PopBack();
	res->SetColor(mIsSelected ? mSelectedColor : mUnselectedColor);
	return res;
}

void UIAssetsIconsScrollArea::FreeSelectionSprite(Sprite* sprite)
{
	mSelectionSpritesPool.Add(sprite);
}

void UIAssetsIconsScrollArea::OnIconDblClicked(UIAssetIcon* icon)
{
	AssetInfo iconAssetInfo = icon->GetAssetInfo();
	auto assetNameLayer = icon->layer["assetName"];
	if (assetNameLayer && assetNameLayer->drawable->IsUnderPoint(o2Input.cursorPos))
	{
		icon->SetState("edit", true);

		auto editBox = (UIEditBox*)icon->GetChild("nameEditBox");

		editBox->text = o2FileSystem.GetFileNameWithoutExtension(
			o2FileSystem.GetPathWithoutDirectories(iconAssetInfo.mPath));

		editBox->SelectAll();
		editBox->UIWidget::Select();
		editBox->ResetSroll();

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

	if (iconAssetInfo.mType == FolderAsset::type.ID())
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

void UIAssetsIconsScrollArea::UpdateHover()
{
	if (!mIconHoverSprite)
		return;

	if (!mSelecting)
	{
		UIAssetIcon* hoverIcon = GetIconUnderPoint(o2Input.GetCursorPos());

		if (hoverIcon)
		{
			SetState("hover", true);
			mTargetHoverSpriteRect = mSelectionSpriteLayout.Calculate(hoverIcon->layout.GetAbsoluteRect());
		}
		else SetState("hover", false);

		mHoverIcon = hoverIcon;
	}
	else SetState("hover", false);
}

#undef CopyFile

void UIAssetsIconsScrollArea::OnContextCopyPressed()
{
	if (!IsSelected())
		return;

	o2EditorAssets.CopyAssets(
		mSelectedAssetsIcons.Select<String>([](const IconSelection& x) { return x.icon->GetAssetInfo().mPath; }));
}

void UIAssetsIconsScrollArea::OnContextCutPressed()
{
	if (!IsSelected())
		return;

	o2EditorAssets.CutAssets(
		mSelectedAssetsIcons.Select<String>([](const IconSelection& x) { return x.icon->GetAssetInfo().mPath; }));
}

void UIAssetsIconsScrollArea::OnContextPastePressed()
{
	if (!IsSelected())
		return;

	o2EditorAssets.PasteAssets(mCurrentPath);
}


void UIAssetsIconsScrollArea::OnContextDeletePressed()
{
	if (!IsSelected())
		return;

	o2EditorAssets.DeleteAssets(
		mSelectedAssetsIcons.Select<String>([](const IconSelection& x) { return x.icon->GetAssetInfo().mPath; }));
}

void UIAssetsIconsScrollArea::OnContextOpenPressed()
{
	if (!IsSelected())
		return;

	if (mSelectedAssetsIcons.Count() > 0)
		o2EditorAssets.OpenAndEditAsset(mSelectedAssetsIcons.Last().icon->GetAssetInfo().mId);
}

void UIAssetsIconsScrollArea::OnContextShowInExplorerPressed()
{
	if (!IsSelected())
		return;

	if (mSelectedAssetsIcons.Count() > 0)
		o2EditorAssets.OpenAsset(mSelectedAssetsIcons.Last().icon->GetAssetInfo().mId);
}

void UIAssetsIconsScrollArea::OnContextImportPressed()
{
	if (!IsSelected())
		return;

	o2EditorAssets.ImportAssets(mCurrentPath);
}

void UIAssetsIconsScrollArea::OnContextCreateFolderPressed()
{
	if (!IsSelected())
		return;

	o2EditorAssets.CreateFolderAsset(mCurrentPath);
}

void UIAssetsIconsScrollArea::OnContextCreatePrefabPressed()
{
	if (!IsSelected())
		return;

	o2EditorAssets.CreatePrefabAsset(mCurrentPath);
}

void UIAssetsIconsScrollArea::OnContextCreateScriptPressed()
{
	if (!IsSelected())
		return;

	o2EditorAssets.CreateScriptAsset(mCurrentPath);
}

void UIAssetsIconsScrollArea::OnContextCreateAnimationPressed()
{
	if (!IsSelected())
		return;

	o2EditorAssets.CreateAnimationAsset(mCurrentPath);
}

UIAssetsIconsScrollArea::IconSelection::IconSelection():
	icon(nullptr), selectionSprite(nullptr)
{}

UIAssetsIconsScrollArea::IconSelection::IconSelection(UIAssetIcon* icon, Sprite* selectionSprite) :
	icon(icon), selectionSprite(selectionSprite)
{}

bool UIAssetsIconsScrollArea::IconSelection::operator==(const IconSelection& other) const
{
	return icon == other.icon;
}
