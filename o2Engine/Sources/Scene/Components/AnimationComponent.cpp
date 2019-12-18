#include "stdafx.h"
#include "AnimationComponent.h"

#include "Animation/AnimatedValue.h"

namespace o2
{
	AnimationComponent::AnimationComponent()
	{}

	AnimationComponent::AnimationComponent(const AnimationComponent& other)
	{
		for (auto state : other.mStates)
		{
			AnimationState* newState = mnew AnimationState(*state);
			AddState(newState);
		}
	}

	AnimationComponent::~AnimationComponent()
	{
		RemoveAllStates();
	}

	AnimationComponent& AnimationComponent::operator=(const AnimationComponent& other)
	{
		RemoveAllStates();

		for (auto state : other.mStates)
		{
			AnimationState* newState = mnew AnimationState(*state);
			AddState(newState);
		}

		return *this;
	}

	void AnimationComponent::Update(float dt)
	{
		for (auto state : mStates)
			state->animation.Update(dt);

		for (auto val : mValues)
			val->Update();

		if (mBlend.time > 0)
			mBlend.Update(dt);
	}

	AnimationState* AnimationComponent::AddState(AnimationState* state)
	{
		mStates.Add(state);

		state->animation.SetTarget(this);
		state->animation.mAnimationState = state;
		state->mOwner = this;

		for (auto& val : state->animation.mAnimatedValues)
			val.animatedValue->RegInAnimatable(state, val.targetPath);

		return state;
	}

	AnimationState* AnimationComponent::AddState(const String& name, const Animation& animation, const AnimationMask& mask,
											 float weight)
	{
		AnimationState* res = mnew AnimationState(name);
		res->animation = animation;
		res->mask = mask;
		res->weight = weight;
		return AddState(res);
	}

	AnimationState* AnimationComponent::AddState(const String& name)
	{
		AnimationState* res = mnew AnimationState(name);
		return AddState(res);
	}

	void AnimationComponent::RemoveState(AnimationState* state)
	{
		for (auto& val : state->animation.mAnimatedValues)
			UnregAnimatedValue(val.animatedValue, val.targetPath);

		mStates.Remove(state);
		delete state;
	}

	void AnimationComponent::RemoveState(const String& name)
	{
		RemoveState(GetState(name));
	}

	void AnimationComponent::RemoveAllStates()
	{
		for (auto state : mStates)
			delete state;

		mStates.Clear();

		for (auto val : mValues)
			delete val;

		mValues.Clear();
	}

	AnimationState* AnimationComponent::GetState(const String& name)
	{
		for (auto state : mStates)
			if (state->name == name)
				return state;

		return nullptr;
	}

	const AnimationStatesVec& AnimationComponent::GetStates() const
	{
		return mStates;
	}

	AnimationState* AnimationComponent::Play(const Animation& animation, const String& name)
	{
		AnimationState* state = AddState(name, animation, AnimationMask(), 1.0f);
		state->animation.Play();
		return state;
	}

	AnimationState* AnimationComponent::Play(const Animation& animation)
	{
		AnimationState* state = AddState("unknown", animation, AnimationMask(), 1.0f);
		state->animation.Play();
		return state;
	}

	AnimationState* AnimationComponent::Play(const String& name)
	{
		AnimationState* state = GetState(name);
		if (!state)
		{
			o2Debug.LogWarning("Can't play animation: " + name);
			return nullptr;
		}
		state->animation.Play();
		return state;
	}

	AnimationState* AnimationComponent::BlendTo(const Animation& animation, const String& name, float duration /*= 1.0f*/)
	{
		AnimationState* state = AddState(name, animation, AnimationMask(), 1.0f);
		return BlendTo(state, duration);
	}

	AnimationState* AnimationComponent::BlendTo(const Animation& animation, float duration /*= 1.0f*/)
	{
		AnimationState* state = AddState("unknown", animation, AnimationMask(), 1.0f);
		return BlendTo(state, duration);
	}

	AnimationState* AnimationComponent::BlendTo(const String& name, float duration /*= 1.0f*/)
	{
		AnimationState* state = GetState(name);
		if (!state)
		{
			o2Debug.LogWarning("Can't blend animation: " + name);
			return nullptr;
		}
		return BlendTo(state, duration);
	}

	AnimationState* AnimationComponent::BlendTo(AnimationState* state, float duration /*= 1.0f*/)
	{
		mBlend.mBlendOffStates.Clear();

		for (auto state : mStates)
			if (state->animation.IsPlaying())
				mBlend.mBlendOffStates.Add(state);

		mBlend.mBlendOnState = state;
		mBlend.duration = duration;
		mBlend.time = duration;

		state->animation.Play();

		return state;
	}

	void AnimationComponent::Stop(const String& animationName)
	{
		AnimationState* state = GetState(animationName);
		if (!state)
		{
			o2Debug.LogWarning("Can't stop animation: " + animationName);
			return;
		}
		state->animation.Stop();
	}

	void AnimationComponent::StopAll()
	{
		for (auto state : mStates)
			state->animation.Stop();

		mBlend.time = -1;
	}

	String AnimationComponent::GetName() const
	{
		return "Animation";
	}

	String AnimationComponent::GetCategory() const
	{
		return "Basic";
	}

	String AnimationComponent::GetIcon() const
	{
		return "ui/UI4_animation_component.png";
	}

	void AnimationComponent::UnregAnimatedValue(IAnimatedValue* value, const String& path)
	{
		for (auto val : mValues)
		{
			if (val->path == path)
			{
				val->RemoveValue(value);

				if (val->IsEmpty())
				{
					mValues.Remove(val);
					delete val;
				}

				return;
			}
		}
	}

	void AnimationComponent::OnStatesListChanged()
	{
		for (auto state : mStates)
		{
			if (!state->mOwner)
			{
				state->animation.SetTarget(this);
				state->animation.mAnimationState = state;
				state->mOwner = this;

				for (auto& val : state->animation.mAnimatedValues)
					val.animatedValue->RegInAnimatable(state, val.targetPath);
			}
		}
	}

	void AnimationComponent::BlendState::Update(float dt)
	{
		time -= dt;
		float cf = Math::Max(0.0f, time) / duration;

		for (auto state : mBlendOffStates)
			state->workWeight = cf;

		mBlendOnState->workWeight = 1.0f - cf;
	}
}

DECLARE_CLASS(o2::AnimationComponent);
