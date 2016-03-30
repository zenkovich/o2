#pragma once

#include "Utils/Math/Transform.h"


namespace o2
{
	class Texture;

	// ------
	// Camera
	// ------
	class Camera: public Transform
	{
	public:
		// Default constructor
		Camera();

		// Constructor
		Camera(const Vec2F& position, const Vec2F& size = Vec2F(), float angle = 0.0f);

		// Returns default camera
		static Camera Default();

		SERIALIZABLE(Camera);
	};
}
