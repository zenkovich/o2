#pragma once

#include "o2/Utils/Math/Math.h"
#include "o2/Utils/Math/Vector2.h"

namespace o2
{
    template<typename T>
    inline T Bezier(const T& a, const T& b, const T& c, const T&d, float coef)
    {
        float m = 1 - coef;
        float n = m*m;
        float o = n*m;
        float p = coef*coef;
        float r = p*coef;

        return a*o + b*3.0f*coef*n + c*3.0f*p*m + d*r;
    }

    inline float SPow(float v)
    {
        return v < 0 ? Math::Pow(-v, 1.0f/3.0f) : Math::Pow(v, 1.0f/3.0f);
    }

    inline float SinErp(float begin, float end, float coef)
    {
        return begin + (end - begin) * Math::Sin(coef * Math::PI() * 0.5f);
    }

    inline float SolveBezier(float p0, float p1, float p2, float p3, float x)
    {
        p0 -= x;
        p1 -= x;
        p2 -= x;
        p3 -= x;

        float p2x3 = p2*3.0f;
        float p1x3 = p1*3.0f;
        float p0x3 = p0*3.0f;

        float a = p3 - p2x3 + p1x3 - p0;
        float b = p2x3 - 6.0f * p1 + p0x3;
        float c = p1x3 - p0x3;
        float d = p0;

        float inva = 1.0f/a;

        b *= inva;
        c *= inva;
        d *= inva;

        float sqb = b*b;

        float p = (3.0f * c - sqb) / 3.0f;
        float q = (2.0f * b * sqb - 9.0f * b * c + 27.0f * d) / 27.0f;

        float hq = q*0.5f;
        float tp = p*0.3333333333f;

        float discriminant = hq*hq + tp*tp*tp;

        if (discriminant > 0)
        {
            float sqrtd = Math::Sqrt(discriminant);
            return SPow(-hq + sqrtd) - SPow(hq + sqrtd) - b*0.333333f;
        }
        else
        {
            float itp = -tp;

            float r = Math::Sqrt(itp*itp*itp);
            float phi = Math::ACos(-(q / (2.0f * r)));
            float s = 2.0f * Math::Pow(r, 0.3333333333f);

            float rs = s * Math::Cos(phi*0.333333f + 4.188790204786390f) - b*0.333333f;

            if (rs < 0.0f || rs > 1.0f)
                rs = s * Math::Cos(phi*0.333333f) - b*0.333333f;

            if (rs < 0.0f || rs > 1.0f)
                rs = s * Math::Cos(phi*0.333333f + 2.0943951023931f) - b*0.333333f;

            return rs;
        }

        return 0.0f;
    }

    inline float YBezier(const Vec2F& a, const Vec2F& b, const Vec2F& c, const Vec2F& d, float x)
    {
        return Bezier(a, b, c, d, SolveBezier(a.x, b.x, c.x, d.x, x)).y;
    }
}
