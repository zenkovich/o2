#pragma once

#ifdef PLATFORM_WINDOWS

#include "o2/Render/Windows/OpenGL.h"

namespace o2
{
	// ------------------------------
	// Base texture class for Windows
	// ------------------------------
	class TextureBase
	{
	protected:
		GLuint mHandle;      // Texture handle
		GLuint mFrameBuffer; // Frame buffer for rendering into texture

	protected:
		friend class Render;
		friend class VectorFont;
	};
}

#endif // PLATFORM_WINDOWS
