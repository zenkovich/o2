#include "o2/stdafx.h"
#include "AnimationComponent.h"

#include "o2/Animation/Tracks/AnimationTrack.h"

namespace o2
{
	AnimationComponent::AnimationComponent()
	{}

	AnimationComponent::AnimationComponent(const AnimationComponent& other)
	{
		for (auto state : other.mStates)
			AddState(state->CloneAs<AnimationState>());
	}

	AnimationComponent::~AnimationComponent()
	{
		RemoveAllStates();
	}

	AnimationComponent& AnimationComponent::operator=(const AnimationComponent& other)
	{
		RemoveAllStates();

		for (auto state : other.mStates)
			AddState(state->CloneAs<AnimationState>());

		return *this;
	}

	void AnimationComponent::Update(float dt)
	{
		if (mInEditMode)
			return;

		for (auto state : mStates)
		{
			if (state->mAnimation)
				state->player.Update(dt);
		}

		for (auto val : mValues)
			val->Update();

		if (mBlend.time > 0)
			mBlend.Update(dt);
	}

	AnimationState* AnimationComponent::AddState(AnimationState* state)
	{
		state->player.SetTarget(mOwner);
		state->player.SetPlaying(state->autoPlay);
		state->player.mAnimationState = state;
		state->mOwner = this;

		for (auto trackPlayer : state->player.mTrackPlayers)
			trackPlayer->RegMixer(state, trackPlayer->GetTrack()->path);

		mStates.Add(state);

		return state;
	}

	AnimationState* AnimationComponent::AddState(const String& name, const AnimationClip& animation, const AnimationMask& mask,
												 float weight)
	{
		AnimationState* res = mnew AnimationState(name);
		res->mAnimation = AnimationAssetRef(mnew AnimationAsset(animation));
		res->mask = mask;
		res->mWeight = weight;
		return AddState(res);
	}

	AnimationState* AnimationComponent::AddState(const String& name)
	{
		AnimationState* res = mnew AnimationState(name);
		return AddState(res);
	}

	void AnimationComponent::RemoveState(AnimationState* state)
	{
		for (auto trackPlayer : state->player.mTrackPlayers)
			UnregTrack(trackPlayer, trackPlayer->GetTrack()->path);

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
		{
			if (state->name == name)
				return state;
		}

		return nullptr;
	}

	const Vector<AnimationState*>& AnimationComponent::GetStates() const
	{
		return mStates;
	}

	AnimationState* AnimationComponent::Play(const AnimationClip& animation, const String& name)
	{
		AnimationState* state = AddState(name, animation, AnimationMask(), 1.0f);
		state->player.Play();
		return state;
	}

	AnimationState* AnimationComponent::Play(const AnimationClip& animation)
	{
		AnimationState* state = AddState("unknown", animation, AnimationMask(), 1.0f);
		state->player.Play();
		return state;
	}

	AnimationState* AnimationComponent::Play(const String& name)
	{
		AnimationState* state = GetState(name);
		if (!state || !state->mAnimation)
		{
			o2Debug.LogWarning("Can't play animation: " + name);
			return nullptr;
		}

		state->player.Play();

		return state;
	}

	AnimationState* AnimationComponent::BlendTo(const AnimationClip& animation, const String& name, float duration /*= 1.0f*/)
	{
		AnimationState* state = AddState(name, animation, AnimationMask(), 1.0f);
		return BlendTo(state, duration);
	}

	AnimationState* AnimationComponent::BlendTo(const AnimationClip& animation, float duration /*= 1.0f*/)
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
		mBlend.blendOffStates.Clear();

		for (auto state : mStates)
		{
			if (state->mAnimation)
			{
				if (state->player.IsPlaying())
					mBlend.blendOffStates.Add(state);
			}
		}

		mBlend.blendOnState = state;
		mBlend.duration = duration;
		mBlend.time = duration;

		if (state->mAnimation)
			state->player.Play();

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

		state->player.Stop();
	}

	void AnimationComponent::StopAll()
	{
		for (auto state : mStates)
			state->player.Stop();

		mBlend.time = -1;
	}

	String AnimationComponent::GetName()
	{
		return "Animation";
	}

	String AnimationComponent::GetCategory()
	{
		return "Basic";
	}

	String AnimationComponent::GetIcon()
	{
		return "ui/UI4_animation_component.png";
	}

	void AnimationComponent::BeginAnimationEdit()
	{
		mInEditMode = true;
	}

	void AnimationComponent::EndAnimationEdit()
	{
		mInEditMode = false;
	}

	void AnimationComponent::OnStart()
	{
		ReattachAnimationStates();

		for (auto state : mStates)
		{
			if (state->autoPlay)
				state->player.Play();
		}
	}

	void AnimationComponent::UnregTrack(IAnimationTrack::IPlayer* player, const String& path)
	{
		for (auto val : mValues)
		{
			if (val->path == path)
			{
				val->RemoveTrack(player);

				if (val->IsEmpty())
				{
					mValues.Remove(val);
					delete val;
				}

				return;
			}
		}
	}

	void AnimationComponent::OnStateAnimationTrackAdded(AnimationState* state, IAnimationTrack::IPlayer* player)
	{
		player->RegMixer(state, player->GetTrack()->path);
	}

	void AnimationComponent::OnStateAnimationTrackRemoved(AnimationState* state, IAnimationTrack::IPlayer* player)
	{
		UnregTrack(player, player->GetTrack()->path);
	}

	void AnimationComponent::ReattachAnimationStates()
	{
		auto statesCopy = mStates;
		for (auto state : statesCopy)
		{
			if (state && !state->mOwner)
			{
				mStates.Remove(state);
				AddState(state);
			}
		}
	}

	void AnimationComponent::BlendState::Update(float dt)
	{
		time -= dt;
		float cf = Math::Max(0.0f, time) / duration;

		for (auto state : blendOffStates)
			state->blend = cf;

		blendOnState->blend = 1.0f - cf;
	}

	template<>
	void AnimationComponent::TrackMixer<int>::Update()
	{
		AnimationState* firstValueState = tracks[0].first;
		AnimationTrack<int>::Player* firstValue = tracks[0].second;

		float weightsSum = firstValueState->mWeight*firstValueState->blend*firstValueState->mask.GetNodeWeight(path);
		float valueSum = (float)firstValue->GetValue();

		for (int i = 1; i < tracks.Count(); i++)
		{
			AnimationState* valueState = tracks[i].first;
			AnimationTrack<int>::Player* value = tracks[i].second;

			weightsSum += valueState->mWeight*valueState->blend*valueState->mask.GetNodeWeight(path);
			valueSum += (float)value->GetValue();
		}

		int resValue = Math::RoundToInt(valueSum / weightsSum);
		target->SetValue(resValue);
	}
	
	template<>
	void AnimationComponent::TrackMixer<bool>::Update()
	{
		AnimationState* firstValueState = tracks[0].first;
		AnimationTrack<bool>::Player* firstValue = tracks[0].second;
	
		float weightsSum = firstValueState->mWeight*firstValueState->blend*firstValueState->mask.GetNodeWeight(path);
		float valueSum = firstValue->GetValue() ? 1.0f : 0.0f;
	
		for (int i = 1; i < tracks.Count(); i++)
		{
			AnimationState* valueState = tracks[i].first;
			AnimationTrack<bool>::Player* value = tracks[i].second;
	
			weightsSum += valueState->mWeight*valueState->blend*valueState->mask.GetNodeWeight(path);
			valueSum += value->GetValue() ? 1.0f : 0.0f;
		}
	
		bool resValue = (valueSum / weightsSum) > 0.5f;
		target->SetValue(resValue);
	}
}

DECLARE_CLASS(o2::AnimationComponent);
