#pragma once

namespace o2
{
	// ---------------------------------------------------
	// Animation, that can be edited from animation editor
	// ---------------------------------------------------
	class IEditableAnimation
	{
	public:
		// It is called when animation started to edit. It means that animation must be deactivated
		virtual void BeginAnimationEdit() = 0;

		// It is called when animation finished editing. ANimation must be reactivated
		virtual void EndAnimationEdit() = 0;
	};
}
