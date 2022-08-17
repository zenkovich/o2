#pragma once
#include "o2/Utils/Function.h"

namespace o2
{
	template <typename UnusedType>
	class ActorSubscription;

	template<typename _res_type, typename ... _args>
	class ActorSubscription<_res_type(_args ...)> : public IFunction<_res_type(_args ...)>
	{
		ActorRef mActor;
		String   mMethod;

	public:
		// Constructor
		ActorSubscription(const ActorRef& actor, const String& method) :
			mActor(actor), mMethod(method)
		{
		}

		// Copy-constructor
		ActorSubscription(const Subscription& other) :
			mActor(other.mActor), mMethod(other.mMethod)
		{
		}

		// Copy-operator
		ActorSubscription& operator=(const ActorSubscription& other)
		{
			mActor = other.mActor;
			mMethod = other.mMethod;

			return *this;
		}

		// Equal operator
		bool operator==(const ActorSubscription& other) const
		{
			return mActor == other.mActor && mMethod == other.mMethod;
		}

		// Not equal operator
		bool operator!=(const ActorSubscription& other) const
		{
			return mActor != other.mActor || mMethod != other.mMethod;
		}

		// Returns cloned copy of this
		IFunction<_res_type(_args ...)>* Clone() const override
		{
			return mnew ActorSubscription(*this);
		}

		// Returns cloned emplace copy of this in memory
		IFunction<_res_type(_args ...)>* Clone(void* memory) const override
		{
			return new (memory) ActorSubscription(*this);
		}

		// Invokes function with arguments as functor
		_res_type Invoke(_args ... args) const override
		{
			if (auto actor = mActor.Get())
			{
				auto& objType = dynamic_cast<const ObjectType&>(actor->GetType());
				return objType.Invoke<_res_type>(mMethod, objType->DynamicCastFromIObject(dynamic_cast<IObject*>(actor)), args ...);
			}

			return _res_type();
		}

		// Returns true if functions is equal
		bool Equals(const IFunction<_res_type(_args ...)>* other) const override
		{
			const ActorSubscription* otherFuncPtr = dynamic_cast<const ActorSubscription*>(other);
			if (otherFuncPtr)
				return *otherFuncPtr == *this;

			return false;
		}

		// Returns size of function
		UInt GetSizeOf() const override
		{
			return sizeof(*this);
		}
	};
}
