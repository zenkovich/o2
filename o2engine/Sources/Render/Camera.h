#pragma once

#include "Utils/Math/Transform.h"
#include "Utils/Memory/Ptr.h"

namespace o2
{
	class Texture;

	class Camera: public Transform
	{
	public:
		Camera(const Vec2F& position = Vec2F(), const Vec2F& size = Vec2F(), float angle = 0.0f);

		static Camera Default();
	};
}
