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
		Property<float>       value;          // Current value property
		Property<float>       minValue;       // Minimal value property
		Property<float>       maxValue;       // Maximal value property
		Property<float>       scrollSense;    // Scroll sense coefficient
		Property<float>       scrollSize;     // Scroll handle size
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
		float GetScrollHandleSize() const;

		// Sets scroll handle minimum size in pixels
		void SetMinimalScrollHandleSize(float pixelSize);

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns is listener scrollable
		bool IsScrollable() const;

		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

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
		// Updates bar, back and handle layers layout by value
		void UpdateProgressLayersLayouts();

		// It is called when layout was updated
		void OnLayoutUpdated();

		// It is called when new layer was added. Here searching bar, back and handle layers
		void OnLayerAdded(UIWidgetLayer* layer);

		// Gets value from cursor position, depends on orientation
		float GetValueFromCursor(const Input::Cursor& cursor);

		// Sets value and calls callback onUserChange
		void SetValueFromUser(float value);

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// It is called when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// It is called when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// It is called when cursor moved on this (or moved outside when this was pressed)
		void OnCursorStillDown(const Input::Cursor& cursor);

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// It is called when scrolling
		void OnScrolled(float scroll);

		// It is called when deserialized
		void OnDeserialized(const DataNode& node);

		// It is called when visible was changed
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
