#pragma once

#include "o2/Scene/UI/Widget.h"
#include "o2/Events/DrawableCursorEventsListener.h"

namespace o2
{
	// -----------------------
	// Horizontal progress bar
	// -----------------------
	class HorizontalScrollBar: public Widget, public DrawableCursorEventsListener
	{
	public:
		PROPERTIES(HorizontalScrollBar);
		PROPERTY(float, value, SetValue, GetValue);          // Current value property

		PROPERTY(float, minValue, SetMinValue, GetMinValue); // Minimal value property
		PROPERTY(float, maxValue, SetMaxValue, GetMaxValue); // Maximal value property

		PROPERTY(float, scrollSense, SetScrollSense, GetScrollSense);          // Scroll sense coefficient
		PROPERTY(float, scrollSize, SetScrollHandleSize, GetScrollHandleSize); // Scroll handle size

	public:
		Function<void(float)> onChange;       // On Value changing event
		Function<void(float)> onChangeByUser; // On Value changing from user event
		Function<void(float)> onSmoothChange; // On smooth value changing event

	public:
		// Constructor
		HorizontalScrollBar();

		// Copy-constructor
		HorizontalScrollBar(const HorizontalScrollBar& other);

		// Destructor
		~HorizontalScrollBar();

		// Copy-operator
		HorizontalScrollBar& operator=(const HorizontalScrollBar& other);

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

		// Returns create menu group in editor
		static String GetCreateMenuGroup();

		SERIALIZABLE(HorizontalScrollBar);

	protected:
		float        mValue = 0.0f;                 // Current value @SERIALIZABLE
		float        mSmoothValue = 0.0f;           // Smooth value
		float        mMinValue = 0.0f;              // Minimum value @SERIALIZABLE
		float        mMaxValue = 1.0f;              // Maximum value @SERIALIZABLE
		float        mScrollSense = 1.0f;           // Scroll sense coefficient @SERIALIZABLE
		float        mScrollHandleSize = 0.2f;      // Scroll handle size (in value range) @SERIALIZABLE
		float        mScrollhandleMinPxSize = 5.0f; // Minimal scroll size in pixels @SERIALIZABLE
		float        mPressHandleOffset = 0.0f;     // Value offset when handle was pressed
		bool         mHandlePressed = false;        // True, when handle was pressed
		WidgetLayer* mHandleLayer = nullptr;        // Handle layer
		WidgetLayer* mBackLayer = nullptr;          // Background layer

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when deserialized
		void OnDeserialized(const DataValue& node) override;

		// It is called when visible was changed
		void OnEnableInHierarchyChanged() override;

		// Updates layers layouts, calls after updating widget layout
		void UpdateLayersLayouts() override;

		// It is called when new layer was added. Here searching bar, back and handle layers
		void OnLayerAdded(WidgetLayer* layer) override;

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

		friend class ContextMenu;
		friend class CustomList;
		friend class EditBox;
		friend class LongList;
		friend class ScrollArea;
		friend class Tree;
	};
}

CLASS_BASES_META(o2::HorizontalScrollBar)
{
	BASE_CLASS(o2::Widget);
	BASE_CLASS(o2::DrawableCursorEventsListener);
}
END_META;
CLASS_FIELDS_META(o2::HorizontalScrollBar)
{
	PUBLIC_FIELD(value);
	PUBLIC_FIELD(minValue);
	PUBLIC_FIELD(maxValue);
	PUBLIC_FIELD(scrollSense);
	PUBLIC_FIELD(scrollSize);
	PUBLIC_FIELD(onChange);
	PUBLIC_FIELD(onChangeByUser);
	PUBLIC_FIELD(onSmoothChange);
	PROTECTED_FIELD(mValue).DEFAULT_VALUE(0.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSmoothValue).DEFAULT_VALUE(0.0f);
	PROTECTED_FIELD(mMinValue).DEFAULT_VALUE(0.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mMaxValue).DEFAULT_VALUE(1.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollSense).DEFAULT_VALUE(1.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollHandleSize).DEFAULT_VALUE(0.2f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollhandleMinPxSize).DEFAULT_VALUE(5.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPressHandleOffset).DEFAULT_VALUE(0.0f);
	PROTECTED_FIELD(mHandlePressed).DEFAULT_VALUE(false);
	PROTECTED_FIELD(mHandleLayer).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mBackLayer).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(o2::HorizontalScrollBar)
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
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuGroup);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, OnEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, OnLayerAdded, WidgetLayer*);
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
}
END_META;
