#include "ActorTransform.h"

#include "Actor.h"

namespace o2
{
	ActorTransform::ActorTransform(const Vec2F& size /*= Vec2F()*/, const Vec2F& position /*= Vec2F()*/,
								   float angle /*= 0.0f*/, const Vec2F& scale /*= Vec2F(1.0f, 1.0f)*/,
								   const Vec2F& pivot /*= Vec2F(0.5f, 0.5f)*/):
		Transform(size, position, angle, scale, pivot), mOwner(nullptr)
	{
		InitializeProperties();
	}

	ActorTransform::ActorTransform(const ActorTransform& other):
		Transform(other), mOwner(nullptr)
	{
		InitializeProperties();
	}

	ActorTransform& ActorTransform::operator=(const ActorTransform& other)
	{
		Transform::operator=(other);
		UpdateTransform();
		return *this;
	}

	ActorTransform& ActorTransform::operator=(const Transform& other)
	{
		SetWorldBasis(other.GetBasis());
		return *this;
	}

	Actor* ActorTransform::GetOwnerActor() const
	{
		return mOwner;
	}

	void ActorTransform::SetWorldPosition(const Vec2F& position)
	{
		CheckParentInvTransform();
		SetPosition(position*mParentInvertedTransform);
	}

	Vec2F ActorTransform::GetWorldPosition() const
	{
		return mPosition*mParentTransform;
	}

	void ActorTransform::SetWorldRect(const RectF& rect)
	{
		CheckParentInvTransform();
		SetRect(RectF(rect.LeftBottom()*mParentInvertedTransform, rect.RightTop()*mParentInvertedTransform));
	}

	RectF ActorTransform::GetWorldRect() const
	{
		RectF localRect = GetRect();
		RectF worldRect(localRect.LeftBottom()*mParentTransform, localRect.RightTop()*mParentTransform);
		return worldRect;
	}

	void ActorTransform::SetWorldAngle(float rad)
	{
		SetAngle(rad - mParentTransform.GetAngle());
	}

	float ActorTransform::GetWorldAngle() const
	{
		return mWorldTransform.GetAngle();
	}

	void ActorTransform::SetWorldBasis(const Basis& basis)
	{
		CheckParentInvTransform();
		SetBasis(basis*mParentInvertedTransform);
	}

	Basis ActorTransform::GetWorldBasis() const
	{
		return mWorldTransform;
	}

	void ActorTransform::SetWorldNonSizedBasis(const Basis& basis)
	{
		CheckParentInvTransform();
		SetNonSizedBasis(basis*mParentInvertedTransform);
	}

	Basis ActorTransform::GetWorldNonSizedBasis() const
	{
		return mWorldNonSizedTransform;
	}

	void ActorTransform::SetWorldAxisAlignedRect(const RectF& rect)
	{
		CheckParentInvTransform();
		SetAxisAlignedRect(RectF(rect.LeftBottom()*mParentInvertedTransform, rect.RightTop()*mParentInvertedTransform));
	}

	RectF ActorTransform::GetWorldAxisAlignedRect() const
	{
		RectF localAARect = GetRect();
		RectF worldAARect(localAARect.LeftBottom()*mParentTransform, localAARect.RightTop()*mParentTransform);
		return worldAARect;
	}

	void ActorTransform::SetWorldLeftTop(const Vec2F& position)
	{

	}

	Vec2F ActorTransform::GetWorldLeftTop() const
	{
		return Vec2F();
	}

	void ActorTransform::SetWorldRightTop(const Vec2F& position)
	{

	}

	Vec2F ActorTransform::GetWorldRightTop() const
	{
		return Vec2F();
	}

	void ActorTransform::SetWorldLeftBottom(const Vec2F& position)
	{

	}

	Vec2F ActorTransform::GetWorldLeftBottom() const
	{
		return Vec2F();
	}

	void ActorTransform::SetWorldRightBottom(const Vec2F& position)
	{

	}

	Vec2F ActorTransform::GetWorldRightBottom() const
	{
		return Vec2F();
	}

	void ActorTransform::SetWorldCenter(const Vec2F& position)
	{

	}

	Vec2F ActorTransform::GetWorldCenter() const
	{
		return Vec2F();
	}

	void ActorTransform::SetRight(const Vec2F& dir)
	{

	}

	Vec2F ActorTransform::GetRight() const
	{
		return mWorldNonSizedTransform.xv;
	}

	void ActorTransform::SetLeft(const Vec2F& dir)
	{

	}

	Vec2F ActorTransform::GetLeft() const
	{
		return mWorldNonSizedTransform.xv.Inverted();
	}

	void ActorTransform::SetUp(const Vec2F& dir)
	{

	}

	Vec2F ActorTransform::GetUp() const
	{
		return mWorldNonSizedTransform.yv;
	}

	void ActorTransform::SetDown(const Vec2F& dir)
	{

	}

	Vec2F ActorTransform::GetDown() const
	{
		return mWorldNonSizedTransform.yv.Inverted();
	}

	void ActorTransform::LookAt(const Vec2F& worldPoint)
	{

	}

	Vec2F ActorTransform::World2LocalPoint(const Vec2F& worldPoint) const
	{
		Vec2F nx = mWorldTransform.xv, ny = mWorldTransform.yv, offs = mWorldTransform.offs, w = worldPoint;
		float lx = (w.x*ny.y - offs.x*ny.y - w.y*ny.x + offs.y*ny.x) / (nx.x*ny.y - ny.x*nx.y);
		float ly = (w.y - offs.y - nx.y*lx) / ny.y;
		return Vec2F(lx, ly)*mSize;
	}

	Vec2F ActorTransform::Local2WorldPoint(const Vec2F& localPoint) const
	{
		return mWorldTransform*(localPoint / mSize);
	}

	Vec2F ActorTransform::World2LocalDir(const Vec2F& worldDir) const
	{
		Vec2F nx = mWorldTransform.xv / (mSize.x*mScale.x), ny = mWorldTransform.yv / (mSize.y*mScale.y), wd = worldDir;
		float ldy = (wd.x*nx.y - wd.y*nx.x) / (nx.y*ny.x - ny.y*nx.x);
		float ldx = (wd.x - ny.x*ldy) / nx.x;
		return Vec2F(ldx, ldy);
	}

	Vec2F ActorTransform::Local2WorldDir(const Vec2F& localDir) const
	{
		Vec2F nx = mWorldTransform.xv / (mSize.x*mScale.x), ny = mWorldTransform.yv / (mSize.y*mScale.y);
		return nx*localDir.x + ny*localDir.y;
	}

	bool ActorTransform::IsPointInside(const Vec2F& point) const
	{
		Vec2F rs = mScale*mSize;
		Vec2F nx = mWorldTransform.xv / rs.x, ny = mWorldTransform.yv / rs.y;
		Vec2F lp = point - mWorldTransform.offs;

		float dx = lp.Dot(nx);
		float dy = lp.Dot(ny);

		return dx >= 0.0f && dx <= rs.x && dy >= 0.0f && dy < rs.y;
	}

	void ActorTransform::SetOwner(Actor* actor)
	{
		mOwner = actor;
		UpdateTransform();
	}

	void ActorTransform::UpdateTransform()
	{
		Transform::UpdateTransform();

		if (mOwner && mOwner->mParent)
		{
			mParentTransform = mOwner->mParent->transform.mWorldNonSizedTransform;
			mWorldTransform = mTransform*mParentTransform;
			mWorldNonSizedTransform = mNonSizedTransform*mParentTransform;
		}
		else
		{
			mWorldNonSizedTransform = mNonSizedTransform;
			mWorldTransform = mTransform;
		}

		mIsParentInvTransformActual = false;

		if (mOwner)
			mOwner->OnTransformChanged();
	}

	void ActorTransform::CheckParentInvTransform()
	{
		if (mIsParentInvTransformActual)
			return;

		mIsParentInvTransformActual = true;

		if (mOwner && mOwner->mParent)
			mParentInvertedTransform = mOwner->mParent->transform.mWorldNonSizedTransform.Inverted();
		else
			mParentInvertedTransform = Basis::Identity();
	}

	void ActorTransform::InitializeProperties()
	{
		INITIALIZE_GETTER(ActorTransform, actor, GetOwnerActor);
		INITIALIZE_PROPERTY(ActorTransform, worldPosition, SetWorldPosition, GetWorldPosition);
		INITIALIZE_PROPERTY(ActorTransform, worldRect, SetWorldRect, GetWorldRect);
		INITIALIZE_PROPERTY(ActorTransform, worldAngle, SetWorldAngle, GetWorldAngle);
		INITIALIZE_PROPERTY(ActorTransform, worldBasis, SetWorldBasis, GetWorldBasis);
		INITIALIZE_PROPERTY(ActorTransform, worldNonSizedBasis, SetWorldNonSizedBasis, GetWorldNonSizedBasis);
		INITIALIZE_PROPERTY(ActorTransform, worldAABB, SetWorldAxisAlignedRect, GetWorldAxisAlignedRect);
	}

}