#include "o2/stdafx.h"
#include "MemoryAnalyzer.h"

#include "xutility"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
    int MemoryAnalyzer::mCurrentBuildMemoryTreeIdx = 0;

    std::vector<void*>& MemoryAnalyzer::GetReferences()
    {
        static std::vector<void*> data;
        return data;
    }

    std::vector<size_t>& MemoryAnalyzer::GetFreeReferences()
    {
        static std::vector<size_t> data;
        return data;
    }

    MemoryAnalyzer::MemoryNode::~MemoryNode()
    {
        for (auto& child : children)
        {
            if (child.isOwner)
                delete child.node;
        }
    }

    void MemoryAnalyzer::MemoryNode::SummarizeSize()
    {
        summarySize = size;

        for (auto& child : children)
        {
            if (!child.isOwner)
                return;

            child.node->SummarizeSize();
            summarySize += child.node->summarySize;
        }
    }

    void MemoryAnalyzer::OnRefCreated(void* ref)
    {
        auto& freeRefs = GetFreeReferences();
        if (freeRefs.empty())
            AllocateReferencesList();

        auto& refs = GetReferences();

        auto nextFreeIdx = freeRefs.back();

        refs[nextFreeIdx] = ref;
        freeRefs.pop_back();

        reinterpret_cast<Ref<RefCounterable>*>(ref)->mManagedIndex = nextFreeIdx;
    }

    void MemoryAnalyzer::AllocateReferencesList()
    {
        auto& refs = GetReferences();
        auto& freeRefs = GetFreeReferences();

        auto size = refs.size();
        auto newSize = std::max((size_t)1024, size + size/2); 

        refs.resize(newSize, nullptr);
        for (size_t i = 0; i < newSize - size; i++)
            freeRefs.push_back(newSize - i - 1);
    }

    void MemoryAnalyzer::OnRefDestroyed(void* ref)
    {
        auto idx = reinterpret_cast<Ref<RefCounterable>*>(ref)->mManagedIndex;
        GetReferences()[idx] = nullptr;
        GetFreeReferences().push_back(idx);
    }

    MemoryAnalyzer::MemoryNode* MemoryAnalyzer::BuildMemoryTree(std::vector<void*> roots)
    {
        // Increment build index for marking tree
        mCurrentBuildMemoryTreeIdx++;

        auto& allRefs = GetReferences();

        // Get all reference pointers and sort
        std::vector<Ref<RefCounterable>*> refs;
        refs.reserve(allRefs.size());
        for (auto ref : allRefs)
        {
            if (ref)
                refs.push_back(reinterpret_cast<Ref<RefCounterable>*>(ref));
        }

        std::sort(refs.begin(), refs.end(), [](Ref<RefCounterable>* a, Ref<RefCounterable>* b) { return a < b; });

        // Create root node
        MemoryNode* root = new MemoryNode();

        // Begin marking and collecting algorithm
        std::vector<Ref<RefCounterable>*> rootRefs;
        for (auto& root : roots)
            rootRefs.push_back(reinterpret_cast<Ref<RefCounterable>*>(root));

        std::vector<std::pair<MemoryNode*, std::vector<Ref<RefCounterable>*>>> currentNodes = { { root, rootRefs } };
        std::vector<std::pair<MemoryNode*, std::vector<Ref<RefCounterable>*>>> nextNodes;

        auto& memoryInfos = MemoryManager::Instance().mAllocs;

        while (!currentNodes.empty())
        {
            for (auto& it : currentNodes)
            {
                auto& node = it.first;

                for (auto& ref : it.second)
                {
                    std::byte* memory = reinterpret_cast<std::byte*>(ref->mRefCounter);
                    if (!memory)
                        continue;

                    auto memoryInfoIt = memoryInfos.find(memory);
                    if (memoryInfoIt == memoryInfos.end())
                    {
                        o2Debug.LogError("Failed to find memory info for memory address: %p", memory);
                        continue;
                    }

                    auto& memoryInfo = memoryInfoIt->second;

                    if (memoryInfo.markIndex == mCurrentBuildMemoryTreeIdx)
                        continue;

                    memoryInfo.markIndex = mCurrentBuildMemoryTreeIdx;

                    MemoryNode* childNode = new MemoryNode();
                    childNode->memory = memory;
                    childNode->size = memoryInfo.size;
                    childNode->parent = node;
                    node->children.push_back({ true, childNode });

                    std::byte* memoryEnd = memory + memoryInfo.size;

                    std::vector<Ref<RefCounterable>*> childRefs;

                    auto refIt = std::lower_bound(refs.begin(), refs.end(), reinterpret_cast<Ref<RefCounterable>*>(memory));
                    while (refIt != refs.end() && reinterpret_cast<std::byte*>(*refIt) < memoryEnd)
                    {
                        childRefs.push_back(*refIt);
                        refIt++;
                    }

                    nextNodes.push_back({ childNode, childRefs });
                }
            }

            currentNodes = nextNodes;
            nextNodes.clear();
        }

        root->SummarizeSize();

        return root;
    }

}
