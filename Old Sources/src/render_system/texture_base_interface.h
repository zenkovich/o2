#pragma once

#include "public.h"
#include "util/math/vector2.h"
#include "util/ref_object.h"

OPEN_O2_NAMESPACE

class RenderSystem;
class Bitmap;

/** Texture format. */
enum class TextureFormat { Default, R8G8B8A8, R8G8B8 };

/** Texture usage. */
enum class TextureUsage { Default, RenderTarget };

class TextureRef;

/** Texture interface. Containing size, format, usage, file name. */
class ITexture
{
protected:
	friend class RenderSystemBaseInterface;
	friend class TextureRef;

	Vec2F         mSize;     /**< Size of texture. */
	TextureFormat mFormat;   /**< Texture format. */
	TextureUsage  mUsage;    /**< Texture usage. */
	String        mFileName; /**< Texture file name. */

protected:
	/** ctor. */
	ITexture();

	/** dtor. */
	virtual ~ITexture();

public:
	/** Returns file name. */
	const String& GetFileName() const;

	/** Returns format of texture. */
	TextureFormat GetFormat() const;

	/** Returns usage of texture. */
	TextureUsage GetUsage() const;

	/** Returns size of texture. */
	Vec2F GetSize() const;
};


/** Texture definition base interface, derives from texture interface and reference counter. */
class TextureBaseInterface: public ITexture, public IRefObject
{
protected:
	/** ctor. */
	TextureBaseInterface();

	/** dtor. */
	~TextureBaseInterface();

	/** Creates texture
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	virtual void Create(const Vec2F& size, TextureFormat format = TextureFormat::Default,
						TextureUsage usage = TextureUsage::Default) = 0;

	/** Creates texture from image. */
	virtual void CreateFromBitmap(Bitmap* bitmap) = 0;

	/** Creates texture from file. */
	virtual void CreateFromFile(const String& fileName) = 0;

	/** Creates texture as render target. */
	virtual void CreateAsRenderTarget(const Vec2F& size, TextureFormat format = TextureFormat::Default) = 0;

	/** Runs when reference counter sets to zero. Inherited from IRefCounter. */
	void OnZeroRefCount();
};

CLOSE_O2_NAMESPACE