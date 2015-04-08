#pragma once

#include "util/public_namespace.h"
#include "util/math/math.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

struct Layout
{
	Vec2F mLTRelative;
	Vec2F mLTAbsolute;
	Vec2F mRBRelative;
	Vec2F mRBAbsolute;
	Vec2F mPosition;
	Vec2F mSize;
	Vec2F mMinSize;
	Vec2F mMaxSize;

	Layout(const Vec2F& LTRelative = Vec2F(), const Vec2F& LTAbsolute = Vec2F(),
		   const Vec2F& RBRelative = Vec2F(1, 1), const Vec2F& RBAbsolute = Vec2F(),
		   const Vec2F& minSize = Vec2F(), const Vec2F& maxSize = Vec2F(FLT_MAX, FLT_MAX));

	void Update(const Vec2F& relPos, const Vec2F& relSize, bool pixelPerfect = true);

	RectF GetRect() const;

	Layout& FixWidth(float width);
	Layout& FixHeight(float height);
	Layout& FixSize(const Vec2F& size);

	Layout& MinWidth(float width);
	Layout& MinHeight(float height);
	Layout& MinSize(const Vec2F& size);

	Layout& MaxWidth(float width);
	Layout& MaxHeight(float height);
	Layout& MaxSize(const Vec2F& size);

	Layout& AbsPosition(const Vec2F& position);
	Layout& LeftTopAbs(const Vec2F& position);
	Layout& RightDownAbs(const Vec2F& position);

	Layout& LeftAbs(float value);
	Layout& RightAbs(float value);
	Layout& TopAbs(float value);
	Layout& DownAbs(float value);

	Layout& LeftTopRel(const Vec2F& relPosition);
	Layout& rightDownRel(const Vec2F& relPosition);

	float Left() const;
	float Right() const;
	float Top() const;
	float Down() const;

	Vec2F LeftTop() const;
	Vec2F RightTop() const;
	Vec2F LeftBottom() const;
	Vec2F RightBottom() const;

	static Layout Both(const RectF& border = RectF());
	static Layout Relative(const Vec2F& relSize, const Vec2F& relPos = Vec2F());
	static Layout Fixed(const Vec2F& size, const Vec2F& position = Vec2F());
};

//implementation

Layout::Layout(const Vec2F& LTRelative /*= vec2f()*/, const Vec2F& LTAbsolute /*= vec2f()*/,
			   const Vec2F& RBRelative /*= vec2f(1, 1)*/, const Vec2F& RBAbsolute /*= vec2f()*/,
			   const Vec2F& minSize /*= vec2f()*/, const Vec2F& maxSize /*= vec2f(FLT_MAX, FLT_MAX)*/):
			   mLTRelative(LTRelative), mLTAbsolute(LTAbsolute), mRBRelative(RBRelative),
			   mRBAbsolute(RBAbsolute), mMinSize(minSize), mMaxSize(maxSize)
{
	Update(Vec2F(), Vec2F());
}

void Layout::Update(const Vec2F& relPos, const Vec2F& relSize, bool pixelPerfect /*= true*/)
{
	mPosition = relSize.Scale(mLTRelative) + mLTAbsolute + relPos;
	mSize = relSize.Scale(mRBRelative) + mRBAbsolute + relPos - mPosition;
	mSize.x = Clamp(mSize.x, mMinSize.x, mMaxSize.x);
	mSize.y = Clamp(mSize.y, mMinSize.y, mMaxSize.y);

	if (pixelPerfect)
	{
		mPosition.x = ceil(mPosition.x); mPosition.y = ceil(mPosition.y);
		mSize.x = ceil(mSize.x); mSize.y = ceil(mSize.y);
	}
}

RectF Layout::GetRect() const
{
	return RectF(mPosition, mPosition + mSize);
}

Layout& Layout::FixWidth(float width)
{
	mRBRelative.x = mLTRelative.x;
	mRBAbsolute.x = mRBAbsolute.x + width;
	return *this;
}

Layout& Layout::FixHeight(float height)
{
	mRBRelative.y = mLTRelative.y;
	mRBAbsolute.y = mRBAbsolute.y + height;
	return *this;
}

Layout& Layout::FixSize(const Vec2F& size)
{
	return FixHeight(size.y).FixWidth(size.x);
}

Layout& Layout::MinWidth(float width)
{
	mMinSize.x = width;
	return *this;
}

Layout& Layout::MinHeight(float height)
{
	mMinSize.y = height;
	return *this;
}

Layout& Layout::MinSize(const Vec2F& size)
{
	mMinSize = size;
	return *this;
}

Layout& Layout::MaxWidth(float width)
{
	mMaxSize.x = width;
	return *this;
}

Layout& Layout::MaxHeight(float height)
{
	mMaxSize.y = height;
	return *this;
}

Layout& Layout::MaxSize(const Vec2F& size)
{
	mMaxSize = size;
	return *this;
}

Layout& Layout::AbsPosition(const Vec2F& position)
{
	Vec2F dd = position - mLTAbsolute;
	mLTAbsolute += dd;
	mRBAbsolute += dd;
	return *this;
}

Layout& Layout::LeftTopAbs(const Vec2F& position)
{
	mLTAbsolute = position;
	return *this;
}

Layout& Layout::RightDownAbs(const Vec2F& position)
{
	mRBAbsolute = position;
	return *this;
}

Layout& Layout::LeftTopRel(const Vec2F& relPosition)
{
	mLTRelative = relPosition;
	return *this;
}

Layout& Layout::rightDownRel(const Vec2F& relPosition)
{
	mRBRelative = relPosition;
	return *this;
}

Layout& Layout::LeftAbs(float value)
{
	mLTAbsolute.x = value;
	return *this;
}

Layout& Layout::RightAbs(float value)
{
	mRBAbsolute.x = value;
	return *this;
}

Layout& Layout::TopAbs(float value)
{
	mLTAbsolute.y = value;
	return *this;
}

Layout& Layout::DownAbs(float value)
{
	mRBAbsolute.y = value;
	return *this;
}

float Layout::Left() const
{
	return mPosition.x;
}

float Layout::Right() const
{
	return mPosition.x + mSize.x;
}

float Layout::Top() const
{
	return mPosition.y;
}

float Layout::Down() const
{
	return mPosition.y + mSize.y;
}

Vec2F Layout::LeftTop() const
{
	return mPosition;
}

Vec2F Layout::RightTop() const
{
	return Vec2F(mPosition.x + mSize.x, mPosition.y);
}

Vec2F Layout::LeftBottom() const
{
	return Vec2F(mPosition.x, mPosition.y + mSize.y);
}

Vec2F Layout::RightBottom() const
{
	return mPosition + mSize;
}

Layout Layout::Both(const RectF& border /*= fRect()*/)
{
	return Layout(Vec2F(), Vec2F(border.left, border.top), Vec2F(1, 1), Vec2F(-border.right, -border.bottom));
}

Layout Layout::Relative(const Vec2F& relSize, const Vec2F& relPos /*= vec2f()*/)
{
	return Layout(relPos, Vec2F(), relPos + relSize, Vec2F());
}

Layout Layout::Fixed(const Vec2F& size, const Vec2F& position /*= vec2f()*/)
{
	return Layout(Vec2F(), position, Vec2F(0, 0), size + position, size, size);
}

CLOSE_O2_NAMESPACE
