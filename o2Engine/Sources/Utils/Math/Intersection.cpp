#include "stdafx.h"
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
}