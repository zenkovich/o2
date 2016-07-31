#pragma once

#include "Animation/Animation.h"
#include "Utils/Task.h"

namespace o2
{
	// ---------------------------------------------------
	// Animation task: waits for delay and plays animation
	// ---------------------------------------------------
	class AnimationTask: public Task
	{
	public:
		// Constructor
		AnimationTask(const Animation& animation, float delay = 0.0f);

		// Updates task
		void Update(float dt);

		// Checks task for complete
		bool IsDone() const;

	protected:
		float     mRemainingDelay; 
		Animation mAnimation;
	};
}