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
		mItemsList->mParent = this;
		mItemsList->onSelectedItem += [&](auto x) { OnItemSelected(); };
		mItemsList->SetMultiselectionAvailable(false);

		InitializeProperties();
	}

	UICustomDropDown::UICustomDropDown(const UICustomDropDown& other):
		UIWidget(other), DrawableCursorEventsListener(this), mClipLayout(other.mClipLayout), 
		mMaxListItems(other.mMaxListItems)
	{
		mItemsList = other.mItemsList->CloneAs<UICustomList>();
		mItemsList->mParent = this;
		mItemsList->Hide(true);
		mItemsList->onSelectedItem += [&](auto x) { OnItemSelected(); };
		mItemsList->SetMultiselectionAvailable(false);

		RetargetStatesAnimations();
		InitializeProperties();
	}

	UICustomDropDown::~UICustomDropDown()
	{
		delete mItemsList;
	}

	UICustomDropDown& UICustomDropDown::operator=(const UICustomDropDown& other)
	{
		delete mItemsList;

		UIWidget::operator=(other);

		mItemsList = other.mItemsList->CloneAs<UICustomList>();
		mItemsList->mParent = this;
		mItemsList->Hide(true);
		mItemsList->onSelectedItem += [&](auto x) { OnItemSelected(); };
		mItemsList->SetMultiselectionAvailable(false);

		mClipLayout = other.mClipLayout;
		mMaxListItems = other.mMaxListItems;

		RetargetStatesAnimations();
		return *this;
	}

	void UICustomDropDown::Update(float dt)
	{
		if (mFullyDisabled || mIsClipped)
			return;

		UIWidget::Update(dt);
		mItemsList->Update(dt);
	}

	void UICustomDropDown::Draw()
	{
		if (mFullyDisabled)
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

		mItemsList->SetVisible(true);
		UpdateLayout();
	}

	void UICustomDropDown::Collapse()
	{
		auto openedState = state["opened"];
		if (openedState)
			*openedState = false;

		mItemsList->SetVisible(false);
	}

	bool UICustomDropDown::IsExpanded() const
	{
		return mItemsList->IsVisible();
	}

	void UICustomDropDown::SetItemSample(UIWidget* sample)
	{
		mItemsList->SetItemSample(sample);
	}

	UIWidget* UICustomDropDown::GetItemSample() const
	{
		return mItemsList->GetItemSample();
	}

	UIVerticalLayout* UICustomDropDown::GetLayout() const
	{
		return mItemsList->GetLayout();
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

	void UICustomDropDown::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	void UICustomDropDown::UpdateLayout(bool withChildren /*= true*/)
	{
		layout->Update();

		mChildrenWorldRect = layout->mData->worldRectangle;
		mAbsoluteClip = mClipLayout.Calculate(layout->mData->worldRectangle);

		if (withChildren)
			UpdateChildrenLayouts();

		mItemsList->UpdateLayout();
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

	void UICustomDropDown::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UICustomDropDown, selectedItem, SelectItem, GetSelectedItem);
		INITIALIZE_PROPERTY(UICustomDropDown, selectedItemPos, SelectItemAt, GetSelectedItemPosition);
		INITIALIZE_ACCESSOR(UICustomDropDown, item, GetItem);
		INITIALIZE_GETTER(UICustomDropDown, itemsCount, GetItemsCount);
	}
}

CLASS_META(o2::UICustomDropDown)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::DrawableCursorEventsListener);

	PUBLIC_FIELD(selectedItem);
	PUBLIC_FIELD(selectedItemPos);
	PUBLIC_FIELD(item);
	PUBLIC_FIELD(itemsCount);
	PUBLIC_FIELD(onSelectedPos);
	PUBLIC_FIELD(onSelectedItem);
	PROTECTED_FIELD(mItemsList).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mClipLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAbsoluteClip);
	PROTECTED_FIELD(mMaxListItems).SERIALIZABLE_ATTRIBUTE();

	typedef const Function<bool(UIWidget*, UIWidget*)>& _tmp1;

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(void, SetItemSample, UIWidget*);
	PUBLIC_FUNCTION(UIWidget*, GetItemSample);
	PUBLIC_FUNCTION(UIVerticalLayout*, GetLayout);
	PUBLIC_FUNCTION(UIWidget*, AddItem);
	PUBLIC_FUNCTION(UIWidget*, AddItem, int);
	PUBLIC_FUNCTION(void, RemoveItem, UIWidget*);
	PUBLIC_FUNCTION(void, RemoveItem, int);
	PUBLIC_FUNCTION(void, MoveItem, int, int);
	PUBLIC_FUNCTION(void, MoveItem, UIWidget*, int);
	PUBLIC_FUNCTION(int, GetItemPosition, UIWidget*);
	PUBLIC_FUNCTION(UIWidget*, GetItem, int);
	PUBLIC_FUNCTION(void, RemoveAllItems);
	PUBLIC_FUNCTION(void, SortItems, _tmp1);
	PUBLIC_FUNCTION(int, GetItemsCount);
	PUBLIC_FUNCTION(void, SelectItem, UIWidget*);
	PUBLIC_FUNCTION(void, SelectItemAt, int);
	PUBLIC_FUNCTION(UIWidget*, GetSelectedItem);
	PUBLIC_FUNCTION(int, GetSelectedItemPosition);
	PUBLIC_FUNCTION(UICustomList*, GetListView);
	PUBLIC_FUNCTION(void, SetMaxListSizeInItems, int);
	PUBLIC_FUNCTION(void, SetClippingLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetClippingLayout);
	PUBLIC_FUNCTION(void, UpdateLayout, bool);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleasedOutside, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(void, OnItemSelected);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
