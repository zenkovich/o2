#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

template<typename T>
struct AnimationFrame;

/** Supporting structure, for interpolation value between two frames. Caching some data. */
template<typename T>
class FrameInterpolation
{
	InterpolationType mInterpolationType; /** Type of time interpolation. */
	bool              mBezier;            /** Bezier usage. */

	T                 mBeginValue;        /** Value at zero coef. */
	T                 mEndValue;          /** Value at coef = 1. */
	T                 mSupportValueBegin; /** Support bezier value for begin. */
	T                 mSupportValueEnd;   /** Support bezier value for end. */
	float*            mDataBegin;         /** Begin data. */
	float*            mDataEnd;           /** End data. */

public:
	/** Initialize from two frames. */
	void Initialize(AnimationFrame<T>* beginFrame, AnimationFrame<T>* endFrame)
	{
		mBeginValue = beginFrame->mValue;
		mEndValue = endFrame->mValue;

		mInterpolationType = endFrame->mType;
		mBezier = endFrame->mBezier;

		mDataBegin = beginFrame->mData;
		mDataEnd = endFrame->mData;

		if (endFrame->mBezier)
		{
			mSupportValueBegin = *(T*)beginFrame->mData;
			mSupportValueEnd = *(T*)endFrame->mData;
		}
	}

	/** Interpolate value with coefficient. */
	T GetValue(float coef)
	{
		float frameCoef;
		switch (mInterpolationType)
		{
			case IT_LINEAR:
			frameCoef = coef; break;
			case IT_FORCIBLE:
			frameCoef = 1.0f; break;
			case IT_EASY_IN:
			frameCoef = Sin((coef - 1.0f)*PI*0.5f) + 1.0f; break;
			case IT_EASY_OUT:
			frameCoef = Sin(coef*PI*0.5f); break;
			case IT_EASY_IN_OUT:
			frameCoef = Sin((coef - 0.5f)*PI)*0.5f + 0.5f; break;
		}

		if (mBezier)
			return InterpolateBezier<T>(mBeginValue, mSupportValueBegin, mSupportValueEnd, mEndValue, frameCoef);
		else
			return Lerp<T>(mBeginValue, mEndValue, frameCoef);
	}
};

CLOSE_O2_NAMESPACE
