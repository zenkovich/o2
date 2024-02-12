#pragma once

#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/StringDef.h"
#include "o2/Utils/Reflection/FieldInfo.h"

namespace o2
{
#if IS_EDITOR
    class Actor;
    class SceneEditableObject;
    class Component;

    // -----------------------------
    // Actor prototype applying info
    // -----------------------------
    struct ApplyActorInfo
    {
        Actor*                       actor;
        Vector<Ref<SceneEditableObject>> allChildren;

        Map<const SceneEditableObject*, SceneEditableObject*> allChildrenByLinks;

        ApplyActorInfo();
        ApplyActorInfo(Actor* actor);

        bool operator==(const ApplyActorInfo& other) const { return actor == other.actor; }

        void CollectChildren(const Ref<SceneEditableObject>& object);
    };

    // ------------------------------------------
    // ActorDifferences between instance and prototype
    // ------------------------------------------
    struct ActorDifferences
    {
        struct IDifference
        {
            virtual IDifference* Clone() const = 0;
            virtual void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo, 
                               const Vector<ApplyActorInfo>& applyInfos) const = 0;
        };

        struct RemovedChild: public IDifference
        {
            SceneEditableObject* prototypeLink;

            IDifference* Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct NewChild: public IDifference
        {
            SceneEditableObject* parentPrototypeLink;
            SceneEditableObject* newChild;

            IDifference* Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct MovedChild: public IDifference
        {
            SceneEditableObject* prototypeLink;
            SceneEditableObject* newParentPrototypeLink;

            IDifference* Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct RemovedComponent: public IDifference
        {
            Actor*     ownerPrototypeLink;
            Component* prototypeLink;

            IDifference* Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct NewComponent: public IDifference
        {
            Actor*     ownerPrototypeLink;
            Component* newComponent;

            IDifference* Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct ChangedObjectField: public IDifference
        {
            SceneEditableObject* prototypeLink;
            String               path;

            IDifference* Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct ChangedComponentField: public IDifference
        {
            SceneEditableObject* ownerPrototypeLink;
            Component*           prototypeLink;
            String               path;

            IDifference* Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

    public:
        Vector<IDifference*> removedChildren;
        Vector<IDifference*> newChildren;
        Vector<IDifference*> movedChildren;
        Vector<IDifference*> removedComponents;
        Vector<IDifference*> newComponents;
        Vector<IDifference*> changedActorFields;
        Vector<IDifference*> changedComponentFields;

    public:
        ActorDifferences();
        ActorDifferences(const ActorDifferences& other);
        ~ActorDifferences();

        // Returns differences between
        static ActorDifferences GetDifference(Actor* changedActor, Actor* prototype);

        // Collects object differences
        static void GetObjectDifferences(const Function<void(const String& path)>& createDiffFunc,
                                         Vector<const FieldInfo*>& stack,
                                         const Type& objectType, void* changedObject, void* protoObject);

        // Checks difference in field
        static void GetFieldDifference(const Function<void(const String& path)>& createDiffFunc,
                                       Vector<const FieldInfo*>& stack,
                                       const Type& fieldType, void* changedFieldObject, void* protoFieldObject);

        // Returns path to field by fields stack
        static String GetFieldPath(const Vector<const FieldInfo*>& stack);
    };
#endif
}
