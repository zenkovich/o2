#pragma once

#ifdef PLATFORM_WINDOWS

#include "Render/Windows/OpenGL.h"
#include "Utils/Types/CommonTypes.h"
#include "Utils/Math/Vector2.h"


namespace o2
{
	class Texture;

	class RenderBase
	{
	protected:
		HGLRC mGLContext; // OpenGL context
		HDC   mHDC;       // Windows frame device context
	};
};

#endif // PLATFORM_WINDOWS
