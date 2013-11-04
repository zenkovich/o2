#ifndef CAMERA_H
#define CAMERA_H

#include "public.h"
#include "util/math/vector2.h"
#include "util/math/basis.h"

OPEN_O2_NAMESPACE

/** Camera. Contains position, scale and rotations. Works as sprite. */
class grCamera
{
public:
	vec2f mPosition; /**< Position of left top corner camera. */
	vec2f mScale;    /**< Scale of camera, from left top corner. */
	float mRotation; /**< Rotation of camera, from left top corner. */

	/** ctor. */
	grCamera(const vec2f& pos = vec2f(0, 0), const vec2f& scale = vec2f(1, 1), float rotation = 0)
	{
		mPosition = pos;
		mScale = scale;
		mRotation = rotation;
	}

	/** Sets the parametres of camera. */
	void set(const vec2f& pos = vec2f(0, 0), const vec2f& scale = vec2f(1, 1), float rotation = 0)
	{
		mPosition = pos;
		mScale = scale;
		mRotation = rotation;
	}

	/** Sets parametres from basis. Shear not supporting. */
	void set(const basis& bas)
	{
		mPosition = bas.offs;
		mScale = bas.getScale();
		mRotation = bas.getAngle();
	}
};

CLOSE_O2_NAMESPACE

#endif //CAMERA_H