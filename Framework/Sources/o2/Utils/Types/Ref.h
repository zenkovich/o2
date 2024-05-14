#pragma once

#include <memory>
#include "Containers/Vector.h"

namespace o2
{
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

    protected:
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
        // Default constructor
        RefCounterable() {}

        // Constructor with reference counter
        RefCounterable(RefCounter* refCounter);

        // Virtual destructor, for inheritance
        virtual ~RefCounterable() {}

        // Returns strong references count
        UInt16 GetStrongReferencesCount() const;

        // Returns weak references count
        UInt16 GetWeakReferencesCount() const;

    protected:
        mutable RefCounter* mRefCounter = nullptr; // Pointer to reference counter

    protected:
        // Returns reference counter
        RefCounter* GetRefCounter() const;

        // Sets reference counter
        void SetRefCounter(RefCounter* refCounter);

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

		template<typename _object_type>
		friend struct RefCountersSetter;
    };

    // ---------------------------------------------------------------------------------
    // Reference counterable interface. Used for multiple reference counters inheritance
    // ---------------------------------------------------------------------------------
    class IRefCounterable
    {
    protected:
        // Returns reference counter
        virtual RefCounter* GetRefCounter() const = 0;

        template<typename _type>
        friend class BaseRef;

        template<typename _type>
        friend class BaseWeakRef;

        template<typename _type>
        friend RefCounter* GetRefCounterImpl(_type* ptr);

		template<typename _object_type>
        friend struct RefCountersSetter;
    };

    template<typename _object_ptr_type>
    struct RefCountersSetter
    {
        template<typename _type, typename ... _other_types>
        static void Set(_object_ptr_type object, RefCounter* refCounter)
        {
            static_cast<_type*>(object)->SetRefCounter(refCounter);

            if constexpr (sizeof...(_other_types) > 0)
                Set<_other_types...>(object, refCounter);
        }
    };

    // Reference counterable implementation to override GetRefCounter method
#define REF_COUNTERABLE_IMPL(BASE_CLASS, ...)                                                                                             \
        RefCounter* GetRefCounter() const { return BASE_CLASS::GetRefCounter(); }                                                         \
        void SetRefCounter(RefCounter* refCounter) { RefCountersSetter<decltype(this)>::Set<BASE_CLASS, __VA_ARGS__>(this, refCounter); } \
        template<typename _type> friend RefCounter* o2::GetRefCounterImpl(_type* ptr);                                                    \
        template<typename _object_type> friend struct o2::RefCountersSetter;                                                              \
        template<typename _type, typename ... _args> friend Ref<_type> o2::MakePlace(const char* location, int line, _args&& ... args)


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
        Ref(Ref<_type>&& other) : BaseRef<_type>(std::move(other)) {}

        // Copy constructor from other type
        template<typename _other_type, typename _enable_m = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
        Ref(const Ref<_other_type>& other) : BaseRef<_type>(other) {}

        // Copy constructor from other type
        template<typename _other_type, typename _enable_m = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
        Ref(Ref<_other_type>&& other) : BaseRef<_type>(std::move(other)) {}

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
        Ref<_type>& operator=(Ref<_type>&& other) { BaseRef<_type>::operator=(std::move(other)); return *this; }

        // Copy operator from other type
        template<typename _other_type, typename _enable_m = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
        Ref<_type>& operator=(const Ref<_other_type>& other) { BaseRef<_type>::operator=(other); return *this; }

        // Move operator from other type
        template<typename _other_type, typename _enable_m = std::enable_if<std::is_convertible<_other_type*, _type*>::value>::type>
        Ref<_type>& operator=(Ref<_other_type>&& other) { BaseRef<_type>::operator=(std::move(other)); return *this; }

        // Returns true if reference is valid
        bool IsValid() const { return BaseRef<_type>::IsValid(); }

        // Returns true if reference is valid
        operator bool() const { return BaseRef<_type>::operator bool(); }

        // Gets reference to object
        _type& operator*() const { return BaseRef<_type>::operator*(); }

        // Returns pointer to objec
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

    // Static check if type is complete
    template<typename T, typename = void>
    struct IsCompleteRef : std::false_type {};

    // Static check if type is complete
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
#define FORWARD_REF_IMPL(CLASS)                                                     \
	o2::RefCounter* GetRefCounterFwd(CLASS* ptr) { return GetRefCounterImpl(ptr); } \
	void DestructObjectFwd(CLASS* obj) { DestructObjectImpl(obj); }

    // Declares friend function for creating new object
#define FRIEND_REF_MAKE()                                                          \
    template<typename _type, typename ... _args>                                   \
    friend Ref<_type> MakePlace(const char* location, int line, _args&& ... args)

    // Static check for presence of PostRefConstruct method
    template<class T, class = std::void_t<>>
    struct HasPostRefConstruct : std::false_type {};

    // Static check for presence of PostRefConstruct method
    template<class T>
    struct HasPostRefConstruct<T, std::void_t<decltype(&T::PostRefConstruct)>> : std::true_type {};

    // Static check for presence of RefConstruct method with specified arguments
    template<typename T, typename... Args>
    struct HasRefConstructArgs
    {
    private:
        template<typename C>
        static auto test(int) -> decltype(std::declval<C>().template RefConstruct(std::declval<Args>()...), std::true_type{});

        template<typename>
        static auto test(...) -> std::false_type;

    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };

    // Makes new object and returns reference to it. Creates memory block with reference counter and object to keep them together. 
    // Stores location and line of creation for debug
    template<typename _type, typename ... _args>
    Ref<_type> MakePlace(const char* location, int line, _args&& ... args)
    {
        constexpr auto refSize = sizeof(RefCounter);
        constexpr auto typeSize = sizeof(_type);

        std::byte* memory = (std::byte*)_mmalloc(refSize + typeSize, location, line);
        auto refCounter = new (memory) RefCounter();
        refCounter->strongReferences += 1;

        _type* object;

        constexpr bool isConstructibleWithRefCounter = std::is_constructible<_type, RefCounter*, _args...>::value;
        if constexpr (isConstructibleWithRefCounter) 
		{
			object = new (memory + refSize) _type(refCounter, std::forward<_args>(args)...);
        }
        else
		{
			object = new (memory + refSize) _type(std::forward<_args>(args)...);
            object->SetRefCounter(refCounter);
        }

        if constexpr (HasRefConstructArgs<_type, _args ...>::value)
            object->RefConstruct(std::forward<_args>(args)...);

        if constexpr (HasPostRefConstruct<_type>::value)
            object->PostRefConstruct();

        refCounter->strongReferences -= 1;

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
	Ref(Ref<_other_type>&& other) : BaseRef<CLASS>(std::move(other)) {}																		\
	bool operator==(const Ref<CLASS>& other) const { return BaseRef<CLASS>::operator==(other); }											\
	bool operator==(const CLASS* other) const { return BaseRef<CLASS>::operator==(other); }											        \
	bool operator!=(const Ref<CLASS>& other) const { return BaseRef<CLASS>::operator!=(other); }											\
	bool operator!=(const CLASS* other) const { return BaseRef<CLASS>::operator!=(other); }											        \
	Ref<CLASS>& operator=(const Ref<CLASS>& other) { BaseRef<CLASS>::operator=(other); return *this; }										\
	Ref<CLASS>& operator=(Ref<CLASS>&& other) { BaseRef<CLASS>::operator=(std::move(other)); return *this; }								\
	template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, CLASS*>::value>::type>				\
	Ref<CLASS>& operator=(const Ref<_other_type>& other) { BaseRef<CLASS>::operator=(other); return *this; }								\
	template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, CLASS*>::value>::type>				\
	Ref<CLASS>& operator=(Ref<_other_type>&& other) { BaseRef<CLASS>::operator=(std::move(other)); return *this; }							\
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
        Ref<_type> Create(_args&& ... args)
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
                refCounter->strongReferences = 1;

                DestructObject(mPtr);

                refCounter->weakReferences--;
                refCounter->strongReferences = 0;

                mPtr = nullptr;

                if (refCounter->weakReferences == 0)
                    _mfree(refCounter);
            }
        }
    }
}
