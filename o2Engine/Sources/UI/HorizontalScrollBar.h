#pragma once

#include "UI/Widget.h"
#include "Events/DrawableCursorEventsListener.h"

namespace o2
{
	// -----------------------
	// Horizontal progress bar
	// -----------------------
	class UIHorizontalScrollBar: public UIWidget, public DrawableCursorEventsListener
	{
	public:
		PROPERTY(float>       value;          // Current value property

		PROPERTY(float>       minValue;       // Minimal value property
		PROPERTY(float>       maxValue;       // Maximal value property

		PROPERTY(float>       scrollSense;    // Scroll sense coefficient
		PROPERTY(float>       scrollSize;     // Scroll handle size

		Function<void(float)> onChange;       // On Value changing event
		Function<void(float)> onUserChange;   // On Value changing from user event
		Function<void(float)> onSmoothChange; // On smooth value changing event

		// Constructor
		UIHorizontalScrollBar();

		// Copy-constructor
		UIHorizontalScrollBar(const UIHorizontalScrollBar& other);

		// Destructor
		~UIHorizontalScrollBar();

		// Copy-operator
		UIHorizontalScrollBar& operator=(const UIHorizontalScrollBar& other);

		// Updates widget and smooth value changing
		void Update(float dt) override;

		// Sets value
		void SetValue(float value);

		// Sets value forcible, without smoothing
		void SetValueForcible(float value);

		// Returns value
		float GetValue() const;

		// Returns smooth value
		float GetSmoothValue() const;

		// Sets minimal value
		void SetMinValue(float minValue);

		// Returns minimal value
		float GetMinValue() const;

		// Sets maximal value
		void SetMaxValue(float maxValue);

		// Returns maximal value
		float GetMaxValue() const;

		// Sets value range
		void SetValueRange(float minValue, float maxValue);

		// Sets scroll sense
		void SetScrollSense(float coef);

		// Returns scroll sense
		float GetScrollSense() const;

		// Sets scroll handle size
		void SetScrollHandleSize(float size);

		// Returns scroll handle size
		float GetScrollHandleSize() const;

		// Sets scroll handle minimum size in pixels
		void SetMinimalScrollHandleSize(float pixelSize);

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns is listener scrollable
		bool IsScrollable() const override;

		SERIALIZABLE(UIHorizontalScrollBar);

	protected:
		float          mValue = 0.0f;                 // Current value @SERIALIZABLE
		float          mSmoothValue = 0.0f;           // Smooth value
		float          mMinValue = 0.0f;              // Minimum value @SERIALIZABLE
		float          mMaxValue = 1.0f;              // Maximum value @SERIALIZABLE
		float          mScrollSense = 1.0f;           // Scroll sense coefficient @SERIALIZABLE
		float          mScrollHandleSize = 0.2f;      // Scroll handle size (in value range) @SERIALIZABLE
		float          mScrollhandleMinPxSize = 5.0f; // Minimal scroll size in pixels @SERIALIZABLE
		float          mPressHandleOffset = 0.0f;     // Value offset when handle was pressed
		bool           mHandlePressed = false;        // True, when handle was pressed
		UIWidgetLayer* mHandleLayer = nullptr;        // Handle layer
		UIWidgetLayer* mBackLayer = nullptr;          // Background layer

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when deserialized
		void OnDeserialized(const DataNode& node) override;

		// It is called when visible was changed
		void OnVisibleChanged() override;

		// Updates layers layouts, calls after updating widget layout
		void UpdateLayersLayouts() override;

		// It is called when new layer was added. Here searching bar, back and handle layers
		void OnLayerAdded(UIWidgetLayer* layer) override;

		// Updates bar, back and handle layers layout by value
		void UpdateProgressLayersLayouts();

		// Gets value from cursor position, depends on orientation
		float GetValueFromCursor(const Input::Cursor& cursor);

		// Sets value and calls callback onUserChange
		void SetValueFromUser(float value);

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor) override;

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor) override;

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorStillDown(const Input::Cursor& cursor) override;

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when scrolling
		void OnScrolled(float scroll) override;

		// Initializes properties
		void InitializeProperties();

		friend class UIContextMenu;
		friend class UICustomList;
		friend class UIEditBox;
		friend class UILongList;
		friend class UIScrollArea;
		friend class UITree;
	};
}

CLASS_BASES_META(o2::UIHorizontalScrollBar)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::UIHorizontalScrollBar)
{
	PUBLIC_FIELD(value);
	PUBLIC_FIELD(minValue);
	PUBLIC_FIELD(maxValue);
	PUBLIC_FIELD(scrollSense);
	PUBLIC_FIELD(scrollSize);
	PUBLIC_FIELD(onChange);
	PUBLIC_FIELD(onUserChange);
	PUBLIC_FIELD(onSmoothChange);
	PROTECTED_FIELD(mValue).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSmoothValue);
	PROTECTED_FIELD(mMinValue).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mMaxValue).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollSense).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollHandleSize).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollhandleMinPxSize).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPressHandleOffset);
	PROTECTED_FIELD(mHandlePressed);
	PROTECTED_FIELD(mHandleLayer);
	PROTECTED_FIELD(mBackLayer);
}
END_META;
CLASS_METHODS_META(o2::UIHorizontalScrollBar)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetValue, float);
	PUBLIC_FUNCTION(void, SetValueForcible, float);
	PUBLIC_FUNCTION(float, GetValue);
	PUBLIC_FUNCTION(float, GetSmoothValue);
	PUBLIC_FUNCTION(void, SetMinValue, float);
	PUBLIC_FUNCTION(float, GetMinValue);
	PUBLIC_FUNCTION(void, SetMaxValue, float);
	PUBLIC_FUNCTION(float, GetMaxValue);
	PUBLIC_FUNCTION(void, SetValueRange, float, float);
	PUBLIC_FUNCTION(void, SetScrollSense, float);
	PUBLIC_FUNCTION(float, GetScrollSense);
	PUBLIC_FUNCTION(void, SetScrollHandleSize, float);
	PUBLIC_FUNCTION(float, GetScrollHandleSize);
	PUBLIC_FUNCTION(void, SetMinimalScrollHandleSize, float);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, OnLayerAdded, UIWidgetLayer*);
	PROTECTED_FUNCTION(void, UpdateProgressLayersLayouts);
	PROTECTED_FUNCTION(float, GetValueFromCursor, const Input::Cursor&);
	PROTECTED_FUNCTION(void, SetValueFromUser, float);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
