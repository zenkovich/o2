#include "o2/stdafx.h"
#include "AnimationClip.h"

#include "o2/Animation/Tracks/AnimationTrack.h"
#include "o2/Scene/Components/AnimationComponent.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Debug/Debug.h"

namespace o2
{
	AnimationClip::AnimationClip() 
	{}

	AnimationClip::AnimationClip(const AnimationClip& other):
		loop(this), duration(this)
	{
		for (auto track : other.mTracks)
		{
			IAnimationTrack* newTrack = track->CloneAs<IAnimationTrack>();
			mTracks.Add(newTrack);
			OnTrackAdded(newTrack);
		}

		mLoop = other.mLoop;

		RecalculateDuration();
	}

	AnimationClip::~AnimationClip()
	{
		Clear();
	}

	AnimationClip& AnimationClip::operator=(const AnimationClip& other)
	{
		Clear();

		for (auto track : other.mTracks)
		{
			IAnimationTrack* newTrack = track->CloneAs<IAnimationTrack>();
			mTracks.Add(newTrack);
			OnTrackAdded(newTrack);
		}

		mLoop = other.mLoop;

		RecalculateDuration();

		return *this;
	}

	void AnimationClip::Clear()
	{
		for (auto track : mTracks)
		{
			track->onKeysChanged -= THIS_FUNC(OnTrackChanged);
			delete track;
		}

		mTracks.Clear();
	}

	float AnimationClip::GetDuration() const
	{
		return mDuration;
	}

	void AnimationClip::SetLoop(Loop loop)
	{
		mLoop = loop;
	}

	Loop AnimationClip::GetLoop() const
	{
		return mLoop;
	}

	Vector<IAnimationTrack*>& AnimationClip::GetTracks()
	{
		return mTracks;
	}

	const Vector<IAnimationTrack*>& AnimationClip::GetTracks() const
	{
		return mTracks;
	}

	bool AnimationClip::ContainsTrack(const String& path) const
	{
		for (auto track : mTracks)
		{
			if (track->path == path)
				return true;
		}

		return false;
	}

	IAnimationTrack* AnimationClip::AddTrack(const String& path, const Type& type)
	{
		auto animTypeName = "o2::AnimationTrack<" + type.GetName() + ">";
		auto animType = dynamic_cast<const ObjectType*>(o2Reflection.GetType(animTypeName));
		if (!animType)
		{
			o2Debug.LogWarning("Can't create Animation track: can't find animation type " + animTypeName);
			return nullptr;
		}

		auto track = dynamic_cast<IAnimationTrack*>(animType->DynamicCastToIObject(animType->CreateSample()));
		track->path = path;
		track->onKeysChanged += THIS_FUNC(OnTrackChanged);

		mTracks.Add(track);
		OnTrackAdded(track);

		return track;
	}

	void AnimationClip::RemoveTrack(const String& path)
	{
		for (auto track : mTracks)
		{
			if (track->path == path)
			{
				onTrackRemove(track);

				delete track;
				mTracks.Remove(track);

				onChanged();
				return;
			}
		}
	}

	void AnimationClip::OnTrackChanged()
	{
		RecalculateDuration();

		onChanged();
	}

	void AnimationClip::RecalculateDuration()
	{
		float lastDuration = mDuration;
		mDuration = 0.0f;

		for (auto track : mTracks)
			mDuration = Math::Max(mDuration, track->GetDuration());

		if (!Math::Equals(lastDuration, mDuration))
			onDurationChange(mDuration);
	}

	void AnimationClip::OnDeserialized(const DataNode& node)
	{
		for (auto track : mTracks)
			track->onKeysChanged += THIS_FUNC(OnTrackChanged);

		OnTrackChanged();
	}

	void AnimationClip::OnTrackAdded(IAnimationTrack* track)
	{
		track->onKeysChanged += THIS_FUNC(OnTrackChanged);

		onTrackAdded(track);
		onChanged();
	}
}

DECLARE_CLASS(o2::AnimationClip);
