#include "CustomDropDown.h"

namespace o2
{
	IOBJECT_CPP(UICustomDropDown);

	UICustomDropDown::UICustomDropDown():
		UIWidget()
	{
		mList = mnew UICustomList();
		mList->mParent = this;

		mDownButton = mnew UIButton();
		mDownButton->mParent = this;

		InitializeProperties();
	}

	UICustomDropDown::UICustomDropDown(const UICustomDropDown& other):
		UIWidget(other)
	{
		mList = other.mList->Clone();
		mList->mParent = this;

		mDownButton = other.mDownButton->Clone();
		mDownButton->mParent = this;

		InitializeProperties();
	}

	UICustomDropDown::~UICustomDropDown()
	{
		mList.Release();
		mDownButton.Release();
	}

	UICustomDropDown& UICustomDropDown::operator=(const UICustomDropDown& other)
	{
		mList.Release();
		mDownButton.Release();

		mList = other.mList->Clone();
		mList->mParent = this;

		mDownButton = other.mDownButton->Clone();
		mDownButton->mParent = this;

		return *this;
	}

	void UICustomDropDown::Update(float dt)
	{
		UIWidget::Update(dt);

		mDownButton->Update(dt);
		mList->Update(dt);
	}

	void UICustomDropDown::Draw()
	{
		UIWidget::Draw();

		mDownButton->Draw();
		mList->Draw();

		Ptr<UIWidget> selectedItem = mList->GetSelectedItem();
		if (selectedItem)
		{
			UIWidgetLayout lastLayout = selectedItem->layout;
			selectedItem->layout.SetAbsoluteRect(layout.mAbsoluteRect);
			selectedItem->Draw();
			selectedItem->layout = lastLayout;
		}
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

	Ptr<UIButton> UICustomDropDown::GetDownButton() const
	{
		return mDownButton;
	}

	void UICustomDropDown::SetListSizeInItems(int itemsCount)
	{
		mList->layout.height = mList->GetItemSample()->layout.height*(float)itemsCount;
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

		for (auto child : mChilds)
			child->UpdateLayout();

		mDownButton->UpdateLayout();
		mList->UpdateLayout();
	}

	void UICustomDropDown::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UICustomDropDown, selectedItem, SelectItem, GetSelectedItem);
		INITIALIZE_PROPERTY(UICustomDropDown, selectedItemPos, SelectItemAt, GetSelectedItemPosition);
		INITIALIZE_ACCESSOR(UICustomDropDown, item, GetItem);
		INITIALIZE_GETTER(UICustomDropDown, itemsCount, GetItemsCount);
	}
}