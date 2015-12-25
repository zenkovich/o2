#pragma once

#include "public.h"
#include "texture_base_interface.h"
#include "util/math/color.h"

OPEN_O2_NAMESPACE

class RenderSystem;
class Texture;

/** Render target base interface. */
class RenderTargetBaseInterface
{
protected:
	Texture* mRenderTexture; /**< Render texture ptr, usage must be RENDER_TARGET. */
	bool     mReady;         /**< True, if we can use that render target. */

	/** ctor. */
	RenderTargetBaseInterface(Texture* texture);

public:
	/** ctor. */
	RenderTargetBaseInterface(const Vec2F& size = Vec2F(), TextureFormat texFormat = TextureFormat::Default);

	/** dtor. */
	virtual ~RenderTargetBaseInterface();

	/** Binding this render target to render system. */
	void Bind();

	/** Unbinding this render target to render system. */
	void Unbind();

	/** Returns using render texture ptr. */
	TextureRef GetTexture() const;

	/** Returns true, if render target ready to use. */
	bool IsReady() const;
};

CLOSE_O2_NAMESPACE