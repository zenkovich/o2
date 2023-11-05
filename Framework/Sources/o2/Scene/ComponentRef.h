#pragma once

#include "o2/Scene/Component.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
    // ---------------------------------------------------------------------
    // Component reference, automatically invalidates when component deletes
    // ---------------------------------------------------------------------
    class ComponentRef: public ISerializable
    {
    public:
        // Default constructor, no reference
        ComponentRef();

        // Constructor with referencing on component @SCRIPTABLE
        ComponentRef(Component* component);

        // Creates a copy of component and returns reference on it
        ComponentRef(const ComponentRef& other);

        // Destructor
        ~ComponentRef();

        // Boolean cast operator, true means that reference is valid
        operator bool() const;

        // Assign operator
        ComponentRef& operator=(const ComponentRef& other);

        // Getter operator
        Component& operator*();

        // Constant getter operator
        const Component& operator*() const;

        // Component members access operator
        Component* operator->();

        // Constant component members access operator
        const Component* operator->() const;

        // Check equals operator
        bool operator==(const ComponentRef& other) const;

        // Check not equals operator
        bool operator!=(const ComponentRef& other) const;

        // Sets component @SCRIPTABLE
        void Set(Component* component);

        // Returns component pointer @SCRIPTABLE
        Component* Get();

        // Returns component pointer
        const Component* Get() const;

        // Destroy the component @SCRIPTABLE
        void Destroy();

        // Returns is reference is valid @SCRIPTABLE
        bool IsValid() const;

        // Returns is component was deleted @SCRIPTABLE
        bool IsWasDeleted() const;

        // Returns component type
        virtual const Type& GetComponentType() const;

        // Returns component type
        static const Type* GetComponentTypeStatic();

        // Checks refs are equals for serializing delta
        static bool EqualsDelta(const ComponentRef& obj, const ComponentRef& origin);

        SERIALIZABLE(ComponentRef);

    protected:
        // ------------------------------
        // Reference resolve request data
        // ------------------------------
        struct IRequiredResolveData
        {
            SceneUID componentUID;

            // Request resolve reference
            virtual void RequireResolve(ComponentRef& ref) = 0;

            // Clone data
            virtual IRequiredResolveData* Clone() const = 0;
        };

        // -------------------------------------------------
        // Reference resolve request data by actor scene uid
        // -------------------------------------------------
        struct SceneRequireResolveData : public IRequiredResolveData
        {
            SceneUID uid;

            // Request resolve reference
            void RequireResolve(ComponentRef& ref) override;

            // Clone data
            IRequiredResolveData* Clone() const override;
        };

        // -------------------------------------------
        // Reference resolve request data by asset uid
        // -------------------------------------------
        struct AssetRequireResolveData : public IRequiredResolveData
        {
            UID uid;

            // Request resolve reference
            void RequireResolve(ComponentRef& ref) override;

            // Clone data
            IRequiredResolveData* Clone() const override;
        };

        Component* mComponent = nullptr;
        bool       mWasDeleted = false;

        IRequiredResolveData* mRequiredResolveData = nullptr; // Reference resolve request data. Used for resolving reference after deserialization.
                                                              // Not null only when reference is required to resolve. Copies in reference copying.

    protected:
        // Updates specialized component pointer
        virtual void UpdateSpecComponent() {}

        // Copying ref without requiring remap
        void CopyWithoutRemap(const ComponentRef& other);

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
    template<typename T>
    class Ref<T, typename std::enable_if<std::is_base_of<Component, T>::value>::type>: public ComponentRef
    {
    public:
        // Default constructor, no reference
        Ref(): ComponentRef() {}

        // Constructor with referencing on component
        Ref(T* component): ComponentRef(component), mSpecComponent(component) {}

        // Creates a copy of component and returns reference on it
        Ref(const Ref<T>& other): ComponentRef(other), mSpecComponent(other.mSpecComponent) {}

        // Boolean cast operator, true means that reference is valid
        operator bool() const { return IsValid(); }

        // Assign operator
        Ref<T>& operator=(const Ref<T>& other)
        {
            ComponentRef::operator=(other);
            mSpecComponent = other.mSpecComponent;
            return *this;
        }

        // Getter operator
        T& operator*() { return *mSpecComponent; }

        // Constant getter operator
        const T& operator*() const { return *mSpecComponent; }

        // Asset members and field operator
        T* operator->() { return mSpecComponent; }

        // Constant asset members and field operator
        const T* operator->() const { return mSpecComponent; }

        // Check equals operator
        bool operator==(const Ref<T>& other) const { return ComponentRef::operator==(other); }

        // Check not equals operator
        bool operator!=(const Ref<T>& other) const { return ComponentRef::operator!=(other); }

        // Returns component pointer @SCRIPTABLE
        T* Get() { return mSpecComponent; }

        // Returns component pointer
        const T* Get() const { return mSpecComponent; }

        // Returns component type
        const Type& GetComponentType() const override { return TypeOf(T); }

        // Returns component type
        static const Type* GetComponentTypeStatic() { return &TypeOf(T); }

    public:
        typedef Ref<T, typename std::enable_if<std::is_base_of<Component, T>::value>::type> _thisType;

        SERIALIZABLE_MAIN(_thisType);

        template<typename _type_processor>
        static void ProcessBaseTypes(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartBases<_thisType>(object, type);

            BASE_CLASS(o2::ComponentRef);
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

    protected:
        T* mSpecComponent = nullptr;

    protected:
        // Updates specialized component pointer
        void UpdateSpecComponent() override { mSpecComponent = dynamic_cast<T*>(mComponent); };
    };
}
// --- META ---

CLASS_BASES_META(o2::ComponentRef)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ComponentRef)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mComponent);
    FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mWasDeleted);
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRequiredResolveData);
}
END_META;
CLASS_METHODS_META(o2::ComponentRef)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(Component*);
    FUNCTION().PUBLIC().CONSTRUCTOR(const ComponentRef&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Set, Component*);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Component*, Get);
    FUNCTION().PUBLIC().SIGNATURE(const Component*, Get);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Destroy);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsValid);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsWasDeleted);
    FUNCTION().PUBLIC().SIGNATURE(const Type&, GetComponentType);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetComponentTypeStatic);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, EqualsDelta, const ComponentRef&, const ComponentRef&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateSpecComponent);
    FUNCTION().PROTECTED().SIGNATURE(void, CopyWithoutRemap, const ComponentRef&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
