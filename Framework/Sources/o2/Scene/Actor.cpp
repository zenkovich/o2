#include "o2/stdafx.h"
#include "Actor.h"

#include "o2/Scene/ActorDataValueConverter.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Scene.h"
#include "o2/Scene/SceneLayer.h"

namespace o2
{
	ActorCreateMode Actor::mDefaultCreationMode = ActorCreateMode::InScene;

	Actor::Actor(ActorTransform* transform, SceneStatus sceneStatus /*= SceneStatus::WaitingAddToScene*/, 
				 const String& name /*= "unnamed"*/, bool enabled /*= true*/, bool resEnabled /*= true*/, 
				 bool locked /*= false*/, bool resLocked /*= false*/, const String& layerName /*= ""*/, 
				 SceneLayer* layer /*= nullptr*/, SceneUID id /*= Math::Random()*/, UID assetId /*= UID(0)*/):
		transform(transform), mName(name), mEnabled(enabled), mResEnabled(enabled), mResEnabledInHierarchy(resEnabled),
		mLocked(locked), mResLocked(resLocked), mLayerName(layerName), mLayer(layer), mId(id), mAssetId(assetId),
		mSceneStatus(sceneStatus)
	{}

	Actor::Actor(ActorTransform* transform, ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(transform, IsModeOnScene(mode) ? SceneStatus::WaitingAddToScene : SceneStatus::NotInScene)
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

		ActorDataValueConverter::ActorCreated(this);
	}

	Actor::Actor(ActorTransform* transform, const ActorAssetRef& prototype, ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(transform, IsModeOnScene(mode) ? SceneStatus::WaitingAddToScene : SceneStatus::NotInScene, 
			  prototype->GetActor()->mName, prototype->GetActor()->mEnabled, prototype->GetActor()->mEnabled,
			  prototype->GetActor()->mLocked, prototype->GetActor()->mLocked, prototype->GetActor()->mLayerName, nullptr)
	{
		transform->SetOwner(this);

		SetPrototype(prototype);

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Map<const Actor*, Actor*> actorsMap;
		Map<const Component*, Component*> componentsMap;

		ProcessCopying(this, prototype->GetActor(), actorPointersFields, componentPointersFields, actorsMap, componentsMap, true);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		transform->SetDirty();

		if (Scene::IsSingletonInitialzed())
		{
			if (IsOnScene())
			{
				mLayer = o2Scene.GetLayer(mLayerName);
				o2Scene.AddActorToSceneDeferred(this);
			}
		}

		ActorDataValueConverter::ActorCreated(this);
	}

	Actor::Actor(ActorTransform* transform, Vector<Component*> components, ActorCreateMode mode /*= ActorCreateMode::Default*/):
		Actor(transform, mode)
	{
		for (auto comp : components)
			AddComponent(comp);

		OnTransformUpdated();
	}

	Actor::Actor(ActorTransform* transform, const Actor& other):
		Actor(transform, IsModeOnScene(mDefaultCreationMode) ? SceneStatus::WaitingAddToScene : SceneStatus::NotInScene, 
			  other.mName, other.mEnabled, other.mEnabled, other.mLocked, other.mLocked, other.mLayerName, other.mLayer, 
			  Math::Random(), other.mAssetId)
	{
		transform->SetOwner(this);

		if (other.mIsAsset)
			SetPrototype(ActorAssetRef(other.GetAssetID()));

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Map<const Actor*, Actor*> actorsMap;
		Map<const Component*, Component*> componentsMap;

		ProcessCopying(this, &other, actorPointersFields, componentPointersFields, actorsMap, componentsMap, true);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		transform->SetDirty();

		if (Scene::IsSingletonInitialzed())
		{
			if (IsOnScene())
			{
				mLayer = o2Scene.GetLayer(mLayerName);
				o2Scene.AddActorToSceneDeferred(this);
			}
		}

		ActorDataValueConverter::ActorCreated(this);
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

	Actor::Actor(const Actor& other) :
		Actor(mnew ActorTransform(*other.transform), other)
	{}

	Actor::~Actor()
	{
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
	}

	Actor& Actor::operator=(const Actor& other)
	{
		RemoveAllChildren();
		RemoveAllComponents();

		SetPrototype(other.mPrototype);

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Map<const Actor*, Actor*> actorsMap;
		Map<const Component*, Component*> componentsMap;

		ProcessCopying(this, &other, actorPointersFields, componentPointersFields, actorsMap, componentsMap, false);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		transform->SetDirty();

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
		mId = id;
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
		mId = Math::Random();

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

		if (mLayer)
		{
			if (mResEnabledInHierarchy)
				mLayer->OnActorDisabled(this);

			mLayer->UnregisterActor(this);
		}

		if (Scene::IsSingletonInitialzed())
			o2Scene.RemoveActorFromScene(this, keepEditorObjects);

		OnRemoveFromScene();

		mLayer = nullptr;
		mSceneStatus = SceneStatus::NotInScene;

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

			if (mLayer)
			{
				mLayer->RegisterActor(this);

				if (mResEnabledInHierarchy)
					mLayer->OnActorEnabled(this);
			}

			o2Scene.AddActorToScene(this);
		}

		mSceneStatus = SceneStatus::InScene;

		for (auto child : mChildren)
			child->AddToScene();
	}

	bool Actor::IsOnScene() const
	{
		return mSceneStatus != SceneStatus::NotInScene;
	}

	bool Actor::IsHieararchyOnScene() const
	{
		if (mParent)
			return IsOnScene() || mParent->IsOnScene() || mParent->IsHieararchyOnScene();

		return IsOnScene();
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
			if (IsHieararchyOnScene())
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

		if (mParent && mParent->mSceneStatus != mSceneStatus)
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

	Vector<Actor*> Actor::AddChildren(Vector<Actor*> actors)
	{
		for (auto actor : actors)
			AddChild(actor);

		return actors;
	}

	Actor* Actor::GetChild(const String& path) const
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

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

	Vector<Actor*> Actor::GetChildren() const
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

	String Actor::GetLayerName() const
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
		for (auto comp : mComponents)
			comp->OnTransformChanged();
	}

	void Actor::SetParentProp(Actor* actor)
	{
		SetParent(actor, false);
	}

	void Actor::OnAddToScene()
	{
		mSceneStatus = SceneStatus::InScene;

		if (mLayer)
		{
			mLayer->RegisterActor(this);

			if (mResEnabledInHierarchy && IsOnScene())
				mLayer->OnActorEnabled(this);
		}

		for (auto comp : mComponents)
			comp->OnAddToScene();
	}

	void Actor::OnRemoveFromScene()
	{
		if (mLayer)
		{
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
		if (mSceneStatus == SceneStatus::InScene)
		{
			if (Scene::IsSingletonInitialzed())
				o2Scene.OnComponentAdded(component);

			component->OnAddToScene();
		}

		for (auto comp : mComponents)
			comp->OnComponentAdded(component);
	}

	void Actor::OnComponentRemoving(Component* component)
	{
		if (IsOnScene())
		{
			if (Scene::IsSingletonInitialzed())
				o2Scene.OnComponentRemoved(component);

			component->OnRemoveFromScene();
		}

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
			if (mLayer && mSceneStatus == SceneStatus::InScene)
			{
				if (mResEnabledInHierarchy)
					mLayer->OnActorEnabled(this);
				else
					mLayer->OnActorDisabled(this);
			}

			if constexpr (IS_EDITOR)
			{
				if (IsHieararchyOnScene())
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

	void Actor::OnSerialize(DataValue& node) const
	{
		if (mPrototypeLink)
			SerializeWithProto(node);
		else
			SerializeRaw(node);
	}

	void Actor::OnDeserialized(const DataValue& node)
	{
		if (node.FindMember("PrototypeLink") || node.FindMember("Prototype"))
			DeserializeWithProto(node);
		else
			DeserializeRaw(node);
	}

	void Actor::SerializeRaw(DataValue& node) const
	{
		node["Id"] = mId;
		node["Name"] = mName;

		if (!mEnabled)
			node["Enabled"] = mEnabled;

		if (mLocked)
			node["Locked"] = mLocked;

		node["Transform"].Set(*transform);
		node["LayerName"] = mLayerName;

		if (!mChildren.IsEmpty())
		{
			auto& childsNode = node.AddMember("Children");
			for (auto child : mChildren)
			{
				auto& childNode = childsNode.AddElement();
				childNode.AddMember("Type") = child->GetType().GetName();
				child->Serialize(childNode.AddMember("Data"));
			}
		}

		if (!mComponents.IsEmpty())
		{
			auto& componentsNode = node.AddMember("Components");
			for (auto component : mComponents)
			{
				auto& componentNode = componentsNode.AddElement();
				component->Serialize(componentNode.AddMember("Data"));
				componentNode.AddMember("Type") = component->GetType().GetName();
			}
		}
	}

	void Actor::DeserializeRaw(const DataValue& node)
	{
		ActorDataValueConverter::Instance().LockPointersResolving();
		if (ActorDataValueConverter::Instance().mLockDepth == 0)
			ActorDataValueConverter::Instance().ActorCreated(this);

		mId = node.GetMember("Id");
		mName = node.GetMember("Name");

		if (auto lockedNode = node.FindMember("Locked"))
			mLocked = *lockedNode;

		if (auto lockedNode = node.FindMember("Enabled"))
			mEnabled = *lockedNode;

		String layerName;
		if (auto layerNode = node.FindMember("LayerName"))
			layerNode->Get(layerName);
		else
			layerName = o2Scene.GetDefaultLayer()->GetName();

		node.GetMember("Transform").Get(*transform);

		RemoveAllComponents();

		if (auto componentsNode = node.FindMember("Components"))
		{
			if (componentsNode->IsArray())
			{
				for (auto& componentNode : *componentsNode)
				{
					Component* component = (Component*)o2Reflection.CreateTypeSample(componentNode.GetMember("Type"));
					component->Deserialize(componentNode.GetMember("Data"));

					AddComponent(component);
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

		ActorDataValueConverter::Instance().UnlockPointersResolving();
		ActorDataValueConverter::Instance().ResolvePointers();

		SetLayer(layerName);
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

	void Actor::CopyData(const Actor& otherActor)
	{
		mName = otherActor.mName;
		mEnabled = otherActor.mEnabled;
		mResEnabled = mEnabled;
		mResEnabledInHierarchy = mEnabled;
		transform->CopyFrom(*otherActor.transform);
		mAssetId = otherActor.mAssetId;
	}

	void Actor::ProcessCopying(Actor* dest, const Actor* source, Vector<Actor**>& actorsPointers,
							   Vector<Component**>& componentsPointers, Map<const Actor*, Actor*>& actorsMap,
							   Map<const Component*, Component*>& componentsMap,
							   bool isSourcePrototype)
	{
		if (!dest->mPrototype && source->mPrototype)
		{
			dest->mPrototype = source->mPrototype;

			ActorAssetRef proto = source->mPrototype;
			while (proto)
			{
				o2Scene.OnActorLinkedToPrototype(proto, dest);
				proto = proto->GetActor()->GetPrototype();
			}
		}

		if (dest->mParent && dest->mParent->mPrototypeLink)
		{
			if (isSourcePrototype)
				dest->mPrototypeLink = const_cast<Actor*>(source);
			else
				dest->mPrototypeLink = source->mPrototypeLink;
		}

		actorsMap.Add(source, dest);

		for (auto child : source->mChildren)
		{
			const ObjectType* type = dynamic_cast<const ObjectType*>(&child->GetType());

			Actor* newChild = dynamic_cast<Actor*>(type->DynamicCastToIObject(child->GetType().CreateSample()));
			if (!dest->IsOnScene())
				newChild->RemoveFromScene();

			dest->AddChild(newChild);

			ProcessCopying(newChild, child, actorsPointers, componentsPointers, actorsMap, componentsMap, isSourcePrototype);
		}

		for (auto component : source->mComponents)
		{
			Component* newComponent = dest->AddComponent(component->CloneAs<Component>());

			componentsMap.Add(component, newComponent);

			if (dest->mPrototypeLink)
			{
				if (isSourcePrototype)
					newComponent->mPrototypeLink = component;
				else
					newComponent->mPrototypeLink = component->mPrototypeLink;
			}

			CollectFixingFields(newComponent, componentsPointers, actorsPointers);
		}

		dest->CopyData(*source);
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

	String Actor::GetCreateMenuCategory()
	{
		return "";
	}

	String Actor::GetCreateMenuGroup()
	{
		return "";
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

ENUM_META(o2::Actor::SceneStatus)
{
	ENUM_ENTRY(InScene);
	ENUM_ENTRY(NotInScene);
	ENUM_ENTRY(WaitingAddToScene);
}
END_ENUM_META;

DECLARE_CLASS(o2::Actor);
