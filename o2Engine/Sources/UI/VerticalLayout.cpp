#include "VerticalLayout.h"

namespace o2
{
	UIVerticalLayout::UIVerticalLayout(): UIWidget()
	{
		InitializeProperties();
		UpdateLayout();
	}

	UIVerticalLayout::UIVerticalLayout(const UIVerticalLayout& other):
		mBaseCorner(other.mBaseCorner), mSpacing(other.mSpacing), mBorder(other.mBorder), mExpandWidth(other.mExpandWidth),
		mExpandHeight(other.mExpandHeight), UIWidget(other), mFitByChildren(other.mFitByChildren)
	{
		RetargetStatesAnimations();
		UpdateLayout();
		InitializeProperties();
	}

	UIVerticalLayout::~UIVerticalLayout()
	{}

	UIVerticalLayout& UIVerticalLayout::operator=(const UIVerticalLayout& other)
	{
		mBaseCorner    = other.mBaseCorner;
		mSpacing       = other.mSpacing;
		mBorder        = other.mBorder;
		mExpandWidth   = other.mExpandWidth;
		mExpandHeight  = other.mExpandHeight;
		mFitByChildren = other.mFitByChildren;

		UIWidget::operator=(other);

		RetargetStatesAnimations();
		UpdateLayout();

		return *this;
	}

	void UIVerticalLayout::SetBaseCorner(BaseCorner baseCorner)
	{
		mBaseCorner = baseCorner;
		RearrangeChilds();
	}

	BaseCorner UIVerticalLayout::GetBaseCorner() const
	{
		return mBaseCorner;
	}

	void UIVerticalLayout::SetSpacing(float spacing)
	{
		mSpacing = spacing;
		RearrangeChilds();
	}

	float UIVerticalLayout::GetSpacing() const
	{
		return mSpacing;
	}

	void UIVerticalLayout::SetBorder(const RectF& border)
	{
		mBorder = border;
		RearrangeChilds();
	}

	RectF UIVerticalLayout::GetBorder() const
	{
		return mBorder;
	}

	void UIVerticalLayout::SetBorderLeft(float value)
	{
		mBorder.left = value;
		RearrangeChilds();
	}

	float UIVerticalLayout::GetBorderLeft() const
	{
		return mBorder.left;
	}

	void UIVerticalLayout::SetBorderRight(float value)
	{
		mBorder.right = value;
		RearrangeChilds();
	}

	float UIVerticalLayout::GetBorderRight() const
	{
		return mBorder.right;
	}

	void UIVerticalLayout::SetBorderTop(float value)
	{
		mBorder.top = value;
		RearrangeChilds();
	}

	float UIVerticalLayout::GetBorderTop() const
	{
		return mBorder.top;
	}

	void UIVerticalLayout::SetBorderBottom(float value)
	{
		mBorder.bottom = value;
		RearrangeChilds();
	}

	float UIVerticalLayout::GetBorderBottom() const
	{
		return mBorder.bottom;
	}

	void UIVerticalLayout::SetWidthExpand(bool expand)
	{
		mExpandWidth = expand;
		RearrangeChilds();
	}

	bool UIVerticalLayout::IsWidthExpand() const
	{
		return mExpandWidth;
	}

	void UIVerticalLayout::SetHeightExpand(bool expand)
	{
		mExpandHeight = expand;
		RearrangeChilds();
	}

	bool UIVerticalLayout::IsHeightExpand() const
	{
		return mExpandHeight;
	}

	void UIVerticalLayout::SetFitByChildren(bool fit)
	{
		mFitByChildren = fit;
		RearrangeChilds();
	}

	bool UIVerticalLayout::IsFittingByChildren() const
	{
		return mFitByChildren;
	}

	void UIVerticalLayout::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		if (CheckIsLayoutDrivenByParent(forcible))
			return;

		UpdateLayoutParametres();

		if (mFitByChildren)
			ExpandSizeByChilds();

		RecalculateAbsRect();
		UpdateLayersLayouts();

		if (withChildren)
			RearrangeChilds();
	}

	float UIVerticalLayout::GetMinHeightWithChildren() const
	{
		if (!mFitByChildren)
			return UIWidget::GetMinHeightWithChildren();

		float res = mBorder.top + mBorder.bottom + Math::Max(mChilds.Count() - 1, 0)*mSpacing;
		for (auto child : mChilds)
		{
			if (!child->mFullyDisabled)
				res += child->GetMinHeightWithChildren();
		}

		return res;
	}

	void UIVerticalLayout::OnChildAdded(UIWidget* child)
	{
		child->layout.mDrivenByParent = true;
	}

	void UIVerticalLayout::OnChildRemoved(UIWidget* child)
	{
		child->layout.mDrivenByParent = false;
	}

	void UIVerticalLayout::RearrangeChilds()
	{
		switch (mBaseCorner)
		{
			case BaseCorner::LeftTop:
			case BaseCorner::Top:
			case BaseCorner::RightTop:
			ArrangeFromTopToBottom();
			break;

			case BaseCorner::Left:
			case BaseCorner::Center:
			case BaseCorner::Right:
			ArrangeFromCenter();
			break;

			case BaseCorner::LeftBottom:
			case BaseCorner::Bottom:
			case BaseCorner::RightBottom:
			ArrangeFromBottomToTop();
			break;
		}

	}

	void UIVerticalLayout::UpdateLayoutParametres()
	{
		layout.mWeight.y = mChilds.Sum<float>([&](UIWidget* child) { return child->layout.GetHeightWeight(); });
		layout.mMinSize.y = mChilds.Sum<float>([&](UIWidget* child) { return child->layout.GetMinimalHeight(); });
		layout.mMinSize.y += mBorder.bottom + mBorder.top;
	}

	void UIVerticalLayout::ArrangeFromCenter()
	{
		if (mExpandHeight)
		{
			float availableHeight = mChildsAbsRect.Height() - mBorder.bottom - mBorder.top;
			float totalHeight = availableHeight;
			float position = -totalHeight*0.5f;
			auto heights = CalculateExpandedHeights();

			int i = 0;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMin.y = position;
				position += heights[i++];

				child->layout.mOffsetMax.y = position;
				position += mSpacing;

				AlignWidgetByWidth(child, 0.5f);
				child->UpdateLayout(true);
			}
		}
		else
		{
			float totalHeight = mChilds.Sum<float>([&](UIWidget* child) { return child->GetMinHeightWithChildren(); });
			totalHeight += (mChilds.Count() - 1)*mSpacing;
			float position = -totalHeight*0.5f;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMin.y = position;
				position += Math::Abs(Math::Max(child->layout.mMinSize.y, child->GetMinHeightWithChildren()));

				child->layout.mOffsetMax.y = position;
				position += mSpacing;

				AlignWidgetByWidth(child, 0.5f);
				child->UpdateLayout(true);
			}
		}
	}

	void UIVerticalLayout::ArrangeFromBottomToTop()
	{
		if (mExpandHeight)
		{
			float position = mBorder.bottom;
			auto heights = CalculateExpandedHeights();

			int i = 0;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMin.y = position;
				position += heights[i++];

				child->layout.mOffsetMax.y = position;
				position += mSpacing;

				AlignWidgetByWidth(child, 0.0f);
				child->UpdateLayout(true);
			}
		}
		else
		{
			float position = mBorder.bottom;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMin.y = position;
				position += Math::Abs(Math::Max(child->layout.mMinSize.y, child->GetMinHeightWithChildren()));

				child->layout.mOffsetMax.y = position;
				position += mSpacing;

				AlignWidgetByWidth(child, 0.0f);
				child->UpdateLayout(true);
			}
		}
	}

	void UIVerticalLayout::ArrangeFromTopToBottom()
	{
		if (mExpandHeight)
		{
			float position = mBorder.top;
			auto heights = CalculateExpandedHeights();

			int i = 0;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMax.y = -position;
				position += heights[i++];

				child->layout.mOffsetMin.y = -position;
				position += mSpacing;

				AlignWidgetByWidth(child, 1.0f);
				child->UpdateLayout(true);
			}
		}
		else
		{
			float position = mBorder.top;
			for (auto child : mChilds)
			{
				child->layout.mOffsetMax.y = -position;
				position += Math::Abs(Math::Max(child->layout.mMinSize.y, child->GetMinHeightWithChildren()));

				child->layout.mOffsetMin.y = -position;
				position += mSpacing;

				AlignWidgetByWidth(child, 1.0f);
				child->UpdateLayout(true);
			}
		}
	}

	void UIVerticalLayout::AlignWidgetByWidth(UIWidget* child, float heightAnchor)
	{
		if (mExpandWidth)
		{
			child->layout.mAnchorMin = Vec2F(0, heightAnchor);
			child->layout.mAnchorMax = Vec2F(1, heightAnchor);
			child->layout.mOffsetMin.x = mBorder.left;
			child->layout.mOffsetMax.x = -mBorder.right;
		}
		else
		{
			float width = child->layout.GetWidth();
			if (mBaseCorner == BaseCorner::LeftBottom || mBaseCorner == BaseCorner::Left || mBaseCorner == BaseCorner::LeftTop)
			{
				child->layout.mAnchorMin = Vec2F(0, heightAnchor);
				child->layout.mAnchorMax = Vec2F(0, heightAnchor);
				child->layout.mOffsetMin.x = mBorder.left;
				child->layout.mOffsetMax.x = mBorder.left + width;
			}
			if (mBaseCorner == BaseCorner::Bottom || mBaseCorner == BaseCorner::Center || mBaseCorner == BaseCorner::Top)
			{
				child->layout.mAnchorMin = Vec2F(0.5f, heightAnchor);
				child->layout.mAnchorMax = Vec2F(0.5f, heightAnchor);
				child->layout.mOffsetMin.x = -width*0.5f;
				child->layout.mOffsetMax.x = width*0.5f;
			}
			if (mBaseCorner == BaseCorner::RightBottom || mBaseCorner == BaseCorner::Right || mBaseCorner == BaseCorner::RightTop)
			{
				child->layout.mAnchorMin = Vec2F(1, heightAnchor);
				child->layout.mAnchorMax = Vec2F(1, heightAnchor);
				child->layout.mOffsetMin.x = -mBorder.right - width;
				child->layout.mOffsetMax.x = -mBorder.right;
			}
		}
	}

	void UIVerticalLayout::ExpandSizeByChilds()
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

	Vector<float> UIVerticalLayout::CalculateExpandedHeights()
	{
		int ichildCount = mChilds.Count();
		float childCount = (float)ichildCount;
		float availableHeight = mChildsAbsRect.Height() - mBorder.bottom - mBorder.top;
		float minHeightSum = mChilds.Sum<float>([&](UIWidget* child) { return child->layout.GetMinimalHeight(); });
		float expandValue = Math::Max(availableHeight - minHeightSum - (childCount - 1.0f)*mSpacing, 0.0f);

		Vector<float> heights(ichildCount + 1);
		mChilds.ForEach([&](auto child) { heights.Add(child->layout.GetMinimalHeight()); });

		while (expandValue > 0)
		{
			float minSz = FLT_MAX;
			float maxSzWeight = 0.0f;
			float minSzWeightsSum = 0.0f;
			Vector<int> minSzChilds(ichildCount + 1);

			for (int i = 0; i < heights.Count(); i++)
			{
				float h = heights[i];
				if (Math::Equals(h, minSz))
				{
					float wweight = mChilds[i]->layout.GetHeightWeight();
					maxSzWeight = Math::Max(maxSzWeight, wweight);
					minSzWeightsSum += wweight;
					minSzChilds.Add(i);
				}

				if (h < minSz)
				{
					minSz = h;
					minSzChilds.Clear();
					minSzChilds.Add(i);
					float wweight = mChilds[i]->layout.GetHeightWeight();
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

				for (int i = 0; i < heights.Count(); i++)
				{
					if (heights[i] > minSz && heights[i] < nearestToMinSz)
						nearestToMinSz = heights[i];
				}

				float deffToNearest = nearestToMinSz - minSz;
				needsDelta = Math::Min((nearestToMinSz - minSz)*minSzWeightsSum/maxSzWeight, expandValue);
			}

			float expValueByWeight = needsDelta/minSzWeightsSum;
			minSzChilds.ForEach([&](int idx) { heights[idx] += expValueByWeight*mChilds[idx]->layout.GetHeightWeight(); });

			expandValue -= needsDelta;
		}

		return heights;
	}

	void UIVerticalLayout::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIVerticalLayout, baseCorner, SetBaseCorner, GetBaseCorner);
		INITIALIZE_PROPERTY(UIVerticalLayout, spacing, SetSpacing, GetSpacing);
		INITIALIZE_PROPERTY(UIVerticalLayout, border, SetBorder, GetBorder);
		INITIALIZE_PROPERTY(UIVerticalLayout, borderLeft, SetBorderLeft, GetBorderLeft);
		INITIALIZE_PROPERTY(UIVerticalLayout, borderRight, SetBorderRight, GetBorderRight);
		INITIALIZE_PROPERTY(UIVerticalLayout, borderTop, SetBorderTop, GetBorderTop);
		INITIALIZE_PROPERTY(UIVerticalLayout, borderBottom, SetBorderBottom, GetBorderBottom);
		INITIALIZE_PROPERTY(UIVerticalLayout, expandWidth, SetWidthExpand, IsWidthExpand);
		INITIALIZE_PROPERTY(UIVerticalLayout, expandHeight, SetHeightExpand, IsHeightExpand);
		INITIALIZE_PROPERTY(UIVerticalLayout, fitByChildren, SetFitByChildren, IsFittingByChildren);
	}
}
 
CLASS_META(o2::UIVerticalLayout)
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
	PROTECTED_FUNCTION(float, GetMinHeightWithChildren);
	PROTECTED_FUNCTION(void, OnChildAdded, UIWidget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, UIWidget*);
	PROTECTED_FUNCTION(void, RearrangeChilds);
	PROTECTED_FUNCTION(void, ArrangeFromTopToBottom);
	PROTECTED_FUNCTION(void, ArrangeFromBottomToTop);
	PROTECTED_FUNCTION(void, ArrangeFromCenter);
	PROTECTED_FUNCTION(void, ExpandSizeByChilds);
	PROTECTED_FUNCTION(Vector<float>, CalculateExpandedHeights);
	PROTECTED_FUNCTION(void, AlignWidgetByWidth, UIWidget*, float);
	PROTECTED_FUNCTION(void, UpdateLayoutParametres);
	PROTECTED_FUNCTION(void, InitializeProperties);
}
END_META;
 