#pragma once

#include "Utils/Math/Vector2.h"
#include "Utils/Math/Rect.h"

namespace o2
{
	// -------------------------
	// Basis. Just as matrix 2x3
	// -------------------------
	struct Basis
	{
		Vec2F xv, yv, offs;

		inline Basis();
		inline Basis(const Vec2F& offsvec, const Vec2F& xvec = Vec2F(1, 0), const Vec2F& yvec = Vec2F(0, 1));
		inline Basis(const Vec2F& offsvec, float angle);

		inline bool operator==(const Basis& cbasis);
		inline bool operator!=(const Basis& cbasis);

		inline Basis operator*(const Basis& cbasis);
		inline Vec2F operator*(const Vec2F& vec) const;

		inline void Set(const Vec2F& offsvec = Vec2F(0, 0), const Vec2F& xvec = Vec2F(1, 0), const Vec2F& yvec = Vec2F(0, 1));
		inline void Set(const Vec2F& offsvec, float angle);

		inline float GetAngle() const;
		inline Vec2F GetScale() const;
		inline float GetShift() const;
		inline float GetShiftFast(const Vec2F& scale) const;

		inline void Decompose(Vec2F* offset, float* angle, Vec2F* scale, float* shift) const;

		inline Basis Inverted() const;
		inline void  Inverse();

		inline void Translate(const Vec2F& voffs);
		inline void Scale(const Vec2F& scalev);
		inline void Rotate(float angle);

		inline void  Transform(float& x, float& y) const;
		inline Vec2F Transform(const Vec2F& vec) const;

		inline RectF AABB() const;

		inline static Basis Identity();
		inline static Basis Scaled(const Vec2F& scale);
		inline static Basis Translated(const Vec2F& voffs);
		inline static Basis Rotated(float angle);
		inline static Basis Build(const Vec2F& position, const Vec2F& scale, float angle, float shift);
	};

	struct BasisDef
	{
		Vec2F mPosition;
		Vec2F mScale;
		float mAngle;
		float mShift;

	public:
		inline BasisDef(const Vec2F& position = Vec2F(), const Vec2F& scale = Vec2F(1, 1), float angle = 0, float shift = 0);
		inline BasisDef(const Basis& bas);

		inline operator Basis() const;

		inline Basis Build() const;
	};

	inline BasisDef Basis2Def(const Basis& bas);
	inline Basis    Def2Basis(const BasisDef& def);

	Basis::Basis():
		xv(1, 0), yv(0, 1), offs()
	{
	}

	Basis::Basis(const Vec2F& offsvec, const Vec2F& xvec /*= vec2f(1, 0)*/, const Vec2F& yvec /*= vec2f(0, 1)*/):
		xv(xvec), yv(yvec), offs(offsvec)
	{
	}

	Basis::Basis(const Vec2F& offsvec, float angle)
	{
		offs = offsvec;
		float cs = cosf(angle), sn = sinf(angle);
		xv.Set(cs, sn);
		yv.Set(-sn, cs);
	}

	bool Basis::operator==(const Basis& cbasis)
	{
		return xv == cbasis.xv && yv == cbasis.yv && offs == cbasis.offs;
	}

	bool Basis::operator!=(const Basis& cbasis)
	{
		return xv != cbasis.xv || yv != cbasis.yv || offs != cbasis.offs;
	}

	Basis Basis::operator*(const Basis& cbasis)
	{
		Basis res;
		res.xv.x = xv.x*cbasis.xv.x + xv.y*cbasis.yv.x;                       res.xv.y = xv.x*cbasis.xv.y + xv.y*cbasis.yv.y;
		res.yv.x = yv.x*cbasis.xv.x + yv.y*cbasis.yv.x;                       res.yv.y = yv.x*cbasis.xv.y + yv.y*cbasis.yv.y;
		res.offs.x = offs.x*cbasis.xv.x + offs.y*cbasis.yv.x + cbasis.offs.x; res.offs.y = offs.x*cbasis.xv.y + offs.y*cbasis.yv.y + cbasis.offs.y;
		return res;
	}

	Vec2F Basis::operator*(const Vec2F& vec) const
	{
		Vec2F ret;
		ret.x = xv.x*vec.x + yv.x*vec.y + offs.x;
		ret.y = xv.y*vec.x + yv.y*vec.y + offs.y;
		return ret;
	}

	void Basis::Set(const Vec2F& offsvec /*= vec2f(0, 0)*/, const Vec2F& xvec /*= vec2f(1, 0)*/, const Vec2F& yvec /*= vec2f(0, 1)*/)
	{
		xv = xvec; yv = yvec;
		offs = offsvec;
	}

	void Basis::Set(const Vec2F& offsvec, float angle)
	{
		offs = offsvec;
		float cs = cosf(angle), sn = sinf(angle);
		xv.Set(cs, sn);
		yv.Set(-sn, cs);
	}

	float Basis::GetAngle() const
	{
		float angle = -atan2f(-xv.y, xv.x);
		if (angle < 0)
			return 6.283185307f + angle;

		return angle;
	}

	Vec2F Basis::GetScale() const
	{
		return Vec2F(xv.Length(), yv.Length());
	}

	float Basis::GetShift() const
	{
		Vec2F scale = GetScale();
		return GetShiftFast(scale);
	}

	float Basis::GetShiftFast(const Vec2F& scale) const
	{
		float proj = yv.Dot(xv/scale.x);
		return proj/scale.y;
	}

	void Basis::Decompose(Vec2F* offset, float* angle, Vec2F* scale, float* shift) const
	{
		*offset = offs;
		*angle = GetAngle();
		*scale = GetScale();
		*shift = GetShiftFast(*scale);
	}

	Basis Basis::Inverted() const
	{
		float invdet = 1.0f/(xv.x*yv.y - yv.x*xv.y);
		Basis res;

		res.xv.x=     yv.y*invdet;
		res.yv.x=    -yv.x*invdet;
		res.offs.x=  (yv.x*offs.y - offs.x*yv.y)*invdet;

		res.xv.y=    -xv.y*invdet;
		res.yv.y=     xv.x*invdet;
		res.offs.y= -(xv.x*offs.y - offs.x*xv.y)*invdet;

		return res;
	}

	void Basis::Inverse()
	{
		*this = Inverted();
	}

	void Basis::Translate(const Vec2F& voffs)
	{
		offs += voffs;
	}

	void Basis::Scale(const Vec2F& scalev)
	{
		xv *= scalev.x;
		yv *= scalev.y;
	}

	void Basis::Rotate(float angle)
	{
		float cs = cosf(angle), sn = sinf(angle);

		Vec2F nxv(cs*xv.x - sn*xv.y, sn*xv.x + cs*xv.y);
		Vec2F nyv(cs*yv.x - sn*yv.y, sn*yv.x + cs*yv.y);

		xv = nxv; yv = nyv;
	}

	void Basis::Transform(float& x, float& y) const
	{
		float lx = x, ly = y;
		x = xv.x*lx + yv.x*ly + offs.x;
		y = xv.y*lx + yv.y*ly + offs.y;
	}

	Vec2F Basis::Transform(const Vec2F& vec) const
	{
		return Vec2F(xv.x*vec.x + yv.x*vec.y + offs.x, xv.y*vec.x + yv.y*vec.y + offs.y);
	}

	RectF Basis::AABB() const
	{
		Vec2F points[4] =
		{
			offs, offs + xv, offs + yv, offs + xv + yv
		};

		return RectF::Bound(points, 4);
	}

	Basis Basis::Identity()
	{
		return Basis(Vec2F(0, 0), Vec2F(1, 0), Vec2F(0, 1));
	}

	Basis Basis::Scaled(const Vec2F& scale)
	{
		return Basis(Vec2F(0, 0), Vec2F(scale.x, 0), Vec2F(0, scale.y));
	}

	Basis Basis::Translated(const Vec2F& voffs)
	{
		return Basis(voffs, Vec2F(1, 0), Vec2F(0, 1));
	}

	Basis Basis::Rotated(float angle)
	{
		float cs = cosf(angle), sn = sinf(angle);
		return Basis(Vec2F(0, 0), Vec2F(cs, sn), Vec2F(-sn, cs));
	}

	Basis Basis::Build(const Vec2F& position, const Vec2F& scale, float angle, float shift)
	{
		float sn = sinf(angle), cs = cosf(angle), sshift = Math::Sqrt(1.0f - shift*shift);
		Vec2F x(scale.x*cs, sn*scale.x), y(-sn*scale.y, cs*scale.y);
		y = y.Rotate(sshift, -shift);
		return Basis(position, x, y);
	}


	BasisDef::BasisDef(const Vec2F& position /*= vec2f()*/, const Vec2F& scale /*= vec2f(1, 1)*/, float angle /*= 0*/,
					   float shift /*= 0*/):
		mPosition(position), mScale(scale), mAngle(angle), mShift(shift)
	{
	}

	BasisDef::BasisDef(const Basis& bas)
	{
		bas.Decompose(&mPosition, &mAngle, &mScale, &mShift);
	}

	BasisDef::operator Basis() const
	{
		return Build();
	}

	Basis BasisDef::Build() const
	{
		return Basis::Build(mPosition, mScale, mAngle, mShift);
	}


	BasisDef Basis2Def(const Basis& bas)
	{
		return BasisDef(bas);
	}

	Basis Def2Basis(const BasisDef& def)
	{
		return def.Build();
	}
}
