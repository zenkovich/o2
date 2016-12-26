#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Render/Sprite.h"
#include "ScrollArea.h"
#include "UI/VerticalLayout.h"

namespace o2
{
	// -------------------------------
	// List view widget with selection
	// -------------------------------
	class UICustomList: public UIScrollArea, public DrawableCursorEventsListener
	{
	public:
		Property<Vector<int>>     selectedItems;   // Selected item widget property
		Property<UIWidget*>       selectedItem;    // Selected item widget
		Property<int>             selectedItemPos; // Selected item position property
		Accessor<UIWidget*, int>  item;            // Items by position accessor
		Getter<int>               itemsCount;      // All items count getter
								  
		Function<void(int)>       onSelectedPos;   // Select item position event
		Function<void(UIWidget*)> onSelectedItem;  // Select item event

		// Default constructor
		UICustomList();

		// Copy-constructor
		UICustomList(const UICustomList& other);

		// Destructor
		~UICustomList();

		// Copy-operator
		UICustomList& operator=(const UICustomList& other);

		// Updates drawables, states and widget
		void Update(float dt);

		// Draws widget
		void Draw();

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

		// Sets items selection
		void SetSelectedItems(const Vector<int>& items);

		// Clears selection
		void ClearSelection();

		// Returns selected item
		Vector<int> GetSelectedItems() const;

		// Returns selected item position
		int GetSelectedItemPos() const;

		// Returns selected item widget
		UIWidget* GetSelectedItem() const;

		// Sets multi selection
		void SetMultiselectionAvailable(bool available);

		// Returns is multi selection available
		bool IsMultiselectionAvailable() const;

		// Returns selection drawable
		Sprite* GetSelectionDrawable() const;

		// Returns hover drawable
		Sprite* GetHoverDrawable() const;

		// Sets selection drawable layout (result rectangle will be calculated by item widget absolute rectangle)
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		// Sets hover drawable layout (result rectangle will be calculated by item widget absolute rectangle)
		void SetHoverDrawableLayout(const Layout& layout);

		// Returns hover drawable layout
		Layout GetHoverDrawableLayout() const;

		// Returns is listener scrollable
		bool IsScrollable() const;

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		SERIALIZABLE(UICustomList);

	protected:
		// ------------------
		// Selected item info
		// ------------------
		struct Selection
		{
			int     idx;       // Item index
			Sprite* selection; // Selection sprite

			// Check equals operator
			bool operator==(const Selection& other) const;
		};

		typedef Vector<Selection> SelectionsVec;
		typedef Vector<Sprite*> SpritesVec;

		UIVerticalLayout* mVerLayout = nullptr;                     // Child vertical layout
		UIWidget*         mItemSample = nullptr;                    // Item sample widget @SERIALIZABLE
		Sprite*           mSelectionDrawable = nullptr;             // Selection sprite @SERIALIZABLE
		Sprite*           mHoverDrawable = nullptr;                 // Item hover drawable @SERIALIZABLE
		Layout            mSelectionLayout = Layout::BothStretch(); // Selection layout, result selection area depends on selected item @SERIALIZABLE
		Layout            mHoverLayout = Layout::BothStretch();     // Hover layout, result selection area depends on selected item @SERIALIZABLE
						  							    
		bool              mMultiSelection = true;                   // Is multi selection available @SERIALIZABLE
		SelectionsVec     mSelectedItems;                           // Current selected items
						  							    
		RectF             mCurrentHoverRect;                        // Current hover rectangle (for smoothing)
		RectF             mTargetHoverRect;                         // Target hover rectangle (over selected item)
						  							    
		Vec2F             mLastHoverCheckCursor;                    // Last cursor position on hover check
		Vec2F             mLastSelectCheckCursor;                   // Last cursor position on selection check
													    
		SpritesVec        mSelectionSpritesPool;                    // Selection sprites pool

	protected:
		// Moves scroll position and updates children widgets clipping and layout
		void MoveScrollPosition(const Vec2F& delta);

		// Updates selections sprites rectangles
		void UpdateSelectionSprites();

		// Updates mouse control
		void UpdateControls(float dt);

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when scrolling
		void OnScrolled(float scroll);

		// Returns item widget under point and stores index in idxPtr, if not null
		UIWidget* GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Calls when object was deserialized and trying to reattach states animations target
		void OnDeserialized(const DataNode& node);

		// Updates transparency for this and children widgets
		void UpdateTransparency();

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// Returns selection sprite
		Sprite* GetSelectionSprite();

		// Calls when selected item index was changed
		virtual void OnSelectionChanged();

		// Calls when visible was changed
		void OnVisibleChanged();

		// Initializes properties
		void InitializeProperties();

		friend class UIDropDown;
		friend class UICustomDropDown;
	};
}
