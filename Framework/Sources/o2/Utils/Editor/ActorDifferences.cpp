#include "o2/stdafx.h"
#include "ActorDifferences.h"

#include "o2/Scene/Component.h"
#include "o2/Scene/Scene.h"
#include "o2/Utils/Editor/SceneEditableObject.h"

namespace o2
{
#if IS_EDITOR
	ApplyActorInfo::ApplyActorInfo()
	{}

	ApplyActorInfo::ApplyActorInfo(Actor* actor):
		actor(actor)
	{
		CollectChildren(actor);
	}

	void ApplyActorInfo::CollectChildren(SceneEditableObject* object)
	{
		allChildren.Add(object);

		auto link = object->GetEditableLink();
		while (link)
		{
			allChildrenByLinks[link] = object;
			link = link->GetEditableLink();
		}

		for (auto child : object->GetEditableChildren())
			CollectChildren(child);
	}

	ActorDifferences::ActorDifferences()
	{}

	ActorDifferences::ActorDifferences(const ActorDifferences& other)
	{
		removedChildren = other.removedChildren.Convert<IDifference*>([](auto x) { return x->Clone(); });
		newChildren = other.newChildren.Convert<IDifference*>([](auto x) { return x->Clone(); });
		movedChildren = other.movedChildren.Convert<IDifference*>([](auto x) { return x->Clone(); });
		removedComponents = other.removedComponents.Convert<IDifference*>([](auto x) { return x->Clone(); });
		newComponents = other.newComponents.Convert<IDifference*>([](auto x) { return x->Clone(); });
		changedActorFields = other.changedActorFields.Convert<IDifference*>([](auto x) { return x->Clone(); });
		changedComponentFields = other.changedComponentFields.Convert<IDifference*>([](auto x) { return x->Clone(); });
	}

	ActorDifferences::~ActorDifferences()
	{
		removedChildren.ForEach([](auto x) { delete x; });
		newChildren.ForEach([](auto x) { delete x; });
		movedChildren.ForEach([](auto x) { delete x; });
		removedComponents.ForEach([](auto x) { delete x; });
		newComponents.ForEach([](auto x) { delete x; });
		changedActorFields.ForEach([](auto x) { delete x; });
		changedComponentFields.ForEach([](auto x) { delete x; });
	}

	ActorDifferences ActorDifferences::GetDifference(Actor* changedActor, Actor* prototype)
	{
		ActorDifferences res;

		// Split prototype children
		Vector<SceneEditableObject*> allProtoChildren;
		allProtoChildren.Add(prototype);
		prototype->GetAllEditableChildren(allProtoChildren);

		// Split this actor children
		Vector<SceneEditableObject*> allThisChildren;
		allThisChildren.Add(changedActor);
		changedActor->GetAllEditableChildren(allThisChildren);

		// Check removed and changed from end of hierarchy
		// Because we should check children before parent removes
		for (int i = allProtoChildren.Count() - 1; i >= 0; i--)
		{
			SceneEditableObject* protoChild = allProtoChildren[i];
			if (!protoChild->IsSupportsLinking())
				continue;

			auto thisLinkedChild = allThisChildren.FindOrDefault([&](SceneEditableObject* x) {
				return x->IsEditableLinkedTo(protoChild);
			});

			// Child was removed
			if (thisLinkedChild == nullptr)
			{
				auto diff = mnew RemovedChild();
				diff->prototypeLink = protoChild;
				res.removedChildren.Add(diff);
				continue;
			}

			// Child moved
			if (thisLinkedChild->GetEditableParent() &&
				thisLinkedChild->GetEditableParent()->GetEditableLink() != protoChild->GetEditableParent())
			{
				auto diff = mnew MovedChild();
				diff->prototypeLink = protoChild;
				diff->newParentPrototypeLink = const_cast<SceneEditableObject*>(thisLinkedChild->GetEditableParent()->GetEditableLink());
				res.movedChildren.Add(diff);
			}

			// Not removed, checking for changes on actor, removed, created or changed components
			Vector<const FieldInfo*> fieldsStack;

			// Get differences from actor
			auto createActorChangedFieldDiff = [&](const String& fieldPath) {
				auto diff = mnew ChangedObjectField();
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
			if (auto actorProtoChild = dynamic_cast<Actor*>(protoChild))
			{
				for (auto protoChildComponent : actorProtoChild->GetComponents())
				{
					if (auto actorThisLinkedChild = dynamic_cast<Actor*>(thisLinkedChild))
					{
						auto thisLinkedChildComponent = actorThisLinkedChild->GetComponents()
							.FindOrDefault([&](Component* x) { return x->IsLinkedToComponent(protoChildComponent); });

						// Component was removed
						if (thisLinkedChildComponent == nullptr)
						{
							auto diff = mnew RemovedComponent();
							diff->prototypeLink = protoChildComponent;
							diff->ownerPrototypeLink = actorProtoChild;
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
				}

				// Check new components
				if (auto actorThisLinkedChild = dynamic_cast<Actor*>(thisLinkedChild))
				{
					for (auto thisChildComponent : actorThisLinkedChild->GetComponents())
					{
						// Not linked, because it is new
						if (thisChildComponent->GetPrototypeLink() == nullptr)
						{
							auto diff = mnew NewComponent();
							diff->ownerPrototypeLink = actorProtoChild;
							diff->newComponent = thisChildComponent;
							res.newComponents.Add(diff);
						}
					}
				}
			}
		}

		// Check new actors
		for (auto child : allThisChildren)
		{
			// Empty prototype link on child, but not empty on parent means that is new actor
			if (child->IsSupportsLinking() && child->GetEditableLink() == nullptr && child->GetEditableParent() &&
				child->GetEditableParent()->GetEditableLink())
			{
				auto diff = mnew NewChild();
				diff->parentPrototypeLink = const_cast<SceneEditableObject*>(child->GetEditableParent()->GetEditableLink());
				diff->newChild = child;
				res.newChildren.Add(diff);
			}
		}

		return res;
	}

	void ActorDifferences::GetObjectDifferences(const Function<void(const String& path)>& createDiffFunc,
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
			if (!fieldInfo.HasAttribute<SerializableAttribute>() && !fieldInfo.HasAttribute<PrototypeDeltaSearchAttribute>())
				continue;

			if (auto serializeIfAttr = fieldInfo.GetAttribute<ISerializeIfAttribute>())
			{
				if (!serializeIfAttr->Invoke(changedObject))
					continue;
			}

			stack.Add(&fieldInfo);

			auto* changedFieldObject = fieldInfo.GetValuePtr(changedObject);
			auto* protoFieldObject = fieldInfo.GetValuePtr(protoObject);
			GetFieldDifference(createDiffFunc, stack, *fieldInfo.GetType(), changedFieldObject, protoFieldObject);

			stack.PopBack();
		}
	}

	void ActorDifferences::GetFieldDifference(const Function<void(const String& path)>& createDiffFunc,
											  Vector<const FieldInfo*>& stack, const Type& fieldType,
											  void* changedFieldObject, void* protoFieldObject)
	{
		if (fieldType.GetUsage() == Type::Usage::Object)
		{
			if (changedFieldObject && protoFieldObject)
			{
				auto& realType = dynamic_cast<const ObjectType&>(fieldType).DynamicCastToIObject(changedFieldObject)->GetType();

				if (realType.GetStaticFunction("IsDeltaAsSingleObject"))
				{
					if (!fieldType.IsValueEquals(changedFieldObject, protoFieldObject))
						createDiffFunc(GetFieldPath(stack));
				}
				else
					GetObjectDifferences(createDiffFunc, stack, realType, changedFieldObject, protoFieldObject);
			}
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

	String ActorDifferences::GetFieldPath(const Vector<const FieldInfo*>& stack)
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

	void ActorDifferences::RemovedChild::Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
											   const Vector<ApplyActorInfo>& applyInfos) const
	{
		for (auto& applyInfo : applyInfos)
		{
			SceneEditableObject* child = nullptr;
			if (applyInfo.allChildrenByLinks.TryGetValue(prototypeLink, child))
			{
				if (auto actor = dynamic_cast<Actor*>(child))
					o2Scene.DestroyActor(actor);
				else
					delete child;
			}
		}

		if (auto actor = dynamic_cast<Actor*>(prototypeLink))
			o2Scene.DestroyActor(actor);
		else
			delete prototypeLink;
	}

	void ActorDifferences::NewChild::Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
										   const Vector<ApplyActorInfo>& applyInfos) const
	{
		newChild->BeginMakePrototype();
		auto newInstanceChild = newChild->CloneAs<SceneEditableObject>();
		parentPrototypeLink->AddEditableChild(newInstanceChild);

		for (auto& applyInfo : applyInfos)
		{
			SceneEditableObject* child = nullptr;
			if (applyInfo.allChildrenByLinks.TryGetValue(parentPrototypeLink, child))
			{
				auto prototypeLink = newChild->GetEditableLink();
				prototypeLink->BeginInstantiatePrototype();
				auto newInstanceChild = prototypeLink->CloneAs<Actor>();
				child->AddEditableChild(newInstanceChild);
			}
		}
	}

	void ActorDifferences::MovedChild::Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
											 const Vector<ApplyActorInfo>& applyInfos) const
	{
		prototypeLink->SetEditableParent(newParentPrototypeLink);

		for (auto& applyInfo : applyInfos)
		{
			SceneEditableObject* child = nullptr;
			if (applyInfo.allChildrenByLinks.TryGetValue(prototypeLink, child))
			{
				SceneEditableObject* newParent = nullptr;
				if (applyInfo.allChildrenByLinks.TryGetValue(newParentPrototypeLink, newParent))
					child->SetEditableParent(newParent);
			}
		}
	}

	void ActorDifferences::RemovedComponent::Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
												   const Vector<ApplyActorInfo>& applyInfos) const
	{
		for (auto& applyInfo : applyInfos)
		{
			SceneEditableObject* child = nullptr;
			if (applyInfo.allChildrenByLinks.TryGetValue(ownerPrototypeLink, child))
			{
				if (auto actor = dynamic_cast<Actor*>(child))
				{
					for (auto comp : actor->GetComponents())
					{
						if (comp->IsLinkedToComponent(prototypeLink))
						{
							o2Scene.DestroyComponent(comp);
							break;
						}
					}
				}
			}
		}

		o2Scene.DestroyComponent(prototypeLink);
	}

	void ActorDifferences::NewComponent::Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
											   const Vector<ApplyActorInfo>& applyInfos) const
	{
		auto protoNewComponent = newComponent->CloneAs<Component>();
		newComponent->mPrototypeLink = protoNewComponent;
		ownerPrototypeLink->AddComponent(protoNewComponent);

		for (auto& applyInfo : applyInfos)
		{
			SceneEditableObject* child = nullptr;
			if (applyInfo.allChildrenByLinks.TryGetValue(ownerPrototypeLink, child))
			{
				if (auto actor = dynamic_cast<Actor*>(child))
				{
					auto newActorComponent = newComponent->GetPrototypeLink()->CloneAs<Component>();
					newActorComponent->mPrototypeLink = protoNewComponent;
					actor->AddComponent(newActorComponent);
				}
			}
		}
	}

	void ActorDifferences::ChangedObjectField::Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
													 const Vector<ApplyActorInfo>& applyInfos) const
	{
		SceneEditableObject* sourceChild = nullptr;
		if (!sourceInfo.allChildrenByLinks.TryGetValue(prototypeLink, sourceChild))
			return;

		auto& objectType = dynamic_cast<const ObjectType&>(sourceChild->GetType());
		const FieldInfo* sourceFieldInfo = nullptr;
		const FieldInfo* protoFieldInfo = nullptr;
		auto sourceFieldPtr = objectType.GetFieldPtr(objectType.DynamicCastFromIObject(sourceChild), path, sourceFieldInfo);
		auto protoFieldPtr = objectType.GetFieldPtr(objectType.DynamicCastFromIObject(prototypeLink), path, protoFieldInfo);

		if (!sourceFieldPtr || !protoFieldPtr)
			return;

		for (auto& applyInfo : applyInfos)
		{
			SceneEditableObject* child = nullptr;
			if (!applyInfo.allChildrenByLinks.TryGetValue(prototypeLink, child))
				continue;

			auto& objectType = dynamic_cast<const ObjectType&>(child->GetType());
			const FieldInfo* fieldInfo = nullptr;
			auto fieldPtr = objectType.GetFieldPtr(objectType.DynamicCastFromIObject(child), path, fieldInfo);
			if (!fieldPtr)
				continue;

			if (sourceFieldInfo->GetType()->IsValueEquals(protoFieldPtr, fieldPtr))
				sourceFieldInfo->GetType()->CopyValue(fieldPtr, sourceFieldPtr);
		}

		sourceFieldInfo->GetType()->CopyValue(protoFieldPtr, sourceFieldPtr);
	}

	void ActorDifferences::ChangedComponentField::Apply(ApplyActorInfo& sourceInfo, ApplyActorInfo& prototypeInfo,
														const Vector<ApplyActorInfo>& applyInfos) const
	{
		SceneEditableObject* sourceChild = nullptr;
		if (!sourceInfo.allChildrenByLinks.TryGetValue(ownerPrototypeLink, sourceChild))
			return;

		auto sourceActor = dynamic_cast<Actor*>(sourceChild);
		if (!sourceActor)
			return;

		for (auto sourceComp : sourceActor->GetComponents())
		{
			if (!sourceComp->IsLinkedToComponent(prototypeLink))
				continue;

			auto& objectType = dynamic_cast<const ObjectType&>(sourceComp->GetType());
			const FieldInfo* sourceFieldInfo = nullptr;
			const FieldInfo* protoFieldInfo = nullptr;
			auto sourceFieldPtr = objectType.GetFieldPtr(objectType.DynamicCastFromIObject(sourceComp), path, sourceFieldInfo);
			auto protoFieldPtr = objectType.GetFieldPtr(objectType.DynamicCastFromIObject(prototypeLink), path, protoFieldInfo);

			if (!sourceFieldPtr || !protoFieldInfo)
				break;

			for (auto& applyInfo : applyInfos)
			{
				SceneEditableObject* child = nullptr;
				if (!applyInfo.allChildrenByLinks.TryGetValue(ownerPrototypeLink, child))
					continue;

				auto actor = dynamic_cast<Actor*>(child);
				if (!actor)
					continue;

				for (auto comp : actor->GetComponents())
				{
					if (!comp->IsLinkedToComponent(prototypeLink))
						continue;

					auto& objectType = dynamic_cast<const ObjectType&>(comp->GetType());
					const FieldInfo* fieldInfo = nullptr;
					auto fieldPtr = objectType.GetFieldPtr(objectType.DynamicCastFromIObject(comp), path, fieldInfo);

					if (!fieldPtr)
						break;

					if (sourceFieldInfo->GetType()->IsValueEquals(protoFieldPtr, fieldPtr))
						sourceFieldInfo->GetType()->CopyValue(fieldPtr, sourceFieldPtr);
				}
			}

			sourceFieldInfo->GetType()->CopyValue(protoFieldPtr, sourceFieldPtr);

			break;
		}
	}

	ActorDifferences::IDifference* ActorDifferences::RemovedChild::Clone() const
	{
		return mnew RemovedChild(*this);
	}

	ActorDifferences::IDifference* ActorDifferences::NewChild::Clone() const
	{
		return mnew NewChild(*this);
	}

	ActorDifferences::IDifference* ActorDifferences::RemovedComponent::Clone() const
	{
		return mnew RemovedComponent(*this);
	}

	ActorDifferences::IDifference* ActorDifferences::NewComponent::Clone() const
	{
		return mnew NewComponent(*this);
	}

	ActorDifferences::IDifference* ActorDifferences::ChangedObjectField::Clone() const
	{
		return mnew ChangedObjectField(*this);
	}

	ActorDifferences::IDifference* ActorDifferences::ChangedComponentField::Clone() const
	{
		return mnew ChangedComponentField(*this);
	}

	ActorDifferences::IDifference* ActorDifferences::MovedChild::Clone() const
	{
		return mnew MovedChild(*this);
	}
#endif
}
