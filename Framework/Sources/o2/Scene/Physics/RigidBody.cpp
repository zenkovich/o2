#include "o2/stdafx.h"
#include "RigidBody.h"
#include "Box2D/Dynamics/b2Body.h"

namespace o2
{
	RigidBody::RigidBody()
	{

	}

	RigidBody::RigidBody(const RigidBody& other)
	{

	}

	RigidBody::~RigidBody()
	{

	}

	RigidBody& RigidBody::operator=(const RigidBody& other)
	{

	}

	void RigidBody::SetBodyType(Type type)
	{

	}

	RigidBody::Type RigidBody::GetBodyType() const
	{

	}

	void RigidBody::SetMass(float mass)
	{

	}

	float RigidBody::GetMass() const
	{

	}

	void RigidBody::SetInertia(float inertia)
	{

	}

	float RigidBody::GetInertia() const
	{

	}

	void RigidBody::SetLinearVelocity(const Vec2F& velocity)
	{

	}

	Vec2F RigidBody::GetLinearVelocity() const
	{

	}

	void RigidBody::SetAngularVelocity(float velocity)
	{

	}

	float RigidBody::GetAngularVelocity() const
	{

	}

	void RigidBody::SetLinearDamping(float damping)
	{

	}

	float RigidBody::GetLinearDamping() const
	{

	}

	void RigidBody::SetAngularDamping(float damping)
	{

	}

	float RigidBody::GetAngularDamping() const
	{

	}

	void RigidBody::SetGravityScale(float scale)
	{

	}

	float RigidBody::GetGravityScale() const
	{

	}

	void RigidBody::SetIsBullet(bool isBullet)
	{

	}

	bool RigidBody::IsBullet() const
	{

	}

	void RigidBody::SetIsSleeping(bool isSleeping)
	{

	}

	bool RigidBody::IsSleeping() const
	{

	}

	void RigidBody::SetIsFixedRotation(bool isFixedRotation)
	{

	}

	bool RigidBody::IsFixedRotation() const
	{

	}

	void RigidBody::OnEnableInHierarchyChanged()
	{

	}

	void RigidBody::OnTransformUpdated()
	{

	}

	void RigidBody::CreateBody()
	{
		b2BodyDef def;
		def.position = Convert(GetNodeImpl()->transform.GetWorldPosition2D());
		def.userData = this;
		def.active = GetNodeImpl()->IsActive();

		mMassData.mass = mMass;
		mMassData.I = mInertia;

		mBody = PhysicsWorld::Instance().mWorld.CreateBody(&def);
		mBody->SetMassData(&_massData);
		mBody->SetType(_bodyType == RigidBodyType::Dynamic ? b2_dynamicBody : (_bodyType == RigidBodyType::Kinematic ? b2_kinematicBody : b2_staticBody));
		mBody->SetLinearVelocity(Convert(_linearVelocity));
		mBody->SetAngularVelocity(_angularVelocity / 180.0f * math::PI);
		mBody->SetLinearDamping(_linearDamping);
		mBody->SetAngularDamping(_angularDamping);
		mBody->SetGravityScale(_gravityScale);
		mBody->SetBullet(_isBullet);
		mBody->SetAwake(!_isSleeping);
		mBody->SetFixedRotation(_isFixedRotation);

		SearchAndAddColliders(GetNodeImpl());
	}

	void RigidBody::RemoveBody()
	{

	}
}
