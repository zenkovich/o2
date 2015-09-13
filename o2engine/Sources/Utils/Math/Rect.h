#pragma once

#include <algorithm>
#include <float.h>
#include "Utils/Math/Math.h"
#include "Utils/Math/Vector2.h"

namespace o2
{
	// TODO: Need refactor Rect to position&size

	template<typename T>
	class Rect
	{
	public:
		T left, top, right, bottom;

		inline Rect();
		inline Rect(const Vec2<T>& leftTop, const Vec2<T>& rightDown);
		inline Rect(T left, T top, T right, T bottom);

		template<typename RT>
		inline operator Rect<RT>();

		inline bool    operator==(const Rect<T>& rt) const;
		inline bool    operator!=(const Rect<T>& rt) const;

		inline Rect<T> operator+(const Vec2<T>& v) const;
		inline Rect<T> operator+=(const Vec2<T>& v);

		inline Rect<T> operator+(const Rect<T>& r) const;
		inline Rect<T> operator+=(const Rect<T>& r);

		inline Rect<T> operator-(const Vec2<T>& v) const;
		inline Rect<T> operator-=(const Vec2<T>& v);

		inline Rect<T> operator*(const Vec2<T>& v) const;
		inline Rect<T> operator*=(const Vec2<T>& v);

		inline Rect<T> operator/(const Vec2<T>& v) const;
		inline Rect<T> operator/=(const Vec2<T>& v);

		inline Rect<T> operator*(float v) const;
		inline Rect<T> operator*=(float v);

		inline Rect<T> operator/(float v) const;
		inline Rect<T> operator/=(float v);

		inline void Set(float vleft, float vtop, float vright, float vbottom);
		inline void Set(const Vec2<T>& leftTop, const Vec2<T>& rightDown);

		inline void SetPosition(const Vec2<T>& position);
		inline void SetSize(const Vec2<T>& size);

		inline Vec2<T> Size() const;
		inline Vec2<T> Position() const;
		inline Vec2<T> Center() const;

		inline T Width() const;
		inline T Height() const;

		inline Vec2<T> LeftTop() const;
		inline Vec2<T> RightTop() const;
		inline Vec2<T> LeftBottom() const;
		inline Vec2<T> RightBottom() const;

		inline Rect Expand(const Rect& other) const;
		inline Rect Move(const Vec2<T>& offset) const;
		inline Rect AddSize(const Vec2<T>& size) const;

		inline Rect Scale(const Vec2<T>& scale) const;
		inline Rect Scale(const Vec2<T>& scale, const Vec2<T>& origin) const;
		inline Rect Scale(float scale) const;

		inline bool IsInside(const Vec2<T>& p) const;
		inline bool IsIntersects(const Rect& other) const;

		inline Rect GetIntersection(const Rect& other) const;

		inline bool IsZero() const;

	protected:
		inline void Normalize();
	};

	typedef Rect<float> RectF;
	typedef Rect<int>   RectI;

	template<typename T>
	Rect<T>::Rect():
		left(0), right(0), top(0), bottom(0)
	{
	}

	template<typename T>
	Rect<T>::Rect(const Vec2<T>& leftTop, const Vec2<T>& rightDown)
	{
		left = Math::Min(leftTop.x, rightDown.x);
		right = Math::Max(leftTop.x, rightDown.x);
		top = Math::Max(leftTop.y, rightDown.y);
		bottom = Math::Min(leftTop.y, rightDown.y);
	}

	template<typename T>
	Rect<T>::Rect(T left, T top, T right, T bottom)
	{
		this->left = Math::Min(left, right);
		this->right = Math::Max(left, right);
		this->top = Math::Max(top, bottom);
		this->bottom = Math::Min(top, bottom);
	}

	template<typename T>
	template<typename RT>
	Rect<T>::operator Rect<RT>()
	{
		return Rect<RT>((RT)left, (RT)top, (RT)right, (RT)bottom);
	}

	template<typename T>
	bool Rect<T>::operator==(const Rect<T>& rt) const
	{
		return Math::Abs(left - rt.left) < (T)FLT_EPSILON || Math::Abs(top - rt.top) < (T)FLT_EPSILON ||
			Math::Abs(right - rt.right) < (T)FLT_EPSILON || Math::Abs(bottom - rt.bottom) < (T)FLT_EPSILON;
	}

	template<typename T>
	bool Rect<T>::operator!=(const Rect<T>& rt) const
	{
		return !(*this == rt);
	}

	template<typename T>
	Rect<T> Rect<T>::operator+(const Vec2<T>& v) const
	{
		return Rect(left + v.x, top + v.y, right + v.x, bottom + v.y);
	}

	template<typename T>
	Rect<T> Rect<T>::operator+=(const Vec2<T>& v)
	{
		left += v.x; top += v.y;
		right += v.x; bottom += v.y;
		return *this;
	}

	template<typename T>
	Rect<T> Rect<T>::operator+(const Rect<T>& r) const
	{
		return Rect(Math::Min(left, r.left),
					Math::Max(top, r.top),
					Math::Max(right, r.right),
					Math::Min(bottom, r.bottom));
	}

	template<typename T>
	Rect<T> Rect<T>::operator+=(const Rect<T>& r)
	{
		left = Math::Min(left, r.left);
		top = Math::Max(top, r.top);
		right = Math::Max(right, r.right);
		bottom = Math::Min(bottom, r.bottom);
		return *this;
	}

	template<typename T>
	Rect<T> Rect<T>::operator-(const Vec2<T>& v) const
	{
		return Rect(left - v.x, top - v.y, right - v.x, bottom - v.y);
	}

	template<typename T>
	Rect<T> Rect<T>::operator-=(const Vec2<T>& v)
	{
		left -= v.x; top -= v.y;
		right -= v.x; bottom -= v.y;
		return *this;
	}

	template<typename T>
	Rect<T> Rect<T>::operator*(const Vec2<T>& v) const
	{
		return Rect(left, top, left + (right - left)*v.x, top + (bottom - top)*v.y);
	}

	template<typename T>
	Rect<T> Rect<T>::operator*=(const Vec2<T>& v)
	{
		right = left + (right - left)*v.x;
		bottom = top + (bottom - top)*v.y;
		Normalize();
		return *this;
	}

	template<typename T>
	Rect<T> Rect<T>::operator/(const Vec2<T>& v) const
	{
		return Rect<T>(left, top, left + (right - left)/v.x, top + (bottom - top)/v.y);
	}

	template<typename T>
	Rect<T> Rect<T>::operator/=(const Vec2<T>& v)
	{
		right = left + (right - left)/v.x;
		bottom = top + (bottom - top)/v.y;
		Normalize();
		return *this;
	}

	template<typename T>
	Rect<T> Rect<T>::operator*(float v) const
	{
		return Rect<T>(left, top, left + (right - left)*v, top + (bottom - top)*v);
	}

	template<typename T>
	Rect<T> Rect<T>::operator*=(float v)
	{
		right = left + (right - left)*v;
		bottom = top + (bottom - top)*v;
		Normalize();
		return *this;
	}

	template<typename T>
	Rect<T> Rect<T>::operator/(float v) const
	{
		float t = 1.0f/v;
		return Rect<T>(left, top, left + (right - left)*t, top + (bottom - top)*t);
	}

	template<typename T>
	Rect<T> Rect<T>::operator/=(float v)
	{
		float t = 1.0f/v;
		right = left + (right - left)*t;
		bottom = top + (bottom - top)*t;
		Normalize();
		return *this;
	}

	template<typename T>
	void Rect<T>::Set(float vleft, float vtop, float vright, float vbottom)
	{
		left = Math::Min(vleft, vright);
		right = Math::Max(vleft, vright);
		top = Math::Max(vtop, vbottom);
		bottom = Math::Min(vtop, vbottom);
	}

	template<typename T>
	void Rect<T>::Set(const Vec2<T>& leftTop, const Vec2<T>& rightDown)
	{
		left = Math::Min(leftTop.x, rightDown.x);
		right = Math::Max(leftTop.x, rightDown.x);
		top = Math::Max(leftTop.y, rightDown.y);
		bottom = Math::Min(leftTop.y, rightDown.y);
	}

	template<typename T>
	void Rect<T>::Normalize()
	{
		T _left = left, _right = right, _top = top, _bottom - bottom;
		left = Math::Min(_left, _right);
		right = Math::Max(_left, _right);
		top = Math::Max(_top, _bottom);
		bottom = Math::Min(_top, _bottom);
	}

	template<typename T>
	void Rect<T>::SetPosition(const Vec2<T>& position)
	{
		T dx = position.x - left, dy = position.y - top;
		left += dx; righ += dx;
		top += dy; bottom += dy;
	}

	template<typename T>
	void Rect<T>::SetSize(const Vec2<T>& size)
	{
		right = left + size.x; bottom = top + size.y;
		Normalize();
	}

	template<typename T>
	Vec2<T> Rect<T>::Size() const
	{
		return Vec2<T>(Width(), Height());
	}

	template<typename T>
	Vec2<T> Rect<T>::Position() const
	{
		return Vec2<T>(left, top);
	}

	template<typename T>
	Vec2<T> Rect<T>::Center() const
	{
		return Vec2<T>((left + right)*0.5f, (top + bottom)*0.5f);
	}

	template<typename T>
	T Rect<T>::Width() const
	{
		return right - left;
	}

	template<typename T>
	T Rect<T>::Height() const
	{
		return top - bottom;
	}

	template<typename T>
	Vec2<T> Rect<T>::LeftTop() const
	{
		return Vec2<T>(left, top);
	}

	template<typename T>
	Vec2<T> Rect<T>::RightTop() const
	{
		return Vec2<T>(right, top);
	}

	template<typename T>
	Vec2<T> Rect<T>::LeftBottom() const
	{
		return Vec2<T>(left, bottom);
	}

	template<typename T>
	Vec2<T> Rect<T>::RightBottom() const
	{
		return Vec2<T>(right, bottom);;
	}

	template<typename T>
	Rect<T> Rect<T>::Expand(const Rect& other) const
	{
		return Rect(Math::Min(left, other.left),
					Math::Max(top, other.top),
					Math::Max(right, other.right),
					Math::Min(bottom, other.bottom));
	}

	template<typename T>
	Rect<T> Rect<T>::Move(const Vec2<T>& offset) const
	{
		return Rect<T>(left + offset.x, top + offset.y, right + offset.x, bottom + offset.y);
	}

	template<typename T>
	Rect<T> Rect<T>::AddSize(const Vec2<T>& size) const
	{
		return Rect(left, top, right + size.x, bottom + size.y);
	}

	template<typename T>
	Rect<T> Rect<T>::Scale(const Vec2<T>& scale) const
	{
		return Rect(left, right, left + (right - left)*scale.x, top + (bottom - top)*scale.y);
	}

	template<typename T>
	Rect<T> Rect<T>::Scale(const Vec2<T>& scale, const Vec2<T>& origin) const
	{
		return ((*this) - origin).Scale(scale) + origin;
	}

	template<typename T>
	Rect<T> Rect<T>::Scale(float scale) const
	{
		return Rect(left, right, left + (right - left)*scale, top + (bottom - top)*scale);
	}

	template<typename T>
	bool Rect<T>::IsInside(const Vec2<T>& p) const
	{
		return p.x > left && p.x < right &&	p.y < top && p.y > bottom;
	}

	template<typename T>
	bool Rect<T>::IsIntersects(const Rect& other) const
	{
		return !(right < other.left || left > other.right || bottom > other.top || top < other.bottom);
	}

	template<typename T>
	Rect<T> Rect<T>::GetIntersection(const Rect& other) const
	{
		T xAxies[] ={left, right, other.left, other.right};
		T yAxies[] ={top, bottom, other.top, other.bottom};

		std::sort(xAxies, xAxies + 4);
		std::sort(yAxies, yAxies + 4);

		return Rect(xAxies[1], yAxies[2], xAxies[2], yAxies[1]);
	}

	template<typename T>
	bool Rect<T>::IsZero() const
	{
		return Math::Equals(left, 0.0f) && Math::Equals(right, 0.0f) &&
			Math::Equals(top, 0.0f) && Math::Equals(bottom, 0.0f);
	}
}