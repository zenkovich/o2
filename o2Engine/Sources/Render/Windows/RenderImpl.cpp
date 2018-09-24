#include "stdafx.h"

#ifdef PLATFORM_WINDOWS

#include "Render/Render.h"

#include "Application/Application.h"
#include "Assets/Assets.h"
#include "Render/Font.h"
#include "Render/Mesh.h"
#include "Render/Sprite.h"
#include "Render/Texture.h"
#include "Utils/Debug/Debug.h"
#include "Utils/Debug/Log/LogStream.h"
#include "Utils/Math/Geometry.h"
#include "Utils/Math/Interpolation.h"
#include "Application/Input.h"

namespace o2
{
	Render::Render():
		mReady(false), mStencilDrawing(false), mStencilTest(false), mClippingEverything(false)
	{
		mVertexBufferSize = USHRT_MAX;
		mIndexBufferSize = USHRT_MAX;

		// Create log stream
		mLog = mnew LogStream("Render");
		o2Debug.GetLog()->BindStream(mLog);

		// Initialize OpenGL
		mLog->Out("Initializing OpenGL render..");

		mResolution = o2Application.GetContentSize();

		GLuint pixelFormat;
		static	PIXELFORMATDESCRIPTOR pfd = // pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
			1,							   // Version Number
			PFD_DRAW_TO_WINDOW |		   // Format Must Support Window
			PFD_SUPPORT_OPENGL |		   // Format Must Support OpenGL
			PFD_DOUBLEBUFFER,			   // Must Support Double Buffering
			PFD_TYPE_RGBA,				   // Request An RGBA Format
			32,  						   // Select Our Color Depth
			0, 0, 0, 0, 0, 0,			   // Color Bits Ignored
			0,							   // No Alpha Buffer
			0,							   // Shift Bit Ignored
			0,							   // No Accumulation Buffer
			0, 0, 0, 0,					   // Accumulation Bits Ignored
			16,							   // 16Bit Z-Buffer (Depth Buffer)  
			1,							   // No Stencil Buffer
			0,							   // No Auxiliary Buffer
			PFD_MAIN_PLANE,				   // Main Drawing Layer
			0,							   // Reserved
			0, 0, 0						   // Layer Masks Ignored
		};

		mHDC = GetDC(o2Application.mHWnd);
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

		// Get OpenGL extensions
		GetGLExtensions(mLog);

		GL_CHECK_ERROR();

		// Check compatibles
		CheckCompatibles();

		// Initialize buffers
		mVertexData = mnew UInt8[mVertexBufferSize*sizeof(Vertex2)];

		mVertexIndexData = mnew UInt16[mIndexBufferSize];
		mLastDrawVertex = 0;
		mTrianglesCount = 0;
		mCurrentPrimitiveType = PrimitiveType::Polygon;

		// Configure OpenGL
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex2), mVertexData + sizeof(float) * 3);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex2), mVertexData + sizeof(float) * 3 + sizeof(unsigned long));
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex2), mVertexData + 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLineWidth(1.0f);

		GL_CHECK_ERROR();

		mLog->Out("GL_VENDOR: " + (String)(char*)glGetString(GL_VENDOR));
		mLog->Out("GL_RENDERER: " + (String)(char*)glGetString(GL_RENDERER));
		mLog->Out("GL_VERSION: " + (String)(char*)glGetString(GL_VERSION));

		HDC dc = GetDC(0);
		mDPI.x = GetDeviceCaps(dc, LOGPIXELSX);
		mDPI.y = GetDeviceCaps(dc, LOGPIXELSY);
		ReleaseDC(0, dc);

		InitializeFreeType();
		InitializeLinesIndexBuffer();
		InitializeLinesTextures();

		mCurrentRenderTarget = TextureRef();

		if (IsDevMode())
			o2Assets.onAssetsRebuilded += Func(this, &Render::OnAssetsRebuilded);

		mReady = true;
	}

	Render::~Render()
	{
		if (!mReady)
			return;

		if (IsDevMode())
			o2Assets.onAssetsRebuilded -= Func(this, &Render::OnAssetsRebuilded);

		mSolidLineTexture = TextureRef::Null();
		mDashLineTexture = TextureRef::Null();

		if (mGLContext)
		{
			auto fonts = mFonts;
			for (auto font : fonts)
				delete font;

			auto textures = mTextures;
			for (auto texture : textures)
				delete texture;

			if (!wglMakeCurrent(NULL, NULL))
				mLog->Error("Release ff DC And RC Failed.\n");

			if (!wglDeleteContext(mGLContext))
				mLog->Error("Release Rendering Context Failed.\n");

			mGLContext = NULL;
		}

		DeinitializeFreeType();

		mReady = false;
	}

	void Render::CheckCompatibles()
	{
		//check render targets available
		char* extensions[] ={ "GL_ARB_framebuffer_object", "GL_EXT_framebuffer_object", "GL_EXT_framebuffer_blit",
			"GL_EXT_packed_depth_stencil" };

		mRenderTargetsAvailable = true;
		for (int i = 0; i < 4; i++)
		{
			if (!IsGLExtensionSupported(extensions[i]))
				mRenderTargetsAvailable = false;
		}

		//get max texture size
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mMaxTextureSize.x);
		mMaxTextureSize.y = mMaxTextureSize.x;
	}

	void Render::Begin()
	{
		if (!mReady)
			return;

		mLastDrawTexture = NULL;
		mLastDrawVertex = 0;
		mLastDrawIdx = 0;
		mTrianglesCount = 0;
		mFrameTrianglesCount = 0;
		mDIPCount = 0;
		mCurrentPrimitiveType = PrimitiveType::Polygon;

		mDrawingDepth = 0.0f;

		mScissorInfos.Clear();
		mStackScissors.Clear();

		mClippingEverything = false;

		SetupViewMatrix(mResolution);
		UpdateCameraTransforms();

		preRender();
		preRender.Clear();
	}

	void Render::DrawPrimitives()
	{
		if (mLastDrawVertex < 1)
			return;

		static const GLenum primitiveType[3]{ GL_TRIANGLES, GL_TRIANGLES, GL_LINES };

		glDrawElements(primitiveType[(int)mCurrentPrimitiveType], mLastDrawIdx, GL_UNSIGNED_SHORT, mVertexIndexData);

		GL_CHECK_ERROR();

		mFrameTrianglesCount += mTrianglesCount;
		mLastDrawVertex = mTrianglesCount = mLastDrawIdx = 0;

		mDIPCount++;
	}

	void Render::SetupViewMatrix(const Vec2I& viewSize)
	{
		mCurrentResolution = viewSize;
		float projMat[16];
		Math::OrthoProjMatrix(projMat, 0.0f, (float)viewSize.x, (float)viewSize.y, 0.0f, 0.0f, 10.0f);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, viewSize.x, viewSize.y);
		glLoadMatrixf(projMat);
		UpdateCameraTransforms();
	}

	void Render::End()
	{
		if (!mReady)
			return;

		postRender();
		postRender.Clear();

		DrawPrimitives();
		SwapBuffers(mHDC);

		GL_CHECK_ERROR();

		CheckTexturesUnloading();
		CheckFontsUnloading();
	}

	void Render::Clear(const Color4& color /*= Color4::Blur()*/)
	{
		glClearColor(color.RF(), color.GF(), color.BF(), color.AF());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GL_CHECK_ERROR();
	}

	void Render::UpdateCameraTransforms()
	{
		DrawPrimitives();

		Vec2F resf = (Vec2F)mCurrentResolution;

		glMatrixMode(GL_MODELVIEW);
		float modelMatrix[16] =
		{
			1,           0,            0, 0,
			0,          -1,            0, 0,
			0,           0,            1, 0,
			Math::Round(resf.x*0.5f), Math::Round(resf.y*0.5f), -1, 1
		};

		glLoadMatrixf(modelMatrix);

		Basis defaultCameraBasis((Vec2F)mCurrentResolution*-0.5f, Vec2F::Right()*resf.x, Vec2F().Up()*resf.y);
		Basis camTransf = mCamera.GetBasis().Inverted()*defaultCameraBasis;
		mViewScale = Vec2F(camTransf.xv.Length(), camTransf.yv.Length());
		mInvViewScale = Vec2F(1.0f/mViewScale.x, 1.0f/mViewScale.y);

		float camTransfMatr[16] =
		{
			camTransf.xv.x,   camTransf.xv.y,   0, 0,
			camTransf.yv.x,   camTransf.yv.y,   0, 0,
			0,                0,                0, 0,
			camTransf.origin.x, camTransf.origin.y, 0, 1
		};

		glMultMatrixf(camTransfMatr);

	}

	void Render::BeginRenderToStencilBuffer()
	{
		if (mStencilDrawing || mStencilTest)
			return;

		DrawPrimitives();

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0x1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		GL_CHECK_ERROR();

		mStencilDrawing = true;
	}

	void Render::EndRenderToStencilBuffer()
	{
		if (!mStencilDrawing)
			return;

		DrawPrimitives();

		glDisable(GL_STENCIL_TEST);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		GL_CHECK_ERROR();

		mStencilDrawing = false;
	}

	void Render::EnableStencilTest()
	{
		if (mStencilTest || mStencilDrawing)
			return;

		DrawPrimitives();

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 0x1, 0xffffffff);

		GL_CHECK_ERROR();

		mStencilTest = true;
	}

	void Render::DisableStencilTest()
	{
		if (!mStencilTest)
			return;

		DrawPrimitives();

		glDisable(GL_STENCIL_TEST);

		mStencilTest = false;
	}

	void Render::ClearStencil()
	{
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		GL_CHECK_ERROR();
	}

	void Render::EnableScissorTest(const RectI& rect)
	{
		DrawPrimitives();

		RectI summaryScissorRect = rect;
		if (!mStackScissors.IsEmpty())
		{
			RectI lastSummaryClipRect = mStackScissors.Last().mSummaryScissorRect;
			mClippingEverything = !summaryScissorRect.IsIntersects(lastSummaryClipRect);
			summaryScissorRect = summaryScissorRect.GetIntersection(lastSummaryClipRect);
			mScissorInfos.Last().mEndDepth = mDrawingDepth;
		}
		else
		{
			glEnable(GL_SCISSOR_TEST);
			GL_CHECK_ERROR();
		}

		mScissorInfos.Add(ScissorInfo(summaryScissorRect, mDrawingDepth));
		mStackScissors.Add(ScissorStackItem(rect, summaryScissorRect));

		glScissor((int)(summaryScissorRect.left + mCurrentResolution.x*0.5f),
			(int)(summaryScissorRect.bottom + mCurrentResolution.y*0.5f),
				  (int)summaryScissorRect.Width(),
				  (int)summaryScissorRect.Height());
	}

	void Render::DisableScissorTest(bool forcible /*= false*/)
	{
		if (mStackScissors.IsEmpty())
		{
			mLog->WarningStr("Can't disable scissor test - no scissor were enabled!");
			return;
		}

		DrawPrimitives();

		if (forcible)
		{
			glDisable(GL_SCISSOR_TEST);
			GL_CHECK_ERROR();

			while (!mStackScissors.IsEmpty() && !mStackScissors.Last().mRenderTarget)
				mStackScissors.PopBack();

			mScissorInfos.Last().mEndDepth = mDrawingDepth;
		}
		else
		{
			if (mStackScissors.Count() == 1)
			{
				glDisable(GL_SCISSOR_TEST);
				GL_CHECK_ERROR();
				mStackScissors.PopBack();

				mScissorInfos.Last().mEndDepth = mDrawingDepth;
				mClippingEverything = false;
			}
			else
			{
				mStackScissors.PopBack();
				RectI lastClipRect = mStackScissors.Last().mSummaryScissorRect;
				glScissor((int)(lastClipRect.left + mCurrentResolution.x*0.5f),
					(int)(lastClipRect.bottom + mCurrentResolution.y*0.5f),
						  (int)lastClipRect.Width(),
						  (int)lastClipRect.Height());

				mScissorInfos.Last().mEndDepth = mDrawingDepth;
				mScissorInfos.Add(ScissorInfo(lastClipRect, mDrawingDepth));

				mClippingEverything = lastClipRect == RectI();
			}
		}
	}

	void Render::DrawBuffer(PrimitiveType primitiveType, Vertex2* vertices, UInt verticesCount,
							UInt16* indexes, UInt elementsCount, const TextureRef& texture)
	{
		if (!mReady)
			return;

		mDrawingDepth += 1.0f;

		if (mClippingEverything)
			return;

		UInt indexesCount;
		if (primitiveType == PrimitiveType::Line)
			indexesCount = elementsCount*2;
		else
			indexesCount = elementsCount*3;

		if (mLastDrawTexture != texture.mTexture ||
			mLastDrawVertex + verticesCount >= mVertexBufferSize ||
			mLastDrawIdx + indexesCount >= mIndexBufferSize ||
			mCurrentPrimitiveType != primitiveType)
		{
			DrawPrimitives();

			mLastDrawTexture = texture.mTexture;
			mCurrentPrimitiveType = primitiveType;

			if (primitiveType == PrimitiveType::PolygonWire)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			if (mLastDrawTexture)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, mLastDrawTexture->mHandle);

				GL_CHECK_ERROR();
			}
			else glDisable(GL_TEXTURE_2D);
		}

		memcpy(&mVertexData[mLastDrawVertex*sizeof(Vertex2)], vertices, sizeof(Vertex2)*verticesCount);

		for (UInt i = mLastDrawIdx, j = 0; j < indexesCount; i++, j++)
			mVertexIndexData[i] = mLastDrawVertex + indexes[j];

		if (primitiveType != PrimitiveType::Line)
			mTrianglesCount += elementsCount;

		mLastDrawVertex += verticesCount;
		mLastDrawIdx += indexesCount;
	}

	void Render::SetRenderTexture(TextureRef renderTarget)
	{
		if (!renderTarget)
		{
			UnbindRenderTexture();
			return;
		}

		if (renderTarget->mUsage != Texture::Usage::RenderTarget)
		{
			mLog->Error("Can't set texture as render target: not render target texture");
			UnbindRenderTexture();
			return;
		}

		if (!renderTarget->IsReady())
		{
			mLog->Error("Can't set texture as render target: texture isn't ready");
			UnbindRenderTexture();
			return;
		}

		DrawPrimitives();

		if (!mStackScissors.IsEmpty())
		{
			mScissorInfos.Last().mEndDepth = mDrawingDepth;
			glDisable(GL_SCISSOR_TEST);
			GL_CHECK_ERROR();
		}

		mStackScissors.Add(ScissorStackItem(RectI(), RectI(), true));

		glBindFramebufferEXT(GL_FRAMEBUFFER, renderTarget->mFrameBuffer);
		GL_CHECK_ERROR();

		SetupViewMatrix(renderTarget->GetSize());

		mCurrentRenderTarget = renderTarget;
	}

	void Render::UnbindRenderTexture()
	{
		if (!mCurrentRenderTarget)
			return;

		DrawPrimitives();

		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
		GL_CHECK_ERROR();

		SetupViewMatrix(mResolution);

		mCurrentRenderTarget = TextureRef();

		DisableScissorTest(true);
		mStackScissors.PopBack();
		if (!mStackScissors.IsEmpty())
		{
			glEnable(GL_SCISSOR_TEST);
			GL_CHECK_ERROR();

			auto clipRect = mStackScissors.Last().mSummaryScissorRect;

			glScissor((int)(clipRect.left + mCurrentResolution.x*0.5f),
				(int)(clipRect.bottom + mCurrentResolution.y*0.5f),
					  (int)clipRect.Width(),
					  (int)clipRect.Height());

			mClippingEverything = clipRect == RectI();
		}
	}
}

#endif // PLATFORM_WINDOWS