#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Function/Function.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Types/Containers/Map.h"

namespace o2
{
    // -------------------------
    // Basic animation interface
    // -------------------------
    class IAnimation : public IObject
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

    public:
        Function<void()>      onPlayEvent;   // Event calling when starting playing
        Function<void()>      onStopEvent;   // Event calling when stopping playing
        Function<void()>      onPlayedEvent; // Event calling when finishing playing
        Function<void(float)> onUpdate;      // Event calling on animation update

    public:
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

        // Returns time in loop
        virtual float GetLoopTime() const;

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
        virtual void AddTimeEvent(float time, const Function<void()>& eventFunc);

        // Removes event by time
        virtual void RemoveTimeEvent(float time);

        // Removes event
        virtual void RemoveTimeEvent(const Function<void()>& eventFunc);

        // Removes all events
        virtual void RemoveAllTimeEvents();

        IOBJECT(IAnimation);

    protected:
        float mTime;           // Current animation time, can be out of bounds
        float mInDurationTime; // In duration time
        float mDuration;       // Animation duration
        float mBeginTime;      // Begin time
        float mEndTime;        // End time
        float mDirection;      // Animation direction: 1 - forward, -1 - reversed
        float mSpeed;          // Animation speed, 1 is default
        Loop  mLoop;           // Loop type
        bool  mPlaying;        // True if animation playing

        Vector<Pair<float, Function<void()>>> mTimeEvents; // Animation time events

    protected:
        // Updates mTime and mInDurationTime
        void UpdateTime();

        // Called for updating animated object, after updating time
        virtual void Evaluate();
    };
}
// --- META ---

CLASS_BASES_META(o2::IAnimation)
{
    BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::IAnimation)
{
    FIELD().PUBLIC().NAME(playing);
    FIELD().PUBLIC().NAME(reversed);
    FIELD().PUBLIC().NAME(speed);
    FIELD().PUBLIC().NAME(time);
    FIELD().PUBLIC().NAME(relTime);
    FIELD().PUBLIC().NAME(beginBound);
    FIELD().PUBLIC().NAME(endBound);
    FIELD().PUBLIC().NAME(loop);
    FIELD().PUBLIC().NAME(duration);
    FIELD().PUBLIC().NAME(onPlayEvent);
    FIELD().PUBLIC().NAME(onStopEvent);
    FIELD().PUBLIC().NAME(onPlayedEvent);
    FIELD().PUBLIC().NAME(onUpdate);
    FIELD().PROTECTED().NAME(mTime);
    FIELD().PROTECTED().NAME(mInDurationTime);
    FIELD().PROTECTED().NAME(mDuration);
    FIELD().PROTECTED().NAME(mBeginTime);
    FIELD().PROTECTED().NAME(mEndTime);
    FIELD().PROTECTED().NAME(mDirection);
    FIELD().PROTECTED().NAME(mSpeed);
    FIELD().PROTECTED().NAME(mLoop);
    FIELD().PROTECTED().NAME(mPlaying);
    FIELD().PROTECTED().NAME(mTimeEvents);
}
END_META;
CLASS_METHODS_META(o2::IAnimation)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const IAnimation&);
    FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
    FUNCTION().PUBLIC().SIGNATURE(void, Play);
    FUNCTION().PUBLIC().SIGNATURE(void, PlayInBounds, float, float);
    FUNCTION().PUBLIC().SIGNATURE(void, PlayBackInBounds, float, float);
    FUNCTION().PUBLIC().SIGNATURE(void, TogglePlay);
    FUNCTION().PUBLIC().SIGNATURE(void, RewindAndPlay);
    FUNCTION().PUBLIC().SIGNATURE(void, Stop);
    FUNCTION().PUBLIC().SIGNATURE(void, SetBeginBound, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetBeginBound);
    FUNCTION().PUBLIC().SIGNATURE(void, SetEndBound, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetEndBound);
    FUNCTION().PUBLIC().SIGNATURE(void, SetBounds, float, float);
    FUNCTION().PUBLIC().SIGNATURE(void, ResetBounds);
    FUNCTION().PUBLIC().SIGNATURE(void, SetPlaying, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsPlaying);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTime, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetTime);
    FUNCTION().PUBLIC().SIGNATURE(float, GetLoopTime);
    FUNCTION().PUBLIC().SIGNATURE(float, GetDuration);
    FUNCTION().PUBLIC().SIGNATURE(void, SetRelTime, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetRelTime);
    FUNCTION().PUBLIC().SIGNATURE(void, GoToBegin);
    FUNCTION().PUBLIC().SIGNATURE(void, GoToEnd);
    FUNCTION().PUBLIC().SIGNATURE(void, PlayForward);
    FUNCTION().PUBLIC().SIGNATURE(void, PlayBack);
    FUNCTION().PUBLIC().SIGNATURE(void, SetReverse, bool);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsReversed);
    FUNCTION().PUBLIC().SIGNATURE(void, SetSpeed, float);
    FUNCTION().PUBLIC().SIGNATURE(float, GetSpeed);
    FUNCTION().PUBLIC().SIGNATURE(void, SetLoop, Loop);
    FUNCTION().PUBLIC().SIGNATURE(Loop, GetLoop);
    FUNCTION().PUBLIC().SIGNATURE(void, AddTimeEvent, float, const Function<void()>&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveTimeEvent, float);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveTimeEvent, const Function<void()>&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveAllTimeEvents);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateTime);
    FUNCTION().PROTECTED().SIGNATURE(void, Evaluate);
}
END_META;
// --- END META ---
