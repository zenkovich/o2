#pragma once

#include <memory>
#include "Containers/Vector.h"

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
        RefCounter(const RefCounterImplementation* implementation) :
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
    public:
        // Returns strong references count
        UInt16 GetStrongReferencesCount() const;

        // Returns weak references count
        UInt16 GetWeakReferencesCount() const;

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
        friend RefCounter* GetRefCounterImpl(_type* ptr);

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
        BaseRef(std::nullptr_t);

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

        // Equality operator
        bool operator==(const BaseRef<_type>& other) const;

        // Equality operator
        bool operator==(const _type* other) const;

        // Inequality operator
        bool operator!=(const BaseRef<_type>& other) const;

        // Inequality operator
        bool operator!=(const _type* other) const;

        // Copy operator from nullptr
        BaseRef<_type>& operator=(std::nullptr_t);

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

        template<typename _other_type>
        friend class BaseWeakRef;

        template<typename _other_type>
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
        Ref(std::nullptr_t) : BaseRef<_type>(nullptr) {}

        // Constructor from pointer
        explicit Ref(_type* ptr) : BaseRef<_type>(ptr) {}

        // Copy constructor
        Ref(const Ref<_type>& other) : BaseRef<_type>(other) {}

        // Move constructor
        Ref(Ref<_type>&& other) : BaseRef<_type>(other) {}

        // Copy constructor from other type
        template<typename _other_type, typename _enable_m = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
        Ref(const Ref<_other_type>& other) : BaseRef<_type>(other) {}

        // Copy constructor from other type
        template<typename _other_type, typename _enable_m = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
        Ref(Ref<_other_type>&& other) : BaseRef<_type>(other) {}

        // Equality operator
        bool operator==(const Ref<_type>& other) const { return BaseRef<_type>::operator==(other); }

        // Equality operator
        bool operator==(const _type* other) const { return BaseRef<_type>::operator==(other); }

        // Inequality operator
        bool operator!=(const Ref<_type>& other) const { return BaseRef<_type>::operator!=(other); }

        // Inequality operator
        bool operator!=(const _type* other) const { return BaseRef<_type>::operator!=(other); }

        // Copy from nullptr operator
        Ref<_type>& operator=(std::nullptr_t) { BaseRef<_type>::operator=(nullptr); return *this; }

        // Copy operator 
        Ref<_type>& operator=(const Ref<_type>& other) { BaseRef<_type>::operator=(other); return *this; }

        // Move operator
        Ref<_type>& operator=(Ref<_type>&& other) { BaseRef<_type>::operator=(other); return *this; }

        // Copy operator from other type
        template<typename _other_type, typename _enable_m = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
        Ref<_type>& operator=(const Ref<_other_type>& other) { BaseRef<_type>::operator=(other); return *this; }

        // Move operator from other type
        template<typename _other_type, typename _enable_m = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
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

    // Returns reference counter
    template<typename _type>
    RefCounter* GetRefCounterImpl(_type* ptr)
    {
        return ptr->GetRefCounter();
    }

    // Destructs object
    template<typename _type>
    void DestructObjectImpl(_type* obj)
    {
        obj->~_type();
    }

    template<typename T, typename = void>
    struct IsCompleteRef : std::false_type {};

    template<typename T>
    struct IsCompleteRef<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

    // Returns reference counter
    template<typename _type>
    RefCounter* GetRefCounter(_type* ptr)
    {
        if constexpr (IsCompleteRef<_type>::value)
            return GetRefCounterImpl(ptr);
        else
            return GetRefCounterFwd(ptr);
    }

    // Destructs object
    template<typename _type>
    void DestructObject(_type* obj)
    {
        if constexpr (IsCompleteRef<_type>::value)
            return DestructObjectImpl(obj);
        else
            return DestructObjectFwd(obj);
    }

    // Forward declaration of class and reference counter implementation
#define FORWARD_REF(CLASS)                        \
	o2::RefCounter* GetRefCounterFwd(CLASS* ptr); \
	void DestructObjectFwd(CLASS* obj) 

    // Forward declaration of class and reference counter implementation
#define FORWARD_CLASS_REF(CLASS) \
    class CLASS;                 \
	FORWARD_REF(CLASS)

    // Implementation of forward declared reference counter 
#define FORWARD_REF_IMPL(CLASS)                                                   \
	o2::RefCounter* GetRefCounterFwd(CLASS* ptr) { return GetRefCounterImpl(ptr); } \
	void DestructObjectFwd(CLASS* obj) { DestructObjectImpl(obj); }

    // Makes new object and returns reference to it. Creates memory block with reference counter and object to keep them together. 
    // Stores location and line of creation for debug
    template<typename _type, typename ... _args>
    Ref<_type> MakePlace(const char* location, int line, _args&& ... args)
    {
        std::byte* memory = (std::byte*)_mmalloc(sizeof(RefCounter) + sizeof(_type), location, line);
        auto object = new (memory + sizeof(RefCounter)) _type(std::forward<_args>(args)...);

		// If object already has reference counter, we need to copy it to new memory block
        if (object->mRefCounter) 
        {
            UInt prevStrongReferences = object->mRefCounter->strongReferences;
            UInt prevWeakReferences = object->mRefCounter->weakReferences;

			(*object->mRefCounter->mImplementation->DestroyCounter)(object->mRefCounter);

			object->mRefCounter = new (memory) RefCounter(&LinkedRefCounterImplementation::Instance);
            object->mRefCounter->strongReferences = prevStrongReferences;
            object->mRefCounter->weakReferences = prevWeakReferences;
        }
        else
            object->mRefCounter = new (memory) RefCounter(&LinkedRefCounterImplementation::Instance);

        return Ref<_type>(object);
	}

	// Makes new object and returns reference to it. Creates memory block with reference counter and object to keep them together
	template<typename _type, typename ... _args>
	Ref<_type> Make(_args&& ... args)
	{
        return MakePlace<_type>(nullptr, 0, std::forward<_args>(args)...);
	}

	// Dynamic cast from one reference type to another
	template<typename _to_type, typename _from_type>
	Ref<_to_type> DynamicCast(const Ref<_from_type>& from)
	{
		return Ref<_to_type>(dynamic_cast<_to_type*>(const_cast<_from_type*>(from.Get())));
	}

	// Dynamic cast from one references vector type to another
	template<typename _to_type, typename _from_type>
	Vector<Ref<_to_type>> DynamicCastVector(const _from_type& from)
	{
		return from.Convert<Ref<_to_type>>([](auto& ref) { return DynamicCast<_to_type>(ref); });
	}

    // Static cast from one reference type to another
    template<typename _to_type, typename _from_type>
    Ref<_to_type> StaticCast(const Ref<_from_type>& from)
    {
        return Ref<_to_type>(static_cast<_to_type*>(from.Get()));
    }

    // Special macro for making new object and returning reference to it with storing location and line of creation for debug
#define mmake NewPlaceHelper(__FILE__, __LINE__).Create

    // Reference class implementation of BaseRef
#define BASE_REF_IMPLEMETATION(CLASS) 																										\
	Ref() : BaseRef<CLASS>() {}																												\
	Ref(nullptr_t) : BaseRef<CLASS>(nullptr) {}																								\
	explicit Ref(CLASS* ptr) : BaseRef<CLASS>(ptr) {}																						\
	Ref(const Ref<CLASS>& other) : BaseRef<CLASS>(other) {}																					\
	Ref(Ref<CLASS>&& other) : BaseRef<CLASS>(other) {}																						\
	template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, CLASS*>::value>::type>				\
	Ref(const Ref<_other_type>& other) : BaseRef<CLASS>(other) {}																			\
	template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, CLASS*>::value>::type>				\
	Ref(Ref<_other_type>&& other) : BaseRef<CLASS>(other) {}																				\
	bool operator==(const Ref<CLASS>& other) const { return BaseRef<CLASS>::operator==(other); }											\
	bool operator==(const CLASS* other) const { return BaseRef<CLASS>::operator==(other); }											        \
	bool operator!=(const Ref<CLASS>& other) const { return BaseRef<CLASS>::operator!=(other); }											\
	bool operator!=(const CLASS* other) const { return BaseRef<CLASS>::operator!=(other); }											        \
	Ref<CLASS>& operator=(const Ref<CLASS>& other) { BaseRef<CLASS>::operator=(other); return *this; }										\
	Ref<CLASS>& operator=(Ref<CLASS>&& other) { BaseRef<CLASS>::operator=(other); return *this; }											\
	template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, CLASS*>::value>::type>				\
	Ref<CLASS>& operator=(const Ref<_other_type>& other) { BaseRef<CLASS>::operator=(other); return *this; }								\
	template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, CLASS*>::value>::type>				\
	Ref<CLASS>& operator=(Ref<_other_type>&& other) { BaseRef<CLASS>::operator=(other); return *this; }										\
	bool IsValid() const { return BaseRef<CLASS>::IsValid(); }																				\
	operator bool() const { return BaseRef<CLASS>::operator bool(); }																		\
	CLASS& operator*() const { return BaseRef<CLASS>::operator*(); }																		\
	CLASS* operator->() const { return BaseRef<CLASS>::operator->(); }															

    // ------------------------------
    // Helper struct for mmake macros
    // ------------------------------
    struct NewPlaceHelper
    {
        const char* location;
        int line;

        NewPlaceHelper(const char* location, int line):location(location), line(line) {}

        template<typename _type, typename ... _args>
        Ref<_type> Create(_args ... args)
        {
            return MakePlace<_type>(location, line, std::forward<_args>(args)...);
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
    BaseRef<_type>::BaseRef(std::nullptr_t)
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
    bool BaseRef<_type>::operator==(const BaseRef<_type>& other) const
    {
        return mPtr == other.mPtr;
    }

    template<typename _type>
    bool BaseRef<_type>::operator==(const _type* other) const
    {
        return mPtr == other;
    }

    template<typename _type>
    bool BaseRef<_type>::operator!=(const BaseRef<_type>& other) const
    {
        return mPtr != other.mPtr;
    }

    template<typename _type>
    bool BaseRef<_type>::operator!=(const _type* other) const
    {
        return mPtr != other;
    }

    template<typename _type>
    BaseRef<_type>& BaseRef<_type>::operator=(std::nullptr_t)
    {
        DecrementRef();
        mPtr = nullptr;

        return *this;
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
                refCounter->weakReferences++;

                DestructObject(mPtr);
                using _type_no_const = std::remove_const<_type>::type;
                (*refCounter->mImplementation->DestroyObject)(const_cast<_type_no_const*>(mPtr));

                refCounter->weakReferences--;

                mPtr = nullptr;

                if (refCounter->weakReferences == 0)
                    (*refCounter->mImplementation->DestroyCounter)(refCounter);
            }
        }
    }
}
