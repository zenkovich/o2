#include "HorizontalScrollBar.h"

#include "Utils/Time.h"

namespace o2
{
	UIHorizontalScrollBar::UIHorizontalScrollBar():
		UIWidget(), DrawableCursorEventsListener(this)
	{
		InitializeProperties();
	}

	UIHorizontalScrollBar::UIHorizontalScrollBar(const UIHorizontalScrollBar& other):
		UIWidget(other), DrawableCursorEventsListener(this), mValue(other.mValue), mMinValue(other.mMinValue), 
		mMaxValue(other.mMaxValue), mScrollSense(other.mScrollSense), mScrollHandleSize(other.mScrollHandleSize), 
		mHandlePressed(false), mScrollhandleMinPxSize(other.mScrollhandleMinPxSize), mSmoothValue(other.mValue)
	{
		mHandleLayer = GetLayer("handle");
		mBackLayer = GetLayer("back");

		RetargetStatesAnimations();
		InitializeProperties();
		UpdateLayout();
	}

	UIHorizontalScrollBar::~UIHorizontalScrollBar()
	{}

	UIHorizontalScrollBar& UIHorizontalScrollBar::operator=(const UIHorizontalScrollBar& other)
	{
		UIWidget::operator=(other);

		mValue                 = other.mValue;
		mMinValue              = other.mMinValue;
		mMaxValue              = other.mMaxValue;
		mScrollSense           = other.mScrollSense;
		mScrollHandleSize      = other.mScrollHandleSize;
		mScrollhandleMinPxSize = other.mScrollhandleMinPxSize;
		mSmoothValue           = mValue;
		mHandlePressed         = false;

		mHandleLayer = GetLayer("handle");
		mBackLayer = GetLayer("back");

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIHorizontalScrollBar::Update(float dt)
	{
		UIWidget::Update(dt);

		const float threshold = 0.01f;
		const float smoothCoef = 30.0f;

		if (!Math::Equals(mValue, mSmoothValue, threshold))
		{
			mSmoothValue = Math::Clamp(Math::Lerpc(mSmoothValue, mValue, dt*smoothCoef), mMinValue, mMaxValue);

			if (Math::Abs(mValue - mSmoothValue) < threshold)
				mSmoothValue = mValue;

			UpdateProgressLayersLayouts();

			onSmoothChange(mSmoothValue);
		}
	}

	void UIHorizontalScrollBar::SetValue(float value)
	{
		mValue = Math::Clamp(value, mMinValue, mMaxValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	void UIHorizontalScrollBar::SetValueForcible(float value)
	{
		mValue = Math::Clamp(value, mMinValue, mMaxValue);
		mSmoothValue = mValue;
		UpdateProgressLayersLayouts();

		onChange(value);
		onSmoothChange(mSmoothValue);
	}

	float UIHorizontalScrollBar::GetValue() const
	{
		return mValue;
	}

	float UIHorizontalScrollBar::GetSmoothValue() const
	{
		return mSmoothValue;
	}

	void UIHorizontalScrollBar::SetMinValue(float minValue)
	{
		mMinValue = minValue;
		mValue = Math::Max(mMinValue, mValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float UIHorizontalScrollBar::GetMinValue() const
	{
		return mMinValue;
	}

	void UIHorizontalScrollBar::SetMaxValue(float maxValue)
	{
		mMaxValue = maxValue;
		mValue = Math::Min(mMaxValue, mValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float UIHorizontalScrollBar::GetMaxValue() const
	{
		return mMaxValue;
	}

	void UIHorizontalScrollBar::SetValueRange(float minValue, float maxValue)
	{
		mMinValue = Math::Min(maxValue, minValue);
		mMaxValue = Math::Max(maxValue, minValue);
		mValue = Math::Clamp(mValue, mMinValue, mMaxValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	void UIHorizontalScrollBar::SetScrollSense(float coef)
	{
		mScrollSense = coef;
	}

	float UIHorizontalScrollBar::GetScrollSense() const
	{
		return mScrollSense;
	}

	void UIHorizontalScrollBar::SetScrollHandleSize(float size)
	{
		mScrollHandleSize = size;
		UpdateProgressLayersLayouts();
	}

	float UIHorizontalScrollBar::GetScrollHandleSize() const
	{
		return mScrollHandleSize;
	}

	bool UIHorizontalScrollBar::IsUnderPoint(const Vec2F& point)
	{
		if (!mDrawingScissorRect.IsInside(point))
			return false;

		if (mHandleLayer && mHandleLayer->IsUnderPoint(point))
			return true;

		if (mBackLayer && mBackLayer->IsUnderPoint(point))
			return true;

		return false;
	}

	bool UIHorizontalScrollBar::IsScrollable() const
	{
		return !Math::Equals(mMinValue, mMaxValue);
	}

	void UIHorizontalScrollBar::OnCursorPressed(const Input::Cursor& cursor)
	{

		if (mHandleLayer && mHandleLayer->IsUnderPoint(cursor.position))
		{
			mHandlePressed = true;
			float pressedValue = GetValueFromCursor(cursor);
			mPressHandleOffset = mValue - pressedValue;

			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = true;
		}
		else
		{
			float pressedValue = GetValueFromCursor(cursor);
			if (pressedValue > mValue + mScrollHandleSize*0.5f)
				SetValueFromUser(mValue + mScrollHandleSize);
			else
				SetValueFromUser(mValue - mScrollHandleSize);
		}
	}

	void UIHorizontalScrollBar::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		mHandlePressed = false;
	}

	void UIHorizontalScrollBar::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		mHandlePressed = false;
	}

	void UIHorizontalScrollBar::OnCursorStillDown(const Input::Cursor& cursor)
	{
		if (mHandlePressed)
			SetValueFromUser(GetValueFromCursor(cursor) + mPressHandleOffset);
		else
		{
			const float timeThreshold = 0.5f;
			const float speed = 5.0f;

			if (cursor.pressedTime > timeThreshold && !(mHandleLayer && mHandleLayer->IsUnderPoint(cursor.position)))
			{
				float pressedValue = GetValueFromCursor(cursor);
				if (pressedValue > mValue + mScrollHandleSize*0.5f)
					SetValueFromUser(mValue + mScrollHandleSize*o2Time.GetDeltaTime()*speed);
				else
					SetValueFromUser(mValue - mScrollHandleSize*o2Time.GetDeltaTime()*speed);
			}
		}
	}

	float UIHorizontalScrollBar::GetValueFromCursor(const Input::Cursor &cursor)
	{
		float range = mMaxValue - mMinValue;

		if (Math::Equals(range, 0.0f))
			return mValue;

		float width = Math::Max(layout.mAbsoluteRect.Width(), 1.0f);
		float minScrollhandleSize = Math::Max(mScrollhandleMinPxSize/width*range, mScrollHandleSize);
		float rangleWithHandle = range + minScrollhandleSize;
		float szCoef = minScrollhandleSize/rangleWithHandle;

		return (cursor.position.x - layout.mAbsoluteRect.left)/width*range/(1.0f - szCoef) + mMinValue;
	}

	void UIHorizontalScrollBar::SetValueFromUser(float value)
	{
		SetValue(value);
		onUserChange(mValue);
	}

	void UIHorizontalScrollBar::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;
	}

	void UIHorizontalScrollBar::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = false;
	}

	void UIHorizontalScrollBar::OnScrolled(float scroll)
	{
		SetValue(mValue - scroll*mScrollSense);
	}

	void UIHorizontalScrollBar::OnDeserialized(const DataNode& node)
	{
		mHandleLayer = GetLayer("handle");
		mBackLayer = GetLayer("back");

		UIWidget::OnDeserialized(node);
	}

	void UIHorizontalScrollBar::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	void UIHorizontalScrollBar::SetMinimalScrollHandleSize(float pixelSize)
	{
		mScrollhandleMinPxSize = pixelSize;
		UpdateProgressLayersLayouts();
	}

	void UIHorizontalScrollBar::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (CheckIsLayoutDrivenByParent(forcible))
			return;

		RecalculateAbsRect();
		UpdateProgressLayersLayouts();
		UpdateLayersLayouts();

		if (withChildren)
			UpdateChildrenLayouts();
	}

	void UIHorizontalScrollBar::UpdateProgressLayersLayouts()
	{
		if (mHandleLayer)
		{
			float range = mMaxValue - mMinValue;
			float width = Math::Max(layout.mAbsoluteRect.Width(), 1.0f);
			float minScrollhandleSize = Math::Max(mScrollhandleMinPxSize/width*range, mScrollHandleSize);
			float rangeWithHandle = range + minScrollhandleSize;
			float szCoef = minScrollhandleSize/rangeWithHandle;
			float szPos = (mSmoothValue - mMinValue)/range*(1.0f - szCoef);

			if (Math::Equals(range, 0.0f))
			{
				szPos = 0;
				szCoef = 1;
			}

			mHandleLayer->layout.offsetMin = Vec2F();
			mHandleLayer->layout.offsetMax = Vec2F();
			mHandleLayer->layout.anchorMin = Vec2F(Math::Clamp01(szPos), 0);
			mHandleLayer->layout.anchorMax = Vec2F(Math::Clamp01(szPos + szCoef), 1);
		}

		if (mBackLayer)
			mBackLayer->layout = Layout::BothStretch();

		UpdateLayersLayouts();
	}

	void UIHorizontalScrollBar::OnLayoutUpdated()
	{
		UpdateProgressLayersLayouts();
	}

	void UIHorizontalScrollBar::OnLayerAdded(UIWidgetLayer* layer)
	{
		if (layer->name == "back")
			mBackLayer = layer;
		else if (layer->name == "handle")
			mHandleLayer = layer;

		UpdateProgressLayersLayouts();
	}

	void UIHorizontalScrollBar::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIHorizontalScrollBar, value, SetValue, GetValue);
		INITIALIZE_PROPERTY(UIHorizontalScrollBar, minValue, SetMinValue, GetMinValue);
		INITIALIZE_PROPERTY(UIHorizontalScrollBar, maxValue, SetMaxValue, GetMaxValue);
		INITIALIZE_PROPERTY(UIHorizontalScrollBar, scrollSense, SetScrollSense, GetScrollSense);
	}
}

CLASS_META(o2::UIHorizontalScrollBar)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::DrawableCursorEventsListener);

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
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
	PROTECTED_FUNCTION(void, UpdateProgressLayersLayouts);
	PROTECTED_FUNCTION(void, OnLayoutUpdated);
	PROTECTED_FUNCTION(void, OnLayerAdded, UIWidgetLayer*);
	PROTECTED_FUNCTION(float, GetValueFromCursor, const Input::Cursor&);
	PROTECTED_FUNCTION(void, SetValueFromUser, float);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressBreak, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorStillDown, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnScrolled, float);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, OnVisibleChanged);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
