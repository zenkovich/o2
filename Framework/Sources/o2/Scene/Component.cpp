#include "o2/stdafx.h"
#include "Component.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/ActorRefResolver.h"
#include "o2/Scene/Scene.h"

namespace o2
{
    Component::Component() :
        mId(Math::Random())
    {
        ActorRefResolver::ComponentCreated(this);
    }

    Component::Component(const Component& other) :
        mEnabled(other.mEnabled), mEnabledInHierarchy(false), mId(Math::Random()),
        actor(this), enabled(this), enabledInHierarchy(this)
    {
        ActorRefResolver::ComponentCreated(this);
    }

    Component::~Component()
    {
        if (mOwner)
            mOwner->RemoveComponent(this, false);

        for (auto ref : mReferences)
        {
            ref->mComponent = nullptr;
            ref->mWasDeleted = true;
        }
    }

    Component& Component::operator=(const Component& other)
    {
        mEnabled = other.mEnabled;
        UpdateEnabledInHierarchy();

#if IS_EDITOR
        if (mOwner)
            mOwner->OnChanged();
#endif

        return *this;
    }

    SceneUID Component::GetID() const
    {
        return mId;
    }

    void Component::GenerateNewID()
    {
        mId = Math::Random();
    }

    void Component::SetEnabled(bool active)
    {
        if (mEnabled == active)
            return;

        mEnabled = active;
        UpdateEnabledInHierarchy();

#if IS_EDITOR
        if (mOwner)
            mOwner->OnChanged();
#endif
    }

    void Component::Enable()
    {
        SetEnabled(true);
    }

    void Component::Disable()
    {
        SetEnabled(false);
    }

    bool Component::IsEnabled() const
    {
        return mEnabled;
    }

    bool Component::IsEnabledInHierarchy() const
    {
        return mEnabledInHierarchy;
    }

    Component* Component::GetPrototypeLink() const
    {
        return mPrototypeLink;
    }

    bool Component::IsLinkedToComponent(Component* component) const
    {
        if (mPrototypeLink)
        {
            auto t = mPrototypeLink;
            while (t)
            {
                if (t == component)
                    return true;

                t = t->mPrototypeLink;
            }
        }

        return false;
    }

    Actor* Component::GetOwnerActor() const
    {
        return mOwner;
    }

    String Component::GetName()
    {
        return String();
    }

    String Component::GetCategory()
    {
        return "";
    }

    String Component::GetIcon()
    {
        return "ui/UI4_component_icon.png";
    }

    bool Component::IsAvailableFromCreateMenu()
    {
        return true;
    }

    void Component::UpdateEnabledInHierarchy()
    {
        bool lastEnabledInHierarchy = mEnabledInHierarchy;

        if (mOwner)
            mEnabledInHierarchy = mEnabled && mOwner->mResEnabledInHierarchy;
        else
            mEnabledInHierarchy = mEnabled;

        if (lastEnabledInHierarchy != mEnabledInHierarchy)
        {
            if (mEnabledInHierarchy)
                OnEnabled();
            else
                OnDisabled();

#if IS_EDITOR
            if (mOwner)
                mOwner->OnChanged();
#endif
        }
    }

    void Component::OnSerialize(DataValue& node) const
    {
        node.AddMember("mId") = mId;
    }

    void Component::OnDeserialized(const DataValue& node)
    {
        auto prevId = mId;
        mId = node.GetMember("mId");
        ActorRefResolver::OnComponentIdChanged(this, prevId);
    }

    void Component::OnSerializeDelta(DataValue& node, const IObject& origin) const
    {
        OnSerialize(node);
    }

    void Component::OnDeserializedDelta(const DataValue& node, const IObject& origin)
    {
        OnDeserialized(node);
    }

    void Component::AddToScene()
    {
        o2Scene.OnComponentAdded(this);
        OnAddToScene();
    }

    void Component::RemoveFromScene()
    {
        o2Scene.OnComponentRemoved(this);
        OnRemoveFromScene();
    }

    void Component::SetOwnerActor(Actor* actor)
    {
        if (mOwner == actor)
            return;

        if (mOwner)
            mOwner->RemoveComponent(this, false);

        mOwner = actor;

        if (mOwner)
            OnTransformUpdated();

        OnParentChanged(nullptr);
    }
}
// --- META ---

DECLARE_CLASS(o2::Component, o2__Component);
// --- END META ---
