#include "o2/stdafx.h"
#include "Intersection.h"

namespace o2
{
	Vec2F Intersection::Lines(const Vec2F& origin1, const Vec2F& dir1, const Vec2F& origin2, const Vec2F& dir2)
	{
		float det = dir1.x * dir2.y - dir2.x * dir1.y;

		if (Math::Abs(det) < FLT_EPSILON)
			return origin1;

		float s = (dir2.y * (origin2.x - origin1.x) - dir2.x * (origin2.y - origin1.y)) / det;

		return Vec2F(origin1.x + s * dir1.x,
					 origin1.y + s * dir1.y);
	}

	Vec2F Intersection::LinesNoChek(const Vec2F& origin1, const Vec2F& dir1, const Vec2F& origin2, const Vec2F& dir2)
	{
		float det = dir1.x * dir2.y - dir2.x * dir1.y;

		float s = (dir2.y * (origin2.x - origin1.x) - dir2.x * (origin2.y - origin1.y)) / det;

		return Vec2F(origin1.x + s * dir1.x,
					 origin1.y + s * dir1.y);
	}

	Vertex Intersection::LinesVertex(const Vertex& a1, const Vertex& b1, const Vec2F& a2, const Vec2F& b2)
	{
		Vec2F origin1 = a1, dir1 = (Vec2F)b1 - (Vec2F)a1;
		Vec2F origin2 = a2, dir2 = b2 - a2;

		float det = dir1.x * dir2.y - dir2.x * dir1.y;

		float s = (dir2.y * (origin2.x - origin1.x) - dir2.x * (origin2.y - origin1.y)) / det;

		return Vertex(origin1.x + s * dir1.x, origin1.y + s * dir1.y, a1.color,
					  Math::Lerp(a1.tu, b1.tu, s), Math::Lerp(a1.tv, b1.tv, s));
	}

}
