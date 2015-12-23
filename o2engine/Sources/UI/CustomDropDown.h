#pragma once

#include "Events/CursorEventsListener.h"
#include "UI/CustomList.h"

namespace o2
{
	// ---------------------------------
	// Custom items drop down ui element
	// ---------------------------------
	class UICustomDropDown: public UIWidget, public CursorEventsListener
	{
	public:
		Property<Ptr<UIWidget>>       selectedItem;    // Selected item widget property
		Property<int>                 selectedItemPos; // Selected item position property
		Accessor<Ptr<UIWidget>, int>  item;            // Items by position accessor
		Getter<int>                   itemsCount;      // All items count getter

		Function<void(int)>           onSelectedPos;   // Select item position event
		Function<void(Ptr<UIWidget>)> onSelectedItem;  // Select item event

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
		void SetItemSample(Ptr<UIWidget> sample);

		// Returns item sample widget
		Ptr<UIWidget> GetItemSample() const;

		// Returns layout of items
		Ptr<UIVerticalLayout> GetLayout() const;

		// Adds new item and returns it
		Ptr<UIWidget> AddItem();

		// Adds new item at position and returns it
		Ptr<UIWidget> AddItem(int position);

		// Removes item
		void RemoveItem(Ptr<UIWidget> item);

		// Removes item in position
		void RemoveItem(int position);

		// Moves item from position to new position
		void MoveItem(int position, int newPosition);

		// Moves item to new position
		void MoveItem(Ptr<UIWidget> item, int newPosition);

		// Returns item position
		int GetItemPosition(Ptr<UIWidget> item);

		// Returns item by position
		Ptr<UIWidget> GetItem(int position) const;

		// Removes all items
		void RemoveAllItems();

		// Sorts items
		void SortItems(const Function<bool(const Ptr<UIWidget>&, const Ptr<UIWidget>&)>& sortFunc);

		// Returns items count
		int GetItemsCount() const;

		// Selects item
		void SelectItem(Ptr<UIWidget> item);

		// Selects item at position
		void SelectItemAt(int position);

		// Returns selected item
		Ptr<UIWidget> GetSelectedItem() const;

		// Returns selected item position
		int GetSelectedItemPosition() const;

		// Returns list view 
		Ptr<UICustomList> GetListView() const;

		// Sets list view size by items size
		void SetMaxListSizeInItems(int itemsCount);

		// Sets clipping layout
		void SetClippingLayout(const Layout& layout);

		// Returns clipping layout
		Layout GetClippingLayout();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		SERIALIZABLE(UICustomDropDown);

	protected:
		Ptr<UICustomList> mList;         // List view @SERIALIZABLE
		Layout            mClipLayout;   // Clipping layout
		RectF             mAbsoluteClip; // Absolute clipping rectangle
		int               mMaxListItems; // Maximum visible items in list

	protected:
		// Calls when cursor pressed on this. Sets state "pressed" to true
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time). Sets state "pressed" to false.
		// Calls onClicked if cursor is still above this
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor enters this object. Sets state "select" to true
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object. Sets state "select" to false
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when visible was changed
		void OnVisibleChanged();

		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Calls when item was selected in list
		void OnItemSelected();

		// Calls when selection was changed
		virtual void OnSelectionChanged();

		// Initializes properties
		void InitializeProperties();
	};
}
