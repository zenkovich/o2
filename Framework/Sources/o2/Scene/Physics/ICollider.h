#pragma once
#include "o2/Scene/Component.h"
#include "Box2D/Collision/Shapes/b2Shape.h"
#include "Box2D/Dynamics/b2Fixture.h"

namespace o2
{
	class RigidBody;

	// --------------------------------
	// Basic physics collider interface
	// --------------------------------
	class ICollider: public Component
	{
	public:
		PROPERTIES(ICollider);
		PROPERTY(float, friction, SetFriction, GetFriction);          // Friction property
		PROPERTY(float, density, SetDensity, GetDensity);             // Density property
		PROPERTY(float, restitution, SetRestitution, GetRestitution); // Restitution property
		PROPERTY(String, layer, SetLayer, GetLayer);                  // Layer name property
		PROPERTY(bool, isSensor, SetIsSensor, IsSensor);              // Is sensor property

	public:
		// Default constructor
		ICollider();

		// Copy-constructor
		ICollider(const ICollider& other);

		// Destructor
		~ICollider();

		// Copy operator
		ICollider& operator=(const ICollider& other);

		// Sets friction coefficient
		void SetFriction(float value);

		// Returns friction coefficient
		float GetFriction() const;

		// Sets density
		void SetDensity(float value);

		// Returns density
		float GetDensity() const;

		// Sets restitution
		void SetRestitution(float value);

		// Returns restitution
		float GetRestitution() const;

		// Sets layer
		void SetLayer(const String& layer);

		// Returns layer name
		const String& GetLayer() const;

		// Sets is sensor
		void SetIsSensor(bool value);

		// Returns is collider sensor
		bool IsSensor() const;

		// Is component visible in create menu
		static bool IsAvailableFromCreateMenu();

		SERIALIZABLE(ICollider);

	protected:
		float mFriction = 0.3f;    // Friction coefficient @SERIALIZABLE
		float mDensity = 1.0f;     // Density @SERIALIZABLE 
		float mRestitution = 0.0f; // Restitution @SERIALIZABLE

		String mLayer = String("Default"); // Layer name @SERIALIZABLE

		bool mIsSensor = false; // Is collider sensor @SERIALIZABLE

		b2Fixture* mFixture = nullptr;
		RigidBody* mRigidBodyComp = nullptr;

	protected:
		// Adds fixture with shape to body
		void AddToRigidBody(RigidBody* body);

		// Removes fixture from body
		void RemoveFromRigidBody();

		// Searches rigid body in parent hierarchy
		RigidBody* FindRigidBody() const;

		// It is called when shape has changed, updates fixture with new shape
		void OnShapeChanged();

		// Returns shape with relative position and angle
		virtual b2Shape* GetShape(const Basis& transform);

		// It is called when transformation was changed 
		void OnTransformChanged() override;

		// It is called when actor was included to scene
		void OnAddToScene() override;

		// It is called when actor was excluded from scene
		void OnRemoveFromScene() override;

		friend class PhysicsWorld;
		friend class RigidBody;
	};
}

CLASS_BASES_META(o2::ICollider)
{
	BASE_CLASS(o2::Component);
}
END_META;
CLASS_FIELDS_META(o2::ICollider)
{
	FIELD().NAME(friction).PUBLIC();
	FIELD().NAME(density).PUBLIC();
	FIELD().NAME(restitution).PUBLIC();
	FIELD().NAME(layer).PUBLIC();
	FIELD().NAME(isSensor).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.3f).NAME(mFriction).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(mDensity).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(mRestitution).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(String("Default")).NAME(mLayer).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mIsSensor).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mFixture).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mRigidBodyComp).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::ICollider)
{

	PUBLIC_FUNCTION(void, SetFriction, float);
	PUBLIC_FUNCTION(float, GetFriction);
	PUBLIC_FUNCTION(void, SetDensity, float);
	PUBLIC_FUNCTION(float, GetDensity);
	PUBLIC_FUNCTION(void, SetRestitution, float);
	PUBLIC_FUNCTION(float, GetRestitution);
	PUBLIC_FUNCTION(void, SetLayer, const String&);
	PUBLIC_FUNCTION(const String&, GetLayer);
	PUBLIC_FUNCTION(void, SetIsSensor, bool);
	PUBLIC_FUNCTION(bool, IsSensor);
	PUBLIC_STATIC_FUNCTION(bool, IsAvailableFromCreateMenu);
	PROTECTED_FUNCTION(void, AddToRigidBody, RigidBody*);
	PROTECTED_FUNCTION(void, RemoveFromRigidBody);
	PROTECTED_FUNCTION(RigidBody*, FindRigidBody);
	PROTECTED_FUNCTION(void, OnShapeChanged);
	PROTECTED_FUNCTION(b2Shape*, GetShape, const Basis&);
	PROTECTED_FUNCTION(void, OnTransformChanged);
	PROTECTED_FUNCTION(void, OnAddToScene);
	PROTECTED_FUNCTION(void, OnRemoveFromScene);
}
END_META;
