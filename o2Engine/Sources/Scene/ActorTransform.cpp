#include "ActorTransform.h"

#include "Actor.h"

namespace o2
{
	ActorTransform::ActorTransform(Data* data):
		mData(data)
	{}

	ActorTransform::ActorTransform(const Vec2F& size /*= Vec2F()*/, const Vec2F& position /*= Vec2F()*/,
								   float angle /*= 0.0f*/, const Vec2F& scale /*= Vec2F(1.0f, 1.0f)*/,
								   const Vec2F& pivot /*= Vec2F(0.5f, 0.5f)*/) :
		ActorTransform(mnew Data())
	{
		mData->size = size;
		mData->position = position;
		mData->angle = angle;
		mData->scale = scale;
		mData->pivot = pivot;

		InitializeProperties();
	}

	ActorTransform::ActorTransform(const ActorTransform& other):
		ActorTransform(mnew Data())
	{
		mData->size = other.mData->size;
		mData->position = other.mData->position;
		mData->angle = other.mData->angle;
		mData->scale = other.mData->scale;
		mData->pivot = other.mData->pivot;
		mData->shear = other.mData->shear;

		InitializeProperties();
	}

	ActorTransform& ActorTransform::operator=(const ActorTransform& other)
	{
		mData->size = other.mData->size;
		mData->position = other.mData->position;
		mData->angle = other.mData->angle;
		mData->scale = other.mData->scale;
		mData->pivot = other.mData->pivot;
		mData->shear = other.mData->shear;

		SetDirty();

		return *this;
	}

	ActorTransform& ActorTransform::operator=(const Transform& other)
	{
		SetWorldBasis(other.GetBasis());
		return *this;
	}

	bool ActorTransform::operator==(const ActorTransform& other) const
	{
		return Math::Equals(mData->size, other.mData->size) &&
			Math::Equals(mData->position, other.mData->position) &&
			Math::Equals(mData->angle, other.mData->angle) &&
			Math::Equals(mData->scale, other.mData->scale) &&
			Math::Equals(mData->pivot, other.mData->pivot) &&
			Math::Equals(mData->shear, other.mData->shear);
	}

	void ActorTransform::OnDeserialized(const DataNode& node)
	{
		SetDirty();
	}

	void ActorTransform::SetPosition(const Vec2F& position)
	{
		mData->position = position;
		SetDirty();
	}

	Vec2F ActorTransform::GetPosition() const
	{
		return mData->position;
	}

	void ActorTransform::SetSize(const Vec2F& size)
	{
		mData->size = size;
		SetDirty();
	}

	Vec2F ActorTransform::GetSize() const
	{
		return mData->size;
	}

	void ActorTransform::SetWidth(float value)
	{
		mData->size.x = value;
		SetDirty();
	}

	float ActorTransform::GetWidht() const
	{
		return mData->size.x;
	}

	void ActorTransform::SetHeight(float value)
	{
		mData->size.y = value;
		SetDirty();
	}

	float ActorTransform::GetHeight() const
	{
		return mData->size.y;
	}

	void ActorTransform::SetPivot(const Vec2F& pivot)
	{
		mData->pivot = pivot;
		SetDirty();
	}

	Vec2F ActorTransform::GetPivot() const
	{
		return mData->pivot;
	}

	void ActorTransform::SetSizePivot(const Vec2F& relPivot)
	{
		SetPivot(relPivot / mData->size);
	}

	Vec2F ActorTransform::GetSizePivot() const
	{
		return mData->pivot*mData->size;
	}

	void ActorTransform::SetRect(const RectF& rect)
	{
		mData->size = rect.Size();
		mData->position = rect.LeftBottom() + mData->size*mData->pivot;

		SetDirty();
	}

	RectF ActorTransform::GetRect() const
	{
		Vec2F leftBottom = mData->position - mData->size*mData->pivot;
		return RectF(leftBottom, leftBottom + mData->size);
	}

	void ActorTransform::SetScale(const Vec2F& scale)
	{
		mData->scale = scale;
		SetDirty();
	}

	Vec2F ActorTransform::GetScale() const
	{
		return mData->scale;
	}

	void ActorTransform::SetAngle(float rad)
	{
		mData->angle = rad;
		SetDirty();
	}

	float ActorTransform::GetAngle() const
	{
		return mData->angle;
	}

	void ActorTransform::SetAngleDegrees(float deg)
	{
		mData->angle = Math::Deg2rad(deg);
		SetDirty();
	}

	float ActorTransform::GetAngleDegrees() const
	{
		return Math::Rad2deg(mData->angle);
	}

	void ActorTransform::SetShear(float shear)
	{
		mData->shear = shear;
		SetDirty();
	}

	float ActorTransform::GetShear() const
	{
		return mData->shear;
	}

	void ActorTransform::SetBasis(const Basis& basis)
	{
		Vec2F offset, scale;
		float angle, shear;
		basis.Decompose(&offset, &angle, &scale, &shear);

		mData->angle = angle;
		mData->size = scale / mData->scale;
		mData->shear = shear;

		mData->position = basis.offs + basis.xv*mData->pivot.x + basis.yv*mData->pivot.y;
		SetDirty();
	}

	Basis ActorTransform::GetBasis() const
	{
		return mData->transform;
	}

	void ActorTransform::SetNonSizedBasis(const Basis& basis)
	{
		Vec2F offset, scale;
		float angle, shear;
		basis.Decompose(&offset, &angle, &scale, &shear);

		mData->angle = angle;
		mData->scale = scale;
		mData->shear = shear;

		mData->position = basis.offs + basis.xv*mData->pivot.x*mData->size.x + basis.yv*mData->pivot.y*mData->size.y;
		SetDirty();
	}

	Basis ActorTransform::GetNonSizedBasis() const
	{
		return mData->nonSizedTransform;
	}

	void ActorTransform::SetAxisAlignedRect(const RectF& rect)
	{
		RectF curRect = GetAxisAlignedRect();
		Basis curRectBasis(curRect.LeftBottom(), Vec2F::Right()*curRect.Width(), Vec2F::Up()*curRect.Height());
		Basis rectBasis(rect.LeftBottom(), Vec2F::Right()*rect.Width(), Vec2F::Up()*rect.Height());

		SetBasis(mData->transform*curRectBasis.Inverted()*rectBasis);
	}

	RectF ActorTransform::GetAxisAlignedRect() const
	{
		return mData->transform.AABB();
	}

	void ActorTransform::SetLeftTop(const Vec2F& position)
	{
		mData->size.Set(GetRight() - position.x, position.y - GetBottom());
		mData->position.Set(position.x + mData->size.x*mData->pivot.x,
							position.y - mData->size.y + mData->size.y*mData->pivot.y);

		SetDirty();
	}

	Vec2F ActorTransform::GetLeftTop() const
	{
		return Vec2F(GetLeft(), GetTop());
	}

	void ActorTransform::SetRightTop(const Vec2F& position)
	{
		mData->size.Set(position.x - GetLeft(), position.y - GetBottom());
		mData->position.Set(position.x - mData->size.x + mData->size.x*mData->pivot.x,
							position.y - mData->size.y + mData->size.y*mData->pivot.y);

		SetDirty();
	}

	Vec2F ActorTransform::GetRightTop() const
	{
		return Vec2F(GetRight(), GetTop());
	}

	void ActorTransform::SetLeftBottom(const Vec2F& position)
	{
		mData->size.Set(GetRight() - position.x, GetTop() - position.y);
		mData->position.Set(position.x + mData->size.x*mData->pivot.x,
							position.y + mData->size.y*mData->pivot.y);

		SetDirty();
	}

	Vec2F ActorTransform::GetLeftBottom() const
	{
		return Vec2F(GetLeft(), GetBottom());
	}

	void ActorTransform::SetRightBottom(const Vec2F& position)
	{
		mData->size.Set(position.x - GetLeft(), GetTop() - position.y);
		mData->position.Set(position.x - mData->size.x + mData->size.x*mData->pivot.x,
							position.y + mData->size.y*mData->pivot.y);

		SetDirty();
	}

	Vec2F ActorTransform::GetRightBottom() const
	{
		return Vec2F(GetRight(), GetBottom());
	}

	void ActorTransform::SetCenter(const Vec2F& position)
	{
		Vec2F translate = position - GetCenter();
		SetBasis(mData->transform*Basis::Translated(translate));
	}

	Vec2F ActorTransform::GetCenter() const
	{
		return mData->transform.offs + (mData->transform.xv + mData->transform.yv)*0.5f;
	}

	void ActorTransform::SetRightDir(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetRightDir().SignedAngle(dir));
		SetWorldBasis(mData->transform*transf);
	}

	Vec2F ActorTransform::GetRightDir() const
	{
		return mData->nonSizedTransform.xv;
	}

	void ActorTransform::SetLeftDir(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetLeftDir().SignedAngle(dir));
		SetWorldBasis(mData->transform*transf);
	}

	Vec2F ActorTransform::GetLeftDir() const
	{
		return mData->nonSizedTransform.xv.Inverted();
	}

	void ActorTransform::SetUpDir(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetUpDir().SignedAngle(dir));
		SetWorldBasis(mData->transform*transf);
	}

	Vec2F ActorTransform::GetUpDir() const
	{
		return mData->nonSizedTransform.yv;
	}

	void ActorTransform::SetDownDir(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetDownDir().SignedAngle(dir));
		SetWorldBasis(mData->transform*transf);
	}

	Vec2F ActorTransform::GetDownDir() const
	{
		return mData->nonSizedTransform.yv.Inverted();
	}

	void ActorTransform::SetRight(float value)
	{
		mData->size.x = value - GetLeft();
		mData->position.x = value - mData->size.x + mData->size.x*mData->pivot.x;

		SetDirty();
	}

	float ActorTransform::GetRight() const
	{
		return mData->position.x - mData->size.x*(mData->pivot.x - 1.0f);
	}

	void ActorTransform::SetLeft(float value)
	{
		mData->size.x = GetRight() - value;
		mData->position.x = value + mData->size.x*mData->pivot.x;

		SetDirty();
	}

	float ActorTransform::GetLeft() const
	{
		return mData->position.x - mData->size.x*mData->pivot.x;
	}

	void ActorTransform::SetTop(float value)
	{
		mData->size.y = value - GetBottom();
		mData->position.y = value - mData->size.y + mData->size.y*mData->pivot.y;

		SetDirty();
	}

	float ActorTransform::GetTop() const
	{
		return mData->position.y - mData->size.y*(mData->pivot.y - 1.0f);
	}

	void ActorTransform::SetBottom(float value)
	{
		mData->size.y = GetTop() - value;
		mData->position.y = value + mData->size.y*mData->pivot.y;

		SetDirty();
	}

	float ActorTransform::GetBottom() const
	{
		return mData->position.y - mData->size.y*mData->pivot.y;
	}

	Actor* ActorTransform::GetOwnerActor() const
	{
		return mData->owner;
	}

	bool ActorTransform::IsDirty() const
	{
		return mData->isDirty;
	}

	void ActorTransform::SetWorldPivot(const Vec2F& pivot)
	{
		Basis trasform = mData->worldTransform;
		SetSizePivot(World2LocalPoint(pivot));
		SetWorldBasis(trasform);
	}

	Vec2F ActorTransform::GetWorldPivot() const
	{
		return Local2WorldPoint(mData->pivot*mData->size);
	}

	void ActorTransform::SetWorldPosition(const Vec2F& position)
	{
		CheckParentInvTransform();
		SetPosition(position*mData->parentInvertedTransform);
	}

	Vec2F ActorTransform::GetWorldPosition() const
	{
		return mData->position*mData->parentTransform;
	}

	void ActorTransform::SetWorldRect(const RectF& rect)
	{
		CheckParentInvTransform();
		SetRect(RectF(rect.LeftBottom()*mData->parentInvertedTransform, rect.RightTop()*mData->parentInvertedTransform));
	}

	RectF ActorTransform::GetWorldRect() const
	{
		RectF localRect = GetRect();
		RectF worldRect(localRect.LeftBottom()*mData->parentTransform, localRect.RightTop()*mData->parentTransform);
		return worldRect;
	}

	void ActorTransform::SetWorldAngle(float rad)
	{
		SetAngle(rad - mData->parentTransform.GetAngle());
	}

	float ActorTransform::GetWorldAngle() const
	{
		return mData->worldTransform.GetAngle();
	}

	void ActorTransform::SetWorldAngleDegree(float deg)
	{
		SetWorldAngle(Math::Deg2rad(deg));
	}

	float ActorTransform::GetWorldAngleDegree() const
	{
		return Math::Rad2deg(GetWorldAngle());
	}

	void ActorTransform::SetWorldBasis(const Basis& basis)
	{
		CheckParentInvTransform();
		SetBasis(basis*mData->parentInvertedTransform);
	}

	Basis ActorTransform::GetWorldBasis() const
	{
		return mData->worldTransform;
	}

	void ActorTransform::SetWorldNonSizedBasis(const Basis& basis)
	{
		CheckParentInvTransform();
		SetNonSizedBasis(basis*mData->parentInvertedTransform);
	}

	Basis ActorTransform::GetWorldNonSizedBasis() const
	{
		return mData->worldNonSizedTransform;
	}

	void ActorTransform::SetWorldAxisAlignedRect(const RectF& rect)
	{
		CheckParentInvTransform();
		SetAxisAlignedRect(RectF(rect.LeftBottom()*mData->parentInvertedTransform, rect.RightTop()*mData->parentInvertedTransform));
	}

	RectF ActorTransform::GetWorldAxisAlignedRect() const
	{
		RectF localAARect = GetRect();
		RectF worldAARect(localAARect.LeftBottom()*mData->parentTransform, localAARect.RightTop()*mData->parentTransform);
		return worldAARect;
	}

	void ActorTransform::SetWorldLeftTop(const Vec2F& position)
	{
		SetLeftTop(position - mData->parentRectangePosition);
	}

	Vec2F ActorTransform::GetWorldLeftTop() const
	{
		return Vec2F(GetWorldLeft(), GetWorldTop());
	}

	void ActorTransform::SetWorldRightTop(const Vec2F& position)
	{
		SetRightTop(position - mData->parentRectangePosition);
	}

	Vec2F ActorTransform::GetWorldRightTop() const
	{
		return Vec2F(GetWorldRight(), GetWorldTop());
	}

	void ActorTransform::SetWorldLeftBottom(const Vec2F& position)
	{
		SetLeftTop(position - mData->parentRectangePosition);
	}

	Vec2F ActorTransform::GetWorldLeftBottom() const
	{
		return Vec2F(GetWorldLeft(), GetWorldBottom());
	}

	void ActorTransform::SetWorldRightBottom(const Vec2F& position)
	{
		SetRightBottom(position - mData->parentRectangePosition);
	}

	Vec2F ActorTransform::GetWorldRightBottom() const
	{
		return Vec2F(GetWorldRight(), GetWorldBottom());
	}

	void ActorTransform::SetWorldCenter(const Vec2F& position)
	{
		Vec2F translate = position - GetWorldCenter();
		SetWorldBasis(mData->worldTransform*Basis::Translated(translate));
	}

	Vec2F ActorTransform::GetWorldCenter() const
	{
		return mData->worldTransform.offs + (mData->worldTransform.xv + mData->worldTransform.yv)*0.5f;
	}

	void ActorTransform::SetWorldRightDir(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetRightDir().SignedAngle(dir));
		SetWorldBasis(mData->worldTransform*transf);
	}

	Vec2F ActorTransform::GetWorldRightDir() const
	{
		return mData->worldNonSizedTransform.xv;
	}

	void ActorTransform::SetWorldLeftDir(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetLeftDir().SignedAngle(dir));
		SetWorldBasis(mData->worldTransform*transf);
	}

	Vec2F ActorTransform::GetWorldLeftDir() const
	{
		return mData->worldNonSizedTransform.xv.Inverted();
	}

	void ActorTransform::SetWorldUpDir(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetUpDir().SignedAngle(dir));
		SetWorldBasis(mData->worldTransform*transf);
	}

	Vec2F ActorTransform::GetWorldUpDir() const
	{
		return mData->worldNonSizedTransform.yv;
	}

	void ActorTransform::SetWorldDownDir(const Vec2F& dir)
	{
		Basis transf = Basis::Rotated(GetDownDir().SignedAngle(dir));
		SetWorldBasis(mData->worldTransform*transf);
	}

	Vec2F ActorTransform::GetWorldDownDir() const
	{
		return mData->worldNonSizedTransform.yv.Inverted();
	}

	void ActorTransform::SetWorldRight(float value)
	{
		SetRight(value - mData->parentRectangePosition.x);
	}

	float ActorTransform::GetWorldRight() const
	{
		return GetRight() + mData->parentRectangePosition.x;
	}

	void ActorTransform::SetWorldLeft(float value)
	{
		SetLeft(value - mData->parentRectangePosition.x);
	}

	float ActorTransform::GetWorldLeft() const
	{
		return GetLeft() + mData->parentRectangePosition.x;
	}

	void ActorTransform::SetWorldTop(float value)
	{
		SetTop(value - mData->parentRectangePosition.y);
	}

	float ActorTransform::GetWorldTop() const
	{
		return GetTop() + mData->parentRectangePosition.y;
	}

	void ActorTransform::SetWorldBottom(float value)
	{
		SetBottom(value - mData->parentRectangePosition.y);
	}

	float ActorTransform::GetWorldBottom() const
	{
		return GetBottom() + mData->parentRectangePosition.y;
	}

	Vec2F ActorTransform::World2LocalPoint(const Vec2F& worldPoint) const
	{
		Vec2F nx = mData->worldTransform.xv, ny = mData->worldTransform.yv, offs = mData->worldTransform.offs, w = worldPoint;
		float lx = (w.x*ny.y - offs.x*ny.y - w.y*ny.x + offs.y*ny.x) / (nx.x*ny.y - ny.x*nx.y);
		float ly = (w.y - offs.y - nx.y*lx) / ny.y;
		return Vec2F(lx, ly)*mData->size;
	}

	Vec2F ActorTransform::Local2WorldPoint(const Vec2F& localPoint) const
	{
		return mData->worldTransform*(localPoint / mData->size);
	}

	Vec2F ActorTransform::World2LocalDir(const Vec2F& worldDir) const
	{
		Vec2F nx = mData->worldTransform.xv / (mData->size.x*mData->scale.x), ny = mData->worldTransform.yv / (mData->size.y*mData->scale.y), wd = worldDir;
		float ldy = (wd.x*nx.y - wd.y*nx.x) / (nx.y*ny.x - ny.y*nx.x);
		float ldx = (wd.x - ny.x*ldy) / nx.x;
		return Vec2F(ldx, ldy);
	}

	Vec2F ActorTransform::Local2WorldDir(const Vec2F& localDir) const
	{
		Vec2F nx = mData->worldTransform.xv / (mData->size.x*mData->scale.x), ny = mData->worldTransform.yv / (mData->size.y*mData->scale.y);
		return nx*localDir.x + ny*localDir.y;
	}

	bool ActorTransform::IsPointInside(const Vec2F& point) const
	{
		Vec2F rs = mData->scale*mData->size;
		Vec2F nx = mData->worldTransform.xv / rs.x, ny = mData->worldTransform.yv / rs.y;
		Vec2F lp = point - mData->worldTransform.offs;

		float dx = lp.Dot(nx);
		float dy = lp.Dot(ny);

		return dx >= 0.0f && dx <= rs.x && dy >= 0.0f && dy < rs.y;
	}

	void ActorTransform::SetOwner(Actor* actor)
	{
		mData->owner = actor;
		SetDirty();
	}

	void ActorTransform::SetDirty()
	{
		mData->isDirty = true;

		if (mData->owner)
			mData->owner->OnChanged();
	}

	void ActorTransform::Update()
	{
		UpdateRectangle();
		UpdateTransform();
		UpdateWorldRectangleAndTransform();

		mData->isParentInvTransformActual = false;
		mData->isDirty = false;
		mData->owner->OnTransformUpdated();
	}

	void ActorTransform::UpdateRectangle()
	{
		Vec2F leftBottom = mData->position - mData->size*mData->pivot;
		Vec2F rightTop = leftBottom + mData->size;
		mData->rectangle.left = leftBottom.x;
		mData->rectangle.right = rightTop.x;
		mData->rectangle.bottom = leftBottom.y;
		mData->rectangle.top = rightTop.y;
	}

	void ActorTransform::UpdateTransform()
	{
		mData->nonSizedTransform = Basis::Build(mData->position, mData->scale, mData->angle, mData->shear);
		mData->transform.Set(mData->nonSizedTransform.offs, mData->nonSizedTransform.xv * mData->size.x, mData->nonSizedTransform.yv * mData->size.y);
		mData->transform.offs = mData->transform.offs - mData->transform.xv*mData->pivot.x - mData->transform.yv*mData->pivot.y;
	}

	void ActorTransform::UpdateWorldRectangleAndTransform()
	{
		if (mData->owner && mData->owner->mParent)
		{
			auto parentData = mData->owner->mParent->transform->mData;
			mData->parentRectangle = parentData->worldRectangle;
			mData->parentRectangePosition = mData->parentRectangle.LeftBottom() + parentData->size*parentData->pivot;
			mData->worldRectangle.left   = mData->parentRectangePosition.x + mData->rectangle.left;
			mData->worldRectangle.right  = mData->parentRectangePosition.x + mData->rectangle.right;
			mData->worldRectangle.bottom = mData->parentRectangePosition.y + mData->rectangle.bottom;
			mData->worldRectangle.top    = mData->parentRectangePosition.y + mData->rectangle.top;

			mData->parentTransform = mData->owner->mParent->transform->mData->worldNonSizedTransform;
			mData->worldTransform = mData->transform*mData->parentTransform;
			mData->worldNonSizedTransform = mData->nonSizedTransform*mData->parentTransform;
		}
		else
		{
			mData->parentRectangle.left = 0; mData->parentRectangle.right = 0;
			mData->parentRectangle.bottom = 0; mData->parentRectangle.top = 0;

			mData->parentRectangePosition = Vec2F();
			mData->worldRectangle.left = 0; mData->worldRectangle.right = 0;
			mData->worldRectangle.bottom = 0; mData->worldRectangle.top = 0;

			mData->parentTransform = Basis::Identity();
			mData->worldNonSizedTransform = mData->nonSizedTransform;
			mData->worldTransform = mData->transform;
		}
	}

	void ActorTransform::CheckParentInvTransform()
	{
		if (mData->isParentInvTransformActual)
			return;

		mData->isParentInvTransformActual = true;

		if (mData->owner && mData->owner->mParent)
			mData->parentInvertedTransform = mData->owner->mParent->transform->mData->worldNonSizedTransform.Inverted();
		else
			mData->parentInvertedTransform = Basis::Identity();
	}

	void ActorTransform::InitializeProperties()
	{
		INITIALIZE_GETTER(ActorTransform, actor, GetOwnerActor);

		INITIALIZE_PROPERTY(ActorTransform, position, SetPosition, GetPosition);
		INITIALIZE_PROPERTY(ActorTransform, size, SetSize, GetSize);
		INITIALIZE_PROPERTY(ActorTransform, width, SetWidth, GetWidht);
		INITIALIZE_PROPERTY(ActorTransform, height, SetHeight, GetHeight);
		INITIALIZE_PROPERTY(ActorTransform, scale, SetScale, GetScale);
		INITIALIZE_PROPERTY(ActorTransform, pivot, SetPivot, GetPivot);
		INITIALIZE_PROPERTY(ActorTransform, szPivot, SetSizePivot, GetSizePivot);
		INITIALIZE_PROPERTY(ActorTransform, angle, SetAngle, GetAngle);
		INITIALIZE_PROPERTY(ActorTransform, angleDegree, SetAngleDegrees, GetAngleDegrees);
		INITIALIZE_PROPERTY(ActorTransform, shear, SetShear, GetShear);
		INITIALIZE_PROPERTY(ActorTransform, basis, SetBasis, GetBasis);
		INITIALIZE_PROPERTY(ActorTransform, nonSizedBasis, SetNonSizedBasis, GetNonSizedBasis);
		INITIALIZE_PROPERTY(ActorTransform, AABB, SetAxisAlignedRect, GetAxisAlignedRect);
		INITIALIZE_PROPERTY(ActorTransform, rect, SetRect, GetRect);
		INITIALIZE_PROPERTY(ActorTransform, leftTop, SetLeftTop, GetLeftTop);
		INITIALIZE_PROPERTY(ActorTransform, leftBottom, SetLeftBottom, GetLeftBottom);
		INITIALIZE_PROPERTY(ActorTransform, rightTop, SetRightTop, GetRightTop);
		INITIALIZE_PROPERTY(ActorTransform, rightBottom, SetRightBottom, GetRightBottom);
		INITIALIZE_PROPERTY(ActorTransform, center, SetCenter, GetCenter);
		INITIALIZE_PROPERTY(ActorTransform, rightDir, SetRightDir, GetRightDir);
		INITIALIZE_PROPERTY(ActorTransform, leftDir, SetLeftDir, GetLeftDir);
		INITIALIZE_PROPERTY(ActorTransform, upDir, SetUpDir, GetUpDir);
		INITIALIZE_PROPERTY(ActorTransform, downDir, SetDownDir, GetDownDir);
		INITIALIZE_PROPERTY(ActorTransform, right, SetRight, GetRight);
		INITIALIZE_PROPERTY(ActorTransform, left, SetLeft, GetLeft);
		INITIALIZE_PROPERTY(ActorTransform, top, SetTop, GetTop);
		INITIALIZE_PROPERTY(ActorTransform, bottom, SetBottom, GetBottom);
		INITIALIZE_PROPERTY(ActorTransform, worldPosition, SetWorldPosition, GetWorldPosition);
		INITIALIZE_PROPERTY(ActorTransform, worldPivot, SetWorldPivot, GetWorldPivot);
		INITIALIZE_PROPERTY(ActorTransform, worldAngle, SetWorldAngle, GetWorldAngle);
		INITIALIZE_PROPERTY(ActorTransform, worldAngleDegree, SetWorldAngleDegree, GetWorldAngleDegree);
		INITIALIZE_PROPERTY(ActorTransform, worldBasis, SetWorldBasis, GetWorldBasis);
		INITIALIZE_PROPERTY(ActorTransform, worldNonSizedBasis, SetWorldNonSizedBasis, GetWorldNonSizedBasis);
		INITIALIZE_PROPERTY(ActorTransform, worldLeftTop, SetWorldLeftTop, GetWorldLeftTop);
		INITIALIZE_PROPERTY(ActorTransform, worldLeftBottom, SetWorldLeftBottom, GetWorldLeftBottom);
		INITIALIZE_PROPERTY(ActorTransform, worldRightTop, SetWorldRightTop, GetWorldRightTop);
		INITIALIZE_PROPERTY(ActorTransform, worldRightBottom, SetWorldRightBottom, GetWorldRightBottom);
		INITIALIZE_PROPERTY(ActorTransform, worldCenter, SetWorldCenter, GetWorldCenter);
		INITIALIZE_PROPERTY(ActorTransform, worldRightDir, SetWorldRightDir, GetWorldRightDir);
		INITIALIZE_PROPERTY(ActorTransform, worldLeftDir, SetWorldLeftDir, GetWorldLeftDir);
		INITIALIZE_PROPERTY(ActorTransform, worldUpDir, SetWorldUpDir, GetWorldUpDir);
		INITIALIZE_PROPERTY(ActorTransform, worldDownDir, SetWorldDownDir, GetWorldDownDir);
		INITIALIZE_PROPERTY(ActorTransform, worldRight, SetWorldRight, GetWorldRight);
		INITIALIZE_PROPERTY(ActorTransform, worldLeft, SetWorldLeft, GetWorldLeft);
		INITIALIZE_PROPERTY(ActorTransform, worldTop, SetWorldTop, GetWorldTop);
		INITIALIZE_PROPERTY(ActorTransform, worldBottom, SetWorldBottom, GetWorldBottom);
		INITIALIZE_PROPERTY(ActorTransform, worldRect, SetWorldRect, GetWorldRect);
		INITIALIZE_PROPERTY(ActorTransform, worldAABB, SetWorldAxisAlignedRect, GetWorldAxisAlignedRect);
	}

}

CLASS_META(o2::ActorTransform)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(actor);
	PUBLIC_FIELD(position);
	PUBLIC_FIELD(size);
	PUBLIC_FIELD(width);
	PUBLIC_FIELD(height);
	PUBLIC_FIELD(scale);
	PUBLIC_FIELD(pivot);
	PUBLIC_FIELD(szPivot);
	PUBLIC_FIELD(angle);
	PUBLIC_FIELD(angleDegree);
	PUBLIC_FIELD(shear);
	PUBLIC_FIELD(basis);
	PUBLIC_FIELD(nonSizedBasis);
	PUBLIC_FIELD(AABB);
	PUBLIC_FIELD(rect);
	PUBLIC_FIELD(leftTop);
	PUBLIC_FIELD(leftBottom);
	PUBLIC_FIELD(rightTop);
	PUBLIC_FIELD(rightBottom);
	PUBLIC_FIELD(center);
	PUBLIC_FIELD(rightDir);
	PUBLIC_FIELD(leftDir);
	PUBLIC_FIELD(upDir);
	PUBLIC_FIELD(downDir);
	PUBLIC_FIELD(right);
	PUBLIC_FIELD(left);
	PUBLIC_FIELD(top);
	PUBLIC_FIELD(bottom);
	PUBLIC_FIELD(worldPosition);
	PUBLIC_FIELD(worldPivot);
	PUBLIC_FIELD(worldAngle);
	PUBLIC_FIELD(worldAngleDegree);
	PUBLIC_FIELD(worldBasis);
	PUBLIC_FIELD(worldNonSizedBasis);
	PUBLIC_FIELD(worldLeftTop);
	PUBLIC_FIELD(worldLeftBottom);
	PUBLIC_FIELD(worldRightTop);
	PUBLIC_FIELD(worldRightBottom);
	PUBLIC_FIELD(worldCenter);
	PUBLIC_FIELD(worldRightDir);
	PUBLIC_FIELD(worldLeftDir);
	PUBLIC_FIELD(worldUpDir);
	PUBLIC_FIELD(worldDownDir);
	PUBLIC_FIELD(worldRight);
	PUBLIC_FIELD(worldLeft);
	PUBLIC_FIELD(worldTop);
	PUBLIC_FIELD(worldBottom);
	PUBLIC_FIELD(worldRect);
	PUBLIC_FIELD(worldAABB);
	PROTECTED_FIELD(mData).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(Actor*, GetOwnerActor);
	PUBLIC_FUNCTION(void, SetDirty);
	PUBLIC_FUNCTION(bool, IsDirty);
	PUBLIC_FUNCTION(void, Update);
	PUBLIC_FUNCTION(void, SetPosition, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPosition);
	PUBLIC_FUNCTION(void, SetSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetSize);
	PUBLIC_FUNCTION(void, SetWidth, float);
	PUBLIC_FUNCTION(float, GetWidht);
	PUBLIC_FUNCTION(void, SetHeight, float);
	PUBLIC_FUNCTION(float, GetHeight);
	PUBLIC_FUNCTION(void, SetPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPivot);
	PUBLIC_FUNCTION(void, SetSizePivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetSizePivot);
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
	PUBLIC_FUNCTION(void, SetRect, const RectF&);
	PUBLIC_FUNCTION(RectF, GetRect);
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
	PUBLIC_FUNCTION(void, SetRightDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetRightDir);
	PUBLIC_FUNCTION(void, SetLeftDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetLeftDir);
	PUBLIC_FUNCTION(void, SetUpDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetUpDir);
	PUBLIC_FUNCTION(void, SetDownDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetDownDir);
	PUBLIC_FUNCTION(void, SetRight, float);
	PUBLIC_FUNCTION(float, GetRight);
	PUBLIC_FUNCTION(void, SetLeft, float);
	PUBLIC_FUNCTION(float, GetLeft);
	PUBLIC_FUNCTION(void, SetTop, float);
	PUBLIC_FUNCTION(float, GetTop);
	PUBLIC_FUNCTION(void, SetBottom, float);
	PUBLIC_FUNCTION(float, GetBottom);
	PUBLIC_FUNCTION(Vec2F, World2LocalPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, Local2WorldPoint, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, World2LocalDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, Local2WorldDir, const Vec2F&);
	PUBLIC_FUNCTION(bool, IsPointInside, const Vec2F&);
	PUBLIC_FUNCTION(void, SetWorldPosition, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldPosition);
	PUBLIC_FUNCTION(void, SetWorldPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldPivot);
	PUBLIC_FUNCTION(void, SetWorldAngle, float);
	PUBLIC_FUNCTION(float, GetWorldAngle);
	PUBLIC_FUNCTION(void, SetWorldAngleDegree, float);
	PUBLIC_FUNCTION(float, GetWorldAngleDegree);
	PUBLIC_FUNCTION(void, SetWorldBasis, const Basis&);
	PUBLIC_FUNCTION(Basis, GetWorldBasis);
	PUBLIC_FUNCTION(void, SetWorldNonSizedBasis, const Basis&);
	PUBLIC_FUNCTION(Basis, GetWorldNonSizedBasis);
	PUBLIC_FUNCTION(void, SetWorldRect, const RectF&);
	PUBLIC_FUNCTION(RectF, GetWorldRect);
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
	PUBLIC_FUNCTION(void, SetWorldRightDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldRightDir);
	PUBLIC_FUNCTION(void, SetWorldLeftDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldLeftDir);
	PUBLIC_FUNCTION(void, SetWorldUpDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldUpDir);
	PUBLIC_FUNCTION(void, SetWorldDownDir, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWorldDownDir);
	PUBLIC_FUNCTION(void, SetWorldRight, float);
	PUBLIC_FUNCTION(float, GetWorldRight);
	PUBLIC_FUNCTION(void, SetWorldLeft, float);
	PUBLIC_FUNCTION(float, GetWorldLeft);
	PUBLIC_FUNCTION(void, SetWorldTop, float);
	PUBLIC_FUNCTION(float, GetWorldTop);
	PUBLIC_FUNCTION(void, SetWorldBottom, float);
	PUBLIC_FUNCTION(float, GetWorldBottom);
	PROTECTED_FUNCTION(void, SetOwner, Actor*);
	PROTECTED_FUNCTION(void, UpdateWorldRectangleAndTransform);
	PROTECTED_FUNCTION(void, UpdateTransform);
	PROTECTED_FUNCTION(void, UpdateRectangle);
	PROTECTED_FUNCTION(void, CheckParentInvTransform);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(o2::ActorTransform::Data)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(owner);
	PUBLIC_FIELD(isDirty);
	PUBLIC_FIELD(position).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(size).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(scale).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(pivot).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(angle).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(shear).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(rectangle);
	PUBLIC_FIELD(parentRectangle);
	PUBLIC_FIELD(parentRectangePosition);
	PUBLIC_FIELD(worldRectangle);
	PUBLIC_FIELD(transform);
	PUBLIC_FIELD(nonSizedTransform);
	PUBLIC_FIELD(worldNonSizedTransform);
	PUBLIC_FIELD(worldTransform);
	PUBLIC_FIELD(parentInvertedTransform);
	PUBLIC_FIELD(parentTransform);
	PUBLIC_FIELD(isParentInvTransformActual);
}
END_META;
