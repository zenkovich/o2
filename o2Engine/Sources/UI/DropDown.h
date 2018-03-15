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
		PROPERTY(WString>      value;    // Current selected item text
		Accessor<WString, int> textItem; // Text item accessor by position

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
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when selected item index was changed
		void OnSelectionChanged() override;

		// Initializes properties
		void InitializeProperties();
	};
}

CLASS_BASES_META(o2::UIDropDown)
{
	BASE_CLASS(o2::UICustomDropDown);
}
END_META;
CLASS_FIELDS_META(o2::UIDropDown)
{
	PUBLIC_FIELD(value);
	PUBLIC_FIELD(textItem);
	PUBLIC_FIELD(onSelectedText);
}
END_META;
CLASS_METHODS_META(o2::UIDropDown)
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
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
