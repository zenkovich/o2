#pragma once

#include "Utils/Math/Border.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Types/CommonTypes.h"

namespace o2
{
	namespace Geometry
	{
		void CreatePolyLineMesh(const Vertex2* points, int pointsCount, 
								Vertex2*& verticies, UInt& vertexCount, UInt& vertexSize,
								UInt16*& indexes, UInt& polyCount, UInt& polySize,
								float width, float texBorderTop, float texBorderBottom, const Vec2F& texSize,
								const Vec2F& invCameraScale = Vec2F(1, 1));
	}
}
