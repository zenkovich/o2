#pragma once

// OpenGL debug stuff

#include "ogl.h"
#include "public.h"

OPEN_O2_NAMESPACE

/** Returns opengl error description by id. */
const char* getGLErrorDesc(GLenum errorId);

void glCheckError(LogStream* log, const char* filename = NULL, unsigned int line = 0);

#ifdef RENDER_DEBUG
// checking opengl errors
#	define GL_CHECK_ERROR(log) glCheckError(log, __FILE__, __LINE__);
#else
#	define GL_CHECK_ERROR(log) 
#endif

CLOSE_O2_NAMESPACE