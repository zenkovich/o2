#include "stdafx.h"
#include "RectDrawable.h"

namespace o2
{
	IRectDrawable::IRectDrawable(const Vec2F& size /*= Vec2F()*/, const Vec2F& position /*= Vec2F()*/,
								 float angle /*= 0.0f*/, const Vec2F& scale /*= Vec2F(1.0f, 1.0f)*/,
								 const Color4& color /*= Color4::White()*/, const Vec2F& pivot /*= Vec2F()*/):
		Transform(size, position, angle, scale, pivot), mColor(color), mEnabled(true)
	{}

	IRectDrawable::IRectDrawable(const IRectDrawable& other) :
		Transform(other.mSize, other.mPosition, other.mAngle, other.mScale, other.mPivot),
		mColor(other.mColor), mEnabled(other.mEnabled), color(this), transparency(this), enabled(this)
	{}

	IRectDrawable& IRectDrawable::operator=(const IRectDrawable& other)
	{
		Transform::operator=(other);

		mColor = other.mColor;
		mEnabled = other.mEnabled;

		ColorChanged();
		EnableChanged();

		return *this;
	}

	void IRectDrawable::SetColor(const Color4& color)
	{
		mColor = color;
		ColorChanged();
	}

	Color4 IRectDrawable::GetColor() const
	{
		return mColor;
	}

	void IRectDrawable::SetTransparency(float transparency)
	{
		mColor.SetAF(transparency);
		ColorChanged();
	}

	float IRectDrawable::GetTransparency() const
	{
		return mColor.AF();
	}

	void IRectDrawable::SetEnabled(bool enabled)
	{
		mEnabled = enabled;
		EnableChanged();
	}

	bool IRectDrawable::IsEnabled() const
	{
		return mEnabled;
	}

	bool IRectDrawable::IsUnderPoint(const Vec2F& point)
	{
		return mDrawingScissorRect.IsInside(point) && Transform::IsPointInside(point);
	}
}

DECLARE_CLASS(o2::IRectDrawable);
