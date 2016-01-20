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
		Property<Vec2F>              scroll;     // Scroll position property
		Property<float>              horScroll;  // Horizontal scroll position property
		Property<float>              verScroll;  // Vertical scroll position property

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

		// Returns scroll position
		Vec2F GetScroll() const;

		// Returns scroll range by width and height
		RectF GetScrollRange() const;

		// Resets scroll to left top corner
		void ResetSroll();

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

		SERIALIZABLE(UIScrollArea);

	protected:
		UIHorizontalScrollBar* mHorScrollBar;            // horizontal scroll bar
		UIVerticalScrollBar*   mVerScrollBar;            // Vertical scroll bar
		bool                   mOwnHorScrollBar;         // True, if this widget is owner of mHorScrollBar
		bool                   mOwnVerScrollBar;         // True, if this widget is owner of mVerScrollBar

		Layout                 mViewAreaLayout;          // Children view area layout with disabled bars @SERIALIZABLE
		RectF                  mAbsoluteViewArea;        // View area

		Layout                 mClipAreaLayout;          // Clipping area layout (relative to this widget layout) @SERIALIZABLE
		RectF                  mAbsoluteClipArea;        // Clipping area
													     
		Vec2F                  mScrollPos;               // Scroll position @SERIALIZABLE
		Vec2F                  mScrollSpeed;             // Scrolling speed
		float                  mScrollSpeedDamp;         // Scroll speed damping
		RectF                  mScrollArea;              // Maximum scroll area size
		RectF                  mScrollRange;             // Scroll range by width and height
		bool                   mEnableHorScroll;         // Is horizontal scroll enabled (range > 0)
		bool                   mEnableVerScroll;         // Is vertical scroll enabled (range > 0)
													     
		float                  mDrawDepth;               // Drawing depth at current frame
													     
		bool                   mUnderCursor;             // True, when widget is under cursor
		bool                   mPressedCursor;           // True, when widget was pressed by cursor
		Vec2F                  mPressedScroll;           // Scroll position when cursor was pressed
		Vec2F                  mPressedCursorPos;        // Cursor position, when was pressed
													     
		float                  mSpeedUpdTime;            // Elapsed time from last scroll speed updating
													     
		bool                   mEnableScrollsHiding;     // Enables scroll hiding when they are stopped
		float                  mLastHorScrollChangeTime; // Last time when horizontal scroll bar was changed
		float                  mLastVerScrollChangeTime; // Last time when horizontal scroll bar was changed

	protected:
		// Calls when child widget was added
		void OnChildAdded(UIWidget* child);

		// Calls when child widget was removed
		void OnChildRemoved(UIWidget* child);

		// Updates mouse control
		virtual void UpdateControls(float dt);

		// Check scroll bars showing
		void CheckScrollBarsVisibility();

		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Updates transparency for this and children widgets
		void UpdateTransparency();

		// Updates scroll parameters: clip area, scroll size
		virtual void UpdateScrollParams();

		// Calls when horizontal scroll bar value was changed
		void OnHorScrollChanged(float value);

		// Calls when vertical scroll bar value was changed
		void OnVerScrollChanged(float value);

		// Beginning serialization callback
		void OnSerialize(DataNode& node);

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		//Calls when widget was scrolled
		virtual void OnScrolled();

		// Initializes properties
		void InitializeProperties();

		friend class UICustomDropDown;
	};
}
