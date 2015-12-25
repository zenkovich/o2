#pragma once

#include "render_system/texture_base_interface.h"
#include "ogl.h"

OPEN_O2_NAMESPACE

/** OpenGL texture definition. Contains OGL texture handle, derives from texture definition base interface. */
class Texture:public TextureBaseInterface
{
	friend class RenderSystemBaseInterface;
	friend class RenderSystem;
	friend class RenderTarget;

	GLuint mHandle; /**< Texture handle. */

protected:
	/** ctor. */
	Texture();

	/** dtor. */
	~Texture();

	/** Creates texture
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	void Create(const Vec2F& size, TextureFormat format = TextureFormat::Default,
				TextureUsage usage = TextureUsage::Default);

	/** Creates texture from image. */
	void CreateFromBitmap(Bitmap* image);

	/** Creates texture from file. */
	void CreateFromFile(const String& fileName);

	/** Creates texture as render target.
	 ** note: recommending to use grRenderTarget*/
	void CreateAsRenderTarget(const Vec2F& size, TextureFormat format = TextureFormat::Default);
};

CLOSE_O2_NAMESPACE