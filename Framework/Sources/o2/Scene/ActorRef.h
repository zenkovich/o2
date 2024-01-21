#pragma once

#include "o2/Scene/ActorCreationMode.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
    class Actor;

    // --------------------------------------------------------------
    // Actor reference, automatically invalidates when actor deleting
    // --------------------------------------------------------------
    class BaseActorRef: public ISerializable
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

        // Returns actor type
        static const Type* GetActorTypeStatic();

        // Checks refs are equals for serializing delta
        static bool EqualsDelta(const BaseActorRef& obj, const BaseActorRef& origin);

        SERIALIZABLE(BaseActorRef);

    protected:
        // ------------------------------
        // Reference resolve request data
        // ------------------------------
        struct IRequiredResolveData: public RefCounterable
        {
            // Request resolve reference
            virtual void RequireResolve(BaseActorRef& ref) = 0;

            // Clone data
            virtual Ref<IRequiredResolveData> Clone() const = 0;
        };

        // -------------------------------------------------
        // Reference resolve request data by actor scene uid
        // -------------------------------------------------
        struct SceneRequireResolveData: public IRequiredResolveData
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
        struct AssetRequireResolveData: public IRequiredResolveData
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
        // Copying ref without requiring remap
        virtual void CopyWithoutRemap(const BaseActorRef& other);

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
    class Ref<_actor_type, typename std::enable_if<IsBaseOf<Actor, _actor_type>::value>::type>: public BaseActorRef, public BaseRef<_actor_type>
	{
		using Base = BaseRef<_actor_type>;

	public:
		// Base reference implementation
		BASE_REF_IMPLEMETATION(_actor_type);

        // Returns actor pointer 
        _actor_type* Get() override { return Base::Get(); }

        // Returns actor pointer
		const _actor_type* Get() const override { return Base::Get(); }

		// Sets actor pointer
        void Set(Actor* actor) override { *this = Ref(actor); }

        // Returns actor type
        const Type& GetActorType() const override { return TypeOf(_actor_type); }

        // Returns actor type
        static const Type* GetActorTypeStatic() { return &TypeOf(_actor_type); }

	protected:
		// Copying ref without requiring remap
        void CopyWithoutRemap(const BaseActorRef& other) override
		{
			mPtr = other.mPtr;
			mRequiredResolveData = nullptr;
        }

    public:
        typedef Ref<_actor_type, typename std::enable_if<std::is_base_of<Actor, _actor_type>::value>::type> _thisType;

        SERIALIZABLE_MAIN(_thisType);
        IOBJECT_SCRIPTING();

        template<typename _type_processor>
        static void ProcessBaseTypes(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartBases<_thisType>(object, type);

            BASE_CLASS(o2::Ref<Actor>);
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
    };
}
// --- META ---

CLASS_BASES_META(o2::Ref<Actor>)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Ref<Actor>)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mActor);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mWasDeleted);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRequiredResolveData);
}
END_META;
CLASS_METHODS_META(o2::Ref<Actor>)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(Actor*);
    FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<Actor>&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, Get);
    FUNCTION().PUBLIC().SIGNATURE(const Actor*, Get);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Destroy);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsValid);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsWasDeleted);
    FUNCTION().PUBLIC().SIGNATURE(const Type&, GetActorType);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetActorTypeStatic);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, EqualsDelta, const Ref<Actor>&, const Ref<Actor>&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateSpecActor);
    FUNCTION().PROTECTED().SIGNATURE(void, CopyWithoutRemap, const Ref<Actor>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
