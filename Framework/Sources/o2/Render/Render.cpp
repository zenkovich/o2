#include "o2/stdafx.h"
#include "o2/Render/Render.h"

#include "o2/Application/Application.h"
#include "o2/Application/Input.h"
#include "o2/Assets/Assets.h"
#include "o2/Assets/Types/AtlasAsset.h"
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
    DECLARE_SINGLETON(Render);

    void Render::OnFrameResized()
    {
        mResolution = o2Application.GetContentSize();
    }

    void Render::InitializeLinesIndexBuffer()
    {
        mHardLinesIndexData = mnew VertexIndex[USHRT_MAX];

        for (UInt i = 0; i < USHRT_MAX/2; i++)
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

    int Render::GetDrawCallsCount() const
    {
        return mDIPCount;
    }

    int Render::GetDrawnPrimitives() const
    {
        return mFrameTrianglesCount;
    }

    void Render::SetCamera(const Camera& camera)
    {
        DrawPrimitives();
        mCamera = camera;
        UpdateCameraTransforms();
    }

    Camera Render::GetCamera() const
    {
        return mCamera;
    }

    void Render::DrawFilledPolygon(const Vertex* verticies, int vertexCount)
    {
        static Mesh mesh(TextureRef(), 1024, 1024);

        int polyCount = vertexCount - 2;
        if (mesh.GetMaxVertexCount() < (UInt)vertexCount || mesh.GetMaxPolyCount() < (UInt)polyCount)
            mesh.Resize(vertexCount, polyCount);

        memcpy(mesh.vertices, verticies, sizeof(Vertex)*vertexCount);

        for (int i = 2; i < vertexCount; i++)
        {
            int ii = (i - 2)*3;
            mesh.indexes[ii] = i - 1;
            mesh.indexes[ii + 1] = i;
            mesh.indexes[ii + 2] = 0;
         }

        mesh.vertexCount = vertexCount;
        mesh.polyCount = polyCount;
        mesh.Draw();
    }

    void Render::DrawFilledPolygon(const Vector<Vec2F>& points, const Color4& color /*= Color4::White()*/)
    {
        static Mesh mesh(TextureRef(), 1024, 1024);

        int vertexCount = points.Count();
        int polyCount = points.Count() - 2;
        if (mesh.GetMaxVertexCount() < points.Count() || mesh.GetMaxPolyCount() < polyCount)
            mesh.Resize(points.Count(), polyCount);

        ULong dcolor = color.ABGR();
        for (int i = 0; i < points.Count(); i++)
            mesh.vertices[i] = Vertex(points[i], dcolor, 0.0f, 0.0f);

        for (int i = 2; i < points.Count(); i++)
        {
            int ii = (i - 2)*3;
            mesh.indexes[ii] = i - 1;
            mesh.indexes[ii + 1] = i;
            mesh.indexes[ii + 2] = 0;
        }

        mesh.vertexCount = vertexCount;
        mesh.polyCount = polyCount;
        mesh.Draw();
    }

    RectI Render::CalculateScreenSpaceScissorRect(const RectF& cameraSpaceScissorRect) const
    {
        Basis defaultCameraBasis((Vec2F)mCurrentResolution*-0.5f, Vec2F((float)mCurrentResolution.x, 0.0f), Vec2F(0.0f, (float)mCurrentResolution.y));
        Basis camTransf = mCamera.GetBasis().Inverted()*defaultCameraBasis;
        Basis scissorBasis(cameraSpaceScissorRect.LeftBottom(), Vec2F(cameraSpaceScissorRect.Width(), 0.0f), Vec2F(0.0f, cameraSpaceScissorRect.Height()));
        Basis screenScissorBasis = scissorBasis * camTransf;
        RectI screenScissorRect = screenScissorBasis.AABB();

        return screenScissorRect;
    }

    void Render::CheckTexturesUnloading()
    {
        Vector<Texture*> unloadTextures;
        for (auto texture : mTextures)
            if (texture->mRefs == 0)
                unloadTextures.Add(texture);

        unloadTextures.ForEach([](auto texture) { delete texture; });
    }

    void Render::CheckFontsUnloading()
    {
        Vector<Font*> unloadFonts;
        for (auto font : mFonts)
        {
            if (font->mRefs.IsEmpty())
                unloadFonts.Add(font);
        }

        unloadFonts.ForEach([](auto fnt) { delete fnt; });
    }

    void Render::OnAssetsRebuilt(const Vector<UID>& changedAssets)
    {
        for (auto tex : mTextures)
            tex->Reload();

        for (auto atlas : mAtlases)
            atlas->ReloadPages();

        for (auto spr : mSprites)
            spr->ReloadImage();
    }

    void Render::OnSpriteCreated(Sprite* sprite)
    {
        if (!IsSingletonInitialzed())
            return;

        Instance().mSprites.Add(sprite);
    }

    void Render::OnSpriteDestroyed(Sprite* sprite)
    {
        if (!IsSingletonInitialzed())
            return;

        Instance().mSprites.Remove(sprite);
    }

    void Render::OnTextureCreated(Texture* texture)
    {
        mTextures.Add(texture);
    }

    void Render::OnTextureDestroyed(Texture* texture)
    {
        mTextures.Remove(texture);
    }

    void Render::OnAtlasCreated(AtlasAsset* atlas)
    {
        mAtlases.Add(atlas);
    }

    void Render::OnAtlasDestroyed(AtlasAsset* atlas)
    {
        mAtlases.Remove(atlas);
    }

    void Render::DrawAALine(const Vec2F& a, const Vec2F& b, const Color4& color /*= Color4::White()*/,
                            float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
    {
        ULong dcolor = color.ABGR();
        Vertex v[] = { Vertex(a.x, a.y, dcolor, 0, 0), Vertex(b.x, b.y, dcolor, 0, 0) };
        DrawAAPolyLine(v, 2, width, lineType);
    }

    void Render::DrawAALine(const Vector<Vec2F>& points, const Color4& color /*= Color4::White()*/,
                            float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
    {
        ULong dcolor = color.ABGR();
        Vertex* v = mnew Vertex[points.Count()];
        for (int i = 0; i < points.Count(); i++)
            v[i] = Vertex(points[i], dcolor, 0, 0);

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

        Vertex v[] = {
            Vertex(a, dcolor, 0, 0), Vertex(b, dcolor, 0, 0),
            Vertex(b - dir*arrowSize.x + ndir*arrowSize.y, dcolor, 0, 0), Vertex(b, dcolor, 0, 0),
            Vertex(b - dir*arrowSize.x - ndir*arrowSize.y, dcolor, 0, 0), Vertex(b, dcolor, 0, 0) };

        DrawAAPolyLine(v, 6, width, lineType);
    }

    void Render::DrawAARectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4& color /*= Color4::White()*/,
                                 float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
    {
        ULong dcolor = color.ABGR();
        Vertex v[] = {
            Vertex(minp.x, minp.y, dcolor, 0, 0),
            Vertex(maxp.x, minp.y, dcolor, 0, 0),
            Vertex(maxp.x, maxp.y, dcolor, 0, 0),
            Vertex(minp.x, maxp.y, dcolor, 0, 0),
            Vertex(minp.x, minp.y, dcolor, 0, 0)
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

        Vertex v[] =
        {
            Vertex(basis.origin + basis.xv, color.ABGR(), 0, 0),
            Vertex(basis.origin + basis.yv + basis.xv, color.ABGR(), 0, 0),
            Vertex(basis.origin + basis.yv, color.ABGR(), 0, 0)
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
        Vertex* v = mnew Vertex[segCount + 1];
        ULong dcolor = color.ABGR();

        float angleSeg = 2.0f*Math::PI() / (float)(segCount - 1);
        for (int i = 0; i < segCount + 1; i++)
        {
            float a = (float)i*angleSeg;
            v[i] = Vertex(Vec2F::Rotated(a)*radius + pos, dcolor, 0, 0);
        }

        DrawAAPolyLine(v, segCount + 1, width, lineType);
        delete[] v;
    }

    void Render::DrawAABezierCurve(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
                                   const Color4& color /*= Color4::White()*/,
                                   float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
    {
        const int segCount = 20;
        Vertex v[segCount + 1];
        ULong dcolor = color.ABGR();

        for (int i = 0; i < segCount + 1; i++)
        {
            float coef = (float)i / (float)segCount;
            Vec2F p = Bezier(p1, p2, p3, p4, coef);
            v[i] = Vertex(p, dcolor, 0, 0);
        }

        DrawAAPolyLine(v, segCount + 1, width, lineType);
    }

    void Render::DrawAABezierCurveArrow(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
                                        const Color4& color /*= Color4::White()*/, const Vec2F& arrowSize /*= Vec2F(10, 10)*/,
                                        float width /*= 1.0f*/, LineType lineType /*= LineType::Solid*/)
    {
        const int segCount = 20;
        Vertex v[segCount + 1];
        ULong dcolor = color.ABGR();

        Vec2F lastp = p1;
        Vec2F dir;
        for (int i = 0; i < segCount + 1; i++)
        {
            float coef = (float)i / (float)segCount;
            Vec2F p = Bezier(p1, p2, p3, p4, coef);
            v[i] = Vertex(p, dcolor, 0, 0);
            dir = p - lastp;
            lastp = p;
        }

        DrawAAPolyLine(v, segCount + 1, width, lineType);

        dir.Normalize();
        Vec2F ndir = dir.Perpendicular();

        Vertex va[] =
        {
            Vertex(p4 - dir*arrowSize.x + ndir*arrowSize.y, dcolor, 0, 0),
            Vertex(p4, dcolor, 0, 0),
            Vertex(p4 - dir*arrowSize.x - ndir*arrowSize.y, dcolor, 0, 0)
        };
        DrawAAPolyLine(va, 3, width, lineType);
    }

    void Render::DrawLine(const Vec2F& a, const Vec2F& b, const Color4& color /*= Color4::White()*/)
    {
        ULong dcolor = color.ABGR();
        Vertex v[] = { Vertex(a.x, a.y, dcolor, 0, 0), Vertex(b.x, b.y, dcolor, 0, 0) };
        DrawPolyLine(v, 2);
    }

    void Render::DrawLine(const Vector<Vec2F>& points, const Color4& color /*= Color4::White()*/)
    {
        ULong dcolor = color.ABGR();
        Vertex* v = mnew Vertex[points.Count()];
        for (int i = 0; i < points.Count(); i++)
            v[i] = Vertex(points[i], dcolor, 0, 0);

        DrawPolyLine(v, points.Count());
        delete[] v;
    }

    void Render::DrawArrow(const Vec2F& a, const Vec2F& b, const Color4& color /*= Color4::White()*/,
                           const Vec2F& arrowSize /*= Vec2F(10, 10)*/)
    {
        ULong dcolor = color.ABGR();
        Vec2F dir = (b - a).Normalized();
        Vec2F ndir = dir.Perpendicular();

        Vertex v[] = {
            Vertex(a, dcolor, 0, 0), Vertex(b, dcolor, 0, 0),
            Vertex(b - dir*arrowSize.x + ndir*arrowSize.y, dcolor, 0, 0), Vertex(b, dcolor, 0, 0),
            Vertex(b - dir*arrowSize.x - ndir*arrowSize.y, dcolor, 0, 0), Vertex(b, dcolor, 0, 0) };

        DrawPolyLine(v, 6);
    }

    void Render::DrawRectFrame(const Vec2F& minp, const Vec2F& maxp, const Color4& color /*= Color4::White()*/)
    {
        ULong dcolor = color.ABGR();
        Vertex v[] = {
            Vertex(minp.x, minp.y, dcolor, 0, 0),
            Vertex(maxp.x, minp.y, dcolor, 0, 0),
            Vertex(maxp.x, maxp.y, dcolor, 0, 0),
            Vertex(minp.x, maxp.y, dcolor, 0, 0),
            Vertex(minp.x, minp.y, dcolor, 0, 0)
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

        Vertex v[] =
        {
            Vertex(basis.origin + basis.xv, color.ABGR(), 0, 0),
            Vertex(basis.origin + basis.yv + basis.xv, color.ABGR(), 0, 0),
            Vertex(basis.origin + basis.yv, color.ABGR(), 0, 0)
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
        static int vertexBufferSize = segCount + 1;
        static Vertex* vertexBuffer = mnew Vertex[vertexBufferSize];

        int vertexCount = segCount + 1;
        if (vertexCount > vertexBufferSize)
        {
            delete[] vertexBuffer;
            vertexBufferSize = vertexCount;
            vertexBuffer = mnew Vertex[vertexBufferSize];
        }

        ULong dcolor = color.ABGR();

        float angleSeg = 2.0f*Math::PI() / (float)(segCount - 1);
        for (int i = 0; i < segCount + 1; i++)
        {
            float a = (float)i*angleSeg;
            vertexBuffer[i] = Vertex(Vec2F::Rotated(a)*radius + pos, dcolor, 0, 0);
        }

        DrawPolyLine(vertexBuffer, segCount + 1);
    }

    void Render::DrawFilledCircle(const Vec2F& pos, float radius /*= 5*/, const Color4& color /*= Color4::White()*/, 
                                  int segCount /*= 20*/)
    {
        static int vertexBufferSize = segCount + 1;
        static Vertex* vertexBuffer = mnew Vertex[vertexBufferSize];

        int vertexCount = segCount + 1;
        if (vertexCount > vertexBufferSize)
        {
            delete[] vertexBuffer;
            vertexBufferSize = vertexCount;
            vertexBuffer = mnew Vertex[vertexBufferSize];
        }

        ULong dcolor = color.ABGR();

        float angleSeg = 2.0f*Math::PI() / (float)(segCount - 1);
        for (int i = 0; i < segCount + 1; i++)
        {
            float a = (float)i*angleSeg;
            vertexBuffer[i] = Vertex(Vec2F::Rotated(a)*radius + pos, dcolor, 0, 0);
        }

        DrawFilledPolygon(vertexBuffer, vertexCount);
    }

    void Render::DrawBezierCurve(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
                                 const Color4& color /*= Color4::White()*/)
    {
        const int segCount = 20;
        Vertex v[segCount + 1];
        ULong dcolor = color.ABGR();

        for (int i = 0; i < segCount + 1; i++)
        {
            float coef = (float)i / (float)segCount;
            Vec2F p = Bezier(p1, p2, p3, p4, coef);
            v[i] = Vertex(p, dcolor, 0, 0);
        }

        DrawPolyLine(v, segCount + 1);
    }

    void Render::DrawBezierCurveArrow(const Vec2F& p1, const Vec2F& p2, const Vec2F& p3, const Vec2F& p4,
                                      const Color4& color /*= Color4::White()*/, const Vec2F& arrowSize /*= Vec2F(10, 10)*/)
    {
        const int segCount = 20;
        Vertex v[segCount + 1];
        ULong dcolor = color.ABGR();

        Vec2F lastp = p1;
        Vec2F dir;
        for (int i = 0; i < segCount + 1; i++)
        {
            float coef = (float)i / (float)segCount;
            Vec2F p = Bezier(p1, p2, p3, p4, coef);
            v[i] = Vertex(p, dcolor, 0, 0);
            dir = p - lastp;
            lastp = p;
        }

        DrawPolyLine(v, segCount + 1);

        dir.Normalize();
        Vec2F ndir = dir.Perpendicular();

        Vertex va[] =
        {
            Vertex(p4 - dir*arrowSize.x + ndir*arrowSize.y, dcolor, 0, 0),
            Vertex(p4, dcolor, 0, 0),
            Vertex(p4 - dir*arrowSize.x - ndir*arrowSize.y, dcolor, 0, 0)
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
        if (mStackScissors.IsEmpty() || mStackScissors.Last().mRenderTarget)
            return RectI(INT_MIN, INT_MIN, INT_MAX, INT_MAX);

        return (RectI)(mStackScissors.Last().mSummaryScissorRect);
    }

    const Vector<Render::ScissorStackEntry>& Render::GetScissorsStack() const
    {
        return mStackScissors;
    }

    bool Render::IsScissorTestEnabled() const
    {
        return !mStackScissors.IsEmpty();
    }

    bool Render::IsClippedByScissor(const RectF& rect) const
    {
        return !GetScissorRect().IsIntersects(rect);
    }

    bool Render::IsClippedByScissor(const Vec2F& point) const
    {
        return !GetScissorRect().IsInside(point);
    }

    void Render::DrawMesh(Mesh* mesh)
    {
        if (mesh->polyCount > 0)
        {
            DrawBuffer(PrimitiveType::Polygon, mesh->vertices, mesh->vertexCount,
                       mesh->indexes, mesh->polyCount, mesh->mTexture);
        }
    }

    void Render::DrawMeshWire(Mesh* mesh, const Color4& color /*= Color4::White()*/)
    {
        auto dcolor = color.ABGR();

        for (UInt i = 0; i < mesh->polyCount; i++)
        {
            Vertex v[] =
            {
                mesh->vertices[mesh->indexes[i*3]],
                mesh->vertices[mesh->indexes[i*3 + 1]],
                mesh->vertices[mesh->indexes[i*3 + 2]],
                mesh->vertices[mesh->indexes[i*3]]
            };

            v[0].color = dcolor;
            v[1].color = dcolor;
            v[2].color = dcolor;
            v[3].color = dcolor;

            DrawPolyLine(v, 4);
        }
    }

    void Render::DrawPolyLine(Vertex* vertices, int count, float width /*= 1.0f*/)
    {
        DrawBuffer(PrimitiveType::Line, vertices, count, mHardLinesIndexData, count - 1, mSolidLineTexture);
    }

    void Render::DrawAAPolyLine(Vertex* vertices, int count, float width /*= 1.0f*/,
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

    const Vector<Render::ScissorInfo>& Render::GetScissorInfos() const
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

    bool Render::ScissorInfo::operator==(const ScissorInfo& other) const
    {
        return Math::Equals(mBeginDepth, other.mBeginDepth) && Math::Equals(mEndDepth, other.mEndDepth) &&
            mScissorRect == other.mScissorRect;
    }

    Render::ScissorStackEntry::ScissorStackEntry()
    {}

    Render::ScissorStackEntry::ScissorStackEntry(const RectI& rect, const RectI& summaryRect, bool renderTarget /*= false*/) :
        mScrissorRect(rect), mSummaryScissorRect(summaryRect), mRenderTarget(renderTarget)
    {}

    bool Render::ScissorStackEntry::operator==(const ScissorStackEntry& other) const
    {
        return mScrissorRect == other.mScrissorRect;
    }
}
