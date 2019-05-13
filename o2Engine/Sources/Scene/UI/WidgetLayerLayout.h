#pragma once

#include "Utils/Serialization/Serializable.h"
#include "Utils/Math/Layout.h"

namespace o2
{
	class WidgetLayer;

	class WidgetLayerLayout : public ISerializable
	{
	public:
		PROPERTIES(WidgetLayerLayout);
		PROPERTY(Vec2F, position, SetPosition, GetPosition); // Left bottom anchor property
		PROPERTY(Vec2F, size, SetSize, GetSize);             // Left bottom anchor property

		PROPERTY(Vec2F, anchorMin, SetAnchorMin, GetAnchorMin); // Left bottom anchor property
		PROPERTY(Vec2F, anchorMax, SetAnchorMax, GetAnchorMax); // Right top anchor property

		PROPERTY(Vec2F, offsetMin, SetOffsetMin, GetOffsetMin); // Left bottom corner offset property
		PROPERTY(Vec2F, offsetMax, SetOffsetMax, GetOffsetMax); // Right top corner offset property

		PROPERTY(float, anchorLeft, SetAnchorLeft, GetAnchorLeft);       // Left anchor property
		PROPERTY(float, anchorRight, SetAnchorRight, GetAnchorRight);    // Right anchor property
		PROPERTY(float, anchorBottom, SetAnchorBottom, GetAnchorBottom); // Bottom anchor property
		PROPERTY(float, anchorTop, SetAnchorTop, GetAnchorTop);          // Top anchor property

		PROPERTY(float, offsetLeft, SetOffsetLeft, GetOffsetLeft);       // Left offset property
		PROPERTY(float, offsetRight, SetOffsetRight, GetOffsetRight);    // Right offset property
		PROPERTY(float, offsetBottom, SetOffsetBottom, GetOffsetBottom); // Bottom offset property
		PROPERTY(float, offsetTop, SetOffsetTop, GetOffsetTop);          // Top offset property

	public:
		// Default unused constructor, required for IObject
		WidgetLayerLayout();

		// Default constructor
		WidgetLayerLayout(WidgetLayer* widgetLayer);

		// Copy constructor
		WidgetLayerLayout(WidgetLayer* widgetLayer, const WidgetLayerLayout& other);

		// Cast to Layout operator
		operator Layout() const;

		// Copy operator from layout
		WidgetLayerLayout& operator=(const Layout& other);

		// Copy operator 
		WidgetLayerLayout& operator=(const WidgetLayerLayout& other);

		// Check equals operator
		bool operator==(const WidgetLayerLayout& other) const;

		// Check equals operator
		bool operator==(const Layout& other) const;

		// Check not equals operator
		bool operator!=(const WidgetLayerLayout& other) const;

		// Check not equals operator
		bool operator!=(const Layout& other) const;

		// Returns calculated rectangle by anchors and offsets relative to source
		RectF Calculate(const RectF& source);

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

		// Sets position of left bottom corner
		void SetPosition(const Vec2F& position);

		// Returns position of left bottom corner
		Vec2F GetPosition() const;

		// Sets size
		void SetSize(const Vec2F& size);

		// Returns size
		Vec2F GetSize() const;

		SERIALIZABLE(WidgetLayerLayout);

	private:
		WidgetLayer* mWidgetLayer = nullptr; // @EXCLUDE_POINTER_SEARCH

		Vec2F mAnchorMin = Vec2F(0, 0); // @SERIALIZABLE
		Vec2F mAnchorMax = Vec2F(1, 1); // @SERIALIZABLE
		Vec2F mOffsetMin = Vec2F(0, 0); // @SERIALIZABLE
		Vec2F mOffsetMax = Vec2F(0, 0); // @SERIALIZABLE
	};
}

CLASS_BASES_META(o2::WidgetLayerLayout)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::WidgetLayerLayout)
{
	PUBLIC_FIELD(position);
	PUBLIC_FIELD(size);
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
	PRIVATE_FIELD(mWidgetLayer).EXCLUDE_POINTER_SEARCH_ATTRIBUTE();
	PRIVATE_FIELD(mAnchorMin).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mAnchorMax).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mOffsetMin).SERIALIZABLE_ATTRIBUTE();
	PRIVATE_FIELD(mOffsetMax).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::WidgetLayerLayout)
{

	PUBLIC_FUNCTION(RectF, Calculate, const RectF&);
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
	PUBLIC_FUNCTION(void, SetOffsetRight, float);
	PUBLIC_FUNCTION(float, GetOffsetRight);
	PUBLIC_FUNCTION(void, SetOffsetBottom, float);
	PUBLIC_FUNCTION(float, GetOffsetBottom);
	PUBLIC_FUNCTION(void, SetOffsetTop, float);
	PUBLIC_FUNCTION(float, GetOffsetTop);
	PUBLIC_FUNCTION(void, SetPosition, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPosition);
	PUBLIC_FUNCTION(void, SetSize, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetSize);
}
END_META;
