#pragma once

namespace o2
{
    struct BaseRef;
    class IObject;

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

            void*    memory;      // Pointer to allocated memory
            IObject* object;      // Pointer to object, if can be casted
            size_t   size;        // Allocated size in bytes
            size_t   summarySize; // Summary size of all children

            MemoryNode* parent = nullptr;
            std::vector<ChildNode> children; // Children nodes

            ~MemoryNode();

            void SummarizeSize();
        };

    public:
        static void OnRefCreated(BaseRef* ref);
        static void OnRefDestroyed(BaseRef* ref);

        // Builds memory tree from roots
        static MemoryNode* BuildMemoryTree(std::vector<BaseRef*> roots);

    private:
        static int mCurrentBuildMemoryTreeIdx;

    private:
        static std::vector<BaseRef*>& GetReferences();
        static std::vector<size_t>& GetFreeReferences();
        static void AllocateReferencesList();
    };
}
