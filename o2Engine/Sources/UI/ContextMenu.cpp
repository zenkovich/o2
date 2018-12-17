#include "stdafx.h"
#include "ContextMenu.h"

#include "Assets/ImageAsset.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace o2
{
	bool UIContextMenu::Item::operator==(const Item& other) const
	{
		return text == other.text && shortcut == other.shortcut && icon == other.icon;
	}

	UIContextMenu::Item::Item():
		checked(false), checkable(false)
	{}

	UIContextMenu::Item::Item(const WString& text, const Function<void()> onClick, 
							  const ImageAssetRef& icon /*= ImageAssetRef()*/,
							  const ShortcutKeys& shortcut /*= ShortcutKeys()*/) :
		text(text), onClick(onClick), shortcut(shortcut), icon(icon), checked(false), checkable(false)
	{}

	UIContextMenu::Item::Item(const WString& text, Vector<Item> subItems, 
							  const ImageAssetRef& icon /*= ImageAssetRef()*/) :
		text(text), subItems(subItems), icon(icon), checked(false), checkable(false)
	{}

	UIContextMenu::Item::Item(const WString& text, bool checked, 
							  Function<void(bool)> onChecked /*= Function<void(bool)>()*/, 
							  const ImageAssetRef& icon /*= ImageAssetRef()*/,
							  const ShortcutKeys& shortcut /*= ShortcutKeys()*/) :
		text(text), checked(checked), onChecked(onChecked), checkable(true), shortcut(shortcut), icon(icon)
	{}

	UIContextMenu::Item::~Item()
	{}

	UIContextMenu::Item UIContextMenu::Item::Separator()
	{
		return Item("---", Function<void()>());
	}

	UIContextMenu::UIContextMenu():
		UIScrollArea(), DrawableCursorEventsListener(this)
	{
		mItemSample = mnew UIContextMenuItem();
		mItemSample->ExcludeFromScene();
		mItemSample->layout->minHeight = 20.0f;
		mItemSample->AddLayer("icon", nullptr, Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(10, 0), Vec2F(10, 0)));
		mItemSample->AddLayer("subIcon", nullptr, Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-10, 0), Vec2F(-10, 0)));
		mItemSample->AddLayer("caption", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));
		mItemSample->AddLayer("shortcut", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));

		mSeparatorSample = mnew UIWidget();
		mSeparatorSample->ExcludeFromScene();
		mSeparatorSample->layout->minHeight = 3.0f;
		mSeparatorSample->layout->height = 3.0f;

		mSelectionDrawable = mnew Sprite();

		mItemsLayout = mnew UIVerticalLayout();
		AddChild(mItemsLayout);

		mItemsLayout->expandHeight  = false;
		mItemsLayout->expandWidth   = true;
		mItemsLayout->baseCorner    = BaseCorner::LeftTop;
		mItemsLayout->fitByChildren = true;
		*mItemsLayout->layout       = UIWidgetLayout::BothStretch();

		SetEnableForcible(false);
	}

	UIContextMenu::UIContextMenu(Vector<Item> items):
		UIContextMenu()
	{
		AddItems(items);
	}

	UIContextMenu::UIContextMenu(const UIContextMenu& other):
		UIScrollArea(other), DrawableCursorEventsListener(this), mMaxVisibleItems(other.mMaxVisibleItems)
	{
		mItemSample        = other.mItemSample->CloneAs<UIContextMenuItem>();
		mItemSample->ExcludeFromScene();

		mSeparatorSample   = other.mSeparatorSample->CloneAs<UIWidget>();
		mSeparatorSample->ExcludeFromScene();

		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mSelectionLayout   = other.mSelectionLayout;
		mItemsLayout       = FindChildByType<UIVerticalLayout>();

		RetargetStatesAnimations();
		SetEnableForcible(false);
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
		return *this;
	}

	void UIContextMenu::Update(float dt)
	{
		if (!mResEnabledInHierarchy)
			return;

		UIScrollArea::Update(dt);

		const float rectLerpCoef = 20.0f;
		if (mCurrentSelectionRect != mTargetSelectionRect)
		{
			mCurrentSelectionRect = Math::Lerp(mCurrentSelectionRect, mTargetSelectionRect, dt*rectLerpCoef);
			mSelectionDrawable->SetRect(mCurrentSelectionRect);
		}

		if (o2Input.IsCursorPressed())
		{
			if (!mChildContextMenu && (o2Input.IsCursorPressed() || Math::Abs(o2Input.GetMouseWheelDelta()) > 0.1f) &&
				!layout->IsPointInside(o2Input.GetCursorPos()) && !mShownAtFrame && mEnabled)
			{
				HideWithParent();
			}
		}

		if (mSelectSubContextTime >= 0.0f)
		{
			mSelectSubContextTime -= dt;

			if (mSelectSubContextTime < 0.0f && GetItemUnderPoint(o2Input.GetCursorPos()) == mSelectedItem)
			{
				if (mChildContextMenu)
					mChildContextMenu->HideWithChild();

				mChildContextMenu = nullptr;

				if (mSelectedItem && mSelectedItem->GetSubMenu())
					mSelectedItem->GetSubMenu()->Show(this, mSelectedItem->layout->worldRightTop);
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

		auto hoverState = state["hover"];
		if (hoverState)
		{
			mSelectionDrawable->SetEnabled(true);
			*hoverState = false;
		}
		else mSelectionDrawable->SetEnabled(false);

		FitSizeAndPosition(position);
		UpdateTransform();
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
			UIWidget* newItem = mSeparatorSample->CloneAs<UIWidget>();
			newItem->name = "Separator";
			mItemsLayout->AddChild(newItem);

			return newItem;
		}

		UIContextMenuItem* newItem = CreateItem(item);
		mItemsLayout->AddChild(newItem);
		newItem->onClick = item.onClick;

		FitSizeAndPosition(layout->worldLeftTop);
		SetLayoutDirty();

		return newItem;
	}

	UIWidget* UIContextMenu::AddItem(const WString& path, 
									 const Function<void()>& clickFunc /*= Function<void()>()*/,
									 const ImageAssetRef& icon /*= ImageAssetRef()*/, 
									 const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		WString targetPath = path;
		UIContextMenu* targetContext = CreateItemsByPath(targetPath);
		return targetContext->AddItem(Item(targetPath, clickFunc, icon, shortcut));
	}

	UIWidget* UIContextMenu::AddToggleItem(const WString& path, bool value,
										  const Function<void(bool)>& clickFunc /*= Function<void(bool)>()*/,
										  const ImageAssetRef& icon /*= ImageAssetRef()*/, 
										  const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		WString targetPath = path;
		UIContextMenu* targetContext = CreateItemsByPath(targetPath);
		return targetContext->AddItem(Item(targetPath, value, clickFunc, icon, shortcut));
	}

	UIContextMenu* UIContextMenu::CreateItemsByPath(WString& path)
	{
		UIContextMenu* resultContext = this;

		while (true)
		{
			int slashPos = path.Find("/");
			if (slashPos < 0)
				break;

			WString subMenu = path.SubStr(0, slashPos);

			UIWidget* subChild = resultContext->mItemsLayout->mChildWidgets.FindMatch([&](auto x) {
				if (auto text = x->template GetLayerDrawable<Text>("caption"))
					return text->text == subMenu;

				return false;
			});

			if (!subChild)
				subChild = resultContext->AddItem(subMenu);

			UIContextMenu* subContext = subChild->FindChildByType<UIContextMenu>();
			if (!subContext)
			{
				subContext = o2UI.CreateWidget<UIContextMenu>();
				subChild->AddChild(subContext);

				if (auto subIconLayer = subChild->FindLayer("subIcon"))
					subIconLayer->transparency = 1.0f;
			}

			resultContext = subContext;
			path = path.SubStr(slashPos + 1);
		}

		return resultContext;
	}

	UIWidget* UIContextMenu::InsertItem(const Item& item, int position)
	{
		if (item.text == "---")
		{
			UIWidget* newItem = mSeparatorSample->CloneAs<UIWidget>();
			newItem->name = "Separator";
			mItemsLayout->AddChild(newItem, position);

			return newItem;
		}

		UIContextMenuItem* newItem = CreateItem(item);
		mItemsLayout->AddChild(newItem, position);

		if (item.subItems.Count() == 0)
			newItem->onClick = item.onClick;

		FitSizeAndPosition(layout->worldLeftTop);
		SetLayoutDirty();

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
		if (position > 0 && position < mItemsLayout->GetChildren().Count())
			return GetItemDef(position);

		return Item();
	}

	int UIContextMenu::FindItem(const WString& text) const
	{
		int idx = 0;
		for (auto child : mItemsLayout->GetChildWidgets())
		{
			auto item = dynamic_cast<UIContextMenuItem*>(child);
			if (item && item->GetText() == text)
				return idx;

			idx++;
		}

		return -1;
	}

	void UIContextMenu::SetItem(int position, const Item& item)
	{
		if (position < 0 || position >= mItemsLayout->mChildren.Count())
			return;

		UIContextMenuItem* itemWidget = (UIContextMenuItem*)(mItemsLayout->mChildren[position]);
		SetupItem(itemWidget, item);
	}

	void UIContextMenu::OnResEnableInHierarchyChanged()
	{
		interactable = mResEnabled;
	}

	UIContextMenuItem* UIContextMenu::GetItemUnderPoint(const Vec2F& point)
	{
		if (!mItemsLayout)
			return nullptr;

		for (auto child : mItemsLayout->mChildWidgets)
		{
			if (child->layout->IsPointInside(point) && child->GetType() == TypeOf(UIContextMenuItem))
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
			mTargetSelectionRect = mSelectionLayout.Calculate(itemUnderCursor->layout->mData->worldRectangle);

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
				mSelectedItem = itemUnderCursor;

				if (mSelectedItem && mSelectedItem->GetSubMenu())
					mSelectSubContextTime = mOpenSubMenuDelay;
			}
		}
	}

	void UIContextMenu::OnCursorPressed(const Input::Cursor& cursor)
	{}

	void UIContextMenu::OnCursorStillDown(const Input::Cursor& cursor)
	{}

	void UIContextMenu::OnCursorReleased(const Input::Cursor& cursor)
	{
		UIContextMenuItem* itemUnderCursor = GetItemUnderPoint(cursor.position);

		if (itemUnderCursor)
		{
			itemUnderCursor->onClick();

			if (itemUnderCursor->IsCheckable())
			{
				itemUnderCursor->SetChecked(!itemUnderCursor->IsChecked());
				itemUnderCursor->onChecked(itemUnderCursor->IsChecked());
			}
		}

		if (itemUnderCursor && itemUnderCursor->FindChildByType<UIContextMenu>() == nullptr)
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
// 		if (mVisibleContextMenu && mVisibleContextMenu->IsVisible() && mVisibleContextMenu != this)
// 			return;
// 
// 		for (auto child : mItemsLayout->mChildren)
// 		{
// 			if (child->GetType() == TypeOf(UIContextMenuItem))
// 			{
// 				auto item = (UIContextMenuItem*)child;
// 
// 				if (item->shortcut.IsPressed())
// 				{
// 					item->onClick();
// 					break;
// 				}
// 			}
// 		}
	}

	void UIContextMenu::HideWithParent()
	{
		Hide();

		if (mParentContextMenu)
			mParentContextMenu->HideWithParent();

		mParentContextMenu = nullptr;
		mChildContextMenu = nullptr;
		mSelectedItem = nullptr;
	}

	void UIContextMenu::HideWithChild()
	{
		Hide();

		if (mChildContextMenu)
			mChildContextMenu->HideWithChild();

		mChildContextMenu = nullptr;
		mSelectedItem = nullptr;
	}

	Vector<UIContextMenu::Item> UIContextMenu::GetItems() const
	{
		Vector<Item> res;
		for (int i = 0; i < mItemsLayout->GetChildren().Count(); i++)
			res.Add(GetItemDef(i));

		return res;
	}

	void UIContextMenu::RemoveItem(int position)
	{
		if (position > 0 && position < mItemsLayout->GetChildren().Count())
			mItemsLayout->RemoveChild(mItemsLayout->GetChildren()[position]);
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

			UIWidget* subChild = targetContext->mItemsLayout->mChildWidgets.FindMatch([&](auto x) {
				if (auto text = x->template GetLayerDrawable<Text>("caption"))
					return text->text == subMenu;

				return false;
			});

			if (!subChild)
			{
				o2Debug.LogError("Failed to remove context item " + path);
				return;
			}

			UIContextMenu* subContext = subChild->FindChildByType<UIContextMenu>();
			if (!subContext)
			{
				o2Debug.LogError("Failed to remove context item " + path);
				return;
			}

			targetContext = subContext;
			targetPath = targetPath.SubStr(slashPos + 1);
		}

		UIWidget* removingItem = targetContext->mItemsLayout->mChildWidgets.FindMatch([&](auto x) {
			if (auto text = x->template GetLayerDrawable<Text>("caption"))
				return text->text == targetPath;

			return false;
		});

		if (!removingItem)
		{
			o2Debug.LogError("Failed to remove context item " + path);
			return;
		}

		targetContext->mItemsLayout->RemoveChild(removingItem);
	}

	void UIContextMenu::RemoveAllItems()
	{
		mItemsLayout->RemoveAllChildren();
		mSelectedItem = nullptr;
	}

	void UIContextMenu::SetItemChecked(int position, bool checked)
	{
		if (position < 0 || position >= mItemsLayout->mChildren.Count())
			return;

		UIContextMenuItem* item = (UIContextMenuItem*)mItemsLayout->mChildren[position];
		if (item->IsCheckable())
			item->SetChecked(checked);
	}

	bool UIContextMenu::IsItemChecked(int position) const
	{
		if (position < 0 || position >= mItemsLayout->mChildren.Count())
			return false;

		UIContextMenuItem* item = (UIContextMenuItem*)mItemsLayout->mChildren[position];
		return item->IsChecked();
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

	void UIContextMenu::SetItemsMaxPriority()
	{
		for (auto child : mItemsLayout->mChildren)
		{
			UIContextMenuItem* item = dynamic_cast<UIContextMenuItem*>(child);
			if (item)
				item->SetMaxPriority();
		}
	}

	void UIContextMenu::SetItemsMinPriority()
	{
		for (auto child : mItemsLayout->mChildren)
		{
			UIContextMenuItem* item = dynamic_cast<UIContextMenuItem*>(child);
			if (item)
				item->SetMinPriority();
		}
	}

	bool UIContextMenu::IsScrollable() const
	{
		return true;
	}

	UIContextMenu* UIContextMenu::mVisibleContextMenu = nullptr;

	void UIContextMenu::UpdateTransform(bool withChildren /*= true*/)
	{
		layout->Update();

		mAbsoluteViewArea = mViewAreaLayout.Calculate(layout->mData->worldRectangle);
		mAbsoluteClipArea = mClipAreaLayout.Calculate(layout->mData->worldRectangle);
		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));

		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		if (withChildren)
			UpdateChildrenTransforms();

		UpdateScrollParams();

		RectF _mChildrenAbsRect = mChildrenWorldRect;
		mChildrenWorldRect = layout->mData->worldRectangle;

		if (mOwnHorScrollBar)
			mHorScrollBar->UpdateTransform(true);

		if (mOwnVerScrollBar)
			mVerScrollBar->UpdateTransform(true);

		mChildrenWorldRect = _mChildrenAbsRect;
	}

	void UIContextMenu::CopyData(const Actor& otherActor)
	{
		const UIContextMenu& other = dynamic_cast<const UIContextMenu&>(otherActor);

		UIScrollArea::CopyData(other);

		delete mItemSample;
		delete mSelectionDrawable;
		delete mSeparatorSample;

		mItemSample        = other.mItemSample->CloneAs<UIContextMenuItem>();
		mSeparatorSample   = other.mSeparatorSample->CloneAs<UIWidget>();
		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mSelectionLayout   = other.mSelectionLayout;
		mItemsLayout       = FindChildByType<UIVerticalLayout>();
		mMaxVisibleItems   = other.mMaxVisibleItems;

		mItemSample->ExcludeFromScene();
		mSeparatorSample->ExcludeFromScene();

		mFitSizeMin = other.mFitSizeMin;

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void UIContextMenu::CheckClipping(const RectF& clipArea)
	{
		mIsClipped = false;

		Vec2F resolution = o2Render.GetCurrentResolution();
		RectF fullScreenRect(resolution*0.5f, resolution*(-0.5f));
		for (auto child : mChildWidgets)
			child->CheckClipping(fullScreenRect);
	}

	void UIContextMenu::FitSizeAndPosition(const Vec2F& position)
	{
		Vec2F size;
		float maxCaption = 0.0f;
		float maxShortcut = 0.0f;

		int i = 0;
		for (auto child : mItemsLayout->GetChildWidgets())
		{
			if (auto childCaption = child->GetLayerDrawable<Text>("caption"))
				maxCaption = Math::Max(childCaption->GetRealSize().x, maxCaption);

			if (auto shortcutCaption = child->GetLayerDrawable<Text>("shortcut"))
				maxShortcut = Math::Max(shortcutCaption->GetRealSize().x, maxShortcut);

			size.y += child->layout->minHeight;

			i++;
			if (i == mMaxVisibleItems)
				break;
		}

		size.x = mFitSizeMin + maxCaption + maxShortcut;
		size.y += mViewAreaLayout.offsetMin.y - mViewAreaLayout.offsetMax.y;

		size.x = Math::Min(size.x, (float)o2Render.GetResolution().x);
		size.y = Math::Min(size.y, (float)o2Render.GetResolution().y);

		RectF thisRect(position.x, position.y, position.x + size.x, position.y - size.y);
		RectF screenRect(-o2Render.GetResolution().x*0.5f, o2Render.GetResolution().y*0.5f,
						 o2Render.GetResolution().x*0.5f, -o2Render.GetResolution().y*0.5f);

		if (thisRect.left < screenRect.left)
			thisRect += Vec2F(screenRect.left - thisRect.left, 0);

		if (thisRect.right > screenRect.right)
			thisRect += Vec2F(screenRect.right - thisRect.right, 0);

		if (thisRect.top > screenRect.top)
			thisRect += Vec2F(0, screenRect.top - thisRect.top);

		if (thisRect.bottom < screenRect.bottom)
			thisRect += Vec2F(0, screenRect.bottom - thisRect.bottom);

		thisRect.left = Math::Round(thisRect.left);
		thisRect.right = Math::Round(thisRect.right);
		thisRect.top = Math::Round(thisRect.top);
		thisRect.bottom = Math::Round(thisRect.bottom);

		layout->worldRect = thisRect;
	}

	void UIContextMenu::SpecialDraw()
	{
		if (!mResEnabledInHierarchy)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		for (auto child : mChildWidgets)
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
		UIContextMenuItem* itemWidget = mItemSample->CloneAs<UIContextMenuItem>();
		SetupItem(itemWidget, item);
		return itemWidget;
	}

	void UIContextMenu::SetupItem(UIContextMenuItem* widget, const Item& item)
	{
		widget->name = (WString)"Context Item " + item.text;

		if (auto iconLayer = widget->FindLayer("icon"))
		{
			if (item.icon)
			{
				Vec2F size = item.icon->GetAtlasRect().Size();

				if (size.x > size.y)
				{
					size.y *= size.x / widget->layout->height;
					size.x = widget->layout->height;
				}
				else
				{
					size.x *= size.y / widget->layout->height;
					size.y = widget->layout->height;
				}

				iconLayer->AddChildLayer("sprite", mnew Sprite(item.icon),
										 Layout(Vec2F(), Vec2F(),
										 Vec2F(-Math::Floor(size.x*0.5f), Math::Floor(size.y*0.5f)),
										 Vec2F(Math::Floor(size.x*0.5f), -Math::Floor(size.y*0.5f))));

				widget->UpdateLayersDrawingSequence();
			}
		}

		if (auto textLayer = widget->GetLayerDrawable<Text>("caption"))
			textLayer->text = item.text;

		if (auto shortcutLayer = widget->GetLayerDrawable<Text>("shortcut"))
			shortcutLayer->text = item.shortcut.AsString();

		if (auto subIconLayer = widget->FindLayer("subIcon"))
			subIconLayer->transparency = item.subItems.Count() > 0 ? 1.0f : 0.0f;

		if (auto checkLayer = widget->GetLayerDrawable<Sprite>("check"))
			checkLayer->enabled = item.checked;

		widget->SetShortcut(item.shortcut);
		widget->SetChecked(item.checked);
		widget->SetCheckable(item.checkable);
		widget->onChecked = item.onChecked;

		widget->RemoveAllChildren();
		if (item.subItems.Count() > 0)
		{
			UIContextMenu* subMenu = mnew UIContextMenu(*this);
			subMenu->RemoveAllItems();
			subMenu->AddItems(item.subItems);

			widget->AddChild(subMenu);
		}
	}

	UIContextMenu::Item UIContextMenu::GetItemDef(int idx) const
	{
		Item res;
		auto item = mItemsLayout->mChildren[idx];

		if (item->name == "Separator")
		{
			res.text = "---";
		}
		else
		{
			auto contextItem = (UIContextMenuItem*)item;
			if (auto iconLayer = contextItem->GetLayerDrawable<Sprite>("icon"))
				res.icon = iconLayer->GetImageAsset();

			if (auto textLayer = contextItem->GetLayerDrawable<Text>("caption"))
				res.text = textLayer->text;

			if (auto shortcutLayer = contextItem->GetLayerDrawable<Text>("shortcut"))
				res.shortcut = contextItem->GetShortcut();

			if (auto subMenu = contextItem->FindChildByType<UIContextMenu>())
				res.subItems = subMenu->GetItems();

			res.checked = contextItem->IsChecked();
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
		UIWidget(other), text(this)
	{
		mSubMenu = FindChildByType<UIContextMenu>();
		if (mSubMenu)
			mSubMenu->Hide(true);

		RetargetStatesAnimations();
	}

	UIContextMenuItem::~UIContextMenuItem()
	{}

	void UIContextMenuItem::SetText(const WString& text)
	{
		auto textLayer = GetLayerDrawable<Text>("caption");
		if (textLayer)
			textLayer->text = text;
	}

	WString UIContextMenuItem::GetText() const
	{
		auto textLayer = GetLayerDrawable<Text>("caption");
		if (textLayer)
			return textLayer->text;

		return "";
	}

	UIContextMenuItem& UIContextMenuItem::operator=(const UIContextMenuItem& other)
	{
		CopyData(other);
		return *this;
	}

	UIContextMenu* UIContextMenuItem::GetSubMenu() const
	{
		return mSubMenu;
	}

	void UIContextMenuItem::SetChecked(bool checked)
	{
		if (auto checkLayer = GetLayerDrawable<Sprite>("check"))
			checkLayer->enabled = checked;

		mChecked = checked;
	}

	bool UIContextMenuItem::IsChecked() const
	{
		return mChecked;
	}

	void UIContextMenuItem::SetCheckable(bool checkable)
	{
		mCheckable = checkable;
	}

	bool UIContextMenuItem::IsCheckable() const
	{
		return mCheckable;
	}

	void UIContextMenuItem::SetShortcut(const ShortcutKeys& shortcut)
	{
		ShortcutKeysListener::SetShortcut(shortcut);
	}

	void UIContextMenuItem::CopyData(const Actor& otherActor)
	{
		const UIContextMenuItem& other = dynamic_cast<const UIContextMenuItem&>(otherActor);

		UIWidget::CopyData(other);

		mSubMenu = FindChildByType<UIContextMenu>();
		if (mSubMenu)
			mSubMenu->Hide(true);
	}

	void UIContextMenuItem::OnChildAdded(UIWidget* child)
	{
		if (child->GetType() == TypeOf(UIContextMenu))
			mSubMenu = (UIContextMenu*)child;
	}

	void UIContextMenuItem::OnShortcutPressed()
	{
		onClick();
	}

}

DECLARE_CLASS(o2::UIContextMenuItem);

DECLARE_CLASS(o2::UIContextMenu);

DECLARE_CLASS(o2::UIContextMenu::Item);
