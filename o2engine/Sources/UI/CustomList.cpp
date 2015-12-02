#include "CustomList.h"

#include "Render/Render.h"
#include "Render/Sprite.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/VerticalLayout.h"
#include "UI/VerticalScrollBar.h"
#include "Utils/Debug.h"

namespace o2
{
	IOBJECT_CPP(UICustomList);

	UICustomList::UICustomList():
		UIScrollArea(), mSelectedItem(-1), mHoverLayout(Layout::Both()), mSelectionLayout(Layout::Both())
	{
		mItemSample = mnew UIWidget();
		mSelectionDrawable = mnew Sprite();
		mHoverDrawable = mnew Sprite();

		mVerLayout = mnew UIVerticalLayout();
		mVerLayout->baseCorner = BaseCorner::LeftTop;
		mVerLayout->name = "layout";
		mVerLayout->expandHeight = false;
		mVerLayout->expandWidth = true;
		mVerLayout->layout.anchorMin = Vec2F(0, 0);
		mVerLayout->layout.anchorMax = Vec2F(1, 1);
		mVerLayout->layout.offsetMin = Vec2F();
		mVerLayout->layout.offsetMax = Vec2F();
		mVerLayout->fitByChildren = true;

		AddChild(mVerLayout);

		InitializeProperties();
	}

	UICustomList::UICustomList(const UICustomList& other):
		UIScrollArea(other), mSelectedItem(-1), mHoverLayout(other.mHoverLayout), mSelectionLayout(other.mSelectionLayout)
	{
		mVerLayout = FindChild<UIVerticalLayout>();
		mItemSample = other.mItemSample->Clone();
		mItemSample->UpdateLayout(true);
		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mHoverDrawable = other.mHoverDrawable->Clone();

		RetargetStatesAnimations();
		UpdateLayout();

		InitializeProperties();
	}

	UICustomList::~UICustomList()
	{
		mItemSample.Release();
		mSelectionDrawable.Release();
		mHoverDrawable.Release();
	}

	UICustomList& UICustomList::operator=(const UICustomList& other)
	{
		mItemSample.Release();
		mSelectionDrawable.Release();
		mHoverDrawable.Release();
		mVerLayout = nullptr;

		mSelectionDrawable = other.mSelectionDrawable->Clone();
		mHoverDrawable = other.mHoverDrawable->Clone();

		mSelectionLayout = other.mSelectionLayout;
		mHoverLayout = other.mHoverLayout;

		UIScrollArea::operator=(other);

		mVerLayout = FindChild<UIVerticalLayout>();
		mItemSample = other.mItemSample->Clone();
		mItemSample->UpdateLayout(true);

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UICustomList::Update(float dt)
	{
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

	void UICustomList::Draw()
	{
		if (mFullyDisabled)
			return;

		for (auto layer : mDrawingLayers)
			layer->Draw();

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

		if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
			DrawDebugFrame();
	}

	void UICustomList::SetItemSample(Ptr<UIWidget> sample)
	{
		RemoveAllItems();

		mItemSample.Release();
		mItemSample = sample;

		UpdateLayout();
	}

	Ptr<UIWidget> UICustomList::GetItemSample() const
	{
		return mItemSample;
	}

	Ptr<UIVerticalLayout> UICustomList::GetLayout() const
	{
		return mVerLayout;
	}

	Ptr<UIWidget> UICustomList::AddItem()
	{
		return mVerLayout->AddChild(mItemSample->Clone());
	}

	Ptr<UIWidget> UICustomList::AddItem(int position)
	{
		position = Math::Max(0, position);

		for (int i = mVerLayout->GetChilds().Count(); i < position; i++)
			AddItem();

		return mVerLayout->AddChild(mItemSample->Clone(), position);
	}

	void UICustomList::RemoveItem(Ptr<UIWidget> item)
	{
		mVerLayout->RemoveChild(item);
	}

	void UICustomList::RemoveItem(int position)
	{
		if (position < 0 || position >= mVerLayout->GetChilds().Count())
		{
			o2Debug.LogWarning("Failed to remove item at %i: out of range (%i)", position, mVerLayout->GetChilds().Count());
			return;
		}

		mVerLayout->RemoveChild(mVerLayout->GetChilds().Get(position));
	}

	void UICustomList::MoveItem(int position, int newPosition)
	{
		if (position < 0 || position >= mVerLayout->GetChilds().Count())
		{
			o2Debug.LogWarning("Failed to move item from %i to %i: out of range (%i)", position, newPosition,
							   mVerLayout->GetChilds().Count());
			return;
		}

		Ptr<UIWidget> item = mVerLayout->GetChilds().Get(position);
		mVerLayout->RemoveChild(item, false);
		mVerLayout->AddChild(item, newPosition);
	}

	void UICustomList::MoveItem(Ptr<UIWidget> item, int newPosition)
	{
		mVerLayout->RemoveChild(item, false);
		mVerLayout->AddChild(item, newPosition);
	}

	int UICustomList::GetItemPosition(Ptr<UIWidget> item)
	{
		int i = 0;
		for (auto child : mVerLayout->GetChilds())
		{
			if (child == item)
				return i;

			i++;
		}

		return -1;
	}

	Ptr<UIWidget> UICustomList::GetItem(int position) const
	{
		if (position < 0 || position >= mVerLayout->GetChilds().Count())
			return nullptr;

		return mVerLayout->GetChilds().Get(position);
	}

	void UICustomList::RemoveAllItems()
	{
		mVerLayout->RemoveAllChilds();
	}

	void UICustomList::SortItems(const Function<bool(const Ptr<UIWidget>&, const Ptr<UIWidget>&)>& sortFunc)
	{
		mVerLayout->mChilds.Sort(sortFunc);
	}

	int UICustomList::GetItemsCount() const
	{
		return mVerLayout->GetChilds().Count();
	}

	void UICustomList::SelectItem(Ptr<UIWidget> item)
	{
		int lastSelected = mSelectedItem;

		mSelectedItem = GetItemPosition(item);

		if (lastSelected != mSelectedItem)
		{
			lastSelected = mSelectedItem;
			onSelectedPos(mSelectedItem);
			onSelectedItem(GetItem(mSelectedItem));
			OnSelectionChanged();
		}
	}

	void UICustomList::SelectItemAt(int position)
	{
		int lastSelected = mSelectedItem;

		if (position < 0 || position >= mVerLayout->GetChilds().Count())
		{
			o2Debug.LogWarning("Can't select item at %i: out of range (%i)", position, GetItemsCount());
			position = -1;
		}

		mSelectedItem = position;

		if (lastSelected != mSelectedItem)
		{
			lastSelected = mSelectedItem;
			onSelectedPos(mSelectedItem);
			onSelectedItem(GetItem(mSelectedItem));
			OnSelectionChanged();
		}
	}

	Ptr<UIWidget> UICustomList::GetSelectedItem() const
	{
		if (mSelectedItem < 0)
			return nullptr;

		return GetItem(mSelectedItem);
	}

	int UICustomList::GetSelectedItemPosition() const
	{
		return mSelectedItem;
	}

	Ptr<Sprite> UICustomList::GetSelectionDrawable() const
	{
		return mSelectionDrawable;
	}

	Ptr<Sprite> UICustomList::GetHoverDrawable() const
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

	bool UICustomList::IsUnderPoint(const Vec2F& point)
	{
		return mAbsoluteClipArea.IsInside(point);
	}

	float UICustomList::Depth()
	{
		return mDrawDepth;
	}

	bool UICustomList::IsScrollable() const
	{
		return mEnableHorScroll || mEnableVerScroll;
	}

	bool UICustomList::IsInteractable() const
	{
		return mResVisible && CursorEventsListener::IsInteractable();
	}

	void UICustomList::UpdateControls(float dt)
	{}

	void UICustomList::UpdateLayout(bool forcible /*= false*/)
	{
		UIScrollArea::UpdateLayout(forcible);

		if (Input::IsSingletonInitialzed())
			UpdateHover(o2Input.cursorPos);

		UpdateSelection(mSelectedItem, GetItem(mSelectedItem));

		mCurrentHoverRect = mTargetHoverRect;
		mCurrentSelectionRect = mTargetSelectionRect;

		mSelectionDrawable->SetRect(mCurrentSelectionRect);
		mHoverDrawable->SetRect(mCurrentHoverRect);
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
		if ((cursor.mPosition - mLastSelectCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastSelectCheckCursor = cursor.mPosition;

		int itemIdx = -1;
		Ptr<UIWidget> itemUnderCursor = GetItemUnderPoint(cursor.mPosition, &itemIdx);

		UpdateSelection(itemIdx, itemUnderCursor);
	}

	void UICustomList::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		int itemIdx = -1;
		Ptr<UIWidget> itemUnderCursor = GetItemUnderPoint(cursor.mPosition, &itemIdx);
		
		onSelectedItem(itemUnderCursor);
		onSelectedPos(itemIdx);
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
		if ((cursor.mPosition - mLastHoverCheckCursor).Length() < checkDeltaThreshold)
			return;

		mLastHoverCheckCursor = cursor.mPosition;

		UpdateHover(cursor.mPosition);
	}

	Ptr<UIWidget> UICustomList::GetItemUnderPoint(const Vec2F& point, int* idxPtr)
	{
		if (!mVerLayout)
			return nullptr;

		int idx = 0;
		for (auto child : mVerLayout->mChilds)
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

		if (mSelectionDrawable)
			mSelectionDrawable->transparency = mResTransparency;

		if (mHoverDrawable)
			mHoverDrawable->transparency = mResTransparency;
	}

	void UICustomList::UpdateHover(const Vec2F& point)
	{
		int itemIdx = -1;
		Ptr<UIWidget> itemUnderCursor = GetItemUnderPoint(point, &itemIdx);

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

	void UICustomList::UpdateSelection(int position, Ptr<UIWidget> item)
	{
		mSelectedItem = position;

		if (position < 0)
		{
			auto selectedState = state["selected"];
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

			auto selectedState = state["selected"];
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
		INITIALIZE_PROPERTY(UICustomList, selectedItem, SelectItem, GetSelectedItem);
		INITIALIZE_PROPERTY(UICustomList, selectedItemPos, SelectItemAt, GetSelectedItemPosition);
		INITIALIZE_ACCESSOR(UICustomList, item, GetItem);
		INITIALIZE_GETTER(UICustomList, itemsCount, GetItemsCount);
	}

}