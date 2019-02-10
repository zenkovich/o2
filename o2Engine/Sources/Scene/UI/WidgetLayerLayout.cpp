#include "stdafx.h"
#include "WidgetLayerLayout.h"

#include "WidgetLayer.h"

namespace o2
{

	UIWidgetLayerLayout::UIWidgetLayerLayout(UIWidgetLayer* widgetLayer) :
		mWidgetLayer(widgetLayer)
	{}

	UIWidgetLayerLayout::UIWidgetLayerLayout(UIWidgetLayer* widgetLayer, const UIWidgetLayerLayout& other) :
		mWidgetLayer(widgetLayer), mAnchorMin(other.mAnchorMin), mAnchorMax(other.mAnchorMax), mOffsetMin(other.mOffsetMin),
		mOffsetMax(other.mOffsetMax)
	{}

	UIWidgetLayerLayout::UIWidgetLayerLayout() :
		mWidgetLayer(nullptr)
	{}

	UIWidgetLayerLayout::operator Layout() const
	{
		return Layout(mAnchorMin, mAnchorMax, mOffsetMin, mOffsetMax);
	}

	UIWidgetLayerLayout& UIWidgetLayerLayout::operator=(const UIWidgetLayerLayout& other)
	{
		mAnchorMax = other.mAnchorMax;
		mAnchorMin = other.mAnchorMin;
		mOffsetMax = other.mOffsetMax;
		mOffsetMin = other.mOffsetMin;

		return *this;
	}

	bool UIWidgetLayerLayout::operator!=(const Layout& other) const
	{
		return mAnchorMin != other.anchorMin || mAnchorMax != other.anchorMax ||
			mOffsetMin != other.offsetMin || mOffsetMax != other.offsetMax;
	}

	bool UIWidgetLayerLayout::operator!=(const UIWidgetLayerLayout& other) const
	{
		return mAnchorMin != other.mAnchorMin || mAnchorMax != other.mAnchorMax ||
			mOffsetMin != other.mOffsetMin || mOffsetMax != other.mOffsetMax;
	}

	bool UIWidgetLayerLayout::operator==(const Layout& other) const
	{
		return mAnchorMin == other.anchorMin && mAnchorMax == other.anchorMax &&
			mOffsetMin == other.offsetMin && mOffsetMax == other.offsetMax;
	}

	bool UIWidgetLayerLayout::operator==(const UIWidgetLayerLayout& other) const
	{
		return mAnchorMin == other.mAnchorMin && mAnchorMax == other.mAnchorMax &&
			mOffsetMin == other.mOffsetMin && mOffsetMax == other.mOffsetMax;
	}

	UIWidgetLayerLayout& UIWidgetLayerLayout::operator=(const Layout& other)
	{
		mAnchorMin = other.anchorMin;
		mAnchorMax = other.anchorMax;
		mOffsetMin = other.offsetMin;
		mOffsetMax = other.offsetMax;

		return *this;
	}

	RectF UIWidgetLayerLayout::Calculate(const RectF& source)
	{
		Vec2F srcSize = source.Size();
		return RectF(source.left + srcSize.x*mAnchorMin.x + mOffsetMin.x,
					 source.bottom + srcSize.y*mAnchorMin.y + mOffsetMin.y,
					 source.left + srcSize.x*mAnchorMax.x + mOffsetMax.x,
					 source.bottom + srcSize.y*mAnchorMax.y + mOffsetMax.y);
	}

	void UIWidgetLayerLayout::SetAnchorMin(const Vec2F& min)
	{
		mAnchorMin = min;
		mWidgetLayer->OnLayoutChanged();
	}

	Vec2F UIWidgetLayerLayout::GetAnchorMin() const
	{
		return mAnchorMin;
	}

	void UIWidgetLayerLayout::SetAnchorMax(const Vec2F& max)
	{
		mAnchorMax = max;
		mWidgetLayer->OnLayoutChanged();
	}

	Vec2F UIWidgetLayerLayout::GetAnchorMax() const
	{
		return mAnchorMax;
	}

	void UIWidgetLayerLayout::SetAnchorLeft(float value)
	{
		mAnchorMin.x = value;
		mWidgetLayer->OnLayoutChanged();
	}

	float UIWidgetLayerLayout::GetAnchorLeft() const
	{
		return mAnchorMin.x;
	}

	void UIWidgetLayerLayout::SetAnchorRight(float value)
	{
		mAnchorMax.x = value;
		mWidgetLayer->OnLayoutChanged();
	}

	float UIWidgetLayerLayout::GetAnchorRight() const
	{
		return mAnchorMax.x;
	}

	void UIWidgetLayerLayout::SetAnchorBottom(float value)
	{
		mAnchorMin.y = value;
		mWidgetLayer->OnLayoutChanged();
	}

	float UIWidgetLayerLayout::GetAnchorBottom() const
	{
		return mAnchorMin.y;
	}

	void UIWidgetLayerLayout::SetAnchorTop(float value)
	{
		mAnchorMax.y = value;
		mWidgetLayer->OnLayoutChanged();
	}

	float UIWidgetLayerLayout::GetAnchorTop() const
	{
		return mAnchorMax.y;
	}

	void UIWidgetLayerLayout::SetOffsetMin(const Vec2F& min)
	{
		mOffsetMin = min;
		mWidgetLayer->OnLayoutChanged();
	}

	Vec2F UIWidgetLayerLayout::GetOffsetMin() const
	{
		return mOffsetMin;
	}

	void UIWidgetLayerLayout::SetOffsetMax(const Vec2F& max)
	{
		mOffsetMax = max;
		mWidgetLayer->OnLayoutChanged();
	}

	Vec2F UIWidgetLayerLayout::GetOffsetMax() const
	{
		return mOffsetMax;
	}

	void UIWidgetLayerLayout::SetOffsetLeft(float value)
	{
		mOffsetMin.x = value;
		mWidgetLayer->OnLayoutChanged();
	}

	float UIWidgetLayerLayout::GetOffsetLeft() const
	{
		return mAnchorMin.x;
	}

	void UIWidgetLayerLayout::SetOffsetRight(float value)
	{
		mOffsetMax.x = value;
		mWidgetLayer->OnLayoutChanged();
	}

	float UIWidgetLayerLayout::GetOffsetRight() const
	{
		return mOffsetMax.x;
	}

	void UIWidgetLayerLayout::SetOffsetBottom(float value)
	{
		mOffsetMin.y = value;
		mWidgetLayer->OnLayoutChanged();
	}

	float UIWidgetLayerLayout::GetOffsetBottom() const
	{
		return mAnchorMin.y;
	}

	void UIWidgetLayerLayout::SetOffsetTop(float value)
	{
		mOffsetMax.y = value;
		mWidgetLayer->OnLayoutChanged();
	}

	float UIWidgetLayerLayout::GetOffsetTop() const
	{
		return mOffsetMax.y;
	}

}

DECLARE_CLASS(o2::UIWidgetLayerLayout);
