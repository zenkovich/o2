#pragma once

/* !!! More information about grTexture interface see in texture_base_interface.h */

#include "engine_settings.h"

#if defined(RENDER_OGL)
#include "ogl/texture_ogl.h"
#elif defined(RENDER_D3D9C)
#include "d3d9c/texture_d3d9c.h"
#endif

OPEN_O2_NAMESPACE

class TextureRef: public cObjectRef<Texture>, public ITexture
{
	friend class RenderSystem;
	friend class RenderTargetBaseInterface;
	friend class Mesh;

	/** type conversion operator. */
	operator Texture*();

public:
	/** ctor. */
	TextureRef(Texture* object = NULL);

	/** Creates texture
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	static TextureRef Create(const Vec2F& size, TextureFormat format = TextureFormat::Default,
							 TextureUsage usage = TextureUsage::Default);

	/** Creates texture from image. */
	static TextureRef CreateFromBitmap(Bitmap* image);

	/** Creates texture from file. */
	static TextureRef CreateFromFile(const String& fileName);

	/** Creates texture as render target. */
	static TextureRef CreateAsRenderTarget(const Vec2F& size, TextureFormat format = TextureFormat::Default);
};

CLOSE_O2_NAMESPACE