#ifndef RENDER_TARGET_BASE_INTERFACE_H
#define RENDER_TARGET_BASE_INTERFACE_H

#include "public.h"
#include "texture_base_interface.h"
#include "util/math/color.h"

OPEN_O2_NAMESPACE
	
class grRenderSystem;
class grTexture;

/** Render target base interface. */
class grRenderTargetBaseInterface
{
protected:
	grRenderSystem* mRenderSystem;  /**< Render system ptr. */
	grTexture*      mRenderTexture; /**< Render texture ptr, usage must be RENDER_TARGET. */
	bool            mReady;         /**< True, if we can use that rander target. */

public:
	/** ctor. */
	grRenderTargetBaseInterface(grRenderSystem* renderSystem, grTexture* texture);

	/** ctor. */
	grRenderTargetBaseInterface(grRenderSystem* renderSystem, const vec2f& size = vec2f(0, 0), 
		                        grTexFormat::type texFormat = grTexFormat::DEFAULT );

	/** dtor. */
	virtual ~grRenderTargetBaseInterface();

	/** Binding this render target to render system. */
	void bind();

	/** Unbinding this render target to render system. */
	void unbind();

	/** Returns using render texture ptr. */
	grTexture* getTexture() const;
	
	/** Returns true, if render target ready to use. */
	bool isReady() const;
};

CLOSE_O2_NAMESPACE

#endif //RENDER_TARGET_BASE_INTERFACE_H