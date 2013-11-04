#ifndef PARAM_ANIMATION_H
#define PARAM_ANIMATION_H

#include <vector>

#include "public.h"
#include "anim_obj.h"

OPEN_O2_NAMESPACE

class cParamAnimation:public cAnimatedObj
{
public:
	enum ParamType { PT_INT = 0, PT_FLOAT, PT_BOOL, PT_VEC2F, PT_VEC2I };
	enum InterpolationType { IT_FORCIBLY = 0, IT_LINEAR, IT_BEZIER };
	enum TimeFuncType { TF_LINEAR = 0, TF_SIN_SMOOTH, TF_POW_FUNC, TF_MANUALLY };

/*
 *
 * | (size of type) |
 * | target value   | 
*/
	enum ParamOffsets
	{
		OFFS_TARGET_VALUE = 0,
		OFFS_
	};


protected:
	struct AnimKey
	{
		char              mData[256];
		InterpolationType mInterpolationType;
		TimeFuncType      mTimeFuncType;
		float             mDurations;
	};
	typedef std::vector<AnimKey> AnimKeysVec;

	struct Parametr
	{
		uint32      mId;
		char        mStrId[32];

		void*       mValuePtr;
		ParamType   mType;

		AnimKeysVec mAnimKeys;
	};
	typedef std::vector<Parametr*> ParamsVec;

	ParamsVec mParametres;
	float     mCurrentTime;

public:
	cParamAnimation();
	virtual ~cParamAnimation();

	void update(float dt);
};

CLOSE_O2_NAMESPACE

#endif //PARAM_ANIMATION_H