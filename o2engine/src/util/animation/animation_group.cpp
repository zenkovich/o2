#include "animation_group.h"

#include "util/math/math.h"

OPEN_O2_NAMESPACE

AnimationGroup::AnimationGroup()
{
}

AnimationGroup::~AnimationGroup()
{
	removeAllAnimations();
}

void AnimationGroup::addAnimation(IAnimation* animation)
{
	mAnimations.push_back(animation);
	mDuration = Max(mDuration, animation->GetDuration());
}

void AnimationGroup::removeAnimation(IAnimation* animation)
{
	AnimationsVec::iterator fnd = std::find(mAnimations.begin(), mAnimations.end(), animation);
	if (fnd != mAnimations.end())
		mAnimations.erase(fnd);

	SafeRelease(animation);

	mDuration = 0;
	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
	{
		if (mDuration < (*it)->GetDuration())
			mDuration = (*it)->GetDuration();
	}
}

void AnimationGroup::removeAllAnimations()
{
	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		SafeRelease(*it);

	mAnimations.clear();
	mDuration = 0;
}

const AnimationGroup::AnimationsVec& AnimationGroup::getAnimations() const
{
	return mAnimations;
}

void AnimationGroup::Play(float begin, float end)
{
	IAnimation::Play(begin, end);

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->Play(begin, end);
}

void AnimationGroup::Play(bool restart /*= false*/, bool stopAtEnd /*= true*/)
{
	IAnimation::Play(restart, stopAtEnd);
}

void AnimationGroup::SetTime(float time)
{
	IAnimation::SetTime(time);

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->SetTime(time);
}

void AnimationGroup::Update(float dt)
{
	IAnimation::Update(dt);

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->Update(dt);
}

void AnimationGroup::Stop()
{
	IAnimation::Stop();

	for (AnimationsVec::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
		(*it)->Stop();
}

CLOSE_O2_NAMESPACE