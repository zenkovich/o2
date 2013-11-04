#include "public.h"
#ifdef RENDER_OGL

#include "ogl_ext_win.h"

#include "util/log/log_stream.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

/** Returns addres of function. */
PROC getSafeWGLProcAddress(const char* id, cLogStream* log)
{
	PROC res = wglGetProcAddress(id);
	if (!res)
		log->out("ERROR: Failed to get func adress: '%s'", id);

	return res;
}

void getGLExtensions( cLogStream* log /*= NULL*/ )
{
	cLogStream* llog = log;
	if (!llog)
		llog = gLog;
	
	glGenFramebuffersEXT        = (PFNGLGENFRAMEBUFFERSEXTPROC) getSafeWGLProcAddress("glGenFramebuffersEXT", llog);
	glBindFramebufferEXT        = (PFNGLBINDFRAMEBUFFEREXTPROC) getSafeWGLProcAddress("glBindFramebufferEXT", llog);
	glFramebufferTexture        = (PFNGLFRAMEBUFFERTEXTUREPROC) getSafeWGLProcAddress("glFramebufferTexture", llog);
	glDrawBuffers               = (PFNGLDRAWBUFFERSPROC) getSafeWGLProcAddress("glDrawBuffers", llog);
	glDeleteBuffers             = (PFNGLDELETEBUFFERSPROC) getSafeWGLProcAddress("glDeleteBuffers", llog);
	glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) getSafeWGLProcAddress("glCheckFramebufferStatusEXT", llog);
}

extern PFNGLGENFRAMEBUFFERSEXTPROC        glGenFramebuffersEXT        = NULL;
extern PFNGLBINDFRAMEBUFFEREXTPROC        glBindFramebufferEXT        = NULL;
extern PFNGLFRAMEBUFFERTEXTUREPROC        glFramebufferTexture        = NULL;
extern PFNGLDRAWBUFFERSPROC               glDrawBuffers               = NULL;
extern PFNGLDELETEBUFFERSPROC             glDeleteBuffers             = NULL;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = NULL;

CLOSE_O2_NAMESPACE

#endif //RENDER_OGL