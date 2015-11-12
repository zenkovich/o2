#pragma once

#include "Animation/Animation.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Serialization.h"
#include "Utils/String.h"

namespace o2
{
	class UIWidget;

	// -----------------------------------------------------------------------
	// UIWidget state. Could be true or false, and animates widget by this state
	// -----------------------------------------------------------------------
	class UIWidgetState: public ISerializable
	{
	public:
		String           name;                   // State name
		Animation        animation;              // UIWidget animation
		float            offStateAnimationSpeed; // False state transition animation speed
		Function<void()> onStateFullyTrue;		 // This event calls when state is completely true (at the end of animation)
		Function<void()> onStateFullyFalse;		 // This event calls when state is completely false (at the end of animation)
		Function<void()> onStateBecomesTrue;	 // This event calls when state becomes to true
		Function<void()> onStateBecomesFalse;	 // This event calls when state becomes to true

		// Default constructor
		UIWidgetState();

		// Copy-constructor
		UIWidgetState(const UIWidgetState& state);

		// Destructor
		~UIWidgetState();

		// Boolean cast operator
		operator bool();

		// Assign from boolean operator
		UIWidgetState& operator=(bool state);

		// Sets current state
		void SetState(bool state);

		// Sets state immediately
		void SetStateForcible(bool state);

		// Returns current state
		bool GetState() const;

		// Updates animation
		void Update(float dt);

		SERIALIZABLE_IMPL(UIWidgetState);

		IOBJECT(UIWidgetState)
		{
			SRLZ_FIELD(name);
			SRLZ_FIELD(mState);
			SRLZ_FIELD(animation);
			SRLZ_FIELD(offStateAnimationSpeed);
		}

	protected:
		bool          mState; // Current state
		Ptr<UIWidget> mOwner; // Owner widget pointer
	};
	typedef Vector<Ptr<UIWidgetState>>  StatesVec;
}
