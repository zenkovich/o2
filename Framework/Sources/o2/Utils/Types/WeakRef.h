#pragma once

#include "o2/Utils/Types/Ref.h"

namespace o2
{
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
        BaseWeakRef(std::nullptr_t);

        // Constructor from pointer
        explicit BaseWeakRef(_type* ptr);

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

        // Equality operator
        bool operator==(const BaseWeakRef<_type>& other) const;

        // Equality operator
        bool operator==(const _type* other) const;

        // Inequality operator
        bool operator!=(const BaseWeakRef<_type>& other) const;

        // Inequality operator
        bool operator!=(const _type* other) const;

        // Copy from nullptr operator
        BaseWeakRef& operator=(std::nullptr_t);

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
        _type* mPtr = nullptr;        // Pointer to object
        RefCounter* mRefCounter = nullptr; // Pointer to reference counter

    protected:
        void CheckRefCounter();  // Gets reference counter if pointer is valid
        void IncrementWeakRef(); // Increments weak reference counter
        void DecrementWeakRef(); // Decrements weak reference counter

        template<typename _other_type>
        friend class BaseRef;
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
        WeakRef(std::nullptr_t) : BaseWeakRef<_type>(nullptr) {}

        // Constructor from pointer
        explicit WeakRef(_type* ptr) : BaseWeakRef<_type>(ptr) {}

        // Copy constructor
        WeakRef(const WeakRef<_type>& other) : BaseWeakRef<_type>(other) {}

        // Move constructor
        WeakRef(WeakRef<_type>&& other) : BaseWeakRef<_type>(other) {}

        // Copy constructor from strong reference
        WeakRef(const Ref<_type>& other) : BaseWeakRef<_type>(other) {}

        // Move constructor from strong reference
        WeakRef(Ref<_type>&& other) : BaseWeakRef<_type>(other) {}

        // Equality operator
        bool operator==(const WeakRef<_type>& other) const { return BaseWeakRef<_type>::operator==(other); }

        // Equality operator
        bool operator==(const _type* other) const { return BaseWeakRef<_type>::operator==(other); }

        // Inequality operator
        bool operator!=(const WeakRef<_type>& other) const { return BaseWeakRef<_type>::operator!=(other); }

        // Inequality operator
        bool operator!=(const _type* other) const { return BaseWeakRef<_type>::operator!=(other); }

        // Copy operator from nullptr
        WeakRef& operator=(std::nullptr_t) { BaseWeakRef<_type>::operator=(nullptr); return *this; }

        // Copy operator
        WeakRef& operator=(const WeakRef<_type>& other) { BaseWeakRef<_type>::operator=(other); return *this; }

        // Move operator
        WeakRef& operator=(WeakRef<_type>&& other) { BaseWeakRef<_type>::operator=(other); return *this; }

        // Copy operator from strong reference
        WeakRef& operator=(const Ref<_type>& other) { BaseWeakRef<_type>::operator=(other); return *this; }

        // Move operator from strong reference
        WeakRef& operator=(Ref<_type>&& other) { BaseWeakRef<_type>::operator=((BaseRef<_type>&)other); return *this; }

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

    // BaseWeakRef implementation
    template<typename _type>
    BaseWeakRef<_type>::BaseWeakRef() = default;

    template<typename _type>
    BaseWeakRef<_type>::BaseWeakRef(std::nullptr_t)
    {}

    template<typename _type>
    BaseWeakRef<_type>::BaseWeakRef(_type* ptr) :
        mPtr(ptr)
    {
        CheckRefCounter();
        IncrementWeakRef();
    }

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
    bool BaseWeakRef<_type>::operator==(const BaseWeakRef<_type>& other) const
    {
        return mPtr == other.mPtr;
    }

    template<typename _type>
    bool BaseWeakRef<_type>::operator==(const _type* other) const
    {
        return mPtr == other;
    }

    template<typename _type>
    bool BaseWeakRef<_type>::operator!=(const BaseWeakRef<_type>& other) const
    {
        return mPtr != other.mPtr;
    }

    template<typename _type>
    bool BaseWeakRef<_type>::operator!=(const _type* other) const
    {
        return mPtr != other;
    }

    template<typename _type>
    BaseWeakRef<_type>& BaseWeakRef<_type>::operator=(std::nullptr_t)
    {
        DecrementWeakRef();

        mPtr = nullptr;
        mRefCounter = nullptr;

        return *this;
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
                    _mfree(mRefCounter);
                    mRefCounter = nullptr;
                    mPtr = nullptr;
                }
            }
        }
    }
}
