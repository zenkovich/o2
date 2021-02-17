#include "o2/stdafx.h"
#include "Actor.h"

#include "o2/Scene/ActorRefResolver.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Scene.h"

#if IS_EDITOR

namespace o2
{
	void Actor::MakePrototypeCloneVisitor::OnCopyActor(const Actor* source, Actor* target)
	{
		SourceToTargetMapCloneVisitor::OnCopyActor(source, target);
		const_cast<Actor*>(source)->mPrototypeLink.CopyWithoutRemap(target);
	}

	void Actor::MakePrototypeCloneVisitor::OnCopyComponent(const Component* source, Component* target)
	{
		SourceToTargetMapCloneVisitor::OnCopyComponent(source, target);
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
		mCopyVisitor = mnew MakePrototypeCloneVisitor();
		auto prototype = CloneAs<Actor>();

		ActorAssetRef prototypeAsset = ActorAssetRef::CreateAsset(prototype);
		SetPrototype(prototypeAsset);

		prototype->UpdateResEnabledInHierarchy();
		prototype->transform->SetDirty();
		prototype->OnChanged();

		return prototypeAsset;
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
				protoChild->SetLayer(child->mLayerName);

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
			newProtoChild->SetLayer(child->mLayerName);

			if (child->mPrototype)
				newProtoChild->SetPrototype(child->mPrototype);

			bool isChildProtoLinked = child->mPrototypeLink != nullptr;

			if (isChildProtoLinked)
				newProtoChild->mPrototypeLink.CopyWithoutRemap(child->mPrototypeLink);

			child->mPrototypeLink.CopyWithoutRemap(newProtoChild);

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
				Actor* newChild = mnew Actor(info.actor->IsOnScene() ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);
				info.allChildren.Add(newChild);
				info.actorsMap.Add(child, newChild);

				Actor* newChildParent = info.allChildren.FindOrDefault([&](Actor* x) { return x->IsLinkedToActor(childParentProtoLink); });

				newChild->SetParent(newChildParent);

				newChild->mName = child->mName;
				newChild->mEnabled = child->mEnabled;
				*newChild->transform = *child->transform;
				newChild->mAssetId = child->mAssetId;
				newChild->SetLayer(child->mLayerName);

				if (child->mPrototype)
					newChild->SetPrototype(child->mPrototype);

				newChild->mPrototypeLink.CopyWithoutRemap(newProtoChild);

				for (auto component : child->mComponents)
				{
					Component* newComponent = newChild->AddComponent(CloneAs<Component>());
					info.componentsMap.Add(component, newComponent);
					newComponent->mPrototypeLink = component->mPrototypeLink;
					CollectFixingFields(newComponent, info.componentPointersFields, info.actorPointersFields);
				}

				if (newChild->IsOnScene())
					newChild->SetLayer(child->mLayerName);
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
		return mChildren.DynamicCast<SceneEditableObject*>();
	}

	void Actor::SetEditableParent(SceneEditableObject* object)
	{
		SetParent(dynamic_cast<Actor*>(object), false);
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

	void Actor::OnParentChanged(Actor* oldParent)
	{
		for (auto comp : mComponents)
			comp->OnParentChanged(oldParent);

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
				newChild = mnew Actor(dest->IsOnScene() ? ActorCreateMode::InScene : ActorCreateMode::NotInScene);

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

		dest->SetLayer(source->mLayerName);
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
			dest->SetLayer(changed->mLayerName);

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
