#include "List.h"

#include "UI/Label.h"
#include "UI/VerticalLayout.h"

namespace o2
{
	UIList::UIList():
		UICustomList()
	{
		UILabel* itemSample = mnew UILabel();
		itemSample->horOverflow = UILabel::HorOverflow::Dots;
		SetItemSample(itemSample);

		InitializeProperties();
	}

	UIList::UIList(const UIList& other):
		UICustomList(other)
	{
		RetargetStatesAnimations();
		InitializeProperties();
	}

	UIList::~UIList()
	{}

	UIList& UIList::operator=(const UIList& other)
	{
		UICustomList::operator=(other);
		RetargetStatesAnimations();
		return *this;
	}

	int UIList::AddItem(const WString& text)
	{
		auto item = (UILabel*)UICustomList::AddItem();
		item->text = text;
		return GetItemsCount() - 1;
	}

	int UIList::AddItem(const WString& text, int position)
	{
		auto item = (UILabel*)UICustomList::AddItem(position);
		item->text = text;
		return position;
	}

	void UIList::AddItems(const Vector<WString>& data)
	{
		for (auto text : data)
			AddItem(text);
	}

	void UIList::RemoveItem(const WString& text)
	{
		int position = FindItem(text);
		if (position >= 0)
			UICustomList::RemoveItem(position);
	}

	int UIList::FindItem(const WString& text)
	{
		int i = 0;
		for (auto child : mVerLayout->mChilds)
		{
			if (((UILabel*)child)->GetText() == text)
				return i;

			i++;
		}

		return -1;
	}

	WString UIList::GetItemText(int position) const
	{
		auto item = (UILabel*)GetItem(position);
		if (item)
			return item->GetText();

		return WString();
	}

	Vector<WString> UIList::GetAllItemsText() const
	{
		Vector<WString> res;
		for (auto child : mVerLayout->mChilds)
			res.Add(((UILabel*)child)->GetText());

		return res;
	}

	WString UIList::GetSelectedItemText()
	{
		auto selectedItem = (UILabel*)GetSelectedItemPos();
		if (selectedItem)
			return selectedItem->GetText();

		return WString();
	}

	void UIList::SelectItemText(const WString& text)
	{
		int idx = FindItem(text);
		SelectItemAt(idx);
	}

	void UIList::SetSelectedItems(const Vector<WString>& items)
	{
		for (auto& x : items)
			SelectItemText(x);
	}

	Vector<WString> UIList::GetSelectedItemsText() const
	{
		return mSelectedItems.Select<WString>([&](auto x) { return GetItemText(x.idx); });
	}

	void UIList::OnSelectionChanged()
	{
		onSelectedText(GetSelectedItemText());
	}

	void UIList::initializeProperties()
	{
		INITIALIZE_PROPERTY(UIList, value, SelectItemText, GetSelectedItemText);
		INITIALIZE_PROPERTY(UIList, values, SetSelectedItems, GetSelectedItemsText);
		INITIALIZE_ACCESSOR(UIList, textItem, GetItemText);
	}
}

CLASS_META(o2::UIList)
{
	BASE_CLASS(o2::UICustomList);

	PUBLIC_FIELD(value);
	PUBLIC_FIELD(values);
	PUBLIC_FIELD(textItem);
	PUBLIC_FIELD(onSelectedText);

	PUBLIC_FUNCTION(int, AddItem, const WString&);
	PUBLIC_FUNCTION(int, AddItem, const WString&, int);
	PUBLIC_FUNCTION(void, AddItems, const Vector<WString>&);
	PUBLIC_FUNCTION(void, RemoveItem, const WString&);
	PUBLIC_FUNCTION(int, FindItem, const WString&);
	PUBLIC_FUNCTION(WString, GetItemText, int);
	PUBLIC_FUNCTION(Vector<WString>, GetAllItemsText);
	PUBLIC_FUNCTION(WString, GetSelectedItemText);
	PUBLIC_FUNCTION(void, SelectItemText, const WString&);
	PUBLIC_FUNCTION(void, SetSelectedItems, const Vector<WString>&);
	PUBLIC_FUNCTION(Vector<WString>, GetSelectedItemsText);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
	PROTECTED_FUNCTION(void, initializeProperties);
}
END_META;
