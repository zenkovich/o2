#pragma once

#include "Utils/Math/Transform.h"
#include "Utils/Memory/Ptr.h"

namespace o2
{
	class Texture;

	class Camera: public Transform
	{
	public:
		static Camera Default();
	};
}
