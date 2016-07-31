#pragma once

#include "Render/Windows/OpenGL.h"

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
