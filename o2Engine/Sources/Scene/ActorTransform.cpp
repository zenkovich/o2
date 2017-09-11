#include "ActorTransform.h"

#include "Actor.h"

namespace o2
{
	ActorTransform::ActorTransform(Data* data):
		mData(data)
	{}

	ActorTransform::ActorTransform(const Vec2F& size /*= Vec2F()*/, const Vec2F& position /*= Vec2F()*/,
								   float angle /*= 0.0f*/, const Vec2F& scale /*= Vec2F(1.0f, 1.0f)*/,
								   const Vec2F& pivot /*= Vec2F(0.5f, 0.5f)*/):
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

		OnChanged();

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
		OnChanged();
	}

	void ActorTransform::SetPosition(const Vec2F& position)
	{
		mData->position = position;
		OnChanged();
	}

	Vec2F ActorTransform::GetPosition() const
	{
		return mData->position;
	}

	void ActorTransform::SetSize(const Vec2F& size)
	{
		mData->size = size;
		OnChanged();
	}

	Vec2F ActorTransform::GetSize() const
	{
		return mData->size;
	}

	void ActorTransform::SetWidth(float value)
	{
		mData->size.x = value;
		OnChanged();
	}

	float ActorTransform::GetWidht() const
	{
		return mData->size.x;
	}

	void ActorTransform::SetHeight(float value)
	{
		mData->size.y = value;
		OnChanged();
	}

	float ActorTransform::GetHeight() const
	{
		return mData->size.y;
	}

	void ActorTransform::SetPivot(const Vec2F& pivot)
	{
		mData->pivot = pivot;
		OnChanged();
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
		mData->position = rect.LeftBottom() + Vec2F(mData->size.x*mData->scale.x, mData->size.y*mData->scale.y)*mData->pivot;

		OnChanged();
	}

	RectF ActorTransform::GetRect() const
	{
		return RectF(mData->transform.offs, mData->transform.offs + mData->transform.xv + mData->transform.yv);
	}

	void ActorTransform::SetScale(const Vec2F& scale)
	{
		mData->scale = scale;
		OnChanged();
	}

	Vec2F ActorTransform::GetScale() const
	{
		return mData->scale;
	}

	void ActorTransform::SetAngle(float rad)
	{
		mData->angle = rad;
		OnChanged();
	}

	float ActorTransform::GetAngle() const
	{
		return mData->angle;
	}

	void ActorTransform::SetAngleDegrees(float deg)
	{
		mData->angle = Math::Deg2rad(deg);
		OnChanged();
	}

	float ActorTransform::GetAngleDegrees() const
	{
		return Math::Rad2deg(mData->angle);
	}

	void ActorTransform::SetShear(float shear)
	{
		mData->shear = shear;
		OnChanged();
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
		OnChanged();
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
		OnChanged();
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
		Basis transformed = mData->transform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mData->transform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->transform.xv);
		Vec2F frameDeltaY = delta.Project(mData->transform.yv);

		transformed.offs += frameDeltaX;
		transformed.xv -= frameDeltaX;
		transformed.yv += frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F ActorTransform::GetLeftTop() const
	{
		return mData->transform.offs + mData->transform.yv;
	}

	void ActorTransform::SetRightTop(const Vec2F& position)
	{
		Basis transformed = mData->transform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mData->transform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->transform.xv);
		Vec2F frameDeltaY = delta.Project(mData->transform.yv);

		transformed.xv += frameDeltaX;
		transformed.yv += frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F ActorTransform::GetRightTop() const
	{
		return mData->transform.offs + mData->transform.yv + mData->transform.xv;
	}

	void ActorTransform::SetLeftBottom(const Vec2F& position)
	{
		Basis transformed = mData->transform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mData->transform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->transform.xv);
		Vec2F frameDeltaY = delta.Project(mData->transform.yv);

		transformed.offs += frameDeltaX + frameDeltaY;
		transformed.xv -= frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F ActorTransform::GetLeftBottom() const
	{
		return mData->transform.offs;
	}

	void ActorTransform::SetRightBottom(const Vec2F& position)
	{
		Basis transformed = mData->transform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mData->transform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->transform.xv);
		Vec2F frameDeltaY = delta.Project(mData->transform.yv);

		transformed.offs += frameDeltaY;
		transformed.xv += frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetBasis(transformed);
	}

	Vec2F ActorTransform::GetRightBottom() const
	{
		return mData->transform.offs + mData->transform.xv;
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
		SetRightTop(Vec2F(value, GetTop()));
	}

	float ActorTransform::GetRight() const
	{
		return GetRightTop().x;
	}

	void ActorTransform::SetLeft(float value)
	{
		SetLeftTop(Vec2F(value, GetTop()));
	}

	float ActorTransform::GetLeft() const
	{
		return GetLeftTop().x;
	}

	void ActorTransform::SetTop(float value)
	{
		SetLeftTop(Vec2F(GetLeft(), value));
	}

	float ActorTransform::GetTop() const
	{
		return GetLeftTop().y;
	}

	void ActorTransform::SetBottom(float value)
	{
		SetLeftBottom(Vec2F(GetLeft(), value));
	}

	float ActorTransform::GetBottom() const
	{
		return GetLeftBottom().y;
	}

	Actor* ActorTransform::GetOwnerActor() const
	{
		return mData->owner;
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
		Basis transformed = mData->worldTransform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 1.0f)*mData->worldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->worldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->worldTransform.yv);

		transformed.offs += frameDeltaX;
		transformed.xv -= frameDeltaX;
		transformed.yv += frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldLeftTop() const
	{
		return mData->worldTransform.offs + mData->worldTransform.yv;
	}

	void ActorTransform::SetWorldRightTop(const Vec2F& position)
	{
		Basis transformed = mData->worldTransform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 1.0f)*mData->worldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->worldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->worldTransform.yv);

		transformed.xv += frameDeltaX;
		transformed.yv += frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldRightTop() const
	{
		return mData->worldTransform.offs + mData->worldTransform.yv + mData->worldTransform.xv;
	}

	void ActorTransform::SetWorldLeftBottom(const Vec2F& position)
	{
		Basis transformed = mData->worldTransform;
		Vec2F lastHandleCoords = Vec2F(0.0f, 0.0f)*mData->worldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->worldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->worldTransform.yv);

		transformed.offs += frameDeltaX + frameDeltaY;
		transformed.xv -= frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldLeftBottom() const
	{
		return mData->worldTransform.offs;
	}

	void ActorTransform::SetWorldRightBottom(const Vec2F& position)
	{
		Basis transformed = mData->worldTransform;
		Vec2F lastHandleCoords = Vec2F(1.0f, 0.0f)*mData->worldTransform;
		Vec2F delta = position - lastHandleCoords;
		Vec2F frameDeltaX = delta.Project(mData->worldTransform.xv);
		Vec2F frameDeltaY = delta.Project(mData->worldTransform.yv);

		transformed.offs += frameDeltaY;
		transformed.xv += frameDeltaX;
		transformed.yv -= frameDeltaY;

		SetWorldBasis(transformed);
	}

	Vec2F ActorTransform::GetWorldRightBottom() const
	{
		return mData->worldTransform.offs + mData->worldTransform.xv;
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

	}

	float ActorTransform::GetWorldRight() const
	{
		return 0;
	}

	void ActorTransform::SetWorldLeft(float value)
	{
	}

	float ActorTransform::GetWorldLeft() const
	{
		return 0;
	}

	void ActorTransform::SetWorldTop(float value)
	{

	}

	float ActorTransform::GetWorldTop() const
	{
		return 0;
	}

	void ActorTransform::SetWorldBottom(float value)
	{

	}

	float ActorTransform::GetWorldBottom() const
	{
		return 0;
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
		OnChanged();
	}

	void ActorTransform::OnChanged()
	{
		Vec2F leftBottom = mData->position - mData->size*mData->pivot;
		Vec2F rightTop = leftBottom + mData->size;
		mData->rectangle.left = leftBottom.x;
		mData->rectangle.right = rightTop.x;
		mData->rectangle.bottom = leftBottom.y;
		mData->rectangle.top = rightTop.y;

		mData->nonSizedTransform = Basis::Build(mData->position, mData->scale, mData->angle, mData->shear);
		mData->transform.Set(mData->nonSizedTransform.offs, mData->nonSizedTransform.xv * mData->size.x, mData->nonSizedTransform.yv * mData->size.y);
		mData->transform.offs = mData->transform.offs - mData->transform.xv*mData->pivot.x - mData->transform.yv*mData->pivot.y;
		mData->nonSizedTransform.offs = mData->transform.offs;

		if (mData->owner && mData->owner->mParent)
		{
			mData->parentTransform = mData->owner->mParent->transform->mData->worldNonSizedTransform;
			mData->worldTransform = mData->transform*mData->parentTransform;
			mData->worldNonSizedTransform = mData->nonSizedTransform*mData->parentTransform;
		}
		else
		{
			mData->worldNonSizedTransform = mData->nonSizedTransform;
			mData->worldTransform = mData->transform;
		}

		mData->isParentInvTransformActual = false;

		if (mData->owner)
			mData->owner->OnTransformChanged();
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
	BASE_CLASS(ISerializable);

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
	PROTECTED_FUNCTION(void, OnChanged);
	PROTECTED_FUNCTION(void, CheckParentInvTransform);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(ActorTransform::Data)
{
	BASE_CLASS(ISerializable);

	PUBLIC_FIELD(position).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(size).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(scale).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(pivot).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(angle).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(shear).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(transform);
	PUBLIC_FIELD(nonSizedTransform);
	PUBLIC_FIELD(worldNonSizedTransform);
	PUBLIC_FIELD(worldTransform);
	PUBLIC_FIELD(parentInvertedTransform);
	PUBLIC_FIELD(parentTransform);
	PUBLIC_FIELD(isParentInvTransformActual);
	PUBLIC_FIELD(owner);
}
END_META;
