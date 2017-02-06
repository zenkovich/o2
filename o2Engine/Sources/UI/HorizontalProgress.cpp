#include "UI/HorizontalProgress.h"

namespace o2
{
	UIHorizontalProgress::UIHorizontalProgress():
		UIWidget(), DrawableCursorEventsListener(this)
	{
		InitializeProperties();
	}

	UIHorizontalProgress::UIHorizontalProgress(const UIHorizontalProgress& other):
		UIWidget(other), DrawableCursorEventsListener(this), mValue(other.mValue), mMinValue(other.mMinValue), 
		mMaxValue(other.mMaxValue), mOrientation(other.mOrientation), mScrollSense(other.mScrollSense)
	{
		mBarLayer = GetLayer("bar");
		mBackLayer = GetLayer("back");

		RetargetStatesAnimations();

		InitializeProperties();
	}

	UIHorizontalProgress::~UIHorizontalProgress()
	{}

	UIHorizontalProgress& UIHorizontalProgress::operator=(const UIHorizontalProgress& other)
	{
		UIWidget::operator=(other);

		mValue       = other.mValue;
		mMinValue    = other.mMinValue;
		mMaxValue    = other.mMaxValue;
		mOrientation = other.mOrientation;
		mScrollSense = other.mScrollSense;

		mBarLayer    = GetLayer("bar");
		mBackLayer   = GetLayer("back");

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIHorizontalProgress::Update(float dt)
	{
		if (mFullyDisabled || mIsClipped)
			return;

		UIWidget::Update(dt);

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

	void UIHorizontalProgress::SetValue(float value)
	{
		mValue = Math::Clamp(value, mMinValue, mMaxValue);
		UpdateProgressLayersLayouts();

		onChange(mValue);
	}

	void UIHorizontalProgress::SetValueForcible(float value)
	{
		mValue = Math::Clamp(value, mMinValue, mMaxValue);
		mSmoothValue = mValue;
		UpdateProgressLayersLayouts();

		onChange(mValue);
	}

	float UIHorizontalProgress::GetValue() const
	{
		return mValue;
	}

	void UIHorizontalProgress::SetMinValue(float minValue)
	{
		mMinValue = minValue;
		mValue = Math::Max(mMinValue, mValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float UIHorizontalProgress::GetMinValue() const
	{
		return mMinValue;
	}

	void UIHorizontalProgress::SetMaxValue(float maxValue)
	{
		mMaxValue = maxValue;
		mValue = Math::Min(mMaxValue, mValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float UIHorizontalProgress::GetMaxValue() const
	{
		return mMaxValue;
	}

	void UIHorizontalProgress::SetValueRange(float minValue, float maxValue)
	{
		mMaxValue = maxValue;
		mValue = Math::Clamp(mValue, mMinValue, mMaxValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	void UIHorizontalProgress::SetScrollSense(float coef)
	{
		mScrollSense = coef;
	}

	float UIHorizontalProgress::GetScrollSense() const
	{
		return mScrollSense;
	}

	void UIHorizontalProgress::SetOrientation(Orientation orientation)
	{
		mOrientation = orientation;
		UpdateLayersLayouts();
	}

	UIHorizontalProgress::Orientation UIHorizontalProgress::GetOrientation() const
	{
		return mOrientation;
	}

	bool UIHorizontalProgress::IsUnderPoint(const Vec2F& point)
	{
		if (mBackLayer)
			return mDrawingScissorRect.IsInside(point) && mBackLayer->IsUnderPoint(point);

		return false;
	}

	bool UIHorizontalProgress::IsScrollable() const
	{
		return true;
	}

	void UIHorizontalProgress::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;

		GetValueFromCursor(cursor);
	}

	void UIHorizontalProgress::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void UIHorizontalProgress::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void UIHorizontalProgress::OnCursorStillDown(const Input::Cursor& cursor)
	{
		GetValueFromCursor(cursor);
	}

	void UIHorizontalProgress::GetValueFromCursor(const Input::Cursor &cursor)
	{
		float width = layout.mAbsoluteRect.Width();
		float d = mMaxValue - mMinValue;
		if (mOrientation == Orientation::Right)
			SetValue((cursor.position.x - layout.mAbsoluteRect.left)/width*d + mMinValue);
		else
			SetValue((width - (cursor.position.x - layout.mAbsoluteRect.left))/width*d + mMinValue);
	}

	void UIHorizontalProgress::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = true;
	}

	void UIHorizontalProgress::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["hover"];
		if (selectState)
			*selectState = false;
	}

	void UIHorizontalProgress::OnScrolled(float scroll)
	{
		SetValue(mValue + scroll*mScrollSense);
	}

	void UIHorizontalProgress::OnDeserialized(const DataNode& node)
	{
		mBarLayer = GetLayer("bar");
		mBackLayer = GetLayer("back");

		UIWidget::OnDeserialized(node);
	}

	void UIHorizontalProgress::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	void UIHorizontalProgress::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (CheckIsLayoutDrivenByParent(forcible))
			return;

		RecalculateAbsRect();
		UpdateProgressLayersLayouts();
		UpdateLayersLayouts();

		if (withChildren)
			UpdateChildrenLayouts();
	}

	void UIHorizontalProgress::UpdateProgressLayersLayouts()
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

		UpdateLayersLayouts();
	}

	void UIHorizontalProgress::OnLayerAdded(UIWidgetLayer* layer)
	{
		if (layer->name == "back")
			mBackLayer = layer;
		else if (layer->name == "bar")
			mBarLayer = layer;

		UpdateProgressLayersLayouts();
	}

	void UIHorizontalProgress::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIHorizontalProgress, value, SetValue, GetValue);
		INITIALIZE_PROPERTY(UIHorizontalProgress, minValue, SetMinValue, GetMinValue);
		INITIALIZE_PROPERTY(UIHorizontalProgress, maxValue, SetMaxValue, GetMaxValue);
		INITIALIZE_PROPERTY(UIHorizontalProgress, scrollSense, SetScrollSense, GetScrollSense);
	}
}

CLASS_META(o2::UIHorizontalProgress)
{
	BASE_CLASS(o2::UIWidget);
	BASE_CLASS(o2::DrawableCursorEventsListener);

	PUBLIC_FIELD(value);
	PUBLIC_FIELD(minValue);
	PUBLIC_FIELD(maxValue);
	PUBLIC_FIELD(scrollSense);
	PUBLIC_FIELD(onChange);
	PROTECTED_FIELD(mValue).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSmoothValue);
	PROTECTED_FIELD(mMinValue).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mMaxValue).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScrollSense).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mOrientation).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mBarLayer);
	PROTECTED_FIELD(mBackLayer);

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetValue, float);
	PUBLIC_FUNCTION(void, SetValueForcible, float);
	PUBLIC_FUNCTION(float, GetValue);
	PUBLIC_FUNCTION(void, SetMinValue, float);
	PUBLIC_FUNCTION(float, GetMinValue);
	PUBLIC_FUNCTION(void, SetMaxValue, float);
	PUBLIC_FUNCTION(float, GetMaxValue);
	PUBLIC_FUNCTION(void, SetValueRange, float, float);
	PUBLIC_FUNCTION(void, SetScrollSense, float);
	PUBLIC_FUNCTION(float, GetScrollSense);
	PUBLIC_FUNCTION(void, SetOrientation, Orientation);
	PUBLIC_FUNCTION(Orientation, GetOrientation);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsScrollable);
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
	PROTECTED_FUNCTION(void, UpdateProgressLayersLayouts);
	PROTECTED_FUNCTION(void, OnLayerAdded, UIWidgetLayer*);
	PROTECTED_FUNCTION(void, GetValueFromCursor, const Input::Cursor&);
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

ENUM_META_(o2::UIHorizontalProgress::Orientation, Orientation)
{
	ENUM_ENTRY(Left);
	ENUM_ENTRY(Right);
}
END_ENUM_META;
 