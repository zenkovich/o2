#ifndef ANIM_OBJ_H
#define ANIM_OBJ_H

#include "public.h"

OPEN_O2_NAMESPACE

class cAnimatedObj
{
public:
	virtual ~cAnimatedObj() {}

	virtual void update(float dt) {}
};

CLOSE_O2_NAMESPACE

#endif //ANIM_OBJ_H