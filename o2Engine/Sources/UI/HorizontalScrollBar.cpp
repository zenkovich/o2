#include "stdafx.h"
#include "HorizontalScrollBar.h"

#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "Utils/System/Time/Time.h"

namespace o2
{
	UIHorizontalScrollBar::UIHorizontalScrollBar():
		UIWidget(), DrawableCursorEventsListener(this)
	{}

	UIHorizontalScrollBar::UIHorizontalScrollBar(const UIHorizontalScrollBar& other) :
		UIWidget(other), DrawableCursorEventsListener(this), mValue(other.mValue), mMinValue(other.mMinValue),
		mMaxValue(other.mMaxValue), mScrollSense(other.mScrollSense), mScrollHandleSize(other.mScrollHandleSize),
		mHandlePressed(false), mScrollhandleMinPxSize(other.mScrollhandleMinPxSize), mSmoothValue(other.mValue), 
		value(this), minValue(this), maxValue(this), scrollSense(this), scrollSize(this)
	{
		mHandleLayer = FindLayer("handle");
		mBackLayer = FindLayer("back");

		RetargetStatesAnimations();
		UpdateTransform();
	}

	UIHorizontalScrollBar::~UIHorizontalScrollBar()
	{}

	UIHorizontalScrollBar& UIHorizontalScrollBar::operator=(const UIHorizontalScrollBar& other)
	{
		UIWidget::operator=(other);
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

		float width = Math::Max(layout->GetWidth(), 1.0f);
		float minScrollhandleSize = Math::Max(mScrollhandleMinPxSize/width*range, mScrollHandleSize);
		float rangleWithHandle = range + minScrollhandleSize;
		float szCoef = minScrollhandleSize/rangleWithHandle;

		return (cursor.position.x - layout->worldLeft)/width*range/(1.0f - szCoef) + mMinValue;
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

	void UIHorizontalScrollBar::CopyData(const Actor& otherActor)
	{
		const UIHorizontalScrollBar& other = dynamic_cast<const UIHorizontalScrollBar&>(otherActor);

		UIWidget::CopyData(other);

		mValue                 = other.mValue;
		mMinValue              = other.mMinValue;
		mMaxValue              = other.mMaxValue;
		mScrollSense           = other.mScrollSense;
		mScrollHandleSize      = other.mScrollHandleSize;
		mScrollhandleMinPxSize = other.mScrollhandleMinPxSize;
		mSmoothValue           = mValue;
		mHandlePressed         = false;

		mHandleLayer = FindLayer("handle");
		mBackLayer = FindLayer("back");

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void UIHorizontalScrollBar::OnDeserialized(const DataNode& node)
	{
		UIWidget::OnDeserialized(node);

		mHandleLayer = FindLayer("handle");
		mBackLayer = FindLayer("back");

		RetargetStatesAnimations();
	}

	void UIHorizontalScrollBar::OnResEnableInHierarchyChanged()
	{
		interactable = mResEnabled;
	}

	void UIHorizontalScrollBar::UpdateLayersLayouts()
	{
		UpdateProgressLayersLayouts();
	}

	void UIHorizontalScrollBar::SetMinimalScrollHandleSize(float pixelSize)
	{
		mScrollhandleMinPxSize = pixelSize;
		UpdateProgressLayersLayouts();
	}

	void UIHorizontalScrollBar::UpdateProgressLayersLayouts()
	{
		if (mHandleLayer)
		{
			float range = mMaxValue - mMinValue;
			float width = Math::Max(layout->GetWidth(), 1.0f);
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

		UIWidget::UpdateLayersLayouts();
	}

	void UIHorizontalScrollBar::OnLayerAdded(UIWidgetLayer* layer)
	{
		if (layer->name == "back")
			mBackLayer = layer;
		else if (layer->name == "handle")
			mHandleLayer = layer;

		UpdateProgressLayersLayouts();
	}
}

DECLARE_CLASS(o2::UIHorizontalScrollBar);
