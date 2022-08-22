#pragma once
#include "o2/Scene/ActorRef.h"
#include "o2/Utils/Function/Function.h"
#include "o2/Utils/Function/SerializableFunction.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class IActorSubscription : public ISerializableFunction
	{
	protected:
		ActorRef mActor;  // Target actor
		String   mMethod; // Method name
	};

	template <typename UnusedType>
	class ActorSubscription;

	template<typename _res_type, typename ... _args>
	class ActorSubscription<_res_type(_args ...)> : public IFunction<_res_type(_args ...)>, public IActorSubscription
	{
	public:
		// Constructor
		ActorSubscription()
		{}

		// Constructor
		ActorSubscription(const ActorRef& actor, const String& method)
		{
			mActor = actor;
			mMethod = method;
		}

		// Copy-constructor
		ActorSubscription(const ActorSubscription& other)
		{
			mActor = other.mActor;
			mMethod = other.mMethod;
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
		IFunction<_res_type(_args ...)>* MakeClone() const override
		{
			return mnew ActorSubscription(*this);
		}

		// Returns cloned emplace copy of this in memory
		IFunction<_res_type(_args ...)>* MakeClone(void* memory) const override
		{
			return new (memory) ActorSubscription(*this);
		}

		// Invokes function with arguments as functor
		_res_type Invoke(_args ... args) const override
		{
			if (auto actor = mActor.Get())
			{
				auto& objType = dynamic_cast<const ObjectType&>(actor->GetType());
				void* obj = objType.DynamicCastFromIObject(const_cast<IObject*>(dynamic_cast<const IObject*>(actor)));
				return objType.Invoke<_res_type, _args ...>(mMethod, obj, args ...);
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

		// Serializes a function
		void Serialize(DataValue& data) const override 
		{
			data["type"] = "ActorFunction";
			data["actor"] = mActor;
			data["func"] = mMethod;
		}

		// Deserializes a function
		void Deserialize(const DataValue& data) override 
		{
			mActor = data["actor"];
			mMethod = data["func"];
		}
	};
}

#include "o2/Utils/Function/FunctionDataValueConverter.h"
