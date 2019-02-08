#pragma once

namespace o2
{

class Layout : public ISerializable
{
public:
	PROPERTIES(Layout);
	PROPERTY(Vec2F, anchorMin, SetAnchorMin, GetAnchorMin);          // Left bottom anchor property
	PROPERTY(Vec2F, anchorMax, SetAnchorMax, GetAnchorMax);          // Right top anchor property

	PROPERTY(Vec2F, offsetMin, SetOffsetMin, GetOffsetMin);          // Left bottom corner offset property
	PROPERTY(Vec2F, offsetMax, SetOffsetMax, GetOffsetMax);          // Right top corner offset property

	PROPERTY(float, anchorLeft, SetAnchorLeft, GetAnchorLeft);       // Left anchor property
	PROPERTY(float, anchorRight, SetAnchorRight, GetAnchorRight);    // Right anchor property
	PROPERTY(float, anchorBottom, SetAnchorBottom, GetAnchorBottom); // Bottom anchor property
	PROPERTY(float, anchorTop, SetAnchorTop, GetAnchorTop);          // Top anchor property

	PROPERTY(float, offsetLeft, SetOffsetLeft, GetOffsetLeft);       // Left offset property
	PROPERTY(float, offsetRight, SetOffsetRight, GetOffsetRight);    // Right offset property
	PROPERTY(float, offsetBottom, SetOffsetBottom, GetOffsetBottom); // Bottom offset property
	PROPERTY(float, offsetTop, SetOffsetTop, GetOffsetTop);          // Top offset property

public:
	Layout(UIWidgetLayer* widgetLayer);

	RectF Calculate(const RectF& source);

	bool operator==(const Layout& other) const;
	bool operator!=(const Layout& other) const;

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

private:
	UIWidgetLayer * mWidgetLayer;

	Vec2F mAnchorMin; // @SERIALIZABLE
	Vec2F mAnchorMax; // @SERIALIZABLE
	Vec2F mOffsetMin; // @SERIALIZABLE
	Vec2F mOffsetMax; // @SERIALIZABLE

	SERIALIZABLE(Layout);
};
}
