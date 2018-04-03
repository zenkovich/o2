#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Delegates.h"
#include "Utils/IObject.h"
#include "Utils/Property.h"
#include "Utils/Serializable.h"

namespace o2
{
	enum class Loop
	{
		None,    // No loop 
		Repeat,  // Repeats from start to end
		PingPong // Repeats from start to end, from end to start, to end ...
	};

	// -------------------------
	// Basic animation interface
	// -------------------------
	class IAnimation: public ISerializable
	{
	public:
		PROPERTIES(IAnimation);
		PROPERTY(bool, playing, SetPlaying, IsPlaying);            // Is animation playing property
		PROPERTY(bool, reversed, SetReverse, IsReversed);          // Is animation reversed property
		PROPERTY(float, speed, SetSpeed, GetSpeed);                // Animation speed. 1.0 is default
		PROPERTY(float, time, SetTime, GetTime);                   // Animation time
		PROPERTY(float, relTime, SetRelTime, GetRelTime);          // Animation relative time. 0 - is start of animation, 1 - time at duration
		PROPERTY(float, beginBound, SetBeginBound, GetBeginBound); // Begin playing time bound property
		PROPERTY(float, endBound, SetEndBound, GetEndBound);       // End playing time bound property
		PROPERTY(Loop, loop, SetLoop, GetLoop);                    // Loop animation property
		GETTER(float, duration, GetDuration);                      // Animation duration property

		Function<void()>      onPlayEvent;   // Event calling when starting playing
		Function<void()>      onStopEvent;   // Event calling when stopping playing
		Function<void()>      onPlayedEvent; // Event calling when finishing playing
		Function<void(float)> onUpdate;      // Event calling on animation update

		// Default constructor
		IAnimation();

		// Copy-constructor
		IAnimation(const IAnimation& other);

		// Virtual destructor
		virtual ~IAnimation();

		// Assign operator
		IAnimation& operator=(const IAnimation& other);

		// Updates animation mTime and mInDurationTime, stops and calls events
		virtual void Update(float dt);

		// Starting playing animation 
		virtual void Play();

		// Starting playing animation in bounds
		virtual void PlayInBounds(float beginTime, float endTime);

		// Starting playing animation back in bounds
		virtual void PlayBackInBounds(float beginTime, float endTime);

		// Toggles animation direction
		virtual void TogglePlay();

		// Rewinds to starts and play animation
		virtual void RewindAndPlay();

		// Stops animation
		virtual void Stop();

		// Sets begin time bound
		virtual void SetBeginBound(float time);

		// Returns begin time bound
		virtual float GetBeginBound() const;

		// Sets end time bound
		virtual void SetEndBound(float time);

		// Returns end time bound
		virtual float GetEndBound() const;

		// Sets animation time bounds
		virtual void SetBounds(float beginTime, float endTime);

		// Resets bound on range from 0 to duration
		virtual void ResetBounds();

		// Sets playing flag
		virtual void SetPlaying(bool playing);

		// Returns true if animation playing
		virtual bool IsPlaying() const;

		// Sets time
		virtual void SetTime(float time);

		// Returns time
		virtual float GetTime() const;

		// Returns duration
		virtual float GetDuration() const;

		// Sets relative time: 0 - is start of animation, 1 - time at duration
		virtual void SetRelTime(float relTime);

		// Returns relative time: 0 - is start of animation, 1 - time at duration
		virtual float GetRelTime() const;

		// Sets animation time to begin
		virtual void GoToBegin();

		// Sets animatio time to end
		virtual void GoToEnd();

		// Starts playing animation forward
		virtual void PlayForward();

		// Starts animation playing reversed
		virtual void PlayBack();

		// Sets reverse animation flag
		virtual void SetReverse(bool reverse);

		// Returns true if animation reversed
		virtual bool IsReversed() const;

		// Sets animation speed. 1 is default
		virtual void SetSpeed(float speed);

		// Returns animation speed. 1 is default
		virtual float GetSpeed() const;

		// Sets animation loop type
		virtual void SetLoop(Loop loop = Loop::Repeat);

		// Returns animation loop type
		virtual Loop GetLoop() const;

		// Adds event on time line
		virtual void AddTimeEvent(float time, const Function<void()> eventFunc);

		// Removes event by time
		virtual void RemoveTimeEvent(float time);

		// Removes event
		virtual void RemoveTimeEvent(const Function<void()> eventFunc);

		// Removes all events
		virtual void RemoveAllTimeEvents();

		SERIALIZABLE(IAnimation);

	protected:
		typedef Dictionary<float, Function<void()>> EventsDict;

		float      mTime;           // Current animation time, can be out of bounds
		float      mInDurationTime; // In duration time
		float      mDuration;       // Animation duration @SERIALIZABLE
		float      mBeginTime;      // Begin time
		float      mEndTime;        // End time
		float      mDirection;      // Animation direction: 1 - forward, -1 - reversed
		float      mSpeed;          // Animation speed, 1 is default
		Loop       mLoop;           // Loop type @SERIALIZABLE
		bool       mPlaying;        // True if animation playing
		EventsDict mTimeEvents;     // Animation time events

	protected:
		// Updates mTime and mInDurationTime
		void UpdateTime();

		// It is called for updating animated object, after updating time
		virtual void Evaluate();
	};
}

CLASS_BASES_META(o2::IAnimation)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::IAnimation)
{
	PUBLIC_FIELD(playing);
	PUBLIC_FIELD(reversed);
	PUBLIC_FIELD(speed);
	PUBLIC_FIELD(time);
	PUBLIC_FIELD(relTime);
	PUBLIC_FIELD(beginBound);
	PUBLIC_FIELD(endBound);
	PUBLIC_FIELD(loop);
	PUBLIC_FIELD(duration);
	PUBLIC_FIELD(onPlayEvent);
	PUBLIC_FIELD(onStopEvent);
	PUBLIC_FIELD(onPlayedEvent);
	PUBLIC_FIELD(onUpdate);
	PROTECTED_FIELD(mTime);
	PROTECTED_FIELD(mInDurationTime);
	PROTECTED_FIELD(mDuration).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mBeginTime);
	PROTECTED_FIELD(mEndTime);
	PROTECTED_FIELD(mDirection);
	PROTECTED_FIELD(mSpeed);
	PROTECTED_FIELD(mLoop).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPlaying);
	PROTECTED_FIELD(mTimeEvents);
}
END_META;
CLASS_METHODS_META(o2::IAnimation)
{

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, Play);
	PUBLIC_FUNCTION(void, PlayInBounds, float, float);
	PUBLIC_FUNCTION(void, PlayBackInBounds, float, float);
	PUBLIC_FUNCTION(void, TogglePlay);
	PUBLIC_FUNCTION(void, RewindAndPlay);
	PUBLIC_FUNCTION(void, Stop);
	PUBLIC_FUNCTION(void, SetBeginBound, float);
	PUBLIC_FUNCTION(float, GetBeginBound);
	PUBLIC_FUNCTION(void, SetEndBound, float);
	PUBLIC_FUNCTION(float, GetEndBound);
	PUBLIC_FUNCTION(void, SetBounds, float, float);
	PUBLIC_FUNCTION(void, ResetBounds);
	PUBLIC_FUNCTION(void, SetPlaying, bool);
	PUBLIC_FUNCTION(bool, IsPlaying);
	PUBLIC_FUNCTION(void, SetTime, float);
	PUBLIC_FUNCTION(float, GetTime);
	PUBLIC_FUNCTION(float, GetDuration);
	PUBLIC_FUNCTION(void, SetRelTime, float);
	PUBLIC_FUNCTION(float, GetRelTime);
	PUBLIC_FUNCTION(void, GoToBegin);
	PUBLIC_FUNCTION(void, GoToEnd);
	PUBLIC_FUNCTION(void, PlayForward);
	PUBLIC_FUNCTION(void, PlayBack);
	PUBLIC_FUNCTION(void, SetReverse, bool);
	PUBLIC_FUNCTION(bool, IsReversed);
	PUBLIC_FUNCTION(void, SetSpeed, float);
	PUBLIC_FUNCTION(float, GetSpeed);
	PUBLIC_FUNCTION(void, SetLoop, Loop);
	PUBLIC_FUNCTION(Loop, GetLoop);
	PUBLIC_FUNCTION(void, AddTimeEvent, float, const Function<void()>);
	PUBLIC_FUNCTION(void, RemoveTimeEvent, float);
	PUBLIC_FUNCTION(void, RemoveTimeEvent, const Function<void()>);
	PUBLIC_FUNCTION(void, RemoveAllTimeEvents);
	PROTECTED_FUNCTION(void, UpdateTime);
	PROTECTED_FUNCTION(void, Evaluate);
}
END_META;
