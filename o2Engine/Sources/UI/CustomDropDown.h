#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "UI/CustomList.h"

namespace o2
{
	// ---------------------------------
	// Custom items drop down ui element
	// ---------------------------------
	class UICustomDropDown: public UIWidget, public DrawableCursorEventsListener
	{
	public:
		Property<UIWidget*>       selectedItem;    // Selected item widget property
		Property<int>             selectedItemPos; // Selected item position property
		Accessor<UIWidget*, int>  item;            // Items by position accessor
		Getter<int>               itemsCount;      // All items count getter

		Function<void(int)>       onSelectedPos;   // Select item position event
		Function<void(UIWidget*)> onSelectedItem;  // Select item event

		// Default constructor
		UICustomDropDown();

		// Copy-constructor
		UICustomDropDown(const UICustomDropDown& other);

		// Destructor
		~UICustomDropDown();

		// Copy operator
		UICustomDropDown& operator=(const UICustomDropDown& other);

		// Updates drawables, states and widget
		void Update(float dt);

		// Draws widget
		void Draw();

		// Expand list
		void Expand();

		// Collapse list
		void Collapse();

		// Returns is list expanded
		bool IsExpanded() const;

		// Sets item sample widget. WARNING: Removing all old items!
		void SetItemSample(UIWidget* sample);

		// Returns item sample widget
		UIWidget* GetItemSample() const;

		// Returns layout of items
		UIVerticalLayout* GetLayout() const;

		// Adds new item and returns it
		UIWidget* AddItem();

		// Adds new item at position and returns it
		UIWidget* AddItem(int position);

		// Removes item
		void RemoveItem(UIWidget* item);

		// Removes item in position
		void RemoveItem(int position);

		// Moves item from position to new position
		void MoveItem(int position, int newPosition);

		// Moves item to new position
		void MoveItem(UIWidget* item, int newPosition);

		// Returns item position
		int GetItemPosition(UIWidget* item);

		// Returns item by position
		UIWidget* GetItem(int position) const;

		// Removes all items
		void RemoveAllItems();

		// Sorts items
		void SortItems(const Function<bool(UIWidget*, UIWidget*)>& sortFunc);

		// Returns items count
		int GetItemsCount() const;

		// Selects item
		void SelectItem(UIWidget* item);

		// Selects item at position
		void SelectItemAt(int position);

		// Returns selected item
		UIWidget* GetSelectedItem() const;

		// Returns selected item position
		int GetSelectedItemPosition() const;

		// Returns list view 
		UICustomList* GetListView() const;

		// Sets list view size by items size
		void SetMaxListSizeInItems(int itemsCount);

		// Sets clipping layout
		void SetClippingLayout(const Layout& layout);

		// Returns clipping layout
		Layout GetClippingLayout();

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		SERIALIZABLE(UICustomDropDown);

	protected:
		UICustomList* mItemsList = nullptr;                // List view @SERIALIZABLE
		Layout        mClipLayout = Layout::BothStretch(); // Clipping layout @SERIALIZABLE
		RectF         mAbsoluteClip;                       // Absolute clipping rectangle
		int           mMaxListItems = 10;                  // Maximum visible items in list @SERIALIZABLE

	protected:
		// Calls when cursor pressed on this. Sets state "pressed" to true
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time). Sets state "pressed" to false.
		// Calls onClicked if cursor is still above this
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor released outside this(only when cursor pressed this at previous time)
		void OnCursorReleasedOutside(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor enters this object. Sets state "select" to true
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object. Sets state "select" to false
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when visible was changed
		void OnVisibleChanged();

		// Calls when item was selected in list
		void OnItemSelected();

		// Calls when selection was changed
		virtual void OnSelectionChanged();

		// Initializes properties
		void InitializeProperties();
	};
}
