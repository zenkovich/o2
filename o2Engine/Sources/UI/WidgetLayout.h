#pragma once

#include "Scene/ActorTransform.h"

namespace o2
{
	class UIWidget;

	// ----------------------------------------------------------------------------------------------------------------
	// Widget layout. Represents the UIWidget transformation layout. The position and size sets by anchors and offsets. 
	// Anchor is a vector value from (0, 0) to (1, 1), where (0, 0) is left bottom corner of parent and (1, 1) is right 
	// top corner. Offsets represents the position and size relative to anchor points. 
	// Also layout has minimal, maximum size and layout weight, used in grouped layouts.
	// ----------------------------------------------------------------------------------------------------------------
	class UIWidgetLayout: public ActorTransform
	{
	public:
		Property<Vec2F> anchorMin;    // Left bottom anchor property
		Property<Vec2F> anchorMax;    // Right top anchor property

		Property<Vec2F> offsetMin;    // Left bottom corner offset property
		Property<Vec2F> offsetMax;    // Right top corner offset property

		Property<float> anchorLeft;   // Left anchor property
		Property<float> anchorRight;  // Right anchor property
		Property<float> anchorBottom; // Bottom anchor property
		Property<float> anchorTop;    // Top anchor property

		Property<float> offsetLeft;   // Left offset property
		Property<float> offsetRight;  // Right offset property
		Property<float> offsetBottom; // Bottom offset property
		Property<float> offsetTop;    // Top offset property

		Property<Vec2F> minSize;      // Minimal size property
		Property<float> minWidth;     // Minimal width property
		Property<float> minHeight;    // Minimal height property

		Property<Vec2F> maxSize;      // Maximum size property
		Property<float> maxWidth;     // Maximum width property
		Property<float> maxHeight;    // Maximum height property

		Property<Vec2F> weight;       // Layout weight property
		Property<float> widthWeight;  // Width layout weight property
		Property<float> heigthWeight; // Height layout weight property

		// Default constructor, creates both stretching layout
		UIWidgetLayout();

		// Constructor with parameters
		UIWidgetLayout(const Vec2F& anchorMin, const Vec2F& anchorMax, const Vec2F& offsetMin, const Vec2F& offsetMax);

		// Constructor with parameters
		UIWidgetLayout(float anchorLeft, float anchorTop, float anchorRight, float anchorBottom,
					   float offsetLeft, float offsetTop, float offsetRight, float offsetBottom);

		// Copy-constructor
		UIWidgetLayout(const UIWidgetLayout& other);

		// Copy-operator
		UIWidgetLayout& operator=(const UIWidgetLayout& other);

		// Equals operator
		bool operator==(const UIWidgetLayout& other) const;

		// Sets position
		void SetPosition(const Vec2F& position) override;

		// Sets size
		void SetSize(const Vec2F& size) override;

		// Sets pivot, in local space, where (0, 0) - left down corner, (1; 1) - right top
		void SetPivot(const Vec2F& pivot) override;

		// Sets basis
		void SetBasis(const Basis& basis) override;

		// Sets basis without size
		void SetNonSizedBasis(const Basis& basis) override;

		// Sets rect
		void SetRect(const RectF& rect) override;

		// Sets direction aligned rectangle transformation
		void SetAxisAlignedRect(const RectF& rect) override;

		// Sets left bottom anchor. (0, 0) - left bottom of parent, (1, 1) - right top
		void SetAnchorMin(const Vec2F& min);

		// Returns left bottom anchor. (0, 0) - left bottom of parent, (1, 1) - right top
		Vec2F GetAnchorMin() const;

		// Sets right top anchor. (0, 0) - left bottom of parent, (1, 1) - right top
		void SetAnchorMax(const Vec2F& max);

		// Returns right top anchor. (0, 0) - left bottom of parent, (1, 1) - right top
		Vec2F GetAnchorMax() const;

		// Sets left anchor. 0 - left parent border, 1 - right
		void SetAnchorLeft(float value);

		// Returns left anchor. 0 - left parent border, 1 - right
		float GetAnchorLeft() const;

		// Sets right anchor. 0 - left parent border, 1 - right
		void SetAnchorRight(float value);

		// Returns right anchor. 0 - left parent border, 1 - right
		float GetAnchorRight() const;

		// Sets bottom anchor. 0 - bottom parent border, 1 - top
		void SetAnchorBottom(float value);

		// Returns bottom anchor. 0 - bottom parent border, 1 - top
		float GetAnchorBottom() const;

		// Sets top anchor. 0 - bottom parent border, 1 - top
		void SetAnchorTop(float value);

		// Returns top anchor. 0 - bottom parent border, 1 - top
		float GetAnchorTop() const;

		// Sets left bottom offset
		void SetOffsetMin(const Vec2F& min);

		// Returns left bottom offset
		Vec2F GetOffsetMin() const;

		// Sets right top offset
		void SetOffsetMax(const Vec2F& max);

		// Returns right top offset
		Vec2F GetOffsetMax() const;

		// Sets left offset
		void SetOffsetLeft(float value);

		// Returns left offset
		float GetOffsetLeft() const;

		// Sets right offset
		void SetOffsetRight(float value);

		// Returns right offset
		float GetOffsetRight() const;

		// Sets bottom offset
		void SetOffsetBottom(float value);

		// Returns bottom offset
		float GetOffsetBottom() const;

		// Sets top offset
		void SetOffsetTop(float value);

		// Returns top offset
		float GetOffsetTop() const;

		// Sets minimal size, enables size check
		void SetMinimalSize(const Vec2F& minSize);

		// Returns minimal size
		Vec2F GetMinimalSize() const;

		// Sets minimal width, enables size check
		void SetMinimalWidth(float value);

		// Returns minimal width
		float GetMinimalWidth() const;

		// Sets minimal height, enables size check
		void SetMinimalHeight(float value);

		// Returns minimal height
		float GetMinimalHeight() const;

		// Sets maximal size, enables size check
		void SetMaximalSize(const Vec2F& maxSize);

		// Returns maximal size
		Vec2F GetMaximalSize() const;

		// Sets maximal width, enables size check
		void SetMaximalWidth(float value);

		// Returns maximal width
		float GetMaximalWidth() const;

		// Sets maximal height, enables size check
		void SetMaximalHeight(float value);

		// Returns maximal height
		float GetMaximalHeight() const;

		// Disables size checking
		void DisableMinMaxSizes();

		// Sets layout weight
		void SetWeight(const Vec2F& weight);

		// Returns layout height
		Vec2F GetWeight() const;

		// Sets width layout weight
		void SetWidthWeight(float widthWeigth);

		// Returns width layout weight 
		float GetWidthWeight();

		// Sets height layout weight
		void SetHeightWeight(float heigthWeigth);

		// Returns height layout weight
		float GetHeightWeight();

		// Returns both axis stretching by parent layout with border offsets
		static UIWidgetLayout BothStretch(float borderLeft = 0, float borderBottom = 0, 
										  float borderRight = 0, float borderTop = 0);

		// Returns fixed by size layout, positioned relative to corner with offset 
		static UIWidgetLayout Based(BaseCorner corner, const Vec2F& size, const Vec2F& offset = Vec2F());

		// Returns horizontal stretching by parent layout, anchored by height with offsets and left and right border
		static UIWidgetLayout HorStretch(VerAlign align, float left, float right, float height, float offsY = 0.0f);

		// Returns vertical stretching by parent layout, anchored by width with offsets and left and right border
		static UIWidgetLayout VerStretch(HorAlign align, float top, float bottom, float width, float offsX = 0.0f);

		SERIALIZABLE(UIWidgetLayout);

	protected:
		class Data: public ActorTransform::Data
		{
		public:
			Vec2F anchorMin = Vec2F(0.0f, 0.0f);   // Left bottom anchor @SERIALIZABLE
			Vec2F anchorMax = Vec2F(0.0f, 0.0f);   // Right top anchor @SERIALIZABLE

			Vec2F offsetMin = Vec2F(0.0f, 0.0f);   // Left bottom offset @SERIALIZABLE
			Vec2F offsetMax = Vec2F(10.0f, 10.0f); // Right top offset @SERIALIZABLE

			Vec2F minSize = Vec2F(0.0f, 0.0f);	   // Minimal size @SERIALIZABLE
			Vec2F maxSize = Vec2F(10000, 10000);   // Maximum size @SERIALIZABLE

			Vec2F weight = Vec2F(1.0f, 1.0f);	   // Layout weight @SERIALIZABLE

			bool  drivenByParent = false;          // Is layout controlling by parent

			UIWidget* owner = nullptr;             // owner widget pointer

			SERIALIZABLE(Data);
		};

		void(UIWidgetLayout::*mCheckMinMaxFunc)(); // Check minimum and maximum of layout delegate
		Data* mData;

	protected:
		// Sets owner and updates transform
		void SetOwner(Actor* actor);

		// Copies data parameters from other layout
		void CopyFrom(const UIWidgetLayout& other);

		// Checks minimum and maximum size
		void CheckMinMax();

		// Doesn't checks size, dummy function
		void DontCheckMinMax();

		// Initializes properties
		void InitializeProperties();

		friend class UIContextMenu;
		friend class UICustomDropDown;
		friend class UICustomList;
		friend class UIEditBox;
		friend class UIGridLayout;
		friend class UIHorizontalLayout;
		friend class UIHorizontalProgress;
		friend class UIHorizontalScrollBar;
		friend class UILabel;
		friend class UILongList;
		friend class UIMenuPanel;
		friend class UIScrollArea;
		friend class UISpoiler;
		friend class UIToggle;
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
