#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

/** Geometry interface. Processing point colliding, AABB, positioning. */
class Geometry
{
public:
	/** Returns true, if point inside. */
	virtual bool IsInside(const Vec2F& point) const = 0;

	/** Returns AABB of that geometry. */
	virtual RectF GetRect() const = 0;

	/** Returns position of that geometry. */
	virtual Vec2F GetPosition() const = 0;

	/** Sets the position. */
	virtual void SetPosition(const Vec2F& pos) = 0;
};

/** Rectangle geometry, aligned by axis. */
class RectGeometry:public Geometry
{
	RectF mRect;

public:
	/** ctor. */
	RectGeometry();

	/** ctor. */
	RectGeometry(const RectF& rect);

	/** ctor. */
	RectGeometry(float left, float top, float right, float down);

	/** ctor. */
	RectGeometry(const Vec2F& minp, const Vec2F& maxp);

	/** Returns true, if point inside. */
	bool IsInside(const Vec2F& point) const;

	/** Returns AABB of that geometry. */
	RectF GetRect() const;

	/** Returns position of that geometry. */
	Vec2F GetPosition() const;

	/** Sets the position. */
	void SetPosition(const Vec2F& pos);

	/** Sets the size of rect. */
	void SetSize(const Vec2F& size);

	/** Sets rectangle. */
	void Set(const RectF& rect);

	/** Sets rectangle by parameters. */
	void Set(const Vec2F& minp, const Vec2F& maxp);
};

/** Circle geometry. Determined by center position and radius. */
class CircleGeometry: public Geometry
{
	Vec2F mCenter;    /**< Center position. */
	float mRadius;    /**< Radius of circle. */
	float mSqrRadius; /**< Square of radius: mRadius*mRadius. */

public:
	/** ctor. */
	CircleGeometry();

	/** ctor. */
	CircleGeometry(const Vec2F& center, float radius);

	/** Returns true, if point inside. */
	bool IsInside(const Vec2F& point) const;

	/** Returns AABB of that geometry. */
	RectF GetRect() const;

	/** Returns position of that geometry. */
	Vec2F GetPosition() const;

	/** Sets the position. */
	void SetPosition(const Vec2F& pos);

	/** Sets by parameters. */
	void Set(const Vec2F& center, float radius);
};

class GroupGeometry:public Geometry
{
	friend class Geometry;

public:
	typedef Array< Geometry* > PartsArr;

protected:
	PartsArr mParts;
	RectF    mAABB;
	Vec2F    mPosition;

public:
	GroupGeometry();
	~GroupGeometry();

	template<typename T>
	T& GetPart(int idx) { return *((T*)mParts[idx]); }

	int GetPartsCount() const;

	int AddPart(Geometry* geom);
	void RemovePart(int idx);
	void RemoveAllParts();

	void SetPosition(const Vec2F& pos);
	Vec2F GetPosition() const;

	bool IsInside(const Vec2F& point) const;

	void UpdateAABB();
};

CLOSE_O2_NAMESPACE
