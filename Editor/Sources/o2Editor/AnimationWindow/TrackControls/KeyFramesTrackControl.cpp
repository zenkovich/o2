#include "o2Editor/stdafx.h"
#include "KeyFramesTrackControl.h"


template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimationTrack<float>>);

template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimationTrack<bool>>);

template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimationTrack<Color4>>);

template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimationTrack<Vec2F>>);

namespace Editor
{
	void DrawCurveInCoords(const ApproximationValue* points, int pointsCount, const RectF& pointsBounds, const Basis& drawBasis,
						   const Color4& color)
	{
		const int bufferSize = 50;
		static Vertex2 buffer[bufferSize];

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
		static Vertex2 buffer[bufferSize];

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
}
