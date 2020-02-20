#include "o2/stdafx.h"
#include "SceneView.h"

namespace o2
{
	Basis ISceneView::GetTransform(const Vec2F& screenViewSize) const
	{
		return Basis::Identity();
	}

	Basis ScreenResolutionSceneView::GetTransform(const Vec2F& screenViewSize) const
	{
		return Basis::Identity();
	}

	Basis FixedResolutionSceneView::GetTransform(const Vec2F& screenViewSize) const
	{
		return Basis(RectF(Vec2F(), screenViewSize)).Inverted()*Basis(RectF(Vec2F(), (Vec2F)fixedResolution));
	}

	Basis FitResolutionSceneView::GetTransform(const Vec2F& screenViewSize) const
	{
		return Basis(RectF(Vec2F(), screenViewSize)).Inverted()*Basis(RectF(Vec2F(), (Vec2F)fitResolution));
	}

}

DECLARE_CLASS(o2::ISceneView);

DECLARE_CLASS(o2::ScreenResolutionSceneView);

DECLARE_CLASS(o2::FixedResolutionSceneView);

DECLARE_CLASS(o2::FitResolutionSceneView);
