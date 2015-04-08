#include "geometry.h"

OPEN_O2_NAMESPACE

GroupGeometry::GroupGeometry()
{
}

GroupGeometry::~GroupGeometry()
{
}

int GroupGeometry::GetPartsCount() const
{
	return mParts.Count();
}

int GroupGeometry::AddPart(Geometry* geom)
{
	mParts.Add(geom);
	UpdateAABB();

	return mParts.Count() - 1;
}

void GroupGeometry::RemovePart(int idx)
{
	if (idx < 0 || idx >(int)mParts.Count() - 1)
		return;

	SafeRelease(mParts[idx]);
	mParts.RemoveAt(idx);

	UpdateAABB();
}

void GroupGeometry::RemoveAllParts()
{
	for (auto it : mParts)
		SafeRelease(it);

	mAABB = RectF();
}

bool GroupGeometry::IsInside(const Vec2F& point) const
{
	if (!mAABB.IsInside(point))
		return false;

	for (auto it : mParts)
	{
		if (it->IsInside(point))
			return true;
	}

	return false;
}

void GroupGeometry::UpdateAABB()
{
	if (mParts.Count() == 0)
		return;

	for (auto it : mParts)
	{
		RectF aabb = it->GetRect();
		if (it == *(mParts.Begin()))
			mAABB = aabb;
		else
			mAABB = mAABB + aabb;
	}
}

void GroupGeometry::SetPosition(const Vec2F& pos)
{
	Vec2F delta = pos - mPosition;
	mPosition = pos;

	for (auto it : mParts)
		it->SetPosition(it->GetPosition() + delta);
}

Vec2F GroupGeometry::GetPosition() const
{
	return mPosition;
}

RectGeometry::RectGeometry():
mRect() { }

RectGeometry::RectGeometry(const RectF& rect) :
mRect(rect) { }

RectGeometry::RectGeometry(float left, float top, float right, float down) :
mRect(left, top, right, down) { }

RectGeometry::RectGeometry(const Vec2F& minp, const Vec2F& maxp) :
mRect(minp, maxp) { }

bool RectGeometry::IsInside(const Vec2F& point) const
{
	return mRect.IsInside(point);
}

RectF RectGeometry::GetRect() const
{
	return mRect;
}

Vec2F RectGeometry::GetPosition() const
{
	return mRect.LeftTop();
}

void RectGeometry::SetPosition(const Vec2F& pos)
{
	mRect = mRect + (pos - mRect.LeftTop());
}

void RectGeometry::SetSize(const Vec2F& size)
{
	mRect.right = mRect.left + size.x; mRect.bottom = mRect.top + size.y;
}

void RectGeometry::Set(const RectF& rect)
{
	mRect = rect;
}

void RectGeometry::Set(const Vec2F& minp, const Vec2F& maxp)
{
	mRect.left = minp.x; mRect.right = maxp.x;
	mRect.top = minp.y; mRect.bottom = maxp.y;
}


CircleGeometry::CircleGeometry():
mRadius(0), mSqrRadius(0) { }

CircleGeometry::CircleGeometry(const Vec2F& center, float radius) :
mCenter(center), mRadius(radius)
{
	mSqrRadius = radius*radius;
}

bool CircleGeometry::IsInside(const Vec2F& point) const
{
	Vec2F d = point - mCenter; return d.Dot(d) < mSqrRadius;
}

RectF CircleGeometry::GetRect() const
{
	return RectF(mCenter - Vec2F(mRadius, mRadius), mCenter + Vec2F(mRadius, mRadius));
}

Vec2F CircleGeometry::GetPosition() const
{
	return mCenter;
}

void CircleGeometry::SetPosition(const Vec2F& pos)
{
	mCenter = pos;
}

void CircleGeometry::Set(const Vec2F& center, float radius)
{
	mCenter = center; mRadius = radius;
}

CLOSE_O2_NAMESPACE