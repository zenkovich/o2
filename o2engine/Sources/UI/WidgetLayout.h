#pragma once

#include "Utils/IObject.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Serialization.h"

namespace o2
{
	class Widget;

	// -------------
	// Widget layout
	// -------------
	class WidgetLayout: public ISerializable
	{
	public:
		Property<Vec2F> pivot;
		Property<Vec2F> position;
		Property<Vec2F> size;
		Property<float> width;
		Property<float> height;
		Property<Vec2F> absPosition;
		Property<float> absLeft;
		Property<float> absRight;
		Property<float> absBottom;
		Property<float> absTop;
		Property<float> pivotX;
		Property<float> pivotY;
		Property<bool>  fitByChildren;
		Property<Vec2F> anchorMin;
		Property<Vec2F> anchorMax;
		Property<Vec2F> offsetMin;
		Property<Vec2F> offsetMax;
		Property<float> anchorLeft;
		Property<float> anchorRight;
		Property<float> anchorBottom;
		Property<float> anchorTop;
		Property<float> offsetLeft;
		Property<float> offsetRight;
		Property<float> offsetBottom;
		Property<float> offsetTop;
		Property<Vec2F> minSize;
		Property<float> minWidth;
		Property<float> minHeight;
		Property<Vec2F> maxSize;
		Property<float> maxWidth;
		Property<float> maxHeight;
		Property<Vec2F> weight;
		Property<float> widthWeight;
		Property<float> heigthWeight;

		WidgetLayout();

		WidgetLayout(const WidgetLayout& other);

		WidgetLayout& operator=(const WidgetLayout& other);

		void SetPosition(const Vec2F& position);
		Vec2F GetPosition();

		void SetSize(const Vec2F& size);
		Vec2F GetSize() const;

		void SetWidth(float width);
		float GetWidth() const;

		void SetHeight(float height);
		float GetHeight() const;

		void SetRect(const RectF& rect);
		RectF GetRect() const;

		void SetAbsoluteLeft(float value);
		float GetAbsoluteLeft() const;

		void SetAbsoluteRight(float value);
		float GetAbsoluteRight() const;

		void SetAbsoluteBottom(float value);
		float GetAbsoluteBottom() const;

		void SetAbsoluteTop(float value);
		float GetAbsoluteTop() const;

		void SetAbsolutePosition(const Vec2F& absPosition);
		Vec2F GetAbsolutePosition() const;

		void SetAbsoluteRect(const RectF& rect);
		RectF GetAbsoluteRect() const;

		void SetPivot(const Vec2F& pivot);
		Vec2F GetPivot() const;

		void SetPivotX(float x);
		float GetPivotX() const;

		void SetPivotY(float y);
		float GetPivotY() const;

		void SetAnchorMin(const Vec2F& min);
		Vec2F GetAnchorMin() const;

		void SetAnchorMax(const Vec2F& max);
		Vec2F GetAnchorMax() const;

		void SetAnchorLeft(float value);
		float GetAnchorLeft() const;

		void SetAnchorRight(float value);
		float GetAnchorRight() const;

		void SetAnchorBottom(float value);
		float GetAnchorBottom() const;

		void SetAnchorTop(float value);
		float GetAnchorTop() const;

		void SetOffsetMin(const Vec2F& min);
		Vec2F GetOffsetMin() const;

		void SetOffsetMax(const Vec2F& max);
		Vec2F GetOffsetMax() const;

		void SetOffsetLeft(float value);
		float GetOffsetLeft() const;

		void SetOffsetRight(float value);
		float GetOffsetRight() const;

		void SetOffsetBottom(float value);
		float GetOffsetBottom() const;

		void SetOffsetTop(float value);
		float GetOffsetTop() const;

		void SetMinimalSize(const Vec2F& minSize);
		Vec2F GetMinimalSize() const;

		void SetMinimalWidth(float value);
		float GetMinimalWidth() const;

		void SetMinimalHeight(float value);
		float GetMinimalHeight() const;

		void SetMaximalSize(const Vec2F& maxSize);
		Vec2F GetMaximalSize() const;

		void SetMaximalWidth(float value);
		float GetMaximalWidth() const;

		void SetMaximalHeight(float value);
		float GetMaximalHeight() const;

		void SetFittingByChildren(bool fit);
		bool IsFittingByChildren() const;

		void SetWeight(const Vec2F& weight);
		Vec2F GetWeight() const;

		void SetWidthWeight(float widthWeigth);
		float GetWidthWeight();

		void SetHeightWeight(float heigthWeigth);
		float GetHeightWeight();

		SERIALIZABLE_IMPL(WidgetLayout);

		IOBJECT(WidgetLayout)
		{
			FIELD(pivot);
			FIELD(position);
			FIELD(size);
			FIELD(width);
			FIELD(height);
			FIELD(absPosition);
			FIELD(absLeft);
			FIELD(absRight);
			FIELD(absBottom);
			FIELD(absTop);
			FIELD(pivotX);
			FIELD(pivotY);
			FIELD(fitByChildren);
			FIELD(anchorMin);
			FIELD(anchorMax);
			FIELD(offsetMin);
			FIELD(offsetMax);
			FIELD(anchorLeft);
			FIELD(anchorRight);
			FIELD(anchorBottom);
			FIELD(anchorTop);
			FIELD(offsetLeft);
			FIELD(offsetRight);
			FIELD(offsetBottom);
			FIELD(offsetTop);
			FIELD(minSize);
			FIELD(minWidth);
			FIELD(minHeight);
			FIELD(maxSize);
			FIELD(maxWidth);
			FIELD(maxHeight);
			FIELD(weight);
			FIELD(widthWeight);
			FIELD(heigthWeight);

			//FIELD(mOwner);
			SRLZ_FIELD(mPivot);
			SRLZ_FIELD(mAnchorMin);
			SRLZ_FIELD(mAnchorMax);
			SRLZ_FIELD(mOffsetMin);
			SRLZ_FIELD(mOffsetMax);
			SRLZ_FIELD(mWeight);
			SRLZ_FIELD(mFitByChilds);

			FIELD(mAbsoluteRect);
		}

	protected:
		Ptr<Widget> mOwner;
		Vec2F       mPivot          = Vec2F(0.5f, 0.5f);
		Vec2F       mAnchorMin      = Vec2F(0.5f, 0.5f);
		Vec2F       mAnchorMax      = Vec2F(0.5f, 0.5f);
		Vec2F       mOffsetMin      = Vec2F(0.0f, 0.0f);
		Vec2F       mOffsetMax      = Vec2F(0.0f, 0.0f);
		Vec2F       mMinSize        = Vec2F(0.0f, 0.0f);
		Vec2F       mMaxSize        = Vec2F(FLT_MAX, FLT_MAX);
		RectF       mAbsoluteRect   = RectF();
		RectF       mLocalRect      = RectF();
		Vec2F       mWeight         = Vec2F(1.0f, 1.0f);
		bool        mFitByChilds    = false;
		bool        mDrivenByParent = false;

	protected:
		void Recalculate();
		void UpdateRect();
		void InitializeProperties();

		friend class Widget;
		friend class WidgetLayer;
		friend class HorizontalLayout;
		friend class VerticalLayout;
	};
}