#pragma once

namespace o2
{
    class MemoryAnalyzer
    {
    public:
        struct MemoryNode
        {
            struct ChildNode
            {
                bool        isOwner; // Is owner of child
                MemoryNode* node;    // Node pointer
            };

            void* memory;       // Pointer to allocated memory
            size_t size;        // Allocated size in bytes
            size_t summarySize; // Summary size of all children

            MemoryNode* parent = nullptr;
            std::vector<ChildNode> children; // Children nodes

            ~MemoryNode();

            void SummarizeSize();
        };

    public:
        static void OnRefCreated(void* ref);
        static void OnRefDestroyed(void* ref);

        // Builds memory tree from roots
        static MemoryNode* BuildMemoryTree(std::vector<void*> roots);

    private:
        static int mCurrentBuildMemoryTreeIdx;

    private:
        static std::vector<void*>& GetReferences();
        static std::vector<size_t>& GetFreeReferences();
        static void AllocateReferencesList();
    };
}
