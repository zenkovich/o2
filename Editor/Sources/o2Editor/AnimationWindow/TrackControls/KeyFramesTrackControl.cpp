#include "o2Editor/stdafx.h"
#include "KeyFramesTrackControl.h"

DECLARE_TEMPLATE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimationTrack<float>>);
DECLARE_TEMPLATE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimationTrack<bool>>);
DECLARE_TEMPLATE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimationTrack<Color4>>);
DECLARE_TEMPLATE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimationTrack<Vec2F>>);

namespace Editor
{
	void DrawCurveInCoords(const ApproximationValue* points, int pointsCount, const RectF& pointsBounds, const Basis& drawBasis,
		const Color4& color)
	{
		const int bufferSize = 50;
		static Vertex buffer[bufferSize];

		if (pointsBounds.Height() < 0.001f)
		{
			o2Render.DrawAALine(Vec2F(0.0f, 0.5f)*drawBasis, Vec2F(1.0f, 0.5f)*drawBasis, color);
			return;
		}

		Basis boundsBasis(pointsBounds);
		Basis transform = boundsBasis.Inverted() * drawBasis;

		for (int j = 0; j < pointsCount; j++)
			buffer[j].Set(Vec2F(points[j].position, points[j].value)*transform, color.ABGR(), 0, 0);

		o2Render.DrawAAPolyLine(buffer, pointsCount, 1.0f, LineType::Solid, false);
	}

	void DrawCenterCurveInCoords(const Vec2F* points, int pointsCount, const RectF& pointsBounds, const Basis& drawBasis,
		const Color4& color)
	{
		const int bufferSize = 50;
		static Vertex buffer[bufferSize];

		Basis boundsBasis(pointsBounds);
		Basis boundsBasisInv = boundsBasis.Inverted();
		Basis newDrawBasis(drawBasis.origin + drawBasis.yv*0.5f, drawBasis.xv, drawBasis.yv);

		for (int j = 0; j < pointsCount; j++)
		{
			Vec2F p = points[j] * boundsBasisInv;
			p.y -= p.x;
			buffer[j].Set(p * newDrawBasis, color.ABGR(), 0, 0);
		}

		o2Render.DrawAAPolyLine(buffer, pointsCount, 1.0f, LineType::Solid, false);
	}

	template<>
	void KeyFramesTrackControl<AnimationTrack<float>>::SetCurveViewEnabled(bool enabled)
	{
		mAddKeyDotButton->enabled = enabled;
		mAddKeyButton->enabled = !enabled;
	}

	template<>
	void KeyFramesTrackControl<AnimationTrack<float>>::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		if (!mHandlesSheet->enabled)
			return;

		OnDrawn();

		o2Render.EnableScissorTest(mTimeline->layout->GetWorldRect());

		for (int i = 1; i < mTrack->GetKeys().Count(); i++)
		{
			auto& key = mTrack->GetKeys()[i];
			auto& prevKey = mTrack->GetKeys()[i - 1];

			Basis drawCoords(RectF(mTimeline->LocalToWorld(prevKey.position) - 3,
				layout->GetWorldTop() - 5,
				mTimeline->LocalToWorld(key.position) - 3,
				layout->GetWorldBottom() + 5));

			DrawCurveInCoords(key.GetApproximatedPoints(), key.GetApproximatedPointsCount(),
				key.GetGetApproximatedPointsBounds(), drawCoords, Color4(44, 62, 80));
		}

		for (auto handle : mHandles)
			handle->handle->Draw();

		o2Render.DisableScissorTest();

		DrawDebugFrame();
	}

	template<>
	void KeyFramesTrackControl<AnimationTrack<Color4>>::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		if (!mHandlesSheet->enabled)
			return;

		OnDrawn();

		if (!mTrack->GetKeys().IsEmpty())
		{
			static Ref<Texture> chessBackTexture;
			static Sprite chessBackSprite;
			if (!chessBackTexture)
			{
				Color4 color1(1.0f, 1.0f, 1.0f, 1.0f), color2(0.7f, 0.7f, 0.7f, 1.0f);
				Bitmap backLayerBitmap(PixelFormat::R8G8B8A8, Vec2I(16, 16));
				backLayerBitmap.Fill(color1);
				backLayerBitmap.FillRect(0, 8, 8, 0, color2);
				backLayerBitmap.FillRect(8, 16, 16, 8, color2);
				chessBackTexture = Ref<Texture>(backLayerBitmap);
				chessBackSprite = Sprite(chessBackTexture, RectI(0, 0, 16, 16));
				chessBackSprite.mode = SpriteMode::Tiled;
			}

			chessBackSprite.SetRect(RectF(mTimeline->LocalToWorld(mTrack->GetKeys()[0].position), layout->GetWorldTop() - 5,
				mTimeline->LocalToWorld(mTrack->GetKeys().Last().position), layout->GetWorldBottom() + 4));
			chessBackSprite.Draw();

			static Mesh mesh;
			int verticies = mTrack->GetKeys().Count()*2;
			int polygons = (mTrack->GetKeys().Count() - 1)*2;

			if (mesh.GetMaxVertexCount() < (UInt)verticies)
				mesh.Resize(verticies, polygons);

			mesh.vertexCount = verticies;
			mesh.polyCount = polygons;

			o2Render.EnableScissorTest(mTimeline->layout->GetWorldRect());

			for (int i = 0; i < mTrack->GetKeys().Count(); i++)
			{
				auto& key = mTrack->GetKeys()[i];
				float keyPos = mTimeline->LocalToWorld(key.position);
				int nv = i*2;

				mesh.vertices[nv] = Vertex(keyPos, layout->GetWorldTop() - 5, key.value.ABGR(), 0, 0);
				mesh.vertices[nv + 1] = Vertex(keyPos, layout->GetWorldBottom() + 4, key.value.ABGR(), 0, 0);

				if (i > 0)
				{
					int np = (i - 1)*2;

					mesh.indexes[np*3] = nv - 2;
					mesh.indexes[np*3 + 1] = nv;
					mesh.indexes[np*3 + 2] = nv - 1;
					mesh.indexes[np*3 + 3] = nv;
					mesh.indexes[np*3 + 4] = nv + 1;
					mesh.indexes[np*3 + 5] = nv - 1;
				}
			}

			mesh.Draw();
		}

		for (auto handle : mHandles)
			handle->handle->Draw();

		o2Render.DisableScissorTest();

		DrawDebugFrame();
	}

	template<>
	void KeyFramesTrackControl<AnimationTrack<bool>>::Draw()
	{
	}
}#include <memory>
#include <vector>

// Define Ref and WeakRef as aliases for shared_ptr and weak_ptr respectively
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

// Define DynamicCast as a template function to replace dynamic_cast
template<typename To, typename From>
Ref<To> DynamicCast(const Ref<From>& ptr) {
    return std::dynamic_pointer_cast<To>(ptr);
}

// Define mmake as a template function to create Ref
template<typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// Define Vertex and Color4 classes
class Vertex {
public:
    float x, y, z;
    int color, u, v;

    Vertex(float x, float y, int color, int u, int v)
        : x(x), y(y), z(0), color(color), u(u), v(v) {}
};

class Color4 {
public:
    int r, g, b, a;

    Color4(int r, int g, int b, int a = 255)
        : r(r), g(g), b(b), a(a) {}

    int ABGR() {
        return (a << 24) | (b << 16) | (g << 8) | r;
    }
};

// Update the function signature to use const Ref<> for function arguments 
// with pointer types
void DrawKeys(const Ref<Sheet>& mHandlesSheet, const Ref<Timeline>& mTimeline,
              const Ref<Layout>& layout, const std::vector<Ref<Key>>& mHandles,
              bool mResEnabledInHierarchy) {
    if (!mResEnabledInHierarchy)
        return;

    if (!mHandlesSheet->enabled)
        return;

    OnDrawn();

    if (!mTimeline->GetKeys().empty()) {
        float lineOffset = 11;

        static Mesh mesh;
        int vertices = (mTimeline->GetKeys().size() - 1) * 4;
        int polygons = (mTimeline->GetKeys().size() - 1) * 2;

        if (mesh.GetMaxVertexCount() < static_cast<UInt>(vertices))
            mesh.Resize(vertices, polygons);

        mesh.vertexCount = vertices;
        mesh.polyCount = polygons;

        o2Render.EnableScissorTest(mTimeline->layout->GetWorldRect());

        Color4 trueColor(44, 62, 80);
        Color4 falseColor(0, 0, 0, 0);

        for (size_t i = 1; i < mTimeline->GetKeys().size(); i++) {
            const auto& key = mTimeline->GetKeys()[i];
            const auto& prevKey = mTimeline->GetKeys()[i - 1];

            float keyPos = mTimeline->LocalToWorld(key.position);
            float prevKeyPos = mTimeline->LocalToWorld(prevKey.position);

            int nv = static_cast<int>((i - 1) * 4);
            int np = static_cast<int>((i - 1) * 2);

            auto color = (prevKey.value ? trueColor : falseColor).ABGR();

            mesh.vertices[nv] = Vertex(prevKeyPos, layout->GetWorldTop() - lineOffset, color, 0, 0);
            mesh.vertices[nv + 1] = Vertex(keyPos, layout->GetWorldTop() - lineOffset, color, 0, 0);
            mesh.vertices[nv + 2] = Vertex(keyPos, layout->GetWorldBottom() + lineOffset, color, 0, 0);
            mesh.vertices[nv + 3] = Vertex(prevKeyPos, layout->GetWorldBottom() + lineOffset, color, 0, 0);

            mesh.indexes[np*3] = nv;
            mesh.indexes[np*3 + 1] = nv + 1;
            mesh.indexes[np*3 + 2] = nv + 2;
            mesh.indexes[np*3 + 3] = nv;
            mesh.indexes[np*3 + 4] = nv + 2;
            mesh.indexes[np*3 + 5] = nv + 3;
        }

        mesh.Draw();
    }

    for (const auto& handle : mHandles)
        handle->handle->Draw();

    o2Render.DisableScissorTest();

    DrawDebugFrame();
}