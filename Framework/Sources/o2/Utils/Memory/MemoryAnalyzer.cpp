#include "o2/stdafx.h"
#include "MemoryAnalyzer.h"

#include "xutility"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
    int MemoryAnalyzer::mCurrentBuildMemoryTreeIdx = 0;
    bool MemoryAnalyzer::enabledObjectsTracking = true;

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

    void MemoryAnalyzer::MemoryNode::SummarizeLeakedSize()
    {
        if (leakedSize != 0 || summarySize != 0)
            return;

        leakedSize = size;

        for (auto& child : children)
        {
            child->SummarizeLeakedSize();

            if (child->mainParent == this)
                leakedSize += child->leakedSize;
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
        if (!enabledObjectsTracking)
            return;

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
        if (!enabledObjectsTracking)
            return;

        auto idx = reinterpret_cast<MemoryAnalyzeObject*>(obj)->manageIndex;
        GetAnalyzeObjects()[idx] = nullptr;
        GetFreeAnalyzeObjects().push_back(idx);
    }

    MemoryAnalyzer::MemoryNode* MemoryAnalyzer::BuildMemoryTree(const std::vector<MemoryAnalyzeObject*>& roots)
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

        std::map<std::byte*, MemoryNode*> memoryNodes;
        std::vector<std::pair<MemoryNode*, std::vector<MemoryAnalyzeObject*>>> currentNodes;
        std::vector<std::pair<MemoryNode*, std::vector<MemoryAnalyzeObject*>>> nextNodes;
        std::vector<MemoryAnalyzeObject*> childRefs;

        // Create root node
        MemoryNode* allMemoryNode = new MemoryNode();
        allMemoryNode->name = "All memory";

        // Build tree from roots
        BuildSubTree(allMemoryNode, roots, memoryNodes, currentNodes, nextNodes, childRefs, sortedObjects);

        // Remove duplicated nodes
        for (auto it = allMemoryNode->children.begin(); it != allMemoryNode->children.end();)
        {
            auto node = *it;

            if (node->parents.size() > 1)
            {
                node->mainParent = node->parents[1];
                node->parents.erase(node->parents.begin());

                it = allMemoryNode->children.erase(it);
            }
            else
                it++;
        }

        allMemoryNode->SummarizeSize();

        // Collect possible leaks nodes
        auto possibleLeaksNode = new MemoryNode();
        possibleLeaksNode->name = "Possible leaks";
        possibleLeaksNode->mainParent = allMemoryNode;
        possibleLeaksNode->parents.push_back(allMemoryNode);
        allMemoryNode->children.push_back(possibleLeaksNode);

        for (int i = 0; i < sortedObjects.size(); i++)
        {
            auto& object = sortedObjects[i];
            if (object->markIndex == mCurrentBuildMemoryTreeIdx)
                continue;

            o2Debug.Log("Processing object: %p (%i/%i)", object, i, sortedObjects.size());

            BuildSubTree(possibleLeaksNode, { object }, memoryNodes, currentNodes, nextNodes, childRefs, sortedObjects);
        }

        possibleLeaksNode->SummarizeLeakedSize();

        // Remove ampty and duplicated leak nodes
        for (auto it = possibleLeaksNode->children.begin(); it != possibleLeaksNode->children.end();)
        {
            auto node = *it;

            if (node->leakedSize == 0 || node->children.empty())
            {
                if (node->mainParent == possibleLeaksNode)
                    delete *it;

                it = possibleLeaksNode->children.erase(it);
            }
            else if (node->parents.size() > 1)
            {
                node->mainParent = node->parents[1];
                node->parents.erase(node->parents.begin());

                it = possibleLeaksNode->children.erase(it);
            }
            else
                it++;
        }

        return allMemoryNode;
    }

    void MemoryAnalyzer::BuildSubTree(MemoryNode* root, const std::vector<MemoryAnalyzeObject*>& roots,
                                      std::map<std::byte*, MemoryNode*>& memoryNodes,
                                      std::vector<std::pair<MemoryNode*, std::vector<MemoryAnalyzeObject*>>>& currentNodes,
                                      std::vector<std::pair<MemoryNode*, std::vector<MemoryAnalyzeObject*>>>& nextNodes,
                                      std::vector<MemoryAnalyzeObject*>& childRefs,
                                      const std::vector<MemoryAnalyzeObject*>& sortedObjects)
    {
        // Begin marking and collecting algorithm
        currentNodes = { { root, roots } };
        nextNodes.clear();
        childRefs.clear();

        auto& memoryInfos = MemoryManager::Instance().mAllocs;

        // Repeat until there are no more nodes to process
        while (!currentNodes.empty())
        {
            // Iterate over current nodes
            for (auto& it : currentNodes)
            {
                auto& node = it.first;

                auto nodeIObject = it.first->iobject;
                void* nodeObject = nullptr;
                const ObjectType* nodeIObjectType = nullptr;

                if (nodeIObject)
                {
                    nodeIObjectType = dynamic_cast<const ObjectType*>(&nodeIObject->GetType());
                    nodeObject = nodeIObjectType->DynamicCastFromIObject(nodeIObject);
                }

                // Iterate children objects of this node
                for (auto& object : it.second)
                {
                    object->markIndex = mCurrentBuildMemoryTreeIdx;

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

                    childNode->type = std::string(object->GetTypeInfo().name());

                    childNode->memory = objectMemoryBegin;
                    childNode->object = object;
                    childNode->iobject = object->GetIObject();
                    childNode->size = objectSize;
                    childNode->mainParent = node;

                    node->children.push_back(childNode);
                    childNode->parents.push_back(node);

                    // Try to determine node's name
                    if (nodeIObjectType)
                        childNode->name = TryFindFieldName(nodeIObjectType, nodeObject, object);

                    // Store this node for future reference
                    memoryNodes[objectMemoryBegin] = childNode;

                    // Get object's memory end address
                    std::byte* objectMemoryEnd = objectMemoryBegin + objectSize;

                    // Find all child objects
                    childRefs.clear();

                    // By memory
                    SearchChildrenObjects(sortedObjects, objectMemoryBegin, objectMemoryEnd, object, childRefs);

                    // By children
                    object->IterateChildren([&](MemoryAnalyzeObject* child)
                                            {
                                                childRefs.push_back(child);
                                            });

                    // By allocations
                    object->IterateAllocations([&](std::byte* data, size_t size)
                                               {
                                                   SearchChildrenObjects(sortedObjects, data, data + size, object, childRefs);
                                               });

                    nextNodes.push_back({ childNode, childRefs });
                }
            }

            // Update current nodes and clear next nodes
            currentNodes = nextNodes;
            nextNodes.clear();
        }
    }

    void MemoryAnalyzer::SearchChildrenObjects(const std::vector<MemoryAnalyzeObject*>& sortedObjects,
                                               std::byte* objectMemoryBegin, std::byte* objectMemoryEnd,
                                               MemoryAnalyzeObject*& object,
                                               std::vector<MemoryAnalyzeObject*>& childRefs)
    {
        auto refIt = std::lower_bound(sortedObjects.begin(), sortedObjects.end(), reinterpret_cast<MemoryAnalyzeObject*>(objectMemoryBegin));
        while (refIt != sortedObjects.end() && reinterpret_cast<std::byte*>(*refIt) < objectMemoryEnd)
        {
            if (*refIt != object)
                childRefs.push_back(*refIt);

            refIt++;
        }
    }

    std::string MemoryAnalyzer::TryFindFieldName(const Type* nodeIObjectType, void* nodeObject,
                                                 MemoryAnalyzeObject*& object)
    {
        for (auto& fieldInfo : nodeIObjectType->GetFields())
        {
            if (fieldInfo.GetValuePtr(nodeObject) == object)
            {
                return (std::string)fieldInfo.GetName();
            }
        }

        for (auto& baseClass : nodeIObjectType->GetBaseTypes())
        {
            auto baseObject = baseClass.dynamicCastUpFunc(nodeObject);
            auto name = TryFindFieldName(baseClass.type, baseObject, object);
            if (!name.empty())
                return name;
        }

        return "";
    }
}
