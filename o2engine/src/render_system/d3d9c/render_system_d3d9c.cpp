#include "public.h"

#ifdef RENDER_D3D9C
#include "render_system_d3d9c.h"

#include "../camera.h"
#include "../mesh.h"
#include "../render_target.h"
#include "../texture.h"
#include "app/application.h"
#include "util/log/log_stream.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

grRenderSystem::grRenderSystem( cApplication* application ):
	grRenderSystemBaseInterface(application), mReady(false), mStencilDrawing(false), mStencilTest(false), 
	mScissorTest(false)
{
	initializeD3D();
}

grRenderSystem::~grRenderSystem()
{
	deinitializeD3D();
}

void grRenderSystem::initializeD3D()
{
	mReady = false;
	
	mApplication->getOption(cApplicationOption::CLIENT_RECT, &mResolution);

//initializing d3d8 render
	mDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!mDirect3D)
	{
		mLog->out("ERROR: Direct3DCreate9 failed!");
		return;
	}

	D3DDISPLAYMODE Display;
	if(FAILED(mDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display)))
	{
		mLog->out("ERROR: GetAdapterDisplayMode failed\n");
		return;
	}

	ZeroMemory(&mDirect3DParametr, sizeof(mDirect3DParametr));
	mDirect3DParametr.Windowed=true;
	mDirect3DParametr.SwapEffect = D3DSWAPEFFECT_COPY;
	mDirect3DParametr.BackBufferFormat = Display.Format;
	mDirect3DParametr.BackBufferCount = 1;
	mDirect3DParametr.BackBufferWidth = (unsigned int)(mResolution.x);
	mDirect3DParametr.BackBufferHeight = (unsigned int)(mResolution.y);  
	mDirect3DParametr.EnableAutoDepthStencil = true;
	mDirect3DParametr.AutoDepthStencilFormat = D3DFMT_D16;

	if(FAILED(mDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mApplication->mHWnd, 
		      D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mDirect3DParametr, &mDirect3DDevice)))
	{
		mLog->out("ERROR: CreateDevice failed\n");
		return;
	}

//render states
	mDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	mDirect3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	mDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	mDirect3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	mDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);	
    mDirect3DDevice->SetRenderState(D3DRS_DITHERENABLE, true);
    mDirect3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00000000);	
    mDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    mDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

//texture state
	mDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	mDirect3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	mDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	mDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	mDirect3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	mDirect3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	printf("Direct3d9c render initialized\n");

//create d3d vertex buffer
	if(FAILED(mDirect3DDevice->CreateVertexBuffer(mVertexBufferSize*sizeof(vertex2),
		D3DUSAGE_DYNAMIC, D3DFVF_VERTEX_2D, D3DPOOL_DEFAULT, &mVertexBuffer, NULL)))
	{
		mLog->out("ERROR: Failed to create Direct3D8 Vertex Buffer\n");
		return;
	}

//create d3d index buffer
	if (FAILED(mDirect3DDevice->CreateIndexBuffer(mIndexBufferSize*sizeof(WORD), D3DUSAGE_DYNAMIC, 
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mIndexBuffer, NULL)))
	{
		mLog->out("ERROR: Failed to create Direct3D8 index buffer");
		return;
	}

//setup d3d buffers
	mDirect3DDevice->SetFVF(D3DFVF_VERTEX_2D);
	mDirect3DDevice->SetStreamSource(0, mVertexBuffer, 0, sizeof(vertex2));
	mDirect3DDevice->SetIndices(mIndexBuffer);

//setup 2d orto matricies
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX invMatrix, translMatrix, orthoMatrix;

	D3DXMatrixScaling(&invMatrix, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&translMatrix, 0, (float)mResolution.y, 0.0f);
	D3DXMatrixOrthoOffCenterLH(&orthoMatrix, 0, (float)mResolution.x, 0, (float)mResolution.y, 0.0f, 128.0f);

	projectionMatrix = invMatrix*translMatrix*orthoMatrix;

	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);

	mDirect3DDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
	mDirect3DDevice->SetTransform(D3DTS_WORLD, &identity);
	mDirect3DDevice->SetTransform(D3DTS_VIEW, &identity);

//getting back buffer render target
	mDirect3DDevice->GetRenderTarget(0, &mBackBufferSurface);

//creating stencil buffer
	if (FAILED(mDirect3DDevice->CreateDepthStencilSurface(mResolution.x, mResolution.y, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, false, 
		&mStencilBufferSurface, NULL)))
	{
		mLog->out("ERROR: failed to create depth stencil surface");
	}
	
	mDirect3DDevice->SetDepthStencilSurface(mStencilBufferSurface);

	checkCapatibles();

	mCurrentRenderTarget = NULL;

	mReady = true;

	return;
}

void grRenderSystem::deinitializeD3D()
{	
	if (!mReady)
		return;

	removeAllTextures();
	
	mVertexBuffer->Release();
	mIndexBuffer->Release();
	mDirect3DDevice->Release();
	mDirect3D->Release();
}

bool grRenderSystem::beginRender()
{
	if (!mReady)
		return false;

	if (FAILED(mDirect3DDevice->BeginScene()))
	{
		mLog->out("ERROR: Failed to call mDirect3DDevice->BeginScene");
		return false;
	}

	clear();	

//reset batching params
	mLastDrawTexture      = NULL;
	mLastDrawVertex       = 0;
	mLastDrawIdx          = 0;
	mTrianglesCount       = 0;
	mPrimitivesCount      = 0;
	mFrameTrianglesCount  = 0;
	mDIPCount             = 0;
	mCurrentPrimitiveType = D3DPT_TRIANGLELIST;

	setupMatrix(mResolution.castTo<float>());

	updateCameraTransforms();

	lockBuffers();

	return true;
}

void grRenderSystem::setupMatrix( const vec2f& size )
{	
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX invMatrix, translMatrix, orthoMatrix;

	D3DXMatrixScaling(&invMatrix, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&translMatrix, 0, (float)size.y, 0.0f);
	D3DXMatrixOrthoOffCenterLH(&orthoMatrix, 0, (float)size.x, 0, (float)size.y, 0.0f, 128.0f);

	projectionMatrix = invMatrix*translMatrix*orthoMatrix;

	mDirect3DDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

void grRenderSystem::updateCameraTransforms()
{	
	unlockBuffers();
	drawPrimitives();
	lockBuffers();

	D3DXMATRIX modelMatrix(1, 0, 0, 0,
		                   0, 1, 0, 0,
						   0, 0, 1, 0,
						   0, 0, 0, 1 );

	if (mCurrentCamera)
	{
		float cs = cosf(-mCurrentCamera->mRotation), sn = sinf(-mCurrentCamera->mRotation);
		vec2f scale(1.0f/mCurrentCamera->mScale.x, 1.0f/mCurrentCamera->mScale.y), offs = mCurrentCamera->mPosition;
		float ofx = -offs.x*scale.x, ofy = -offs.y*scale.y;
		
		modelMatrix.m[0][0] = cs*scale.x;      modelMatrix.m[0][1] = sn*scale.x; 
		modelMatrix.m[1][0] = -sn*scale.y;     modelMatrix.m[1][1] = cs*scale.y; 
		modelMatrix.m[3][0] = cs*ofx - sn*ofy; modelMatrix.m[3][1] = sn*ofx + cs*ofy;
	}

	mDirect3DDevice->SetTransform(D3DTS_VIEW, &modelMatrix);
}

void grRenderSystem::lockBuffers()
{
	if (!mReady)
		return;

	if (FAILED(mVertexBuffer->Lock(0, 0, (void**)&mVertexData, D3DLOCK_DISCARD)))
	{
		mLog->out("ERROR: Failed to lock d3d vertex buffer\n");
		return;
	}

	if (FAILED(mIndexBuffer->Lock(0, 0, (void**)&mVertexIndexData, D3DLOCK_DISCARD)))
	{
		mLog->out("ERROR: Failed to lock d3d vertex buffer\n");
		return;
	}

	mFrameTrianglesCount += mTrianglesCount;
	mLastDrawVertex = mTrianglesCount = mLastDrawIdx = 0;
	mPrimitivesCount = 0;
}

void grRenderSystem::unlockBuffers()
{
	if (mReady)
		return;

	if (FAILED(mVertexBuffer->Unlock()))
	{
		mLog->out("ERROR: Failed to unlock vertex buffer");
	}

	if (FAILED(mIndexBuffer->Unlock()))
	{
		mLog->out("ERROR: Failed to unlock vertex buffer");
	}
}

void grRenderSystem::drawPrimitives()
{	
	if (mLastDrawVertex < 1)
		return;

	if ((mCurrentRenderTarget && mCurrentRenderTarget->isReady()) || !mCurrentRenderTarget)
	{
		if (FAILED(mDirect3DDevice->DrawIndexedPrimitive(mCurrentPrimitiveType, 0, 0, mLastDrawVertex, 0, mPrimitivesCount)))
		{
			mLog->out("ERROR: Failed call DrawIndexedPrimitive\n");
		}
	}

	mDIPCount++;
}

bool grRenderSystem::endRender()
{	
	if (!mReady)
		return false;

//flush geometry
	unlockBuffers();
	drawPrimitives();

	if (FAILED(mDirect3DDevice->EndScene()))
	{
		mLog->out("ERROR: Failed EndScene\n");
		return false;
	}

	mDirect3DDevice->Present(NULL, NULL, NULL, NULL);

	//printf("Triangles: %i, DIP: %i\n", mFrameTrianglesCount, mDIPCount);

	return true;
}

void grRenderSystem::clear( const color4& color /*= color4(0, 0, 0, 255)*/ )
{
	mDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(color.r, color.g, color.b), 1.0F, 0); 
}

bool grRenderSystem::drawMesh( grMesh* mesh )
{
	if (!mReady)
		return false;
	
//check difference
	if (mLastDrawTexture != mesh->mTexture || 
		mLastDrawVertex + mesh->mVertexCount >= mVertexBufferSize ||
		mLastDrawIdx + mesh->mPolyCount*3 >= mIndexBufferSize ||
		mCurrentPrimitiveType == D3DPT_LINELIST)
	{
		unlockBuffers();
		drawPrimitives();
		lockBuffers();

		mLastDrawTexture = mesh->mTexture;
		mCurrentPrimitiveType = D3DPT_TRIANGLELIST;

		if (mLastDrawTexture) 
		{			
			mDirect3DDevice->SetTexture(0, mLastDrawTexture->mTexturePtr);
		}
		else          
		{
			mDirect3DDevice->SetTexture(0, NULL);
		}
	}

//copy data
	//memcpy(&mVertexData[mLastDrawVertex*sizeof(vertex2)], mesh->mVerticies, sizeof(vertex2)*mesh->mVertexCount);
	for (unsigned int i = mLastDrawVertex, j = 0; j < mesh->mVertexCount; j++, i++)
	{
		vertex2* v = &((vertex2*)mVertexData)[i];
		*v = mesh->mVerticies[j];
		//v->tv = 1.0f - v->tv;
	}

	for (unsigned int i = mLastDrawIdx, j = 0; j < mesh->mPolyCount*3; i++, j++)
	{
		mVertexIndexData[i] = mLastDrawVertex + mesh->mIndexes[j];
	}

	mTrianglesCount += mesh->mPolyCount;
	mPrimitivesCount += mesh->mPolyCount;
	mLastDrawVertex += mesh->mVertexCount;
	mLastDrawIdx += mesh->mPolyCount*3;

	return true;
}

bool grRenderSystem::drawLines( vertex2* verticies, int count )
{
	if (!mReady)
		return false;
	
//check difference
	if (mCurrentPrimitiveType == D3DPT_TRIANGLELIST)
	{
		unlockBuffers();
		drawPrimitives();
		lockBuffers();

		mLastDrawTexture = NULL;
		mCurrentPrimitiveType = D3DPT_LINELIST;
		mDirect3DDevice->SetTexture(0, NULL);
	}

//copy data
	memcpy(&mVertexData[mLastDrawVertex*sizeof(vertex2)], verticies, sizeof(vertex2)*count*2);

	for (unsigned int i = mLastDrawIdx, j = 0; j < (unsigned int)count*2; i++, j++)
	{
		mVertexIndexData[i] = mLastDrawVertex + j;
	}

	mPrimitivesCount += 2;
	mLastDrawVertex += count*2;
	mLastDrawIdx += count*2;

	return true;
}

void grRenderSystem::setLinesWidth( float width )
{

}

void grRenderSystem::beginRenderToStencilBuffer()
{	
	if (mStencilDrawing || mStencilTest)
		return;

	unlockBuffers();
	drawPrimitives();
	lockBuffers();

	mDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	mDirect3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	mDirect3DDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
	mDirect3DDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	mDirect3DDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	mDirect3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	mDirect3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	mDirect3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

	DWORD rsColorWriteFlag = 0;

	mDirect3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, rsColorWriteFlag);

	mStencilDrawing = true;
}

void grRenderSystem::endRenderToStencilBuffer()
{
	if (!mStencilDrawing)
		return;

	unlockBuffers();
	drawPrimitives();
	lockBuffers();

	mDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, false);

	DWORD rsColorWriteFlag = D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		                     D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA;
	mDirect3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, rsColorWriteFlag);

	mStencilDrawing = false;
}

void grRenderSystem::enableStencilTest()
{
	if (mStencilTest || mStencilDrawing)
		return;

	unlockBuffers();
	drawPrimitives();
	lockBuffers();

	mDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	mDirect3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	mStencilTest = true;
}

void grRenderSystem::disableStencilTest()
{
	if (!mStencilTest)
		return;

	unlockBuffers();
	drawPrimitives();
	lockBuffers();

	mDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, false);

	mStencilTest = false;
}

bool grRenderSystem::isStencilTestEnabled() const
{
	return mStencilTest;
}

void grRenderSystem::clearStencil()
{
	mDirect3DDevice->Clear(0, NULL, D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}

void grRenderSystem::setupScissorRect( const fRect& rect )
{
	mScissorRect = rect;
}

const fRect& grRenderSystem::getScissorRect() const
{
	return mScissorRect;
}

void grRenderSystem::enableScissorTest()
{
	if (mScissorTest)
		return;

	unlockBuffers();
	drawPrimitives();
	lockBuffers();
	
	RECT rt = { (long)mScissorRect.left, (long)mScissorRect.top, (long)mScissorRect.right, (long)mScissorRect.down };
	mDirect3DDevice->SetScissorRect(&rt);
	mDirect3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);

	mScissorTest = true;
}

void grRenderSystem::disableScissorTest()
{
	if (!mScissorTest)
		return;

	unlockBuffers();
	drawPrimitives();
	lockBuffers();

	mDirect3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

	mScissorTest = false;
}

bool grRenderSystem::isScissorTestEnabled() const
{	
	return mScissorTest;
}

bool grRenderSystem::bindRenderTarget( grRenderTarget* renderTarget )
{	
	if (!renderTarget)
		return false;

	unlockBuffers();
	drawPrimitives();
	lockBuffers();

	if (renderTarget->isReady())
	{
		LPDIRECT3DSURFACE9 renderTargetSurface;
		LPDIRECT3DTEXTURE9 renderTargetTexture = renderTarget->mRenderTexture->mTexturePtr;
		if (FAILED(renderTargetTexture->GetSurfaceLevel(0, &renderTargetSurface)))
		{
			mLog->out("ERROR: Can't get surface level 0 from  render target texture %x", renderTarget);
			return false;
		}
		if (renderTargetSurface) renderTargetSurface->Release();

		if (FAILED(mDirect3DDevice->SetRenderTarget(0, renderTargetSurface)))
		{
			mLog->out("ERROR: Can't setup render target %x: failed SetRenderTarget", renderTarget);
			return false;
		}

		setupMatrix(renderTarget->getTexture()->getSize());
	}

	mCurrentRenderTarget = renderTarget;

	return true;
}

bool grRenderSystem::unbindRenderTarget()
{	
	if (!mCurrentRenderTarget)
		return false;	
	
	unlockBuffers();
	drawPrimitives();
	lockBuffers();

	if (FAILED(mDirect3DDevice->SetRenderTarget(0, mBackBufferSurface)))
	{
		mLog->out("ERROR: Can't setup backbuffer render target %x", mBackBufferSurface);
		return false;
	}

	setupMatrix(mResolution.castTo<float>());

	mCurrentRenderTarget = NULL;

	return true;
}

grRenderTarget* grRenderSystem::getCurrentRenderTarget() const
{
	return mCurrentRenderTarget;
}

bool grRenderSystem::isRenderTargetAvailable() const
{
	return mRenderTargetsAvailable;
}

vec2i grRenderSystem::getMaxTextureSize() const
{
	return mMaxTextureSize;
}

void grRenderSystem::frameResized()
{	
	mApplication->getOption(cApplicationOption::CLIENT_RECT, &mResolution);
}

void grRenderSystem::checkCapatibles()
{
	D3DCAPS9 caps;
	mDirect3DDevice->GetDeviceCaps(&caps);
	mMaxTextureSize.x = (int)caps.MaxTextureWidth;
	mMaxTextureSize.y = (int)caps.MaxTextureHeight;
	mRenderTargetsAvailable = true;
}

CLOSE_O2_NAMESPACE

#endif //RENDER_D3D9C