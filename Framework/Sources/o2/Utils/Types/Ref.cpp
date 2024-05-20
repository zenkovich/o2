#include "o2/stdafx.h"
#include "Ref.h"

namespace o2
{
    RefCounterable::RefCounterable(RefCounter* refCounter):
        mRefCounter(refCounter)
    {}

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
        Assert(mRefCounter, "Trying to get the reference counter, but it is null. Use mmake<> to create Ref<> and PostRefConstruct() to initialize references during object creation.");
        return mRefCounter;
    }

    void RefCounterable::SetRefCounter(RefCounter* refCounter)
    {
        mRefCounter = refCounter;
    }
}