#include "CustomDropDown.h"

#include "Render/Render.h"
#include "UIManager.h"

namespace o2
{
	UICustomDropDown::UICustomDropDown():
		UIWidget(), mClipLayout(Layout::Both()), mMaxListItems(10)
	{
		mList = mnew UICustomList();
		mList->mParent = this;
		mList->onSelectedItem += [&](auto x) { OnItemSelected(); };

		InitializeProperties();
	}

	UICustomDropDown::UICustomDropDown(const UICustomDropDown& other):
		UIWidget(other), mClipLayout(other.mClipLayout), mMaxListItems(other.mMaxListItems)
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
		mList.Release();
	}

	UICustomDropDown& UICustomDropDown::operator=(const UICustomDropDown& other)
	{
		mList.Release();

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
		UIWidget::Update(dt);
		mList->Update(dt);
	}

	void UICustomDropDown::Draw()
	{
		UIWidget::Draw();

		o2UI.RegTopWidget(mList);

		Ptr<UIWidget> selectedItem = mList->GetSelectedItem();
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

	void UICustomDropDown::SetItemSample(Ptr<UIWidget> sample)
	{
		mList->SetItemSample(sample);
	}

	Ptr<UIWidget> UICustomDropDown::GetItemSample() const
	{
		return mList->GetItemSample();
	}

	Ptr<UIVerticalLayout> UICustomDropDown::GetLayout() const
	{
		return mList->GetLayout();
	}

	Ptr<UIWidget> UICustomDropDown::AddItem()
	{
		return mList->AddItem();
	}

	Ptr<UIWidget> UICustomDropDown::AddItem(int position)
	{
		return mList->AddItem(position);
	}

	void UICustomDropDown::RemoveItem(Ptr<UIWidget> item)
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

	void UICustomDropDown::MoveItem(Ptr<UIWidget> item, int newPosition)
	{
		mList->MoveItem(item, newPosition);
	}

	int UICustomDropDown::GetItemPosition(Ptr<UIWidget> item)
	{
		return mList->GetItemPosition(item);
	}

	Ptr<UIWidget> UICustomDropDown::GetItem(int position) const
	{
		return mList->GetItem(position);
	}

	void UICustomDropDown::RemoveAllItems()
	{
		mList->RemoveAllItems();
	}

	void UICustomDropDown::SortItems(const Function<bool(const Ptr<UIWidget>&, const Ptr<UIWidget>&)>& sortFunc)
	{
		mList->SortItems(sortFunc);
	}

	int UICustomDropDown::GetItemsCount() const
	{
		return mList->GetItemsCount();
	}

	void UICustomDropDown::SelectItem(Ptr<UIWidget> item)
	{
		mList->SelectItem(item);
	}

	void UICustomDropDown::SelectItemAt(int position)
	{
		mList->SelectItemAt(position);
	}

	Ptr<UIWidget> UICustomDropDown::GetSelectedItem() const
	{
		return mList->GetSelectedItem();
	}

	int UICustomDropDown::GetSelectedItemPosition() const
	{
		return mList->GetSelectedItemPosition();
	}

	Ptr<UICustomList> UICustomDropDown::GetListView() const
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

	bool UICustomDropDown::IsUnderPoint(const Vec2F& point)
	{
		return layout.GetAbsoluteRect().IsInside(point);
	}

	float UICustomDropDown::Depth()
	{
		return GetMaxDrawingDepth();
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

		if (IsUnderPoint(cursor.mPosition))
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