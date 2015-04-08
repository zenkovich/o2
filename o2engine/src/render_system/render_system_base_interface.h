#pragma once

#include "public.h"
#include "util/math/color.h"
#include "util/math/rect.h"
#include "util/math/vector2.h"
#include "util/math/vertex.h"
#include "util/property.h"

#include "camera.h"
#include "font_manager.h"
#include "texture_base_interface.h"

OPEN_O2_NAMESPACE

class LogStream;
class Mesh;
class RenderTarget;
class TextureRef;
class Texture;

/** Render system base interface. Containing resolution of render frame, textures, camera and log. */
class RenderSystemBaseInterface
{
	friend class DeviceInfo;
	friend class FontManager;
	friend class RenderTargetBaseInterface;
	friend class Texture;
	friend class TextureBaseInterface;

public:
	typedef Array< Texture* > TexturesArr;

protected:
	Vec2I        mResolution;    /**< Resolution of rendering frame. */
	TexturesArr  mTextures;      /**< Textures array. */
	FontManager* mFontManager;   /**< Font manager. */
	Camera*      mCurrentCamera; /**< Current camera. Null if standard camera. */
	LogStream*   mLog;           /**< Log stream for render messages. */

public:
	//properties
	PROPERTY(RenderSystemBaseInterface, Camera*) camera; /** Camera property. Uses bindCamera and currentCamera. */


	/** ctor. */
	RenderSystemBaseInterface();

	/** dtor. */
	virtual ~RenderSystemBaseInterface();

	/** Returns resolution of rendering frame. */
	Vec2I GetResolution() const;

	/** Binding camera. NULL - standard camera. */
	void BindCamera(Camera* camera);

	/** Returns current camera. */
	Camera* CurrentCamera() const;

	/** Returns font manager. */
	FontManager* GetFontManager() const;

	/** Creating texture, if no exist, else returning created texture. */
	TextureRef GetTextureFromFile(const String& fileName);

	/** Creates texture
	 *  @size - size of texture
	 *  @format - texture format
	 *  @usage - texture usage. */
	TextureRef CreateTexture(const Vec2F& size, TextureFormat format = TextureFormat::Default,
							 TextureUsage usage = TextureUsage::Default);

	/** Creates texture from image. */
	TextureRef CreateTextureFromBitmap(Bitmap* image);

	/** Creates texture as render target.
	 ** note: recommending to use grRenderTarget for rendering to texture*/
	TextureRef CreateRenderTargetTexture(const Vec2F& size, TextureFormat format = TextureFormat::Default);

	/** Draw single line with color. */
	void DrawLine(const Vec2F& a, const Vec2F& b, const Color4 color = Color4::White());

	/** Draw rect frame with color. */
	void DrawRectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4 color = Color4::White());

	/** Draw cross with color. */
	void DrawCross(const Vec2F& pos, float size = 5, const Color4 color = Color4::White());

	/** Draw circle with color. */
	void DrawCircle(const Vec2F& pos, float radius = 5, const Color4 color = Color4::White());

	/** Beginning rendering. */
	virtual bool BeginRender() = 0;

	/** Finishing rendering. */
	virtual bool EndRender() = 0;

	/** Clearing current frame buffer with color. */
	virtual void Clear(const Color4& color = Color4::Black()) = 0;

	/** Beginning render to stencil buffer. */
	virtual void BeginRenderToStencilBuffer() = 0;

	/** Finishing rendering in stencil buffer. */
	virtual void EndRenderToStencilBuffer() = 0;

	/** Enabling stencil test. */
	virtual void EnableStencilTest() = 0;

	/** Disabling stencil test. */
	virtual void DisableStencilTest() = 0;

	/** Returns true, if stencil test enabled. */
	virtual bool IsStencilTestEnabled() const = 0;

	/** Clearing stencil buffer. */
	virtual void ClearStencil() = 0;

	/** Sets scissor rect. */
	virtual void SetupScissorRect(const RectF& rect) = 0;

	/** Returns scissor rect. */
	virtual const RectF& GetScissorRect() const = 0;

	/** Enabling scissor test. */
	virtual void EnableScissorTest() = 0;

	/** Disabling scissor test. */
	virtual void DisableScissorTest() = 0;

	/** Returns true, if scissor test enabled. */
	virtual bool IsScissorTestEnabled() const = 0;

	/** Drawing mesh. */
	virtual bool DrawMesh(Mesh* mesh) = 0;

	/** Drawing lines. */
	virtual bool DrawLines(Vertex2* verticies, int count) = 0;

	/** Sets lines width. */
	virtual void SetLinesWidth(float width) = 0;

	/** Binding render target. */
	virtual bool BindRenderTarget(RenderTarget* renderTarget) = 0;

	/** Unbinding render target. */
	virtual bool UnbindRenderTarget() = 0;

	/** Returns current render target. Returns NULL if no render target. */
	virtual RenderTarget* GetCurrentRenderTarget() const = 0;

	/** Returns true, if render target is can be used with current device. */
	virtual bool IsRenderTargetAvailable() const = 0;

	/** Returns maximum texture size. */
	virtual Vec2I GetMaxTextureSize() const = 0;

protected:
	/** Initializing properties. */
	void InitializeProperties();

	/** Calls for update camera transformations. */
	virtual void UpdateCameraTransforms() = 0;

	/** Calls when frame changed client size. */
	virtual void FrameResized() = 0;

	/** Adding texture an array and return pointer. */
	Texture* AddTexture(Texture* texture);

	/** Removes texture. */
	void RemoveTexture(Texture* texture);

	/** Removes all textures. */
	void RemoveAllTextures();
};

CLOSE_O2_NAMESPACE