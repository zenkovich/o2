#include "public.h"

#ifdef RENDER_D3D9C
#include "render_system_d3d9c.h"

#include "app/application.h"
#include "render_system/camera.h"
#include "render_system/font_manager.h"
#include "render_system/mesh.h"
#include "render_system/render_target.h"
#include "render_system/texture.h"
#include "util/log/log_stream.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

RenderSystem::RenderSystem():
RenderSystemBaseInterface(), mReady(false), mStencilDrawing(false), mStencilTest(false), 
mScissorTest(false)
{
	InitializeD3D();
}

RenderSystem::~RenderSystem()
{
	DeinitializeD3D();
}

void RenderSystem::InitializeD3D()
{
	mReady = false;

	App()->GetContentSize();

	//initializing d3d8 render
	mDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!mDirect3D)
	{
		mLog->Error("Direct3DCreate9 failed!");
		return;
	}

	D3DDISPLAYMODE Display;
	if(FAILED(mDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display)))
	{
		mLog->Error("GetAdapterDisplayMode failed\n");
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
	mDirect3DParametr.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if(FAILED(mDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, App()->mHWnd, 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mDirect3DParametr, &mDirect3DDevice)))
	{
		mLog->Error("CreateDevice failed\n");
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
	if(FAILED(mDirect3DDevice->CreateVertexBuffer(mVertexBufferSize*sizeof(Vertex2),
		D3DUSAGE_DYNAMIC, D3DFVF_VERTEX_2D, D3DPOOL_DEFAULT, &mVertexBuffer, NULL)))
	{
		mLog->Error("Failed to create Direct3D8 Vertex Buffer\n");
		return;
	}

	//create d3d index buffer
	if (FAILED(mDirect3DDevice->CreateIndexBuffer(mIndexBufferSize*sizeof(WORD), D3DUSAGE_DYNAMIC, 
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mIndexBuffer, NULL)))
	{
		mLog->Error("Failed to create Direct3D8 index buffer");
		return;
	}

	//setup d3d buffers
	mDirect3DDevice->SetFVF(D3DFVF_VERTEX_2D);
	mDirect3DDevice->SetStreamSource(0, mVertexBuffer, 0, sizeof(Vertex2));
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
		mLog->Error("failed to create depth stencil surface");
	}

	mDirect3DDevice->SetDepthStencilSurface(mStencilBufferSurface);

	CheckCompatibles();

	mCurrentRenderTarget = NULL;

	mReady = true;

	return;
}

void RenderSystem::DeinitializeD3D()
{	
	if (!mReady)
		return;

	mFontManager->UnloadFonts();

	RemoveAllTextures();

	mVertexBuffer->Release();
	mIndexBuffer->Release();
	mDirect3DDevice->Release();
	mDirect3D->Release();
}

bool RenderSystem::BeginRender()
{
	if (!mReady)
		return false;

	if (FAILED(mDirect3DDevice->BeginScene()))
	{
		mLog->Error("Failed to call mDirect3DDevice->BeginScene");
		return false;
	}

	Clear();	

	//reset batching params
	mLastDrawTexture      = NULL;
	mLastDrawVertex       = 0;
	mLastDrawIdx          = 0;
	mTrianglesCount       = 0;
	mPrimitivesCount      = 0;
	mFrameTrianglesCount  = 0;
	mDIPCount             = 0;
	mCurrentPrimitiveType = D3DPT_TRIANGLELIST;

	SetupMatrix(mResolution.castTo<float>());

	UpdateCameraTransforms();

	LockBuffers();

	return true;
}

void RenderSystem::SetupMatrix( const Vec2F& size )
{	
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX invMatrix, translMatrix, orthoMatrix;

	D3DXMatrixScaling(&invMatrix, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&translMatrix, 0, (float)size.y, 0.0f);
	D3DXMatrixOrthoOffCenterLH(&orthoMatrix, 0, (float)size.x, 0, (float)size.y, 0.0f, 128.0f);

	projectionMatrix = invMatrix*translMatrix*orthoMatrix;

	mDirect3DDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

void RenderSystem::UpdateCameraTransforms()
{	
	UnlockBuffers();
	DrawPrimitives();
	LockBuffers();

	D3DXMATRIX modelMatrix(1, 0, 0, 0,
						   0, 1, 0, 0,
						   0, 0, 1, 0,
						   0, 0, 0, 1 );

	if (mCurrentCamera)
	{
		float cs = cosf(-mCurrentCamera->mRotation), sn = sinf(-mCurrentCamera->mRotation);
		Vec2F scale(1.0f/mCurrentCamera->mScale.x, 1.0f/mCurrentCamera->mScale.y);
		Vec2F pivotOffset = mCurrentCamera->mPivot.scale(mResolution).scale(mCurrentCamera->mScale).rotate(mCurrentCamera->mRotation);
		Vec2F offs = mCurrentCamera->mPosition - pivotOffset;
		float ofx = -offs.x*scale.x, ofy = -offs.y*scale.y;

		modelMatrix.m[0][0] = cs*scale.x;      modelMatrix.m[0][1] = sn*scale.x; 
		modelMatrix.m[1][0] = -sn*scale.y;     modelMatrix.m[1][1] = cs*scale.y; 
		modelMatrix.m[3][0] = cs*ofx - sn*ofy; modelMatrix.m[3][1] = sn*ofx + cs*ofy;
	}

	mDirect3DDevice->SetTransform(D3DTS_VIEW, &modelMatrix);
}

void RenderSystem::LockBuffers()
{
	if (!mReady)
		return;

	if (FAILED(mVertexBuffer->Lock(0, 0, (void**)&mVertexData, D3DLOCK_DISCARD)))
	{
		mLog->Error("Failed to lock d3d vertex buffer\n");
		return;
	}

	if (FAILED(mIndexBuffer->Lock(0, 0, (void**)&mVertexIndexData, D3DLOCK_DISCARD)))
	{
		mLog->Error("Failed to lock d3d vertex buffer\n");
		return;
	}

	mFrameTrianglesCount += mTrianglesCount;
	mLastDrawVertex = mTrianglesCount = mLastDrawIdx = 0;
	mPrimitivesCount = 0;
}

void RenderSystem::UnlockBuffers()
{
	if (mReady)
		return;

	if (FAILED(mVertexBuffer->Unlock()))
	{
		mLog->Error("Failed to unlock vertex buffer");
	}

	if (FAILED(mIndexBuffer->Unlock()))
	{
		mLog->Error("Failed to unlock vertex buffer");
	}
}

void RenderSystem::DrawPrimitives()
{	
	if (mLastDrawVertex < 1)
		return;

	if ((mCurrentRenderTarget && mCurrentRenderTarget->IsReady()) || !mCurrentRenderTarget)
	{
		if (FAILED(mDirect3DDevice->DrawIndexedPrimitive(mCurrentPrimitiveType, 0, 0, mLastDrawVertex, 0, mPrimitivesCount)))
		{
			mLog->Error("Failed call DrawIndexedPrimitive\n");
		}
	}

	mDIPCount++;
}

bool RenderSystem::EndRender()
{	
	if (!mReady)
		return false;

	//flush geometry
	UnlockBuffers();
	DrawPrimitives();

	if (FAILED(mDirect3DDevice->EndScene()))
	{
		mLog->Error("Failed EndScene\n");
		return false;
	}

	mDirect3DDevice->Present(NULL, NULL, NULL, NULL);

	//printf("Triangles: %i, DIP: %i\n", mFrameTrianglesCount, mDIPCount);

	return true;
}

void RenderSystem::Clear( const Color4& color /*= color4(0, 0, 0, 255)*/ )
{
	mDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(color.r, color.g, color.b), 1.0F, 0); 
}

bool RenderSystem::DrawMesh( Mesh* mesh )
{
	if (!mReady)
		return false;

	//check difference
	if (mLastDrawTexture != mesh->mTexture || 
		mLastDrawVertex + mesh->mVertexCount >= mVertexBufferSize ||
		mLastDrawIdx + mesh->mPolyCount*3 >= mIndexBufferSize ||
		mCurrentPrimitiveType == D3DPT_LINELIST)
	{
		UnlockBuffers();
		DrawPrimitives();
		LockBuffers();

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
		Vertex2* v = &((Vertex2*)mVertexData)[i];
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

bool RenderSystem::DrawLines( Vertex2* verticies, int count )
{
	if (!mReady)
		return false;

	//check difference
	if (mCurrentPrimitiveType == D3DPT_TRIANGLELIST)
	{
		UnlockBuffers();
		DrawPrimitives();
		LockBuffers();

		mLastDrawTexture = NULL;
		mCurrentPrimitiveType = D3DPT_LINELIST;
		mDirect3DDevice->SetTexture(0, NULL);
	}

	//copy data
	memcpy(&mVertexData[mLastDrawVertex*sizeof(Vertex2)], verticies, sizeof(Vertex2)*count*2);

	for (unsigned int i = mLastDrawIdx, j = 0; j < (unsigned int)count*2; i++, j++)
	{
		mVertexIndexData[i] = mLastDrawVertex + j;
	}

	mPrimitivesCount += 2;
	mLastDrawVertex += count*2;
	mLastDrawIdx += count*2;

	return true;
}

void RenderSystem::SetLinesWidth( float width )
{

}

void RenderSystem::BeginRenderToStencilBuffer()
{	
	if (mStencilDrawing || mStencilTest)
		return;

	UnlockBuffers();
	DrawPrimitives();
	LockBuffers();

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

void RenderSystem::EndRenderToStencilBuffer()
{
	if (!mStencilDrawing)
		return;

	UnlockBuffers();
	DrawPrimitives();
	LockBuffers();

	mDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, false);

	DWORD rsColorWriteFlag = D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA;
	mDirect3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, rsColorWriteFlag);

	mStencilDrawing = false;
}

void RenderSystem::EnableStencilTest()
{
	if (mStencilTest || mStencilDrawing)
		return;

	UnlockBuffers();
	DrawPrimitives();
	LockBuffers();

	mDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	mDirect3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	mStencilTest = true;
}

void RenderSystem::DisableStencilTest()
{
	if (!mStencilTest)
		return;

	UnlockBuffers();
	DrawPrimitives();
	LockBuffers();

	mDirect3DDevice->SetRenderState(D3DRS_STENCILENABLE, false);

	mStencilTest = false;
}

bool RenderSystem::IsStencilTestEnabled() const
{
	return mStencilTest;
}

void RenderSystem::ClearStencil()
{
	mDirect3DDevice->Clear(0, NULL, D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}

void RenderSystem::SetupScissorRect( const RectF& rect )
{
	mScissorRect = rect;
}

const RectF& RenderSystem::GetScissorRect() const
{
	return mScissorRect;
}

void RenderSystem::EnableScissorTest()
{
	if (mScissorTest)
		return;

	UnlockBuffers();
	DrawPrimitives();
	LockBuffers();

	RECT rt = { (long)mScissorRect.left, (long)mScissorRect.top, (long)mScissorRect.right, (long)mScissorRect.down };
	mDirect3DDevice->SetScissorRect(&rt);
	mDirect3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);

	mScissorTest = true;
}

void RenderSystem::DisableScissorTest()
{
	if (!mScissorTest)
		return;

	UnlockBuffers();
	DrawPrimitives();
	LockBuffers();

	mDirect3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

	mScissorTest = false;
}

bool RenderSystem::IsScissorTestEnabled() const
{	
	return mScissorTest;
}

bool RenderSystem::BindRenderTarget( RenderTarget* renderTarget )
{	
	if (!renderTarget)
		return false;

	UnlockBuffers();
	DrawPrimitives();
	LockBuffers();

	if (renderTarget->IsReady())
	{
		LPDIRECT3DSURFACE9 renderTargetSurface;
		LPDIRECT3DTEXTURE9 renderTargetTexture = renderTarget->mRenderTexture->mTexturePtr;
		if (FAILED(renderTargetTexture->GetSurfaceLevel(0, &renderTargetSurface)))
		{
			mLog->Error("Can't get surface level 0 from  render target texture %x", renderTarget);
			return false;
		}
		if (renderTargetSurface) renderTargetSurface->Release();

		if (FAILED(mDirect3DDevice->SetRenderTarget(0, renderTargetSurface)))
		{
			mLog->Error("Can't setup render target %x: failed SetRenderTarget", renderTarget);
			return false;
		}

		SetupMatrix(renderTarget->GetTexture().getSize());
	}

	mCurrentRenderTarget = renderTarget;

	return true;
}

bool RenderSystem::UnbindRenderTarget()
{	
	if (!mCurrentRenderTarget)
		return false;	

	UnlockBuffers();
	DrawPrimitives();
	LockBuffers();

	if (FAILED(mDirect3DDevice->SetRenderTarget(0, mBackBufferSurface)))
	{
		mLog->Error("Can't setup backbuffer render target %x", mBackBufferSurface);
		return false;
	}

	SetupMatrix(mResolution.castTo<float>());

	mCurrentRenderTarget = NULL;

	return true;
}

RenderTarget* RenderSystem::GetCurrentRenderTarget() const
{
	return mCurrentRenderTarget;
}

bool RenderSystem::IsRenderTargetAvailable() const
{
	return mRenderTargetsAvailable;
}

Vec2I RenderSystem::GetMaxTextureSize() const
{
	return mMaxTextureSize;
}

void RenderSystem::FrameResized()
{	
	App()->GetContentSize();
}

void RenderSystem::CheckCompatibles()
{
	D3DCAPS9 caps;
	mDirect3DDevice->GetDeviceCaps(&caps);
	mMaxTextureSize.x = (int)caps.MaxTextureWidth;
	mMaxTextureSize.y = (int)caps.MaxTextureHeight;
	mRenderTargetsAvailable = true;
}

CLOSE_O2_NAMESPACE

#endif //RENDER_D3D9C