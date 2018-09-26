#include "stdafx.h"
#include "WidgetLayout.h"

#include "UI/Widget.h"
#include "Utils/Delegates.h"

namespace o2
{
	UIWidgetLayout::UIWidgetLayout():
		ActorTransform(mnew Data())
	{
		mData = static_cast<Data*>(ActorTransform::mData);
		mCheckMinMaxFunc = &UIWidgetLayout::DontCheckMinMax;
	}

	UIWidgetLayout::UIWidgetLayout(const UIWidgetLayout& other):
		ActorTransform(mnew Data()), anchorMin(this), anchorMax(this), offsetMin(this), offsetMax(this),
		anchorLeft(this), anchorRight(this), anchorBottom(this), anchorTop(this), offsetLeft(this),
		offsetBottom(this), offsetTop(this), minSize(this), minWidth(this), minHeight(this),
		maxSize(this), maxWidth(this), maxHeight(this), weight(this), widthWeight(this),
		heigthWeight(this), offsetRight(this)
	{
		mData = (Data*)ActorTransform::mData;

		CopyFrom(other);
		mCheckMinMaxFunc = other.mCheckMinMaxFunc;
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
		RectF parentRect = GetParentRectangle();
		RectF rectangle(mData->offsetMin + mData->anchorMin*parentRect.Size(),
						mData->offsetMax + mData->anchorMax*parentRect.Size());

		Vec2F delta = position - rectangle.LeftBottom() + rectangle.Size()*mData->pivot;
		mData->offsetMin += delta;
		mData->offsetMax += delta;

		SetDirty();
	}

	void UIWidgetLayout::SetSize(const Vec2F& size)
	{
		RectF parentRect = GetParentRectangle();
		RectF rectangle(mData->offsetMin + mData->anchorMin*parentRect.Size(),
						mData->offsetMax + mData->anchorMax*parentRect.Size());

		Vec2F szDelta = size - rectangle.Size();
		mData->offsetMax += szDelta*(Vec2F::One() - mData->pivot);
		mData->offsetMin -= szDelta*mData->pivot;

		SetDirty();
	}

	void UIWidgetLayout::SetRect(const RectF& rect)
	{
		RectF parentRect = GetParentRectangle();
		RectF parentAnchoredRect(parentRect.Size()*mData->anchorMin,
								 parentRect.Size()*mData->anchorMax);

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

	RectF UIWidgetLayout::GetRect() const
	{
		RectF parentRect;
		if (mData->owner->mParentWidget)
			parentRect = mData->owner->mParentWidget->mChildrenWorldRect;

		RectF rectangle(mData->offsetMin + mData->anchorMin*parentRect.Size(),
						mData->offsetMax + mData->anchorMax*parentRect.Size());

		return rectangle;
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

			default:
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

			case VerAlign::Both:
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

	void UIWidgetLayout::SetDirty(bool fromParent /*= true*/)
	{
		if (!fromParent && mData->drivenByParent && mData->owner && mData->owner->mParent)
			mData->owner->mParent->transform->SetDirty(fromParent);

		ActorTransform::SetDirty(fromParent);
	}

	RectF UIWidgetLayout::GetParentRectangle() const
	{
		if (mData->owner->mParentWidget)
			return mData->owner->mParentWidget->mChildrenWorldRect;
		else if (mData->owner->mParent)
			return mData->owner->mParent->transform->mData->worldRectangle;

		return RectF();
	}

	void UIWidgetLayout::Update()
	{
		RectF lastWorldRect = mData->worldRectangle;

		RectF parentWorldRect; 
		Vec2F parentWorldPosition;

		if (mData->owner->mParentWidget)
		{
			parentWorldRect = mData->owner->mParentWidget->mChildrenWorldRect;

			RectF notWidgetWorldRect = mData->owner->mParentWidget->transform->mData->worldRectangle;
			parentWorldPosition = notWidgetWorldRect.LeftBottom() +
				mData->owner->mParentWidget->transform->mData->pivot*notWidgetWorldRect.Size();
		}
		else if (mData->owner->mParent)
		{
			parentWorldRect = mData->owner->mParent->transform->mData->worldRectangle;

			parentWorldPosition = parentWorldRect.LeftBottom() +
				mData->owner->mParent->transform->mData->pivot*parentWorldRect.Size();
		}

		RectF worldRectangle(parentWorldRect.LeftBottom() + mData->offsetMin + mData->anchorMin*parentWorldRect.Size(),
							 parentWorldRect.LeftBottom() + mData->offsetMax + mData->anchorMax*parentWorldRect.Size());

		mData->size = worldRectangle.Size();
		mData->position = worldRectangle.LeftBottom() - parentWorldPosition + mData->size*mData->pivot;

		(this->*mCheckMinMaxFunc)();

		FloorRectangle();
		UpdateRectangle();
		UpdateTransform();
		UpdateWorldRectangleAndTransform();

		mData->updateFrame = mData->dirtyFrame;

		if (mData->owner)
		{
			mData->owner->mChildrenWorldRect = mData->worldRectangle;
			mData->owner->OnTransformUpdated();
		}
	}

	void UIWidgetLayout::FloorRectangle()
	{
		mData->size.x     = Math::Round(mData->size.x);
		mData->size.y     = Math::Round(mData->size.y);
		mData->position.x = Math::Round(mData->position.x);
		mData->position.y = Math::Round(mData->position.y);
	}

	void UIWidgetLayout::UpdateOffsetsByCurrentTransform()
	{
		SetRect(ActorTransform::GetRect());
	}

	void UIWidgetLayout::CopyFrom(const ActorTransform& other)
	{
		const UIWidgetLayout* otherLayout = dynamic_cast<const UIWidgetLayout*>(&other);

		if (otherLayout)
		{

			mData->anchorMin = otherLayout->mData->anchorMin;
			mData->anchorMax = otherLayout->mData->anchorMax;
			mData->offsetMin = otherLayout->mData->offsetMin;
			mData->offsetMax = otherLayout->mData->offsetMax;
			mData->minSize = otherLayout->mData->minSize;
			mData->maxSize = otherLayout->mData->maxSize;
			mData->weight = otherLayout->mData->weight;

			mCheckMinMaxFunc = otherLayout->mCheckMinMaxFunc;
		}

		ActorTransform::CopyFrom(other);
	}

	void UIWidgetLayout::CheckMinMax()
	{
		Vec2F resSize = mData->size;
		Vec2F minSizeWithChildren(mData->owner->GetMinWidthWithChildren(), mData->owner->GetMinHeightWithChildren());

		Vec2F clampSize(Math::Clamp(resSize.x, minSizeWithChildren.x, mData->maxSize.x),
						Math::Clamp(resSize.y, minSizeWithChildren.y, mData->maxSize.y));

		Vec2F szDelta = clampSize - resSize;

		if (szDelta != Vec2F())
			mData->size += szDelta;
	}

	void UIWidgetLayout::DontCheckMinMax()
	{}
}

DECLARE_CLASS(o2::UIWidgetLayout);

DECLARE_CLASS(o2::UIWidgetLayout::Data);
