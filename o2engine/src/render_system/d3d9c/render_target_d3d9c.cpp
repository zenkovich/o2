#include "public.h"
#ifdef RENDER_D3D9C

#include "render_target_d3d9c.h"

#include "other/device_info.h"
#include "renser_system/render_system.h"
#include "renser_system/texture.h"
#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE

RenderTarget::RenderTarget(Texture* texture):
RenderTargetBaseInterface(texture)
{
	mReady = true;
}

RenderTarget::RenderTarget(const Vec2F& size /*= vec2f()*/,
						   TextureFormat::type texFormat /*= grTexFormat::DEFAULT */):
						   RenderTargetBaseInterface(size, texFormat)
{
	mReady = true;
}

RenderTarget::~RenderTarget()
{
}

CLOSE_O2_NAMESPACE

#endif //RENDER_D3D9C