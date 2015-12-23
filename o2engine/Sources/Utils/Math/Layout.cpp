#include "Layout.h"

namespace o2
{
	Layout::Layout():
		anchorMin(0, 0), anchorMax(1.0f, 1.0f), offsetMin(), offsetMax()
	{}

	Layout::Layout(const Vec2F& anchorMin, const Vec2F& anchorMax, const Vec2F& offsetMin, const Vec2F& offsetMax):
		anchorMin(anchorMin), anchorMax(anchorMax), offsetMin(offsetMin), offsetMax(offsetMax)
	{}

	RectF Layout::Calculate(const RectF& source)
	{
		Vec2F srcSize = source.Size();
		return RectF(source.left + srcSize.x*anchorMin.x + offsetMin.x,
					 source.bottom + srcSize.y*anchorMin.y + offsetMin.y,
					 source.left + srcSize.x*anchorMax.x + offsetMax.x,
					 source.bottom + srcSize.y*anchorMax.y + offsetMax.y);
	}

	bool Layout::operator==(const Layout& other) const
	{
		return anchorMin == other.anchorMin && anchorMax == other.anchorMax &&
			offsetMin == other.offsetMin && offsetMax == other.offsetMax;
	}

	bool Layout::operator!=(const Layout& other) const
	{
		return anchorMin != other.anchorMin || anchorMax != other.anchorMax ||
			offsetMin != other.offsetMin || offsetMax != other.offsetMax;
	}

	Layout Layout::Both(const RectF& border)
	{
		return Layout(Vec2F(), Vec2F::One(), border.LeftBottom(), border.RightTop()*-1.0f);
	}

	Layout Layout::Both(float left, float bottom, float right, float top)
	{
		return Layout(Vec2F(), Vec2F::One(), Vec2F(left, bottom), Vec2F(-right, -top));
	}

	Layout Layout::Straight(const RectF& rect)
	{
		return Layout(Vec2F(), Vec2F(), rect.LeftBottom(), rect.RightTop());
	}

	Layout Layout::Straight(float left, float bottom, float right, float top)
	{
		return Layout(Vec2F(), Vec2F(), Vec2F(left, bottom), Vec2F(right, top));
	}
}