#include "o2/stdafx.h"
#include "Singleton.h"

namespace o2
{
    Vector<Ref<RefCounterable>>& GetSingletonsList()
    {
        static Vector<Ref<RefCounterable>> list;
        return list;
    }
}
