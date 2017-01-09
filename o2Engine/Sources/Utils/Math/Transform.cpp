#include "Transform.h"

namespace o2
{
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
		mPivot(other.mPivot), mShear(other.mShear), mTransform(other.mTransform),
		mNonSizedTransform(other.mNonSizedTransform)
	{
		InitializeProperties();
	}

	void Transform::OnDeserialized(const DataNode& node)
	{
		UpdateTransform();
	}

	void Transform::UpdateTransform()
	{
		mNonSizedTransform = Basis::Build(mPosition, mScale, mAngle, mShear);
		mTransform.Set(mNonSizedTransform.offs, mNonSizedTransform.xv * mSize.x, mNonSizedTransform.yv * mSize.y);
		mTransform.offs = mTransform.offs - mTransform.xv*mPivot.x - mTransform.yv*mPivot.y;
		mNonSizedTransform.offs = mTransform.offs;

		BasisChanged();
	}

	Transform& Transform::operator=(const Transform& other)
	{
		mPosition = other.mPosition;
		mSize = other.mSize;
		mScale = other.mScale;
		mPivot = other.mPivot;
		mAngle = other.mAngle;
		mShear = other.mShear;
		mTransform = other.mTransform;
		mNonSizedTransform = other.mNonSizedTransform;

		UpdateTransform();
		BasisChanged();

		return *this;
	}

	bool Transform::operator==(const Transform& other) const
	{
		return mPosition == other.mPosition &&
			mSize == other.mSize &&
			mScale == other.mScale &&
			mPivot == other.mPivot &&
			mAngle == other.mAngle &&
			mShear == other.mShear &&
			mTransform == other.mTransform &&
			mNonSizedTransform == other.mNonSizedTransform;
	}

	void Transform::SetPosition(const Vec2F& position)
	{
		mPosition = position;
		UpdateTransform();
	}

	Vec2F Transform::GetPosition() const
	{
		return mPosition;
	}

	void Transform::SetSize(const Vec2F& size)
	{
		mSize = size;
		UpdateTransform();
	}

	Vec2F Transform::GetSize() const
	{
		return mSize;
	}

	void Transform::SetPivot(const Vec2F& pivot)
	{
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
		return Local2WorldPoint(mPivot*mSize);
	}

	void Transform::SetSizePivot(const Vec2F& relPivot)
	{
		SetPivot(relPivot / mSize);
	}

	Vec2F Transform::GetSizePivot() const
	{
		return mPivot*mSize;
	}

	void Transform::SetRect(const RectF& rect)
	{
		mSize = rect.Size();
		mPosition = rect.LeftBottom() + Vec2F(mSize.x*mScale.x, mSize.y*mScale.y)*mPivot;

		UpdateTransform();
	}

	RectF Transform::GetRect() const
	{
		return RectF(mTransform.offs, mTransform.offs + mTransform.xv + mTransform.yv);
	}

	void Transform::SetScale(const Vec2F& scale)
	{
		mScale = scale;
		UpdateTransform();
	}

	Vec2F Transform::GetScale() const
	{
		return mScale;
	}

	void Transform::SetAngle(float rad)
	{
		mAngle = rad;
		UpdateTransform();
	}

	float Transform::GetAngle() const
	{
		return mAngle;
	}

	void Transform::SetAngleDegrees(float deg)
	{
		mAngle = Math::Deg2rad(deg);
		UpdateTransform();
	}

	float Transform::GetAngleDegrees() const
	{
		return Math::Rad2deg(mAngle);
	}

	void Transform::SetShear(float shear)
	{
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
		mSize = scale / mScale;
		mShear = shear;

		mPosition = basis.offs + basis.xv*mPivot.x + basis.yv*mPivot.y;
		UpdateTransform();
	}

	Basis Transform::GetBasis() const
	{
		return mTransform;
	}

	void Transform::SetNonSizedBasis(const Basis& basis)
	{
		Vec2F offset, scale;
		float angle, shear;
		basis.Decompose(&offset, &angle, &scale, &shear);

		mAngle = angle;
		mScale = scale;
		mShear = shear;

		mPosition = basis.offs + basis.xv*mPivot.x*mSize.x + basis.yv*mPivot.y*mSize.y;
		UpdateTransform();
	}

	Basis Transform::GetNonSizedBasis() const
	{
		return mNonSizedTransform;
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
		Basis transformed = mTransform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mTransform.xv);
		Vec2F frameDeltaY = delta.Project(mTransform.yv);

		transformed.offs += frameDeltaX;
		transformed.xv -= frameDeltaX;
		transformed.yv += frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F Transform::GetLeftTop() const
	{
		return mTransform.offs + mTransform.yv;
	}

	void Transform::SetRightTop(const Vec2F& position)
	{
		Basis transformed = mTransform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mTransform.xv);
		Vec2F frameDeltaY = delta.Project(mTransform.yv);

		transformed.xv += frameDeltaX;
		transformed.yv += frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F Transform::GetRightTop() const
	{
		return mTransform.offs + mTransform.yv + mTransform.xv;
	}

	void Transform::SetLeftBottom(const Vec2F& position)
	{
		Basis transformed = mTransform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mTransform.xv);
		Vec2F frameDeltaY = delta.Project(mTransform.yv);

		transformed.offs += frameDeltaX + frameDeltaY;
		transformed.xv -= frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F Transform::GetLeftBottom() const
	{
		return mTransform.offs;
	}

	void Transform::SetRightBottom(const Vec2F& position)
	{
		Basis transformed = mTransform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mTransform.xv);
		Vec2F frameDeltaY = delta.Project(mTransform.yv);

		transformed.offs += frameDeltaY;
		transformed.xv += frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F Transform::GetRightBottom() const
	{
		return mTransform.offs + mTransform.xv;
	}

	void Transform::SetCenter(const Vec2F& position)
	{
		Vec2F translate = position - GetCenter();
		SetBasis(mTransform*Basis::Translated(translate));
	}

	Vec2F Transform::GetCenter() const
	{
		return mTransform.offs + (mTransform.xv + mTransform.yv)*0.5f;
	}

	void Transform::SetRight(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetRight().SignedAngle(dir));
		SetBasis(mTransform*transf);
	}

	Vec2F Transform::GetRight() const
	{
		return mNonSizedTransform.xv / (mSize.x*mScale.x);
	}

	void Transform::SetLeft(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetLeft().SignedAngle(dir));
		SetBasis(mTransform*transf);
	}

	Vec2F Transform::GetLeft() const
	{
		return mNonSizedTransform.xv / (-(mSize.x*mScale.x));
	}

	void Transform::SetUp(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetUp().SignedAngle(dir));
		SetBasis(mTransform*transf);
	}

	Vec2F Transform::GetUp() const
	{
		return mNonSizedTransform.yv / (mSize.y*mScale.y);
	}

	void Transform::SetDown(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetDown().SignedAngle(dir));
		SetBasis(mTransform*transf);
	}

	Vec2F Transform::GetDown() const
	{
		return mNonSizedTransform.yv / (-(mSize.y*mScale.y));
	}

	void Transform::LookAt(const Vec2F& worldPoint)
	{
		SetUp((worldPoint - mPosition).Normalized());
	}

	Vec2F Transform::World2LocalPoint(const Vec2F& worldPoint) const
	{
		Vec2F nx = mTransform.xv, ny = mTransform.yv, offs = mTransform.offs, w = worldPoint;
		float lx = (w.x*ny.y - offs.x*ny.y - w.y*ny.x + offs.y*ny.x) / (nx.x*ny.y - ny.x*nx.y);
		float ly = (w.y - offs.y - nx.y*lx) / ny.y;
		return Vec2F(lx, ly)*mSize;
	}

	Vec2F Transform::Local2WorldPoint(const Vec2F& localPoint) const
	{
		return mTransform*(localPoint / mSize);
	}

	Vec2F Transform::World2LocalDir(const Vec2F& worldDir) const
	{
		Vec2F nx = mTransform.xv / (mSize.x*mScale.x), ny = mTransform.yv / (mSize.y*mScale.y), wd = worldDir;
		float ldy = (wd.x*nx.y - wd.y*nx.x) / (nx.y*ny.x - ny.y*nx.x);
		float ldx = (wd.x - ny.x*ldy) / nx.x;
		return Vec2F(ldx, ldy);
	}

	Vec2F Transform::Local2WorldDir(const Vec2F& localDir) const
	{
		Vec2F nx = mTransform.xv / (mSize.x*mScale.x), ny = mTransform.yv / (mSize.y*mScale.y);
		return nx*localDir.x + ny*localDir.y;
	}

	bool Transform::IsPointInside(const Vec2F& point) const
	{
		Vec2F rs = mScale*mSize;
		Vec2F nx = mTransform.xv / rs.x, ny = mTransform.yv / rs.y;
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
		INITIALIZE_PROPERTY(Transform, angleDegree, SetAngleDegrees, GetAngleDegrees);
		INITIALIZE_PROPERTY(Transform, shear, SetShear, GetShear);
		INITIALIZE_PROPERTY(Transform, basis, SetBasis, GetBasis);
		INITIALIZE_PROPERTY(Transform, nonSizedBasis, SetNonSizedBasis, GetNonSizedBasis);
		INITIALIZE_PROPERTY(Transform, AABB, SetAxisAlignedRect, GetAxisAlignedRect);
		INITIALIZE_PROPERTY(Transform, right, SetRight, GetRight);
		INITIALIZE_PROPERTY(Transform, left, SetLeft, GetLeft);
		INITIALIZE_PROPERTY(Transform, up, SetUp, GetUp);
		INITIALIZE_PROPERTY(Transform, down, SetDown, GetDown);
		INITIALIZE_SETTER(Transform, lookAtPoint, LookAt);
	}
}
 
CLASS_META(o2::Transform)
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
	PROTECTED_FIELD(mPosition).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSize).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mScale).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mPivot).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAngle).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mShear).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTransform);
	PROTECTED_FIELD(mNonSizedTransform);

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
	PROTECTED_FUNCTION(void, BasisChanged);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, UpdateTransform);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
 