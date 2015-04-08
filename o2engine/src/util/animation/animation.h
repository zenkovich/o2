#pragma once

#include "public.h"
#include "animation_base.h"
#include "frame.h"
#include "frame_interpolation.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE

/** Animated parameter. */
template<typename T>
class Animation: virtual public IAnimation
{
	friend class AnimationTest;
	typedef Array< AnimationFrame<T> > FramesArr;

public:

protected:
	T          mValue;                   /**< Animating value. */
	T          mLastValue;               /**< Last value. */
	T*         mBindValue;               /**< Pointer to binded value. */
	ICallback* mUpdateFunc;
	FramesArr  mFrames;                  /**< Frames vector. */

	int        mCurrentFrame;            /**< Index of current frame. */
	float      mCurrentFrameBeginTime;   /**< Current frame begin time. */
	float      mCurrentFrameEndTime;     /**< Current frame end time. */
	float      mCurrentFrameDuration;    /**< Duration of current frame. */
	float      mCurrentFrameInvDuration; /**< Inverted duration of current frame. */

	FrameInterpolation<T> mFrameInterp;  /**< Frames interpolator. */

public:

	/** ctor. */
	Animation(int frames = 0, float duration = 1.0f): IAnimation(),
		mCurrentFrame(0), mCurrentFrameBeginTime(0), mCurrentFrameEndTime(0), mCurrentFrameDuration(0),
		mCurrentFrameInvDuration(1), mUpdateFunc(NULL)
	{
		mBindValue = &mValue;

		float frameDuration = (frames == 0) ? 0.0f:duration/(float)frames;
		for (int i = 0; i < frames; i++)
			mFrames.push_back(AnimationFrame<T>(mValue, frameDuration));

		mDuration = frames*frameDuration;
	}

	/** dtor. */
	virtual ~Animation() {}

	/** Access operator. */
	T& operator*() { return mValue; }

	/** Access operator. */
	T& operator->() { return mValue; }

	/** Bind value. */
	void BindValue(T* valuePtr)
	{
		mBindValue = valuePtr;
	}

	void SetUpdateFunc(ICallback* callBack)
	{
		mUpdateFunc = callBack;
	}

	/** Returns true, if value changed from last frame. */
	bool IsChanged() const
	{
		return mValue != mLastValue;
	}

	/** Add frame. */
	int AddFrame(const T& value, int position = -1, float time = 1.0f, bool bezier = false,
				 InterpolationType type = IT_LINEAR, uint dataSize = 0, float* data = NULL)
	{
		return AddFrame(AnimationFrame<T>(value, time, bezier, type, dataSize, data), position);
	}

	/** Add frame. */
	int AddFrame(const AnimationFrame<T>& frm, int position = -1)
	{
		if (mFrames.size() == 0)
			mDuration -= frm.mTime;

		if (position < 0)
		{
			mFrames.push_back(frm);
			mDuration += frm.mTime;

			if (mFrames.size() == 2)
				FirstInitialize();

			return mFrames.size() - 1;
		}

		mFrames.insert(mFrames.begin() + position, frm);
		mDuration += frm.mTime;

		if (mFrames.size() == 2)
			FirstInitialize();

		return position;
	}

	/** Removing frame. */
	void RemoveFrame(int position)
	{
		if (mFrames.size() < 1)
			return;

		int idx = Clamp(position, 0, mFrames.size() - 1);
		mDuration -= mFrames[idx].mTime;
		mFrames.erase(mFrames.begin() + idx);
	}

	/** Removing all frames. */
	void RemoveAllFrames()
	{
		mDuration = 0;
		mFrames.clear();
	}

	/** Returns frames vector. */
	const FramesArr& GetFrames() const
	{
		return mFrames;
	}

protected:
	/** Updating value. */
	void Evaluate()
	{
		SeekFrame();

		float timeCoef = mCurrentFrameInvDuration*(mTime - mCurrentFrameBeginTime);

		mValue = mFrameInterp.GetValue(timeCoef);

		mLastValue = mValue;
		*mBindValue = mValue;

		if (mUpdateFunc)
			mUpdateFunc->Invoke(&mValue);
	}

	/** Seek frame by local time. */
	void SeekFrame()
	{
		bool x = true;
		while (mTime < mCurrentFrameBeginTime && mCurrentFrame > 1)
		{
			mCurrentFrame--;
			AnimationFrame<T>* frm = &mFrames[mCurrentFrame];
			mCurrentFrameEndTime = mCurrentFrameBeginTime;
			mCurrentFrameBeginTime -= frm->mTime;

			UpdateFrameSupportValues();
			x = false;
		}

		while (x && mTime > mCurrentFrameEndTime && mCurrentFrame < (int)mFrames.size() - 1)
		{
			mCurrentFrame++;
			AnimationFrame<T>* frm = &mFrames[mCurrentFrame];
			mCurrentFrameBeginTime = mCurrentFrameEndTime;
			mCurrentFrameEndTime += frm->mTime;
			UpdateFrameSupportValues();
		}
	}

	/** Updates support values for current frame, initializes frame interpolator. */
	void UpdateFrameSupportValues()
	{
		mFrameInterp.Initialize(&mFrames[mCurrentFrame - 1], &mFrames[mCurrentFrame]);

		mCurrentFrameDuration = mCurrentFrameEndTime - mCurrentFrameBeginTime;
		mCurrentFrameInvDuration = 1.0f/mCurrentFrameDuration;
	}

	/** First frame initializations. */
	void FirstInitialize()
	{
		mCurrentFrameBeginTime = 0;
		mCurrentFrameEndTime = mFrames[1].mTime;
		mCurrentFrame = 1;
		UpdateFrameSupportValues();
	}
};

CLOSE_O2_NAMESPACE