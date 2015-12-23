#pragma once

#include "UI/CustomDropDown.h"

namespace o2
{
	// --------------
	// Text drop down
	// --------------
	class UIDropDown: public UICustomDropDown
	{
	public:
		Property<WString>              value;    // Current selected item text
		Accessor<WString, int>         textItem; // Text item accessor by position

		Function<void(const WString&)> onSelectedText; // Change text selected event

		// Default constructor
		UIDropDown();

		// Copy-constructor
		UIDropDown(const UIDropDown& other);

		// Destructor
		~UIDropDown();

		// Copy operator
		UIDropDown& operator=(const UIDropDown& other);

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
		WString GetItemText(int position);

		// Returns array of all text items
		Vector<WString> GetAllItemsText() const;

		// Returns current selected text item
		WString GetSelectedItemText();

		// Selects item text
		void SelectItemText(const WString& text);

		SERIALIZABLE(UIDropDown);

	protected:
		// Calls when selected item index was changed
		void OnSelectionChanged();

		// Initializes properties
		void InitializeProperties();
	};
}
