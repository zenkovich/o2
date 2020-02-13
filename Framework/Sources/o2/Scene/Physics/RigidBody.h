#pragma once

#include "o2/Scene/Actor.h"

class b2Body;

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
		PROPERTY(float, mass, SetMass, GetMass);
		PROPERTY(float, inertia, SetInertia, GetInertia);
		PROPERTY(Vec2F, linearVelocity, SetLinearVelocity, GetLinearVelocity);
		PROPERTY(float, angularVelocity, SetAngularVelocity, GetAngularVelocity);
		PROPERTY(float, linearDamping, SetLinearDamping, GetLinearDamping);
		PROPERTY(float, angularDamping, SetAngularDamping, GetAngularDamping);
		PROPERTY(float, gravityScale, SetGravityScale, GetGravityScale);
		PROPERTY(bool, isBullet, SetIsBullet, IsBullet);
		PROPERTY(bool, isSleeping, SetIsSleeping, IsSleeping);
		PROPERTY(bool, isFixedRotation, SetIsFixedRotation, IsFixedRotation);

	public:
		// Default constructor
		RigidBody();

		// Copy-constructor
		RigidBody(const RigidBody& other);

		// Copy-operator
		RigidBody& operator=(const RigidBody& other);

		// Destructor
		~RigidBody();

		// Sets mass, in kilograms
		void SetMass(float mass);

		// Returns mass in kilograms
		float GetMass() const;

		void SetInertia(float inertia);
		float GetInertia() const;

		void SetLinearVelocity(const Vec2F& velocity);
		Vec2F GetLinearVelocity() const;

		void SetAngularVelocity(float velocity);
		float GetAngularVelocity() const;

		void SetLinearDamping(float damping);
		float GetLinearDamping() const;

		void SetAngularDamping(float damping);
		float GetAngularDamping() const;

		void SetGravityScale(float scale);
		float GetGravityScale() const;

		void SetIsBullet(bool isBullet);
		bool IsBullet() const;

		void SetIsSleeping(bool isSleeping);
		bool IsSleeping() const;

		void SetIsFixedRotation(bool isFixedRotation);
		bool IsFixedRotation() const;

		SERIALIZABLE(RigidBody);

	protected:
		b2Body* mBody = nullptr; // Box 2d physics body

		Type mType = Type::Dynamic; // Type of body @SERIALIZABLE

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
	};
}
