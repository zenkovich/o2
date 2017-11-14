#include "CustomList.h"

#include "Render/Render.h"
#include "Render/Sprite.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/VerticalLayout.h"
#include "UI/VerticalScrollBar.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "Utils/Debug.h"

namespace o2
{
	UICustomList::UICustomList():
		UIScrollArea(), DrawableCursorEventsListener(this)
	{
		mItemSample        = mnew UIWidget();
		mSelectionDrawable = mnew Sprite();
		mHoverDrawable     = mnew Sprite();

		mVerLayout                    = mnew UIVerticalLayout();
		mVerLayout->baseCorner        = BaseCorner::LeftTop;
		mVerLayout->name              = "layout";
		mVerLayout->expandHeight      = false;
		mVerLayout->expandWidth       = true;
		mVerLayout->layout->anchorMin = Vec2F(0, 0);
		mVerLayout->layout->anchorMax = Vec2F(1, 1);
		mVerLayout->layout->offsetMin = Vec2F();
		mVerLayout->layout->offsetMax = Vec2F();
		mVerLayout->fitByChildren     = true;

		AddChild(mVerLayout);

		InitializeProperties();
	}

	UICustomList::UICustomList(const UICustomList& other):
		UIScrollArea(other), DrawableCursorEventsListener(this), mHoverLayout(other.mHoverLayout),
		mSelectionLayout(other.mSelectionLayout)
	{
		mVerLayout = GetChildByType<UIVerticalLayout>();

		mItemSample = other.mItemSample->CloneAs<UIWidget>();
		mItemSample->UpdateLayout(true);

		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mHoverDrawable = other.mHoverDrawable->CloneAs<Sprite>();

		RetargetStatesAnimations();
		SetLayoutDirty();

		InitializeProperties();
	}

	UICustomList::~UICustomList()
	{
		delete mItemSample;
		delete mSelectionDrawable;
		delete mHoverDrawable;
	}

	UICustomList& UICustomList::operator=(const UICustomList& other)
	{
		CopyData(other);
		return *this;
	}

	void UICustomList::Update(float dt)
	{
		if (mFullyDisabled || mIsClipped)
			return;

		UIScrollArea::Update(dt);

		const float rectLerpCoef = 20.0f;

		if (mCurrentHoverRect != mTargetHoverRect)
		{
			mCurrentHoverRect = Math::Lerp(mCurrentHoverRect, mTargetHoverRect, dt*rectLerpCoef);
			mHoverDrawable->SetRect(mCurrentHoverRect);
		}
	}

	void UICustomList::Draw()
	{
		if (mFullyDisabled || mIsClipped)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		for (auto child : mDrawingChildren)
			child->Draw();

		for (auto& sel : mSelectedItems)
			sel.selection->Draw();

		mHoverDrawable->Draw();

		o2Render.DisableScissorTest();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (mOwnHorScrollBar)
			mHorScrollBar->Draw();

		if (mOwnVerScrollBar)
			mVerScrollBar->Draw();

		DrawDebugFrame();
	}

	void UICustomList::SetItemSample(UIWidget* sample)
	{
		RemoveAllItems();

		delete mItemSample;
		mItemSample = sample;

		SetLayoutDirty();
	}

	UIWidget* UICustomList::GetItemSample() const
	{
		return mItemSample;
	}

	UIVerticalLayout* UICustomList::GetLayout() const
	{
		return mVerLayout;
	}

	UIWidget* UICustomList::AddItem()
	{
		return mVerLayout->AddChildWidget(mItemSample->CloneAs<UIWidget>());
	}

	UIWidget* UICustomList::AddItem(int position)
	{
		position = Math::Max(0, position);

		for (int i = mVerLayout->GetChildren().Count(); i < position; i++)
			AddItem();

		return mVerLayout->AddChildWidget(mItemSample->CloneAs<UIWidget>(), position);
	}

	void UICustomList::RemoveItem(UIWidget* item)
	{
		mVerLayout->RemoveChild(item);
	}

	void UICustomList::RemoveItem(int position)
	{
		if (position < 0 || position >= mVerLayout->GetChildren().Count())
		{
			o2Debug.LogWarning("Failed to remove item at %i: out of range (%i)", position, mVerLayout->GetChildren().Count());
			return;
		}

		mVerLayout->RemoveChild(mVerLayout->GetChildren().Get(position));
	}

	void UICustomList::MoveItem(int position, int newPosition)
	{
		if (position < 0 || position >= mVerLayout->GetChildren().Count())
		{
			o2Debug.LogWarning("Failed to move item from %i to %i: out of range (%i)", position, newPosition,
							   mVerLayout->GetChildren().Count());
			return;
		}

		UIWidget* item = mVerLayout->GetChildWidgets().Get(position);
		mVerLayout->RemoveChild(item, false);
		mVerLayout->AddChild(item, newPosition);
	}

	void UICustomList::MoveItem(UIWidget* item, int newPosition)
	{
		mVerLayout->RemoveChild(item, false);
		mVerLayout->AddChild(item, newPosition);
	}

	int UICustomList::GetItemPosition(UIWidget* item)
	{
		int i = 0;
		for (auto child : mVerLayout->GetChildWidgets())
		{
			if (child == item)
				return i;

			i++;
		}

		return -1;
	}

	UIWidget* UICustomList::GetItem(int position) const
	{
		if (position < 0 || position >= mVerLayout->GetChildWidgets().Count())
			return nullptr;

		return mVerLayout->GetChildWidgets().Get(position);
	}

	void UICustomList::RemoveAllItems()
	{
		mVerLayout->RemoveAllChildren();
	}

	void UICustomList::SortItems(const Function<bool(UIWidget*, UIWidget*)>& sortFunc)
	{
		mVerLayout->mChildWidgets.Sort(sortFunc);
	}

	int UICustomList::GetItemsCount() const
	{
		return mVerLayout->GetChildWidgets().Count();
	}

	void UICustomList::SelectItem(UIWidget* item)
	{
		int itemPos = GetItemPosition(item);

		if (mSelectedItems.ContainsPred([=](auto x) { return x.idx == itemPos; }))
			return;

		if (!mMultiSelection)
			ClearSelection();

		Selection selection;
		selection.idx = itemPos;
		selection.selection = GetSelectionSprite();
		mSelectedItems.Add(selection);

		onSelectedPos(itemPos);
		onSelectedItem(item);
		OnSelectionChanged();
		SetLayoutDirty();
	}

	void UICustomList::SelectItemAt(int position)
	{
		if (!mMultiSelection)
			ClearSelection();

		if (position >= mVerLayout->GetChildWidgets().Count())
		{
			o2Debug.LogWarning("Can't select item at %i: out of range (%i)", position, GetItemsCount());
			return;
		}

		if (mSelectedItems.ContainsPred([=](auto x) { return x.idx == position; }))
			return;

		if (position >= 0)
		{
			Selection selection;
			selection.idx = position;
			selection.selection = GetSelectionSprite();
			mSelectedItems.Add(selection);

			onSelectedPos(position);
			onSelectedItem(GetItem(position));
		}

		OnSelectionChanged();
		SetLayoutDirty();
	}

	void UICustomList::SetSelectedItems(const Vector<int>& items)
	{
		for (auto x : items)
			SelectItemAt(x);
	}

	void UICustomList::ClearSelection()
	{
		for (auto& sel : mSelectedItems)
			mSelectionSpritesPool.Add(sel.selection);

		mSelectedItems.Clear();
	}

	Vector<int> UICustomList::GetSelectedItems() const
	{
		return mSelectedItems.Select<int>([](auto x) { return x.idx; });
	}

	int UICustomList::GetSelectedItemPos() const
	{
		if (mSelectedItems.IsEmpty())
			return -1;

		return mSelectedItems.Last().idx;
	}

	UIWidget* UICustomList::GetSelectedItem() const
	{
		if (mSelectedItems.IsEmpty())
			return nullptr;

		return GetItem(mSelectedItems.Last().idx);
	}

	void UICustomList::SetMultiselectionAvailable(bool available)
	{
		mMultiSelection = available;

		if (!mMultiSelection)
		{
			int count = mSelectedItems.Count();
			for (int i = 0; i < count - 1; i++)
			{
				mSelectionSpritesPool.Add(mSelectedItems.Last().selection);
				mSelectedItems.PopBack();
			}
		}
	}

	bool UICustomList::IsMultiselectionAvailable() const
	{
		return mMultiSelection;
	}

	Sprite* UICustomList::GetSelectionDrawable() const
	{
		return mSelectionDrawable;
	}

	Sprite* UICustomList::GetHoverDrawable() const
	{
		return mHoverDrawable;
	}

	void UICustomList::SetSelectionDrawableLayout(const Layout& layout)
	{
		mSelectionLayout = layout;
	}

	Layout UICustomList::GetSelectionDrawableLayout() const
	{
		return mSelectionLayout;
	}

	void UICustomList::SetHoverDrawableLayout(const Layout& layout)
	{
		mHoverLayout = layout;
	}

	Layout UICustomList::GetHoverDrawableLayout() const
	{
		return mHoverLayout;
	}

	bool UICustomList::IsScrollable() const
	{
		return mEnableHorScroll || mEnableVerScroll;
	}

	void UICustomList::MoveScrollPosition(const Vec2F& delta)
	{
		UIScrollArea::MoveScrollPosition(delta);
		UpdateSelectionSprites();
	}

	void UICustomList::UpdateControls(float dt)
	{}

	void UICustomList::UpdateLayout(bool withChildren /*= true*/)
	{
		UIScrollArea::UpdateLayout(withChildren);

		if (Input::IsSingletonInitialzed())
			UpdateHover(o2Input.cursorPos);

		mCurrentHoverRect = mTargetHoverRect;
		mHoverDrawable->SetRect(mCurrentHoverRect);

		UpdateSelectionSprites();
	}

	void UICustomList::UpdateSelectionSprites()
	{
		for (auto& sel : mSelectedItems)
		{
			UIWidget* item = GetItem(sel.idx);
			sel.selection->SetRect(mSelectionLayout.Calculate(item->layout->worldRect));
		}
	}

	void UICustomList::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;
	}

	void UICustomList::OnCursorStillDown(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastSelectCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastSelectCheckCursor = cursor.position;
	}

	void UICustomList::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		int itemIdx = -1;
		UIWidget* itemUnderCursor = GetItemUnderPoint(cursor.position, &itemIdx);
		SelectItemAt(itemIdx);

		OnSelectionChanged();
	}

	void UICustomList::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void UICustomList::OnCursorExit(const Input::Cursor& cursor)
	{
		auto hoverState = state["hover"];
		if (hoverState)
			*hoverState = false;
		else
			mHoverDrawable->SetEnabled(false);
	}

	void UICustomList::OnCursorMoved(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastHoverCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastHoverCheckCursor = cursor.position;

		UpdateHover(cursor.position);
	}

	UIWidget* UICustomList::GetItemUnderPoint(const Vec2F& point, int* idxPtr)
	{
		if (!mVerLayout)
			return nullptr;

		int idx = 0;
		for (auto child : mVerLayout->mChildWidgets)
		{
			if (child->layout->IsPointInside(point))
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

	void UICustomList::CopyData(const Actor& otherActor)
	{
		const UICustomList& other = dynamic_cast<const UICustomList&>(otherActor);

		delete mItemSample;
		delete mSelectionDrawable;
		delete mHoverDrawable;
		mVerLayout = nullptr;

		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mHoverDrawable = other.mHoverDrawable->CloneAs<Sprite>();

		mSelectionLayout = other.mSelectionLayout;
		mHoverLayout = other.mHoverLayout;

		UIScrollArea::CopyData(other);

		mVerLayout = GetChildByType<UIVerticalLayout>();
		mItemSample = other.mItemSample->CloneAs<UIWidget>();
		mItemSample->SetLayoutDirty();

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void UICustomList::OnDeserialized(const DataNode& node)
	{
		UIScrollArea::OnDeserialized(node);
		RetargetStatesAnimations();
	}

	void UICustomList::UpdateTransparency()
	{
		UIWidget::UpdateTransparency();

		if (mHorScrollBar)
			mHorScrollBar->UpdateTransparency();

		if (mVerScrollBar)
			mVerScrollBar->UpdateTransparency();

		if (mHoverDrawable)
			mHoverDrawable->transparency = mResTransparency;

		for (auto& sel : mSelectedItems)
			sel.selection->transparency = mResTransparency;
	}

	void UICustomList::UpdateHover(const Vec2F& point)
	{
		int itemIdx = -1;
		UIWidget* itemUnderCursor = GetItemUnderPoint(point, &itemIdx);

		if (itemIdx < 0)
		{
			auto hoverState = state["hover"];
			if (hoverState)
			{
				mHoverDrawable->SetEnabled(true);
				*hoverState = false;
			}
			else mHoverDrawable->SetEnabled(false);
		}
		else
		{
			mTargetHoverRect = mHoverLayout.Calculate(itemUnderCursor->layout->worldRect);

			auto hoverState = state["hover"];
			if (hoverState)
			{
				mHoverDrawable->SetEnabled(true);
				*hoverState = true;
			}
			else mHoverDrawable->SetEnabled(true);
		}
	}

	Sprite* UICustomList::GetSelectionSprite()
	{
		if (mSelectionSpritesPool.IsEmpty())
		{
			const int poolStep = 5;
			for (int i = 0; i < poolStep; i++)
				mSelectionSpritesPool.Add(mSelectionDrawable->CloneAs<Sprite>());
		}

		return mSelectionSpritesPool.PopBack();
	}

	void UICustomList::OnScrolled(float scroll)
	{
		if (mVerScrollBar && mEnableVerScroll)
			mVerScrollBar->OnScrolled(scroll);
		else if (mHorScrollBar && mEnableVerScroll)
			mHorScrollBar->OnScrolled(scroll);
	}

	void UICustomList::OnSelectionChanged()
	{}

	void UICustomList::OnVisibleChanged()
	{
		SetInteractable(mResVisible);
	}

	void UICustomList::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UICustomList, selectedItems, SetSelectedItems, GetSelectedItems);
		INITIALIZE_PROPERTY(UICustomList, selectedItem, SelectItem, GetSelectedItem);
		INITIALIZE_PROPERTY(UICustomList, selectedItemPos, SelectItemAt, GetSelectedItemPos);
		INITIALIZE_ACCESSOR(UICustomList, item, GetItem);
		INITIALIZE_GETTER(UICustomList, itemsCount, GetItemsCount);
	}

	bool UICustomList::Selection::operator==(const Selection& other) const
	{
		return idx == other.idx;
	}
}

DECLARE_CLASS(o2::UICustomList);
