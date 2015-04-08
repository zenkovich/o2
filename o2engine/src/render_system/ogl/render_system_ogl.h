#pragma once

#include "render_system/render_system_base_interface.h"

#include "ogl.h"
#include "render_system/ogl/texture_ogl.h"
#include "util/math/vector2.h"
#include "util/math/vertex.h"

OPEN_O2_NAMESPACE

/** Render system, using OpenGL. */
class RenderSystem: public RenderSystemBaseInterface
{
	friend class Application;
	friend class Sprite;
	friend class RenderTarget;
	friend class Font;
	friend class DeviceInfo;

	static const unsigned int mVertexBufferSize = 6000;  /** Maximum size of vertex buffer. */
	static const unsigned int mIndexBufferSize = 6000*3; /** Maximum size of index buffer. */

	//opengl context
	HGLRC           mGLContext;              /**< OpenGL context. */
	HDC             mHDC;                    /**< Win frame device context. */

	bool            mRenderTargetsAvailable; /**< True, if render targets is available. */
	Vec2I           mMaxTextureSize;         /**< Max texture size. */

	//vertex & index buffers				   
	unsigned char*  mVertexData;             /**< Vertex data buffer. */
	unsigned short* mVertexIndexData;        /**< Index data buffer. */
	GLenum          mCurrentPrimitiveType;   /**< TYpe of drawing primitives for next DIP. */

	//batching parameters					     
	Texture*   mLastDrawTexture;        /**< Stored texture ptr from last DIP. */
	unsigned int    mLastDrawVertex;         /**< Last vertex idx for next DIP. */
	unsigned int    mLastDrawIdx;            /**< Last vertex index for next DIP. */
	unsigned int    mTrianglesCount;         /**< Triangles count for next DIP. */
	unsigned int    mFrameTrianglesCount;    /**< Total triangles at current frame. */
	unsigned int    mDIPCount;               /**< DrawIndexedPrimitives calls count. */

	//stencil			 
	bool            mStencilDrawing;         /**< True, if drawing in stencil buffer. */
	bool            mStencilTest;            /**< True, if drawing with stencil test. */

	//scissor test		 
	RectF           mScissorRect;            /**< Scissor rect, in screen space. */
	bool            mScissorTest;            /**< True, if scissor test enabled. */

	//other
	RenderTarget*   mCurrentRenderTarget;    /**< Current render target. NULL if rendering in back buffer. */

	bool            mReady;                  /**< True, if render system initialized. */

public:
	/* ctor. */
	RenderSystem();

	/** dtor. */
	~RenderSystem();

	/** Beginning rendering. */
	bool BeginRender();

	/** Finishing rendering. */
	bool EndRender();

	/** Clearing current frame buffer with color. */
	void Clear(const Color4& color = Color4::Black());

	/** Beginning render to stencil buffer. */
	void BeginRenderToStencilBuffer();

	/** Finishing rendering in stencil buffer. */
	void EndRenderToStencilBuffer();

	/** Enabling stencil test. */
	void EnableStencilTest();

	/** Disabling stencil test. */
	void DisableStencilTest();

	/** Returns true, if stencil test enabled. */
	bool IsStencilTestEnabled() const;

	/** Clearing stencil buffer. */
	void ClearStencil();

	/** Sets scissor rect. */
	void SetupScissorRect(const RectF& rect);

	/** Returns scissor rect. */
	const RectF& GetScissorRect() const;

	/** Enabling scissor test. */
	void EnableScissorTest();

	/** Disabling scissor test. */
	void DisableScissorTest();

	/** Returns true, if scissor test enabled. */
	bool IsScissorTestEnabled() const;

	/** Drawing mesh. */
	bool DrawMesh(Mesh* mesh);

	/** Drawing lines. */
	bool DrawLines(Vertex2* verticies, int count);

	/** Sets lines width. */
	void SetLinesWidth(float width);

	/** Binding render target. */
	bool BindRenderTarget(RenderTarget* renderTarget);

	/** Unbinding render target. */
	bool UnbindRenderTarget();

	/** Returns current render target. Returns NULL if no render target. */
	RenderTarget* GetCurrentRenderTarget() const;

	/** Returns true, if render target is can be used with current device. */
	bool IsRenderTargetAvailable() const;

	/** Returns maximum texture size. */
	Vec2I GetMaxTextureSize() const;

protected:
	/** Calls for update camera transformations. */
	void UpdateCameraTransforms();

	/** Initializing opengl renderer. */
	void InitializeGL();

	/** Destroying opengl renderer. */
	void DeinitializeGL();

	void DrawPrimitives();

	/** Calls when frame changed client size. */
	void FrameResized();

	/** Set's viewport and projection matrix size. */
	void SetupMatrix(const Vec2F& size);

	/** Returns true, if device supports specified extension. */
	static bool IsExtensionSupported(const char *extension);

	/** Checking compatibles on this device. */
	void CheckCompatibles();
};

CLOSE_O2_NAMESPACE