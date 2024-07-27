#include "o2/stdafx.h"
#include "MemoryAnalyzeableObject.h"

#include "o2/Utils/Memory/MemoryAnalyzer.h"

namespace o2
{
    MemoryAnalyzeObject::MemoryAnalyzeObject()
    {
        MemoryAnalyzer::OnObjectCreated(this);
        createIndex = GetNextCreateIndex();
    }

    MemoryAnalyzeObject::~MemoryAnalyzeObject()
    {
        MemoryAnalyzer::OnObjectDestroyed(this);
    }

    int MemoryAnalyzeObject::GetNextCreateIndex()
    {
        static int idx = 0;
        idx++;

        return idx;
    }
}
