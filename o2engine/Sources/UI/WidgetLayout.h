#pragma once

#include "Utils/IObject.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Serialization.h"

namespace o2
{
	class UIWidget;

	// -------------
	// Widget layout
	// -------------
	class UIWidgetLayout: public ISerializable
	{
	public:
		Property<Vec2F> pivot;
		Property<Vec2F> position;
		Property<Vec2F> size;
		Property<float> width;
		Property<float> height;
		Property<Vec2F> absPosition;
		Property<Vec2F> absLeftTop;
		Property<Vec2F> absLeftBottom;
		Property<Vec2F> absRightTop;
		Property<Vec2F> absRightBottom;
		Property<RectF> absRect;
		Property<float> absLeft;
		Property<float> absRight;
		Property<float> absBottom;
		Property<float> absTop;
		Property<float> pivotX;
		Property<float> pivotY;
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

		UIWidgetLayout();

		UIWidgetLayout(const UIWidgetLayout& other);

		UIWidgetLayout& operator=(const UIWidgetLayout& other);

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

		void SetAbsoluteLeftTop(const Vec2F& absPosition);
		Vec2F GetAbsoluteLeftTop() const;

		void SetAbsoluteLeftBottom(const Vec2F& absPosition);
		Vec2F GetAbsoluteLeftBottom() const;

		void SetAbsoluteRightTop(const Vec2F& absPosition);
		Vec2F GetAbsoluteRightTop() const;

		void SetAbsoluteRightBottom(const Vec2F& absPosition);
		Vec2F GetAbsoluteRightBottom() const;

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

		void SetWeight(const Vec2F& weight);
		Vec2F GetWeight() const;

		void SetWidthWeight(float widthWeigth);
		float GetWidthWeight();

		void SetHeightWeight(float heigthWeigth);
		float GetHeightWeight();

		static UIWidgetLayout Both(float borderLeft = 0, float borderBottom = 0, float borderRight = 0, float borderTop = 0);

		SERIALIZABLE(UIWidgetLayout);

	protected:
		Ptr<UIWidget> mOwner;								
		Vec2F         mPivot          = Vec2F(0.5f, 0.5f);	   // @SERIALIZABLE
		Vec2F         mAnchorMin      = Vec2F(0.5f, 0.5f);	   // @SERIALIZABLE
		Vec2F         mAnchorMax      = Vec2F(0.5f, 0.5f);	   // @SERIALIZABLE
		Vec2F         mOffsetMin      = Vec2F(-10.0f, -10.0f); // @SERIALIZABLE
		Vec2F         mOffsetMax      = Vec2F(10.0f, 10.0f);   // @SERIALIZABLE
		Vec2F         mMinSize        = Vec2F(0.0f, 0.0f);	   // @SERIALIZABLE
		Vec2F         mMaxSize        = Vec2F(10000, 10000);   // @SERIALIZABLE
		RectF         mAbsoluteRect   = RectF();			 
		RectF         mLocalRect      = RectF();			  
		Vec2F         mWeight         = Vec2F(1.0f, 1.0f);	   // @SERIALIZABLE
		bool          mDrivenByParent = false;

	protected:
		void CopyFrom(const UIWidgetLayout& other);
		void InitializeProperties();

		friend class UIContextMenu;
		friend class UICustomDropDown;
		friend class UICustomList;
		friend class UIEditBox;
		friend class UIHorizontalLayout;
		friend class UIHorizontalProgress;
		friend class UIHorizontalScrollBar;
		friend class UILabel;
		friend class UIScrollArea;
		friend class UITree;
		friend class UITreeNode;
		friend class UIVerticalLayout;
		friend class UIVerticalProgress;
		friend class UIVerticalScrollBar;
		friend class UIWidget;
		friend class UIWidgetLayer;
		friend class UIWindow;
	};
}