#include "stdafx.h"
#include "VerticalScrollBar.h"

#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "Utils/Time.h"

namespace o2
{
	UIVerticalScrollBar::UIVerticalScrollBar():
		UIWidget(), DrawableCursorEventsListener(this)
	{}

	UIVerticalScrollBar::UIVerticalScrollBar(const UIVerticalScrollBar& other) :
		UIWidget(other), DrawableCursorEventsListener(this), mValue(other.mValue), mMinValue(other.mMinValue),
		mMaxValue(other.mMaxValue), mScrollSense(other.mScrollSense), mScrollHandleSize(other.mScrollHandleSize),
		mHandlePressed(false), mScrollhandleMinPxSize(other.mScrollhandleMinPxSize), mSmoothValue(other.mValue)
	{
		mHandleLayer = FindLayer("handle");
		mBackLayer = FindLayer("back");

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	UIVerticalScrollBar::~UIVerticalScrollBar()
	{}

	UIVerticalScrollBar& UIVerticalScrollBar::operator=(const UIVerticalScrollBar& other)
	{
		CopyData(other);
		return *this;
	}

	void UIVerticalScrollBar::Update(float dt)
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

	void UIVerticalScrollBar::SetValue(float value)
	{
		mValue = Math::Clamp(value, mMinValue, mMaxValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	void UIVerticalScrollBar::SetValueForcible(float value)
	{
		mValue = Math::Clamp(value, mMinValue, mMaxValue);
		mSmoothValue = mValue;
		UpdateProgressLayersLayouts();

		onChange(value);
		onSmoothChange(mSmoothValue);
	}

	float UIVerticalScrollBar::GetValue() const
	{
		return mValue;
	}

	void UIVerticalScrollBar::SetMinValue(float minValue)
	{
		mMinValue = minValue;
		mValue = Math::Max(mMinValue, mValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float UIVerticalScrollBar::GetMinValue() const
	{
		return mMinValue;
	}

	void UIVerticalScrollBar::SetMaxValue(float maxValue)
	{
		mMaxValue = maxValue;
		mValue = Math::Min(mMaxValue, mValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float UIVerticalScrollBar::GetMaxValue() const
	{
		return mMaxValue;
	}

	void UIVerticalScrollBar::SetValueRange(float minValue, float maxValue)
	{
		mMinValue = Math::Min(maxValue, minValue);
		mMaxValue = Math::Max(maxValue, minValue);
		mValue = Math::Clamp(mValue, mMinValue, mMaxValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	void UIVerticalScrollBar::SetScrollSense(float coef)
	{
		mScrollSense = coef;
	}

	float UIVerticalScrollBar::GetScrollSense() const
	{
		return mScrollSense;
	}

	void UIVerticalScrollBar::SetScrollHandleSize(float size)
	{
		mScrollHandleSize = size;
		UpdateProgressLayersLayouts();
	}

	float UIVerticalScrollBar::GetScrollHandleSize() const
	{
		return mScrollHandleSize;
	}

	bool UIVerticalScrollBar::IsUnderPoint(const Vec2F& point)
	{
		if (!mDrawingScissorRect.IsInside(point))
			return false;

		if (mHandleLayer && mHandleLayer->IsUnderPoint(point))
			return true;

		if (mBackLayer && mBackLayer->IsUnderPoint(point))
			return true;

		return false;
	}

	bool UIVerticalScrollBar::IsScrollable() const
	{
		return !Math::Equals(mMinValue, mMaxValue);
	}

	void UIVerticalScrollBar::OnCursorPressed(const Input::Cursor& cursor)
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

	void UIVerticalScrollBar::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		mHandlePressed = false;
	}

	void UIVerticalScrollBar::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		mHandlePressed = false;
	}

	void UIVerticalScrollBar::OnCursorStillDown(const Input::Cursor& cursor)
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

	float UIVerticalScrollBar::GetValueFromCursor(const Input::Cursor &cursor)
	{
		float range = mMaxValue - mMinValue;

		if (Math::Equals(range, 0.0f))
			return mValue;

		float height = layout->worldRect->Height();
		float minScrollhandleSize = Math::Max(mScrollhandleMinPxSize/height*range, mScrollHandleSize);
		float rangleWithHandle = range + minScrollhandleSize;
		float szCoef = minScrollhandleSize/rangleWithHandle;

		return (height - (cursor.position.y - layout->worldRect->bottom))/height*range/(1.0f - szCoef) + mMinValue;
	}

	void UIVerticalScrollBar::SetValueFromUser(float value)
	{
		SetValue(value);
		onUserChange(mValue);
	}

	void UIVerticalScrollBar::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;
	}

	void UIVerticalScrollBar::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = false;
	}

	void UIVerticalScrollBar::OnScrolled(float scroll)
	{
		SetValue(mValue - scroll*mScrollSense);
	}

	void UIVerticalScrollBar::OnDeserialized(const DataNode& node)
	{
		UIWidget::OnDeserialized(node);

		mHandleLayer = FindLayer("handle");
		mBackLayer = FindLayer("back");

		RetargetStatesAnimations();
	}

	void UIVerticalScrollBar::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	void UIVerticalScrollBar::SetMinimalScrollHandleSize(float pixelSize)
	{
		mScrollhandleMinPxSize = pixelSize;
		UpdateProgressLayersLayouts();
	}

	void UIVerticalScrollBar::UpdateTransform(bool withChildren /*= true*/)
	{
		layout->Update();

		if (withChildren)
			UpdateChildrenTransforms();
	}

	void UIVerticalScrollBar::UpdateProgressLayersLayouts()
	{
		if (mHandleLayer)
		{
			float range = mMaxValue - mMinValue;
			float height = Math::Max(layout->worldRect->Height(), 1.0f);
			float minScrollhandleSize = Math::Max(mScrollhandleMinPxSize/height*range, mScrollHandleSize);
			float rangleWithHandle = range + minScrollhandleSize;
			float szCoef = minScrollhandleSize/rangleWithHandle;
			float szPos = (mSmoothValue - mMinValue)/range*(1.0f - szCoef);

			if (Math::Equals(range, 0.0f))
			{
				szPos = 0;
				szCoef = 1;
			}

			mHandleLayer->layout.offsetMin = Vec2F();
			mHandleLayer->layout.offsetMax = Vec2F();
			mHandleLayer->layout.anchorMin = Vec2F(0, Math::Clamp01(1.0f - szPos));
			mHandleLayer->layout.anchorMax = Vec2F(1, Math::Clamp01(1.0f - szPos - szCoef));
		}

		if (mBackLayer)
			mBackLayer->layout = Layout::BothStretch();

		UIWidget::UpdateLayersLayouts();
	}

	void UIVerticalScrollBar::CopyData(const Actor& otherActor)
	{
		const UIVerticalScrollBar& other = dynamic_cast<const UIVerticalScrollBar&>(otherActor);

		UIWidget::CopyData(other);

		mValue                 = other.mValue;
		mMinValue              = other.mMinValue;
		mMaxValue              = other.mMaxValue;
		mScrollSense           = other.mScrollSense;
		mScrollHandleSize      = other.mScrollHandleSize;
		mScrollhandleMinPxSize = other.mScrollhandleMinPxSize;
		mSmoothValue           = mValue;
		mHandlePressed         = false;
		mHandleLayer           = FindLayer("handle");
		mBackLayer             = FindLayer("back");

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void UIVerticalScrollBar::UpdateLayersLayouts()
	{
		UpdateProgressLayersLayouts();
	}

	void UIVerticalScrollBar::OnLayerAdded(UIWidgetLayer* layer)
	{
		if (layer->name == "back")
			mBackLayer = layer;
		else if (layer->name == "handle")
			mHandleLayer = layer;

		UpdateProgressLayersLayouts();
	}

	float UIVerticalScrollBar::GetSmoothValue() const
	{
		return mSmoothValue;
	}
}

DECLARE_CLASS(o2::UIVerticalScrollBar);
