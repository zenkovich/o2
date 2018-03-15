#include "stdafx.h"
#include "DropDown.h"

#include "UI/Label.h"

namespace o2
{
	UIDropDown::UIDropDown():
		UICustomDropDown()
	{
		UILabel* itemSample = mnew UILabel();
		itemSample->horOverflow = UILabel::HorOverflow::Dots;
		SetItemSample(itemSample);
	}

	UIDropDown::UIDropDown(const UIDropDown& other):
		UICustomDropDown(other)
	{
		RetargetStatesAnimations();
	}

	UIDropDown::~UIDropDown()
	{}

	UIDropDown& UIDropDown::operator=(const UIDropDown& other)
	{
		CopyData(other);
		return *this;
	}

	int UIDropDown::AddItem(const WString& text)
	{
		auto item = (UILabel*)UICustomDropDown::AddItem();
		item->text = text;
		return GetItemsCount() - 1;
	}

	int UIDropDown::AddItem(const WString& text, int position)
	{
		auto item = (UILabel*)UICustomDropDown::AddItem(position);
		item->text = text;
		return position;
	}

	void UIDropDown::AddItems(const Vector<WString>& data)
	{
		for (auto text : data)
			AddItem(text);
	}

	void UIDropDown::RemoveItem(const WString& text)
	{
		int position = FindItem(text);
		if (position >= 0)
			UICustomDropDown::RemoveItem(position);
	}

	int UIDropDown::FindItem(const WString& text)
	{
		int i = 0;
		for (auto child : mItemsList->mVerLayout->mChildWidgets)
		{
			UILabel* childLabel = dynamic_cast<UILabel*>(child);
			if (childLabel && childLabel->GetText() == text)
				return i;

			i++;
		}

		return -1;
	}

	WString UIDropDown::GetItemText(int position)
	{
		auto item = (UILabel*)GetItem(position);
		if (item)
			return item->GetText();

		return WString();
	}

	Vector<WString> UIDropDown::GetAllItemsText() const
	{
		Vector<WString> res;
		for (auto child : mItemsList->mVerLayout->mChildWidgets)
		{
			UILabel* childLabel = dynamic_cast<UILabel*>(child);

			if (childLabel)
				res.Add(childLabel->GetText());
		}

		return res;
	}

	WString UIDropDown::GetSelectedItemText()
	{
		auto selectedItem = (UILabel*)GetSelectedItem();
		if (selectedItem)
			return selectedItem->GetText();

		return WString();
	}

	void UIDropDown::SelectItemText(const WString& text)
	{
		int idx = FindItem(text);
		mItemsList->SelectItemAt(idx);
	}

	void UIDropDown::CopyData(const Actor& otherActor)
	{
		const UIDropDown& other = dynamic_cast<const UIDropDown&>(otherActor);
		UICustomDropDown::CopyData(other);
		RetargetStatesAnimations();
	}

	void UIDropDown::OnSelectionChanged()
	{
		onSelectedText(GetSelectedItemText());
	}
}

DECLARE_CLASS(o2::UIDropDown);
