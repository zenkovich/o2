#pragma once

#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Scene/ActorCreationMode.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	// --------------------------------------------------------------
	// Actor reference, automatically invalidates when actor deleting
	// --------------------------------------------------------------
	class ActorRef: public ISerializable
	{
	public:
		// Default constructor, no reference
		ActorRef();

		// Constructor with referencing on actor
		ActorRef(Actor* actor);

		// Creates a copy of actor and returns reference on it
		ActorRef(const ActorRef& other);

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

		// Destroy the actor
		void Destroy();

		// Returns is reference is valid
		bool IsValid() const;

		// Returns is actor was deleted
		bool IsWasDeleted() const;

		// Returns actor type
		virtual const Type& GetActorType() const;

		// Returns actor type
		static const Type* GetActorTypeStatic();

		SERIALIZABLE(ActorRef);

	protected:
		Actor* mActor = nullptr;
		bool   mWasDeleted = false;

	protected:
		// Updates specialized actor pointer
		virtual void UpdateSpecActor() {}

		// Beginning serialization callback
		void OnSerialize(DataValue& node) const override;

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		friend class Actor;
		friend class ActorRefResolver;
	};

	// ---------------------------------------
	// Reference on derived from actor classes
	// ---------------------------------------
	template<typename T>
	class Ref<T, typename std::enable_if<std::is_base_of<Actor, T>::value>::type>: public ActorRef
	{
	public:
		// Default constructor, no reference
		Ref(): ActorRef() {}

		// Constructor with referencing on actor
		Ref(T* actor): ActorRef(actor), mSpecActor(actor) {}

		// Creates a copy of actor and returns reference on it
		Ref(const Ref<T>& other): ActorRef(other), mSpecActor(other.mSpecActor) {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		Ref<T>& operator=(const Ref<T>& other)
		{
			ActorRef::operator=(other);
			mSpecActor = other.mSpecActor;
			return *this;
		}

		// Getter operator
		T& operator*() { return *mSpecActor; }

		// Constant getter operator
		const T& operator*() const { return *mSpecActor; }

		// Asset members and field operator
		T* operator->() { return mSpecActor; }

		// Constant asset members and field operator
		const T* operator->() const { return mSpecActor; }

		// Check equals operator
		bool operator==(const Ref<T>& other) const { return ActorRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const Ref<T>& other) const { return ActorRef::operator!=(other); }

		// Returns actor type
		const Type& GetActorType() const override { return TypeOf(T); }

		// Returns actor type
		static const Type* GetActorTypeStatic() { return &TypeOf(T); }

	public:
		typedef Ref<T, typename std::enable_if<std::is_base_of<Actor, T>::value>::type> _thisType;

		SERIALIZABLE_MAIN(_thisType);

		template<typename _type_processor>
		static void ProcessBaseTypes(_thisType* object, _type_processor& processor)
		{
			typedef _thisType thisclass;
			processor.template StartBases<_thisType>(object, type);

			BASE_CLASS(o2::ActorRef);
		}

		template<typename _type_processor>
		static void ProcessFields(_thisType* object, _type_processor& processor)
		{
			typedef _thisType thisclass;
			processor.template StartFields<_thisType>(object, type);

			FIELD().NAME(mSpecActor).PROTECTED();
		}

		template<typename _type_processor>
		static void ProcessMethods(_thisType* object, _type_processor& processor)
		{
			typedef _thisType thisclass;
			processor.template StartMethods<_thisType>(object, type);

			PUBLIC_FUNCTION(const Type&, GetActorType);
			PUBLIC_STATIC_FUNCTION(const Type*, GetActorTypeStatic);
		}

	protected:
		T* mSpecActor = nullptr;

	protected:
		// Updates specialized actor pointer
		void UpdateSpecActor() override { mSpecActor = dynamic_cast<T*>(mActor); };
	};
}

CLASS_BASES_META(o2::ActorRef)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ActorRef)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(mActor).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mWasDeleted).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::ActorRef)
{

	PUBLIC_FUNCTION(Actor*, Get);
	PUBLIC_FUNCTION(const Actor*, Get);
	PUBLIC_FUNCTION(void, Destroy);
	PUBLIC_FUNCTION(bool, IsValid);
	PUBLIC_FUNCTION(bool, IsWasDeleted);
	PUBLIC_FUNCTION(const Type&, GetActorType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetActorTypeStatic);
	PROTECTED_FUNCTION(void, UpdateSpecActor);
	PROTECTED_FUNCTION(void, OnSerialize, DataValue&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
}
END_META;
