#pragma once

#include "Render/Sprite.h"
#include "ScrollArea.h"
#include "UI/VerticalLayout.h"

namespace o2
{
	// -------------------------------
	// List view widget with selection
	// -------------------------------
	class UICustomList: public UIScrollArea
	{
	public:
		Property<Ptr<UIWidget>>       selectedItem;    // Selected item widget property
		Property<int>                 selectedItemPos; // Selected item position property
		Accessor<Ptr<UIWidget>, int>  item;            // Items by position accessor
		Getter<int>                   itemsCount;      // All items count getter

		Function<void(int)>           onSelectedPos;   // Select item position event
		Function<void(Ptr<UIWidget>)> onSelectedItem;  // Select item event

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

		// Returns selection drawable
		Ptr<Sprite> GetSelectionDrawable() const;

		// Returns hover drawable
		Ptr<Sprite> GetHoverDrawable() const;

		// Sets selection drawable layout (result rectangle will be calculated by item widget absolute rectangle)
		void SetSelectionDrawableLayout(const Layout& layout);

		// Returns selection drawable layout
		Layout GetSelectionDrawableLayout() const;

		// Sets hover drawable layout (result rectangle will be calculated by item widget absolute rectangle)
		void SetHoverDrawableLayout(const Layout& layout);

		// Returns hover drawable layout
		Layout GetHoverDrawableLayout() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		SERIALIZABLE_IMPL(UICustomList);

		IOBJECT(UICustomList)
		{
			BASE_CLASS(UIScrollArea);

			FIELD(selectedItem);
			FIELD(selectedItemPos);
			FIELD(item);
			FIELD(itemsCount);

			SRLZ_FIELD(mItemSample);
			SRLZ_FIELD(mSelectionDrawable);
			SRLZ_FIELD(mHoverDrawable);
			SRLZ_FIELD(mSelectionLayout);
			SRLZ_FIELD(mHoverLayout);

			FIELD(mVerLayout);
			FIELD(mSelectedItem);
			FIELD(mCurrentSelectionRect);
			FIELD(mTargetSelectionRect);
			FIELD(mCurrentHoverRect);
			FIELD(mTargetHoverRect);
		}

	protected:
		Ptr<UIVerticalLayout> mVerLayout;             // Child vertical layout
		Ptr<UIWidget>         mItemSample;            // Item sample widget
		Ptr<Sprite>           mSelectionDrawable;     // Selection sprite
		Ptr<Sprite>           mHoverDrawable;         // Item hover drawable
		Layout                mSelectionLayout;       // Selection layout, result selection area depends on selected item
		Layout                mHoverLayout;           // Hover layout, result selection area depends on selected item

		int                   mSelectedItem;          // Position of current selected item (-1 if no item isn't selected)

		RectF                 mCurrentSelectionRect;  // Current selection rectangle (for smoothing)
		RectF                 mTargetSelectionRect;   // Target selection rectangle (over selected item)
		RectF                 mCurrentHoverRect;      // Current hover rectangle (for smoothing)
		RectF                 mTargetHoverRect;       // Target hover rectangle (over selected item)

		Vec2F                 mLastHoverCheckCursor;  // Last cursor position on hover check
		Vec2F                 mLastSelectCheckCursor; // Last cursor position on selection check

	protected:
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

		// Returns item widget under point and stores index in idxPtr, if not null
		Ptr<UIWidget> GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Calls when object was deserialized and trying to reattach states animations target
		void OnDeserialized(const DataNode& node);

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// Updates selection
		void UpdateSelection(int position, Ptr<UIWidget> item);

		// Calls when scrolled - updates selection and hover drawables
		void OnScrolled();

		// Calls when selected item index was changed
		virtual void OnSelectionChanged();

		// Initializes properties
		void InitializeProperties();
	};
}
