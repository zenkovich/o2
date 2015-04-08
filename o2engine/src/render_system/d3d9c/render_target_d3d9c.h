#pragma once

#include "d3d9c.h"
#include "render_system/render_target_base_interface.h"

OPEN_O2_NAMESPACE

/** D3D9C render target. */
class RenderTarget:public RenderTargetBaseInterface
{
	friend class RenderSystem;

	/** ctor. */
	RenderTarget(Texture* texture);

public:
	/** ctor. */
	RenderTarget(const Vec2F& size = Vec2F(), TextureFormat texFormat = TextureFormat::Default);

	/** dtor. */
	~RenderTarget();
};

CLOSE_O2_NAMESPACE