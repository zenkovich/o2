#include "o2/stdafx.h"
#include "Vertex.h"

namespace o2
{
    Vertex::Vertex() :
        x(0), y(0), color(0), tu(0), tv(0), z(1)
    {}

    Vertex::Vertex(float vx, float vy) :
        x(vx), y(vy), z(1), color(0), tu(0), tv(0)
    {}

    Vertex::Vertex(float vx, float vy, float vz) :
        x(vx), y(vy), z(vz), color(0), tu(0), tv(0)
    {}

    Vertex::Vertex(float vx, float vy, float vz, Color32Bit vcolor, float vtu, float vtv) :
        x(vx), y(vy), z(vz), color(vcolor), tu(vtu), tv(vtv)
    {}

    Vertex::Vertex(float vx, float vy, Color32Bit vcolor, float vtu, float vtv) :
        x(vx), y(vy), z(1), color(vcolor), tu(vtu), tv(vtv)
    {}

    Vertex::Vertex(const Vec2F& pos, Color32Bit vcolor, float vtu, float vtv) :
        x(pos.x), y(pos.y), z(1), color(vcolor), tu(vtu), tv(vtv)
    {}

    void Vertex::Set(const Vec2F& pos, Color32Bit ccolor, float u, float v)
    {
        x = pos.x; y = pos.y;
        color = ccolor;
        tu = u; tv = v;
    }

    void Vertex::Set(float px, float py, Color32Bit ccolor, float u, float v)
    {
        x = px; y = py;
        color = ccolor;
        tu = u; tv = v;
    }

    void Vertex::Set(const Vec2F& pos, float cz, Color32Bit ccolor, float u, float v)
    {
        x = pos.x; y = pos.y;
        z = cz;
        color = ccolor;
        tu = u; tv = v;
    }

    void Vertex::SetPosition(const Vec2F& pos)
    {
        x = pos.x;
        y = pos.y;
    }

    void Vertex::SetPosition(float px, float py, float pz /*= 0*/)
    {
        x = px; y = py; z = pz;
    }

    void Vertex::SetUV(float u, float v)
    {
        tu = u; tv = v;
    }

    void Vertex::SetUV(const Vec2F& uv)
    {
        tu = uv.x; tv = uv.y;
    }

    bool Vertex::operator==(const Vertex& other) const
    {
        return Math::Equals(x, other.x) && Math::Equals(y, other.y) && color == other.color &&
            Math::Equals(tu, other.tu) && Math::Equals(tv, other.tv);
    }

    Vertex Vertex::operator=(const Vec2F& vec)
    {
        x = vec.x; y = vec.y;
        return *this;
    }

    Vertex::operator Vec2F() const
    {
        return Vec2F(x, y);
    }
}