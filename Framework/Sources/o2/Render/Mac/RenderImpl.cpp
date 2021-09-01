#include "o2/stdafx.h"

#ifdef PLATFORM_MAC

#include "o2/Render/Render.h"

#include "o2/Application/Application.h"
#include "o2/Assets/Assets.h"
#include "o2/Render/Font.h"
#include "o2/Render/Mesh.h"
#include "o2/Render/Sprite.h"
#include "o2/Render/Texture.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Utils/Math/Geometry.h"
#include "o2/Utils/Math/Interpolation.h"
#include "o2/Application/Input.h"

namespace o2
{
	Render::Render() :
		mReady(false), mStencilDrawing(false), mStencilTest(false), mClippingEverything(false)
	{
//		mVertexBufferSize = USHRT_MAX;
//		mIndexBufferSize = USHRT_MAX;
//
//		mLog = mnew LogStream("Render");
//		o2Debug.GetLog()->BindStream(mLog);
//
//		mResolution = o2Application.GetContentSize();
//
//		CheckCompatibles();
//
//		mVertexData = mnew UInt8[mVertexBufferSize * sizeof(Vertex2)];
//		mVertexIndexData = mnew UInt16[mIndexBufferSize * sizeof(UInt16)];
//
//		glEnable(GL_BLEND);
//		glDisable(GL_DEPTH_TEST);
//		glDisable(GL_CULL_FACE);
//
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//        glLineWidth(1.0f);
//
//		glGenBuffers(1, &mVertexBufferObject);
//		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
//		glBufferData(GL_ARRAY_BUFFER, mVertexBufferSize * sizeof(Vertex2), mVertexData, GL_DYNAMIC_DRAW);
//
//		glGenBuffers(1, &mIndexBufferObject);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(mIndexBufferSize * sizeof(UInt16)), mVertexIndexData, GL_DYNAMIC_DRAW);
//
//		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//
//		InitializeStdShader();
//
//		GL_CHECK_ERROR();
//
//		mLog->Out("GL_VENDOR: " + (String)(char*)glGetString(GL_VENDOR));
//		mLog->Out("GL_RENDERER: " + (String)(char*)glGetString(GL_RENDERER));
//		mLog->Out("GL_VERSION: " + (String)(char*)glGetString(GL_VERSION));
//
//        mDPI = Vec2I(100, 100);
//
//		InitializeFreeType();
//		InitializeLinesIndexBuffer();
//		InitializeLinesTextures();
//
//		mCurrentRenderTarget = TextureRef();
//
//		if (IsDevMode())
//			o2Assets.onAssetsRebuilded += Func(this, &Render::OnAssetsRebuilded);

		mReady = true;
	}

	Render::~Render()
	{
		if (!mReady)
			return;
//
//		if (IsDevMode())
//			o2Assets.onAssetsRebuilded -= Func(this, &Render::OnAssetsRebuilded);
//
//		mSolidLineTexture = TextureRef::Null();
//		mDashLineTexture = TextureRef::Null();
//
//		DeinitializeFreeType();

		mReady = false;
	}

	void Render::CheckCompatibles()
	{
		//get max texture size
//		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mMaxTextureSize.x);
//		mMaxTextureSize.y = mMaxTextureSize.x;
	}

	void Render::Begin()
	{
		if (!mReady)
			return;

//		mLastDrawTexture = NULL;
//		mLastDrawVertex = 0;
//		mLastDrawIdx = 0;
//		mTrianglesCount = 0;
//		mFrameTrianglesCount = 0;
//		mDIPCount = 0;
//		mCurrentPrimitiveType = PrimitiveType::Polygon;
//
//		mDrawingDepth = 0.0f;
//
//		mScissorInfos.Clear();
//		mStackScissors.Clear();
//
//		mClippingEverything = false;
//
//		SetupViewMatrix(mResolution);
//		UpdateCameraTransforms();
//
//        glClearColor(1, 0, 0, 1);
//
//		preRender();
//		preRender.Clear();
	}

	void Render::DrawPrimitives()
	{
		if (mLastDrawVertex < 1)
			return;

//		static const GLenum primitiveType[3]{ GL_TRIANGLES, GL_TRIANGLES, GL_LINES };
//
//        glBufferData(GL_ARRAY_BUFFER, mLastDrawVertex * sizeof(Vertex2), mVertexData, GL_DYNAMIC_DRAW);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(mLastDrawIdx * sizeof(UInt16)), mVertexIndexData, GL_DYNAMIC_DRAW);
//
//		glDrawElements(primitiveType[(int)mCurrentPrimitiveType], mLastDrawIdx, GL_UNSIGNED_SHORT, (void*)0);
//
//		GL_CHECK_ERROR();
//
//		mFrameTrianglesCount += mTrianglesCount;
//		mLastDrawVertex = mTrianglesCount = mLastDrawIdx = 0;
//
//		mDIPCount++;
	}

	void Render::SetupViewMatrix(const Vec2I& viewSize)
	{
//		mCurrentResolution = viewSize;
//		UpdateCameraTransforms();
	}

	void Render::End()
	{
		if (!mReady)
			return;

//		postRender();
//		postRender.Clear();
//
//		DrawPrimitives();
//
//		GL_CHECK_ERROR();
//
//		CheckTexturesUnloading();
//		CheckFontsUnloading();
	}

	void Render::Clear(const Color4& color /*= Color4::Blur()*/)
	{
//		glClearColor(color.RF(), color.GF(), color.BF(), color.AF());
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		GL_CHECK_ERROR();
	}

	void Render::UpdateCameraTransforms()
	{
//		DrawPrimitives();
//
//		float projMat[16];
//		Math::OrthoProjMatrix(projMat, 0.0f, (float)mCurrentResolution.x, (float)mCurrentResolution.y, 0.0f, 0.0f, 10.0f);
//		glViewport(0, 0, mCurrentResolution.x, mCurrentResolution.y);
//
//		float modelMatrix[16] =
//		{
//			1,                                      0,                                       0, 0,
//			0,                                     -1,                                       0, 0,
//			0,                                      0,                                       1, 0,
//			Math::Round(mCurrentResolution.x*0.5f), Math::Round(mCurrentResolution.y*0.5f), -1, 1
//		};
//
//		Basis defaultCameraBasis((Vec2F)mCurrentResolution*-0.5f, Vec2F::Right()*mCurrentResolution.x, Vec2F().Up()*mCurrentResolution.y);
//		Basis camTransf = mCamera.GetBasis().Inverted()*defaultCameraBasis;
//
//		float camTransfMatr[16] =
//		{
//			camTransf.xv.x,     camTransf.xv.y,     0, 0,
//			camTransf.yv.x,     camTransf.yv.y,     0, 0,
//			0,                  0,                  0, 0,
//			camTransf.origin.x, camTransf.origin.y, 0, 1
//		};
//
//		float mvp[16];
//		float finalCamMtx[16];
//		mtxMultiply(finalCamMtx, modelMatrix, camTransfMatr);
//		mtxMultiply(mvp, projMat, finalCamMtx);
//
//		glUniformMatrix4fv(mStdShaderMvpUniform, 1, GL_FALSE, mvp);
//
//		GL_CHECK_ERROR();

	}

	void Render::BeginRenderToStencilBuffer()
	{
//		if (mStencilDrawing || mStencilTest)
//			return;
//
//		DrawPrimitives();
//
//		glEnable(GL_STENCIL_TEST);
//		glStencilFunc(GL_ALWAYS, 0x1, 0xffffffff);
//		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//
//		GL_CHECK_ERROR();
//
//		mStencilDrawing = true;
	}

	void Render::EndRenderToStencilBuffer()
	{
//		if (!mStencilDrawing)
//			return;
//
//		DrawPrimitives();
//
//		glDisable(GL_STENCIL_TEST);
//		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//
//		GL_CHECK_ERROR();
//
//		mStencilDrawing = false;
	}

	void Render::EnableStencilTest()
	{
//		if (mStencilTest || mStencilDrawing)
//			return;
//
//		DrawPrimitives();
//
//		glEnable(GL_STENCIL_TEST);
//		glStencilFunc(GL_EQUAL, 0x1, 0xffffffff);
//
//		GL_CHECK_ERROR();
//
//		mStencilTest = true;
	}

	void Render::DisableStencilTest()
	{
//		if (!mStencilTest)
//			return;
//
//		DrawPrimitives();
//
//		glDisable(GL_STENCIL_TEST);
//
//		mStencilTest = false;
	}

	void Render::ClearStencil()
	{
//		glClearStencil(0);
//		glClear(GL_STENCIL_BUFFER_BIT);
//
//		GL_CHECK_ERROR();
	}

	void Render::EnableScissorTest(const RectI& rect)
	{
//		DrawPrimitives();
//
//		RectI summaryScissorRect = rect;
//		if (!mStackScissors.IsEmpty())
//		{
//			RectI lastSummaryClipRect = mStackScissors.Last().mSummaryScissorRect;
//			mClippingEverything = !summaryScissorRect.IsIntersects(lastSummaryClipRect);
//			summaryScissorRect = summaryScissorRect.GetIntersection(lastSummaryClipRect);
//			mScissorInfos.Last().mEndDepth = mDrawingDepth;
//		}
//		else
//		{
//			glEnable(GL_SCISSOR_TEST);
//			GL_CHECK_ERROR();
//		}
//
//		mScissorInfos.Add(ScissorInfo(summaryScissorRect, mDrawingDepth));
//		mStackScissors.Add(ScissorStackItem(rect, summaryScissorRect));
//
//		glScissor((int)(summaryScissorRect.left + mCurrentResolution.x*0.5f),
//			(int)(summaryScissorRect.bottom + mCurrentResolution.y*0.5f),
//				  (int)summaryScissorRect.Width(),
//				  (int)summaryScissorRect.Height());
	}

	void Render::DisableScissorTest(bool forcible /*= false*/)
	{
//		if (mStackScissors.IsEmpty())
//		{
//			mLog->WarningStr("Can't disable scissor test - no scissor were enabled!");
//			return;
//		}
//
//		DrawPrimitives();
//
//		if (forcible)
//		{
//			glDisable(GL_SCISSOR_TEST);
//			GL_CHECK_ERROR();
//
//			while (!mStackScissors.IsEmpty() && !mStackScissors.Last().mRenderTarget)
//				mStackScissors.PopBack();
//
//			mScissorInfos.Last().mEndDepth = mDrawingDepth;
//		}
//		else
//		{
//			if (mStackScissors.Count() == 1)
//			{
//				glDisable(GL_SCISSOR_TEST);
//				GL_CHECK_ERROR();
//				mStackScissors.PopBack();
//
//				mScissorInfos.Last().mEndDepth = mDrawingDepth;
//				mClippingEverything = false;
//			}
//			else
//			{
//				mStackScissors.PopBack();
//				RectI lastClipRect = mStackScissors.Last().mSummaryScissorRect;
//				glScissor((int)(lastClipRect.left + mCurrentResolution.x*0.5f),
//					(int)(lastClipRect.bottom + mCurrentResolution.y*0.5f),
//						  (int)lastClipRect.Width(),
//						  (int)lastClipRect.Height());
//
//				mScissorInfos.Last().mEndDepth = mDrawingDepth;
//				mScissorInfos.Add(ScissorInfo(lastClipRect, mDrawingDepth));
//
//				mClippingEverything = lastClipRect == RectI();
//			}
//		}
	}

	void Render::DrawBuffer(PrimitiveType primitiveType, Vertex2* vertices, UInt verticesCount,
							UInt16* indexes, UInt elementsCount, const TextureRef& texture)
	{
//		if (!mReady)
//			return;
//
//		mDrawingDepth += 1.0f;
//
//		if (mClippingEverything)
//			return;
//
//		UInt indexesCount;
//		if (primitiveType == PrimitiveType::Line)
//			indexesCount = elementsCount*2;
//		else
//			indexesCount = elementsCount*3;
//
//		if (mLastDrawTexture != texture.mTexture ||
//			mLastDrawVertex + verticesCount >= mVertexBufferSize ||
//			mLastDrawIdx + indexesCount >= mIndexBufferSize ||
//			mCurrentPrimitiveType != primitiveType)
//		{
//			DrawPrimitives();
//
//			mLastDrawTexture = texture.mTexture;
//			mCurrentPrimitiveType = primitiveType;
//
//			if (mLastDrawTexture)
//			{
//				glActiveTexture(GL_TEXTURE0);
//				glBindTexture(GL_TEXTURE_2D, mLastDrawTexture->mHandle);
//				glUniform1i(mStdShaderTextureSample, 0);
//
//				GL_CHECK_ERROR();
//			}
//			else
//			{
//				glActiveTexture(GL_TEXTURE0);
//				glBindTexture(GL_TEXTURE_2D, 0);
//				glUniform1i(mStdShaderTextureSample, 0);
//
//				GL_CHECK_ERROR();
//			}
//		}
//
//		memcpy(&mVertexData[sizeof(Vertex2)*mLastDrawVertex], vertices, sizeof(Vertex2)*verticesCount);
//
//		for (UInt i = mLastDrawIdx, j = 0; j < indexesCount * 3; i++, j++)
//            mVertexIndexData[i] = mLastDrawVertex + indexes[j];
//
//		if (primitiveType != PrimitiveType::Line)
//			mTrianglesCount += elementsCount;
//
//		mLastDrawVertex += verticesCount;
//		mLastDrawIdx += indexesCount;
	}

	void Render::BindRenderTexture(TextureRef renderTarget)
	{
//		if (!renderTarget)
//		{
//			UnbindRenderTexture();
//			return;
//		}
//
//		if (renderTarget->mUsage != Texture::Usage::RenderTarget)
//		{
//			mLog->Error("Can't set texture as render target: not render target texture");
//			UnbindRenderTexture();
//			return;
//		}
//
//		if (!renderTarget->IsReady())
//		{
//			mLog->Error("Can't set texture as render target: texture isn't ready");
//			UnbindRenderTexture();
//			return;
//		}
//
//		DrawPrimitives();
//
//		if (!mStackScissors.IsEmpty())
//		{
//			mScissorInfos.Last().mEndDepth = mDrawingDepth;
//			glDisable(GL_SCISSOR_TEST);
//			GL_CHECK_ERROR();
//		}
//
//		mStackScissors.Add(ScissorStackItem(RectI(), RectI(), true));
//
//		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->mFrameBuffer);
//		GL_CHECK_ERROR();
//
//		SetupViewMatrix(renderTarget->GetSize());
//
//		mCurrentRenderTarget = renderTarget;
	}

	void Render::UnbindRenderTexture()
	{
//		if (!mCurrentRenderTarget)
//			return;
//
//		DrawPrimitives();
//
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		GL_CHECK_ERROR();
//
//		SetupViewMatrix(mResolution);
//
//		mCurrentRenderTarget = TextureRef();
//
//		DisableScissorTest(true);
//		mStackScissors.PopBack();
//		if (!mStackScissors.IsEmpty())
//		{
//			glEnable(GL_SCISSOR_TEST);
//			GL_CHECK_ERROR();
//
//			auto clipRect = mStackScissors.Last().mSummaryScissorRect;
//
//			glScissor((int)(clipRect.left + mCurrentResolution.x*0.5f),
//				(int)(clipRect.bottom + mCurrentResolution.y*0.5f),
//					  (int)clipRect.Width(),
//					  (int)clipRect.Height());
//
//			mClippingEverything = clipRect == RectI();
//		}
	}
}

#endif // PLATFORM_MAC
