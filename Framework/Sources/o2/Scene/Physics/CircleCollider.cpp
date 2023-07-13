#include "o2/stdafx.h"
#include "CircleCollider.h"

#include "o2/Physics/PhysicsWorld.h"

namespace o2
{
	CircleCollider::CircleCollider()
	{}

	CircleCollider::CircleCollider(const CircleCollider& other):
		ICollider(other), mRadius(other.mRadius), mFitByActor(other.mFitByActor)
	{}

	void CircleCollider::SetRadius(float radius)
	{
		mRadius = radius;
		OnShapeChanged();
	}

	float CircleCollider::GetRadius() const
	{
		return mRadius;
	}

	void CircleCollider::SetFitByActor(bool fit)
	{
		mFitByActor = fit;

		if (fit)
		{
			mRadius = mOwner->transform->GetSize().x*0.5f;
			OnShapeChanged();
		}
	}

	bool CircleCollider::IsFitByActor() const
	{
		return mFitByActor;
	}

	String CircleCollider::GetName()
	{
		return "Circle collider";
	}

	String CircleCollider::GetCategory()
	{
		return "Physics";
	}

	bool CircleCollider::IsAvailableFromCreateMenu()
	{
		return true;
	}

	CircleCollider& CircleCollider::operator=(const CircleCollider& other)
	{
		ICollider::operator=(other);
		mRadius = other.mRadius;
		mFitByActor = other.mFitByActor;
		return *this;
	}

	b2Shape* CircleCollider::GetShape(const Basis& transform)
	{
		float radius = mRadius;
		if (radius < FLT_EPSILON)
			radius = 1.0f;

		mShape.m_radius = radius*transform.xv.Length();
		mShape.m_p = transform.origin;

		return &mShape;
	}

	void CircleCollider::OnTransformChanged()
	{
		if (!o2Physics.IsUpdatingPhysicsNow() && mFitByActor)
		{
			float prevRadius = mRadius;
			mRadius = mOwner->transform->GetSize().x*0.5f;

			if (prevRadius != mRadius)
				OnShapeChanged();
		}

		ICollider::OnTransformChanged();
	}

#if IS_EDITOR
	void CircleCollider::OnAddedFromEditor()
	{
		mRadius = mOwner->transform->GetSize().x*0.5f;
	}
#endif
}
// --- META ---

DECLARE_CLASS(o2::CircleCollider);
// --- END META ---
