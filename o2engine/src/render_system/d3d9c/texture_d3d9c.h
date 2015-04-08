#pragma once

#include "render_system/texture_base_interface.h"
#include "d3d9c.h"

OPEN_O2_NAMESPACE

/** Direct3D9 texture definition. Contains D3D texture pointer, derives from texture definition base interface. */
class Texture:public TextureBaseInterface
{
	friend class RenderSystemBaseInterface;
	friend class RenderSystem;
	friend class RenderTarget;

	LPDIRECT3DTEXTURE9 mTexturePtr; /**< Direct3D9 texture pointer. */


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

	/** Creates texture as render target. */
	void CreateAsRenderTarget(const Vec2F& size, TextureFormat format = TextureFormat::Default);
};

CLOSE_O2_NAMESPACE