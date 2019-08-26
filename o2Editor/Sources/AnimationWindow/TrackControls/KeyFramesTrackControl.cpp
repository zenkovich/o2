#include "stdafx.h"
#include "KeyFramesTrackControl.h"


template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimatedValue<float>>);

template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimatedValue<bool>>);

template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimatedValue<Color4>>);

template<>
DECLARE_CLASS_MANUAL(Editor::KeyFramesTrackControl<AnimatedValue<Vec2F>>);

namespace Editor
{
	template<>
	void KeyFramesTrackControl<AnimatedValue<float>>::Draw()
	{
		if (!mResEnabledInHierarchy)
			return;

		OnDrawn();

		RectF clipRect = mTimeline->layout->GetWorldRect();
		o2Render.EnableScissorTest(clipRect);

		const int bufferSize = 50;
		Vertex2 buffer[bufferSize];

		auto color = Color4(44, 62, 80).ABGR();

		for (int i = 1; i < mAnimatedValue->GetKeys().Count(); i++)
		{
			auto& key = mAnimatedValue->GetKeys()[i];
			auto& prevKey = mAnimatedValue->GetKeys()[i - 1];

			Basis drawCoords(RectF(mTimeline->LocalToWorld(prevKey.position), clipRect.top, mTimeline->LocalToWorld(key.position), clipRect.bottom));

			Basis boundsBasis(key.GetGetApproximatedPointsBounds());
			Basis transform = boundsBasis.Inverted() * drawCoords;
			for (int j = 0; j < key.GetApproximatedPointsCount(); j++)
				buffer[j].Set(key.GetApproximatedPoints()[j] * transform, color, 0, 0);

			o2Render.DrawAAPolyLine(buffer, key.GetApproximatedPointsCount(), 1.0f, LineType::Solid, false);
		}

		o2Render.DisableScissorTest();

		DrawDebugFrame();
	}
}
