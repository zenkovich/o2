#pragma once

#include "o2/Scene/ActorCreationMode.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class Actor;

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

		// Returns actor pointer @SCRIPTABLE
		Actor* Get();

		// Returns actor pointer
		const Actor* Get() const;

		// Destroy the actor @SCRIPTABLE
		void Destroy();

		// Returns is reference is valid @SCRIPTABLE
		bool IsValid() const;

		// Returns is actor was deleted @SCRIPTABLE
		bool IsWasDeleted() const;

		// Returns actor type
		virtual const Type& GetActorType() const;

		// Returns actor type
		static const Type* GetActorTypeStatic();

		// Checks refs are equals for serializing delta
		static bool EqualsDelta(const ActorRef& obj, const ActorRef& origin);

		SERIALIZABLE(ActorRef);

	protected:
		// ------------------------------
		// Reference resolve request data
		// ------------------------------
		struct IRequiredResolveData
		{
			// Request resolve reference
			virtual void RequireResolve(ActorRef& ref) = 0; 

			// Clone data
			virtual IRequiredResolveData* Clone() const = 0;
		};

		// -------------------------------------------------
		// Reference resolve request data by actor scene uid
		// -------------------------------------------------
		struct SceneRequireResolveData: public IRequiredResolveData
		{
			SceneUID uid;

			// Request resolve reference
			void RequireResolve(ActorRef& ref) override;

			// Clone data
			IRequiredResolveData* Clone() const override;
		};

		// -------------------------------------------
		// Reference resolve request data by asset uid
		// -------------------------------------------
		struct AssetRequireResolveData: public IRequiredResolveData
		{
			UID uid;

			// Request resolve reference
			void RequireResolve(ActorRef& ref) override;

			// Clone data
			IRequiredResolveData* Clone() const override;
		};

		Actor* mActor = nullptr;
		bool   mWasDeleted = false;

		IRequiredResolveData* mRequiredResolveData = nullptr; // Reference resolve request data. Used for resolving reference after deserialization.
		                                                      // Not null only when reference is required to resolve. Copies in reference copying.

	protected:
		// Updates specialized actor pointer
		virtual void UpdateSpecActor() {}

		// Copying ref without requiring remap
		void CopyWithoutRemap(const ActorRef& other);

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

		// Returns actor pointer 
		T* Get() { return mSpecActor; }

		// Returns actor pointer
		const T* Get() const { return mSpecActor; }

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

			FIELD().PROTECTED().NAME(mSpecActor);
		}

		template<typename _type_processor>
		static void ProcessMethods(_thisType* object, _type_processor& processor)
		{
			typedef _thisType thisclass;
			processor.template StartMethods<_thisType>(object, type);

			FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR();
			FUNCTION().PUBLIC().SIGNATURE(const Type&, GetActorType);
			FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetActorTypeStatic);
		}

	protected:
		T* mSpecActor = nullptr;

	protected:
		// Updates specialized actor pointer
		void UpdateSpecActor() override { mSpecActor = dynamic_cast<T*>(mActor); };
	};
}
// --- META ---

CLASS_BASES_META(o2::ActorRef)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ActorRef)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mActor);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mWasDeleted);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRequiredResolveData);
}
END_META;
CLASS_METHODS_META(o2::ActorRef)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(Actor*);
	FUNCTION().PUBLIC().CONSTRUCTOR(const ActorRef&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, Get);
	FUNCTION().PUBLIC().SIGNATURE(const Actor*, Get);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Destroy);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsValid);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsWasDeleted);
	FUNCTION().PUBLIC().SIGNATURE(const Type&, GetActorType);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetActorTypeStatic);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, EqualsDelta, const ActorRef&, const ActorRef&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateSpecActor);
	FUNCTION().PROTECTED().SIGNATURE(void, CopyWithoutRemap, const ActorRef&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
