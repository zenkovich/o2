#pragma once

#include "render_system/render_system_base_interface.h"

#include "d3d9c.h"
#include "util/math/vector2.h"
#include "util/math/vertex.h"

OPEN_O2_NAMESPACE

//simple d3d vertex format
#define D3DFVF_VERTEX_2D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2)

/** Render system, using D3D9C. */
class RenderSystem:public RenderSystemBaseInterface
{
	friend class Application;
	friend class Sprite;
	friend class RenderTarget;
	friend class Font;
	friend class DeviceInfo;
	friend class Texture;

	static const uint mVertexBufferSize = 12000;  /** Maximum size of vertex buffer. */
	static const uint mIndexBufferSize = 12000*3; /** Maximum size of index buffer. */

	//d3d context
	LPDIRECT3D9             mDirect3D;               /**< o_O */
	LPDIRECT3DDEVICE9       mDirect3DDevice;         /**< Direct 3d device. */
	D3DPRESENT_PARAMETERS   mDirect3DParametr;       /**< Direct3d present parametres. */
	LPDIRECT3DSURFACE9      mBackBufferSurface;      /**< Backbuffer surface. */
	LPDIRECT3DSURFACE9      mStencilBufferSurface;   /**< Stencil buffer surface. */

	bool                    mRenderTargetsAvailable; /**< True, if render targets is available. */
	Vec2I                   mMaxTextureSize;         /**< Max texture size. */

	//vertex & index buffers	  			   
	unsigned char*          mVertexData;             /**< Vertex data buffer. */
	unsigned short*         mVertexIndexData;        /**< Index data buffer. */
	LPDIRECT3DVERTEXBUFFER9 mVertexBuffer;           /**< Vertex buffer object. */
	LPDIRECT3DINDEXBUFFER9  mIndexBuffer;            /**< index buffer object. */
	D3DPRIMITIVETYPE        mCurrentPrimitiveType;   /**< TYpe of drawing primitives for next DIP. */

	//batching parametres 	     				     
	Texture*           mLastDrawTexture;        /**< Stored texture ptr from last DIP. */
	unsigned int            mLastDrawVertex;         /**< Last vertex idx for next DIP. */
	unsigned int            mLastDrawIdx;            /**< Last vertex index for nex DIP. */
	unsigned int            mTrianglesCount;         /**< Triatgles count for next DIP. */
	unsigned int            mPrimitivesCount;        /**< Primitives count for next DIP. */
	unsigned int            mFrameTrianglesCount;    /**< Total triangles at current frame. */
	unsigned int            mDIPCount;               /**< DrawIndexedPrimitives calls count. */

	//stencil		            
	bool                    mStencilDrawing;         /**< True, if drawing in stencil buffer. */
	bool                    mStencilTest;            /**< True, if drawing with stencil test. */

	//scissor test	            
	RectF                   mScissorRect;            /**< Scissor rect, in screen space. */
	bool                    mScissorTest;            /**< True, if scissor test enabled. */

	//other			            
	RenderTarget*         mCurrentRenderTarget;    /**< Current render target. NULL if rendering in back buffer. */

	bool                    mReady;                  /**< True, if render system initialized. */

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
	/** Initializing direct 3d graphics. */
	void InitializeD3D();

	/** Deinitializing direct 3d.*/
	void DeinitializeD3D();

	/** Locking vertex & index buffers. */
	void LockBuffers();

	/** unlocking vertex & index buffers. */
	void UnlockBuffers();

	/** Drawing primitives. */
	void DrawPrimitives();

	/** Calls for update camera transformations. */
	void UpdateCameraTransforms();

	/** Calls when frame changed client size. */
	void FrameResized();

	/** Set's viewport and projection matrix size. */
	void SetupMatrix(const Vec2F& size);

	/** Checking capatibles on this device. */
	void CheckCompatibles();
};

CLOSE_O2_NAMESPACE