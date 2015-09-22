#include "VectorFontEffects.h"

#include "Utils/Bitmap.h"

namespace o2
{
	IOBJECT_CPP(FontStrokeEffect);
	IOBJECT_CPP(FontShadowEffect);
	IOBJECT_CPP(FontGradientEffect);
	IOBJECT_CPP(FontColorEffect);

	FontStrokeEffect::FontStrokeEffect(float radius /*= 1.0f*/, const Color4& color /*= Color4::Black()*/,
									   int alphaThreshold /*= 100*/):
		radius(radius), color(color), alphaThreshold(alphaThreshold)
	{}

	void FontStrokeEffect::Process(Ptr<Bitmap> bitmap)
	{
		bitmap->Outline(radius, color, alphaThreshold);
	}

	Vec2I FontStrokeEffect::GetSizeExtend() const
	{
		return Vec2F::One()*radius;
	}

	FontGradientEffect::FontGradientEffect(const Color4& color1 /*= Color4(200, 200, 200, 255)*/, 
										   const Color4& color2 /*= Color4(255, 255, 255, 255)*/, 
										   float angle /*= 0*/, float length /*= 0*/, Vec2F origin /*= Vec2F()*/):
		color1(color1), color2(color2), angle(angle), length(length), origin(origin)
	{}

	void FontGradientEffect::Process(Ptr<Bitmap> bitmap)
	{
		bitmap->GradientByAlpha(color1, color2, angle, length, origin);
	}

	Vec2I FontGradientEffect::GetSizeExtend() const
	{
		return Vec2I();
	}

	FontColorEffect::FontColorEffect(const Color4& color /*= Color4::White()*/):
		color(color)
	{}

	void FontColorEffect::Process(Ptr<Bitmap> bitmap)
	{
		bitmap->Colorise(color);
	}

	Vec2I FontColorEffect::GetSizeExtend() const
	{
		return Vec2I();
	}

	FontShadowEffect::FontShadowEffect(float blurRadius /*= 2.0f*/, const Vec2I offset /*= Vec2I(2, 2)*/, 
									   const Color4& color /*= Color4::Black()*/):
		blurRadius(blurRadius), offset(offset), color(color)
	{}

	void FontShadowEffect::Process(Ptr<Bitmap> bitmap)
	{
		Bitmap shadow(*bitmap);
		shadow.Colorise(color);
		shadow.Blur(blurRadius);
		bitmap->BlendImage(&shadow, offset);
	}

	Vec2I FontShadowEffect::GetSizeExtend() const
	{
		return offset + (Vec2I)(Vec2F::One()*blurRadius);
	}

}