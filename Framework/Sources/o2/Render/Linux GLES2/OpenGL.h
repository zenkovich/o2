#pragma once

#if defined(PLATFORM_LINUX) && defined(O2_RENDER_GLES2)

#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

// Returns opengl error description by id
const char* GetGLErrorDesc(GLenum errorId);

// Checks OpenGL error
void glCheckError(const char* filename = nullptr, unsigned int line = 0);

#if RENDER_DEBUG || true
#	define GL_CHECK_ERROR() glCheckError(__FILE__, __LINE__);
#else
#	define GL_CHECK_ERROR()
#endif

#endif // PLATFORM_LINUX
