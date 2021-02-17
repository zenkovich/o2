#include "o2/stdafx.h"
#include "o2/Utils/Math/Math.h"

#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Math/Rect.h"

namespace o2
{
	namespace Math
	{

		Vec2F Round(const Vec2F& value)
		{
			return Vec2F(Round(value.x), Round(value.y));
		}

		RectF Round(const RectF& value)
		{
			return RectF(Round(value.left), Round(value.top), Round(value.right), Round(value.bottom));
		}

		RectF Lerp(const RectF& a, const RectF& b, float coef)
		{
			return RectF(Lerp(a.left, b.left, coef), Lerp(a.top, b.top, coef),
						 Lerp(a.right, b.right, coef), Lerp(a.bottom, b.bottom, coef));
		}

		bool Lerp(bool a, bool b, float coef)
		{
			return a;
		}

		bool Lerp(int a, int b, float coef)
		{
			return (int)((float)(b - a)*coef) + a;
		}

		void OrthoProjMatrix(float* mat, float left, float right, float bottom, float top, float nearz, float farz)
		{
			float tx = -(right + left)/(right - left);
			float ty = -(top + bottom)/(top - bottom);
			float tz = -(farz + nearz)/(farz - nearz);

			mat[0] = 2.0f/(right - left); mat[4] = 0.0f;                mat[8] = 0.0f;                  mat[12] = tx;
			mat[1] = 0.0f;                mat[5] = 2.0f/(top - bottom); mat[9] = 0.0f;                  mat[13] = ty;
			mat[2] = 0.0f;                mat[6] = 0.0f;                mat[10] = -2.0f/(farz - nearz); mat[14] = tz;
			mat[3] = 0.0f;                mat[7] = 0.0f;                mat[11] = 0.0f;                 mat[15] = 1.0f;
		}

		Vec2F CalculateEllipseTangent(const Vec2F& begin, const Vec2F& middle, const Vec2F& end)
		{
			Vec2F axis = end - begin;
			float axisLength = axis.Length();
			Vec2F axisNorm = axis/axisLength;
			Vec2F axisNormPerpendicular = axisNorm.Perpendicular();

			Vec2F centerToMiddle = middle - (begin + end)*0.5f;

			float xProjection = axisNorm.Dot(centerToMiddle + (begin + end)*0.5f - begin);
			float xProjectionFromCenter = xProjection - axisLength*0.5f;

			float ssq = axisLength*axisLength*0.25f - xProjectionFromCenter*xProjectionFromCenter;
			float circleYProjection = ssq > 0 ? Math::Sqrt(ssq) : 0.00001f;

			Vec2F circlePoint = axisNorm*xProjection + axisNormPerpendicular*circleYProjection;
			Vec2F centerToCirclePoint = circlePoint - (axis*0.5f);
			Vec2F circleTangent = centerToCirclePoint.Perpendicular().Inverted().Normalized();
			float yc = axisNormPerpendicular.Dot(centerToMiddle);

			Vec2F decomposedCircleTangent(axisNorm.Dot(circleTangent), axisNormPerpendicular.Dot(circleTangent));
			float circleToEllipseScale = (yc/circleYProjection);
			Vec2F ellipseTangent = (axisNorm*decomposedCircleTangent.x + axisNormPerpendicular*decomposedCircleTangent.y*circleToEllipseScale).Normalized();

			return ellipseTangent;
		}
	}
} 
