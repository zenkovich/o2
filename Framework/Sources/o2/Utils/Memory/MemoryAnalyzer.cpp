#include "o2/stdafx.h"
#include "MemoryAnalyzer.h"

#include "xutility"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
    int MemoryAnalyzer::mCurrentBuildMemoryTreeIdx = 0;

    MemoryAnalyzeObject::MemoryAnalyzeObject()
    {
        MemoryAnalyzer::OnObjectCreated(this);
    }

    MemoryAnalyzeObject::~MemoryAnalyzeObject()
    {
        MemoryAnalyzer::OnObjectDestroyed(this);
    }

    void MemoryAnalyzer::MemoryNode::SummarizeSize()
    {
        if (summarySize != 0)
            return;

        summarySize = size;

        for (auto& child : children)
        {
            child->SummarizeSize();
            
            if (child->mainParent == this)
                summarySize += child->summarySize;
        }
    }

    MemoryAnalyzer::MemoryNode::~MemoryNode()
    {
        for (auto& child : children)
        {
            if (child->mainParent == this)
                delete child;
        }
    }

    std::vector<MemoryAnalyzeObject*>& MemoryAnalyzer::GetAnalyzeObjects()
    {
        static std::vector<MemoryAnalyzeObject*> data;
        return data;
    }

    std::vector<size_t>& MemoryAnalyzer::GetFreeAnalyzeObjects()
    {
        static std::vector<size_t> data;
        return data;
    }

    void MemoryAnalyzer::AllocateAnalyzeObjects()
    {
        auto& objects = GetAnalyzeObjects();
        auto& freeIndexes = GetFreeAnalyzeObjects();

        auto size = objects.size();
        auto newSize = std::max((size_t)1024, size + size/2);

        objects.resize(newSize, nullptr);
        for (size_t i = 0; i < newSize - size; i++)
            freeIndexes.push_back(newSize - i - 1);
    }

    void MemoryAnalyzer::OnObjectCreated(MemoryAnalyzeObject* obj)
    {
        auto& freeIndexes = GetFreeAnalyzeObjects();
        if (freeIndexes.empty())
            AllocateAnalyzeObjects();

        auto& objects = GetAnalyzeObjects();

        auto nextFreeIdx = freeIndexes.back();

        objects[nextFreeIdx] = obj;
        freeIndexes.pop_back();

        reinterpret_cast<MemoryAnalyzeObject*>(obj)->manageIndex = nextFreeIdx;
    }

    void MemoryAnalyzer::OnObjectDestroyed(MemoryAnalyzeObject* obj)
    {
        auto idx = reinterpret_cast<MemoryAnalyzeObject*>(obj)->manageIndex;
        GetAnalyzeObjects()[idx] = nullptr;
        GetFreeAnalyzeObjects().push_back(idx);
    }
    
    void ReplaceAllStr(std::string& str, const std::string& oldStr, const std::string& newStr)
    {
        while (true)
        {
            auto pos = str.find(oldStr);
            if (pos == std::string::npos)
                break;

            str.replace(pos, oldStr.size(), newStr);
        }
    }

    void FixClassName(std::string& className)
    {
        ReplaceAllStr(className, "class ", "");
        ReplaceAllStr(className, "> >", ">>");
    }

    MemoryAnalyzer::MemoryNode* MemoryAnalyzer::BuildMemoryTree(std::vector<MemoryAnalyzeObject*> roots)
    {
        // Increment build index for marking tree
        mCurrentBuildMemoryTreeIdx++;

        auto& objects = GetAnalyzeObjects();

        // Get all object pointers and sort
        std::vector<MemoryAnalyzeObject*> sortedObjects;
        sortedObjects.reserve(objects.size());
        for (auto object : objects)
        {
            if (object)
                sortedObjects.push_back(object);
        }

        std::sort(sortedObjects.begin(), sortedObjects.end(), [](MemoryAnalyzeObject* a, MemoryAnalyzeObject* b) { return a < b; });

        // Create root node
        MemoryNode* root = new MemoryNode();

        // Begin marking and collecting algorithm
        std::vector<std::pair<MemoryNode*, std::vector<MemoryAnalyzeObject*>>> currentNodes = { { root, roots } };
        std::vector<std::pair<MemoryNode*, std::vector<MemoryAnalyzeObject*>>> nextNodes;

        std::map<std::byte*, MemoryNode*> memoryNodes;

        auto& memoryInfos = MemoryManager::Instance().mAllocs;

        // Repeat until there are no more nodes to process
        while (!currentNodes.empty())
        {
            // Iterate over current nodes
            for (auto& it : currentNodes)
            {
                auto& node = it.first;

                // Iterate children objects of this node
                for (auto& object : it.second)
                {
                    // Get object's memory begin address
                    std::byte* objectMemoryBegin = object->GetMemory();
                    if (!objectMemoryBegin)
                        continue;

                    // Try to find existing node for this memory address. Stop looking deeper if found
                    auto nodesMapFndIt = memoryNodes.find(objectMemoryBegin);
                    if (nodesMapFndIt != memoryNodes.end())
                    {
                        node->children.push_back(nodesMapFndIt->second);
                        nodesMapFndIt->second->parents.push_back(node);

                        continue;
                    }

                    // Get object's memory size
                    size_t objectSize = object->GetMemorySize();
                    if (objectSize == 0)
                    {
                        // If it's zero, try to find it in memory info
                        auto memoryInfosFndIt = memoryInfos.find(objectMemoryBegin);
                        if (memoryInfosFndIt == memoryInfos.end())
                        {
                            o2Debug.LogError("Failed to find memory info for memory address: %p", objectMemoryBegin);
                            continue;
                        }

                        auto& memoryInfo = memoryInfosFndIt->second;
                        objectSize = memoryInfo.size;
                        memoryInfo.markIndex = mCurrentBuildMemoryTreeIdx;
                    }

                    // Create new node for this object
                    MemoryNode* childNode = new MemoryNode();

                    childNode->name = std::string(object->GetTypeInfo().name());
                    FixClassName(childNode->name);

                    childNode->memory = objectMemoryBegin;
                    childNode->object = object;
                    childNode->iobject = object->GetIObject();
                    childNode->size = objectSize;
                    childNode->mainParent = node;

                    node->children.push_back(childNode);
                    childNode->parents.push_back(node);

                    // Store this node for future reference
                    memoryNodes[objectMemoryBegin] = childNode;

                    // Get object's memory end address
                    std::byte* objectMemoryEnd = objectMemoryBegin + objectSize;

                    // Find all child objects
                    std::vector<MemoryAnalyzeObject*> childRefs;

                    auto refIt = std::lower_bound(sortedObjects.begin(), sortedObjects.end(), reinterpret_cast<MemoryAnalyzeObject*>(objectMemoryBegin));
                    while (refIt != sortedObjects.end() && reinterpret_cast<std::byte*>(*refIt) < objectMemoryEnd)
                    {
                        childRefs.push_back(*refIt);
                        refIt++;
                    }

                    nextNodes.push_back({ childNode, childRefs });
                }
            }

            // Update current nodes and clear next nodes
            currentNodes = nextNodes;
            nextNodes.clear();
        }

        root->SummarizeSize();

        return root;
    }

}
