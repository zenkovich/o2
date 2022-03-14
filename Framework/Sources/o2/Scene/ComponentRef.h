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

		// Constructor with referencing on component
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

		// Returns component pointer
		Component* Get();

		// Returns component pointer
		const Component* Get() const;

		// Destroy the component
		void Destroy();

		// Returns is reference is valid
		bool IsValid() const;

		// Returns is component was deleted
		bool IsWasDeleted() const;

		// Returns component type
		virtual const Type& GetComponentType() const;

		// Returns component type
		static const Type* GetComponentTypeStatic();

		// Checks refs are equals for serializing delta
		static bool EqualsDelta(const ComponentRef& obj, const ComponentRef& origin);

		SERIALIZABLE(ComponentRef);

	protected:
		Component* mComponent = nullptr;
		bool       mWasDeleted = false;

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

CLASS_BASES_META(o2::ComponentRef)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ComponentRef)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mComponent);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mWasDeleted);
}
END_META;
CLASS_METHODS_META(o2::ComponentRef)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(Component*);
	FUNCTION().PUBLIC().CONSTRUCTOR(const ComponentRef&);
	FUNCTION().PUBLIC().SIGNATURE(Component*, Get);
	FUNCTION().PUBLIC().SIGNATURE(const Component*, Get);
	FUNCTION().PUBLIC().SIGNATURE(void, Destroy);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsValid);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsWasDeleted);
	FUNCTION().PUBLIC().SIGNATURE(const Type&, GetComponentType);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetComponentTypeStatic);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, EqualsDelta, const ComponentRef&, const ComponentRef&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateSpecComponent);
	FUNCTION().PROTECTED().SIGNATURE(void, CopyWithoutRemap, const ComponentRef&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
}
END_META;
