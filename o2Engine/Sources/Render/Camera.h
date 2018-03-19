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

		// Equals operator
		bool operator==(const Camera& other) const;

		// Not equals operator
		bool operator!=(const Camera& other) const;

		// Returns default camera
		static Camera Default();

		SERIALIZABLE(Camera);
	};
}

CLASS_BASES_META(o2::Camera)
{
	BASE_CLASS(o2::Transform);
}
END_META;
CLASS_FIELDS_META(o2::Camera)
{
}
END_META;
CLASS_METHODS_META(o2::Camera)
{
}
END_META;
