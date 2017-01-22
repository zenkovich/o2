#include "MenuPanel.h"

#include "Render/Sprite.h"
#include "UI/HorizontalLayout.h"
#include "UI/UIManager.h"

namespace o2
{

	UIMenuPanel::UIMenuPanel():
		UIWidget(), DrawableCursorEventsListener(this)
	{
		mItemSample = mnew UIWidget();
		mItemSample->AddLayer("text", nullptr, Layout(Vec2F(0.0f, 0.0f), Vec2F(1.0f, 1.0f), Vec2F(20, 0), Vec2F(0, 0)));

		mSelectionDrawable = mnew Sprite();

		mLayout = mnew UIHorizontalLayout();
		AddChild(mLayout);

		mLayout->expandHeight  = true;
		mLayout->expandWidth   = false;
		mLayout->baseCorner    = BaseCorner::LeftTop;
		mLayout->fitByChildren = true;
		mLayout->layout        = UIWidgetLayout::BothStretch();
	}

	UIMenuPanel::UIMenuPanel(const UIMenuPanel& other):
		UIWidget(other), DrawableCursorEventsListener(this)
	{
		mItemSample        = other.mItemSample->Clone();
		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mSelectionLayout   = other.mSelectionLayout;
		mLayout            = FindChild<UIHorizontalLayout>();

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UIMenuPanel::~UIMenuPanel()
	{
		delete mItemSample;
		delete mSelectionDrawable;
	}

	UIMenuPanel& UIMenuPanel::operator=(const UIMenuPanel& other)
	{
		UIWidget::operator=(other);

		delete mItemSample;
		delete mSelectionDrawable;

		mItemSample        = other.mItemSample->Clone();
		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mSelectionLayout   = other.mSelectionLayout;
		mLayout            = FindChild<UIHorizontalLayout>();

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIMenuPanel::Update(float dt)
	{
		if (mFullyDisabled || mIsClipped)
			return;

		UIWidget::Update(dt);

		const float rectLerpCoef = 20.0f;
		if (mCurrentSelectionRect != mTargetSelectionRect)
		{
			mCurrentSelectionRect = Math::Lerp(mCurrentSelectionRect, mTargetSelectionRect, dt*rectLerpCoef);
			mSelectionDrawable->SetRect(mCurrentSelectionRect);
		}

		if (mSelectSubContextTime >= 0.0f)
		{
			mSelectSubContextTime -= dt;

			if (mSelectSubContextTime < 0.0f)
			{
				if (mOpenedContext)
				{
					mOpenedContext->HideWithChild();

					if (mSelectedItem >= 0)
					{
						if (auto contextMenu = mLayout->mChilds[mSelectedItem]->FindChild<UIContextMenu>())
						{
							contextMenu->Show(mLayout->mChilds[mSelectedItem]->layout.absLeftBottom);
							mOpenedContext = contextMenu;
						}
					}
				}
			}
		}
	}

	void UIMenuPanel::Draw()
	{
		if (mFullyDisabled)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();

		for (auto child : mChilds)
			child->Draw();

		mSelectionDrawable->Draw();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		DrawDebugFrame();
	}

	UIWidget* UIMenuPanel::AddItem(const Item& item)
	{
		UIWidget* newItem = CreateItem(item);
		mLayout->AddChild(newItem);
		mClickFunctions.Add(item.onClick);

		return newItem;
	}

	UIWidget* UIMenuPanel::AddItem(const WString& path, const Function<void()>& clickFunc /*= Function<void()>()*/,
								   const ImageAssetRef& icon /*= ImageAssetRef()*/, 
								   const ShortcutKeys& shortcut /*= ShortcutKeys()*/)
	{
		int slashPos = path.Find("/");
		if (slashPos < 0)
			return AddItem(Item(path, clickFunc));

		WString subMenu = path.SubStr(0, slashPos);

		UIWidget* subChild = mLayout->mChilds.FindMatch([&](auto x) {
			if (auto text = x->GetLayerDrawable<Text>("text"))
				return text->text == subMenu;

			return false;
		});

		if (!subChild)
			subChild = AddItem(subMenu);

		UIContextMenu* subContext = subChild->FindChild<UIContextMenu>();
		if (!subContext)
		{
			subContext = o2UI.CreateWidget<UIContextMenu>();
			subChild->AddChild(subContext);
		}

		return subContext->AddItem(path.SubStr(slashPos + 1), clickFunc, icon, shortcut);
	}

	UIWidget* UIMenuPanel::InsertItem(const Item& item, int position)
	{
		UIWidget* newItem = CreateItem(item);
		mLayout->AddChild(newItem, position);

		if (item.subItems.Count() > 0)
			mClickFunctions.Insert([=]() { newItem->FindChild<UIContextMenu>()->Show(newItem->layout.absRightBottom); }, position);
		else
			mClickFunctions.Insert(item.onClick, position);

		return newItem;
	}

	void UIMenuPanel::AddItems(Vector<Item> items)
	{
		for (auto item : items)
			AddItem(item);
	}

	void UIMenuPanel::InsertItems(Vector<Item> items, int position)
	{
		int i = 0;
		for (auto item : items)
		{
			InsertItem(item, position + i);
			i++;
		}
	}

	UIMenuPanel::Item UIMenuPanel::GetItem(int position)
	{
		if (position > 0 && position < mLayout->GetChilds().Count())
			return GetItemDef(position);

		return Item();
	}

	Vector<UIMenuPanel::Item> UIMenuPanel::GetItems() const
	{
		Vector<Item> res;
		for (int i = 0; i < mLayout->GetChilds().Count(); i++)
			res.Add(GetItemDef(i));

		return res;
	}

	void UIMenuPanel::RemoveItem(int position)
	{
		if (position > 0 && position < mLayout->GetChilds().Count())
			mLayout->RemoveChild(mLayout->GetChilds()[position]);
	}

	void UIMenuPanel::RemoveItem(const WString& path)
	{
		int slashPos = path.Find("/");
		if (slashPos < 0)
		{
			UIWidget* removingItem = mLayout->mChilds.FindMatch([&](auto x) {
				if (auto text = x->GetLayerDrawable<Text>("text"))
					return text->text == path;

				return false;
			});

			if (!removingItem)
			{
				o2Debug.LogError("Failed to remove menu item %s", path);
				return;
			}

			mLayout->RemoveChild(removingItem);

			return;
		}

		WString subMenu = path.SubStr(0, slashPos);

		UIWidget* subChild = mLayout->mChilds.FindMatch([&](auto x) {
			if (auto text = x->GetLayerDrawable<Text>("text"))
				return text->text == subMenu;

			return false;
		});

		if (!subChild)
		{
			o2Debug.LogError("Failed to remove menu item %s", path);
			return;
		}

		UIContextMenu* subContext = subChild->FindChild<UIContextMenu>();
		if (!subContext)
		{
			o2Debug.LogError("Failed to remove menu item %s", path);
			return;
		}

		subContext->RemoveItem(path.SubStr(slashPos + 1));
	}

	void UIMenuPanel::RemoveAllItems()
	{
		mLayout->RemoveAllChilds();
	}

	UIHorizontalLayout* UIMenuPanel::GetItemsLayout() const
	{
		return mLayout;
	}

	UIWidget* UIMenuPanel::GetItemSample() const
	{
		return mItemSample;
	}

	void UIMenuPanel::SetItemSample(UIWidget* sample)
	{
		delete mItemSample;
		mItemSample = sample;
	}

	Sprite* UIMenuPanel::GetSelectionDrawable() const
	{
		return mSelectionDrawable;
	}

	void UIMenuPanel::SetSelectionDrawableLayout(const Layout& layout)
	{
		mSelectionLayout = layout;
	}

	Layout UIMenuPanel::GetSelectionDrawableLayout() const
	{
		return mSelectionLayout;
	}

	UIWidget* UIMenuPanel::CreateItem(const Item& item)
	{
		UIWidget* newItem = mItemSample->Clone();
		newItem->name = (WString)"Menu Item:" + item.text;

		if (auto textLayer = newItem->GetLayerDrawable<Text>("text"))
			textLayer->text = item.text;

		if (item.subItems.Count() > 0)
		{
			UIContextMenu* subMenu = o2UI.CreateWidget<UIContextMenu>();
			subMenu->AddItems(item.subItems);
			newItem->AddChild(subMenu);
		}

		return newItem;
	}

	UIMenuPanel::Item UIMenuPanel::GetItemDef(int idx) const
	{
		Item res;
		auto item = mLayout->mChilds[idx];

		if (auto textLayer = item->GetLayerDrawable<Text>("text"))
			res.text = textLayer->text;

		if (auto subMenu = item->FindChild<UIContextMenu>())
			res.subItems = subMenu->GetItems();

		res.onClick = mClickFunctions[idx];

		return res;
	}

	void UIMenuPanel::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	UIWidget* UIMenuPanel::GetItemUnderPoint(const Vec2F& point, int* idxPtr)
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

	void UIMenuPanel::UpdateHover(const Vec2F& point)
	{
		int itemIdx = -1;
		UIWidget* itemUnderCursor = GetItemUnderPoint(point, &itemIdx);

		if (itemIdx < 0)
		{
			auto hoverState = state["hover"];
			if (hoverState)
			{
				mSelectionDrawable->SetEnabled(true);
				*hoverState = mOpenedContext != nullptr;
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
				mSelectSubContextTime = 0.5f;
				mSelectedItem = itemIdx;
			}
		}
	}

	void UIMenuPanel::OnCursorPressed(const Input::Cursor& cursor)
	{}

	void UIMenuPanel::OnCursorStillDown(const Input::Cursor& cursor)
	{}

	void UIMenuPanel::OnCursorReleased(const Input::Cursor& cursor)
	{
		int itemIdx = -1;
		UIWidget* itemUnderCursor = GetItemUnderPoint(cursor.position, &itemIdx);

		if (itemIdx >= 0)
		{
			mClickFunctions[itemIdx]();

			if (mOpenedContext)
				mOpenedContext->HideWithChild();

			if (auto context = itemUnderCursor->FindChild<UIContextMenu>())
			{
				context->Show(itemUnderCursor->layout.absLeftBottom);
				mOpenedContext = context;
			}
		}
	}

	void UIMenuPanel::OnCursorPressBreak(const Input::Cursor& cursor)
	{}

	void UIMenuPanel::OnCursorMoved(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastSelectCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastSelectCheckCursor = cursor.position;

		UpdateHover(cursor.position);
	}

	void UIMenuPanel::OnCursorExit(const Input::Cursor& cursor)
	{
		UpdateHover(cursor.position);
	}

	UIMenuPanel::Item::Item()
	{}

	UIMenuPanel::Item::Item(const WString& text, Vector<UIContextMenu::Item> subItems):
		text(text), subItems(subItems)
	{}

	UIMenuPanel::Item::Item(const WString& text, const Function<void()> onClick) :
		text(text), onClick(onClick)
	{}

	bool UIMenuPanel::Item::operator==(const Item& other) const
	{
		return text == other.text;
	}
}

CLASS_META(o2::UIMenuPanel)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::DrawableCursorEventsListener);

	PROTECTED_FIELD(mLayout);
	PROTECTED_FIELD(mItemSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mClickFunctions);
	PROTECTED_FIELD(mSelectionDrawable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelectionLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCurrentSelectionRect);
	PROTECTED_FIELD(mTargetSelectionRect);
	PROTECTED_FIELD(mLastSelectCheckCursor);
	PROTECTED_FIELD(mSelectedItem);
	PROTECTED_FIELD(mSelectSubContextTime);
	PROTECTED_FIELD(mOpenedContext);

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(UIWidget*, AddItem, const Item&);
	PUBLIC_FUNCTION(UIWidget*, AddItem, const WString&, const Function<void()>&, const ImageAssetRef&, const ShortcutKeys&);
	PUBLIC_FUNCTION(UIWidget*, InsertItem, const Item&, int);
	PUBLIC_FUNCTION(void, AddItems, Vector<Item>);
	PUBLIC_FUNCTION(void, InsertItems, Vector<Item>, int);
	PUBLIC_FUNCTION(Item, GetItem, int);
	PUBLIC_FUNCTION(Vector<Item>, GetItems);
	PUBLIC_FUNCTION(void, RemoveItem, int);
	PUBLIC_FUNCTION(void, RemoveItem, const WString&);
	PUBLIC_FUNCTION(void, RemoveAllItems);
	PUBLIC_FUNCTION(UIHorizontalLayout*, GetItemsLayout);
	PUBLIC_FUNCTION(UIWidget*, GetItemSample);
	PUBLIC_FUNCTION(void, SetItemSample, UIWidget*);
	PUBLIC_FUNCTION(Sprite*, GetSelectionDrawable);
	PUBLIC_FUNCTION(void, SetSelectionDrawableLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetSelectionDrawableLayout);
	PROTECTED_FUNCTION(UIWidget*, CreateItem, const Item&);
	PROTECTED_FUNCTION(Item, GetItemDef, int);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(UIWidget*, GetItemUnderPoint, const Vec2F&, int*);
	PROTECTED_FUNCTION(void, UpdateHover, const Vec2F&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
}
END_META;

CLASS_META(o2::UIMenuPanel::Item)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(text).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(subItems).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(onClick);
}
END_META;
 