#ifndef BASIS_H
#define BASIS_H

#include "public.h"
#include "vector2.h"

OPEN_O2_NAMESPACE

/** Basis. Just as matrix 2x3. */
struct basis
{
	vec2f xv, yv, offs;

	inline basis()
	{ 
	}

	inline basis(const vec2f& offsvec, const vec2f& xvec = vec2f(1, 0), const vec2f& yvec = vec2f(0, 1))
	{
		xv = xvec; yv = yvec;
		offs = offsvec;
	}

	inline basis(const vec2f& offsvec, float angle)
	{
		offs = offsvec;
		float cs = cosf(angle), sn = sinf(angle);
		xv.set(cs, sn);
		yv.set(-sn, cs);
	}

	inline void set(const vec2f& offsvec = vec2f(0, 0), const vec2f& xvec = vec2f(1, 0), const vec2f& yvec = vec2f(0, 1))
	{
		xv = xvec; yv = yvec;
		offs = offsvec;
	}

	inline void set(const vec2f& offsvec, float angle)
	{
		offs = offsvec;
		float cs = cosf(angle), sn = sinf(angle);
		xv.set(cs, sn);
		yv.set(-sn, cs);
	}

	inline basis operator=(const basis& cbasis)
	{
		offs = cbasis.offs;
		xv = cbasis.xv;
		yv = cbasis.yv;
		return *this;
	}

	inline basis operator*(const basis& cbasis)
	{ 
		basis res;
		res.xv.x = xv.x*cbasis.xv.x + xv.y*cbasis.yv.x;                       res.xv.y = xv.x*cbasis.xv.y + xv.y*cbasis.yv.y;
		res.yv.x = yv.x*cbasis.xv.x + yv.y*cbasis.yv.x;                       res.yv.y = yv.x*cbasis.xv.y + yv.y*cbasis.yv.y;
		res.offs.x = offs.x*cbasis.xv.x + offs.y*cbasis.yv.x + cbasis.offs.x; res.offs.y = offs.x*cbasis.xv.y + offs.y*cbasis.yv.y + cbasis.offs.y;
		return res;
	}

	inline vec2f operator*(const vec2f& vec) const
	{
		vec2f ret;
		ret.x = xv.x*vec.x + xv.y*vec.y + offs.x;
		ret.y = yv.x*vec.x + yv.y*vec.y + offs.y;
		return ret;
	}

	inline float getAngle() const
	{
		float angle = -atan2f( -xv.y, xv.x );
		if (angle < 0)
			return 6.283185307f + angle;
		
		return angle;
	}

	inline vec2f getScale() const
	{
		return vec2f( xv.len(), yv.len() );
	}

	inline void decompose(vec2f* offset, float* angle, vec2f* scale)
	{
		*offset = offs;
		*angle = getAngle();
		*scale = getScale();
	}

	inline basis inverted() const
	{
		float invdet = 1.0f/(xv.x*yv.y - yv.x*xv.y);
		basis old = *this, res;   

		res.xv.x=     old.yv.y*invdet;
		res.yv.x=    -old.yv.x*invdet;
		res.offs.x=  (old.yv.x*old.offs.y - old.offs.x*old.yv.y)*invdet;
				    
		res.xv.y=    -old.xv.y*invdet;
		res.yv.y=     old.xv.x*invdet;
		res.offs.y= -(old.xv.x*old.offs.y - old.offs.x*old.xv.y)*invdet;
		
		return res;
	}

	inline void inverse()
	{
		*this = inverted();
	}

	inline void translate(const vec2f& voffs)
	{
		offs += voffs;
	}

	inline void scale(const vec2f& scalev)
	{
		xv *= scalev.x;
		yv *= scalev.y;
	}

	inline void rotate(float angle)
	{
		float cs = cosf(angle), sn = sinf(angle);
		
		vec2f nxv( cs*xv.x - sn*xv.y, sn*xv.x + cs*xv.y );
		vec2f nyv( cs*yv.x - sn*yv.y, sn*yv.x + cs*yv.y );

		xv = nxv; yv = nyv;
	}

	inline static basis nullBasis()
	{
		return basis(vec2f(0, 0), vec2f(1, 0), vec2f(0, 1));
	}

	inline static basis scaledBasis(const vec2f& scale)
	{
		return basis(vec2f(0, 0), vec2f(scale.x, 0), vec2f(0, scale.y));
	}

	inline static basis translatedBasis(const vec2f& voffs)
	{
		return basis(voffs, vec2f(1, 0), vec2f(0, 1));
	}

	inline static basis rotatedBasis(float angle)
	{
		float cs = cosf(angle), sn = sinf(angle);
		basis(vec2f(0, 0), vec2f(cs, sn), vec2f(-sn, cs));
	}
};

CLOSE_O2_NAMESPACE

#endif //MATRIX3_H