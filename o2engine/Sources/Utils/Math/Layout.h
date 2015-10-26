#pragma once

#include "Utils/Math/Vector2.h"
#include "Utils/Math/Rect.h"
#include "Utils/Serialization.h"

namespace o2
{
	class Layout: public ISerializable
	{
	public:
		Vec2F anchorMin;
		Vec2F anchorMax;
		Vec2F offsetMin;
		Vec2F offsetMax;

		Layout();
		Layout(const Vec2F& anchorMin, const Vec2F& anchorMax, const Vec2F& offsetMin, const Vec2F& offsetMax);

		RectF Calculate(const RectF& source);

		static Layout Both(const RectF& border = RectF());
		static Layout Both(float left, float bottom, float right, float top);
		static Layout Straight(const RectF& rect);
		static Layout Straight(float left, float bottom, float right, float top);

		SERIALIZABLE_IMPL(Layout);

		IOBJECT(Layout)
		{
			SRLZ_FIELD(anchorMin);
			SRLZ_FIELD(anchorMax);
			SRLZ_FIELD(offsetMin);
			SRLZ_FIELD(offsetMax);
		}
	};
}