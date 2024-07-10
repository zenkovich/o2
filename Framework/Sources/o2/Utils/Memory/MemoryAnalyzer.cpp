#include "o2/stdafx.h"
#include "MemoryAnalyzer.h"

#include "xutility"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
    int MemoryAnalyzer::mCurrentBuildMemoryTreeIdx = 0;

    std::vector<BaseRef*>& MemoryAnalyzer::GetReferences()
    {
        static std::vector<BaseRef*> data;
        return data;
    }

    std::vector<size_t>& MemoryAnalyzer::GetFreeReferences()
    {
        static std::vector<size_t> data;
        return data;
    }

    std::vector<BaseVector*>& MemoryAnalyzer::GetVectors()
    {
        static std::vector<BaseVector*> data;
        return data;
    }

    std::vector<size_t>& MemoryAnalyzer::GetFreeVectors()
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

    void MemoryAnalyzer::OnRefCreated(BaseRef* ref)
    {
        auto& freeRefs = GetFreeReferences();
        if (freeRefs.empty())
            AllocateReferencesList();

        auto& refs = GetReferences();

        auto nextFreeIdx = freeRefs.back();

        refs[nextFreeIdx] = ref;
        freeRefs.pop_back();

        reinterpret_cast<BaseRef*>(ref)->mManagedIndex = nextFreeIdx;
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

    void MemoryAnalyzer::OnRefDestroyed(BaseRef* ref)
    {
        auto idx = reinterpret_cast<BaseRef*>(ref)->mManagedIndex;
        GetReferences()[idx] = nullptr;
        GetFreeReferences().push_back(idx);
    }

    void MemoryAnalyzer::OnVectorCreated(BaseVector* vector)
    {
        auto& freeVectors = GetFreeVectors();
        if (freeVectors.empty())
            AllocateVectorsList();

        auto& vectors = GetVectors();

        auto nextFreeIdx = freeVectors.back();

        vectors[nextFreeIdx] = vector;
        freeVectors.pop_back();

        reinterpret_cast<BaseVector*>(vector)->mManagedIndex = nextFreeIdx;
    }

    void MemoryAnalyzer::OnVectorDestroyed(BaseVector* vector)
    {
        auto idx = reinterpret_cast<BaseVector*>(vector)->mManagedIndex;
        GetVectors()[idx] = nullptr;
        GetFreeVectors().push_back(idx);
    }

    void MemoryAnalyzer::AllocateVectorsList()
    {
        auto& vectors = GetVectors();
        auto& freeVectors = GetFreeVectors();

        auto size = vectors.size();
        auto newSize = std::max((size_t)1024, size + size/2);

        vectors.resize(newSize, nullptr);
        for (size_t i = 0; i < newSize - size; i++)
            freeVectors.push_back(newSize - i - 1);
    }

    MemoryAnalyzer::MemoryNode* MemoryAnalyzer::BuildMemoryTree(std::vector<BaseRef*> roots)
    {
        // Increment build index for marking tree
        mCurrentBuildMemoryTreeIdx++;

        auto& allRefs = GetReferences();
        auto& allVectors = GetVectors();

        // Get all reference pointers and sort
        std::vector<BaseRef*> refs;
        refs.reserve(allRefs.size());
        for (auto ref : allRefs)
        {
            if (ref)
                refs.push_back(reinterpret_cast<BaseRef*>(ref));
        }

        std::sort(refs.begin(), refs.end(), [](BaseRef* a, BaseRef* b) { return a < b; });

        // Get all vector pointers and sort
        std::vector<BaseVector*> vectors;
        vectors.reserve(allVectors.size());
        for (auto vector : allVectors)
        {
            if (vector)
                vectors.push_back(reinterpret_cast<BaseVector*>(vector));
        }

        std::sort(vectors.begin(), vectors.end(), [](BaseVector* a, BaseVector* b) { return a < b; });

        // Create root node
        MemoryNode* root = new MemoryNode();

        // Begin marking and collecting algorithm
        std::vector<BaseRef*> rootRefs;
        for (auto& root : roots)
            rootRefs.push_back(reinterpret_cast<BaseRef*>(root));

        std::vector<std::pair<MemoryNode*, std::vector<BaseRef*>>> currentNodes = { { root, rootRefs } };
        std::vector<std::pair<MemoryNode*, std::vector<BaseRef*>>> nextNodes;

        auto& memoryInfos = MemoryManager::Instance().mAllocs;

        while (!currentNodes.empty())
        {
            for (auto& it : currentNodes)
            {
                auto& node = it.first;

                for (auto& ref : it.second)
                {
                    std::byte* memory = reinterpret_cast<std::byte*>(ref->GetRefCounterPtr());
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
                    {
                        node->children.push_back({ false, reinterpret_cast<MemoryNode*>(memoryInfo.memoryNode) });
                        continue;
                    }

                    memoryInfo.markIndex = mCurrentBuildMemoryTreeIdx;

                    MemoryNode* childNode = new MemoryNode();
                    childNode->name = (String)ref->GetTypeInfo().name();
                    childNode->memory = memory;
                    childNode->object = ref->GetObjectPtr();
                    childNode->size = memoryInfo.size;
                    childNode->parent = node;
                    node->children.push_back({ true, childNode });
                    memoryInfo.memoryNode = childNode;

                    std::byte* memoryEnd = memory + memoryInfo.size;

                    std::vector<BaseRef*> childRefs;

                    auto refIt = std::lower_bound(refs.begin(), refs.end(), reinterpret_cast<BaseRef*>(memory));
                    while (refIt != refs.end() && reinterpret_cast<std::byte*>(*refIt) < memoryEnd)
                    {
                        childRefs.push_back(*refIt);
                        refIt++;
                    }

                    auto vectorIt = std::lower_bound(vectors.begin(), vectors.end(), reinterpret_cast<BaseVector*>(memory));
                    while (vectorIt != vectors.end() && reinterpret_cast<std::byte*>(*vectorIt) < memoryEnd)
                    {
                        if ((*vectorIt)->mMarkIndex == mCurrentBuildMemoryTreeIdx)
                        {
                            node->children.push_back({ false, reinterpret_cast<MemoryNode*>((*vectorIt)->memoryNode) });

                            vectorIt++;
                            continue;
                        }

                        (*vectorIt)->mMarkIndex = mCurrentBuildMemoryTreeIdx;

                        std::byte* vectorBeginMemory = reinterpret_cast<std::byte*>((*vectorIt)->GetDataPtr());
                        std::byte* vectorEndMemory = vectorBeginMemory + (*vectorIt)->GetDataSize();

                        std::vector<BaseRef*> childVecRefs;

                        MemoryNode* vectorNode = new MemoryNode();
                        vectorNode->name = (String)(*vectorIt)->GetTypeInfo().name() + " [" + (String)(*vectorIt)->GetElementsCount() + "]";
                        vectorNode->memory = reinterpret_cast<std::byte*>(*vectorIt);
                        vectorNode->size = (*vectorIt)->GetDataSize();
                        vectorNode->parent = childNode;
                        childNode->children.push_back({ true, vectorNode });
                        (*vectorIt)->memoryNode = vectorNode;

                        auto vecRefIt = std::lower_bound(refs.begin(), refs.end(), reinterpret_cast<BaseRef*>(vectorBeginMemory));
                        while (vecRefIt != refs.end() && reinterpret_cast<std::byte*>(*vecRefIt) < vectorEndMemory)
                        {
                            childVecRefs.push_back(*vecRefIt);
                            vecRefIt++;
                        }

                        nextNodes.push_back({ vectorNode, childVecRefs });

                        vectorIt++;
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
