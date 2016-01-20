#pragma once

#include <windows.h>	
#include <GL/gl.h>
#include <GL/glu.h>
#include "Dependencies/OpenGL/glext.h"
#include "Dependencies/OpenGL/wglext.h"


namespace o2
{
	class LogStream;
}

// Getting openGL extensions
void GetGLExtensions(o2::LogStream* log = nullptr);

// Returns opengl error description by id
const char* GetGLErrorDesc(GLenum errorId);

// Checks OpenGL extension supporting
bool IsGLExtensionSupported(const char *extension);

// Checks OpenGL error
void glCheckError(o2::LogStream* log, const char* filename = nullptr, unsigned int line = 0);

#if RENDER_DEBUG
#	define GL_CHECK_ERROR(log) glCheckError(log, __FILE__, __LINE__);
#else
#	define GL_CHECK_ERROR(log) 
#endif

extern PFNGLGENFRAMEBUFFERSEXTPROC        glGenFramebuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC        glBindFramebufferEXT;
extern PFNGLFRAMEBUFFERTEXTUREPROC        glFramebufferTexture;
extern PFNGLDRAWBUFFERSPROC               glDrawBuffers;
extern PFNGLDELETEBUFFERSPROC             glDeleteBuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;