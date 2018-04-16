#include "stdafx.h"
#include "Geometry.h"

#include "Utils/Math/Intersection.h"

namespace o2
{
	void Geometry::CreatePolyLineMesh(const Vertex2* points, int pointsCount,
									  Vertex2*& verticies, int& vertexCount, int& vertexSize,
									  UInt16*& indexes, int& polyCount, int& polySize,
									  float width, float texBorderTop, float texBorderBottom, const Vec2F& texSize)
	{
		int newVertexCount = pointsCount*4;
		int newPolyCount = (pointsCount - 1)*6;

		if (newVertexCount > vertexSize)
		{
			if (verticies)
				delete[] verticies;

			verticies = new Vertex2[newVertexCount];
			vertexSize = newVertexCount;
		}

		if (newPolyCount > polySize)
		{
			if (indexes)
				delete[] indexes;

			indexes = new UInt16[newPolyCount*3];
			polySize = newPolyCount;
		}

		float halfWidth = width*0.5f;
		float halfWidhtBorderTop = halfWidth + texBorderTop;
		float halfWidhtBorderBottom = halfWidth + texBorderBottom;

		Vec2F invTexSize(1.0f/texSize.x, 1.0f/texSize.y);

		float upTexV = 1.0f - texBorderTop*invTexSize.y;
		float downTexV = texBorderBottom*invTexSize.y;

		int vertex = 0; int poly = 0;
		float length = 0;
		for (int i = 0; i < pointsCount; i++)
		{
			Vec2F point = points[i];
			ULong color = points[i].color;

			if (i == 0)
			{
				Vec2F dir = ((Vec2F)points[i + 1] - point).Normalized();
				Vec2F norm = dir.Perpendicular();

				verticies[vertex++].Set(point + norm*halfWidth, color, 0, upTexV);
				verticies[vertex++].Set(point + norm*halfWidhtBorderTop, color, 0, 1);
				verticies[vertex++].Set(point - norm*halfWidth, color, 0, downTexV);
				verticies[vertex++].Set(point - norm*halfWidhtBorderBottom, color, 0, 0);
			}
			else if (i == pointsCount - 1)
			{
				Vec2F dir = point - points[i - 1];
				float segLength = dir.Length();
				dir /= segLength;
				Vec2F norm = dir.Perpendicular();

				length += segLength;
				float u = length*invTexSize.x;

				verticies[vertex++].Set(point + norm*halfWidth, color, u, upTexV);
				verticies[vertex++].Set(point + norm*halfWidhtBorderBottom, color, u, 1);
				verticies[vertex++].Set(point - norm*halfWidth, color, u, downTexV);
				verticies[vertex++].Set(point - norm*halfWidhtBorderTop, color, u, 0);
			}
			else
			{
				Vec2F prev = points[i - 1];
				Vec2F next = points[i + 1];

				Vec2F prevDir = point - prev;
				float segLength = prevDir.Length();
				prevDir /= segLength;

				Vec2F nextDir = (point - next).Normalized();

				Vec2F prevNorm = prevDir.Perpendicular().Inverted();
				Vec2F nextNorm = nextDir.Perpendicular();

				length += segLength;
				float u = length*invTexSize.x;

				verticies[vertex++].Set(Intersection::Lines(point - prevNorm*halfWidth, prevDir,
										point - nextNorm*halfWidth, nextDir), color, u, upTexV);

				verticies[vertex++].Set(Intersection::Lines(point - prevNorm*halfWidhtBorderTop, prevDir,
										point - nextNorm*halfWidhtBorderTop, nextDir), color, u, 1);

				verticies[vertex++].Set(Intersection::Lines(point + prevNorm*halfWidth, prevDir,
										point + nextNorm*halfWidth, nextDir), color, u, downTexV);

				verticies[vertex++].Set(Intersection::Lines(point + prevNorm*halfWidhtBorderBottom, prevDir,
										point + nextNorm*halfWidhtBorderBottom, nextDir), color, u, 0.0f);
			}

#define POLYGON(A, B, C) \
    indexes[poly] = vertex - A - 1; indexes[poly + 1] = vertex - B - 1; indexes[poly + 2] = vertex - C - 1; poly += 3;

			if (i > 0)
			{
				POLYGON(6, 2, 7);
				POLYGON(7, 2, 3);
				POLYGON(5, 7, 3);
				POLYGON(5, 3, 1);
				POLYGON(4, 5, 1);
				POLYGON(4, 1, 0);
			}
		}

		vertexCount = vertex;
		polyCount = poly/3;
	}
}