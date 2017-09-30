#pragma once

#include "Scene/Scene.h"
#include "Utils/Serializable.h"

namespace o2
{
	class Actor;

	// ---------------------------
	// Actor's component interface
	// ---------------------------
	class Component: public ISerializable
	{
	public:
		Getter<Actor*> actor;              // Owner actor getter
		Property<bool> enabled;            // Enabling property
		Getter<bool>   enabledInHierarchy; // Is enabled in hierarchy property

		// Default constructor
		Component();

		// Copy-constructor
		Component(const Component& other);

		// Virtual destructor
		virtual ~Component();

		// Copy-operator
		Component& operator=(const Component& other);

		// Returns component id
		UInt64 GetID() const;

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

		// Returns prototype link
		Component* GetPrototypeLink() const;

		// Returns is this linked to specified component with depth links search
		bool IsLinkedToComponent(Component* component) const;

		// Returns owner actor
		Actor* GetOwnerActor() const;

		// Returns component with type
		template<typename _type>
		_type* GetComponent() const;

		// Returns component with type in children
		template<typename _type>
		_type* GetComponentInChildren() const;

		// Returns components with type
		template<typename _type>
		Vector<_type>* GetComponents() const;

		// Returns components with type in children
		template<typename _type>
		Vector<_type>* GetComponentsInChildren() const;

		// Returns name of component
		virtual String GetName() const;

		SERIALIZABLE(Component);

	protected:
		Component* mPrototypeLink = nullptr; // Prototype actor component pointer. Null if no actor prototype
		UInt64 mId;                          // Component id @SERIALIZABLE
		Actor* mOwner = nullptr;             // Owner actor
		bool   mEnabled = true;              // Is component enabled @SERIALIZABLE
		bool   mResEnabled = true;           // Is component enabled in hierarchy

	protected:
		// It is called when actor changed layer
		virtual void OnLayerChanged(SceneLayer* oldLayer, SceneLayer* newLayer) {}

		// Updates component enable
		virtual void UpdateEnabled();

		// It is called when actor's transform was changed
		virtual void OnTransformChanged() {}

		// Sets owner actor
		virtual void SetOwnerActor(Actor* actor);

		// It is called when actor was excluded from scene
		virtual void OnExcludeFromScene() {}

		// It is called when actor was included to scene
		virtual void OnIncludeToScene() {}

		// Initializes properties
		void InitializeProperties();

		friend class Actor;
	};

	// ------------------------
	// Component data converter
	// ------------------------
	class ComponentDataNodeConverter: public IDataNodeTypeConverter
	{
	public:
		// Converts component pointer to data
		void ToData(void* object, DataNode& data);

		// Converts from data to component pointer
		void FromData(void* object, const DataNode& data);

		// Checks that type is based on Component type
		bool IsConvertsType(const Type* type) const;
	};

	template<typename _type>
	Vector<_type>* Component::GetComponentsInChildren() const
	{
		if (mOwner)
			return mOwner->GetComponentsInChildren();

		return Vector<_type>*();
	}

	template<typename _type>
	Vector<_type>* Component::GetComponents() const
	{
		if (mOwner)
			return mOwner->GetComponents();

		return Vector<_type>*();
	}

	template<typename _type>
	_type* Component::GetComponentInChildren() const
	{
		if (mOwner)
			return mOwner->GetComponentInChildren();

		return nullptr;
	}

	template<typename _type>
	_type* Component::GetComponent() const
	{
		if (mOwner)
			return mOwner->GetComponent();

		return nullptr;
	}

}
