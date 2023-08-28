#include "o2/stdafx.h"
#include "Actor.h"

#include "o2/Scene/ActorRefResolver.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/Scene.h"
#include "o2/Utils/Editor/ActorDifferences.h"
#include "o2/Utils/Editor/Attributes/PrototypeDeltaSearchAttribute.h"
#include "o2/Utils/Serialization/Serializable.h"

#if IS_EDITOR

namespace o2
{
	void Actor::MakePrototypeCloneVisitor::OnCopyActor(const Actor* source, Actor* target)
	{
		SourceToTargetMapCloneVisitor::OnCopyActor(source, target);
		target->mPrototypeLink = source->mPrototypeLink;
		const_cast<Actor*>(source)->mPrototypeLink.CopyWithoutRemap(target);
	}

	void Actor::MakePrototypeCloneVisitor::OnCopyComponent(const Component* source, Component* target)
	{
		SourceToTargetMapCloneVisitor::OnCopyComponent(source, target);
		target->mPrototypeLink = source->mPrototypeLink;
		const_cast<Component*>(source)->mPrototypeLink = target;
	}

	void Actor::SetProtytypeDummy(ActorAssetRef asset)
	{}

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
		prototype->UpdateTransform();
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

	void Actor::ApplyChangesToPrototype()
	{
		if (!mPrototype)
			return;

		// Get difference between this actor and prototype first
		auto diffs = ActorDifferences::GetDifference(this, mPrototype->GetActor());

		// Get applying actors infos
		ApplyActorInfo prototypeApplyInfo(mPrototype->GetActor());
		ApplyActorInfo thisApplyInfo(this);
		Vector<ApplyActorInfo> linkedActorsApplyInfos;

		auto& linkedActors = o2Scene.mPrototypeLinksCache[mPrototype];
		linkedActorsApplyInfos.Reserve(linkedActors.Count());
		linkedActors.ForEach([&](auto x) { if (x != this) linkedActorsApplyInfos.Add(ApplyActorInfo(x)); });
	
		// Apply differences
		diffs.newChildren.ForEach([&](auto d) { d->Apply(thisApplyInfo, prototypeApplyInfo, linkedActorsApplyInfos); });
		diffs.movedChildren.ForEach([&](auto d) { d->Apply(thisApplyInfo, prototypeApplyInfo, linkedActorsApplyInfos); });
		diffs.newComponents.ForEach([&](auto d) { d->Apply(thisApplyInfo, prototypeApplyInfo, linkedActorsApplyInfos); });
		diffs.changedActorFields.ForEach([&](auto d) { d->Apply(thisApplyInfo, prototypeApplyInfo, linkedActorsApplyInfos); });
		diffs.changedComponentFields.ForEach([&](auto d) { d->Apply(thisApplyInfo, prototypeApplyInfo, linkedActorsApplyInfos); });
		diffs.removedComponents.ForEach([&](auto d) { d->Apply(thisApplyInfo, prototypeApplyInfo, linkedActorsApplyInfos); });
		diffs.removedChildren.ForEach([&](auto d) { d->Apply(thisApplyInfo, prototypeApplyInfo, linkedActorsApplyInfos); });

		// Invoke changed callback for actors and save assets
		for (auto info : linkedActorsApplyInfos)
		{
			info.actor->OnChanged();
			info.actor->UpdateTransform();

			if (info.actor->IsAsset())
			{
				ActorAssetRef asset(info.actor->GetAssetID());
				if (info.actor != asset->GetActor())
					asset->SetActor(info.actor, false);

				asset->Save();
			}
		}

		// Save prototype
		mPrototype->GetActor()->UpdateTransform();
		mPrototype->GetActor()->OnChanged();
		mPrototype->Save();

		OnChanged();
	}

	void Actor::GetDifferences(ActorDifferences& differences) const
	{
		Vector<const FieldInfo*> stack;
		stack.Add(GetType().GetField("transform"));

		auto createActorChangedFieldDiff = [&](const String& fieldPath) {
			auto diff = mnew ActorDifferences::ChangedObjectField();
			diff->prototypeLink = const_cast<Actor*>(mPrototypeLink.Get());
			diff->path = fieldPath;
			differences.changedActorFields.Add(diff);
		};

		ActorDifferences::GetObjectDifferences(createActorChangedFieldDiff, stack, transform->GetType(), transform, mPrototypeLink->transform);
	}

	void Actor::BeginMakePrototype() const
	{
		mCopyVisitor = mnew MakePrototypeCloneVisitor();
	}

	void Actor::BeginInstantiatePrototype() const
	{
		mCopyVisitor = mnew InstantiatePrototypeCloneVisitor();
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

	Vector<SceneEditableObject*> Actor::GetEditableChildren() const
	{
		return mChildren.DynamicCast<SceneEditableObject*>();
	}

	void Actor::SetEditableParent(SceneEditableObject* object)
	{
		SetParent(dynamic_cast<Actor*>(object), false);
	}

	const SceneEditableObject* Actor::GetEditableLink() const
	{
		return mPrototypeLink.Get();
	}

	SceneEditableObject* Actor::GetEditableParent() const
	{
		return mParent;
	}

	void Actor::OnChanged()
	{
		onChanged();

		if (Scene::IsSingletonInitialzed() && IsOnScene())
			o2Scene.OnObjectChanged(this);
	}

	void Actor::OnLockChanged()
	{
		onLockChanged(mLocked);
		onChanged();

		if (Scene::IsSingletonInitialzed() && IsOnScene())
		{
			o2Scene.OnObjectChanged(this);
			o2Scene.onLockChanged(this);
		}
	}

	void Actor::OnNameChanged()
	{
		onNameChanged();
		onChanged();

		if (Scene::IsSingletonInitialzed() && IsOnScene())
		{
			o2Scene.OnObjectChanged(this);
			o2Scene.onNameChanged(this);
		}
	}

	void Actor::OnChildrenChanged()
	{
		for (auto comp : mComponents)
			comp->OnChildrenChanged();

		onChildHierarchyChanged();
		onChanged();

		if (Scene::IsSingletonInitialzed() && IsOnScene())
		{
			o2Scene.OnObjectChanged(this);
			o2Scene.onChildrenHierarchyChanged(this);
		}
	}

	void Actor::OnParentChanged(Actor* oldParent)
	{
		ISceneDrawable::OnDrawbleParentChanged();

		for (auto comp : mComponents)
			comp->OnParentChanged(oldParent);

		onParentChanged(oldParent);
		onChanged();

		if (Scene::IsSingletonInitialzed() && IsOnScene())
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
