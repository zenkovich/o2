#pragma once

#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Scene/ActorCreationMode.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	// --------------------------------------------------------------
	// Actor reference, automatically invalidates when actor deleting
	// --------------------------------------------------------------
	template<>
	class Ref<Actor>: public ISerializable
	{
	public:
		// Default constructor, no reference
		Ref();

		// Constructor with referencing on actor
		Ref(Actor* actor);

		// Creates actor by prototype and returns reference on it
		Ref(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Creates actor with components and returns reference on it
		Ref(Vector<Component*> components, ActorCreateMode mode = ActorCreateMode::Default);

		// Creates a copy of actor and returns reference on it
		Ref(const Actor& other);

		// Destructor
		~Ref();

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Assign operator
		Ref& operator=(const Ref& other);

		// Getter operator
		Actor& operator*();

		// Constant getter operator
		const Actor& operator*() const;

		// Actor members access operator
		Actor* operator->();

		// Constant actor members access operator
		const Actor* operator->() const;

		// Check equals operator
		bool operator==(const Ref& other) const;

		// Check not equals operator
		bool operator!=(const Ref& other) const;

		// Returns actor pointer
		Actor* Get();

		// Returns actor pointer
		const Actor* Get() const;

		// Destroy the actor
		void Destroy();

		// Returns is reference is valid
		bool IsValid() const;

		// Returns is actor was deleted
		bool IsWasDeleted() const;

		SERIALIZABLE(Ref<Actor>);

	protected:
		Actor* mActor = nullptr;
		bool   mWasDeleted = false;

		friend class Actor;
	};

	typedef Ref<Actor> ActorRef;
}

CLASS_BASES_META(o2::Ref<o2::Actor>)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Ref<o2::Actor>)
{
	PROTECTED_FIELD(mActor).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mWasDeleted).DEFAULT_VALUE(false);
}
END_META;
CLASS_METHODS_META(o2::Ref<o2::Actor>)
{

	PUBLIC_FUNCTION(Actor*, Get);
	PUBLIC_FUNCTION(const Actor*, Get);
	PUBLIC_FUNCTION(void, Destroy);
	PUBLIC_FUNCTION(bool, IsValid);
	PUBLIC_FUNCTION(bool, IsWasDeleted);
}
END_META;
