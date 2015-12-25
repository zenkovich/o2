#include "render_target_base_interface.h"

#include "render_system.h"
#include "render_target.h"
#include "texture.h"
#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE

RenderTargetBaseInterface::RenderTargetBaseInterface(Texture* texture):
mReady(false)
{

	if (!AppRender()->IsRenderTargetAvailable())
	{
		AppRender()->mLog->Error("Render targets on current Renderer is not available!");
		return;
	}

	if (texture->GetUsage() != TextureUsage::RenderTarget)
	{
		AppRender()->mLog->Error("render target can't use not render target texture!");
	}
	else
	{
		mRenderTexture = texture;
	}
}

RenderTargetBaseInterface::RenderTargetBaseInterface(const Vec2F& size /*= vec2f(0, 0)*/,
													 TextureFormat texFormat /*= grTexFormat::DEFAULT */):
													 mReady(false)
{
	if (!AppRender()->IsRenderTargetAvailable())
	{
		AppRender()->mLog->Error("Render targets on current Renderer is not available!");
		mRenderTexture = NULL;
		return;
	}

	Vec2F texSize = size;
	if (texSize.x < 1)
	{
		texSize = AppRender()->GetResolution();
	}

	Vec2I maxTextureSize = AppRender()->GetMaxTextureSize();
	if (texSize.x > maxTextureSize.x || texSize.y > maxTextureSize.y)
	{
		AppRender()->mLog->Warning("Render target size too large! size %ix%i bounding by max %ix%i",
								   (int)texSize.x, (int)texSize.y, maxTextureSize.x, maxTextureSize.y);

		texSize.x = Clamp(texSize.x, 64.0f, (float)maxTextureSize.x);
		texSize.y = Clamp(texSize.y, 64.0f, (float)maxTextureSize.y);
	}

	mRenderTexture = AppRender()->CreateRenderTargetTexture(texSize, texFormat);
}

RenderTargetBaseInterface::~RenderTargetBaseInterface()
{
	if (mRenderTexture)
		AppRender()->RemoveTexture(mRenderTexture);
}

void RenderTargetBaseInterface::Bind()
{
	if (mRenderTexture)
		AppRender()->BindRenderTarget(static_cast<RenderTarget*>(this));
}

void RenderTargetBaseInterface::Unbind()
{
	if (mRenderTexture)
		AppRender()->UnbindRenderTarget();
}

TextureRef RenderTargetBaseInterface::GetTexture() const
{
	return TextureRef(mRenderTexture);
}

bool RenderTargetBaseInterface::IsReady() const
{
	return mReady;
}

CLOSE_O2_NAMESPACE