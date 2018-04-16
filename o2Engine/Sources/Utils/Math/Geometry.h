#pragma once

#include "Utils/Math/Border.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Types/CommonTypes.h"

namespace o2
{
	namespace Geometry
	{
		void CreatePolyLineMesh(const Vertex2* points, int pointsCount, 
								Vertex2*& verticies, int& vertexCount, int& vertexSize,
								UInt16*& indexes, int& polyCount, int& polySize,
								float width, float texBorderTop, float texBorderBottom, const Vec2F& texSize);
	}
}
