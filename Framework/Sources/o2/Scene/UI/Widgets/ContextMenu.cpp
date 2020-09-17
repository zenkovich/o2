#include "o2/stdafx.h"
#include "ContextMenu.h"

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Render/Render.h"
#include "o2/Render/Sprite.h"
#include "o2/Render/Text.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	bool ContextMenu::Item::operator==(const Item& other) const
	{
		return text == other.text && shortcut == other.shortcut && icon == other.icon;
	}

	ContextMenu::Item::Item():
		checked(false), checkable(false)
	{}

	ContextMenu::Item::Item(const WString& text, const Function<void()> onClick,
							const WString& group /*= ""*/, const ImageAssetRef& icon /*= ImageAssetRef()*/,
							const ShortcutKeys& shortcut /*= ShortcutKeys()*/) :
		text(text), group(group), onClick(onClick), shortcut(shortcut), icon(icon), checked(false), checkable(false)
	{}

	ContextMenu::Item::Item(const WString& text, Vector<Item> subItems,
							const WString& group /*= ""*/, const ImageAssetRef& icon /*= ImageAssetRef()*/) :
		text(text), group(group), subItems(subItems), icon(icon), checked(false), checkable(false)
	{}

	ContextMenu::Item::Item(const WString& text, bool checked,
							Function<void(bool)> onChecked /*= Function<void(bool)>()*/,
							const WString& group /*= ""*/, const ImageAssetRef& icon /*= ImageAssetRef()*/,
							const ShortcutKeys& shortcut /*= ShortcutKeys()*/) :
		text(text), group(group), checked(checked), onChecked(onChecked), checkable(true), shortcut(shortcut), icon(icon)
	{}

	ContextMenu::Item::~Item()
	{}

	ContextMenu::Item ContextMenu::Item::Separator()
	{
		return Item(separatorText, Function<void()>());
	}

	WString ContextMenu::Item::separatorText = "---";

	ContextMenu::ContextMenu():
		PopupWidget()
	{
		mItemSample = mnew ContextMenuItem();
		mItemSample->RemoveFromScene();
		mItemSample->layout->minHeight = 20.0f;
		auto basicLayer = mItemSample->AddLayer("basic", nullptr);
		basicLayer->AddChildLayer("icon", nullptr, Layout(Vec2F(0.0f, 0.5f), Vec2F(0.0f, 0.5f), Vec2F(10, 0), Vec2F(10, 0)));
		basicLayer->AddChildLayer("subIcon", nullptr, Layout(Vec2F(1.0f, 0.5f), Vec2F(1.0f, 0.5f), Vec2F(-10, 0), Vec2F(-10, 0)));
		basicLayer->AddChildLayer("caption", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));
		basicLayer->AddChildLayer("shortcut", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));

		mSeparatorSample = mnew Widget();
		mSeparatorSample->RemoveFromScene();
		mSeparatorSample->layout->minHeight = 3.0f;
		mSeparatorSample->layout->height = 3.0f;

		mSelectionDrawable = mnew Sprite();

		mItemsLayout = mnew VerticalLayout();
		AddChild(mItemsLayout);

		mItemsLayout->expandHeight = false;
		mItemsLayout->expandWidth = true;
		mItemsLayout->baseCorner = BaseCorner::LeftTop;
		mItemsLayout->fitByChildren = true;
		*mItemsLayout->layout = WidgetLayout::BothStretch();
	}

	ContextMenu::ContextMenu(Vector<Item> items):
		ContextMenu()
	{
		AddItems(items);
	}

	ContextMenu::ContextMenu(const ContextMenu& other):
		PopupWidget(other), mMaxVisibleItems(other.mMaxVisibleItems)
	{
		mItemSample = other.mItemSample->CloneAs<ContextMenuItem>();
		mItemSample->RemoveFromScene();

		mSeparatorSample = other.mSeparatorSample->CloneAs<Widget>();
		mSeparatorSample->RemoveFromScene();

		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mSelectionLayout = other.mSelectionLayout;
		mItemsLayout = FindChildByType<VerticalLayout>();

		RetargetStatesAnimations();
	}

	ContextMenu::~ContextMenu()
	{
		delete mItemSample;
		delete mSelectionDrawable;
		delete mSeparatorSample;
	}

	ContextMenu& ContextMenu::operator=(const ContextMenu& other)
	{
		PopupWidget::operator=(other);
		return *this;
	}

	void ContextMenu::Update(float dt)
	{
		if (!mResEnabledInHierarchy)
			return;

		PopupWidget::Update(dt);

		const float rectLerpCoef = 20.0f;
		if (mCurrentSelectionRect != mTargetSelectionRect)
		{
			mCurrentSelectionRect = Math::Lerp(mCurrentSelectionRect, mTargetSelectionRect, dt*rectLerpCoef);
			mSelectionDrawable->SetRect(mCurrentSelectionRect);
		}

		bool cursorPressed = o2Input.IsCursorPressed() || o2Input.IsRightMousePressed();
		if (cursorPressed)
		{
			if (!mChildPopup && (cursorPressed || Math::Abs(o2Input.GetMouseWheelDelta()) > 0.1f) &&
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
				if (mChildPopup)
					mChildPopup->HideWithChild();

				mChildPopup = nullptr;

				if (mSelectedItem && mSelectedItem->GetSubMenu())
					mSelectedItem->GetSubMenu()->Show(this, mSelectedItem->layout->worldRightTop);
			}
		}

		mShownAtFrame = false;
	}

	void ContextMenu::Show(PopupWidget* parent, const Vec2F& position /*= o2Input.GetCursorPos()*/)
	{
		if (!mItemsBuilt)
			RebuildItems();

		PopupWidget::Show(parent, position);

		auto hoverState = state["hover"];
		if (hoverState)
		{
			mSelectionDrawable->SetEnabled(true);
			*hoverState = false;
		}
		else mSelectionDrawable->SetEnabled(false);
	}

	void ContextMenu::Show(const Vec2F& position /*= o2Input.GetCursorPos()*/)
	{
		PopupWidget::Show(position);
	}

	void ContextMenu::AddItem(const Item& item)
	{
		mItems.Add(item);
		mItemsBuilt = false;

		if (mResEnabledInHierarchy)
		{
			RebuildItems();
			FitSizeAndPosition(layout->worldLeftTop);
			SetLayoutDirty();
		}
	}

	void ContextMenu::AddItem(const WString& path,
							  const Function<void()>& clickFunc /*= Function<void()>()*/,
							  const ImageAssetRef& icon /*= ImageAssetRef()*/,
							  const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		WString targetPath = path;
		auto itemsList = CreateItemsByPath(targetPath);
		int groupDelPos = targetPath.FindLast(".");
		itemsList->Add(Item(targetPath.SubStr(0, groupDelPos), clickFunc,
					   groupDelPos < 0 ? "" : targetPath.SubStr(groupDelPos + 1),
					   icon, shortcut));
	}

	void ContextMenu::AddToggleItem(const WString& path, bool value,
									const Function<void(bool)>& clickFunc /*= Function<void(bool)>()*/,
									const ImageAssetRef& icon /*= ImageAssetRef()*/,
									const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		WString targetPath = path;
		auto itemsList = CreateItemsByPath(targetPath);
		int groupDelPos = targetPath.FindLast(".");
		itemsList->Add(Item(targetPath.SubStr(0, groupDelPos), value, clickFunc,
					   groupDelPos < 0 ? "" : targetPath.SubStr(groupDelPos + 1),
					   icon, shortcut));
	}

	Vector<ContextMenu::Item>* ContextMenu::CreateItemsByPath(WString& path)
	{
		Vector<Item>* items = &mItems;

		while (true)
		{
			int slashPos = path.Find("/");
			if (slashPos < 0)
				break;

			WString subMenu = path.SubStr(0, slashPos);

			auto subItem = items->Find([&](const Item& x) { return x.text == subMenu; });

			if (!subItem)
			{
				items->Add(Item(subMenu, Function<void()>(), o2FileSystem.GetFileExtension(subMenu)));
				subItem = &items->Last();
			}

			items = &subItem->subItems;
			path = path.SubStr(slashPos + 1);
		}

		return items;
	}

	void ContextMenu::InsertItem(const Item& item, int position)
	{
		mItems.Insert(item, position);
		mItemsBuilt = false;

		if (mResEnabledInHierarchy)
		{
			RebuildItems();
			FitSizeAndPosition(layout->worldLeftTop);
			SetLayoutDirty();
		}
	}

	void ContextMenu::AddItems(Vector<Item> items)
	{
		for (auto& item : items)
			AddItem(item);
	}

	void ContextMenu::InsertItems(Vector<Item> items, int position)
	{
		int i = 0;
		for (auto& item : items)
		{
			InsertItem(item, position + i);
			i++;
		}
	}

	ContextMenu::Item ContextMenu::GetItem(int position)
	{
		if (position > 0 && position < mItemsLayout->GetChildren().Count())
			return GetItemDef(position);

		return Item();
	}

	ContextMenuItem* ContextMenu::GetItemWidget(int position)
	{
		if (position > 0 && position < mItemsLayout->GetChildren().Count())
			return dynamic_cast<ContextMenuItem*>(mItemsLayout->GetChildren()[position]);

		return nullptr;
	}

	int ContextMenu::FindItem(const WString& text) const
	{
		int idx = 0;
		for (auto child : mItemsLayout->GetChildWidgets())
		{
			auto item = dynamic_cast<ContextMenuItem*>(child);
			if (item && item->GetText() == text)
				return idx;

			idx++;
		}

		return -1;
	}

	ContextMenuItem* ContextMenu::FindItemWidget(const WString& path) const
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		for (auto child : mItemsLayout->mChildren)
		{
			auto item = dynamic_cast<ContextMenuItem*>(child);
			if (item && item->GetText() == pathPart)
			{
				if (delPos == -1)
					return item;
				else if (auto subContext = item->GetSubMenu())
					return subContext->FindItemWidget(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	void ContextMenu::SetItem(int position, const Item& item)
	{
		if (position < 0 || position >= mItems.Count())
			return;

		mItems[position] = item;
		mItemsBuilt = false;

		if (mResEnabledInHierarchy)
		{
			RebuildItems();
			FitSizeAndPosition(layout->worldLeftTop);
			SetLayoutDirty();
		}
	}

	ContextMenuItem* ContextMenu::GetItemUnderPoint(const Vec2F& point)
	{
		if (!mItemsLayout)
			return nullptr;

		for (auto child : mItemsLayout->mChildWidgets)
		{
			if (child->layout->IsPointInside(point) && child->GetType() == TypeOf(ContextMenuItem))
				return (ContextMenuItem*)child;
		}

		return nullptr;
	}

	void ContextMenu::UpdateHover(const Vec2F& point)
	{
		ContextMenuItem* itemUnderCursor = GetItemUnderPoint(point);

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
			mTargetSelectionRect = mSelectionLayout.Calculate(itemUnderCursor->GetLayoutData().worldRectangle);

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

	void ContextMenu::OnCursorPressed(const Input::Cursor& cursor)
	{}

	void ContextMenu::OnCursorStillDown(const Input::Cursor& cursor)
	{}

	void ContextMenu::OnCursorReleased(const Input::Cursor& cursor)
	{
		ContextMenuItem* itemUnderCursor = GetItemUnderPoint(cursor.position);

		if (!itemUnderCursor)
			return;

		if (!itemUnderCursor->IsEnabled())
			return;

		if (itemUnderCursor)
		{
			itemUnderCursor->onClick();

			if (itemUnderCursor->IsCheckable())
			{
				itemUnderCursor->SetChecked(!itemUnderCursor->IsChecked());
				itemUnderCursor->onChecked(itemUnderCursor->IsChecked());
			}
		}

		if (itemUnderCursor && itemUnderCursor->FindChildByType<ContextMenu>() == nullptr)
		{
			HideWithParent();
			HideWithChild();
		}
	}

	void ContextMenu::OnCursorMoved(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastSelectCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastSelectCheckCursor = cursor.position;

		UpdateHover(cursor.position);
	}

	void ContextMenu::HideWithParent()
	{
		PopupWidget::HideWithParent();
		mSelectedItem = nullptr;
	}

	void ContextMenu::HideWithChild()
	{
		PopupWidget::HideWithChild();
		mSelectedItem = nullptr;
	}

	Vector<ContextMenu::Item> ContextMenu::GetItems() const
	{
		Vector<Item> res;
		for (int i = 0; i < mItemsLayout->GetChildren().Count(); i++)
			res.Add(GetItemDef(i));

		return res;
	}

	void ContextMenu::RemoveItem(int position)
	{
		if (position > 0 && position < mItemsLayout->GetChildren().Count())
			mItemsLayout->RemoveChild(mItemsLayout->GetChildren()[position]);
	}

	void ContextMenu::RemoveItem(const WString& path)
	{
		ContextMenu* targetContext = this;
		WString targetPath = path;

		while (true)
		{
			int slashPos = targetPath.Find("/");
			if (slashPos < 0)
				break;

			WString subMenu = targetPath.SubStr(0, slashPos);

			Widget* subChild = targetContext->mItemsLayout->mChildWidgets.FindOrDefault([&](auto x) {
				if (auto text = x->template GetLayerDrawable<Text>("basic/caption"))
					return text->text == subMenu;

				return false;
			});

			if (!subChild)
			{
				o2Debug.LogError("Failed to remove context item " + path);
				return;
			}

			ContextMenu* subContext = subChild->FindChildByType<ContextMenu>();
			if (!subContext)
			{
				o2Debug.LogError("Failed to remove context item " + path);
				return;
			}

			targetContext = subContext;
			targetPath = targetPath.SubStr(slashPos + 1);
		}

		Widget* removingItem = targetContext->mItemsLayout->mChildWidgets.FindOrDefault([&](auto x) {
			if (auto text = x->template GetLayerDrawable<Text>("basic/caption"))
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

	void ContextMenu::RemoveAllItems()
	{
		mItemsLayout->RemoveAllChildren();
		mSelectedItem = nullptr;
	}

	void ContextMenu::SetItemChecked(int position, bool checked)
	{
		if (position < 0 || position >= mItemsLayout->mChildren.Count())
			return;

		ContextMenuItem* item = (ContextMenuItem*)mItemsLayout->mChildren[position];
		if (item->IsCheckable())
			item->SetChecked(checked);
	}

	bool ContextMenu::IsItemChecked(int position) const
	{
		if (position < 0 || position >= mItemsLayout->mChildren.Count())
			return false;

		ContextMenuItem* item = (ContextMenuItem*)mItemsLayout->mChildren[position];
		return item->IsChecked();
	}

	VerticalLayout* ContextMenu::GetItemsLayout() const
	{
		return mItemsLayout;
	}

	ContextMenuItem* ContextMenu::GetItemSample() const
	{
		return mItemSample;
	}

	Widget* ContextMenu::GetSeparatorSample() const
	{
		return mSeparatorSample;
	}

	Sprite* ContextMenu::GetSelectionDrawable() const
	{
		return mSelectionDrawable;
	}

	void ContextMenu::SetSelectionDrawableLayout(const Layout& layout)
	{
		mSelectionLayout = layout;
	}

	Layout ContextMenu::GetSelectionDrawableLayout() const
	{
		return mSelectionLayout;
	}

	void ContextMenu::SetMaxItemsVisible(int count)
	{
		mMaxVisibleItems = count;
	}

	void ContextMenu::SetItemsMaxPriority()
	{
		for (auto child : mItemsLayout->mChildren)
		{
			if (ContextMenuItem* item = dynamic_cast<ContextMenuItem*>(child))
				item->SetMaxPriority();
		}
	}

	void ContextMenu::SetItemsMinPriority()
	{
		for (auto child : mItemsLayout->mChildren)
		{
			if (ContextMenuItem* item = dynamic_cast<ContextMenuItem*>(child))
				item->SetMinPriority();
		}
	}

	void ContextMenu::CopyData(const Actor& otherActor)
	{
		const ContextMenu& other = dynamic_cast<const ContextMenu&>(otherActor);

		PopupWidget::CopyData(other);

		delete mItemSample;
		delete mSelectionDrawable;
		delete mSeparatorSample;

		mItemSample = other.mItemSample->CloneAs<ContextMenuItem>();
		mSeparatorSample = other.mSeparatorSample->CloneAs<Widget>();
		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mSelectionLayout = other.mSelectionLayout;
		mItemsLayout = FindChildByType<VerticalLayout>();
		mMaxVisibleItems = other.mMaxVisibleItems;

		mItemSample->RemoveFromScene();
		mSeparatorSample->RemoveFromScene();

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void ContextMenu::FitSizeAndPosition(const Vec2F& position)
	{
		Vec2F size;
		float maxCaption = 0.0f;
		float maxShortcut = 0.0f;

		int i = 0;
		for (auto child : mItemsLayout->GetChildWidgets())
		{
			if (auto childCaption = child->GetLayerDrawable<Text>("basic/caption"))
				maxCaption = Math::Max(childCaption->GetRealSize().x, maxCaption);

			if (auto shortcutCaption = child->GetLayerDrawable<Text>("basic/shortcut"))
				maxShortcut = Math::Max(shortcutCaption->GetRealSize().x, maxShortcut);

			size.y += child->layout->minHeight;

			i++;
			if (i == mMaxVisibleItems)
				break;
		}

		size.x = mFitSizeMin + maxCaption + maxShortcut;
		size.y += mViewAreaLayout.offsetMin.y - mViewAreaLayout.offsetMax.y;

		FitPosition(position, size);
	}

	void ContextMenu::SpecialDraw()
	{
		if (!mResEnabledInHierarchy)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();
		CursorAreaEventsListener::OnDrawn();

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

		if (mChildPopup)
			mChildPopup->SpecialDraw();
	}

	void ContextMenu::RebuildItems()
	{
		Vector<ContextMenuItem*> cache;

		for (auto& item : mItems)
		{
			if (item.widget)
				mItemsLayout->RemoveChild(item.widget, false);
		}

		auto children = mItemsLayout->GetChildren();
		for (auto child : children)
		{
			if (auto item = dynamic_cast<ContextMenuItem*>(child))
			{
				cache.Add(item);
				mItemsLayout->RemoveChild(item, false);
			}
		}

		Map<WString, Vector<Item*>> groups;
		for (auto& item : mItems)
			groups[item.group].Add(&item);

		bool isFirst = true;
		for (auto& kv : groups)
		{
			if (!isFirst)
			{
				Widget* newItem = mSeparatorSample->CloneAs<Widget>();
				newItem->name = "Separator";
				mItemsLayout->AddChild(newItem);
			}

			if (isFirst)
				isFirst = false;

			for (auto& item : kv.second)
			{
				if (item->widget)
					mItemsLayout->AddChild(item->widget);
				else
				{
					if (item->text == Item::separatorText)
					{
						Widget* newItem = mSeparatorSample->CloneAs<Widget>();
						newItem->name = "Separator";
						mItemsLayout->AddChild(newItem);
					}
					else
					{
						PushEditorScopeOnStack scope(IEventsListener::mIsEditorMode ? 1 : 0);

						ContextMenuItem* newItem = cache.IsEmpty() ? mItemSample->CloneAs<ContextMenuItem>() : cache.PopBack();
						newItem->Setup(*item);
						mItemsLayout->AddChild(newItem);
					}
				}
			}
		}

		mItemsBuilt = true;
	}

	ContextMenu::Item ContextMenu::GetItemDef(int idx) const
	{
		Item res;
		auto item = mItemsLayout->mChildren[idx];

		if (item->name == "Separator")
			res.text = Item::separatorText;
		else
		{
			auto contextItem = (ContextMenuItem*)item;
			if (auto iconLayer = contextItem->GetLayerDrawable<Sprite>("basic/icon"))
				res.icon = iconLayer->GetImageAsset();

			if (auto textLayer = contextItem->GetLayerDrawable<Text>("basic/caption"))
				res.text = textLayer->text;

			if (auto shortcutLayer = contextItem->GetLayerDrawable<Text>("basic/shortcut"))
				res.shortcut = contextItem->GetShortcut();

			if (auto subMenu = contextItem->FindChildByType<ContextMenu>())
				res.subItems = subMenu->GetItems();

			res.checked = contextItem->IsChecked();
			res.onClick = contextItem->onClick;
		}

		return res;
	}

	ContextMenuItem::ContextMenuItem():
		Widget(), mSubMenu(nullptr)
	{
		RetargetStatesAnimations();
	}

	ContextMenuItem::ContextMenuItem(const ContextMenuItem& other):
		Widget(other), text(this)
	{
		mSubMenu = FindChildByType<ContextMenu>();
		if (mSubMenu)
			mSubMenu->Hide(true);

		RetargetStatesAnimations();
	}

	ContextMenuItem::~ContextMenuItem()
	{}

	void ContextMenuItem::Setup(ContextMenu::Item& item)
	{
		item.widget = this;
		name = (WString)"Context Item " + item.text;

		if (auto iconLayer = FindLayer("icon"))
		{
			if (item.icon)
			{
				Vec2F size = item.icon->GetAtlasRect().Size();

				if (size.x > size.y)
				{
					size.y *= size.x / layout->height;
					size.x = layout->height;
				}
				else
				{
					size.x *= size.y / layout->height;
					size.y = layout->height;
				}

				iconLayer->AddChildLayer("sprite", mnew Sprite(item.icon),
										 Layout(Vec2F(), Vec2F(),
										 Vec2F(-Math::Floor(size.x*0.5f), Math::Floor(size.y*0.5f)),
										 Vec2F(Math::Floor(size.x*0.5f), -Math::Floor(size.y*0.5f))));

				UpdateLayersDrawingSequence();
			}
		}

		if (auto textLayer = GetLayerDrawable<Text>("basic/caption"))
			textLayer->text = item.text;

		if (auto shortcutLayer = GetLayerDrawable<Text>("basic/shortcut"))
			shortcutLayer->text = item.shortcut.AsString();

		if (auto subIconLayer = FindLayer("subIcon"))
			subIconLayer->transparency = item.subItems.Count() > 0 ? 1.0f : 0.0f;

		if (auto checkLayer = GetLayerDrawable<Sprite>("check"))
			checkLayer->enabled = item.checked;

		SetShortcut(item.shortcut);
		SetChecked(item.checked);
		SetCheckable(item.checkable);

		onClick = item.onClick;
		onChecked = item.onChecked;

		RemoveAllChildren();
		if (item.subItems.Count() > 0)
		{
			ContextMenu* subMenu = o2UI.CreateWidget<ContextMenu>();
			subMenu->RemoveAllItems();
			subMenu->AddItems(item.subItems);

			AddChild(subMenu);
		}
	}

	void ContextMenuItem::SetText(const WString& text)
	{
		auto textLayer = GetLayerDrawable<Text>("basic/caption");
		if (textLayer)
			textLayer->text = text;
	}

	WString ContextMenuItem::GetText() const
	{
		auto textLayer = GetLayerDrawable<Text>("basic/caption");
		if (textLayer)
			return textLayer->text;

		return "";
	}

	ContextMenuItem& ContextMenuItem::operator=(const ContextMenuItem& other)
	{
		CopyData(other);
		return *this;
	}

	ContextMenu* ContextMenuItem::GetSubMenu() const
	{
		return mSubMenu;
	}

	void ContextMenuItem::SetChecked(bool checked)
	{
		if (auto checkLayer = GetLayerDrawable<Sprite>("check"))
			checkLayer->enabled = checked;

		mChecked = checked;
	}

	bool ContextMenuItem::IsChecked() const
	{
		return mChecked;
	}

	void ContextMenuItem::SetCheckable(bool checkable)
	{
		mCheckable = checkable;
	}

	bool ContextMenuItem::IsCheckable() const
	{
		return mCheckable;
	}

	void ContextMenuItem::SetShortcut(const ShortcutKeys& shortcut)
	{
		ShortcutKeysListener::SetShortcut(shortcut);
	}

	void ContextMenuItem::SetEnabled(bool enabled)
	{
		SetState("enabled", enabled);
	}

	bool ContextMenuItem::IsEnabled() const
	{
		return GetState("enabled");
	}

	void ContextMenuItem::Enable()
	{
		SetEnabled(true);
	}

	void ContextMenuItem::Disable()
	{
		SetEnabled(false);
	}

	void ContextMenuItem::CopyData(const Actor& otherActor)
	{
		const ContextMenuItem& other = dynamic_cast<const ContextMenuItem&>(otherActor);

		Widget::CopyData(other);

		mSubMenu = FindChildByType<ContextMenu>();
		if (mSubMenu)
			mSubMenu->Hide(true);
	}

	void ContextMenuItem::OnChildAdded(Widget* child)
	{
		if (child->GetType() == TypeOf(ContextMenu))
			mSubMenu = (ContextMenu*)child;
	}

	void ContextMenuItem::OnShortcutPressed()
	{
		onClick();
	}

}

DECLARE_CLASS(o2::ContextMenu);

DECLARE_CLASS(o2::ContextMenuItem);

DECLARE_CLASS(o2::ContextMenu::Item);
