#pragma once

#include "o2/Utils/Types/CommonTypes.h"
#include "o2/Utils/Math/Math.h"
#include "Box2D/Common/b2Draw.h"

namespace o2
{
	class DataValue;

	class Color4
	{
	public:
		int r;
		int g;
		int b;
		int a;

		Color4();
		Color4(int r, int g, int b, int a = 255);
		Color4(float r, float g, float b, float a = 1.0f);
		Color4(const b2Color& color);

		operator b2Color() const;
		
		bool operator==(const Color4& color) const;
		bool operator!=(const Color4& color) const;

		Color4 operator+(const Color4& v) const;
		Color4 operator+=(const Color4& v);

		Color4 operator-(const Color4& v) const;
		Color4 operator-=(const Color4& v);

		Color4 operator*(float v) const;
		Color4 operator*=(float v);

		Color4 operator/(float v) const;
		Color4 operator/=(float v);

		Color4 operator*(const Color4& v) const;
		Color4 operator*=(const Color4& v);

		Color4 operator/(const Color4& v) const;
		Color4 operator/=(const Color4& v);

		float RF() const;
		float GF() const;
		float BF() const;
		float AF() const;

		void SetRF(float value);
		void SetGF(float value);
		void SetBF(float value);
		void SetAF(float value);

		float GetF(int i) const;
		int&  Get(int i) const;

		void   Normalize();
		Color4 Normalized() const;

		ULong ARGB() const;
		ULong ABGR() const;
		ULong RGBA() const;

		void SetARGB(ULong color);
		void SetABGR(ULong color);
		void SetRGBA(ULong color);

		void SetHSL(float hue, float saturation, float lightness);
		void ToHSL(float& hue, float& saturation, float& lightness);

		Color4& ChangeHue(float offset);
		Color4& ChangeSaturation(float offset);
		Color4& ChangeLightness(float offset);

		Color4 BlendByAlpha(const Color4& other);

		static Color4 White();
		static Color4 Black();
		static Color4 Red();
		static Color4 Blue();
		static Color4 Green();
		static Color4 Gray();
		static Color4 SomeColor(int idx);
	};
}
