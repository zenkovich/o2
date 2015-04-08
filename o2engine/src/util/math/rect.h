#pragma once

#include <algorithm>
#include <float.h>
#include "util/public_namespace.h"
#include "vector2.h"
#include "math.h"

OPEN_O2_NAMESPACE

template<typename T>
struct Rect
{
	T left, top, right, bottom;

	inline Rect();
	inline Rect(const Vec2<T>& leftTop, const Vec2<T>& rightDown);
	inline Rect(float left, float top, float right, float bottom);

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
	inline void Set(const Vec2F& leftTop, const Vec2F& rightDown);

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
};

typedef Rect<float> RectF;
typedef Rect<int>   RectI;

//implementation

template<typename T>
Rect<T>::Rect():
left(0), right(0), top(0), bottom(0)
{
}

template<typename T>
Rect<T>::Rect(const Vec2<T>& leftTop, const Vec2<T>& rightDown):
left(leftTop.x), top(leftTop.y), right(rightDown.x), bottom(rightDown.y)
{
}

template<typename T>
Rect<T>::Rect(float left, float top, float right, float bottom):
left(left), top(top), right(right), bottom(bottom)
{
}

template<typename T>
bool Rect<T>::operator==(const Rect<T>& rt) const
{
	return Abs(left - rt.left)   < (T)FLT_EPSILON || Abs(top - rt.top)     < (T)FLT_EPSILON ||
		Abs(right - rt.right) < (T)FLT_EPSILON || Abs(bottom - rt.bottom) < (T)FLT_EPSILON;
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
	return Rect(Min(left, r.left),
				Max(top, r.top),
				Max(right, r.right),
				Min(bottom, r.bottom));
}

template<typename T>
Rect<T> Rect<T>::operator+=(const Rect<T>& r)
{
	left = Min(left, r.left);
	top = Max(top, r.top);
	right = Max(right, r.right);
	bottom = Min(bottom, r.bottom);
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
	return *this;
}

template<typename T>
void Rect<T>::Set(float vleft, float vtop, float vright, float vbottom)
{
	left = vleft; top = vtop;
	right = vright; bottom = vbottom;
}

template<typename T>
void Rect<T>::Set(const Vec2F& leftTop, const Vec2F& rightDown)
{
	left = leftTop.x; top = leftTop.y;
	right = rightDown.x; bottom = rightDown.y;
}

template<typename T>
void Rect<T>::SetPosition(const Vec2<T>& position)
{
	left = position.x; right = position.y;
}

template<typename T>
void Rect<T>::SetSize(const Vec2<T>& size)
{
	right = left + size.x; bottom = top + size.y;
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
	return Abs(right - left);
}

template<typename T>
T Rect<T>::Height() const
{
	return Abs(bottom - top);
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
	return Rect(Min(left, other.left),
				Max(top, other.top),
				Max(right, other.right),
				Min(bottom, other.down));
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
	return left < FLT_EPSILON && right < FLT_EPSILON && top < FLT_EPSILON && bottom < FLT_EPSILON;
}

CLOSE_O2_NAMESPACE