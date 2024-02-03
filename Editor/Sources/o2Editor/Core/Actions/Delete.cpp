#include "o2Editor/stdafx.h"
#include "Delete.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/SceneHierarchyTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

namespace Editor
{

    DeleteAction::DeleteAction()
    {}

    DeleteAction::DeleteAction(const Vector<Ref<SceneEditableObject>>& objects)
    {
        for (const auto& object : objects)
        {
            ObjectInfo info;
            info.objectData.Set(object);
            info.idx = o2Scene.GetObjectHierarchyIdx(object.Get());

            if (auto parent = object->GetEditableParent())
            {
                info.parentId = parent->GetID();

                auto parentChilds = parent->GetEditableChildren();
                info.prevObjectId = 0;

                for (auto child : parentChilds)
                {
                    if (child == object.Get())
                        break;

                    info.prevObjectId = child->GetID();
                }
            }
            else
            {
                info.parentId = 0;

                auto rootObjects = o2Scene.GetRootEditableObjects();
                info.prevObjectId = 0;

                for (auto child : rootObjects)
                {
                    if (child == object.Get())
                        break;

                    info.prevObjectId = child->GetID();
                }
            }

            objectsInfos.Add(info);
        }

        objectsInfos.Sort([](const auto& a, const auto& b) { return a.idx < b.idx; });
    }

    String DeleteAction::GetName() const
    {
        return "Actors deletion";
    }

    void DeleteAction::Redo()
    {
        for (const auto& info : objectsInfos)
        {
            auto object = o2Scene.GetEditableObjectByID((SceneUID)info.objectData["Value"]["Id"]);
            if (object)
                delete object;
        }

        o2EditorSceneScreen.ClearSelectionWithoutAction();
        o2EditorTree.UpdateTreeView();
    }

    void DeleteAction::Undo()
    {
        SceneEditableObject* lastRestored = nullptr;
        for (const auto& info : objectsInfos)
        {
            SceneEditableObject* parent = o2Scene.GetEditableObjectByID(info.parentId);
            if (parent)
            {
                SceneUID prevId = info.prevObjectId;
                int idx = parent->GetEditableChildren().IndexOf([=](SceneEditableObject* x) { return x->GetID() == prevId; }) + 1;

                SceneEditableObject* newObject;
                info.objectData.Get(newObject);
                parent->AddEditableChild(newObject, idx);

                o2EditorSceneScreen.SelectObjectWithoutAction(newObject);
                lastRestored = newObject;
            }
            else
            {
                int idx = o2Scene.GetRootActors().IndexOf([&](Actor* x) { return x->GetID() == info.prevObjectId; }) + 1;

                SceneEditableObject* newObject;
                info.objectData.Get(newObject);
                newObject->SetIndexInSiblings(idx);

                o2EditorSceneScreen.SelectObjectWithoutAction(newObject);
                lastRestored = newObject;
            }
        }

        o2EditorTree.HighlightObjectTreeNode(lastRestored);
        o2EditorTree.UpdateTreeView();
    }

    bool DeleteAction::ObjectInfo::operator==(const ObjectInfo& other) const
    {
        return objectData == other.objectData && parentId == other.parentId && prevObjectId == other.prevObjectId;
    }
}
// --- META ---

DECLARE_CLASS(Editor::DeleteAction, Editor__DeleteAction);

DECLARE_CLASS(Editor::DeleteAction::ObjectInfo, Editor__DeleteAction__ObjectInfo);
// --- END META ---