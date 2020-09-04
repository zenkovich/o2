#pragma once

#include "o2/Scene/Actor.h"
#include "Box2D/Collision/Shapes/b2Shape.h"
#include "Box2D/Dynamics/b2Body.h"

namespace o2
{
	class ICollider;

	// ------------------
	// Physics rigid body
	// ------------------
	class RigidBody: public Actor
	{
	public:
		enum class Type { Dynamic, Static, Kinematic };

		PROPERTIES(RigidBody);
		PROPERTY(Type, bodyType, SetBodyType, GetBodyType);                       // Body type property
		PROPERTY(float, mass, SetMass, GetMass);                                  // Mass property, in kg
		PROPERTY(float, inertia, SetInertia, GetInertia);                         // Inertia property, in kg-m^2
		PROPERTY(Vec2F, linearVelocity, SetLinearVelocity, GetLinearVelocity);    // Linear velocity property, in m/s
		PROPERTY(float, angularVelocity, SetAngularVelocity, GetAngularVelocity); // Angular velocity property, in rad/s
		PROPERTY(float, linearDamping, SetLinearDamping, GetLinearDamping);       // Linear damping property
		PROPERTY(float, angularDamping, SetAngularDamping, GetAngularDamping);    // Angular damping property
		PROPERTY(float, gravityScale, SetGravityScale, GetGravityScale);          // Gravity scale property
		PROPERTY(bool, isBullet, SetIsBullet, IsBullet);                          // Is body using continuous collision detection property
		PROPERTY(bool, isSleeping, SetIsSleeping, IsSleeping);                    // Is body sleeping property
		PROPERTY(bool, isFixedRotation, SetIsFixedRotation, IsFixedRotation);     // Is fixed rotation property

	public:
		// Default constructor
		RigidBody();

		// Copy-constructor
		RigidBody(const RigidBody& other);

		// Copy-operator
		RigidBody& operator=(const RigidBody& other);

		// Destructor
		~RigidBody();

		// Sets body type
		void SetBodyType(Type type);

		// Returns body type
		Type GetBodyType() const;

		// Sets mass, in kilograms
		void SetMass(float mass);

		// Returns mass in kilograms
		float GetMass() const;

		// Sets body inertia in kg-m^2
		void SetInertia(float inertia);

		// Returns body inertia kg-m^2
		float GetInertia() const;

		// Sets linear velocity in m/s 
		void SetLinearVelocity(const Vec2F& velocity);

		// Returns linear velocity in m/s
		Vec2F GetLinearVelocity() const;

		// Sets angular velocity in rad/s
		void SetAngularVelocity(float velocity);

		// Returns angular velocity in rad/s
		float GetAngularVelocity() const;

		// Sets linear damping
		void SetLinearDamping(float damping);

		// Returns linear damping
		float GetLinearDamping() const;

		// Sets angular damping
		void SetAngularDamping(float damping);

		// Returns angular damping
		float GetAngularDamping() const;

		// Sets gravity scale
		void SetGravityScale(float scale);

		// Returns gravity scale
		float GetGravityScale() const;

		// Sets is body using continuous collision detection
		void SetIsBullet(bool isBullet);

		// Returns is body using continuous collision detection
		bool IsBullet() const;

		// Sets body is sleeping
		void SetIsSleeping(bool isSleeping);

		// Returns body is sleeping
		bool IsSleeping() const;

		// Sets is body fixed rotation
		void SetIsFixedRotation(bool isFixedRotation);

		// Returns is body fixed rotation
		bool IsFixedRotation() const;

		SERIALIZABLE(RigidBody);

	protected:
		b2Body*    mBody = nullptr; // Box 2d physics body
		b2MassData mMassData;       // Body mass data

		Type mBodyType = Type::Dynamic; // Type of body @SERIALIZABLE

		float mMass = 1.0f;             // Mass @SERIALIZABLE
		float mInertia = 1.0f;          // Inertia @SERIALIZABLE
		float mLinearDamping = 0.0f;    // Linear damping @SERIALIZABLE
		float mAngularDamping = 0.0f;   // Angular damping @SERIALIZABLE
		float mGravityScale = 1.0f;     // Gravity scale @SERIALIZABLE
		bool  mIsBullet = false;        // Is using continuous collision detection @SERIALIZABLE
		bool  mIsFixedRotation = false; // Is fixed rotation @SERIALIZABLE

		Vector<ICollider*> mColliders; // Attached colliders list

	protected:
		// It is called when result enable was changed
		void OnEnableInHierarchyChanged() override;

		// Is is called when actor has added to scene; creates rigid body
		void OnAddToScene() override;

		// It is called when actor has removed from scene; destroys rigid body
		void OnRemoveFromScene() override;

		// Creates box2d body, registers in physics world
		void CreateBody();

		// Removes body
		void RemoveBody();

		// Adds collider to body
		void AddCollider(ICollider* collider);

		// Removes collider from body
		void RemoveCollider(ICollider* collider);

		// Converts body type
		static b2BodyType GetBodyType(Type type);

		friend class ICollider;
	};
}

PRE_ENUM_META(o2::RigidBody::Type);

CLASS_BASES_META(o2::RigidBody)
{
	BASE_CLASS(o2::Actor);
}
END_META;
CLASS_FIELDS_META(o2::RigidBody)
{
	PUBLIC_FIELD(bodyType);
	PUBLIC_FIELD(mass);
	PUBLIC_FIELD(inertia);
	PUBLIC_FIELD(linearVelocity);
	PUBLIC_FIELD(angularVelocity);
	PUBLIC_FIELD(linearDamping);
	PUBLIC_FIELD(angularDamping);
	PUBLIC_FIELD(gravityScale);
	PUBLIC_FIELD(isBullet);
	PUBLIC_FIELD(isSleeping);
	PUBLIC_FIELD(isFixedRotation);
	PROTECTED_FIELD(mBody).DEFAULT_VALUE(nullptr);
	PROTECTED_FIELD(mMassData);
	PROTECTED_FIELD(mBodyType).DEFAULT_VALUE(Type::Dynamic).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mMass).DEFAULT_VALUE(1.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mInertia).DEFAULT_VALUE(1.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLinearDamping).DEFAULT_VALUE(0.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAngularDamping).DEFAULT_VALUE(0.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mGravityScale).DEFAULT_VALUE(1.0f).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIsBullet).DEFAULT_VALUE(false).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIsFixedRotation).DEFAULT_VALUE(false).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mColliders);
}
END_META;
CLASS_METHODS_META(o2::RigidBody)
{

	PUBLIC_FUNCTION(void, SetBodyType, Type);
	PUBLIC_FUNCTION(Type, GetBodyType);
	PUBLIC_FUNCTION(void, SetMass, float);
	PUBLIC_FUNCTION(float, GetMass);
	PUBLIC_FUNCTION(void, SetInertia, float);
	PUBLIC_FUNCTION(float, GetInertia);
	PUBLIC_FUNCTION(void, SetLinearVelocity, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetLinearVelocity);
	PUBLIC_FUNCTION(void, SetAngularVelocity, float);
	PUBLIC_FUNCTION(float, GetAngularVelocity);
	PUBLIC_FUNCTION(void, SetLinearDamping, float);
	PUBLIC_FUNCTION(float, GetLinearDamping);
	PUBLIC_FUNCTION(void, SetAngularDamping, float);
	PUBLIC_FUNCTION(float, GetAngularDamping);
	PUBLIC_FUNCTION(void, SetGravityScale, float);
	PUBLIC_FUNCTION(float, GetGravityScale);
	PUBLIC_FUNCTION(void, SetIsBullet, bool);
	PUBLIC_FUNCTION(bool, IsBullet);
	PUBLIC_FUNCTION(void, SetIsSleeping, bool);
	PUBLIC_FUNCTION(bool, IsSleeping);
	PUBLIC_FUNCTION(void, SetIsFixedRotation, bool);
	PUBLIC_FUNCTION(bool, IsFixedRotation);
	PROTECTED_FUNCTION(void, OnEnableInHierarchyChanged);
	PROTECTED_FUNCTION(void, OnAddToScene);
	PROTECTED_FUNCTION(void, OnRemoveFromScene);
	PROTECTED_FUNCTION(void, CreateBody);
	PROTECTED_FUNCTION(void, RemoveBody);
	PROTECTED_FUNCTION(void, AddCollider, ICollider*);
	PROTECTED_FUNCTION(void, RemoveCollider, ICollider*);
	PROTECTED_STATIC_FUNCTION(b2BodyType, GetBodyType, Type);
}
END_META;
