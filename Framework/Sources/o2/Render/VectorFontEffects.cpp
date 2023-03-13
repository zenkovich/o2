#include "o2/stdafx.h"
#include "VectorFontEffects.h"

#include "o2/Utils/Bitmap/Bitmap.h"

namespace o2
{
	FontStrokeEffect::FontStrokeEffect(float radius /*= 1.0f*/, const Color4& color /*= Color4::Black()*/,
									   int alphaThreshold /*= 100*/):
		radius(radius), color(color), alphaThreshold(alphaThreshold)
	{}

	void FontStrokeEffect::Process(const Ref<Bitmap>& bitmap)
	{
		bitmap->Outline(radius, color, alphaThreshold);
	}

	Vec2I FontStrokeEffect::GetSizeExtend() const
	{
		return Vec2F::One()*radius;
	}

	bool FontStrokeEffect::IsEqual(const Ref<VectorFont::Effect>& other) const
	{
		if (!VectorFont::Effect::IsEqual(other))
			return false;

		Ref<FontStrokeEffect> otherEff = StaticCast<FontStrokeEffect>(other);
		return Math::Equals(radius, otherEff->radius) && alphaThreshold == otherEff->alphaThreshold &&
			color == otherEff->color;
	}

	FontGradientEffect::FontGradientEffect(const Color4& color1 /*= Color4(200, 200, 200, 255)*/, 
										   const Color4& color2 /*= Color4(255, 255, 255, 255)*/, 
										   float angle /*= 0*/, float length /*= 0*/, Vec2F origin /*= Vec2F()*/):
		color1(color1), color2(color2), angle(angle), length(length), origin(origin)
	{}

	void FontGradientEffect::Process(const Ref<Bitmap>& bitmap)
	{
		bitmap->GradientByAlpha(color1, color2, angle, length, origin);
	}

	Vec2I FontGradientEffect::GetSizeExtend() const
	{
		return Vec2I();
	}

	bool FontGradientEffect::IsEqual(const Ref<VectorFont::Effect>& other) const
	{
		if (!VectorFont::Effect::IsEqual(other))
			return false;

		Ref<FontGradientEffect> otherEff = StaticCast<FontGradientEffect>(other);
		return Math::Equals(angle, otherEff->angle) && Math::Equals(length, otherEff->length) &&
			color1 == otherEff->color1 && color2 == otherEff->color2 && origin == otherEff->origin;
	}

	FontColorEffect::FontColorEffect(const Color4& color /*= Color4::White()*/):
		color(color)
	{}

	void FontColorEffect::Process(const Ref<Bitmap>& bitmap)
	{
		bitmap->Colorise(color);
	}

	Vec2I FontColorEffect::GetSizeExtend() const
	{
		return Vec2I();
	}

	bool FontColorEffect::IsEqual(const Ref<VectorFont::Effect>& other) const
	{
		if (!VectorFont::Effect::IsEqual(other))
			return false;

		Ref<FontColorEffect> otherEff = StaticCast<FontColorEffect>(other);
		return color == otherEff->color;
	}

	FontShadowEffect::FontShadowEffect(float blurRadius /*= 2.0f*/, const Vec2I offset /*= Vec2I(2, 2)*/, 
									   const Color4& color /*= Color4::Black()*/):
		blurRadius(blurRadius), offset(offset), color(color)
	{}

	void FontShadowEffect::Process(const Ref<Bitmap>& bitmap)
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

	bool FontShadowEffect::IsEqual(const Ref<VectorFont::Effect>& other) const
	{
		if (!VectorFont::Effect::IsEqual(other))
			return false;

		Ref<FontShadowEffect> otherEff = StaticCast<FontShadowEffect>(other);
		return Math::Equals(blurRadius, otherEff->blurRadius) && offset == otherEff->offset && 
			color == otherEff->color;
	}
}

DECLARE_CLASS(o2::FontStrokeEffect);

DECLARE_CLASS(o2::FontGradientEffect);

DECLARE_CLASS(o2::FontColorEffect);

DECLARE_CLASS(o2::FontShadowEffect);
