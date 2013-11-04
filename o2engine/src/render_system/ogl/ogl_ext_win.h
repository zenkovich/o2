#ifndef OGL_EXT_WIN_H
#define OGL_EXT_WIN_H

// OpenGL extensions

#include "ogl.h"
#include "public.h"

OPEN_O2_NAMESPACE

class cLogStream;

/** Getting openGL extensions. */
void getGLExtensions(cLogStream* log = NULL);

extern PFNGLGENFRAMEBUFFERSEXTPROC        glGenFramebuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC        glBindFramebufferEXT;
extern PFNGLFRAMEBUFFERTEXTUREPROC        glFramebufferTexture;
extern PFNGLDRAWBUFFERSPROC               glDrawBuffers;
extern PFNGLDELETEBUFFERSPROC             glDeleteBuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;

CLOSE_O2_NAMESPACE

#endif //OGL_EXT_WIN_H