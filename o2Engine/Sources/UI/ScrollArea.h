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
		void Draw() override;

		// Updates widget
		void Update(float dt) override;

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

		// Updates layout
		void UpdateTransform(bool withChildren = true) override;

		// Updates childs, calculates scroll rectangle and updates scrollbars
		void UpdateChildren(float dt) override;

		// Updates children transforms, calculates scroll rectangle and updates scrollbars
		void UpdateChildrenTransforms() override;

		SERIALIZABLE(UIScrollArea);

	protected:
		UIHorizontalScrollBar* mHorScrollBar = nullptr;                 // horizontal scroll bar
		UIVerticalScrollBar*   mVerScrollBar = nullptr;                 // Vertical scroll bar
		bool                   mOwnHorScrollBar = false;                // True, if this widget is owner of mHorScrollBar
		bool                   mOwnVerScrollBar = false;                // True, if this widget is owner of mVerScrollBar

		bool                   mLayoutUpdated = false;                  // Is is true when layout was updated on current frame

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
		bool                   mShowScrollBarsByCursor = false;         // Enables showing scroll bars when cursor hovers them @SERIALIZABLE
		float                  mLastHorScrollChangeTime = -1.0f;        // Last time when horizontal scroll bar was changed
		float                  mLastVerScrollChangeTime = -1.0f;        // Last time when horizontal scroll bar was changed

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Beginning serialization callback
		void OnSerialize(DataNode& node) const override;

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node) override;

		// It is called when transformation was changed and updated
		void OnTransformUpdated() override;

		// It is called when child widget was added
		void OnChildAdded(UIWidget* child) override;

		// It is called when child widget was removed
		void OnChildRemoved(UIWidget* child) override;

		// Checks widget clipping by area
		void CheckClipping(const RectF& clipArea) override;

		// Updates transparency for this and children widgets
		void UpdateTransparency() override;

		// Moves widget's to delta and checks for clipping
		void MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea) override;

		// Updates mouse control
		virtual void UpdateControls(float dt);

		// Moves scroll position and updates children widgets clipping and layout
		virtual void MoveScrollPosition(const Vec2F& delta);

		// Calculates scroll area
		virtual void CalculateScrollArea();

		// Updates scroll parameters: clip area, scroll size
		virtual void UpdateScrollParams();

		//Calls when widget was scrolled
		virtual void OnScrolled();

		// Check scroll bars showing
		void CheckScrollBarsVisibility();

		// Updates scrollbars layouts
		void UpdateScrollBarsLayout();

		// Checks children clippings
		void CheckChildrenClipping();

		// It is called when horizontal scroll bar value was changed
		void OnHorScrollChanged(float value);

		// It is called when vertical scroll bar value was changed
		void OnVerScrollChanged(float value);

		// Initializes properties
		void InitializeProperties();

		friend class UICustomDropDown;
	};
}

CLASS_BASES_META(o2::UIScrollArea)
{
	BASE_CLASS(o2::UIWidget);
}
END_META;
CLASS_FIELDS_META(o2::UIScrollArea)
{
	PUBLIC_FIELD(scroll);
	PUBLIC_FIELD(horScroll);
	PUBLIC_FIELD(verScroll);
	PUBLIC_FIELD(onScrolled);
	PROTECTED_FIELD(mHorScrollBar);
	PROTECTED_FIELD(mVerScrollBar);
	PROTECTED_FIELD(mOwnHorScrollBar);
	PROTECTED_FIELD(mOwnVerScrollBar);
	PROTECTED_FIELD(mLayoutUpdated);
	PROTECTED_FIELD(mViewAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAbsoluteViewArea);
	PROTECTED_FIELD(mClipAreaLayout).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAbsoluteClipArea);
	PROTECTED_FIELD(mScrollPos).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollSpeed);
	PROTECTED_FIELD(mScrollSpeedDamp).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollArea);
	PROTECTED_FIELD(mScrollRange);
	PROTECTED_FIELD(mEnableHorScroll);
	PROTECTED_FIELD(mEnableVerScroll);
	PROTECTED_FIELD(mUnderCursor);
	PROTECTED_FIELD(mPressedCursor);
	PROTECTED_FIELD(mPressedScroll);
	PROTECTED_FIELD(mPressedCursorPos);
	PROTECTED_FIELD(mSpeedUpdTime);
	PROTECTED_FIELD(mEnableScrollsHiding).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mShowScrollBarsByCursor).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLastHorScrollChangeTime);
	PROTECTED_FIELD(mLastVerScrollChangeTime);
}
END_META;
CLASS_METHODS_META(o2::UIScrollArea)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetScroll, const Vec2F&);
	PUBLIC_FUNCTION(void, SetScrollForcible, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetScroll);
	PUBLIC_FUNCTION(RectF, GetScrollRange);
	PUBLIC_FUNCTION(void, ResetScroll);
	PUBLIC_FUNCTION(void, SetHorizontalScroll, float);
	PUBLIC_FUNCTION(float, GetHorizontalScroll);
	PUBLIC_FUNCTION(void, SetVerticalScroll, float);
	PUBLIC_FUNCTION(float, GetVerticalScroll);
	PUBLIC_FUNCTION(void, SetHorizontalScrollBar, UIHorizontalScrollBar*, bool);
	PUBLIC_FUNCTION(UIHorizontalScrollBar*, GetHorizontalScrollbar);
	PUBLIC_FUNCTION(void, SetVerticalScrollBar, UIVerticalScrollBar*, bool);
	PUBLIC_FUNCTION(UIVerticalScrollBar*, GetVerticalScrollbar);
	PUBLIC_FUNCTION(void, SetEnableScrollsHiding, bool);
	PUBLIC_FUNCTION(bool, IsScrollsHiding);
	PUBLIC_FUNCTION(void, SetScrollBarsShowingByCursor, bool);
	PUBLIC_FUNCTION(bool, IsScrollBarsShowingByCursor);
	PUBLIC_FUNCTION(void, SetClippingLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetClippingLayout);
	PUBLIC_FUNCTION(void, SetViewLayout, const Layout&);
	PUBLIC_FUNCTION(Layout, GetViewLayout);
	PUBLIC_FUNCTION(void, UpdateTransform, bool);
	PUBLIC_FUNCTION(void, UpdateChildren, float);
	PUBLIC_FUNCTION(void, UpdateChildrenTransforms);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, OnChildAdded, UIWidget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, UIWidget*);
	PROTECTED_FUNCTION(void, CheckClipping, const RectF&);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, MoveAndCheckClipping, const Vec2F&, const RectF&);
	PROTECTED_FUNCTION(void, UpdateControls, float);
	PROTECTED_FUNCTION(void, MoveScrollPosition, const Vec2F&);
	PROTECTED_FUNCTION(void, CalculateScrollArea);
	PROTECTED_FUNCTION(void, UpdateScrollParams);
	PROTECTED_FUNCTION(void, OnScrolled);
	PROTECTED_FUNCTION(void, CheckScrollBarsVisibility);
	PROTECTED_FUNCTION(void, UpdateScrollBarsLayout);
	PROTECTED_FUNCTION(void, CheckChildrenClipping);
	PROTECTED_FUNCTION(void, OnHorScrollChanged, float);
	PROTECTED_FUNCTION(void, OnVerScrollChanged, float);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
