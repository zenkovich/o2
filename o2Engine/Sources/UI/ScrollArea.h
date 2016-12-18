#pragma once

#include "UI/HorizontalScrollBar.h"
#include "UI/VerticalScrollBar.h"
#include "UI/Widget.h"

namespace o2
{
	// -----------------------------------------------------
	// Scrolling area with scrollbars and clipping rectangle
	// -----------------------------------------------------
	class UIScrollArea: public UIWidget
	{
	public:
		Property<Vec2F> scroll;     // Scroll position property
		Property<float> horScroll;  // Horizontal scroll position property
		Property<float> verScroll;  // Vertical scroll position property

		Function<void(const Vec2F&)> onScrolled; // Scrolling event. Scroll position is parameter

		// Default constructor
		UIScrollArea();

		// Copy-constructor
		UIScrollArea(const UIScrollArea& other);

		// Destructor
		~UIScrollArea();

		// Copy-operator
		UIScrollArea& operator=(const UIScrollArea& other);

		// Draws widget
		void Draw();

		// Updates widget
		void Update(float dt);

		// Sets scroll position
		void SetScroll(const Vec2F& scroll);

		// Sets scroll position
		void SetScrollForcible(const Vec2F& scroll);

		// Returns scroll position
		Vec2F GetScroll() const;

		// Returns scroll range by width and height
		RectF GetScrollRange() const;

		// Resets scroll to left top corner
		void ResetScroll();

		// Sets horizontal scroll position
		void SetHorizontalScroll(float scroll);

		// Returns horizontal scroll position
		float GetHorizontalScroll() const;

		// Sets vertical scroll position
		void SetVerticalScroll(float scroll);

		// Returns vertical scroll position
		float GetVerticalScroll() const;

		// Sets horizontal scroll bar
		void SetHorizontalScrollBar(UIHorizontalScrollBar* scrollbar, bool owner = true);

		// Returns horizontal scroll bar
		UIHorizontalScrollBar* GetHorizontalScrollbar() const;

		// Sets Vertical scroll bar
		void SetVerticalScrollBar(UIVerticalScrollBar* scrollbar, bool owner = true);

		// Returns Vertical scroll bar
		UIVerticalScrollBar* GetVerticalScrollbar() const;

		// Sets scroll bars hiding
		void SetEnableScrollsHiding(bool hideScrolls);

		// Returns is scroll bars hiding
		bool IsScrollsHiding();

		// Sets clipping layout
		void SetClippingLayout(const Layout& clipLayout);

		// Returns clipping layout
		Layout GetClippingLayout() const;

		// Sets view layout
		void SetViewLayout(const Layout& viewLayout);

		// Returns view layout
		Layout GetViewLayout() const;

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		SERIALIZABLE(UIScrollArea);

	protected:
		UIHorizontalScrollBar* mHorScrollBar = nullptr;                 // horizontal scroll bar
		UIVerticalScrollBar*   mVerScrollBar = nullptr;                 // Vertical scroll bar
		bool                   mOwnHorScrollBar = false;                // True, if this widget is owner of mHorScrollBar
		bool                   mOwnVerScrollBar = false;                // True, if this widget is owner of mVerScrollBar

		Layout                 mViewAreaLayout = Layout::BothStretch(); // Children view area layout with disabled bars @SERIALIZABLE
		RectF                  mAbsoluteViewArea;                       // View area

		Layout                 mClipAreaLayout = Layout::BothStretch(); // Clipping area layout (relative to this widget layout) @SERIALIZABLE
		RectF                  mAbsoluteClipArea;                       // Clipping area

		Vec2F                  mScrollPos;                              // Scroll position @SERIALIZABLE
		Vec2F                  mScrollSpeed;                            // Scrolling speed
		float                  mScrollSpeedDamp = 7.0f;                 // Scroll speed damping  @SERIALIZABLE
		RectF                  mScrollArea;                             // Maximum scroll area size
		RectF                  mScrollRange;                            // Scroll range by width and height
		bool                   mEnableHorScroll = false;                // Is horizontal scroll enabled (range > 0)
		bool                   mEnableVerScroll = false;                // Is vertical scroll enabled (range > 0)

		bool                   mUnderCursor = false;                    // True, when widget is under cursor
		bool                   mPressedCursor = false;                  // True, when widget was pressed by cursor
		Vec2F                  mPressedScroll;                          // Scroll position when cursor was pressed
		Vec2F                  mPressedCursorPos;                       // Cursor position, when was pressed

		float                  mSpeedUpdTime = 0.0f;                    // Elapsed time from last scroll speed updating

		bool                   mEnableScrollsHiding = true;             // Enables scroll hiding when they are stopped @SERIALIZABLE
		float                  mLastHorScrollChangeTime = -1.0f;        // Last time when horizontal scroll bar was changed
		float                  mLastVerScrollChangeTime = -1.0f;        // Last time when horizontal scroll bar was changed

	protected:
		// Calls when child widget was added
		void OnChildAdded(UIWidget* child);

		// Calls when child widget was removed
		void OnChildRemoved(UIWidget* child);

		// Updates mouse control
		virtual void UpdateControls(float dt);

		// Check scroll bars showing
		void CheckScrollBarsVisibility();

		// Moves scroll position and updates children widgets clipping and layout
		virtual void MoveScrollPosition(const Vec2F& delta);

		// Moves widget's to delta and checks for clipping
		void MoveWidgetAndCheckClipping(UIWidget* widget, const Vec2F& delta);

		// Updates scrollbars layouts
		void UpdateScrollBarsLayout();

		// Checks children clippings
		void CheckChildrenClipping();

		// Checks widget clipping by area
		void CheckClipping(const RectF& clipArea);

		// Updates transparency for this and children widgets
		void UpdateTransparency();

		// Calculates scroll area
		virtual void CalculateScrollArea();

		// Updates scroll parameters: clip area, scroll size
		virtual void UpdateScrollParams();

		// Calls when horizontal scroll bar value was changed
		void OnHorScrollChanged(float value);

		// Calls when vertical scroll bar value was changed
		void OnVerScrollChanged(float value);

		// Beginning serialization callback
		void OnSerialize(DataNode& node) const;

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		//Calls when widget was scrolled
		virtual void OnScrolled();

		// Initializes properties
		void InitializeProperties();

		friend class UICustomDropDown;
	};
}
