#pragma once
#include "o2/Animation/IAnimation.h"
#include "o2/Animation/Tracks/IAnimationTrack.h"

namespace o2
{
	class AnimationClip;

	// ---------------------
	// Animation clip player
	// ---------------------
	class AnimationPlayer: public IAnimation
	{
	public:
		Function<void(IAnimationTrack::IPlayer*)> onTrackPlayerAdded;  // It is called when new track added
		Function<void(IAnimationTrack::IPlayer*)> onTrackPlayerRemove; // It is called when new track removing

	public:
		// Default constructor
		AnimationPlayer(IObject* target = nullptr, AnimationClip* clip = nullptr);

		// Destructor
		~AnimationPlayer();

		// Sets animation target
		// Bind all animation tracks to target's child fields (if it possible)
		void SetTarget(IObject* target, bool errors = true);

		// Returns animation's target
		IObject* GetTarget() const;

		// Sets animation clip
		void SetClip(AnimationClip* clip, bool owner = false);

		// Returns animation clip
		AnimationClip* GetClip() const;

		// Returns track players list
		const Vector<IAnimationTrack::IPlayer*>& GetTrackPlayers() const;

		IOBJECT(AnimationPlayer);

	protected:
		AnimationClip*  mClip = nullptr;           // Animation clip
		bool            mClipOwner = false;        // Is animation clip owned by this player
		IObject*        mTarget = nullptr;         // Target object
		AnimationState* mAnimationState = nullptr; // Animation state owner

		Vector<IAnimationTrack::IPlayer*> mTrackPlayers; // Animation clip track players

	protected:
		// Evaluates all Animation tracks by time
		void Evaluate() override;

		// Creates clip tracks players and bind to properties from target
		void BindTracks(bool errors);

		// Binds animation track
		void BindTrack(const ObjectType* type, void* castedTarget, IAnimationTrack* track, bool errors);

		// It is called when added new track in clip
		void OnClipTrackAdded(IAnimationTrack* track);

		// It is called when removing track from clip
		void OnClipTrackRemove(IAnimationTrack* track);

		friend class AnimationComponent;
	};
}

CLASS_BASES_META(o2::AnimationPlayer)
{
	BASE_CLASS(o2::IAnimation);
}
END_META;
CLASS_FIELDS_META(o2::AnimationPlayer)
{
	PUBLIC_FIELD(onTrackPlayerAdded);
	PUBLIC_FIELD(onTrackPlayerRemove);
	PROTECTED_FIELD(mClip);
	PROTECTED_FIELD(mClipOwner);
	PROTECTED_FIELD(mTarget);
	PROTECTED_FIELD(mAnimationState);
	PROTECTED_FIELD(mTrackPlayers);
}
END_META;
CLASS_METHODS_META(o2::AnimationPlayer)
{

	PUBLIC_FUNCTION(void, SetTarget, IObject*, bool);
	PUBLIC_FUNCTION(IObject*, GetTarget);
	PUBLIC_FUNCTION(void, SetClip, AnimationClip*, bool);
	PUBLIC_FUNCTION(AnimationClip*, GetClip);
	PUBLIC_FUNCTION(const Vector<IAnimationTrack::IPlayer*>&, GetTrackPlayers);
	PROTECTED_FUNCTION(void, Evaluate);
	PROTECTED_FUNCTION(void, BindTracks, bool);
	PROTECTED_FUNCTION(void, BindTrack, const ObjectType*, void*, IAnimationTrack*, bool);
	PROTECTED_FUNCTION(void, OnClipTrackAdded, IAnimationTrack*);
	PROTECTED_FUNCTION(void, OnClipTrackRemove, IAnimationTrack*);
}
END_META;
