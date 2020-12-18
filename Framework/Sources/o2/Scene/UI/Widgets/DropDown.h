#pragma once

#include "o2/Scene/UI/Widgets/CustomDropDown.h"

namespace o2
{
	class Text;

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

		// Draws widget
		void Draw() override;

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

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(DropDown);

	protected:
		Text* mSelectedText = nullptr; // Selected text label, draws separately

	protected:
		// It is called when selected item index was changed
		void OnSelectionChanged() override;

		// It is called when layer added and updates drawing sequence, searches selected text drawable
		void OnLayerAdded(WidgetLayer* layer) override;
	};
}

CLASS_BASES_META(o2::DropDown)
{
	BASE_CLASS(o2::CustomDropDown);
}
END_META;
CLASS_FIELDS_META(o2::DropDown)
{
	FIELD().NAME(value).PUBLIC();
	FIELD().NAME(onSelectedText).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mSelectedText).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::DropDown)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(int, AddItem, const WString&);
	PUBLIC_FUNCTION(int, AddItem, const WString&, int);
	PUBLIC_FUNCTION(void, AddItems, const Vector<WString>&);
	PUBLIC_FUNCTION(void, RemoveItem, const WString&);
	PUBLIC_FUNCTION(int, FindItem, const WString&);
	PUBLIC_FUNCTION(WString, GetItemText, int);
	PUBLIC_FUNCTION(Vector<WString>, GetAllItemsText);
	PUBLIC_FUNCTION(WString, GetSelectedItemText);
	PUBLIC_FUNCTION(void, SelectItemText, const WString&);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
	PROTECTED_FUNCTION(void, OnLayerAdded, WidgetLayer*);
}
END_META;
