#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

/** Camera. Contains pivot, position, scale and rotations. Works as sprite. */
class Camera
{
public:
	Vec2F mPivot;    /**< Relative pivot. (0...1). */
	Vec2F mPosition; /**< Position of pivot camera. */
	Vec2F mScale;    /**< Scale of camera, around pivot. */
	float mRotation; /**< Rotation of camera, around pivot. */

	/** ctor. */
	Camera(const Vec2F& pos = Vec2F(), const Vec2F& scale = Vec2F(1, 1), float rotation = 0,
		   const Vec2F& pivot = Vec2F())
	{
		mPivot = pivot;
		mPosition = pos;
		mScale = scale;
		mRotation = rotation;
	}

	/** Sets the parameters of camera. */
	void Set(const Vec2F& pos = Vec2F(), const Vec2F& scale = Vec2F(1, 1), float rotation = 0,
			 const Vec2F& pivot = Vec2F())
	{
		mPivot = pivot;
		mPosition = pos;
		mScale = scale;
		mRotation = rotation;
	}

	/** Sets parameters from basis. Shear not supporting. */
	void Set(const Basis& bas)
	{
		mPivot = Vec2F();
		mPosition = bas.offs;
		mScale = bas.GetScale();
		mRotation = bas.GetAngle();
	}
};