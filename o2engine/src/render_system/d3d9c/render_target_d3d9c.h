#ifndef RENDER_TARGET_D3D9C_H
#define RENDER_TARGET_D3D9C_H

#include "d3d9c.h"

#include "../render_target_base_interface.h"

OPEN_O2_NAMESPACE

/** D3D9C render target. */
class grRenderTarget:public grRenderTargetBaseInterface
{
	friend class grRenderSystem;

public:
	/** ctor. */
	grRenderTarget(grRenderSystem* renderSystem, grTexture* texture);
	
	/** ctor. */
	grRenderTarget(grRenderSystem* renderSystem, const vec2f& size = vec2f(0, 0), 
		           grTexFormat::type texFormat = grTexFormat::DEFAULT );
	
	/** dtor. */
	~grRenderTarget();
};

CLOSE_O2_NAMESPACE

#endif //RENDER_TARGET_D3D9C_H