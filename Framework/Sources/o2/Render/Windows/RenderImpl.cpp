#include "o2/stdafx.h"

#ifdef PLATFORM_WINDOWS
#include "o2/Render/Render.h"

#include "o2/Application/Application.h"
#include "o2/Application/Input.h"
#include "o2/Assets/Assets.h"
#include "o2/Events/EventSystem.h"
#include "o2/Render/Font.h"
#include "o2/Render/Mesh.h"
#include "o2/Render/Sprite.h"
#include "o2/Render/Texture.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Utils/Math/Geometry.h"
#include "o2/Utils/Math/Interpolation.h"

namespace o2
{
	Render::Render() :
		mReady(false), mStencilDrawing(false), mStencilTest(false), mClippingEverything(false)
	{
		// Create log stream
		mLog = mnew LogStream("Render");
		o2Debug.GetLog()->BindStream(mLog);

		// Initialize OpenGL
		mLog->Out("Initializing OpenGL render..");

		mResolution = o2Application.GetContentSize();

		if (o2Application.mNeedPlatformInitialization)
		{
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
		}

		// Get OpenGL extensions
		GetGLExtensions(mLog);

		GL_CHECK_ERROR();

		// Check compatibles
		CheckCompatibles();

		// Initialize buffers
		mVertexBufferSize = USHRT_MAX;
		mIndexBufferSize = USHRT_MAX;

		mVertexData = mnew UInt8[mVertexBufferSize * sizeof(Vertex)];
		mVertexIndexData = mnew UInt16[mIndexBufferSize * sizeof(UInt16)];

		mLastDrawVertex = 0;
		mTrianglesCount = 0;
		mCurrentPrimitiveType = PrimitiveType::Polygon;

		// Configure OpenGL
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(1.0f);

		glGenBuffers(1, &mVertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, mVertexBufferSize * sizeof(Vertex), mVertexData, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &mIndexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(mIndexBufferSize * sizeof(UInt16)), mVertexIndexData, GL_DYNAMIC_DRAW);

		InitializeStdShader();

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
			o2Assets.onAssetsRebuilt += MakeFunction(this, &Render::OnAssetsRebuilded);

		mReady = true;
	}

	Render::~Render()
	{
		if (!mReady)
			return;

		if (IsDevMode())
			o2Assets.onAssetsRebuilt -= MakeFunction(this, &Render::OnAssetsRebuilded);

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

	GLuint RenderBase::LoadShader(GLenum shaderType, const char* source)
	{
		GLuint shader = glCreateShader(shaderType);

		if (shader)
		{
			glShaderSource(shader, 1, &source, NULL);
			glCompileShader(shader);

			GLint compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

			if (!compiled)
			{
				GLint infoLen = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

				if (infoLen > 0)
				{
					char* infoLog = (char*)malloc(sizeof(char) * infoLen);
					glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
					o2Debug.LogError((String)"Error compiling shader:\n" + infoLog);
					free(infoLog);
				}

				glDeleteShader(shader);
				shader = 0;
			}
		}

		return shader;
	}

	GLuint RenderBase::BuildShaderProgram(const char* vertexSource, const char* fragmentSource)
	{
		GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertexSource);
		if (!vertexShader)
			return 0;

		GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentSource);
		if (!fragmentShader)
			return 0;

		GLuint program = glCreateProgram();
		if (program)
		{
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);

			GLint linkStatus;
			glLinkProgram(program);
			glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

			if (!linkStatus)
			{
				GLint infoLen = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

				if (infoLen > 0)
				{
					char* infoLog = (char*)malloc(sizeof(char) * infoLen);
					glGetProgramInfoLog(program, infoLen, NULL, infoLog);
					o2Debug.LogError((String)"Error linking shader:\n" + infoLog);
					free(infoLog);
				}

				glDeleteProgram(program);
				program = 0;
			}
		}

		return program;
	}

	void RenderBase::InitializeStdShader()
	{
		const char* fragShader = " \
                                                                        \n \
        varying vec4 v_color;                                           \n \
        varying vec2 v_texCoords;                                       \n \
                                                                        \n \
        uniform sampler2D u_texture;                                    \n \
                                                                        \n \
        void main()                                                     \n \
        {                                                               \n \
            gl_FragColor = v_color * texture2D(u_texture, v_texCoords); \n \
        }";

		const char* vtxShader = " \
	    uniform mat4 u_transformMatrix;                           \n \
                                                                  \n \
        attribute vec4 a_position;                                \n \
        attribute vec4 a_color;                                   \n \
        attribute vec2 a_texCoords;                               \n \
                                                                  \n \
        varying vec4 v_color;                                     \n \
        varying vec2 v_texCoords;                                 \n \
                                                                  \n \
        void main()                                               \n \
        {                                                         \n \
            v_color = a_color;                                    \n \
            v_texCoords = a_texCoords;                            \n \
            gl_Position = u_transformMatrix * a_position;         \n \
        }";

		mStdShader = BuildShaderProgram(vtxShader, fragShader);
		GL_CHECK_ERROR();

		mStdShaderMvpUniform = glGetUniformLocation(mStdShader, "u_transformMatrix");
		GL_CHECK_ERROR();

		mStdShaderTextureSample = glGetUniformLocation(mStdShader, "u_texture");
		GL_CHECK_ERROR();

		mStdShaderPosAttribute = glGetAttribLocation(mStdShader, "a_position");
		GL_CHECK_ERROR();

		mStdShaderColorAttribute = glGetAttribLocation(mStdShader, "a_color");
		GL_CHECK_ERROR();

		mStdShaderUVAttribute = glGetAttribLocation(mStdShader, "a_texCoords");
		GL_CHECK_ERROR();

		glUseProgram(mStdShader);
		GL_CHECK_ERROR();

		glVertexAttribPointer((GLuint)mStdShaderPosAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &((Vertex*)0)->x);
		glEnableVertexAttribArray((GLuint)mStdShaderPosAttribute);
		GL_CHECK_ERROR();

		glVertexAttribPointer((GLuint)mStdShaderColorAttribute, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), &((Vertex*)0)->color);
		glEnableVertexAttribArray((GLuint)mStdShaderColorAttribute);
		GL_CHECK_ERROR();

		glVertexAttribPointer((GLuint)mStdShaderUVAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &((Vertex*)0)->tu);
		glEnableVertexAttribArray((GLuint)mStdShaderUVAttribute);
		GL_CHECK_ERROR();
	}

	void Render::CheckCompatibles()
	{
		//check render targets available
		const char* extensions[] = { "GL_ARB_framebuffer_object", "GL_EXT_framebuffer_object", "GL_EXT_framebuffer_blit",
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
		//preRender.Clear();
	}

	void Render::DrawPrimitives()
	{
		if (mLastDrawVertex < 1)
			return;

		static const GLenum primitiveType[3]{ GL_TRIANGLES, GL_TRIANGLES, GL_LINES };

		glBufferData(GL_ARRAY_BUFFER, mLastDrawVertex * sizeof(Vertex), mVertexData, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(mLastDrawIdx * sizeof(UInt16)), mVertexIndexData, GL_DYNAMIC_DRAW);

		glDrawElements(primitiveType[(int)mCurrentPrimitiveType], mLastDrawIdx, GL_UNSIGNED_SHORT, (void*)0);

		GL_CHECK_ERROR();

		mFrameTrianglesCount += mTrianglesCount;
		mLastDrawVertex = mTrianglesCount = mLastDrawIdx = 0;

		mDIPCount++;
	}

	void Render::SetupViewMatrix(const Vec2I& viewSize)
	{
		mCurrentResolution = viewSize;
		UpdateCameraTransforms();
	}

	void Render::End()
	{
		if (!mReady)
			return;

		postRender();
		postRender.Clear();

		DrawPrimitives();

		if (o2Application.mNeedPlatformInitialization)
			SwapBuffers(mHDC);

		GL_CHECK_ERROR();

		CheckTexturesUnloading();
		CheckFontsUnloading();
	}

	void Render::BeginCustomRender()
	{
		DrawPrimitives();
	}

	void Render::EndCustomRender()
	{
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_SCISSOR_TEST);
		GL_CHECK_ERROR();

		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);
		GL_CHECK_ERROR();

// 		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
// 		GL_CHECK_ERROR();

		glUseProgram(mStdShader);
		GL_CHECK_ERROR();

		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
		GL_CHECK_ERROR();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		GL_CHECK_ERROR();

		glUniform1i(mStdShaderTextureSample, 0);
		GL_CHECK_ERROR();

		glVertexAttribPointer((GLuint)mStdShaderPosAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &((Vertex*)0)->x);
		glEnableVertexAttribArray((GLuint)mStdShaderPosAttribute);
		GL_CHECK_ERROR();

		glVertexAttribPointer((GLuint)mStdShaderColorAttribute, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), &((Vertex*)0)->color);
		glEnableVertexAttribArray((GLuint)mStdShaderColorAttribute);
		GL_CHECK_ERROR();

		glVertexAttribPointer((GLuint)mStdShaderUVAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &((Vertex*)0)->tu);
		glEnableVertexAttribArray((GLuint)mStdShaderUVAttribute);
		GL_CHECK_ERROR();

		mLastDrawTexture = nullptr;

		SetupViewMatrix(mResolution);
		UpdateCameraTransforms();
	}

	void Render::Clear(const Color4& color /*= Color4::Blur()*/)
	{
		glClearColor(color.RF(), color.GF(), color.BF(), color.AF());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GL_CHECK_ERROR();
	}

	void mtxMultiply(float* ret, const float* lhs, const float* rhs)
	{
		// [ 0 4  8 12 ]   [ 0 4  8 12 ]
		// [ 1 5  9 13 ] x [ 1 5  9 13 ]
		// [ 2 6 10 14 ]   [ 2 6 10 14 ]
		// [ 3 7 11 15 ]   [ 3 7 11 15 ]
		ret[0] = lhs[0] * rhs[0] + lhs[4] * rhs[1] + lhs[8] * rhs[2] + lhs[12] * rhs[3];
		ret[1] = lhs[1] * rhs[0] + lhs[5] * rhs[1] + lhs[9] * rhs[2] + lhs[13] * rhs[3];
		ret[2] = lhs[2] * rhs[0] + lhs[6] * rhs[1] + lhs[10] * rhs[2] + lhs[14] * rhs[3];
		ret[3] = lhs[3] * rhs[0] + lhs[7] * rhs[1] + lhs[11] * rhs[2] + lhs[15] * rhs[3];

		ret[4] = lhs[0] * rhs[4] + lhs[4] * rhs[5] + lhs[8] * rhs[6] + lhs[12] * rhs[7];
		ret[5] = lhs[1] * rhs[4] + lhs[5] * rhs[5] + lhs[9] * rhs[6] + lhs[13] * rhs[7];
		ret[6] = lhs[2] * rhs[4] + lhs[6] * rhs[5] + lhs[10] * rhs[6] + lhs[14] * rhs[7];
		ret[7] = lhs[3] * rhs[4] + lhs[7] * rhs[5] + lhs[11] * rhs[6] + lhs[15] * rhs[7];

		ret[8] = lhs[0] * rhs[8] + lhs[4] * rhs[9] + lhs[8] * rhs[10] + lhs[12] * rhs[11];
		ret[9] = lhs[1] * rhs[8] + lhs[5] * rhs[9] + lhs[9] * rhs[10] + lhs[13] * rhs[11];
		ret[10] = lhs[2] * rhs[8] + lhs[6] * rhs[9] + lhs[10] * rhs[10] + lhs[14] * rhs[11];
		ret[11] = lhs[3] * rhs[8] + lhs[7] * rhs[9] + lhs[11] * rhs[10] + lhs[15] * rhs[11];

		ret[12] = lhs[0] * rhs[12] + lhs[4] * rhs[13] + lhs[8] * rhs[14] + lhs[12] * rhs[15];
		ret[13] = lhs[1] * rhs[12] + lhs[5] * rhs[13] + lhs[9] * rhs[14] + lhs[13] * rhs[15];
		ret[14] = lhs[2] * rhs[12] + lhs[6] * rhs[13] + lhs[10] * rhs[14] + lhs[14] * rhs[15];
		ret[15] = lhs[3] * rhs[12] + lhs[7] * rhs[13] + lhs[11] * rhs[14] + lhs[15] * rhs[15];
	}

	void Render::UpdateCameraTransforms()
	{
		DrawPrimitives();

		Vec2F resf = (Vec2F)mCurrentResolution; 
		
		float projMat[16];
		Math::OrthoProjMatrix(projMat, 0.0f, (float)mCurrentResolution.x, (float)mCurrentResolution.y, 0.0f, 0.0f, 10.0f);
		glViewport(0, 0, mCurrentResolution.x, mCurrentResolution.y);

		float modelMatrix[16] =
		{
			1,           0,            0, 0,
			0,          -1,            0, 0,
			0,           0,            1, 0,
			Math::Round(resf.x*0.5f), Math::Round(resf.y*0.5f), -1, 1
		};

		Basis defaultCameraBasis((Vec2F)mCurrentResolution*-0.5f, Vec2F::Right()*resf.x, Vec2F().Up()*resf.y);
		Basis camTransf = mCamera.GetBasis().Inverted()*defaultCameraBasis;
		mViewScale = Vec2F(camTransf.xv.Length(), camTransf.yv.Length());
		mInvViewScale = Vec2F(1.0f / mViewScale.x, 1.0f / mViewScale.y);

		float camTransfMatr[16] =
		{
			camTransf.xv.x,   camTransf.xv.y,   0, 0,
			camTransf.yv.x,   camTransf.yv.y,   0, 0,
			0,                0,                0, 0,
			camTransf.origin.x, camTransf.origin.y, 0, 1
		};

		float mvp[16];
		float finalCamMtx[16];
		mtxMultiply(finalCamMtx, modelMatrix, camTransfMatr);
		mtxMultiply(mvp, projMat, finalCamMtx);

		glUniformMatrix4fv(mStdShaderMvpUniform, 1, GL_FALSE, mvp);

		GL_CHECK_ERROR();
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
			mScissorInfos.Last().mEndDepth = mDrawingDepth;

			if (!mStackScissors.Last().mRenderTarget)
			{
				RectI lastSummaryClipRect = mStackScissors.Last().mSummaryScissorRect;
				mClippingEverything = !summaryScissorRect.IsIntersects(lastSummaryClipRect);
				summaryScissorRect = summaryScissorRect.GetIntersection(lastSummaryClipRect);
			}
			else
			{
				glEnable(GL_SCISSOR_TEST);
				GL_CHECK_ERROR();
				mClippingEverything = false;
			}
		}
		else
		{
			glEnable(GL_SCISSOR_TEST);
			GL_CHECK_ERROR();
			mClippingEverything = false;
		}

		mScissorInfos.Add(ScissorInfo(summaryScissorRect, mDrawingDepth));
		mStackScissors.Add(ScissorStackEntry(rect, summaryScissorRect));
		
		RectI screenScissorRect = CalculateScreenSpaceScissorRect(summaryScissorRect);
		glScissor((int)(screenScissorRect.left + mCurrentResolution.x*0.5f), (int)(screenScissorRect.bottom + mCurrentResolution.y*0.5f),
				  (int)screenScissorRect.Width(), (int)screenScissorRect.Height());
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

				mScissorInfos.Last().mEndDepth = mDrawingDepth;
				mScissorInfos.Add(ScissorInfo(lastClipRect, mDrawingDepth));

				if (mStackScissors.Last().mRenderTarget)
				{
					glDisable(GL_SCISSOR_TEST);
					GL_CHECK_ERROR();
					mClippingEverything = false;
				}
				else
				{
					RectI screenScissorRect = CalculateScreenSpaceScissorRect(lastClipRect);
					glScissor((int)(screenScissorRect.left + mCurrentResolution.x*0.5f), (int)(screenScissorRect.bottom + mCurrentResolution.y*0.5f),
						      (int)screenScissorRect.Width(), (int)screenScissorRect.Height());

					mClippingEverything = lastClipRect == RectI();
				}
			}
		}
	}

	void Render::DrawBuffer(PrimitiveType primitiveType, Vertex* vertices, UInt verticesCount,
							UInt16* indexes, UInt elementsCount, const TextureRef& texture)
	{
		if (!mReady)
			return;

		mDrawingDepth += 1.0f;

		if (mClippingEverything)
			return;

		UInt indexesCount;
		if (primitiveType == PrimitiveType::Line)
			indexesCount = elementsCount * 2;
		else
			indexesCount = elementsCount * 3;

		if (mLastDrawTexture != texture.mTexture ||
			mLastDrawVertex + verticesCount >= mVertexBufferSize ||
			mLastDrawIdx + indexesCount >= mIndexBufferSize ||
			mCurrentPrimitiveType != primitiveType)
		{
			DrawPrimitives();

			mLastDrawTexture = texture.mTexture;
			mCurrentPrimitiveType = primitiveType;

			if (mLastDrawTexture)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mLastDrawTexture->mHandle);
				glUniform1i(mStdShaderTextureSample, 0);

				GL_CHECK_ERROR();
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(mStdShaderTextureSample, 0);

				GL_CHECK_ERROR();
			}
		}

		memcpy(&mVertexData[mLastDrawVertex * sizeof(Vertex)], vertices, sizeof(Vertex)*verticesCount);

		for (UInt i = mLastDrawIdx, j = 0; j < indexesCount; i++, j++)
			mVertexIndexData[i] = mLastDrawVertex + indexes[j];

		if (primitiveType != PrimitiveType::Line)
			mTrianglesCount += elementsCount;

		mLastDrawVertex += verticesCount;
		mLastDrawIdx += indexesCount;
	}

	void Render::BindRenderTexture(TextureRef renderTarget)
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

		mStackScissors.Add(ScissorStackEntry(RectI(), RectI(), true));

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
