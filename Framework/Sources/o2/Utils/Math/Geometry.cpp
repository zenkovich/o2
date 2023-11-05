#include "o2/stdafx.h"
#include "Geometry.h"

#include "o2/Utils/Math/Intersection.h"

namespace o2
{
    void Geometry::CreatePolyLineMesh(const Vertex* points, int pointsCount,
                                      Vertex*& verticies, UInt& vertexCount, UInt& vertexSize,
                                      VertexIndex*& indexes, UInt& polyCount, UInt& polySize,
                                      float width, float texBorderTop, float texBorderBottom, const Vec2F& texSize,
                                      const Vec2F& invCameraScale /*= Vec2F(1, 1)*/)
    {
        int newVertexCount = pointsCount*4;
        int newPolyCount = (pointsCount - 1)*6;

        if (newVertexCount > (int)vertexSize)
        {
            if (verticies)
                delete[] verticies;

            verticies = new Vertex[newVertexCount];
            vertexSize = newVertexCount;
        }

        if (newPolyCount > (int)polySize)
        {
            if (indexes)
                delete[] indexes;

            indexes = new VertexIndex[newPolyCount*3];
            polySize = newPolyCount;
        }

        float halfWidth = width*0.5f;
        float halfWidhtBorderTop = halfWidth + texBorderTop;
        float halfWidhtBorderBottom = halfWidth + texBorderBottom;

        Vec2F invTexSize(1.0f/texSize.x, 1.0f/texSize.y);

        float upTexV = 1.0f - texBorderTop*invTexSize.y;
        float downTexV = texBorderBottom*invTexSize.y;

        Vec2F offs = Vec2F(0.5f, 0.5f)*invCameraScale;

        int vertex = 0; int poly = 0;
        float u = 0;

        for (int i = 0; i < pointsCount; i++)
        {
            Vec2F point = (Vec2F)points[i] + offs;
            ULong color = points[i].color;
            ULong zeroAlphaColor = color << 8 >> 8;

            if (i == 0)
            {
                Vec2F dir = ((Vec2F)points[i + 1] + offs - point).Normalized();
                Vec2F norm = dir.Perpendicular();

                Vec2F upBorder = point + norm*invCameraScale*halfWidhtBorderBottom;
                Vec2F up = point + norm*invCameraScale*halfWidth;
                Vec2F down = point - norm*invCameraScale*halfWidth;
                Vec2F downBorder = point - norm*invCameraScale*halfWidhtBorderTop;

                verticies[vertex++].Set(up, color, 0, upTexV);
                verticies[vertex++].Set(upBorder, zeroAlphaColor, 0, 1);
                verticies[vertex++].Set(down, color, 0, downTexV);
                verticies[vertex++].Set(downBorder, zeroAlphaColor, 0, 0);
            }
            else if (i == pointsCount - 1)
            {
                Vec2F dir = point - points[i - 1] - offs;
                float segLength = dir.Length();
                dir /= segLength;
                Vec2F norm = dir.Perpendicular();

                Vec2F upBorder = point + norm*invCameraScale*halfWidhtBorderBottom;
                Vec2F up = point + norm*invCameraScale*halfWidth;
                Vec2F down = point - norm*invCameraScale*halfWidth;
                Vec2F downBorder = point - norm*invCameraScale*halfWidhtBorderTop;

                if (i%2 == 0)
                    u -= segLength/invCameraScale.x*invTexSize.x;
                else
                    u += segLength/invCameraScale.x*invTexSize.x;

                verticies[vertex++].Set(up, color, u, upTexV);
                verticies[vertex++].Set(upBorder, zeroAlphaColor, u, 1);
                verticies[vertex++].Set(down, color, u, downTexV);
                verticies[vertex++].Set(downBorder, zeroAlphaColor, u, 0);
            }
            else
            {
                Vec2F prev = (Vec2F)points[i - 1] + offs;
                Vec2F next = (Vec2F)points[i + 1] + offs;

                Vec2F prevDir = point - prev;
                float segLength = prevDir.Length();
                prevDir /= segLength;

                Vec2F nextDir = (point - next).Normalized();

                Vec2F prevNorm = prevDir.Perpendicular().Inverted();
                Vec2F nextNorm = nextDir.Perpendicular();

                Vec2F upBorder = Intersection::LinesNoChek(point - prevNorm*halfWidhtBorderTop, prevDir,
                                                           point - nextNorm*halfWidhtBorderTop, nextDir);


                Vec2F up = Intersection::LinesNoChek(point - prevNorm*halfWidth, prevDir,
                                                     point - nextNorm*halfWidth, nextDir);

                Vec2F down = Intersection::LinesNoChek(point + prevNorm*halfWidth, prevDir,
                                                       point + nextNorm*halfWidth, nextDir);

                Vec2F downBorder = Intersection::LinesNoChek(point + prevNorm*halfWidhtBorderBottom, prevDir,
                                                             point + nextNorm*halfWidhtBorderBottom, nextDir);

                up = (up - point)*invCameraScale + point;
                down = (down - point)*invCameraScale + point;
                upBorder = (upBorder - point)*invCameraScale + point;
                downBorder = (downBorder - point)*invCameraScale + point;

                float segSign = i%2 == 0 ? -1.0f : 1.0f;

                u += segLength/invCameraScale.x*invTexSize.x*segSign;

                verticies[vertex++].Set(up, color, u + prevDir.Dot(up - point)*invTexSize.x*segSign, upTexV);
                verticies[vertex++].Set(upBorder, zeroAlphaColor, u + prevDir.Dot(upBorder - point)*invTexSize.x*segSign, 1);
                verticies[vertex++].Set(down, color, u + prevDir.Dot(down - point)*invTexSize.x*segSign, downTexV);
                verticies[vertex++].Set(downBorder, zeroAlphaColor, u + prevDir.Dot(downBorder - point)*invTexSize.x*segSign, 0.0f);
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

    // Function that determines if a point is inside the half-space defined by the line
    bool Inside(const Vec2F& p, const Vec2F& lineBegin, const Vec2F& lineEnd)
    {
        return ((lineEnd.x - lineBegin.x) * (p.y - lineBegin.y) - (lineEnd.y - lineBegin.y) * (p.x - lineBegin.x)) < 0;
    }

    int Geometry::ClipTriangleByLine(const Vertex& a, const Vertex& b, const Vertex& c, const Vec2F& lineBegin, const Vec2F& lineEnd, Vertex* output)
    {
        int trianglesCount = 0;

        // Classify vertices as being on one side of the line or the other
        bool aInside = Inside(a, lineBegin, lineEnd);
        bool bInside = Inside(b, lineBegin, lineEnd);
        bool cInside = Inside(c, lineBegin, lineEnd);

        // If all vertices are inside, then return the original triangle
        if (aInside && bInside && cInside) {
            output[0] = a;
            output[1] = b;
            output[2] = c;
            trianglesCount = 1;
        }
        // If all vertices are outside, return no triangles
        else if (!aInside && !bInside && !cInside)
        {
            trianglesCount = 0;
        }
        // If only one vertex is inside, form a new triangle
        else if (aInside ^ bInside ^ cInside) 
        {
            if (aInside) 
            {
                Vertex abIntersection = Intersection::LinesVertex(a, b, lineBegin, lineEnd);
                Vertex acIntersection = Intersection::LinesVertex(a, c, lineBegin, lineEnd);
                output[0] = a;
                output[1] = { abIntersection };
                output[2] = { acIntersection };
                trianglesCount = 1;
            }
            else if (bInside)
            {
                Vertex baIntersection = Intersection::LinesVertex(b, a, lineBegin, lineEnd);
                Vertex bcIntersection = Intersection::LinesVertex(b, c, lineBegin, lineEnd);
                output[0] = b;
                output[1] = { baIntersection };
                output[2] = { bcIntersection };
                trianglesCount = 1;
            }
            else
            { // cInside
                Vertex caIntersection = Intersection::LinesVertex(c, a, lineBegin, lineEnd);
                Vertex cbIntersection = Intersection::LinesVertex(c, b, lineBegin, lineEnd);
                output[0] = c;
                output[1] = { caIntersection };
                output[2] = { cbIntersection };
                trianglesCount = 1;
            }
        }
        // If two vertices are inside, form two new triangles
        else
        {
            if (!aInside) 
            {
                Vertex baIntersection = Intersection::LinesVertex(b, a, lineBegin, lineEnd);
                Vertex caIntersection = Intersection::LinesVertex(c, a, lineBegin, lineEnd);
                output[0] = b;
                output[1] = c;
                output[2] = { baIntersection };
                output[3] = c;
                output[4] = { baIntersection };
                output[5] = { caIntersection };
                trianglesCount = 2;
            }
            else if (!bInside) 
            {
                Vertex abIntersection = Intersection::LinesVertex(a, b, lineBegin, lineEnd);
                Vertex cbIntersection = Intersection::LinesVertex(c, b, lineBegin, lineEnd);
                output[0] = a;
                output[1] = c;
                output[2] = { abIntersection };
                output[3] = c;
                output[4] = { abIntersection };
                output[5] = { cbIntersection };
                trianglesCount = 2;
            }
            else
            { // !cInside
                Vertex acIntersection = Intersection::LinesVertex(a, c, lineBegin, lineEnd);
                Vertex bcIntersection = Intersection::LinesVertex(b, c, lineBegin, lineEnd);
                output[0] = a;
                output[1] = b;
                output[2] = { acIntersection };
                output[3] = b;
                output[4] = { acIntersection };
                output[5] = { bcIntersection };
                trianglesCount = 2;
            }
        }

        return trianglesCount;
    }

    int Geometry::ClipTriangleByRectangle(const Vertex& a, const Vertex& b, const Vertex& c, Vertex* output, const RectF& clippRect)
    {
        // Edges of the rectangle
        Pair<Vec2F, Vec2F> rectEdges[4] =
        {
            { clippRect.LeftTop(), clippRect.RightTop() },
            { clippRect.RightTop(), clippRect.RightBottom() },
            { clippRect.RightBottom(), clippRect.LeftBottom() },
            { clippRect.LeftBottom(), clippRect.LeftTop() }
        };

        // Two buffers for algorithm
        Vertex* inputBuffer = output;
        Vertex* outputBuffer = output + 16;

        // Start with original triangle
        inputBuffer[0] = a;
        inputBuffer[1] = b;
        inputBuffer[2] = c;
        int inputTrianglesCount = 1;
        int outputTrianglesCount = 0;

        // For each edge of the rectangle
        for (int edgeIdx = 0; edgeIdx < 4; edgeIdx++)
        {
            for (int i = 0; i < inputTrianglesCount; i++)
            {
                // Clip triangle by edge
                int idx = i*3;
                outputTrianglesCount += ClipTriangleByLine(inputBuffer[idx], inputBuffer[idx + 1], inputBuffer[idx + 2],
                                                           rectEdges[edgeIdx].first, rectEdges[edgeIdx].second,
                                                           outputBuffer + outputTrianglesCount*3);
            }

            // Swap buffers
            Math::Swap(inputBuffer, outputBuffer);
            inputTrianglesCount = outputTrianglesCount;
            outputTrianglesCount = 0;
        }

        return inputTrianglesCount;
    }

}
