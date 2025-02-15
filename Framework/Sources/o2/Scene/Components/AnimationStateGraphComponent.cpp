#include "o2/stdafx.h"
#include "AnimationStateGraphComponent.h"
#include "AnimationComponent.h"

namespace o2
{
    AnimationStateGraphComponent::AnimationStateGraphComponent()
    {}

    AnimationStateGraphComponent::AnimationStateGraphComponent(const AnimationStateGraphComponent& other) :
        Component(other)
    {
        Reset();
    }

    AnimationStateGraphComponent::~AnimationStateGraphComponent()
    {}

    AnimationStateGraphComponent& AnimationStateGraphComponent::operator=(const AnimationStateGraphComponent& other)
    {
        Component::operator=(other);

        mStateGraph = other.mStateGraph;
        Reset();

        return *this;
    }

    void AnimationStateGraphComponent::OnUpdate(float dt)
    {
        CheckStartNextTransition();
        UpdateCurrentTransition(dt);
    }

    void AnimationStateGraphComponent::SetGraph(const AssetRef<AnimationStateGraphAsset>& graph)
    {
        mStateGraph = graph;
        Reset();
    }

    const AssetRef<AnimationStateGraphAsset>& AnimationStateGraphComponent::GetGraph() const
    {
        return mStateGraph;
    }

    void AnimationStateGraphComponent::GoToState(const String& name)
    {
        if (!mStateGraph)
            return;

        GoToState(mStateGraph->GetState(name));
    }

    void AnimationStateGraphComponent::GoToState(const Ref<AnimationGraphState>& state)
    {
        if (!state)
            return;

        StopTransition();

        auto sourceState = mCurrentState;
        mNextTransitions = mStateGraph->CalculatePath(sourceState, state);
    }

    void AnimationStateGraphComponent::ForcePlayState(const String& name)
    {
        if (!mStateGraph)
            return;

        ForcePlayState(mStateGraph->GetState(name));
    }

    void AnimationStateGraphComponent::ForcePlayState(const Ref<AnimationGraphState>& state)
    {
        if (!state)
            return;

        StopTransition();

        if (mCurrentState)
            mCurrentStatePlayer.Stop();

        mCurrentState = state;

        mCurrentStatePlayer.Setup(GetAnimationComponent(), mCurrentState, Ref(this));
        mCurrentStatePlayer.Play();
    }

    void AnimationStateGraphComponent::StopTransition()
    {
        mCurrentTransition = nullptr;
        mNextTransitions.Clear();

        if (mNextState)
        {
            mNextStatePlayer.Stop();
            mNextState = nullptr;
        }
    }

    const Ref<AnimationGraphState>& AnimationStateGraphComponent::GetCurrentState() const
    {
        return mCurrentState;
    }

    String AnimationStateGraphComponent::GetCurrentStateName() const
    {
        if (mCurrentState)
            return mCurrentState->name;

        return "";
    }

    void AnimationStateGraphComponent::OnInitialized()
    {
        Reset();
    }

    void AnimationStateGraphComponent::Reset()
    {
        StopTransition();

        if (mStateGraph)
            ForcePlayState(mStateGraph->GetState(mStateGraph->GetInitialState()));
    }

    void AnimationStateGraphComponent::CheckStartNextTransition()
    {
        if (mCurrentTransition)
            return;

        if (mNextTransitions.IsEmpty())
            return;

        auto nextTransition = mNextTransitions[0];
        float currentRelativeTime = mCurrentStatePlayer.GetTime() / mCurrentStatePlayer.GetDuration();
        bool canStartByTime = currentRelativeTime >= nextTransition->beginTimeRange && 
                              currentRelativeTime <= nextTransition->endTimeRange;

        if (!canStartByTime)
            return;

        mCurrentTransition = nextTransition;
        mNextTransitions.RemoveAt(0);

        mNextState = mCurrentTransition->GetDestinationState();
        mNextStatePlayer.Setup(GetAnimationComponent(), mNextState, Ref(this));
        mNextStatePlayer.Play();

        mCurrentTransitionTime = 0.0f;

		onTransitionStarted(mCurrentTransition);
    }

    void AnimationStateGraphComponent::UpdateCurrentTransition(float dt)
    {
        if (!mCurrentTransition)
            return;

        mCurrentTransitionTime += dt;
        float coef = mCurrentTransitionTime / mCurrentTransition->duration;

        bool isFinished = coef >= 1.0f;
        if (isFinished)
            coef = 1.0f;

        if (mCurrentTransition->curve)
            coef = mCurrentTransition->curve->Evaluate(coef);

        mCurrentStatePlayer.SetWeight(1.0f - coef);
        mNextStatePlayer.SetWeight(coef);

        if (isFinished)
        {
            mCurrentStatePlayer.Stop();

			onTransitionFinished(mCurrentTransition);

            mCurrentState = mNextState;
            mCurrentStatePlayer = mNextStatePlayer;

            mCurrentTransition = nullptr;
            mNextState = nullptr;
        }
    }

    Ref<AnimationComponent> AnimationStateGraphComponent::GetAnimationComponent()
    {
        if (mAnimationComponent)
            return mAnimationComponent.Lock();

        mAnimationComponent = GetActor()->GetComponent<AnimationComponent>();
        return mAnimationComponent.Lock();
    }

	void AnimationStateGraphComponent::StatePlayer::Setup(const Ref<AnimationComponent>& animationComponent, 
                                                          const Ref<AnimationGraphState>& state,
														  const Ref<AnimationStateGraphComponent>& owner)
    {
        players.Clear();
        this->state = state;
        this->owner = owner;
        
        if (!state)
            return;

        for (auto& animation : state->GetAnimations())
        {
            auto player = animationComponent->GetState(animation->name);
            if (!player)
                continue;

            players.Add({ animation, player });
        }
    }

    void AnimationStateGraphComponent::StatePlayer::Play()
    {
        for (auto& player : players)
            player.second->GetPlayer().Play();

		if (auto ownerRef = owner.Lock())
			ownerRef->onStateStarted(state);
    }

    void AnimationStateGraphComponent::StatePlayer::Stop()
    {
        for (auto& player : players)
            player.second->GetPlayer().Stop();

		if (auto ownerRef = owner.Lock())
			ownerRef->onStateFinished(state);
    }

    void AnimationStateGraphComponent::StatePlayer::SetWeight(float weight)
    {
        for (auto& player : players)
            player.second->SetWeight(weight);
    }

    float AnimationStateGraphComponent::StatePlayer::GetTime() const
    {
        if (players.IsEmpty())
            return 0.0f;

        return players[0].second->GetPlayer().GetInDurationTime();
    }

    float AnimationStateGraphComponent::StatePlayer::GetDuration() const
    {
        if (players.IsEmpty())
            return 0.0f;

        return players[0].second->GetPlayer().GetDuration();
    }

    const Ref<AnimationGraphState>& AnimationStateGraphComponent::GetNextState() const
    {
        return mNextState;
    }

    const Ref<AnimationGraphTransition>& AnimationStateGraphComponent::GetCurrentTransition() const
    {
        return mCurrentTransition;
    }

    const Vector<Ref<AnimationGraphTransition>>& AnimationStateGraphComponent::GetNextTransitions() const
    {
        return mNextTransitions;
    }

    String AnimationStateGraphComponent::GetName()
    {
        return "Animation State Graph";
    }

    String AnimationStateGraphComponent::GetCategory()
    {
        return "Animation";
    }

    String AnimationStateGraphComponent::GetIcon()
    {
        return "ui/UI4_graph_component.png";
    }

    DECLARE_TEMPLATE_CLASS(LinkRef<AnimationStateGraphComponent>);
}
// --- META ---

DECLARE_CLASS(o2::AnimationStateGraphComponent, o2__AnimationStateGraphComponent);
// --- END META ---
