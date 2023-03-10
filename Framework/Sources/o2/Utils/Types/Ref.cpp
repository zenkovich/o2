#include "o2/stdafx.h"
#include "Ref.h"

namespace o2
{
	RefCounter* RefCounterable::GetRefCounter() const
	{
		if (!mRefCounter)
			mRefCounter = new RefCounter(&DefaultRefCounterImplementation::Instance);

		return mRefCounter;
	}
}