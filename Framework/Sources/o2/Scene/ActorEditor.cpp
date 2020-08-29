#include "o2/stdafx.h"
#include "Actor.h"

#include "o2/Scene/ActorDataValueConverter.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Scene.h"

#if IS_EDITOR

namespace o2
{
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

		if (mName != proto->mName)
			node["Name"] = mName;

		if (mEnabled != proto->mEnabled)
			node["Enabled"] = mEnabled;

		if (mLocked != proto->mLocked)
			node["Locked"] = mLocked;

		if (mLayer != proto->mLayer)
			node["LayerName"] = mLayer->mName;

		// Transform data
		auto transformNode = node.AddMember("Transform");

		if (transform->mData->position != proto->transform->mData->position)
			transformNode["Position"] = transform->mData->position;

		if (transform->mData->size != proto->transform->mData->size)
			transformNode["Size"] = transform->mData->size;

		if (transform->mData->scale != proto->transform->mData->scale)
			transformNode["Scale"] = transform->mData->scale;

		if (transform->mData->pivot != proto->transform->mData->pivot)
			transformNode["Pivot"] = transform->mData->pivot;

		if (!Math::Equals(transform->mData->angle, proto->transform->mData->angle))
			transformNode["Angle"] = transform->mData->angle;

		if (!Math::Equals(transform->mData->shear, proto->transform->mData->shear))
			transformNode["Shear"] = transform->mData->shear;

		// Children data
		auto childsNode = node.AddMember("Childs");
		for (auto child : mChildren)
		{
			auto childNode = childsNode.AddMember("Child");
			childNode.AddMember("Type") = child->GetType().GetName();
			child->Serialize(childNode.AddMember("Data"));
		}

		// Components data
		auto componentsNode = node.AddMember("Components");
		for (auto component : mComponents)
		{
			auto compNode = componentsNode.AddMember("Component");
			compNode.AddMember("Type") = component->GetType().GetName();

			auto& dataValue = compNode.AddMember("Data");
			if (auto componentProtoLink = component->mPrototypeLink)
			{
				dataValue["PrototypeLink"] = componentProtoLink->mId;

				dataValue.SetValueDelta(*component, *component->mPrototypeLink);
			}
			else 
				component->Serialize(dataValue);
		}
	}

	void Actor::DeserializeWithProto(const DataValue& node)
	{
		ActorDataValueConverter::Instance().LockPointersResolving();
		ActorDataValueConverter::Instance().ActorCreated(this);

		RemoveAllChildren();
		RemoveAllComponents();

		if (auto prototypeNode = node.FindMember("Prototype"))
		{
			SetPrototype(*prototypeNode);
		}

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
						mPrototypeLink = fnd;
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

		mId = node.GetMember("Id");

		if (!mPrototypeLink)
			return;

		const Actor* proto = mPrototypeLink.Get();

		if (auto subNode = node.FindMember("Name"))
			mName = *subNode;
		else
			mName = proto->mName;

		if (auto subNode = node.FindMember("Enabled"))
			mEnabled = *subNode;
		else
			mEnabled = proto->mEnabled;

		if (auto subNode = node.FindMember("Locked"))
			mLocked = *subNode;
		else
			mLocked = proto->mLocked;

		if (auto subNode = node.FindMember("LayerName"))
			SetLayer(o2Scene.GetLayer(*subNode));
		else
			SetLayer(proto->mLayer);

		// Transform data
		if (auto transformNode = node.FindMember("Transform"))
		{
			if (auto subNode = transformNode->FindMember("Position"))
				transform->mData->position = *subNode;
			else
				transform->mData->position = proto->transform->mData->position;

			if (auto subNode = transformNode->FindMember("Size"))
				transform->mData->size = *subNode;
			else
				transform->mData->size = proto->transform->mData->size;

			if (auto subNode = transformNode->FindMember("Scale"))
				transform->mData->scale = *subNode;
			else
				transform->mData->scale = proto->transform->mData->scale;

			if (auto subNode = transformNode->FindMember("Pivot"))
				transform->mData->pivot = *subNode;
			else
				transform->mData->pivot = proto->transform->mData->pivot;

			if (auto subNode = transformNode->FindMember("Angle"))
				transform->mData->angle = *subNode;
			else
				transform->mData->angle = proto->transform->mData->angle;

			if (auto subNode = transformNode->FindMember("Shear"))
				transform->mData->shear = *subNode;
			else
				transform->mData->shear = proto->transform->mData->shear;
		}

		RemoveAllChildren();

		// children
		if (auto childsNode = node.FindMember("Childs"))
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

		RemoveAllComponents();

		// components
		if (auto componentsNode = node.FindMember("Components"))
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
							Actor* x = mPrototypeLink.Get();
							while (x)
							{
								bool found = false;

								for (auto protoLinkComponent : x->mComponents)
								{
									if (protoLinkComponent->mId == id)
									{
										newComponent->mPrototypeLink = protoLinkComponent;
										found = true;
										break;
									}
								}

								if (found)
									break;

								x = x->mPrototypeLink.Get();
							}
						}

						if (!newComponent->mPrototypeLink)
							newComponent->Deserialize(componentDataValue);
						else
							componentDataValue.GetValueDelta(*newComponent, *newComponent->mPrototypeLink);
					}
				}
				else o2Debug.LogError("Can't create component with type:" + type);
			}
		}

		transform->SetDirty();

		ActorDataValueConverter::Instance().UnlockPointersResolving();
		ActorDataValueConverter::Instance().ResolvePointers();
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

	void Actor::BreakPrototypeLink()
	{
		if (!mPrototype && !mPrototypeLink)
			return;

		SetPrototype(ActorAssetRef());

		// remove from cache

		for (auto child : mChildren)
			child->BreakPrototypeLink();

		for (auto component : mComponents)
			component->mPrototypeLink = nullptr;

		OnChanged();
	}

	void Actor::RevertToPrototype()
	{
		if (!mPrototypeLink)
			return;

		Vector<Actor*> separatedActors;
		SeparateActors(separatedActors);

		for (auto it = separatedActors.Begin(); it != separatedActors.End();)
		{
			if ((*it)->mPrototypeLink)
				++it;
			else
			{
				delete *it;
				it = separatedActors.Remove(it);
			}
		}

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Map<const Actor*, Actor*> actorsMap;
		Map<const Component*, Component*> componentsMap;
		Vector<ISerializable*> serializableObjects;

		ProcessReverting(this, mPrototypeLink.Get(), separatedActors, actorPointersFields, componentPointersFields, actorsMap,
						 componentsMap, serializableObjects);

		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

// 		for (auto serializable : serializableObjects)
// 			serializable->OnDeserialized();

		UpdateResEnabledInHierarchy();
		transform->SetDirty();

		OnChanged();
	}

	ActorAssetRef Actor::MakePrototype()
	{
		ActorAssetRef prototypeAsset = ActorAssetRef::CreateAsset();
		Actor* prototype = prototypeAsset->GetActor();
		prototype->RemoveFromScene();

		prototype->RemoveAllChildren();
		prototype->RemoveAllComponents();

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Map<const Actor*, Actor*> actorsMap;
		Map<const Component*, Component*> componentsMap;

		ProcessPrototypeMaking(prototype, this, actorPointersFields, componentPointersFields, actorsMap, componentsMap, false);
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		SetPrototype(prototypeAsset);

		prototype->UpdateResEnabledInHierarchy();
		prototype->transform->SetDirty();
		prototype->OnChanged();

		return prototypeAsset;
	}

	void Actor::SetPrototype(ActorAssetRef asset)
	{
		o2Scene.OnActorPrototypeBroken(this);

		auto linkAsset = asset;
		while (linkAsset)
		{
			o2Scene.OnActorLinkedToPrototype(linkAsset, this);
			linkAsset = linkAsset->GetActor()->GetPrototype();
		}

		mPrototype = asset;
		if (asset)
			mPrototypeLink = asset->GetActor();
		else
			mPrototypeLink = nullptr;
	}

	ActorRef Actor::GetPrototypeLink() const
	{
		return mPrototypeLink;
	}

	void Actor::SeparateActors(Vector<Actor*>& separatedActors)
	{
		for (auto child : mChildren)
		{
			child->mParent = nullptr;
			separatedActors.Add(child);

			child->SeparateActors(separatedActors);
		}

		mChildren.Clear();
	}

	void Actor::GetAllChildrenActors(Vector<Actor*>& actors)
	{
		actors.Add(mChildren);

		for (auto child : mChildren)
			child->GetAllChildrenActors(actors);
	}

	void Actor::ApplyChangesToPrototype()
	{
		if (!mPrototype)
			return;

		Actor* prototypeActor = mPrototype->GetActor();

		// collecting data
		Vector<Actor*> linkedProtoActors = o2Scene.mPrototypeLinksCache[mPrototype];
		linkedProtoActors.Remove(this);

		Vector<ApplyActorInfo> applyActorsInfos;
		applyActorsInfos.Reserve(linkedProtoActors.Count());

		for (auto linkedActor : linkedProtoActors)
		{
			applyActorsInfos.Add(ApplyActorInfo());
			applyActorsInfos.Last().actor = linkedActor;
			applyActorsInfos.Last().allChildren.Add(linkedActor);
			linkedActor->GetAllChildrenActors(applyActorsInfos.Last().allChildren);
		}

		Vector<Actor*> allProtoChildren, allThisChildren;
		allProtoChildren.Add(prototypeActor);
		prototypeActor->GetAllChildrenActors(allProtoChildren);

		allThisChildren.Add(this);
		GetAllChildrenActors(allThisChildren);

		Vector<ISerializable*> serializableObjects;

		// check removed actors
		for (int i = allProtoChildren.Count() - 1; i >= 0; i--)
		{
			Actor* itActor = allProtoChildren[i];
			bool removed = allThisChildren.FindOrDefault([&](Actor* x) { return x->IsLinkedToActor(itActor); }) == nullptr;
			if (removed)
			{
				for (auto& info : applyActorsInfos)
				{
					for (int j = info.allChildren.Count() - 1; j >= 0; j--)
					{
						Actor* childItActor = info.allChildren[j];

						if (childItActor->IsLinkedToActor(itActor))
						{
							delete childItActor;
							info.allChildren.RemoveAt(j);
						}
					}
				}

				delete itActor;
				allProtoChildren.RemoveAt(i);
			}
		}

		Vector<Actor**> actorPointersFields;
		Vector<Component**> componentPointersFields;
		Map<const Actor*, Actor*> actorsMap;
		Map<const Component*, Component*> componentsMap;

		// check new and modified actors
		for (auto child : allThisChildren)
		{
			Actor* protoChild = child->mPrototypeLink.Get();

			while (protoChild && !allProtoChildren.Contains(protoChild) && protoChild->mPrototypeLink)
				protoChild = protoChild->mPrototypeLink.Get();

			if (!allProtoChildren.Contains(protoChild))
				protoChild = nullptr;

			// Modified actor
			if (protoChild)
			{
				actorsMap.Add(child, protoChild);

				for (auto& info : applyActorsInfos)
				{
					info.matchingChild = info.allChildren.FindOrDefault([&](Actor* x) { return x->IsLinkedToActor(protoChild); });

					if (info.matchingChild)
					{
						info.actorsMap.Add(child, info.matchingChild);
						CopyActorChangedFields(protoChild, child, info.matchingChild, info.allChildren, info.actor != info.matchingChild);
					}
				}

				if (child->mParent && child->mParent->mPrototypeLink)
				{
					Actor* newParent = allProtoChildren.FindOrDefault([&](Actor* x) { return child->mParent->IsLinkedToActor(x); });
					protoChild->SetParent(newParent);
				}

				protoChild->mName = child->mName;
				protoChild->mEnabled = child->mEnabled;
				*protoChild->transform = *child->transform;
				protoChild->mAssetId = child->mAssetId;
				protoChild->SetLayer(child->mLayer);

				// check new/removed components 
				// removing
				auto childProtoLinkComponents = protoChild->mComponents;
				for (auto protoComponent : childProtoLinkComponents)
				{
					bool removed = child->mComponents.FindOrDefault([&](Component* x) {
						return x->IsLinkedToComponent(protoComponent); }) == nullptr;

					if (removed)
					{
						for (auto& info : applyActorsInfos)
						{
							if (!info.matchingChild)
								continue;

							Component* matchingComponent = info.matchingChild->mComponents.FindOrDefault([&](Component* x) {
								return x->IsLinkedToComponent(protoComponent); });

							if (matchingComponent)
								delete matchingComponent;
						}

						delete protoComponent;
					}
				}

				// new or changed
				for (auto component : child->mComponents)
				{
					Component* protoComponent = component->GetPrototypeLink();
					if (protoComponent)
					{
						// check differences
						Vector<const FieldInfo*> fields;
						GetComponentFields(protoComponent, fields);

						for (auto& info : applyActorsInfos)
						{
							if (!info.matchingChild)
								continue;

							Component* matchingComponent = info.matchingChild->mComponents.FindOrDefault([&](Component* x) {
								return x->IsLinkedToComponent(protoComponent); });

							if (!matchingComponent)
								continue;

							CopyChangedFields(fields, protoComponent, component, matchingComponent,
											  info.actorPointersFields, info.componentPointersFields,
											  serializableObjects);
						}

						CopyFields(fields, component, protoComponent, actorPointersFields, componentPointersFields,
								   serializableObjects);
					}
					else
					{
						// new component
						Component* newProtoComponent = protoChild->AddComponent(component->CloneAs<Component>());
						componentsMap.Add(component, newProtoComponent);

						component->mPrototypeLink = newProtoComponent;

						CollectFixingFields(newProtoComponent, componentPointersFields, actorPointersFields);

						for (auto& info : applyActorsInfos)
						{
							if (!info.matchingChild)
								continue;

							Component* newComponent = info.matchingChild->AddComponent(component->CloneAs<Component>());
							newComponent->mPrototypeLink = newProtoComponent;
							info.componentsMap.Add(component, newComponent);

							CollectFixingFields(newComponent, info.componentPointersFields, info.actorPointersFields);
						}
					}
				}

				continue;
			}

			// new child
			Actor* newProtoChild = mnew Actor(ActorCreateMode::NotInScene);
			allProtoChildren.Add(newProtoChild);
			actorsMap.Add(child, newProtoChild);

			Actor* childParentProtoLink = child->mParent->mPrototypeLink.Get();
			if (!allProtoChildren.Contains(childParentProtoLink))
				childParentProtoLink = allProtoChildren.FindOrDefault([&](Actor* x) { return x->IsLinkedToActor(protoChild); });

			newProtoChild->SetParent(childParentProtoLink);

			newProtoChild->mName = child->mName;
			newProtoChild->mEnabled = child->mEnabled;
			*newProtoChild->transform = *child->transform;
			newProtoChild->mAssetId = child->mAssetId;
			newProtoChild->SetLayer(child->mLayer);

			if (child->mPrototype)
				newProtoChild->SetPrototype(child->mPrototype);

			bool isChildProtoLinked = child->mPrototypeLink != nullptr;

			if (isChildProtoLinked)
				newProtoChild->mPrototypeLink = child->mPrototypeLink;

			child->mPrototypeLink = newProtoChild;

			// copy components
			for (auto component : child->mComponents)
			{
				Component* newProtoComponent = newProtoChild->AddComponent(component->CloneAs<Component>());
				componentsMap.Add(component, newProtoComponent);

				if (isChildProtoLinked)
					newProtoComponent->mPrototypeLink = component->mPrototypeLink;
				else
					component->mPrototypeLink = newProtoComponent;

				CollectFixingFields(newProtoComponent, componentPointersFields, actorPointersFields);
			}

			for (auto& info : applyActorsInfos)
			{
				Actor* newChild = mnew Actor(info.actor->mIsOnScene ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);
				info.allChildren.Add(newChild);
				info.actorsMap.Add(child, newChild);

				Actor* newChildParent = info.allChildren.FindOrDefault([&](Actor* x) { return x->IsLinkedToActor(childParentProtoLink); });

				newChild->SetParent(newChildParent);

				newChild->mName = child->mName;
				newChild->mEnabled = child->mEnabled;
				*newChild->transform = *child->transform;
				newChild->mAssetId = child->mAssetId;
				newChild->SetLayer(child->mLayer);

				if (child->mPrototype)
					newChild->SetPrototype(child->mPrototype);

				newChild->mPrototypeLink = newProtoChild;

				for (auto component : child->mComponents)
				{
					Component* newComponent = newChild->AddComponent(CloneAs<Component>());
					info.componentsMap.Add(component, newComponent);
					newComponent->mPrototypeLink = component->mPrototypeLink;
					CollectFixingFields(newComponent, info.componentPointersFields, info.actorPointersFields);
				}

				if (newChild->mIsOnScene)
					newChild->SetLayer(child->mLayer);
			}
		}

		// fix actors and components fields
		FixComponentFieldsPointers(actorPointersFields, componentPointersFields, actorsMap, componentsMap);

		for (auto& info : applyActorsInfos)
		{
			FixComponentFieldsPointers(info.actorPointersFields, info.componentPointersFields, info.actorsMap,
									   info.componentsMap);

			info.actor->OnChanged();
		}

// 		for (auto serializable : serializableObjects)
// 			serializable->OnDeserialized(DataValue());

		// update transformation
		transform->SetDirty();

		for (auto& info : applyActorsInfos)
			info.actor->transform->UpdateTransform();

		OnChanged();

		mPrototype->Save();
	}

	bool Actor::IsLinkedToActor(Actor* actor) const
	{
		if (mPrototypeLink)
		{
			auto t = mPrototypeLink.Get();
			while (t)
			{
				if (t == actor)
					return true;

				t = t->mPrototypeLink.Get();
			}
		}

		return false;
	}

	bool Actor::IsLinkedToActor(SceneUID id) const
	{
		if (mPrototypeLink)
		{
			auto t = mPrototypeLink.Get();
			while (t)
			{
				if (t->mId == id)
					return true;

				t = t->mPrototypeLink.Get();
			}
		}

		return false;
	}

	Actor* Actor::FindLinkedActor(Actor* linkActor)
	{
		if (GetPrototypeLink() == linkActor)
			return this;

		for (auto child : mChildren)
		{
			if (auto res = child->FindLinkedActor(linkActor))
				return res;
		}

		return nullptr;
	}

	Actor* Actor::FindLinkedActor(SceneUID id)
	{
		if (IsLinkedToActor(id))
			return this;

		for (auto child : mChildren)
		{
			if (auto res = child->FindLinkedActor(id))
				return res;
		}

		return nullptr;
	}

	Actor* Actor::FindActorById(SceneUID id)
	{
		if (mId == id)
			return this;

		for (auto child : mChildren)
			if (auto res = child->FindActorById(id))
				return res;

		return nullptr;
	}

	bool Actor::IsSupportsLocking() const
	{
		return true;
	}

	void Actor::SetLocked(bool locked)
	{
		mLocked = locked;
		UpdateLocking();

		OnLockChanged();
	}

	void Actor::Lock()
	{
		SetLocked(true);
	}

	void Actor::Unlock()
	{
		SetLocked(false);
	}

	bool Actor::IsLocked() const
	{
		return mLocked;
	}

	bool Actor::IsLockedInHierarchy() const
	{
		return mResLocked;
	}

	void Actor::UpdateLocking()
	{
		bool lastResLocked = mResLocked;

		if (mParent)
			mResLocked = mLocked || mParent->mResLocked;
		else
			mResLocked = mLocked;

		if (lastResLocked != mResLocked)
			OnLockChanged();

		for (auto child : mChildren)
			child->UpdateLocking();
	}

	bool Actor::IsSupportsTransforming() const
	{
		return true;
	}

	Basis Actor::GetTransform() const
	{
		return transform->GetWorldBasis();
	}

	void Actor::SetTransform(const Basis& transform)
	{
		this->transform->SetWorldBasis(transform);
	}

	bool Actor::IsSupportsPivot() const
	{
		return true;
	}

	void Actor::SetPivot(const Vec2F& pivot)
	{
		transform->SetWorldPivot(pivot);
	}

	Vec2F Actor::GetPivot() const
	{
		return transform->GetWorldPivot();
	}

	void Actor::AddEditableChild(SceneEditableObject* object, int idx /*= -1*/)
	{
		if (Actor* actor = dynamic_cast<Actor*>(object))
		{
			if (idx >= 0)
				AddChild(actor, idx);
			else
				AddChild(actor);
		}
	}

	bool Actor::IsSupportsDisabling() const
	{
		return true;
	}

	Vector<SceneEditableObject*> Actor::GetEditablesChildren() const
	{
		return mChildren.Convert<SceneEditableObject*>([](Actor* x) { return dynamic_cast<SceneEditableObject*>(x); });
	}

	void Actor::SetEditableParent(SceneEditableObject* object)
	{
		SetParent(dynamic_cast<Actor*>(object));
	}

	SceneEditableObject* Actor::GetEditableParent() const
	{
		return dynamic_cast<SceneEditableObject*>(mParent);
	}

	void Actor::OnChanged()
	{
		onChanged();

		if (Scene::IsSingletonInitialzed() && IsHieararchyOnScene())
			o2Scene.OnObjectChanged(this);
	}

	void Actor::OnLockChanged()
	{
		onLockChanged(mLocked);
		onChanged();

		if (Scene::IsSingletonInitialzed() && IsHieararchyOnScene())
		{
			o2Scene.OnObjectChanged(this);
			o2Scene.onLockChanged(this);
		}
	}

	void Actor::OnNameChanged()
	{
		onNameChanged();
		onChanged();

		if (Scene::IsSingletonInitialzed() && IsHieararchyOnScene())
		{
			o2Scene.OnObjectChanged(this);
			o2Scene.onNameChanged(this);
		}
	}

	void Actor::OnChildrenChanged()
	{
		onChildHierarchyChanged();
		onChanged();

		if (Scene::IsSingletonInitialzed() && IsHieararchyOnScene())
		{
			o2Scene.OnObjectChanged(this);
			o2Scene.onChildrenHierarchyChanged(this);
		}
	}

	const String& Actor::GetCreateMenuCategory() const
	{
		return GetType().GetName();
	}

	void Actor::OnParentChanged(Actor* oldParent)
	{
		onParentChanged(oldParent);
		onChanged();

		if (Scene::IsSingletonInitialzed() && IsHieararchyOnScene())
		{
			o2Scene.OnObjectChanged(this);
			o2Scene.OnObjectChanged(oldParent);
			o2Scene.OnObjectChanged(mParent);

			o2Scene.onChildrenHierarchyChanged(oldParent);
			o2Scene.onChildrenHierarchyChanged(mParent);
		}
	}

	void Actor::OnChildAdded(Actor* child)
	{}

	void Actor::OnChildRemoved(Actor* child)
	{}

	void Actor::OnLayerChanged(SceneLayer* oldLayer)
	{}
	
	void Actor::ProcessPrototypeMaking(Actor* dest, Actor* source, Vector<Actor**>& actorsPointers,
										 Vector<Component**>& componentsPointers,
										 Map<const Actor*, Actor*>& actorsMap,
										 Map<const Component*, Component*>& componentsMap,
										 bool isInsidePrototype)
	{
		dest->mName = source->mName;
		dest->mEnabled = source->mEnabled;
		*dest->transform = *source->transform;
		dest->mAssetId = source->mAssetId;

		if (!isInsidePrototype && !source->mPrototype && source->mPrototypeLink)
			source->mPrototypeLink = nullptr;

		dest->SetPrototype(source->mPrototype);

		dest->mPrototypeLink = source->mPrototypeLink;
		source->mPrototypeLink = dest;

		actorsMap.Add(source, dest);

		for (auto child : source->mChildren)
		{
			Actor* newChild = mnew Actor(dest->mIsOnScene ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);
			dest->AddChild(newChild);

			ProcessPrototypeMaking(newChild, child, actorsPointers, componentsPointers, actorsMap, componentsMap,
								   source->mPrototype.IsValid() || isInsidePrototype);
		}

		for (auto component : source->mComponents)
		{
			Component* newComponent = dest->AddComponent(CloneAs<Component>());
			componentsMap.Add(component, newComponent);
			newComponent->mPrototypeLink = component->mPrototypeLink;
			component->mPrototypeLink = newComponent;
			CollectFixingFields(newComponent, componentsPointers, actorsPointers);
		}

		dest->SetLayer(source->mLayer);
	}

	void Actor::ProcessReverting(Actor* dest, const Actor* source, const Vector<Actor*>& separatedActors,
								 Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers,
								 Map<const Actor*, Actor*>& actorsMap,
								 Map<const Component*, Component*>& componentsMap,
								 Vector<ISerializable*>& serializableObjects)
	{
		dest->mName = source->mName;
		dest->mEnabled = source->mEnabled;
		*dest->transform = *source->transform;
		dest->mAssetId = source->mAssetId;

		actorsMap.Add(source, dest);

		for (auto child : source->mChildren)
		{
			Actor* newChild = nullptr;

			newChild = separatedActors.FindOrDefault([&](Actor* x) { return x->GetPrototypeLink() == child; });

			if (!newChild)
				newChild = mnew Actor(dest->mIsOnScene ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);

			dest->AddChild(newChild);

			ProcessReverting(newChild, child, separatedActors, actorsPointers, componentsPointers, actorsMap,
							 componentsMap, serializableObjects);
		}

		for (auto it = dest->mComponents.Begin(); it != dest->mComponents.End();)
		{
			if (!(*it)->mPrototypeLink)
			{
				(*it)->mOwner = nullptr;
				delete *it;
				it = dest->mComponents.Remove(it);
			}
			else ++it;
		}

		for (auto component : source->mComponents)
		{
			Component* matchingComponent = dest->mComponents.FindOrDefault([&](Component* x) { return x->GetPrototypeLink() == component; });
			if (matchingComponent)
			{
				Vector<const FieldInfo*> fields;
				GetComponentFields(matchingComponent, fields);

				CopyFields(fields, component, matchingComponent, actorsPointers, componentsPointers,
						   serializableObjects);

				continue;
			}

			Component* newComponent = dest->AddComponent(CloneAs<Component>());
			componentsMap.Add(component, newComponent);

			CollectFixingFields(newComponent, componentsPointers, actorsPointers);
		}

		dest->SetLayer(source->mLayer);
	}

	void Actor::CopyChangedFields(Vector<const FieldInfo*>& fields, IObject* source, IObject* changed, IObject* dest, 
								  Vector<Actor**>& actorsPointers, Vector<Component**>& componentsPointers, 
								  Vector<ISerializable*>& serializableObjects)
	{
		for (auto field : fields)
		{
			if (!field->HasAttribute<SerializableAttribute>())
				continue;

			if (*field->GetType() == TypeOf(Actor*))
			{
				Actor* changedValue = field->GetValue<Actor*>(changed);
				Actor* sourceValue = field->GetValue<Actor*>(source);
				Actor** destValuePtr = (Actor**)(field->GetValuePtrStrong(dest));

				bool valueChanged = false;
				if (changedValue != nullptr)
					valueChanged = changedValue->GetPrototypeLink() != sourceValue;
				else
					valueChanged = sourceValue != nullptr;

				bool destValueSameToSource = false;
				if (*destValuePtr != nullptr)
					destValueSameToSource = (*destValuePtr)->GetPrototypeLink() == sourceValue;
				else
					destValueSameToSource = sourceValue == nullptr;

				if (valueChanged && destValueSameToSource)
				{
					*destValuePtr = changedValue;
					actorsPointers.Add(destValuePtr);
				}
			}
			else if (*field->GetType() == TypeOf(Component*))
			{
				Component* changedValue = field->GetValue<Component*>(changed);
				Component* sourceValue = field->GetValue<Component*>(source);
				Component** destValuePtr = (Component**)(field->GetValuePtrStrong(dest));

				bool valueChanged = false;
				if (changedValue != nullptr)
					valueChanged = changedValue->GetPrototypeLink() != sourceValue;
				else
					valueChanged = sourceValue != nullptr;

				bool destValueSameToSource = false;
				if (*destValuePtr != nullptr)
					destValueSameToSource = (*destValuePtr)->GetPrototypeLink() == sourceValue;
				else
					destValueSameToSource = sourceValue == nullptr;

				if (valueChanged && destValueSameToSource)
				{
					*destValuePtr = changedValue;
					componentsPointers.Add(destValuePtr);
				}
			}
			else if (field->GetType()->IsBasedOn(TypeOf(AssetRef)))
			{
				if (!field->IsValueEquals(changed, source) && field->IsValueEquals(source, dest))
					field->CopyValue(dest, changed);
			}
			else if (field->GetType()->IsBasedOn(TypeOf(IObject)))
			{
				if (field->GetType()->IsBasedOn(TypeOf(ISerializable)))
					serializableObjects.Add((ISerializable*)field->GetValuePtr(dest));

				auto fields = field->GetType()->GetFieldsWithBaseClasses();
				CopyChangedFields(fields, (IObject*)field->GetValuePtr(source),
								  (IObject*)field->GetValuePtr(changed),
								  (IObject*)field->GetValuePtr(dest),
								  actorsPointers, componentsPointers, serializableObjects);
			}
			else
			{
				if (!field->IsValueEquals(changed, source) && field->IsValueEquals(source, dest))
					field->CopyValue(dest, changed);
			}
		}
	}

	void Actor::CopyActorChangedFields(Actor* source, Actor* changed, Actor* dest, Vector<Actor*>& allDestChilds,
									   bool withTransform)
	{
		if (changed->mParent && changed->mParent->mPrototypeLink)
		{
			if (!changed->mParent->IsLinkedToActor(source->mParent) && dest->mParent &&
				dest->mParent->IsLinkedToActor(source->mParent))
			{
				Actor* newParent = allDestChilds.FindOrDefault([&](Actor* x) { return x->IsLinkedToActor(changed->mParent->mPrototypeLink.Get()); });
				dest->SetParent(newParent);
			}
		}

		if (source->mName != changed->mName && dest->mName == source->mName)
			dest->mName = changed->mName;

		if (source->mEnabled != changed->mEnabled && dest->mEnabled == source->mEnabled)
			dest->mEnabled = changed->mEnabled;

		if (source->mLocked != changed->mLocked && dest->mLocked == source->mLocked)
			dest->mLocked = changed->mLocked;

		if (source->mLayer != changed->mLayer && dest->mLayer == source->mLayer)
			dest->SetLayer(changed->mLayer);

		// transform
		if (withTransform)
		{
			if (source->transform->mData->position != changed->transform->mData->position &&
				dest->transform->mData->position == source->transform->mData->position)
			{
				dest->transform->mData->position = changed->transform->mData->position;
			}

			if (source->transform->mData->scale != changed->transform->mData->scale &&
				dest->transform->mData->scale == source->transform->mData->scale)
			{
				dest->transform->mData->scale = changed->transform->mData->scale;
			}

			if (source->transform->mData->size != changed->transform->mData->size &&
				dest->transform->mData->size == source->transform->mData->size)
			{
				dest->transform->mData->size = changed->transform->mData->size;
			}

			if (source->transform->mData->pivot != changed->transform->mData->pivot &&
				dest->transform->mData->pivot == source->transform->mData->pivot)
			{
				dest->transform->mData->pivot = changed->transform->mData->pivot;
			}

			if (!Math::Equals(source->transform->mData->angle, changed->transform->mData->angle) &&
				Math::Equals(dest->transform->mData->angle, source->transform->mData->angle))
			{
				dest->transform->mData->angle = changed->transform->mData->angle;
			}

			if (!Math::Equals(source->transform->mData->shear, changed->transform->mData->shear) &&
				Math::Equals(dest->transform->mData->shear, source->transform->mData->shear))
			{
				dest->transform->mData->shear = changed->transform->mData->shear;
			}
		}
	}
}

#endif //IS_EDITOR
