#include "o2/stdafx.h"
#include "Actor.h"

#include "o2/Scene/ActorRefResolver.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Components/ScriptableComponent.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Scene/Scene.h"
#include "o2/Utils/Debug/Debug.h"

namespace o2
{
    ActorCreateMode Actor::mDefaultCreationMode = ActorCreateMode::InScene;

    Actor::Actor(ActorTransform* transform, bool onScene /*= true*/, const String& name /*= "unnamed"*/, bool enabled /*= true*/,
                 SceneUID id /*= Math::Random()*/, UID assetId /*= UID(0)*/) :
        transform(transform), mName(name), mEnabled(enabled), mResEnabled(enabled), mResEnabledInHierarchy(false),
        mId(id), mAssetId(assetId), mState(State::Initializing), mIsOnScene(onScene)
    {
        tags.onTagAdded = [&](Tag* tag) { tag->mActors.Add(this); };
        tags.onTagRemoved = [&](Tag* tag) { tag->mActors.Remove(this); };

        transform->SetOwner(this);

        Scene::OnActorCreated(this);
    }

    Actor::Actor(ActorTransform* transform, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
        Actor(transform, IsModeOnScene(mode))
    {
        ActorRefResolver::ActorCreated(this);
    }

    Actor::Actor(ActorTransform* transform, const Actor& other, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
        Actor(transform, IsModeOnScene(mode), other.mName, other.mEnabled, Math::Random(), other.mAssetId)
    {
        ActorRefResolver::LockResolving();

        ISceneDrawable::operator=(other);
        mSceneLayer = other.mSceneLayer;

        if (other.mIsAsset)
        {
            other.mCopyVisitor = mnew InstantiatePrototypeCloneVisitor();
            SetPrototype(ActorAssetRef(other.GetAssetID()));
        }

        transform->CopyFrom(*other.transform);
        mAssetId = other.mAssetId;
        mPrototypeLink.CopyWithoutRemap(other.mPrototypeLink);

        if (!other.mCopyVisitor)
            other.mCopyVisitor = mnew SourceToTargetMapCloneVisitor();

        other.mCopyVisitor->OnCopyActor(&other, this);
        other.mCopyVisitor->depth++;

        if (!mPrototype && other.mPrototype)
        {
            mPrototype = other.mPrototype;

#if IS_EDITOR
            Scene::LinkActorToPrototypesHierarchy(this, mPrototype);
#endif
        }

        for (auto child : other.mChildren)
        {
            child->mCopyVisitor = other.mCopyVisitor;

            AddChild(child->CloneAs<Actor>());

            child->mCopyVisitor = nullptr;
        }

        for (auto component : other.mComponents)
        {
            Component* newComponent = AddComponent(component->CloneAs<Component>());

            if (other.mCopyVisitor)
                other.mCopyVisitor->OnCopyComponent(component, newComponent);
        }

        ActorRefResolver::ActorCreated(this);
        ActorRefResolver::UnlockResolving();

        other.CheckCopyVisitorFinalization();
    }

    Actor::Actor(ActorTransform* transform, const ActorAssetRef& prototype, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
        Actor(transform, *prototype->GetActor(), mode)
    {}

    Actor::Actor(ActorTransform* transform, Vector<Component*> components, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
        Actor(transform, mode)
    {
        for (auto comp : components)
            AddComponent(comp);
    }

    Actor::Actor(ActorCreateMode mode /*= CreateMode::Default*/) :
        Actor(mnew ActorTransform(), mode)
    {}

    Actor::Actor(Vector<Component*> components, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
        Actor(mnew ActorTransform(), components, mode)
    {}

    Actor::Actor(const ActorAssetRef& prototype, ActorCreateMode mode /*= CreateMode::Default*/) :
        Actor(mnew ActorTransform(*prototype->GetActor()->transform), prototype, mode)
    {}

    Actor::Actor(const Actor& other, ActorCreateMode mode) :
        Actor(mnew ActorTransform(*other.transform), other, mode)
    {}

    Actor::Actor(const Actor& other) :
        Actor(other, ActorCreateMode::Default)
    {}

    Actor::~Actor()
    {
        mState = State::Destroyed;

        SetPrototype(nullptr);

        for (auto ref : mReferences)
        {
            ref->mActor = nullptr;
            ref->mWasDeleted = true;
        }

        if (mParent)
            mParent->RemoveChild(this, false);

        RemoveAllChildren();
        RemoveAllComponents();

        Scene::OnActorDestroy(this);

        delete transform;
    }

    Actor& Actor::operator=(const Actor& other)
    {
        RemoveAllChildren();
        RemoveAllComponents();

        ActorRefResolver::LockResolving();

        if (other.mCopyVisitor)
            other.mCopyVisitor->OnCopyActor(&other, this);

        if (other.mIsAsset)
            SetPrototype(ActorAssetRef(other.GetAssetID()));

        ISceneDrawable::operator=(other);

        mName = other.mName;
        mEnabled = other.mEnabled;
        mResEnabled = mEnabled;
        mResEnabledInHierarchy = mEnabled;
        mSceneLayer = other.mSceneLayer;
        transform->CopyFrom(*other.transform);
        mAssetId = other.mAssetId;
        mPrototypeLink.CopyWithoutRemap(other.mPrototypeLink);

        if (!other.mCopyVisitor)
            other.mCopyVisitor = mnew SourceToTargetMapCloneVisitor();

        other.mCopyVisitor->OnCopyActor(&other, this);
        other.mCopyVisitor->depth++;

        if (!mPrototype && other.mPrototype)
        {
            mPrototype = other.mPrototype;

#if IS_EDITOR
            Scene::LinkActorToPrototypesHierarchy(this, mPrototype);
#endif
        }

        for (auto child : other.mChildren)
        {
            child->mCopyVisitor = other.mCopyVisitor;

            AddChild(child->CloneAs<Actor>());

            child->mCopyVisitor = nullptr;
        }

        for (auto component : other.mComponents)
        {
            Component* newComponent = AddComponent(component->CloneAs<Component>());

            if (other.mCopyVisitor)
                other.mCopyVisitor->OnCopyComponent(component, newComponent);

            if (mPrototypeLink)
                newComponent->mPrototypeLink = component->mPrototypeLink;
        }

        Scene::OnActorCreated(this);

        ActorRefResolver::ActorCreated(this);
        ActorRefResolver::UnlockResolving();

        other.CheckCopyVisitorFinalization();
        UpdateResEnabledInHierarchy();

#if IS_EDITOR
        OnChanged();
#endif

        return *this;
    }

    void Actor::CheckCopyVisitorFinalization() const
    {
        if (mCopyVisitor)
        {
            mCopyVisitor->depth--;

            if (mCopyVisitor->depth == 0)
            {
                mCopyVisitor->Finalize();
                delete mCopyVisitor;
                mCopyVisitor = nullptr;
            }
        }
    }

    void Actor::Destroy()
    {
        if (Scene::IsSingletonInitialzed())
            o2Scene.DestroyActor(this);
        else
            o2Debug.LogError("Cant destroy actor in scene, because it's not exists. Delete actor manually");
    }

    void Actor::Draw()
    {
        for (auto component : mDrawComponents)
            component->Draw();

        ISceneDrawable::Draw();
    }

    void Actor::Update(float dt)
    {
        if (transform->IsDirty())
        {
            for (auto child : mChildren)
                child->transform->SetDirty(true);

            UpdateSelfTransform();
        }

        OnUpdate(dt);

        for (auto comp : mComponents)
            comp->OnUpdate(dt);
    }

    void Actor::FixedUpdate(float dt)
    {
        OnFixedUpdate(dt);

        for (auto comp : mComponents)
            comp->OnFixedUpdate(dt);
    }

    void Actor::UpdateChildren(float dt)
    {
        for (auto child : mChildren)
            child->Update(dt);

        for (auto child : mChildren)
            child->UpdateChildren(dt);
    }

    void Actor::FixedUpdateChildren(float dt)
    {
        for (auto child : mChildren)
            child->FixedUpdate(dt);

        for (auto child : mChildren)
            child->FixedUpdateChildren(dt);
    }

    void Actor::UpdateTransform()
    {
        UpdateSelfTransform();
        UpdateChildrenTransforms();
    }

    void Actor::UpdateSelfTransform()
    {
        transform->Update();
        
        for (auto comp : mComponents)
            comp->OnTransformUpdated();
    }

    void Actor::UpdateChildrenTransforms()
    {
        for (auto child : mChildren)
            child->UpdateSelfTransform();

        for (auto child : mChildren)
            child->UpdateChildrenTransforms();
    }

    void Actor::SetName(const String& name)
    {
        mName = name;

#if IS_EDITOR
        OnNameChanged();
#endif
    }

    const String& Actor::GetName() const
    {
        return mName;
    }

    SceneUID Actor::GetID() const
    {
        return mId;
    }

    void Actor::SetID(SceneUID id)
    {
        auto prevId = mId;
        mId = id;

        Scene::OnActorIdChanged(this, prevId);
        ActorRefResolver::OnActorIdChanged(this, prevId);
    }

    UID Actor::GetAssetID() const
    {
        return mAssetId;
    }

    bool Actor::IsAsset() const
    {
        if (mParent)
            return mIsAsset || mParent->IsAsset();

        return mIsAsset;
    }

    void Actor::GenerateNewID(bool withChildren /*= true*/)
    {
        SetID(Math::Random());

        if (withChildren)
        {
            for (auto child : mChildren)
                child->GenerateNewID(withChildren);
        }
    }

    void Actor::AddToScene()
    {
        if (mIsOnScene)
            return;

        mIsOnScene = true;;

        Scene::OnAddActorToScene(this);

        for (auto child : mChildren)
            child->AddToScene();
    }

    void Actor::RemoveFromScene(bool keepEditorObjects /*= false*/)
    {
        if (!mIsOnScene)
            return;

        mIsOnScene = false;

        Scene::OnRemoveActorFromScene(this, keepEditorObjects);

        for (auto child : mChildren)
            child->RemoveFromScene();
    }

    bool Actor::IsOnScene() const
    {
        return mIsOnScene;
    }

    void Actor::SetEnabled(bool enabled)
    {
        if (mEnabled == enabled)
            return;

        mEnabled = enabled;
        UpdateResEnabled();

#if IS_EDITOR
        onEnableChanged(mEnabled);

        if (mIsOnScene)
            o2Scene.onEnableChanged(this);

        OnChanged();
#endif
    }

    void Actor::Enable()
    {
        SetEnabled(true);
    }

    void Actor::Disable()
    {
        SetEnabled(false);
    }

    bool Actor::IsEnabled() const
    {
        return mEnabled;
    }

    bool Actor::IsResEnabled() const
    {
        return mResEnabled;
    }

    bool Actor::IsEnabledInHierarchy() const
    {
        return mResEnabledInHierarchy;
    }

    void Actor::SetIndexInSiblings(int index)
    {
        if (mParent)
        {
            int lastIdx = mParent->mChildren.IndexOf(this);
            mParent->mChildren.Insert(this, index);

            if (index <= lastIdx)
                lastIdx++;

            mParent->mChildren.RemoveAt(lastIdx);

            mParent->OnChildrenChanged();
        }
        else
        {
            if (mIsOnScene)
            {
                int lastIdx = o2Scene.mRootActors.IndexOf(this);
                o2Scene.mRootActors.Insert(this, index);

                if (index <= lastIdx)
                    lastIdx++;

                o2Scene.mRootActors.RemoveAt(lastIdx);
            }
        }
    }

    void Actor::SetParent(Actor* actor, bool worldPositionStays /*= true*/, int idx /*= -1*/)
    {
        if ((actor && actor->mParent == this) || actor == this || actor == mParent)
            return;

        // Store initial transform
        Basis lastParentBasis = transform->GetWorldBasis();
        auto oldParent = mParent;

        // Remove from parent list or root actors
        if (mParent)
            mParent->RemoveChild(this, false, false);
        else if (mIsOnScene && Scene::IsSingletonInitialzed())
            o2Scene.mRootActors.Remove(this);

        // Change parent
        mParent = actor;
        transform->mData->parentInvTransformActualFrame = 0;

        // Restore in new parent children list or root actors
        if (mParent)
        {
            if (idx < 0)
                mParent->mChildren.Add(this);
            else
                mParent->mChildren.Insert(this, idx);

            mParent->OnChildAdded(this);
        }
        else if (mIsOnScene && Scene::IsSingletonInitialzed())
        {
            if (idx < 0)
                o2Scene.mRootActors.Add(this);
            else
                o2Scene.mRootActors.Insert(this, idx);
        }

        // Restore transform if needed
        if (worldPositionStays)
            transform->SetWorldBasis(lastParentBasis);
        else
            transform->SetDirty();

        // Call events only when actor was initialized
        if (mState == State::Default)
        {
            // Update enabled state in hierarchy
            UpdateResEnabledInHierarchy();

            // Checks if actor should be added or removed from scene
            if (mParent && mParent->mIsOnScene != mIsOnScene)
            {
                if (mParent->mIsOnScene && !mIsOnScene)
                    AddToScene();
                else if (!mParent->mIsOnScene && mIsOnScene)
                    RemoveFromScene();
            }
        }
        else if (mState == State::Initializing && mParent && mParent->mState == State::Initializing)
        {
            Scene::OnNewActorParented(this);
        }

        // Call parent update events
        OnParentChanged(oldParent);
    }

    Actor* Actor::GetParent() const
    {
        return mParent;
    }

    Actor* Actor::AddChild(Actor* actor)
    {
        Assert(actor, "Actor is null");

        actor->SetParent(this, false);
        return actor;
    }

    Actor* Actor::AddChild(Actor* actor, int index)
    {
        Assert(actor, "Actor is null");

        actor->SetParent(this, false, index);
        return actor;
    }

    void Actor::AddChildren(const Vector<Actor*>& actors)
    {
        for (auto actor : actors)
            AddChild(actor);
    }

    Actor* Actor::GetChild(const String& path) const
    {
        int delPos = path.Find("/");
        String pathPart = path.SubStr(0, delPos);

        if (pathPart == "..")
        {
            if (mParent)
            {
                if (delPos == -1)
                    return mParent;
                else
                    return mParent->GetChild(path.SubStr(delPos + 1));
            }

            return nullptr;
        }

        for (auto child : mChildren)
        {
            if (child->mName == pathPart)
            {
                if (delPos == -1)
                    return child;
                else
                    return child->GetChild(path.SubStr(delPos + 1));
            }
        }

        return nullptr;
    }

    Actor* Actor::FindChild(const String& name) const
    {
        for (auto child : mChildren)
        {
            if (child->mName == name)
                return child;

            if (auto res = child->FindChild(name))
                return res;
        }

        return nullptr;
    }

    Actor* Actor::FindChild(const Function<bool(const Actor* child)>& pred) const
    {
        for (auto child : mChildren)
        {
            if (pred(child))
                return child;

            if (auto res = child->FindChild(pred))
                return res;
        }

        return nullptr;
    }

    const Vector<Actor*>& Actor::GetChildren() const
    {
        return mChildren;
    }

    void Actor::GetAllChildrenActors(Vector<Actor*>& actors)
    {
        actors.Add(mChildren);

        for (auto child : mChildren)
            child->GetAllChildrenActors(actors);
    }

    void Actor::RemoveChild(Actor* actor, bool release /*= true*/, bool withEvent /*= true*/)
    {
        auto oldParent = actor->mParent;

        actor->mParent = nullptr;
        mChildren.Remove(actor);

        if (withEvent)
            actor->OnParentChanged(oldParent);

        OnChildRemoved(actor);

        if (release)
        {
            actor->OnBeforeDestroy();
            delete actor;
        }
        else if (actor->mState != State::Destroyed)
        {
            actor->transform->SetDirty();
            actor->UpdateResEnabledInHierarchy();
        }
    }

    void Actor::RemoveAllChildren(bool release /*= true*/)
    {
        for (auto child : mChildren)
        {
            if (release)
                child->OnBeforeDestroy();

            child->mParent = nullptr;

            OnChildRemoved(child);
            child->OnParentChanged(this);

            if (release)
                delete child;
        }

        mChildren.Clear();
    }

    Actor* Actor::FindActorById(SceneUID id)
    {
        if (mId == id)
            return this;

        for (auto child : mChildren)
        {
            if (auto res = child->FindActorById(id))
                return res;
        }

        return nullptr;
    }

    Component* Actor::AddComponent(Component* component)
    {
        component->SetOwnerActor(this);
        mComponents.Add(component);

        if (mIsOnScene && mState == State::Default)
            component->AddToScene();

        if (auto drawableComponent = dynamic_cast<DrawableComponent*>(component))
            mDrawComponents.Add(drawableComponent);

        OnComponentAdded(component);

        component->OnTransformUpdated();

#if IS_EDITOR
        OnChanged();
#endif

        return component;
    }

    void Actor::RemoveComponent(Component* component, bool release /*= true*/)
    {
        if (mIsOnScene)
            component->RemoveFromScene();

        if (auto drawableComponent = dynamic_cast<DrawableComponent*>(component))
            mDrawComponents.Remove(drawableComponent);

        OnComponentRemoving(component);

        mComponents.Remove(component);
        component->mOwner = nullptr;

        if (release)
            delete component;

#if IS_EDITOR
        OnChanged();
#endif
    }

    void Actor::RemoveAllComponents()
    {
        auto components = mComponents;
        mComponents.Clear();

        for (auto component : components)
        {
            if (mIsOnScene)
                component->RemoveFromScene();

            OnComponentRemoving(component);

            delete component;
        }

#if IS_EDITOR
        OnChanged();
#endif
    }

    Component* Actor::GetComponent(const String& typeName)
    {
        for (auto comp : mComponents)
            if (comp->GetType().GetName() == typeName)
                return comp;

        return nullptr;
    }

    Component* Actor::GetComponent(const Type* type)
    {
        for (auto comp : mComponents)
            if (comp->GetType().IsBasedOn(*type))
                return comp;

        return nullptr;
    }

    Component* Actor::GetComponent(SceneUID id)
    {
        for (auto comp : mComponents)
            if (comp->GetID() == id)
                return comp;

        return nullptr;
    }

#if IS_SCRIPTING_SUPPORTED
    Component* Actor::GetComponent(const ScriptValue& typeValue)
    {
        auto proto = typeValue.GetPrototype();
        auto protoYpe = proto.GetValueType();
        if (proto.IsObject())
        {
            auto typeProp = proto.GetProperty("type");
            if (typeProp.IsObjectContainer())
            {
                const Type* type = typeProp.GetValue<const Type*>();
                for (auto component : mComponents)
                {
                    if (component->GetType().IsBasedOn(*type))
                        return component;
                }
            }
        }
        else
        {
            for (auto component : mComponents)
            {
                if (auto scriptComponent = dynamic_cast<ScriptableComponent*>(component))
                {
                    if (scriptComponent->GetClass() == typeValue)
                        return component;
                }
            }
        }

        return nullptr;
    }

    ActorTransform* Actor::GetTransform()
    {
        return transform;
    }
#endif

    const Vector<Component*>& Actor::GetComponents() const
    {
        return mComponents;
    }

    void Actor::SetDefaultCreationMode(ActorCreateMode mode)
    {
        if (mode == ActorCreateMode::Default)
            mode = ActorCreateMode::InScene;

        if (mode != mDefaultCreationMode)
            mode = mode;

        mDefaultCreationMode = mode;
    }

    ActorCreateMode Actor::GetDefaultCreationMode()
    {
        return mDefaultCreationMode;
    }

    bool Actor::IsModeOnScene(ActorCreateMode mode)
    {
        return mode == ActorCreateMode::InScene || (mode == ActorCreateMode::Default && mDefaultCreationMode != ActorCreateMode::NotInScene);
    }

    void Actor::SetParentProp(Actor* actor)
    {
        SetParent(actor, false);
    }

    void Actor::OnAddToScene()
    {
        ISceneDrawable::OnAddToScene();

        for (auto comp : mComponents)
            comp->AddToScene();
    }

    void Actor::OnRemoveFromScene()
    {
        ISceneDrawable::OnRemoveFromScene();

        for (auto comp : mComponents)
            comp->RemoveFromScene();
    }

    void Actor::OnInitialized()
    {
        mState = State::Default;

        for (auto comp : mComponents)
            comp->OnInitialized();
    }

    void Actor::OnStart()
    {
        for (auto comp : mComponents)
            comp->OnStart();
    }

    void Actor::OnDestroy()
    {}

    void Actor::OnUpdate(float dt)
    {}

    void Actor::OnFixedUpdate(float dt)
    {}

    void Actor::OnEnabled()
    {
        ISceneDrawable::OnEnabled();
    }

    void Actor::OnDisabled()
    {
        ISceneDrawable::OnDisabled();
    }

    void Actor::OnTransformUpdated()
    {
        for (auto comp : mComponents)
            comp->OnTransformUpdated();
    }

    void Actor::OnChildAdded(Actor* child)
    {
        for (auto comp : mComponents)
            comp->OnChildAdded(child);

        OnChildrenChanged();
    }

    void Actor::OnChildRemoved(Actor* child)
    {
        for (auto comp : mComponents)
            comp->OnChildRemoved(child);

        OnChildrenChanged();
    }

    void Actor::OnComponentAdded(Component* component)
    {
        for (auto comp : mComponents)
            comp->OnComponentAdded(component);
    }

    void Actor::OnComponentRemoving(Component* component)
    {
        for (auto comp : mComponents)
            comp->OnComponentRemoving(component);
    }

#if !IS_EDITOR
    void Actor::OnChildrenChanged()
    {
        for (auto comp : mComponents)
            comp->OnChildrenChanged();
    }

    void Actor::OnParentChanged(Actor* oldParent) 
    {
        ISceneDrawable::OnDrawbleParentChanged();

        for (auto comp : mComponents)
            comp->OnParentChanged(oldParent);
    }
#endif

    void Actor::UpdateResEnabled(bool withChildren /*= true*/)
    {
        mResEnabled = mEnabled;
        UpdateResEnabledInHierarchy(withChildren);
    }

    void Actor::UpdateResEnabledInHierarchy(bool withChildren /*= true*/)
    {
        bool lastResEnabledInHierarchy = mResEnabledInHierarchy;

        if (mParent)
            mResEnabledInHierarchy = mResEnabled && mParent->mResEnabledInHierarchy;
        else
            mResEnabledInHierarchy = mResEnabled;

        if (lastResEnabledInHierarchy != mResEnabledInHierarchy)
        {
            if (mResEnabledInHierarchy)
                OnEnabled();
            else
                OnDisabled();

            for (auto comp : mComponents)
                comp->UpdateEnabledInHierarchy();

#if IS_EDITOR
            if (mIsOnScene)
                o2Scene.onEnableChanged(this);

            OnChanged();
#endif
        }

        if (withChildren)
        {
            for (auto child : mChildren)
                child->UpdateResEnabledInHierarchy();
        }
    }

    void Actor::SerializeBasicOverride(DataValue& node) const
    {
        if (mPrototypeLink)
            SerializeWithProto(node);
        else
            SerializeRaw(node);

        OnSerialize(node);
    }

    void Actor::DeserializeBasicOverride(const DataValue& node)
    {
        if (node.FindMember("PrototypeLink") || node.FindMember("Prototype"))
            DeserializeWithProto(node);
        else
            DeserializeRaw(node);

        OnDeserialized(node);
    }

    void Actor::SerializeRaw(DataValue& node) const
    {
        SerializeBasic(node);

        node["Id"] = mId;
        node["Transform"].Set(*transform);

        if (!mChildren.IsEmpty())
        {
            auto& childsNode = node.AddMember("Children");
            for (auto child : mChildren)
            {
                auto& childNode = childsNode.AddElement();
                child->Serialize(childNode.AddMember("Data"));
                childNode.AddMember("Type") = child->GetType().GetName();
            }
        }

        if (!mComponents.IsEmpty())
        {
            auto& componentsNode = node.AddMember("Components");
            for (auto component : mComponents)
            {
                auto& componentNode = componentsNode.AddElement();
                componentNode.AddMember("Type") = component->GetType().GetName();
                component->Serialize(componentNode.AddMember("Data"));
            }
        }
    }

    void Actor::DeserializeRaw(const DataValue& node)
    {
        DeserializeBasic(node);

        ActorRefResolver::Instance().LockResolving();
        if (ActorRefResolver::Instance().mLockDepth == 0)
            ActorRefResolver::Instance().ActorCreated(this);

        SetID(node.GetMember("Id"));
        node.GetMember("Transform").Get(*transform);

        RemoveAllComponents();

        if (auto componentsNode = node.FindMember("Components"))
        {
            if (componentsNode->IsArray())
            {
                for (auto& componentNode : *componentsNode)
                {
                    String componentType = componentNode.GetMember("Type");
                    Component* component = (Component*)o2Reflection.CreateTypeSample(componentType);
                    if (component)
                    {
                        component->Deserialize(componentNode.GetMember("Data"));
                        AddComponent(component);
                    }
                    else
                        o2Debug.LogError("Can't create component by type %s", componentType.Data());

                }
            }
        }

        RemoveAllChildren();

        if (auto childsNode = node.FindMember("Children"))
        {
            if (childsNode->IsArray())
            {
                for (auto& childNode : *childsNode)
                {
                    const DataValue* typeNode = childNode.FindMember("Type");
                    const DataValue* dataValue = childNode.FindMember("Data");
                    if (typeNode && dataValue)
                    {
                        const ObjectType* type = dynamic_cast<const ObjectType*>(o2Reflection.GetType(*typeNode));
                        if (type)
                        {
                            Actor* child = dynamic_cast<Actor*>(type->DynamicCastToIObject(type->CreateSample()));
                            child->Deserialize(*dataValue);
                            o2Scene.mRootActors.Remove(child);
                            AddChild(child);
                        }
                    }
                }
            }
        }

        ActorRefResolver::Instance().UnlockResolving();
        ActorRefResolver::Instance().ResolveRefs();
    }

    void Actor::SerializeWithProto(DataValue& node) const
    {
        const Actor* proto = mPrototypeLink.Get();

        // Prototype data
        if (mPrototype)
            node["Prototype"] = mPrototype;

        if (mPrototypeLink)
            node["PrototypeLink"] = proto->GetID();

        // Basic data
        node["Id"] = mId;

        // Transform data
        auto& transformNode = node.AddMember("Transform");
        transform->SerializeDelta(transformNode, *proto->transform);

        if (transformNode.IsEmpty() || transformNode.IsNull())
            node.RemoveMember("Transform");

        SerializeDelta(node, *mPrototypeLink.Get());

        // Children data
        if (!mChildren.IsEmpty())
        {
            auto& childsNode = node.AddMember("Children");
            childsNode.SetArray();
            for (auto child : mChildren)
            {
                auto& childNode = childsNode.AddElement();
                childNode.AddMember("Type") = child->GetType().GetName();
                child->Serialize(childNode.AddMember("Data"));
            }
        }

        // Components data
        if (!mComponents.IsEmpty())
        {
            auto& componentsNode = node.AddMember("Components");
            componentsNode.SetArray();
            for (auto component : mComponents)
            {
                auto& compNode = componentsNode.AddElement();
                compNode.AddMember("Type") = component->GetType().GetName();

                auto& dataValue = compNode.AddMember("Data");
                if (auto componentProtoLink = component->mPrototypeLink)
                {
                    dataValue["PrototypeLink"] = componentProtoLink->mId;
                    dataValue.SetDelta(*component, *component->mPrototypeLink);
                }
                else
                    component->Serialize(dataValue);
            }
        }
    }

    void Actor::DeserializeWithProto(const DataValue& node)
    {
        RemoveAllChildren();
        RemoveAllComponents();

        ActorRefResolver::Instance().LockResolving();
        ActorRefResolver::Instance().ActorCreated(this);

        if (auto prototypeNode = node.FindMember("Prototype"))
            SetPrototype(*prototypeNode);

        if (auto prototypeLinkNode = node.FindMember("PrototypeLink"))
        {
            SceneUID id = *prototypeLinkNode;
            Actor* parent = mParent;
            while (parent && parent->mPrototypeLink)
            {
                bool found = false;

                Actor* protoLink = parent->mPrototypeLink.Get();
                while (protoLink)
                {
                    if (auto fnd = protoLink->FindActorById(id))
                    {
                        mPrototypeLink.CopyWithoutRemap(fnd);
                        found = true;
                        break;
                    }

                    protoLink = protoLink->mPrototypeLink.Get();
                }

                if (found)
                    break;

                parent = parent->mParent;
            }
        }

        if (mPrototypeLink)
            DeserializeDelta(node, *mPrototypeLink.Get());
        else
            DeserializeBasic(node);

        SetID(node.GetMember("Id"));

        if (!mPrototypeLink)
        {
            ActorRefResolver::Instance().UnlockResolving();
            ActorRefResolver::Instance().ResolveRefs();
            return;
        }

        const Actor* proto = mPrototypeLink.Get();

        if (!mCopyVisitor)
            mCopyVisitor = mnew SourceToTargetMapCloneVisitor();

        mCopyVisitor->depth++;
        mCopyVisitor->OnCopyActor(proto, this);

        // Transform data
        if (auto transformNode = node.FindMember("Transform"))
            transform->DeserializeDelta(*transformNode, *proto->transform);
        else
            transform->CopyFrom(*proto->transform);

        // children
        auto childrenNode = node.FindMember("Children");
        if (childrenNode && childrenNode->IsArray())
        {
            for (auto& childNode : *childrenNode)
            {
                const DataValue* typeNode = childNode.FindMember("Type");
                const DataValue* dataValue = childNode.FindMember("Data");
                if (typeNode && dataValue)
                {
                    const ObjectType* type = dynamic_cast<const ObjectType*>(o2Reflection.GetType(*typeNode));
                    if (type)
                    {
                        Actor* child = dynamic_cast<Actor*>(type->DynamicCastToIObject(type->CreateSample()));
                        child->mParent = this;
                        child->mCopyVisitor = mCopyVisitor;
                        child->Deserialize(*dataValue);
                        child->mParent = nullptr;
                        AddChild(child);
                    }
                }
            }
        }

        // components
        auto componentsNode = node.FindMember("Components");
        if (componentsNode && componentsNode->IsArray())
        {
            for (auto& componentNode : *componentsNode)
            {
                String type = componentNode["Type"];
                Component* newComponent = (Component*)o2Reflection.CreateTypeSample(type);
                if (!newComponent)
                {
                    o2Debug.LogError("Can't create component with type:" + type);
                    continue;
                }

                auto& componentDataValue = componentNode["Data"];

                if (auto prototypeLinkNode = componentDataValue.FindMember("PrototypeLink"))
                {
                    SceneUID id = *prototypeLinkNode;
                    if (mPrototypeLink)
                    {
                        for (auto protoLinkComponent : mPrototypeLink->mComponents)
                        {
                            if (protoLinkComponent->mId == id)
                            {
                                newComponent->mPrototypeLink = protoLinkComponent;
                                break;
                            }
                        }
                    }
                }

                if (!newComponent->mPrototypeLink)
                    newComponent->Deserialize(componentDataValue);
                else
                {
                    componentDataValue.GetDelta(*newComponent, *newComponent->mPrototypeLink);
                    mCopyVisitor->OnCopyComponent(newComponent->mPrototypeLink, newComponent);
                }

                AddComponent(newComponent);
            }
        }

        transform->SetDirty();

        ActorRefResolver::Instance().UnlockResolving();
        ActorRefResolver::Instance().ResolveRefs();

        if (mCopyVisitor)
        {
            mCopyVisitor->depth--;

            if (mCopyVisitor->depth == 0)
            {
                mCopyVisitor->Finalize();
                delete mCopyVisitor;
                mCopyVisitor = nullptr;
            }
        }
    }

    ActorAssetRef Actor::GetPrototype() const
    {
        if (mPrototype)
            return mPrototype;

        if (mPrototypeLink && mParent)
            return mParent->GetPrototype();

        return ActorAssetRef();
    }

    ActorAssetRef Actor::GetPrototypeDirectly() const
    {
        return mPrototype;
    }

    void Actor::SetPrototype(ActorAssetRef asset)
    {
#if IS_EDITOR
        if (Scene::IsSingletonInitialzed())
            o2Scene.OnActorPrototypeBroken(this);
#endif

        mPrototype = asset;

        if (asset)
            mPrototypeLink.CopyWithoutRemap(asset->GetActor());
        else
            mPrototypeLink.CopyWithoutRemap(nullptr);

#if IS_EDITOR
        Scene::LinkActorToPrototypesHierarchy(this, mPrototype);
#endif
    }

    ActorRef Actor::GetPrototypeLink() const
    {
        return mPrototypeLink;
    }

    void Actor::SetLayer(const Ref<SceneLayer>& layer)
    {
        mSceneLayer = layer;
        ISceneDrawable::SetDrawingDepthInheritFromParent(false); // Reregister inside
    }

    void Actor::SetLayer(const String& name)
    {
        SetLayer(Ref<SceneLayer>(name));
    }

    const Ref<SceneLayer>& Actor::GetLayer() const
    {
        return mSceneLayer;
    }

    SceneLayer* Actor::GetSceneDrawableSceneLayer() const
    {
        return &const_cast<SceneLayer&>(mSceneLayer.Get());
    }

    ISceneDrawable* Actor::GetParentDrawable()
    {
        return mParent;
    }

    int Actor::GetIndexInParentDrawable() const
    {
        if (mParent)
            return mParent->mChildren.IndexOf((Actor*)this);

        if (Scene::IsSingletonInitialzed() && mIsOnScene)
            return o2Scene.mRootActors.IndexOf((Actor*)this);

        return 0;
    }

    Map<String, Actor*> Actor::GetAllChilds()
    {
        Map<String, Actor*> res;
        for (auto child : mChildren)
            res.Add(child->GetName(), child);

        return res;
    }

    Map<String, Component*> Actor::GetAllComponents()
    {
        Map<String, Component*> res;
        for (auto child : mComponents)
            res.Add(child->GetType().GetName(), child);

        return res;
    }

    void Actor::OnBeforeDestroy()
    {
        if (mResEnabledInHierarchy)
            OnDisabled();

        if (mIsOnScene)
        {
            OnRemoveFromScene();
            mIsOnScene = false;
        }

        OnDestroy();
    }

    String Actor::GetCreateMenuCategory()
    {
        return "";
    }

    String Actor::GetCreateMenuGroup()
    {
        return "";
    }

    void Actor::SourceToTargetMapCloneVisitor::OnCopyActor(const Actor* source, Actor* target)
    {
        sourceToTargetActors[source] = target;
    }

    void Actor::SourceToTargetMapCloneVisitor::OnCopyComponent(const Component* source, Component* target)
    {
        sourceToTargetComponents[source] = target;
    }

    void Actor::SourceToTargetMapCloneVisitor::Finalize()
    {
        ActorRefResolver::RemapReferences(sourceToTargetActors, sourceToTargetComponents);
    }

    void Actor::InstantiatePrototypeCloneVisitor::OnCopyActor(const Actor* source, Actor* target)
    {
        SourceToTargetMapCloneVisitor::OnCopyActor(source, target);
        target->mPrototypeLink.CopyWithoutRemap(const_cast<Actor*>(source));
    }

    void Actor::InstantiatePrototypeCloneVisitor::OnCopyComponent(const Component* source, Component* target)
    {
        SourceToTargetMapCloneVisitor::OnCopyComponent(source, target);
        target->mPrototypeLink = const_cast<Component*>(source);
    }
}
// --- META ---

ENUM_META(o2::Actor::State)
{
    ENUM_ENTRY(Default);
    ENUM_ENTRY(Destroyed);
    ENUM_ENTRY(Destroying);
    ENUM_ENTRY(Initializing);
}
END_ENUM_META;

DECLARE_CLASS(o2::Actor, o2__Actor);
// --- END META ---
