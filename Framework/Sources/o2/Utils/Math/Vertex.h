#pragma once

#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Types/CommonTypes.h"

namespace o2
{
	struct Vertex
	{
		float x, y, z;
		Color32Bit color;
		float tu, tv;

	public:
		Vertex();
		Vertex(float vx, float vy);
		Vertex(float vx, float vy, float vz);
		Vertex(float vx, float vy, float vz, Color32Bit vcolor, float vtu, float vtv);
		Vertex(float vx, float vy, Color32Bit vcolor, float vtu, float vtv);
		Vertex(const Vec2F& pos, Color32Bit vcolor, float vtu, float vtv);

		void Set(const Vec2F& pos, Color32Bit ccolor, float u, float v);
		void Set(float px, float py, Color32Bit ccolor, float u, float v);
		void Set(const Vec2F& pos, float cz, Color32Bit ccolor, float u, float v);

		void SetPosition(const Vec2F& pos);
		void SetPosition(float px, float py, float pz = 0);

		void SetUV(float u, float v);
		void SetUV(const Vec2F& uv);

		bool operator==(const Vertex& other) const;
		Vertex operator=(const Vec2F& vec);
		operator Vec2F() const;
	};
}
