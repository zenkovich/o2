#pragma once

#include "Utils/String.h"
#include "Utils/Memory/Ptr.h"

namespace o2
{
	class State
	{
	public:
		String name;

		State();
		virtual ~State();

		operator bool();
		State& operator=(bool state);

		virtual Ptr<State> Clone() const;

		virtual void SetState(bool state);
		virtual bool GetState() const;
		virtual void Update(float dt);

	protected:
		bool mState;
	};
	typedef Vector<Ptr<State>>  StatesVec;
}
