#pragma once

#include "Utils/Math/Vector2.h"
#include "Utils/Math/Rect.h"
#include "Utils/Serialization.h"

namespace o2
{
	class Layout: public ISerializable
	{
	public:
		Vec2F anchorMin; // @SERIALIZABLE
		Vec2F anchorMax; // @SERIALIZABLE
		Vec2F offsetMin; // @SERIALIZABLE
		Vec2F offsetMax; // @SERIALIZABLE

		Layout();
		Layout(const Vec2F& anchorMin, const Vec2F& anchorMax, const Vec2F& offsetMin, const Vec2F& offsetMax);

		RectF Calculate(const RectF& source);

		bool operator==(const Layout& other) const;
		bool operator!=(const Layout& other) const;

		static Layout Both(const RectF& border = RectF());
		static Layout Both(float left, float bottom, float right, float top);
		static Layout Straight(const RectF& rect);
		static Layout Straight(float left, float bottom, float right, float top);

		SERIALIZABLE(Layout);
	};
}