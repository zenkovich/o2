#pragma once

#include "Utils/Math/Transform.h"
#include "Utils/Memory/Ptr.h"

namespace o2
{
	class Texture;

	// ------
	// Camera
	// ------
	class Camera: public Transform
	{
	public:
		// Constructor
		Camera(const Vec2F& position = Vec2F(), const Vec2F& size = Vec2F(), float angle = 0.0f);

		// Returns default camera
		static Camera Default();

		SERIALIZABLE(Camera);
	};
}
