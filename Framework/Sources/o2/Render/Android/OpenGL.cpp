#include "o2/stdafx.h"

#ifdef PLATFORM_ANDROID

#include "OpenGL.h"
#include "Utils/Debug/Log/LogStream.h"
#include "Utils/Debug/Debug.h"

const char* GetGLErrorDesc(GLenum errorId)
{
	if (errorId == GL_NO_ERROR) return "GL_NO_ERROR";
	if (errorId == GL_INVALID_ENUM) return "GL_INVALID_ENUM";
	if (errorId == GL_INVALID_VALUE) return "GL_INVALID_VALUE";
	if (errorId == GL_INVALID_OPERATION) return "GL_INVALID_OPERATION";
	if (errorId == GL_INVALID_FRAMEBUFFER_OPERATION) return "GL_INVALID_FRAMEBUFFER_OPERATION";
	if (errorId == GL_OUT_OF_MEMORY) return "GL_OUT_OF_MEMORY";

	return "UNKNOWN";
}

void glCheckError(const char* filename /*= nullptr*/, unsigned int line /*= 0*/)
{
	GLenum errId = glGetError();
	if (errId != GL_NO_ERROR)
	{
		o2Debug.LogError("OpenGL ERROR " + (o2::String)errId + ": " + (o2::String)GetGLErrorDesc(errId) +
						 " at file: " + (o2::String)(filename ? filename : "unknown") + " line: " + (o2::String)line);
	}
}

#endif // PLATFORM_ANDROID