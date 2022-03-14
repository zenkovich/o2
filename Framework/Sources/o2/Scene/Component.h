#pragma once

#include "o2/Scene/SceneLayer.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"
#include "o2/Utils/Serialization/Serializable.h"

namespace o2
{
	class Actor;
	class ComponentRef;

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
		SceneUID GetID() const;

		// Updates component
		virtual void Update(float dt);

		// Updates component with fixed delta time
		virtual void FixedUpdate(float dt);

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

		// Is component visible in create menu
		static bool IsAvailableFromCreateMenu();

#if IS_EDITOR
		// It is called when component added from editor
		virtual void OnAddedFromEditor() {}
#endif

		SERIALIZABLE(Component);

	protected:
		Component* mPrototypeLink = nullptr; // Prototype actor component pointer. Null if no actor prototype
		SceneUID   mId;                      // Component id @EDITOR_IGNORE
		Actor*     mOwner = nullptr;         // Owner actor
		bool       mEnabled = true;          // Is component enabled @SERIALIZABLE @EDITOR_IGNORE
		bool       mResEnabled = true;       // Is component enabled in hierarchy

		Vector<ComponentRef*> mReferences; // References to this component

	protected:
		// Beginning serialization callback
		void OnSerialize(DataValue& node) const override;

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		// Beginning serialization delta callback
		void OnSerializeDelta(DataValue& node, const IObject& origin) const override;

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;

		// Sets owner actor
		virtual void SetOwnerActor(Actor* actor);

		// It is called when actor was included to scene
		virtual void OnAddToScene();

		// It is called when actor was excluded from scene
		virtual void OnRemoveFromScene();

		// It is called when component started working on first update frame
		virtual void OnStart() {}

		// Updates component enable
		virtual void UpdateEnabled();

		// Is is called when actor enabled in hierarchy
		virtual void OnEnabled() {}

		// It is called when actor disabled in hierarchy
		virtual void OnDisabled() {}

		// It is called when transformation was changed 
		virtual void OnTransformChanged() {}

		// It is called when actor's transform was changed
		virtual void OnTransformUpdated() {}

		// It is called when parent changed
		virtual void OnParentChanged(Actor* oldParent) {}

		// It is called when child actor was added
		virtual void OnChildAdded(Actor* child) {}

		// It is called when child actor was removed
		virtual void OnChildRemoved(Actor* child) {}

		// It is called when layer was changed
		virtual void OnLayerChanged(SceneLayer* oldLayer) {}

		// It is called when new component has added to actor
		virtual void OnComponentAdded(Component* component) {}

		// It is called when component going to be removed from actor
		virtual void OnComponentRemoving(Component* component) {}

		friend class Actor;
		friend struct ActorDifferences;
		friend class ActorRefResolver;
		friend class ComponentRef;
		friend class Scene;
		friend class Widget;
	};
}

#include "o2/Scene/Actor.h"

namespace o2
{
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
	FIELD().PUBLIC().NAME(actor);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(enabled);
	FIELD().PUBLIC().NAME(enabledInHierarchy);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mPrototypeLink);
	FIELD().PROTECTED().EDITOR_IGNORE_ATTRIBUTE().NAME(mId);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mOwner);
	FIELD().PROTECTED().EDITOR_IGNORE_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(true).NAME(mEnabled);
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mResEnabled);
	FIELD().PROTECTED().NAME(mReferences);
}
END_META;
CLASS_METHODS_META(o2::Component)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const Component&);
	FUNCTION().PUBLIC().SIGNATURE(SceneUID, GetID);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, FixedUpdate, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEnabled, bool);
	FUNCTION().PUBLIC().SIGNATURE(void, Enable);
	FUNCTION().PUBLIC().SIGNATURE(void, Disable);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabled);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEnabledInHierarchy);
	FUNCTION().PUBLIC().SIGNATURE(Component*, GetPrototypeLink);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsLinkedToComponent, Component*);
	FUNCTION().PUBLIC().SIGNATURE(Actor*, GetOwnerActor);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetName);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCategory);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetIcon);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(bool, IsAvailableFromCreateMenu);
	FUNCTION().PUBLIC().SIGNATURE(void, OnAddedFromEditor);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerializeDelta, DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserializedDelta, const DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(void, SetOwnerActor, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnStart);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
	FUNCTION().PROTECTED().SIGNATURE(void, OnParentChanged, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnChildAdded, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnChildRemoved, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnLayerChanged, SceneLayer*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnComponentAdded, Component*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnComponentRemoving, Component*);
}
END_META;
