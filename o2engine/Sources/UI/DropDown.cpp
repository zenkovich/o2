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

		InitializeProperties();
	}

	UIDropDown::UIDropDown(const UIDropDown& other):
		UICustomDropDown(other)
	{
		RetargetStatesAnimations();
		InitializeProperties();
	}

	UIDropDown::~UIDropDown()
	{}

	UIDropDown& UIDropDown::operator=(const UIDropDown& other)
	{
		UICustomDropDown::operator=(other);
		RetargetStatesAnimations();
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
		for (auto child : mList->mVerLayout->mChilds)
		{
			if (((UILabel*)child)->GetText() == text)
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
		for (auto child : mList->mVerLayout->mChilds)
			res.Add(((UILabel*)child)->GetText());

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
		mList->SelectItemAt(idx);
	}

	void UIDropDown::OnSelectionChanged()
	{
		onSelectedText(GetSelectedItemText());
	}

	void UIDropDown::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIDropDown, value, SelectItemText, GetSelectedItemText);
		INITIALIZE_ACCESSOR(UIDropDown, textItem, GetItemText);
	}
}