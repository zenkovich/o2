#include "State.h"

namespace o2
{
	State::State()
	{}

	State::~State()
	{}

	State::operator bool()
	{
		return GetState();
	}

	Ptr<State> State::Clone() const
	{
		return mnew State(*this);
	}

	void State::SetState(bool state)
	{
		mState = state;
	}

	bool State::GetState() const
	{
		return mState;
	}

	void State::Update(float dt)
	{}

	State& State::operator=(bool state)
	{
		SetState(state);
		return *this;
	}
}