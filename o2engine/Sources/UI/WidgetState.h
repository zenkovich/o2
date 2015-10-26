#pragma once

#include "Animation/Animation.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Serialization.h"
#include "Utils/String.h"

namespace o2
{
	class Widget;

	// -----------------------------------------------------------------------
	// Widget state. Could be true or false, and animates widget by this state
	// -----------------------------------------------------------------------
	class WidgetState: public ISerializable
	{
	public:
		String    name;                   // State name
		Animation animation;              // Widget animation
		float     offStateAnimationSpeed; // False state transition animation speed

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

		// Returns current state
		bool GetState() const;

		// Updates animation
		void Update(float dt);

		SERIALIZABLE_IMPL(WidgetState);

		IOBJECT(WidgetState)
		{
			SRLZ_FIELD(name);
			SRLZ_FIELD(mState);
			SRLZ_FIELD(animation);
			SRLZ_FIELD(offStateAnimationSpeed);
		}

	protected:
		bool        mState; // Current state
		Ptr<Widget> mOwner; // Owner widget pointer
	};
	typedef Vector<Ptr<WidgetState>>  StatesVec;
}
