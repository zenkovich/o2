#pragma once
#include "o2/Utils/Math/Vector2.h"

namespace o2
{
	struct ApproximationValue
	{
		float position;
		float value;

	public:
		ApproximationValue() = default;
		ApproximationValue(const Vec2F& v):position(v.x), value(v.y) {}

		operator Vec2F() const { return Vec2F(position, value); }
	};
}
