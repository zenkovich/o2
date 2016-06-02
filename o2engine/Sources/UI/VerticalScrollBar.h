#pragma once

#include "UI/Widget.h"
#include "Events/DrawableCursorEventsListener.h"

namespace o2
{
	// ---------------------
	// Vertical progress bar
	// ---------------------
	class UIVerticalScrollBar: public UIWidget, public DrawableCursorEventsListener
	{
	public:
		Property<float>       value;          // Current value property
		Property<float>       minValue;       // Minimal value property
		Property<float>       maxValue;       // Maximal value property
		Property<float>       scrollSense;    // Scroll sense coefficient
		Property<float>       scrollSize;     // Scroll handle size
		Function<void(float)> onChange;       // On value changing event
		Function<void(float)> onSmoothChange; // On smooth value changing event

		// Constructor
		UIVerticalScrollBar();

		// Copy-constructor
		UIVerticalScrollBar(const UIVerticalScrollBar& other);

		// Destructor
		~UIVerticalScrollBar();

		// Copy-operator
		UIVerticalScrollBar& operator=(const UIVerticalScrollBar& other);

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

		// Sets scroll handle minimum size in pixels
		void SetMinimalScrollhandleSize(float pixelSize);

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns is listener scrollable
		bool IsScrollable() const;

		SERIALIZABLE(UIVerticalScrollBar);

	protected:
		float          mValue = 0.0f;                 // Current value @SERIALIZABLE
		float          mSmoothValue = 0.0f;           // Smooth value
		float          mMinValue = 0.0f;              // Minimum value @SERIALIZABLE
		float          mMaxValue = 1.0f;              // Maximum value @SERIALIZABLE
		float          mScrollSense = 1.0f;           // Scroll sense coefficient @SERIALIZABLE
		float          mScrollHandleSize = 0.2f;      // Scroll handle size (in value range) @SERIALIZABLE
		float          mScrollhandleMinPxSize = 5.0f; // Minimal scroll size in pixels
		float          mPressHandleOffset = 0.0f;     // Value offset when handle was pressed
		bool           mHandlePressed = false;        // True, when handle was pressed
		UIWidgetLayer* mHandleLayer = nullptr;        // Handle layer
		UIWidgetLayer* mBackLayer = nullptr;          // Background layer

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Updates bar, back and handle layers layout by value
		void UpdateProgressLayersLayouts();

		// Calls when layout was updated
		void OnLayoutUpdated();

		// Calls when new layer was added. Here searching bar, back and handle layers
		void OnLayerAdded(UIWidgetLayer* layer);

		// Gets value from cursor position, depends on orientation
		float GetValueFromCursor(const Input::Cursor& cursor);

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

		// Calls when deserialized
		void OnDeserialized(const DataNode& node);

		// Calls when visible was changed
		void OnVisibleChanged();

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

