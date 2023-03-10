#pragma once

#include <memory>

namespace o2
{
	struct RefCounterImplementation
	{
		void(*DestroyObject)(void*);
		void(*DestroyCounter)(void*);
	};

	struct DefaultRefCounterImplementation
	{
		static void DestroyObject(void* object)
		{
			free(object);
		}

		static void DestroyCounter(void* counter)
		{
			delete counter;
		}

		static constexpr RefCounterImplementation Instance = { &DestroyObject, &DestroyCounter };
	};

	struct LinkedRefCounterImplementation
	{
		static void DestroyObject(void* object)
		{
		}

		static void DestroyCounter(void* counter)  
		{
			free(counter);
		}

		static constexpr RefCounterImplementation Instance = { &DestroyObject, &DestroyCounter };
	}; 
	
	template<typename _type, typename _enable = void>
	class Ref;

	template<typename _type, typename ... _args>
	Ref<_type> New(_args&& ... args);

	struct RefCounter
	{
		UInt strongReferences = 0;
		UInt weakReferences = 0;

	public:
		RefCounter(const RefCounterImplementation* implementation):
			mImplementation(implementation)
		{}

	protected:
		const RefCounterImplementation* mImplementation = &DefaultRefCounterImplementation::Instance;

		template<typename _type>
		friend class BaseRef;

		template<typename _type>
		friend class BaseWeakRef;

		template<typename _type, typename ... _args>
		friend Ref<_type> New(_args&& ... args);
	};

	class RefCounterable
	{
	protected:
		mutable RefCounter* mRefCounter = nullptr;

	protected:
		RefCounter* GetRefCounter() const;

		template<typename _type>
		friend class BaseRef;

		template<typename _type>
		friend class BaseWeakRef;

		template<typename _type, typename ... _args>
		friend Ref<_type> New(_args&& ... args);
	};

	template<typename _type>
	class BaseRef
	{
	public:
		BaseRef();
		BaseRef(_type* ptr);
		BaseRef(const BaseRef<_type>& other);
		BaseRef(BaseRef<_type>&& other);

		~BaseRef();

		BaseRef& operator=(const BaseRef<_type>& other);
		BaseRef& operator=(const BaseRef<_type>&& other);
		BaseRef& operator=(_type* ptr);

		bool IsValid() const;

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Getter operator
		_type& operator*();

		// Constant getter operator
		const _type& operator*() const;

		// Asset members and field operator
		_type* operator->();

		// Constant asset members and field operator
		const _type* operator->() const;

	protected:
		_type* mPtr = nullptr;

	protected:
		void IncrementRef();
		void DecrementRef();

		template<typename _type>
		friend class BaseWeakRef;
	};

	template<typename _type>
	class BaseWeakRef
	{
	public:
		BaseWeakRef();
		BaseWeakRef(_type* ptr);
		BaseWeakRef(const BaseWeakRef<_type>& other);
		BaseWeakRef(BaseWeakRef<_type>&& other);
		BaseWeakRef(const BaseRef<_type>& other);
		BaseWeakRef(BaseRef<_type>&& other);

		~BaseWeakRef();

		BaseWeakRef& operator=(const BaseWeakRef<_type>& other);
		BaseWeakRef& operator=(const BaseWeakRef<_type>&& other);
		BaseWeakRef& operator=(const BaseRef<_type>& other);
		BaseWeakRef& operator=(const BaseRef<_type>&& other);
		BaseWeakRef& operator=(_type* ptr);

		bool IsValid() const;
		bool IsExpired() const;

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		BaseRef<_type> Lock() const;

	protected:
		_type* mPtr = nullptr;
		RefCounter* mRefCounter = nullptr;

	protected:
		void CheckRefCounter();
		void IncrementWeakRef();
		void DecrementWeakRef();

		template<typename _type>
		friend class BaseRef;
	};

	template<typename _type, typename _enable>
	class Ref : public BaseRef<_type>
	{
	public:
		Ref() : BaseRef<_type>() {}
		Ref(_type* ptr) : BaseRef<_type>(ptr) {}
		Ref(const Ref<_type>& other) : BaseRef<_type>(other) {}
		Ref(Ref<_type>&& other) : BaseRef<_type>(other) {}

		Ref& operator=(const Ref<_type>& other) { BaseRef<_type>::operator=(other); return *this; }
		Ref& operator=(const Ref<_type>&& other) { BaseRef<_type>::operator=(other); return *this; }
		Ref& operator=(_type* ptr) { BaseRef<_type>::operator=(ptr); return *this; }

		bool IsValid() const { return BaseRef<_type>::IsValid(); }

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return BaseRef<_type>::operator bool(); }

		// Getter operator
		_type& operator*() { return BaseRef<_type>::operator*(); }

		// Constant getter operator
		const _type& operator*() const { return BaseRef<_type>::operator*(); }

		// Asset members and field operator
		_type* operator->() { return BaseRef<_type>::operator->(); }

		// Constant asset members and field operator
		const _type* operator->() const { return BaseRef<_type>::operator->(); }
	};

	template<typename _type, typename _enable = void>
	class WeakRef : public BaseWeakRef<_type>
	{
	public:
		WeakRef() : BaseWeakRef<_type>() {}
		WeakRef(_type* ptr) : BaseWeakRef<_type>(ptr) {}
		WeakRef(const WeakRef<_type>& other) : BaseWeakRef<_type>(other) {}
		WeakRef(WeakRef<_type>&& other) : BaseWeakRef<_type>(other) {}
		WeakRef(const Ref<_type>& other) : BaseWeakRef<_type>(other) {}
		WeakRef(Ref<_type>&& other) : BaseWeakRef<_type>(other) {}

		WeakRef& operator=(const WeakRef<_type>& other) { BaseWeakRef<_type>::operator=(other); return *this; }
		WeakRef& operator=(const WeakRef<_type>&& other) { BaseWeakRef<_type>::operator=(other); return *this; }
		WeakRef& operator=(const Ref<_type>& other) { BaseWeakRef<_type>::operator=(other); return *this; }
		WeakRef& operator=(const Ref<_type>&& other) { BaseWeakRef<_type>::operator=(other); return *this; }
		WeakRef& operator=(_type* ptr) { BaseWeakRef<_type>::operator=(ptr); return *this; }

		bool IsValid() const { return BaseWeakRef<_type>::IsValid(); }
		bool IsExpired() const { return BaseWeakRef<_type>::IsExpired(); }

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return BaseWeakRef<_type>::operator bool(); }

		Ref<_type> Lock() const { return BaseWeakRef<_type>::Lock(); }
	};

	template<typename _type, typename ... _args>
	Ref<_type> New(_args&& ... args)
	{
		std::byte* memory = (std::byte*)malloc(sizeof(RefCounter) + sizeof(_type));
		auto object = new (memory + sizeof(RefCounter)) _type(std::forward<_args>(args)...);
		object->mRefCounter = new (memory) RefCounter(&LinkedRefCounterImplementation::Instance);

		return Ref<_type>(object);
	}

	// BaseRef implementation
	template<typename _type>
	BaseRef<_type>::BaseRef() = default;

	template<typename _type>
	BaseRef<_type>::BaseRef(_type* ptr) :
		mPtr(ptr)
	{
		IncrementRef();
	}

	template<typename _type>
	BaseRef<_type>::BaseRef(const BaseRef<_type>& other) :
		mPtr(other.mPtr)
	{
		IncrementRef();
	}

	template<typename _type>
	BaseRef<_type>::BaseRef(BaseRef<_type>&& other) :
		mPtr(other.mPtr)
	{
		other.mPtr = nullptr;
	}

	template<typename _type>
	BaseRef<_type>::~BaseRef()
	{
		DecrementRef();
	}

	template<typename _type>
	BaseRef<_type>& BaseRef<_type>::operator=(const BaseRef<_type>& other)
	{
		DecrementRef();

		mPtr = other.mPtr;

		IncrementRef();

		return *this;
	}

	template<typename _type>
	BaseRef<_type>& BaseRef<_type>::operator=(const BaseRef<_type>&& other)
	{
		DecrementRef();

		mPtr = other.mPtr;
		other.mPtr = nullptr;

		return *this;
	}

	template<typename _type>
	BaseRef<_type>& BaseRef<_type>::operator=(_type* ptr)
	{
		DecrementRef();

		mPtr = ptr;

		IncrementRef();

		return *this;
	}

	template<typename _type>
	bool BaseRef<_type>::IsValid() const
	{
		return mPtr != nullptr;
	}

	template<typename _type>
	BaseRef<_type>::operator bool() const
	{
		return IsValid();
	}

	template<typename _type>
	_type& BaseRef<_type>::operator*()
	{
		return *mPtr;
	}

	template<typename _type>
	const _type& BaseRef<_type>::operator*() const
	{
		return *mPtr;
	}

	template<typename _type>
	_type* BaseRef<_type>::operator->()
	{
		return mPtr;
	}

	template<typename _type>
	const _type* BaseRef<_type>::operator->() const
	{
		return mPtr;
	}

	template<typename _type>
	void BaseRef<_type>::IncrementRef()
	{
		if (mPtr)
			mPtr->GetRefCounter()->strongReferences++;
	}

	template<typename _type>
	void BaseRef<_type>::DecrementRef()
	{
		if (mPtr)
		{
			auto refCounter = mPtr->GetRefCounter();

			refCounter->strongReferences--;
			if (refCounter->strongReferences == 0)
			{
				mPtr->~_type();
				(*refCounter->mImplementation->DestroyObject)(mPtr);

				if (refCounter->weakReferences == 0)
					(*refCounter->mImplementation->DestroyCounter)(refCounter);
			}
		}
	}

	// BaseWeakRef implementation
	template<typename _type>
	BaseWeakRef<_type>::BaseWeakRef() = default;

	template<typename _type>
	BaseWeakRef<_type>::BaseWeakRef(_type* ptr) :
		mPtr(ptr)
	{
		CheckRefCounter();
		IncrementWeakRef();
	}

	template<typename _type>
	BaseWeakRef<_type>::BaseWeakRef(const BaseWeakRef<_type>& other) :
		mPtr(other.mPtr)
	{
		CheckRefCounter();
		IncrementWeakRef();
	}

	template<typename _type>
	BaseWeakRef<_type>::BaseWeakRef(BaseWeakRef<_type>&& other) :
		mPtr(other.mPtr)
	{
		CheckRefCounter();
		other.mPtr = nullptr;
	}

	template<typename _type>
	BaseWeakRef<_type>::BaseWeakRef(const BaseRef<_type>& other) :
		mPtr(other.mPtr)
	{
		CheckRefCounter();
		IncrementWeakRef();
	}

	template<typename _type>
	BaseWeakRef<_type>::BaseWeakRef(BaseRef<_type>&& other) :
		mPtr(other.mPtr)
	{
		CheckRefCounter();
		other.mPtr = nullptr;
	}

	template<typename _type>
	BaseWeakRef<_type>::~BaseWeakRef()
	{
		DecrementWeakRef();
	}

	template<typename _type>
	BaseWeakRef<_type>& BaseWeakRef<_type>::operator=(const BaseWeakRef<_type>& other)
	{
		DecrementWeakRef();

		mPtr = other.mPtr;
		CheckRefCounter();

		IncrementWeakRef();

		return *this;
	}

	template<typename _type>
	BaseWeakRef<_type>& BaseWeakRef<_type>::operator=(const BaseWeakRef<_type>&& other)
	{
		DecrementWeakRef();

		mPtr = other.mPtr;
		mRefCounter = other.mRefCounter;
		other.mPtr = nullptr;
		other.mRefCounter = nullptr;

		return *this;
	}

	template<typename _type>
	BaseWeakRef<_type>& BaseWeakRef<_type>::operator=(_type* ptr)
	{
		DecrementWeakRef();

		mPtr = ptr;

		IncrementWeakRef();

		return *this;
	}

	template<typename _type>
	bool BaseWeakRef<_type>::IsValid() const
	{
		return !IsExpired();
	}

	template<typename _type>
	bool BaseWeakRef<_type>::IsExpired() const
	{
		return mRefCounter ? mRefCounter->strongReferences == 0 : true;
	}

	template<typename _type>
	BaseWeakRef<_type>::operator bool() const
	{
		return IsValid();
	}

	template<typename _type>
	BaseRef<_type> BaseWeakRef<_type>::Lock() const
	{
		if (IsExpired())
			return BaseRef<_type>();

		return BaseRef<_type>(mPtr);
	}

	template<typename _type>
	void BaseWeakRef<_type>::CheckRefCounter()
	{
		mRefCounter = mPtr ? mPtr->GetRefCounter() : nullptr;
	}

	template<typename _type>
	void BaseWeakRef<_type>::IncrementWeakRef()
	{
		if (mRefCounter)
			mRefCounter->weakReferences++;
	}

	template<typename _type>
	void BaseWeakRef<_type>::DecrementWeakRef()
	{
		if (mRefCounter)
		{
			mRefCounter->weakReferences--;
			if (mRefCounter->strongReferences == 0)
			{
				if (mRefCounter->weakReferences == 0)
				{
					(*mRefCounter->mImplementation->DestroyCounter)(mRefCounter);
					mRefCounter = nullptr;
				}
			}
		}
	}
}
