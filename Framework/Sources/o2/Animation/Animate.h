#pragma once
#include "o2/Animation/AnimationClip.h"
#include "o2/Animation/Tracks/AnimationFloatTrack.h"
#include "o2/Animation/Tracks/AnimationTrack.h"
#include "o2/Animation/Tracks/AnimationVec2FTrack.h"
#include "o2/Animation/Tracks/AnimationColor4Track.h"

namespace o2
{
    // -----------------------
    // Key container interface
    // -----------------------
    struct IKeyContainer: public RefCounterable
    {
        // Virtual destructor
        virtual ~IKeyContainer() {}

        // Applies stored key to animation
        virtual void Apply(float time) = 0;
    };

    // ----------------------
    // Template key container
    // ----------------------
    template <typename T>
    struct KeyContainer : public IKeyContainer
    {
        typename AnimationTrack<T>::Key key;

        Ref<AnimationTrack<T>> animatedValue;

    public:
        void Apply(float time);
    };

    // ---------------
    // Vec2F container
    // ---------------
    template <>
    struct KeyContainer<Vec2F> : public IKeyContainer
    {
        Curve::Key timeKey;

        Ref<AnimationTrack<Vec2F>> animatedValue;

    public:
        void Apply(float time);
    };

    // -------------------
    // Scale key container
    // -------------------
    struct ScaleKeyContainer : public IKeyContainer
    {
        AnimationTrack<float>::Key keyX;
        AnimationTrack<float>::Key keyY;

        Ref<AnimationTrack<float>> animatedValueX;
        Ref<AnimationTrack<float>> animatedValueY;

    public:
        void Apply(float time);
    };

    // --------------------------------------------
    // Class for building simple animation sequence
    // --------------------------------------------
    class Animate
    {
    public:
        // Constructor. Takes object as parameter
        explicit Animate(IObject& object);

        // Animation cast operator. Needs to store as animation
        operator Ref<AnimationClip>() const;

        // Inserts delay for seconds
        Animate& Wait(float seconds);

        // Applies stored transformations after seconds
        Animate& For(float seconds);

        // Splits sequence
        Animate& Then();

        // Moves object in (x,y)
        Animate& Move(float x, float y);

        // Moves object into position
        Animate& Move(const Vec2F& point);

        // Moves object by path from points
        Animate& Move(const Vector<Vec2F>& points);

        // Changes alpha
        Animate& Alpha(float alpha);

        // Shows object
        Animate& Show();

        // Hides object
        Animate& Hide();

        // Sets color
        Animate& Color(const Color4& color);

        // Sets color
        Animate& Color(int r, int g, int b, int a);

        // Sets scale
        Animate& Scale(float scale);

        // Sets scale
        Animate& Scale(const Vec2F& scale);

        // Rotates object
        Animate& Rotate(float angle);

        // Sets animation looped
        Animate& Looped();

        // Sets pin pong loop
        Animate& PingPong();

        // Changes specified parameter
        template<typename T>
        Animate& Change(T* target, const T& value);

    protected:
        IObject* mTarget = nullptr; // Target animating object

        Ref<AnimationClip> mAnimation; // Building animation

        bool  mKeysApplied = false; // Is stored keys was applied
        float mTime = 0.0f;         // Current sequence time

        Vector<Ref<IKeyContainer>> mKeyContainers; // Stored keys that applies in For()

        Function<void()> mFunction; // Stored callback that applies in For()

        Ref<AnimationTrack<Color4>> mColorAnimatedValue;   // Color Animation track, stored when needs
        Ref<AnimationTrack<Vec2F>> mPositionAnimatedValue; // Position Animation track, stored when needs
        Ref<AnimationTrack<float>> mScaleXAnimatedValue;   // Scale X Animation track, stored when needs
        Ref<AnimationTrack<float>> mScaleYAnimatedValue;   // Scale Y Animation track, stored when needs
        Ref<AnimationTrack<float>> mRotationAnimatedValue; // Rotation Animation track, stored when needs

    protected:
        // Checks color Animation track: creates them if needed
        void CheckColorAnimatedValue();

        // Checks position Animation track: creates them if needed
        void CheckPositionAnimatedvalue();

        // Checks scale Animation track: creates them if needed
        void CheckScaleAnimatedValue();

        // Checks rotate Animation track: creates them if needed
        void CheckRotateAnimatedValue();

        // Checks applied keys: if keys was applied, clears keys containers
        void CheckAppliedKeys();

        // Applies keys and function to animation at current time
        void ApplyKeys();
    };

    template<typename T>
    Animate& Animate::Change(T* target, const T& value)
    {
        CheckAppliedKeys();

        KeyContainer<T>* container = mnew KeyContainer<T>();
        container->animatedValue = GetAnimatedValue(target);
        container->key.value = value;
        mKeyContainers.Add(container);

        return *this;
    }

    template <typename T>
    void KeyContainer<T>::Apply(float time)
    {
        key.position = time;
        animatedValue->AddKey(key);
    }

}
