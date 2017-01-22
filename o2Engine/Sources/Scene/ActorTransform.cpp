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

	bool ActorTransform::operator==(const ActorTransform& other) const
	{
		return Transform::operator==(other);
	}

	Actor* ActorTransform::GetOwnerActor() const
	{
		return mOwner;
	}

	void ActorTransform::SetWorldPivot(const Vec2F& pivot)
	{
		Basis trasform = mWorldTransform;
		SetSizePivot(World2LocalPoint(pivot));
		SetWorldBasis(trasform);
	}

	Vec2F ActorTransform::GetWorldPivot() const
	{
		return Local2WorldPoint(mPivot*mSize);
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
		Basis transformed = mWorldTransform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mWorldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mWorldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mWorldTransform.yv);

		transformed.offs += frameDeltaX;
		transformed.xv -= frameDeltaX;
		transformed.yv += frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldLeftTop() const
	{
		return mWorldTransform.offs + mWorldTransform.yv;
	}

	void ActorTransform::SetWorldRightTop(const Vec2F& position)
	{
		Basis transformed = mWorldTransform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mWorldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mWorldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mWorldTransform.yv);

		transformed.xv += frameDeltaX;
		transformed.yv += frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldRightTop() const
	{
		return mWorldTransform.offs + mWorldTransform.yv + mWorldTransform.xv;
	}

	void ActorTransform::SetWorldLeftBottom(const Vec2F& position)
	{
		Basis transformed = mWorldTransform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mWorldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mWorldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mWorldTransform.yv);

		transformed.offs += frameDeltaX + frameDeltaY;
		transformed.xv -= frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldLeftBottom() const
	{
		return mWorldTransform.offs;
	}

	void ActorTransform::SetWorldRightBottom(const Vec2F& position)
	{
		Basis transformed = mWorldTransform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mWorldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mWorldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mWorldTransform.yv);

		transformed.offs += frameDeltaY;
		transformed.xv += frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldRightBottom() const
	{
		return mWorldTransform.offs + mWorldTransform.xv;
	}

	void ActorTransform::SetWorldCenter(const Vec2F& position)
	{
		Vec2F translate = position - GetWorldCenter();
		SetWorldBasis(mWorldTransform*Basis::Translated(translate));
	}

	Vec2F ActorTransform::GetWorldCenter() const
	{
		return mWorldTransform.offs + (mWorldTransform.xv + mWorldTransform.yv)*0.5f;
	}

	void ActorTransform::SetRight(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetRight().SignedAngle(dir));
		SetWorldBasis(mWorldTransform*transf);
	}

	Vec2F ActorTransform::GetRight() const
	{
		return mWorldNonSizedTransform.xv;
	}

	void ActorTransform::SetLeft(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetLeft().SignedAngle(dir));
		SetWorldBasis(mWorldTransform*transf);
	}

	Vec2F ActorTransform::GetLeft() const
	{
		return mWorldNonSizedTransform.xv.Inverted();
	}

	void ActorTransform::SetUp(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetUp().SignedAngle(dir));
		SetWorldBasis(mWorldTransform*transf);
	}

	Vec2F ActorTransform::GetUp() const
	{
		return mWorldNonSizedTransform.yv;
	}

	void ActorTransform::SetDown(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetDown().SignedAngle(dir));
		SetWorldBasis(mWorldTransform*transf);
	}

	Vec2F ActorTransform::GetDown() const
	{
		return mWorldNonSizedTransform.yv.Inverted();
	}

	void ActorTransform::LookAt(const Vec2F& worldPoint)
	{
		SetUp((worldPoint - mPosition).Normalized());
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

CLASS_META(o2::ActorTransform)
{
	BASE_CLASS(o2::Transform);

	PUBLIC_FIELD(actor);
	PUBLIC_FIELD(worldPosition);
	PUBLIC_FIELD(worldRect);
	PUBLIC_FIELD(worldAngle);
	PUBLIC_FIELD(worldBasis);
	PUBLIC_FIELD(worldNonSizedBasis);
	PUBLIC_FIELD(worldAABB);
	PROTECTED_FIELD(mWorldNonSizedTransform);
	PROTECTED_FIELD(mWorldTransform);
	PROTECTED_FIELD(mParentInvertedTransform);
	PROTECTED_FIELD(mParentTransform);
	PROTECTED_FIELD(mIsParentInvTransformActual);
	PROTECTED_FIELD(mOwner);

	PUBLIC_FUNCTION(Actor*, GetOwnerActor);
	PUBLIC_FUNCTION(void, SetWorldPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldPivot);
	PUBLIC_FUNCTION(void, SetWorldPosition, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldPosition);
	PUBLIC_FUNCTION(void, SetWorldRect, const RectF&);
	PUBLIC_FUNCTION(RectF, GetWorldRect);
	PUBLIC_FUNCTION(void, SetWorldAngle, float);
	PUBLIC_FUNCTION(float, GetWorldAngle);
	PUBLIC_FUNCTION(void, SetWorldBasis, const Basis&);
	PUBLIC_FUNCTION(Basis, GetWorldBasis);
	PUBLIC_FUNCTION(void, SetWorldNonSizedBasis, const Basis&);
	PUBLIC_FUNCTION(Basis, GetWorldNonSizedBasis);
	PUBLIC_FUNCTION(void, SetWorldAxisAlignedRect, const RectF&);
	PUBLIC_FUNCTION(RectF, GetWorldAxisAlignedRect);
	PUBLIC_FUNCTION(void, SetWorldLeftTop, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldLeftTop);
	PUBLIC_FUNCTION(void, SetWorldRightTop, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldRightTop);
	PUBLIC_FUNCTION(void, SetWorldLeftBottom, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldLeftBottom);
	PUBLIC_FUNCTION(void, SetWorldRightBottom, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldRightBottom);
	PUBLIC_FUNCTION(void, SetWorldCenter, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldCenter);
	PUBLIC_FUNCTION(void, SetRight, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetRight);
	PUBLIC_FUNCTION(void, SetLeft, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetLeft);
	PUBLIC_FUNCTION(void, SetUp, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetUp);
	PUBLIC_FUNCTION(void, SetDown, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetDown);
	PUBLIC_FUNCTION(void, LookAt, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, World2LocalPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, Local2WorldPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, World2LocalDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, Local2WorldDir, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsPointInside, const Vec2F&);
	PROTECTED_FUNCTION(void, SetOwner, Actor*);
	PROTECTED_FUNCTION(void, UpdateTransform);
	PROTECTED_FUNCTION(void, CheckParentInvTransform);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
 