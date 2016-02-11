#include "CustomDropDown.h"

#include "Render/Render.h"
#include "UIManager.h"

namespace o2
{
	UICustomDropDown::UICustomDropDown():
		UIWidget(), DrawableCursorEventsListener(this), mClipLayout(Layout::BothStretch()), mMaxListItems(10), mList(nullptr)
	{
		mList = mnew UICustomList();
		mList->mParent = this;
		mList->onSelectedItem += [&](auto x) { OnItemSelected(); };

		InitializeProperties();
	}

	UICustomDropDown::UICustomDropDown(const UICustomDropDown& other):
		UIWidget(other), DrawableCursorEventsListener(this), mClipLayout(other.mClipLayout), mMaxListItems(other.mMaxListItems)
	{
		mList = other.mList->Clone();
		mList->mParent = this;
		mList->Hide(true);
		mList->onSelectedItem += [&](auto x) { OnItemSelected(); };

		RetargetStatesAnimations();
		InitializeProperties();
	}

	UICustomDropDown::~UICustomDropDown()
	{
		delete mList;
	}

	UICustomDropDown& UICustomDropDown::operator=(const UICustomDropDown& other)
	{
		delete mList;

		UIWidget::operator=(other);

		mList = other.mList->Clone();
		mList->mParent = this;
		mList->Hide(true);
		mList->onSelectedItem += [&](auto x) { OnItemSelected(); };

		mClipLayout = other.mClipLayout;
		mMaxListItems = other.mMaxListItems;

		RetargetStatesAnimations();
		return *this;
	}

	void UICustomDropDown::Update(float dt)
	{
		if (mFullyDisabled)
			return;

		UIWidget::Update(dt);
		mList->Update(dt);

		if ((o2Input.IsCursorPressed() || Math::Abs(o2Input.GetMouseWheelDelta()) > 0.1f) &&
			!mList->layout.mAbsoluteRect.IsInside(o2Input.GetCursorPos()))
		{
			Collapse();
		}
	}

	void UICustomDropDown::Draw()
	{
		if (mFullyDisabled)
			return;

		UIWidget::Draw();

		o2UI.RegTopWidget(mList);

		UIWidget* selectedItem = mList->GetSelectedItem();
		if (selectedItem)
		{
			o2Render.EnableScissorTest(mAbsoluteClip);
			selectedItem->ForceDraw(mAbsoluteClip, mResTransparency);
			o2Render.DisableScissorTest();
		}

		if (UI_DEBUG || o2Input.IsKeyDown(VK_F1))
			DrawDebugFrame();
	}

	void UICustomDropDown::Expand()
	{
		float itemHeight = mList->GetItemSample()->layout.height;
		int itemsVisible = Math::Min(mMaxListItems, mList->GetItemsCount());
		RectF listViewArea = mList->mAbsoluteViewArea;
		RectF listAbsRect = mList->layout.mAbsoluteRect;
		RectF border(listViewArea.left - listAbsRect.left, listViewArea.bottom - listAbsRect.bottom,
					 listAbsRect.right - listViewArea.right, listAbsRect.top - listViewArea.top);
		mList->layout.height = itemHeight*(float)itemsVisible + border.bottom + border.top;

		auto openedState = state["opened"];
		if (openedState)
			*openedState = true;

		mList->SetVisible(true);
		UpdateLayout();
	}

	void UICustomDropDown::Collapse()
	{
		auto openedState = state["opened"];
		if (openedState)
			*openedState = false;

		mList->SetVisible(false);
	}

	bool UICustomDropDown::IsExpanded() const
	{
		return mList->IsVisible();
	}

	void UICustomDropDown::SetItemSample(UIWidget* sample)
	{
		mList->SetItemSample(sample);
	}

	UIWidget* UICustomDropDown::GetItemSample() const
	{
		return mList->GetItemSample();
	}

	UIVerticalLayout* UICustomDropDown::GetLayout() const
	{
		return mList->GetLayout();
	}

	UIWidget* UICustomDropDown::AddItem()
	{
		return mList->AddItem();
	}

	UIWidget* UICustomDropDown::AddItem(int position)
	{
		return mList->AddItem(position);
	}

	void UICustomDropDown::RemoveItem(UIWidget* item)
	{
		mList->RemoveItem(item);
	}

	void UICustomDropDown::RemoveItem(int position)
	{
		mList->RemoveItem(position);
	}

	void UICustomDropDown::MoveItem(int position, int newPosition)
	{
		mList->MoveItem(position, newPosition);
	}

	void UICustomDropDown::MoveItem(UIWidget* item, int newPosition)
	{
		mList->MoveItem(item, newPosition);
	}

	int UICustomDropDown::GetItemPosition(UIWidget* item)
	{
		return mList->GetItemPosition(item);
	}

	UIWidget* UICustomDropDown::GetItem(int position) const
	{
		return mList->GetItem(position);
	}

	void UICustomDropDown::RemoveAllItems()
	{
		mList->RemoveAllItems();
	}

	void UICustomDropDown::SortItems(const Function<bool(UIWidget*, UIWidget*)>& sortFunc)
	{
		mList->SortItems(sortFunc);
	}

	int UICustomDropDown::GetItemsCount() const
	{
		return mList->GetItemsCount();
	}

	void UICustomDropDown::SelectItem(UIWidget* item)
	{
		mList->SelectItem(item);
	}

	void UICustomDropDown::SelectItemAt(int position)
	{
		mList->SelectItemAt(position);
	}

	UIWidget* UICustomDropDown::GetSelectedItem() const
	{
		return mList->GetSelectedItem();
	}

	int UICustomDropDown::GetSelectedItemPosition() const
	{
		return mList->GetSelectedItemPosition();
	}

	UICustomList* UICustomDropDown::GetListView() const
	{
		return mList;
	}

	void UICustomDropDown::SetMaxListSizeInItems(int itemsCount)
	{
		mMaxListItems = itemsCount;
	}

	void UICustomDropDown::SetClippingLayout(const Layout& layout)
	{
		mClipLayout = layout;
		UpdateLayout();
	}

	Layout UICustomDropDown::GetClippingLayout()
	{
		return mClipLayout;
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

		if (UIWidget::IsUnderPoint(cursor.mPosition))
		{
			if (IsExpanded())
				Collapse();
			else
				Expand();
		}
	}

	void UICustomDropDown::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void UICustomDropDown::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = true;
	}

	void UICustomDropDown::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = false;
	}

	void UICustomDropDown::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	void UICustomDropDown::UpdateLayout(bool forcible /*= false*/)
	{
		if (layout.mDrivenByParent && !forcible)
		{
			if (mParent)
				mParent->UpdateLayout();

			return;
		}

		RecalculateAbsRect();
		UpdateLayersLayouts();

		mChildsAbsRect = layout.mAbsoluteRect;
		mAbsoluteClip = mClipLayout.Calculate(layout.mAbsoluteRect);

		for (auto child : mChilds)
			child->UpdateLayout();

		mList->UpdateLayout();
	}

	void UICustomDropDown::OnItemSelected()
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		Collapse();
		onSelectedPos(mList->GetSelectedItemPosition());
		onSelectedItem(mList->GetSelectedItem());

		OnSelectionChanged();
	}

	void UICustomDropDown::OnSelectionChanged()
	{}

	void UICustomDropDown::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UICustomDropDown, selectedItem, SelectItem, GetSelectedItem);
		INITIALIZE_PROPERTY(UICustomDropDown, selectedItemPos, SelectItemAt, GetSelectedItemPosition);
		INITIALIZE_ACCESSOR(UICustomDropDown, item, GetItem);
		INITIALIZE_GETTER(UICustomDropDown, itemsCount, GetItemsCount);
	}
}