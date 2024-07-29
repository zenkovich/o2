#pragma once
#include <functional>
#include "o2/Utils/Debug/StackTrace.h"

namespace o2
{
    class IObject;

    struct MemoryAnalyzeObject
    {
        int markIndex = 0;   // Mark index for memory tree building
        int manageIndex = 0; // Manage index in objects array
        int createIndex = 0; // Index of object creation, for debugging

        StackTrace trace; // Stack trace of object creation

    public:
        MemoryAnalyzeObject();
        virtual ~MemoryAnalyzeObject();

        virtual std::byte* GetMemory() const { return nullptr; }
        virtual size_t GetMemorySize() const { return 0; }

        virtual IObject* GetIObject() const { return nullptr; }

        virtual void IterateChildren(const std::function<void(MemoryAnalyzeObject*)>& callback) {}
        virtual void IterateAllocations(const std::function<void(std::byte*, size_t)>& callback) {}

        virtual const std::type_info& GetTypeInfo() const { return typeid(this); }

        static int GetNextCreateIndex();
    };
}
