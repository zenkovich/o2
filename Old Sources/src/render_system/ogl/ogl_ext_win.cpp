#include "public.h"
#ifdef RENDER_OGL

#include "ogl_ext_win.h"
#include "util/log/log_stream.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

/** Returns address of function. */
PROC getSafeWGLProcAddress(const char* id, LogStream* log)
{
	PROC res = wglGetProcAddress(id);
	if (!res)
		log->Error("Failed to get func address: '%s'", id);

	return res;
}

void getGLExtensions(LogStream* log /*= NULL*/)
{
	LogStream* llog = log ? log:gLog;

	glGenFramebuffersEXT        = (PFNGLGENFRAMEBUFFERSEXTPROC)getSafeWGLProcAddress("glGenFramebuffersEXT", llog);
	glBindFramebufferEXT        = (PFNGLBINDFRAMEBUFFEREXTPROC)getSafeWGLProcAddress("glBindFramebufferEXT", llog);
	glFramebufferTexture        = (PFNGLFRAMEBUFFERTEXTUREPROC)getSafeWGLProcAddress("glFramebufferTexture", llog);
	glDrawBuffers               = (PFNGLDRAWBUFFERSPROC)getSafeWGLProcAddress("glDrawBuffers", llog);
	glDeleteBuffers             = (PFNGLDELETEBUFFERSPROC)getSafeWGLProcAddress("glDeleteBuffers", llog);
	glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)getSafeWGLProcAddress("glCheckFramebufferStatusEXT", llog);
}

extern PFNGLGENFRAMEBUFFERSEXTPROC        glGenFramebuffersEXT        = NULL;
extern PFNGLBINDFRAMEBUFFEREXTPROC        glBindFramebufferEXT        = NULL;
extern PFNGLFRAMEBUFFERTEXTUREPROC        glFramebufferTexture        = NULL;
extern PFNGLDRAWBUFFERSPROC               glDrawBuffers               = NULL;
extern PFNGLDELETEBUFFERSPROC             glDeleteBuffers             = NULL;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = NULL;

CLOSE_O2_NAMESPACE

#endif //RENDER_OGL