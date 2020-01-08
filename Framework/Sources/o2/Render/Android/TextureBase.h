#pragma once

#ifdef PLATFORM_ANDROID

#include <GLES2/gl2.h>

namespace o2
{
	class TextureBase
	{
		friend class Render;
		friend class VectorFont;

	protected:
		GLuint mHandle;      // Texture handle
		GLuint mFrameBuffer; // Frame buffer for rendering into texture
	};
}

#endif // PLATFORM_ANDROID
