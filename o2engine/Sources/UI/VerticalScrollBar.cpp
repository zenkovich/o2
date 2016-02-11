#include "VerticalScrollBar.h"

#include "Utils/Time.h"

namespace o2
{
	UIVerticalScrollBar::UIVerticalScrollBar():
		UIWidget(), DrawableCursorEventsListener(this), mValue(0), mMinValue(0), mMaxValue(1), mScrollSense(1.0f), 
		mScrollHandleSize(0.2f), mHandlePressed(false), mScrollhandleMinPxSize(5), mSmoothValue(mValue), 
		mHandleLayer(nullptr), mBackLayer(nullptr)
	{
		InitializeProperties();
	}

	UIVerticalScrollBar::UIVerticalScrollBar(const UIVerticalScrollBar& other):
		UIWidget(other), DrawableCursorEventsListener(this), mValue(other.mValue), mMinValue(other.mMinValue), 
		mMaxValue(other.mMaxValue), mScrollSense(other.mScrollSense), mScrollHandleSize(other.mScrollHandleSize), 
		mHandlePressed(false), mScrollhandleMinPxSize(other.mScrollhandleMinPxSize), mSmoothValue(other.mValue)
	{
		mHandleLayer = GetLayer("handle");
		mBackLayer = GetLayer("back");

		RetargetStatesAnimations();
		InitializeProperties();
	}

	UIVerticalScrollBar::~UIVerticalScrollBar()
	{}

	UIVerticalScrollBar& UIVerticalScrollBar::operator=(const UIVerticalScrollBar& other)
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

	void UIVerticalScrollBar::Update(float dt)
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
		mScrollHandleSize = Math::Clamp(size, 0.0f, mMaxValue - mMinValue);
		UpdateProgressLayersLayouts();
	}

	float UIVerticalScrollBar::GetScrollhandleSize() const
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

	float UIVerticalScrollBar::GetValueFromCursor(const Input::Cursor &cursor)
	{
		float range = mMaxValue - mMinValue;
		float szRange = range - mScrollHandleSize*(range/(range + mScrollHandleSize));
		float height = layout.mAbsoluteRect.Height();

		return (height - (cursor.mPosition.y - layout.mAbsoluteRect.bottom))/height*range/szRange*range + mMinValue;
	}

	void UIVerticalScrollBar::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = true;
	}

	void UIVerticalScrollBar::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = false;
	}

	void UIVerticalScrollBar::OnScrolled(float scroll)
	{
		SetValue(mValue - scroll*mScrollSense);
	}

	void UIVerticalScrollBar::OnDeserialized(const DataNode& node)
	{
		mHandleLayer = GetLayer("handle");
		mBackLayer = GetLayer("back");

		UIWidget::OnDeserialized(node);
	}

	void UIVerticalScrollBar::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	void UIVerticalScrollBar::SetMinimalScrollhandleSize(float pixelSize)
	{
		mScrollhandleMinPxSize = pixelSize;
		UpdateProgressLayersLayouts();
	}

	void UIVerticalScrollBar::UpdateLayout(bool forcible /*= false*/)
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

	void UIVerticalScrollBar::UpdateProgressLayersLayouts()
	{
		if (mHandleLayer)
		{
			float range = mMaxValue - mMinValue;
			float height = Math::Max(layout.mAbsoluteRect.Height(), 1.0f);
			float minScrollhandleSize = Math::Max(mScrollhandleMinPxSize/height*range, mScrollHandleSize);
			float szCoef = minScrollhandleSize/(range + minScrollhandleSize);
			float szRange = range - minScrollhandleSize*(range/(range + minScrollhandleSize));
			float szPos = (mSmoothValue - mMinValue)/range*(szRange/range);

			mHandleLayer->layout.offsetMin = Vec2F();
			mHandleLayer->layout.offsetMax = Vec2F();
			mHandleLayer->layout.anchorMin = Vec2F(0, 1.0f - szPos);
			mHandleLayer->layout.anchorMax = Vec2F(1, 1.0f - szPos - szCoef);
		}

		if (mBackLayer)
			mBackLayer->layout = Layout::BothStretch();

		UpdateLayersLayouts();
	}

	void UIVerticalScrollBar::OnLayoutUpdated()
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

	void UIVerticalScrollBar::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIVerticalScrollBar, value, SetValue, GetValue);
		INITIALIZE_PROPERTY(UIVerticalScrollBar, minValue, SetMinValue, GetMinValue);
		INITIALIZE_PROPERTY(UIVerticalScrollBar, maxValue, SetMaxValue, GetMaxValue);
		INITIALIZE_PROPERTY(UIVerticalScrollBar, scrollSense, SetScrollSense, GetScrollSense);
	}

	float UIVerticalScrollBar::GetSmoothValue() const
	{
		return mSmoothValue;
	}

}