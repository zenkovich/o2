#include "public.h"

#ifdef RENDER_OGL

#include "app/application.h"
#include "ogl_debug.h"
#include "render_system/camera.h"
#include "render_system/font_manager.h"
#include "render_system/mesh.h"
#include "render_system/render_target.h"
#include "render_system/texture.h"
#include "render_system_ogl.h"
#include "util/log/log_stream.h"
#include "util/math/math.h"

#ifdef PLATFORM_WIN
#include "ogl_ext_win.h"
#endif // PLATFORM_WIN

OPEN_O2_NAMESPACE

RenderSystem::RenderSystem():
RenderSystemBaseInterface(), mReady(false), mStencilDrawing(false), mStencilTest(false),
mScissorTest(false)
{
	InitializeGL();
}

RenderSystem::~RenderSystem()
{
	DeinitializeGL();
}

void RenderSystem::InitializeGL()
{
	mLog->Out("Initializing OpenGL render..");

	mResolution = App()->GetContentSize();

#ifdef PLATFORM_WIN
	GLuint pixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,  										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		1,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	mHDC = GetDC(App()->mHWnd);
	if (!mHDC)
	{
		mLog->Error("Can't Create A GL Device Context.\n");
		return;
	}

	pixelFormat = ChoosePixelFormat(mHDC, &pfd);
	if (!pixelFormat)
	{
		mLog->Error("Can't Find A Suitable PixelFormat.\n");
		return;
	}

	if (!SetPixelFormat(mHDC, pixelFormat, &pfd))
	{
		mLog->Error("Can't Set The PixelFormat.\n");
		return;
	}

	mGLContext = wglCreateContext(mHDC);
	if (!mGLContext)
	{
		mLog->Error("Can't Create A GL Rendering Context.\n");
		return;
	}

	if (!wglMakeCurrent(mHDC, mGLContext))
	{
		mLog->Error("Can't Activate The GL Rendering Context.\n");
		return;
	}

	//get opengl extensions
	getGLExtensions(mLog);

	GL_CHECK_ERROR(mLog);

	CheckCompatibles();

#endif //PLATFORM_WIN

	mVertexData = mnew unsigned char[mVertexBufferSize*sizeof(Vertex2)];

	mVertexIndexData = mnew unsigned short[mIndexBufferSize];
	mLastDrawVertex = 0;
	mTrianglesCount = 0;
	mCurrentPrimitiveType = GL_TRIANGLES;

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex2), mVertexData + sizeof(float)*3);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex2), mVertexData + sizeof(float)*3 + sizeof(unsigned long));
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex2), mVertexData + 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GL_CHECK_ERROR(mLog);

	mLog->Out("GL_VENDOR: %s", glGetString(GL_VENDOR));
	mLog->Out("GL_RENDERER: %s", glGetString(GL_RENDERER));
	mLog->Out("GL_VERSION: %s", glGetString(GL_VERSION));

	glLineWidth(1.0f);

	mCurrentRenderTarget = NULL;

	mReady = true;
}

bool RenderSystem::BeginRender()
{
	if (!mReady)
		return false;

	//reset batching params
	mLastDrawTexture      = NULL;
	mLastDrawVertex       = 0;
	mLastDrawIdx          = 0;
	mTrianglesCount       = 0;
	mFrameTrianglesCount  = 0;
	mDIPCount             = 0;
	mCurrentPrimitiveType = GL_TRIANGLES;

	SetupMatrix(mResolution.castTo<float>());

	UpdateCameraTransforms();

	Clear();

	return true;
}

void RenderSystem::DrawPrimitives()
{
	if (mLastDrawVertex < 1)
		return;

	if ((mCurrentRenderTarget && mCurrentRenderTarget->mReady) || !mCurrentRenderTarget)
		glDrawElements(mCurrentPrimitiveType, mLastDrawIdx, GL_UNSIGNED_SHORT, mVertexIndexData);

	GL_CHECK_ERROR(mLog);

	mFrameTrianglesCount += mTrianglesCount;
	mLastDrawVertex = mTrianglesCount = mLastDrawIdx = 0;

	mDIPCount++;
}

bool RenderSystem::EndRender()
{
	if (!mReady)
		return false;

	//flush geometry
	DrawPrimitives();

	SwapBuffers(mHDC);

	GL_CHECK_ERROR(mLog);

	return true;
}

bool RenderSystem::DrawMesh(Mesh* mesh)
{
	if (!mReady)
		return false;

	//check difference
	if (mLastDrawTexture != mesh->mTexture ||
		mLastDrawVertex + mesh->mVertexCount >= mVertexBufferSize ||
		mLastDrawIdx + mesh->mPolyCount*3 >= mIndexBufferSize ||
		mCurrentPrimitiveType == GL_LINES)
	{
		DrawPrimitives();

		mLastDrawTexture = mesh->mTexture;
		mCurrentPrimitiveType = GL_TRIANGLES;

		if (mLastDrawTexture)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mLastDrawTexture->mHandle);

			GL_CHECK_ERROR(mLog);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
	}

	//copy data
	//memcpy(&mVertexData[mLastDrawVertex*sizeof(vertex2)], mesh->mVerticies, sizeof(vertex2)*mesh->mVertexCount);
	for (unsigned int i = mLastDrawVertex, j = 0; j < mesh->mVertexCount; j++, i++)
	{
		Vertex2* v = &((Vertex2*)mVertexData)[i];
		*v = mesh->mVerticies[j];
		v->tv = 1.0f - v->tv;
	}

	for (unsigned int i = mLastDrawIdx, j = 0; j < mesh->mPolyCount*3; i++, j++)
	{
		mVertexIndexData[i] = mLastDrawVertex + mesh->mIndexes[j];
	}

	mTrianglesCount += 2;
	mLastDrawVertex += mesh->mVertexCount;
	mLastDrawIdx += mesh->mPolyCount*3;

	return true;
}

void RenderSystem::SetLinesWidth(float width)
{
	glLineWidth(width);
}

bool RenderSystem::DrawLines(Vertex2* verticies, int count)
{
	if (!mReady)
		return false;

	//check difference
	if (mCurrentPrimitiveType == GL_TRIANGLES ||
		mLastDrawVertex + count*2 >= mVertexBufferSize ||
		mLastDrawIdx + count*2 >= mIndexBufferSize)
	{
		DrawPrimitives();

		mLastDrawTexture = NULL;
		mCurrentPrimitiveType = GL_LINES;
		glDisable(GL_TEXTURE_2D);
	}

	//copy data
	memcpy(&mVertexData[mLastDrawVertex*sizeof(Vertex2)], verticies, sizeof(Vertex2)*count*2);

	for (unsigned int i = mLastDrawIdx, j = 0; j < (unsigned int)count*2; i++, j++)
	{
		mVertexIndexData[i] = mLastDrawVertex + j;
	}

	mTrianglesCount += 2;
	mLastDrawVertex += count*2;
	mLastDrawIdx += count*2;

	return true;
}

void RenderSystem::UpdateCameraTransforms()
{
	DrawPrimitives();

	glMatrixMode(GL_MODELVIEW);
	float modelMatrix[16] ={1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, -1, 1};

	if (mCurrentCamera)
	{
		float cs = cosf(-mCurrentCamera->mRotation), sn = sinf(-mCurrentCamera->mRotation);
		Vec2F scale(1.0f/mCurrentCamera->mScale.x, 1.0f/mCurrentCamera->mScale.y);
		Vec2F pivotOffset = mCurrentCamera->mPivot.scale(mResolution).scale(mCurrentCamera->mScale).rotate(mCurrentCamera->mRotation);
		Vec2F offs = mCurrentCamera->mPosition - pivotOffset;
		float ofx = -offs.x*scale.x, ofy = -offs.y*scale.y;

		modelMatrix[0] = cs*scale.x;  modelMatrix[1] = sn*scale.x;
		modelMatrix[4] = -sn*scale.y; modelMatrix[5] = cs*scale.y;
		modelMatrix[12] = cs*ofx - sn*ofy;     modelMatrix[13] = sn*ofx + cs*ofy;
	}

	glLoadMatrixf(modelMatrix);
}

void RenderSystem::DeinitializeGL()
{
	if (!mReady)
		return;

	if (mGLContext)
	{
		mFontManager->UnloadFonts();
		RemoveAllTextures();

		if (!wglMakeCurrent(NULL, NULL))
		{
			mLog->Error("Release Of DC And RC Failed.\n");
		}

		if (!wglDeleteContext(mGLContext))
		{
			mLog->Error("Release Rendering Context Failed.\n");
		}

		mGLContext = NULL;
	}

	mReady = false;
}

void RenderSystem::FrameResized()
{
	mResolution = App()->GetContentSize();
}

bool RenderSystem::BindRenderTarget(RenderTarget* renderTarget)
{
	if (!renderTarget)
		return false;

	DrawPrimitives();

	if (renderTarget->IsReady())
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER, renderTarget->mFrameBuffer);

		GL_CHECK_ERROR(mLog);

		SetupMatrix(renderTarget->GetTexture().getSize());
	}

	mCurrentRenderTarget = renderTarget;

	return true;
}

bool RenderSystem::UnbindRenderTarget()
{
	if (!mCurrentRenderTarget)
		return false;

	DrawPrimitives();

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	GL_CHECK_ERROR(mLog);

	SetupMatrix(mResolution.castTo<float>());

	mCurrentRenderTarget = NULL;

	return true;
}

RenderTarget* RenderSystem::GetCurrentRenderTarget() const
{
	return mCurrentRenderTarget;
}

void RenderSystem::Clear(const Color4& color /*= color4(0, 0, 0, 255)*/)
{
	if ((mCurrentRenderTarget && mCurrentRenderTarget->IsReady()) || !mCurrentRenderTarget)
	{
		glClearColor(color.RF(), color.GF(), color.BF(), color.AF());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GL_CHECK_ERROR(mLog);
	}
}

void RenderSystem::BeginRenderToStencilBuffer()
{
	if (mStencilDrawing || mStencilTest)
		return;

	DrawPrimitives();

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0x1, 0xffffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	GL_CHECK_ERROR(mLog);

	mStencilDrawing = true;
}

void RenderSystem::EndRenderToStencilBuffer()
{
	if (!mStencilDrawing)
		return;

	DrawPrimitives();

	glDisable(GL_STENCIL_TEST);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	GL_CHECK_ERROR(mLog);

	mStencilDrawing = false;
}

void RenderSystem::EnableStencilTest()
{
	if (mStencilTest || mStencilDrawing)
		return;

	DrawPrimitives();

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 0x1, 0xffffffff);

	GL_CHECK_ERROR(mLog);

	mStencilTest = true;
}

void RenderSystem::DisableStencilTest()
{
	if (!mStencilTest)
		return;

	DrawPrimitives();

	glDisable(GL_STENCIL_TEST);

	mStencilTest = false;
}

bool RenderSystem::IsStencilTestEnabled() const
{
	return mStencilTest;
}

void RenderSystem::ClearStencil()
{
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);

	GL_CHECK_ERROR(mLog);
}

void RenderSystem::SetupMatrix(const Vec2F& size)
{
	float projMat[16];
	OrthoProjMatrix(projMat, 0.0f, size.x, size.y, 0.0f, 0.0f, 10.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (int)size.x, (int)size.y);
	glLoadMatrixf(projMat);
}

void RenderSystem::SetupScissorRect(const RectF& rect)
{
	if (rect == mScissorRect)
		return;

	glScissor((int)rect.left, mResolution.y - (int)rect.down, (int)rect.getSizeX(), (int)rect.getSizeY());

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

	DrawPrimitives();

	glEnable(GL_SCISSOR_TEST);

	GL_CHECK_ERROR(mLog);

	mScissorTest = true;
}

void RenderSystem::DisableScissorTest()
{
	if (!mScissorTest)
		return;

	DrawPrimitives();

	glDisable(GL_SCISSOR_TEST);

	GL_CHECK_ERROR(mLog);

	mScissorTest = false;
}

bool RenderSystem::IsScissorTestEnabled() const
{
	return mScissorTest;
}

bool RenderSystem::IsRenderTargetAvailable() const
{
	return mRenderTargetsAvailable;
}

Vec2I RenderSystem::GetMaxTextureSize() const
{
	return mMaxTextureSize;
}

void RenderSystem::CheckCompatibles()
{
	//check render targets available
	char* extensions[] ={"GL_ARB_framebuffer_object", "GL_EXT_framebuffer_object", "GL_EXT_framebuffer_blit",
		"GL_EXT_packed_depth_stencil"};

	mRenderTargetsAvailable = true;
	for (int i = 0; i < 4; i++)
	{
		if (!IsExtensionSupported(extensions[i]))
			mRenderTargetsAvailable = false;
	}

	//get max texture size
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mMaxTextureSize.x);
	mMaxTextureSize.y = mMaxTextureSize.x;
}

bool RenderSystem::IsExtensionSupported(const char *extension)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;

	GLubyte *where, *terminator;
	/* Extension names should not have spaces. */

	where = (GLubyte *)strchr(extension, ' ');

	if (where || *extension == '\0')
		return 0;

	extensions = glGetString(GL_EXTENSIONS);

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings,
	etc. */

	start = extensions;
	for (;;)
	{
		where = (GLubyte *)strstr((const char *)start, extension);

		if (!where)
			break;

		terminator = where + strlen(extension);

		if (where == start || *(where - 1) == ' ')
		if (*terminator == ' ' || *terminator == '\0')
			return true;

		start = terminator;
	}

	return false;
}

CLOSE_O2_NAMESPACE

#endif //RENDER_OGL