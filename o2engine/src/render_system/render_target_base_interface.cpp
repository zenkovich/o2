#include "render_target_base_interface.h"

#include "render_system.h"
#include "texture.h"
#include "util/log/log_stream.h"
#include "render_target.h"

OPEN_O2_NAMESPACE

grRenderTargetBaseInterface::grRenderTargetBaseInterface( grRenderSystem* renderSystem, grTexture* texture ):
	mRenderSystem(renderSystem), mReady(false)
{
	if (!mRenderSystem)
		return;
	
	if (!mRenderSystem->isRenderTargetAvailable())
	{
		mRenderSystem->mLog->out("ERROR: Render targets on current Renderer is not available!");
		mRenderSystem = NULL;
		return;
	}

	if (texture->getUsage() != grTexUsage::RENDER_TARGET)
	{
		mRenderSystem->mLog->out("ERROR: render target can't use not render target texture!");
	}
	else
	{
		mRenderTexture = texture;
		mRenderTexture->incRefCount();
	}
}

grRenderTargetBaseInterface::grRenderTargetBaseInterface( grRenderSystem* renderSystem, const vec2f& size /*= vec2f(0, 0)*/, 
	                                                      grTexFormat::type texFormat /*= grTexFormat::DEFAULT */ ):
	mRenderSystem(renderSystem), mReady(false)
{
	if (!mRenderSystem)
		return;
	
	if (!mRenderSystem->isRenderTargetAvailable())
	{
		mRenderSystem->mLog->out("ERROR: Render targets on current Renderer is not available!");
		mRenderSystem = NULL;
		mRenderTexture = NULL;
		return;
	}

	vec2f texSize = size;
	if (texSize.x < 1)
	{
		texSize = mRenderSystem->getResolution().castTo<float>();
	}

	vec2i maxTextureSize = mRenderSystem->getMaxTextureSize();
	if (texSize.x > maxTextureSize.x || texSize.y > maxTextureSize.y)
	{
		mRenderSystem->mLog->out("WARNING: Render target size too large! size %ix%i boundin by max %ix%i",
			(int)texSize.x, (int)texSize.y, maxTextureSize.x, maxTextureSize.y);
		
		texSize.x = clamp<float>(texSize.x, 64.0f, (float)maxTextureSize.x);
		texSize.y = clamp<float>(texSize.y, 64.0f, (float)maxTextureSize.y);
	}

	mRenderTexture = mRenderSystem->addTexture(new grTexture(mRenderSystem, texSize, texFormat, grTexUsage::RENDER_TARGET));
	mRenderTexture->incRefCount();
}

grRenderTargetBaseInterface::~grRenderTargetBaseInterface()
{
	if (mRenderTexture)
		mRenderSystem->removeTexture(mRenderTexture);
}

void grRenderTargetBaseInterface::bind()
{
	if (mRenderTexture && mRenderSystem)
		mRenderSystem->bindRenderTarget(static_cast<grRenderTarget*>(this));
}

void grRenderTargetBaseInterface::unbind()
{
	if (mRenderTexture && mRenderSystem)
		mRenderSystem->unbindRenderTarget();
}

grTexture* grRenderTargetBaseInterface::getTexture() const
{
	return mRenderTexture;
}

bool grRenderTargetBaseInterface::isReady() const
{
	return mReady;
}

CLOSE_O2_NAMESPACE