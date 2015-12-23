#pragma once

#include "Utils/Containers/Dictionary.h"
#include "Utils/Delegates.h"
#include "Utils/IObject.h"
#include "Utils/Property.h"
#include "Utils/Serialization.h"

namespace o2
{
	enum class Loop { 
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
		Property<bool>  playing;    // Is animation playing property
		Property<bool>  reversed;   // Is animation reversed property
		Property<float> speed;      // Animation speed. 1.0 is default
		Property<float> time;       // Animation time
		Property<float> relTime;    // Animation relative time. 0 - is start of animation, 1 - time at duration
		Property<float> beginBound; // Begin playing time bound property
		Property<float> endBound;   // End playing time bound property
		Property<Loop>  loop;       // Loop animation property
		Getter<float>   duration;   // Animation duration property

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
		virtual bool IsPlaying();

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

		// Calls for updating animated object, after updating time
		virtual void Evaluate();

		// Initializes properties
		void InitializeProperties();
	};
}