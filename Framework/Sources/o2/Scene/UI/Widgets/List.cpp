#include "o2/stdafx.h"
#include "List.h"

#include "o2/Scene/UI/Widgets/Label.h"
#include "o2/Scene/UI/Widgets/VerticalLayout.h"

namespace o2
{
    List::List():
        CustomList()
    {
        Label* itemSample = mnew Label();
        itemSample->horOverflow = Label::HorOverflow::Dots;
        SetItemSample(itemSample);
    }

    List::List(const List& other):
        CustomList(other), value(this), values(this)
    {
        RetargetStatesAnimations();
    }

    List::~List()
    {}

    List& List::operator=(const List& other)
    {
        CustomList::operator=(other);
        return *this;
    }

    int List::AddItem(const WString& text)
    {
        auto item = (Label*)CustomList::AddItem();
        item->text = text;
        return GetItemsCount() - 1;
    }

    int List::AddItem(const WString& text, int position)
    {
        auto item = (Label*)CustomList::AddItem(position);
        item->text = text;
        return position;
    }

    void List::AddItems(const Vector<WString>& data)
    {
        for (auto text : data)
            AddItem(text);
    }

    void List::RemoveItem(const WString& text)
    {
        int position = FindItem(text);
        if (position >= 0)
            CustomList::RemoveItem(position);
    }

    int List::FindItem(const WString& text)
    {
        int i = 0;
        for (auto child : mVerLayout->mChildren)
        {
            if (((Label*)child)->GetText() == text)
                return i;

            i++;
        }

        return -1;
    }

    WString List::GetItemText(int position) const
    {
        auto item = (Label*)GetItem(position);
        if (item)
            return item->GetText();

        return WString();
    }

    Vector<WString> List::GetAllItemsText() const
    {
        Vector<WString> res;
        for (auto child : mVerLayout->mChildren)
            res.Add(((Label*)child)->GetText());

        return res;
    }

    WString List::GetSelectedItemText()
    {
        auto selectedItem = dynamic_cast<Label*>(GetSelectedItem());
        if (selectedItem)
            return selectedItem->GetText();

        return WString();
    }

    void List::SelectItemText(const WString& text)
    {
        int idx = FindItem(text);
        SelectItemAt(idx);
    }

    void List::SetSelectedItems(const Vector<WString>& items)
    {
        for (auto& x : items)
            SelectItemText(x);
    }

    Vector<WString> List::GetSelectedItemsText() const
    {
        return mSelectedItems.Convert<WString>([&](auto x) { return GetItemText(x.idx); });
    }

    void List::OnSelectionChanged()
    {
        onSelectedText(GetSelectedItemText());
    }

    String List::GetCreateMenuGroup()
    {
        return "List";
    }
}
// --- META ---

DECLARE_CLASS(o2::List, o2__List);
// --- END META ---
