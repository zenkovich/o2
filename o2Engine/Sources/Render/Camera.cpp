#include "stdafx.h"
#include "Camera.h"

#include "Render/Render.h"

namespace o2
{
	Camera::Camera(const Vec2F& position /*= Vec2F()*/, const Vec2F& size /*= o2Render.GetResolution()*/, 
				   float angle /*= 0.0f*/):
		Transform(size, position, angle)
	{
		if (size == Vec2F() && Render::IsSingletonInitialzed())
			SetSize(o2Render.GetCurrentResolution());
	}

	Camera::Camera():
		Transform(Vec2F(), Vec2F(), 0.0f)
	{
		if (size == Vec2F() && Render::IsSingletonInitialzed())
			SetSize(o2Render.GetCurrentResolution());
	}

	Camera Camera::Default()
	{
		return Camera();
	}

}

DECLARE_CLASS(o2::Camera);
