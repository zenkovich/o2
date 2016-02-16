#include "AssetsIconsScroll.h"

#include "Assets/Assets.h"
#include "Assets/FolderAsset.h"
#include "Assets/ImageAsset.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "UI/GridLayout.h"
#include "UI/UIManager.h"
#include "UIAssetIcon.h"

UIAssetsIconsScrollArea::UIAssetsIconsScrollArea():
	UIScrollArea(), DrawableCursorEventsListener(this)
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

	mSelection = mnew Sprite("ui/UI_Window_place.png");
	InitializeSelectionSprite();
}

UIAssetsIconsScrollArea::UIAssetsIconsScrollArea(const UIAssetsIconsScrollArea& other):
	UIScrollArea(other), mSelection(other.mSelection->Clone()), DrawableCursorEventsListener(this)
{
	mGrid = FindChild<UIGridLayout>();
	onLayoutChanged += Function<void()>(this, &UIAssetsIconsScrollArea::UpdateAssetsGridSize);
	InitializeSelectionSprite();

	RetargetStatesAnimations();
	UpdateLayout();
}

UIAssetsIconsScrollArea::~UIAssetsIconsScrollArea()
{
	delete mSelection;

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

	if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
		DrawDebugFrame();
}

void UIAssetsIconsScrollArea::SetAssetPath(const String& path)
{
	o2Debug.LogWarning("Update assets folder: %s", path);

	ClearAssetIconsSelection();

	auto prevIcons = mGrid->GetChilds().Select<UIAssetIcon*>([](auto x) { return (UIAssetIcon*)x; });
	prevIcons.ForEach([&](auto x) { FreeAssetIconToPool(x); });
	mGrid->RemoveAllChilds(false);

	AssetInfosVec folderAssetsInfos;
	if (path != "")
		folderAssetsInfos = FolderAsset(path).GetContainingAssetsInfos();
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
		else
		{
			assetIcon = GetAssetIconFromPool("standard");
			assetIcon->name = "standard";
		}

		assetIcon->SetAssetInfo(asset);

		addingIcons.Add(assetIcon);
	}

	mGrid->AddChilds(addingIcons);
}

bool UIAssetsIconsScrollArea::IsSelectable() const
{
	return true;
}

void UIAssetsIconsScrollArea::UpdateLayout(bool forcible /*= false*/)
{
	UIScrollArea::UpdateLayout(forcible);

	for (auto& sel : mSelectedAssetsIcons)
		sel.selectionSprite->SetRect(mSelectionSpriteLayout.Calculate(sel.icon->layout.GetAbsoluteRect()));

	for (auto& sel : mCurrentSelectingIcons)
		sel.selectionSprite->SetRect(mSelectionSpriteLayout.Calculate(sel.icon->layout.GetAbsoluteRect()));
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

void UIAssetsIconsScrollArea::OnCursorReleased(const Input::Cursor& cursor)
{
	if (!mSelecting)
	{
		if (!o2Input.IsKeyDown(VK_CONTROL))
			ClearAssetIconsSelection();

		for (auto child : mGrid->GetChilds())
		{
			if (child->layout.IsUnderPoint(cursor.mPosition))
			{
				UIAssetIcon* icon = (UIAssetIcon*)child;
				icon->SetState("assetSelection", true);
				auto selSprite = GetSelectionSprite();
				selSprite->SetRect(mSelectionSpriteLayout.Calculate(icon->layout.GetAbsoluteRect()));
				mSelectedAssetsIcons.Add(IconSelection(icon, selSprite));
			}
		}
	}
	else
	{
		mSelectedAssetsIcons.Add(mCurrentSelectingIcons);
		mCurrentSelectingIcons.Clear();
		mSelecting = false;
	}
}

void UIAssetsIconsScrollArea::OnCursorPressBreak(const Input::Cursor& cursor)
{}

void UIAssetsIconsScrollArea::OnCursorStillDown(const Input::Cursor& cursor)
{
	if (cursor.mDelta == Vec2F())
		return;

	if (!mSelecting && (cursor.mPosition - mPressedPoint).Length() > 5.0f)
	{
		mSelecting = true;

		if (!o2Input.IsKeyDown(VK_CONTROL))
			ClearAssetIconsSelection();
	}

	if (mSelecting)
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
}

void UIAssetsIconsScrollArea::OnCursorMoved(const Input::Cursor& cursor)
{}

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
		return o2UI.CreateWidget<UIAssetIcon>(style);

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

void UIAssetsIconsScrollArea::ClearAssetIconsSelection()
{
	for (auto icon : mSelectedAssetsIcons)
		FreeSelectionSprite(icon.selectionSprite);

	mSelectedAssetsIcons.Clear();
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
	mGrid->arrangeAxisMaxCells = Math::Max(1, Math::FloorToInt(layout.GetWidth() / mAssetIconSize.x) - 1);
}

void UIAssetsIconsScrollArea::InitializeSelectionSprite()
{
	mIconSelectionSprite = mnew Sprite("ui/UI_Context_menu_white.png");
	mSelectionSpriteLayout = Layout::BothStretch(-13, -9, -13, -18);

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
