#include "o2/stdafx.h"
#include "Scene.h"

#include "o2/Application/Input.h"
#include "o2/Assets/Assets.h"
#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Physics/PhysicsWorld.h"
#include "o2/Render/Render.h"
#include "o2/Render/Text.h"
#include "o2/Render/VectorFontEffects.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/ActorRefResolver.h"
#include "o2/Scene/CameraActor.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/DrawableComponent.h"
#include "o2/Scene/SceneLayer.h"
#include "o2/Scene/Tags.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
    DECLARE_SINGLETON(Scene);

    Scene::Scene()
    {
        mDefaultLayer = AddLayer("Default");
        auto camera = mmake<CameraActor>();
        camera->name = "Camera";

        mLog = mmake<LogStream>("Scene");
        o2Debug.GetLog()->BindStream(mLog);
    }

    Scene::~Scene()
    {
        Clear();
        ClearCache();
    }

    const LogStream& Scene::GetLogStream() const
    {
        return *mLog;
    }

    const Vector<WeakRef<CameraActor>>& Scene::GetCameras() const
    {
        return mCameras;
    }

    void Scene::Update(float dt)
    {
        mIsUpdatingScene = true;

        UpdateAddedEntities();
        UpdateStartingEntities();
        UpdateDestroyingEntities();
        UpdateActors(dt);

        mIsUpdatingScene = false;
    }

    void Scene::FixedUpdate(float dt)
    {
        for (auto& actor : mRootActors)
            actor->FixedUpdate(dt);

        for (auto& actor : mRootActors)
            actor->FixedUpdateChildren(dt);
    }

    void Scene::UpdateAddedEntities()
    {
        struct helper
        {
            static void RecursiveCall(const Ref<Actor>& actor, const Function<void(const Ref<Actor>&)>& func)
            {
                if (actor->mState == Actor::State::Initializing)
                {
                    func(actor);

                    for (auto& child : actor->GetChildren())
                        RecursiveCall(child, func);
                }
            }
        };

        auto addedActors = mAddedActors;
        mAddedActors.Clear();

        mStartActors = addedActors;

        for (auto& actor : addedActors)
        {
            if (actor->IsOnScene())
                helper::RecursiveCall(actor, [&](const Ref<Actor>& actor) { AddActorToScene(actor.Get()); });
        }

        for (auto& actor : addedActors)
            helper::RecursiveCall(actor, [&](const Ref<Actor>& actor) { actor->UpdateResEnabledInHierarchy(); });

        for (auto& actor : addedActors)
            helper::RecursiveCall(actor, [&](const Ref<Actor>& actor) { actor->OnInitialized(); });
    }

    void Scene::UpdateTransforms()
    {
        for (auto& actor : mRootActors)
            actor->UpdateSelfTransform();

        for (auto& actor : mRootActors)
            actor->UpdateChildrenTransforms();
    }

    void Scene::UpdateStartingEntities()
    {
        auto startActors = mStartActors;
        mStartActors.Clear();

        for (auto& actor : startActors)
            actor->OnStart();

        auto startComponents = mStartComponents;
        mStartComponents.Clear();

        for (auto& comp : startComponents)
            comp->OnStart();
    }

    void Scene::UpdateDestroyingEntities()
    {
        auto destroyActors = mDestroyActors;

        mDestroyActors.Clear();
        mDestroyComponents.Clear();

        for (auto& actor : destroyActors)
            actor->OnBeforeDestroy();

        destroyActors.Clear();

#if IS_EDITOR
        mDestroyingObjects.Clear();
#endif
    }

    void Scene::DestroyActor(const Ref<Actor>& actor)
    {
        if (actor->mState != Actor::State::Destroying)
        {
            actor->mState = Actor::State::Destroying;
            mDestroyActors.Add(actor);

#if IS_EDITOR
            mChangedObjects.Remove(actor);
#endif
        }
    }

    void Scene::DestroyComponent(const Ref<Component>& component)
    {
        mDestroyComponents.Add(component);
    }

    bool Scene::IsUpdating() const
    {
        return mIsUpdatingScene;
    }

    bool Scene::IsEditor() const
    {
#if IS_EDITOR
        return true;
#endif

        return false;
    }

    void Scene::OnActorCreated(Actor* actor)
    {
        if (!IsSingletonInitialzed())
            return;

        Instance().mAddedActors.Add(Ref(actor));
    }

    void Scene::OnActorDestroy(Actor* actor)
    {
        if (!IsSingletonInitialzed())
            return;

        if (actor->IsOnScene())
            Instance().RemoveActorFromScene(actor, false);

        Instance().mAddedActors.RemoveFirst([=](auto& x) { return x == actor; });
        Instance().mStartActors.RemoveFirst([=](auto& x) { return x == actor; });

#if IS_EDITOR
        Instance().mChangedObjects.RemoveFirst([=](auto& x) { return x == actor; });
#endif
    }

    void Scene::OnNewActorParented(Actor* actor)
    {
        if (!IsSingletonInitialzed())
            return;

        Instance().mAddedActors.Remove(Ref(actor));
    }

    void Scene::OnAddActorToScene(Actor* actor)
    {
        Assert(IsSingletonInitialzed(), "Cant add actor to scene, because scene not initialized")

            Instance().AddActorToScene(actor);
    }

    void Scene::OnRemoveActorFromScene(Actor* actor, bool keepEditorObjects /*= false*/)
    {
        if (!IsSingletonInitialzed())
            return;

        Instance().RemoveActorFromScene(actor, keepEditorObjects);
    }

    void Scene::OnActorIdChanged(Actor* actor, SceneUID prevId)
    {
        if (!IsSingletonInitialzed())
            return

            Instance().mActorsMap.Remove(prevId);
        Instance().mActorsMap[actor->mId] = Ref(actor);
    }

    void Scene::UpdateActors(float dt)
    {
        for (auto& actor : mRootActors)
            actor->Update(dt);

        for (auto& actor : mRootActors)
            actor->UpdateChildren(dt);
    }

#undef DrawText

    void Scene::Draw()
    {
        DrawCameras();
    }

    void Scene::DrawCameras()
    {
#if IS_EDITOR
        BeginDrawingScene();
#endif

        if (mCameras.IsEmpty())
        {
            if constexpr (IS_EDITOR)
            {
                o2Render.SetCamera(Camera());
                o2Render.Clear();

                static auto font = mnew VectorFont(o2Assets.GetBuiltAssetsPath() + "debugFont.ttf");
                static auto text = mmake<Text>(Ref<Font>(font));

                text->SetText("No camera");
                text->Draw();
            }
        }
        else
        {
            PROFILE_SAMPLE("Draw cameras");

            for (auto& camera : mCameras)
            {
                if (auto cameraRef = camera.Lock()) 
                {
                    cameraRef->SetupAndDraw();
                    cameraRef->listenersLayer.OnDrawn(Camera().GetBasis());
                }
            }
        }

        DrawCursorDebugInfo();

#if IS_EDITOR
        EndDrawingScene();
#endif
    }

    void Scene::DrawCursorDebugInfo()
    {
        if (!o2Input.IsKeyDown(VK_F4))
            return;

        struct helper
        {
            static void Process(String& debugInfo, Actor* actor)
            {
                if (actor->transform->IsPointInside(o2Input.GetCursorPos()))
                {
                    auto parent = actor;
                    String path;
                    while (parent)
                    {
                        path = parent->GetName() + "/" + path;
                        parent = parent->GetParent().Lock().Get();
                    }

                    debugInfo += " -- " + path + " - " + actor->GetType().GetName() + "\n";
                    debugInfo += "Rect: " + (String)actor->transform->worldRect + "\n";

                    Widget* widget = dynamic_cast<Widget*>(actor);
                    if (widget)
                    {
                        debugInfo += "layout - " +
                            (String)widget->layout->anchorMin + " " + (String)widget->layout->offsetMin + " - " +
                            (String)widget->layout->anchorMax + " " + (String)widget->layout->offsetMax + "\n";

                        debugInfo += (widget->mIsClipped ? (String)"clipped, " : (String)"not clipped, ") +
                            (widget->IsEnabled() ? (String)"visible\n" : ((String)"hidden" + (!widget->mResEnabledInHierarchy ? (String)" fully\n" : (String)"\n")));

                        debugInfo += "alpha: " + (String)widget->mTransparency +
                            ", res alpha: " + (String)widget->mResTransparency + "\n";
                    }

                    debugInfo += "\n";

                    if (widget)
                    {
                        for (auto& child : widget->mInternalWidgets)
                            Process(debugInfo, child.Get());
                    }
                }

                for (auto& child : actor->mChildren)
                    Process(debugInfo, child.Get());
            }
        };

        String debugInfo;
        for (auto& actor : mAllActors)
            helper::Process(debugInfo, actor.Lock().Get());

        o2Debug.DrawText(((Vec2F)o2Render.GetResolution().InvertedX()) * 0.5f, debugInfo);
    }

    void Scene::AddActorToScene(Actor* actor)
    {
        auto actorRef = Ref(actor);

        if (!actor->mParent)
            mRootActors.Add(actorRef);

        mAllActors.Add(actorRef);
        mActorsMap[actor->mId] = actorRef;

        actor->OnAddToScene();

#if IS_EDITOR
        mChangedObjects.Add(actorRef);
        AddEditableObjectToScene(actorRef);
        onAddedToScene(actorRef);
#endif
    }

    void Scene::RemoveActorFromScene(Actor* actor, bool keepEditorObjects /*= false*/)
	{
		auto actorRef = Ref(actor);

        if (!actor->mParent)
            mRootActors.Remove(actorRef);

        mAllActors.Remove(actorRef);
        mActorsMap.Remove(actor->mId);

        mStartActors.Remove(actorRef);
        mAddedActors.Remove(actorRef);

        actor->OnRemoveFromScene();

#if IS_EDITOR
        if (!keepEditorObjects)
            RemoveEditableObjectFromScene(actorRef);

        OnObjectRemoveFromScene(actorRef);
#endif
    }

    void Scene::OnComponentAdded(Component* component)
    {
        mStartComponents.Add(Ref(component));
    }

    void Scene::OnComponentRemoved(Component* component)
    {
        mStartComponents.RemoveFirst([&](auto& x) { return x == component; });
    }

    void Scene::OnLayerRenamed(SceneLayer* layer, const String& oldName)
    {
        mLayersMap.Remove(oldName);
        mLayersMap[layer->GetName()] = Ref(layer);

#if IS_EDITOR
        onLayersListChanged();
#endif
    }

    void Scene::OnCameraAddedOnScene(CameraActor* camera)
    {
        mCameras.Add(Ref(camera));
    }

    void Scene::OnCameraRemovedScene(CameraActor* camera)
    {
        mCameras.Remove(Ref(camera));
    }

    bool Scene::HasLayer(const String& name) const
    {
        return mLayersMap.ContainsKey(name);
    }

    Ref<SceneLayer> Scene::GetLayer(const String& name)
    {
        WeakRef<SceneLayer> layer;
        if (mLayersMap.TryGetValue(name, layer))
            return layer.Lock();

        return AddLayer(name);
    }

    const Ref<SceneLayer>& Scene::GetDefaultLayer() const
    {
        return mDefaultLayer;
    }

    Ref<SceneLayer> Scene::AddLayer(const String& name)
    {
        WeakRef<SceneLayer> layer;
        if (mLayersMap.TryGetValue(name, layer))
            return layer.Lock();

        auto newLayer = mmake<SceneLayer>();
        newLayer->mName = name;
        mLayers.Add(newLayer);
        mLayersMap[name] = newLayer;

#if IS_EDITOR
        onLayersListChanged();
#endif

        return newLayer;
    }

    void Scene::RemoveLayer(const Ref<SceneLayer>& layer)
    {
        if (layer == mDefaultLayer)
            return;

        mLayers.Remove(layer);
        mLayersMap.Remove(layer->mName);

#if IS_EDITOR
        onLayersListChanged();
#endif
    }

    void Scene::RemoveLayer(const String& name)
    {
        RemoveLayer(GetLayer(name));
    }

    void Scene::SetLayerOrder(const Ref<SceneLayer>& layer, int idx)
    {
        mLayers.Remove(layer);
        mLayers.Insert(layer, idx);
    }

    Ref<Tag> Scene::GetTag(const String& name) const
    {
        return mTags.FindOrDefault([&](auto x) { return x->GetName() == name; });
    }

    Ref<Tag> Scene::AddTag(const String& name)
    {
        if (GetTag(name))
        {
            o2Debug.LogError("Can't create new tag with name " + name + ": already exist");
            return nullptr;
        }

        auto newTag = mmake<Tag>();
        newTag->SetName(name);
        mTags.Add(newTag);

        return newTag;
    }

    void Scene::RemoveTag(const Ref<Tag>& tag)
    {
        mTags.Remove(tag);
    }

    void Scene::RemoveTag(const String& name)
    {
        RemoveTag(GetTag(name));
    }

    const Vector<Ref<SceneLayer>>& Scene::GetLayers()const
    {
        return mLayers;
    }

    Vector<String> Scene::GetLayersNames() const
    {
        return mLayers.Convert<String>([](auto& x) { return x->GetName(); });
    }

    const Map<String, WeakRef<SceneLayer>>& Scene::GetLayersMap() const
    {
        return mLayersMap;
    }

    const Vector<Ref<Tag>>& Scene::GetTags() const
    {
        return mTags;
    }

    const Vector<Ref<Actor>>& Scene::GetRootActors() const
    {
        return mRootActors;
    }

    Vector<Ref<Actor>>& Scene::GetRootActors()
    {
        return mRootActors;
    }

    const Vector<WeakRef<Actor>>& Scene::GetAllActors() const
    {
        return mAllActors;
    }

    Vector<WeakRef<Actor>>& Scene::GetAllActors()
    {
        return mAllActors;
    }

    Ref<Actor> Scene::GetActorByID(SceneUID id) const
    {
        return mActorsMap.FindKey(id).second.Lock();
    }

    Ref<Actor> Scene::GetAssetActorByID(const UID& id)
    {
        auto cached = mAssetsCache.FindOrDefault([=](const Ref<ActorAsset>& x) { return x->GetUID() == id; });

        if (!cached)
        {
            cached = Ref<ActorAsset>(id);
            mAssetsCache.Add(cached);
        }

        return cached->GetActor();
    }

    Ref<Actor> Scene::FindActor(const String& path)
    {
        int delPos = path.Find("/");
        String pathPart = path.SubStr(0, delPos);

        for (auto& actor : mRootActors)
        {
            if (actor->mName == pathPart)
            {
                if (delPos == -1)
                    return actor;
                else
                    return actor->GetChild(path.SubStr(delPos + 1));
            }
        }

        return nullptr;
    }

    void Scene::Clear(bool keepDefaultLayer /*= true*/)
    {
        auto allActors = mRootActors;
        for (auto& actor : allActors)
            actor->OnBeforeDestroy();

        mRootActors.Clear();
        allActors.Clear();
        mLayers.Clear();

        mAddedActors.Clear();
        mStartActors.Clear();
        mStartComponents.Clear();
        mLayers.Clear();
        mLayersMap.Clear();

        if (keepDefaultLayer)
            mDefaultLayer = AddLayer("Default");

        mTags.Clear();
    }

    void Scene::ClearCache()
    {
        mAssetsCache.Clear();
    }

    void Scene::Load(const String& path, bool append /*= false*/)
    {
        DataDocument data;
        if (data.LoadFromFile(path))
            Load(data, append);
        else
            o2Debug.LogError("Failed to load scene " + path);
    }

    void Scene::Load(const DataDocument& doc, bool append /*= false*/)
    {
        ActorRefResolver::Instance().LockResolving();

        if (!append)
            Clear(false);

        auto& layersNode = doc.GetMember("Layers");
        if (!layersNode.IsEmpty())
        {
            for (auto& layerNode : layersNode)
            {
                auto layer = mmake<SceneLayer>();
                layer->Deserialize(layerNode);
                mLayers.Add(layer);
                mLayersMap[layer->mName] = layer;
            }
        }

        auto& defaultLayerNode = doc.GetMember("DefaultLayer");
        if (!defaultLayerNode.IsEmpty())
            mDefaultLayer = GetLayer(defaultLayerNode);
        else
            mDefaultLayer = AddLayer("Default");

#if IS_EDITOR
        onLayersListChanged();
#endif

        auto& tagsNode = doc.GetMember("Tags");
        if (!tagsNode.IsEmpty())
        {
            for (auto& tagNode : tagsNode)
            {
                auto tag = mmake<Tag>();
                tag->Deserialize(tagNode);
                mTags.Add(tag);
            }
        }

        auto& actorsNode = doc.GetMember("Actors");
        if (!actorsNode.IsEmpty())
        {
            mRootActors = actorsNode;
            mRootActors.Clear();
        }

        ActorRefResolver::Instance().UnlockResolving();
        ActorRefResolver::Instance().ResolveRefs();

        UpdateAddedEntities();
        UpdateTransforms();
        UpdateDestroyingEntities();

#if IS_EDITOR
        mChangedObjects.Clear();
#endif
    }

    void Scene::Save(const String& path)
    {
        DataDocument data;
        Save(data);
        data.SaveToFile(path);
    }

    void Scene::Save(DataDocument& doc)
    {
        auto& layersNode = doc.AddMember("Layers");
        for (auto& layer : mLayers)
            layer->Serialize(layersNode.AddElement());

        doc.AddMember("DefaultLayer") = mDefaultLayer->mName;

        auto& tagsNode = doc.AddMember("Tags");
        for (auto& tag : mTags)
            tag->Serialize(tagsNode.AddElement());

        doc.AddMember("Actors") = mRootActors;
    }

#if IS_EDITOR
    void Scene::LinkActorToPrototypesHierarchy(Ref<Actor> actor, Ref<ActorAsset> proto)
    {
        if (!IsSingletonInitialzed())
            return;

        while (proto)
        {
            o2Scene.OnActorLinkedToPrototype(proto, actor);
            proto = proto->GetActor()->GetPrototype();
        }
    }

    bool Scene::IsEditorPlaying() const
    {
        return mIsPlaying;
    }
    
    void Scene::SetEditorPlaying(bool playing)
    {
        mIsPlaying = playing;
    }

    Vector<Ref<SceneEditableObject>> Scene::GetRootEditableObjects()
    {
        return mRootActors.Convert<Ref<SceneEditableObject>>([](const Ref<Actor>& x) { return DynamicCast<SceneEditableObject>(x); });
    }

    void Scene::AddEditableObjectToScene(const Ref<SceneEditableObject>& object)
    {
        mEditableObjects.Add(object);
        mEditableObjectsByUID[object->GetID()] = object;
    }

    void Scene::RemoveEditableObjectFromScene(const Ref<SceneEditableObject>& object)
    {
        mChangedObjects.RemoveAll([&](auto x) { return x == object; });
        mEditableObjects.RemoveAll([&](auto x) { return x == object; });
        mEditableObjectsByUID.Remove(object->GetID());
    }

    void Scene::DestroyEditableObject(const Ref<SceneEditableObject>& object)
    {
        mDestroyingObjects.Add(object);
    }

    const Vector<WeakRef<SceneEditableObject>>& Scene::GetAllEditableObjects()
    {
        return mEditableObjects;
    }

    Ref<SceneEditableObject> Scene::GetEditableObjectByID(SceneUID id) const
    {
        return mEditableObjectsByUID[id].Lock();
    }

    int Scene::GetObjectHierarchyIdx(const Ref<SceneEditableObject>& object) const
    {
        if (object->GetEditableParent())
        {
            return object->GetEditableParent()->GetEditableChildren().IndexOf(object) + GetObjectHierarchyIdx(object->GetEditableParent());
        }

        return mRootActors.IndexOf([=](const Ref<Actor>& x) { return DynamicCast<SceneEditableObject>(x) == object; });
    }

    void Scene::ReparentEditableObjects(const Vector<Ref<SceneEditableObject>>& objects,
                                        const Ref<SceneEditableObject>& newParent, const Ref<SceneEditableObject>& prevObject)
    {
        struct Object
        {
            Ref<SceneEditableObject> object;

            int   idx;
            Basis transform;

            bool operator==(const Object& other) const { return object == other.object; }
        };
        Vector<Object> objectsDefs;

        for (auto& object : objects)
        {
            Object def;
            def.object = object;
            def.transform = object->GetTransform();
            def.idx = o2Scene.GetObjectHierarchyIdx(object);
            objectsDefs.Add(def);

            object->SetEditableParent(nullptr);
            mRootActors.Remove(DynamicCast<Actor>(object));
        }

        objectsDefs.Sort([](auto& a, auto& b) { return a.idx < b.idx; });

        if (newParent)
        {
            int insertIdx = newParent->GetEditableChildren().IndexOf(prevObject) + 1;

            for (auto& def : objectsDefs)
            {
                newParent->AddEditableChild(def.object, insertIdx++);
                def.object->SetTransform(def.transform);
            }
        }
        else
        {
            int insertIdx = 0;

            if (prevObject)
            {
                insertIdx = mRootActors.IndexOf([=](const Ref<Actor>& x) { return DynamicCast<SceneEditableObject>(x) == prevObject; });

                if (insertIdx < 0)
                    insertIdx = mRootActors.Count();
                else
                    insertIdx++;
            }

            for (auto& def : objectsDefs)
            {
                auto actorEditableObject = DynamicCast<Actor>(def.object);
                if (actorEditableObject)
                {
                    mRootActors.Insert(actorEditableObject, insertIdx++);
                    def.object->OnEditableParentChanged(nullptr);
                    def.object->SetTransform(def.transform);
                }
            }
        }

        if (newParent)
            OnObjectChanged(newParent);

        if (prevObject)
            OnObjectChanged(prevObject);
    }

    void Scene::CheckChangedObjects()
    {
        mChangedObjects.RemoveAll([](auto x) { return x == nullptr; });

        if (mChangedObjects.Count() > 0)
        {
            onObjectsChanged(mChangedObjects);

            auto changedObjectsCopy = mChangedObjects;
            for (auto& obj : changedObjectsCopy)
                obj->Update(0.0f);

            mChangedObjects.Clear();

            UpdateAddedEntities();
            UpdateDestroyingEntities();
        }
    }

    const Vector<Ref<SceneEditableObject>>& Scene::GetChangedObjects() const
    {
        return mChangedObjects;
    }

    const Vector<WeakRef<SceneEditableObject>>& Scene::GetDrawnEditableObjects() const
    {
        return mDrawnObjects;
    }

    Map<Ref<ActorAsset>, Vector<WeakRef<Actor>>>& Scene::GetPrototypesLinksCache()
    {
        return mPrototypeLinksCache;
    }

    void Scene::BeginDrawingScene()
    {
        mIsDrawingScene = true;
        mDrawnObjects.Clear();
    }

    void Scene::EndDrawingScene()
    {
        mIsDrawingScene = false;
    }

    void Scene::OnObjectAddToScene(const Ref<SceneEditableObject>& object)
    {
        onAddedToScene(object);
        OnObjectChanged(object);
    }

    void Scene::OnObjectRemoveFromScene(const Ref<SceneEditableObject>& object)
    {
        onRemovedFromScene(object);
        OnObjectChanged(nullptr);
    }

    void Scene::OnObjectChanged(const Ref<SceneEditableObject>& object)
    {
        if (mIsUpdatingScene)
            return;

        if (PhysicsWorld::IsSingletonInitialzed() && o2Physics.IsUpdatingPhysicsNow())
            return;

        if (object && object->changedFrame == o2Time.GetCurrentFrame())
            return;

        if (object)
            object->changedFrame = o2Time.GetCurrentFrame();

        mChangedObjects.Add(object);
    }

    void Scene::OnObjectDrawn(const Ref<SceneEditableObject>& object)
    {
        if (mIsDrawingScene)
            mDrawnObjects.Add(object);
    }

    void Scene::OnActorWithPrototypeCreated(const Ref<Actor>& actor)
    {
        auto proto = actor->GetPrototype();
        OnActorLinkedToPrototype(proto, actor);
    }

    void Scene::OnActorLinkedToPrototype(Ref<ActorAsset>& assetRef, const Ref<Actor>& actor)
    {
        if (!mPrototypeLinksCache.ContainsKey(assetRef))
            mPrototypeLinksCache.Add(assetRef, Vector<WeakRef<Actor>>());

        mPrototypeLinksCache[assetRef].Add(actor);
    }

    void Scene::OnActorPrototypeBroken(Actor* actor)
    {
        // !!! TODO: Optimize this
        for (auto it = mPrototypeLinksCache.Begin(); it != mPrototypeLinksCache.End();)
        {
            it->second.RemoveFirst([=](auto& x) { return x == actor; });
            if (it->second.IsEmpty())
            {
                mPrototypeLinksCache.Remove(it->first);
                it = mPrototypeLinksCache.Begin();
            }
            else it++;
        }
    }

#endif
}
// --- META ---

DECLARE_CLASS(o2::Scene, o2__Scene);
// --- END META ---
