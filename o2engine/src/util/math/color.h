#ifndef COLOR_H
#define COLOR_H

#include "public.h"
#include "engine_settings.h"
#include "math.h"

OPEN_O2_NAMESPACE
	
#define n255 0.00392156862745f

struct color4
{
	int r, g, b, a;

	inline color4()
	{
		r = g = b = a = 0;
	}

	inline color4(int cr, int cg, int cb, int ca) 
	{ 
		r = cr; g = cg; b = cb; a = ca;
	}

	inline color4(int c) 
	{ 
		r = g = b = a = c;
	}

	inline color4(float cr, float cg, float cb, float ca) 
	{ 
		r = (int)(cr*255.0f);
		g = (int)(cg*255.0f);
		b = (int)(cb*255.0f);
		a = (int)(ca*255.0f);
	}

	inline color4 operator+(const color4& v) const
	{ 
		return color4(r + v.r, g + v.g, b + v.g, a + v.a);
	}

	inline color4 operator+=(const color4& v) 
	{
		*this = *this + v; 
		return *this;
	}

	inline color4 operator-(const color4& v) const
	{
		return color4(r - v.r, g - v.g, b - v.g, a - v.a);
	}

	inline color4 operator-=(const color4& v) 
	{ 
		*this = *this - v; 
		return *this; 
	}

	inline color4 operator*(float v) const
	{ 
		return color4((int)((float)r*v), (int)((float)g*v), (int)((float)b*v), (int)((float)a*v)); 
	}

	inline color4 operator*=(float v)
	{ 
		*this = (*this)*v; 
		return *this;
	}

	inline color4 operator/(float v) const
	{
		return *this*(1.0f/v);
	}

	inline color4 operator/=(float v) 
	{ 
		*this = *this/v; 
		return *this; 
	}

	inline color4 operator*(const color4& v) const
	{ 
		return color4(r*v.r, g*v.g, b*v.g, a*v.a);
	}

	inline color4 operator/(const color4& v) const
	{ 
		return color4(r/v.r, g/v.g, b/v.g, a/v.a);
	}

	inline color4 operator*=(const color4& v) const
	{
		return *this*v; 
	}

	inline bool operator==(const color4& color)
	{
		if (abs(r - color.r) > 0 || abs(g - color.g) > 0 || abs(b - color.b) > 0 || abs(a - color.a) > 0)
			return false;
		return true;
	}
	
	inline float rf() const
	{
		return (float)r*n255;
	}

	inline float gf() const
	{
		return (float)g*n255; 
	}

	inline float bf() const
	{
		return (float)b*n255; 
	}

	inline float af() const
	{
		return (float)a*n255;
	}

	inline float getFloat(int i) const
	{
		return (float)(*(int*)(this + sizeof(float)*i))*n255;
		return 0;
	}

	inline char getParam(int i) const
	{	
		return (*(int*)(this + sizeof(float)*i));
		return 0;
	}

	inline void normalize()
	{
		r = clamp(r, 0, 255);
		g = clamp(g, 0, 255);
		b = clamp(b, 0, 255);
		a = clamp(a, 0, 255);
	}
	
	unsigned long ARGB() const { return (unsigned long)( (a << 24) | (r << 16) | (g << 8) | b ); }
	unsigned long ABGR() const { return (unsigned long)( (a << 24) | (b << 16) | (g << 8) | r ); }
	
	void setARGB(unsigned long color) 
	{ 
		a = (int)(color >> 24);
		r = (int)((color << 8) >> 24);
		g = (int)((color << 16) >> 24);
		b = (int)((color << 24) >> 24);
	}
	
	void setABGR(unsigned long color) 
	{ 
		a = (int)(color >> 24);
		b = (int)((color << 8) >> 24);
		g = (int)((color << 16) >> 24);
		r = (int)((color << 24) >> 24);
	}

	unsigned long dword() const 
	{
#ifdef RENDER_OGL
		return ARGB();
#elif defined(RENDER_D3D9C)
		return ABGR();
#endif
	}

	void setDword(unsigned long color)  
	{
#ifdef RENDER_OGL
		setARGB(color);
#elif defined(RENDER_D3D9C)
		setABGR(color);
#endif
	}

	static unsigned long dword(int r, int g, int b, int a)
	{
#ifdef RENDER_OGL
		return (unsigned long)( (a << 24) | (b << 16) | (g << 8) | r );
#elif defined(RENDER_D3D9C)
		return (unsigned long)( (a << 24) | (r << 16) | (g << 8) | b );
#endif
	}
};

CLOSE_O2_NAMESPACE

#endif //COLOR_H