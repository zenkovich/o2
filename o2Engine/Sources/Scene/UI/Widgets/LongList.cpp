#include "stdafx.h"
#include "LongList.h"

#include "Render/Render.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"

namespace o2
{
	UILongList::UILongList():
		UIScrollArea(), DrawableCursorEventsListener(this), mHoverLayout(Layout::BothStretch()),
		mSelectionLayout(Layout::BothStretch())
	{
		mItemSample = mnew UIWidget();
		mSelectionDrawable = mnew Sprite();
		mHoverDrawable = mnew Sprite();
	}

	UILongList::UILongList(const UILongList& other):
		UIScrollArea(other), DrawableCursorEventsListener(this), mHoverLayout(other.mHoverLayout),
		mSelectionLayout(other.mSelectionLayout), selectedItemPos(this)
	{
		mItemSample = other.mItemSample->CloneAs<UIWidget>();
		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mHoverDrawable = other.mHoverDrawable->CloneAs<Sprite>();

		mItemSample->UpdateTransform();

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	UILongList::~UILongList()
	{
		delete mItemSample;
		delete mSelectionDrawable;
		delete mHoverDrawable;
	}

	UILongList& UILongList::operator=(const UILongList& other)
	{
		UIScrollArea::operator=(other);
		return *this;
	}

	void UILongList::Update(float dt)
	{
		UIScrollArea::Update(dt);

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

	void UILongList::Draw()
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

		SetLayoutDirty();
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
			UpdateSelfTransform();
			UpdateChildrenTransforms();
			getItemsCountFunc = countFunc;
		}

		SetLayoutDirty();
	}

	void UILongList::CalculateScrollArea()
	{
		Vec2F offset;
		InitializeScrollAreaRectCalculation(offset);

		float itemsHeight = (float)getItemsCountFunc()*mItemSample->layout->GetMinimalHeight();
		RecalculateScrollAreaRect(RectF(0, mAbsoluteViewArea.Height(), mAbsoluteViewArea.Width(), mAbsoluteViewArea.Height() - itemsHeight), Vec2F());
	}

	void UILongList::UpdateControls(float dt)
	{}

	void UILongList::UpdateSelfTransform()
{
		UpdateVisibleItems();

		UIScrollArea::UpdateSelfTransform();

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
		mChildrenWorldRect = mAbsoluteViewArea + roundedScrollPos;

		UpdateVisibleItems();

		for (auto child : mChildWidgets)
		{
			child->UpdateSelfTransform();
			child->UpdateChildrenTransforms();
			child->mIsClipped = false;
		}

		mChildrenWorldRect = mAbsoluteViewArea;

		mTargetSelectionRect += delta;
		mTargetHoverRect += delta;

		mCurrentSelectionRect = mTargetSelectionRect;
		mCurrentHoverRect = mTargetHoverRect;

		mSelectionDrawable->SetRect(mCurrentSelectionRect);
		mHoverDrawable->SetRect(mCurrentHoverRect);

		UpdateScrollParams();
	}

	void UILongList::UpdateVisibleItems()
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
		Vector<UIWidget*> itemsWidgets;
		itemsWidgets.Resize(mMaxVisibleItemIdx - mMinVisibleItemIdx + 1);

		WidgetsVec removingItems;
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
					UIWidget* newItem = mItemSample->CloneAs<UIWidget>();
					newItem->ExcludeFromScene();
					mItemsPool.Add(newItem);
				}
			}

			UIWidget* newItem = mItemsPool.PopBack();

			setupItemFunc(newItem, itemsInRange[i - mMinVisibleItemIdx]);

			*newItem->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, itemHeight, itemHeight*(float)i);

			newItem->mParent = this;
			newItem->mParentWidget = this;

			itemsWidgets[i - mMinVisibleItemIdx] = newItem;
		}

		mChildren.Add(itemsWidgets.Cast<Actor*>());
		mChildWidgets.Add(itemsWidgets);
		mDrawingChildren.Add(itemsWidgets);
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

	void UILongList::CopyData(const Actor& otherActor)
	{
		const UILongList& other = dynamic_cast<const UILongList&>(otherActor);

		delete mItemSample;
		delete mSelectionDrawable;
		delete mHoverDrawable;

		mSelectionDrawable = other.mSelectionDrawable->CloneAs<Sprite>();
		mHoverDrawable = other.mHoverDrawable->CloneAs<Sprite>();

		mSelectionLayout = other.mSelectionLayout;
		mHoverLayout = other.mHoverLayout;

		UIScrollArea::CopyData(other);

		mItemSample = other.mItemSample->CloneAs<UIWidget>();
		mItemSample->UpdateSelfTransform();
		mItemSample->UpdateChildrenTransforms();

		RetargetStatesAnimations();
		SetLayoutDirty();
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

	void UILongList::UpdateSelection(int position)
	{
		mSelectedItem = position;
		UIWidget* item = nullptr;
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

	void UILongList::OnSelectionChanged()
	{}

	void UILongList::OnResEnableInHierarchyChanged()
	{
		SetInteractable(mResEnabled);
	}
}

DECLARE_CLASS(o2::UILongList);
