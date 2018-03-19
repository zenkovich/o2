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
		PROPERTIES(UICustomDropDown);
		PROPERTY(UIWidget*, selectedItem, SelectItem, GetSelectedItem);       // Selected item widget property
		PROPERTY(int, selectedItemPos, SelectItemAt, GetSelectedItemPosition); // Selected item position property
		GETTER(int, itemsCount, GetItemsCount);                                // All items count getter

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

		// Draws widget
		void Draw() override;

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
		void UpdateTransform(bool withChildren = true) override;

		SERIALIZABLE(UICustomDropDown);

	protected:
		UICustomList* mItemsList = nullptr;                // List view @SERIALIZABLE
		Layout        mClipLayout = Layout::BothStretch(); // Clipping layout @SERIALIZABLE
		RectF         mAbsoluteClip;                       // Absolute clipping rectangle
		int           mMaxListItems = 10;                  // Maximum visible items in list @SERIALIZABLE

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
		void OnVisibleChanged() override;

		// It is called when item was selected in list
		void OnItemSelected();

		// It is called when selection was changed
		virtual void OnSelectionChanged();
	};
}

CLASS_BASES_META(o2::UICustomDropDown)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::UICustomDropDown)
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
CLASS_METHODS_META(o2::UICustomDropDown)
{

	typedef const Function<bool(UIWidget*, UIWidget*)>& _tmp1;

	PUBLIC_FUNCTION(void, PROPERTIES, UICustomDropDown);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(void, SetItemSample, UIWidget*);
	PUBLIC_FUNCTION(UIWidget*, GetItemSample);
	PUBLIC_FUNCTION(UIVerticalLayout*, GetLayout);
	PUBLIC_FUNCTION(UIWidget*, AddItem);
	PUBLIC_FUNCTION(UIWidget*, AddItem, int);
	PUBLIC_FUNCTION(void, RemoveItem, UIWidget*);
	PUBLIC_FUNCTION(void, RemoveItem, int);
	PUBLIC_FUNCTION(void, MoveItem, int, int);
	PUBLIC_FUNCTION(void, MoveItem, UIWidget*, int);
	PUBLIC_FUNCTION(int, GetItemPosition, UIWidget*);
	PUBLIC_FUNCTION(UIWidget*, GetItem, int);
	PUBLIC_FUNCTION(void, RemoveAllItems);
	PUBLIC_FUNCTION(void, SortItems, _tmp1);
	PUBLIC_FUNCTION(int, GetItemsCount);
	PUBLIC_FUNCTION(void, SelectItem, UIWidget*);
	PUBLIC_FUNCTION(void, SelectItemAt, int);
	PUBLIC_FUNCTION(UIWidget*, GetSelectedItem);
	PUBLIC_FUNCTION(int, GetSelectedItemPosition);
	PUBLIC_FUNCTION(UICustomList*, GetListView);
	PUBLIC_FUNCTION(void, SetMaxListSizeInItems, int);
	PUBLIC_FUNCTION(void, SetClippingLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetClippingLayout);
	PUBLIC_FUNCTION(void, UpdateTransform, bool);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, MoveAndCheckClipping, const Vec2F&, const RectF&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleasedOutside, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(void, OnItemSelected);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
}
END_META;
