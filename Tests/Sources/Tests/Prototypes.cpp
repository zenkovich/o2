#include "o2/stdafx.h"
#include "Prototypes.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Components/EditorTestComponent.h"
#include "o2/Scene/Physics/RigidBody.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Utils/Editor/ActorDifferences.h"
#include "o2/Scene/Components/AnimationComponent.h"
#include "o2/Scene/Scene.h"

using namespace o2;

template<typename DiffType>
const DiffType* FindDiff(const ActorDifferences& diffs, const Function<bool(const DiffType&)>& check)
{
	auto checkCast = [&](ActorDifferences::IDifference* x) { return check(*dynamic_cast<DiffType*>(x)); };

	if constexpr (std::is_base_of<ActorDifferences::NewChild, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.newChildren.FindOrDefault(checkCast));

	if constexpr (std::is_same<ActorDifferences::NewComponent, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.newComponents.FindOrDefault(checkCast));

	if constexpr (std::is_same<ActorDifferences::ChangedObjectField, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.changedActorFields.FindOrDefault(checkCast));

	if constexpr (std::is_same<ActorDifferences::ChangedComponentField, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.changedComponentFields.FindOrDefault(checkCast));

	if constexpr (std::is_same<ActorDifferences::RemovedChild, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.removedChildren.FindOrDefault(checkCast));

	if constexpr (std::is_same<ActorDifferences::RemovedComponent, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.removedComponents.FindOrDefault(checkCast));

	return nullptr;
}

struct IDiffChecker
{
	String description;
	String status = "OK";

	IDiffChecker(const String& description): description(description) {}

	virtual bool Test(const ActorDifferences& diffs) const = 0;
};

template<typename DiffType>
struct DiffChecker: public IDiffChecker
{
	Function<bool(const DiffType&)> check;

	DiffChecker(const String& description, const Function<bool(const DiffType&)>& check):
		IDiffChecker(description), check(check) {}

	bool Test(const ActorDifferences& diffs) const override
	{
		auto diff = FindDiff<DiffType>(diffs, check);
		return diff != nullptr;
	}
};

// This is the test of applying prototypes
// Here we creating some sample actor, then making prototype from it
// Then instantiate two instances of it
// Modificating first instance 
// And checking applying this changes to prototype and other instance
void TestPrototypes()
{
	// Prepare original sample
	Actor* sample = mnew Actor({ mnew EditorTestComponent(), mnew AnimationComponent() });
	sample->transform->size = Vec2F(10, 10);
	sample->AddChild(o2UI.CreateButton(""))->name = "button";
	sample->AddChild(mnew RigidBody())->name = "rigid body";
	sample->AddChild(mnew Actor())->name = "removing";

	auto moving = sample->AddChild(mnew Actor());
	moving->name = "moving";
	moving->transform->size = Vec2F(10, 10);

	// Create prototype and instance
	auto protoAsset = sample->MakePrototype();
	Actor* instance = mnew Actor(protoAsset);
	Actor* otherInstance = mnew Actor(protoAsset);

	// Check that sample linked to prototype correctly
	if (sample->GetPrototypeLink() == protoAsset->GetActor())
		o2Debug.Log("Sample link to proto - OK");
	else
		o2Debug.LogError("Sample link to proto - FAILED");

	// Child linked
	if (sample->GetChild("button")->GetPrototypeLink() == protoAsset->GetActor()->GetChild("button"))
		o2Debug.Log("Sample link child to proto - OK");
	else
		o2Debug.LogError("Sample link child to proto - FAILED");

	// Component linked
	if (sample->GetComponent<EditorTestComponent>()->GetPrototypeLink() == protoAsset->GetActor()->GetComponent<EditorTestComponent>())
		o2Debug.Log("Sample link component to proto - OK");
	else
		o2Debug.LogError("Sample link component to proto - FAILED");

	// Child widget layer linked
	if (sample->GetChildByType<Button>("button")->GetLayer("hover")->GetPrototypeLink() ==
		protoAsset->GetActor()->GetChildByType<Button>("button")->GetLayer("hover"))
	{
		o2Debug.Log("Sample link widget layer to proto - OK");
	}
	else
		o2Debug.LogError("Sample link widget layer to proto - FAILED");

	Vector<IDiffChecker*> checkers;

	// Check transform change
	{
		instance->transform->position = Vec2F(10, 10);

		checkers.Add(mnew DiffChecker<ActorDifferences::ChangedObjectField>("Transform difference - position",
					 [](auto& diff) { return diff.path == "transform/mData/position"; }));
	}

	// Check widget layout change
	{
		if (auto button = instance->FindChildByType<Button>())
		{
			button->layout->offsetLeft = -100;

			checkers.Add(mnew DiffChecker<ActorDifferences::ChangedObjectField>("Widget layout difference - offsetLeft",
						 [](auto& diff) { return diff.path == "transform/mData/offsetMin"; }));
		}
	}

	// Check component field change
	if (auto testComponent = instance->GetComponent<EditorTestComponent>())
	{
		testComponent->mInteger = 5;
		testComponent->mTestInside.mFloat = 5;

		checkers.Add(mnew DiffChecker<ActorDifferences::ChangedComponentField>("Component difference - mInteger",
					 [](auto& diff) { return diff.path == "mInteger"; }));

		checkers.Add(mnew DiffChecker<ActorDifferences::ChangedComponentField>("Component difference - mTestInside/mFloat",
					 [](auto& diff) { return diff.path == "mTestInside/mFloat"; }));
	}

	// Check actor field change
	if (auto rigiBodyChild = instance->FindChildByType<RigidBody>())
	{
		rigiBodyChild->bodyType = RigidBody::Type::Static;
		rigiBodyChild->mass = 10;

		checkers.Add(mnew DiffChecker<ActorDifferences::ChangedObjectField>("Actor difference 'bodyType'",
					 [](auto& x) {	return x.path == "mBodyType"; }));

		checkers.Add(mnew DiffChecker<ActorDifferences::ChangedObjectField>("Actor difference 'mass'",
					 [](auto& x) {	return x.path == "mMass"; }));
	}

	// Check widget layer changes
	if (auto button = instance->FindChildByType<Button>())
	{
		// Layer change
		if (auto regularLayer = button->GetLayer("regular"))
		{
			regularLayer->layout.SetAnchorLeft(10);

			checkers.Add(mnew DiffChecker<ActorDifferences::ChangedObjectField>("Widget layer transform difference",
						 [&](auto& x) {  return x.prototypeLink == regularLayer->GetEditableLink() && x.path == "layout/mAnchorMin"; }));

			if (auto sprite = dynamic_cast<Sprite*>(regularLayer->GetDrawable()))
			{
				sprite->transparency = 0.5f;

				checkers.Add(mnew DiffChecker<ActorDifferences::ChangedObjectField>("Widget layer difference 'mColor'",
							 [=](auto& x) {  return x.prototypeLink == regularLayer->GetEditableLink() && x.path == "mDrawable/mColor"; }));
			}
		}

		// Removed layer
		if (auto focusLayer = button->GetLayer("focus"))
		{
			auto link = focusLayer->GetEditableLink();
			button->RemoveLayer(focusLayer);

			checkers.Add(mnew DiffChecker<ActorDifferences::RemovedChild>("Widget layer removed",
						 [=](auto& x) {  return x.prototypeLink == link; }));
		}

		// New layer
		{
			auto newLayer = button->AddLayer("new layer", mnew Sprite());

			checkers.Add(mnew DiffChecker<ActorDifferences::NewChild>("Widget layer added",
						 [=](auto& x) {  return x.newChild == newLayer; }));
		}

		// Moved layer
		if (auto hoverLayer = button->GetLayer("hover"))
		{
			if (auto pressedLayer = button->GetLayer("pressed"))
			{
				hoverLayer->SetParent(pressedLayer);

				checkers.Add(mnew DiffChecker<ActorDifferences::MovedChild>("Widget layer moved",
							 [=](auto& x) { return x.prototypeLink->GetName() == "hover" && x.newParentPrototypeLink->GetName() == "pressed"; }));
			}
		}
	}

	// New child
	Actor* newChild = nullptr;
	if (auto rigiBodyChild = instance->FindChildByType<RigidBody>())
	{
		newChild = rigiBodyChild->AddChild(mnew Actor());
		newChild->name = "new child";

		checkers.Add(mnew DiffChecker<ActorDifferences::NewChild>("New actor child",
					 [&](auto& x) { return x.newChild == newChild; }));
	}

	// Removed child
	if (auto removingComponent = instance->GetChild("removing"))
	{
		o2Scene.DestroyActor(removingComponent);

		checkers.Add(mnew DiffChecker<ActorDifferences::RemovedChild>("Removed child",
					 [&](auto& x) { return x.prototypeLink->GetName() == "removing"; }));
	}

	// Move child
	if (auto movingChild = instance->GetChild("moving"))
	{
		if (auto rigiBodyChild = instance->FindChildByType<RigidBody>())
		{
			movingChild->SetParent(rigiBodyChild);

			checkers.Add(mnew DiffChecker<ActorDifferences::MovedChild>("Moved child",
						 [&](auto& x) { return x.prototypeLink->GetName() == "moving" && x.newParentPrototypeLink->GetName() == "rigid body"; }));
		}
	}

	// New component
	Component* newComponent = nullptr;
	if (auto rigiBodyChild = instance->FindChildByType<RigidBody>())
	{
		auto newImageComponent = mnew ImageComponent("Game field/Back.png");
		newComponent = rigiBodyChild->AddComponent(newImageComponent);
		newImageComponent->FitActorByImage();
		rigiBodyChild->UpdateTransform();
		
		checkers.Add(mnew DiffChecker<ActorDifferences::NewComponent>("New actor component",
					 [&](auto& x) { return x.newComponent == newComponent; }));

		checkers.Add(mnew DiffChecker<ActorDifferences::ChangedObjectField>("Transform difference - size (after fitting by image)",
					 [](auto& diff) { return diff.path == "transform/mData/size"; }));
	}

	// Removed component
	if (auto removingComponent = instance->GetComponent<AnimationComponent>())
	{
		o2Scene.DestroyComponent(removingComponent);

		checkers.Add(mnew DiffChecker<ActorDifferences::RemovedComponent>("Removed component",
					 [&](auto& x) {
			return x.prototypeLink == protoAsset->GetActor()->GetComponent<AnimationComponent>();
		}));
	}

	// Update scene to update transforms and other stuff
	o2Scene.Update(0.0f);

	// Get differences and check them
	auto& diffs = ActorDifferences::GetDifference(instance, protoAsset->GetActor());

	for (auto check : checkers)
	{
		if (!check->Test(diffs))
			check->status = "can't detect";
	}

	int allDifferencesCount = diffs.newChildren.Count() + diffs.newComponents.Count() +
		diffs.changedActorFields.Count() + diffs.changedComponentFields.Count() +
		diffs.removedChildren.Count() + diffs.removedComponents.Count();

	if (allDifferencesCount > checkers.Count())
		o2Debug.LogError("There are extra differences before applying:" + (String)(allDifferencesCount - checkers.Count()));

	// Apply actor to prefab and check differences
	instance->ApplyChangesToPrototype();

	// Update scene to update transforms and other stuff
	o2Scene.Update(0.0f);

	auto& newDiffs = ActorDifferences::GetDifference(instance, protoAsset->GetActor());
	for (auto check : checkers)
	{
		if (check->Test(newDiffs))
			check->status = "not applied";
	}

	allDifferencesCount = newDiffs.newChildren.Count() + newDiffs.newComponents.Count() +
		newDiffs.changedActorFields.Count() + newDiffs.changedComponentFields.Count() +
		newDiffs.removedChildren.Count() + newDiffs.removedComponents.Count();

	if (allDifferencesCount > 0)
		o2Debug.LogError("There are extra differences after applying:" + (String)allDifferencesCount);

	for (auto check : checkers)
		o2Debug.Log(check->description + " - " + check->status);
}

