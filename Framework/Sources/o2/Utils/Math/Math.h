#pragma once

#include <float.h>
#include <math.h>
#include <random>
#include <stdlib.h>
#include "o2/Utils/Types/CommonTypes.h"

namespace o2
{
	template<typename T>
	class Rect;

	template<typename T>
	class Vec2;

	typedef Vec2<float> Vec2F;

	typedef Rect<float> RectF;

	namespace Math
	{
		inline float Floor(float value)
		{
			return floorf(value);
		}

		inline float Ceil(float value)
		{
			return ceilf(value);
		}

		inline float Round(float value)
		{
			return roundf(value);
		}

		inline int RoundToInt(float value)
		{
			return (int)roundf(value);
		}

		inline float Mod(float val, float x)
		{ 
			return fmodf(val, x);
		}

		Vec2F Round(const Vec2F& value);

		RectF Round(const RectF& value);

		inline float Pow(float value, float s)
		{
			return powf(value, s);
		}

		inline int FloorToInt(float value)
		{
			return (int)floorf(value);
		}

		inline int CeilToInt(float value)
		{
			return (int)ceilf(value);
		}

		template<typename T>
		inline T Sign(const T& value)
		{
			if (value < 0)
				return (T)(-1);

			return (T)1;
		}

		template<typename T>
		inline T Clamp(const T& value, const T& minv, const T& maxv)
		{
			if (value < minv)
				return minv;
			else if (value > maxv)
				return maxv;

			return value;
		}

		inline float Clamp01(float value)
		{
			return Clamp(value, 0.0f, 1.0f);
		}

		template<typename T>
		inline T Min(const T& v1, const T& v2)
		{
			if (v1 < v2)
				return v1;
			return v2;
		}

		template<typename T>
		inline T Max(const T& v1, const T& v2)
		{
			if (v1 > v2)
				return v1;

			return v2;
		}

		template<typename T>
		inline void Swap(T& v1, T& v2)
		{
			std::swap(v1, v2);
		}

		template<typename T>
		inline T Abs(const T& value)
		{
			if (value < 0)
				return -value;

			return value;
		}

		template<typename T>
		inline T Random(const T& minValue = 0, const T& maxValue = 1)
		{
			return (T)((float)rand() / RAND_MAX*(float)(maxValue - minValue) + (float)minValue);
		}
		
		inline UInt64 Random()
		{
			static std::default_random_engine generator;
			static std::uniform_int_distribution<unsigned long long> distribution;
			return distribution(generator);
		}

		template<typename T>
		inline T Lerp(const T& a, const T& b, float coef)
		{
			return (b - a)*coef + a;
		}

		template<typename T>
		inline T Lerpc(const T& a, const T& b, float coef)
		{
			return (b - a)*Clamp01(coef) + a;
		}

		RectF Lerp(const RectF& a, const RectF& b, float coef);

		bool Lerp(const bool& a, const bool& b, float coef);

		inline bool Equals(float a, float b, float range = FLT_EPSILON)
		{
			float x = a - b;
			return x*x < range*range;
		}

		template<typename T>
		inline bool Equals(const T& a, const T& b)
		{
			return a == b;
		}

		template<typename T>
		inline bool Fewer(const T& a, const T& b)
		{
			return a < b;
		}

		template<typename T>
		inline bool Greater(const T& a, const T& b)
		{
			return a > b;
		}

		inline float Sqrt(float value)
		{
			return sqrtf(value);
		}

		template<typename T>
		inline T Sqr(T value)
		{
			return value*value;
		}

		inline float PI()
		{
			return 3.1415926535897932384626433832795f;
		}

		inline float Deg2rad(const float& value)
		{
			return value*(PI() / 180.0f);
		}

		inline float Rad2deg(const float& value)
		{
			return value*(180.0f / PI());
		}

		inline float Sin(float rad)
		{
			return sinf(rad);
		}

		inline float Cos(float rad)
		{
			return cosf(rad);
		}

		inline float ASin(float value)
		{
			return asinf(value);
		}

		inline float ACos(float value)
		{
			return acosf(value);
		}

		inline float Atan2F(float x, float y)
		{
			return atan2f(x, y);
		}

		void OrthoProjMatrix(float* mat, float left, float right, float bottom, float top, float nearz, float farz);

		// Calculates tangent vector on ellipse that located on axis begin-end at point middle
		Vec2F CalculateEllipseTangent(const Vec2F& begin, const Vec2F& middle, const Vec2F& end);
	}

} //namespace o2
