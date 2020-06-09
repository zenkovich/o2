#pragma once
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class IAnimationTrack;

	template<typename _type>
	class AnimationTrack;

	// -----------------------------------------
	// Animation clip. Contains animation tracks
	// -----------------------------------------
	class AnimationClip: public ISerializable
	{
	public:
		PROPERTIES(AnimationClip);
		PROPERTY(Loop, loop, SetLoop, GetLoop); // Loop type property
		GETTER(float, duration, GetDuration);   // Duration getter

	public:
		Function<void()> onChanged; // It is called when some Animation track has changed

		Function<void(float)>            onDurationChange; // It is called when duration has changed
		Function<void(IAnimationTrack*)> onTrackAdded;     // It is called when new track added
		Function<void(IAnimationTrack*)> onTrackRemove;    // It is called when new track removing

	public:
		// Default constructor
		AnimationClip();

		// Copy-constructor
		AnimationClip(const AnimationClip& other);

		// Destructor
		~AnimationClip();

		// Assign operator
		AnimationClip& operator=(const AnimationClip& other);

		// Removes and clears all Animation tracks
		void Clear();

		// Returns clip duration
		float GetDuration() const;

		// Sets loop
		void SetLoop(Loop loop);

		// Returns loop
		Loop GetLoop() const;

		// Returns animation tracks
		Vector<IAnimationTrack*>& GetTracks();

		// Returns animation tracks
		const Vector<IAnimationTrack*>& GetTracks() const;

		// Returns is contains Animation track by path (some like "path/abc/cde")
		bool ContainsTrack(const String& path) const;

		// Returns Animation track by path (some like "path/abc/cde")
		template<typename _type>
		AnimationTrack<_type>* GetTrack(const String& path);

		// Adds animation track with specified path
		template<typename _type>
		AnimationTrack<_type>* AddTrack(const String& path);

		// Add animation track with specified path and type
		IAnimationTrack* AddTrack(const String& path, const Type& type);

		// Removes Animation track by path
		void RemoveTrack(const String& path);

		//insert animation

		// Returns parametric specified animation
		// Sample: Parametric(targetObjectPtr, targetValuePtr, someBegin, someEnd, 1.0f, 0.0f, 0.4f, 1.0f, 0.6f) 
		template<typename _type>
		static AnimationClip Parametric(const String& animatingValuePath,
										const _type& begin, const _type& end, float duration,
										float beginCoef, float beginCoefPosition,
										float endCoef, float endCoefPosition);

		// Returns animation from begin to end in duration with ease in
		template<typename _type>
		static AnimationClip EaseIn(const String& animatingValuePath,
									const _type& begin, const _type& end, float duration = 1.0f);

		// Returns animation from begin to end in duration with ease out
		template<typename _type>
		static AnimationClip EaseOut(const String& animatingValuePath,
									 const _type& begin, const _type& end, float duration = 1.0f);

		// Returns animation from begin to end in duration with ease in-out
		template<typename _type>
		static AnimationClip EaseInOut(const String& animatingValuePath,
									   const _type& begin, const _type& end, float duration = 1.0f);

		// Returns animation from begin to end in duration with linear transition
		template<typename _type>
		static AnimationClip Linear(const String& animatingValuePath,
									const _type& begin, const _type& end, float duration = 1.0f);

		SERIALIZABLE(AnimationClip);

	protected:
		Vector<IAnimationTrack*> mTracks; // Animation track @SERIALIZABLE

		float mDuration = 0.0f;   // Animation duration @SERIALIZABLE
		Loop  mLoop = Loop::None; // Animation loop type @SERIALIZABLE

	protected:
		// Returns Animation track by path
		template<typename _type>
		AnimationTrack<_type>* FindTrack(const String& path);

		// It is called when some Animation track has changed
		void OnTrackChanged();

		// Recalculates maximum duration by Animation tracks
		void RecalculateDuration();

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		// It is called when Animation track was added. Need to register value agent in animation target
		void OnTrackAdded(IAnimationTrack* track);

		friend class Animate;
		friend class AnimationComponent;
		friend class AnimationPlayer;
	};
}

#include "o2/Animation/Tracks/AnimationTrack.h"

namespace o2
{
	template<typename _type>
	AnimationTrack<_type>* AnimationClip::FindTrack(const String& path)
	{
		for (auto track : mTracks)
		{
			if (track->path == path)
				return dynamic_cast<AnimationTrack<_type>*>(val.animatedValue);
		}

		return nullptr;
	}

	template<typename _type>
	AnimationClip AnimationClip::Parametric(const String& animatingValuePath,
											const _type& begin, const _type& end, float duration,
											float beginCoef, float beginCoefPosition, float endCoef, float endCoefPosition)
	{
		AnimationClip res;
		*res.AddTrack<_type>(animatingValuePath) = AnimationTrack<_type>::Parametric(begin, end, duration,
																					 beginCoef, beginCoefPosition,
																					 endCoef, endCoefPosition);
		return res;
	}

	template<typename _type>
	AnimationClip AnimationClip::EaseIn(const String& animatingValuePath,
										const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		AnimationClip res;
		*res.AddTrack<_type>(animatingValuePath) = AnimationTrack<_type>::EaseIn(begin, end, duration);
		return res;
	}

	template<typename _type>
	AnimationClip AnimationClip::EaseOut(const String& animatingValuePath,
										 const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		AnimationClip res;
		*res.AddTrack<_type>(animatingValuePath) = AnimationTrack<_type>::EaseOut(begin, end, duration);
		return res;
	}

	template<typename _type>
	AnimationClip AnimationClip::EaseInOut(const String& animatingValuePath,
										   const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		AnimationClip res;
		*res.AddTrack<_type>(animatingValuePath) = AnimationTrack<_type>::EaseInOut(begin, end, duration);
		return res;
	}

	template<typename _type>
	AnimationClip AnimationClip::Linear(const String& animatingValuePath,
										const _type& begin, const _type& end, float duration /*= 1.0f*/)
	{
		AnimationClip res;
		*res.AddTrack<_type>(animatingValuePath) = AnimationTrack<_type>::Linear(begin, end, duration);
		return res;
	}

	template<typename _type>
	AnimationTrack<_type>* AnimationClip::AddTrack(const String& path)
	{
		auto track = mnew AnimationTrack<_type>();
		track->onKeysChanged += THIS_FUNC(OnTrackChanged);
		track->path = path;

		mTracks.Add(track);
		OnTrackAdded(track);

		return track;
	}

	template<typename _type>
	AnimationTrack<_type>* AnimationClip::GetTrack(const String& path)
	{
		for (auto track : mTracks)
		{
			if (track->path == path)
				return dynamic_cast<AnimationTrack<_type>*>(track);;
		}

		return nullptr;
	}
}

CLASS_BASES_META(o2::AnimationClip)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::AnimationClip)
{
	PUBLIC_FIELD(loop);
	PUBLIC_FIELD(duration);
	PUBLIC_FIELD(onChanged);
	PUBLIC_FIELD(onDurationChange);
	PUBLIC_FIELD(onTrackAdded);
	PUBLIC_FIELD(onTrackRemove);
	PROTECTED_FIELD(mTracks).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mDuration).DEFAULT_VALUE(0.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLoop).DEFAULT_VALUE(Loop::None).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::AnimationClip)
{

	PUBLIC_FUNCTION(void, Clear);
	PUBLIC_FUNCTION(float, GetDuration);
	PUBLIC_FUNCTION(void, SetLoop, Loop);
	PUBLIC_FUNCTION(Loop, GetLoop);
	PUBLIC_FUNCTION(Vector<IAnimationTrack*>&, GetTracks);
	PUBLIC_FUNCTION(const Vector<IAnimationTrack*>&, GetTracks);
	PUBLIC_FUNCTION(bool, ContainsTrack, const String&);
	PUBLIC_FUNCTION(IAnimationTrack*, AddTrack, const String&, const Type&);
	PUBLIC_FUNCTION(void, RemoveTrack, const String&);
	PROTECTED_FUNCTION(void, OnTrackChanged);
	PROTECTED_FUNCTION(void, RecalculateDuration);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, OnTrackAdded, IAnimationTrack*);
}
END_META;
