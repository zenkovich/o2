#include "AnimatedValue.h"

#include "Animation/Animatable.h"

namespace o2
{
	IOBJECT_CPP(IAnimatedValue);

	IOBJECT_CPP(AnimatedValue<Color4>);
	IOBJECT_CPP(AnimatedValue<Color4>::Key);

	IOBJECT_CPP(AnimatedValue<RectF>);
	IOBJECT_CPP(AnimatedValue<RectF>::Key);
}