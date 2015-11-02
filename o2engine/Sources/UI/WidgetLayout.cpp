#include "WidgetLayout.h"

#include "UI/Widget.h"

namespace o2
{
	IOBJECT_CPP(WidgetLayout);

	WidgetLayout::WidgetLayout()
	{
		InitializeProperties();
	}

	WidgetLayout::WidgetLayout(const WidgetLayout& other):
		mPivot(other.mPivot), mAnchorMin(other.mAnchorMin), mAnchorMax(other.mAnchorMax),
		mOffsetMin(other.mOffsetMin), mOffsetMax(other.mOffsetMax), mFitByChilds(other.mFitByChilds)
	{
		InitializeProperties();
	}

	WidgetLayout& WidgetLayout::operator=(const WidgetLayout& other)
	{
		mPivot = other.mPivot;
		mAnchorMin = other.mAnchorMin;
		mAnchorMax = other.mAnchorMax;
		mOffsetMin = other.mOffsetMin;
		mOffsetMax = other.mOffsetMax;
		mFitByChilds = other.mFitByChilds;

		Recalculate();

		return *this;
	}

	void WidgetLayout::SetPosition(const Vec2F& position)
	{
		Vec2F delta = position - GetPosition();
		mOffsetMin += delta;
		mOffsetMax += delta;
		Recalculate();
	}

	Vec2F WidgetLayout::GetPosition()
	{
		return mLocalRect.LeftBottom() + mLocalRect.Size()*mPivot;
	}

	void WidgetLayout::SetSize(const Vec2F& size)
	{
		Vec2F szDelta = size - GetSize();
		mOffsetMax += szDelta*(Vec2F::One() - mPivot);
		mOffsetMin -= szDelta*mPivot;
		Recalculate();
	}

	Vec2F WidgetLayout::GetSize() const
	{
		return mAbsoluteRect.Size();
	}

	void WidgetLayout::SetWidth(float width)
	{
		float delta = width - GetWidth();
		mOffsetMax.x += delta*(1.0f - mPivot.x);
		mOffsetMin.x -= delta*mPivot.x;
		Recalculate();
	}

	float WidgetLayout::GetWidth() const
	{
		return mAbsoluteRect.Width();
	}

	void WidgetLayout::SetHeight(float height)
	{
		float delta = height - GetHeight();
		mOffsetMax.y += delta*(1.0f - mPivot.y);
		mOffsetMin.y -= delta*mPivot.y;
		Recalculate();
	}

	float WidgetLayout::GetHeight() const
	{
		return mAbsoluteRect.Height();
	}

	void WidgetLayout::SetRect(const RectF& rect)
	{
		mOffsetMin += rect.LeftBottom() - mLocalRect.LeftBottom();
		mOffsetMax += rect.RightTop() - mLocalRect.RightTop();
		Recalculate();
	}

	RectF WidgetLayout::GetRect() const
	{
		return mLocalRect;
	}

	void WidgetLayout::SetAbsoluteLeft(float value)
	{
		mOffsetMin.x += value - mAbsoluteRect.left;
		Recalculate();
	}

	float WidgetLayout::GetAbsoluteLeft() const
	{
		return mAbsoluteRect.left;
	}

	void WidgetLayout::SetAbsoluteRight(float value)
	{
		mOffsetMax.x += value - mAbsoluteRect.right;
		Recalculate();
	}

	float WidgetLayout::GetAbsoluteRight() const
	{
		return mAbsoluteRect.right;
	}

	void WidgetLayout::SetAbsoluteBottom(float value)
	{
		mOffsetMin.y += value - mAbsoluteRect.bottom;
		Recalculate();
	}

	float WidgetLayout::GetAbsoluteBottom() const
	{
		return mAbsoluteRect.bottom;
	}

	void WidgetLayout::SetAbsoluteTop(float value)
	{
		mOffsetMax.y += value - mAbsoluteRect.top;
		Recalculate();
	}

	float WidgetLayout::GetAbsoluteTop() const
	{
		return mAbsoluteRect.top;
	}

	void WidgetLayout::SetAbsolutePosition(const Vec2F& absPosition)
	{
		Vec2F delta = position - GetAbsolutePosition();
		mOffsetMin += delta;
		mOffsetMax += delta;
		Recalculate();
	}

	Vec2F WidgetLayout::GetAbsolutePosition() const
	{
		return mAbsoluteRect.LeftBottom() + mAbsoluteRect.Size()*mPivot;
	}

	void WidgetLayout::SetAbsoluteRect(const RectF& rect)
	{
		mOffsetMin += rect.LeftBottom() - mAbsoluteRect.LeftBottom();
		mOffsetMax += rect.RightTop() - mAbsoluteRect.RightTop();
		Recalculate();
	}

	RectF WidgetLayout::GetAbsoluteRect() const
	{
		return mAbsoluteRect;
	}

	void WidgetLayout::SetPivot(const Vec2F& pivot)
	{
		mPivot = pivot;
		Recalculate();
	}

	Vec2F WidgetLayout::GetPivot() const
	{
		return mPivot;
	}

	void WidgetLayout::SetPivotX(float x)
	{
		mPivot.x = x;
		Recalculate();
	}

	float WidgetLayout::GetPivotX() const
	{
		return mPivot.x;
	}

	void WidgetLayout::SetPivotY(float y)
	{
		mPivot.y = y;
		Recalculate();
	}

	float WidgetLayout::GetPivotY() const
	{
		return mPivot.y;
	}

	void WidgetLayout::SetAnchorMin(const Vec2F& min)
	{
		mAnchorMin = min;
		Recalculate();
	}

	Vec2F WidgetLayout::GetAnchorMin() const
	{
		return mAnchorMin;
	}

	void WidgetLayout::SetAnchorMax(const Vec2F& max)
	{
		mAnchorMax = max;
		Recalculate();
	}

	Vec2F WidgetLayout::GetAnchorMax() const
	{
		return mAnchorMax;
	}

	void WidgetLayout::SetAnchorLeft(float value)
	{
		mAnchorMin.x = value;
		Recalculate();
	}

	float WidgetLayout::GetAnchorLeft() const
	{
		return mAnchorMin.x;
	}

	void WidgetLayout::SetAnchorRight(float value)
	{
		mAnchorMax.x = value;
		Recalculate();
	}

	float WidgetLayout::GetAnchorRight() const
	{
		return mAnchorMax.x;
	}

	void WidgetLayout::SetAnchorBottom(float value)
	{
		mAnchorMin.y = value;
		Recalculate();
	}

	float WidgetLayout::GetAnchorBottom() const
	{
		return mAnchorMin.y;
	}

	void WidgetLayout::SetAnchorTop(float value)
	{
		mAnchorMax.y = value;
		Recalculate();
	}

	float WidgetLayout::GetAnchorTop() const
	{
		return mAnchorMax.y;
	}

	void WidgetLayout::SetOffsetMin(const Vec2F& min)
	{
		mOffsetMin = min;
		Recalculate();
	}

	Vec2F WidgetLayout::GetOffsetMin() const
	{
		return mOffsetMin;
	}

	void WidgetLayout::SetOffsetMax(const Vec2F& max)
	{
		mOffsetMax = max;
		Recalculate();
	}

	Vec2F WidgetLayout::GetOffsetMax() const
	{
		return mOffsetMax;
	}

	void WidgetLayout::SetOffsetLeft(float value)
	{
		mOffsetMin.x = value;
		Recalculate();
	}

	float WidgetLayout::GetOffsetLeft() const
	{
		return mOffsetMin.x;
	}

	void WidgetLayout::SetOffsetRight(float value)
	{
		mOffsetMax.x = value;
		Recalculate();
	}

	float WidgetLayout::GetOffsetRight() const
	{
		return mOffsetMax.x;
	}

	void WidgetLayout::SetOffsetBottom(float value)
	{
		mOffsetMin.y = value;
		Recalculate();
	}

	float WidgetLayout::GetOffsetBottom() const
	{
		return mOffsetMin.y;
	}

	void WidgetLayout::SetOffsetTop(float value)
	{
		mOffsetMax.y = value;
		Recalculate();
	}

	float WidgetLayout::GetOffsetTop() const
	{
		return mOffsetMax.y;
	}

	void WidgetLayout::SetMinimalSize(const Vec2F& minSize)
	{
		mMinSize = minSize;
		Recalculate();
	}

	Vec2F WidgetLayout::GetMinimalSize() const
	{
		return mMinSize;
	}

	void WidgetLayout::SetMinimalWidth(float value)
	{
		mMinSize.x = value;
		Recalculate();
	}

	float WidgetLayout::GetMinimalWidth() const
	{
		return mMinSize.x;
	}

	void WidgetLayout::SetMinimalHeight(float value)
	{
		mMinSize.y = value;
		Recalculate();
	}

	float WidgetLayout::GetMinimalHeight() const
	{
		return mMinSize.y;
	}

	void WidgetLayout::SetMaximalSize(const Vec2F& maxSize)
	{
		mMaxSize = maxSize;
		Recalculate();
	}

	Vec2F WidgetLayout::GetMaximalSize() const
	{
		return mMaxSize;
	}

	void WidgetLayout::SetMaximalWidth(float value)
	{
		mMaxSize.x = value;
		Recalculate();
	}

	float WidgetLayout::GetMaximalWidth() const
	{
		return mMaxSize.x;
	}

	void WidgetLayout::SetMaximalHeight(float value)
	{
		mMaxSize.y = value;
		Recalculate();
	}

	float WidgetLayout::GetMaximalHeight() const
	{
		return mMaxSize.y;
	}

	void WidgetLayout::SetFittingByChildren(bool fit)
	{
		mFitByChilds = fit;
		Recalculate();
	}

	bool WidgetLayout::IsFittingByChildren() const
	{
		return mFitByChilds;
	}

	void WidgetLayout::SetWeight(const Vec2F& weight)
	{
		mWeight = weight;
		Recalculate();
	}

	Vec2F WidgetLayout::GetWeight() const
	{
		return mWeight;
	}

	void WidgetLayout::SetWidthWeight(float widthWeigth)
	{
		mWeight.x = widthWeigth;
		Recalculate();
	}

	float WidgetLayout::GetWidthWeight()
	{
		return mWeight.x;
	}

	void WidgetLayout::SetHeightWeight(float heigthWeigth)
	{
		mWeight.y = heigthWeigth;
		Recalculate();
	}

	float WidgetLayout::GetHeightWeight()
	{
		return mWeight.y;
	}

	void WidgetLayout::Recalculate()
	{
		if (mDrivenByParent)
		{
			if (mOwner->mParent)
				mOwner->mParent->layout.UpdateRect();
		}
		else
			UpdateRect();
	}

	void WidgetLayout::UpdateRect()
	{
		RectF lastAbsRect = mAbsoluteRect;

		Vec2F parentSize, parentPos;
		if (mOwner->mParent)
		{
			parentSize = mOwner->mParent->layout.mAbsoluteRect.Size();
			parentPos = mOwner->mParent->layout.mAbsoluteRect.LeftBottom();
		}

		mLocalRect.left   = parentSize.x*mAnchorMin.x + mOffsetMin.x;
		mLocalRect.right  = parentSize.x*mAnchorMax.x + mOffsetMax.x;
		mLocalRect.bottom = parentSize.y*mAnchorMin.y + mOffsetMin.y;
		mLocalRect.top    = parentSize.y*mAnchorMax.y + mOffsetMax.y;

		Vec2F resSize = mLocalRect.Size();
		Vec2F clampSize(Math::Clamp(resSize.x, mMinSize.x, mMaxSize.x),
						Math::Clamp(resSize.y, mMinSize.y, mMaxSize.y));
		Vec2F szDelta = clampSize - resSize;

		mLocalRect.left   -= szDelta.x*mPivot.x;
		mLocalRect.right  += szDelta.x*(1.0f - mPivot.x);
		mLocalRect.bottom -= szDelta.y*mPivot.y;
		mLocalRect.top    += szDelta.y*(1.0f - mPivot.y);

		mAbsoluteRect = mLocalRect + parentPos;

		mOwner->UpdateLayersLayouts();
		mOwner->OnLayoutUpdated();

		for (auto child : mOwner->mChilds)
			child->layout.UpdateRect();
	}

	void WidgetLayout::InitializeProperties()
	{
		INITIALIZE_PROPERTY(WidgetLayout, pivot, SetPivot, GetPivot);
		INITIALIZE_PROPERTY(WidgetLayout, position, SetPosition, GetPosition);
		INITIALIZE_PROPERTY(WidgetLayout, size, SetSize, GetSize);
		INITIALIZE_PROPERTY(WidgetLayout, width, SetWidth, GetWidth);
		INITIALIZE_PROPERTY(WidgetLayout, height, SetHeight, GetHeight);
		INITIALIZE_PROPERTY(WidgetLayout, absPosition, SetAbsolutePosition, GetAbsolutePosition);
		INITIALIZE_PROPERTY(WidgetLayout, absLeft, SetAbsoluteLeft, GetAbsoluteLeft);
		INITIALIZE_PROPERTY(WidgetLayout, absRight, SetAbsoluteRight, GetAbsoluteRight);
		INITIALIZE_PROPERTY(WidgetLayout, absBottom, SetAbsoluteBottom, GetAbsoluteBottom);
		INITIALIZE_PROPERTY(WidgetLayout, absTop, SetAbsoluteTop, GetAbsoluteTop);
		INITIALIZE_PROPERTY(WidgetLayout, pivotX, SetPivotX, GetPivotX);
		INITIALIZE_PROPERTY(WidgetLayout, pivotY, SetPivotY, GetPivotY);
		INITIALIZE_PROPERTY(WidgetLayout, fitByChildren, SetFittingByChildren, IsFittingByChildren);
		INITIALIZE_PROPERTY(WidgetLayout, anchorMin, SetAnchorMin, GetAnchorMin);
		INITIALIZE_PROPERTY(WidgetLayout, anchorMax, SetAnchorMax, GetAnchorMax);
		INITIALIZE_PROPERTY(WidgetLayout, offsetMin, SetOffsetMin, GetOffsetMin);
		INITIALIZE_PROPERTY(WidgetLayout, offsetMax, SetOffsetMax, GetOffsetMax);
		INITIALIZE_PROPERTY(WidgetLayout, anchorLeft, SetAnchorLeft, GetAnchorLeft);
		INITIALIZE_PROPERTY(WidgetLayout, anchorRight, SetAnchorRight, GetAnchorRight);
		INITIALIZE_PROPERTY(WidgetLayout, anchorBottom, SetAnchorBottom, GetAnchorBottom);
		INITIALIZE_PROPERTY(WidgetLayout, anchorTop, SetAnchorTop, GetAnchorTop);
		INITIALIZE_PROPERTY(WidgetLayout, offsetLeft, SetOffsetLeft, GetOffsetLeft);
		INITIALIZE_PROPERTY(WidgetLayout, offsetRight, SetOffsetRight, GetOffsetRight);
		INITIALIZE_PROPERTY(WidgetLayout, offsetBottom, SetOffsetBottom, GetOffsetBottom);
		INITIALIZE_PROPERTY(WidgetLayout, offsetTop, SetOffsetTop, GetOffsetTop);
		INITIALIZE_PROPERTY(WidgetLayout, minSize, SetMinimalSize, GetMinimalSize);
		INITIALIZE_PROPERTY(WidgetLayout, minWidth, SetMinimalWidth, GetMinimalWidth);
		INITIALIZE_PROPERTY(WidgetLayout, minHeight, SetMinimalHeight, GetMinimalHeight);
		INITIALIZE_PROPERTY(WidgetLayout, maxSize, SetMaximalSize, GetMaximalSize);
		INITIALIZE_PROPERTY(WidgetLayout, maxWidth, SetMaximalWidth, GetMaximalWidth);
		INITIALIZE_PROPERTY(WidgetLayout, maxHeight, SetMaximalHeight, GetMaximalHeight);
		INITIALIZE_PROPERTY(WidgetLayout, weight, SetWeight, GetWeight);
		INITIALIZE_PROPERTY(WidgetLayout, widthWeight, SetWidthWeight, GetWidthWeight);
		INITIALIZE_PROPERTY(WidgetLayout, heigthWeight, SetHeightWeight, GetHeightWeight);
	}
}