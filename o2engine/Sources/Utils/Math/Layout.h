#pragma once

#include "Utils/Math/Vector2.h"
#include "Utils/Math/Rect.h"

namespace o2
{
	class Layout
	{
	public:
		Vec2F anchorMin;
		Vec2F anchorMax;
		Vec2F offsetMin;
		Vec2F offsetMax;

		Layout();
		Layout(const Vec2F& anchorMin, const Vec2F& anchorMax, const Vec2F& offsetMin, const Vec2F& offsetMax);

		RectF Calculate(const RectF& source);

		static Layout Both(const RectF& border);
		static Layout Straight(const RectF& rect);
	};
}