#include "Utils/Math/Math.h"

#include "Utils/Math/Vector2.h"

namespace o2
{
	namespace Math
	{
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
	}
} 