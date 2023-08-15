#include "o2/stdafx.h"
#include "CustomDropDown.h"

#include "o2/Render/Render.h"
#include "o2/Scene/UI/Widgets/CustomList.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/WidgetState.h"

namespace o2
{
	CustomDropDown::CustomDropDown():
		Widget(), DrawableCursorEventsListener(this)
	{
		mItemsList = mnew CustomList();
		mItemsList->SetInternalParent(this, false);
		mItemsList->onSelectedItem += [&](auto x) { OnItemSelected(); };
		mItemsList->SetMultiselectionAvailable(false);
	}

	CustomDropDown::CustomDropDown(const CustomDropDown& other):
		Widget(other), DrawableCursorEventsListener(this), mClipLayout(other.mClipLayout),
		mMaxListItems(other.mMaxListItems), selectedItem(this), selectedItemPos(this), itemsCount(this)
	{
		mItemsList = FindInternalWidgetByType<CustomList>();
		mItemsList->onSelectedItem += [&](auto x) { OnItemSelected(); };
		mItemsList->Hide(true);
		mItemsList->SetMultiselectionAvailable(false);

		RetargetStatesAnimations();
	}

	CustomDropDown::~CustomDropDown()
	{}

	CustomDropDown& CustomDropDown::operator=(const CustomDropDown& other)
	{
		Widget::operator=(other);

		mItemsList = FindInternalWidgetByType<CustomList>();
		mItemsList->onSelectedItem += [&](auto x) { OnItemSelected(); };
		mItemsList->Hide(true);
		mItemsList->SetMultiselectionAvailable(false);

		mClipLayout = other.mClipLayout;
		mMaxListItems = other.mMaxListItems;

		RetargetStatesAnimations();

		return *this;
	}

	void CustomDropDown::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		Widget::Draw();

		o2UI.DrawWidgetAtTop(mItemsList);

		Widget* selectedItem = mItemsList->GetItem(mItemsList->GetSelectedItemPos());
		if (selectedItem)
		{
			o2Render.EnableScissorTest(mAbsoluteClip);
			selectedItem->ForceDraw(mAbsoluteClip, mResTransparency);
			o2Render.DisableScissorTest();
		}

		DrawDebugFrame();
	}

	void CustomDropDown::Expand()
	{
		onBeforeExpand();

		float itemHeight = mItemsList->GetItemSample()->layout->minHeight;
		int itemsVisible = Math::Min(mMaxListItems, mItemsList->GetItemsCount());

		mItemsList->layout->minHeight = itemHeight*(float)itemsVisible +
			mItemsList->mViewAreaLayout.offsetMin.y - mItemsList->mViewAreaLayout.offsetMax.y;

		auto openedState = state["opened"];
		if (openedState)
			*openedState = true;

		mItemsList->SetEnabled(true);
		mItemsList->UpdateSelfTransform();
		mItemsList->UpdateChildrenTransforms();

		SetLayoutDirty();
	}

	void CustomDropDown::Collapse()
	{
		auto openedState = state["opened"];
		if (openedState)
			*openedState = false;

		mItemsList->SetEnabled(false);
	}

	bool CustomDropDown::IsExpanded() const
	{
		return mItemsList->IsEnabled();
	}

	void CustomDropDown::SetItemSample(Widget* sample)
	{
		mItemsList->SetItemSample(sample);
	}

	Widget* CustomDropDown::GetItemSample() const
	{
		return mItemsList->GetItemSample();
	}

	VerticalLayout* CustomDropDown::GetItemsLayout() const
	{
		return mItemsList->GetItemsLayout();
	}

	Widget* CustomDropDown::AddItem()
	{
		return mItemsList->AddItem();
	}

	Widget* CustomDropDown::AddItem(int position)
	{
		return mItemsList->AddItem(position);
	}

	void CustomDropDown::RemoveItem(Widget* item)
	{
		mItemsList->RemoveItem(item);
	}

	void CustomDropDown::RemoveItem(int position)
	{
		mItemsList->RemoveItem(position);
	}

	void CustomDropDown::MoveItem(int position, int newPosition)
	{
		mItemsList->MoveItem(position, newPosition);
	}

	void CustomDropDown::MoveItem(Widget* item, int newPosition)
	{
		mItemsList->MoveItem(item, newPosition);
	}

	int CustomDropDown::GetItemPosition(Widget* item)
	{
		return mItemsList->GetItemPosition(item);
	}

	Widget* CustomDropDown::GetItem(int position) const
	{
		return mItemsList->GetItem(position);
	}

	void CustomDropDown::RemoveAllItems()
	{
		mItemsList->RemoveAllItems();
	}

	void CustomDropDown::SortItems(const Function<bool(Widget*, Widget*)>& sortFunc)
	{
		mItemsList->SortItems(sortFunc);
	}

	int CustomDropDown::GetItemsCount() const
	{
		return mItemsList->GetItemsCount();
	}

	void CustomDropDown::SelectItem(Widget* item)
	{
		mItemsList->SelectItem(item);
	}

	void CustomDropDown::SelectItemAt(int position)
	{
		mItemsList->SelectItemAt(position);
	}

	Widget* CustomDropDown::GetSelectedItem() const
	{
		return mItemsList->GetSelectedItem();
	}

	int CustomDropDown::GetSelectedItemPosition() const
	{
		return mItemsList->GetSelectedItemPos();
	}

	CustomList* CustomDropDown::GetListView() const
	{
		return mItemsList;
	}

	void CustomDropDown::SetMaxListSizeInItems(int itemsCount)
	{
		mMaxListItems = itemsCount;
	}

	void CustomDropDown::SetClippingLayout(const Layout& layout)
	{
		mClipLayout = layout;
		SetLayoutDirty();
	}

	Layout CustomDropDown::GetClippingLayout()
	{
		return mClipLayout;
	}

	void CustomDropDown::MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea)
	{
		mBoundsWithChilds += delta;
		mIsClipped = !mBoundsWithChilds.IsIntersects(clipArea);

		if (!mIsClipped)
			UpdateSelfTransform();

		for (auto child : mChildWidgets)
			child->MoveAndCheckClipping(delta, clipArea);

		if (IsExpanded())
			Collapse();
	}

	void CustomDropDown::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;
	}

	void CustomDropDown::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (Widget::IsUnderPoint(cursor.position))
		{
			if (!IsExpanded())
				Expand();
		}
	}

	void CustomDropDown::OnCursorReleasedOutside(const Input::Cursor& cursor)
	{
		if (!mItemsList->layout->IsPointInside(o2Input.GetCursorPos()) && IsExpanded())
			Collapse();
	}

	void CustomDropDown::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void CustomDropDown::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;
	}

	void CustomDropDown::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = false;
	}

	void CustomDropDown::OnEnabled()
	{
		interactable = true;
	}

	void CustomDropDown::OnDisabled()
	{
		interactable = false;
	}

	void CustomDropDown::UpdateSelfTransform()
{
		layout->Update();
		mAbsoluteClip = mClipLayout.Calculate(GetLayoutData().worldRectangle);
	}

	String CustomDropDown::GetCreateMenuGroup()
	{
		return "Dropping";
	}

	void CustomDropDown::OnItemSelected()
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		Collapse();
		onSelectedPos(mItemsList->GetSelectedItemPos());
		onSelectedItem(mItemsList->GetSelectedItem());

		OnSelectionChanged();
	}

	void CustomDropDown::OnSelectionChanged()
	{}
}
// --- META ---

DECLARE_CLASS(o2::CustomDropDown);
// --- END META ---
