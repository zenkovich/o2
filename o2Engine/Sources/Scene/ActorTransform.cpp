#include "ActorTransform.h"

#include "Actor.h"

namespace o2
{
	ActorTransform::ActorTransform(const Vec2F& size /*= Vec2F()*/, const Vec2F& position /*= Vec2F()*/,
								   float angle /*= 0.0f*/, const Vec2F& scale /*= Vec2F(1.0f, 1.0f)*/,
								   const Vec2F& pivot /*= Vec2F(0.5f, 0.5f)*/)
	{
		mData = mnew Data();

		mData->mSize = size;
		mData->mPosition = position;
		mData->mAngle = angle;
		mData->mScale = scale;
		mData->mPivot = pivot;
		mData->mShear = 0;

		mData->mOwner = nullptr;

		InitializeProperties();
	}

	ActorTransform::ActorTransform(const ActorTransform& other)
	{
		mData = mnew Data();

		mData->mSize = other.mData->mSize;
		mData->mPosition = other.mData->mPosition;
		mData->mAngle = other.mData->mAngle;
		mData->mScale = other.mData->mScale;
		mData->mPivot = other.mData->mPivot;
		mData->mShear = other.mData->mShear;

		mData->mOwner = nullptr;

		InitializeProperties();
	}

	ActorTransform& ActorTransform::operator=(const ActorTransform& other)
	{
		mData->mSize = other.mData->mSize;
		mData->mPosition = other.mData->mPosition;
		mData->mAngle = other.mData->mAngle;
		mData->mScale = other.mData->mScale;
		mData->mPivot = other.mData->mPivot;
		mData->mShear = other.mData->mShear;

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
		return Math::Equals(mData->mSize, other.mData->mSize) &&
			Math::Equals(mData->mPosition, other.mData->mPosition) &&
			Math::Equals(mData->mAngle, other.mData->mAngle) &&
			Math::Equals(mData->mScale, other.mData->mScale) &&
			Math::Equals(mData->mPivot, other.mData->mPivot) &&
			Math::Equals(mData->mShear, other.mData->mShear);
	}

	void ActorTransform::OnDeserialized(const DataNode& node)
	{
		UpdateTransform();
	}

	void ActorTransform::SetPosition(const Vec2F& position)
	{
		mData->mPosition = position;
		UpdateTransform();
	}

	Vec2F ActorTransform::GetPosition() const
	{
		return mData->mPosition;
	}

	void ActorTransform::SetSize(const Vec2F& size)
	{
		mData->mSize = size;
		UpdateTransform();
	}

	Vec2F ActorTransform::GetSize() const
	{
		return mData->mSize;
	}

	void ActorTransform::SetPivot(const Vec2F& pivot)
	{
		mData->mPivot = pivot;
		UpdateTransform();
	}

	Vec2F ActorTransform::GetPivot() const
	{
		return mData->mPivot;
	}

	void ActorTransform::SetSizePivot(const Vec2F& relPivot)
	{
		SetPivot(relPivot / mData->mSize);
	}

	Vec2F ActorTransform::GetSizePivot() const
	{
		return mData->mPivot*mData->mSize;
	}

	void ActorTransform::SetRect(const RectF& rect)
	{
		mData->mSize = rect.Size();
		mData->mPosition = rect.LeftBottom() + Vec2F(mData->mSize.x*mData->mScale.x, mData->mSize.y*mData->mScale.y)*mData->mPivot;

		UpdateTransform();
	}

	RectF ActorTransform::GetRect() const
	{
		return RectF(mData->mTransform.offs, mData->mTransform.offs + mData->mTransform.xv + mData->mTransform.yv);
	}

	void ActorTransform::SetScale(const Vec2F& scale)
	{
		mData->mScale = scale;
		UpdateTransform();
	}

	Vec2F ActorTransform::GetScale() const
	{
		return mData->mScale;
	}

	void ActorTransform::SetAngle(float rad)
	{
		mData->mAngle = rad;
		UpdateTransform();
	}

	float ActorTransform::GetAngle() const
	{
		return mData->mAngle;
	}

	void ActorTransform::SetAngleDegrees(float deg)
	{
		mData->mAngle = Math::Deg2rad(deg);
		UpdateTransform();
	}

	float ActorTransform::GetAngleDegrees() const
	{
		return Math::Rad2deg(mData->mAngle);
	}

	void ActorTransform::SetShear(float shear)
	{
		mData->mShear = shear;
		UpdateTransform();
	}

	float ActorTransform::GetShear() const
	{
		return mData->mShear;
	}

	void ActorTransform::SetBasis(const Basis& basis)
	{
		Vec2F offset, scale;
		float angle, shear;
		basis.Decompose(&offset, &angle, &scale, &shear);

		mData->mAngle = angle;
		mData->mSize = scale / mData->mScale;
		mData->mShear = shear;

		mData->mPosition = basis.offs + basis.xv*mData->mPivot.x + basis.yv*mData->mPivot.y;
		UpdateTransform();
	}

	Basis ActorTransform::GetBasis() const
	{
		return mData->mTransform;
	}

	void ActorTransform::SetNonSizedBasis(const Basis& basis)
	{
		Vec2F offset, scale;
		float angle, shear;
		basis.Decompose(&offset, &angle, &scale, &shear);

		mData->mAngle = angle;
		mData->mScale = scale;
		mData->mShear = shear;

		mData->mPosition = basis.offs + basis.xv*mData->mPivot.x*mData->mSize.x + basis.yv*mData->mPivot.y*mData->mSize.y;
		UpdateTransform();
	}

	Basis ActorTransform::GetNonSizedBasis() const
	{
		return mData->mNonSizedTransform;
	}

	void ActorTransform::SetAxisAlignedRect(const RectF& rect)
	{
		RectF curRect = GetAxisAlignedRect();

		Basis curRectBasis(curRect.LeftBottom(), Vec2F::Right()*curRect.Width(), Vec2F::Up()*curRect.Height());
		Basis rectBasis(rect.LeftBottom(), Vec2F::Right()*rect.Width(), Vec2F::Up()*rect.Height());

		SetBasis(mData->mTransform*curRectBasis.Inverted()*rectBasis);
	}

	RectF ActorTransform::GetAxisAlignedRect() const
	{
		return mData->mTransform.AABB();
	}

	void ActorTransform::SetLeftTop(const Vec2F& position)
	{
		Basis transformed = mData->mTransform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mData->mTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->mTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->mTransform.yv);

		transformed.offs += frameDeltaX;
		transformed.xv -= frameDeltaX;
		transformed.yv += frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F ActorTransform::GetLeftTop() const
	{
		return mData->mTransform.offs + mData->mTransform.yv;
	}

	void ActorTransform::SetRightTop(const Vec2F& position)
	{
		Basis transformed = mData->mTransform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mData->mTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->mTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->mTransform.yv);

		transformed.xv += frameDeltaX;
		transformed.yv += frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F ActorTransform::GetRightTop() const
	{
		return mData->mTransform.offs + mData->mTransform.yv + mData->mTransform.xv;
	}

	void ActorTransform::SetLeftBottom(const Vec2F& position)
	{
		Basis transformed = mData->mTransform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mData->mTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->mTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->mTransform.yv);

		transformed.offs += frameDeltaX + frameDeltaY;
		transformed.xv -= frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F ActorTransform::GetLeftBottom() const
	{
		return mData->mTransform.offs;
	}

	void ActorTransform::SetRightBottom(const Vec2F& position)
	{
		Basis transformed = mData->mTransform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mData->mTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->mTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->mTransform.yv);

		transformed.offs += frameDeltaY;
		transformed.xv += frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F ActorTransform::GetRightBottom() const
	{
		return mData->mTransform.offs + mData->mTransform.xv;
	}

	void ActorTransform::SetCenter(const Vec2F& position)
	{
		Vec2F translate = position - GetCenter();
		SetBasis(mData->mTransform*Basis::Translated(translate));
	}

	Vec2F ActorTransform::GetCenter() const
	{
		return mData->mTransform.offs + (mData->mTransform.xv + mData->mTransform.yv)*0.5f;
	}

	Actor* ActorTransform::GetOwnerActor() const
	{
		return mData->mOwner;
	}

	void ActorTransform::SetWorldPivot(const Vec2F& pivot)
	{
		Basis trasform = mData->mWorldTransform;
		SetSizePivot(World2LocalPoint(pivot));
		SetWorldBasis(trasform);
	}

	Vec2F ActorTransform::GetWorldPivot() const
	{
		return Local2WorldPoint(mData->mPivot*mData->mSize);
	}

	void ActorTransform::SetWorldPosition(const Vec2F& position)
	{
		CheckParentInvTransform();
		SetPosition(position*mData->mParentInvertedTransform);
	}

	Vec2F ActorTransform::GetWorldPosition() const
	{
		return mData->mPosition*mData->mParentTransform;
	}

	void ActorTransform::SetWorldRect(const RectF& rect)
	{
		CheckParentInvTransform();
		SetRect(RectF(rect.LeftBottom()*mData->mParentInvertedTransform, rect.RightTop()*mData->mParentInvertedTransform));
	}

	RectF ActorTransform::GetWorldRect() const
	{
		RectF localRect = GetRect();
		RectF worldRect(localRect.LeftBottom()*mData->mParentTransform, localRect.RightTop()*mData->mParentTransform);
		return worldRect;
	}

	void ActorTransform::SetWorldAngle(float rad)
	{
		SetAngle(rad - mData->mParentTransform.GetAngle());
	}

	float ActorTransform::GetWorldAngle() const
	{
		return mData->mWorldTransform.GetAngle();
	}

	void ActorTransform::SetWorldBasis(const Basis& basis)
	{
		CheckParentInvTransform();
		SetBasis(basis*mData->mParentInvertedTransform);
	}

	Basis ActorTransform::GetWorldBasis() const
	{
		return mData->mWorldTransform;
	}

	void ActorTransform::SetWorldNonSizedBasis(const Basis& basis)
	{
		CheckParentInvTransform();
		SetNonSizedBasis(basis*mData->mParentInvertedTransform);
	}

	Basis ActorTransform::GetWorldNonSizedBasis() const
	{
		return mData->mWorldNonSizedTransform;
	}

	void ActorTransform::SetWorldAxisAlignedRect(const RectF& rect)
	{
		CheckParentInvTransform();
		SetAxisAlignedRect(RectF(rect.LeftBottom()*mData->mParentInvertedTransform, rect.RightTop()*mData->mParentInvertedTransform));
	}

	RectF ActorTransform::GetWorldAxisAlignedRect() const
	{
		RectF localAARect = GetRect();
		RectF worldAARect(localAARect.LeftBottom()*mData->mParentTransform, localAARect.RightTop()*mData->mParentTransform);
		return worldAARect;
	}

	void ActorTransform::SetWorldLeftTop(const Vec2F& position)
	{
		Basis transformed = mData->mWorldTransform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mData->mWorldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->mWorldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->mWorldTransform.yv);

		transformed.offs += frameDeltaX;
		transformed.xv -= frameDeltaX;
		transformed.yv += frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldLeftTop() const
	{
		return mData->mWorldTransform.offs + mData->mWorldTransform.yv;
	}

	void ActorTransform::SetWorldRightTop(const Vec2F& position)
	{
		Basis transformed = mData->mWorldTransform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mData->mWorldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->mWorldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->mWorldTransform.yv);

		transformed.xv += frameDeltaX;
		transformed.yv += frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldRightTop() const
	{
		return mData->mWorldTransform.offs + mData->mWorldTransform.yv + mData->mWorldTransform.xv;
	}

	void ActorTransform::SetWorldLeftBottom(const Vec2F& position)
	{
		Basis transformed = mData->mWorldTransform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mData->mWorldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->mWorldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->mWorldTransform.yv);

		transformed.offs += frameDeltaX + frameDeltaY;
		transformed.xv -= frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldLeftBottom() const
	{
		return mData->mWorldTransform.offs;
	}

	void ActorTransform::SetWorldRightBottom(const Vec2F& position)
	{
		Basis transformed = mData->mWorldTransform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mData->mWorldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->mWorldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->mWorldTransform.yv);

		transformed.offs += frameDeltaY;
		transformed.xv += frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldRightBottom() const
	{
		return mData->mWorldTransform.offs + mData->mWorldTransform.xv;
	}

	void ActorTransform::SetWorldCenter(const Vec2F& position)
	{
		Vec2F translate = position - GetWorldCenter();
		SetWorldBasis(mData->mWorldTransform*Basis::Translated(translate));
	}

	Vec2F ActorTransform::GetWorldCenter() const
	{
		return mData->mWorldTransform.offs + (mData->mWorldTransform.xv + mData->mWorldTransform.yv)*0.5f;
	}

	void ActorTransform::SetRight(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetRight().SignedAngle(dir));
		SetWorldBasis(mData->mWorldTransform*transf);
	}

	Vec2F ActorTransform::GetRight() const
	{
		return mData->mWorldNonSizedTransform.xv;
	}

	void ActorTransform::SetLeft(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetLeft().SignedAngle(dir));
		SetWorldBasis(mData->mWorldTransform*transf);
	}

	Vec2F ActorTransform::GetLeft() const
	{
		return mData->mWorldNonSizedTransform.xv.Inverted();
	}

	void ActorTransform::SetUp(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetUp().SignedAngle(dir));
		SetWorldBasis(mData->mWorldTransform*transf);
	}

	Vec2F ActorTransform::GetUp() const
	{
		return mData->mWorldNonSizedTransform.yv;
	}

	void ActorTransform::SetDown(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetDown().SignedAngle(dir));
		SetWorldBasis(mData->mWorldTransform*transf);
	}

	Vec2F ActorTransform::GetDown() const
	{
		return mData->mWorldNonSizedTransform.yv.Inverted();
	}

	void ActorTransform::LookAt(const Vec2F& worldPoint)
	{
		SetUp((worldPoint - mData->mPosition).Normalized());
	}

	Vec2F ActorTransform::World2LocalPoint(const Vec2F& worldPoint) const
	{
		Vec2F nx = mData->mWorldTransform.xv, ny = mData->mWorldTransform.yv, offs = mData->mWorldTransform.offs, w = worldPoint;
		float lx = (w.x*ny.y - offs.x*ny.y - w.y*ny.x + offs.y*ny.x) / (nx.x*ny.y - ny.x*nx.y);
		float ly = (w.y - offs.y - nx.y*lx) / ny.y;
		return Vec2F(lx, ly)*mData->mSize;
	}

	Vec2F ActorTransform::Local2WorldPoint(const Vec2F& localPoint) const
	{
		return mData->mWorldTransform*(localPoint / mData->mSize);
	}

	Vec2F ActorTransform::World2LocalDir(const Vec2F& worldDir) const
	{
		Vec2F nx = mData->mWorldTransform.xv / (mData->mSize.x*mData->mScale.x), ny = mData->mWorldTransform.yv / (mData->mSize.y*mData->mScale.y), wd = worldDir;
		float ldy = (wd.x*nx.y - wd.y*nx.x) / (nx.y*ny.x - ny.y*nx.x);
		float ldx = (wd.x - ny.x*ldy) / nx.x;
		return Vec2F(ldx, ldy);
	}

	Vec2F ActorTransform::Local2WorldDir(const Vec2F& localDir) const
	{
		Vec2F nx = mData->mWorldTransform.xv / (mData->mSize.x*mData->mScale.x), ny = mData->mWorldTransform.yv / (mData->mSize.y*mData->mScale.y);
		return nx*localDir.x + ny*localDir.y;
	}

	bool ActorTransform::IsPointInside(const Vec2F& point) const
	{
		Vec2F rs = mData->mScale*mData->mSize;
		Vec2F nx = mData->mWorldTransform.xv / rs.x, ny = mData->mWorldTransform.yv / rs.y;
		Vec2F lp = point - mData->mWorldTransform.offs;

		float dx = lp.Dot(nx);
		float dy = lp.Dot(ny);

		return dx >= 0.0f && dx <= rs.x && dy >= 0.0f && dy < rs.y;
	}

	void ActorTransform::SetOwner(Actor* actor)
	{
		mData->mOwner = actor;
		UpdateTransform();
	}

	void ActorTransform::UpdateTransform()
	{
		mData->mNonSizedTransform = Basis::Build(mData->mPosition, mData->mScale, mData->mAngle, mData->mShear);
		mData->mTransform.Set(mData->mNonSizedTransform.offs, mData->mNonSizedTransform.xv * mData->mSize.x, mData->mNonSizedTransform.yv * mData->mSize.y);
		mData->mTransform.offs = mData->mTransform.offs - mData->mTransform.xv*mData->mPivot.x - mData->mTransform.yv*mData->mPivot.y;
		mData->mNonSizedTransform.offs = mData->mTransform.offs;

		if (mData->mOwner && mData->mOwner->mParent)
		{
			mData->mParentTransform = mData->mOwner->mParent->transform->mData->mWorldNonSizedTransform;
			mData->mWorldTransform = mData->mTransform*mData->mParentTransform;
			mData->mWorldNonSizedTransform = mData->mNonSizedTransform*mData->mParentTransform;
		}
		else
		{
			mData->mWorldNonSizedTransform = mData->mNonSizedTransform;
			mData->mWorldTransform = mData->mTransform;
		}

		mData->mIsParentInvTransformActual = false;

		if (mData->mOwner)
			mData->mOwner->OnTransformChanged();
	}

	void ActorTransform::CheckParentInvTransform()
	{
		if (mData->mIsParentInvTransformActual)
			return;

		mData->mIsParentInvTransformActual = true;

		if (mData->mOwner && mData->mOwner->mParent)
			mData->mParentInvertedTransform = mData->mOwner->mParent->transform->mData->mWorldNonSizedTransform.Inverted();
		else
			mData->mParentInvertedTransform = Basis::Identity();
	}

	void ActorTransform::InitializeProperties()
	{
		INITIALIZE_PROPERTY(ActorTransform, position, SetPosition, GetPosition);
		INITIALIZE_PROPERTY(ActorTransform, size, SetSize, GetSize);
		INITIALIZE_PROPERTY(ActorTransform, scale, SetScale, GetScale);
		INITIALIZE_PROPERTY(ActorTransform, pivot, SetPivot, GetPivot);
		INITIALIZE_PROPERTY(ActorTransform, worldPivot, SetWorldPivot, GetWorldPivot);
		INITIALIZE_PROPERTY(ActorTransform, szPivot, SetSizePivot, GetSizePivot);
		INITIALIZE_PROPERTY(ActorTransform, rect, SetRect, GetRect);
		INITIALIZE_PROPERTY(ActorTransform, angle, SetAngle, GetAngle);
		INITIALIZE_PROPERTY(ActorTransform, angleDegree, SetAngleDegrees, GetAngleDegrees);
		INITIALIZE_PROPERTY(ActorTransform, shear, SetShear, GetShear);
		INITIALIZE_PROPERTY(ActorTransform, basis, SetBasis, GetBasis);
		INITIALIZE_PROPERTY(ActorTransform, nonSizedBasis, SetNonSizedBasis, GetNonSizedBasis);
		INITIALIZE_PROPERTY(ActorTransform, AABB, SetAxisAlignedRect, GetAxisAlignedRect);
		INITIALIZE_PROPERTY(ActorTransform, right, SetRight, GetRight);
		INITIALIZE_PROPERTY(ActorTransform, left, SetLeft, GetLeft);
		INITIALIZE_PROPERTY(ActorTransform, up, SetUp, GetUp);
		INITIALIZE_PROPERTY(ActorTransform, down, SetDown, GetDown);
		INITIALIZE_PROPERTY(ActorTransform, leftTop, SetLeftTop, GetLeftTop);
		INITIALIZE_PROPERTY(ActorTransform, rightTop, SetRightTop, GetRightTop);
		INITIALIZE_PROPERTY(ActorTransform, leftBottom, SetLeftBottom, GetLeftBottom);
		INITIALIZE_PROPERTY(ActorTransform, rightBottom, SetRightBottom, GetRightBottom);
		INITIALIZE_SETTER(ActorTransform, lookAtPoint, LookAt);
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
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(position);
	PUBLIC_FIELD(size);
	PUBLIC_FIELD(scale);
	PUBLIC_FIELD(pivot);
	PUBLIC_FIELD(worldPivot);
	PUBLIC_FIELD(szPivot);
	PUBLIC_FIELD(rect);
	PUBLIC_FIELD(angle);
	PUBLIC_FIELD(angleDegree);
	PUBLIC_FIELD(shear);
	PUBLIC_FIELD(basis);
	PUBLIC_FIELD(nonSizedBasis);
	PUBLIC_FIELD(AABB);
	PUBLIC_FIELD(leftTop);
	PUBLIC_FIELD(leftBottom);
	PUBLIC_FIELD(rightTop);
	PUBLIC_FIELD(rightBottom);
	PUBLIC_FIELD(right);
	PUBLIC_FIELD(left);
	PUBLIC_FIELD(up);
	PUBLIC_FIELD(down);
	PUBLIC_FIELD(lookAtPoint);
	PUBLIC_FIELD(actor);
	PUBLIC_FIELD(worldPosition);
	PUBLIC_FIELD(worldRect);
	PUBLIC_FIELD(worldAngle);
	PUBLIC_FIELD(worldBasis);
	PUBLIC_FIELD(worldNonSizedBasis);
	PUBLIC_FIELD(worldAABB);
	PROTECTED_FIELD(mData).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(void, SetPosition, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPosition);
	PUBLIC_FUNCTION(void, SetSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetSize);
	PUBLIC_FUNCTION(void, SetPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPivot);
	PUBLIC_FUNCTION(void, SetWorldPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldPivot);
	PUBLIC_FUNCTION(void, SetSizePivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetSizePivot);
	PUBLIC_FUNCTION(void, SetRect, const RectF&);
	PUBLIC_FUNCTION(RectF, GetRect);
	PUBLIC_FUNCTION(void, SetScale, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetScale);
	PUBLIC_FUNCTION(void, SetAngle, float);
	PUBLIC_FUNCTION(float, GetAngle);
	PUBLIC_FUNCTION(void, SetAngleDegrees, float);
	PUBLIC_FUNCTION(float, GetAngleDegrees);
	PUBLIC_FUNCTION(void, SetShear, float);
	PUBLIC_FUNCTION(float, GetShear);
	PUBLIC_FUNCTION(void, SetBasis, const Basis&);
	PUBLIC_FUNCTION(Basis, GetBasis);
	PUBLIC_FUNCTION(void, SetNonSizedBasis, const Basis&);
	PUBLIC_FUNCTION(Basis, GetNonSizedBasis);
	PUBLIC_FUNCTION(void, SetAxisAlignedRect, const RectF&);
	PUBLIC_FUNCTION(RectF, GetAxisAlignedRect);
	PUBLIC_FUNCTION(void, SetLeftTop, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetLeftTop);
	PUBLIC_FUNCTION(void, SetRightTop, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetRightTop);
	PUBLIC_FUNCTION(void, SetLeftBottom, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetLeftBottom);
	PUBLIC_FUNCTION(void, SetRightBottom, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetRightBottom);
	PUBLIC_FUNCTION(void, SetCenter, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetCenter);
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
	PUBLIC_FUNCTION(Actor*, GetOwnerActor);
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
	PROTECTED_FUNCTION(void, SetOwner, Actor*);
	PROTECTED_FUNCTION(void, UpdateTransform);
	PROTECTED_FUNCTION(void, CheckParentInvTransform);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(o2::ActorTransform::Data)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(mPosition).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mScale).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mPivot).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mAngle).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mShear).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mTransform);
	PUBLIC_FIELD(mNonSizedTransform);
	PUBLIC_FIELD(mWorldNonSizedTransform);
	PUBLIC_FIELD(mWorldTransform);
	PUBLIC_FIELD(mParentInvertedTransform);
	PUBLIC_FIELD(mParentTransform);
	PUBLIC_FIELD(mIsParentInvTransformActual);
	PUBLIC_FIELD(mOwner);
}
END_META;
