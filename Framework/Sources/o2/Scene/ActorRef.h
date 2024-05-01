#pragma once

#include "o2/Scene/ActorCreationMode.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
    class Actor;

    // --------------------------------------------------------------
    // Actor reference, automatically invalidates when actor deleting
    // --------------------------------------------------------------
    class BaseActorRef : public ISerializable
    {
    public:
        // Default constructor, no reference
        BaseActorRef();

        // Creates a copy of actor and returns reference on it
        BaseActorRef(const BaseActorRef& other);

        // Destructor
        ~BaseActorRef();

        // Assign operator
        BaseActorRef& operator=(const BaseActorRef& other);

        // Returns actor pointer @SCRIPTABLE
        virtual Actor* Get();

        // Returns actor pointer
        virtual const Actor* Get() const;

        // Sets actor pointer
        virtual void Set(Actor* actor);

        // Destroy the actor @SCRIPTABLE
        void Destroy();

        // Returns is reference is valid @SCRIPTABLE
        bool IsValid() const;

        // Returns actor type
        virtual const Type& GetActorType() const;

        // Copying ref without requiring remap
        virtual void CopyWithoutRemap(const BaseActorRef& other);

        // Returns actor type
        static const Type* GetActorTypeStatic();

        // Checks refs are equals for serializing delta
        static bool EqualsDelta(const BaseActorRef& obj, const BaseActorRef& origin);

        SERIALIZABLE(BaseActorRef);

    protected:
        // ------------------------------
        // Reference resolve request data
        // ------------------------------
        struct IRequiredResolveData : public RefCounterable
        {
            // Request resolve reference
            virtual void RequireResolve(BaseActorRef& ref) = 0;

            // Clone data
            virtual Ref<IRequiredResolveData> Clone() const = 0;
        };

        // -------------------------------------------------
        // Reference resolve request data by actor scene uid
        // -------------------------------------------------
        struct SceneRequireResolveData : public IRequiredResolveData
        {
            SceneUID uid;

            // Request resolve reference
            void RequireResolve(BaseActorRef& ref) override;

            // Clone data
            Ref<IRequiredResolveData> Clone() const override;
        };

        // -------------------------------------------
        // Reference resolve request data by asset uid
        // -------------------------------------------
        struct AssetRequireResolveData : public IRequiredResolveData
        {
            UID uid;

            // Request resolve reference
            void RequireResolve(BaseActorRef& ref) override;

            // Clone data
            Ref<IRequiredResolveData> Clone() const override;
        };

        Ref<IRequiredResolveData> mRequiredResolveData; // Reference resolve request data. Used for resolving reference after deserialization.
        // Not null only when reference is required to resolve. Copies in reference copying.

    protected:
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
    template<typename _actor_type>
    class ActorRef : public BaseActorRef
    {
        static_assert(std::is_base_of<Actor, _actor_type>::value, "ActorRef type must be derived from Actor");

        using Base = BaseRef<_actor_type>;

    public:
        // Default constructor, no reference
        ActorRef();

        // Nullptr constructor
        ActorRef(nullptr_t);

        // Constructor with actor pointer
        explicit ActorRef(_actor_type* ptr);

        // Constructor with actor reference
        explicit ActorRef(const Ref<_actor_type>& ref);

        // Copy constructor
        ActorRef(const ActorRef<_actor_type>& other);

        // Move constructor
        ActorRef(ActorRef<_actor_type>&& other);

        // Copy constructor from other actor reference
        template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _actor_type*>::value>::type>
        ActorRef(const ActorRef<_other_type>& other);

        // Move constructor from other actor reference
        template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _actor_type*>::value>::type>
        ActorRef(ActorRef<_other_type>&& other);

        // Equality operator
        bool operator==(const ActorRef<_actor_type>& other) const;

        // Equality operator
        bool operator==(const _actor_type* other) const;

        // Inequality operator
        bool operator!=(const ActorRef<_actor_type>& other) const;

        // Inequality operator
        bool operator!=(const _actor_type* other) const;

        // Copy operator
        ActorRef<_actor_type>& operator=(const ActorRef<_actor_type>& other);

        // Move operator
        ActorRef<_actor_type>& operator=(ActorRef<_actor_type>&& other);

        // Copy operator from other actor reference
        template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _actor_type*>::value>::type>
        ActorRef<_actor_type>& operator=(const ActorRef<_other_type>& other);

        // Move operator from other actor reference
        template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _actor_type*>::value>::type>
        ActorRef<_actor_type>& operator=(Ref<_other_type>&& other);

        // Returns is reference is valid
        bool IsValid() const;

        // Returns is reference is valid
        operator bool() const;

        // Returns actor reference
        _actor_type& operator*() const;

        // Returns actor pointer
        _actor_type* operator->() const;

        // Returns actor pointer 
        _actor_type* Get() override;

        // Returns actor pointer
        const _actor_type* Get() const override;

        // Sets actor pointer
        void Set(Actor* actor) override;

        // Returns actor type
        const Type& GetActorType() const override;

        // Copying ref without requiring remap
        void CopyWithoutRemap(const BaseActorRef& other) override;

        // Returns actor type
        static const Type* GetActorTypeStatic();

    protected:
        Ref<_actor_type> mRef; // Reference to actor

    public:
        typedef ActorRef<_actor_type> _thisType;

        SERIALIZABLE_MAIN(_thisType);
        IOBJECT_SCRIPTING();

        template<typename _type_processor>
        static void ProcessBaseTypes(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartBases<_thisType>(object, type);

            BASE_CLASS(o2::BaseActorRef);
        }

        template<typename _type_processor>
        static void ProcessFields(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartFields<_thisType>(object, type);

            FIELD().PROTECTED().NAME(mRef);
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
    };

    template<typename _actor_type>
    ActorRef<_actor_type>::ActorRef()
    {}

    template<typename _actor_type>
    ActorRef<_actor_type>::ActorRef(nullptr_t) :
        mRef(nullptr)
    {}

    template<typename _actor_type>
    ActorRef<_actor_type>::ActorRef(_actor_type* ptr) :
        mRef(ptr)
    {}

    template<typename _actor_type>
    ActorRef<_actor_type>::ActorRef(const Ref<_actor_type>& ref) :
        mRef(ref)
    {}

    template<typename _actor_type>
    ActorRef<_actor_type>::ActorRef(const ActorRef<_actor_type>& other) :
        mRef(other.mRef)
    {}

    template<typename _actor_type>
    ActorRef<_actor_type>::ActorRef(ActorRef<_actor_type>&& other) :
        mRef(other.mRef)
    {}

    template<typename _actor_type>
    template<typename _other_type, typename _enable>
    ActorRef<_actor_type>::ActorRef(const ActorRef<_other_type>& other) :
        mRef(other.mRef)
    {}

    template<typename _actor_type>
    template<typename _other_type, typename>
    ActorRef<_actor_type>::ActorRef(ActorRef<_other_type>&& other) :
        mRef(std::move(other.mRef))
    {}

    template<typename _actor_type>
    bool ActorRef<_actor_type>::operator==(const ActorRef<_actor_type>& other) const
    {
        return mRef == other.mRef;
    }

    template<typename _actor_type>
    bool ActorRef<_actor_type>::operator==(const _actor_type* other) const
    {
        return mRef == other;
    }

    template<typename _actor_type>
    bool ActorRef<_actor_type>::operator!=(const ActorRef<_actor_type>& other) const
    {
        return mRef != other.mRef;
    }

    template<typename _actor_type>
    bool ActorRef<_actor_type>::operator!=(const _actor_type* other) const
    {
        return mRef != other;
    }

    template<typename _actor_type>
    ActorRef<_actor_type>& ActorRef<_actor_type>::operator=(const ActorRef<_actor_type>& other)
    {
        mRef = other.mRef;
        return *this;
    }

    template<typename _actor_type>
    ActorRef<_actor_type>& ActorRef<_actor_type>::operator=(ActorRef<_actor_type>&& other)
    {
        mRef = std::move(other.mRef);
        return *this;
    }

    template<typename _actor_type>
    template<typename _other_type, typename _enable>
    ActorRef<_actor_type>& ActorRef<_actor_type>::operator=(const ActorRef<_other_type>& other)
    {
        mRef = other.mRef;
        return *this;
    }

    template<typename _actor_type>
    template<typename _other_type, typename _enable>
    ActorRef<_actor_type>& ActorRef<_actor_type>::operator=(Ref<_other_type>&& other)
    {
        mRef = std::move(other.mRef);
        return *this;
    }

    template<typename _actor_type>
    bool ActorRef<_actor_type>::IsValid() const
    {
        return mRef.IsValid();
    }

    template<typename _actor_type>
    ActorRef<_actor_type>::operator bool() const
    {
        return IsValid();
    }

    template<typename _actor_type>
    _actor_type& ActorRef<_actor_type>::operator*() const
    {
        return *mRef;
    }

    template<typename _actor_type>
    _actor_type* ActorRef<_actor_type>::operator->() const
    {
        return mRef.Get();
    }

    template<typename _actor_type>
    _actor_type* ActorRef<_actor_type>::Get()
    {
        return mRef.Get();
    }

    template<typename _actor_type>
    const _actor_type* ActorRef<_actor_type>::Get() const
    {
        return mRef.Get();
    }

    template<typename _actor_type>
    void ActorRef<_actor_type>::Set(Actor* actor)
    {
        *this = ActorRef(dynamic_cast<_actor_type*>(actor));
    }

    template<typename _actor_type>
    const Type& ActorRef<_actor_type>::GetActorType() const
    {
        return TypeOf(_actor_type);
    }

    template<typename _actor_type>
    void ActorRef<_actor_type>::CopyWithoutRemap(const BaseActorRef& other)
    {
        mRef = Ref(dynamic_cast<_actor_type*>(const_cast<Actor*>(other.Get())));
        mRequiredResolveData = nullptr;
    }

    template<typename _actor_type>
    const Type* ActorRef<_actor_type>::GetActorTypeStatic()
    {
        return &TypeOf(_actor_type);
    }
}
// --- META ---

CLASS_BASES_META(o2::BaseActorRef)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::BaseActorRef)
{
    FIELD().PROTECTED().NAME(mRequiredResolveData);
}
END_META;
CLASS_METHODS_META(o2::BaseActorRef)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const BaseActorRef&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, Get);
    FUNCTION().PUBLIC().SIGNATURE(const Actor*, Get);
    FUNCTION().PUBLIC().SIGNATURE(void, Set, Actor*);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Destroy);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsValid);
    FUNCTION().PUBLIC().SIGNATURE(const Type&, GetActorType);
    FUNCTION().PUBLIC().SIGNATURE(void, CopyWithoutRemap, const BaseActorRef&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetActorTypeStatic);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, EqualsDelta, const BaseActorRef&, const BaseActorRef&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
