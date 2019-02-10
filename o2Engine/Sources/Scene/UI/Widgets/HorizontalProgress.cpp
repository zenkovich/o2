#include "stdafx.h"
#include "Scene/UI/Widgets/HorizontalProgress.h"

#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/WidgetState.h"

namespace o2
{
	HorizontalProgress::HorizontalProgress():
		Widget(), DrawableCursorEventsListener(this)
	{}

	HorizontalProgress::HorizontalProgress(const HorizontalProgress& other) :
		Widget(other), DrawableCursorEventsListener(this), mValue(other.mValue), mMinValue(other.mMinValue),
		mMaxValue(other.mMaxValue), mOrientation(other.mOrientation), mScrollSense(other.mScrollSense), value(this),
		minValue(this), maxValue(this), scrollSense(this)
	{
		mBarLayer = FindLayer("bar");
		mBackLayer = FindLayer("back");

		RetargetStatesAnimations();
	}

	HorizontalProgress::~HorizontalProgress()
	{}

	HorizontalProgress& HorizontalProgress::operator=(const HorizontalProgress& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void HorizontalProgress::Update(float dt)
	{
		Widget::Update(dt);

		if (!mResEnabledInHierarchy || mIsClipped)
			return;

		const float threshold = 0.01f;
		const float smoothCoef = 30.0f;

		if (!Math::Equals(mValue, mSmoothValue, threshold))
		{
			mSmoothValue = Math::Clamp(Math::Lerpc(mSmoothValue, mValue, dt*smoothCoef), mMinValue, mMaxValue);

			if (Math::Abs(mValue - mSmoothValue) < threshold)
				mSmoothValue = mValue;

			UpdateProgressLayersLayouts();
		}
	}

	void HorizontalProgress::SetValue(float value)
	{
		mValue = Math::Clamp(value, mMinValue, mMaxValue);
		UpdateProgressLayersLayouts();

		onChange(mValue);
	}

	void HorizontalProgress::SetValueForcible(float value)
	{
		mValue = Math::Clamp(value, mMinValue, mMaxValue);
		mSmoothValue = mValue;
		UpdateProgressLayersLayouts();

		onChange(mValue);
	}

	float HorizontalProgress::GetValue() const
	{
		return mValue;
	}

	void HorizontalProgress::SetMinValue(float minValue)
	{
		mMinValue = minValue;
		mValue = Math::Max(mMinValue, mValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float HorizontalProgress::GetMinValue() const
	{
		return mMinValue;
	}

	void HorizontalProgress::SetMaxValue(float maxValue)
	{
		mMaxValue = maxValue;
		mValue = Math::Min(mMaxValue, mValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float HorizontalProgress::GetMaxValue() const
	{
		return mMaxValue;
	}

	void HorizontalProgress::SetValueRange(float minValue, float maxValue)
	{
		mMaxValue = maxValue;
		mValue = Math::Clamp(mValue, mMinValue, mMaxValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	void HorizontalProgress::SetScrollSense(float coef)
	{
		mScrollSense = coef;
	}

	float HorizontalProgress::GetScrollSense() const
	{
		return mScrollSense;
	}

	void HorizontalProgress::SetOrientation(Orientation orientation)
	{
		mOrientation = orientation;
		UpdateLayersLayouts();
	}

	HorizontalProgress::Orientation HorizontalProgress::GetOrientation() const
	{
		return mOrientation;
	}

	bool HorizontalProgress::IsUnderPoint(const Vec2F& point)
	{
		if (mBackLayer)
			return mDrawingScissorRect.IsInside(point) && mBackLayer->IsUnderPoint(point);

		return false;
	}

	bool HorizontalProgress::IsScrollable() const
	{
		return true;
	}

	void HorizontalProgress::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;

		GetValueFromCursor(cursor);
	}

	void HorizontalProgress::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void HorizontalProgress::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void HorizontalProgress::OnCursorStillDown(const Input::Cursor& cursor)
	{
		GetValueFromCursor(cursor);
	}

	void HorizontalProgress::GetValueFromCursor(const Input::Cursor &cursor)
	{
		float width = layout->width;
		float d = mMaxValue - mMinValue;
		if (mOrientation == Orientation::Right)
			SetValue((cursor.position.x - layout->worldLeft)/width*d + mMinValue);
		else
			SetValue((width - (cursor.position.x - layout->worldLeft))/width*d + mMinValue);
	}

	void HorizontalProgress::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;
	}

	void HorizontalProgress::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = false;
	}

	void HorizontalProgress::OnScrolled(float scroll)
	{
		SetValue(mValue + scroll*mScrollSense);
	}

	void HorizontalProgress::OnDeserialized(const DataNode& node)
	{
		Widget::OnDeserialized(node);

		mBarLayer = FindLayer("bar");
		mBackLayer = FindLayer("back");

		RetargetStatesAnimations();
	}

	void HorizontalProgress::OnResEnableInHierarchyChanged()
	{
		interactable = mResEnabled;
	}

	void HorizontalProgress::UpdateLayersLayouts()
	{
		UpdateProgressLayersLayouts();
	}

	void HorizontalProgress::UpdateProgressLayersLayouts()
	{
		if (mBarLayer)
		{
			mBarLayer->layout.offsetMin = Vec2F();
			mBarLayer->layout.offsetMax = Vec2F();

			if (mOrientation == Orientation::Right)
			{
				mBarLayer->layout.anchorMin = Vec2F(0, 0);
				mBarLayer->layout.anchorMax = Vec2F((mSmoothValue - mMinValue)/(mMaxValue - mMinValue), 1);
			}
			else if (mOrientation == Orientation::Left)
			{
				mBarLayer->layout.anchorMin = Vec2F(1.0f - (mSmoothValue - mMinValue)/(mMaxValue - mMinValue), 0.0f);
				mBarLayer->layout.anchorMax = Vec2F(1, 1);
			}
		}

		if (mBackLayer)
			mBackLayer->layout = Layout::BothStretch();

		Widget::UpdateLayersLayouts();
	}

	void HorizontalProgress::CopyData(const Actor& otherActor)
	{
		const HorizontalProgress& other = dynamic_cast<const HorizontalProgress&>(otherActor);

		Widget::CopyData(other);

		mValue       = other.mValue;
		mMinValue    = other.mMinValue;
		mMaxValue    = other.mMaxValue;
		mOrientation = other.mOrientation;
		mScrollSense = other.mScrollSense;

		mBarLayer    = FindLayer("bar");
		mBackLayer   = FindLayer("back");

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	void HorizontalProgress::OnLayerAdded(WidgetLayer* layer)
	{
		if (layer->name == "back")
			mBackLayer = layer;
		else if (layer->name == "bar")
			mBarLayer = layer;

		UpdateProgressLayersLayouts();
	}
}

DECLARE_CLASS(o2::HorizontalProgress);

ENUM_META_(o2::HorizontalProgress::Orientation, Orientation)
{
	ENUM_ENTRY(Left);
	ENUM_ENTRY(Right);
}
END_ENUM_META;
