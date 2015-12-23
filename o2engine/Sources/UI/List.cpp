#include "List.h"

#include "UI/Label.h"
#include "UI/VerticalLayout.h"

namespace o2
{
	UIList::UIList():
		UICustomList()
	{
		Ptr<UILabel> itemSample = mnew UILabel();
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
		auto item = UICustomList::AddItem().Cast<UILabel>();
		item->text = text;
		return GetItemsCount() - 1;
	}

	int UIList::AddItem(const WString& text, int position)
	{
		auto item = UICustomList::AddItem(position).Cast<UILabel>();
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
			if (child.Cast<UILabel>()->GetText() == text)
				return i;

			i++;
		}

		return -1;
	}

	WString UIList::GetItemText(int position)
	{
		auto item = GetItem(position).Cast<UILabel>();
		if (item)
			return item->GetText();

		return WString();
	}

	Vector<WString> UIList::GetAllItemsText() const
	{
		Vector<WString> res;
		for (auto child : mVerLayout->mChilds)
			res.Add(child.Cast<UILabel>()->GetText());

		return res;
	}

	WString UIList::GetSelectedItemText()
	{
		auto selectedItem = GetSelectedItem().Cast<UILabel>();
		if (selectedItem)
			return selectedItem->GetText();

		return WString();
	}

	void UIList::SelectItemText(const WString& text)
	{
		int idx = FindItem(text);
		UpdateSelection(idx, GetItem(idx));
	}

	void UIList::OnSelectionChanged()
	{
		onSelectedText(GetSelectedItemText());
	}

	void UIList::initializeProperties()
	{
		INITIALIZE_PROPERTY(UIList, value, SelectItemText, GetSelectedItemText);
		INITIALIZE_ACCESSOR(UIList, textItem, GetItemText);
	}
}