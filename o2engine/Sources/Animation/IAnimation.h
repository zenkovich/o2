#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Delegates.h"
#include "Utils/Property.h"

namespace o2
{
	class IAnimation
	{
	public: 
		enum class Loop { None, Repeat, PingPong };

	public:
		Property<bool>  playing;
		Property<bool>  reversed;
		Property<float> speed;
		Property<float> time;
		Property<float> relTime;
		Property<float> beginBound;
		Property<float> endBount;
		Property<Loop>  loop;

		Function<void()>      onPlayEvent;
		Function<void()>      onStopEvent;
		Function<void()>      onPlayedEvent;
		Function<void(float)> onUpdate;

		IAnimation();
		IAnimation(const IAnimation& other);
		virtual ~IAnimation();

		IAnimation& operator=(const IAnimation& other);

		virtual void Update(float dt);

		virtual void Play();
		virtual void PlayInBounds(float beginTime, float endTime);
		virtual void PlayBackInBounds(float beginTime, float endTime);
		virtual void TogglePlay();
		virtual void RewindAndPlay();

		virtual void Stop();

		virtual void SetBeginBound(float time);
		virtual float GetBeginBound() const;

		virtual void SetEndBound(float time);
		virtual float GetEndBound() const;

		virtual void SetBounds(float beginTime, float endTime);
		virtual void ResetBounds();

		virtual void SetPlaying(bool playing);
		virtual bool IsPlaying();

		virtual void SetTime(float time);
		virtual float GetTime() const;

		virtual float GetDuration() const;

		virtual void SetRelTime(float relTime);
		virtual float GetRelTime() const;

		virtual void GoToBegin();
		virtual void GoToEnd();

		virtual void PlayForward();
		virtual void PlayBack();

		virtual void SetReverse(bool reverse);
		virtual bool IsReversed() const;

		virtual void SetSpeed(float speed);
		virtual float GetSpeed() const;

		virtual void SetLoop(Loop loop = Loop::Repeat);
		virtual Loop GetLoop() const;

		virtual void AddTimeEvent(float time, const Function<void()> eventFunc);
		virtual void RemoveTimeEvent(float time);
		virtual void RemoveTimeEvent(const Function<void()> eventFunc);
		virtual void RemoveAllTimeEvents();

	protected:
		typedef Dictionary<float, Function<void()>> EventsDict;

		float      mTime;
		float      mInDurationTime;
		float      mDuration;
		float      mBeginTime;
		float      mEndTime;
		float      mDirection;
		float      mSpeed;
		Loop       mLoop;
		bool       mPlaying;
		EventsDict mTimeEvents;

	protected:
		virtual void Evaluate();

		void InitializeProperties();
	};
}