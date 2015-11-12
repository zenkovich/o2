#pragma once

#include "UI/Widget.h"
#include "Events/CursorEventsListener.h"

namespace o2
{
	// -----------------------
	// Horizontal progress bar
	// -----------------------
	class UIHorizontalScrollBar: public UIWidget, public CursorEventsListener
	{
	public:
		Property<float>       value;          // Current value property
		Property<float>       minValue;       // Minimal value property
		Property<float>       maxValue;       // Maximal value property
		Property<float>       scrollSense;    // Scroll sense coefficient
		Property<float>       scrollSize;     // Scroll handle size
		Function<void(float)> onChange;       // On Value changing event
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
		void Update(float dt);

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
		float GetScrollhandleSize() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor moved on this (or moved outside when this was pressed)
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when scrolling
		void OnScrolled(float scroll);

		// Sets scroll handle minimum size in pixels
		void SetMinimalScrollhandleSize(float pixelSize);

		SERIALIZABLE_IMPL(UIHorizontalScrollBar);

		IOBJECT(UIHorizontalScrollBar)
		{
			BASE_CLASS(UIWidget);
			SRLZ_FIELD(mValue);
			SRLZ_FIELD(mMinValue);
			SRLZ_FIELD(mMaxValue);
			SRLZ_FIELD(mScrollSense);
			SRLZ_FIELD(mScrollHandleSize);

			FIELD(value);
			FIELD(minValue);
			FIELD(maxValue);
			FIELD(scrollSense);
			FIELD(scrollSize);
		}

	protected:
		float              mValue;                 // Current value
		float              mSmoothValue;           // Smooth value
		float              mMinValue;              // Minimum value
		float              mMaxValue;              // Maximum value
		float              mScrollSense;           // Scroll sense coefficient
		float              mScrollHandleSize;      // Scroll handle size (in value range)
		float              mScrollhandleMinPxSize; // Minimal scroll size in pixels
		float              mPressHandleOffset;     // Value offset when handle was pressed
		bool               mHandlePressed;         // True, when handle was pressed
		Ptr<UIWidgetLayer> mHandleLayer;           // Handle layer
		Ptr<UIWidgetLayer> mBackLayer;             // Background layer

	protected:
		// Updates bar, back and handle layers layout by value
		void UpdateProgressLayersLayouts();

		// Calls when layout was updated
		void OnLayoutUpdated();

		// Calls when new layer was added. Here searching bar, back and handle layers
		void OnLayerAdded(Ptr<UIWidgetLayer> layer);

		// Gets value from cursor position, depends on orientation
		float GetValueFromCursor(const Input::Cursor &cursor);

		// Initializes properties
		void InitializeProperties();
	};
}
