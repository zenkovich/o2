#pragma once

#include "o2/Utils/Math/Transform.h"


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

		// Returns camera with fixed size
		static Camera FixedSize(const Vec2F& size);

		// Returns camera with fixed aspect
		static Camera FittedSize(const Vec2F& size);

		// Returns camera with physical correct units
		static Camera PhysicalCorrect(Units units);

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

	PUBLIC_STATIC_FUNCTION(Camera, Default);
	PUBLIC_STATIC_FUNCTION(Camera, FixedSize, const Vec2F&);
	PUBLIC_STATIC_FUNCTION(Camera, FittedSize, const Vec2F&);
	PUBLIC_STATIC_FUNCTION(Camera, PhysicalCorrect, Units);
}
END_META;
