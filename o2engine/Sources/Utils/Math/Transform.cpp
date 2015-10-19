#include "Transform.h"

namespace o2
{
	IOBJECT_CPP(Transform);

	Transform::Transform(const Vec2F& size /*= Vec2F()*/, const Vec2F& position /*= Vec2F()*/,
						 float angle /*= 0.0f*/, const Vec2F& scale /*= Vec2F(1.0f, 1.0f)*/,
						 const Vec2F& pivot /*= Vec2F(0.5f, 0.5f)*/):
		mSize(size), mPosition(position), mAngle(angle), mScale(scale), mPivot(pivot), mShear(0)
	{
		UpdateTransform();
		InitializeProperties();
	}

	Transform::Transform(const Transform& other):
		mSize(other.mSize), mPosition(other.mPosition), mAngle(other.mAngle), mScale(other.mScale),
		mPivot(other.mPivot), mShear(other.mShear), mTransform(other.mTransform)
	{
		InitializeProperties();
	}

	void Transform::OnDeserialized(const DataNode& node)
	{
		UpdateTransform();
	}

	void Transform::UpdateTransform()
	{
		mTransform = Basis::Build(mPosition, mScale*mSize, mAngle, mShear);
		mTransform.offs = mTransform.offs - mTransform.xv*mPivot.x - mTransform.yv*mPivot.y;

		BasisChanged();
	}

	Transform& Transform::operator=(const Transform& other)
	{
		mPosition  = other.mPosition;
		mSize	   = other.mSize;
		mScale	   = other.mScale;
		mPivot	   = other.mPivot;
		mAngle	   = other.mAngle;
		mShear	   = other.mShear;
		mTransform = other.mTransform;

		UpdateTransform();
		BasisChanged();

		return *this;
	}

	void Transform::SetPosition(const Vec2F& position)
	{
		if (mPosition == position)
			return;

		mPosition = position;
		UpdateTransform();
	}

	Vec2F Transform::GetPosition() const
	{
		return mPosition;
	}

	void Transform::SetSize(const Vec2F& size)
	{
		if (mSize == size)
			return;

		mSize = size;
		UpdateTransform();
	}

	Vec2F Transform::GetSize() const
	{
		return mSize;
	}

	void Transform::SetPivot(const Vec2F& pivot)
	{
		if (mPivot == pivot)
			return;

		mPivot = pivot;
		UpdateTransform();
	}

	Vec2F Transform::GetPivot() const
	{
		return mPivot;
	}

	void Transform::SetWorldPivot(const Vec2F& pivot)
	{
		SetSizePivot(World2LocalPoint(pivot));
	}

	Vec2F Transform::GetWorldPivot() const
	{
		return Local2WorldPoint(mPivot);
	}

	void Transform::SetSizePivot(const Vec2F& relPivot)
	{
		SetPivot(relPivot/mSize);
	}

	Vec2F Transform::GetSizePivot() const
	{
		return mPivot*mSize;
	}

	void Transform::SetRect(const RectF& rect)
	{
		Vec2F x = mTransform.xv/(mSize.x*mScale.x);
		Vec2F y = mTransform.yv/(mSize.y*mScale.y);

		mSize = rect.Size()/mScale;
		Vec2F xv = x*mSize.x*mScale.x;
		Vec2F yv = y*mSize.y*mScale.y;

		mPosition = rect.LeftBottom() + xv*mPivot.x + yv*mPivot.y;

		UpdateTransform();
	}

	RectF Transform::GetRect() const
	{
		return RectF(mTransform.offs, mTransform.offs + mTransform.xv + mTransform.yv);
	}

	void Transform::SetScale(const Vec2F& scale)
	{
		if (mScale == scale)
			return;

		mScale = scale;
		UpdateTransform();
	}

	Vec2F Transform::GetScale() const
	{
		return mScale;
	}

	void Transform::SetAngle(float rad)
	{
		if (Math::Equals(mAngle, rad))
			return;

		mAngle = rad;
		UpdateTransform();
	}

	float Transform::GetAngle() const
	{
		return mAngle;
	}

	void Transform::SetShear(float shear)
	{
		if (Math::Equals(mShear, shear))
			return;

		mShear = shear;
		UpdateTransform();
	}

	float Transform::GetShear() const
	{
		return mShear;
	}

	void Transform::SetBasis(const Basis& basis)
	{
		Vec2F offset, scale;
		float angle, shear;
		basis.Decompose(&offset, &angle, &scale, &shear);

		mAngle = angle;
		mSize = scale/mScale;
		mShear = shear;

		mPosition = basis.offs + basis.xv*mPivot.x + basis.yv*mPivot.y;
		UpdateTransform();
	}

	Basis Transform::GetBasis() const
	{
		return mTransform;
	}

	void Transform::SetAxisAlignedRect(const RectF& rect)
	{
		RectF curRect = GetAxisAlignedRect();

		Basis curRectBasis(curRect.LeftBottom(), Vec2F::Right()*curRect.Width(), Vec2F::Up()*curRect.Height());
		Basis rectBasis(rect.LeftBottom(), Vec2F::Right()*rect.Width(), Vec2F::Up()*rect.Height());

		SetBasis(mTransform*curRectBasis.Inverted()*rectBasis);
	}

	RectF Transform::GetAxisAlignedRect() const
	{
		return mTransform.AABB();
	}

	void Transform::SetLeftTop(const Vec2F& position)
	{
	}

	Vec2F Transform::GetLeftTop() const
	{
		return mTransform.offs + mTransform.yv;
	}

	void Transform::SetRightTop(const Vec2F& position)
	{

	}

	Vec2F Transform::GetRightTop() const
	{
		return mTransform.offs + mTransform.yv + mTransform.xv;
	}

	void Transform::SetLeftBottom(const Vec2F& position)
	{

	}

	Vec2F Transform::GetLeftBottom() const
	{
		return mTransform.offs;
	}

	void Transform::SetRightBottom(const Vec2F& position)
	{
	}

	Vec2F Transform::GetRightBottom() const
	{
		return mTransform.offs + mTransform.xv;
	}

	void Transform::SetCenter(const Vec2F& position)
	{
	}

	Vec2F Transform::GetCenter() const
	{
		return mTransform.offs + (mTransform.xv + mTransform.yv)*0.5f;
	}

	void Transform::SetRight(const Vec2F& dir)
	{

	}

	Vec2F Transform::GetRight() const
	{
		return mTransform.xv/(mSize.x*mScale.x);
	}

	void Transform::SetLeft(const Vec2F& dir)
	{

	}

	Vec2F Transform::GetLeft() const
	{
		return mTransform.xv/(-(mSize.x*mScale.x));
	}

	void Transform::SetUp(const Vec2F& dir)
	{
	}

	Vec2F Transform::GetUp() const
	{
		return mTransform.yv/(mSize.y*mScale.y);
	}

	void Transform::SetDown(const Vec2F& dir)
	{
	}

	Vec2F Transform::GetDown() const
	{
		return mTransform.yv/(-(mSize.y*mScale.y));
	}

	void Transform::LookAt(const Vec2F& worldPoint)
	{
		SetUp((worldPoint - mPosition).Normalized());
	}

	Vec2F Transform::World2LocalPoint(const Vec2F& worldPoint) const
	{
		Vec2F nx = mTransform.xv, ny = mTransform.yv, offs = mTransform.offs, w = worldPoint;
		float lx = (w.x*ny.y - offs.x*ny.y - w.y*ny.x + offs.y*ny.x)/(nx.x*ny.y - ny.x*nx.y);
		float ly = (w.y - offs.y - nx.y*lx)/ny.y;
		return Vec2F(lx, ly)*mSize;
	}

	Vec2F Transform::Local2WorldPoint(const Vec2F& localPoint) const
	{
		return mTransform*(localPoint/mSize);
	}

	Vec2F Transform::World2LocalDir(const Vec2F& worldDir) const
	{
		Vec2F nx = mTransform.xv/(mSize.x*mScale.x), ny = mTransform.yv/(mSize.y*mScale.y), wd = worldDir;
		float ldy = (wd.x*nx.y - wd.y*nx.x)/(nx.y*ny.x - ny.y*nx.x);
		float ldx = (wd.x - ny.x*ldy)/nx.x;
		return Vec2F(ldx, ldy);
	}

	Vec2F Transform::Local2WorldDir(const Vec2F& localDir) const
	{
		Vec2F nx = mTransform.xv/(mSize.x*mScale.x), ny = mTransform.yv/(mSize.y*mScale.y);
		return nx*localDir.x + ny*localDir.y;
	}

	bool Transform::IsPointInside(const Vec2F& point) const
	{
		Vec2F rs = mScale*mSize;
		Vec2F nx = mTransform.xv/rs.x, ny = mTransform.yv/rs.y;
		Vec2F lp = point - mTransform.offs;

		float dx = lp.Dot(nx);
		float dy = lp.Dot(ny);

		return dx >= 0.0f && dx <= rs.x && dy >= 0.0f && dy < rs.y;
	}

	void Transform::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Transform, position, SetPosition, GetPosition);
		INITIALIZE_PROPERTY(Transform, size, SetSize, GetSize);
		INITIALIZE_PROPERTY(Transform, scale, SetScale, GetScale);
		INITIALIZE_PROPERTY(Transform, pivot, SetPivot, GetPivot);
		INITIALIZE_PROPERTY(Transform, worldPivot, SetWorldPivot, GetWorldPivot);
		INITIALIZE_PROPERTY(Transform, szPivot, SetSizePivot, GetSizePivot);
		INITIALIZE_PROPERTY(Transform, rect, SetRect, GetRect);
		INITIALIZE_PROPERTY(Transform, angle, SetAngle, GetAngle);
		INITIALIZE_PROPERTY(Transform, shear, SetShear, GetShear);
		INITIALIZE_PROPERTY(Transform, resBasis, SetBasis, GetBasis);
		INITIALIZE_PROPERTY(Transform, AABB, SetAxisAlignedRect, GetAxisAlignedRect);
		INITIALIZE_PROPERTY(Transform, right, SetRight, GetRight);
		INITIALIZE_PROPERTY(Transform, left, SetLeft, GetLeft);
		INITIALIZE_PROPERTY(Transform, up, SetUp, GetUp);
		INITIALIZE_PROPERTY(Transform, down, SetDown, GetDown);
		INITIALIZE_SETTER(Transform, lookAtPoint, LookAt);
	}
}