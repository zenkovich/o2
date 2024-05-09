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
        // Reference resolve request data by component scene uid
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
        friend class ComponentRefResolver;
    };

    // ---------------------------------------
    // Reference on derived from component classes
    // ---------------------------------------
    template<typename _component_type>
    class ComponentRef: public BaseComponentRef
    {
	public:
		static_assert(std::is_base_of<Component, _component_type>::value, "ComponentRef type must be derived from Component");

	public:
		// Default constructor, no reference
		ComponentRef();

		// Nullptr constructor
		ComponentRef(nullptr_t);

		// Constructor with component pointer
		explicit ComponentRef(_component_type* ptr);

		// Constructor with component reference
		ComponentRef(const Ref<_component_type>& ref);

		// Copy constructor
		ComponentRef(const ComponentRef<_component_type>& other);

		// Move constructor
		ComponentRef(ComponentRef<_component_type>&& other);

		// Copy constructor from other component reference
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _component_type*>::value>::type>
		ComponentRef(const ComponentRef<_other_type>& other);

		// Move constructor from other component reference
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _component_type*>::value>::type>
		ComponentRef(ComponentRef<_other_type>&& other);

		// Equality operator
		bool operator==(const ComponentRef<_component_type>& other) const;

		// Equality operator
		bool operator==(const _component_type* other) const;

		// Inequality operator
		bool operator!=(const ComponentRef<_component_type>& other) const;

		// Inequality operator
		bool operator!=(const _component_type* other) const;

		// Copy operator
		ComponentRef<_component_type>& operator=(const ComponentRef<_component_type>& other);

		// Move operator
		ComponentRef<_component_type>& operator=(ComponentRef<_component_type>&& other);

		// Copy operator from other component reference
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _component_type*>::value>::type>
		ComponentRef<_component_type>& operator=(const ComponentRef<_other_type>& other);

		// Move operator from other component reference
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_other_type*, _component_type*>::value>::type>
		ComponentRef<_component_type>& operator=(Ref<_other_type>&& other);

		// Returns is reference is valid
		bool IsValid() const;

		// Returns is reference is valid
		operator bool() const;

		// Returns component reference
		operator Ref<_component_type>() const;

		// Returns component reference with different type
		template<typename _other_type, typename _enable = std::enable_if<std::is_convertible<_component_type*, _other_type*>::value>::type>
		operator Ref<_other_type>() const;

		// Returns component reference
		_component_type& operator*() const;

		// Returns component pointer
		_component_type* operator->() const;

		// Returns component pointer 
		_component_type* Get() override;

		// Returns component pointer
		const _component_type* Get() const override;

		// Returns reference
		Ref<_component_type>& GetRef();

		// Returns reference
		const Ref<_component_type>& GetRef() const;

		// Sets component pointer
		void Set(Component* component) override;

        // Returns component type
		const Type& GetComponentType() const override;

        // Copying ref without requiring remap
		void CopyWithoutRemap(const BaseComponentRef& other) override;

        // Returns component type
		static const Type* GetComponentTypeStatic();

	protected:
		Ref<_component_type> mRef; // Reference to component

    public:
		typedef ComponentRef<_component_type> _thisType;

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

            FIELD().PROTECTED().NAME(mRef);
        }

        template<typename _type_processor>
        static void ProcessMethods(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartMethods<_thisType>(object, type);

            FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(_component_type*);
            FUNCTION().PUBLIC().SIGNATURE(const Type&, GetComponentType);
            FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetComponentTypeStatic);
        }
	};

	// Dynamic cast from one component reference type to another
	template<typename _to_type, typename _from_type>
	ComponentRef<_to_type> DynamicComponentCast(const ComponentRef<_from_type>& from)
	{
		return ComponentRef<_to_type>(dynamic_cast<_to_type*>(const_cast<_from_type*>(from.Get())));
	}

	// Dynamic cast from one component reference type to another
	template<typename _to_type, typename _from_type>
	Ref<_to_type> DynamicCast(const ComponentRef<_from_type>& from)
	{
		return Ref<_to_type>(dynamic_cast<_to_type*>(const_cast<_from_type*>(from.Get())));
	}

	template<typename _component_type>
	ComponentRef<_component_type>::ComponentRef()
	{}

	template<typename _component_type>
	ComponentRef<_component_type>::ComponentRef(nullptr_t) :
		mRef(nullptr)
	{}

	template<typename _component_type>
	ComponentRef<_component_type>::ComponentRef(_component_type* ptr) :
		mRef(ptr)
	{}

	template<typename _component_type>
	ComponentRef<_component_type>::ComponentRef(const Ref<_component_type>& ref) :
		mRef(ref)
	{}

	template<typename _component_type>
	ComponentRef<_component_type>::ComponentRef(const ComponentRef<_component_type>& other) :
		mRef(other.mRef)
	{}

	template<typename _component_type>
	ComponentRef<_component_type>::ComponentRef(ComponentRef<_component_type>&& other) :
		mRef(other.mRef)
	{}

	template<typename _component_type>
	template<typename _other_type, typename _enable>
	ComponentRef<_component_type>::ComponentRef(const ComponentRef<_other_type>& other) :
		mRef(other.mRef)
	{}

	template<typename _component_type>
	template<typename _other_type, typename>
	ComponentRef<_component_type>::ComponentRef(ComponentRef<_other_type>&& other) :
		mRef(std::move(other.mRef))
	{}

	template<typename _component_type>
	bool ComponentRef<_component_type>::operator==(const ComponentRef<_component_type>& other) const
	{
		return mRef == other.mRef;
	}

	template<typename _component_type>
	bool ComponentRef<_component_type>::operator==(const _component_type* other) const
	{
		return mRef == other;
	}

	template<typename _component_type>
	bool ComponentRef<_component_type>::operator!=(const ComponentRef<_component_type>& other) const
	{
		return mRef != other.mRef;
	}

	template<typename _component_type>
	bool ComponentRef<_component_type>::operator!=(const _component_type* other) const
	{
		return mRef != other;
	}

	template<typename _component_type>
	ComponentRef<_component_type>& ComponentRef<_component_type>::operator=(const ComponentRef<_component_type>& other)
	{
		mRef = other.mRef;
		return *this;
	}

	template<typename _component_type>
	ComponentRef<_component_type>& ComponentRef<_component_type>::operator=(ComponentRef<_component_type>&& other)
	{
		mRef = std::move(other.mRef);
		return *this;
	}

	template<typename _component_type>
	template<typename _other_type, typename _enable>
	ComponentRef<_component_type>& ComponentRef<_component_type>::operator=(const ComponentRef<_other_type>& other)
	{
		mRef = other.mRef;
		return *this;
	}

	template<typename _component_type>
	template<typename _other_type, typename _enable>
	ComponentRef<_component_type>& ComponentRef<_component_type>::operator=(Ref<_other_type>&& other)
	{
		mRef = std::move(other.mRef);
		return *this;
	}

	template<typename _component_type>
	bool ComponentRef<_component_type>::IsValid() const
	{
		return mRef.IsValid();
	}

	template<typename _component_type>
	ComponentRef<_component_type>::operator bool() const
	{
		return IsValid();
	}

	template<typename _component_type>
	ComponentRef<_component_type>::operator Ref<_component_type>() const
	{
		return mRef;
	}

	template<typename _component_type>
	template<typename _other_type, typename _enable>
	ComponentRef<_component_type>::operator Ref<_other_type>() const
	{
		return mRef;
	}

	template<typename _component_type>
	_component_type& ComponentRef<_component_type>::operator*() const
	{
		return *mRef;
	}

	template<typename _component_type>
	_component_type* ComponentRef<_component_type>::operator->() const
	{
		return mRef.Get();
	}

	template<typename _component_type>
	_component_type* ComponentRef<_component_type>::Get()
	{
		return mRef.Get();
	}

	template<typename _component_type>
	const _component_type* ComponentRef<_component_type>::Get() const
	{
		return mRef.Get();
	}

	template<typename _component_type>
	Ref<_component_type>& ComponentRef<_component_type>::GetRef()
	{
		return mRef;
	}

	template<typename _component_type>
	const Ref<_component_type>& ComponentRef<_component_type>::GetRef() const
	{
		return mRef;
	}

	template<typename _component_type>
	void ComponentRef<_component_type>::Set(Component* component)
	{
		*this = ComponentRef(dynamic_cast<_component_type*>(component));
	}

	template<typename _component_type>
	const Type& ComponentRef<_component_type>::GetComponentType() const
	{
		return TypeOf(_component_type);
	}

	template<typename _component_type>
	void ComponentRef<_component_type>::CopyWithoutRemap(const BaseComponentRef& other)
	{
		mRef = Ref(dynamic_cast<_component_type*>(const_cast<Component*>(other.Get())));
		mRequiredResolveData = nullptr;
	}

	template<typename _component_type>
	const Type* ComponentRef<_component_type>::GetComponentTypeStatic()
	{
		return &TypeOf(_component_type);
	}
}
// --- META ---

CLASS_BASES_META(o2::BaseComponentRef)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::BaseComponentRef)
{
    FIELD().PROTECTED().NAME(mRequiredResolveData);
}
END_META;
CLASS_METHODS_META(o2::BaseComponentRef)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const BaseComponentRef&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Set, Component*);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Component*, Get);
    FUNCTION().PUBLIC().SIGNATURE(const Component*, Get);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Destroy);
    FUNCTION().PUBLIC().SIGNATURE(const Type&, GetComponentType);
    FUNCTION().PUBLIC().SIGNATURE(void, CopyWithoutRemap, const BaseComponentRef&);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetComponentTypeStatic);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, EqualsDelta, const BaseComponentRef&, const BaseComponentRef&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
// --- END META ---
