#pragma once

#include "o2/Scene/UI/Widget.h"
#include "o2/Events/DrawableCursorEventsListener.h"

namespace o2
{
	// -----------------------
	// Horizontal progress bar
	// -----------------------
	class HorizontalProgress: public Widget, public DrawableCursorEventsListener
	{
	public:
		enum class Orientation { Right, Left };

	public:
		PROPERTIES(HorizontalProgress);
		PROPERTY(float, value, SetValue, GetValue);          // Current value property

		PROPERTY(float, minValue, SetMinValue, GetMinValue); // Minimal value property
		PROPERTY(float, maxValue, SetMaxValue, GetMaxValue); // Maximal value property

		PROPERTY(float, scrollSense, SetScrollSense, GetScrollSense); // Scroll sense coefficient

	public:
		Function<void(float)> onChange;      // On Value changed event
		Function<void(float)> onChangeByUser; // On Value changed by user event

	public:
		// Constructor
		HorizontalProgress();

		// Copy-constructor
		HorizontalProgress(const HorizontalProgress& other);

		// Destructor
		~HorizontalProgress();

		// Copy-operator
		HorizontalProgress& operator=(const HorizontalProgress& other);

		// Updates widget and smooth value changing
		void Update(float dt) override;

		// Sets value
		void SetValue(float value, bool byUser = false);

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

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(HorizontalProgress);

	protected:
		float        mValue = 0.0f;                     // Current value @SERIALIZABLE
		float        mSmoothValue = 0.0f;               // Current smoothed value
		float        mMinValue = 0.0f;                  // Minimal value @SERIALIZABLE
		float        mMaxValue = 1.0f;                  // Maximal value @SERIALIZABLE
		float        mScrollSense = 1.0f;               // Scroll sense coefficient @SERIALIZABLE
		Orientation  mOrientation = Orientation::Right; // Bar orientation @SERIALIZABLE
		WidgetLayer* mBarLayer = nullptr;               // Bar layer
		WidgetLayer* mBackLayer = nullptr;              // background layer

	protected:
		// It is called when new layer was added. Here searching bar, back and handle layers
		void OnLayerAdded(WidgetLayer* layer) override;

		// It is called when deserialized
		void OnDeserialized(const DataValue& node) override;

		// It is called when visible was changed
		void OnEnableInHierarchyChanged() override;

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
	};
}

PRE_ENUM_META(o2::HorizontalProgress::Orientation);

CLASS_BASES_META(o2::HorizontalProgress)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::HorizontalProgress)
{
	FIELD().PUBLIC().NAME(value);
	FIELD().PUBLIC().NAME(minValue);
	FIELD().PUBLIC().NAME(maxValue);
	FIELD().PUBLIC().NAME(scrollSense);
	FIELD().PUBLIC().NAME(onChange);
	FIELD().PUBLIC().NAME(onChangeByUser);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(mValue);
	FIELD().PROTECTED().DEFAULT_VALUE(0.0f).NAME(mSmoothValue);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(mMinValue);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(mMaxValue);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(mScrollSense);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Orientation::Right).NAME(mOrientation);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mBarLayer);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mBackLayer);
}
END_META;
CLASS_METHODS_META(o2::HorizontalProgress)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const HorizontalProgress&);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValue, float, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueForcible, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetValue);
	FUNCTION().PUBLIC().SIGNATURE(void, SetMinValue, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetMinValue);
	FUNCTION().PUBLIC().SIGNATURE(void, SetMaxValue, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetMaxValue);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueRange, float, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetScrollSense, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetScrollSense);
	FUNCTION().PUBLIC().SIGNATURE(void, SetOrientation, Orientation);
	FUNCTION().PUBLIC().SIGNATURE(Orientation, GetOrientation);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsScrollable);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuGroup);
	FUNCTION().PROTECTED().SIGNATURE(void, OnLayerAdded, WidgetLayer*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnableInHierarchyChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateLayersLayouts);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateProgressLayersLayouts);
	FUNCTION().PROTECTED().SIGNATURE(void, GetValueFromCursor, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressBreak, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorStillDown, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorEnter, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorExit, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnScrolled, float);
}
END_META;
