#pragma once

#include "Utils/Math/Vector2.h"
#include "Utils/CommonTypes.h"

namespace o2
{
	class Vertex2: public Vec2<float>
	{
	public:
		//    x, y; - from vec2<float>
		float z;
		ULong color;
		float tu, tv;

		Vertex2();
		Vertex2(float vx, float vy);
		Vertex2(float vx, float vy, float vz);
		Vertex2(float vx, float vy, float vz, ULong vcolor, float vtu, float vtv);
		Vertex2(float vx, float vy, ULong vcolor, float vtu, float vtv);
		Vertex2(const Vec2F& pos, ULong vcolor, float vtu, float vtv);

		void Set(const Vec2F& pos, ULong ccolor, float u, float v);
		void Set(float px, float py, ULong ccolor, float u, float v);
		void Set(const Vec2F& pos, float cz, ULong ccolor, float u, float v);

		void SetUV(float u, float v);
		void SetUV(const Vec2F& uv);

		Vertex2 operator=(const Vec2F& vec);
		operator Vec2F() const;
	};

	//implementation Vertex2

	Vertex2::Vertex2():
		Vec2<float>(), color(0), tu(0), tv(0), z(1)
	{}

	Vertex2::Vertex2(float vx, float vy):
		Vec2<float>(vx, vy), z(1), color(0), tu(0), tv(0)
	{}

	Vertex2::Vertex2(float vx, float vy, float vz):
		Vec2<float>(vx, vy), z(vz), color(0), tu(0), tv(0)
	{}

	Vertex2::Vertex2(float vx, float vy, float vz, ULong vcolor, float vtu, float vtv):
		Vec2<float>(vx, vy), z(vz), color(vcolor), tu(vtu), tv(vtv)
	{}

	Vertex2::Vertex2(float vx, float vy, ULong vcolor, float vtu, float vtv):
		Vec2<float>(vx, vy), z(1), color(vcolor), tu(vtu), tv(vtv)
	{}

	Vertex2::Vertex2(const Vec2F& pos, ULong vcolor, float vtu, float vtv):
		Vec2<float>(pos), z(1), color(vcolor), tu(vtu), tv(vtv)
	{}

	void Vertex2::Set(const Vec2F& pos, ULong ccolor, float u, float v)
	{
		x = pos.x; y = pos.y;
		color = ccolor;
		tu = u; tv = v;
	}

	void Vertex2::Set(float px, float py, ULong ccolor, float u, float v)
	{
		x = px; y = py;
		color = ccolor;
		tu = u; tv = v;
	}

	void Vertex2::Set(const Vec2F& pos, float cz, ULong ccolor, float u, float v)
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