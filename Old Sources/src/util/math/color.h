#pragma once

#include "util/public_namespace.h"
#include "math.h"

OPEN_O2_NAMESPACE

#define n255 0.00392156862745f

struct Color4
{
	int r, g, b, a;

	inline Color4();
	inline Color4(int r, int g, int b, int a);
	inline Color4(float r, float g, float b, float a);
	inline Color4(ulong value);

	inline operator ulong() const;

	inline bool operator==(const Color4& color) const;
	inline bool operator!=(const Color4& color) const;

	inline Color4 operator+(const Color4& v) const;
	inline Color4 operator+=(const Color4& v);

	inline Color4 operator-(const Color4& v) const;
	inline Color4 operator-=(const Color4& v);

	inline Color4 operator*(float v) const;
	inline Color4 operator*=(float v);

	inline Color4 operator/(float v) const;
	inline Color4 operator/=(float v);

	inline Color4 operator*(const Color4& v) const;
	inline Color4 operator*=(const Color4& v);

	inline Color4 operator/(const Color4& v) const;
	inline Color4 operator/=(const Color4& v);

	inline float RF() const;
	inline float GF() const;
	inline float BF() const;
	inline float AF() const;

	inline void SetRF(float value);
	inline void SetGF(float value);
	inline void SetBF(float value);
	inline void SetAF(float value);

	inline float GetF(int i) const;
	inline int&  Get(int i) const;

	inline void Set(ulong color);

	inline void   Normalize();
	inline Color4 Normalized() const;

	ulong ARGB() const;
	ulong ABGR() const;

	inline void SetARGB(ulong color);
	inline void SetABGR(ulong color);

	inline Color4 BlendByAlpha(const Color4& other);

	inline ulong AsULong() const;

	inline static ulong AsULong(int r, int g, int b, int a);

	inline static Color4 White();
	inline static Color4 Black();
	inline static Color4 Red();
	inline static Color4 Blue();
	inline static Color4 Green();
	inline static Color4 Gray();
	inline static Color4 SomeColor(int idx);
};

//implementation

Color4::Color4():
r(255), g(255), b(255), a(255)
{
}

Color4::Color4(int r, int g, int b, int a):
r(r), g(g), b(b), a(a)
{
}

Color4::Color4(float r, float g, float b, float a)
{
	this->r = (int)(r*255.0f);
	this->g = (int)(g*255.0f);
	this->b = (int)(b*255.0f);
	this->a = (int)(a*255.0f);
}

Color4::Color4(ulong value)
{
	Set(value);
}

Color4::operator ulong() const
{
	return AsULong();
}

bool Color4::operator==(const Color4& color) const
{
	return a == color.a && r == color.r && g == color.g && b == color.b;
}

bool Color4::operator!=(const Color4& color) const
{
	return a != color.a || r != color.r || g != color.g || b != color.b;
}

Color4 Color4::operator+(const Color4& v) const
{
	return Color4(r + v.r, g + v.g, b + v.g, a + v.a);
}

Color4 Color4::operator+=(const Color4& v)
{
	r += v.r; g += v.g; b += v.b; a += v.a;
	return *this;
}

Color4 Color4::operator-(const Color4& v) const
{
	return Color4(r - v.r, g - v.g, b - v.g, a - v.a);
}

Color4 Color4::operator-=(const Color4& v)
{
	r -= v.r; g -= v.g; b -= v.b; a -= v.a;
	return *this;
}

Color4 Color4::operator*(float v) const
{
	return Color4((int)((float)r*v), (int)((float)g*v), (int)((float)b*v), (int)((float)a*v));
}

Color4 Color4::operator*=(float v)
{
	*this = (*this)*v;
	return *this;
}

Color4 Color4::operator/(float v) const
{
	return *this*(1.0f/v);
}

Color4 Color4::operator/=(float v)
{
	*this = *this/v;
	return *this;
}

Color4 Color4::operator*(const Color4& v) const
{
	return Color4(r*v.r/255, g*v.g/255, b*v.g/255, a*v.a/255);
}

Color4 Color4::operator/(const Color4& v) const
{
	return Color4((int)(255.0f*(RF()/v.RF())),
				  (int)(255.0f*(GF()/v.GF())),
				  (int)(255.0f*(BF()/v.BF())),
				  (int)(255.0f*(AF()/v.AF())));
}

Color4 Color4::operator/=(const Color4& v)
{
	*this = *this/v;
	return *this;
}

Color4 Color4::operator*=(const Color4& v)
{
	*this = *this*v;
	return *this;
}

float Color4::RF() const
{
	return (float)r*n255;
}

float Color4::GF() const
{
	return (float)g*n255;
}

float Color4::BF() const
{
	return (float)b*n255;
}

float Color4::AF() const
{
	return (float)a*n255;
}

void Color4::SetRF(float value)
{
	r = (int)(value*255.0f);
}

void Color4::SetGF(float value)
{
	g = (int)(value*255.0f);
}

void Color4::SetBF(float value)
{
	b = (int)(value*255.0f);
}

void Color4::SetAF(float value)
{
	a = (int)(value*255.0f);
}

float Color4::GetF(int i) const
{
	return (float)(*(int*)(this + sizeof(int)*i))*n255;
}

int& Color4::Get(int i) const
{
	return (*(int*)(this + sizeof(int)*i));
}

void Color4::Normalize()
{
	r = Clamp(r, 0, 255);
	g = Clamp(g, 0, 255);
	b = Clamp(b, 0, 255);
	a = Clamp(a, 0, 255);
}

Color4 Color4::Normalized() const
{
	return Color4(Clamp(r, 0, 255),
				  Clamp(g, 0, 255),
				  Clamp(b, 0, 255),
				  Clamp(a, 0, 255));
}

ulong Color4::ARGB() const
{
	return (ulong)((a << 24) | (r << 16) | (g << 8) | b);
}

ulong Color4::ABGR() const
{
	return (ulong)((a << 24) | (b << 16) | (g << 8) | r);
}

void Color4::SetARGB(ulong color)
{
	a = (int)(color >> 24);
	r = (int)((color << 8) >> 24);
	g = (int)((color << 16) >> 24);
	b = (int)((color << 24) >> 24);
}

void Color4::SetABGR(ulong color)
{
	a = (int)(color >> 24);
	b = (int)((color << 8) >> 24);
	g = (int)((color << 16) >> 24);
	r = (int)((color << 24) >> 24);
}

Color4 Color4::BlendByAlpha(const Color4& other)
{
	float a1 = AF(), a2 = other.AF();
	float inva1 = 1.0f - a1;

	return Color4(int(a1*(float)r + a2*inva1*(float)other.r),
				  int(a1*(float)g + a2*inva1*(float)other.g),
				  int(a1*(float)b + a2*inva1*(float)other.b),
				  int(255.0f*(a1 + a2*inva1)));
}

ulong Color4::AsULong() const
{
#ifdef RENDER_OGL
	return ABGR();
#elif defined(RENDER_D3D9C)
	return ARGB();
#endif
}

void Color4::Set(ulong color)
{
#ifdef RENDER_OGL
	SetABGR(color);
#elif defined(RENDER_D3D9C)
	SetARGB(color);
#endif
}

ulong Color4::AsULong(int r, int g, int b, int a)
{
#ifdef RENDER_OGL
	return (unsigned long)((a << 24) | (b << 16) | (g << 8) | r);
#elif defined(RENDER_D3D9C)
	return (unsigned long)((a << 24) | (r << 16) | (g << 8) | b);
#endif
}

Color4 Color4::White()
{
	return Color4(255, 255, 255, 255);
}

Color4 Color4::Black()
{
	return Color4(0, 0, 0, 255);
}

Color4 Color4::Red()
{
	return Color4(255, 0, 0, 255);
}

Color4 Color4::Blue()
{
	return Color4(0, 0, 255, 255);
}

Color4 Color4::Green()
{
	return Color4(0, 255, 0, 255);
}

Color4 Color4::Gray()
{
	return Color4(150, 150, 150, 255);
}

Color4 Color4::SomeColor(int idx)
{
	return Color4((idx*33)%255, (idx*58)%255, (idx*77)%255, 255);
}

CLOSE_O2_NAMESPACE