#include "o2/stdafx.h"
#include "Ref.h"

namespace o2
{
	UInt16 RefCounterable::GetStrongReferencesCount() const
	{
		return GetRefCounter()->strongReferences;
	}

	UInt16 RefCounterable::GetWeakReferencesCount() const
	{
		return GetRefCounter()->weakReferences;
	}

	RefCounter* RefCounterable::GetRefCounter() const
	{
		if (!mRefCounter)
			mRefCounter = new RefCounter(&DefaultRefCounterImplementation::Instance);

		return mRefCounter;
	}
}