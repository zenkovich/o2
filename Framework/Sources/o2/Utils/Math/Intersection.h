#pragma once

#include "o2/Utils/Math/Vector2.h"

namespace o2
{
    namespace Intersection
    {
        Vec2F Lines(const Vec2F& origin1, const Vec2F& dir1, const Vec2F& origin2, const Vec2F& dir2);
        Vec2F LinesNoChek(const Vec2F& origin1, const Vec2F& dir1, const Vec2F& origin2, const Vec2F& dir2);
        Vertex LinesVertex(const Vertex& a1, const Vertex& b1, const Vec2F& a2, const Vec2F& b2);
    }
}
