#pragma once

#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
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

			IOBJECT(IPlayer);
		};

	public:
		PROPERTIES(IAnimationTrack);
		GETTER(float, duration, GetDuration);   // Animation duration getter

		Loop loop = Loop::None; // Animation loop type @SERIALIZABLE

		String path; // Animated property path @SERIALIZABLE

	public:
		Function<void()> onKeysChanged; // It is called when keys has changed

	public:
		// Default constructor
		IAnimationTrack() {}

		// Copy-constructor
		IAnimationTrack(const IAnimationTrack& other): duration(this), loop(other.loop), path(other.path) {}

		// Copy operator
		IAnimationTrack& operator=(const IAnimationTrack& other);

		// It is called when beginning keys batch change. After this call all keys modifications will not be update approximation
		// Used for optimizing many keys change
		virtual void BeginKeysBatchChange() {}

		// It is called when keys batch change completed. Updates approximation
		virtual void CompleteKeysBatchingChange() {}

		// Returns track duration
		virtual float GetDuration() const { return 0; }

		// Creates track-type specific player
		virtual IPlayer* CreatePlayer() const { return nullptr; }

		SERIALIZABLE(IAnimationTrack);
	};
};

CLASS_BASES_META(o2::IAnimationTrack)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::IAnimationTrack)
{
	PUBLIC_FIELD(duration);
	PUBLIC_FIELD(loop).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(path).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(onKeysChanged);
}
END_META;
CLASS_METHODS_META(o2::IAnimationTrack)
{

	PUBLIC_FUNCTION(void, BeginKeysBatchChange);
	PUBLIC_FUNCTION(void, CompleteKeysBatchingChange);
	PUBLIC_FUNCTION(float, GetDuration);
	PUBLIC_FUNCTION(IPlayer*, CreatePlayer);
}
END_META;

CLASS_BASES_META(o2::IAnimationTrack::IPlayer)
{
	BASE_CLASS(o2::IAnimation);
}
END_META;
CLASS_FIELDS_META(o2::IAnimationTrack::IPlayer)
{
}
END_META;
CLASS_METHODS_META(o2::IAnimationTrack::IPlayer)
{

	PUBLIC_FUNCTION(void, SetTargetDelegate, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetVoid, void*);
	PUBLIC_FUNCTION(void, SetTargetVoid, void*, const Function<void()>&);
	PUBLIC_FUNCTION(void, SetTargetProxyVoid, void*);
	PUBLIC_FUNCTION(void, SetTrack, IAnimationTrack*);
	PUBLIC_FUNCTION(IAnimationTrack*, GetTrack);
	PUBLIC_FUNCTION(void, RegMixer, AnimationState*, const String&);
	PUBLIC_FUNCTION(void, ForceSetTime, float, float);
}
END_META;
