#pragma once
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class PhysicsConfig: public ISerializable
	{
	public:
		Vec2F gravity = Vec2F(0, -98.0f); // Gravity force @SERIALIZABLE

		float scale = 10.0f; // World metrics to physics metrics scale @SERIALIZABLE

		int velocityIterations = 8; // Number of velocity solver iterations @SERIALIZABLE
		int positionIterations = 3; // Number of position solver iterations @SERIALIZABLE

		SERIALIZABLE(PhysicsConfig);
	};
}

CLASS_BASES_META(o2::PhysicsConfig)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::PhysicsConfig)
{
	PUBLIC_FIELD(gravity).DEFAULT_VALUE(Vec2F(0, -98.0f)).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(scale).DEFAULT_VALUE(10.0f).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(velocityIterations).DEFAULT_VALUE(8).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(positionIterations).DEFAULT_VALUE(3).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::PhysicsConfig)
{
}
END_META;
