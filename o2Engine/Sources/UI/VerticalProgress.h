#pragma once

#include "UI/Widget.h"
#include "Events/DrawableCursorEventsListener.h"

namespace o2
{
	// ---------------------
	// Vertical progress bar
	// ---------------------
	class UIVerticalProgress: public UIWidget, public DrawableCursorEventsListener
	{
	public:
		enum class Orientation { Down, Up };

	public:
		Property<float>       value;       // Current value property

		Property<float>       minValue;    // Minimal value property
		Property<float>       maxValue;    // Maximal value property

		Property<float>       scrollSense; // Scroll sense coefficient

		Function<void(float)> onChange;    // On Value changing event

		// Constructor
		UIVerticalProgress();

		// Copy-constructor
		UIVerticalProgress(const UIVerticalProgress& other);

		// Destructor
		~UIVerticalProgress();

		// Copy-operator
		UIVerticalProgress& operator=(const UIVerticalProgress& other);

		// Updates widget and smooth value changing
		void Update(float dt) override;

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
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns is listener scrollable
		bool IsScrollable() const override;

		SERIALIZABLE(UIVerticalProgress);

	protected:
		float          mValue = 0.0f;                    // Current value @SERIALIZABLE
		float          mSmoothValue = 0.0f;              // Current smoothed value
		float          mMinValue = 0.0f;                 // Minimal value @SERIALIZABLE
		float          mMaxValue = 1.0f;                 // Maximal value @SERIALIZABLE
		float          mScrollSense = 1.0f;              // Scroll sense coefficient @SERIALIZABLE
		Orientation    mOrientation = Orientation::Down; // Bar orientation @SERIALIZABLE
		UIWidgetLayer* mBarLayer = nullptr;              // Bar layer
		UIWidgetLayer* mBackLayer = nullptr;             // Background layer

	protected:
		// It is called when new layer was added. Here searching bar, back and handle layers
		void OnLayerAdded(UIWidgetLayer* layer) override;

		// It is called when deserialized
		void OnDeserialized(const DataNode& node) override;

		// It is called when visible was changed
		void OnVisibleChanged() override;

		// Updates layers layouts, calls after updating widget layout
		void UpdateLayersLayouts() override;

		// Updates bar, back and handle layers layout by value
		void UpdateProgressLayersLayouts();

		// Gets value from cursor position, depends on orientation
		void GetValueFromCursor(const Input::Cursor& cursor);

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
	};
}
