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
							  const ShortcutKeys& shortcut /*= ShortcutKeys()*/) :
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
		UIScrollArea(), DrawableCursorEventsListener(this)
	{
		mItemSample = mnew UIContextMenuItem();
		mItemSample->layout.minHeight = 20.0f;
		mItemSample->AddLayer("icon", nullptr, Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(10, 0), Vec2F(10, 0)));
		mItemSample->AddLayer("subIcon", nullptr, Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-10, 0), Vec2F(-10, 0)));
		mItemSample->AddLayer("caption", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));
		mItemSample->AddLayer("shortcut", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));

		mSeparatorSample = mnew UIWidget();
		mSeparatorSample->layout.minHeight = 3.0f;
		mSeparatorSample->layout.height = 3.0f;

		mSelectionDrawable = mnew Sprite();

		mItemsLayout = mnew UIVerticalLayout();
		AddChild(mItemsLayout);

		mItemsLayout->expandHeight  = false;
		mItemsLayout->expandWidth   = true;
		mItemsLayout->baseCorner    = BaseCorner::LeftTop;
		mItemsLayout->fitByChildren = true;
		mItemsLayout->layout        = UIWidgetLayout::BothStretch();

		SetVisibleForcible(false);
	}

	UIContextMenu::UIContextMenu(Vector<Item> items):
		UIContextMenu()
	{
		AddItems(items);
	}

	UIContextMenu::UIContextMenu(const UIContextMenu& other):
		UIScrollArea(other), DrawableCursorEventsListener(this), mMaxVisibleItems(other.mMaxVisibleItems)
	{
		mItemSample        = other.mItemSample->Clone();
		mSeparatorSample   = other.mSeparatorSample->Clone();
		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mSelectionLayout   = other.mSelectionLayout;
		mItemsLayout       = FindChild<UIVerticalLayout>();

		RetargetStatesAnimations();
		UpdateLayout();

		SetVisibleForcible(false);
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

		mItemSample        = other.mItemSample->Clone();
		mSeparatorSample   = other.mSeparatorSample->Clone();
		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mSelectionLayout   = other.mSelectionLayout;
		mItemsLayout       = FindChild<UIVerticalLayout>();
		mMaxVisibleItems   = other.mMaxVisibleItems;

		mFitSizeMin = other.mFitSizeMin;

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
			!layout.mAbsoluteRect.IsInside(o2Input.GetCursorPos()) && !mShownAtFrame && mVisible)
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

				if (mSelectedItem && mSelectedItem->GetSubMenu())
					mSelectedItem->GetSubMenu()->Show(this, mSelectedItem->layout.absRightTop);
			}
		}

		mShownAtFrame = false;
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

		layout.mOffsetMin.x = Math::Round(position.x);
		layout.mOffsetMax.y = Math::Round(position.y);

		auto hoverState = state["hover"];
		if (hoverState)
		{
			mSelectionDrawable->SetEnabled(true);
			*hoverState = false;
		}
		else mSelectionDrawable->SetEnabled(false);

		FitSize();
		FitPosition();
		UpdateLayout();
		UIWidget::Show();

		mShownAtFrame = true;
	}

	void UIContextMenu::Show(const Vec2F& position /*= o2Input.GetCursorPos()*/)
	{
		Show(nullptr, position);
	}

	UIWidget* UIContextMenu::AddItem(const Item& item)
	{
		if (item.text == "---")
		{
			UIWidget* newItem = mSeparatorSample->Clone();
			newItem->name = "Separator";
			mItemsLayout->AddChild(newItem);

			return newItem;
		}

		UIContextMenuItem* newItem = CreateItem(item);
		mItemsLayout->AddChild(newItem);
		newItem->onClick = item.onClick;

		FitSize();
		FitPosition();
		UpdateLayout();

		return newItem;
	}

	UIWidget* UIContextMenu::AddItem(const WString& path, const Function<void()>& clickFunc /*= Function<void()>()*/,
									 ImageAsset* icon /*= nullptr*/, const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		UIContextMenu* targetContext = this;
		WString targetPath = path;

		while (true)
		{
			int slashPos = targetPath.Find("/");
			if (slashPos < 0)
				break;

			WString subMenu = targetPath.SubStr(0, slashPos);

			UIWidget* subChild = targetContext->mItemsLayout->mChilds.FindMatch([&](auto x) {
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

				if (auto subIconLayer = subChild->GetLayer("subIcon"))
					subIconLayer->transparency = 1.0f;
			}

			targetContext = subContext;
			targetPath = targetPath.SubStr(slashPos + 1);
		}

		return targetContext->AddItem(Item(targetPath, clickFunc, icon, shortcut));
	}

	UIWidget* UIContextMenu::InsertItem(const Item& item, int position)
	{
		if (item.text == "---")
		{
			UIWidget* newItem = mSeparatorSample->Clone();
			newItem->name = "Separator";
			mItemsLayout->AddChild(newItem, position);

			return newItem;
		}

		UIContextMenuItem* newItem = CreateItem(item);
		mItemsLayout->AddChild(newItem, position);

		if (item.subItems.Count() == 0)
			newItem->onClick = item.onClick;

		FitSize();
		FitPosition();
		UpdateLayout();

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
		if (position > 0 && position < mItemsLayout->GetChilds().Count())
			return GetItemDef(position);

		return Item();
	}

	void UIContextMenu::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	UIContextMenuItem* UIContextMenu::GetItemUnderPoint(const Vec2F& point)
	{
		if (!mItemsLayout)
			return nullptr;

		for (auto child : mItemsLayout->mChilds)
		{
			if (child->layout.mAbsoluteRect.IsInside(point) && child->GetType() == UIContextMenuItem::type)
				return (UIContextMenuItem*)child;
		}

		return nullptr;
	}

	void UIContextMenu::UpdateHover(const Vec2F& point)
	{
		UIContextMenuItem* itemUnderCursor = GetItemUnderPoint(point);

		if (!itemUnderCursor)
		{
			auto hoverState = state["hover"];
			if (hoverState)
			{
				mSelectionDrawable->SetEnabled(true);
				*hoverState = false;
			}
			else
				mSelectionDrawable->SetEnabled(false);

			mSelectedItem = itemUnderCursor;
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

			if (itemUnderCursor != mSelectedItem)
			{
				mSelectSubContextTime = mOpenSubMenuDelay;
				mSelectedItem = itemUnderCursor;
			}
		}
	}

	void UIContextMenu::OnCursorPressed(const Input::Cursor& cursor)
	{
	}

	void UIContextMenu::OnCursorStillDown(const Input::Cursor& cursor)
	{}

	void UIContextMenu::OnCursorReleased(const Input::Cursor& cursor)
	{
		UIContextMenuItem* itemUnderCursor = GetItemUnderPoint(cursor.position);

		if (itemUnderCursor)
			itemUnderCursor->onClick();

		if (itemUnderCursor && itemUnderCursor->FindChild<UIContextMenu>() == nullptr)
		{
			HideWithParent();
			HideWithChild();
		}
	}

	void UIContextMenu::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		HideWithParent();
		HideWithChild();
	}

	void UIContextMenu::OnCursorMoved(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastSelectCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastSelectCheckCursor = cursor.position;

		UpdateHover(cursor.position);
	}

	void UIContextMenu::OnKeyPressed(const Input::Key& key)
	{
		if (mVisibleContextMenu && mVisibleContextMenu->IsVisible() && mVisibleContextMenu != this)
			return;

		for (auto child : mItemsLayout->mChilds)
		{
			if (child->GetType() == UIContextMenuItem::type)
			{
				auto item = (UIContextMenuItem*)child;

				if (item->shortcut.IsPressed())
				{
					item->onClick();
					break;
				}
			}
		}
	}

	void UIContextMenu::HideWithParent()
	{
		Hide();

		if (mParentContextMenu)
			mParentContextMenu->HideWithParent();

		mParentContextMenu = nullptr;
		mChildContextMenu = nullptr;
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
		for (int i = 0; i < mItemsLayout->GetChilds().Count(); i++)
			res.Add(GetItemDef(i));

		return res;
	}

	void UIContextMenu::RemoveItem(int position)
	{
		if (position > 0 && position < mItemsLayout->GetChilds().Count())
			mItemsLayout->RemoveChild(mItemsLayout->GetChilds()[position]);
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

			UIWidget* subChild = targetContext->mItemsLayout->mChilds.FindMatch([&](auto x) {
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

		UIWidget* removingItem = targetContext->mItemsLayout->mChilds.FindMatch([&](auto x) {
			if (auto text = x->GetLayerDrawable<Text>("caption"))
				return text->text == targetPath;

			return false;
		});

		if (!removingItem)
		{
			o2Debug.LogError("Failed to remove context item %s", path);
			return;
		}

		targetContext->mItemsLayout->RemoveChild(removingItem);
	}

	void UIContextMenu::RemoveAllItems()
	{
		mItemsLayout->RemoveAllChilds();
		mSelectedItem = nullptr;
	}

	UIVerticalLayout* UIContextMenu::GetItemsLayout() const
	{
		return mItemsLayout;
	}

	UIContextMenuItem* UIContextMenu::GetItemSample() const
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

	void UIContextMenu::SetMinFitSize(float size)
	{
		mFitSizeMin = size;
	}

	void UIContextMenu::SetMaxItemsVisible(int count)
	{
		mMaxVisibleItems = count;
	}

	bool UIContextMenu::IsScrollable() const
	{
		return true;
	}

	UIContextMenu* UIContextMenu::mVisibleContextMenu = nullptr;

	void UIContextMenu::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		layout.mLocalRect.left   = layout.mOffsetMin.x;
		layout.mLocalRect.right  = layout.mOffsetMax.x;
		layout.mLocalRect.bottom = layout.mOffsetMin.y;
		layout.mLocalRect.top    = layout.mOffsetMax.y;
		layout.mAbsoluteRect     = layout.mLocalRect;

		UpdateLayersLayouts();

		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout.mAbsoluteRect);
		mAbsoluteClipArea = mClipAreaLayout.Calculate(layout.mAbsoluteRect);
		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));

		mChildsAbsRect = mAbsoluteViewArea + roundedScrollPos;

		if (withChildren)
			UpdateChildrenLayouts(true);

		UpdateScrollParams();

		RectF _mChildsAbsRect = mChildsAbsRect;
		mChildsAbsRect = layout.mAbsoluteRect;

		if (mOwnHorScrollBar)
			mHorScrollBar->UpdateLayout(true);

		if (mOwnVerScrollBar)
			mVerScrollBar->UpdateLayout(true);

		mChildsAbsRect = _mChildsAbsRect;
	}

	void UIContextMenu::CheckClipping(const RectF& clipArea)
	{
		mIsClipped = false;

		Vec2F resolution = o2Render.GetCurrentResolution();
		RectF fullScreenRect(resolution*0.5f, resolution*(-0.5f));
		for (auto child : mChilds)
			child->CheckClipping(fullScreenRect);
	}

	void UIContextMenu::FitSize()
	{
		Vec2F size;
		float maxCaption = 0.0f;
		float maxShortcut = 0.0f;

		int i = 0;
		for (auto child : mItemsLayout->GetChilds())
		{
			if (auto childCaption = child->GetLayerDrawable<Text>("caption"))
				maxCaption = Math::Max(childCaption->GetRealSize().x, maxCaption);

			if (auto shortcutCaption = child->GetLayerDrawable<Text>("shortcut"))
				maxShortcut = Math::Max(shortcutCaption->GetRealSize().x, maxShortcut);

			size.y += child->layout.minHeight;

			i++;
			if (i == mMaxVisibleItems)
				break;
		}

		size.x = mFitSizeMin + maxCaption + maxShortcut;
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

		offs.x = Math::Round(offs.x);
		offs.y = Math::Round(offs.y);

		layout.mOffsetMax += offs;
		layout.mOffsetMin += offs;
	}

	void UIContextMenu::SpecialDraw()
	{
		if (mFullyDisabled)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();

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

		DrawDebugFrame();

		if (mChildContextMenu)
			mChildContextMenu->SpecialDraw();
	}

	UIContextMenuItem* UIContextMenu::CreateItem(const Item& item)
	{
		UIContextMenuItem* newItem = mItemSample->Clone();
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

				iconLayer->AddChildLayer("sprite", mnew Sprite(*item.icon),
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

		newItem->shortcut = item.shortcut;

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
		auto item = mItemsLayout->mChilds[idx];

		if (item->name == "Separator")
		{
			res.text = "---";
		}
		else
		{
			auto contextItem = (UIContextMenuItem*)item;
			if (auto iconLayer = contextItem->GetLayerDrawable<Sprite>("icon"))
				res.icon = mnew ImageAsset(iconLayer->imageAssetId);

			if (auto textLayer = contextItem->GetLayerDrawable<Text>("caption"))
				res.text = textLayer->text;

			if (auto shortcutLayer = contextItem->GetLayerDrawable<Text>("shortcut"))
				res.shortcut = contextItem->shortcut;

			if (auto subMenu = contextItem->FindChild<UIContextMenu>())
				res.subItems = subMenu->GetItems();

			res.onClick = contextItem->onClick;
		}

		return res;
	}

	UIContextMenuItem::UIContextMenuItem():
		UIWidget(), mSubMenu(nullptr)
	{
		RetargetStatesAnimations();
	}

	UIContextMenuItem::UIContextMenuItem(const UIContextMenuItem& other):
		UIWidget(other)
	{
		mSubMenu = FindChild<UIContextMenu>();
		if (mSubMenu) mSubMenu->Hide(true);

		RetargetStatesAnimations();
	}

	UIContextMenuItem::~UIContextMenuItem()
	{}

	UIContextMenuItem& UIContextMenuItem::operator=(const UIContextMenuItem& other)
	{
		UIWidget::operator =(other);
		mSubMenu = FindChild<UIContextMenu>();
		if (mSubMenu) mSubMenu->Hide(true);

		return *this;
	}

	UIContextMenu* UIContextMenuItem::GetSubMenu() const
	{
		return mSubMenu;
	}

	void UIContextMenuItem::OnChildAdded(UIWidget* child)
	{
		if (child->GetType() == TypeOf(UIContextMenu))
			mSubMenu = (UIContextMenu*)child;
	}

}