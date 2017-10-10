#include "WidgetLayout.h"

#include "UI/Widget.h"
#include "Utils/Delegates.h"

namespace o2
{
	UIWidgetLayout::UIWidgetLayout():
		ActorTransform(mnew Data())
	{
		mData = (Data*)ActorTransform::mData;
		mCheckMinMaxFunc = &UIWidgetLayout::DontCheckMinMax;

		InitializeProperties();
	}

	UIWidgetLayout::UIWidgetLayout(const UIWidgetLayout& other):
		ActorTransform(mnew Data())
	{
		mData = (Data*)ActorTransform::mData;

		CopyFrom(other);
		mCheckMinMaxFunc = other.mCheckMinMaxFunc;

		InitializeProperties();
	}

	UIWidgetLayout::UIWidgetLayout(const Vec2F& anchorMin, const Vec2F& anchorMax,
								   const Vec2F& offsetMin, const Vec2F& offsetMax):
		ActorTransform(mnew Data())
	{
		mData = (Data*)ActorTransform::mData;

		mData->anchorMin = anchorMin;
		mData->anchorMax = anchorMax;
		mData->offsetMin = offsetMin;
		mData->offsetMax = offsetMax;

		mCheckMinMaxFunc = &UIWidgetLayout::DontCheckMinMax;

		InitializeProperties();
	}

	UIWidgetLayout::UIWidgetLayout(float anchorLeft, float anchorTop, float anchorRight, float anchorBottom,
								   float offsetLeft, float offsetTop, float offsetRight, float offsetBottom):
		ActorTransform(mnew Data())
	{
		mData = (Data*)ActorTransform::mData;

		mData->anchorMin.Set(anchorLeft, anchorBottom);
		mData->anchorMax.Set(anchorRight, anchorTop);
		mData->offsetMin.Set(offsetLeft, offsetBottom);
		mData->offsetMax.Set(offsetRight, offsetTop);

		mCheckMinMaxFunc = &UIWidgetLayout::DontCheckMinMax;

		InitializeProperties();
	}

	UIWidgetLayout& UIWidgetLayout::operator=(const UIWidgetLayout& other)
	{
		CopyFrom(other);
		SetDirty();

		return *this;
	}

	bool UIWidgetLayout::operator==(const UIWidgetLayout& other) const
	{
		return mData->anchorMin == other.mData->anchorMin &&
			mData->anchorMax == other.mData->anchorMax &&
			mData->offsetMin == other.mData->offsetMin &&
			mData->offsetMax == other.mData->offsetMax;
	}

	void UIWidgetLayout::SetPosition(const Vec2F& position)
	{
		Vec2F delta = position - mData->position;
		mData->offsetMin += delta;
		mData->offsetMax += delta;

		SetDirty();
	}

	void UIWidgetLayout::SetSize(const Vec2F& size)
	{
		Vec2F szDelta = size - mData->size;
		mData->offsetMax += szDelta*(Vec2F::One() - mData->pivot);
		mData->offsetMin -= szDelta*mData->pivot;

		SetDirty();
	}

	void UIWidgetLayout::SetRect(const RectF& rect)
	{
		RectF parentAnchoredRect(mData->parentRectangle.LeftBottom()*mData->anchorMin,
								 mData->parentRectangle.RightTop()*mData->anchorMax);

		mData->offsetMin = rect.LeftBottom() - parentAnchoredRect.LeftBottom();
		mData->offsetMax = rect.RightTop() - parentAnchoredRect.RightTop();

		SetDirty();
	}

	void UIWidgetLayout::SetAxisAlignedRect(const RectF& rect)
	{
		ActorTransform::SetAxisAlignedRect(rect);
		UpdateOffsetsByCurrentTransform();
	}

	void UIWidgetLayout::SetPivot(const Vec2F& pivot)
	{
		mData->pivot = pivot;
		SetDirty();
	}

	void UIWidgetLayout::SetBasis(const Basis& basis)
	{
		ActorTransform::SetBasis(basis);
		UpdateOffsetsByCurrentTransform();
	}

	void UIWidgetLayout::SetNonSizedBasis(const Basis& basis)
	{
		ActorTransform::SetNonSizedBasis(basis);
		UpdateOffsetsByCurrentTransform();
	}

	void UIWidgetLayout::SetAnchorMin(const Vec2F& min)
	{
		mData->anchorMin = min;
		SetDirty();
	}

	Vec2F UIWidgetLayout::GetAnchorMin() const
	{
		return mData->anchorMin;
	}

	void UIWidgetLayout::SetAnchorMax(const Vec2F& max)
	{
		mData->anchorMax = max;
		SetDirty();
	}

	Vec2F UIWidgetLayout::GetAnchorMax() const
	{
		return mData->anchorMax;
	}

	void UIWidgetLayout::SetAnchorLeft(float value)
	{
		mData->anchorMin.x = value;
		SetDirty();
	}

	float UIWidgetLayout::GetAnchorLeft() const
	{
		return mData->anchorMin.x;
	}

	void UIWidgetLayout::SetAnchorRight(float value)
	{
		mData->anchorMax.x = value;
		SetDirty();
	}

	float UIWidgetLayout::GetAnchorRight() const
	{
		return mData->anchorMax.x;
	}

	void UIWidgetLayout::SetAnchorBottom(float value)
	{
		mData->anchorMin.y = value;
		SetDirty();
	}

	float UIWidgetLayout::GetAnchorBottom() const
	{
		return mData->anchorMin.y;
	}

	void UIWidgetLayout::SetAnchorTop(float value)
	{
		mData->anchorMax.y = value;
		SetDirty();
	}

	float UIWidgetLayout::GetAnchorTop() const
	{
		return mData->anchorMax.y;
	}

	void UIWidgetLayout::SetOffsetMin(const Vec2F& min)
	{
		mData->offsetMin = min;
		SetDirty();
	}

	Vec2F UIWidgetLayout::GetOffsetMin() const
	{
		return mData->offsetMin;
	}

	void UIWidgetLayout::SetOffsetMax(const Vec2F& max)
	{
		mData->offsetMax = max;
		SetDirty();
	}

	Vec2F UIWidgetLayout::GetOffsetMax() const
	{
		return mData->offsetMax;
	}

	void UIWidgetLayout::SetOffsetLeft(float value)
	{
		mData->offsetMin.x = value;
		SetDirty();
	}

	float UIWidgetLayout::GetOffsetLeft() const
	{
		return mData->offsetMin.x;
	}

	void UIWidgetLayout::SetOffsetRight(float value)
	{
		mData->offsetMax.x = value;
		SetDirty();
	}

	float UIWidgetLayout::GetOffsetRight() const
	{
		return mData->offsetMax.x;
	}

	void UIWidgetLayout::SetOffsetBottom(float value)
	{
		mData->offsetMin.y = value;
		SetDirty();
	}

	float UIWidgetLayout::GetOffsetBottom() const
	{
		return mData->offsetMin.y;
	}

	void UIWidgetLayout::SetOffsetTop(float value)
	{
		mData->offsetMax.y = value;
		SetDirty();
	}

	float UIWidgetLayout::GetOffsetTop() const
	{
		return mData->offsetMax.y;
	}

	void UIWidgetLayout::SetMinimalSize(const Vec2F& minSize)
	{
		mData->minSize = minSize;
		mCheckMinMaxFunc = &UIWidgetLayout::CheckMinMax;
		SetDirty();
	}

	Vec2F UIWidgetLayout::GetMinimalSize() const
	{
		return mData->minSize;
	}

	void UIWidgetLayout::SetMinimalWidth(float value)
	{
		mData->minSize.x = value;
		mCheckMinMaxFunc = &UIWidgetLayout::CheckMinMax;
		SetDirty();
	}

	float UIWidgetLayout::GetMinimalWidth() const
	{
		return mData->minSize.x;
	}

	void UIWidgetLayout::SetMinimalHeight(float value)
	{
		mData->minSize.y = value;
		mCheckMinMaxFunc = &UIWidgetLayout::CheckMinMax;
		SetDirty();
	}

	float UIWidgetLayout::GetMinimalHeight() const
	{
		return mData->minSize.y;
	}

	void UIWidgetLayout::SetMaximalSize(const Vec2F& maxSize)
	{
		mData->maxSize = maxSize;
		mCheckMinMaxFunc = &UIWidgetLayout::CheckMinMax;
		SetDirty();
	}

	Vec2F UIWidgetLayout::GetMaximalSize() const
	{
		return mData->maxSize;
	}

	void UIWidgetLayout::SetMaximalWidth(float value)
	{
		mData->maxSize.x = value;
		mCheckMinMaxFunc = &UIWidgetLayout::CheckMinMax;
		SetDirty();
	}

	float UIWidgetLayout::GetMaximalWidth() const
	{
		return mData->maxSize.x;
	}

	void UIWidgetLayout::SetMaximalHeight(float value)
	{
		mData->maxSize.y = value;
		mCheckMinMaxFunc = &UIWidgetLayout::CheckMinMax;
		SetDirty();
	}

	float UIWidgetLayout::GetMaximalHeight() const
	{
		return mData->maxSize.y;
	}

	void UIWidgetLayout::DisableMinMaxSizes()
	{
		mCheckMinMaxFunc = &UIWidgetLayout::CheckMinMax;
		SetDirty();
	}

	void UIWidgetLayout::SetWeight(const Vec2F& weight)
	{
		mData->weight = weight;
		SetDirty();
	}

	Vec2F UIWidgetLayout::GetWeight() const
	{
		return mData->weight;
	}

	void UIWidgetLayout::SetWidthWeight(float widthWeigth)
	{
		mData->weight.x = widthWeigth;
		SetDirty();
	}

	float UIWidgetLayout::GetWidthWeight()
	{
		return mData->weight.x;
	}

	void UIWidgetLayout::SetHeightWeight(float heigthWeigth)
	{
		mData->weight.y = heigthWeigth;
		SetDirty();
	}

	float UIWidgetLayout::GetHeightWeight()
	{
		return mData->weight.y;
	}

	UIWidgetLayout UIWidgetLayout::BothStretch(float borderLeft, float borderBottom, float borderRight, float borderTop)
	{
		UIWidgetLayout res;
		res.mData->anchorMin = Vec2F(0, 0);
		res.mData->anchorMax = Vec2F(1, 1);
		res.mData->offsetMin = Vec2F(borderLeft, borderBottom);
		res.mData->offsetMax = Vec2F(-borderRight, -borderTop);
		return res;
	}

	UIWidgetLayout UIWidgetLayout::Based(BaseCorner corner, const Vec2F& size, const Vec2F& offset)
	{
		UIWidgetLayout res;
		switch (corner)
		{
			case BaseCorner::Left:
			res.mData->anchorMin = Vec2F(0.0f, 0.5f);
			res.mData->anchorMax = Vec2F(0.0f, 0.5f);
			res.mData->offsetMin = Vec2F(0.0f, -size.y*0.5f) + offset;
			res.mData->offsetMax = Vec2F(size.x, size.y*0.5f) + offset;
			break;

			case BaseCorner::Right:
			res.mData->anchorMin = Vec2F(1.0f, 0.5f);
			res.mData->anchorMax = Vec2F(1.0f, 0.5f);
			res.mData->offsetMin = Vec2F(-size.x, -size.y*0.5f) + offset;
			res.mData->offsetMax = Vec2F(0.0f, size.y*0.5f) + offset;
			break;
			case BaseCorner::Top:
			res.mData->anchorMin = Vec2F(0.5f, 1.0f);
			res.mData->anchorMax = Vec2F(0.5f, 1.0f);
			res.mData->offsetMin = Vec2F(-size.x*0.5f, -size.y) + offset;
			res.mData->offsetMax = Vec2F(size.x*0.5f, 0.0f) + offset;
			break;

			case BaseCorner::Bottom:
			res.mData->anchorMin = Vec2F(0.5f, 0.0f);
			res.mData->anchorMax = Vec2F(0.5f, 0.0f);
			res.mData->offsetMin = Vec2F(-size.x*0.5f, 0.0f) + offset;
			res.mData->offsetMax = Vec2F(size.x*0.5f, size.y) + offset;
			break;

			case BaseCorner::Center:
			res.mData->anchorMin = Vec2F(0.5f, 0.5f);
			res.mData->anchorMax = Vec2F(0.5f, 0.5f);
			res.mData->offsetMin = Vec2F(-size.x*0.5f, -size.y*0.5f) + offset;
			res.mData->offsetMax = Vec2F(size.x*0.5f, size.y*0.5f) + offset;
			break;

			case BaseCorner::LeftBottom:
			res.mData->anchorMin = Vec2F(0.0f, 0.0f);
			res.mData->anchorMax = Vec2F(0.0f, 0.0f);
			res.mData->offsetMin = Vec2F(0.0f, 0.0f) + offset;
			res.mData->offsetMax = Vec2F(size.x, size.y) + offset;
			break;

			case BaseCorner::LeftTop:
			res.mData->anchorMin = Vec2F(0.0f, 1.0f);
			res.mData->anchorMax = Vec2F(0.0f, 1.0f);
			res.mData->offsetMin = Vec2F(0.0f, -size.y) + offset;
			res.mData->offsetMax = Vec2F(size.x, 0.0f) + offset;
			break;

			case BaseCorner::RightBottom:
			res.mData->anchorMin = Vec2F(1.0f, 0.0f);
			res.mData->anchorMax = Vec2F(1.0f, 0.0f);
			res.mData->offsetMin = Vec2F(-size.x, 0.0f) + offset;
			res.mData->offsetMax = Vec2F(0.0f, size.y) + offset;
			break;

			case BaseCorner::RightTop:
			res.mData->anchorMin = Vec2F(1.0f, 1.0f);
			res.mData->anchorMax = Vec2F(1.0f, 1.0f);
			res.mData->offsetMin = Vec2F(-size.x, -size.y) + offset;
			res.mData->offsetMax = Vec2F(0.0f, 0.0f) + offset;
			break;
		}

		return res;
	}

	UIWidgetLayout UIWidgetLayout::VerStretch(HorAlign align, float top, float bottom, float width, float offsX)
	{
		UIWidgetLayout res;
		res.mData->anchorMin.y = 0.0f;
		res.mData->anchorMax.y = 1.0f;
		res.mData->offsetMin.y = bottom;
		res.mData->offsetMax.y = -top;

		switch (align)
		{
			case HorAlign::Left:
			res.mData->anchorMin.x = 0.0f;
			res.mData->anchorMax.x = 0.0f;
			res.mData->offsetMin.x = offsX;
			res.mData->offsetMax.x = offsX + width;
			break;

			case HorAlign::Middle:
			res.mData->anchorMin.x = 0.5f;
			res.mData->anchorMax.x = 0.5f;
			res.mData->offsetMin.x = offsX - width*0.5f;
			res.mData->offsetMax.x = offsX + width*0.5f;
			break;

			case HorAlign::Right:
			res.mData->anchorMin.x = 1.0f;
			res.mData->anchorMax.x = 1.0f;
			res.mData->offsetMin.x = -offsX - width;
			res.mData->offsetMax.x = -offsX;
			break;
		}

		return res;
	}

	UIWidgetLayout UIWidgetLayout::HorStretch(VerAlign align, float left, float right, float height, float offsY)
	{
		UIWidgetLayout res;
		res.mData->anchorMin.x = 0.0f;
		res.mData->anchorMax.x = 1.0f;
		res.mData->offsetMin.x = left;
		res.mData->offsetMax.x = -right;

		switch (align)
		{
			case VerAlign::Top:
			res.mData->anchorMin.y = 1.0f;
			res.mData->anchorMax.y = 1.0f;
			res.mData->offsetMin.y = -offsY - height;
			res.mData->offsetMax.y = -offsY;
			break;

			case VerAlign::Middle:
			res.mData->anchorMin.y = 0.5f;
			res.mData->anchorMax.y = 0.5f;
			res.mData->offsetMin.y = offsY - height*0.5f;
			res.mData->offsetMax.y = offsY + height*0.5f;
			break;

			case VerAlign::Bottom:
			res.mData->anchorMin.y = 0.0f;
			res.mData->anchorMax.y = 0.0f;
			res.mData->offsetMin.y = offsY;
			res.mData->offsetMax.y = offsY + height;
			break;
		}

		return res;
	}

	void UIWidgetLayout::SetOwner(Actor* actor)
	{
		ActorTransform::SetOwner(actor);
		mData->owner = (UIWidget*)actor;
		SetDirty();
	}

	void UIWidgetLayout::SetDirty()
	{
		if (mData->drivenByParent && mData->owner && mData->owner->mParent)
			mData->owner->mParent->transform->SetDirty();

		ActorTransform::SetDirty();
	}

	void UIWidgetLayout::Update()
	{
		RectF lastWorldRect = mData->worldRectangle;

		RectF parentRect;
		if (mData->owner->mParentWidget)
			parentRect = mData->owner->mParentWidget->mChildrenWorldRect;

		RectF rectangle(mData->offsetMin + mData->anchorMin*parentRect.Size(),
						mData->offsetMax + mData->anchorMax*parentRect.Size());

		mData->size = rectangle.Size();
		mData->position = rectangle.LeftBottom() + mData->size*mData->pivot;

		(this->*mCheckMinMaxFunc)();

		UpdateRectangle();
		FloorRectangle();
		UpdateTransform();
		UpdateWorldRectangleAndTransform();

		mData->isParentInvTransformActual = false;
		mData->isDirty = false;

		if (mData->owner)
		{
			mData->owner->mChildrenWorldRect = mData->worldRectangle;
			mData->owner->OnTransformUpdated();
		}
	}

	void UIWidgetLayout::FloorRectangle()
	{
		mData->rectangle.left   = Math::Floor(mData->rectangle.left);
		mData->rectangle.right  = Math::Floor(mData->rectangle.right);
		mData->rectangle.bottom = Math::Floor(mData->rectangle.bottom);
		mData->rectangle.top    = Math::Floor(mData->rectangle.top);
	}

	void UIWidgetLayout::UpdateOffsetsByCurrentTransform()
	{
		SetRect(GetRect());
	}

	void UIWidgetLayout::CopyFrom(const UIWidgetLayout& other)
	{
		mData->size = other.mData->size;
		mData->position = other.mData->position;
		mData->angle = other.mData->angle;
		mData->scale = other.mData->scale;
		mData->pivot = other.mData->pivot;
		mData->shear = other.mData->shear;

		mData->anchorMin = other.mData->anchorMin;
		mData->anchorMax = other.mData->anchorMax;
		mData->offsetMin = other.mData->offsetMin;
		mData->offsetMax = other.mData->offsetMax;
		mData->minSize = other.mData->minSize;
		mData->maxSize = other.mData->maxSize;
		mData->weight = other.mData->weight;

		mCheckMinMaxFunc = other.mCheckMinMaxFunc;

		SetDirty();
	}

	void UIWidgetLayout::CheckMinMax()
	{
		Vec2F resSize = mData->size;
		Vec2F clampSize(Math::Clamp(resSize.x, mData->minSize.x, mData->maxSize.x),
						Math::Clamp(resSize.y, mData->minSize.y, mData->maxSize.y));
		Vec2F szDelta = clampSize - resSize;

		if (szDelta != Vec2F())
			mData->size += szDelta;
	}

	void UIWidgetLayout::DontCheckMinMax()
	{}

	void UIWidgetLayout::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIWidgetLayout, pivot, SetPivot, GetPivot);
		INITIALIZE_PROPERTY(UIWidgetLayout, position, SetPosition, GetPosition);
		INITIALIZE_PROPERTY(UIWidgetLayout, size, SetSize, GetSize);
		INITIALIZE_PROPERTY(UIWidgetLayout, width, SetWidth, GetWidth);
		INITIALIZE_PROPERTY(UIWidgetLayout, height, SetHeight, GetHeight);
		INITIALIZE_PROPERTY(UIWidgetLayout, absPosition, SetAbsolutePosition, GetAbsolutePosition);
		INITIALIZE_PROPERTY(UIWidgetLayout, absRect, SetAbsoluteRect, GetAbsoluteRect);
		INITIALIZE_PROPERTY(UIWidgetLayout, absLeftTop, SetAbsoluteLeftTop, GetAbsoluteLeftTop);
		INITIALIZE_PROPERTY(UIWidgetLayout, absLeftBottom, SetAbsoluteLeftBottom, GetAbsoluteLeftBottom);
		INITIALIZE_PROPERTY(UIWidgetLayout, absRightBottom, SetAbsoluteRightBottom, GetAbsoluteRightBottom);
		INITIALIZE_PROPERTY(UIWidgetLayout, absRightTop, SetAbsoluteRightTop, GetAbsoluteRightTop);
		INITIALIZE_PROPERTY(UIWidgetLayout, absLeft, SetAbsoluteLeft, GetAbsoluteLeft);
		INITIALIZE_PROPERTY(UIWidgetLayout, absRight, SetAbsoluteRight, GetAbsoluteRight);
		INITIALIZE_PROPERTY(UIWidgetLayout, absBottom, SetAbsoluteBottom, GetAbsoluteBottom);
		INITIALIZE_PROPERTY(UIWidgetLayout, absTop, SetAbsoluteTop, GetAbsoluteTop);
		INITIALIZE_PROPERTY(UIWidgetLayout, pivotX, SetPivotX, GetPivotX);
		INITIALIZE_PROPERTY(UIWidgetLayout, pivotY, SetPivotY, GetPivotY);
		INITIALIZE_PROPERTY(UIWidgetLayout, anchorMin, SetAnchorMin, GetAnchorMin);
		INITIALIZE_PROPERTY(UIWidgetLayout, anchorMax, SetAnchorMax, GetAnchorMax);
		INITIALIZE_PROPERTY(UIWidgetLayout, offsetMin, SetOffsetMin, GetOffsetMin);
		INITIALIZE_PROPERTY(UIWidgetLayout, offsetMax, SetOffsetMax, GetOffsetMax);
		INITIALIZE_PROPERTY(UIWidgetLayout, anchorLeft, SetAnchorLeft, GetAnchorLeft);
		INITIALIZE_PROPERTY(UIWidgetLayout, anchorRight, SetAnchorRight, GetAnchorRight);
		INITIALIZE_PROPERTY(UIWidgetLayout, anchorBottom, SetAnchorBottom, GetAnchorBottom);
		INITIALIZE_PROPERTY(UIWidgetLayout, anchorTop, SetAnchorTop, GetAnchorTop);
		INITIALIZE_PROPERTY(UIWidgetLayout, offsetLeft, SetOffsetLeft, GetOffsetLeft);
		INITIALIZE_PROPERTY(UIWidgetLayout, offsetRight, SetOffsetRight, GetOffsetRight);
		INITIALIZE_PROPERTY(UIWidgetLayout, offsetBottom, SetOffsetBottom, GetOffsetBottom);
		INITIALIZE_PROPERTY(UIWidgetLayout, offsetTop, SetOffsetTop, GetOffsetTop);
		INITIALIZE_PROPERTY(UIWidgetLayout, minSize, SetMinimalSize, GetMinimalSize);
		INITIALIZE_PROPERTY(UIWidgetLayout, minWidth, SetMinimalWidth, GetMinimalWidth);
		INITIALIZE_PROPERTY(UIWidgetLayout, minHeight, SetMinimalHeight, GetMinimalHeight);
		INITIALIZE_PROPERTY(UIWidgetLayout, maxSize, SetMaximalSize, GetMaximalSize);
		INITIALIZE_PROPERTY(UIWidgetLayout, maxWidth, SetMaximalWidth, GetMaximalWidth);
		INITIALIZE_PROPERTY(UIWidgetLayout, maxHeight, SetMaximalHeight, GetMaximalHeight);
		INITIALIZE_PROPERTY(UIWidgetLayout, weight, SetWeight, GetWeight);
		INITIALIZE_PROPERTY(UIWidgetLayout, widthWeight, SetWidthWeight, GetWidthWeight);
		INITIALIZE_PROPERTY(UIWidgetLayout, heigthWeight, SetHeightWeight, GetHeightWeight);
	}
}

CLASS_META(o2::UIWidgetLayout)
{
	BASE_CLASS(o2::ActorTransform);

	PUBLIC_FIELD(anchorMin);
	PUBLIC_FIELD(anchorMax);
	PUBLIC_FIELD(offsetMin);
	PUBLIC_FIELD(offsetMax);
	PUBLIC_FIELD(anchorLeft);
	PUBLIC_FIELD(anchorRight);
	PUBLIC_FIELD(anchorBottom);
	PUBLIC_FIELD(anchorTop);
	PUBLIC_FIELD(offsetLeft);
	PUBLIC_FIELD(offsetRight);
	PUBLIC_FIELD(offsetBottom);
	PUBLIC_FIELD(offsetTop);
	PUBLIC_FIELD(minSize);
	PUBLIC_FIELD(minWidth);
	PUBLIC_FIELD(minHeight);
	PUBLIC_FIELD(maxSize);
	PUBLIC_FIELD(maxWidth);
	PUBLIC_FIELD(maxHeight);
	PUBLIC_FIELD(weight);
	PUBLIC_FIELD(widthWeight);
	PUBLIC_FIELD(heigthWeight);
	PROTECTED_FIELD(mCheckMinMaxFunc);
	PROTECTED_FIELD(mData);

	PUBLIC_FUNCTION(void, Update);
	PUBLIC_FUNCTION(void, SetPosition, const Vec2F&);
	PUBLIC_FUNCTION(void, SetSize, const Vec2F&);
	PUBLIC_FUNCTION(void, SetPivot, const Vec2F&);
	PUBLIC_FUNCTION(void, SetBasis, const Basis&);
	PUBLIC_FUNCTION(void, SetNonSizedBasis, const Basis&);
	PUBLIC_FUNCTION(void, SetRect, const RectF&);
	PUBLIC_FUNCTION(void, SetAxisAlignedRect, const RectF&);
	PUBLIC_FUNCTION(void, SetAnchorMin, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetAnchorMin);
	PUBLIC_FUNCTION(void, SetAnchorMax, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetAnchorMax);
	PUBLIC_FUNCTION(void, SetAnchorLeft, float);
	PUBLIC_FUNCTION(float, GetAnchorLeft);
	PUBLIC_FUNCTION(void, SetAnchorRight, float);
	PUBLIC_FUNCTION(float, GetAnchorRight);
	PUBLIC_FUNCTION(void, SetAnchorBottom, float);
	PUBLIC_FUNCTION(float, GetAnchorBottom);
	PUBLIC_FUNCTION(void, SetAnchorTop, float);
	PUBLIC_FUNCTION(float, GetAnchorTop);
	PUBLIC_FUNCTION(void, SetOffsetMin, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetOffsetMin);
	PUBLIC_FUNCTION(void, SetOffsetMax, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetOffsetMax);
	PUBLIC_FUNCTION(void, SetOffsetLeft, float);
	PUBLIC_FUNCTION(float, GetOffsetLeft);
	PUBLIC_FUNCTION(void, SetOffsetRight, float);
	PUBLIC_FUNCTION(float, GetOffsetRight);
	PUBLIC_FUNCTION(void, SetOffsetBottom, float);
	PUBLIC_FUNCTION(float, GetOffsetBottom);
	PUBLIC_FUNCTION(void, SetOffsetTop, float);
	PUBLIC_FUNCTION(float, GetOffsetTop);
	PUBLIC_FUNCTION(void, SetMinimalSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetMinimalSize);
	PUBLIC_FUNCTION(void, SetMinimalWidth, float);
	PUBLIC_FUNCTION(float, GetMinimalWidth);
	PUBLIC_FUNCTION(void, SetMinimalHeight, float);
	PUBLIC_FUNCTION(float, GetMinimalHeight);
	PUBLIC_FUNCTION(void, SetMaximalSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetMaximalSize);
	PUBLIC_FUNCTION(void, SetMaximalWidth, float);
	PUBLIC_FUNCTION(float, GetMaximalWidth);
	PUBLIC_FUNCTION(void, SetMaximalHeight, float);
	PUBLIC_FUNCTION(float, GetMaximalHeight);
	PUBLIC_FUNCTION(void, DisableMinMaxSizes);
	PUBLIC_FUNCTION(void, SetWeight, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWeight);
	PUBLIC_FUNCTION(void, SetWidthWeight, float);
	PUBLIC_FUNCTION(float, GetWidthWeight);
	PUBLIC_FUNCTION(void, SetHeightWeight, float);
	PUBLIC_FUNCTION(float, GetHeightWeight);
	PROTECTED_FUNCTION(void, SetOwner, Actor*);
	PROTECTED_FUNCTION(void, SetDirty);
	PROTECTED_FUNCTION(void, FloorRectangle);
	PROTECTED_FUNCTION(void, UpdateOffsetsByCurrentTransform);
	PROTECTED_FUNCTION(void, CopyFrom, const UIWidgetLayout&);
	PROTECTED_FUNCTION(void, CheckMinMax);
	PROTECTED_FUNCTION(void, DontCheckMinMax);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;

CLASS_META(o2::UIWidgetLayout::Data)
{
	BASE_CLASS(o2::ActorTransform::Data);

	PUBLIC_FIELD(anchorMin).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(anchorMax).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(offsetMin).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(offsetMax).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(minSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(maxSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(weight).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(drivenByParent);
	PUBLIC_FIELD(owner);
}
END_META;
