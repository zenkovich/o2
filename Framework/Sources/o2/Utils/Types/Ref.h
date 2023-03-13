#pragma once

#include <memory>

namespace o2
{
	// -------------------------------------------------------- 
	// Reference counters types implementation container
	// Contains function pointers to destroy object and counter
	// --------------------------------------------------------
	struct RefCounterImplementation
	{
		void(*DestroyObject)(void*);  // Function pointer to destroy object
		void(*DestroyCounter)(void*); // Function pointer to destroy counter
	};

	// -----------------------------------------
	// Default reference counters implementation
	// -----------------------------------------
	struct DefaultRefCounterImplementation
	{
		// Default destroy object function
		static void DestroyObject(void* object)
		{
			free(object);
		}

		// Default destroy counter function
		static void DestroyCounter(void* counter)
		{
			delete counter;
		}

		static constexpr RefCounterImplementation Instance = { &DestroyObject, &DestroyCounter };
	};

	// ----------------------------------------------------------------------------
	// Reference counter implementation for object and counter in same memory block
	// ----------------------------------------------------------------------------
	struct LinkedRefCounterImplementation
	{
		// Destroy object function, does nothing
		static void DestroyObject(void* object)
		{}

		// Destroy counter function, frees memory block of object and counter
		static void DestroyCounter(void* counter)  
		{
			free(counter);
		}

		static constexpr RefCounterImplementation Instance = { &DestroyObject, &DestroyCounter };
	}; 
	
	template<typename _type, typename _enable = void>
	class Ref;

	template<typename _type, typename ... _args>
	Ref<_type> Make(_args&& ... args);

	// --------------------------------
	// Reference counter implementation
	// --------------------------------
	struct RefCounter
	{
		UInt strongReferences = 0; // Strong references count
		UInt weakReferences = 0;   // Weak references count

	public:
		// Default constructor
		RefCounter(const RefCounterImplementation* implementation):
			mImplementation(implementation)
		{}

	protected:
		const RefCounterImplementation* mImplementation = &DefaultRefCounterImplementation::Instance; // It's pointer to function pointers to destroy object and counter

		template<typename _type>
		friend class BaseRef;

		template<typename _type>
		friend class BaseWeakRef;

		template<typename _type, typename ... _args>
		friend Ref<_type> Make(_args&& ... args);

		template<typename _type, typename ... _args>
		friend Ref<_type> MakePlace(const char* location, int line, _args&& ... args);
	};

	// -------------------------------------------------------------
	// Base reference counterable object. 
	// Use it as base class for your objects, that can be referenced
	// -------------------------------------------------------------
	class RefCounterable
	{
	protected:
		mutable RefCounter* mRefCounter = nullptr; // Pointer to reference counter. Creates when first reference created

	protected:
		// Returns reference counter, creates if not created
		RefCounter* GetRefCounter() const;

		template<typename _type>
		friend class BaseRef;

		template<typename _type>
		friend class BaseWeakRef;

		template<typename _type>
		friend RefCounter* GetRefCounter(_type* ptr);

		template<typename _type, typename ... _args>
		friend Ref<_type> Make(_args&& ... args);

		template<typename _type, typename ... _args>
		friend Ref<_type> MakePlace(const char* location, int line, _args&& ... args);
	};

	// -------------------------------
	// Base strong reference to object
	// -------------------------------
	template<typename _type>
	class BaseRef
	{
	public:
		// Default constructor
		BaseRef();

		// Constructor from pointer
		explicit BaseRef(_type* ptr);

		// Constructor from nullptr
		BaseRef(nullptr_t);

		// Copy constructor
		BaseRef(const BaseRef<_type>& other);

		// Move constructor
		BaseRef(BaseRef<_type>&& other);

		// Copy constructor from other type
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
		BaseRef(const BaseRef<_other_type>& other);

		// Copy constructor from other type
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
		BaseRef(BaseRef<_other_type>&& other);

		// Destructor
		~BaseRef();

		// Copy operator
		BaseRef<_type>& operator=(const BaseRef<_type>& other);

		// Move operator
		BaseRef<_type>& operator=(BaseRef<_type>&& other);

		// Copy operator from other type
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
		BaseRef<_type>& operator=(const BaseRef<_other_type>& other);

		// Move operator from other type
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
		BaseRef<_type>& operator=(BaseRef<_other_type>&& other);

		// Returns true if reference is valid
		bool IsValid() const;

		// Returns pointer to object
		_type* Get() const;
				
		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Getter operator
		_type& operator*() const;

		// Pointer getter operator
		_type* operator->() const;

	protected:
		_type* mPtr = nullptr; // Pointer to object

	protected:
		void IncrementRef(); // Increments reference counter
		void DecrementRef(); // Decrements reference counter

		template<typename _type>
		friend class BaseWeakRef;

		template<typename _other_type>
		friend class BaseRef;
	};

	// -----------------------------
	// Base weak reference to object
	// -----------------------------
	template<typename _type>
	class BaseWeakRef
	{
	public:
		// Default constructor
		BaseWeakRef();

		// Constructor from nullptr
		BaseWeakRef(nullptr_t);

		// Copy constructor
		BaseWeakRef(const BaseWeakRef<_type>& other);

		// Move constructor
		BaseWeakRef(BaseWeakRef<_type>&& other);

		// Copy constructor from strong reference
		BaseWeakRef(const BaseRef<_type>& other);

		// Move constructor from strong reference
		BaseWeakRef(BaseRef<_type>&& other);

		// Destructor
		~BaseWeakRef();

		// Copy operator
		BaseWeakRef& operator=(const BaseWeakRef<_type>& other);

		// Move operator
		BaseWeakRef& operator=(BaseWeakRef<_type>&& other);

		// Copy operator from strong reference
		BaseWeakRef& operator=(const BaseRef<_type>& other);

		// Move operator from strong reference
		BaseWeakRef& operator=(BaseRef<_type>&& other);

		// Returns true if reference is valid
		bool IsValid() const;

		// Returns true if reference is expired
		bool IsExpired() const;

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Returns strong reference to object
		BaseRef<_type> Lock() const;

	protected:
		_type*      mPtr = nullptr;        // Pointer to object
		RefCounter* mRefCounter = nullptr; // Pointer to reference counter

	protected:
		void CheckRefCounter();  // Gets reference counter if pointer is valid
		void IncrementWeakRef(); // Increments weak reference counter
		void DecrementWeakRef(); // Decrements weak reference counter

		template<typename _type>
		friend class BaseRef;
	};

	// ----------------
	// Strong reference
	// ----------------
	template<typename _type, typename _enable>
	class Ref : public BaseRef<_type>
	{
	public:
		// Default constructor
		Ref() : BaseRef<_type>() {}

		// Constructor from nullptr
		Ref(nullptr_t) : BaseRef<_type>(nullptr) {}

		// Constructor from pointer
		explicit Ref(_type* ptr) : BaseRef<_type>(ptr) {}

		// Copy constructor
		Ref(const Ref<_type>& other) : BaseRef<_type>(other) {}

		// Move constructor
		Ref(Ref<_type>&& other) : BaseRef<_type>(other) {}

		// Copy constructor from other type
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
		Ref(const Ref<_other_type>& other) : BaseRef<_type>(other) {}

		// Copy constructor from other type
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
		Ref(Ref<_other_type>&& other) : BaseRef<_type>(other) {}

		// Copy operator 
		Ref<_type>& operator=(const Ref<_type>& other) { BaseRef<_type>::operator=(other); return *this; }

		// Move operator
		Ref<_type>& operator=(Ref<_type>&& other) { BaseRef<_type>::operator=(other); return *this; }

		// Copy operator from other type
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
		Ref<_type>& operator=(const Ref<_other_type>& other) { BaseRef<_type>::operator=(other); return *this; }

		// Move operator from other type
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
		Ref<_type>& operator=(Ref<_other_type>&& other) { BaseRef<_type>::operator=(other); return *this; }

		// Returns true if reference is valid
		bool IsValid() const { return BaseRef<_type>::IsValid(); }

		// Returns true if reference is valid
		operator bool() const { return BaseRef<_type>::operator bool(); }

		// Gets reference to object
		_type& operator*() const { return BaseRef<_type>::operator*(); }

		// Returns pointer to object
		_type* operator->() const { return BaseRef<_type>::operator->(); }
	};

	// --------------
	// Weak reference
	// --------------
	template<typename _type, typename _enable = void>
	class WeakRef : public BaseWeakRef<_type>
	{
	public:
		// Default constructor
		WeakRef() : BaseWeakRef<_type>() {}

		// Constructor from nullptr
		WeakRef(nullptr_t) : BaseWeakRef<_type>(nullptr) {}

		// Copy constructor
		WeakRef(const WeakRef<_type>& other) : BaseWeakRef<_type>(other) {}

		// Move constructor
		WeakRef(WeakRef<_type>&& other) : BaseWeakRef<_type>(other) {}

		// Copy constructor from strong reference
		WeakRef(const Ref<_type>& other) : BaseWeakRef<_type>(other) {}

		// Move constructor from strong reference
		WeakRef(Ref<_type>&& other) : BaseWeakRef<_type>(other) {}

		// Copy operator
		WeakRef& operator=(const WeakRef<_type>& other) { BaseWeakRef<_type>::operator=(other); return *this; }

		// Move operator
		WeakRef& operator=(WeakRef<_type>&& other) { BaseWeakRef<_type>::operator=(other); return *this; }

		// Copy operator from strong reference
		WeakRef& operator=(const Ref<_type>& other) { BaseWeakRef<_type>::operator=(other); return *this; }

		// Move operator from strong reference
		WeakRef& operator=(Ref<_type>&& other) { BaseWeakRef<_type>::operator=(other); return *this; }

		// Returns true if reference is valid
		bool IsValid() const { return BaseWeakRef<_type>::IsValid(); }

		// Returns true if reference is expired
		bool IsExpired() const { return BaseWeakRef<_type>::IsExpired(); }

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return BaseWeakRef<_type>::operator bool(); }

		// Returns strong reference to object
		Ref<_type> Lock() const
		{
			if (BaseWeakRef<_type>::IsExpired())
				return Ref<_type>();

			return Ref<_type>(BaseWeakRef<_type>::mPtr);;
		}
	};

	// Returns reference counter
	template<typename _type>
	inline RefCounter* GetRefCounter(_type* ptr)
	{
		return ptr->GetRefCounter();
	}

	// Destructs object
	template<typename _type>
	inline void DestructObject(_type* obj)
	{
		obj->~_type();
	}

	// Forward declaration of class and reference counter implementation
#define FORWARD_REF(CLASS) \
	class CLASS; \
	template<> o2::RefCounter* GetRefCounter(CLASS* ptr); \
	template<> void DestructObject(CLASS* obj) 

	// Implementation of forward declared reference counter 
#define FORWARD_REF_IMPL(CLASS) \
	template<> inline o2::RefCounter* GetRefCounter(CLASS* ptr) { return ptr->GetRefCounter(); } \
	template<> inline void DestructObject(CLASS* obj) { obj->~CLASS(); }

	// Makes new object and returns reference to it. Creates memory block with reference counter and object to keep them together
	template<typename _type, typename ... _args>
	Ref<_type> Make(_args&& ... args)
	{
		std::byte* memory = (std::byte*)malloc(sizeof(RefCounter) + sizeof(_type));
		auto object = new (memory + sizeof(RefCounter)) _type(std::forward<_args>(args)...);
		object->mRefCounter = new (memory) RefCounter(&LinkedRefCounterImplementation::Instance);

		return Ref<_type>(object);
	}

	// Makes new object and returns reference to it. Creates memory block with reference counter and object to keep them together. 
	// Stores location and line of creation for debug
	template<typename _type, typename ... _args>
	Ref<_type> MakePlace(const char* location, int line, _args&& ... args)
	{
		std::byte* memory = (std::byte*)_mmalloc(sizeof(RefCounter) + sizeof(_type), location, line);
		auto object = new (memory + sizeof(RefCounter)) _type(std::forward<_args>(args)...);
		object->mRefCounter = new (memory) RefCounter(&LinkedRefCounterImplementation::Instance);

		return Ref<_type>(object);
	}

	// Dynamic cast from one reference type to another
	template<typename _from_type, typename _to_type>
	Ref<_to_type> DynamicCast(const Ref<_from_type>& from)
	{
		return Ref<_to_type>(dynamic_cast<_to_type*>(from.Get()));
	}

	// Static cast from one reference type to another
	template<typename _from_type, typename _to_type>
	Ref<_to_type> StaticCast(const Ref<_from_type>& from)
	{
		return Ref<_to_type>(static_cast<_to_type*>(from.Get()));
	}

	// Special macro for making new object and returning reference to it with storing location and line of creation for debug
#define mmake NewPlaceHelper(__FILE__, __LINE__).Create

	// ------------------------------
	// Helper struct for mmake macros
	// ------------------------------
	struct NewPlaceHelper
	{
		const char* location;
		int line;

		NewPlaceHelper(const char* location, int line) :location(location), line(line) {}

		template<typename _type, typename ... _args>
		Ref<_type> Create(_args&& ... args)
		{
			return MakePlace<_type, _args...>(location, line, std::forward<_args>(args)...);
		}
	};

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
	BaseRef<_type>::BaseRef(nullptr_t)
	{}

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
	template<typename _other_type, typename _enable>
	BaseRef<_type>::BaseRef(const BaseRef<_other_type>& other) :
		mPtr(other.mPtr)
	{
		IncrementRef();
	}

	template<typename _type>
	template<typename _other_type, typename _enable>
	BaseRef<_type>::BaseRef(BaseRef<_other_type>&& other) :
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
	template<typename _other_type, typename _enable>
	BaseRef<_type>& BaseRef<_type>::operator=(const BaseRef<_other_type>& other)
	{
		DecrementRef();

		mPtr = other.mPtr;

		IncrementRef();

		return *this;
	}

	template<typename _type>
	template<typename _other_type, typename _enable>
	BaseRef<_type>& BaseRef<_type>::operator=(BaseRef<_other_type>&& other)
	{
		DecrementRef();

		mPtr = other.mPtr;
		other.mPtr = nullptr;

		return *this;
	}

	template<typename _type>
	BaseRef<_type>& BaseRef<_type>::operator=(BaseRef<_type>&& other)
	{
		DecrementRef();

		mPtr = other.mPtr;
		other.mPtr = nullptr;

		return *this;
	}

	template<typename _type>
	_type* BaseRef<_type>::Get() const
	{
		return mPtr;
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
	_type& BaseRef<_type>::operator*() const
	{
		return *mPtr;
	}

	template<typename _type>
	_type* BaseRef<_type>::operator->() const
	{
		return mPtr;
	}

	template<typename _type>
	void BaseRef<_type>::IncrementRef()
	{
		if (mPtr)
			GetRefCounter(mPtr)->strongReferences++;
	}

	template<typename _type>
	void BaseRef<_type>::DecrementRef()
	{
		if (mPtr)
		{
			auto refCounter = GetRefCounter(mPtr);

			refCounter->strongReferences--;
			if (refCounter->strongReferences == 0)
			{
				DestructObject(mPtr);
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
	BaseWeakRef<_type>::BaseWeakRef(nullptr_t)
	{}

	template<typename _type>
	BaseWeakRef<_type>::BaseWeakRef(const BaseWeakRef<_type>& other) :
		mPtr(other.mPtr), mRefCounter(other.mRefCounter)
	{
		IncrementWeakRef();
	}

	template<typename _type>
	BaseWeakRef<_type>::BaseWeakRef(BaseWeakRef<_type>&& other) :
		mPtr(other.mPtr), mRefCounter(other.mRefCounter)
	{
		other.mPtr = nullptr;
		other.mRefCounter = nullptr;
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
	BaseWeakRef<_type>& BaseWeakRef<_type>::operator=(BaseWeakRef<_type>&& other)
	{
		DecrementWeakRef();

		mPtr = other.mPtr;
		mRefCounter = other.mRefCounter;
		other.mPtr = nullptr;
		other.mRefCounter = nullptr;

		return *this;
	}

	template<typename _type>
	BaseWeakRef<_type>& BaseWeakRef<_type>::operator=(const BaseRef<_type>& other)
	{
		DecrementWeakRef();

		mPtr = other.mPtr;
		CheckRefCounter();

		IncrementWeakRef();

		return *this;
	}

	template<typename _type>
	BaseWeakRef<_type>& BaseWeakRef<_type>::operator=(BaseRef<_type>&& other)
	{
		DecrementWeakRef();

		mPtr = other.mPtr;
		CheckRefCounter();
		other.mPtr = nullptr;

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
		mRefCounter = mPtr ? GetRefCounter(mPtr) : nullptr;
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
