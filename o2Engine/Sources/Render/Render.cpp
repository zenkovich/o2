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

	void Render::OnFrameResized()
	{
		mResolution = o2Application.GetContentSize();
	}

	void Render::InitializeLinesIndexBuffer()
	{
		mHardLinesIndexData = mnew UInt16[mIndexBufferSize];

		for (UInt i = 0; i < mIndexBufferSize/2; i++)
		{
			mHardLinesIndexData[i*2] = i;
			mHardLinesIndexData[i*2 + 1] = i + 1;
		}
	}

	void Render::InitializeLinesTextures()
	{
		mSolidLineTexture = TextureRef::Null();

		Bitmap bitmap(PixelFormat::R8G8B8A8, Vec2I(32, 32));
		bitmap.Fill(Color4(255, 255, 255, 255));
		bitmap.FillRect(0, 32, 16, 0, Color4(255, 255, 255, 0));
		mDashLineTexture = new Texture(&bitmap);
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

	void Render::DrawAALine(const Vec2F& a, const Vec2F& b, const Color4& color /*= Color4::White()*/,
							float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2 v[] = { Vertex2(a.x, a.y, dcolor, 0, 0), Vertex2(b.x, b.y, dcolor, 0, 0) };
		DrawAAPolyLine(v, 2, width, lineType);
	}

	void Render::DrawAALine(const Vector<Vec2F>& points, const Color4& color /*= Color4::White()*/,
							float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2* v = mnew Vertex2[points.Count()];
		for (int i = 0; i < points.Count(); i++)
			v[i] = Vertex2(points[i], dcolor, 0, 0);

		DrawAAPolyLine(v, points.Count(), width, lineType);
		delete[] v;
	}

	void Render::DrawAAArrow(const Vec2F& a, const Vec2F& b, const Color4& color /*= Color4::White()*/,
							 const Vec2F& arrowSize /*= Vec2F(10, 10)*/,
							 float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
	{
		ULong dcolor = color.ABGR();
		Vec2F dir = (b - a).Normalized();
		Vec2F ndir = dir.Perpendicular();

		Vertex2 v[] = {
			Vertex2(a, dcolor, 0, 0), Vertex2(b, dcolor, 0, 0),
			Vertex2(b - dir*arrowSize.x + ndir*arrowSize.y, dcolor, 0, 0), Vertex2(b, dcolor, 0, 0),
			Vertex2(b - dir*arrowSize.x - ndir*arrowSize.y, dcolor, 0, 0), Vertex2(b, dcolor, 0, 0) };

		DrawAAPolyLine(v, 6, width, lineType);
	}

	void Render::DrawAARectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4& color /*= Color4::White()*/,
								 float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2 v[] = {
			Vertex2(minp.x, minp.y, dcolor, 0, 0),
			Vertex2(maxp.x, minp.y, dcolor, 0, 0),
			Vertex2(maxp.x, maxp.y, dcolor, 0, 0),
			Vertex2(minp.x, maxp.y, dcolor, 0, 0),
			Vertex2(minp.x, minp.y, dcolor, 0, 0)
		};
		DrawAAPolyLine(v, 5, width, lineType);
	}

	void Render::DrawAARectFrame(const RectF& rect, const Color4& color /*= Color4::White()*/,
								 float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
	{
		DrawAARectFrame(rect.LeftBottom(), rect.RightTop(), color, width, lineType);
	}

	void Render::DrawAABasis(const Basis& basis, const Color4& xcolor /*= Color4::Red()*/,
							 const Color4& ycolor /*= Color4::Blue()*/, const Color4& color /*= Color4::White()*/,
							 float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
	{
		DrawAALine(basis.origin, basis.origin + basis.xv, xcolor, width, lineType);
		DrawAALine(basis.origin, basis.origin + basis.yv, ycolor, width, lineType);

		Vertex2 v[] =
		{
			Vertex2(basis.origin + basis.xv, color.ABGR(), 0, 0),
			Vertex2(basis.origin + basis.yv + basis.xv, color.ABGR(), 0, 0),
			Vertex2(basis.origin + basis.yv, color.ABGR(), 0, 0)
		};

		DrawAAPolyLine(v, 3, width, lineType);
	}

	void Render::DrawAACross(const Vec2F& pos, float size /*= 5*/, const Color4& color /*= Color4::White()*/,
							 float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
	{
		DrawAALine(Vec2F(pos.x - size, pos.y), Vec2F(pos.x + size, pos.y), color, width, lineType);
		DrawAALine(Vec2F(pos.x, pos.y - size), Vec2F(pos.x, pos.y + size), color, width, lineType);
	}

	void Render::DrawAACircle(const Vec2F& pos, float radius /*= 5*/, const Color4& color /*= Color4::White()*/,
							  int segCount /*= 20*/,
							  float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
	{
		Vertex2* v = mnew Vertex2[segCount + 1];
		ULong dcolor = color.ABGR();

		float angleSeg = 2.0f*Math::PI() / (float)(segCount - 1);
		for (int i = 0; i < segCount + 1; i++)
		{
			float a = (float)i*angleSeg;
			v[i] = Vertex2(Vec2F::Rotated(a)*radius + pos, dcolor, 0, 0);
		}

		DrawAAPolyLine(v, segCount + 1, width, lineType);
		delete[] v;
	}

	void Render::DrawAABezierCurve(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
								   const Color4& color /*= Color4::White()*/,
								   float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
	{
		const int segCount = 20;
		Vertex2 v[segCount + 1];
		ULong dcolor = color.ABGR();

		for (int i = 0; i < segCount + 1; i++)
		{
			float coef = (float)i / (float)segCount;
			Vec2F p = Bezier(p1, p2, p3, p4, coef);
			v[i] = Vertex2(p, dcolor, 0, 0);
		}

		DrawAAPolyLine(v, segCount + 1, width, lineType);
	}

	void Render::DrawAABezierCurveArrow(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
										const Color4& color /*= Color4::White()*/, const Vec2F& arrowSize /*= Vec2F(10, 10)*/,
										float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
	{
		const int segCount = 20;
		Vertex2 v[segCount + 1];
		ULong dcolor = color.ABGR();

		Vec2F lastp = p1;
		Vec2F dir;
		for (int i = 0; i < segCount + 1; i++)
		{
			float coef = (float)i / (float)segCount;
			Vec2F p = Bezier(p1, p2, p3, p4, coef);
			v[i] = Vertex2(p, dcolor, 0, 0);
			dir = p - lastp;
			lastp = p;
		}

		DrawAAPolyLine(v, segCount + 1, width, lineType);

		dir.Normalize();
		Vec2F ndir = dir.Perpendicular();

		Vertex2 va[] =
		{
			Vertex2(p4 - dir*arrowSize.x + ndir*arrowSize.y, dcolor, 0, 0),
			Vertex2(p4, dcolor, 0, 0),
			Vertex2(p4 - dir*arrowSize.x - ndir*arrowSize.y, dcolor, 0, 0)
		};
		DrawAAPolyLine(va, 3, width, lineType);
	}

	void Render::DrawLine(const Vec2F& a, const Vec2F& b, const Color4& color /*= Color4::White()*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2 v[] = { Vertex2(a.x, a.y, dcolor, 0, 0), Vertex2(b.x, b.y, dcolor, 0, 0) };
		DrawPolyLine(v, 2);
	}

	void Render::DrawLine(const Vector<Vec2F>& points, const Color4& color /*= Color4::White()*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2* v = mnew Vertex2[points.Count()];
		for (int i = 0; i < points.Count(); i++)
			v[i] = Vertex2(points[i], dcolor, 0, 0);

		DrawPolyLine(v, points.Count());
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

		DrawPolyLine(v, 6);
	}

	void Render::DrawRectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4& color /*= Color4::White()*/)
	{
		ULong dcolor = color.ABGR();
		Vertex2 v[] = {
			Vertex2(minp.x, minp.y, dcolor, 0, 0),
			Vertex2(maxp.x, minp.y, dcolor, 0, 0),
			Vertex2(maxp.x, maxp.y, dcolor, 0, 0),
			Vertex2(minp.x, maxp.y, dcolor, 0, 0),
			Vertex2(minp.x, minp.y, dcolor, 0, 0)
		};
		DrawPolyLine(v, 5);
	}

	void Render::DrawRectFrame(const RectF& rect, const Color4& color /*= Color4::White()*/)
	{
		DrawRectFrame(rect.LeftBottom(), rect.RightTop(), color);
	}

	void Render::DrawBasis(const Basis& basis, const Color4& xcolor /*= Color4::Red()*/,
						   const Color4& ycolor /*= Color4::Blue()*/, const Color4& color /*= Color4::White()*/)
	{
		DrawLine(basis.origin, basis.origin + basis.xv, xcolor);
		DrawLine(basis.origin, basis.origin + basis.yv, ycolor);

		Vertex2 v[] =
		{
			Vertex2(basis.origin + basis.xv, color.ABGR(), 0, 0),
			Vertex2(basis.origin + basis.yv + basis.xv, color.ABGR(), 0, 0),
			Vertex2(basis.origin + basis.yv, color.ABGR(), 0, 0)
		};

		DrawPolyLine(v, 3);
	}

	void Render::DrawCross(const Vec2F& pos, float size /*= 5*/, const Color4& color /*= Color4::White()*/)
	{
		DrawLine(Vec2F(pos.x - size, pos.y), Vec2F(pos.x + size, pos.y), color);
		DrawLine(Vec2F(pos.x, pos.y - size), Vec2F(pos.x, pos.y + size), color);
	}

	void Render::DrawCircle(const Vec2F& pos, float radius /*= 5*/, const Color4& color /*= Color4::White()*/,
							int segCount /*= 20*/)
	{
		Vertex2* v = mnew Vertex2[segCount + 1];
		ULong dcolor = color.ABGR();

		float angleSeg = 2.0f*Math::PI() / (float)(segCount - 1);
		for (int i = 0; i < segCount + 1; i++)
		{
			float a = (float)i*angleSeg;
			v[i] = Vertex2(Vec2F::Rotated(a)*radius + pos, dcolor, 0, 0);
		}

		DrawPolyLine(v, segCount + 1);
		delete[] v;
	}

	void Render::DrawBezierCurve(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
								 const Color4& color /*= Color4::White()*/)
	{
		const int segCount = 20;
		Vertex2 v[segCount + 1];
		ULong dcolor = color.ABGR();

		for (int i = 0; i < segCount + 1; i++)
		{
			float coef = (float)i / (float)segCount;
			Vec2F p = Bezier(p1, p2, p3, p4, coef);
			v[i] = Vertex2(p, dcolor, 0, 0);
		}

		DrawPolyLine(v, segCount + 1);
	}

	void Render::DrawBezierCurveArrow(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
									  const Color4& color /*= Color4::White()*/, const Vec2F& arrowSize /*= Vec2F(10, 10)*/)
	{
		const int segCount = 20;
		Vertex2 v[segCount + 1];
		ULong dcolor = color.ABGR();

		Vec2F lastp = p1;
		Vec2F dir;
		for (int i = 0; i < segCount + 1; i++)
		{
			float coef = (float)i / (float)segCount;
			Vec2F p = Bezier(p1, p2, p3, p4, coef);
			v[i] = Vertex2(p, dcolor, 0, 0);
			dir = p - lastp;
			lastp = p;
		}

		DrawPolyLine(v, segCount + 1);

		dir.Normalize();
		Vec2F ndir = dir.Perpendicular();

		Vertex2 va[] =
		{
			Vertex2(p4 - dir*arrowSize.x + ndir*arrowSize.y, dcolor, 0, 0),
			Vertex2(p4, dcolor, 0, 0),
			Vertex2(p4 - dir*arrowSize.x - ndir*arrowSize.y, dcolor, 0, 0)
		};
		DrawPolyLine(va, 3);
	}

	bool Render::IsStencilTestEnabled() const
	{
		return mStencilTest;
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

	bool Render::IsScissorTestEnabled() const
	{
		return !mStackScissors.IsEmpty();
	}

	void Render::DrawMesh(Mesh* mesh)
	{
		DrawBuffer(PrimitiveType::Polygon, mesh->vertices, mesh->vertexCount,
				   mesh->indexes, mesh->polyCount, mesh->mTexture);
	}

	void Render::DrawMeshWire(Mesh* mesh, const Color4& color /*= Color4::White()*/)
	{
		auto dcolor = color.ABGR();

		for (UInt i = 0; i < mesh->polyCount; i++)
		{
			Vertex2 v[] =
			{
				mesh->vertices[mesh->indexes[i*3]],
				mesh->vertices[mesh->indexes[i*3 + 1]],
				mesh->vertices[mesh->indexes[i*3 + 2]]
			};

			v[0].color = dcolor;
			v[1].color = dcolor;
			v[2].color = dcolor;

			DrawPolyLine(v, 3);
		}
	}

	void Render::DrawPolyLine(Vertex2* vertices, int count, float width /*= 1.0f*/)
	{
		DrawBuffer(PrimitiveType::Line, vertices, count, mHardLinesIndexData, count - 1, mSolidLineTexture);
	}

	void Render::DrawAAPolyLine(Vertex2* vertices, int count, float width /*= 1.0f*/,
								LineType lineType /*= LineType::Solid*/,
								bool scaleToScreenSpace /*= true*/)
	{
		static Mesh mesh(mSolidLineTexture, 1024, 1024);

		TextureRef texture = lineType == LineType::Solid ? mSolidLineTexture : mDashLineTexture;
		Vec2I texSize = lineType == LineType::Solid ? Vec2I(1, 1) : mDashLineTexture->GetSize();

		if (scaleToScreenSpace)
		{
			Geometry::CreatePolyLineMesh(vertices, count,
										 mesh.vertices, mesh.vertexCount, mesh.mMaxVertexCount,
										 mesh.indexes, mesh.polyCount, mesh.mMaxPolyCount,
										 width - 0.5f, 0.5f, 0.5f, texSize, mInvViewScale);
		}
		else
		{
			Geometry::CreatePolyLineMesh(vertices, count,
										 mesh.vertices, mesh.vertexCount, mesh.mMaxVertexCount,
										 mesh.indexes, mesh.polyCount, mesh.mMaxPolyCount,
										 width, 0.5f, 0.5f, texSize, Vec2F(1, 1));
		}

		mesh.SetTexture(texture);
		mesh.Draw();
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

	Render::ScissorInfo::ScissorInfo() :
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

	Render::ScissorStackItem::ScissorStackItem(const RectI& rect, const RectI& summaryRect, bool renderTarget /*= false*/) :
		mScrissorRect(rect), mSummaryScissorRect(summaryRect), mRenderTarget(renderTarget)
	{}

	bool Render::ScissorStackItem::operator==(const ScissorStackItem& other)
	{
		return mScrissorRect == other.mScrissorRect;
	}
}