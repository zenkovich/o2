#pragma once
#include <functional>

namespace o2
{
    struct BaseRef;
    struct BaseVector;
    class IObject;

    struct MemoryAnalyzeObject
    {
        int markIndex = 0;   // Mark index for memory tree building
        int manageIndex = 0; // Manage index in objects array

    public:
        MemoryAnalyzeObject();
        virtual ~MemoryAnalyzeObject();

        virtual std::byte* GetMemory() const = 0;
        virtual size_t GetMemorySize() const { return 0; }

        virtual IObject* GetIObject() const { return nullptr; }

        virtual std::vector<std::pair<void*, size_t>> GetAllocations() const { return {}; }

        virtual void IterateChildren(const std::function<void(const std::string&, MemoryAnalyzeObject*)>& callback) {}

        virtual const std::type_info& GetTypeInfo() const { return typeid(this); }
    };

    class MemoryAnalyzer
    {
    public:
        struct MemoryNode
        {
            std::string name; // Name of object

            void* memory = nullptr; // Pointer to allocated memory

            MemoryAnalyzeObject* object = nullptr; // Memory analyzeable object
            IObject*             iobject = nullptr; // Pointer to IObject, if can be casted

            size_t size = 0;        // Allocated size in bytes
            size_t summarySize = 0; // Summary size of all children

            MemoryNode*              mainParent = nullptr; // Main parent node, the owner of this node
            std::vector<MemoryNode*> parents;              // Parent nodes
            std::vector<MemoryNode*> children;             // Children nodes

        public:
            ~MemoryNode();

            void SummarizeSize();
        };

    public:
        static void OnObjectCreated(MemoryAnalyzeObject* obj);
        static void OnObjectDestroyed(MemoryAnalyzeObject* obj);

        // Builds memory tree from roots
        static MemoryNode* BuildMemoryTree(std::vector<MemoryAnalyzeObject*> roots);

    private:
        static int mCurrentBuildMemoryTreeIdx;

    private:
        static std::vector<MemoryAnalyzeObject*>& GetAnalyzeObjects();
        static std::vector<size_t>& GetFreeAnalyzeObjects();

        static void AllocateAnalyzeObjects();
    };
}
