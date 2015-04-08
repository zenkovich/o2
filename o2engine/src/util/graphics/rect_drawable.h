#pragma once

#include "public.h"
#include "util/objects.h"
#include "util/property.h"
#include "util/property_list.h"

OPEN_O2_NAMESPACE

/** Basic rect drawable interface. It could be drawed, determinates by position, size, pivot and color.
 ** pivot is in local space, in pixels. */
class IRectDrawable: public virtual IDrawable, public PropertyList
{
protected:
	Vec2F  mPosition; /** Position. */
	Vec2F  mSize;     /** Size. */
	Vec2F  mPivot;    /** Pivot, in local space, in pixels. */
	Color4 mColor;    /** Color. */
	bool   mEnabled;  /** True, when darwable enabled and needs to draw. */

public:
	PROPERTY(IRectDrawable, Vec2F)  position;     /** Position property. Using set/getPosition. */
	PROPERTY(IRectDrawable, Vec2F)  size;         /** Size property. Using set/getSize. */
	PROPERTY(IRectDrawable, Vec2F)  pivot;        /** Pivot property, in local pixel space. Using set/getPivot. */
	PROPERTY(IRectDrawable, Vec2F)  relPivot;     /** Relative pivot property, in local scale,
													* where (0, 0) - left top corner, (1, 1) - right down.
													* Using set/getRelativePivot. */
	PROPERTY(IRectDrawable, RectF)  rect;         /** Rect property. Sets the position and size. Using set/getRect. */
	PROPERTY(IRectDrawable, float)  transparency; /** Transparency property, changing alpha in color. Using set/getTransparency. */
	PROPERTY(IRectDrawable, Color4) color;        /** Color property. Using set/getProperty. */
	PROPERTY(IRectDrawable, bool)   enabled;      /** Enable property. Using set/isEnabled. */

	/** ctor. */
	IRectDrawable(const Vec2F& size = Vec2F(), const Vec2F& position = Vec2F(), const Color4& color = Color4::White(),
				  const Vec2F& pivot = Vec2F());

	/** copy-ctor. */
	IRectDrawable(const IRectDrawable& drawable);

	/** dtor. */
	virtual ~IRectDrawable() {}

	/** Returns a copy of drawable. */
	virtual IRectDrawable* Clone() const = 0;

	/** Drawing. */
	virtual void Draw() {}

	/** Sets position. */
	virtual void SetPosition(const Vec2F& position);

	/** Returns position. */
	virtual Vec2F GetPosition() const;

	/** Sets size. */
	virtual void SetSize(const Vec2F& size);

	/** Return size. */
	virtual Vec2F GetSize() const;

	/** Sets pivot, in local pixel space. */
	virtual void SetPivot(const Vec2F& pivot);

	/** Return pivot, in local pixel space. */
	virtual Vec2F GetPivot() const;

	/** Sets relative pivot, in local space, where (0, 0) - left top corner, (1, 1) - right down. */
	virtual void SetRelativePivot(const Vec2F& relPivot);

	/** Returns relative pivot, in local space, where (0, 0) - left top corner, (1, 1) - right down. */
	virtual Vec2F GetRelativePivot() const;

	/** Sets rect. */
	virtual void GetRect(const RectF& rect);

	/** Returns rect. */
	virtual RectF GetRect() const;

	/** Sets color. */
	virtual void SetColor(const Color4& color);

	/** Returns color. */
	virtual Color4 GetColor() const;

	/** Sets transparency. Changing color alpha. */
	virtual void SetTransparency(float transparency);

	/** Returns transparency(color alpha). */
	virtual float GetTransparency() const;

	/** Sets enabled. */
	virtual void SetEnabled(bool enabled);

	/** Returns enabled. */
	virtual bool IsEnabled() const;

protected:
	/** Calls when position was changed. */
	virtual void PositionChanged() {}

	/** Calls when size was changed. */
	virtual void SizeChanged() {}

	/** Calls when pivot was changed. */
	virtual void PivotChanged() {}

	/** Calls when color was changed. */
	virtual void ColorChanged() {}

	/** Calls when enabling changed. */
	virtual void EnableChanged() {}

	/** Initializing properties. */
	void InitializeProperties();
};

CLOSE_O2_NAMESPACE