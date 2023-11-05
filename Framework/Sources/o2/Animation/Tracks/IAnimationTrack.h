#pragma once

#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
    class AnimationClip;
    class AnimationPlayer;
    class AnimationState;

    // -------------------------
    // Animation track interface
    // -------------------------
    class IAnimationTrack: public ISerializable
    {
    public:
        // --------------------------------
        // Animation track player interface
        // --------------------------------
        class IPlayer: public IAnimation
        {
        public:
            // Sets target changing delegate
            virtual void SetTargetDelegate(const Function<void()>& changeEvent) {}

            // Sets target by void pointer
            virtual void SetTargetVoid(void* target) {}

            // Sets target by void pointer and change event
            virtual void SetTargetVoid(void* target, const Function<void()>& changeEvent) {}

            // Sets target property by void pointer
            virtual void SetTargetProxyVoid(void* target) {}

            // Sets animation track
            virtual void SetTrack(IAnimationTrack* track);

            // Returns animation track
            virtual IAnimationTrack* GetTrack() const { return nullptr; }

            // Registering this in animation track agent
            virtual void RegMixer(AnimationState* state, const String& path) {}

            // Force setting time (using in Animation): works same as update, but by hard setting time
            void ForceSetTime(float time, float duration);

            //Returns owner player
            const AnimationPlayer* GetOwnerPlayer() const;

            IOBJECT(IPlayer);

        protected:
            AnimationPlayer* mOwnerPlayer = nullptr;

            friend class AnimationPlayer;
        };

    public:
        PROPERTIES(IAnimationTrack);
        GETTER(float, duration, GetDuration);   // Animation duration getter

        Loop loop = Loop::None; // Animation loop type @SERIALIZABLE

        String path; // Animated property path @SERIALIZABLE

    public:
        Function<void()> onKeysChanged; // Called when keys has changed

    public:
        // Default constructor
        IAnimationTrack() {}

        // Copy-constructor
        IAnimationTrack(const IAnimationTrack& other): duration(this), loop(other.loop), path(other.path) {}

        // Copy operator
        IAnimationTrack& operator=(const IAnimationTrack& other);

        // Called when beginning keys batch change. After this call all keys modifications will not be update approximation
        // Used for optimizing many keys change
        virtual void BeginKeysBatchChange() {}

        // Called when keys batch change completed. Updates approximation
        virtual void CompleteKeysBatchingChange() {}

        // Returns track duration
        virtual float GetDuration() const { return 0; }

        // Creates track-type specific player
        virtual IPlayer* CreatePlayer() const { return nullptr; }

        // Returns owner clip
        const AnimationClip* GetOwnerClip() const;

        SERIALIZABLE(IAnimationTrack);

    protected:
        AnimationClip* mOwnerClip = nullptr;

        friend class AnimationClip;
    };
};
// --- META ---

CLASS_BASES_META(o2::IAnimationTrack)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::IAnimationTrack)
{
    FIELD().PUBLIC().NAME(duration);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(Loop::None).NAME(loop);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(path);
    FIELD().PUBLIC().NAME(onKeysChanged);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOwnerClip);
}
END_META;
CLASS_METHODS_META(o2::IAnimationTrack)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const IAnimationTrack&);
    FUNCTION().PUBLIC().SIGNATURE(void, BeginKeysBatchChange);
    FUNCTION().PUBLIC().SIGNATURE(void, CompleteKeysBatchingChange);
    FUNCTION().PUBLIC().SIGNATURE(float, GetDuration);
    FUNCTION().PUBLIC().SIGNATURE(IPlayer*, CreatePlayer);
    FUNCTION().PUBLIC().SIGNATURE(const AnimationClip*, GetOwnerClip);
}
END_META;

CLASS_BASES_META(o2::IAnimationTrack::IPlayer)
{
    BASE_CLASS(o2::IAnimation);
}
END_META;
CLASS_FIELDS_META(o2::IAnimationTrack::IPlayer)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOwnerPlayer);
}
END_META;
CLASS_METHODS_META(o2::IAnimationTrack::IPlayer)
{

    FUNCTION().PUBLIC().SIGNATURE(void, SetTargetDelegate, const Function<void()>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargetVoid, void*);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargetVoid, void*, const Function<void()>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargetProxyVoid, void*);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTrack, IAnimationTrack*);
    FUNCTION().PUBLIC().SIGNATURE(IAnimationTrack*, GetTrack);
    FUNCTION().PUBLIC().SIGNATURE(void, RegMixer, AnimationState*, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, ForceSetTime, float, float);
    FUNCTION().PUBLIC().SIGNATURE(const AnimationPlayer*, GetOwnerPlayer);
}
END_META;
// --- END META ---
