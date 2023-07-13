#include "o2/stdafx.h"
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

	Layout Layout::BothStretch(const BorderF& border)
	{
		return Layout(Vec2F(), Vec2F::One(), border.LeftBottom(), border.RightTop()*-1.0f);
	}

	Layout Layout::BothStretch(float borderLeft /*= 0*/, float borderBottom /*= 0*/, 
							   float borderRight /*= 0*/, float borderTop /*= 0*/)
	{
		Layout res;
		res.anchorMin = Vec2F(0, 0);
		res.anchorMax = Vec2F(1, 1);
		res.offsetMin = Vec2F(borderLeft, borderBottom);
		res.offsetMax = Vec2F(-borderRight, -borderTop);
		return res;
	}

	Layout Layout::Based(BaseCorner corner, const Vec2F& size, const Vec2F& offset /*= Vec2F()*/)
	{
		Layout res;
		switch (corner)
		{
			case BaseCorner::Left:
			res.anchorMin = Vec2F(0.0f, 0.5f);
			res.anchorMax = Vec2F(0.0f, 0.5f);
			res.offsetMin = Vec2F(0.0f, -size.y*0.5f) + offset;
			res.offsetMax = Vec2F(size.x, size.y*0.5f) + offset;
			break;
			case BaseCorner::Right:
			res.anchorMin = Vec2F(1.0f, 0.5f);
			res.anchorMax = Vec2F(1.0f, 0.5f);
			res.offsetMin = Vec2F(-size.x, -size.y*0.5f) + offset;
			res.offsetMax = Vec2F(0.0f, size.y*0.5f) + offset;
			break;
			case BaseCorner::Top:
			res.anchorMin = Vec2F(0.5f, 1.0f);
			res.anchorMax = Vec2F(0.5f, 1.0f);
			res.offsetMin = Vec2F(-size.x*0.5f, -size.y) + offset;
			res.offsetMax = Vec2F(size.x*0.5f, 0.0f) + offset;
			break;
			case BaseCorner::Bottom:
			res.anchorMin = Vec2F(0.5f, 0.0f);
			res.anchorMax = Vec2F(0.5f, 0.0f);
			res.offsetMin = Vec2F(-size.x*0.5f, 0.0f) + offset;
			res.offsetMax = Vec2F(size.x*0.5f, size.y) + offset;
			break;
			case BaseCorner::Center:
			res.anchorMin = Vec2F(0.5f, 0.5f);
			res.anchorMax = Vec2F(0.5f, 0.5f);
			res.offsetMin = Vec2F(-size.x*0.5f, -size.y*0.5f) + offset;
			res.offsetMax = Vec2F(size.x*0.5f, size.y*0.5f) + offset;
			break;
			case BaseCorner::LeftBottom:
			res.anchorMin = Vec2F(0.0f, 0.0f);
			res.anchorMax = Vec2F(0.0f, 0.0f);
			res.offsetMin = Vec2F(0.0f, 0.0f) + offset;
			res.offsetMax = Vec2F(size.x, size.y) + offset;
			break;
			case BaseCorner::LeftTop:
			res.anchorMin = Vec2F(0.0f, 1.0f);
			res.anchorMax = Vec2F(0.0f, 1.0f);
			res.offsetMin = Vec2F(0.0f, -size.y) + offset;
			res.offsetMax = Vec2F(size.x, 0.0f) + offset;
			break;
			case BaseCorner::RightBottom:
			res.anchorMin = Vec2F(1.0f, 0.0f);
			res.anchorMax = Vec2F(1.0f, 0.0f);
			res.offsetMin = Vec2F(-size.x, 0.0f) + offset;
			res.offsetMax = Vec2F(0.0f, size.y) + offset;
			break;
			case BaseCorner::RightTop:
			res.anchorMin = Vec2F(1.0f, 1.0f);
			res.anchorMax = Vec2F(1.0f, 1.0f);
			res.offsetMin = Vec2F(-size.x, -size.y) + offset;
			res.offsetMax = Vec2F(0.0f, 0.0f) + offset;
			break;
		}

		return res;
	}

	Layout Layout::HorStretch(VerAlign align, float left, float right, float height, float offsY /*= 0.0f*/)
	{
		Layout res;
		res.anchorMin.x = 0.0f;
		res.anchorMax.x = 1.0f;
		res.offsetMin.x = left;
		res.offsetMax.x = -right;

		switch (align)
		{
			case VerAlign::Top:
			res.anchorMin.y = 1.0f;
			res.anchorMax.y = 1.0f;
			res.offsetMin.y = -offsY - height;
			res.offsetMax.y = -offsY;
			break;

			case VerAlign::Middle:
			res.anchorMin.y = 0.5f;
			res.anchorMax.y = 0.5f;
			res.offsetMin.y = offsY - height*0.5f;
			res.offsetMax.y = offsY + height*0.5f;
			break;

			case VerAlign::Bottom:
			res.anchorMin.y = 0.0f;
			res.anchorMax.y = 0.0f;
			res.offsetMin.y = offsY;
			res.offsetMax.y = offsY + height;
			break;

			case VerAlign::Both:
			break;
		}

		return res;
	}

	Layout Layout::VerStretch(HorAlign align, float top, float bottom, float width, float offsX /*= 0.0f*/)
	{
		Layout res;
		res.anchorMin.y = 0.0f;
		res.anchorMax.y = 1.0f;
		res.offsetMin.y = bottom;
		res.offsetMax.y = -top;

		switch (align)
		{
			case HorAlign::Left:
			res.anchorMin.x = 0.0f;
			res.anchorMax.x = 0.0f;
			res.offsetMin.x = offsX + width;
			res.offsetMax.x = offsX;
			break;

			case HorAlign::Middle:
			res.anchorMin.x = 0.5f;
			res.anchorMax.x = 0.5f;
			res.offsetMin.x = offsX - width*0.5f;
			res.offsetMax.x = offsX + width*0.5f;
			break;

			case HorAlign::Right:
			res.anchorMin.x = 1.0f;
			res.anchorMax.x = 1.0f;
			res.offsetMin.x = -offsX - width;
			res.offsetMax.x = -offsX;
			break;

			case HorAlign::Both:
			break;
		}

		return res;
	}

}
// --- META ---

DECLARE_CLASS(o2::Layout);
// --- END META ---
