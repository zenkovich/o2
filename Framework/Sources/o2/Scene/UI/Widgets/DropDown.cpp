#include "o2/stdafx.h"
#include "DropDown.h"

#include "o2/Render/Render.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Label.h"

namespace o2
{
	DropDown::DropDown():
		CustomDropDown()
	{
		Label* itemSample = mnew Label();
		itemSample->horOverflow = Label::HorOverflow::Dots;
		SetItemSample(itemSample);
	}

	DropDown::DropDown(const DropDown& other):
		CustomDropDown(other), value(this)
	{
		RetargetStatesAnimations();
		mSelectedText = GetLayerDrawable<Text>("selectedText");
	}

	DropDown::~DropDown()
	{}

	DropDown& DropDown::operator=(const DropDown& other)
	{
		CustomDropDown::operator=(other);

		mSelectedText = GetLayerDrawable<Text>("selectedText");

		return *this;
	}

	void DropDown::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		Widget::Draw();

		o2UI.DrawWidgetAtTop(mItemsList);

		if (!mSelectedText)
		{
			Label* selectedItem = (Label*)(mItemsList->GetItem(mItemsList->GetSelectedItemPos()));
			if (selectedItem)
			{
				o2Render.EnableScissorTest(mAbsoluteClip);
				selectedItem->ForceDraw(mAbsoluteClip, mResTransparency);
				o2Render.DisableScissorTest();
			}
		}

		DrawDebugFrame();
	}

	int DropDown::AddItem(const WString& text)
	{
		auto item = (Label*)CustomDropDown::AddItem();
		item->text = text;
		return GetItemsCount() - 1;
	}

	int DropDown::AddItem(const WString& text, int position)
	{
		auto item = (Label*)CustomDropDown::AddItem(position);
		item->text = text;
		return position;
	}

	void DropDown::AddItems(const Vector<WString>& data)
	{
		for (auto text : data)
			AddItem(text);
	}

	void DropDown::RemoveItem(const WString& text)
	{
		int position = FindItem(text);
		if (position >= 0)
			CustomDropDown::RemoveItem(position);
	}

	int DropDown::FindItem(const WString& text)
	{
		int i = 0;
		for (auto child : mItemsList->mVerLayout->mChildWidgets)
		{
			Label* childLabel = dynamic_cast<Label*>(child);
			if (childLabel && childLabel->GetText() == text)
				return i;

			i++;
		}

		return -1;
	}

	WString DropDown::GetItemText(int position)
	{
		auto item = (Label*)GetItem(position);
		if (item)
			return item->GetText();

		return WString();
	}

	Vector<WString> DropDown::GetAllItemsText() const
	{
		Vector<WString> res;
		for (auto child : mItemsList->mVerLayout->mChildWidgets)
		{
			Label* childLabel = dynamic_cast<Label*>(child);

			if (childLabel)
				res.Add(childLabel->GetText());
		}

		return res;
	}

	WString DropDown::GetSelectedItemText()
	{
		auto selectedItem = (Label*)GetSelectedItem();
		if (selectedItem)
			return selectedItem->GetText();

		return WString();
	}

	void DropDown::SelectItemText(const WString& text)
	{
		int idx = FindItem(text);
		mItemsList->SelectItemAt(idx);
	}

	String DropDown::GetCreateMenuGroup()
	{
		return "Dropping";
	}

	void DropDown::OnSelectionChanged()
	{
		onSelectedText(GetSelectedItemText());

		if (mSelectedText)
			mSelectedText->SetText(GetSelectedItemText());
	}

	void DropDown::OnLayerAdded(WidgetLayer* layer)
	{
		if (layer->name == "selectedText" && layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(Text))
			mSelectedText = (Text*)layer->GetDrawable();
	}

}
// --- META ---

DECLARE_CLASS(o2::DropDown);
// --- END META ---
