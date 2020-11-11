#pragma once

#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
	class Component;

	// ----------------------------------------------------------------------
	// Component reference, automatically invalidates when component deleting
	// ----------------------------------------------------------------------
	template<>
	class Ref<Component>: public ISerializable
	{
	public:
		// Default constructor, no reference
		Ref();

		// Constructor with referencing on component
		Ref(Component* component);

		// Creates a copy of component and returns reference on it
		Ref(const Component& other);

		// Destructor
		~Ref();

		// Boolean cast operator, true means that reference is valid
		operator bool() const;

		// Assign operator
		Ref& operator=(const Ref& other);

		// Getter operator
		Component& operator*();

		// Constant getter operator
		const Component& operator*() const;

		// Actor members access operator
		Component* operator->();

		// Constant actor members access operator
		const Component* operator->() const;

		// Check equals operator
		bool operator==(const Ref& other) const;

		// Check not equals operator
		bool operator!=(const Ref& other) const;

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

		SERIALIZABLE(Ref<Component>);

	protected:
		Component* mComponent = nullptr;
		bool       mWasDeleted = false;

		friend class Component;
	};

	typedef Ref<Component> ComponentRef;
}

CLASS_BASES_META(o2::Ref<o2::Component>)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Ref<o2::Component>)
{
	PROTECTED_FIELD(mComponent).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mWasDeleted).DEFAULT_VALUE(false);
}
END_META;
CLASS_METHODS_META(o2::Ref<o2::Component>)
{

	PUBLIC_FUNCTION(Component*, Get);
	PUBLIC_FUNCTION(const Component*, Get);
	PUBLIC_FUNCTION(void, Destroy);
	PUBLIC_FUNCTION(bool, IsValid);
	PUBLIC_FUNCTION(bool, IsWasDeleted);
}
END_META;
