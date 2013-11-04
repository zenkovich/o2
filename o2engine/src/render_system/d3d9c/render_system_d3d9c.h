#ifndef RENDER_SYSTEM_D3D9C_H
#define RENDER_SYSTEM_D3D9C_H

#include "render_system/render_system_base_interface.h"

#include "d3d9c.h"

#include "util/math/vector2.h"
#include "util/math/vertex.h"

OPEN_O2_NAMESPACE

//simple d3d vertex format
#define D3DFVF_VERTEX_2D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2)

/** Render system, using D3D9C. */
class grRenderSystem:public grRenderSystemBaseInterface
{	
	friend class cApplication;
	friend class grSprite;
	friend class grRenderTarget;
	friend class cDeviceInfo;
	friend class grTexture;
	
	static const unsigned int mVertexBufferSize = 12000;  /** Maximum size of vertex buffer. */
	static const unsigned int mIndexBufferSize = 12000*3; /** Maximum size of index buffer. */

//d3d context
	LPDIRECT3D9             mDirect3D;               /**< o_O */
	LPDIRECT3DDEVICE9       mDirect3DDevice;         /**< Direct 3d device. */
	D3DPRESENT_PARAMETERS   mDirect3DParametr;       /**< Direct3d present parametres. */
	LPDIRECT3DSURFACE9      mBackBufferSurface;      /**< Backbuffer surface. */
	LPDIRECT3DSURFACE9      mStencilBufferSurface;   /**< Stencil buffer surface. */
						    
	bool                    mRenderTargetsAvailable; /**< True, if render targets is available. */
	vec2i                   mMaxTextureSize;         /**< Max texture size. */
					        				   
//vertex & index buffers	  			   
	unsigned char*          mVertexData;             /**< Vertex data buffer. */
	unsigned short*         mVertexIndexData;        /**< Index data buffer. */
	LPDIRECT3DVERTEXBUFFER9 mVertexBuffer;           /**< Vertex buffer object. */
	LPDIRECT3DINDEXBUFFER9  mIndexBuffer;            /**< index buffer object. */
	D3DPRIMITIVETYPE        mCurrentPrimitiveType;   /**< TYpe of drawing primitives for next DIP. */
						      				     
//batching parametres 	     				     
	grTexture*              mLastDrawTexture;        /**< Stored texture ptr from last DIP. */
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
	fRect                   mScissorRect;            /**< Scissor rect, in screen space. */
	bool                    mScissorTest;            /**< True, if scissor test enabled. */
					         					
//other			            
	grRenderTarget*         mCurrentRenderTarget;    /**< Current render target. NULL if rendering in back buffer. */
				            
	bool                    mReady;                  /**< True, if render system initialized. */

public:
	/* ctor. */
	grRenderSystem(cApplication* application);
	
	/** dtor. */ 
	~grRenderSystem();
	
	/** Beginning rendering. */
	bool beginRender();
	
	/** Finishing rendering. */
	bool endRender();
	
	/** Clearing current frame buffer with color. */
	void clear(const color4& color = color4(0, 0, 0, 255));
	
	/** Beginning render to stencil buffer. */
	void beginRenderToStencilBuffer();
	
	/** Finishing rendering in stencil buffer. */
	void endRenderToStencilBuffer();
	
	/** Enabling stencil test. */
	void enableStencilTest();
	
	/** Disabling stencil test. */
	void disableStencilTest();
	
	/** Returns true, if stencil test enabled. */
	bool isStencilTestEnabled() const;
	
	/** Clearing stencil buffer. */
	void clearStencil();
	
	/** Sets scissor rect. */
	void setupScissorRect(const fRect& rect);
	
	/** Returns scissor rect. */
	const fRect& getScissorRect() const;
	
	/** Enabling scissor test. */
	void enableScissorTest();
	
	/** Disabling scissor test. */
	void disableScissorTest();
	
	/** Returns true, if scissor test enabled. */
	bool isScissorTestEnabled() const;
	
	/** Drawing mesh. */
	bool drawMesh(grMesh* mesh);
	
	/** Drawing lines. */
	bool drawLines(vertex2* verticies, int count);
	
	/** Sets lines width. */
	void setLinesWidth(float width);
	
	/** Binding render target. */
	bool bindRenderTarget(grRenderTarget* renderTarget);
	
	/** Unbinding render target. */
	bool unbindRenderTarget();
	
	/** Returns current render target. Returns NULL if no render target. */
	grRenderTarget* getCurrentRenderTarget() const;	

	/** Returns true, if render target is can be used with current device. */
	bool isRenderTargetAvailable() const;

	/** Returns maximum texture size. */
	vec2i getMaxTextureSize() const;

protected:
	/** Initializing direct 3d graphics. */
	void initializeD3D();

	/** Deinitializing direct 3d.*/
	void deinitializeD3D();

	/** Locking vertex & index buffers. */
	void lockBuffers();

	/** unlocking vertex & index buffers. */
	void unlockBuffers();

	/** Drawing primitives. */
	void drawPrimitives();

	/** Calls for update camera transformations. */
	void updateCameraTransforms();
	
	/** Calls when frame changed client size. */
	void frameResized();

	/** Set's viewport and projection matrix size. */
	void setupMatrix(const vec2f& size);

	/** Checking capatibles on this device. */
	void checkCapatibles();
};

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_D3D9C_H