#include "stdafx.h"
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
	DECLARE_SINGLETON(Render);

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

		GL_CHECK_ERROR(mLog);

		// Check compatibles
		CheckCompatibles();

		// Initialize buffers
		mVertexData = mnew UInt8[mVertexBufferSize*sizeof(Vertex2)];

		mVertexIndexData = mnew UInt16[mIndexBufferSize];
		mLastDrawVertex = 0;
		mTrianglesCount = 0;
		mCurrentPrimitiveType = GL_TRIANGLES;

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

		GL_CHECK_ERROR(mLog);

		mLog->Out("GL_VENDOR: %s", (String)(char*)glGetString(GL_VENDOR));
		mLog->Out("GL_RENDERER: %s", (String)(char*)glGetString(GL_RENDERER));
		mLog->Out("GL_VERSION: %s", (String)(char*)glGetString(GL_VERSION));

		HDC dc = GetDC(0);
		mDPI.x = GetDeviceCaps(dc, LOGPIXELSX);
		mDPI.y = GetDeviceCaps(dc, LOGPIXELSY);
		ReleaseDC(0, dc);

		InitializeFreeType();
		InitializeLinesTextures();

		mCurrentRenderTarget = TextureRef();

		if (IsDevMode())
			o2Assets.onAssetsRebuilded += Func(this, &Render::OnAssetsRebuilded);

		mReady = true;
	}

	void Render::OnFrameResized()
	{
		mResolution = o2Application.GetContentSize();
	}

	void Render::InitializeLinesTextures()
	{
		Bitmap bitmap(Bitmap::Format::R8G8B8A8, Vec2I(8, 8));
		bitmap.Fill(Color4(255, 255, 255, 0));
		bitmap.FillRect(0, 7, 8, 6, Color4(255, 255, 255, 10));
		bitmap.FillRect(0, 6, 8, 1, Color4(255, 255, 255, 255));
		bitmap.FillRect(0, 2, 8, 1, Color4(255, 255, 255, 10));

		mSolidLineTexture = new Texture(&bitmap);
	}

	void Render::InitializeFreeType()
	{
		FT_Error error = FT_Init_FreeType(&mFreeTypeLib);
		if (error)
			mLog->Out("Failed to initialize FreeType: %i", error);
	}

	void Render::DeinitializeFreeType()
	{
		FT_Done_FreeType(mFreeTypeLib);
	}

	Render::~Render()
	{
		if (!mReady)
			return;

		if (IsDevMode())
			o2Assets.onAssetsRebuilded -= Func(this, &Render::OnAssetsRebuilded);

		mSolidLineTexture = TextureRef::Null();

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
		char* extensions[] = { "GL_ARB_framebuffer_object", "GL_EXT_framebuffer_object", "GL_EXT_framebuffer_blit",
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

		// Reset batching params
		mLastDrawTexture = NULL;
		mLastDrawVertex = 0;
		mLastDrawIdx = 0;
		mTrianglesCount = 0;
		mFrameTrianglesCount = 0;
		mDIPCount = 0;
		mCurrentPrimitiveType = GL_TRIANGLES;

		mDrawingDepth = 0.0f;

		mScissorInfos.Clear();
		mStackScissors.Clear();

		mClippingEverything = false;

		// Reset view matrices
		SetupViewMatrix(mResolution);

		UpdateCameraTransforms();

		preRender();
		preRender.Clear();
	}

	void Render::DrawPrimitives()
	{
		if (mLastDrawVertex < 1)
			return;

		glDrawElements(mCurrentPrimitiveType, mLastDrawIdx, GL_UNSIGNED_SHORT, mVertexIndexData);

		GL_CHECK_ERROR(mLog);

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

		DrawLine({ Vec2F(0, 0), Vec2F(100, 100), o2Input.GetCursorPos() }, Color4::Red());

		postRender();
		postRender.Clear();

		DrawPrimitives();
		SwapBuffers(mHDC);

		GL_CHECK_ERROR(mLog);

		CheckTexturesUnloading();
		CheckFontsUnloading();
	}

	void Render::Clear(const Color4& color /*= Color4::Blur()*/)
	{
		glClearColor(color.RF(), color.GF(), color.BF(), color.AF());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GL_CHECK_ERROR(mLog);
	}

	Vec2I Render::GetResolution() const
	{
		return mResolution;
	}

	Vec2I Render::GetCurrentResolution() const
	{
		return mCurrentResolution;
	}

	Vec2I Render::GetDPI() const
	{
		return mDPI;
	}

	int Render::GetDrawCallsCount()
	{
		return mDIPCount;
	}

	void Render::SetCamera(const Camera& camera)
	{
		mCamera = camera;
		UpdateCameraTransforms();
	}

	Camera Render::GetCamera() const
	{
		return mCamera;
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

		float camTransfMatr[16] =
		{
			camTransf.xv.x,   camTransf.xv.y,   0, 0,
			camTransf.yv.x,   camTransf.yv.y,   0, 0,
			0,                0,                0, 0,
			camTransf.offs.x, camTransf.offs.y, 0, 1
		};

		glMultMatrixf(camTransfMatr);

	}

	void Render::CheckTexturesUnloading()
	{
		TexturesVec unloadTextures;
		for (auto texture : mTextures)
			if (texture->mRefs.Count() == 0)
				unloadTextures.Add(texture);

		unloadTextures.ForEach([](auto texture) { delete texture; });
	}

	void Render::CheckFontsUnloading()
	{
		FontsVec unloadFonts;
		for (auto font : mFonts)
			if (font->mRefs.Count() == 0)
				unloadFonts.Add(font);

		unloadFonts.ForEach([](auto fnt) { delete fnt; });
	}

	void Render::OnAssetsRebuilded(const Vector<UID>& changedAssets)
	{
		for (auto tex : mTextures)
		{
			if (changedAssets.Contains(tex->GetAtlasAssetId()))
				tex->Reload();
		}

		for (auto spr : mSprites)
		{
			if (changedAssets.Contains(spr->GetAtlasAssetId()))
				spr->ReloadImage();
		}
	}

	void Render::DrawLine(const Vec2F& a, const Vec2F& b, const Color4& color /*= Color4::White()*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2 v[] = { Vertex2(a.x, a.y, dcolor, 0, 0), Vertex2(b.x, b.y, dcolor, 0, 0) };
		DrawLines(v, 2);
	}

	void Render::DrawLine(const Vector<Vec2F>& points, const Color4& color /*= Color4::White()*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2* v = mnew Vertex2[points.Count()];
		for (int i = 0; i < points.Count(); i++)
			v[i] = Vertex2(points[i], dcolor, 0, 0);

		DrawLines(v, points.Count());
		delete[] v;
	}

	void Render::DrawArrow(const Vec2F& a, const Vec2F& b, const Color4& color /*= Color4::White()*/,
						   const Vec2F& arrowSize /*= Vec2F(10, 10)*/)
	{
		ULong dcolor = color.ABGR();
		Vec2F dir = (b - a).Normalized();
		Vec2F ndir = dir.Perpendicular();

		Vertex2 v[] = {
			Vertex2(a, dcolor, 0, 0), Vertex2(b, dcolor, 0, 0),
			Vertex2(b - dir*arrowSize.x + ndir*arrowSize.y, dcolor, 0, 0), Vertex2(b, dcolor, 0, 0),
			Vertex2(b - dir*arrowSize.x - ndir*arrowSize.y, dcolor, 0, 0), Vertex2(b, dcolor, 0, 0) };
		DrawLines(v, 6);
	}

	void Render::DrawRectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4& color /*= Color4::White()*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2 v[] = {
			Vertex2(minp.x, minp.y, dcolor, 0, 0), Vertex2(maxp.x, minp.y, dcolor, 0, 0),
			Vertex2(maxp.x, minp.y, dcolor, 0, 0), Vertex2(maxp.x, maxp.y, dcolor, 0, 0),
			Vertex2(maxp.x, maxp.y, dcolor, 0, 0), Vertex2(minp.x, maxp.y, dcolor, 0, 0),
			Vertex2(minp.x, maxp.y, dcolor, 0, 0), Vertex2(minp.x, minp.y, dcolor, 0, 0)
		};
		DrawLines(v, 8);
	}

	void Render::DrawRectFrame(const RectF& rect, const Color4& color /*= Color4::White()*/)
	{
		DrawRectFrame(rect.LeftBottom(), rect.RightTop(), color);
	}

	void Render::DrawBasis(const Basis& basis, const Color4& xcolor /*= Color4::Red()*/,
						   const Color4& ycolor /*= Color4::Blue()*/, const Color4& color /*= Color4::White()*/)
	{
		Vertex2 v[] =
		{
			Vertex2(basis.offs, xcolor.ABGR(), 0, 0), Vertex2(basis.offs + basis.xv, xcolor.ABGR(), 0, 0),
			Vertex2(basis.offs, ycolor.ABGR(), 0, 0), Vertex2(basis.offs + basis.yv, ycolor.ABGR(), 0, 0),
			Vertex2(basis.offs + basis.xv, color.ABGR(), 0, 0), Vertex2(basis.offs + basis.yv + basis.xv, color.ABGR(), 0, 0),
			Vertex2(basis.offs + basis.yv, color.ABGR(), 0, 0), Vertex2(basis.offs + basis.yv + basis.xv, color.ABGR(), 0, 0)
		};
		DrawLines(v, 8);
	}

	void Render::DrawCross(const Vec2F& pos, float size /*= 5*/, const Color4& color /*= Color4::White()*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2 v[] = {
			Vertex2(pos.x - size, pos.y, dcolor, 0, 0), Vertex2(pos.x + size, pos.y, dcolor, 0, 0),
			Vertex2(pos.x, pos.y - size, dcolor, 0, 0), Vertex2(pos.x, pos.y + size, dcolor, 0, 0) };
		DrawLines(v, 4);
	}

	void Render::DrawCircle(const Vec2F& pos, float radius /*= 5*/, const Color4& color /*= Color4::White()*/,
							int segCount /*= 20*/)
	{
		Vertex2* v = mnew Vertex2[segCount * 2];
		ULong dcolor = color.ABGR();

		float angleSeg = 2.0f*Math::PI() / (float)(segCount - 1);
		for (int i = 0; i < segCount; i++)
		{
			float a = (float)i*angleSeg;
			v[i] = Vertex2(Vec2F::Rotated(a)*radius + pos, dcolor, 0, 0);
		}

		DrawLines(v, segCount);
		delete[] v;
	}

	void Render::DrawBezierCurve(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
								 const Color4& color /*= Color4::White()*/)
	{
		const int segCount = 20;
		Vertex2 v[segCount * 2];
		ULong dcolor = color.ABGR();

		Vec2F lastp = p1;
		for (int i = 0; i < segCount; i++)
		{
			float coef = (float)(i + 1) / (float)segCount;
			Vec2F p = Bezier(p1, p2, p3, p4, coef);

			v[i] = Vertex2(lastp, dcolor, 0, 0);

			lastp = p;
		}

		DrawLines(v, segCount);
	}

	void Render::DrawBezierCurveArrow(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
									  const Color4& color /*= Color4::White()*/, const Vec2F& arrowSize /*= Vec2F(10, 10)*/)
	{
		const int segCount = 20;
		Vertex2 v[segCount * 2 + 4];
		ULong dcolor = color.ABGR();

		Vec2F lastp = p1;
		Vec2F dir;
		for (int i = 0; i < segCount; i++)
		{
			float coef = (float)(i + 1) / (float)segCount;
			Vec2F p = Bezier(p1, p2, p3, p4, coef);

			v[i] = Vertex2(lastp, dcolor, 0, 0);

			dir = p - lastp;
			lastp = p;
		}

		dir.Normalize();
		Vec2F ndir = dir.Perpendicular();

		v[segCount + 0] = Vertex2(p4, dcolor, 0, 0);
		v[segCount + 1] = Vertex2(p4 - dir*arrowSize.x + ndir*arrowSize.y, dcolor, 0, 0);
		v[segCount + 2] = Vertex2(p4, dcolor, 0, 0);
		v[segCount + 3] = Vertex2(p4 - dir*arrowSize.x - ndir*arrowSize.y, dcolor, 0, 0);

		DrawLines(v, segCount + 4);
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

		GL_CHECK_ERROR(mLog);

		mStencilDrawing = true;
	}

	void Render::EndRenderToStencilBuffer()
	{
		if (!mStencilDrawing)
			return;

		DrawPrimitives();

		glDisable(GL_STENCIL_TEST);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		GL_CHECK_ERROR(mLog);

		mStencilDrawing = false;
	}

	void Render::EnableStencilTest()
	{
		if (mStencilTest || mStencilDrawing)
			return;

		DrawPrimitives();

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 0x1, 0xffffffff);

		GL_CHECK_ERROR(mLog);

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

	bool Render::IsStencilTestEnabled() const
	{
		return mStencilTest;
	}

	void Render::ClearStencil()
	{
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		GL_CHECK_ERROR(mLog);
	}

	RectI Render::GetScissorRect() const
	{
		if (mStackScissors.IsEmpty())
			return RectI(-(int)(mCurrentResolution.x*0.5f), -(int)(mCurrentResolution.y*0.5f),
						 (int)(mCurrentResolution.x*0.5f), (int)(mCurrentResolution.y*0.5f));

		return (RectI)(mStackScissors.Last().mScrissorRect);
	}

	RectI Render::GetResScissorRect() const
	{
		if (mStackScissors.IsEmpty())
			return RectI(-(int)(mCurrentResolution.x*0.5f), -(int)(mCurrentResolution.y*0.5f),
						 (int)(mCurrentResolution.x*0.5f), (int)(mCurrentResolution.y*0.5f));

		return (RectI)(mStackScissors.Last().mSummaryScissorRect);
	}

	const Render::StackScissorVec& Render::GetScissorsStack() const
	{
		return mStackScissors;
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
			GL_CHECK_ERROR(mLog);
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
			GL_CHECK_ERROR(mLog);

			while (!mStackScissors.IsEmpty() && !mStackScissors.Last().mRenderTarget)
				mStackScissors.PopBack();

			mScissorInfos.Last().mEndDepth = mDrawingDepth;
		}
		else
		{
			if (mStackScissors.Count() == 1)
			{
				glDisable(GL_SCISSOR_TEST);
				GL_CHECK_ERROR(mLog);
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

	bool Render::IsScissorTestEnabled() const
	{
		return !mStackScissors.IsEmpty();
	}

	bool Render::DrawMesh(Mesh* mesh)
	{
		if (!mReady)
			return false;

		mDrawingDepth += 1.0f;

		if (mClippingEverything)
			return true;

		// Check difference
		if (mLastDrawTexture != mesh->mTexture.mTexture ||
			mLastDrawVertex + mesh->vertexCount >= mVertexBufferSize ||
			mLastDrawIdx + mesh->polyCount * 3 >= mIndexBufferSize ||
			mCurrentPrimitiveType == GL_LINES)
		{
			DrawPrimitives();

			mLastDrawTexture = mesh->mTexture.mTexture;
			mCurrentPrimitiveType = GL_TRIANGLES; 
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			if (mLastDrawTexture)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, mLastDrawTexture->mHandle);

				GL_CHECK_ERROR(mLog);
			}
			else glDisable(GL_TEXTURE_2D);
		}

		memcpy(&mVertexData[mLastDrawVertex*sizeof(Vertex2)], mesh->vertices, sizeof(Vertex2)*mesh->vertexCount);

		for (UInt i = mLastDrawIdx, j = 0; j < mesh->polyCount * 3; i++, j++)
			mVertexIndexData[i] = mLastDrawVertex + mesh->indexes[j];

		mTrianglesCount += mesh->polyCount;
		mLastDrawVertex += mesh->vertexCount;
		mLastDrawIdx += mesh->polyCount * 3;

		return true;
	}

	bool Render::DrawMeshWire(Mesh* mesh, const Color4& color /*= Color4::White()*/)
	{
		Vertex2* vertices = new Vertex2[mesh->polyCount * 6];
		auto dcolor = color.ABGR();

		for (UInt i = 0; i < mesh->polyCount; i++)
		{
			vertices[i * 6] = mesh->vertices[mesh->indexes[i * 3]];
			vertices[i * 6 + 1] = mesh->vertices[mesh->indexes[i * 3 + 1]];
			vertices[i * 6 + 2] = mesh->vertices[mesh->indexes[i * 3 + 1]];
			vertices[i * 6 + 3] = mesh->vertices[mesh->indexes[i * 3 + 2]];
			vertices[i * 6 + 4] = mesh->vertices[mesh->indexes[i * 3 + 2]];
			vertices[i * 6 + 5] = mesh->vertices[mesh->indexes[i * 3]];
		}

		for (UInt i = 0; i < mesh->polyCount * 6; i++)
			vertices[i].color = dcolor;

		bool res = DrawLines(vertices, mesh->polyCount * 3);
		delete[] vertices;

		return res;
	}

	bool Render::DrawLines(Vertex2* verticies, int count)
	{
		if (!mReady)
			return false;

		mDrawingDepth += 1.0f;

		if (mClippingEverything)
			return true;

		static Vertex2* vertexBuf = nullptr;
		static int vertexBufCount = 0;
		static int vertexBufSize = 0;

		static UInt16* indexesBuf = nullptr;
		static int indexesBufCount = 0;
		static int indexesBufSize = 0;

		Geometry::CreatePolyLineMesh(verticies, count,
									 vertexBuf, vertexBufCount, vertexBufSize,
									 indexesBuf, indexesBufCount, indexesBufSize,
									 1.0f, 2, 2, mSolidLineTexture->GetSize());

		// Check difference
		if (mLastDrawTexture != mSolidLineTexture.mTexture ||
			mLastDrawVertex + vertexBufCount >= mVertexBufferSize ||
			mLastDrawIdx + indexesBufCount * 3 >= mIndexBufferSize ||
			mCurrentPrimitiveType == GL_LINES)
		{
			DrawPrimitives();

			mLastDrawTexture = mSolidLineTexture.mTexture;
			mCurrentPrimitiveType = GL_TRIANGLES;
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			if (mLastDrawTexture)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, mLastDrawTexture->mHandle);

				GL_CHECK_ERROR(mLog);
			}
			else glDisable(GL_TEXTURE_2D);
		}

		memcpy(&mVertexData[mLastDrawVertex*sizeof(Vertex2)], vertexBuf, sizeof(Vertex2)*vertexBufCount);

		for (UInt i = mLastDrawIdx, j = 0; j < indexesBufCount * 3; i++, j++)
			mVertexIndexData[i] = mLastDrawVertex + indexesBuf[j];

		mTrianglesCount += indexesBufCount;
		mLastDrawVertex += vertexBufCount;
		mLastDrawIdx += indexesBufCount * 3;

		return true;
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
			GL_CHECK_ERROR(mLog);
		}

		mStackScissors.Add(ScissorStackItem(RectI(), RectI(), true));

		glBindFramebufferEXT(GL_FRAMEBUFFER, renderTarget->mFrameBuffer);
		GL_CHECK_ERROR(mLog);

		SetupViewMatrix(renderTarget->GetSize());

		mCurrentRenderTarget = renderTarget;
	}

	void Render::UnbindRenderTexture()
	{
		if (!mCurrentRenderTarget)
			return;

		DrawPrimitives();

		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
		GL_CHECK_ERROR(mLog);

		SetupViewMatrix(mResolution);

		mCurrentRenderTarget = TextureRef();

		DisableScissorTest(true);
		mStackScissors.PopBack();
		if (!mStackScissors.IsEmpty())
		{
			glEnable(GL_SCISSOR_TEST);
			GL_CHECK_ERROR(mLog);

			auto clipRect = mStackScissors.Last().mSummaryScissorRect;

			glScissor((int)(clipRect.left + mCurrentResolution.x*0.5f), 
					  (int)(clipRect.bottom + mCurrentResolution.y*0.5f),
					  (int)clipRect.Width(), 
					  (int)clipRect.Height());
		}
	}

	TextureRef Render::GetRenderTexture() const
	{
		return mCurrentRenderTarget;
	}

	bool Render::IsRenderTextureAvailable() const
	{
		return mRenderTargetsAvailable;
	}

	Vec2I Render::GetMaxTextureSize() const
	{
		return mMaxTextureSize;
	}

	float Render::GetDrawingDepth()
	{
		mDrawingDepth += 1.0f;
		return mDrawingDepth;
	}

	const Render::ScissorInfosVec& Render::GetScissorInfos() const
	{
		return mScissorInfos;
	}

	Render& Render::operator=(const Render& other)
	{
		return *this;
	}

	Render::ScissorInfo::ScissorInfo():
		mBeginDepth(0), mEndDepth(0)
	{}

	Render::ScissorInfo::ScissorInfo(const RectI& rect, float beginDepth) :
		mScissorRect(rect), mBeginDepth(beginDepth), mEndDepth(beginDepth)
	{}

	bool Render::ScissorInfo::operator==(const ScissorInfo& other)
	{
		return Math::Equals(mBeginDepth, other.mBeginDepth) && Math::Equals(mEndDepth, other.mEndDepth) &&
			mScissorRect == other.mScissorRect;
	}

	Render::ScissorStackItem::ScissorStackItem()
	{}

	Render::ScissorStackItem::ScissorStackItem(const RectI& rect, const RectI& summaryRect, bool renderTarget /*= false*/):
		mScrissorRect(rect), mSummaryScissorRect(summaryRect), mRenderTarget(renderTarget)
	{}

	bool Render::ScissorStackItem::operator==(const ScissorStackItem& other)
	{
		return mScrissorRect == other.mScrissorRect;
	}

}