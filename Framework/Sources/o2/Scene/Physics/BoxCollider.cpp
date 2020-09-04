#include "o2/stdafx.h"
#include "BoxCollider.h"

#include "o2/Physics/PhysicsWorld.h"

namespace o2
{
	BoxCollider::BoxCollider()
	{}

	BoxCollider::BoxCollider(const BoxCollider& other):
		ICollider(other), mSize(other.mSize), mFitByActor(other.mFitByActor)
	{}

	void BoxCollider::SetSize(const Vec2F& size)
	{
		mSize = size;
		OnShapeChanged();
	}

	Vec2F BoxCollider::GetSize() const
	{
		return mSize;
	}

	void BoxCollider::SetFitByActor(bool fit)
	{
		mFitByActor = fit;

		if (fit)
		{
			mSize = mOwner->transform->GetSize();
			OnShapeChanged();
		}
	}

	bool BoxCollider::IsFitByActor() const
	{
		return mFitByActor;
	}

	String BoxCollider::GetName()
	{
		return "Box collider";
	}

	String BoxCollider::GetCategory()
	{
		return "Physics";
	}

	BoxCollider& BoxCollider::operator=(const BoxCollider& other)
	{
		ICollider::operator=(other);
		mSize = other.mSize;
		mFitByActor = other.mFitByActor;
		return *this;
	}

	b2Shape* BoxCollider::GetShape(const Basis& transform)
	{
		Vec2F halfSize = mSize*0.5f;

		if (halfSize.x < FLT_EPSILON)
			halfSize.x = 1.0f;

		if (halfSize.y < FLT_EPSILON)
			halfSize.y = 1.0f;

		b2Vec2 verticies[4];
		verticies[0] = Vec2F(-halfSize.x, -halfSize.y)*transform;
		verticies[1] = Vec2F(halfSize.x, -halfSize.y)*transform;
		verticies[2] = Vec2F(halfSize.x, halfSize.y)*transform;
		verticies[3] = Vec2F(-halfSize.x, halfSize.y)*transform;

		mShape.Set(verticies, 4);

		return &mShape;
	}

	void BoxCollider::OnTransformChanged()
	{
		if (!o2Physics.IsUpdatingPhysicsNow() && mFitByActor)
		{
			Vec2F prevSize = mSize;
			mSize = mOwner->transform->GetSize();

			if (prevSize != mSize)
				OnShapeChanged();
		}

		ICollider::OnTransformChanged();
	}

#if IS_EDITOR
	void BoxCollider::OnAddedFromEditor()
	{
		mSize = mOwner->transform->GetSize();
	}
#endif
}

DECLARE_CLASS(o2::BoxCollider);
