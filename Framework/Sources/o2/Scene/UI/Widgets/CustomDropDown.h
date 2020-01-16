#pragma once

#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Scene/UI/Widgets/CustomList.h"

namespace o2
{
	// ---------------------------------
	// Custom items drop down ui element
	// ---------------------------------
	class CustomDropDown: public Widget, public DrawableCursorEventsListener
	{
	public:
		PROPERTIES(CustomDropDown);
		PROPERTY(Widget*, selectedItem, SelectItem, GetSelectedItem);       // Selected item widget property
		PROPERTY(int, selectedItemPos, SelectItemAt, GetSelectedItemPosition); // Selected item position property
		GETTER(int, itemsCount, GetItemsCount);                                // All items count getter

	public:
		Function<void(int)>     onSelectedPos;   // Select item position event
		Function<void(Widget*)> onSelectedItem;  // Select item event

	public:
		// Default constructor
		CustomDropDown();

		// Copy-constructor
		CustomDropDown(const CustomDropDown& other);

		// Destructor
		~CustomDropDown();

		// Copy operator
		CustomDropDown& operator=(const CustomDropDown& other);

		// Draws widget
		void Draw() override;

		// Expand list
		void Expand();

		// Collapse list
		void Collapse();

		// Returns is list expanded
		bool IsExpanded() const;

		// Sets item sample widget. WARNING: Removing all old items!
		void SetItemSample(Widget* sample);

		// Returns item sample widget
		Widget* GetItemSample() const;

		// Returns layout of items
		VerticalLayout* GetItemsLayout() const;

		// Adds new item and returns it
		Widget* AddItem();

		// Adds new item at position and returns it
		Widget* AddItem(int position);

		// Removes item
		void RemoveItem(Widget* item);

		// Removes item in position
		void RemoveItem(int position);

		// Moves item from position to new position
		void MoveItem(int position, int newPosition);

		// Moves item to new position
		void MoveItem(Widget* item, int newPosition);

		// Returns item position
		int GetItemPosition(Widget* item);

		// Returns item by position
		Widget* GetItem(int position) const;

		// Removes all items
		void RemoveAllItems();

		// Sorts items
		void SortItems(const Function<bool(Widget*, Widget*)>& sortFunc);

		// Returns items count
		int GetItemsCount() const;

		// Selects item
		void SelectItem(Widget* item);

		// Selects item at position
		void SelectItemAt(int position);

		// Returns selected item
		Widget* GetSelectedItem() const;

		// Returns selected item position
		int GetSelectedItemPosition() const;

		// Returns list view 
		CustomList* GetListView() const;

		// Sets list view size by items size
		void SetMaxListSizeInItems(int itemsCount);

		// Sets clipping layout
		void SetClippingLayout(const Layout& layout);

		// Returns clipping layout
		Layout GetClippingLayout();

		// Updates layout
		void UpdateSelfTransform() override;

		SERIALIZABLE(CustomDropDown);

	protected:
		CustomList* mItemsList = nullptr;                // List view @SERIALIZABLE
		Layout      mClipLayout = Layout::BothStretch(); // Clipping layout @SERIALIZABLE
		RectF       mAbsoluteClip;                       // Absolute clipping rectangle
		int         mMaxListItems = 10;                  // Maximum visible items in list @SERIALIZABLE

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Moves widget's to delta and checks for clipping
		void MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea) override;

		// It is called when cursor pressed on this. Sets state "pressed" to true
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time). Sets state "pressed" to false.
		// It is called onClicked if cursor is still above this
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor released outside this(only when cursor pressed this at previous time)
		void OnCursorReleasedOutside(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor enters this object. Sets state "select" to true
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object. Sets state "select" to false
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when visible was changed
		void OnResEnableInHierarchyChanged() override;

		// It is called when item was selected in list
		void OnItemSelected();

		// It is called when selection was changed
		virtual void OnSelectionChanged();
	};
}

CLASS_BASES_META(o2::CustomDropDown)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::CustomDropDown)
{
	PUBLIC_FIELD(selectedItem);
	PUBLIC_FIELD(selectedItemPos);
	PUBLIC_FIELD(itemsCount);
	PUBLIC_FIELD(onSelectedPos);
	PUBLIC_FIELD(onSelectedItem);
	PROTECTED_FIELD(mItemsList).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mClipLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAbsoluteClip);
	PROTECTED_FIELD(mMaxListItems).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::CustomDropDown)
{

	typedef const Function<bool(Widget*, Widget*)>& _tmp1;

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(void, SetItemSample, Widget*);
	PUBLIC_FUNCTION(Widget*, GetItemSample);
	PUBLIC_FUNCTION(VerticalLayout*, GetItemsLayout);
	PUBLIC_FUNCTION(Widget*, AddItem);
	PUBLIC_FUNCTION(Widget*, AddItem, int);
	PUBLIC_FUNCTION(void, RemoveItem, Widget*);
	PUBLIC_FUNCTION(void, RemoveItem, int);
	PUBLIC_FUNCTION(void, MoveItem, int, int);
	PUBLIC_FUNCTION(void, MoveItem, Widget*, int);
	PUBLIC_FUNCTION(int, GetItemPosition, Widget*);
	PUBLIC_FUNCTION(Widget*, GetItem, int);
	PUBLIC_FUNCTION(void, RemoveAllItems);
	PUBLIC_FUNCTION(void, SortItems, _tmp1);
	PUBLIC_FUNCTION(int, GetItemsCount);
	PUBLIC_FUNCTION(void, SelectItem, Widget*);
	PUBLIC_FUNCTION(void, SelectItemAt, int);
	PUBLIC_FUNCTION(Widget*, GetSelectedItem);
	PUBLIC_FUNCTION(int, GetSelectedItemPosition);
	PUBLIC_FUNCTION(CustomList*, GetListView);
	PUBLIC_FUNCTION(void, SetMaxListSizeInItems, int);
	PUBLIC_FUNCTION(void, SetClippingLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetClippingLayout);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, MoveAndCheckClipping, const Vec2F&, const RectF&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleasedOutside, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnResEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, OnItemSelected);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
}
END_META;
