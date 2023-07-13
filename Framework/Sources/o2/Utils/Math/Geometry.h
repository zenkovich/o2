#pragma once

#include "o2/Utils/Math/Border.h"
#include "o2/Utils/Math/Vertex.h"
#include "o2/Utils/Types/CommonTypes.h"

namespace o2
{
	namespace Geometry
	{
		// Function that creates mesh from points
		void CreatePolyLineMesh(const Vertex* points, int pointsCount, 
								Vertex*& verticies, UInt& vertexCount, UInt& vertexSize,
								VertexIndex*& indexes, UInt& polyCount, UInt& polySize,
								float width, float texBorderTop, float texBorderBottom, const Vec2F& texSize,
								const Vec2F& invCameraScale = Vec2F(1, 1));

		// Function that clips triangle by line
		int ClipTriangleByLine(const Vertex& a, const Vertex& b, const Vertex& c,
							   const Vec2F& lineBegin, const Vec2F& lineEnd,
							   Vertex* output);

		// Function that clips triangle by rectangle
		int ClipTriangleByRectangle(const Vertex& a, const Vertex& b, const Vertex& c,
									Vertex* output,
									const RectF& clippRect);
	}
}
