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

		// It is called when clip duration changed
		void OnClipDurationChanged(float duration);

		friend class AnimationComponent;
		friend class AnimationState;
	};
}

CLASS_BASES_META(o2::AnimationPlayer)
{
	BASE_CLASS(o2::IAnimation);
}
END_META;
CLASS_FIELDS_META(o2::AnimationPlayer)
{
	FIELD().NAME(onTrackPlayerAdded).PUBLIC();
	FIELD().NAME(onTrackPlayerRemove).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mClip).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mClipOwner).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mTarget).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mAnimationState).PROTECTED();
	FIELD().NAME(mTrackPlayers).PROTECTED();
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
	PROTECTED_FUNCTION(void, OnClipDurationChanged, float);
}
END_META;
