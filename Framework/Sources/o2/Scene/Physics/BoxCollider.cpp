#include "o2/stdafx.h"
#include "BoxCollider.h"

namespace o2
{
	BoxCollider::BoxCollider()
	{}

	BoxCollider::BoxCollider(const BoxCollider& other):
		ICollider(other), mSize(other.mSize)
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
		return *this;
	}

	b2Shape* BoxCollider::GetShape(const Basis& transform)
	{
		Vec2F halfSize = mSize*0.5f;
		mShape.SetAsBox(halfSize.x, halfSize.y, Vec2F(), transform.GetAngle());

		return &mShape;
	}
}

DECLARE_CLASS(o2::BoxCollider);
