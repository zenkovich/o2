#pragma once

#include "o2/Assets/AssetTypes/ActorAsset.h"
#include "o2/Scene/ActorCreationMode.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	// --------------------------------------------------------------
	// Actor reference, automatically invalidates when actor deleting
	// --------------------------------------------------------------
	class ActorRef : public ISerializable
	{
	public:
		// Default constructor, no reference
		ActorRef();

		// Constructor with referencing on actor
		ActorRef(Actor* actor);

		// Creates actor by prototype and returns reference on it
		ActorRef(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Creates actor with components and returns reference on it
		ActorRef(Vector<Component*> components, ActorCreateMode mode = ActorCreateMode::Default);

		// Creates a copy of actor and returns reference on it
		ActorRef(const Actor& other);

		// Destructor
		~ActorRef();

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Assign operator
		ActorRef& operator=(const ActorRef& other);

		// Getter operator
		Actor& operator*();

		// Constant getter operator
		const Actor& operator*() const;

		// Actor members access operator
		Actor* operator->();

		// Constant actor members access operator
		const Actor* operator->() const;

		// Check equals operator
		bool operator==(const ActorRef& other) const;

		// Check not equals operator
		bool operator!=(const ActorRef& other) const;

		// Returns actor pointer
		Actor* Get();

		// Returns actor pointer
		const Actor* Get() const;

		// Returns is reference is valid
		bool IsValid() const;

		// Returns is actor was deleted
		bool IsWasDeleted() const;

		SERIALIZABLE(ActorRef);

	protected:
		Actor* mActor = nullptr;
		bool   mWasDeleted = false;

		friend class Actor;
	};
}

CLASS_BASES_META(o2::ActorRef)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ActorRef)
{
	PROTECTED_FIELD(mActor);
	PROTECTED_FIELD(mWasDeleted);
}
END_META;
CLASS_METHODS_META(o2::ActorRef)
{

	PUBLIC_FUNCTION(Actor*, Get);
	PUBLIC_FUNCTION(const Actor*, Get);
	PUBLIC_FUNCTION(bool, IsValid);
	PUBLIC_FUNCTION(bool, IsWasDeleted);
}
END_META;
