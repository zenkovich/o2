#ifndef VECTOR2_H
#define VECTOR2_H

#include "public.h"

OPEN_O2_NAMESPACE

#include <math.h>

template<typename T>
struct vec2
{
	T x, y;

	inline vec2()
	{
		x = y = 0; 
	}

	inline vec2(T vx, T vy)
	{
		x = vx; y = vy;
	}

	inline vec2 operator+(const vec2& v) const
	{ 
		return vec2(x + v.x, y + v.y);
	}

	inline vec2 operator+=(const vec2& v) 
	{
		*this = *this + v; 
		return *this;
	}

	inline vec2 operator-(const vec2& v) const
	{
		return vec2(x - v.x, y - v.y);
	}

	inline vec2 operator-=(const vec2& v) 
	{ 
		*this = *this - v; return *this; 
	}

	inline vec2 operator*(T v) const
	{ 
		return vec2(x*v, y*v); 
	}

	inline vec2 operator*=(T v)
	{ 
		*this = (*this)*v; 
		return *this;
	}

	inline vec2 operator/(float v) const
	{
		return *this*(1.0f/v);
	}

	inline vec2 operator/(const vec2& v) const
	{
		return vec2(x/v.x, y/v.y);
	}

	inline vec2 operator/=(T v) 
	{ 
		*this = *this/v; 
		return *this; 
	}

	inline float operator*(const vec2& v) const
	{ 
		return (x*v.x + y*v.y);
	}

	inline float operator*=(const vec2& v) const
	{
		return *this*v; 
	}

	inline bool operator==(const vec2& v) const
	{ 
		if (abs(x - v.x) > (T)0.0001f || abs(y - v.y) > (T)0.0001f) return false; 		
		return true; 
	}

	inline bool operator!=(const vec2& v) const
	{ 
		return !(v == *this);
	}

	inline vec2 operator^(T s) const
	{
		return vec2(s*y, -s*x); 
	}

	inline float operator^(const vec2& v) const
	{
		return v.x*y - v.y*x; 
	}

	inline T& operator[](int idx) 
	{ 
		if (idx == 0)
			return x; 
		return y; 
	}

	inline vec2 scale(const vec2& v) const
	{
		return vec2(x*v.x, y*v.y);
	}	

	inline T& get(int i) 
	{ 
		if (i == 0) return x; 
		if (i == 1) return y;
		return x; 
	}

	inline void set(T vx, T vy)
	{
		x = vx; y = vy;
	}

	inline T dot(const vec2& v) const
	{
		return *this*v;
	}

	inline T len() const
	{
		return sqrt((*this)*(*this));
	}

	inline vec2 normalize() const
	{ 
		T ln = len();
		if (ln > 0) 
			return *this/ln; 
		else 
			return vec2(0, 0); 
	}

	inline vec2 Rotate(float rad) const
	{
		float cs = cosf(rad), 
			  sn = sinf(rad);

		vec2 v( (T)(cs*x - sn*y), (T)(sn*x + cs*y) );

		return v;
	}

	inline vec2 Rotate(float cs, float sn) const
	{
		vec2 v( (T)(cs*x - sn*y), (T)(sn*x + cs*y) );
		v = v^(T)(1.0f);

		return v;
	}

	inline void Rotate(float rad, const vec2& c) 
	{
		*this -= c;
		Rotate(rad); 
		*this += c;
	}	

	inline vec2 Inv(bool bx = true, bool by = true)  const
	{
		vec2 r = *this; 
		if (bx) r.x = -r.x; 
		if (by) r.y = -r.y;
		return r; 
	} 

	inline vec2 InvX() const
	{ 
		vec2 r = *this;
		r.x = -r.x; 
		return r; 
	}

	inline vec2 InvY() const
	{ 
		vec2 r = *this;
		r.y = -r.y;
		return r; 
	}

	template<typename T2>
	vec2<T2> castTo() const
	{
		return vec2<T2>((T2)x, (T2)y);
	}
};

template<typename T>
inline vec2<T> operator^(float s, const vec2<T>& a)
{
	return vec2<T>(-s*a.y, s*a.x);
}

template<typename T>
inline float len(const vec2<T>& a, const vec2<T>& b)
{ 
	return (b - a).len();
}

template<typename T>
inline vec2<T> proj(const vec2<T>& a, const vec2<T>& b)
{
	vec2<T> n = b.normalize();

	return n*(n*a);
}

template<typename T>
inline vec2<T> scale(const vec2<T>& a, const vec2<T>& b)
{
	return vec2(a.x*b.x, a.y*b.y);
}

template<typename T>
inline vec2<T> rotateVec(float rad, const vec2<T>& v)
{ 
	v.Rotate(rad); return v; 
}

#define vec2f vec2<float>
#define vec2i vec2<int>

CLOSE_O2_NAMESPACE

#endif //VECTOR2_H