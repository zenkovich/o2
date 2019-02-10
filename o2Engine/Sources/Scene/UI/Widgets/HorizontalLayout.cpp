#include "stdafx.h"
#include "HorizontalLayout.h"

#include "Scene/UI/WidgetLayout.h"

namespace o2
{
	HorizontalLayout::HorizontalLayout(): Widget()
	{
		SetLayoutDirty();
	}

	HorizontalLayout::HorizontalLayout(const HorizontalLayout& other):
		mBaseCorner(other.mBaseCorner), mSpacing(other.mSpacing), mBorder(other.mBorder), mExpandWidth(other.mExpandWidth),
		mExpandHeight(other.mExpandHeight), Widget(other), mFitByChildren(other.mFitByChildren), baseCorner(this), 
		spacing(this), border(this), borderLeft(this), borderRight(this), expandWidth(this), expandHeight(this),
		borderTop(this), borderBottom(this), fitByChildren(this)
	{
		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	HorizontalLayout::~HorizontalLayout()
	{}

	HorizontalLayout& HorizontalLayout::operator=(const HorizontalLayout& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void HorizontalLayout::SetBaseCorner(BaseCorner baseCorner)
	{
		mBaseCorner = baseCorner;
		layout->SetDirty();
	}

	BaseCorner HorizontalLayout::GetBaseCorner() const
	{
		return mBaseCorner;
	}

	void HorizontalLayout::SetSpacing(float spacing)
	{
		mSpacing = spacing;
		layout->SetDirty();
	}

	float HorizontalLayout::GetSpacing() const
	{
		return mSpacing;
	}

	void HorizontalLayout::SetBorder(const BorderF& border)
	{
		mBorder = border;
		layout->SetDirty();
	}

	BorderF HorizontalLayout::GetBorder() const
	{
		return mBorder;
	}

	void HorizontalLayout::SetBorderLeft(float value)
	{
		mBorder.left = value;
		layout->SetDirty();
	}

	float HorizontalLayout::GetBorderLeft() const
	{
		return mBorder.left;
	}

	void HorizontalLayout::SetBorderRight(float value)
	{
		mBorder.right = value;
		layout->SetDirty();
	}

	float HorizontalLayout::GetBorderRight() const
	{
		return mBorder.right;
	}

	void HorizontalLayout::SetBorderTop(float value)
	{
		mBorder.top = value;
		layout->SetDirty();
	}

	float HorizontalLayout::GetBorderTop() const
	{
		return mBorder.top;
	}

	void HorizontalLayout::SetBorderBottom(float value)
	{
		mBorder.bottom = value;
		layout->SetDirty();
	}

	float HorizontalLayout::GetBorderBottom() const
	{
		return mBorder.bottom;
	}

	void HorizontalLayout::SetWidthExpand(bool expand)
	{
		mExpandWidth = expand;
		layout->SetDirty();
	}

	bool HorizontalLayout::IsWidthExpand() const
	{
		return mExpandWidth;
	}

	void HorizontalLayout::SetHeightExpand(bool expand)
	{
		mExpandHeight = expand;
		layout->SetDirty();
	}

	bool HorizontalLayout::IsHeightExpand() const
	{
		return mExpandHeight;
	}

	void HorizontalLayout::SetFitByChildren(bool fit)
	{
		mFitByChildren = fit;
		layout->SetDirty();
	}

	bool HorizontalLayout::IsFittingByChildren() const
	{
		return mFitByChildren;
	}

	void HorizontalLayout::UpdateSelfTransform()
{
		UpdateLayoutParametres();

		if (mFitByChildren)
			ExpandSizeByChilds();

		Widget::UpdateSelfTransform();

		RearrangeChilds();
	}

	void HorizontalLayout::CopyData(const Actor& otherActor)
	{
		const HorizontalLayout& other = dynamic_cast<const HorizontalLayout&>(otherActor);

		mBaseCorner   = other.mBaseCorner;
		mSpacing      = other.mSpacing;
		mBorder       = other.mBorder;
		mExpandWidth  = other.mExpandWidth;
		mExpandHeight = other.mExpandHeight;

		Widget::CopyData(other);

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	float HorizontalLayout::GetMinWidthWithChildren() const
	{
		if (!mFitByChildren)
			return Widget::GetMinWidthWithChildren();

		float res = mBorder.left + mBorder.right + Math::Max(mChildWidgets.Count() - 1, 0)*mSpacing;
		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
				res += child->GetMinWidthWithChildren();
		}

		res = Math::Max(res, layout->mData->minSize.x);

		return res;
	}

	float HorizontalLayout::GetMinHeightWithChildren() const
	{
		if (!mFitByChildren)
			return Widget::GetMinHeightWithChildren();

		float res = 0;
		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
				res = Math::Max(res, child->GetMinHeightWithChildren() + mBorder.top + mBorder.bottom);
		}

		res = Math::Max(res, layout->mData->minSize.y);

		return res;
	}

	float HorizontalLayout::GetWidthWeightWithChildren() const
	{
		float res = 0;
		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
				res += child->GetWidthWeightWithChildren();
		}

		return res;
	}

	void HorizontalLayout::OnChildAdded(Widget* child)
	{
		child->layout->mData->drivenByParent = true;
	}

	void HorizontalLayout::OnChildRemoved(Widget* child)
	{
		child->layout->mData->drivenByParent = false;
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
		layout->mData->weight.x = 0;

		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
				layout->mData->weight.x += child->GetWidthWeightWithChildren();
		}

		if (layout->mData->weight.x < FLT_EPSILON)
			layout->mData->weight.x = 1.0f;

		layout->mCheckMinMaxFunc = &WidgetLayout::CheckMinMax;
	}

	void HorizontalLayout::ArrangeFromCenter()
	{
		if (mExpandWidth)
		{
			float availableWidth = mChildrenWorldRect.Width() - mBorder.left - mBorder.right;
			float totalWidth = availableWidth;
			float position = -totalWidth*0.5f;
			auto widths = CalculateExpandedWidths();

			int i = 0;
			for (auto child : mChildWidgets)
			{
				if (!child->mResEnabledInHierarchy)
					continue;

				child->layout->mData->offsetMin.x = position;
				position += widths[i++];

				child->layout->mData->offsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.5f);
			}
		}
		else
		{
			float totalWidth = mChildWidgets.Sum<float>([&](Widget* child) { return child->GetMinWidthWithChildren(); });
			totalWidth += (mChildWidgets.Count() - 1)*mSpacing;
			float position = -totalWidth*0.5f;
			for (auto child : mChildWidgets)
			{
				if (!child->mResEnabledInHierarchy)
					continue;

				child->layout->mData->offsetMin.x = position;
				position += Math::Abs(Math::Max(child->layout->mData->minSize.x, child->GetMinWidthWithChildren()));

				child->layout->mData->offsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.5f);
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
			for (auto child : mChildWidgets)
			{
				if (!child->mResEnabledInHierarchy)
					continue;

				child->layout->mData->offsetMin.x = position;
				position += widths[i++];

				child->layout->mData->offsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.0f);
			}
		}
		else
		{
			float position = mBorder.left;
			for (auto child : mChildWidgets)
			{
				if (!child->mResEnabledInHierarchy)
					continue;

				child->layout->mData->offsetMin.x = position;
				position += Math::Abs(Math::Max(child->layout->mData->minSize.x, child->GetMinWidthWithChildren()));

				child->layout->mData->offsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.0f);
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
			for (auto child : mChildWidgets)
			{
				if (!child->mResEnabledInHierarchy)
					continue;

				child->layout->mData->offsetMax.x = -position;
				position += widths[i++];

				child->layout->mData->offsetMin.x = -position;
				position += mSpacing;

				AlignWidgetByHeight(child, 1.0f);
			}
		}
		else
		{
			float position = mBorder.right;
			for (auto child : mChildWidgets)
			{
				if (!child->mResEnabledInHierarchy)
					continue;

				child->layout->mData->offsetMax.x = -position;
				position += Math::Abs(Math::Max(child->layout->mData->minSize.x, child->GetMinWidthWithChildren()));

				child->layout->mData->offsetMin.x = -position;
				position += mSpacing;

				AlignWidgetByHeight(child, 1.0f);
			}
		}
	}

	void HorizontalLayout::AlignWidgetByHeight(Widget* child, float widthAnchor)
	{
		if (mExpandHeight)
		{
			child->layout->mData->anchorMin = Vec2F(widthAnchor, 0);
			child->layout->mData->anchorMax = Vec2F(widthAnchor, 1);
			child->layout->mData->offsetMin.y = mBorder.bottom;
			child->layout->mData->offsetMax.y = -mBorder.top;
		}
		else
		{
			float height = child->layout->height;
			if (mBaseCorner == BaseCorner::Bottom || mBaseCorner == BaseCorner::LeftBottom || mBaseCorner == BaseCorner::RightBottom)
			{
				child->layout->mData->anchorMin = Vec2F(widthAnchor, 0);
				child->layout->mData->anchorMax = Vec2F(widthAnchor, 0);
				child->layout->mData->offsetMin.y = mBorder.bottom;
				child->layout->mData->offsetMax.y = mBorder.bottom + height;
			}
			if (mBaseCorner == BaseCorner::Center || mBaseCorner == BaseCorner::Left || mBaseCorner == BaseCorner::Right)
			{
				child->layout->mData->anchorMin = Vec2F(widthAnchor, 0.5f);
				child->layout->mData->anchorMax = Vec2F(widthAnchor, 0.5f);
				child->layout->mData->offsetMin.y = -height*0.5f;
				child->layout->mData->offsetMax.y = height*0.5f;
			}
			if (mBaseCorner == BaseCorner::Top || mBaseCorner == BaseCorner::LeftTop || mBaseCorner == BaseCorner::RightTop)
			{
				child->layout->mData->anchorMin = Vec2F(widthAnchor, 1);
				child->layout->mData->anchorMax = Vec2F(widthAnchor, 1);
				child->layout->mData->offsetMin.y = -mBorder.top - height;
				child->layout->mData->offsetMax.y = -mBorder.top;
			}
		}
	}

	void HorizontalLayout::ExpandSizeByChilds()
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

		Vec2F parentSize = mParent ? mParent->transform->size : Vec2F();
		Vec2F szDelta = size - (layout->mData->offsetMax - layout->mData->offsetMin + (layout->mData->anchorMax - layout->mData->anchorMin)*parentSize);

		if (mExpandWidth)
			szDelta.x = 0;

		if (mExpandHeight)
			szDelta.y = 0;

		layout->mData->offsetMax += szDelta*(Vec2F::One() - relativePivot);
		layout->mData->offsetMin -= szDelta*relativePivot;
	}

	Vector<float> HorizontalLayout::CalculateExpandedWidths()
	{
		int ichildCount = mChildWidgets.Count();
		float childCount = (float)ichildCount;
		float availableWidth = mChildrenWorldRect.Width() - mBorder.left - mBorder.right;
		float minWidthSum = mChildWidgets.Sum<float>([&](Widget* child) { return child->layout->GetMinimalWidth(); });
		float expandValue = Math::Max(availableWidth - minWidthSum - (childCount - 1.0f)*mSpacing, 0.0f);

		Vector<float> widths(ichildCount + 1);
		mChildWidgets.ForEach([&](auto child) { widths.Add(child->layout->GetMinimalWidth()); });

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
					float wweight = mChildWidgets[i]->layout->GetWidthWeight();
					maxSzWeight = Math::Max(maxSzWeight, wweight);
					minSzWeightsSum += wweight;
					minSzChilds.Add(i);
				}

				if (w < minSz)
				{
					minSz = w;
					minSzChilds.Clear();
					minSzChilds.Add(i);
					float wweight = mChildWidgets[i]->layout->GetWidthWeight();
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
			minSzChilds.ForEach([&](int idx) { widths[idx] += expValueByWeight*mChildWidgets[idx]->layout->GetWidthWeight(); });

			expandValue -= needsDelta;
		}

		return widths;
	}
}

DECLARE_CLASS(o2::HorizontalLayout);
