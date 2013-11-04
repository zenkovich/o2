#ifndef OGL_DEBUG_H
#define OGL_DEBUG_H

// OpenGL debug stuff

#include "ogl.h"
#include "public.h"

OPEN_O2_NAMESPACE

/** Returns opengl error description by id. */
const char* getGLErrorDesc(GLenum errorId);

class cLogStream;
void glCheckError(cLogStream* log, const char* filename = NULL, unsigned int line = 0);

#ifdef RENDER_DEBUG
    // checking ogl errors
#	define GL_CHECK_ERROR(log) glCheckError(log, __FILE__, __LINE__);
#else
#	define GL_CHECK_ERROR(log) 
#endif

CLOSE_O2_NAMESPACE

#endif //OGL_DEBUG_H