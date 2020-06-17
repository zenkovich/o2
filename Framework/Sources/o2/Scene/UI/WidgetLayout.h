#pragma once

#include "o2/Scene/ActorTransform.h"

namespace o2
{
	class Widget;
	class WidgetLayoutData;

	// ----------------------------------------------------------------------------------------------------------------
	// Widget layout. Represents the Widget transformation layout. The position and size sets by anchors and offsets. 
	// Anchor is a vector value from (0, 0) to (1, 1), where (0, 0) is left bottom corner of parent and (1, 1) is right 
	// top corner. Offsets represents the position and size relative to anchor points. 
	// Also layout has minimal, maximum size and layout weight, used in grouped layouts.
	// ----------------------------------------------------------------------------------------------------------------
	class WidgetLayout: public ActorTransform
	{
	public:
		PROPERTIES(WidgetLayout);
		PROPERTY(Vec2F, anchorMin, SetAnchorMin, GetAnchorMin); // Left bottom anchor property
		PROPERTY(Vec2F, anchorMax, SetAnchorMax,GetAnchorMax);  // Right top anchor property

		PROPERTY(Vec2F, offsetMin, SetOffsetMin, GetOffsetMin); // Left bottom corner offset property
		PROPERTY(Vec2F, offsetMax, SetOffsetMax, GetOffsetMax); // Right top corner offset property

		PROPERTY(float, anchorLeft, SetAnchorLeft, GetAnchorLeft);       // Left anchor property
		PROPERTY(float, anchorRight, SetAnchorRight, GetAnchorRight);    // Right anchor property
		PROPERTY(float, anchorBottom, SetAnchorBottom, GetAnchorBottom); // Bottom anchor property
		PROPERTY(float, anchorTop, SetAnchorTop, GetAnchorTop);          // Top anchor property

		PROPERTY(float, offsetLeft, SetOffsetLeft, GetOffsetLeft);       // Left offset property
		PROPERTY(float, offsetRight, SetoffsetRight, GetoffsetRight);    // Right offset property
		PROPERTY(float, offsetBottom, SetOffsetBottom, GetOffsetBottom); // Bottom offset property
		PROPERTY(float, offsetTop, SetOffsetTop, GetOffsetTop);          // Top offset property

		PROPERTY(Vec2F, minSize, SetMinimalSize, GetMinimalSize);      // Minimal size property
		PROPERTY(float, minWidth, SetMinimalWidth, GetMinWidth);   // Minimal width property
		PROPERTY(float, minHeight,SetMinimalHeight, GetMinHeight); // Minimal height property

		PROPERTY(Vec2F, maxSize, SetMaximalSize, GetMaximalSize);       // Maximum size property
		PROPERTY(float, maxWidth, SetMaximalWidth, GetMaxWidth);    // Maximum width property
		PROPERTY(float, maxHeight, SetMaximalHeight, GetMaxHeight); // Maximum height property

		PROPERTY(Vec2F, weight, SetWeight, GetWeight);                   // Layout weight property
		PROPERTY(float, widthWeight, SetWidthWeight, GetWidthWeight);    // Width layout weight property
		PROPERTY(float, heigthWeight, SetHeightWeight, GetHeightWeight); // Height layout weight property

	public:
		// Default constructor, creates both stretching layout  
		WidgetLayout();

		// Constructor with parameters
		WidgetLayout(const Vec2F& anchorMin, const Vec2F& anchorMax, const Vec2F& offsetMin, const Vec2F& offsetMax);

		// Constructor with parameters
		WidgetLayout(float anchorLeft, float anchorTop, float anchorRight, float anchorBottom,
					   float offsetLeft, float offsetTop, float offsetRight, float offsetBottom);

		// Copy-constructor
		WidgetLayout(const WidgetLayout& other);

		// Copy-operator
		WidgetLayout& operator=(const WidgetLayout& other);

		// Equals operator
		bool operator==(const WidgetLayout& other) const;

		// Updates layout and transformation
		void Update() override;

		// Sets transform dirty and needed to update. Checks is driven by parent and marks parent as dirty too
		void SetDirty(bool fromParent = false) override;

		// Copies data parameters from other layout
		void CopyFrom(const ActorTransform& other);

		// Sets position
		void SetPosition(const Vec2F& position) override;

		// Sets size
		void SetSize(const Vec2F& size) override;

		// Sets width
		void SetWidth(float value) override;

		// Sets height
		void SetHeight(float value) override;

		// Return size
		Vec2F GetSize() const override;

		// Return width
		float GetWidth() const override;

		// Return height
		float GetHeight() const override;

		// Sets pivot, in local space, where (0, 0) - left down corner, (1; 1) - right top
		void SetPivot(const Vec2F& pivot) override;

		// Sets basis
		void SetBasis(const Basis& basis) override;

		// Sets basis without size
		void SetNonSizedBasis(const Basis& basis) override;

		// Returns rect
		RectF GetRect() const override;

		// Sets rect
		void SetRect(const RectF& rect) override;

		// Return rectangle for children in worlds space 
		RectF GetChildrenWorldRect() const;

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
		void SetoffsetRight(float value);

		// Returns right offset
		float GetoffsetRight() const;

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
		float GetMinWidth() const;

		// Sets minimal height, enables size check
		void SetMinimalHeight(float value);

		// Returns minimal height
		float GetMinHeight() const;

		// Sets maximal size, enables size check
		void SetMaximalSize(const Vec2F& maxSize);

		// Returns maximal size
		Vec2F GetMaximalSize() const;

		// Sets maximal width, enables size check
		void SetMaximalWidth(float value);

		// Returns maximal width
		float GetMaxWidth() const;

		// Sets maximal height, enables size check
		void SetMaximalHeight(float value);

		// Returns maximal height
		float GetMaxHeight() const;

		// Disables size checking
		void DisableSizeChecks();

		// Enables size checking
		void EnableSizeChecks();

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
		static WidgetLayout BothStretch(float borderLeft = 0, float borderBottom = 0, 
										  float borderRight = 0, float borderTop = 0);

		// Returns fixed by size layout, positioned relative to corner with offset 
		static WidgetLayout Based(BaseCorner corner, const Vec2F& size, const Vec2F& offset = Vec2F());

		// Returns horizontal stretching by parent layout, anchored by height with offsets and left and right border
		static WidgetLayout HorStretch(VerAlign align, float left, float right, float height, float offsY = 0.0f);

		// Returns vertical stretching by parent layout, anchored by width with offsets and left and right border
		static WidgetLayout VerStretch(HorAlign align, float top, float bottom, float width, float offsX = 0.0f);

		SERIALIZABLE(WidgetLayout);

	protected:
		void(WidgetLayout::*mCheckMinMaxFunc)(); // Check minimum and maximum of layout delegate
		WidgetLayoutData* mData;

	protected:
		// Sets owner and updates transform
		void SetOwner(Actor* actor) override;

		// Returns parent rectange, or zero when no parent
		RectF GetParentRectangle() const override;

		// Floors all local rectangle properties
		void FloorRectangle();

		// Updates offsets to match existing rectangle to offsets and anchors rectangle
		void UpdateOffsetsByCurrentTransform();

		// Checks minimum and maximum size
		void CheckMinMax();

		// Doesn't checks size, dummy function
		void DontCheckMinMax();

		friend class Widget;
		friend class WidgetLayer;
	};

	class WidgetLayoutData : public ActorTransformData
	{
	public:
		Vec2F anchorMin = Vec2F(0, 0); // Left bottom anchor @SERIALIZABLE
		Vec2F anchorMax = Vec2F(0, 0); // Right top anchor @SERIALIZABLE

		Vec2F offsetMin = Vec2F(0, 0);   // Left bottom offset @SERIALIZABLE
		Vec2F offsetMax = Vec2F(10, 10); // Right top offset @SERIALIZABLE

		Vec2F minSize = Vec2F(0, 0);	 // Minimal size @SERIALIZABLE
		Vec2F maxSize = Vec2F(10000, 10000); // Maximum size @SERIALIZABLE

		Vec2F weight = Vec2F(1, 1); // Layout weight @SERIALIZABLE

		RectF childrenWorldRect; // World rectangle for children arranging

		bool drivenByParent = false; // Is layout controlling by parent

		Widget* owner = nullptr; // owner widget pointer @EXCLUDE_POINTER_SEARCH

		SERIALIZABLE(WidgetLayoutData);
	};

	// Calculates children widths or heights by weights and min/max sizes
	Vector<float> CalculateExpandedSize(Vector<Widget*>& widgets, bool horizontal, float availableWidth, float spacing);
}

CLASS_BASES_META(o2::WidgetLayout)
{
	BASE_CLASS(o2::ActorTransform);
}
END_META;
CLASS_FIELDS_META(o2::WidgetLayout)
{
	PUBLIC_FIELD(anchorMin);
	PUBLIC_FIELD(anchorMax);
	PUBLIC_FIELD(offsetMin);
	PUBLIC_FIELD(offsetMax);
	PUBLIC_FIELD(anchorLeft);
	PUBLIC_FIELD(anchorRight);
	PUBLIC_FIELD(anchorBottom);
	PUBLIC_FIELD(anchorTop);
	PUBLIC_FIELD(offsetLeft);
	PUBLIC_FIELD(offsetRight);
	PUBLIC_FIELD(offsetBottom);
	PUBLIC_FIELD(offsetTop);
	PUBLIC_FIELD(minSize);
	PUBLIC_FIELD(minWidth);
	PUBLIC_FIELD(minHeight);
	PUBLIC_FIELD(maxSize);
	PUBLIC_FIELD(maxWidth);
	PUBLIC_FIELD(maxHeight);
	PUBLIC_FIELD(weight);
	PUBLIC_FIELD(widthWeight);
	PUBLIC_FIELD(heigthWeight);
	PROTECTED_FIELD(mCheckMinMaxFunc);
	PROTECTED_FIELD(mData);
}
END_META;
CLASS_METHODS_META(o2::WidgetLayout)
{

	PUBLIC_FUNCTION(void, Update);
	PUBLIC_FUNCTION(void, SetDirty, bool);
	PUBLIC_FUNCTION(void, CopyFrom, const ActorTransform&);
	PUBLIC_FUNCTION(void, SetPosition, const Vec2F&);
	PUBLIC_FUNCTION(void, SetSize, const Vec2F&);
	PUBLIC_FUNCTION(void, SetWidth, float);
	PUBLIC_FUNCTION(void, SetHeight, float);
	PUBLIC_FUNCTION(Vec2F, GetSize);
	PUBLIC_FUNCTION(float, GetWidth);
	PUBLIC_FUNCTION(float, GetHeight);
	PUBLIC_FUNCTION(void, SetPivot, const Vec2F&);
	PUBLIC_FUNCTION(void, SetBasis, const Basis&);
	PUBLIC_FUNCTION(void, SetNonSizedBasis, const Basis&);
	PUBLIC_FUNCTION(RectF, GetRect);
	PUBLIC_FUNCTION(void, SetRect, const RectF&);
	PUBLIC_FUNCTION(RectF, GetChildrenWorldRect);
	PUBLIC_FUNCTION(void, SetAxisAlignedRect, const RectF&);
	PUBLIC_FUNCTION(void, SetAnchorMin, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetAnchorMin);
	PUBLIC_FUNCTION(void, SetAnchorMax, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetAnchorMax);
	PUBLIC_FUNCTION(void, SetAnchorLeft, float);
	PUBLIC_FUNCTION(float, GetAnchorLeft);
	PUBLIC_FUNCTION(void, SetAnchorRight, float);
	PUBLIC_FUNCTION(float, GetAnchorRight);
	PUBLIC_FUNCTION(void, SetAnchorBottom, float);
	PUBLIC_FUNCTION(float, GetAnchorBottom);
	PUBLIC_FUNCTION(void, SetAnchorTop, float);
	PUBLIC_FUNCTION(float, GetAnchorTop);
	PUBLIC_FUNCTION(void, SetOffsetMin, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetOffsetMin);
	PUBLIC_FUNCTION(void, SetOffsetMax, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetOffsetMax);
	PUBLIC_FUNCTION(void, SetOffsetLeft, float);
	PUBLIC_FUNCTION(float, GetOffsetLeft);
	PUBLIC_FUNCTION(void, SetoffsetRight, float);
	PUBLIC_FUNCTION(float, GetoffsetRight);
	PUBLIC_FUNCTION(void, SetOffsetBottom, float);
	PUBLIC_FUNCTION(float, GetOffsetBottom);
	PUBLIC_FUNCTION(void, SetOffsetTop, float);
	PUBLIC_FUNCTION(float, GetOffsetTop);
	PUBLIC_FUNCTION(void, SetMinimalSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetMinimalSize);
	PUBLIC_FUNCTION(void, SetMinimalWidth, float);
	PUBLIC_FUNCTION(float, GetMinWidth);
	PUBLIC_FUNCTION(void, SetMinimalHeight, float);
	PUBLIC_FUNCTION(float, GetMinHeight);
	PUBLIC_FUNCTION(void, SetMaximalSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetMaximalSize);
	PUBLIC_FUNCTION(void, SetMaximalWidth, float);
	PUBLIC_FUNCTION(float, GetMaxWidth);
	PUBLIC_FUNCTION(void, SetMaximalHeight, float);
	PUBLIC_FUNCTION(float, GetMaxHeight);
	PUBLIC_FUNCTION(void, DisableSizeChecks);
	PUBLIC_FUNCTION(void, EnableSizeChecks);
	PUBLIC_FUNCTION(void, SetWeight, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetWeight);
	PUBLIC_FUNCTION(void, SetWidthWeight, float);
	PUBLIC_FUNCTION(float, GetWidthWeight);
	PUBLIC_FUNCTION(void, SetHeightWeight, float);
	PUBLIC_FUNCTION(float, GetHeightWeight);
	PUBLIC_STATIC_FUNCTION(WidgetLayout, BothStretch, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(WidgetLayout, Based, BaseCorner, const Vec2F&, const Vec2F&);
	PUBLIC_STATIC_FUNCTION(WidgetLayout, HorStretch, VerAlign, float, float, float, float);
	PUBLIC_STATIC_FUNCTION(WidgetLayout, VerStretch, HorAlign, float, float, float, float);
	PROTECTED_FUNCTION(void, SetOwner, Actor*);
	PROTECTED_FUNCTION(RectF, GetParentRectangle);
	PROTECTED_FUNCTION(void, FloorRectangle);
	PROTECTED_FUNCTION(void, UpdateOffsetsByCurrentTransform);
	PROTECTED_FUNCTION(void, CheckMinMax);
	PROTECTED_FUNCTION(void, DontCheckMinMax);
}
END_META;

CLASS_BASES_META(o2::WidgetLayoutData)
{
	BASE_CLASS(o2::ActorTransformData);
}
END_META;
CLASS_FIELDS_META(o2::WidgetLayoutData)
{
	PUBLIC_FIELD(anchorMin).DEFAULT_VALUE(Vec2F(0, 0)).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(anchorMax).DEFAULT_VALUE(Vec2F(0, 0)).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(offsetMin).DEFAULT_VALUE(Vec2F(0, 0)).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(offsetMax).DEFAULT_VALUE(Vec2F(10, 10)).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(minSize).DEFAULT_VALUE(Vec2F(0, 0)).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(maxSize).DEFAULT_VALUE(Vec2F(10000, 10000)).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(weight).DEFAULT_VALUE(Vec2F(1, 1)).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(childrenWorldRect);
	PUBLIC_FIELD(drivenByParent).DEFAULT_VALUE(false);
	PUBLIC_FIELD(owner).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(o2::WidgetLayoutData)
{
}
END_META;
