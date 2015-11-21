#pragma once

#include "UI/Widget.h"
#include "Events/CursorEventsListener.h"

namespace o2
{
	// -----------------------
	// Horizontal progress bar
	// -----------------------
	class UIHorizontalProgress: public UIWidget, public CursorEventsListener
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

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		// Returns is listener scrollable
		bool IsScrollable() const;

		SERIALIZABLE_IMPL(UIHorizontalProgress);

		IOBJECT(UIHorizontalProgress)
		{
			BASE_CLASS(UIWidget);
			SRLZ_FIELD(mValue);
			SRLZ_FIELD(mMinValue);
			SRLZ_FIELD(mMaxValue);
			SRLZ_FIELD(mScrollSense);
			SRLZ_FIELD(mOrientation);

			FIELD(value);
			FIELD(minValue);
			FIELD(maxValue);
			FIELD(scrollSense);
		}

	protected:
		float              mValue;       // Current value
		float              mSmoothValue; // Current smoothed value
		float              mMinValue;    // Minimal value
		float              mMaxValue;    // Maximal value
		float              mScrollSense; // Scroll sense coefficient
		Orientation        mOrientation; // Bar orientation
		Ptr<UIWidgetLayer> mBarLayer;    // Bar layer
		Ptr<UIWidgetLayer> mBackLayer;   // background layer
 
	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Updates bar, back and handle layers layout by value
		void UpdateProgressLayersLayouts();

		// Calls when new layer was added. Here searching bar, back and handle layers
		void OnLayerAdded(Ptr<UIWidgetLayer> layer);

		// Gets value from cursor position, depends on orientation
		void GetValueFromCursor(const Input::Cursor &cursor);

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

		// Initializes properties
		void InitializeProperties();
	};
}
