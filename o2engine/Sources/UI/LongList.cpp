#include "LongList.h"

#include "Render/Render.h"

namespace o2
{
	UILongList::UILongList():
		UIScrollArea(), DrawableCursorEventsListener(this), mHoverLayout(Layout::BothStretch()),
		mSelectionLayout(Layout::BothStretch())
	{
		mItemSample = mnew UIWidget();
		mSelectionDrawable = mnew Sprite();
		mHoverDrawable = mnew Sprite();

		InitializeProperties();
	}

	UILongList::UILongList(const UILongList& other):
		UIScrollArea(other), DrawableCursorEventsListener(this), mHoverLayout(other.mHoverLayout),
		mSelectionLayout(other.mSelectionLayout)
	{
		mItemSample = other.mItemSample->Clone();
		mItemSample->UpdateLayout(true);
		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mHoverDrawable = other.mHoverDrawable->Clone();

		RetargetStatesAnimations();
		UpdateLayout();

		InitializeProperties();
	}

	UILongList::~UILongList()
	{
		delete mItemSample;
		delete mSelectionDrawable;
		delete mHoverDrawable;
	}

	UILongList& UILongList::operator=(const UILongList& other)
	{
		delete mItemSample;
		delete mSelectionDrawable;
		delete mHoverDrawable;

		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mHoverDrawable = other.mHoverDrawable->Clone();

		mSelectionLayout = other.mSelectionLayout;
		mHoverLayout = other.mHoverLayout;

		UIScrollArea::operator=(other);

		mItemSample = other.mItemSample->Clone();
		mItemSample->UpdateLayout(true);

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UILongList::Update(float dt)
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

		if (mCurrentSelectionRect != mTargetSelectionRect)
		{
			mCurrentSelectionRect = Math::Lerp(mCurrentSelectionRect, mTargetSelectionRect, dt*rectLerpCoef);
			mSelectionDrawable->SetRect(mCurrentSelectionRect);
		}
	}

	void UILongList::Draw()
	{
		if (mFullyDisabled || mIsClipped)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		for (auto child : mChilds)
			child->Draw();

		mSelectionDrawable->Draw();
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

	void UILongList::SetItemSample(UIWidget* sample)
	{
		delete mItemSample;
		mItemSample = sample;

		UpdateLayout();
	}

	UIWidget* UILongList::GetItemSample() const
	{
		return mItemSample;
	}

	void UILongList::SelectItemAt(int position)
	{
		int lastSelected = mSelectedItem;

		int itemsCount = getItemsCountFunc();
		if (position < 0 || position >= itemsCount)
		{
			o2Debug.LogWarning("Can't select item at %i: out of range (%i)", position, itemsCount);
			position = -1;
		}

		mSelectedItem = position;

		if (lastSelected != mSelectedItem)
		{
			lastSelected = mSelectedItem;
			onFocused(mSelectedItem);
			OnSelectionChanged();
		}
	}

	int UILongList::GetSelectedItemPosition() const
	{
		return mSelectedItem;
	}

	Sprite* UILongList::GetSelectionDrawable() const
	{
		return mSelectionDrawable;
	}

	Sprite* UILongList::GetHoverDrawable() const
	{
		return mHoverDrawable;
	}

	void UILongList::SetSelectionDrawableLayout(const Layout& layout)
	{
		mSelectionLayout = layout;
	}

	Layout UILongList::GetSelectionDrawableLayout() const
	{
		return mSelectionLayout;
	}

	void UILongList::SetHoverDrawableLayout(const Layout& layout)
	{
		mHoverLayout = layout;
	}

	Layout UILongList::GetHoverDrawableLayout() const
	{
		return mHoverLayout;
	}

	bool UILongList::IsScrollable() const
	{
		return mEnableHorScroll || mEnableVerScroll;
	}

	void UILongList::OnItemsUpdated(bool itemsRearranged /*= false*/)
	{
		if (itemsRearranged)
		{
			auto countFunc = getItemsCountFunc;
			getItemsCountFunc = []() { return 0; };
			UpdateLayout();
			getItemsCountFunc = countFunc;
		}

		UpdateLayout();
	}

	void UILongList::CalculateScrollArea()
	{
		mScrollArea = RectF(0.0f, 0.0f, mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height());
		mScrollArea.bottom = Math::Min(mScrollArea.bottom,
									   mScrollArea.top - (float)getItemsCountFunc()*mItemSample->layout.GetHeight());
	}

	void UILongList::UpdateControls(float dt)
	{}

	void UILongList::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (CheckIsLayoutDrivenByParent(forcible))
			return;

		UpdateVisibleItems();

		UIScrollArea::UpdateLayout(forcible, withChildren);

		if (Input::IsSingletonInitialzed())
			UpdateHover(o2Input.cursorPos);

		UpdateSelection(mSelectedItem);

		mCurrentHoverRect = mTargetHoverRect;
		mCurrentSelectionRect = mTargetSelectionRect;

		mSelectionDrawable->SetRect(mCurrentSelectionRect);
		mHoverDrawable->SetRect(mCurrentHoverRect);
	}

	void UILongList::MoveScrollPosition(const Vec2F& delta)
	{
		mScrollPos += delta;

		Vec2F roundedScrollPos(-Math::Round(mScrollPos.x), Math::Round(mScrollPos.y));
		mChildsAbsRect = mAbsoluteViewArea + roundedScrollPos;

		UpdateVisibleItems();

		Vec2F widgetsMove(-delta.x, delta.y);
		for (auto child : mChilds)
			MoveWidgetAndCheckClipping(child, widgetsMove);

		UpdateScrollParams();
		UpdateScrollBarsLayout();
	}

	void UILongList::UpdateVisibleItems()
	{
		int lastMinItemIdx = mMinVisibleItemIdx;
		int lastMaxItemIdx = mMaxVisibleItemIdx;

		float itemHeight = mItemSample->layout.height;

		if (itemHeight < FLT_EPSILON)
			return;

		mMinVisibleItemIdx = Math::FloorToInt(mScrollPos.y / itemHeight);
		mMaxVisibleItemIdx = Math::FloorToInt((mScrollPos.y + mAbsoluteViewArea.Height()) / itemHeight);
		mMaxVisibleItemIdx = Math::Min(mMaxVisibleItemIdx, getItemsCountFunc() - 1);

		auto itemsInRange = getItemsRangeFunc(mMinVisibleItemIdx, mMaxVisibleItemIdx + 1);
		Vector<UIWidget*> itemsWidgets;
		itemsWidgets.Resize(mMaxVisibleItemIdx - mMinVisibleItemIdx + 1);

		WidgetsVec removingItems;
		for (int i = lastMinItemIdx; i <= lastMaxItemIdx; i++)
		{
			if (i < 0)
				continue;

			if (i < mMinVisibleItemIdx || i > mMaxVisibleItemIdx)
				removingItems.Add(mChilds[i - lastMinItemIdx]);
			else
				itemsWidgets[i - mMinVisibleItemIdx] = mChilds[i - lastMinItemIdx];
		}

		for (auto item : removingItems)
			mItemsPool.Add(item);

		mChilds.Clear();

		for (int i = mMinVisibleItemIdx; i <= mMaxVisibleItemIdx; i++)
		{
			if (i < 0)
				continue;

			if (i >= lastMinItemIdx && i <= lastMaxItemIdx)
				continue;

			if (mItemsPool.Count() == 0)
			{
				for (int j = 0; j < 10; j++)
					mItemsPool.Add(mItemSample->Clone());
			}

			UIWidget* newItem = mItemsPool.PopBack();
			setupItemFunc(newItem, itemsInRange[i - mMinVisibleItemIdx]);
			newItem->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, itemHeight, itemHeight*(float)i);
			itemsWidgets[i - mMinVisibleItemIdx] = newItem;
			newItem->mParent = this;
		}

		mChilds.Add(itemsWidgets);
	}

	void UILongList::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;
	}

	void UILongList::OnCursorStillDown(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastSelectCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastSelectCheckCursor = cursor.position;

		int itemIdx = -1;
		GetItemUnderPoint(cursor.position, &itemIdx);
		UpdateSelection(itemIdx);
	}

	void UILongList::OnCursorMoved(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastHoverCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastHoverCheckCursor = cursor.position;

		UpdateHover(cursor.position);
	}

	void UILongList::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		int itemIdx = -1;
		UIWidget* itemUnderCursor = GetItemUnderPoint(cursor.position, &itemIdx);

		onFocused(itemIdx);
		OnSelectionChanged();
	}

	void UILongList::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void UILongList::OnCursorExit(const Input::Cursor& cursor)
	{
		auto hoverState = state["hover"];
		if (hoverState)
			*hoverState = false;
		else
			mHoverDrawable->SetEnabled(false);
	}

	void UILongList::OnScrolled(float scroll)
	{
		if (mVerScrollBar && mEnableVerScroll)
			mVerScrollBar->OnScrolled(scroll);
		else if (mHorScrollBar && mEnableVerScroll)
			mHorScrollBar->OnScrolled(scroll);
	}

	UIWidget* UILongList::GetItemUnderPoint(const Vec2F& point, int* idxPtr)
	{
		int idx = mMinVisibleItemIdx;
		for (auto child : mChilds)
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

	void UILongList::OnDeserialized(const DataNode& node)
	{
		UIScrollArea::OnDeserialized(node);
		RetargetStatesAnimations();
	}

	void UILongList::UpdateTransparency()
	{
		UIWidget::UpdateTransparency();

		if (mHorScrollBar)
			mHorScrollBar->UpdateTransparency();

		if (mVerScrollBar)
			mVerScrollBar->UpdateTransparency();

		if (mSelectionDrawable)
			mSelectionDrawable->transparency = mResTransparency;

		if (mHoverDrawable)
			mHoverDrawable->transparency = mResTransparency;
	}

	void UILongList::UpdateHover(const Vec2F& point)
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
			else
				mHoverDrawable->SetEnabled(false);
		}
		else
		{
			mTargetHoverRect = mHoverLayout.Calculate(itemUnderCursor->layout.mAbsoluteRect);

			auto hoverState = state["hover"];
			if (hoverState)
			{
				mHoverDrawable->SetEnabled(true);
				*hoverState = itemIdx != mSelectedItem;
			}
			else
				mHoverDrawable->SetEnabled(itemIdx != mSelectedItem);
		}
	}

	void UILongList::UpdateSelection(int position)
	{
		mSelectedItem = position;
		UIWidget* item = nullptr;
		if (position < mMaxVisibleItemIdx && position >= mMinVisibleItemIdx)
			item = mChilds[position - mMinVisibleItemIdx];

		if (position < 0 || item == nullptr)
		{
			auto selectedState = state["focused"];
			if (selectedState)
			{
				mSelectionDrawable->SetEnabled(true);
				*selectedState = false;
			}
			else
			{
				if (mSelectionDrawable)
					mSelectionDrawable->SetEnabled(false);
			}
		}
		else
		{
			mTargetSelectionRect = mHoverLayout.Calculate(item->layout.mAbsoluteRect);

			auto selectedState = state["focused"];
			if (selectedState)
			{
				mSelectionDrawable->SetEnabled(true);
				*selectedState = true;
			}
			else
			{
				if (mSelectionDrawable)
					mSelectionDrawable->SetEnabled(false);
			}

			UpdateHover(o2Input.cursorPos);
		}
	}

	void UILongList::OnSelectionChanged()
	{}

	void UILongList::OnVisibleChanged()
	{
		SetInteractable(mResVisible);
	}

	void UILongList::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UILongList, selectedItemPos, SelectItemAt, GetSelectedItemPosition);
	}
}