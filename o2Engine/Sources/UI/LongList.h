#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Render/Sprite.h"
#include "ScrollArea.h"
#include "UI/VerticalLayout.h"

namespace o2
{
	// ---------------------------------------------
	// List view widget with selection and many data
	// ---------------------------------------------
	class UILongList: public UIScrollArea, public DrawableCursorEventsListener
	{
	public:
		typedef Vector<UnknownType*> UnknownsVec;

	public:
		Property<int>                           selectedItemPos;   // Selected item position property	

		Function<void(int)>                     onFocused;         // Select item position event

		Function<int()>                         getItemsCountFunc; // Items count getting function
		Function<UnknownsVec(int, int)>         getItemsRangeFunc; // Items getting in range function
		Function<void(UIWidget*, UnknownType*)> setupItemFunc;     // Setup item widget function

	    // Default constructor
		UILongList();

		// Copy-constructor
		UILongList(const UILongList& other);

		// Destructor
		~UILongList();

		// Copy-operator
		UILongList& operator=(const UILongList& other);

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Draws widget
		void Draw() override;

		// Sets item sample widget. WARNING: Removing all old items!
		void SetItemSample(UIWidget* sample);

		// Returns item sample widget
		UIWidget* GetItemSample() const;

		// Selects item at position
		void SelectItemAt(int position);

		// Returns selected item position
		int GetSelectedItemPosition() const;

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
		void UpdateLayout(bool withChildren = true) override;

		SERIALIZABLE(UILongList);

	protected:
		UIWidget*         mItemSample = nullptr;                    // Item sample widget @SERIALIZABLE

		Sprite*           mSelectionDrawable = nullptr;             // Selection sprite @SERIALIZABLE
		Sprite*           mHoverDrawable = nullptr;                 // Item hover drawable @SERIALIZABLE
		Layout            mSelectionLayout = Layout::BothStretch(); // Selection layout, result selection area depends on selected item @SERIALIZABLE
		Layout            mHoverLayout = Layout::BothStretch();     // Hover layout, result selection area depends on selected item @SERIALIZABLE
													    
		int               mMinVisibleItemIdx = -1;                  // Visible item with minimal index
		int               mMaxVisibleItemIdx = -1;                  // Visible item with maximal index
		int               mSelectedItem = -1;                       // Position of current selected item (-1 if no item isn't selected)
													    
		RectF             mCurrentSelectionRect;                    // Current selection rectangle (for smoothing)
		RectF             mTargetSelectionRect;                     // Target selection rectangle (over selected item)
		RectF             mCurrentHoverRect;                        // Current hover rectangle (for smoothing)
		RectF             mTargetHoverRect;                         // Target hover rectangle (over selected item)
													    
		Vec2F             mLastHoverCheckCursor;                    // Last cursor position on hover check
		Vec2F             mLastSelectCheckCursor;                   // Last cursor position on selection check
													    
		WidgetsVec        mItemsPool;                               // Items pool

	protected:
		// It is called when object was deserialized and trying to reattach states animations target
		void OnDeserialized(const DataNode& node) override;

		// It is called when visible was changed
		void OnVisibleChanged() override;

		// Updates transparency for this and children widgets
		void UpdateTransparency() override;

		// Calculates scroll area
		void CalculateScrollArea() override;

		// Updates mouse control
		void UpdateControls(float dt) override;

		// Moves scroll position and updates children widgets clipping and layout
		void MoveScrollPosition(const Vec2F& delta) override;

		// Updates visible items
		void UpdateVisibleItems();

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
		UIWidget* GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// Updates selection
		void UpdateSelection(int position);

		// It is called when selected item index was changed
		void OnSelectionChanged();

		// Updates items
		void OnItemsUpdated(bool itemsRearranged = false);

		// Initializes properties
		void InitializeProperties();

		friend class UIDropDown;
		friend class UICustomDropDown;
	};
}
