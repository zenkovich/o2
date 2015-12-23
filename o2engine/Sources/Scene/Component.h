#pragma once

#include "Utils/Serialization.h"

namespace o2
{
	class Actor;

	// ---------------------------
	// Actor's component interface
	// ---------------------------
	class Component: public ISerializable
	{
	public:
		Getter<Ptr<Actor>> actor;              // Owner actor getter
		Property<bool>     enabled;            // Enabling property
		Getter<bool>       enabledInHierarchy; // Is enabled in hierarchy property

		// Default constructor
		Component();

		// Copy-constructor
		Component(const Component& other);

		// Virtual destructor
		virtual ~Component();

		// Copy-operator
		Component& operator=(const Component& other);

		// Updates component
		virtual void Update(float dt);

		// Sets component enable
		virtual void SetEnabled(bool active);

		// Enables component
		void Enable();

		// Disables component
		void Disable();

		// Returns is component enabled
		bool IsEnabled() const;

		// Returns is compontent enabled in hierarchy
		bool IsEnabledInHierarchy() const;

		// Returns owner actor
		Ptr<Actor> GetOwnerActor() const;

		// Returns component with type
		template<typename _type>
		Ptr<_type> GetComponent() const;

		// Returns component with type in children
		template<typename _type>
		Ptr<_type> GetComponentInChildren() const;

		// Returns components with type
		template<typename _type>
		Vector<Ptr<_type>> GetComponents() const;

		// Returns components with type in children
		template<typename _type>
		Vector<Ptr<_type>> GetComponentsInChildren() const;

		SERIALIZABLE(Component);

	protected:
		Ptr<Actor> mOwner;      // Owner actor
		bool       mEnabled;    // Is component enabled @SERIALIZABLE
		bool       mResEnabled; // Is component enabled in hierarchy

	protected:
		// Updates component enable
		virtual void UpdateEnabled();

		// Calls when actor's transform was changed
		virtual void OnTransformChanged() {}

		// Sets owner actor
		virtual void SetOwnerActor(Ptr<Actor> actor);

		// Initializes properties
		void InitializeProperties();

		friend class Actor;
	};

	template<typename _type>
	Vector<Ptr<_type>> Component::GetComponentsInChildren() const
	{
		if (mOwner)
			return mOwner->GetComponentsInChildren();

		return Vector<Ptr<_type>>();
	}

	template<typename _type>
	Vector<Ptr<_type>> Component::GetComponents() const
	{
		if (mOwner)
			return mOwner->GetComponents();

		return Vector<Ptr<_type>>();
	}

	template<typename _type>
	Ptr<_type> Component::GetComponentInChildren() const
	{
		if (mOwner)
			return mOwner->GetComponentInChildren();

		return nullptr;
	}

	template<typename _type>
	Ptr<_type> Component::GetComponent() const
	{
		if (mOwner)
			return mOwner->GetComponent();

		return nullptr;
	}

}
