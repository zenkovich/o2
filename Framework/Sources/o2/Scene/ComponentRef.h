#pragma once

#include "o2/Scene/Component.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
    // ---------------------------------------------------------------------
    // Component reference, automatically invalidates when component deletes
    // ---------------------------------------------------------------------
    class BaseComponentRef: public ISerializable
    {
    public:
        // Default constructor, no reference
        BaseComponentRef();

        // Creates a copy of component and returns reference on it
        BaseComponentRef(const BaseComponentRef& other);

        // Destructor
        ~BaseComponentRef();

        // Assign operator
        BaseComponentRef& operator=(const BaseComponentRef& other);

        // Sets component @SCRIPTABLE
        virtual void Set(Component* component);

        // Returns component pointer @SCRIPTABLE
        virtual Component* Get();

        // Returns component pointer
        virtual const Component* Get() const;

        // Destroy the component @SCRIPTABLE
        void Destroy();

        // Returns component type
        virtual const Type& GetComponentType() const;

        // Copying ref without requiring remap
        virtual void CopyWithoutRemap(const BaseComponentRef& other);

        // Returns component type
        static const Type* GetComponentTypeStatic();

        // Checks refs are equals for serializing delta
        static bool EqualsDelta(const BaseComponentRef& obj, const BaseComponentRef& origin);

        SERIALIZABLE(BaseComponentRef);

    protected:
        // ------------------------------
        // Reference resolve request data
        // ------------------------------
        struct IRequiredResolveData: public RefCounterable
        {
            SceneUID componentUID;

            // Request resolve reference
            virtual void RequireResolve(BaseComponentRef& ref) = 0;

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
            void RequireResolve(BaseComponentRef& ref) override;

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
            void RequireResolve(BaseComponentRef& ref) override;

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

        friend class Component;
        friend class ActorRefResolver;
    };

    // ---------------------------------------
    // Reference on derived from component classes
    // ---------------------------------------
    template<typename _component_type>
    class Ref<_component_type, typename std::enable_if<IsBaseOf<Component, _component_type>::value>::type>: public BaseComponentRef, public BaseRef<_component_type>
    {
	public:
		using Base = BaseRef<_component_type>;

	public:
		// Base reference implementation
		BASE_REF_IMPLEMETATION(_component_type);

		// Sets component @SCRIPTABLE
        void Set(Component* component) override { *this = component; }

        // Returns component pointer @SCRIPTABLE
        _component_type* Get() override { return Base::Get(); }

        // Returns component pointer
        const _component_type* Get() const { return Base::Get(); }

        // Returns component type
        const Type& GetComponentType() const override { return TypeOf(_component_type); }

        // Copying ref without requiring remap
        void CopyWithoutRemap(const BaseComponentRef& other) override
        {
            mPtr = other.mPtr;
            mRequiredResolveData = nullptr;
        }

        // Returns component type
        static const Type* GetComponentTypeStatic() { return &TypeOf(_component_type); }

    public:
		typedef Ref<_component_type, typename std::enable_if<std::is_base_of<Component, _component_type>::value>::type> _thisType;

        SERIALIZABLE_MAIN(_thisType);
        IOBJECT_SCRIPTING();

        template<typename _type_processor>
        static void ProcessBaseTypes(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartBases<_thisType>(object, type);

            BASE_CLASS(o2::BaseComponentRef);
        }

        template<typename _type_processor>
        static void ProcessFields(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartFields<_thisType>(object, type);

            FIELD().PROTECTED().NAME(mSpecComponent);
        }

        template<typename _type_processor>
        static void ProcessMethods(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartMethods<_thisType>(object, type);

            FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(T*);
            FUNCTION().PUBLIC().SIGNATURE(const Type&, GetComponentType);
            FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetComponentTypeStatic);
        }
    };
}
// --- META ---

CLASS_BASES_META(o2::BaseComponentRef)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::BaseComponentRef)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mComponent);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mWasDeleted);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRequiredResolveData);
}
END_META;
CLASS_METHODS_META(o2::BaseComponentRef)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(Component*);
    FUNCTION().PUBLIC().CONSTRUCTOR(const BaseComponentRef&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Set, Component*);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Component*, Get);
    FUNCTION().PUBLIC().SIGNATURE(const Component*, Get);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Destroy);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsValid);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsWasDeleted);
    FUNCTION().PUBLIC().SIGNATURE(const Type&, GetComponentType);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetComponentTypeStatic);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, EqualsDelta, const BaseComponentRef&, const BaseComponentRef&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateSpecComponent);
    FUNCTION().PROTECTED().SIGNATURE(void, CopyWithoutRemap, const BaseComponentRef&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
