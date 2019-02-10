#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Render/Sprite.h"
#include "ScrollArea.h"
#include "Scene/UI/Widgets/VerticalLayout.h"

namespace o2
{
	// ---------------------------------------------
	// List view widget with selection and many data
	// ---------------------------------------------
	class LongList: public ScrollArea, public DrawableCursorEventsListener
	{
	public:
		typedef Vector<UnknownType*> UnknownsVec;

	public:
		PROPERTIES(LongList);
		PROPERTY(int, selectedItemPos, SelectItemAt, GetSelectedItemPosition); // Selected item position property	

		Function<void(int)>                     onFocused;         // Select item position event

		Function<int()>                         getItemsCountFunc; // Items count getting function
		Function<UnknownsVec(int, int)>         getItemsRangeFunc; // Items getting in range function
		Function<void(Widget*, UnknownType*)> setupItemFunc;     // Setup item widget function

	    // Default constructor
		LongList();

		// Copy-constructor
		LongList(const LongList& other);

		// Destructor
		~LongList();

		// Copy-operator
		LongList& operator=(const LongList& other);

		// Updates drawables, states and widget
		void Update(float dt) override;

		// Draws widget
		void Draw() override;

		// Sets item sample widget. WARNING: Removing all old items!
		void SetItemSample(Widget* sample);

		// Returns item sample widget
		Widget* GetItemSample() const;

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

		// Updates items
		void OnItemsUpdated(bool itemsRearranged = false);

		// Returns is listener scrollable
		bool IsScrollable() const override;

		// Updates layout
		void UpdateSelfTransform() override;

		SERIALIZABLE(LongList);

	protected:
		Widget*         mItemSample = nullptr;                    // Item sample widget @SERIALIZABLE

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
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when object was deserialized and trying to reattach states animations target
		void OnDeserialized(const DataNode& node) override;

		// It is called when visible was changed
		void OnResEnableInHierarchyChanged() override;

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
		Widget* GetItemUnderPoint(const Vec2F& point, int* idxPtr);

		// Updates hover
		void UpdateHover(const Vec2F& point);

		// Updates selection
		void UpdateSelection(int position);

		// It is called when selected item index was changed
		void OnSelectionChanged();

		friend class DropDown;
		friend class CustomDropDown;
	};
}

CLASS_BASES_META(o2::LongList)
{
	BASE_CLASS(o2::ScrollArea);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::LongList)
{
	PUBLIC_FIELD(selectedItemPos);
	PUBLIC_FIELD(onFocused);
	PUBLIC_FIELD(getItemsCountFunc);
	PUBLIC_FIELD(getItemsRangeFunc);
	PUBLIC_FIELD(setupItemFunc);
	PROTECTED_FIELD(mItemSample).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelectionDrawable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHoverDrawable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSelectionLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHoverLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mMinVisibleItemIdx);
	PROTECTED_FIELD(mMaxVisibleItemIdx);
	PROTECTED_FIELD(mSelectedItem);
	PROTECTED_FIELD(mCurrentSelectionRect);
	PROTECTED_FIELD(mTargetSelectionRect);
	PROTECTED_FIELD(mCurrentHoverRect);
	PROTECTED_FIELD(mTargetHoverRect);
	PROTECTED_FIELD(mLastHoverCheckCursor);
	PROTECTED_FIELD(mLastSelectCheckCursor);
	PROTECTED_FIELD(mItemsPool);
}
END_META;
CLASS_METHODS_META(o2::LongList)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetItemSample, Widget*);
	PUBLIC_FUNCTION(Widget*, GetItemSample);
	PUBLIC_FUNCTION(void, SelectItemAt, int);
	PUBLIC_FUNCTION(int, GetSelectedItemPosition);
	PUBLIC_FUNCTION(Sprite*, GetSelectionDrawable);
	PUBLIC_FUNCTION(Sprite*, GetHoverDrawable);
	PUBLIC_FUNCTION(void, SetSelectionDrawableLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetSelectionDrawableLayout);
	PUBLIC_FUNCTION(void, SetHoverDrawableLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetHoverDrawableLayout);
	PUBLIC_FUNCTION(void, OnItemsUpdated, bool);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, OnResEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, CalculateScrollArea);
	PROTECTED_FUNCTION(void, UpdateControls, float);
	PROTECTED_FUNCTION(void, MoveScrollPosition, const Vec2F&);
	PROTECTED_FUNCTION(void, UpdateVisibleItems);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(Widget*, GetItemUnderPoint, const Vec2F&, int*);
	PROTECTED_FUNCTION(void, UpdateHover, const Vec2F&);
	PROTECTED_FUNCTION(void, UpdateSelection, int);
	PROTECTED_FUNCTION(void, OnSelectionChanged);
}
END_META;
