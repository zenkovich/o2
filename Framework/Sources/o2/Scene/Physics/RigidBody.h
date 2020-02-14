#pragma once

#include "o2/Scene/Actor.h"
#include "Box2D/Collision/Shapes/b2Shape.h"
#include "Box2D/Dynamics/b2Body.h"

namespace o2
{
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

	protected:
		// It is called when result enable was changed
		void OnEnableInHierarchyChanged() override;

		// It is called when transformation was changed and updated
		void OnTransformUpdated() override;

		// Creates box2d body, registers in physics world
		void CreateBody();

		// Removes body
		void RemoveBody();

		// Converts body type
		static b2BodyType GetBodyType(Type type);
	};
}
