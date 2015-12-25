#pragma once

#include "public.h"
#include "util/callback.h"
#include "util/objects.h"

OPEN_O2_NAMESPACE

/** Basic animation structure. Controlling time with looping and intervals. */
class IAnimation: public virtual IDynamic
{
public:
	enum class LoopType { None, Repeat, Toggle };

protected:

	/** Callback by time. */
	struct TimeCallback
	{
		float      mTime;     /**< Time, when callback will be invoke. */
		ICallback* mCallback; /**< Callback. */

		/** ctor. */
		TimeCallback(ICallback* cb, float time):mTime(time), mCallback(cb) {}
	};
	typedef Vector<TimeCallback> TimeCallbacksVec;

	ICallback*        mBeginPlayingCallback; /**< Callback, what calls when animation starting, independ of time. */
	ICallback*        mEndPlayingCallback;   /**< Callback, what calls when animation finished, independ of time. */
	TimeCallbacksVec  mTimedCallbacks;       /**< Callback, what calls by time. */

	float     mTime;           /**< Local time, in [0...mDuration].*/
	float     mLastFrameTime;  /**< Last local time. For calling callbacks by time. */
	float     mAbsoluteTime;   /**< Absolute time. */
	float     mAnimationSpeed; /**< Animation speed coefficient. 1 - standard coef, less - slower animation. */
	float     mDirectionSign;  /**< Is -1 if animation playing reversed. */
	LoopType  mLoop;           /**< Loop type. */
	float     mDuration;       /**< Duration of local time animation. */
	float     mBeginPlaying;   /**< Absolute begin playing time. */
	float     mEndPlaying;     /**< Absolute end playing time. */
	bool      mPlaying;        /**< True, when playing. */
	bool      mPlayed;         /**< True, when local time was expected. */

public:
	CallbackChain onChangedEvent;  /** Calls when animation changed, after evaluate(). */

	/** ctor. */
	IAnimation();

	/** dtor. */
	virtual ~IAnimation();

	/** Updating animation. */
	virtual void Update(float dt);

	/** Playing animation. */
	virtual void Play(bool restart = false, bool stopAtEnd = true);

	/** Play in interval. */
	virtual void Play(float begin, float end);

	/** Stop animation. */
	virtual void Stop();

	/** Return current absolute time. */
	virtual float GetTime() const;

	/** Returns current local time. */
	virtual float GetLocalTime() const;

	/** Setting absolute time. */
	virtual void SetTime(float time);

	/** Returns duration of local animation. */
	virtual float GetDuration() const;

	/** Returns true, if animation playing. */
	virtual bool IsPlaying() const;

	/** Returns true, if local animation was played. */
	virtual bool IsPlayed() const;

	/** Setting loop type. */
	virtual void SetLoop(LoopType type = LoopType::Repeat);

	/** Setting speed of animation (0...1...inf). */
	virtual void SetSpeed(float speed = 1);

	/** Returns speed of animation. */
	virtual float GetSpeed() const;

	/** Returns begin playing time. */
	virtual float GetBeginTime() const;

	/** Returns end playing time. */
	virtual float GetEndTime() const;

	/** Setting playing flag. */
	virtual void SetPlaying(bool playing);

	/** Setting begin callback. */
	virtual void SetBeginPlayingCallback(ICallback* cb);

	/** Setting end playing callback. */
	virtual void SetEndPlayingCallback(ICallback* cb);

	/** Adding time callback. */
	virtual void SetTimeCallback(float time, ICallback* cb);

protected:
	/** Invoke begin callback, if exist. */
	void CheckBeginCallback();

	/** Invoke end callback, if exist. */
	void CheckEndCallback();

	/** Invoke callbacks in time interval. */
	void CheckTimeCallbacks(float begin, float end);

	/** Update local time (mTime). */
	void UpdateTime();

	/** Change some data here. */
	virtual void Evaluate() {}
};

CLOSE_O2_NAMESPACE
