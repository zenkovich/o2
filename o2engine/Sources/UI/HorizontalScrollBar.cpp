#include "HorizontalScrollBar.h"

#include "Utils/Time.h"

namespace o2
{
	UIHorizontalScrollBar::UIHorizontalScrollBar():
		mValue(0), mMinValue(0), mMaxValue(1), mScrollSense(1.0f), mScrollHandleSize(0.2f), mHandlePressed(false),
		mScrollhandleMinPxSize(5), mSmoothValue(mValue)
	{
		InitializeProperties();
	}

	UIHorizontalScrollBar::UIHorizontalScrollBar(const UIHorizontalScrollBar& other):
		UIWidget(other), mValue(other.mValue), mMinValue(other.mMinValue), mMaxValue(other.mMaxValue),
		mScrollSense(other.mScrollSense), mScrollHandleSize(other.mScrollHandleSize), mHandlePressed(false),
		mScrollhandleMinPxSize(other.mScrollhandleMinPxSize), mSmoothValue(other.mValue)
	{
		mHandleLayer = GetLayer("handle");
		mBackLayer = GetLayer("back");

		RetargetStatesAnimations();
		InitializeProperties();
	}

	UIHorizontalScrollBar::~UIHorizontalScrollBar()
	{}

	UIHorizontalScrollBar& UIHorizontalScrollBar::operator=(const UIHorizontalScrollBar& other)
	{
		UIWidget::operator=(other);
		mValue = other.mValue;
		mMinValue = other.mMinValue;
		mMaxValue = other.mMaxValue;
		mScrollSense = other.mScrollSense;
		mScrollHandleSize = other.mScrollHandleSize;
		mScrollhandleMinPxSize = other.mScrollhandleMinPxSize;
		mSmoothValue = mValue;
		mHandlePressed = false;

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
			mSmoothValue = Math::Clamp(Math::Lerp(mSmoothValue, mValue, dt*smoothCoef), mMinValue, mMaxValue);

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
		mScrollHandleSize = Math::Clamp(size, 0.0f, mMaxValue - mMinValue);
		UpdateProgressLayersLayouts();
	}

	float UIHorizontalScrollBar::GetScrollhandleSize() const
	{
		return mScrollHandleSize;
	}

	bool UIHorizontalScrollBar::IsUnderPoint(const Vec2F& point)
	{
		if (mHandleLayer && mHandleLayer->IsUnderPoint(point))
			return true;

		if (mBackLayer && mBackLayer->IsUnderPoint(point))
			return true;

		return false;
	}

	float UIHorizontalScrollBar::Depth()
	{
		return GetMaxDrawingDepth();
	}

	bool UIHorizontalScrollBar::IsScrollable() const
	{
		return !Math::Equals(mMinValue, mMaxValue);
	}

	void UIHorizontalScrollBar::OnCursorPressed(const Input::Cursor& cursor)
	{

		if (mHandleLayer && mHandleLayer->IsUnderPoint(cursor.mPosition))
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
				SetValue(mValue + mScrollHandleSize);
			else
				SetValue(mValue - mScrollHandleSize);
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
			SetValue(GetValueFromCursor(cursor) + mPressHandleOffset);
		else
		{
			const float timeThreshold = 0.5f;
			const float speed = 5.0f;

			if (cursor.mPressedTime > timeThreshold && !(mHandleLayer && mHandleLayer->IsUnderPoint(cursor.mPosition)))
			{
				float pressedValue = GetValueFromCursor(cursor);
				if (pressedValue > mValue + mScrollHandleSize*0.5f)
					SetValue(mValue + mScrollHandleSize*o2Time.GetDeltaTime()*speed);
				else
					SetValue(mValue - mScrollHandleSize*o2Time.GetDeltaTime()*speed);
			}
		}
	}

	float UIHorizontalScrollBar::GetValueFromCursor(const Input::Cursor &cursor)
	{
		float range = mMaxValue - mMinValue;
		float szRange = range - mScrollHandleSize*(range/(range + mScrollHandleSize));
		float width = layout.mAbsoluteRect.Width();

		return (cursor.mPosition.x - layout.mAbsoluteRect.left)/width*range/szRange*range + mMinValue;
	}

	void UIHorizontalScrollBar::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = true;
	}

	void UIHorizontalScrollBar::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
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

	void UIHorizontalScrollBar::SetMinimalScrollhandleSize(float pixelSize)
	{
		mScrollhandleMinPxSize = pixelSize;
		UpdateProgressLayersLayouts();
	}

	void UIHorizontalScrollBar::UpdateLayout(bool forcible /*= false*/)
	{
		if (layout.mDrivenByParent && !forcible)
		{
			if (mParent)
				mParent->UpdateLayout();

			return;
		}

		RecalculateAbsRect();
		UpdateProgressLayersLayouts();
		UpdateLayersLayouts();

		mChildsAbsRect = layout.mAbsoluteRect;

		for (auto child : mChilds)
			child->UpdateLayout();
	}

	void UIHorizontalScrollBar::UpdateProgressLayersLayouts()
	{
		if (mHandleLayer)
		{
			float range = mMaxValue - mMinValue;
			float width = Math::Max(layout.mAbsoluteRect.Width(), 1.0f);
			float minScrollhandleSize = Math::Max(mScrollhandleMinPxSize/width*range, mScrollHandleSize);
			float szCoef = minScrollhandleSize/(range + minScrollhandleSize);
			float szRange = range - minScrollhandleSize*(range/(range + minScrollhandleSize));
			float szPos = (mSmoothValue - mMinValue)/range*(szRange/range);

			mHandleLayer->layout.offsetMin = Vec2F();
			mHandleLayer->layout.offsetMax = Vec2F();
			mHandleLayer->layout.anchorMin = Vec2F(szPos, 0);
			mHandleLayer->layout.anchorMax = Vec2F(szPos + szCoef, 1);
		}

		if (mBackLayer)
			mBackLayer->layout = Layout::BothStretch();

		UpdateLayersLayouts();
	}

	void UIHorizontalScrollBar::OnLayoutUpdated()
	{
		UpdateProgressLayersLayouts();
	}

	void UIHorizontalScrollBar::OnLayerAdded(Ptr<UIWidgetLayer> layer)
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