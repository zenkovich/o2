#ifndef MATH_H
#define MATH_H

#include "public.h"

OPEN_O2_NAMESPACE
	
#undef min
#undef max
#undef clamp
	
/* Basic Math functions. */
template<typename T>
T sign(const T& value)
{
	if (value < 0) 
		return (T)(-1);

	return (T)1;
}
	
template<typename T>
T clamp(const T& value, const T& minv, const T& maxv)
{
	if (value < minv)
		return minv;
	else if (value > maxv)
		return maxv;

	return value;
}

template<typename T>
T min(const T& v1, const T& v2)
{
	if (v1 < v2)
		return v1;
	return v2;
}

template<typename T>
T max(const T& v1, const T& v2)
{
	if (v1 > v2)
		return v1;
	return v2;
}

template<typename T>
void tswap(T& v1, T& v2)
{
	T tmp = v1;
	v1 = v2;
	v2 = tmp;
}

template<typename T>
T abs(const T& value)
{
	if (value < 0) 
		return -value;

	return value;
}

template<typename T>
T random(const T& minValue = 0, const T& maxValue = 1)
{
	return (T)((float)rand()/RAND_MAX*(float)(maxValue - minValue) + (float)minValue);
}

/* Trigonometry. */
#define PI 3.1415926535897932384626433832795f

template<typename T>
T deg2rad(const T& value)
{
	return value*( (T)(PI/180.0f) );
}

template<typename T>
T rad2deg(const T& value)
{
	return value*( (T)(180.0f/PI) );
}

/* Matricies. */
void orthoProjMatrix(float* mat, float left, float right, float bottom, float top, float nearz, float farz);

CLOSE_O2_NAMESPACE

#endif //MATH_H