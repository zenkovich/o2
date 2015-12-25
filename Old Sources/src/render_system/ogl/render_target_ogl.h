#pragma once

#include "ogl.h"
#include "render_system/render_target_base_interface.h"

OPEN_O2_NAMESPACE

/** Opengl render target. */
class RenderTarget:public RenderTargetBaseInterface
{
	friend class RenderSystem;

	GLuint mFrameBuffer; /**< Opengl frame buffer handle. */

	/** ctor. */
	RenderTarget(Texture* texture);

public:
	/** ctor. */
	RenderTarget(const Vec2F& size = Vec2F(), TextureFormat texFormat = TextureFormat::Default);

	/** dtor. */
	~RenderTarget();

protected:
	void initializeBuffer();
};

CLOSE_O2_NAMESPACE