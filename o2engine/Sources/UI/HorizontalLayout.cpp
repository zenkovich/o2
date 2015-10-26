#include "HorizontalLayout.h"

namespace o2
{
	IOBJECT_CPP(HorizontalLayout);

	HorizontalLayout::HorizontalLayout():
		mBaseCorner(BaseCorner::Left), mSpacing(0), mExpandWidth(true), mExpandHeight(true), Widget()
	{
		InitializeProperties();
	}

	HorizontalLayout::HorizontalLayout(const HorizontalLayout& other):
		mBaseCorner(other.mBaseCorner), mSpacing(other.mSpacing), mBorder(other.mBorder), mExpandWidth(other.mExpandWidth),
		mExpandHeight(other.mExpandHeight), Widget(other)
	{
		InitializeProperties();
	}

	HorizontalLayout::~HorizontalLayout()
	{}

	HorizontalLayout& HorizontalLayout::operator=(const HorizontalLayout& other)
	{
		mBaseCorner = other.mBaseCorner;
		mSpacing = other.mSpacing;
		mBorder = other.mBorder;
		mExpandWidth = other.mExpandWidth;
		mExpandHeight = other.mExpandHeight;
		Widget::operator=(other);

		return *this;
	}

	void HorizontalLayout::SetBaseCorner(BaseCorner baseCorner)
	{
		mBaseCorner = baseCorner;
		RearrangeChilds();
	}

	HorizontalLayout::BaseCorner HorizontalLayout::GetBaseCorner() const
	{
		return mBaseCorner;
	}

	void HorizontalLayout::SetSpacing(float spacing)
	{
		mSpacing = spacing;
		RearrangeChilds();
	}

	float HorizontalLayout::GetSpacing() const
	{
		return mSpacing;
	}

	void HorizontalLayout::SetBorder(const RectF& border)
	{
		mBorder = border;
		RearrangeChilds();
	}

	RectF HorizontalLayout::GetBorder() const
	{
		return mBorder;
	}

	void HorizontalLayout::SetBorderLeft(float value)
	{
		mBorder.left = value;
		RearrangeChilds();
	}

	float HorizontalLayout::GetBorderLeft() const
	{
		return mBorder.left;
	}

	void HorizontalLayout::SetBorderRight(float value)
	{
		mBorder.right = value;
		RearrangeChilds();
	}

	float HorizontalLayout::GetBorderRight() const
	{
		return mBorder.right;
	}

	void HorizontalLayout::SetBorderTop(float value)
	{
		mBorder.top = value;
		RearrangeChilds();
	}

	float HorizontalLayout::GetBorderTop() const
	{
		return mBorder.top;
	}

	void HorizontalLayout::SetBorderBottom(float value)
	{
		mBorder.bottom = value;
		RearrangeChilds();
	}

	float HorizontalLayout::GetBorderBottom() const
	{
		return mBorder.bottom;
	}

	void HorizontalLayout::SetWidthExpand(bool expand)
	{
		mExpandWidth = expand;
		RearrangeChilds();
	}

	bool HorizontalLayout::IsWidthExpand() const
	{
		return mExpandWidth;
	}

	void HorizontalLayout::SetHeightExpand(bool expand)
	{
		mExpandHeight = expand;
		RearrangeChilds();
	}

	bool HorizontalLayout::IsHeightExpand() const
	{
		return mExpandHeight;
	}

	void HorizontalLayout::OnLayoutUpdated()
	{
		RearrangeChilds();
	}

	void HorizontalLayout::RearrangeChilds()
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
	
	void HorizontalLayout::UpdateLayoutParametres()
	{
		layout.mWeight.x = mChilds.Sum<float>([&](const Ptr<Widget>& child) { return child->layout.GetWidthWeight(); });
		layout.mMinSize.x = mChilds.Sum<float>([&](const Ptr<Widget>& child) { return child->layout.GetMinimalWidth(); });
	}

	void HorizontalLayout::ArrangeFromCenter()
	{
		if (mExpandWidth)
		{
			float availableWidth = layout.mAbsoluteRect.Width() - mBorder.left - mBorder.right;
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
				child->layout.Recalculate();
			}
		}
		else
		{
			float totalWidth = mChilds.Sum<float>([&](const Ptr<Widget>& child) { return child->layout.GetWidth(); });
			totalWidth += (mChilds.Count() - 1)*mSpacing;
			float position = -totalWidth*0.5f;
			for (auto child : mChilds)
			{
				Vec2F childSize = child->layout.GetOffsetMax() - child->layout.GetOffsetMin();

				child->layout.mOffsetMin.x = position;
				position += Math::Abs(childSize.x);

				child->layout.mOffsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.5f);
				child->layout.Recalculate();
			}
		}
	}

	void HorizontalLayout::ArrangeFromLeftToRight()
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
				child->layout.Recalculate();
			}
		}
		else
		{
			float position = mBorder.left;
			for (auto child : mChilds)
			{
				Vec2F childSize = child->layout.GetOffsetMax() - child->layout.GetOffsetMin();

				child->layout.mOffsetMin.x = position;
				position += Math::Abs(childSize.x);

				child->layout.mOffsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.0f);
				child->layout.Recalculate();
			}
		}
	}

	void HorizontalLayout::ArrangeFromRightToLeft()
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
				child->layout.Recalculate();
			}
		}
		else
		{
			float position = mBorder.right;
			for (auto child : mChilds)
			{
				Vec2F childSize = child->layout.GetOffsetMax() - child->layout.GetOffsetMin();

				child->layout.mOffsetMax.x = -position;
				position += Math::Abs(childSize.x);

				child->layout.mOffsetMin.x = -position;
				position += mSpacing;

				AlignWidgetByHeight(child, 1.0f);
				child->layout.Recalculate();
			}
		}
	}

	void HorizontalLayout::AlignWidgetByHeight(Ptr<Widget> child, float widthAnchor)
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

Vector<float> HorizontalLayout::CalculateExpandedWidths()
	{
		int ichildCount = mChilds.Count();
		float childCount = (float)ichildCount;
		float availableWidth = layout.mAbsoluteRect.Width() - mBorder.left - mBorder.right;
		float minWidthSum = mChilds.Sum<float>([&](const Ptr<Widget>& child) { return child->layout.GetMinimalWidth(); });
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

	void HorizontalLayout::InitializeProperties()
	{
		INITIALIZE_PROPERTY(HorizontalLayout, baseCorner, SetBaseCorner, GetBaseCorner);
		INITIALIZE_PROPERTY(HorizontalLayout, spacing, SetSpacing, GetSpacing);
		INITIALIZE_PROPERTY(HorizontalLayout, border, SetBorder, GetBorder);
		INITIALIZE_PROPERTY(HorizontalLayout, borderLeft, SetBorderLeft, GetBorderLeft);
		INITIALIZE_PROPERTY(HorizontalLayout, borderRight, SetBorderRight, GetBorderRight);
		INITIALIZE_PROPERTY(HorizontalLayout, borderTop, SetBorderTop, GetBorderTop);
		INITIALIZE_PROPERTY(HorizontalLayout, borderBottom, SetBorderBottom, GetBorderBottom);
		INITIALIZE_PROPERTY(HorizontalLayout, expandWidth, SetWidthExpand, IsWidthExpand);
		INITIALIZE_PROPERTY(HorizontalLayout, expandHeight, SetHeightExpand, IsHeightExpand);
	}
}