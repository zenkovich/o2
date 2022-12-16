#include "o2/stdafx.h"
#include "Actor.h"

#include "o2/Scene/ActorRefResolver.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/SceneLayer.h"
#include "o2/Utils/Debug/Debug.h"
#include "Components/ScriptableComponent.h"

namespace o2
{
	ActorCreateMode Actor::mDefaultCreationMode = ActorCreateMode::InScene;

	Actor::Actor(ActorTransform* transform, State sceneStatus /*= SceneStatus::WaitingAddToScene*/,
				 const String& name /*= "unnamed"*/, bool enabled /*= true*/, bool resEnabled /*= true*/,
				 bool locked /*= false*/, bool resLocked /*= false*/, const String& layerName /*= ""*/,
				 SceneLayer* layer /*= nullptr*/, SceneUID id /*= Math::Random()*/, UID assetId /*= UID(0)*/):
		transform(transform), mName(name), mEnabled(enabled), mResEnabled(enabled), mResEnabledInHierarchy(resEnabled),
		mLocked(locked), mResLocked(resLocked), mLayerName(layerName), mLayer(layer), mId(id), mAssetId(assetId),
		mState(sceneStatus)
	{}

	Actor::Actor(ActorTransform* transform, ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(transform, IsModeOnScene(mode) ? State::WaitingAddToScene : State::NotInScene)
	{
		tags.onTagAdded = [&](Tag* tag) { tag->mActors.Add(this); };
		tags.onTagRemoved = [&](Tag* tag) { tag->mActors.Remove(this); };

		transform->SetOwner(this);

		if (Scene::IsSingletonInitialzed())
		{
			mLayerName = o2Scene.GetDefaultLayer()->GetName();

			if (IsOnScene())
			{
				mLayer = o2Scene.GetLayer(mLayerName);
				o2Scene.AddActorToSceneDeferred(this);
			}
		}

		ActorRefResolver::ActorCreated(this);
	}

	Actor::Actor(ActorTransform* transform, const Actor& other, ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(transform, IsModeOnScene(mode) ? State::WaitingAddToScene : State::NotInScene,
			  other.mName, other.mEnabled, other.mEnabled, other.mLocked, other.mLocked, other.mLayerName, other.mLayer,
			  Math::Random(), other.mAssetId)
	{
		transform->SetOwner(this);

		ActorRefResolver::LockResolving();

		if (other.mIsAsset)
		{
			other.mCopyVisitor = mnew InstantiatePrototypeCloneVisitor();
			SetPrototype(ActorAssetRef(other.GetAssetID()));
		}

		mName = other.mName;
		mEnabled = other.mEnabled;
		mResEnabled = mEnabled;
		mResEnabledInHierarchy = mEnabled;
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

			ActorAssetRef proto = other.mPrototype;
			while (proto)
			{
				o2Scene.OnActorLinkedToPrototype(proto, this);
				proto = proto->GetActor()->GetPrototype();
			}
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

		if (Scene::IsSingletonInitialzed())
		{
			if (IsOnScene())
			{
				mLayer = o2Scene.GetLayer(mLayerName);
				o2Scene.AddActorToSceneDeferred(this);
			}
		}

		ActorRefResolver::ActorCreated(this);
		ActorRefResolver::UnlockResolving();

		if (other.mCopyVisitor)
		{
			other.mCopyVisitor->depth--;

			if (other.mCopyVisitor->depth == 0)
			{
				other.mCopyVisitor->Finalize();
				delete other.mCopyVisitor;
				other.mCopyVisitor = nullptr;
			}
		}
	}

	Actor::Actor(ActorTransform* transform, const ActorAssetRef& prototype, ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(transform, *prototype->GetActor(), mode)
	{}

	Actor::Actor(ActorTransform* transform, Vector<Component*> components, ActorCreateMode mode /*= ActorCreateMode::Default*/) :
		Actor(transform, mode)
	{
		for (auto comp : components)
			AddComponent(comp);

		OnTransformUpdated();
	}

	Actor::Actor(ActorCreateMode mode /*= CreateMode::Default*/):
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

		if (Scene::IsSingletonInitialzed())
		{
			if (IsOnScene())
			{
				if (mLayer)
				{
					if (mResEnabledInHierarchy)
						mLayer->OnActorDisabled(this);

					mLayer->UnregisterActor(this);
				}

				o2Scene.RemoveActorFromScene(this);
			}
		}

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

		mName = other.mName;
		mEnabled = other.mEnabled;
		mResEnabled = mEnabled;
		mResEnabledInHierarchy = mEnabled;
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

			ActorAssetRef proto = other.mPrototype;
			while (proto)
			{
				o2Scene.OnActorLinkedToPrototype(proto, this);
				proto = proto->GetActor()->GetPrototype();
			}
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

		if (Scene::IsSingletonInitialzed())
		{
			if (IsOnScene())
			{
				mLayer = o2Scene.GetLayer(mLayerName);
				o2Scene.AddActorToSceneDeferred(this);
			}
		}

		ActorRefResolver::ActorCreated(this);
		ActorRefResolver::UnlockResolving();

		if (other.mCopyVisitor)
		{
			other.mCopyVisitor->depth--;

			if (other.mCopyVisitor->depth == 0)
			{
				other.mCopyVisitor->Finalize();
				delete other.mCopyVisitor;
				other.mCopyVisitor = nullptr;
			}
		}

		UpdateResEnabledInHierarchy();
		OnChanged();

		return *this;
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
			comp->Update(dt);
	}

	void Actor::FixedUpdate(float dt)
	{
		OnFixedUpdate(dt);
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
		OnNameChanged();
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

		if (IsOnScene())
			o2Scene.OnActorIdChanged(this, prevId);

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

	void Actor::RemoveFromScene(bool keepEditorObjects /*= false*/)
	{
		if (!IsOnScene())
			return;

		if (Scene::IsSingletonInitialzed())
			o2Scene.RemoveActorFromScene(this, keepEditorObjects);

		OnRemoveFromScene();

		mLayer = nullptr;
		mState = State::NotInScene;

		for (auto child : mChildren)
			child->RemoveFromScene();
	}

	void Actor::AddToScene()
	{
		if (IsOnScene())
			return;

		if (Scene::IsSingletonInitialzed())
		{
			mLayer = o2Scene.GetLayer(mLayerName);
			o2Scene.AddActorToScene(this);
		}

		OnAddToScene();

		mState = State::InScene;

		for (auto child : mChildren)
			child->AddToScene();
	}

	bool Actor::IsOnScene() const
	{
		return mState != State::NotInScene;
	}

	void Actor::SetEnabled(bool enabled)
	{
		if (mEnabled == enabled)
			return;

		mEnabled = enabled;
		UpdateResEnabled();

		onEnableChanged(mEnabled);

		if constexpr (IS_EDITOR)
		{
			if (IsOnScene())
				o2Scene.onEnableChanged(this);
		}

		OnChanged();
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
			if (IsOnScene())
			{
				int lastIdx = o2Scene.mRootActors.IndexOf(this);
				o2Scene.mRootActors.Insert(this, index);

				if (index <= lastIdx)
					lastIdx++;

				o2Scene.mRootActors.RemoveAt(lastIdx);
			}
		}
	}

	void Actor::SetParent(Actor* actor, bool worldPositionStays /*= true*/)
	{
		if ((actor && actor->mParent == this) || actor == this || actor == mParent)
			return;

		Basis lastParentBasis = transform->GetWorldBasis();
		auto oldParent = mParent;

		if (mParent)
			mParent->RemoveChild(this, false);
		else if (IsOnScene() && Scene::IsSingletonInitialzed())
			o2Scene.mRootActors.Remove(this);

		mParent = actor;
		transform->mData->parentInvTransformActualFrame = 0;

		if (mParent)
		{
			mParent->mChildren.Add(this);
			mParent->OnChildAdded(this);
			mParent->OnChildrenChanged();
		}
		else if (IsOnScene() && Scene::IsSingletonInitialzed())
			o2Scene.mRootActors.Add(this);

		if (worldPositionStays)
			transform->SetWorldBasis(lastParentBasis);
		else
			transform->SetDirty();

		UpdateResEnabledInHierarchy();

		if (mParent && mParent->IsOnScene() != IsOnScene())
		{
			if (mParent->IsOnScene())
				AddToScene();
			else
				RemoveFromScene();
		}

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
		AddChild(actor);
		mChildren.Insert(actor, index);
		mChildren.PopBack();

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

	void Actor::RemoveChild(Actor* actor, bool release /*= true*/)
	{
		auto oldParent = actor->mParent;

		actor->mParent = nullptr;
		mChildren.Remove(actor);

		actor->OnParentChanged(oldParent);
		OnChildRemoved(actor);
		OnChildrenChanged();

		if (release)
			delete actor;
		else
		{
			actor->transform->SetDirty();
			actor->UpdateResEnabledInHierarchy();
		}
	}

	void Actor::RemoveAllChildren(bool release /*= true*/)
	{
		for (auto child : mChildren)
		{
			child->mParent = nullptr;
			OnChildRemoved(child);
			child->OnParentChanged(this);

			if (release)
				delete child;
		}

		mChildren.Clear();

		OnChildrenChanged();
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

		OnComponentAdded(component);
		OnChanged();

		return component;
	}

	void Actor::RemoveComponent(Component* component, bool release /*= true*/)
	{
		OnComponentRemoving(component);

		mComponents.Remove(component);
		component->mOwner = nullptr;

		if (release)
			delete component;

		OnChanged();
	}

	void Actor::RemoveAllComponents()
	{
		auto components = mComponents;
		mComponents.Clear();

		for (auto component : components)
		{
			OnComponentRemoving(component);
			delete component;
		}

		OnChanged();
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
#endif


	const Vector<Component*>& Actor::GetComponents() const
	{
		return mComponents;
	}

	void Actor::SetLayer(const String& layerName)
	{
		if (layerName == mLayerName)
			return;

		if (!IsOnScene())
			return;

		auto oldLayer = mLayer;

		OnRemoveFromScene();

		mLayerName = layerName;

		if (Scene::IsSingletonInitialzed())
			mLayer = o2Scene.GetLayer(mLayerName);

		OnAddToScene();

		OnLayerChanged(oldLayer);
		OnChanged();
	}

	SceneLayer* Actor::GetLayer() const
	{
		return mLayer;
	}

	const String& Actor::GetLayerName() const
	{
		return mLayerName;
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

	void Actor::SetProtytypeDummy(ActorAssetRef asset)
	{}

	void Actor::OnTransformUpdated()
	{
		for (auto comp : mComponents)
			comp->OnTransformUpdated();
	}

	void Actor::OnTransformChanged()
	{
		if (mState == State::WaitingAddToScene)
			return;

		for (auto comp : mComponents)
			comp->OnTransformChanged();
	}

	void Actor::SetParentProp(Actor* actor)
	{
		SetParent(actor, false);
	}

	void Actor::OnAddToScene()
	{
		mState = State::InScene;

		if (mLayer)
		{
			mLayer->RegisterActor(this);

			if (mResEnabledInHierarchy)
				mLayer->OnActorEnabled(this);
		}

		for (auto comp : mComponents)
			comp->OnAddToScene();
	}

	void Actor::OnRemoveFromScene()
	{
		if (mLayer) {
			if (mResEnabledInHierarchy)
				mLayer->OnActorDisabled(this);

			mLayer->UnregisterActor(this);
		}

		for (auto comp : mComponents)
			comp->OnRemoveFromScene();
	}

	void Actor::OnStart()
	{}

	void Actor::OnUpdate(float dt)
	{}

	void Actor::OnFixedUpdate(float dt)
	{}

	void Actor::OnEnabled()
	{}

	void Actor::OnDisabled()
	{}

	void Actor::OnChildAdded(Actor* child)
	{
		for (auto comp : mComponents)
			comp->OnChildAdded(child);
	}

	void Actor::OnChildRemoved(Actor* child)
	{
		for (auto comp : mComponents)
			comp->OnChildRemoved(child);
	}

	void Actor::OnLayerChanged(SceneLayer* oldLayer)
	{
		for (auto comp : mComponents)
			comp->OnLayerChanged(oldLayer);
	}

	void Actor::OnComponentAdded(Component* component)
	{
		if (mState == State::InScene)
			component->OnAddToScene();

		for (auto comp : mComponents)
			comp->OnComponentAdded(component);
	}

	void Actor::OnComponentRemoving(Component* component)
	{
		if (IsOnScene())
			component->OnRemoveFromScene();

		for (auto comp : mComponents)
			comp->OnComponentRemoving(component);
	}

	void Actor::UpdateResEnabled()
	{
		mResEnabled = mEnabled;
		UpdateResEnabledInHierarchy();
	}

	void Actor::UpdateResEnabledInHierarchy()
	{
		bool lastResEnabledInHierarchy = mResEnabledInHierarchy;

		if (mParent)
			mResEnabledInHierarchy = mResEnabled && mParent->mResEnabledInHierarchy;
		else
			mResEnabledInHierarchy = mResEnabled;

		if (lastResEnabledInHierarchy != mResEnabledInHierarchy)
		{
			if (mLayer && mState == State::InScene)
			{
				if (mResEnabledInHierarchy)
					mLayer->OnActorEnabled(this);
				else
					mLayer->OnActorDisabled(this);
			}

			if constexpr (IS_EDITOR)
			{
				if (IsOnScene())
					o2Scene.onEnableChanged(this);
			}

			OnEnableInHierarchyChanged();
			OnChanged();
		}

		for (auto comp : mComponents)
			comp->UpdateEnabled();

		for (auto child : mChildren)
			child->UpdateResEnabledInHierarchy();
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

		SetLayer(layerName);
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
						AddChild(child);
						child->mCopyVisitor = mCopyVisitor;
						child->Deserialize(*dataValue);
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

				mComponents.Add(newComponent);
				newComponent->mOwner = this;

				if (newComponent)
				{
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
				}
				else o2Debug.LogError("Can't create component with type:" + type);
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
		if (Scene::IsSingletonInitialzed())
			o2Scene.OnActorPrototypeBroken(this);

		auto linkAsset = asset;
		while (linkAsset)
		{
			if (Scene::IsSingletonInitialzed())
				o2Scene.OnActorLinkedToPrototype(linkAsset, this);

			linkAsset = linkAsset->GetActor()->GetPrototype();
		}

		mPrototype = asset;

		if (asset)
			mPrototypeLink.CopyWithoutRemap(asset->GetActor());
		else
			mPrototypeLink.CopyWithoutRemap(nullptr);
	}

	ActorRef Actor::GetPrototypeLink() const
	{
		return mPrototypeLink;
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

	void Actor::OnEnableInHierarchyChanged()
	{
		if (mResEnabledInHierarchy)
			OnEnabled();
		else
			OnDisabled();
	}

	void Actor::CopyFields(Vector<const FieldInfo*>& fields, IObject* source, IObject* dest, Vector<Actor**>& actorsPointers,
						   Vector<Component**>& componentsPointers, Vector<ISerializable*>& serializableObjects)
	{
		for (auto field : fields)
		{
			if (!field->HasAttribute<SerializableAttribute>())
				continue;

			if (*field->GetType() == TypeOf(Actor*))
			{
				Actor* sourceValue = field->GetValue<Actor*>(source);
				Actor** destValuePtr = (Actor**)(field->GetValuePtrStrong(dest));

				*destValuePtr = sourceValue;
				actorsPointers.Add(destValuePtr);
			}
			else if (*field->GetType() == TypeOf(Component*))
			{
				Component* sourceValue = field->GetValue<Component*>(source);
				Component** destValuePtr = (Component**)(field->GetValuePtrStrong(dest));

				*destValuePtr = sourceValue;
				componentsPointers.Add(destValuePtr);
			}
			else if (field->GetType()->IsBasedOn(TypeOf(AssetRef)))
			{
				field->CopyValue(dest, source);
			}
			else if (field->GetType()->IsBasedOn(TypeOf(IObject)))
			{
				if (field->GetType()->IsBasedOn(TypeOf(ISerializable)))
					serializableObjects.Add((ISerializable*)field->GetValuePtr(dest));

				auto fields = field->GetType()->GetFieldsWithBaseClasses();
				CopyFields(fields, (IObject*)field->GetValuePtr(source),
					(IObject*)field->GetValuePtr(dest),
						   actorsPointers, componentsPointers, serializableObjects);
			}
			else field->CopyValue(dest, source);
		}
	}

	void Actor::CollectFixingFields(Component* newComponent, Vector<Component**>& componentsPointers,
									Vector<Actor**>& actorsPointers)
	{
		Vector<const FieldInfo*> fields;
		GetComponentFields(newComponent, fields);

		for (auto field : fields)
		{
			if (field->GetType()->GetUsage() == Type::Usage::Pointer)
			{
				const PointerType* fieldType = (const PointerType*)field->GetType();

				if (fieldType->GetUnpointedType()->IsBasedOn(TypeOf(Component)))
					componentsPointers.Add((Component**)(field->GetValuePtrStrong(newComponent)));

				if (*fieldType == TypeOf(Actor*))
					actorsPointers.Add((Actor**)(field->GetValuePtrStrong(newComponent)));
			}
		}
	}

	void Actor::GetComponentFields(Component* component, Vector<const FieldInfo*>& fields)
	{
		struct helper
		{
			static void GetFields(const Type* type, Vector<const FieldInfo*>& fields)
			{
				fields.Add(type->GetFields().Convert<const FieldInfo*>([](const auto& x) { return &x; }));

				for (auto baseType : type->GetBaseTypes())
				{
					if (*baseType.type != TypeOf(Component))
						GetFields(baseType.type, fields);
				}
			}
		};

		helper::GetFields(&component->GetType(), fields);
	}

	void Actor::FixComponentFieldsPointers(const Vector<Actor**>& actorsPointers,
										   const Vector<Component**>& componentsPointers,
										   const Map<const Actor*, Actor*>& actorsMap,
										   const Map<const Component*, Component*>& componentsMap)
	{
		for (auto actorPtr : actorsPointers)
		{
			Actor* newActorPtr;
			if (actorsMap.TryGetValue(*actorPtr, newActorPtr))
				*actorPtr = newActorPtr;
		}

		for (auto componentPtr : componentsPointers)
		{
			Component* newComponentPtr;
			if (componentsMap.TryGetValue(*componentPtr, newComponentPtr))
				*componentPtr = newComponentPtr;
		}
	}
	
#if IS_SCRIPTING_SUPPORTED
	ActorTransform* Actor::GetTransform()
	{
		return transform;
	}
#endif

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

#if !IS_EDITOR

	void Actor::OnChanged() {}

	void Actor::OnLockChanged() {}

	void Actor::OnNameChanged() {}

	void Actor::OnChildrenChanged() {}

	void Actor::OnParentChanged(Actor* oldParent)
	{
		for (auto comp : mComponents)
			comp->OnParentChanged(oldParent);
	}

#endif // !IS_EDITOR
}

ENUM_META(o2::Actor::State)
{
	ENUM_ENTRY(Destroying);
	ENUM_ENTRY(InScene);
	ENUM_ENTRY(NotInScene);
	ENUM_ENTRY(WaitingAddToScene);
}
END_ENUM_META;

DECLARE_CLASS(o2::Actor);
