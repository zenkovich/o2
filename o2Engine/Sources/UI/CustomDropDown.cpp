#include "stdafx.h"
#include "CustomDropDown.h"

#include "Render/Render.h"
#include "UI/CustomList.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace o2
{
	UICustomDropDown::UICustomDropDown():
		UIWidget(), DrawableCursorEventsListener(this)
	{
		mItemsList = mnew UICustomList();
		mItemsList->SetInternalParent(this, false);
		mItemsList->onSelectedItem += [&](auto x) { OnItemSelected(); };
		mItemsList->SetMultiselectionAvailable(false);
	}

	UICustomDropDown::UICustomDropDown(const UICustomDropDown& other):
		UIWidget(other), DrawableCursorEventsListener(this), mClipLayout(other.mClipLayout),
		mMaxListItems(other.mMaxListItems), selectedItem(this), selectedItemPos(this), itemsCount(this)
	{
		mItemsList = other.mItemsList->CloneAs<UICustomList>();
		mItemsList->SetInternalParent(this, false);
		mItemsList->Hide(true);
		mItemsList->onSelectedItem += [&](auto x) { OnItemSelected(); };
		mItemsList->SetMultiselectionAvailable(false);

		RetargetStatesAnimations();
	}

	UICustomDropDown::~UICustomDropDown()
	{}

	UICustomDropDown& UICustomDropDown::operator=(const UICustomDropDown& other)
	{
		UIWidget::operator=(other);
		return *this;
	}

	void UICustomDropDown::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		UIWidget::Draw();

		o2UI.DrawWidgetAtTop(mItemsList);

		UIWidget* selectedItem = mItemsList->GetItem(mItemsList->GetSelectedItemPos());
		if (selectedItem)
		{
			o2Render.EnableScissorTest(mAbsoluteClip);
			selectedItem->ForceDraw(mAbsoluteClip, mResTransparency);
			o2Render.DisableScissorTest();
		}

		DrawDebugFrame();
	}

	void UICustomDropDown::Expand()
	{
		float itemHeight   = mItemsList->GetItemSample()->layout->minHeight;
		int itemsVisible   = Math::Min(mMaxListItems, mItemsList->GetItemsCount());
		RectF listViewArea = mItemsList->mAbsoluteViewArea;
		RectF listAbsRect  = mItemsList->layout->worldRect;

		RectF border(listViewArea.left - listAbsRect.left, listViewArea.bottom - listAbsRect.bottom,
					 listAbsRect.right - listViewArea.right, listAbsRect.top - listViewArea.top);

		mItemsList->layout->height = itemHeight*(float)itemsVisible + border.bottom + border.top;

		auto openedState = state["opened"];
		if (openedState)
			*openedState = true;

		mItemsList->SetEnabled(true);
		mItemsList->UpdateTransform(true);

		SetLayoutDirty();
	}

	void UICustomDropDown::Collapse()
	{
		auto openedState = state["opened"];
		if (openedState)
			*openedState = false;

		mItemsList->SetEnabled(false);
	}

	bool UICustomDropDown::IsExpanded() const
	{
		return mItemsList->IsEnabled();
	}

	void UICustomDropDown::SetItemSample(UIWidget* sample)
	{
		mItemsList->SetItemSample(sample);
	}

	UIWidget* UICustomDropDown::GetItemSample() const
	{
		return mItemsList->GetItemSample();
	}

	UIVerticalLayout* UICustomDropDown::GetItemsLayout() const
	{
		return mItemsList->GetItemsLayout();
	}

	UIWidget* UICustomDropDown::AddItem()
	{
		return mItemsList->AddItem();
	}

	UIWidget* UICustomDropDown::AddItem(int position)
	{
		return mItemsList->AddItem(position);
	}

	void UICustomDropDown::RemoveItem(UIWidget* item)
	{
		mItemsList->RemoveItem(item);
	}

	void UICustomDropDown::RemoveItem(int position)
	{
		mItemsList->RemoveItem(position);
	}

	void UICustomDropDown::MoveItem(int position, int newPosition)
	{
		mItemsList->MoveItem(position, newPosition);
	}

	void UICustomDropDown::MoveItem(UIWidget* item, int newPosition)
	{
		mItemsList->MoveItem(item, newPosition);
	}

	int UICustomDropDown::GetItemPosition(UIWidget* item)
	{
		return mItemsList->GetItemPosition(item);
	}

	UIWidget* UICustomDropDown::GetItem(int position) const
	{
		return mItemsList->GetItem(position);
	}

	void UICustomDropDown::RemoveAllItems()
	{
		mItemsList->RemoveAllItems();
	}

	void UICustomDropDown::SortItems(const Function<bool(UIWidget*, UIWidget*)>& sortFunc)
	{
		mItemsList->SortItems(sortFunc);
	}

	int UICustomDropDown::GetItemsCount() const
	{
		return mItemsList->GetItemsCount();
	}

	void UICustomDropDown::SelectItem(UIWidget* item)
	{
		mItemsList->SelectItem(item);
	}

	void UICustomDropDown::SelectItemAt(int position)
	{
		mItemsList->SelectItemAt(position);
	}

	UIWidget* UICustomDropDown::GetSelectedItem() const
	{
		return mItemsList->GetSelectedItem();
	}

	int UICustomDropDown::GetSelectedItemPosition() const
	{
		return mItemsList->GetSelectedItemPos();
	}

	UICustomList* UICustomDropDown::GetListView() const
	{
		return mItemsList;
	}

	void UICustomDropDown::SetMaxListSizeInItems(int itemsCount)
	{
		mMaxListItems = itemsCount;
	}

	void UICustomDropDown::SetClippingLayout(const Layout& layout)
	{
		mClipLayout = layout;
		SetLayoutDirty();
	}

	Layout UICustomDropDown::GetClippingLayout()
	{
		return mClipLayout;
	}

	void UICustomDropDown::CopyData(const Actor& otherActor)
	{
		const UICustomDropDown& other = dynamic_cast<const UICustomDropDown&>(otherActor);

		UIWidget::CopyData(other);

		mItemsList = other.mItemsList->CloneAs<UICustomList>();
		mItemsList->SetInternalParent(this, false);
		mItemsList->Hide(true);
		mItemsList->onSelectedItem += [&](auto x) { OnItemSelected(); };
		mItemsList->SetMultiselectionAvailable(false);

		mClipLayout = other.mClipLayout;
		mMaxListItems = other.mMaxListItems;

		RetargetStatesAnimations();
	}

	void UICustomDropDown::MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea)
	{
		RectF last = mBoundsWithChilds;

		mBoundsWithChilds += delta;
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		if (!mIsClipped)
			UpdateTransform(false);

		for (auto child : mChildWidgets)
			child->MoveAndCheckClipping(delta, clipArea);

		if (IsExpanded())
			Collapse();
	}

	void UICustomDropDown::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;
	}

	void UICustomDropDown::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (UIWidget::IsUnderPoint(cursor.position))
		{
			if (!IsExpanded())
				Expand();
		}
	}

	void UICustomDropDown::OnCursorReleasedOutside(const Input::Cursor& cursor)
	{
		if (!mItemsList->layout->IsPointInside(o2Input.GetCursorPos()) && IsExpanded())
			Collapse();
	}

	void UICustomDropDown::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void UICustomDropDown::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;
	}

	void UICustomDropDown::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = false;
	}

	void UICustomDropDown::OnResEnableInHierarchyChanged()
	{
		interactable = mResEnabled;
	}

	void UICustomDropDown::UpdateTransform(bool withChildren /*= true*/)
	{
		layout->Update();

		mChildrenWorldRect = layout->mData->worldRectangle;
		mAbsoluteClip = mClipLayout.Calculate(layout->mData->worldRectangle);

		if (withChildren)
			UpdateChildrenTransforms();
	}

	void UICustomDropDown::OnItemSelected()
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		Collapse();
		onSelectedPos(mItemsList->GetSelectedItemPos());
		onSelectedItem(mItemsList->GetSelectedItem());

		OnSelectionChanged();
	}

	void UICustomDropDown::OnSelectionChanged()
	{}
}

DECLARE_CLASS(o2::UICustomDropDown);
