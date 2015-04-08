#pragma once

#include "public.h"
#include "animation.h"
#include "util/smart_ptrs.h"

OPEN_O2_NAMESPACE

/** Animations group. Animates some animation with same absolute time and intervals. */
class AnimationGroup: public IAnimation
{
public:
	typedef Vector< IAnimation* > AnimationsVec;

protected:
	AnimationsVec mAnimations; /**< Animations. */

public:
	/** ctor. */
	AnimationGroup();

	/** dtor. */
	~AnimationGroup();

	/** Adding animation in group. */
	void addAnimation(IAnimation* animation);

	/** Removing animation. */
	void removeAnimation(IAnimation* animation);

	/** Removing all animations. */
	void removeAllAnimations();

	/** Returns vector of animations. */
	const AnimationsVec& getAnimations() const;

	/** Updating animation. */
	void Update(float dt);

	/** Playing animation. */
	void Play(float begin, float end);

	/** Playing animation. */
	void Play(bool restart = false, bool stopAtEnd = true);

	/** Stop animation. */
	void Stop();

	/** Setting absolute time. */
	void SetTime(float time);
};

CLOSE_O2_NAMESPACE
