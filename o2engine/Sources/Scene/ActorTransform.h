#pragma once

#include "Utils/Math/Transform.h"

namespace o2
{
	class Actor;

	// ---------------
	// Actor transform
	// ---------------
	class ActorTransform: public Transform
	{
	public:
		Getter<Actor*>  actor;         // Owner actor getter
		Property<Vec2F> worldPosition; // World position property
		Property<RectF> worldRect;     // World rectangle property. Sets the position and size
		Property<float> worldAngle;    // World rotation angle in radians
		Property<Basis> worldBasis;    // World transformation basis
		Property<RectF> worldAABB;     // World axis aligned rectangle

		ActorTransform(const Vec2F& size = Vec2F(), const Vec2F& position = Vec2F(), float angle = 0.0f,
					   const Vec2F& scale = Vec2F(1.0f, 1.0f), const Vec2F& pivot = Vec2F(0.5f, 0.5f));

		// Copy-constructor
		ActorTransform(const ActorTransform& other);

		// Assign operator
		ActorTransform& operator=(const ActorTransform& other);

		// Assign operator
		ActorTransform& operator=(const Transform& other);

		// Returns owner actor
		Actor* GetOwnerActor() const;

		// Sets world position
		void SetWorldPosition(const Vec2F& position);

		// Returns world position
		Vec2F GetWorldPosition() const;

		// Sets world rect
		void SetWorldRect(const RectF& rect);

		// Returns world rect
		RectF GetWorldRect() const;

		// Sets world rotation angle, in radians
		void SetWorldAngle(float rad);

		// Returns world rotation angle in radians
		float GetWorldAngle() const;

		// Sets world basis
		void SetWorldBasis(const Basis& basis);

		// Returns world basis
		Basis GetWorldBasis() const;

		// Sets world axis aligned rectangle transformation
		void SetWorldAxisAlignedRect(const RectF& rect);

		// Returns world axis aligned rectangle transformation
		RectF GetWorldAxisAlignedRect() const;

		// Sets world left top corner position
		void SetWorldLeftTop(const Vec2F& position);

		// Returns world left top corner position
		Vec2F GetWorldLeftTop() const;

		// Sets world right top corner position
		void SetWorldRightTop(const Vec2F& position);

		// Returns world right top corner position
		Vec2F GetWorldRightTop() const;

		// Sets world left down corner position
		void SetWorldLeftBottom(const Vec2F& position);

		// Returns world left down corner position
		Vec2F GetWorldLeftBottom() const;

		// Sets world left right bottom position
		void SetWorldRightBottom(const Vec2F& position);

		// Returns world right bottom corner position
		Vec2F GetWorldRightBottom() const;

		// Sets world center position
		void SetWorldCenter(const Vec2F& position);

		// Returns world center position
		Vec2F GetWorldCenter() const;

		// Set local x axis direction
		void SetRight(const Vec2F& dir);

		// Returns local x axis direction
		Vec2F GetRight() const;

		// Set negative local x axis direction
		void SetLeft(const Vec2F& dir);

		// Returns negative local x axis direction
		Vec2F GetLeft() const;

		// Set local y axis direction
		void SetUp(const Vec2F& dir);

		// Returns local y axis direction
		Vec2F GetUp() const;

		// Set negative local y axis direction
		void SetDown(const Vec2F& dir);

		// Returns negative local y axis direction
		Vec2F GetDown() const;

		// Sets x axis directed to worldPoint
		void LookAt(const Vec2F& worldPoint);

		// Transforms point from world space into local
		Vec2F World2LocalPoint(const Vec2F& worldPoint) const;

		// Transforms point from local space into world
		Vec2F Local2WorldPoint(const Vec2F& localPoint) const;

		// Transforms direction from world space into local
		Vec2F World2LocalDir(const Vec2F& worldDir) const;

		// Transforms direction from local space into world
		Vec2F Local2WorldDir(const Vec2F& localDir) const;

		// Returns true when point inside this
		bool IsPointInside(const Vec2F& point) const;

		SERIALIZABLE(ActorTransform);

	protected:
		Basis  mWorldTransform;          // Result world basis
		Basis  mParentInvertedTransform; // Parent world transform inverted
		Basis  mParentTransform;         // Parent world transform
		bool   mIsParentInvTransformActual; // Is mParentInvertedTransform is actual
		Actor* mOwner;                   // Owner actor

	protected:
		// Sets owner and updates transform
		void SetOwner(Actor* actor);

		// Updates mTransform 
		void UpdateTransform();

		// Check mParentInvertedTransform for actual
		void CheckParentInvTransform();

		// Initializes properties
		void InitializeProperties();

		friend class Actor;
	};
}