#pragma once

#include "UI/CustomList.h"
#include "UI/Button.h"

namespace o2
{
	// ---------------------------------
	// Custom items drop down ui element
	// ---------------------------------
	class UICustomDropDown: public UIWidget 
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

		// Returns down button
		Ptr<UIButton> GetDownButton() const;

		// Sets list view size by items size
		void SetListSizeInItems(int itemsCount);

		SERIALIZABLE_IMPL(UICustomDropDown);

		IOBJECT(UICustomDropDown)
		{
			BASE_CLASS(UIWidget);

			FIELD(selectedItem);
			FIELD(selectedItemPos);
			FIELD(item);
			FIELD(itemsCount);

			SRLZ_FIELD(mList);
			SRLZ_FIELD(mDownButton);
		}

	protected:
		Ptr<UICustomList> mList;       // List view
		Ptr<UIButton>     mDownButton; // Down button

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Initializes properties
		void InitializeProperties();
	};
}
