#include "ContextMenu.h"

#include "Assets/ImageAsset.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/Button.h"
#include "UI/VerticalLayout.h"

namespace o2
{
	bool UIContextMenu::Item::operator==(const Item& other) const
	{
		return text == other.text && shortcut == other.shortcut && icon == other.icon;
	}

	UIContextMenu::Item::Item():
		icon(nullptr)
	{}

	UIContextMenu::Item::Item(const WString& text, const Function<void()> onClick, ImageAsset* icon /*= nullptr*/,
							  const ShortcutKeys& shortcut /*= ShortcutKeys()*/):
		text(text), onClick(onClick), shortcut(shortcut), icon(icon)
	{}

	UIContextMenu::Item::Item(const WString& text, Vector<Item> subItems, ImageAsset* icon /*= nullptr*/) :
		text(text), subItems(subItems), icon(icon)
	{}

	UIContextMenu::Item::~Item()
	{
		delete icon;
	}

	UIContextMenu::Item UIContextMenu::Item::Separator()
	{
		return Item("---", Function<void()>());
	}

	UIContextMenu::UIContextMenu():
		UIScrollArea(), mSelectedItem(-1), mPressedItem(-1), mSelectSubContextTime(-1), mParentContextMenu(nullptr),
		mChildContextMenu(nullptr), mLayout(nullptr), mItemSample(nullptr), mSelectionDrawable(nullptr),
		mSeparatorSample(nullptr)
	{
		mItemSample = mnew UIWidget();
		mItemSample->AddLayer("icon", nullptr, Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(10, 0), Vec2F(10, 0)));
		mItemSample->AddLayer("subIcon", nullptr, Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-10, 0), Vec2F(-10, 0)));
		mItemSample->AddLayer("caption", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));
		mItemSample->AddLayer("shortcut", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));

		mSeparatorSample = mnew UIWidget();
		mSeparatorSample->layout.minHeight = 3.0f;
		mSeparatorSample->layout.height = 3.0f;

		mSelectionDrawable = mnew Sprite();

		mLayout = mnew UIVerticalLayout();
		AddChild(mLayout);

		mLayout->expandHeight = false;
		mLayout->expandWidth = true;
		mLayout->baseCorner = BaseCorner::LeftTop;
		mLayout->fitByChildren = true;
		mLayout->layout = UIWidgetLayout::BothStretch();
	}

	UIContextMenu::UIContextMenu(Vector<Item> items):
		UIContextMenu()
	{
		AddItems(items);
	}

	UIContextMenu::UIContextMenu(const UIContextMenu& other):
		UIScrollArea(other), mSelectedItem(-1), mPressedItem(-1), mSelectSubContextTime(-1), mParentContextMenu(nullptr),
		mChildContextMenu(nullptr), mLayout(nullptr), mItemSample(nullptr), mSelectionDrawable(nullptr)
	{
		mItemSample = other.mItemSample->Clone();
		mSeparatorSample = other.mSeparatorSample->Clone();
		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mSelectionLayout = other.mSelectionLayout;
		mLayout = FindChild<UIVerticalLayout>();

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UIContextMenu::~UIContextMenu()
	{
		delete mItemSample;
		delete mSelectionDrawable;
		delete mSeparatorSample;
	}

	UIContextMenu& UIContextMenu::operator=(const UIContextMenu& other)
	{
		UIScrollArea::operator=(other);

		delete mItemSample;
		delete mSelectionDrawable;
		delete mSeparatorSample;

		mItemSample = other.mItemSample->Clone();
		mSeparatorSample = other.mSeparatorSample->Clone();
		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mSelectionLayout = other.mSelectionLayout;
		mLayout = FindChild<UIVerticalLayout>();

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIContextMenu::Update(float dt)
	{
		if (mFullyDisabled)
			return;

		UIScrollArea::Update(dt);

		const float rectLerpCoef = 20.0f;
		if (mCurrentSelectionRect != mTargetSelectionRect)
		{
			mCurrentSelectionRect = Math::Lerp(mCurrentSelectionRect, mTargetSelectionRect, dt*rectLerpCoef);
			mSelectionDrawable->SetRect(mCurrentSelectionRect);
		}

		if (!mChildContextMenu && (o2Input.IsCursorPressed() || Math::Abs(o2Input.GetMouseWheelDelta()) > 0.1f) &&
			!layout.mAbsoluteRect.IsInside(o2Input.GetCursorPos()))
		{
			HideWithParent();
		}

		if (mSelectSubContextTime >= 0.0f)
		{
			mSelectSubContextTime -= dt;

			if (mSelectSubContextTime < 0.0f)
			{
				if (mChildContextMenu)
					mChildContextMenu->HideWithChild();

				if (mSelectedItem >= 0 && mLayout->mChilds[mSelectedItem]->FindChild<UIContextMenu>())
					mClickFunctions[mSelectedItem]();
			}
		}
	}

	void UIContextMenu::Draw()
	{}

	void UIContextMenu::Show(UIContextMenu* parent, const Vec2F& position /*= o2Input.GetCursorPos()*/)
	{
		if (parent)
		{
			mParentContextMenu = parent;
			parent->mChildContextMenu = this;
		}
		else mVisibleContextMenu = this;

		layout.mOffsetMin.x = position.x;
		layout.mOffsetMax.y = position.y;

		auto hoverState = state["hover"];
		if (hoverState)
		{
			mSelectionDrawable->SetEnabled(true);
			*hoverState = false;
		}
		else
			mSelectionDrawable->SetEnabled(false);

		FitSize();
		FitPosition();
		UpdateLayout();

		UIWidget::Show();
	}

	void UIContextMenu::Show(const Vec2F& position /*= o2Input.GetCursorPos()*/)
	{
		Show(nullptr, position);
	}

	UIWidget* UIContextMenu::AddItem(const Item& item)
	{
		UIWidget* newItem = CreateItem(item);
		mLayout->AddChild(newItem);

		if (item.subItems.Count() > 0)
		{
			mClickFunctions.Add([=]() {
				newItem->FindChild<UIContextMenu>()->Show(this, newItem->layout.absRightTop);
			});
		}
		else mClickFunctions.Add(item.onClick);

		return newItem;
	}

	UIWidget* UIContextMenu::AddItem(const WString& path, const Function<void()>& clickFunc /*= Function<void()>()*/)
	{
		UIContextMenu* targetContext = this;
		WString targetPath = path;

		while (true)
		{
			int slashPos = targetPath.Find("/");
			if (slashPos < 0)
				break;

			WString subMenu = targetPath.SubStr(0, slashPos);

			UIWidget* subChild = targetContext->mLayout->mChilds.FindMatch([&](auto x) {
				if (auto text = x->GetLayerDrawable<Text>("caption"))
					return text->text == subMenu;

				return false;
			});

			if (!subChild)
				subChild = AddItem(subMenu);

			UIContextMenu* subContext = subChild->FindChild<UIContextMenu>();
			if (!subContext)
			{
				subContext = Clone();
				subContext->RemoveAllItems();

				subChild->AddChild(subContext);
				int idx = targetContext->mLayout->mChilds.Find(subChild);

				mClickFunctions.Insert([=]() {
					subContext->Show(this, subChild->layout.absRightTop);
				}, idx);

				if (auto subIconLayer = subChild->GetLayer("subIcon"))
					subIconLayer->transparency = 1.0f;
			}

			targetContext = subContext;
			targetPath = targetPath.SubStr(slashPos + 1);
		}

		return targetContext->AddItem(Item(targetPath, clickFunc));
	}

	UIWidget* UIContextMenu::InsertItem(const Item& item, int position)
	{
		UIWidget* newItem = CreateItem(item);
		mLayout->AddChild(newItem, position);

		if (item.subItems.Count() > 0)
			mClickFunctions.Insert([=]() { newItem->FindChild<UIContextMenu>()->Show(this, newItem->layout.absRightBottom); }, position);
		else
			mClickFunctions.Insert(item.onClick, position);

		return newItem;
	}

	void UIContextMenu::AddItems(Vector<Item> items)
	{
		for (auto item : items)
			AddItem(item);
	}

	void UIContextMenu::InsertItems(Vector<Item> items, int position)
	{
		int i = 0;
		for (auto item : items)
		{
			InsertItem(item, position + i);
			i++;
		}
	}

	UIContextMenu::Item UIContextMenu::GetItem(int position)
	{
		if (position > 0 && position < mLayout->GetChilds().Count())
			return GetItemDef(position);

		return Item();
	}

	void UIContextMenu::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	UIWidget* UIContextMenu::GetItemUnderPoint(const Vec2F& point, int* idxPtr)
	{
		if (!mLayout)
			return nullptr;

		int idx = 0;
		for (auto child : mLayout->mChilds)
		{
			if (child->layout.mAbsoluteRect.IsInside(point))
			{
				if (idxPtr)
					*idxPtr = idx;

				return child;
			}

			idx++;
		}

		if (idxPtr)
			*idxPtr = -1;

		return nullptr;
	}

	void UIContextMenu::UpdateHover(const Vec2F& point)
	{
		int itemIdx = -1;
		UIWidget* itemUnderCursor = GetItemUnderPoint(point, &itemIdx);

		if (itemIdx < 0)
		{
			auto hoverState = state["hover"];
			if (hoverState)
			{
				mSelectionDrawable->SetEnabled(true);
				*hoverState = false;
			}
			else
				mSelectionDrawable->SetEnabled(false);

			mSelectedItem = itemIdx;
		}
		else
		{
			mTargetSelectionRect = mSelectionLayout.Calculate(itemUnderCursor->layout.mAbsoluteRect);

			auto hoverState = state["hover"];
			if (hoverState)
			{
				mSelectionDrawable->SetEnabled(true);
				*hoverState = true;
			}
			else
				mSelectionDrawable->SetEnabled(true);

			if (itemIdx != mSelectedItem)
			{
				mSelectSubContextTime = mOpenSubMenuDelay;
				mSelectedItem = itemIdx;
			}
		}
	}

	void UIContextMenu::OnCursorPressed(const Input::Cursor& cursor)
	{
		int itemIdx = -1;
		UIWidget* itemUnderCursor = GetItemUnderPoint(cursor.mPosition, &itemIdx);
		mPressedItem = itemIdx;
	}

	void UIContextMenu::OnCursorStillDown(const Input::Cursor& cursor)
	{}

	void UIContextMenu::OnCursorReleased(const Input::Cursor& cursor)
	{
		int itemIdx = -1;
		UIWidget* itemUnderCursor = GetItemUnderPoint(cursor.mPosition, &itemIdx);

		if (itemIdx >= 0)
			mClickFunctions[itemIdx]();

		mPressedItem = -1;

		if (itemUnderCursor && itemUnderCursor->FindChild<UIContextMenu>() == nullptr)
		{
			HideWithParent();
			HideWithChild();
		}
	}

	void UIContextMenu::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		mPressedItem = -1;
		HideWithParent();
		HideWithChild();
	}

	void UIContextMenu::OnCursorMoved(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.mPosition - mLastSelectCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastSelectCheckCursor = cursor.mPosition;

		UpdateHover(cursor.mPosition);
	}

	void UIContextMenu::HideWithParent()
	{
		Hide();

		if (mParentContextMenu)
			mParentContextMenu->HideWithParent();

		mParentContextMenu = nullptr;
	}

	void UIContextMenu::HideWithChild()
	{
		Hide();

		if (mChildContextMenu)
			mChildContextMenu->HideWithChild();

		mChildContextMenu = nullptr;
	}

	Vector<UIContextMenu::Item> UIContextMenu::GetItems() const
	{
		Vector<Item> res;
		for (int i = 0; i < mLayout->GetChilds().Count(); i++)
			res.Add(GetItemDef(i));

		return res;
	}

	void UIContextMenu::RemoveItem(int position)
	{
		if (position > 0 && position < mLayout->GetChilds().Count())
			mLayout->RemoveChild(mLayout->GetChilds()[position]);
	}

	void UIContextMenu::RemoveItem(const WString& path)
	{
		UIContextMenu* targetContext = this;
		WString targetPath = path;

		while (true)
		{
			int slashPos = targetPath.Find("/");
			if (slashPos < 0)
				break;

			WString subMenu = targetPath.SubStr(0, slashPos);

			UIWidget* subChild = targetContext->mLayout->mChilds.FindMatch([&](auto x) {
				if (auto text = x->GetLayerDrawable<Text>("caption"))
					return text->text == subMenu;

				return false;
			});

			if (!subChild)
			{
				o2Debug.LogError("Failed to remove context item %s", path);
				return;
			}

			UIContextMenu* subContext = subChild->FindChild<UIContextMenu>();
			if (!subContext)
			{
				o2Debug.LogError("Failed to remove context item %s", path);
				return;
			}

			targetContext = subContext;
			targetPath = targetPath.SubStr(slashPos + 1);
		}

		UIWidget* removingItem = targetContext->mLayout->mChilds.FindMatch([&](auto x) {
			if (auto text = x->GetLayerDrawable<Text>("caption"))
				return text->text == targetPath;

			return false;
		});

		if (!removingItem)
		{
			o2Debug.LogError("Failed to remove context item %s", path);
			return;
		}

		targetContext->mLayout->RemoveChild(removingItem);
	}

	void UIContextMenu::RemoveAllItems()
	{
		mLayout->RemoveAllChilds();
	}

	UIVerticalLayout* UIContextMenu::GetItemsLayout() const
	{
		return mLayout;
	}

	UIWidget* UIContextMenu::GetItemSample() const
	{
		return mItemSample;
	}

	UIWidget* UIContextMenu::GetSeparatorSample() const
	{
		return mSeparatorSample;
	}

	Sprite* UIContextMenu::GetSelectionDrawable() const
	{
		return mSelectionDrawable;
	}

	void UIContextMenu::SetSelectionDrawableLayout(const Layout& layout)
	{
		mSelectionLayout = layout;
	}

	Layout UIContextMenu::GetSelectionDrawableLayout() const
	{
		return mSelectionLayout;
	}

	bool UIContextMenu::IsUnderPoint(const Vec2F& point)
	{
		return layout.mAbsoluteRect.IsInside(point);
	}

	float UIContextMenu::Depth()
	{
		return GetMaxDrawingDepth();
	}

	bool UIContextMenu::IsScrollable() const
	{
		return true;
	}

	UIContextMenu* UIContextMenu::mVisibleContextMenu = nullptr;

	void UIContextMenu::UpdateLayout(bool forcible /*= false*/)
	{
		layout.mLocalRect.left = layout.mOffsetMin.x;
		layout.mLocalRect.right = layout.mOffsetMax.x;
		layout.mLocalRect.bottom = layout.mOffsetMin.y;
		layout.mLocalRect.top = layout.mOffsetMax.y;
		layout.mAbsoluteRect = layout.mLocalRect;

		UpdateLayersLayouts();

		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout.mAbsoluteRect);
		mAbsoluteClipArea = mClipAreaLayout.Calculate(layout.mAbsoluteRect);
		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));

		mChildsAbsRect = mAbsoluteViewArea + roundedScrollPos;

		for (auto child : mChilds)
			child->UpdateLayout(true);

		UpdateScrollParams();

		RectF _mChildsAbsRect = mChildsAbsRect;
		mChildsAbsRect = layout.mAbsoluteRect;

		if (mOwnHorScrollBar)
			mHorScrollBar->UpdateLayout(true);

		if (mOwnVerScrollBar)
			mVerScrollBar->UpdateLayout(true);

		mChildsAbsRect = _mChildsAbsRect;
	}

	void UIContextMenu::FitSize()
	{
		Vec2F size;
		for (auto child : mLayout->GetChilds())
		{
			auto childCaption = child->GetLayerDrawable<Text>("caption");

			if (!childCaption)
				continue;

			size.x = Math::Max(size.x, childCaption->GetRealSize().x + 39.0f);
			size.y += child->layout.height;
		}

		size.y += layout.height - mAbsoluteViewArea.Height();

		size.x = Math::Min(size.x, (float)o2Render.resolution->x);
		size.y = Math::Min(size.y, (float)o2Render.resolution->y);

		layout.mOffsetMax.x = layout.mOffsetMin.x + size.x;
		layout.mOffsetMin.y = layout.mOffsetMax.y - size.y;
	}

	void UIContextMenu::FitPosition()
	{
		RectF thisRect(layout.mOffsetMin, layout.mOffsetMax);
		RectF screenRect(-o2Render.resolution->x*0.5f, o2Render.resolution->y*0.5f,
						 o2Render.resolution->x*0.5f, -o2Render.resolution->y*0.5f);

		Vec2F offs;

		if (thisRect.left < screenRect.left)
			offs.x = screenRect.left - thisRect.left;

		if (thisRect.right > screenRect.right)
			offs.x = screenRect.right - thisRect.right;

		if (thisRect.top > screenRect.top)
			offs.y = screenRect.top - thisRect.top;

		if (thisRect.bottom < screenRect.bottom)
			offs.y = screenRect.bottom - thisRect.bottom;

		layout.mOffsetMax += offs;
		layout.mOffsetMin += offs;
	}

	void UIContextMenu::SpecialDraw()
	{
		if (mFullyDisabled)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		for (auto child : mChilds)
			child->Draw();

		mSelectionDrawable->Draw();

		o2Render.DisableScissorTest();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (mOwnHorScrollBar)
			mHorScrollBar->Draw();

		if (mOwnVerScrollBar)
			mVerScrollBar->Draw();

		if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
			DrawDebugFrame();

		if (mChildContextMenu)
			mChildContextMenu->SpecialDraw();
	}

	UIWidget* UIContextMenu::CreateItem(const Item& item)
	{

		if (item.text == "---") // is separator
		{
			UIWidget* newItem = mSeparatorSample->Clone();
			newItem->name = "Separator";

			return newItem;
		}

		UIWidget* newItem = mItemSample->Clone();
		newItem->name = (WString)"Context Item " + item.text;

		if (auto iconLayer = newItem->GetLayer("icon"))
		{
			if (item.icon)
			{
				Vec2F size = item.icon->GetAtlasRect().Size();

				if (size.x > size.y)
				{
					size.y *= size.x / newItem->layout.height;
					size.x = newItem->layout.height;
				}
				else
				{
					size.x *= size.y / newItem->layout.height;
					size.y = newItem->layout.height;
				}

				iconLayer->AddChildLayer("sprite", mnew Sprite(item.icon),
										 Layout(Vec2F(), Vec2F(),
												Vec2F(-Math::Floor(size.x*0.5f), Math::Floor(size.y*0.5f)),
												Vec2F(Math::Floor(size.x*0.5f), -Math::Floor(size.y*0.5f))));

				newItem->UpdateLayersDrawingSequence();
			}
		}

		if (auto textLayer = newItem->GetLayerDrawable<Text>("caption"))
			textLayer->text = item.text;

		if (auto shortcutLayer = newItem->GetLayerDrawable<Text>("shortcut"))
			shortcutLayer->text = item.shortcut.AsString();

		if (auto subIconLayer = newItem->GetLayer("subIcon"))
			subIconLayer->transparency = item.subItems.Count() > 0 ? 1.0f : 0.0f;

		if (item.subItems.Count() > 0)
		{
			UIContextMenu* subMenu = mnew UIContextMenu(*this);
			subMenu->RemoveAllItems();
			subMenu->AddItems(item.subItems);

			newItem->AddChild(subMenu);
		}

		return newItem;
	}

	UIContextMenu::Item UIContextMenu::GetItemDef(int idx) const
	{
		Item res;
		auto item = mLayout->mChilds[idx];

		if (item->name == "Separator")
		{
			res.text = "---";
		}
		else
		{
			if (auto iconLayer = item->GetLayerDrawable<Sprite>("icon"))
				res.icon = mnew ImageAsset(iconLayer->imageAssetId);

			if (auto textLayer = item->GetLayerDrawable<Text>("caption"))
				res.text = textLayer->text;

			/*if (auto shortcutLayer = item->GetLayerDrawable<Text>("shortcut"))
				res.shortcut = shortcutLayer->text;*/

			if (auto subMenu = item->FindChild<UIContextMenu>())
				res.subItems = subMenu->GetItems();

			res.onClick = mClickFunctions[idx];
		}

		return res;
	}
}