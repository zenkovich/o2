#pragma once

#include "UI/Widget.h"
#include "Events/DrawableCursorEventsListener.h"

namespace o2
{
	// -----------------------
	// Horizontal progress bar
	// -----------------------
	class UIHorizontalProgress: public UIWidget, public DrawableCursorEventsListener
	{
	public: 
		enum class Orientation { Right, Left };

	public:
		Property<float>       value;       // Current value property
		Property<float>       minValue;    // Minimal value property
		Property<float>       maxValue;    // Maximal value property
		Property<float>       scrollSense; // Scroll sense coefficient
		Function<void(float)> onChange;    // On Value changing event

		// Constructor
		UIHorizontalProgress();

		// Copy-constructor
		UIHorizontalProgress(const UIHorizontalProgress& other);

		// Destructor
		~UIHorizontalProgress();

		// Copy-operator
		UIHorizontalProgress& operator=(const UIHorizontalProgress& other);

		// Updates widget and smooth value changing
		void Update(float dt);

		// Sets value
		void SetValue(float value);

		// Sets value forcible, without smoothing
		void SetValueForcible(float value);

		// Returns value
		float GetValue() const;

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

		// Sets bar orientation
		void SetOrientation(Orientation orientation);

		// Returns bar orientation
		Orientation GetOrientation() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns is listener scrollable
		bool IsScrollable() const;

		SERIALIZABLE(UIHorizontalProgress);

	protected:
		float          mValue;       // Current value @SERIALIZABLE
		float          mSmoothValue; // Current smoothed value
		float          mMinValue;    // Minimal value @SERIALIZABLE
		float          mMaxValue;    // Maximal value @SERIALIZABLE
		float          mScrollSense; // Scroll sense coefficient @SERIALIZABLE
		Orientation    mOrientation; // Bar orientation @SERIALIZABLE
		UIWidgetLayer* mBarLayer;    // Bar layer
		UIWidgetLayer* mBackLayer;   // background layer
 
	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Updates bar, back and handle layers layout by value
		void UpdateProgressLayersLayouts();

		// Calls when new layer was added. Here searching bar, back and handle layers
		void OnLayerAdded(UIWidgetLayer* layer);

		// Gets value from cursor position, depends on orientation
		void GetValueFromCursor(const Input::Cursor& cursor);

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
	};
}
