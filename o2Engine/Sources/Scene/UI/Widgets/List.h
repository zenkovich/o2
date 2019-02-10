#pragma once

#include "Scene/UI/Widgets/CustomList.h"

namespace o2
{
	// --------------------
	// Text items list view
	// --------------------
	class UIList: public UICustomList
	{
	public:
		PROPERTIES(UIList);
		PROPERTY(WString, value, SelectItemText, GetSelectedItemText);             // Current selected item text
		PROPERTY(Vector<WString>, values, SetSelectedItems, GetSelectedItemsText); // Current selected items texts

		Function<void(const WString&)> onSelectedText; // Change text selected event

		// Default constructor
		UIList();

		// Copy-constructor
		UIList(const UIList& other);

		// Destructor
		~UIList();

		// Copy operator
		UIList& operator=(const UIList& other);

		// Adds new text item and returns position
		int AddItem(const WString& text);

		// Add new text item at position and returns this position
		int AddItem(const WString& text, int position);

		// Adds array of text items
		void AddItems(const Vector<WString>& data);

		// Removes item, if exist
		void RemoveItem(const WString& text);

		// Returns position of item. Returns -1 if can't find item
		int FindItem(const WString& text);

		// Returns item text by position
		WString GetItemText(int position) const;

		// Returns array of all text items
		Vector<WString> GetAllItemsText() const;

		// Returns current selected text item
		WString GetSelectedItemText();

		// Selects item text
		void SelectItemText(const WString& text);

		// Selects items texts
		void SetSelectedItems(const Vector<WString>& items);

		// Returns selected items texts
		Vector<WString> GetSelectedItemsText() const;

		SERIALIZABLE(UIList);

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when selected item index was changed
		void OnSelectionChanged() override;
	};
}

CLASS_BASES_META(o2::UIList)
{
	BASE_CLASS(o2::UICustomList);
}
END_META;
CLASS_FIELDS_META(o2::UIList)
{
	PUBLIC_FIELD(value);
	PUBLIC_FIELD(values);
	PUBLIC_FIELD(onSelectedText);
}
END_META;
CLASS_METHODS_META(o2::UIList)
{

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
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
}
END_META;
