#include "DropDown.h"

#include "UI/Label.h"

namespace o2
{
	IOBJECT_CPP(UIDropDown);


	UIDropDown::UIDropDown():
		UICustomDropDown()
	{
		Ptr<UILabel> itemSample = mnew UILabel();
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
		auto item = UICustomDropDown::AddItem().Cast<UILabel>();
		item->text = text;
		return GetItemsCount() - 1;
	}

	int UIDropDown::AddItem(const WString& text, int position)
	{
		auto item = UICustomDropDown::AddItem(position).Cast<UILabel>();
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
			if (child.Cast<UILabel>()->GetText() == text)
				return i;

			i++;
		}

		return -1;
	}

	WString UIDropDown::GetItemText(int position)
	{
		auto item = GetItem(position).Cast<UILabel>();
		if (item)
			return item->GetText();

		return WString();
	}

	Vector<WString> UIDropDown::GetAllItemsText() const
	{
		Vector<WString> res;
		for (auto child : mList->mVerLayout->mChilds)
			res.Add(child.Cast<UILabel>()->GetText());

		return res;
	}

	WString UIDropDown::GetSelectedItemText()
	{
		auto selectedItem = GetSelectedItem().Cast<UILabel>();
		if (selectedItem)
			return selectedItem->GetText();

		return WString();
	}

	void UIDropDown::SelectItemText(const WString& text)
	{
		int idx = FindItem(text);
		mList->UpdateSelection(idx, GetItem(idx));
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