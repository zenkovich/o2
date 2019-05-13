#pragma once

#include "Scene/UI/Widgets/CustomDropDown.h"

namespace o2
{
	// --------------
	// Text drop down
	// --------------
	class DropDown: public CustomDropDown
	{
	public:
		PROPERTIES(DropDown);
		PROPERTY(WString, value, SelectItemText, GetSelectedItemText); // Current selected item text

	public:
		Function<void(const WString&)> onSelectedText; // Change text selected event

	public:
		// Default constructor
		DropDown();

		// Copy-constructor
		DropDown(const DropDown& other);

		// Destructor
		~DropDown();

		// Copy operator
		DropDown& operator=(const DropDown& other);

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

		SERIALIZABLE(DropDown);

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when selected item index was changed
		void OnSelectionChanged() override;
	};
}

CLASS_BASES_META(o2::DropDown)
{
	BASE_CLASS(o2::CustomDropDown);
}
END_META;
CLASS_FIELDS_META(o2::DropDown)
{
	PUBLIC_FIELD(value);
	PUBLIC_FIELD(onSelectedText);
}
END_META;
CLASS_METHODS_META(o2::DropDown)
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
}
END_META;
