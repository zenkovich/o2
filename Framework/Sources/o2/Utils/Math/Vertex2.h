#pragma once

#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Types/CommonTypes.h"

namespace o2
{
	class Vertex2
	{
	public:
		float x, y;
		float z;
		Color32Bit color;
		float tu, tv;

		inline Vertex2();
		inline Vertex2(float vx, float vy);
		inline Vertex2(float vx, float vy, float vz);
		inline Vertex2(float vx, float vy, float vz, Color32Bit vcolor, float vtu, float vtv);
		inline Vertex2(float vx, float vy, Color32Bit vcolor, float vtu, float vtv);
		inline Vertex2(const Vec2F& pos, Color32Bit vcolor, float vtu, float vtv);

		inline void Set(const Vec2F& pos, Color32Bit ccolor, float u, float v);
		inline void Set(float px, float py, Color32Bit ccolor, float u, float v);
		inline void Set(const Vec2F& pos, float cz, Color32Bit ccolor, float u, float v);

		inline void SetUV(float u, float v);
		inline void SetUV(const Vec2F& uv);

		inline bool operator==(const Vertex2& other) const;
		inline Vertex2 operator=(const Vec2F& vec);
		inline operator Vec2F() const;
	};
	
	Vertex2::Vertex2():
		x(0), y(0), color(0), tu(0), tv(0), z(1)
	{}

	Vertex2::Vertex2(float vx, float vy):
		x(vx), y(vy), z(1), color(0), tu(0), tv(0)
	{}

	Vertex2::Vertex2(float vx, float vy, float vz):
		x(vx), y(vy), z(vz), color(0), tu(0), tv(0)
	{}

	Vertex2::Vertex2(float vx, float vy, float vz, Color32Bit vcolor, float vtu, float vtv):
		x(vx), y(vy), z(vz), color(vcolor), tu(vtu), tv(vtv)
	{}

	Vertex2::Vertex2(float vx, float vy, Color32Bit vcolor, float vtu, float vtv):
		x(vx), y(vy), z(1), color(vcolor), tu(vtu), tv(vtv)
	{}

	Vertex2::Vertex2(const Vec2F& pos, Color32Bit vcolor, float vtu, float vtv):
		x(pos.x), y(pos.y), z(1), color(vcolor), tu(vtu), tv(vtv)
	{}

	void Vertex2::Set(const Vec2F& pos, Color32Bit ccolor, float u, float v)
	{
		x = pos.x; y = pos.y;
		color = ccolor;
		tu = u; tv = v;
	}

	void Vertex2::Set(float px, float py, Color32Bit ccolor, float u, float v)
	{
		x = px; y = py;
		color = ccolor;
		tu = u; tv = v;
	}

	void Vertex2::Set(const Vec2F& pos, float cz, Color32Bit ccolor, float u, float v)
	{
		x = pos.x; y = pos.y;
		z = cz;
		color = ccolor;
		tu = u; tv = v;
	}

	void Vertex2::SetUV(float u, float v)
	{
		tu = u; tv = v;
	}

	void Vertex2::SetUV(const Vec2F& uv)
	{
		tu = uv.x; tv = uv.y;
	}

	bool Vertex2::operator==(const Vertex2& other) const
	{
		return Math::Equals(x, other.x) && Math::Equals(y, other.y) && color == other.color &&
			Math::Equals(tu, other.tu) && Math::Equals(tv, other.tv);
	}

	Vertex2 Vertex2::operator=(const Vec2F& vec)
	{
		x = vec.x; y = vec.y;
		return *this;
	}

	Vertex2::operator Vec2F() const
	{
		return Vec2F(x, y);
	}
}
