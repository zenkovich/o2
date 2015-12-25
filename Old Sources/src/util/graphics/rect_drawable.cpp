#include "rect_drawable.h"

OPEN_O2_NAMESPACE

IRectDrawable::IRectDrawable(const Vec2F& size /*= vec2f()*/, const Vec2F& position /*= vec2f()*/,
const Color4& color /*= color4::white()*/, const Vec2F& pivot /*= vec2f()*/):
mPivot(pivot), mPosition(position), mSize(size), mColor(color), mEnabled(true)
{
	InitializeProperties();
}

IRectDrawable::IRectDrawable(const IRectDrawable& drawable)
{
	mPosition = drawable.mPosition;
	mPivot = drawable.mPivot;
	mSize = drawable.mSize;
	mColor = drawable.mColor;

	InitializeProperties();
}

void IRectDrawable::SetPosition(const Vec2F& position)
{
	if (Equals(position, mPosition))
		return;

	mPosition = position;
	PositionChanged();
}

Vec2F IRectDrawable::GetPosition() const
{
	return mPosition;
}

void IRectDrawable::SetSize(const Vec2F& size)
{
	if (Equals(size, mSize))
		return;

	mSize= size;
	SizeChanged();
}

Vec2F IRectDrawable::GetSize() const
{
	return mSize;
}

void IRectDrawable::SetPivot(const Vec2F& pivot)
{
	if (Equals(pivot, mPivot))
		return;

	mPivot = pivot;
	PivotChanged();
}

Vec2F IRectDrawable::GetPivot() const
{
	return mPivot;
}

void IRectDrawable::GetRect(const RectF& rect)
{
	Vec2F lt = mPosition - mPivot;
	if (Equals(lt, rect.LeftTop()) && Equals(lt + mSize, rect.RightBottom()))
		return;

	mSize = rect.Size();
	mPosition = rect.LeftTop() + mPivot;

	PositionChanged();
	SizeChanged();
}

RectF IRectDrawable::GetRect() const
{
	Vec2F lt = mPosition - mPivot;
	return RectF(lt, lt + mSize);
}

void IRectDrawable::SetColor(const Color4& color)
{
	if (Equals(color, mColor))
		return;

	mColor = color;
	ColorChanged();
}

Color4 IRectDrawable::GetColor() const
{
	return mColor;
}

void IRectDrawable::SetTransparency(float transparency)
{
	if (Equals(mColor.AF(), transparency))
		return;

	mColor.a = (int)(255.0f*transparency);
	ColorChanged();
}

float IRectDrawable::GetTransparency() const
{
	return mColor.AF();
}

void IRectDrawable::SetRelativePivot(const Vec2F& relPivot)
{
	SetPivot(relPivot.Scale(mSize));
}

Vec2F IRectDrawable::GetRelativePivot() const
{
	return mPivot.InvScale(mSize);
}

void IRectDrawable::SetEnabled(bool enabled)
{
	if (mEnabled == enabled)
		return;

	mEnabled = enabled;
	EnableChanged();
}

bool IRectDrawable::IsEnabled() const
{
	return mEnabled;
}

void IRectDrawable::InitializeProperties()
{
	REG_PROPERTY(IRectDrawable, position, SetPosition, GetPosition);
	REG_PROPERTY(IRectDrawable, size, SetSize, GetSize);
	REG_PROPERTY(IRectDrawable, pivot, SetPivot, GetPivot);
	REG_PROPERTY(IRectDrawable, relPivot, SetRelativePivot, GetRelativePivot);
	REG_PROPERTY(IRectDrawable, rect, GetRect, GetRect);
	REG_PROPERTY_SETTER_NONCONST(IRectDrawable, transparency, SetTransparency, GetTransparency);
	REG_PROPERTY_SETTER_NONCONST(IRectDrawable, enabled, SetEnabled, IsEnabled);
	REG_PROPERTY(IRectDrawable, color, SetColor, GetColor);
}

CLOSE_O2_NAMESPACE