#include "o2/stdafx.h"
#include "Actor.h"

#include "o2/Scene/ActorRefResolver.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Scene.h"
#include "o2/Utils/Serialization/Serializable.h"

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

		// Get difference between this actor and prototype first
		auto& diffs = Differences::GetDifference(this, mPrototype->GetActor());

		// Get applying actors infos
		ApplyActorInfo prototypeApplyInfo(mPrototype->GetActor());
		ApplyActorInfo thisApplyInfo(this);
		Vector<ApplyActorInfo> linkedActorsApplyInfos;

		auto& linkedActors = o2Scene.mPrototypeLinksCache[mPrototype];
		linkedActorsApplyInfos.Reserve(linkedActors.Count());
		linkedActors.ForEach([&](auto x) { if (x != this) linkedActorsApplyInfos.Add(ApplyActorInfo(x)); });

		// Apply new children first, because we need that new children and components in this actor must be linked to prototype
		// Then apply other differences to prototype, except removing children and components. They are required to
		// apply to other actors
		diffs.newChildren.ForEach([&](auto d) { d->Apply(prototypeApplyInfo, true); });
		diffs.newComponents.ForEach([&](auto d) { d->Apply(prototypeApplyInfo, true); });
		diffs.changedActorFields.ForEach([&](auto d) { d->Apply(thisApplyInfo, true); });     // Apply changes with this apply info. It is required
		diffs.changedComponentFields.ForEach([&](auto d) { d->Apply(thisApplyInfo, true); }); // to find original changed entities and apply to linked

		// Then apply differences to other actors. Apply removing components before children to avoid searching components 
		// by deleted children
		diffs.newChildren.ForEach([&](auto d) { linkedActorsApplyInfos.ForEach([&](ApplyActorInfo& info) { d->Apply(info, false); }); });
		diffs.newComponents.ForEach([&](auto d) { linkedActorsApplyInfos.ForEach([&](ApplyActorInfo& info) { d->Apply(info, false); }); });
		diffs.changedActorFields.ForEach([&](auto d) { linkedActorsApplyInfos.ForEach([&](ApplyActorInfo& info) { d->Apply(info, false); }); });
		diffs.changedComponentFields.ForEach([&](auto d) { linkedActorsApplyInfos.ForEach([&](ApplyActorInfo& info) { d->Apply(info, false); }); });
		diffs.removedComponents.ForEach([&](auto d) { linkedActorsApplyInfos.ForEach([&](ApplyActorInfo& info) { d->Apply(info, false); }); });
		diffs.removedChildren.ForEach([&](auto d) { linkedActorsApplyInfos.ForEach([&](ApplyActorInfo& info) { d->Apply(info, false); }); });

		// And remove children and components from prototype
		diffs.removedComponents.ForEach([&](auto d) { d->Apply(prototypeApplyInfo, true); });
		diffs.removedChildren.ForEach([&](auto d) { d->Apply(prototypeApplyInfo, true); });

		// Save prototype
		mPrototype->GetActor()->UpdateTransform();
		mPrototype->GetActor()->OnChanged();
		mPrototype->Save();

		// Invoke changed callback for actors
		linkedActorsApplyInfos.ForEach([](ApplyActorInfo& info) { info.actor->OnChanged(); info.actor->UpdateTransform(); });

		OnChanged();
	}

	Actor::ApplyActorInfo::ApplyActorInfo()
	{}

	Actor::ApplyActorInfo::ApplyActorInfo(Actor* actor):
		actor(actor)
	{
		allChildren.Add(actor);
		actor->GetAllChildrenActors(allChildren);
	}

	Actor::Differences::Differences()
	{}

	Actor::Differences::Differences(const Differences& other)
	{
		removedChildren = other.removedChildren.Convert<IDifference*>([](auto x) { return x->Clone(); });
		newChildren = other.newChildren.Convert<IDifference*>([](auto x) { return x->Clone(); });
		removedComponents = other.removedComponents.Convert<IDifference*>([](auto x) { return x->Clone(); });
		newComponents = other.newComponents.Convert<IDifference*>([](auto x) { return x->Clone(); });
		changedActorFields = other.changedActorFields.Convert<IDifference*>([](auto x) { return x->Clone(); });
		changedComponentFields = other.changedComponentFields.Convert<IDifference*>([](auto x) { return x->Clone(); });
	}

	Actor::Differences::~Differences()
	{
		removedChildren.ForEach([](auto x) { delete x; });
		newChildren.ForEach([](auto x) { delete x; });
		removedComponents.ForEach([](auto x) { delete x; });
		newComponents.ForEach([](auto x) { delete x; });
		changedActorFields.ForEach([](auto x) { delete x; });
		changedComponentFields.ForEach([](auto x) { delete x; });
	}

	Actor::Differences Actor::Differences::GetDifference(Actor* changedActor, Actor* prototype)
	{
		Differences res;

		// Split prototype children
		Vector<Actor*> allProtoChildren;
		allProtoChildren.Add(prototype);
		prototype->GetAllChildrenActors(allProtoChildren);

		// Split this actor children
		Vector<Actor*> allThisChildren;
		allThisChildren.Add(changedActor);
		changedActor->GetAllChildrenActors(allThisChildren);

		// Check removed and changed from end of hierarchy
		// Because we should check children before parent removes
		for (int i = allProtoChildren.Count() - 1; i >= 0; i--)
		{
			Actor* protoChild = allProtoChildren[i];
			auto thisLinkedChild = allThisChildren.FindOrDefault([&](Actor* x) { return x->IsLinkedToActor(protoChild); });

			// Child was removed
			if (thisLinkedChild == nullptr)
			{
				auto diff = mnew RemovedChild();
				diff->prototypeLink = protoChild;
				res.removedChildren.Add(diff);
				continue;
			}

			// Not removed, checking for changes on actor, removed, created or changed components
			Vector<const FieldInfo*> fieldsStack;

			// Get differences from actor
			auto createActorChangedFieldDiff = [&](const String& fieldPath) {
				auto diff = mnew ChangedActorField();
				diff->prototypeLink = protoChild;
				diff->path = fieldPath;
				res.changedActorFields.Add(diff);
			};

			auto& objectType = dynamic_cast<const ObjectType&>(thisLinkedChild->GetType());
			GetObjectDifferences(createActorChangedFieldDiff, fieldsStack, objectType,
								 objectType.DynamicCastFromIObject(thisLinkedChild),
								 objectType.DynamicCastFromIObject(protoChild));

			thisLinkedChild->GetDifferences(res);

			// Check removed and changed components
			for (auto protoChildComponent : protoChild->GetComponents())
			{
				auto thisLinkedChildComponent = thisLinkedChild->GetComponents()
					.FindOrDefault([&](Component* x) { return x->IsLinkedToComponent(protoChildComponent); });

				// Component was removed
				if (thisLinkedChildComponent == nullptr)
				{
					auto diff = mnew RemovedComponent();
					diff->prototypeLink = protoChildComponent;
					diff->ownerPrototypeLink = protoChild;
					res.removedComponents.Add(diff);
					continue;
				}

				// Check component changes
				auto createComponentChangedFieldDiff = [&](const String& fieldPath) {
					auto diff = mnew ChangedComponentField();
					diff->ownerPrototypeLink = protoChild;
					diff->prototypeLink = protoChildComponent;
					diff->path = fieldPath;
					res.changedComponentFields.Add(diff);
				};

				auto& objectType = dynamic_cast<const ObjectType&>(thisLinkedChildComponent->GetType());
				GetObjectDifferences(createComponentChangedFieldDiff, fieldsStack, objectType,
									 objectType.DynamicCastFromIObject(thisLinkedChildComponent),
									 objectType.DynamicCastFromIObject(protoChildComponent));
			}

			// Check new components
			for (auto thisChildComponent : thisLinkedChild->GetComponents())
			{
				// Not linked, because it is new
				if (thisChildComponent->GetPrototypeLink() == nullptr)
				{
					auto diff = mnew NewComponent();
					diff->ownerPrototypeLink = protoChild;
					diff->newComponent = thisChildComponent;
					res.newComponents.Add(diff);
				}
			}
		}

		// Check new actors
		for (auto child : allThisChildren)
		{
			// Empty prototype link on child, but not empty on parent means that is new actor
			if (child->GetPrototypeLink() == nullptr && child->GetParent() && child->GetParent()->GetPrototypeLink())
			{
				auto diff = mnew NewChild();
				diff->parentPrototypeLink = child->GetParent()->GetPrototypeLink().Get();
				diff->newChild = child;
				res.newChildren.Add(diff);
			}
		}

		return res;
	}

	void Actor::Differences::GetObjectDifferences(const Function<void(const String& path)>& createDiffFunc,
												  Vector<const FieldInfo*>& stack,
												  const Type& objectType, void* changedObject, void* protoObject)
	{
		for (auto& baseType : objectType.GetBaseTypes())
		{
			void* changedObjectBase = baseType.dynamicCastUpFunc(changedObject);
			void* protoObjectBase = baseType.dynamicCastUpFunc(protoObject);
			GetObjectDifferences(createDiffFunc, stack, *baseType.type, changedObjectBase, protoObjectBase);
		}

		for (auto& fieldInfo : objectType.GetFields())
		{
			if (!fieldInfo.HasAttribute<SerializableAttribute>())
				continue;

			stack.Add(&fieldInfo);

			auto* changedFieldObject = fieldInfo.GetValuePtr(changedObject);
			auto* protoFieldObject = fieldInfo.GetValuePtr(protoObject);
			GetFieldDifference(createDiffFunc, stack, *fieldInfo.GetType(), changedFieldObject, protoFieldObject);

			stack.PopBack();
		}
	}

	void Actor::Differences::GetFieldDifference(const Function<void(const String& path)>& createDiffFunc,
												Vector<const FieldInfo*>& stack, const Type& fieldType, 
												void* changedFieldObject, void* protoFieldObject)
	{
		if (fieldType.GetUsage() == Type::Usage::Object)
		{
			if (changedFieldObject && protoFieldObject)
				GetObjectDifferences(createDiffFunc, stack, fieldType, changedFieldObject, protoFieldObject);
			else if (changedFieldObject || protoFieldObject)
				createDiffFunc(GetFieldPath(stack));
		}
		else if (fieldType.GetUsage() == Type::Usage::Pointer)
		{
			auto& pointerType = dynamic_cast<const PointerType&>(fieldType);
			GetFieldDifference(createDiffFunc, stack, *pointerType.GetUnpointedType(), changedFieldObject, protoFieldObject);
		}
		else if (!fieldType.IsValueEquals(changedFieldObject, protoFieldObject))
			createDiffFunc(GetFieldPath(stack));
	}

	String Actor::Differences::GetFieldPath(const Vector<const FieldInfo*>& stack)
	{
		String res;

		for (int i = 0; i < stack.Count(); i++)
		{
			res += stack[i]->GetName();
			if (i != stack.Count() - 1)
				res += "/";
		}

		return res;
	}

	void Actor::Differences::RemovedChild::Apply(ApplyActorInfo& applyInfo, bool applyPrototype) const
	{
		if (applyPrototype)
		{
			o2Scene.DestroyActor(prototypeLink);
			return;
		}

		for (auto child : applyInfo.allChildren)
		{
			if (child->IsLinkedToActor(prototypeLink))
			{
				o2Scene.DestroyActor(child);
				break;
			}
		}
	}

	void Actor::Differences::NewChild::Apply(ApplyActorInfo& applyInfo, bool applyPrototype) const
	{
		if (applyPrototype)
		{
			newChild->mCopyVisitor = mnew MakePrototypeCloneVisitor();
			auto newInstanceChild = newChild->CloneAs<Actor>();
			parentPrototypeLink->AddChild(newInstanceChild);

			return;
		}

		for (auto child : applyInfo.allChildren)
		{
			if (child->IsLinkedToActor(parentPrototypeLink))
			{
				Actor* prototypeLink = newChild->GetPrototypeLink().Get();
				prototypeLink->mCopyVisitor = mnew InstantiatePrototypeVisitor();
				auto newInstanceChild = prototypeLink->CloneAs<Actor>();
				child->AddChild(newInstanceChild);

				break;
			}
		}
	}

	void Actor::Differences::RemovedComponent::Apply(ApplyActorInfo& applyInfo, bool applyPrototype) const
	{
		if (applyPrototype)
		{
			o2Scene.DestroyComponent(prototypeLink);
			return;
		}

		for (auto child : applyInfo.allChildren)
		{
			if (child->IsLinkedToActor(ownerPrototypeLink))
			{
				for (auto comp : child->GetComponents())
				{
					if (comp->IsLinkedToComponent(prototypeLink))
					{
						o2Scene.DestroyComponent(comp);
						break;
					}
				}

				break;
			}
		}
	}

	void Actor::Differences::NewComponent::Apply(ApplyActorInfo& applyInfo, bool applyPrototype) const
	{
		if (applyPrototype)
		{
			auto protoNewComponent = newComponent->CloneAs<Component>();
			newComponent->mPrototypeLink = protoNewComponent;
			ownerPrototypeLink->AddComponent(protoNewComponent);
			return;
		}

		for (auto child : applyInfo.allChildren)
		{
			if (child->IsLinkedToActor(ownerPrototypeLink))
			{
				child->AddComponent(newComponent->GetPrototypeLink()->CloneAs<Component>());
				break;
			}
		}
	}

	void Actor::Differences::ChangedActorField::Apply(ApplyActorInfo& applyInfo, bool applyPrototype) const
	{
		for (auto child : applyInfo.allChildren)
		{
			if (child->IsLinkedToActor(prototypeLink))
			{
				auto& objectType = dynamic_cast<const ObjectType&>(child->GetType());
				const FieldInfo* fieldInfo = nullptr;
				const FieldInfo* protoFieldInfo = nullptr;
				if (auto fieldPtr = objectType.GetFieldPtr(objectType.DynamicCastFromIObject(child), path, fieldInfo))
				{
					if (auto protoFieldPtr = objectType.GetFieldPtr(objectType.DynamicCastFromIObject(prototypeLink), path, protoFieldInfo))
					{
						if (applyPrototype)
							fieldInfo->GetType()->CopyValue(protoFieldPtr, fieldPtr);
						else
							fieldInfo->GetType()->CopyValue(fieldPtr, protoFieldPtr);
					}
				}

				break;
			}
		}
	}

	void Actor::Differences::ChangedComponentField::Apply(ApplyActorInfo& applyInfo, bool applyPrototype) const
	{
		for (auto child : applyInfo.allChildren)
		{
			if (child->IsLinkedToActor(ownerPrototypeLink))
			{
				for (auto comp : child->GetComponents())
				{
					if (comp->IsLinkedToComponent(prototypeLink))
					{
						auto& objectType = dynamic_cast<const ObjectType&>(comp->GetType());
						const FieldInfo* fieldInfo = nullptr;
						const FieldInfo* protoFieldInfo = nullptr;
						if (auto fieldPtr = objectType.GetFieldPtr(objectType.DynamicCastFromIObject(comp), path, fieldInfo))
						{
							if (auto protoFieldPtr = objectType.GetFieldPtr(objectType.DynamicCastFromIObject(prototypeLink), path, protoFieldInfo))
							{
								if (applyPrototype)
									fieldInfo->GetType()->CopyValue(protoFieldPtr, fieldPtr);
								else
									fieldInfo->GetType()->CopyValue(fieldPtr, protoFieldPtr);
							}
						}

						break;
					}
				}

				break;
			}
		}
	}

	Actor::Differences::IDifference* Actor::Differences::RemovedChild::Clone() const
	{
		return mnew RemovedChild(*this);
	}

	Actor::Differences::IDifference* Actor::Differences::NewChild::Clone() const
	{
		return mnew NewChild(*this);
	}

	Actor::Differences::IDifference* Actor::Differences::RemovedComponent::Clone() const
	{
		return mnew RemovedComponent(*this);
	}

	Actor::Differences::IDifference* Actor::Differences::NewComponent::Clone() const
	{
		return mnew NewComponent(*this);
	}

	Actor::Differences::IDifference* Actor::Differences::ChangedActorField::Clone() const
	{
		return mnew ChangedActorField(*this);
	}

	Actor::Differences::IDifference* Actor::Differences::ChangedComponentField::Clone() const
	{
		return mnew ChangedComponentField(*this);
	}
	
	void Actor::GetDifferences(Differences& differences) const
	{
		Vector<const FieldInfo*> stack;
		stack.Add(GetType().GetField("transform"));

		auto createActorChangedFieldDiff = [&](const String& fieldPath) {
			auto diff = mnew Differences::ChangedActorField();
			diff->prototypeLink = const_cast<Actor*>(mPrototypeLink.Get());
			diff->path = fieldPath;
			differences.changedActorFields.Add(diff);
		};

		Differences::GetObjectDifferences(createActorChangedFieldDiff, stack, transform->GetType(), transform, mPrototypeLink->transform);
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
