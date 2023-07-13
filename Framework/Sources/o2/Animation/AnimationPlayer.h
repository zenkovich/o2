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
		Function<void(IAnimationTrack::IPlayer*)> onTrackPlayerAdded;  // Called when new track added
		Function<void(IAnimationTrack::IPlayer*)> onTrackPlayerRemove; // Called when new track removing

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

		// Called when added new track in clip
		void OnClipTrackAdded(IAnimationTrack* track);

		// Called when removing track from clip
		void OnClipTrackRemove(IAnimationTrack* track);

		// Called when clip duration changed
		void OnClipDurationChanged(float duration);

		friend class AnimationComponent;
		friend class AnimationState;
	};
}
// --- META ---

CLASS_BASES_META(o2::AnimationPlayer)
{
	BASE_CLASS(o2::IAnimation);
}
END_META;
CLASS_FIELDS_META(o2::AnimationPlayer)
{
	FIELD().PUBLIC().NAME(onTrackPlayerAdded);
	FIELD().PUBLIC().NAME(onTrackPlayerRemove);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mClip);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mClipOwner);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTarget);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mAnimationState);
	FIELD().PROTECTED().NAME(mTrackPlayers);
}
END_META;
CLASS_METHODS_META(o2::AnimationPlayer)
{

	FUNCTION().PUBLIC().CONSTRUCTOR(IObject*, AnimationClip*);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTarget, IObject*, bool);
	FUNCTION().PUBLIC().SIGNATURE(IObject*, GetTarget);
	FUNCTION().PUBLIC().SIGNATURE(void, SetClip, AnimationClip*, bool);
	FUNCTION().PUBLIC().SIGNATURE(AnimationClip*, GetClip);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<IAnimationTrack::IPlayer*>&, GetTrackPlayers);
	FUNCTION().PROTECTED().SIGNATURE(void, Evaluate);
	FUNCTION().PROTECTED().SIGNATURE(void, BindTracks, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, BindTrack, const ObjectType*, void*, IAnimationTrack*, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnClipTrackAdded, IAnimationTrack*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnClipTrackRemove, IAnimationTrack*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnClipDurationChanged, float);
}
END_META;
// --- END META ---
