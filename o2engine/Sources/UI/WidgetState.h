#pragma once

#include "Animation/Animation.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Serialization.h"
#include "Utils/String.h"

namespace o2
{
	class Widget;

	class WidgetState: public ISerializable
	{
	public:
		String    name;
		Animation animation;
		float     offStateAnimationSpeed;

		WidgetState();
		WidgetState(const WidgetState& state);
		virtual ~WidgetState();

		WidgetState& operator=(const WidgetState& state);

		operator bool();
		WidgetState& operator=(bool state);

		virtual void SetState(bool state);
		virtual bool GetState() const;
		virtual void Update(float dt);

		SERIALIZABLE_IMPL(WidgetState);

		IOBJECT(WidgetState)
		{
			SRLZ_FIELD(name);
			SRLZ_FIELD(mState);
			SRLZ_FIELD(animation);
			SRLZ_FIELD(offStateAnimationSpeed);
		}

	protected:
		bool        mState;
		Ptr<Widget> mOwner;
	};
	typedef Vector<Ptr<WidgetState>>  StatesVec;
}
