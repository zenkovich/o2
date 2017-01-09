#include "HorizontalLayout.h"

namespace o2
{
	UIHorizontalLayout::UIHorizontalLayout(): UIWidget()
	{
		InitializeProperties();
		UpdateLayout();
	}

	UIHorizontalLayout::UIHorizontalLayout(const UIHorizontalLayout& other):
		mBaseCorner(other.mBaseCorner), mSpacing(other.mSpacing), mBorder(other.mBorder), mExpandWidth(other.mExpandWidth),
		mExpandHeight(other.mExpandHeight), UIWidget(other), mFitByChildren(other.mFitByChildren)
	{
		RetargetStatesAnimations();
		UpdateLayout();
		InitializeProperties();
	}

	UIHorizontalLayout::~UIHorizontalLayout()
	{}

	UIHorizontalLayout& UIHorizontalLayout::operator=(const UIHorizontalLayout& other)
	{
		mBaseCorner   = other.mBaseCorner;
		mSpacing      = other.mSpacing;
		mBorder       = other.mBorder;
		mExpandWidth  = other.mExpandWidth;
		mExpandHeight = other.mExpandHeight;

		UIWidget::operator=(other);

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIHorizontalLayout::SetBaseCorner(BaseCorner baseCorner)
	{
		mBaseCorner = baseCorner;
		RearrangeChilds();
	}

	BaseCorner UIHorizontalLayout::GetBaseCorner() const
	{
		return mBaseCorner;
	}

	void UIHorizontalLayout::SetSpacing(float spacing)
	{
		mSpacing = spacing;
		RearrangeChilds();
	}

	float UIHorizontalLayout::GetSpacing() const
	{
		return mSpacing;
	}

	void UIHorizontalLayout::SetBorder(const RectF& border)
	{
		mBorder = border;
		RearrangeChilds();
	}

	RectF UIHorizontalLayout::GetBorder() const
	{
		return mBorder;
	}

	void UIHorizontalLayout::SetBorderLeft(float value)
	{
		mBorder.left = value;
		RearrangeChilds();
	}

	float UIHorizontalLayout::GetBorderLeft() const
	{
		return mBorder.left;
	}

	void UIHorizontalLayout::SetBorderRight(float value)
	{
		mBorder.right = value;
		RearrangeChilds();
	}

	float UIHorizontalLayout::GetBorderRight() const
	{
		return mBorder.right;
	}

	void UIHorizontalLayout::SetBorderTop(float value)
	{
		mBorder.top = value;
		RearrangeChilds();
	}

	float UIHorizontalLayout::GetBorderTop() const
	{
		return mBorder.top;
	}

	void UIHorizontalLayout::SetBorderBottom(float value)
	{
		mBorder.bottom = value;
		RearrangeChilds();
	}

	float UIHorizontalLayout::GetBorderBottom() const
	{
		return mBorder.bottom;
	}

	void UIHorizontalLayout::SetWidthExpand(bool expand)
	{
		mExpandWidth = expand;
		RearrangeChilds();
	}

	bool UIHorizontalLayout::IsWidthExpand() const
	{
		return mExpandWidth;
	}

	void UIHorizontalLayout::SetHeightExpand(bool expand)
	{
		mExpandHeight = expand;
		RearrangeChilds();
	}

	bool UIHorizontalLayout::IsHeightExpand() const
	{
		return mExpandHeight;
	}

	void UIHorizontalLayout::SetFitByChildren(bool fit)
	{
		mFitByChildren = fit;
		RearrangeChilds();
	}

	bool UIHorizontalLayout::IsFittingByChildren() const
	{
		return mFitByChildren;
	}

	void UIHorizontalLayout::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (CheckIsLayoutDrivenByParent(forcible))
			return;

		if (mFitByChildren)
			ExpandSizeByChilds();

		RecalculateAbsRect();
		UpdateLayersLayouts();

		if (withChildren)
			RearrangeChilds();
	}

	float UIHorizontalLayout::GetMinWidthWithChildren() const
	{
		if (!mFitByChildren)
			return UIWidget::GetMinWidthWithChildren();

		float res = mBorder.left + mBorder.right + Math::Max(mChilds.Count() - 1, 0)*mSpacing;
		for (auto child : mChilds)
		{
			if (!child->mFullyDisabled)
				res += child->GetMinWidthWithChildren();
		}

		return res;
	}

	void UIHorizontalLayout::OnChildAdded(UIWidget* child)
	{
		child->layout.mDrivenByParent = true;
	}

	void UIHorizontalLayout::OnChildRemoved(UIWidget* child)
	{
		child->layout.mDrivenByParent = false;
	}

	void UIHorizontalLayout::RearrangeChilds()
	{
		UpdateLayoutParametres();

		switch (mBaseCorner)
		{
			case BaseCorner::LeftBottom:
			case BaseCorner::Left:
			case BaseCorner::LeftTop:
			ArrangeFromLeftToRight();
			break;

			case BaseCorner::RightBottom:
			case BaseCorner::Right:
			case BaseCorner::RightTop:
			ArrangeFromRightToLeft();
			break;

			case BaseCorner::Bottom:
			case BaseCorner::Center:
			case BaseCorner::Top:
			ArrangeFromCenter();
			break;
		}

	}

	void UIHorizontalLayout::UpdateLayoutParametres()
	{
		layout.mWeight.x = mChilds.Sum<float>([&](UIWidget* child) { return child->layout.GetWidthWeight(); });
		layout.mMinSize.x = mChilds.Sum<float>([&](UIWidget* child) { return child->layout.GetMinimalWidth(); });
		layout.mMinSize.x += mBorder.left + mBorder.right;
	}

	void UIHorizontalLayout::ArrangeFromCenter()
	{
		if (mExpandWidth)
		{
			float availableWidth = mChildsAbsRect.Width() - mBorder.left - mBorder.right;
			float totalWidth = availableWidth;
			float position = -totalWidth*0.5f;
			auto widths = CalculateExpandedWidths();

			int i = 0;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMin.x = position;
				position += widths[i++];

				child->layout.mOffsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.5f);
				child->UpdateLayout(true);
			}
		}
		else
		{
			float totalWidth = mChilds.Sum<float>([&](UIWidget* child) { return child->GetMinWidthWithChildren(); });
			totalWidth += (mChilds.Count() - 1)*mSpacing;
			float position = -totalWidth*0.5f;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMin.x = position;
				position += Math::Abs(Math::Max(child->layout.mMinSize.x, child->GetMinWidthWithChildren()));

				child->layout.mOffsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.5f);
				child->UpdateLayout(true);
			}
		}
	}

	void UIHorizontalLayout::ArrangeFromLeftToRight()
	{
		if (mExpandWidth)
		{
			float position = mBorder.left;
			auto widths = CalculateExpandedWidths();

			int i = 0;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMin.x = position;
				position += widths[i++];

				child->layout.mOffsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.0f);
				child->UpdateLayout(true);
			}
		}
		else
		{
			float position = mBorder.left;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMin.x = position;
				position += Math::Abs(Math::Max(child->layout.mMinSize.x, child->GetMinWidthWithChildren()));

				child->layout.mOffsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.0f);
				child->UpdateLayout(true);
			}
		}
	}

	void UIHorizontalLayout::ArrangeFromRightToLeft()
	{
		if (mExpandWidth)
		{
			float position = mBorder.right;
			auto widths = CalculateExpandedWidths();

			int i = 0;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMax.x = -position;
				position += widths[i++];

				child->layout.mOffsetMin.x = -position;
				position += mSpacing;

				AlignWidgetByHeight(child, 1.0f);
				child->UpdateLayout(true);
			}
		}
		else
		{
			float position = mBorder.right;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMax.x = -position;
				position += Math::Abs(Math::Max(child->layout.mMinSize.x, child->GetMinWidthWithChildren()));

				child->layout.mOffsetMin.x = -position;
				position += mSpacing;

				AlignWidgetByHeight(child, 1.0f);
				child->UpdateLayout(true);
			}
		}
	}

	void UIHorizontalLayout::AlignWidgetByHeight(UIWidget* child, float widthAnchor)
	{
		if (mExpandHeight)
		{
			child->layout.mAnchorMin = Vec2F(widthAnchor, 0);
			child->layout.mAnchorMax = Vec2F(widthAnchor, 1);
			child->layout.mOffsetMin.y = mBorder.bottom;
			child->layout.mOffsetMax.y = -mBorder.top;
		}
		else
		{
			float height = child->layout.GetHeight();
			if (mBaseCorner == BaseCorner::Bottom || mBaseCorner == BaseCorner::LeftBottom || mBaseCorner == BaseCorner::RightBottom)
			{
				child->layout.mAnchorMin = Vec2F(widthAnchor, 0);
				child->layout.mAnchorMax = Vec2F(widthAnchor, 0);
				child->layout.mOffsetMin.y = mBorder.bottom;
				child->layout.mOffsetMax.y = mBorder.bottom + height;
			}
			if (mBaseCorner == BaseCorner::Center || mBaseCorner == BaseCorner::Left || mBaseCorner == BaseCorner::Right)
			{
				child->layout.mAnchorMin = Vec2F(widthAnchor, 0.5f);
				child->layout.mAnchorMax = Vec2F(widthAnchor, 0.5f);
				child->layout.mOffsetMin.y = -height*0.5f;
				child->layout.mOffsetMax.y = height*0.5f;
			}
			if (mBaseCorner == BaseCorner::Top || mBaseCorner == BaseCorner::LeftTop || mBaseCorner == BaseCorner::RightTop)
			{
				child->layout.mAnchorMin = Vec2F(widthAnchor, 1);
				child->layout.mAnchorMax = Vec2F(widthAnchor, 1);
				child->layout.mOffsetMin.y = -mBorder.top - height;
				child->layout.mOffsetMax.y = -mBorder.top;
			}
		}
	}

	void UIHorizontalLayout::ExpandSizeByChilds()
	{
		const static Vec2F relativePivots[] ={
			Vec2F(0.0f, 0.5f), // Left
			Vec2F(1.0f, 0.5f), // Right
			Vec2F(0.5f, 1.0f), // Top
			Vec2F(0.5f, 0.0f), // Bottom
			Vec2F(0.5f, 0.5f), // Center
			Vec2F(0.0f, 0.0f), // LeftBottom
			Vec2F(0.0f, 1.0f), // LeftTop
			Vec2F(1.0f, 0.0f), // RightBottom
			Vec2F(1.0f, 1.0f)  // RightTop
		};

		Vec2F relativePivot = relativePivots[(int)mBaseCorner];
		Vec2F size(GetMinWidthWithChildren(), GetMinHeightWithChildren());

		Vec2F parentSize = mParent ? mParent->layout.mAbsoluteRect.Size() : Vec2F();
		Vec2F szDelta = size - (layout.mOffsetMax - layout.mOffsetMin + (layout.mAnchorMax - layout.mAnchorMin)*parentSize);

		if (mExpandWidth)
			szDelta.x = 0;

		if (mExpandHeight)
			szDelta.y = 0;

		layout.mOffsetMax += szDelta*(Vec2F::One() - relativePivot);
		layout.mOffsetMin -= szDelta*relativePivot;
	}

	Vector<float> UIHorizontalLayout::CalculateExpandedWidths()
	{
		int ichildCount = mChilds.Count();
		float childCount = (float)ichildCount;
		float availableWidth = mChildsAbsRect.Width() - mBorder.left - mBorder.right;
		float minWidthSum = mChilds.Sum<float>([&](UIWidget* child) { return child->layout.GetMinimalWidth(); });
		float expandValue = Math::Max(availableWidth - minWidthSum - (childCount - 1.0f)*mSpacing, 0.0f);

		Vector<float> widths(ichildCount + 1);
		mChilds.ForEach([&](auto child) { widths.Add(child->layout.GetMinimalWidth()); });

		while (expandValue > 0)
		{
			float minSz = FLT_MAX;
			float maxSzWeight = 0.0f;
			float minSzWeightsSum = 0.0f;
			Vector<int> minSzChilds(ichildCount + 1);

			for (int i = 0; i < widths.Count(); i++)
			{
				float w = widths[i];
				if (Math::Equals(w, minSz))
				{
					float wweight = mChilds[i]->layout.GetWidthWeight();
					maxSzWeight = Math::Max(maxSzWeight, wweight);
					minSzWeightsSum += wweight;
					minSzChilds.Add(i);
				}

				if (w < minSz)
				{
					minSz = w;
					minSzChilds.Clear();
					minSzChilds.Add(i);
					float wweight = mChilds[i]->layout.GetWidthWeight();
					maxSzWeight = wweight;
					minSzWeightsSum = wweight;
				}
			}

			float needsDelta = 0.0f;
			if (minSzChilds.Count() == ichildCount)
			{
				needsDelta = expandValue;
			}
			else
			{
				float nearestToMinSz = FLT_MAX;

				for (int i = 0; i < widths.Count(); i++)
				{
					if (widths[i] > minSz && widths[i] < nearestToMinSz)
						nearestToMinSz = widths[i];
				}

				float deffToNearest = nearestToMinSz - minSz;
				needsDelta = Math::Min((nearestToMinSz - minSz)*minSzWeightsSum/maxSzWeight, expandValue);
			}

			float expValueByWeight = needsDelta/minSzWeightsSum;
			minSzChilds.ForEach([&](int idx) { widths[idx] += expValueByWeight*mChilds[idx]->layout.GetWidthWeight(); });

			expandValue -= needsDelta;
		}

		return widths;
	}

	void UIHorizontalLayout::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIHorizontalLayout, baseCorner, SetBaseCorner, GetBaseCorner);
		INITIALIZE_PROPERTY(UIHorizontalLayout, spacing, SetSpacing, GetSpacing);
		INITIALIZE_PROPERTY(UIHorizontalLayout, border, SetBorder, GetBorder);
		INITIALIZE_PROPERTY(UIHorizontalLayout, borderLeft, SetBorderLeft, GetBorderLeft);
		INITIALIZE_PROPERTY(UIHorizontalLayout, borderRight, SetBorderRight, GetBorderRight);
		INITIALIZE_PROPERTY(UIHorizontalLayout, borderTop, SetBorderTop, GetBorderTop);
		INITIALIZE_PROPERTY(UIHorizontalLayout, borderBottom, SetBorderBottom, GetBorderBottom);
		INITIALIZE_PROPERTY(UIHorizontalLayout, expandWidth, SetWidthExpand, IsWidthExpand);
		INITIALIZE_PROPERTY(UIHorizontalLayout, expandHeight, SetHeightExpand, IsHeightExpand);
		INITIALIZE_PROPERTY(UIHorizontalLayout, fitByChildren, SetFitByChildren, IsFittingByChildren);
	}
}
 
CLASS_META(o2::UIHorizontalLayout)
{
	BASE_CLASS(o2::UIWidget);

	PUBLIC_FIELD(baseCorner);
	PUBLIC_FIELD(spacing);
	PUBLIC_FIELD(border);
	PUBLIC_FIELD(borderLeft);
	PUBLIC_FIELD(borderRight);
	PUBLIC_FIELD(borderTop);
	PUBLIC_FIELD(borderBottom);
	PUBLIC_FIELD(expandWidth);
	PUBLIC_FIELD(expandHeight);
	PUBLIC_FIELD(fitByChildren);
	PROTECTED_FIELD(mBaseCorner).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSpacing).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mBorder).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mExpandWidth).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mExpandHeight).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mFitByChildren).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(void, SetBaseCorner, BaseCorner);
	PUBLIC_FUNCTION(BaseCorner, GetBaseCorner);
	PUBLIC_FUNCTION(void, SetSpacing, float);
	PUBLIC_FUNCTION(float, GetSpacing);
	PUBLIC_FUNCTION(void, SetBorder, const RectF&);
	PUBLIC_FUNCTION(RectF, GetBorder);
	PUBLIC_FUNCTION(void, SetBorderLeft, float);
	PUBLIC_FUNCTION(float, GetBorderLeft);
	PUBLIC_FUNCTION(void, SetBorderRight, float);
	PUBLIC_FUNCTION(float, GetBorderRight);
	PUBLIC_FUNCTION(void, SetBorderTop, float);
	PUBLIC_FUNCTION(float, GetBorderTop);
	PUBLIC_FUNCTION(void, SetBorderBottom, float);
	PUBLIC_FUNCTION(float, GetBorderBottom);
	PUBLIC_FUNCTION(void, SetWidthExpand, bool);
	PUBLIC_FUNCTION(bool, IsWidthExpand);
	PUBLIC_FUNCTION(void, SetHeightExpand, bool);
	PUBLIC_FUNCTION(bool, IsHeightExpand);
	PUBLIC_FUNCTION(void, SetFitByChildren, bool);
	PUBLIC_FUNCTION(bool, IsFittingByChildren);
	PUBLIC_FUNCTION(void, UpdateLayout, bool, bool);
	PROTECTED_FUNCTION(float, GetMinWidthWithChildren);
	PROTECTED_FUNCTION(void, OnChildAdded, UIWidget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, UIWidget*);
	PROTECTED_FUNCTION(void, RearrangeChilds);
	PROTECTED_FUNCTION(void, ArrangeFromLeftToRight);
	PROTECTED_FUNCTION(void, ArrangeFromRightToLeft);
	PROTECTED_FUNCTION(void, ArrangeFromCenter);
	PROTECTED_FUNCTION(void, ExpandSizeByChilds);
	PROTECTED_FUNCTION(Vector<float>, CalculateExpandedWidths);
	PROTECTED_FUNCTION(void, AlignWidgetByHeight, UIWidget*, float);
	PROTECTED_FUNCTION(void, UpdateLayoutParametres);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
 