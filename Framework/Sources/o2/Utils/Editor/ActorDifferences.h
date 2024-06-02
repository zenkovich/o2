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
        Ref<Actor>                       actor;
        Vector<Ref<SceneEditableObject>> allChildren;

        Map<const SceneEditableObject*, SceneEditableObject*> allChildrenByLinks;

        ApplyActorInfo();
        ApplyActorInfo(const Ref<Actor>& actor);

        bool operator==(const ApplyActorInfo& other) const { return actor == other.actor; }

        void CollectChildren(const Ref<SceneEditableObject>& object);
    };

    // ------------------------------------------
    // ActorDifferences between instance and prototype
    // ------------------------------------------
    struct ActorDifferences
    {
        struct IDifference: public RefCounterable
        {
            virtual Ref<IDifference> Clone() const = 0;
            virtual void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo, 
                               const Vector<ApplyActorInfo>& applyInfos) const = 0;
        };

        struct RemovedChild: public IDifference
        {
            Ref<SceneEditableObject> prototypeLink;

            Ref<IDifference> Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct NewChild: public IDifference
        {
            Ref<SceneEditableObject> parentPrototypeLink;
            Ref<SceneEditableObject> newChild;

            Ref<IDifference> Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct MovedChild: public IDifference
        {
            Ref<SceneEditableObject> prototypeLink;
            Ref<SceneEditableObject> newParentPrototypeLink;

            Ref<IDifference> Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct RemovedComponent: public IDifference
        {
            Ref<Actor>     ownerPrototypeLink;
            Ref<Component> prototypeLink;

            Ref<IDifference> Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct NewComponent: public IDifference
        {
            Ref<Actor>     ownerPrototypeLink;
            Ref<Component> newComponent;

            Ref<IDifference> Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct ChangedObjectField: public IDifference
        {
            Ref<SceneEditableObject> prototypeLink;
            String                   path;

            Ref<IDifference> Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

        struct ChangedComponentField: public IDifference
        {
            Ref<SceneEditableObject> ownerPrototypeLink;
            Ref<Component>           prototypeLink;
            String                   path;

            Ref<IDifference> Clone() const override;
            void Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
                       const Vector<ApplyActorInfo>& applyInfos) const override;
        };

    public:
        Vector<Ref<IDifference>> removedChildren;
        Vector<Ref<IDifference>> newChildren;
        Vector<Ref<IDifference>> movedChildren;
        Vector<Ref<IDifference>> removedComponents;
        Vector<Ref<IDifference>> newComponents;
        Vector<Ref<IDifference>> changedActorFields;
        Vector<Ref<IDifference>> changedComponentFields;

    public:
        ActorDifferences();
        ActorDifferences(const ActorDifferences& other);
        ~ActorDifferences();

        // Returns differences between
        static ActorDifferences GetDifference(const Ref<Actor>& changedActor, const Ref<Actor>& prototype);

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
