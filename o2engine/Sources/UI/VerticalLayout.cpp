#include "VerticalLayout.h"

namespace o2
{
	IOBJECT_CPP(VerticalLayout);

	VerticalLayout::VerticalLayout():
		mBaseCorner(BaseCorner::Left), mSpacing(0), mExpandWidth(true), mExpandHeight(true), Widget()
	{
		InitializeProperties();
	}

	VerticalLayout::VerticalLayout(const VerticalLayout& other):
		mBaseCorner(other.mBaseCorner), mSpacing(other.mSpacing), mBorder(other.mBorder), mExpandWidth(other.mExpandWidth),
		mExpandHeight(other.mExpandHeight), Widget(other)
	{
		InitializeProperties();
	}

	VerticalLayout::~VerticalLayout()
	{}

	VerticalLayout& VerticalLayout::operator=(const VerticalLayout& other)
	{
		mBaseCorner = other.mBaseCorner;
		mSpacing = other.mSpacing;
		mBorder = other.mBorder;
		mExpandWidth = other.mExpandWidth;
		mExpandHeight = other.mExpandHeight;
		Widget::operator=(other);

		return *this;
	}

	void VerticalLayout::SetBaseCorner(BaseCorner baseCorner)
	{
		mBaseCorner = baseCorner;
		RearrangeChilds();
	}

	VerticalLayout::BaseCorner VerticalLayout::GetBaseCorner() const
	{
		return mBaseCorner;
	}

	void VerticalLayout::SetSpacing(float spacing)
	{
		mSpacing = spacing;
		RearrangeChilds();
	}

	float VerticalLayout::GetSpacing() const
	{
		return mSpacing;
	}

	void VerticalLayout::SetBorder(const RectF& border)
	{
		mBorder = border;
		RearrangeChilds();
	}

	RectF VerticalLayout::GetBorder() const
	{
		return mBorder;
	}

	void VerticalLayout::SetBorderLeft(float value)
	{
		mBorder.left = value;
		RearrangeChilds();
	}

	float VerticalLayout::GetBorderLeft() const
	{
		return mBorder.left;
	}

	void VerticalLayout::SetBorderRight(float value)
	{
		mBorder.right = value;
		RearrangeChilds();
	}

	float VerticalLayout::GetBorderRight() const
	{
		return mBorder.right;
	}

	void VerticalLayout::SetBorderTop(float value)
	{
		mBorder.top = value;
		RearrangeChilds();
	}

	float VerticalLayout::GetBorderTop() const
	{
		return mBorder.top;
	}

	void VerticalLayout::SetBorderBottom(float value)
	{
		mBorder.bottom = value;
		RearrangeChilds();
	}

	float VerticalLayout::GetBorderBottom() const
	{
		return mBorder.bottom;
	}

	void VerticalLayout::SetWidthExpand(bool expand)
	{
		mExpandWidth = expand;
		RearrangeChilds();
	}

	bool VerticalLayout::IsWidthExpand() const
	{
		return mExpandWidth;
	}

	void VerticalLayout::SetHeightExpand(bool expand)
	{
		mExpandHeight = expand;
		RearrangeChilds();
	}

	bool VerticalLayout::IsHeightExpand() const
	{
		return mExpandHeight;
	}

	void VerticalLayout::OnLayoutUpdated()
	{
		RearrangeChilds();
	}

	void VerticalLayout::RearrangeChilds()
	{
		UpdateLayoutParametres();

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

	void VerticalLayout::UpdateLayoutParametres()
	{
		layout.mWeight.y = mChilds.Sum<float>([&](const Ptr<Widget>& child) { return child->layout.GetHeightWeight(); });
		layout.mMinSize.y = mChilds.Sum<float>([&](const Ptr<Widget>& child) { return child->layout.GetMinimalHeight(); });
	}

	void VerticalLayout::ArrangeFromCenter()
	{
		if (mExpandHeight)
		{
			float availableHeight = layout.mAbsoluteRect.Height() - mBorder.bottom - mBorder.top;
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
				child->layout.Recalculate();
			}
		}
		else
		{
			float totalHeight = mChilds.Sum<float>([&](const Ptr<Widget>& child) { return child->layout.GetHeight(); });
			totalHeight += (mChilds.Count() - 1)*mSpacing;
			float position = -totalHeight*0.5f;
			for (auto child : mChilds)
			{
				Vec2F childSize = child->layout.GetOffsetMax() - child->layout.GetOffsetMin();

				child->layout.mOffsetMin.y = position;
				position += Math::Abs(childSize.y);

				child->layout.mOffsetMax.y = position;
				position += mSpacing;

				AlignWidgetByWidth(child, 0.5f);
				child->layout.Recalculate();
			}
		}
	}

	void VerticalLayout::ArrangeFromBottomToTop()
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
				child->layout.Recalculate();
			}
		}
		else
		{
			float position = mBorder.bottom;
			for (auto child : mChilds)
			{
				Vec2F childSize = child->layout.GetOffsetMax() - child->layout.GetOffsetMin();

				child->layout.mOffsetMin.y = position;
				position += Math::Abs(childSize.y);

				child->layout.mOffsetMax.y = position;
				position += mSpacing;

				AlignWidgetByWidth(child, 0.0f);
				child->layout.Recalculate();
			}
		}
	}

	void VerticalLayout::ArrangeFromTopToBottom()
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
				child->layout.Recalculate();
			}
		}
		else
		{
			float position = mBorder.top;
			for (auto child : mChilds)
			{
				Vec2F childSize = child->layout.GetOffsetMax() - child->layout.GetOffsetMin();

				child->layout.mOffsetMax.y = -position;
				position += Math::Abs(childSize.y);

				child->layout.mOffsetMin.y = -position;
				position += mSpacing;

				AlignWidgetByWidth(child, 1.0f);
				child->layout.Recalculate();
			}
		}
	}

	void VerticalLayout::AlignWidgetByWidth(Ptr<Widget> child, float heightAnchor)
	{
		if (mExpandHeight)
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

	Vector<float> VerticalLayout::CalculateExpandedHeights()
	{
		int ichildCount = mChilds.Count();
		float childCount = (float)ichildCount;
		float availableHeight = layout.mAbsoluteRect.Height() - mBorder.bottom - mBorder.top;
		float minHeightSum = mChilds.Sum<float>([&](const Ptr<Widget>& child) { return child->layout.GetMinimalHeight(); });
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

	void VerticalLayout::InitializeProperties()
	{
		INITIALIZE_PROPERTY(VerticalLayout, baseCorner, SetBaseCorner, GetBaseCorner);
		INITIALIZE_PROPERTY(VerticalLayout, spacing, SetSpacing, GetSpacing);
		INITIALIZE_PROPERTY(VerticalLayout, border, SetBorder, GetBorder);
		INITIALIZE_PROPERTY(VerticalLayout, borderLeft, SetBorderLeft, GetBorderLeft);
		INITIALIZE_PROPERTY(VerticalLayout, borderRight, SetBorderRight, GetBorderRight);
		INITIALIZE_PROPERTY(VerticalLayout, borderTop, SetBorderTop, GetBorderTop);
		INITIALIZE_PROPERTY(VerticalLayout, borderBottom, SetBorderBottom, GetBorderBottom);
		INITIALIZE_PROPERTY(VerticalLayout, expandWidth, SetWidthExpand, IsWidthExpand);
		INITIALIZE_PROPERTY(VerticalLayout, expandHeight, SetHeightExpand, IsHeightExpand);
	}
}