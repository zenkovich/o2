#pragma once

#include "UI/Widget.h"
#include "Events/CursorEventsListener.h"

namespace o2
{
	class UIHorizontalScrollBar;
	class UIVerticalScrollBar;

	// -----------------------------------------------------
	// Scrolling area with scrollbars and clipping rectangle
	// -----------------------------------------------------
	class UIScrollArea: public UIWidget
	{
	public:
		Property<Vec2F> scroll;    // Scroll position property
		Property<float> horScroll; // Horizontal scroll position property
		Property<float> verScroll; // Vertical scroll position property
		Layout          clipArea;  // Clipping area layout (relative to this widget layout)
		Layout          viewArea;  // Children view area layout

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
		void SetHorizontalScrollBar(Ptr<UIHorizontalScrollBar> scrollbar, bool owner = true);

		// Returns horizontal scroll bar
		Ptr<UIHorizontalScrollBar> GetHorizontalScrollbar() const;

		// Sets Vertical scroll bar
		void SetVerticalScrollBar(Ptr<UIVerticalScrollBar> scrollbar, bool owner = true);

		// Returns Vertical scroll bar
		Ptr<UIVerticalScrollBar> GetVerticalScrollbar() const;

		// Sets scroll bars hiding
		void SetEnableScrollsHiding(bool hideScrolls);

		// Returns is scroll bars hiding
		bool IsScrollsHiding();

		SERIALIZABLE_IMPL(UIScrollArea);

		IOBJECT(UIScrollArea)
		{
			BASE_CLASS(UIWidget);

			FIELD(scroll);
			FIELD(horScroll);
			FIELD(verScroll);
			FIELD(mHorScrollBar);
			FIELD(mVerScrollBar);
			FIELD(mOwnHorScrollBar);
			FIELD(mOwnVerScrollBar);

			SRLZ_FIELD(clipArea);
			SRLZ_FIELD(mScrollPos);
		}

	protected:
		Ptr<UIHorizontalScrollBar> mHorScrollBar;        // horizontal scroll bar
		Ptr<UIVerticalScrollBar>   mVerScrollBar;        // Vertical scroll bar
		bool                       mOwnHorScrollBar;     // True, if this widget is owner of mHorScrollBar
		bool                       mOwnVerScrollBar;     // True, if this widget is owner of mVerScrollBar
		RectF                      mAbsoluteClipArea;    // Clipping area
		RectF                      mAbsoluteViewArea;    // View area
		Vec2F                      mScrollPos;           // Scroll position
		Vec2F                      mScrollSpeed;         // Scrolling speed
		float                      mScrollSpeedDamp;     // Scroll speed damping
		RectF                      mScrollArea;          // Maximum scroll area size
		RectF                      mScrollRange;         // Scroll range by width and height
		float                      mDrawDepth;           // Drawing depth at current frame
		bool                       mUnderCursor;         // True, when widget is under cursor
		bool                       mPressedCursor;       // True, when widget was pressed by cursor
		Vec2F                      mPressedScroll;       // Scroll position when cursor was pressed
		Vec2F                      mPressedCursorPos;    // Cursor position, when was pressed
		float                      mSpeedUpdTime;        // Elapsed time from last scroll speed updating
		bool                       mEnableScrollsHiding; // Enables scroll hiding when they are stopped

	protected:
		// Updates mouse control
		void UpdateControls(float dt);

		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Updates scroll parameters: clip area, scroll size
		void UpdateScrollParams();

		// Calls when horizontal scroll bar value was changed
		void OnHorScrollChanged(float value);

		// Calls when vertical scroll bar value was changed
		void OnVerScrollChanged(float value);

		// Beginning serialization callback
		void OnSerialize(DataNode& node);

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Initializes properties
		void InitializeProperties();
	};
}
