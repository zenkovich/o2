#pragma once

#include "Utils/Math/Math.h"
#include "Utils/Math/Vector2.h"

namespace o2
{
	template<typename T>
	struct Border
	{
		T left, bottom, right, top;

		inline Border();
		inline Border(T left, T bottom, T right, T top);

		template<typename T2>
		inline Border(const Border<T2>& other);

		inline bool operator==(const Border<T>& other) const;
		inline bool operator!=(const Border<T>& other) const;

		inline Border<T> operator+(const Border<T>& other);
		inline Border<T> operator-(const Border<T>& other);
		inline Border<T> operator/(const Border<T>& other);
		inline Border<T> operator*(const Border<T>& other);

		inline Vec2<T> LeftTop() const;
		inline Vec2<T> LeftBottom() const;
		inline Vec2<T> RightTop() const;
		inline Vec2<T> RightBottom() const;
	};

	typedef Border<float> BorderF;
	typedef Border<int>   BorderI;

	template<typename T>
	Border<T>::Border():left(0), bottom(0), right(0), top(0) {}

	template<typename T>
	Border<T>::Border(T left, T bottom, T right, T top):left(left), bottom(bottom), right(right), top(top) {}

	template<typename T>
	template<typename T2>
	Border<T>::Border(const Border<T2>& other) : left((T)other.left), bottom((T)other.bottom), right((T)other.right), top((T)other.top) {}

	template<typename T>
	bool Border<T>::operator==(const Border<T>& other) const
	{
		return Math::Equals(left, other.left) && Math::Equals(bottom, other.bottom) &&
			Math::Equals(right, other.right) && Math::Equals(top, other.top);
	}

	template<typename T>
	bool Border<T>::operator!=(const Border<T>& other) const
	{
		return !(*this == other);
	}

	template<typename T>
	Border<T> Border<T>::operator+(const Border<T>& other)
	{
		left   += other.left;
		right  += other.right;
		bottom += other.bottom;
		top    += other.top;

		return *this;
	}

	template<typename T>
	Border<T> Border<T>::operator-(const Border<T>& other)
	{
		left   -= other.left;
		right  -= other.right;
		bottom -= other.bottom;
		top    -= other.top;

		return *this;
	}

	template<typename T>
	Border<T> Border<T>::operator/(const Border<T>& other)
	{
		left   /= other.left;
		right  /= other.right;
		bottom /= other.bottom;
		top    /= other.top;

		return *this;
	}

	template<typename T>
	Border<T> Border<T>::operator*(const Border<T>& other)
	{
		left   *= other.left;
		right  *= other.right;
		bottom *= other.bottom;
		top    *= other.top;

		return *this;
	}

	template<typename T>
	Vec2<T> Border<T>::RightBottom() const
	{
		return Vec2<T>(right, bottom);
	}

	template<typename T>
	Vec2<T> Border<T>::RightTop() const
	{
		return Vec2<T>(right, top);
	}

	template<typename T>
	Vec2<T> Border<T>::LeftBottom() const
	{
		return Vec2<T>(left, bottom);
	}

	template<typename T>
	Vec2<T> Border<T>::LeftTop() const
	{
		return Vec2<T>(left, top);
	}
}
