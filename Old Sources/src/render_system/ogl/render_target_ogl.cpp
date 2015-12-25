#include "public.h"

#ifdef RENDER_OGL

#include "ogl_debug.h"
#include "ogl_ext_win.h"
#include "other/device_info.h"
#include "render_system/render_system.h"
#include "render_system/texture.h"
#include "render_target_ogl.h"
#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE

RenderTarget::RenderTarget(Texture* texture):
RenderTargetBaseInterface(texture), mFrameBuffer(0)
{
	if (mRenderTexture)
		initializeBuffer();
}

RenderTarget::RenderTarget(const Vec2F& size /*= vec2f()*/,
						   TextureFormat texFormat /*= grTexFormat::DEFAULT */):
						   RenderTargetBaseInterface(size, texFormat), mFrameBuffer(0)
{
	if (!mRenderTexture)
		initializeBuffer();
}

RenderTarget::~RenderTarget()
{
	glDeleteBuffers(1, &mFrameBuffer);
}

void RenderTarget::initializeBuffer()
{
	glGenFramebuffersEXT(1, &mFrameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER, mFrameBuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderTexture->mHandle, 0);

	GLenum DrawBuffers[2] ={GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		GLenum glError = glGetError();

		AppRender()->mLog->Error("Failed to create GL frame buffer object! GL Error %x %s", glError,
								 getGLErrorDesc(glError));

		mReady = false;

		return;
	}

	mReady = true;

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
}

CLOSE_O2_NAMESPACE

#endif //RENDER_OGL