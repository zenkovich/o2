#pragma once
#include <functional>

namespace o2
{
    class IObject;
    class Type;
    struct MemoryAnalyzeObject;

    class MemoryAnalyzer
    {
    public:
        struct MemoryNode
        {
            std::string name; // Name of object
            std::string type; // Type of object

            void* memory = nullptr; // Pointer to allocated memory

            MemoryAnalyzeObject* object = nullptr; // Memory analyzeable object
            IObject* iobject = nullptr; // Pointer to IObject, if can be casted

            size_t size = 0;        // Allocated size in bytes
            size_t summarySize = 0; // Summary size of all children
            size_t leakedSize = 0;  // Summary leaked size

            MemoryNode* mainParent = nullptr; // Main parent node, the owner of this node
            std::vector<MemoryNode*> parents;              // Parent nodes
            std::vector<MemoryNode*> children;             // Children nodes

        public:
            ~MemoryNode();

            void SummarizeSize();
            void SummarizeLeakedSize();
        };

    public:
        static bool enabledObjectsTracking; // Enable objects tracking

    public:
        static void OnObjectCreated(MemoryAnalyzeObject* obj);
        static void OnObjectDestroyed(MemoryAnalyzeObject* obj);

        // Builds memory tree from roots
        static MemoryNode* BuildMemoryTree(const std::vector<MemoryAnalyzeObject*>& roots);

    private:
        static int mCurrentBuildMemoryTreeIdx;

    private:
        static std::vector<MemoryAnalyzeObject*>& GetAnalyzeObjects();
        static std::vector<size_t>& GetFreeAnalyzeObjects();

        static void AllocateAnalyzeObjects();

        static void BuildSubTree(MemoryNode* root, const std::vector<MemoryAnalyzeObject*>& roots,
                                 std::map<std::byte*, MemoryNode*>& memoryNodes,
                                 std::vector<std::pair<MemoryNode*, std::vector<MemoryAnalyzeObject*>>>& currentNodes,
                                 std::vector<std::pair<MemoryNode*, std::vector<MemoryAnalyzeObject*>>>& nextNodes,
                                 std::vector<MemoryAnalyzeObject*>& childRefs,
                                 const std::vector<MemoryAnalyzeObject*>& sortedObjects);

        static std::string TryFindFieldName(const Type* nodeIObjectType, void* nodeObject,
                                            MemoryAnalyzeObject*& object);

        static void SearchChildrenObjects(const std::vector<MemoryAnalyzeObject*>& sortedObjects,
                                          std::byte* objectMemoryBegin, std::byte* objectMemoryEnd,
                                          MemoryAnalyzeObject*& object,
                                          std::vector<MemoryAnalyzeObject*>& childRefs);
    };
}
