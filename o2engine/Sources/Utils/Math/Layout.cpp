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
					 source.right + srcSize.x*anchorMax.x + offsetMax.x,
					 source.top + srcSize.y*anchorMax.x + offsetMax.y);
	}

	Layout Layout::Both(const RectF& border)
	{
		return Layout(Vec2F(), Vec2F::One(), border.LeftBottom(), border.RightTop());
	}

	Layout Layout::Straight(const RectF& rect)
	{
		return Layout(Vec2F(), Vec2F(), rect.LeftBottom(), rect.RightTop());
	}

}