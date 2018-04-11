#include "stdafx.h"
#include "HorizontalLayout.h"

#include "UI/WidgetLayout.h"

namespace o2
{
	UIHorizontalLayout::UIHorizontalLayout(): UIWidget()
	{
		SetLayoutDirty();
	}

	UIHorizontalLayout::UIHorizontalLayout(const UIHorizontalLayout& other):
		mBaseCorner(other.mBaseCorner), mSpacing(other.mSpacing), mBorder(other.mBorder), mExpandWidth(other.mExpandWidth),
		mExpandHeight(other.mExpandHeight), UIWidget(other), mFitByChildren(other.mFitByChildren), baseCorner(this), 
		spacing(this), border(this), borderLeft(this), borderRight(this), expandWidth(this), expandHeight(this),
		borderTop(this), borderBottom(this), fitByChildren(this)
	{
		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	UIHorizontalLayout::~UIHorizontalLayout()
	{}

	UIHorizontalLayout& UIHorizontalLayout::operator=(const UIHorizontalLayout& other)
	{
		CopyData(other);
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

	void UIHorizontalLayout::SetBorder(const BorderF& border)
	{
		mBorder = border;
		RearrangeChilds();
	}

	BorderF UIHorizontalLayout::GetBorder() const
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

	void UIHorizontalLayout::UpdateTransform(bool withChildren /*= true*/)
	{
		UpdateLayoutParametres();

		if (mFitByChildren)
			ExpandSizeByChilds();

		UIWidget::UpdateTransform(withChildren);

		RearrangeChilds();
	}

	void UIHorizontalLayout::CopyData(const Actor& otherActor)
	{
		const UIHorizontalLayout& other = dynamic_cast<const UIHorizontalLayout&>(otherActor);

		mBaseCorner   = other.mBaseCorner;
		mSpacing      = other.mSpacing;
		mBorder       = other.mBorder;
		mExpandWidth  = other.mExpandWidth;
		mExpandHeight = other.mExpandHeight;

		UIWidget::CopyData(other);

		RetargetStatesAnimations();
		SetLayoutDirty();
	}

	float UIHorizontalLayout::GetMinWidthWithChildren() const
	{
		if (!mFitByChildren)
			return UIWidget::GetMinWidthWithChildren();

		float res = mBorder.left + mBorder.right + Math::Max(mChildWidgets.Count() - 1, 0)*mSpacing;
		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
				res += child->GetMinWidthWithChildren();
		}

		res = Math::Max(res, layout->mData->minSize.x);

		return res;
	}

	float UIHorizontalLayout::GetMinHeightWithChildren() const
	{
		if (!mFitByChildren)
			return UIWidget::GetMinHeightWithChildren();

		float res = 0;
		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
				res = Math::Max(res, child->GetMinHeightWithChildren() + mBorder.top + mBorder.bottom);
		}

		res = Math::Max(res, layout->mData->minSize.y);

		return res;
	}

	float UIHorizontalLayout::GetWidthWeightWithChildren() const
	{
		float res = 0;
		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
				res += child->GetWidthWeightWithChildren();
		}

		return res;
	}

	void UIHorizontalLayout::OnChildAdded(UIWidget* child)
	{
		child->layout->mData->drivenByParent = true;
	}

	void UIHorizontalLayout::OnChildRemoved(UIWidget* child)
	{
		child->layout->mData->drivenByParent = false;
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
		layout->mData->weight.x = 0;

		for (auto child : mChildWidgets)
		{
			if (child->mResEnabledInHierarchy)
				layout->mData->weight.x += child->GetWidthWeightWithChildren();
		}

		layout->mCheckMinMaxFunc = &UIWidgetLayout::CheckMinMax;
	}

	void UIHorizontalLayout::ArrangeFromCenter()
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
				child->layout->mData->offsetMin.x = position;
				position += widths[i++];

				child->layout->mData->offsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.5f);
			}
		}
		else
		{
			float totalWidth = mChildWidgets.Sum<float>([&](UIWidget* child) { return child->GetMinWidthWithChildren(); });
			totalWidth += (mChildWidgets.Count() - 1)*mSpacing;
			float position = -totalWidth*0.5f;
			for (auto child : mChildWidgets)
			{
				child->layout->mData->offsetMin.x = position;
				position += Math::Abs(Math::Max(child->layout->mData->minSize.x, child->GetMinWidthWithChildren()));

				child->layout->mData->offsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.5f);
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
			for (auto child : mChildWidgets)
			{
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
				child->layout->mData->offsetMin.x = position;
				position += Math::Abs(Math::Max(child->layout->mData->minSize.x, child->GetMinWidthWithChildren()));

				child->layout->mData->offsetMax.x = position;
				position += mSpacing;

				AlignWidgetByHeight(child, 0.0f);
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
			for (auto child : mChildWidgets)
			{
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
				child->layout->mData->offsetMax.x = -position;
				position += Math::Abs(Math::Max(child->layout->mData->minSize.x, child->GetMinWidthWithChildren()));

				child->layout->mData->offsetMin.x = -position;
				position += mSpacing;

				AlignWidgetByHeight(child, 1.0f);
			}
		}
	}

	void UIHorizontalLayout::AlignWidgetByHeight(UIWidget* child, float widthAnchor)
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

		Vec2F parentSize = mParent ? mParent->transform->size : Vec2F();
		Vec2F szDelta = size - (layout->mData->offsetMax - layout->mData->offsetMin + (layout->mData->anchorMax - layout->mData->anchorMin)*parentSize);

		if (mExpandWidth)
			szDelta.x = 0;

		if (mExpandHeight)
			szDelta.y = 0;

		layout->mData->offsetMax += szDelta*(Vec2F::One() - relativePivot);
		layout->mData->offsetMin -= szDelta*relativePivot;
	}

	Vector<float> UIHorizontalLayout::CalculateExpandedWidths()
	{
		int ichildCount = mChildWidgets.Count();
		float childCount = (float)ichildCount;
		float availableWidth = mChildrenWorldRect.Width() - mBorder.left - mBorder.right;
		float minWidthSum = mChildWidgets.Sum<float>([&](UIWidget* child) { return child->layout->GetMinimalWidth(); });
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

DECLARE_CLASS(o2::UIHorizontalLayout);
