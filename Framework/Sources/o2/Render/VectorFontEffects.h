#pragma once

#include "o2/Render/VectorFont.h"

namespace o2
{
	// ------------------
	// Font stroke effect
	// ------------------
	class FontStrokeEffect: public VectorFont::Effect
	{
	public:
		float  radius;         // Stroke radius @SERIALIZABLE
		int    alphaThreshold; // Stroke alpha threshold @SERIALIZABLE
		Color4 color;          // Color @SERIALIZABLE

	public:
		// Constructor
		FontStrokeEffect(float radius = 1.0f, const Color4& color = Color4(0, 0, 0, 150), int alphaThreshold = 100);

		// Process bitmap with glyph
		void Process(Bitmap* bitmap);

		// Returns bitmap extending size
		Vec2I GetSizeExtend() const;

		// Check effects equals
		bool IsEqual(VectorFont::Effect* other) const;

		SERIALIZABLE(FontStrokeEffect);
	};

	// -----------------------
	// Gradient filling effect
	// -----------------------
	class FontGradientEffect: public VectorFont::Effect
	{
	public:
		Color4 color1; // First color @SERIALIZABLE
		Color4 color2; // Second color @SERIALIZABLE
		float  angle;  // Angle in degrees @SERIALIZABLE
		float  length; // Length. 0 means maximum size @SERIALIZABLE
		Vec2F  origin; // Relative origin @SERIALIZABLE
		
	public:
		// Constructor
		FontGradientEffect(const Color4& color1 = Color4(200, 200, 200, 255),
						   const Color4& color2 = Color4(255, 255, 255, 255),
						   float angle = 0, float length = 0, Vec2F origin = Vec2F());

		// Process bitmap with glyph
		void Process(Bitmap* bitmap);

		// Returns bitmap extending size
		Vec2I GetSizeExtend() const;

		// Check effects equals
		bool IsEqual(VectorFont::Effect* other) const;

		SERIALIZABLE(FontGradientEffect);
	};

	class FontColorEffect: public VectorFont::Effect
	{
	public:
		Color4 color; // @SERIALIZABLE

	public:
		// Constructor
		FontColorEffect(const Color4& color = Color4::White());

		// Process bitmap with glyph
		void Process(Bitmap* bitmap);

		// Returns bitmap extending size
		Vec2I GetSizeExtend() const;

		// Check effects equals
		bool IsEqual(VectorFont::Effect* other) const;

		SERIALIZABLE(FontColorEffect);
	};

	class FontShadowEffect: public VectorFont::Effect
	{
	public:
		float  blurRadius; // Bluring radius @SERIALIZABLE
		Vec2I  offset;     // Offset @SERIALIZABLE
		Color4 color;      // Shadow color @SERIALIZABLE

	public:
		// Constructor
		FontShadowEffect(float blurRadius = 2.0f, const Vec2I offset = Vec2I(2, 2), 
						 const Color4& color = Color4(0, 0, 0, 100));

		// Process bitmap with glyph
		void Process(Bitmap* bitmap);

		// Returns bitmap extending size
		Vec2I GetSizeExtend() const;

		// Check effects equals
		bool IsEqual(VectorFont::Effect* other) const;

		SERIALIZABLE(FontShadowEffect);
	};
}

CLASS_BASES_META(o2::FontStrokeEffect)
{
	BASE_CLASS(o2::VectorFont::Effect);
}
END_META;
CLASS_FIELDS_META(o2::FontStrokeEffect)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(radius).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(alphaThreshold).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(color).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::FontStrokeEffect)
{

	PUBLIC_FUNCTION(void, Process, Bitmap*);
	PUBLIC_FUNCTION(Vec2I, GetSizeExtend);
	PUBLIC_FUNCTION(bool, IsEqual, VectorFont::Effect*);
}
END_META;

CLASS_BASES_META(o2::FontGradientEffect)
{
	BASE_CLASS(o2::VectorFont::Effect);
}
END_META;
CLASS_FIELDS_META(o2::FontGradientEffect)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(color1).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(color2).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(angle).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(length).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(origin).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::FontGradientEffect)
{

	PUBLIC_FUNCTION(void, Process, Bitmap*);
	PUBLIC_FUNCTION(Vec2I, GetSizeExtend);
	PUBLIC_FUNCTION(bool, IsEqual, VectorFont::Effect*);
}
END_META;

CLASS_BASES_META(o2::FontColorEffect)
{
	BASE_CLASS(o2::VectorFont::Effect);
}
END_META;
CLASS_FIELDS_META(o2::FontColorEffect)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(color).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::FontColorEffect)
{

	PUBLIC_FUNCTION(void, Process, Bitmap*);
	PUBLIC_FUNCTION(Vec2I, GetSizeExtend);
	PUBLIC_FUNCTION(bool, IsEqual, VectorFont::Effect*);
}
END_META;

CLASS_BASES_META(o2::FontShadowEffect)
{
	BASE_CLASS(o2::VectorFont::Effect);
}
END_META;
CLASS_FIELDS_META(o2::FontShadowEffect)
{
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(blurRadius).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(offset).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(color).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::FontShadowEffect)
{

	PUBLIC_FUNCTION(void, Process, Bitmap*);
	PUBLIC_FUNCTION(Vec2I, GetSizeExtend);
	PUBLIC_FUNCTION(bool, IsEqual, VectorFont::Effect*);
}
END_META;
