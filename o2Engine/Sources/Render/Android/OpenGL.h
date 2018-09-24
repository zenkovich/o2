#pragma once

#ifdef PLATFORM_ANDROID

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace o2
{
	class LogStream;
}

// Returns open gl error description by id
const char* GetGLErrorDesc(GLenum errorId);

// Checks OpenGL error
void glCheckError(const char* filename = nullptr, unsigned int line = 0);

#if RENDER_DEBUG
#	define GL_CHECK_ERROR() glCheckError(__FILE__, __LINE__);
#else
#	define GL_CHECK_ERROR(log) 
#endif

#endif // PLATFORM_ANDROID
