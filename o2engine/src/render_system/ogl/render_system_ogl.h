#ifndef RENDER_SYSTEM_OGL_H
#define RENDER_SYSTEM_OGL_H

#include "render_system/render_system_base_interface.h"

#include "ogl.h"

#include "util/math/vector2.h"
#include "util/math/vertex.h"

OPEN_O2_NAMESPACE

/** Render system, using OpenGL. */
class grRenderSystem:public grRenderSystemBaseInterface
{	
	friend class cApplication;
	friend class grSprite;
	friend class grRenderTarget;
	friend class cDeviceInfo;
	
	static const unsigned int mVertexBufferSize = 6000;  /** Maximum size of vertex buffer. */
	static const unsigned int mIndexBufferSize = 6000*3; /** Maximum size of index buffer. */

//gl context
	HGLRC           mGLContext;              /**< OpenGL context. */
	HDC             mHDC;                    /**< Win frame device context. */

	bool            mRenderTargetsAvailable; /**< True, if render targets is available. */
	vec2i           mMaxTextureSize;         /**< Max texture size. */
										   
//vertex & index buffers				   
	unsigned char*  mVertexData;             /**< Vertex data buffer. */
	unsigned short* mVertexIndexData;        /**< Index data buffer. */
	GLenum          mCurrentPrimitiveType;   /**< TYpe of drawing primitives for next DIP. */
										     
//batching parametres					     
	grTexture*      mLastDrawTexture;        /**< Stored texture ptr from last DIP. */
	unsigned int    mLastDrawVertex;         /**< Last vertex idx for next DIP. */
	unsigned int    mLastDrawIdx;            /**< Last vertex index for nex DIP. */
	unsigned int    mTrianglesCount;         /**< Triatgles count for next DIP. */
	unsigned int    mFrameTrianglesCount;    /**< Total triangles at current frame. */
	unsigned int    mDIPCount;               /**< DrawIndexedPrimitives calls count. */
										     
//stencil
	bool            mStencilDrawing;         /**< True, if drawing in stencil buffer. */
	bool            mStencilTest;            /**< True, if drawing with stencil test. */

//scissor test
	fRect           mScissorRect;            /**< Scissor rect, in screen space. */
	bool            mScissorTest;            /**< True, if scissor test enabled. */
										
//other
	grRenderTarget* mCurrentRenderTarget;    /**< Current render target. NULL if rendering in back buffer. */

	bool            mReady;                  /**< True, if render system initialized. */

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
	/** Calls for update camera transformations. */
	void updateCameraTransforms();

	/** Initializing opengl renderer. */
	void initializeGL();

	/** Destroying opengl renderer. */
	void deinitializeGL();

	void drawPrimitives();
	
	/** Calls when frame changed client size. */
	void frameResized();

	/** Set's viewport and projection matrix size. */
	void setupMatrix(const vec2f& size);

	/** Returns true, if device supports specified extension. */
	static bool isExtensionSupported(const char *extension);

	/** Checking capatibles on this device. */
	void checkCapatibles();
};

CLOSE_O2_NAMESPACE

#endif //RENDER_SYSTEM_OGL_H