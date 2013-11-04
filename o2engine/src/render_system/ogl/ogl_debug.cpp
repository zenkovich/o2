#include "public.h"
#ifdef RENDER_OGL

#include "ogl_debug.h"

#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE

const char* getGLErrorDesc( GLenum errorId )
{
	if ( errorId == GL_NO_ERROR) return "GL_NO_ERROR";
	if ( errorId == GL_INVALID_ENUM) return "GL_INVALID_ENUM";
	if ( errorId == GL_INVALID_VALUE) return "GL_INVALID_VALUE";
	if ( errorId == GL_INVALID_OPERATION) return "GL_INVALID_OPERATION";
	if ( errorId == GL_INVALID_FRAMEBUFFER_OPERATION) return "GL_INVALID_FRAMEBUFFER_OPERATION";
	if ( errorId == GL_OUT_OF_MEMORY) return "GL_OUT_OF_MEMORY";
	if ( errorId == GL_STACK_UNDERFLOW) return "GL_STACK_UNDERFLOW";
	if ( errorId == GL_STACK_OVERFLOW) return "GL_STACK_OVERFLOW";

	return "UNKNOWN";
}

void glCheckError( cLogStream* log, const char* filename /*= NULL*/, unsigned int line /*= 0*/ )
{
	GLenum errId = glGetError();
	if (errId != GL_NO_ERROR)
	{
		log->out("GL ERROR %x: %s at file: %s line: %i", errId, getGLErrorDesc(errId), ( filename ? filename:"unknown" ),
			line);
	}
}

CLOSE_O2_NAMESPACE

#endif //RENDER_OGL