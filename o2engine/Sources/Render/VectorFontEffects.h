#pragma once

#include "Render/VectorFont.h"

namespace o2
{
	// ------------------
	// Font stroke effect
	// ------------------
	class FontStrokeEffect: public VectorFont::Effect
	{
	public:
		float  radius;         // Stroke radius
		int    alphaThreshold; // Stroke alpha threshold
		Color4 color;          // Color

		// Constructor
		FontStrokeEffect(float radius = 1.0f, const Color4& color = Color4(0, 0, 0, 150), int alphaThreshold = 100);

		// Process bitmap with glyph
		void Process(Ptr<Bitmap> bitmap);

		// Returns bitmap extending size
		Vec2I GetSizeExtend() const;

		IOBJECT(FontStrokeEffect)
		{
			BASE_CLASS(Effect);
			SRLZ_FIELD(radius);
		}

		SERIALIZABLE_IMPL(FontStrokeEffect);
	};

	// -----------------------
	// Gradient filling effect
	// -----------------------
	class FontGradientEffect: public VectorFont::Effect
	{
	public:
		Color4 color1; // First color
		Color4 color2; // Second color
		float  angle;  // Angle in degrees
		float  length; // Length. 0 means maximum size
		Vec2F  origin; // Relative origin
		
		// Constructor
		FontGradientEffect(const Color4& color1 = Color4(200, 200, 200, 255),
						   const Color4& color2 = Color4(255, 255, 255, 255),
						   float angle = 0, float length = 0, Vec2F origin = Vec2F());

		// Process bitmap with glyph
		void Process(Ptr<Bitmap> bitmap);

		// Returns bitmap extending size
		Vec2I GetSizeExtend() const;

		IOBJECT(FontGradientEffect)
		{
			BASE_CLASS(Effect);
			SRLZ_FIELD(color1);
			SRLZ_FIELD(color2);
			SRLZ_FIELD(angle);
			SRLZ_FIELD(length);
			SRLZ_FIELD(origin);
		}

		SERIALIZABLE_IMPL(FontGradientEffect);
	};

	class FontColorEffect: public VectorFont::Effect
	{
	public:
		Color4 color; 

		// Constructor
		FontColorEffect(const Color4& color = Color4::White());

		// Process bitmap with glyph
		void Process(Ptr<Bitmap> bitmap);

		// Returns bitmap extending size
		Vec2I GetSizeExtend() const;

		IOBJECT(FontColorEffect)
		{
			BASE_CLASS(Effect);
			SRLZ_FIELD(color);
		}

		SERIALIZABLE_IMPL(FontColorEffect);
	};

	class FontShadowEffect: public VectorFont::Effect
	{
	public:
		float  blurRadius; // Bluring radius
		Vec2I  offset;     // Offset
		Color4 color;      // Shadow color

		// Constructor
		FontShadowEffect(float blurRadius = 2.0f, const Vec2I offset = Vec2I(2, 2), 
						 const Color4& color = Color4(0, 0, 0, 100));

		// Process bitmap with glyph
		void Process(Ptr<Bitmap> bitmap);

		// Returns bitmap extending size
		Vec2I GetSizeExtend() const;

		IOBJECT(FontShadowEffect)
		{
			BASE_CLASS(Effect);
			SRLZ_FIELD(blurRadius);
			SRLZ_FIELD(offset);
			SRLZ_FIELD(color);
		}

		SERIALIZABLE_IMPL(FontShadowEffect);
	};
}