#include "stdafx.h"
#include "VerticalLayout.h"

#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"

namespace o2
{
	UIVerticalLayout::UIVerticalLayout(): UIWidget()
	{
		SetLayoutDirty();
	}

	UIVerticalLayout::UIVerticalLayout(const UIVerticalLayout& other):
		mBaseCorner(other.mBaseCorner), mSpacing(other.mSpacing), mBorder(other.mBorder), mExpandWidth(other.mExpandWidth),
		mExpandHeight(other.mExpandHeight), UIWidget(other), mFitByChildren(other.mFitByChildren)
	{
		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	UIVerticalLayout::~UIVerticalLayout()
	{}

	UIVerticalLayout& UIVerticalLayout::operator=(const UIVerticalLayout& other)
	{
		CopyData(other);
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

	void UIVerticalLayout::SetBorder(const BorderF& border)
	{
		mBorder = border;
		RearrangeChilds();
	}

	BorderF UIVerticalLayout::GetBorder() const
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

	void UIVerticalLayout::UpdateTransform(bool withChildren /*= true*/)
	{
		UpdateLayoutParametres();

		if (mFitByChildren)
			ExpandSizeByChilds();

		UIWidget::UpdateTransform(withChildren);

		RearrangeChilds();
	}

	void UIVerticalLayout::CopyData(const Actor& otherActor)
	{
		const UIVerticalLayout& other = dynamic_cast<const UIVerticalLayout&>(otherActor);

		mBaseCorner    = other.mBaseCorner;
		mSpacing       = other.mSpacing;
		mBorder        = other.mBorder;
		mExpandWidth   = other.mExpandWidth;
		mExpandHeight  = other.mExpandHeight;
		mFitByChildren = other.mFitByChildren;

		UIWidget::CopyData(other);

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	float UIVerticalLayout::GetMinWidthWithChildren() const
	{
		if (!mFitByChildren)
			return UIWidget::GetMinWidthWithChildren();

		float res = 0;
		for (auto child : mChildWidgets)
		{
			if (!child->mFullyDisabled)
				res = Math::Max(res, child->GetMinWidthWithChildren() + mBorder.left + mBorder.right);
		}

		res = Math::Max(res, layout->mData->minSize.x);

		return res;
	}

	float UIVerticalLayout::GetMinHeightWithChildren() const
	{
		if (!mFitByChildren)
			return UIWidget::GetMinHeightWithChildren();

		float res = mBorder.top + mBorder.bottom + Math::Max(mChildWidgets.Count() - 1, 0)*mSpacing;
		for (auto child : mChildWidgets)
		{
			if (!child->mFullyDisabled)
				res += child->GetMinHeightWithChildren();
		}

		res = Math::Max(res, layout->mData->minSize.y);

		return res;
	}

	float UIVerticalLayout::GetHeightWeightWithChildren() const
	{
		float res = 0;
		for (auto child : mChildWidgets)
		{
			if (!child->mFullyDisabled)
				res += child->GetHeightWeightWithChildren();
		}

		return res;
	}

	void UIVerticalLayout::OnChildAdded(UIWidget* child)
	{
		child->layout->mData->drivenByParent = true;
	}

	void UIVerticalLayout::OnChildRemoved(UIWidget* child)
	{
		child->layout->mData->drivenByParent = false;
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
		layout->mData->weight.y = 0;
		for (auto child : mChildWidgets)
		{
			if (!child->mFullyDisabled)
				layout->mData->weight.y += child->GetHeightWeightWithChildren();
		}

		layout->mCheckMinMaxFunc = &UIWidgetLayout::CheckMinMax;
	}

	void UIVerticalLayout::ArrangeFromCenter()
	{
		if (mExpandHeight)
		{
			float availableHeight = mChildrenWorldRect.Height() - mBorder.bottom - mBorder.top;
			float totalHeight = availableHeight;
			float position = -totalHeight*0.5f;
			auto heights = CalculateExpandedHeights();

			int i = 0;
			for (auto child : mChildWidgets)
			{
				child->layout->mData->offsetMin.y = position;
				position += heights[i++];

				child->layout->mData->offsetMax.y = position;
				position += mSpacing;

				AlignWidgetByWidth(child, 0.5f);
			}
		}
		else
		{
			float totalHeight = mChildWidgets.Sum<float>([&](UIWidget* child) { return child->GetMinHeightWithChildren(); });
			totalHeight += (mChildWidgets.Count() - 1)*mSpacing;
			float position = -totalHeight*0.5f;
			for (auto child : mChildWidgets)
			{
				child->layout->mData->offsetMin.y = position;
				position += Math::Abs(Math::Max(child->layout->mData->minSize.y, child->GetMinHeightWithChildren()));

				child->layout->mData->offsetMax.y = position;
				position += mSpacing;

				AlignWidgetByWidth(child, 0.5f);
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
			for (auto child : mChildWidgets)
			{
				child->layout->mData->offsetMin.y = position;
				position += heights[i++];

				child->layout->mData->offsetMax.y = position;
				position += mSpacing;

				AlignWidgetByWidth(child, 0.0f);
			}
		}
		else
		{
			float position = mBorder.bottom;
			for (auto child : mChildWidgets)
			{
				child->layout->mData->offsetMin.y = position;
				position += Math::Abs(Math::Max(child->layout->mData->minSize.y, child->GetMinHeightWithChildren()));

				child->layout->mData->offsetMax.y = position;
				position += mSpacing;

				AlignWidgetByWidth(child, 0.0f);
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
			for (auto child : mChildWidgets)
			{
				child->layout->mData->offsetMax.y = -position;
				position += heights[i++];

				child->layout->mData->offsetMin.y = -position;
				position += mSpacing;

				AlignWidgetByWidth(child, 1.0f);
			}
		}
		else
		{
			float position = mBorder.top;
			for (auto child : mChildWidgets)
			{
				child->layout->mData->offsetMax.y = -position;
				position += Math::Abs(Math::Max(child->layout->mData->minSize.y, child->GetMinHeightWithChildren()));

				child->layout->mData->offsetMin.y = -position;
				position += mSpacing;

				AlignWidgetByWidth(child, 1.0f);
			}
		}
	}

	void UIVerticalLayout::AlignWidgetByWidth(UIWidget* child, float heightAnchor)
	{
		if (mExpandWidth)
		{
			child->layout->mData->anchorMin = Vec2F(0, heightAnchor);
			child->layout->mData->anchorMax = Vec2F(1, heightAnchor);
			child->layout->mData->offsetMin.x = mBorder.left;
			child->layout->mData->offsetMax.x = -mBorder.right;
		}
		else
		{
			float width = child->layout->GetWidth();
			if (mBaseCorner == BaseCorner::LeftBottom || mBaseCorner == BaseCorner::Left || mBaseCorner == BaseCorner::LeftTop)
			{
				child->layout->mData->anchorMin = Vec2F(0, heightAnchor);
				child->layout->mData->anchorMax = Vec2F(0, heightAnchor);
				child->layout->mData->offsetMin.x = mBorder.left;
				child->layout->mData->offsetMax.x = mBorder.left + width;
			}
			if (mBaseCorner == BaseCorner::Bottom || mBaseCorner == BaseCorner::Center || mBaseCorner == BaseCorner::Top)
			{
				child->layout->mData->anchorMin = Vec2F(0.5f, heightAnchor);
				child->layout->mData->anchorMax = Vec2F(0.5f, heightAnchor);
				child->layout->mData->offsetMin.x = -width*0.5f;
				child->layout->mData->offsetMax.x = width*0.5f;
			}
			if (mBaseCorner == BaseCorner::RightBottom || mBaseCorner == BaseCorner::Right || mBaseCorner == BaseCorner::RightTop)
			{
				child->layout->mData->anchorMin = Vec2F(1, heightAnchor);
				child->layout->mData->anchorMax = Vec2F(1, heightAnchor);
				child->layout->mData->offsetMin.x = -mBorder.right - width;
				child->layout->mData->offsetMax.x = -mBorder.right;
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

		Vec2F parentSize = mParentWidget ? mParentWidget->mChildrenWorldRect.Size() : Vec2F();		
		Vec2F szDelta = size - (layout->mData->offsetMax - layout->mData->offsetMin + (layout->mData->anchorMax - layout->mData->anchorMin)*parentSize);

		if (mExpandWidth)
			szDelta.x = 0;

		if (mExpandHeight)
			szDelta.y = 0;

		layout->mData->offsetMax += szDelta*(Vec2F::One() - relativePivot);
		layout->mData->offsetMin -= szDelta*relativePivot;
	}

	Vector<float> UIVerticalLayout::CalculateExpandedHeights()
	{
		int ichildCount = mChildWidgets.Count();
		float childCount = (float)ichildCount;
		float availableHeight = mChildrenWorldRect.Height() - mBorder.bottom - mBorder.top;
		float minHeightSum = mChildWidgets.Sum<float>([&](UIWidget* child) { return child->layout->GetMinimalHeight(); });
		float expandValue = Math::Max(availableHeight - minHeightSum - (childCount - 1.0f)*mSpacing, 0.0f);

		Vector<float> heights(ichildCount + 1);
		mChildWidgets.ForEach([&](UIWidget* child) { heights.Add(child->layout->GetMinimalHeight()); });

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
					float wweight = mChildWidgets[i]->layout->GetHeightWeight();
					maxSzWeight = Math::Max(maxSzWeight, wweight);
					minSzWeightsSum += wweight;
					minSzChilds.Add(i);
				}

				if (h < minSz)
				{
					minSz = h;
					minSzChilds.Clear();
					minSzChilds.Add(i);
					float wweight = mChildWidgets[i]->layout->GetHeightWeight();
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
			minSzChilds.ForEach([&](int idx) { heights[idx] += expValueByWeight*mChildWidgets[idx]->layout->GetHeightWeight(); });

			expandValue -= needsDelta;
		}

		return heights;
	}
}

DECLARE_CLASS(o2::UIVerticalLayout);
