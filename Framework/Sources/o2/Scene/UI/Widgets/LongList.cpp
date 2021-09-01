#include "o2/stdafx.h"
#include "LongList.h"

#include "o2/Render/Render.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"

namespace o2
{
	LongList::LongList():
		ScrollArea(), mHoverLayout(Layout::BothStretch()),
		mSelectionLayout(Layout::BothStretch())
	{
		mItemSample = mnew Widget();
		mSelectionDrawable = mnew Sprite();
		mHoverDrawable = mnew Sprite();
	}

	LongList::LongList(const LongList& other):
		ScrollArea(other), mHoverLayout(other.mHoverLayout),
		mSelectionLayout(other.mSelectionLayout), selectedItemPos(this)
	{
		mItemSample = other.mItemSample->CloneAs<Widget>();
		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mHoverDrawable = other.mHoverDrawable->CloneAs<Sprite>();

		mItemSample->UpdateTransform();

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	LongList::~LongList()
	{
		delete mItemSample;
		delete mSelectionDrawable;
		delete mHoverDrawable;
	}

	LongList& LongList::operator=(const LongList& other)
	{
		delete mItemSample;
		delete mSelectionDrawable;
		delete mHoverDrawable;

		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mHoverDrawable = other.mHoverDrawable->CloneAs<Sprite>();

		mSelectionLayout = other.mSelectionLayout;
		mHoverLayout = other.mHoverLayout;

		ScrollArea::operator=(other);

		mItemSample = other.mItemSample->CloneAs<Widget>();
		mItemSample->UpdateSelfTransform();
		mItemSample->UpdateChildrenTransforms();

		RetargetStatesAnimations();
		SetLayoutDirty();

		return *this;
	}

	void LongList::Update(float dt)
	{
		ScrollArea::Update(dt);

		if (!mResEnabledInHierarchy || mIsClipped)
			return;

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

	void LongList::Draw()
	{
		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

		IDrawable::OnDrawn();

		o2Render.EnableScissorTest(mAbsoluteClipArea);

		for (auto child : mDrawingChildren)
			child->Draw();

		mSelectionDrawable->Draw();
		mHoverDrawable->Draw();

		o2Render.DisableScissorTest();

		CursorAreaEventsListener::OnDrawn();

		for (auto layer : mTopDrawingLayers)
			layer->Draw();

		if (mOwnHorScrollBar)
			mHorScrollBar->Draw();

		if (mOwnVerScrollBar)
			mVerScrollBar->Draw();

		DrawDebugFrame();
	}

	void LongList::SetItemSample(Widget* sample)
	{
		delete mItemSample;
		mItemSample = sample;

		SetLayoutDirty();
	}

	Widget* LongList::GetItemSample() const
	{
		return mItemSample;
	}

	void LongList::SelectItemAt(int position)
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

	int LongList::GetSelectedItemPosition() const
	{
		return mSelectedItem;
	}

	Sprite* LongList::GetSelectionDrawable() const
	{
		return mSelectionDrawable;
	}

	Sprite* LongList::GetHoverDrawable() const
	{
		return mHoverDrawable;
	}

	void LongList::SetSelectionDrawableLayout(const Layout& layout)
	{
		mSelectionLayout = layout;
	}

	Layout LongList::GetSelectionDrawableLayout() const
	{
		return mSelectionLayout;
	}

	void LongList::SetHoverDrawableLayout(const Layout& layout)
	{
		mHoverLayout = layout;
	}

	Layout LongList::GetHoverDrawableLayout() const
	{
		return mHoverLayout;
	}

	void LongList::OnItemsUpdated(bool itemsRearranged /*= false*/)
	{
		if (itemsRearranged)
		{
			auto countFunc = getItemsCountFunc;
			getItemsCountFunc = []() { return 0; };
			UpdateSelfTransform();
			UpdateChildrenTransforms();
			getItemsCountFunc = countFunc;
		}

		SetLayoutDirty();
	}

	void LongList::CalculateScrollArea()
	{
		Vec2F offset;
		InitializeScrollAreaRectCalculation(offset);

		float itemsHeight = (float)getItemsCountFunc()*mItemSample->layout->GetMinHeight();
		RecalculateScrollAreaRect(RectF(0, mAbsoluteViewArea.Height(), mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height() - itemsHeight), Vec2F());
	}

	void LongList::UpdateSelfTransform()
{
		UpdateVisibleItems();

		ScrollArea::UpdateSelfTransform();

		if (Input::IsSingletonInitialzed())
			UpdateHover(o2Input.cursorPos);

		UpdateSelection(mSelectedItem);

		mCurrentHoverRect = mTargetHoverRect;
		mCurrentSelectionRect = mTargetSelectionRect;

		mSelectionDrawable->SetRect(mCurrentSelectionRect);
		mHoverDrawable->SetRect(mCurrentHoverRect);
	}

	void LongList::MoveScrollPosition(const Vec2F& delta)
	{
		ScrollArea::MoveScrollPosition(delta);

		UpdateVisibleItems();

		mTargetSelectionRect += delta;
		mTargetHoverRect += delta;

		mCurrentSelectionRect = mTargetSelectionRect;
		mCurrentHoverRect = mTargetHoverRect;

		mSelectionDrawable->SetRect(mCurrentSelectionRect);
		mHoverDrawable->SetRect(mCurrentHoverRect);
	}

	void LongList::UpdateVisibleItems()
	{
		int lastMinItemIdx = mMinVisibleItemIdx;
		int lastMaxItemIdx = mMaxVisibleItemIdx;

		float itemHeight = mItemSample->layout->minHeight;

		if (itemHeight < FLT_EPSILON)
			return;

		mMinVisibleItemIdx = Math::FloorToInt(mScrollPos.y / itemHeight);
		mMaxVisibleItemIdx = Math::FloorToInt((mScrollPos.y + mAbsoluteViewArea.Height()) / itemHeight);
		mMaxVisibleItemIdx = Math::Min(mMaxVisibleItemIdx, getItemsCountFunc() - 1);

		auto itemsInRange = getItemsRangeFunc(mMinVisibleItemIdx, mMaxVisibleItemIdx + 1);
		Vector<Widget*> itemsWidgets;
		itemsWidgets.Resize(mMaxVisibleItemIdx - mMinVisibleItemIdx + 1);

		Vector<Widget*> removingItems;
		for (int i = lastMinItemIdx; i <= lastMaxItemIdx; i++)
		{
			if (i < 0)
				continue;

			if (i < mMinVisibleItemIdx || i > mMaxVisibleItemIdx)
				removingItems.Add(mChildWidgets[i - lastMinItemIdx]);
			else
				itemsWidgets[i - mMinVisibleItemIdx] = mChildWidgets[i - lastMinItemIdx];
		}

		for (auto item : removingItems)
			mItemsPool.Add(item);

		mChildren.Clear();
		mChildWidgets.Clear();
		mDrawingChildren.Clear();

		for (int i = mMinVisibleItemIdx; i <= mMaxVisibleItemIdx; i++)
		{
			if (i < 0)
				continue;

			if (i >= lastMinItemIdx && i <= lastMaxItemIdx)
				continue;

			if (mItemsPool.Count() == 0)
			{
				for (int j = 0; j < 10; j++)
				{
					Widget* newItem = mItemSample->CloneAs<Widget>();
					newItem->RemoveFromScene();
					mItemsPool.Add(newItem);
				}
			}

			Widget* newItem = mItemsPool.PopBack();

			setupItemFunc(newItem, itemsInRange[i - mMinVisibleItemIdx]);

			*newItem->layout = WidgetLayout::HorStretch(VerAlign::Top, 0, 0, itemHeight, itemHeight*(float)i);

			newItem->mParent = this;
			newItem->mParentWidget = this;

			itemsWidgets[i - mMinVisibleItemIdx] = newItem;
		}

		mChildren.Add(itemsWidgets.Cast<Actor*>());
		mChildWidgets.Add(itemsWidgets);
		mDrawingChildren.Add(itemsWidgets);

		for (auto child : mChildWidgets)
		{
			child->UpdateSelfTransform();
			child->UpdateChildrenTransforms();
			child->mIsClipped = false;
		}
	}

	void LongList::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;
	}

	void LongList::OnCursorStillDown(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastSelectCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastSelectCheckCursor = cursor.position;

		int itemIdx = -1;
		GetItemUnderPoint(cursor.position, &itemIdx);
		UpdateSelection(itemIdx);
	}

	void LongList::OnCursorMoved(const Input::Cursor& cursor)
	{
		const float checkDeltaThreshold = 2.0f;
		if ((cursor.position - mLastHoverCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastHoverCheckCursor = cursor.position;

		UpdateHover(cursor.position);
	}

	void LongList::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		int itemIdx = -1;
		GetItemUnderPoint(cursor.position, &itemIdx);

		onFocused(itemIdx);
		OnSelectionChanged();
	}

	void LongList::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void LongList::OnCursorExit(const Input::Cursor& cursor)
	{
		auto hoverState = state["hover"];
		if (hoverState)
			*hoverState = false;
		else
			mHoverDrawable->SetEnabled(false);
	}

	Widget* LongList::GetItemUnderPoint(const Vec2F& point, int* idxPtr)
	{
		int idx = mMinVisibleItemIdx;
		for (auto child : mChildWidgets)
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

	void LongList::OnDeserialized(const DataValue& node)
	{
		ScrollArea::OnDeserialized(node);
		RetargetStatesAnimations();
	}

	void LongList::UpdateTransparency()
	{
		Widget::UpdateTransparency();

		if (mHorScrollBar)
			mHorScrollBar->UpdateTransparency();

		if (mVerScrollBar)
			mVerScrollBar->UpdateTransparency();

		if (mSelectionDrawable)
			mSelectionDrawable->transparency = mResTransparency;

		if (mHoverDrawable)
			mHoverDrawable->transparency = mResTransparency;
	}

	void LongList::UpdateHover(const Vec2F& point)
	{
		int itemIdx = -1;
		Widget* itemUnderCursor = GetItemUnderPoint(point, &itemIdx);

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
			mTargetHoverRect = mHoverLayout.Calculate(itemUnderCursor->layout->worldRect);

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

	void LongList::UpdateSelection(int position)
	{
		mSelectedItem = position;
		Widget* item = nullptr;
		if (position < mMaxVisibleItemIdx && position >= mMinVisibleItemIdx)
			item = mChildWidgets[position - mMinVisibleItemIdx];

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
			mTargetSelectionRect = mHoverLayout.Calculate(item->layout->worldRect);

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

	void LongList::OnSelectionChanged()
	{}

	void LongList::OnEnableInHierarchyChanged()
	{
		SetInteractable(mResEnabled);
	}

	String LongList::GetCreateMenuGroup()
	{
		return "List";
	}
}

DECLARE_CLASS(o2::LongList);
