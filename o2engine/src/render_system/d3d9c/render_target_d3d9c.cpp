#include "public.h"
#ifdef RENDER_D3D9C

#include "render_target_d3d9c.h"

#include "../render_system.h"
#include "../texture.h"
#include "other/device_info.h"
#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE

grRenderTarget::grRenderTarget( grRenderSystem* renderSystem, grTexture* texture ):
	grRenderTargetBaseInterface(renderSystem, texture)
{
	mReady = true;
}

grRenderTarget::grRenderTarget( grRenderSystem* renderSystem, const vec2f& size /*= vec2f(0, 0)*/, 
	                            grTexFormat::type texFormat /*= grTexFormat::DEFAULT */ ):
	grRenderTargetBaseInterface(renderSystem, size, texFormat)
{
	mReady = true;
}

grRenderTarget::~grRenderTarget()
{
}

CLOSE_O2_NAMESPACE

#endif //RENDER_D3D9C