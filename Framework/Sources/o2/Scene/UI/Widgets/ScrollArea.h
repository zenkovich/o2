#pragma once

#include "o2/Scene/UI/Widgets/HorizontalScrollBar.h"
#include "o2/Scene/UI/Widgets/VerticalScrollBar.h"
#include "o2/Scene/UI/Widget.h"

namespace o2
{
	// -----------------------------------------------------
	// Scrolling area with scrollbars and clipping rectangle
	// -----------------------------------------------------
	class ScrollArea: public Widget, virtual public CursorAreaEventsListener
	{
	public:
		PROPERTIES(ScrollArea);
		PROPERTY(Vec2F, scroll, SetScroll, GetScroll);                        // Scroll position property
		PROPERTY(float, horScroll, SetHorizontalScroll, GetHorizontalScroll); // Horizontal scroll position property
		PROPERTY(float, verScroll, SetVerticalScroll, GetVerticalScroll);     // Vertical scroll position property

	public:
		Function<void(const Vec2F&)> onScrolled; // Scrolling event. Scroll position is parameter

	public:
		// Default constructor
		ScrollArea();

		// Copy-constructor
		ScrollArea(const ScrollArea& other);

		// Destructor
		~ScrollArea();

		// Copy-operator
		ScrollArea& operator=(const ScrollArea& other);

		// Draws widget
		void Draw() override;

		// Updates widget
		void Update(float dt) override;

		// Updates children
		void UpdateChildren(float dt) override;

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
		void SetHorizontalScrollBar(HorizontalScrollBar* scrollbar, bool owner = true);

		// Returns horizontal scroll bar
		HorizontalScrollBar* GetHorizontalScrollbar() const;

		// Sets Vertical scroll bar
		void SetVerticalScrollBar(VerticalScrollBar* scrollbar, bool owner = true);

		// Returns Vertical scroll bar
		VerticalScrollBar* GetVerticalScrollbar() const;

		// Sets scroll bars hiding
		void SetEnableScrollsHiding(bool hideScrolls);

		// Returns is scroll bars hiding
		bool IsScrollsHiding();

		// Sets is scroll bars will shown when cursor hover them
		void SetScrollBarsShowingByCursor(bool showByCursor);

		// Returns is scroll bars will shown when cursor hover them
		bool IsScrollBarsShowingByCursor();

		// Sets clipping layout
		void SetClippingLayout(const Layout& clipLayout);

		// Returns clipping layout
		Layout GetClippingLayout() const;

		// Sets view layout
		void SetViewLayout(const Layout& viewLayout);

		// Returns view layout
		Layout GetViewLayout() const;

		// Updates children transforms, calculates scroll rectangle and updates scrollbars
		void UpdateChildrenTransforms() override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns is listener scrollable
		bool IsScrollable() const override;

		// Returns true when input events can be handled by down listeners
		bool IsInputTransparent() const override;

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(ScrollArea);

	protected:
		HorizontalScrollBar*   mHorScrollBar = nullptr; // horizontal scroll bar
		VerticalScrollBar*     mVerScrollBar = nullptr; // Vertical scroll bar

		bool mOwnHorScrollBar = false; // True, if this widget is owner of mHorScrollBar
		bool mOwnVerScrollBar = false; // True, if this widget is owner of mVerScrollBar

		bool mLayoutUpdated = false; // Is is true when layout was updated on current frame

		Layout mViewAreaLayout = Layout::BothStretch(); // Children view area layout with disabled bars @SERIALIZABLE
		RectF  mAbsoluteViewArea;                       // View area

		Layout mClipAreaLayout = Layout::BothStretch(); // Clipping area layout (relative to this widget layout) @SERIALIZABLE
		RectF  mAbsoluteClipArea;                       // Clipping area

		Vec2F mScrollPos;               // Scroll position @SERIALIZABLE
		bool  mHasScrolled = false;     // True when area has scrolled at this frame, resets to false on update
		Vec2F mScrollSpeed;             // Scrolling speed
		float mScrollSpeedDamp = 7.0f;  // Scroll speed damping  @SERIALIZABLE
		RectF mScrollArea;              // Maximum scroll area size
		RectF mScrollRange;             // Scroll range by width and height
		bool  mEnableHorScroll = false; // Is horizontal scroll enabled (range > 0)
		bool  mEnableVerScroll = false; // Is vertical scroll enabled (range > 0)

		bool  mUnderCursor = false;   // True, when widget is under cursor
		bool  mPressedCursor = false; // True, when widget was pressed by cursor
		Vec2F mPressedScroll;         // Scroll position when cursor was pressed
		Vec2F mPressedCursorPos;      // Cursor position, when was pressed

		float mSpeedUpdTime = 0.0f; // Elapsed time from last scroll speed updating

		bool  mEnableScrollsHiding = true;      // Enables scroll hiding when they are stopped @SERIALIZABLE
		bool  mShowScrollBarsByCursor = false;  // Enables showing scroll bars when cursor hovers them @SERIALIZABLE
		float mLastHorScrollChangeTime = -1.0f; // Last time when horizontal scroll bar was changed
		float mLastVerScrollChangeTime = -1.0f; // Last time when horizontal scroll bar was changed

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		// It is called when child widget was added
		void OnChildAdded(Widget* child) override;

		// It is called when child widget was removed
		void OnChildRemoved(Widget* child) override;

		// Sets children world rectangle. Moves children rectangle to scroll position offset
		void SetChildrenWorldRect(const RectF& childrenWorldRect) override;

		// Checks widget clipping by area
		void CheckClipping(const RectF& clipArea) override;

		// It is called when scrolling
		void OnScrolled(float scroll) override;

		// Moves scroll position and updates children widgets clipping and layout
		virtual void MoveScrollPosition(const Vec2F& delta);

		// Calculates scroll area
		virtual void CalculateScrollArea();

		// Calculates mScrollArea bounds by new childrenRectangle
		void RecalculateScrollAreaRect(const RectF& childRect, const Vec2F& offset);

		// Initializes mScrollArea by default rectangle and calculates offset
		void InitializeScrollAreaRectCalculation(Vec2F& offset);

		// Updates scroll parameters: clip area, scroll size
		virtual void UpdateScrollParams();

		//Calls when widget was scrolled
		virtual void OnScrolled();

		// Check scroll bars showing
		void CheckScrollBarsVisibility();

		// Checks children clippings
		void CheckChildrenClipping();

		// It is called when horizontal scroll bar value was changed
		void OnHorScrollChanged(float value);

		// It is called when vertical scroll bar value was changed
		void OnVerScrollChanged(float value);

		friend class CustomDropDown;
	};
}

CLASS_BASES_META(o2::ScrollArea)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::CursorAreaEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::ScrollArea)
{
	PUBLIC_FIELD(scroll);
	PUBLIC_FIELD(horScroll);
	PUBLIC_FIELD(verScroll);
	PUBLIC_FIELD(onScrolled);
	PROTECTED_FIELD(mHorScrollBar).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mVerScrollBar).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mOwnHorScrollBar).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mOwnVerScrollBar).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mLayoutUpdated).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mViewAreaLayout).DEFAULT_VALUE(Layout::BothStretch()).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAbsoluteViewArea);
	PROTECTED_FIELD(mClipAreaLayout).DEFAULT_VALUE(Layout::BothStretch()).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAbsoluteClipArea);
	PROTECTED_FIELD(mScrollPos).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHasScrolled).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mScrollSpeed);
	PROTECTED_FIELD(mScrollSpeedDamp).DEFAULT_VALUE(7.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollArea);
	PROTECTED_FIELD(mScrollRange);
	PROTECTED_FIELD(mEnableHorScroll).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mEnableVerScroll).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mUnderCursor).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mPressedCursor).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mPressedScroll);
	PROTECTED_FIELD(mPressedCursorPos);
	PROTECTED_FIELD(mSpeedUpdTime).DEFAULT_VALUE(0.0f);
	PROTECTED_FIELD(mEnableScrollsHiding).DEFAULT_VALUE(true).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mShowScrollBarsByCursor).DEFAULT_VALUE(false).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLastHorScrollChangeTime).DEFAULT_VALUE(-1.0f);
	PROTECTED_FIELD(mLastVerScrollChangeTime).DEFAULT_VALUE(-1.0f);
}
END_META;
CLASS_METHODS_META(o2::ScrollArea)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, UpdateChildren, float);
	PUBLIC_FUNCTION(void, SetScroll, const Vec2F&);
	PUBLIC_FUNCTION(void, SetScrollForcible, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetScroll);
	PUBLIC_FUNCTION(RectF, GetScrollRange);
	PUBLIC_FUNCTION(void, ResetScroll);
	PUBLIC_FUNCTION(void, SetHorizontalScroll, float);
	PUBLIC_FUNCTION(float, GetHorizontalScroll);
	PUBLIC_FUNCTION(void, SetVerticalScroll, float);
	PUBLIC_FUNCTION(float, GetVerticalScroll);
	PUBLIC_FUNCTION(void, SetHorizontalScrollBar, HorizontalScrollBar*, bool);
	PUBLIC_FUNCTION(HorizontalScrollBar*, GetHorizontalScrollbar);
	PUBLIC_FUNCTION(void, SetVerticalScrollBar, VerticalScrollBar*, bool);
	PUBLIC_FUNCTION(VerticalScrollBar*, GetVerticalScrollbar);
	PUBLIC_FUNCTION(void, SetEnableScrollsHiding, bool);
	PUBLIC_FUNCTION(bool, IsScrollsHiding);
	PUBLIC_FUNCTION(void, SetScrollBarsShowingByCursor, bool);
	PUBLIC_FUNCTION(bool, IsScrollBarsShowingByCursor);
	PUBLIC_FUNCTION(void, SetClippingLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetClippingLayout);
	PUBLIC_FUNCTION(void, SetViewLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetViewLayout);
	PUBLIC_FUNCTION(void, UpdateChildrenTransforms);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(bool, IsInputTransparent);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, OnChildAdded, Widget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, Widget*);
	PROTECTED_FUNCTION(void, SetChildrenWorldRect, const RectF&);
	PROTECTED_FUNCTION(void, CheckClipping, const RectF&);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(void, MoveScrollPosition, const Vec2F&);
	PROTECTED_FUNCTION(void, CalculateScrollArea);
	PROTECTED_FUNCTION(void, RecalculateScrollAreaRect, const RectF&, const Vec2F&);
	PROTECTED_FUNCTION(void, InitializeScrollAreaRectCalculation, Vec2F&);
	PROTECTED_FUNCTION(void, UpdateScrollParams);
	PROTECTED_FUNCTION(void, OnScrolled);
	PROTECTED_FUNCTION(void, CheckScrollBarsVisibility);
	PROTECTED_FUNCTION(void, CheckChildrenClipping);
	PROTECTED_FUNCTION(void, OnHorScrollChanged, float);
	PROTECTED_FUNCTION(void, OnVerScrollChanged, float);
}
END_META;
