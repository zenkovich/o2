#pragma once

#include "o2/Scene/SceneLayer.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class Actor;

	// ---------------------------
	// Actor's component interface
	// ---------------------------
	class Component: virtual public ISerializable
	{
	public:
		PROPERTIES(Component);
		GETTER(Actor*, actor, GetOwnerActor);                   // Owner actor getter
		PROPERTY(bool, enabled, SetEnabled, IsEnabled);         // Enabling property @EDITOR_IGNORE
		GETTER(bool, enabledInHierarchy, IsEnabledInHierarchy); // Is enabled in hierarchy property

	public:
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

		// Returns is component enabled in hierarchy
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
		Vector<_type*> GetComponents() const;

		// Returns components with type in children
		template<typename _type>
		Vector<_type*> GetComponentsInChildren() const;

		// Returns name of component
		static String GetName();

		// Returns category of component
		static String GetCategory();

		// Returns name of component icon
		static String GetIcon();

		SERIALIZABLE(Component);

	protected:
		Component* mPrototypeLink = nullptr; // Prototype actor component pointer. Null if no actor prototype
		UInt64     mId;                      // Component id @SERIALIZABLE @EDITOR_IGNORE
		Actor*     mOwner = nullptr;         // Owner actor
		bool       mEnabled = true;          // Is component enabled @SERIALIZABLE @EDITOR_IGNORE
		bool       mResEnabled = true;       // Is component enabled in hierarchy

	protected:
		// Sets owner actor
		virtual void SetOwnerActor(Actor* actor);

		// It is called when actor was included to scene
		virtual void OnAddToScene() {}

		// It is called when actor was excluded from scene
		virtual void OnRemoveFromScene() {}

		// It is called when component started working on first update frame
		virtual void OnStart() {}

		// It is called when actor changed layer
		virtual void OnLayerChanged(SceneLayer* oldLayer, SceneLayer* newLayer) {}

		// Updates component enable
		virtual void UpdateEnabled();

		// It is called when actor's transform was changed
		virtual void OnTransformUpdated() {}

		friend class Actor;
		friend class Scene;
		friend class Widget;
	};

	// ------------------------
	// Component data converter
	// ------------------------
// 	class ComponentDataValueConverter: public IDataValueTypeConverter
// 	{
// 	public:
// 		// Converts component pointer to data
// 		void ToData(void* object, DataValue& data);
// 
// 		// Converts from data to component pointer
// 		void FromData(void* object, const DataValue& data);
// 
// 		// Checks that type is based on Component type
// 		bool IsConvertsType(const Type* type) const;
// 	};

	template<typename _type>
	Vector<_type*> Component::GetComponentsInChildren() const
	{
		if (mOwner)
			return mOwner->GetComponentsInChildren<_type>();

		return Vector<_type*>();
	}

	template<typename _type>
	Vector<_type*> Component::GetComponents() const
	{
		if (mOwner)
			return mOwner->GetComponents();

		return Vector<_type*>();
	}

	template<typename _type>
	_type* Component::GetComponentInChildren() const
	{
		if (mOwner)
			return mOwner->GetComponentInChildren<_type>();

		return nullptr;
	}

	template<typename _type>
	_type* Component::GetComponent() const
	{
		if (mOwner)
			return mOwner->GetComponent<_type>();

		return nullptr;
	}

}

CLASS_BASES_META(o2::Component)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Component)
{
	PUBLIC_FIELD(actor);
	PUBLIC_FIELD(enabled).EDITOR_IGNORE_ATTRIBUTE();
	PUBLIC_FIELD(enabledInHierarchy);
	PROTECTED_FIELD(mPrototypeLink).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mId).EDITOR_IGNORE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mOwner).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mEnabled).DEFAULT_VALUE(true).EDITOR_IGNORE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mResEnabled).DEFAULT_VALUE(true);
}
END_META;
CLASS_METHODS_META(o2::Component)
{

	PUBLIC_FUNCTION(UInt64, GetID);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(void, Enable);
	PUBLIC_FUNCTION(void, Disable);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(bool, IsEnabledInHierarchy);
	PUBLIC_FUNCTION(Component*, GetPrototypeLink);
	PUBLIC_FUNCTION(bool, IsLinkedToComponent, Component*);
	PUBLIC_FUNCTION(Actor*, GetOwnerActor);
	PUBLIC_STATIC_FUNCTION(String, GetName);
	PUBLIC_STATIC_FUNCTION(String, GetCategory);
	PUBLIC_STATIC_FUNCTION(String, GetIcon);
	PROTECTED_FUNCTION(void, SetOwnerActor, Actor*);
	PROTECTED_FUNCTION(void, OnAddToScene);
	PROTECTED_FUNCTION(void, OnRemoveFromScene);
	PROTECTED_FUNCTION(void, OnStart);
	PROTECTED_FUNCTION(void, OnLayerChanged, SceneLayer*, SceneLayer*);
	PROTECTED_FUNCTION(void, UpdateEnabled);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
}
END_META;
