#include "Prototypes.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Components/EditorTestComponent.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Scene/Physics/RigidBody.h"

using namespace o2;

template<typename DiffType>
const DiffType* FindDiff(const Actor::Differences& diffs, const Function<bool(const DiffType&)>& check)
{
	auto checkCast = [&](Actor::Differences::IDifference* x) { return check(*dynamic_cast<DiffType*>(x)); };

	if constexpr (std::is_base_of<Actor::Differences::NewChild, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.newChildren.FindOrDefault(checkCast));

	if constexpr (std::is_same<Actor::Differences::NewComponent, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.newComponents.FindOrDefault(checkCast));

	if constexpr (std::is_same<Actor::Differences::ChangedActorField, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.changedActorFields.FindOrDefault(checkCast));

	if constexpr (std::is_same<Actor::Differences::ChangedComponentField, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.changedComponentFields.FindOrDefault(checkCast));

	if constexpr (std::is_same<Actor::Differences::RemovedChild, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.removedChildren.FindOrDefault(checkCast));

	if constexpr (std::is_same<Actor::Differences::RemovedComponent, DiffType>::value)
		return dynamic_cast<const DiffType*>(diffs.removedComponents.FindOrDefault(checkCast));

	return nullptr;
}

struct IDiffChecker
{
	String description;
	String status = "OK";

	IDiffChecker(const String& description): description(description) {}

	virtual bool Test(const Actor::Differences& diffs) const = 0;
};

template<typename DiffType>
struct DiffChecker: public IDiffChecker
{
	Function<bool(const DiffType&)> check;

	DiffChecker(const String& description, const Function<bool(const DiffType&)>& check):
		IDiffChecker(description), check(check) {}

	bool Test(const Actor::Differences& diffs) const override
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
	Actor* sample = mnew Actor({ mnew EditorTestComponent() });
	sample->transform->size = Vec2F(10, 10);
	sample->AddChild(o2UI.CreateButton("button"));
	sample->AddChild(mnew RigidBody())->name = "rigid body";
	sample->AddChild(mnew Actor())->name = "removing";
	sample->AddComponent(mnew AnimationComponent());

	// Create prototype and instance
	auto protoAsset = sample->MakePrototype();
	Actor* instance = mnew Actor(protoAsset);
	Actor* otherInstance = mnew Actor(protoAsset);

	Vector<IDiffChecker*> checkers;

	// Check transform change
	{
		instance->transform->position = Vec2F(10, 10);

		checkers.Add(mnew DiffChecker<Actor::Differences::ChangedActorField>("Transform difference - position",
					 [](auto& diff) { return diff.path == "transform/mData/position"; }));
	}

	// Check component field change
	if (auto testComponent = instance->GetComponent<EditorTestComponent>())
	{
		testComponent->mInteger = 5;
		testComponent->mTestInside.mFloat = 5;

		checkers.Add(mnew DiffChecker<Actor::Differences::ChangedComponentField>("Component difference - mInteger",
					 [](auto& diff) { return diff.path == "mInteger"; }));

		checkers.Add(mnew DiffChecker<Actor::Differences::ChangedComponentField>("Component difference - mTestInside/mFloat",
					 [](auto& diff) { return diff.path == "mTestInside/mFloat"; }));
	}

	// Check actor field change
	if (auto rigiBodyChild = instance->FindChildByType<RigidBody>())
	{
		rigiBodyChild->bodyType = RigidBody::Type::Static;
		rigiBodyChild->mass = 10;

		checkers.Add(mnew DiffChecker<Actor::Differences::ChangedActorField>("Actor difference 'bodyType'",
					 [](auto& x) {	return x.path == "mBodyType"; }));

		checkers.Add(mnew DiffChecker<Actor::Differences::ChangedActorField>("Actor difference 'mass'",
					 [](auto& x) {	return x.path == "mMass"; }));
	}

	// New child
	Actor* newChild = nullptr;
	if (auto rigiBodyChild = instance->FindChildByType<RigidBody>())
	{
		newChild = rigiBodyChild->AddChild(mnew Actor());
		newChild->name = "new child";

		checkers.Add(mnew DiffChecker<Actor::Differences::NewChild>("New actor child",
					 [&](auto& x) { return x.newChild == newChild; }));
	}

	// Removed child
	if (auto removingComponent = instance->GetChild("removing"))
	{
		o2Scene.DestroyActor(removingComponent);

		checkers.Add(mnew DiffChecker<Actor::Differences::RemovedChild>("Removed child",
					 [&](auto& x) { return x.prototypeLink->name == "removing"; }));
	}

	// New component
	Component* newComponent = nullptr;
	if (auto rigiBodyChild = instance->FindChildByType<RigidBody>())
	{
		auto newImageComponent = mnew ImageComponent("Game field/Back.png");
		newComponent = rigiBodyChild->AddComponent(newImageComponent);
		newImageComponent->FitActorByImage();
		rigiBodyChild->UpdateTransform();
		
		checkers.Add(mnew DiffChecker<Actor::Differences::NewComponent>("New actor component",
					 [&](auto& x) { return x.newComponent == newComponent; }));

		checkers.Add(mnew DiffChecker<Actor::Differences::ChangedActorField>("Transform difference - size (after fitting by image)",
					 [](auto& diff) { return diff.path == "transform/mData/size"; }));
	}

	// Removed component
	if (auto removingComponent = instance->GetComponent<AnimationComponent>())
	{
		o2Scene.DestroyComponent(removingComponent);

		checkers.Add(mnew DiffChecker<Actor::Differences::RemovedComponent>("Removed component",
					 [&](auto& x) {
			return x.prototypeLink == protoAsset->GetActor()->GetComponent<AnimationComponent>();
		}));
	}

	// Update scene to update transforms and other stuff
	o2Scene.Update(0.0f);

	// Get differences and check them
	auto& diffs = Actor::Differences::GetDifference(instance, protoAsset->GetActor());

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

	auto& newDiffs = Actor::Differences::GetDifference(instance, protoAsset->GetActor());
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

