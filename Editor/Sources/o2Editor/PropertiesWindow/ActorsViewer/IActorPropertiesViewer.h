#pragma once

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Reflection/Reflection.h"
#include "o2/Utils/Reflection/Type.h"
#include "o2/Utils/Types/Containers/Vector.h"

using namespace o2;

namespace o2
{
    class Actor;
    class Component;
    class Widget;
}

namespace Editor
{
    class SpoilerWithHead;

    class IActorPropertiesViewer : public IObject
    {
    public:
        IActorPropertiesViewer();

        virtual ~IActorPropertiesViewer();

        virtual void SetTargetActors(const Vector<Ref<const Actor>>& actors) {}

        virtual const Type* GetActorType() const { return nullptr; }

        virtual const Ref<Widget>& GetWidget() const;

        void Expand();

        void Collapse();

        virtual void Refresh();

        virtual bool IsEmpty() const;

        void SetEnabled(bool enabled);

        bool IsEnabled() const;

        IOBJECT(IActorPropertiesViewer);

    protected:
        Ref<SpoilerWithHead> mSpoiler = nullptr;

        bool mEnabled = false;

    protected:
        virtual void OnEnabled() {}

        virtual void OnDisabled() {}

        friend class ActorViewer;
    };
}

CLASS_BASES_META(Editor::IActorPropertiesViewer)
{
    BASE_CLASS(o2::IObject);
}
END_META;

CLASS_FIELDS_META(Editor::IActorPropertiesViewer)
{
    FIELD().PROTECTED().DEFAULT_VALUE(Ref<SpoilerWithHead>()).NAME(mSpoiler),
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mEnabled)
}
END_META;

CLASS_METHODS_META(Editor::IActorPropertiesViewer)
{
    FUNCTION().PUBLIC().CONSTRUCTOR(),
    FUNCTION().PUBLIC().SIGNATURE(void, SetTargetActors, const Vector<Ref<const Actor>>&),
    FUNCTION().PUBLIC().SIGNATURE(const Type*, GetActorType),
    FUNCTION().PUBLIC().SIGNATURE(const Ref<Widget>&, GetWidget),
    FUNCTION().PUBLIC().SIGNATURE(void, Expand),
    FUNCTION().PUBLIC().SIGNATURE(void, Collapse),
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh),
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEmpty),
    FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool),
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabled),
    FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled),
    FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled)
}
END_META;