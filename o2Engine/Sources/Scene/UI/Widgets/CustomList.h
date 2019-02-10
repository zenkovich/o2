#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Render/Sprite.h"
#include "ScrollArea.h"
#include "Scene/UI/Widgets/VerticalLayout.h"

namespace o2
{
	// -------------------------------
	// List view widget with selection
	// -------------------------------
	class CustomList: public ScrollArea, public DrawableCursorEventsListener
	{
	public:
		PROPERTIES(CustomList);
		PROPERTY(Vector<int>, selectedItems, SetSelectedItems, GetSelectedItems); // Selected item widget property
		PROPERTY(Widget*, selectedItem, SelectItem, GetSelectedItem);           // Selected item widget
		PROPERTY(int, selectedItemPos, SelectItemAt, GetSelectedItemPos);         // Selected item position property
		GETTER(int, itemsCount, GetItemsCount);                                   // All items count getter

		Function<void(int)>       onSelectedPos;   // Select item position event
		Function<void(Widget*)> onSelectedItem;  // Select item event

		// Default constructor
		CustomList();

		// Copy-constructor
		CustomList(const CustomList& other);

		// Destructor
		~CustomList();

		// Copy-operator
		CustomList& operator=(const CustomList& other);

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Draws widget
		void Draw() override;

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

		// Sets items selection
		void SetSelectedItems(const Vector<int>& items);

		// Clears selection
		void ClearSelection();

		// Returns selected item
		Vector<int> GetSelectedItems() const;

		// Returns selected item position
		int GetSelectedItemPos() const;

		// Returns selected item widget
		Widget* GetSelectedItem() const;

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
		bool IsScrollable() const override;

		// Updates layout
		void UpdateSelfTransform() override;

		SERIALIZABLE(CustomList);

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

		VerticalLayout* mVerLayout = nullptr;                     // Child vertical layout
		Widget*         mItemSample = nullptr;                    // Item sample widget @SERIALIZABLE

		bool              mMultiSelection = true;                   // Is multi selection available @SERIALIZABLE
		SelectionsVec     mSelectedItems;                           // Current selected items

		Sprite*           mSelectionDrawable = nullptr;             // Selection sprite @SERIALIZABLE
		Sprite*           mHoverDrawable = nullptr;                 // Item hover drawable @SERIALIZABLE

		Layout            mSelectionLayout = Layout::BothStretch(); // Selection layout, result selection area depends on selected item @SERIALIZABLE
		Layout            mHoverLayout = Layout::BothStretch();     // Hover layout, result selection area depends on selected item @SERIALIZABLE

		RectF             mCurrentHoverRect;                        // Current hover rectangle (for smoothing)
		RectF             mTargetHoverRect;                         // Target hover rectangle (over selected item)

		Vec2F             mLastHoverCheckCursor;                    // Last cursor position on hover check
		Vec2F             mLastSelectCheckCursor;                   // Last cursor position on selection check

		SpritesVec        mSelectionSpritesPool;                    // Selection sprites pool

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when object was deserialized and trying to reattach states animations target
		void OnDeserialized(const DataNode& node) override;

		// It is called when visible was changed
		void OnResEnableInHierarchyChanged() override;

		// Updates transparency for this and children widgets
		void UpdateTransparency() override;

		// Updates mouse control
		void UpdateControls(float dt) override;

		// It is called when transformation was changed and updated
		void OnTransformUpdated() override;

		// It is called when selected item index was changed
		virtual void OnSelectionChanged();

		// Moves scroll position and updates children widgets clipping and layout
		void MoveScrollPosition(const Vec2F& delta);

		// Updates selections sprites rectangles
		void UpdateSelectionSprites();

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorMoved(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when scrolling
		void OnScrolled(float scroll) override;

		// Returns item widget under point and stores index in idxPtr, if not null
		Widget* GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// Returns selection sprite
		Sprite* GetSelectionSprite();

		friend class DropDown;
		friend class CustomDropDown;
	};
}

CLASS_BASES_META(o2::CustomList)
{
	BASE_CLASS(o2::ScrollArea);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::CustomList)
{
	PUBLIC_FIELD(selectedItems);
	PUBLIC_FIELD(selectedItem);
	PUBLIC_FIELD(selectedItemPos);
	PUBLIC_FIELD(itemsCount);
	PUBLIC_FIELD(onSelectedPos);
	PUBLIC_FIELD(onSelectedItem);
	PROTECTED_FIELD(mVerLayout);
	PROTECTED_FIELD(mItemSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mMultiSelection).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelectedItems);
	PROTECTED_FIELD(mSelectionDrawable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHoverDrawable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelectionLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHoverLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mCurrentHoverRect);
	PROTECTED_FIELD(mTargetHoverRect);
	PROTECTED_FIELD(mLastHoverCheckCursor);
	PROTECTED_FIELD(mLastSelectCheckCursor);
	PROTECTED_FIELD(mSelectionSpritesPool);
}
END_META;
CLASS_METHODS_META(o2::CustomList)
{

	typedef const Function<bool(Widget*, Widget*)>& _tmp1;

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
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
	PUBLIC_FUNCTION(void, SetSelectedItems, const Vector<int>&);
	PUBLIC_FUNCTION(void, ClearSelection);
	PUBLIC_FUNCTION(Vector<int>, GetSelectedItems);
	PUBLIC_FUNCTION(int, GetSelectedItemPos);
	PUBLIC_FUNCTION(Widget*, GetSelectedItem);
	PUBLIC_FUNCTION(void, SetMultiselectionAvailable, bool);
	PUBLIC_FUNCTION(bool, IsMultiselectionAvailable);
	PUBLIC_FUNCTION(Sprite*, GetSelectionDrawable);
	PUBLIC_FUNCTION(Sprite*, GetHoverDrawable);
	PUBLIC_FUNCTION(void, SetSelectionDrawableLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetSelectionDrawableLayout);
	PUBLIC_FUNCTION(void, SetHoverDrawableLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetHoverDrawableLayout);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, OnResEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, UpdateControls, float);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
	PROTECTED_FUNCTION(void, MoveScrollPosition, const Vec2F&);
	PROTECTED_FUNCTION(void, UpdateSelectionSprites);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(Widget*, GetItemUnderPoint, const Vec2F&, int*);
	PROTECTED_FUNCTION(void, UpdateHover, const Vec2F&);
	PROTECTED_FUNCTION(Sprite*, GetSelectionSprite);
}
END_META;
