#pragma once

#include "o2/Animation/Animation.h"

#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Widget;

	// -----------------------------------------------------------------------
	// Widget state. Could be true or false, and animates widget by this state
	// -----------------------------------------------------------------------
	class WidgetState: public ISerializable
	{
	public:
		String    name;                          // State name @SERIALIZABLE
		Animation animation;                     // Widget animation @SERIALIZABLE
		float     offStateAnimationSpeed = 1.0f; // False state transition animation speed @SERIALIZABLE

	public:
		PROPERTIES(WidgetState);
		PROPERTY(bool, state, SetState, GetState); // State flag property

	public:
		Function<void()> onStateFullyTrue;	  // This event calls when state is completely true (at the end of animation)
		Function<void()> onStateFullyFalse;   // This event calls when state is completely false (at the end of animation)
		Function<void()> onStateBecomesTrue;  // This event calls when state becomes to true
		Function<void()> onStateBecomesFalse; // This event calls when state becomes to true

	public:
		// Default constructor
		WidgetState();

		// Copy-constructor
		WidgetState(const WidgetState& state);

		// Destructor
		~WidgetState();

		// Boolean cast operator
		operator bool();

		// Assign from boolean operator
		WidgetState& operator=(bool state);

		// Sets current state
		void SetState(bool state);

		// Sets state immediately
		void SetStateForcible(bool state);

		// Returns current state
		bool GetState() const;

		// Updates animation
		void Update(float dt);

		SERIALIZABLE(WidgetState);

	protected:
		bool    mState = false;   // Current state @SERIALIZABLE
		Widget* mOwner = nullptr; // Owner widget pointer @EXCLUDE_POINTER_SEARCH

		friend class Widget;
	};
}

CLASS_BASES_META(o2::WidgetState)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::WidgetState)
{
	PUBLIC_FIELD(name).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(animation).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(offStateAnimationSpeed).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(state);
	PUBLIC_FIELD(onStateFullyTrue);
	PUBLIC_FIELD(onStateFullyFalse);
	PUBLIC_FIELD(onStateBecomesTrue);
	PUBLIC_FIELD(onStateBecomesFalse);
	PROTECTED_FIELD(mState).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mOwner);
}
END_META;
CLASS_METHODS_META(o2::WidgetState)
{

	PUBLIC_FUNCTION(void, SetState, bool);
	PUBLIC_FUNCTION(void, SetStateForcible, bool);
	PUBLIC_FUNCTION(bool, GetState);
	PUBLIC_FUNCTION(void, Update, float);
}
END_META;
